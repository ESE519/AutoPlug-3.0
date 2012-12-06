/* MATLAB - CAN gateway
 *
 * Receives driver inputs from MATLAB and sends them on the CAN bus
 * Receives car parameters from the CAN bus and sends them to MATLAB
 */

#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h>     /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12c128"
#include <string.h>
#include "common.h"
#include "CAN.h"
#include "SCI.h"
#include "types.h"
#include "PLL.h"

CarInputs carInputs;
//CarParams carParams;
UINT8 serialRxBuffer[sizeof(CarInputs)];
volatile UINT8 carInputsUpdated = 0;
volatile UINT8 carParamsUpdated = 0;
volatile UINT8 brakeParamsUpdated = 0;

//observer vars

static float y1;
static float y2;
static float y4;

static float u1;
static float u2;
static float u3;


static double x1;
static double x2;
static double x3;
static double x4;

static double x1_prev;
static double x2_prev;
static double x3_prev;
static double x4_prev;

static double x1_e;
static double x2_e;
static double x3_e;
static double x4_e;

static double y1_e;
volatile double y2_e;
static double y3_e;
static double y4_e;







//BrakeMsg Brakeparams;
  
typedef struct _Allparams{
        CarParams carParams;
        BrakeMsg Brakeparams;
        
}Allparams;
        
 volatile Allparams params; 


void init(void);

#pragma CODE_SEG __NEAR_SEG NON_BANKED

interrupt 20 void SCIRx_vect(void)
{
    UINT8 status, dummy;
    static UINT8 serialRxState = 0;
    UINT8 serialData;
    static UINT8 serialDataLength = 0;
    static UINT8 serialRxChksum = 0;
    static UINT8 *rxPtr;

    status = SCISR1;

    if(SCISR1_RDRF == 0)  /* No data */
        return;

    /* Check for Errors (Framing, Noise, Parity) */
    if( (status & 0x07) != 0 )
    {
        dummy = SCIDRL;
        return;
    }

    /* Good Data */
    serialData = SCIDRL; /* load SCI register to data */
    SCIDataFlag = 1;

    switch(serialRxState)
    {
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
                serialDataLength = sizeof(CarInputs);
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
            if(serialDataLength > 0)
            {
                *rxPtr = serialData;
                serialRxChksum ^= serialData;
                rxPtr++;
                serialDataLength--;
            }
            else
            {
                if(serialData == serialRxChksum)
                {
                    if(!carInputsUpdated) // Only update when old value has been used
                    {
                        memcpy(&carInputs, serialRxBuffer, sizeof(CarInputs));
                        carInputsUpdated = 1;
                    }
                }
                serialRxState = 0;
            }
            break;
    }
}

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
    else if(identifier == CAN_PARAM_MSG_ID)
    {
        if(!carParamsUpdated) // Only update when old values have been used
        {
            if(length > sizeof(CarParams))
                length = sizeof(CarParams);

            memcpy(&(params.carParams), &CANRXDSR0, length);
            carParamsUpdated = 1;
        }
    }
    
    /*else if(identifier == CAN_BRAKE_MSG_ID)
    {
        //if(!brakeParamsUpdated) // Only update when old values have been used
        {
            if(length > sizeof(BrakeMsg))
                length = sizeof(BrakeMsg);

            memcpy(&(params.Brakeparams), &CANRXDSR0, length);
            brakeParamsUpdated = 1;
        }
    } */
    CANRFLG_RXF = 1; // Reset the flag
}

#pragma CODE_SEG DEFAULT


void main(void)
{
    UINT16 i;
    
    
    init();
     setclk24();    // set Eclock to 24 MHZ

    EnableInterrupts;

    for(;;)
    {     
        if(carInputsUpdated)
        {
            CANTx(CAN_INPUT_MSG_ID, &carInputs, sizeof(CarInputs));
            carInputsUpdated = 0;
        }

       if(carParamsUpdated)
        {       //get inputs and outputs
                              u1=((INT8) (params.carParams.wheelSpeedFL))*(180/3.142f)/50;
                              u2=(INT8) (params.carParams.wheelSpeedFR);
                              u3=(INT8) (params.carParams.wheelSpeedRL);
                              
                              y1= ((INT8) ((params.carParams.engineRPM & 0xFF00)>>8))/50; //lat vel      
                              y2= ((INT8) (params.carParams.yawRate))*(180/3.142f)/20; //yaw rt        
                              y4= ((INT8) (params.carParams.engineRPM & 0xFF))/100; //roll angle           
                         
        
            
            //the observer for the lateral velocity sensor
            
       
           
           x1=0.9871f*x1_prev + 0.007472f*x2_prev - 0.0431f*x4_prev -0.01405f*u1 -0.01687f*y1;
           
                     x2=-0.1929f*x1_prev + 0.9527f*x2_prev -0.08966f*x4_prev + 0.06814f*u1 + 0.002547f*u2 - 0.002505f*u3 + 0.1158f*y1;
           
          // x2=0.9527*x2_prev + 
           
           x3= 0.004697f*x1_prev - 0.0008303f*x2_prev + 0.7131f*x3_prev -0.07101f*x4_prev + 0.0008849f*u1 -0.0001495f*y1;
           
           x4= -0.00002747f*x1_prev - 0.5611f*x3_prev + 0.8176f*x4_prev - 0.0002529f*y1;
           
           
           x1_e= 0.998f*x1 - 0.01796f*y1;
           
           x2_e= 0.01278f*x1 +x2 +0.1177f*y1;
           
           x3_e= 0.000001781f*x1 + x3 + 0.0000164f*y1;
           
           x4_e= -0.00003237f*x1 + x4 -0.0002981f*y1;
           
           x1_prev=x1;
           x2_prev=x2;
           x3_prev=x3;
           x4_prev=x4;
           
           y1_e= (-0.1086f*x1_e)*50;
           y2_e= (1.2310f*x2_e)*1;
           y3_e= (-0.0411f*x3_e)*100;
           y4_e= (0.5834f*x4_e)*100;
                        
           
           
            
                    // send estimates  
               
            params.Brakeparams.brakeFL= y1_e;//((params.carParams.engineRPM & 0xFF00)>>8); //test y1
            params.Brakeparams.brakeFR= y2_e;//(params.carParams.yawRate); //test y2
            params.Brakeparams.brakeRL=carParamsUpdated;   //when does this happen??            
            params.Brakeparams.brakeRR=y4_e;//(params.carParams.engineRPM & 0xFF); //test y4       
        
        
        
            SCITxPkt(&params, sizeof(Allparams));
            carParamsUpdated = 0;
            brakeParamsUpdated = 0;
            //for(i = 0; i < 1000; i++);
        }
    }
}

void init()
{
    setclk24();    // set Eclock to 24 MHZ
    SCIInit();
    CANInit();
}
