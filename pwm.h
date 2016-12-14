/****************************************************************************
;*	Microchip Technology Inc. 2007                                      *
;*	Assembler version:  XC8 v1.34                                       *
;*	Filename:                                                           *
;*		pwm.h                                                       *
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

#ifndef _PWM_H
#define _PWM_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>

/**
  Section: PWM Module APIs
*/

//#define PWM1DutyCicle(value)        PWM1DCH = value;
//#define PWM2DutyCicle(value)        PWM2DCH = value;

#define PWMLeftLoadDutyCicle(value)     PWM1LoadDutyValue(value)
#define PWMRightLoadDutyCicle(value)    PWM2LoadDutyValue(value)

#define TurnOffLeftLed()                PWMLeftLoadDutyCicle(0x00)
#define TurnOffRightLed()               PWMRightLoadDutyCicle(0x00)

#define TurnOnLeftLed()                 PWMLeftLoadDutyCicle(0xFF)
#define TurnOnRightLed()                PWMRightLoadDutyCicle(0xFF)

#define TurnOnLeds()                    PWMLeftLoadDutyCicle(0xFF); PWMRightLoadDutyCicle(0xFF);
#define TurnOffLeds()                   PWMLeftLoadDutyCicle(0x00); PWMRightLoadDutyCicle(0x00);

/**
  @Summary
    Initializes the PWM

  @Description
    This routine initializes the PWM_Initialize
  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment


 @Example
    <code>
    uint16_t dutycycle;

    PWM_Initialize();
	PWM_LoadDutyValue(dutycycle);
    </code>
 */
void PWM_Initialize(void);

/**
  @Summary
    Loads 16-bit duty cycle.

  @Description
    This routine loads the 16 bit duty cycle value.

  @Preconditions
    PWM_Initialize() function should have been called
	before calling this function.

  @Param
    Pass 16bit duty cycle value.

  @Returns
    None

  @Example
    <code>
    uint16_t dutycycle;

    PWM_Initialize();
    PWM_LoadDutyValue(dutycycle);
    </code>
*/

void PWM1LoadDutyValue(uint8_t dutyValue);
void PWM2LoadDutyValue(uint8_t dutyValue);

bool PWMLeftIncreaseDutyCicle(void);
bool PWMRightIncreaseDutyCicle(void);
bool PWMLeftDecreaseDutyCicle(void);
bool PWMRightDecreaseDutyCicle(void);

#endif
/**
 End of File
*/
