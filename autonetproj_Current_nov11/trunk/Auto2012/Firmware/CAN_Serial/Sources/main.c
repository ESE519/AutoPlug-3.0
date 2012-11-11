/* TORCS - CAN gateway
 *
 * Receives car params from TORCS and sends them on the CAN bus
 * Receives driver inputs from the CAN bus and sends them to TORCS
 */

#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h> /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12c128"
#include <string.h>
#include "common.h"
#include "CAN.h"
#include "SCI.h"
#include "types.h"
#include "PLL.h"
#include "stdio.h"


CarParams carParams;  //speed | engineRPM | wheel speed (FL,FR,RL,RR) | yaw rate
CarParams carParams2;
Params params;
StabMsg stabmsg;  //lateral speed  |  lateral acceleration
CarDistance carDistance;
CarDistance carDistance2;
volatile TorcsInput torcsInput; //accel | brake (FL,FR,RL,RR) | steer | gear | clutch
volatile TorcsInput torcsInput2;
volatile UINT8 carInputsUpdated = 0;
volatile UINT8 carParamsUpdated = 0;
volatile UINT8 carInputsUpdated2 = 0;
volatile UINT8 carParamsUpdated2 = 0;

UINT8 serialRxBuffer[sizeof(Params) + sizeof(StabMsg)];

void init(void);

#pragma CODE_SEG __NEAR_SEG NON_BANKED

interrupt 20 void SCIRx_vect(void)   //receive messages from serial port
{
    UINT8 status, dummy;
    static UINT8 serialRxState = 0;
    static UINT8 serialData;
    static UINT8 serialDataLength = 0;
    static UINT8 serialRxChksum = 0;
    static UINT8 *rxPtr;

    status = SCISR1;
    
    //printf("SCR\n");

    if(SCISR1_RDRF == 0)  // No data 
    {
        //printf("EMPTY\n");
        return;
    }
    //Check for Errors (Framing, Noise, Parity) 
    if( (status & 0x07) != 0 )
    {
        dummy = SCIDRL;
        //printf("Yes DOOM\n");
        return;
    } 
       //printf("SCR\n");

    // Good Data 
    serialData = SCIDRL; // load SCI register to data 
    SCIDataFlag = 1;

    switch(serialRxState)   //serialdata (AA | CC | ...)
    {
        //printf("No Doom2\n");
        case 0:
            if(serialData == 0xAA)
            {
                serialRxChksum = 0xAA;  
                serialRxState = 1;
            }
            break;

        case 1:
            if(serialData == 0xCC && serialRxState == 1)
            {
                serialDataLength = sizeof(Params);
                serialRxChksum ^= 0xCC;
                rxPtr = serialRxBuffer;
                serialRxState = 2;
            }
            else
            {
                serialRxState = 0;
            }
            break;

        case 2:
            if(serialDataLength > 0)      //copy data to serial buffer
            {
                *rxPtr = serialData;
                serialRxChksum ^= serialData;
                rxPtr++;
                serialDataLength--;
            }
            else
            {
               // printf("Data  \n");
               // printf("Chksum \n");
                if(serialData == serialRxChksum)
                {
                    //printf("Receive Serial\n");
                    if(!carParamsUpdated) // Only update when old value has been used
                    {
                        memcpy(&params, serialRxBuffer, sizeof(Params));
                        //memcpy(&carParams2, serialRxBuffer, sizeof(CarParams));
                        //memcpy(&stabmsg, serialRxBuffer + 8, sizeof(StabMsg));
                        carParamsUpdated = 1;
                        carParamsUpdated2=1;
                    }
                }
                serialRxState = 0;
            }
            break;
    }
}

interrupt 38 void CANRx_vect(void)     //receive messages via CAN bus
{
    UINT16 identifier;
    UINT8 length;

    identifier = (CANRXIDR0 << 3) + (CANRXIDR1 >> 5);

    length = CANRXDLR & 0x0F;

    if(identifier == CAN_INPUT_MSG_ID)     //messages from wiimote
    {
        CarInputs carInputs;

        if(length > sizeof(CarInputs))
            length = sizeof(CarInputs);

        memcpy(&carInputs, &CANRXDSR0, length);
        torcsInput.steer = carInputs.steer;  //update steer information
        carInputsUpdated = 1;
    }
    else if(identifier == CAN_BRAKE_MSG_ID) //messages from ABS ECU (brake message)
    {
        BrakeMsg brakeMsg;

        if(length > sizeof(BrakeMsg))
            length = sizeof(BrakeMsg);

        memcpy(&brakeMsg, &CANRXDSR0, length);
        torcsInput.brakeFL = brakeMsg.brakeFL;  //update brake information
        torcsInput.brakeFR = brakeMsg.brakeFR;
        torcsInput.brakeRL = brakeMsg.brakeRL;
        torcsInput.brakeRR = brakeMsg.brakeRR;
        carInputsUpdated = 1;
    }
    else if(identifier == CAN_ACCEL_MSG_ID) //corrected acceleration messages
    {
        AccelMsg accelMsg;
        if(length > sizeof(AccelMsg))
            length = sizeof(AccelMsg);

        memcpy(&accelMsg, &CANRXDSR0, length);
        torcsInput.accel = accelMsg.accel;    //update acceleration, gear, clutch information
        torcsInput.gear = accelMsg.gear;
        torcsInput.clutch = accelMsg.clutch;
        carInputsUpdated = 1;
    }
    //Receiving the CAN messages of 2nd car 
    else if(identifier == CAN_INPUT_MSG_ID_2)     //messages from wiimote
    {
        CarInputs carInputs2;

        if(length > sizeof(CarInputs))
            length = sizeof(CarInputs);

        memcpy(&carInputs2, &CANRXDSR0, length);
        torcsInput.steer2 = carInputs2.steer;  //update steer information
        carInputsUpdated2 = 1;
    }
    else if(identifier == CAN_BRAKE_MSG_ID_2) //messages from ABS ECU (brake message)
    {
        BrakeMsg brakeMsg2;

        if(length > sizeof(BrakeMsg))
            length = sizeof(BrakeMsg);

        memcpy(&brakeMsg2, &CANRXDSR0, length);
        torcsInput.brakeFL2 = brakeMsg2.brakeFL;  //update brake information
        torcsInput.brakeFR2 = brakeMsg2.brakeFR;
        torcsInput.brakeRL2 = brakeMsg2.brakeRL;
        torcsInput.brakeRR2 = brakeMsg2.brakeRR; 
        carInputsUpdated2 = 1;
    }
    else if(identifier == CAN_ACCEL_MSG_ID_2) //corrected acceleration messages
    {
        AccelMsg accelMsg2;
        if(length > sizeof(AccelMsg))
            length = sizeof(AccelMsg);

        memcpy(&accelMsg2, &CANRXDSR0, length);
        torcsInput.accel2 = accelMsg2.accel;    //update acceleration, gear, clutch information
        torcsInput.gear2 = accelMsg2.gear;
        torcsInput.clutch2 = accelMsg2.clutch;
        carInputsUpdated2 = 1; 
       // printf("CAN ACCEL 2 RECEIVE\n");
    }
    CANRFLG_RXF = 1; // Reset the flag
}

#pragma CODE_SEG DEFAULT


void main(void)
{
    UINT16 i;

    init();

    EnableInterrupts;

    //TERMIO_Init();
    
    for(;;)
    {
        //printf("Foo\n");
        if(carParamsUpdated)   //send simulator's data on CAN bus
        {
            carParams.speed=params.speed;
            carParams.engineRPM=params.engineRPM ;
            carParams.wheelSpeedFL=params.wheelSpeedFL;
            carParams.wheelSpeedFR=params.wheelSpeedFR;
            carParams.wheelSpeedRL=params.wheelSpeedRL;
            carParams.wheelSpeedRR=params.wheelSpeedRR;
            carParams.yawRate=params.yawRate;
            carParams.distance=params.distance;
            CANTx(CAN_PARAM_MSG_ID, &carParams, sizeof(CarParams));
            //CANTx(CAN_STAB_MSG_ID, &stabmsg, sizeof(StabMsg));
            carDistance.distance=carParams.distance;
            CANTx(CAN_DISTANCE_MSG_ID, &carDistance, sizeof(CarDistance));
            carParamsUpdated = 0;
        }

        if((carInputsUpdated == 1) || (carInputsUpdated2 == 1))   //send wiimote's data to simulator via serial port
        {
            TorcsInput temp;
            memcpy(&temp, &torcsInput, sizeof(TorcsInput));
            
            //printf("Sending SERIAL\n");
            SCITxPkt(&temp, sizeof(TorcsInput));
            //printf("\n Torcs Input : %x \n " , temp);
            carInputsUpdated = 0;
        }
        //2nd car
        if(carParamsUpdated2)   //send simulator's data on CAN bus
        {
            carParams2.speed=params.speed2;
            carParams2.engineRPM=params.engineRPM2;
            carParams2.wheelSpeedFL=params.wheelSpeedFL2;
            carParams2.wheelSpeedFR=params.wheelSpeedFR2;
            carParams2.wheelSpeedRL=params.wheelSpeedRL2;
            carParams2.wheelSpeedRR=params.wheelSpeedRR2;
            carParams2.yawRate=params.yawRate2;
            carParams2.distance=params.distance2;
            CANTx(CAN_PARAM_MSG_ID_2, &carParams2, sizeof(CarParams));
            //CANTx(CAN_STAB_MSG_ID, &stabmsg, sizeof(StabMsg));
            carDistance2.distance = carParams2.distance;
            CANTx(CAN_DISTANCE_MSG_ID_2, &carDistance2, sizeof(CarDistance));
            
            carParamsUpdated2 = 0;
        }
        
        /*if(carInputsUpdated2)   //send wiimote's data to simulator via serial port
        {
            TorcsInput temp2;
            memcpy(&temp2, &torcsInput2, sizeof(TorcsInput));
            SCITxPkt(&temp2, sizeof(TorcsInput));
            carInputsUpdated2 = 0;
        }*/
    }
}

void init()
{
    setclk24();    // set Eclock to 24 MHZ
    SCIInit();
    CANInit();
}
