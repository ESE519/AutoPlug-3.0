#pragma CODE_SEG DEFAULT

#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h> /* derivative information */
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "SCI.h"
#pragma LINK_INFO DERIVATIVE "mc9s12c128"

UINT8 SCIData[2];
UINT8 SCIDataFlag;

/*****************************************************************************
* Method: SCIInit
* Used to Initialize the SCI module for debuging purposes
*****************************************************************************/
void SCIInit(void)
{
    SCIBDH = 0;
    SCIBDL = BDL9600;

    SCICR1 = 0x00;          /*SCI0CR2 = 0x0C;  8 bit, TE and RE set */
    SCICR2 = 0x0C;          /* TE and RE enable */
}

void SCITxBuf(UINT8 *buf, UINT8 length)
{
    UINT8 i;

    for(i = 0; i < length; i++)
    {
        SCITx(buf[i]);
    }
}

void SCITx(UINT8 cData)
{
    while(!SCISR1_TDRE) ;   /* ensure Tx data buffer empty */
    SCIDRL = cData;         /* load data to SCI2 register */
    while(!SCISR1_TC) ;     /* wait for Tx complete */
}

void SCITxStr(char *pStr)
{
    UINT16 i;
    UINT16 nStrLen = strlen( (const char *)pStr );

    for(i = 0; i < nStrLen; i++)
    {
        SCITx(pStr[i]);
    }
}

/*****************************************************************************
* Method: SCI Interrupt
* Interrupt Declaration
*****************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED

interrupt void SCIRx_vect(void)
{
    UINT8 status, dummy;

    /* ensure Rx data buffer is full */
    do
    {
        status = SCISR1;
    }
    while(SCISR1_RDRF == 0);

    /* Check for Errors (Framing, Noise, Parity) */
    if( (status & 0x07) != 0 )
    {
        dummy = SCIDRL;
        return;
    }

    /* Good Data */
    SCIData[0] = SCIDRL; /* load SCI register to data */
    SCIData[1] = 0;

    SCIDataFlag = 1;
}

#pragma CODE_SEG DEFAULT

UINT8 SCIgets(char *pPrompt, UINT8 *buf, UINT8 length)
{
    UINT8 exitFlag = FALSE;
    UINT8 offset   = 0;

    // Echo the string for the command line
    SCITxStr(pPrompt);

    // Keep processing SCI inputs until length is reached or terminating NULL occurs
    while(exitFlag == FALSE)
    {
        if(SCIDataFlag == 1)
        {
            SCIDataFlag = 0;

            // Check the terminating string
            switch(SCIData[0])
            {
                // Check for Terminating string
                case 0x0D:
                    exitFlag    = TRUE;
                    buf[offset] = 0; // Terminate sting with null.
                    break;

                case '\b':
                    if(offset != 0)
                        buf[offset--] = 0;
                    else
                        buf[offset] = 0;
                    break;

                case '\n':
                default:
                    buf[offset++] = SCIData[0];
                    break;
            }

            //Check the buffersize not to overrun buffer.

            if(offset == length)
            {
                exitFlag = TRUE;
            }
        }
    }

    return offset;
}

UINT8 SCIRx(void)
{
    while(!SCISR1_RDRF) ;
    return (SCIDRL);
}

#pragma CODE_SEG DEFAULT
