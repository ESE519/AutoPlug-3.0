#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h> /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12c128"
#include <string.h>

#include "common.h"
#include "CAN.h"
#include "SCI.h"
#include "types.h"
#include "PLL.h"
#include "termio.h"
#include "stdio.h"

#define KP_NUM (5)
#define KP_DEN (1)
#define KI_NUM (1)
#define KI_DEN (20)

#define KP_BRAKE_NUM (5)
#define KP_BRAKE_DEN (1)
#define KI_BRAKE_NUM (1)
#define KI_BRAKE_DEN (20)

typedef struct _Car
{
    float gearRatio[8];
    float enginerpmRedLine; /* rad/s */
    float SHIFT; /* = 0.95 (% of rpmredline) */
    UINT16 SHIFT_MARGIN;
    float wheelRadius[4];
} Car;

CarInputs carInputs;
CarParams carParams;
CarInputs carInputs2;
CarParams carParams2;
CarDistance carDistance;
CarDistance carDistance2;

volatile UINT8 carInputsUpdated = 0;
volatile UINT8 carInputsUpdated2 = 0;
volatile UINT8 carParamsUpdated = 0;
volatile UINT8 carParamsUpdated2 = 0;
volatile UINT8 accelCorrection =  0;
volatile UINT8 accelCorrection1 = 50;
volatile UINT8 accelCorrection2 =  0;
const Car car = {
    {0, 3.9*4.5, 2.9*4.5, 2.3*4.5, 1.87*4.5, 1.68*4.5, 1.54*4.5, 1.46*4.5},
    1958.26,
    0.95,
    4,
    {0.3024, 0.3024, 0.3151, 0.3151}
};


void init(void);
INT8 getGear(INT8 currentGear);
INT8 getGear2(INT8 currentGear);

void main(void)
{
    INT8 setSpeed = -1;
    INT8 setSpeed1 = -1;
    INT8 carSpeed = 0;
    
    INT8 setSpeed2 = -1;
    INT8 setSpeed21 = -1;
    INT8 carSpeed2 = 0;

    INT16 error;
    INT16 errInteg;
    INT16 controlOutput;
    INT16 controlOutput1;
    
    INT16 error2;
    INT16 errInteg2;
    INT16 controlOutput2;
    INT16 controlOutput21;

    INT8 accel;
    INT8 brake;
    INT8 gear = 0;
    
    INT8 accel2;
    INT8 brake2;
    INT8 gear2 = 0;
    
    INT8 des_distance = 7;
    INT8 timegap = 0;
    INT8 des_speed = 0;
    INT8 des_distance2 = 7;
    INT8 timegap2 = 0;
    INT8 des_speed2 = 0;
    INT8 flag=0;
    INT8 flag2=0;

    UINT8 cruiseOn = 0;
    UINT8 cruiseOn2 = 0;

    AccelMsg accelMsg;
    BrakeMsg brake_msg;
    AccelMsg accelMsg2;
    BrakeMsg brake_msg2;

    init();

    DDRB = 0xF0;
    PORTB = 0xF0;

    EnableInterrupts;

    for(;;)
    {
        if(carParamsUpdated)
        {
            if(!cruiseOn)
            {
                if(carInputs.gear != -1)
                    gear = (INT8) limit(getGear(gear), 0, 7);
                else
                    gear = -1;

                accelMsg.accel = (UINT8) limit(carInputs.accel - accelCorrection, 0, 100);
                accelMsg.clutch = carInputs.clutch;
                accelMsg.gear = gear;
            }
            else
            {
                error = (setSpeed - carParams.speed);
                errInteg += error;
                errInteg = limit(errInteg, -100*KI_DEN/KI_NUM, 100*KI_DEN/KI_NUM);

                controlOutput = KP_NUM*error/KP_DEN + KI_NUM*errInteg/KI_DEN;
                controlOutput = limit(controlOutput, 0, 100);

                accel = (UINT8) controlOutput;

                gear = (UINT8) limit(getGear(gear), 0, 7);

                accelMsg.accel = (UINT8)limit(accel - accelCorrection, 0, 100) ;
                accelMsg.gear = gear;
                accelMsg.clutch = 0;
                
                controlOutput1 = KP_BRAKE_NUM*error/KP_BRAKE_DEN + KP_BRAKE_NUM*errInteg/KP_BRAKE_DEN;
                controlOutput1 = limit(controlOutput1, 0, 100);

                brake = (UINT8) controlOutput1; 
                //For braking in ACC
                brake_msg.brakeFL = (UINT8)limit(brake - accelCorrection, 0, 100) ;
                brake_msg.brakeFR = (UINT8)limit(brake - accelCorrection, 0, 100) ;
                brake_msg.brakeRL = (UINT8)limit(brake - accelCorrection, 0, 100) ;
                brake_msg.brakeRR = (UINT8)limit(brake - accelCorrection, 0, 100) ; 
                
                if(( carParams.speed > setSpeed)&&(flag==1))           
                 {
                    CANTx(CAN_BRAKE_MSG_ID,&brake_msg,sizeof(BrakeMsg));
                 }  
                 
            }
            
            CANTx(CAN_ACCEL_MSG_ID, &accelMsg, sizeof(AccelMsg));
            carParamsUpdated = 0;
        }
         if(carInputsUpdated)   // removed else
        {
            if((carInputs.controls & CRUISE) && (carParams.speed > 0))
            {
                if(!cruiseOn)
                {
                    setSpeed = carParams.speed;
                    setSpeed1= carParams.speed;
                    cruiseOn = 1;
                    errInteg = 0;
                    PORTB = 0x70;
                    flag=1;
                }
                if(abs(carDistance.distance) < des_distance)             //When distance between the two cars goes below the 
                {                                                   //desired limit, ACC kicks in by calculating the new speed
                    timegap=(des_distance)/carParams.speed;         //from the desired timegap inorder to maintain clearance
                    des_speed=((abs(carDistance.distance)))/timegap;
                    //printf("des distance = %d",des_speed);
                    setSpeed = des_speed;
                    flag=1;
                } 
                else                                                           //if car in front moves off go back to the speed when
                {                                                              //cruise control was pressed
                    setSpeed = setSpeed1;
                    flag=0;                   
                } 
                
            }
            else
            {
                cruiseOn = 0;
                setSpeed = -1;
                PORTB = 0xF0;
            }
            if(!cruiseOn)
            {
                if(carInputs.gear != -1)
                    gear = (UINT8)limit(getGear(gear), 0, 7);
                else
                    gear = -1;

                accelMsg.accel = (UINT8)limit(carInputs.accel - accelCorrection, 0, 100);
                accelMsg.clutch = carInputs.clutch;
                accelMsg.gear = gear;
            }
            carInputsUpdated = 0;
        }
        
        //for 2nd car
        if(carParamsUpdated2)
        {
            if(!cruiseOn2)
            {
                if(carInputs2.gear != -1)
                    gear2 = (INT8) limit(getGear2(gear), 0, 7);
                else
                    gear2 = -1;

                accelMsg2.accel = (UINT8) limit(carInputs2.accel - accelCorrection2, 0, 100);
                accelMsg2.clutch = carInputs2.clutch;                              
                accelMsg2.gear = gear2;
            }
            else
            {
                error2 = (setSpeed2 - carParams2.speed);
                errInteg2 += error2;
                errInteg2 = limit(errInteg2, -100*KI_DEN/KI_NUM, 100*KI_DEN/KI_NUM);

                controlOutput2 = KP_NUM*error2/KP_DEN + KI_NUM*errInteg2/KI_DEN;
                controlOutput2 = limit(controlOutput2, 0, 100);

                accel2 = (UINT8) controlOutput2;

                gear2 = (UINT8) limit(getGear2(gear2), 0, 7);

                accelMsg2.accel = (UINT8)limit(accel2 - accelCorrection2, 0, 100) ;
                accelMsg2.gear = gear2;
                accelMsg2.clutch = 0;
                
                controlOutput21 = KP_BRAKE_NUM*error2/KP_BRAKE_DEN + KP_BRAKE_NUM*errInteg2/KP_BRAKE_DEN;
                controlOutput21 = limit(controlOutput21, 0, 100);

                brake2 = (UINT8) controlOutput21; 
                //For braking in ACC
                brake_msg2.brakeFL = (UINT8)limit(brake2 - accelCorrection2, 0, 100) ;
                brake_msg2.brakeFR = (UINT8)limit(brake2 - accelCorrection2, 0, 100) ;
                brake_msg2.brakeRL = (UINT8)limit(brake2 - accelCorrection2, 0, 100) ;
                brake_msg2.brakeRR = (UINT8)limit(brake2 - accelCorrection2, 0, 100) ; 
                
                if(( carParams2.speed > setSpeed2)&&(flag2==1))           
                 {
                    CANTx(CAN_BRAKE_MSG_ID_2,&brake_msg,sizeof(BrakeMsg));
                 }  
                 
            }
            
            CANTx(CAN_ACCEL_MSG_ID_2, &accelMsg2, sizeof(AccelMsg));
            carParamsUpdated2 = 0;
        }
         if(carInputsUpdated2)      // removed else
        {
            if((carInputs2.controls & CRUISE) && (carParams2.speed > 0))
            {
                if(!cruiseOn2)
                {
                    setSpeed2 = carParams2.speed;
                    setSpeed21= carParams2.speed;
                    cruiseOn2 = 1;
                    errInteg2 = 0;
                    PORTB = 0x70;
                    flag=1;
                }
                if(abs(carDistance2.distance) < des_distance)             //When distance between the two cars goes below the 
                {                                                   //desired limit, ACC kicks in by calculating the new speed
                    timegap2=(des_distance)/carParams2.speed;         //from the desired timegap inorder to maintain clearance
                    des_speed2=((abs(carDistance2.distance)))/timegap2;
                    //printf("des distance = %d",des_speed);
                    setSpeed2 = des_speed2;
                    flag=1;
                } 
                else                                                           //if car in front moves off go back to the speed when
                {                                                              //cruise control was pressed
                    setSpeed2 = setSpeed21;
                    flag2=0;                   
                } 
                
            }
            else
            {
                cruiseOn2 = 0;
                setSpeed2 = -1;
                PORTB = 0xF0;
            }
            if(!cruiseOn2)
            {
                if(carInputs2.gear != -1)
                    gear2 = (UINT8)limit(getGear2(gear), 0, 7);
                else
                    gear2 = -1;

                accelMsg2.accel = (UINT8)limit(carInputs2.accel - accelCorrection2, 0, 100);
                accelMsg2.clutch = carInputs2.clutch;
                accelMsg2.gear = gear2;
            }
            carInputsUpdated2 = 0;   
        }
    }
}

/* Compute gear */
INT8 getGear(INT8 currentGear)
{
    float gr_up, omega, wr;
    float rearWheelSpeed;

    if (currentGear <= 0)
        return 1;

    //rearWheelSpeed = (carParams.wheelSpeedRL + carParams.wheelSpeedRR)/2;
    rearWheelSpeed = carParams.speed;

    gr_up = car.gearRatio[currentGear];
    omega = car.enginerpmRedLine/gr_up;
    wr = car.wheelRadius[2];

    if (rearWheelSpeed > omega*wr*car.SHIFT)
    {
        return currentGear + 1;
    }
    else
    {
        float gr_down = car.gearRatio[currentGear - 1];
        omega = car.enginerpmRedLine/gr_down;
        if ( (currentGear > 1) && (rearWheelSpeed + car.SHIFT_MARGIN < omega*wr*car.SHIFT) )
            return currentGear - 1;
    }
    return currentGear;
}

INT8 getGear2(INT8 currentGear)
{
    float gr_up, omega, wr;
    float rearWheelSpeed;

    if (currentGear <= 0)
        return 1;

    //rearWheelSpeed = (carParams.wheelSpeedRL + carParams.wheelSpeedRR)/2;
    rearWheelSpeed = carParams2.speed;

    gr_up = car.gearRatio[currentGear];
    omega = car.enginerpmRedLine/gr_up;
    wr = car.wheelRadius[2];

    if (rearWheelSpeed > omega*wr*car.SHIFT)
    {
        return currentGear + 1;
    }
    else
    {
        float gr_down = car.gearRatio[currentGear - 1];
        omega = car.enginerpmRedLine/gr_down;
        if ( (currentGear > 1) && (rearWheelSpeed + car.SHIFT_MARGIN < omega*wr*car.SHIFT) )
            return currentGear - 1;
    }
    return currentGear;
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED

interrupt 38 void CANRx_vect(void)
{
    UINT16 identifier;
    UINT8 length;

    identifier = (CANRXIDR0 << 3) + (CANRXIDR1 >> 5);

    length = CANRXDLR & 0x0F;

    if(identifier == CAN_INPUT_MSG_ID)
    {
        if(!carInputsUpdated) // Only update when old value has been used
        {
            if(length > sizeof(CarInputs))
                length = sizeof(CarInputs);

            memcpy(&carInputs, &CANRXDSR0, length);
            carInputsUpdated = 1;
        }
    }
    else if (identifier == CAN_PARAM_MSG_ID)
    {
        if(!carParamsUpdated) // Only update when old value has been used
        {
            if(length > sizeof(CarParams))
                length = sizeof(CarParams);

            memcpy(&carParams, &CANRXDSR0, length);
            carParamsUpdated = 1;
        }
    }
    else if (identifier == CAN_ACCEL_CORR_MSG_ID)
    {
        accelCorrection = CANRXDSR0;
    }
    else if (identifier == CAN_DISTANCE_MSG_ID)
    {
        if(length > sizeof(CarDistance))
                length = sizeof(CarDistance);

            memcpy(&carDistance, &CANRXDSR0, length);
          
    }
    if(identifier == CAN_INPUT_MSG_ID_2)
    {
        if(!carInputsUpdated2) // Only update when old value has been used
        {
            if(length > sizeof(CarInputs))
                length = sizeof(CarInputs);

            memcpy(&carInputs2, &CANRXDSR0, length);
            carInputsUpdated2 = 1;
        }
    }
    else if (identifier == CAN_PARAM_MSG_ID_2)
    {
        if(!carParamsUpdated2) // Only update when old value has been used
        {
            if(length > sizeof(CarParams))
                length = sizeof(CarParams);

            memcpy(&carParams2, &CANRXDSR0, length);
            carParamsUpdated2 = 1;
        }
    }
    else if (identifier == CAN_ACCEL_CORR_MSG_ID_2)
    {
        accelCorrection2 = CANRXDSR0;
    }
    else if (identifier == CAN_DISTANCE_MSG_ID_2)
    {
        if(length > sizeof(CarDistance))
        length = sizeof(CarDistance);
        
        memcpy(&carDistance2, &CANRXDSR0, length);
        //printf("\n Distance is %d \r\n ", carDistance.distance * 8);
    }


    CANRFLG_RXF = 1; // Reset the flag
}

#pragma CODE_SEG DEFAULT


void init()
{
    setclk24();
    CANInit();

}
