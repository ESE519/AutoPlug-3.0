#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h>     /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12c128"
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "CAN.h"
#include "types.h"
#include "PLL.h"

#define ABS_KP  20
#define TRAC_KP 20
#define STAB_KP 15

void init(void);
void Abs(void);
void Traction(void);
void stabilityControl(void);

CarParams car;
AccelMsg accel_corr;
volatile UINT8 carParamsUpdated = 0;
volatile UINT8 carInputsUpdated = 0;
BrakeMsg brake_msg = {0,0,0,0};
CarInputs driver_input = {0,0,0,0,0,0};
UINT8 buffer[9]; // recives the CAN DATA
const INT16 abs_threshold = 2;
const INT16 tc_threshold = 2;
const INT16 yawRate_threshold = 10;


volatile float y1;
volatile float y2;
volatile float y3;
volatile float y4;
volatile float y4_prev;


/* ABS */

void main(void)
{
    EnableInterrupts;

    init();

    for(;;)
    {
        if(carParamsUpdated)
        {
            accel_corr.accel = 0;       // no change in accl.

            brake_msg.brakeFL = driver_input.brake;
            brake_msg.brakeFR = driver_input.brake;
            brake_msg.brakeRL = driver_input.brake;
            brake_msg.brakeRR = driver_input.brake;

           // comment out all old stuff
            if((driver_input.controls & STABILITY) && (abs(car.yawRate) > yawRate_threshold))
            {
                stabilityControl();
                Abs();
            }

            if((driver_input.brake > 0) && (driver_input.controls & ABS))
                Abs();

            if((driver_input.accel > 0) && (driver_input.controls & TRACTION))
                Traction();

            
            
            
            // u=-Kx=-[-1.5335 2.3662 7.2193 -3.7494;1.5081 -2.3269 -7.0995 3.6872]x;
            
            //roll rate=roll_angle(t)-roll_angle(t-1)
            
            
            /*y1= ((INT8)((car.engineRPM & 0xFF00)>>8))/50;      //lat vel
            y2= ((INT8)(car.yawRate))*(180/3.142)/20; // yaw rate
            y4= ((INT8)(car.engineRPM & 0xFF))/100; //roll angle (k)
            y3= y4-y4_prev;              // rollr rate (difference)
            y4_prev=y4;
            */
            //y1=-0.10859*x1; y2=   1.231*x2; y3= -0.041089*x3; y4= 0.5834*x4
            
            //lqr
                               
           /* brake_msg.brakeFL= limit(1.5335*(-y1/0.10859) -2.3662*(y2/1.231) -7.2193*(-y3/0.041089) +3.7494*(y4/0.5834),0,100);
            brake_msg.brakeRL=brake_msg.brakeFL;
            
            brake_msg.brakeFR= limit(-1.5081*(-y1/0.10859) +2.3269*(y2/1.231) +7.0995*(-y3/0.041089) -3.6872*(y4/0.5834),0,100);
            brake_msg.brakeRR=brake_msg.brakeFR;*/
            
            CANTx(CAN_ACCEL_CORR_MSG_ID,&accel_corr,sizeof(AccelMsg));
            CANTx(CAN_BRAKE_MSG_ID,&brake_msg,sizeof(BrakeMsg));
            carParamsUpdated = 0;
        }
        carInputsUpdated = 0;
    }
}

void init()
{
    setclk24();
    CANInit();
}

void Abs()
{
    INT16 avg_ws;
    //avg_ws = (car.wheelSpeedFL + car.wheelSpeedFR+ car.wheelSpeedRL + car.wheelSpeedRR)/4;
    avg_ws = car.speed;

    if(avg_ws-car.wheelSpeedFL > abs_threshold)
        brake_msg.brakeFL = (UINT8) limit((INT16)brake_msg.brakeFL + (car.wheelSpeedFL - avg_ws) * ABS_KP,0,100);

    if(avg_ws-car.wheelSpeedFR > abs_threshold)
        brake_msg.brakeFR = (UINT8) limit((INT16)brake_msg.brakeFR + (car.wheelSpeedFR - avg_ws) * ABS_KP,0,100);

    if(avg_ws-car.wheelSpeedRL> abs_threshold)
        brake_msg.brakeRL = (UINT8) limit((INT16)brake_msg.brakeRL + (car.wheelSpeedRL - avg_ws) * ABS_KP,0,100);

    if(avg_ws-car.wheelSpeedRR > abs_threshold)
        brake_msg.brakeRR = (UINT8) limit((INT16)brake_msg.brakeRR + (car.wheelSpeedRR - avg_ws) * ABS_KP,0,100);
}

void Traction()
{
    INT16 avg_rws;
    INT16 correction = 0;

    avg_rws = (car.wheelSpeedRL + car.wheelSpeedRR)/2;
    if((avg_rws - car.speed) > tc_threshold)
    {
        correction = (avg_rws - car.speed) * TRAC_KP;
    }
    accel_corr.accel = (UINT8) limit(correction,0,100);
}

void stabilityControl()
{
    INT16 correction;
    correction = limit((abs(car.yawRate) - yawRate_threshold)*STAB_KP, 0, 100);

    if(car.yawRate > 0)
        brake_msg.brakeRR = (UINT8) limit(brake_msg.brakeRR + correction, 0, 100);
    else
        brake_msg.brakeRL = (UINT8) limit(brake_msg.brakeRL + correction, 0, 100);
    
    accel_corr.accel = (UINT8) limit(correction,0,100);
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

            memcpy(&driver_input, &CANRXDSR0, length);

            carInputsUpdated = 1;
        }
    }
    else if (identifier == CAN_PARAM_MSG_ID)
    {
        if(length > sizeof(CarParams))
            length = sizeof(CarParams);

        memcpy(&car, &CANRXDSR0, length);

        carParamsUpdated = 1;
    }

    CANRFLG_RXF = 1; // Reset the flag
}

#pragma CODE_SEG DEFAULT
