/****************************************************************************
;*	Microchip Technology Inc. 2007                                      *
;*	Assembler version:  XC8 v1.34                                       *
;*	Filename:                                                           *
;*		pwm.c                                                       *
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
#include <stdbool.h>
#include "pwm.h"

// Section: PWM Module APIs
const uint8_t pattern[] = {0, 1, 2, 3, 4, 6, 7, 9, 11, 13, 16, 20, 24, 29, 36, 44, 54, 66, 81, 99, 121, 148, 181, 221, 255};

//void PWM_Initialize(void) {
//    // PWM1EN Enabled; PWM1OE Enabled; PWM1POL active_hi
//    PWM1CON = 0xC0;
//    // PWM2EN Enabled; PWM2OE Enabled; PWM2POL active_hi
//    PWM2CON = 0xC0;
//
////    // PWM modules have 8 bit resolution
////    PWM1LoadDutyValue(0x0000);
////    PWM2LoadDutyValue(0x0000);
//}

void PWM1LoadDutyValue(uint8_t dutyValue) {
    //2 LSB's of dutyValue
    PWM1DCL = dutyValue << 6; // Left shift by 6 to write 2 LSB bits of ADRES

    //8 MSB's of dutyValue
    PWM1DCH = dutyValue >> 2; // Right shift by 2 to write 8 MSB bits of ADRES
}

void PWM2LoadDutyValue(uint8_t dutyValue) {
    //2 LSB's of dutyValue
    PWM2DCL = dutyValue << 6; // Left shift by 6 to write 2 LSB bits of ADRES

    //8 MSB's of dutyValue
    PWM2DCH = dutyValue >> 2; // Right shift by 2 to write 8 MSB bits of ADRES
}


// End of File


