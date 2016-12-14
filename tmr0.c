/****************************************************************************
;*	Microchip Technology Inc. 2007                                      *
;*	Assembler version:  XC8 v1.34                                       *
;*	Filename:                                                           *
;*		tmr2.c                                                      *
;*	Dependents:                                                         *
;*                                                                          *
;*	April 13, 2015                                                      *
;*      PIC10(L)32X Developmental Board                                     *
;*      Demonstration program:                                              *
;*          After applying power to the PIC10(L)F32X Development Board,     *
;*          LED (D1) will automatically turn on.  Turn POT1 clockwise       *
;*          to increase the brightness of LED (D2).  Press switch (SW1)     *
;*          to turn both LEDs D1 and D2 off, release switch (SW1) and       *
;*          LEDs D1 and D2 will turn on.                                    *
;*                                                                          *
;****************************************************************************/

// Section: Included Files

#include <xc.h>
#include "tmr0.h"

//void TMR0_Initialize(void) {
//
//    // Interrupt period = 1/(Fosc/4/256/256)
//    // 8.46 sec => 31kHz
//
//    // Clear TMR0 register, the value at reset is undefined
//    TMR0 = 0;
//
////    // T0CS: TMR0 Clock Source Select bit
////    OPTION_REGbits.T0CS = 0; // Internal instruction cycle clock (FOSC/4)
////
////    // PS<2:0>: Prescaler Rate Select bits
////    OPTION_REGbits.PS = 0x7; // 111b 1:256
////
////    // PSA: Prescaler Assignment bit
////    OPTION_REGbits.PSA = 0; // Prescaler is assigned to the Timer0 module
//
//    OPTION_REG = 0xD7;
//
//    // TMR0IE: Timer0 Overflow Interrupt Enable bit
//    INTCONbits.TMR0IE = 1; // Enables the Timer0 interrupt
//}



// End of File

