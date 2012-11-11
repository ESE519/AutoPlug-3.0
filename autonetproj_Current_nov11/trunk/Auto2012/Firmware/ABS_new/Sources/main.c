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

//for car2
void Abs2(void);
void Traction2(void);
void stabilityControl2(void);

CarParams car;
CarParams car2;
AccelMsg accel_corr;
AccelMsg accel_corr2;
volatile UINT8 carParamsUpdated = 0;
volatile UINT8 carParamsUpdated2 = 0;
volatile UINT8 carInputsUpdated = 0;
volatile UINT8 carInputsUpdated2 = 0;
BrakeMsg brake_msg = {0,0,0,0};
BrakeMsg brake_msg2 = {0,0,0,0};
CarInputs driver_input = {0,0,0,0,0,0};
CarInputs driver_input2 = {0,0,0,0,0,0};
UINT8 buffer[9]; // recives the CAN DATA
const INT16 abs_threshold = 2;
const INT16 tc_threshold = 2;
const INT16 yawRate_threshold = 10;



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
 
            
            CANTx(CAN_ACCEL_CORR_MSG_ID,&accel_corr,sizeof(AccelMsg));
            CANTx(CAN_BRAKE_MSG_ID,&brake_msg,sizeof(BrakeMsg));
            carParamsUpdated = 0;
        }
        if(carParamsUpdated2)
        {
            accel_corr2.accel = 0;       // no change in accl.

            brake_msg2.brakeFL = driver_input2.brake;
            brake_msg2.brakeFR = driver_input2.brake;
            brake_msg2.brakeRL = driver_input2.brake;
            brake_msg2.brakeRR = driver_input2.brake;

           // comment out all old stuff
            if((driver_input2.controls & STABILITY) && (abs(car2.yawRate) > yawRate_threshold))
            {
                stabilityControl2();
                Abs2();
            }

            if((driver_input2.brake > 0) && (driver_input2.controls & ABS))
                Abs2();

            if((driver_input2.accel > 0) && (driver_input2.controls & TRACTION))
                Traction2();
 
            CANTx(CAN_ACCEL_CORR_MSG_ID_2,&accel_corr2,sizeof(AccelMsg));
            CANTx(CAN_BRAKE_MSG_ID_2,&brake_msg2,sizeof(BrakeMsg));
            carParamsUpdated2 = 0;
        }
        carInputsUpdated = 0;
         carInputsUpdated2 = 0;
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

void Abs2()
{
    INT16 avg_ws2;
    //avg_ws = (car.wheelSpeedFL + car.wheelSpeedFR+ car.wheelSpeedRL + car.wheelSpeedRR)/4;
    avg_ws2 = car2.speed;

    if(avg_ws2-car2.wheelSpeedFL > abs_threshold)
        brake_msg2.brakeFL = (UINT8) limit((INT16)brake_msg2.brakeFL + (car2.wheelSpeedFL - avg_ws2) * ABS_KP,0,100);

    if(avg_ws2-car2.wheelSpeedFR > abs_threshold)
        brake_msg2.brakeFR = (UINT8) limit((INT16)brake_msg2.brakeFR + (car2.wheelSpeedFR - avg_ws2) * ABS_KP,0,100);

    if(avg_ws2-car2.wheelSpeedRL> abs_threshold)
        brake_msg2.brakeRL = (UINT8) limit((INT16)brake_msg2.brakeRL + (car2.wheelSpeedRL - avg_ws2) * ABS_KP,0,100);

    if(avg_ws2-car2.wheelSpeedRR > abs_threshold)
        brake_msg2.brakeRR = (UINT8) limit((INT16)brake_msg2.brakeRR + (car2.wheelSpeedRR - avg_ws2) * ABS_KP,0,100);
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

void Traction2()
{
    INT16 avg_rws2;
    INT16 correction2 = 0;

    avg_rws2 = (car2.wheelSpeedRL + car2.wheelSpeedRR)/2;
    if((avg_rws2 - car2.speed) > tc_threshold)
    {
        correction2 = (avg_rws2 - car2.speed) * TRAC_KP;
    }
    accel_corr2.accel = (UINT8) limit(correction2,0,100);
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

void stabilityControl2()
{
    INT16 correction2;
    correction2 = limit((abs(car2.yawRate) - yawRate_threshold)*STAB_KP, 0, 100);

    if(car2.yawRate > 0)
        brake_msg2.brakeRR = (UINT8) limit(brake_msg2.brakeRR + correction2, 0, 100);
    else
        brake_msg2.brakeRL = (UINT8) limit(brake_msg2.brakeRL + correction2, 0, 100);
    
    accel_corr2.accel = (UINT8) limit(correction2,0,100);
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
    else if(identifier == CAN_INPUT_MSG_ID_2)
    {
        if(!carInputsUpdated) // Only update when old value has been used
        {
            if(length > sizeof(CarInputs))
                length = sizeof(CarInputs);

            memcpy(&driver_input2, &CANRXDSR0, length);

            carInputsUpdated2 = 1;
        }
    }
    else if (identifier == CAN_PARAM_MSG_ID_2)
    {
        if(length > sizeof(CarParams))
            length = sizeof(CarParams);

        memcpy(&car2, &CANRXDSR0, length);

        carParamsUpdated2 = 1;
    }

    CANRFLG_RXF = 1; // Reset the flag
}

#pragma CODE_SEG DEFAULT
