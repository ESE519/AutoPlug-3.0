#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h> /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12c128"
#include "common.h"
#include "SCI.h"
#include "ADC.h"
#include "CAN.h"

#define CAN_MSG_ID (0xF0)

enum
{
    ACCEL = 0,
    BRAKE,
    STEER,
    GEAR,
    CLUTCH,
    NUM_PKT_FIELDS
} CANPacketFields;

void init(void);

void main(void)
{
    UINT8 accelVal;
    UINT8 steerVal;
    INT8  ret;
    UINT8 txBuffer[NUM_PKT_FIELDS];

    init();

    for(;; )
    {
        steerVal = ADCConvertSingle(7)/2;
        if(steerVal > 100)
            steerVal = 100;
        
        accelVal = ADCConvertSingle(6)/2;
        if(accelVal > 100)
            accelVal = 100;

        txBuffer[ACCEL]  = accelVal;
        txBuffer[BRAKE]  = 0;
        txBuffer[STEER]  = steerVal;
        txBuffer[GEAR]   = 1;
        txBuffer[CLUTCH] = 30;

        ret = CANTx(CAN_MSG_ID, txBuffer, NUM_PKT_FIELDS);
        if(ret != NUM_PKT_FIELDS)
            SCITxStr("CAN Tx error!\r\n");
    }
}

void init()
{
    SCIInit();
    ADCInit();
    CANInit();
}
