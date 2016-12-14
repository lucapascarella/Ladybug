/****************************************************************************
;*	Microchip Technology Inc. 2007                                      *
;*	Assembler version:  XC8 v1.34                                       *
;*	Filename:                                                           *
;*		main.c (main routine)                                       *
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


/*
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */


#include "device_initialize.h"

void resetAfterSleep(void);

void DelayMs(uint16_t delay);
void alternateLeds(uint16_t delay);

#define SLEEP_TIMEOUT       4u
uint8_t timer0counter;

struct {
    uint8_t leftButton : 1;
    uint8_t rightButton : 1;
    uint8_t putInSleep : 1;
    uint8_t exitFromSleepLeft : 1;
    uint8_t exitFromSleepRight : 1;
    uint8_t leftLed : 1;
    uint8_t rightLed : 1;
} flags;

void interrupt interruptHandling(void) {

    INTCONbits.GIE = 0; // 1 = Disables all active interrupts

    // IOCIF: Interrupt-on-Change Interrupt Flag bit
    if (INTCONbits.IOCIF) { // 1 = When at least one of the interrupt-on-change pins changed state
        // IOCIF bit is read only, to clear it, clear IOCAF register
        //if (flags.leftButton == false && flags.rightButton == false)
        if (flags.leftButton == false && flags.rightButton == false)
            if (IOCAFbits.IOCAF2) {
                flags.leftButton = true;
            } else if (IOCAFbits.IOCAF3) {
                flags.rightButton = true;
            }
        IOCAF = 0x00; // 0 = No change was detected, or the user cleared the detected change.
        ResetSleepTimer();
    }

    // TMR0IF: Timer0 Overflow Interrupt Flag bit
    if (INTCONbits.TMR0IF) { // 1 = TMR0 register has overflowed
        INTCONbits.TMR0IF = 0; // Clear flag
        // 8.46 sec => 31kHz
        if (timer0counter++ > SLEEP_TIMEOUT) {
            flags.putInSleep = true;
            timer0counter = 0;
        }
    }

    INTCONbits.GIE = 1; // 1 = Enables all active interrupts
}

//uint16_t dutycycle; // define variable - unsigned integer

// Main application

void main(void) {

    uint8_t state;
    uint16_t count, limit, round, wdt;
    int8_t i, patt;
    extern const uint8_t pattern[];


    //SYSTEM_Initialize(); // Initialize the device

    // IRCF = 000b => 31kHz with LFINTOSC 31kHz oscillator
    OSCCON = 0x00;

    // IOCIE: Interrupt-on-Change Interrupt Enable bit
    INTCONbits.IOCIE = 1; // 1 = Enables the interrupt-on-change interrupt

    // Disable analog input
    ANSELA = 0x00; // 0 = Digital I/O. Pin is assigned to port or Digital special function.
    //    // Disable all wake internal pull-up
    WPUA = 0x00; // 0 = Weak Pull-up disabled

    //    TRISAbits.TRISA2 = 1; // Set Channel RA2 as input
    //    //TRISAbits.TRISA3 = 1;   // Set Channel RA3 as input, it is shared with MCLR and is an only input pin when High voltage programming is enabled
    TRISA = 0x0C;
    
    //
    //    TRISAbits.TRISA0 = 0; // Set Channel RA0 as output
    //    TRISAbits.TRISA1 = 0; // Set Channel RA1 as output

    LATAbits.LATA0 = 0; // Set RA0 (LED SX) low
    LATAbits.LATA1 = 0; // Set RA1 (LED DX) low

    


    // Timer 0 used to put in sleep mode
    //TMR0_Initialize();
    // Clear TMR0 register, the value at reset is undefined
    TMR0 = 0;
    //    // T0CS: TMR0 Clock Source Select bit
    //    OPTION_REGbits.T0CS = 0; // Internal instruction cycle clock (FOSC/4)
    //
    //    // PS<2:0>: Prescaler Rate Select bits
    //    OPTION_REGbits.PS = 0x7; // 111b 1:256
    //
    //    // PSA: Prescaler Assignment bit
    //    OPTION_REGbits.PSA = 0; // Prescaler is assigned to the Timer0 module

    OPTION_REG = 0xD7;

    // TMR0IE: Timer0 Overflow Interrupt Enable bit
    INTCONbits.TMR0IE = 1; // Enables the Timer0 interrupt

    //PWM_Initialize();
    // PWM1EN Enabled; PWM1OE Enabled; PWM1POL active_hi
    PWM1CON = 0xC0;
    // PWM2EN Enabled; PWM2OE Enabled; PWM2POL active_hi
    PWM2CON = 0xC0;

    //TMR2_Initialize();
    // TMR2ON off; T2CKPS 1:1; T2OUTPS 1:16;
    T2CON = 0x78;
    //T2CON = 0x00;

    // PR2 255;
    //PR2 = 0xFF;
    // PR2 = 63
    PR2 = 0x3F;

    // TMR2 0x00;
    TMR2 = 0x00;

//    // Clearing IF flag.
//    PIR1bits.TMR2IF = 0;

    // GIE: Global Interrupt Enable bit
    INTCONbits.GIE = 1; // 1 = Enables all active interrupts

    //TMR2_StartTimer(); // Start Timer2 for PWM operation
    // Start the Timer by writing to TMRxON bit
    T2CONbits.TMR2ON = 1;

    // Enable Interrupt on RA2 and RA3
    IOCAN = 0x0C;

    // Set WDT
    WDTCON = 0x24; // 256 seconds, 4,26 minutes
    //WDTCON = 0x14; // 1 second

    // Put in sleep mode
    flags.putInSleep = true;
    flags.leftButton = false;
    flags.rightButton = false;

    while (1) {
        if (flags.putInSleep) {
            TurnOnLeds();
            DelayMs(30);
            TurnOffLeds();
            DelayMs(50);
            TurnOnLeds();
            DelayMs(30);
            TurnOffLeds();
            // Turn off all LEDs
            PWM1CONbits.PWM1OE = 0;
            PWM2CONbits.PWM2OE = 0;
            WDTCONbits.SWDTEN = true;
            wdt = 42; // About 3 hours
            //wdt = 1; // About 3 hours
            do {
                SLEEP();
            } while (wdt-- && STATUSbits.nTO == false && STATUSbits.nPD == false);
            WDTCONbits.SWDTEN = false;
            resetAfterSleep();
            state = 0;
            limit = 2000;
            round = 50;
            patt = 0;
            flags.leftLed = flags.rightLed = false;

            if (STATUSbits.nTO == false && STATUSbits.nPD == false) {
                // Put in sleep mode
                flags.putInSleep = true;
                state = 2;
            }
        }

        if (flags.exitFromSleepLeft) {
            if (state == 0) {
                alternateLeds(round);
                if (TMR2 & 0x01) {
                    TurnOnLeftLed();
                    flags.leftLed = true;
                    flags.rightLed = false;
                } else {
                    TurnOnRightLed();
                    flags.rightLed = true;
                    flags.leftLed = false;
                }
                flags.leftButton = false;
                flags.rightButton = false;
                count = 0;
                state = 1;
            } else if (state == 1) {
                if (flags.leftButton) {
                    flags.leftButton = false;
                    if (flags.leftLed) {
                        for (i = 24; i >= 0; i--) {
                            PWMLeftLoadDutyCicle(pattern[i]);
                            DelayMs(7);
                        }
                        state = 2;
                    } else {
                        state = 3;
                    }
                } else if (flags.rightButton) {
                    flags.rightButton = false;
                    if (flags.rightLed) {
                        for (i = 24; i >= 0; i--) {
                            PWMRightLoadDutyCicle(pattern[i]);
                            DelayMs(7);
                        }
                        state = 2;
                    } else {
                        state = 3;
                    }
                } else if (count++ > limit) {
                    state = 3;
                }
            } else if (state == 2) {
                if (limit > 50) {
                    limit -= 50;
                    round--;
                }
                DelayMs(500);
                state = 0;
            } else if (state == 3) {
                for (i = 0; i < 3; i++) {
                    TurnOnLeds();
                    DelayMs(80);
                    TurnOffLeds();
                    DelayMs(100);
                }
                DelayMs(800);
                state = 0;
            }
        }

        if (flags.exitFromSleepRight) {
            if (flags.leftButton) {
                flags.leftButton = false;
                if (patt > 0) {
                    PWMLeftLoadDutyCicle(pattern[--patt]);
                    PWMRightLoadDutyCicle(pattern[patt]);
                }
            }
            if (flags.rightButton) {
                flags.rightButton = false;
                if (patt < 24) {
                    PWMLeftLoadDutyCicle(pattern[++patt]);
                    PWMRightLoadDutyCicle(pattern[patt]);
                }
            }
        }
    }
}
// End of File

void resetAfterSleep(void) {

    uint8_t i;
    int8_t j;
    extern const uint8_t pattern[];

    INTCONbits.GIE = 0; // 1 = Disables all active interrupts

    // Re-enable the PWM output
    PWM1CONbits.PWM1OE = 1;
    PWM2CONbits.PWM2OE = 1;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 25; j++) {
            PWMLeftLoadDutyCicle(pattern[j]);
            PWMRightLoadDutyCicle(pattern[j]);
            DelayMs(1);
        }
        for (j = 24; j >= 0; j--) {
            PWMLeftLoadDutyCicle(pattern[j]);
            PWMRightLoadDutyCicle(pattern[j]);
            DelayMs(1);
        }
    }

    //ResetSleepTimer();
    flags.putInSleep = false;
    flags.exitFromSleepLeft = flags.leftButton;
    flags.exitFromSleepRight = flags.rightButton;

    flags.leftButton = false;
    flags.rightButton = false;

    INTCONbits.GIE = 1; // 1 = Enables all active interrupts
}


//// 1 instruction => 130us

void DelayMs(uint16_t delay) {

    while (delay--) {
        asm("goto $+1");
        asm("goto $+1");
        NOP();
    }
    asm("goto $+1");
}

void alternateLeds(uint16_t delay) {
    uint8_t i;
    for (i = 0; i < 4; i++) {
        TurnOnLeftLed();
        TurnOffRightLed();
        DelayMs(delay);
        TurnOnRightLed();
        TurnOffLeftLed();
        DelayMs(delay);
    }
    TurnOffLeds();
}