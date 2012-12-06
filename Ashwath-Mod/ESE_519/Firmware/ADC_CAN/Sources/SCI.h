#ifndef _SCI_H
#define _SCI_H

#define SCI_FBUS F_BUS
#define BDL9600 (SCI_FBUS / 16 / 9600)

#pragma CODE_SEG DEFAULT

void SCIInit(void);
void SCITx(UINT8 cData);
void SCITxStr(char *pStr);
void SCITxBuf(UINT8 *buf, UINT8 length);
UINT8 SCIRx(void);

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt void SCIRx_vect();
#pragma CODE_SEG DEFAULT

#endif
