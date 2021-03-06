{ =====================================================================
Register and vector equates for MSP430x43x

Copyright (C) 2003  FORTH, Inc.

Standard register, bit, and interrupt vector definitions for the
Texas Instruments MSP430 microcontroller.

===================================================================== }

ASSEMBLER

\ ***********************************************************
\  STATUS REGISTER BITS
\ **********************************************************

$0001 CONSTANT C
$0002 CONSTANT Z
$0004 CONSTANT N
$0100 CONSTANT V
$0008 CONSTANT GIE
$0010 CONSTANT CPUOFF
$0020 CONSTANT OSCOFF
$0040 CONSTANT SCG0
$0080 CONSTANT SCG1

\ Low Power Modes coded with Bits 4-7 in SR.  See "OPERATING MODES"
\ section of the MSP430x43x User's Guide for details about LPM.

0                               EQU AM          \ Active Mode
CPUOFF                          EQU LPM0        \ Low Power Mode 0
SCG0 CPUOFF +                   EQU LPM1        \ etc...
SCG1 CPUOFF +                   EQU LPM2
SCG1 SCG0 CPUOFF + +            EQU LPM3
SCG1 SCG0 OSCOFF CPUOFF + + +   EQU LPM4

SCG1 SCG0 OSCOFF CPUOFF + + +   EQU &LPM        \ LPM bit mask

TARGET

\ ***********************************************************
\  SPECIAL FUNCTION REGISTER ADDRESSES + CONTROL BITS
\ **********************************************************

$0000 EQU IE1
$01 EQU WDTIE
$02 EQU OFIE
$10 EQU NMIIE
$20 EQU ACCVIE
$40 EQU URXIE0
$80 EQU UTXIE0

$0002 EQU IFG1
$01 EQU WDTIFG
$02 EQU OFIFG
$10 EQU NMIIFG
$40 EQU URXIFG0
$80 EQU UTXIFG0

$0004 EQU ME1
$40 EQU URXE0
$40 EQU USPIE0
$80 EQU UTXE0

$0001 EQU IE2
$80 EQU BTIE

$0003 EQU IFG2
$80 EQU BTIFG

\ ***********************************************************
\  WATCHDOG TIMER
\ **********************************************************

$0120 EQU WDTCTL

$0001 EQU WDTIS0
$0002 EQU WDTIS1
$0004 EQU WDTSSEL
$0008 EQU WDTCNTCL
$0010 EQU WDTTMSEL
$0020 EQU WDTNMI
$0040 EQU WDTNMIES
$0080 EQU WDTHOLD

$5A00 EQU WDTPW

\ ***********************************************************
\  DIGITAL I/O Port1/2
\ **********************************************************

$0020 EQU P1IN
$0021 EQU P1OUT
$0022 EQU P1DIR
$0023 EQU P1IFG
$0024 EQU P1IES
$0025 EQU P1IE
$0026 EQU P1SEL

$0028 EQU P2IN
$0029 EQU P2OUT
$002A EQU P2DIR
$002B EQU P2IFG
$002C EQU P2IES
$002D EQU P2IE
$002E EQU P2SEL

\ ***********************************************************
\  DIGITAL I/O Port3/4
\ **********************************************************

$0018 EQU P3IN
$0019 EQU P3OUT
$001A EQU P3DIR
$001B EQU P3SEL

$001C EQU P4IN
$001D EQU P4OUT
$001E EQU P4DIR
$001F EQU P4SEL

\ ***********************************************************
\  DIGITAL I/O Port5/6
\ **********************************************************

$0030 EQU P5IN
$0031 EQU P5OUT
$0032 EQU P5DIR
$0033 EQU P5SEL

$0034 EQU P6IN
$0035 EQU P6OUT
$0036 EQU P6DIR
$0037 EQU P6SEL

\ ***********************************************************
\  BASIC TIMER
\ **********************************************************

$0040 EQU BTCTL

$01 EQU BTIP0
$02 EQU BTIP1
$04 EQU BTIP2
$08 EQU BTFRFQ0
$10 EQU BTFRFQ1
$20 EQU BTDIV
$40 EQU BTRESET
$40 EQU BTHOLD
$80 EQU BTSSEL

$0046 EQU BTCNT1
$0047 EQU BTCNT2

\ ***********************************************************
\  SYSTEM CLOCK, FLL+
\ **********************************************************

$0050 EQU SCFI0
$04 EQU FN_2
$08 EQU FN_3
$10 EQU FN_4
$20 EQU FN_8
$40 EQU FLLD0
$80 EQU FLLD1

$00 EQU FLLD_1
$40 EQU FLLD_2
$80 EQU FLLD_4
$C0 EQU FLLD_8

$0051 EQU SCFI1
$0052 EQU SCFQCTL

$0053 EQU FLL_CTL0
$01 EQU DCOF
$02 EQU LFOF
$04 EQU XT1OF
$08 EQU XT2OF
$00 EQU XCAP0PF
$10 EQU XCAP10PF
$20 EQU XCAP14PF
$30 EQU XCAP18PF
$40 EQU XTS_FLL
$80 EQU DCOPLUS

$0054 EQU FLL_CTL1

$00 EQU FLL_DIV_1
$01 EQU FLL_DIV_2
$02 EQU FLL_DIV_4
$03 EQU FLL_DIV_8
$04 EQU SELS
$00 EQU SELM_DCO
$10 EQU SELM_XT2
$18 EQU SELM_A
$20 EQU XT2OFF
$40 EQU SMCLKOFF

\ ***********************************************************
\  Brown-Out, Supply Voltage Supervision SVS
\ **********************************************************

$0056 EQU SVSCTL
$01 EQU SVSFG
$02 EQU SVSOP
$04 EQU SVSON
$08 EQU PORON
$00 EQU VLDOFF
$10 EQU VLDON
$10 EQU VLD_1_8V

\ ***********************************************************
\  LCD
\ **********************************************************

$0090 EQU LCDCTL

$01 EQU LCDON
$02 EQU LCDLOWR
$04 EQU LCDSON
$08 EQU LCDMX0
$10 EQU LCDMX1
$20 EQU LCDP0
$40 EQU LCDP1
$80 EQU LCDP2

$0091 EQU LCDMEM1
$0092 EQU LCDMEM2
$0093 EQU LCDMEM3
$0094 EQU LCDMEM4
$0095 EQU LCDMEM5
$0096 EQU LCDMEM6
$0097 EQU LCDMEM7
$0098 EQU LCDMEM8
$0099 EQU LCDMEM9
$009A EQU LCDMEM10
$009B EQU LCDMEM11
$009C EQU LCDMEM12
$009D EQU LCDMEM13
$009E EQU LCDMEM14
$009F EQU LCDMEM15
$00A0 EQU LCDMEM16
$00A1 EQU LCDMEM17
$00A2 EQU LCDMEM18
$00A3 EQU LCDMEM19
$00A4 EQU LCDMEM20

\ ***********************************************************
\  USART
\ **********************************************************

$80 EQU PENA
$40 EQU PEV
$20 EQU SPB
$10 EQU CHARB           \ Renamed -- conflicts with standard word CHAR
$08 EQU LISTEN
$04 EQU SYNC
$02 EQU MM
$01 EQU SWRST

$80 EQU CKPH
$40 EQU CKPL
$20 EQU SSEL1
$10 EQU SSEL0
$08 EQU URXSE
$04 EQU TXWAKE
$02 EQU STC
$01 EQU TXEPT

$80 EQU FE
$40 EQU PE
$20 EQU OE
$10 EQU BRK
$08 EQU URXEIE
$04 EQU URXWIE
$02 EQU RXWAKE
$01 EQU RXERR

\ ***********************************************************
\  USART 0
\ **********************************************************

$0070 EQU U0CTL
$0071 EQU U0TCTL
$0072 EQU U0RCTL
$0073 EQU U0MCTL
$0074 EQU U0BR0
$0075 EQU U0BR1
$0076 EQU U0RXBUF
$0077 EQU U0TXBUF

\ ***********************************************************
\  Timer A
\ **********************************************************

$012E EQU TAIV
$0160 EQU TACTL
$0162 EQU TACCTL0
$0164 EQU TACCTL1
$0166 EQU TACCTL2
$0170 EQU TAR
$0172 EQU TACCR0
$0174 EQU TACCR1
$0176 EQU TACCR2

\  Alternate register names

$0162 EQU CCTL0
$0164 EQU CCTL1
$0166 EQU CCTL2
$0172 EQU CCR0
$0174 EQU CCR1
$0176 EQU CCR2

$0400 EQU TASSEL2
$0200 EQU TASSEL1
$0100 EQU TASSEL0
$0080 EQU ID1
$0040 EQU ID0
$0020 EQU MC1
$0010 EQU MC0
$0004 EQU TACLR
$0002 EQU TAIE
$0001 EQU TAIFG

$8000 EQU CM1
$4000 EQU CM0
$2000 EQU CCIS1
$1000 EQU CCIS0
$0800 EQU SCS
$0400 EQU SCCI
$0100 EQU CAP
$0080 EQU OUTMOD2
$0040 EQU OUTMOD1
$0020 EQU OUTMOD0
$0010 EQU CCIE
$0008 EQU CCI
$0004 EQU OUT
$0002 EQU COV
$0001 EQU CCIFG

\ ***********************************************************
\  Timer B
\ **********************************************************

$011E EQU TBIV
$0180 EQU TBCTL
$0182 EQU TBCCTL0
$0184 EQU TBCCTL1
$0186 EQU TBCCTL2
$0190 EQU TBR
$0192 EQU TBCCR0
$0194 EQU TBCCR1
$0196 EQU TBCCR2

$4000 EQU SHR1
$2000 EQU SHR0
$4000 EQU TBCLGRP1
$2000 EQU TBCLGRP0
$1000 EQU CNTL1
$0800 EQU CNTL0
$0400 EQU TBSSEL2
$0200 EQU TBSSEL1
$0100 EQU TBSSEL0
$0004 EQU TBCLR
$0002 EQU TBIE
$0001 EQU TBIFG

$0400 EQU SLSHR1
$0200 EQU SLSHR0
$0400 EQU CLLD1
$0200 EQU CLLD0

\ ************************************************************
\  Flash Memory
\ ***********************************************************

$0128 EQU FCTL1
$012A EQU FCTL2
$012C EQU FCTL3

$9600 EQU FRKEY
$A500 EQU FWKEY
$3300 EQU FXKEY

$0002 EQU ERAS          \ Renamed -- conflicts with standard word ERASE
$0004 EQU MERAS
$0040 EQU WRT
$0080 EQU BLKWRT
$0080 EQU SEGWRT

$0001 EQU FN0
$0002 EQU FN1
$0004 EQU FN2
$0008 EQU FN3
$0010 EQU FN4
$0020 EQU FN5
$0040 EQU FSSEL0
$0080 EQU FSSEL1

$0001 EQU BUSY
$0002 EQU KEYV
$0004 EQU ACCVIFG
$0008 EQU FWAIT         \ Renamed -- conflicts with Multitasker WAIT
$0010 EQU LOCK
$0020 EQU EMEX

\ ***********************************************************
\  Comparator A
\ **********************************************************

$0059 EQU CACTL1
$005A EQU CACTL2
$005B EQU CAPD

$01 EQU CAIFG
$02 EQU CAIE
$04 EQU CAIES
$08 EQU CAON
$10 EQU CAREF0
$20 EQU CAREF1
$40 EQU CARSEL
$80 EQU CAEX

$01 EQU CAOUT
$02 EQU CAF
$04 EQU P2CA0
$08 EQU P2CA1
$10 EQU CACTL24
$20 EQU CACTL25
$40 EQU CACTL26
$80 EQU CACTL27

$01 EQU CAPD0
$02 EQU CAPD1
$04 EQU CAPD2
$08 EQU CAPD3
$10 EQU CAPD4
$20 EQU CAPD5
$40 EQU CAPD6
$80 EQU CAPD7

\ ***********************************************************
\  ADC12
\ **********************************************************

$01A0 EQU ADC12CTL0
$01A2 EQU ADC12CTL1
$01A4 EQU ADC12IFG
$01A6 EQU ADC12IE
$01A8 EQU ADC12IV

$0140 EQU ADC12MEM0
$0142 EQU ADC12MEM1
$0144 EQU ADC12MEM2
$0146 EQU ADC12MEM3
$0148 EQU ADC12MEM4
$014A EQU ADC12MEM5
$014C EQU ADC12MEM6
$014E EQU ADC12MEM7
$0150 EQU ADC12MEM8
$0152 EQU ADC12MEM9
$0154 EQU ADC12MEM10
$0156 EQU ADC12MEM11
$0158 EQU ADC12MEM12
$015A EQU ADC12MEM13
$015C EQU ADC12MEM14
$015E EQU ADC12MEM15

$0080 EQU ADC12MCTL0
$0081 EQU ADC12MCTL1
$0082 EQU ADC12MCTL2
$0083 EQU ADC12MCTL3
$0084 EQU ADC12MCTL4
$0085 EQU ADC12MCTL5
$0086 EQU ADC12MCTL6
$0087 EQU ADC12MCTL7
$0088 EQU ADC12MCTL8
$0089 EQU ADC12MCTL9
$008A EQU ADC12MCTL10
$008B EQU ADC12MCTL11
$008C EQU ADC12MCTL12
$008D EQU ADC12MCTL13
$008E EQU ADC12MCTL14
$008F EQU ADC12MCTL15

$001 EQU ADC12SC
$002 EQU ENC
$004 EQU ADC12TOVIE
$008 EQU ADC12OVIE
$010 EQU ADC12ON
$020 EQU REFON
$040 EQU REF2_5V
$080 EQU MSH

$80 EQU EOS

\ ***********************************************************
\  Interrupt Vectors
\ **********************************************************

$FFE0 EQU BASICTIMER_VECTOR
$FFE2 EQU PORT2_VECTOR
$FFE8 EQU PORT1_VECTOR
$FFEA EQU TIMERA1_VECTOR
$FFEC EQU TIMERA0_VECTOR
$FFEE EQU ADC_VECTOR
$FFF0 EQU UART0TX_VECTOR
$FFF2 EQU UART0RX_VECTOR
$FFF4 EQU WDT_VECTOR
$FFF6 EQU COMPARATORA_VECTOR
$FFF8 EQU TIMERB1_VECTOR
$FFFA EQU TIMERB0_VECTOR
$FFFC EQU NMI_VECTOR
$FFFE EQU RESET_VECTOR

