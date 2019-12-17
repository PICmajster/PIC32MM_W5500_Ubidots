/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.h

  @Summary:
    This is the mcc.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.145.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.20
        MPLAB             :  MPLAB X v5.25
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef MCC_H
#define	MCC_H
#include <xc.h>
#include "system.h"
#include "clock.h"
#include "pin_manager.h"
#include <stdint.h>
#include <stdbool.h>
#include "interrupt_manager.h"
#include "exceptions.h"
#include "spi3.h"
#include "uart1.h"
#include "tmr1.h"
#include "watchdog.h"
#ifndef _XTAL_FREQ
#define _XTAL_FREQ  24000000UL
#endif
#define WDT_CLR_KEY 0x5743

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Initializes the oscillator to the default states configured in the
 *                  MCC GUI
 * @Example
    OSCILLATOR_Initialize(void);
 */
void OSCILLATOR_Initialize(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse CLOCK_Initialize (void) instead. ")));

/* Enables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WDT_WatchdogtimerSoftwareEnable();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse WATCHDOG_TimeroftwareEnable (void) instead. ")))
inline static void WDT_WatchdogtimerSoftwareEnable(void)
{
    WDTCONbits.ON = 1;
}
/* Disables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WDT_WatchdogtimerSoftwareDisable();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse WATCHDOG_TimerSoftwareDisable (void) instead. ")))
inline static void WDT_WatchdogtimerSoftwareDisable(void)
{
    WDTCONbits.ON = 0;
}
/* Clears the Watch Dog Timer (WDT).
 * @example
 * <code>
 * WDT_WatchdogTimerClear();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse WATCHDOG_TimerClear (void) instead. ")))
inline static void WDT_WatchdogTimerClear(void)
{
    WDTCON = WDT_CLR_KEY;
}
#endif	/* MCC_H */
/**
 End of File
*/