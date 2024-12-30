/**
 * @file main.c
 * @author Seb Madgwick
 * @brief Main file.
 *
 * Device:
 * PIC32MM0064GPL020
 *
 * Compiler:
 * XC32 v4.45, MPLAB Harmony 3
 */

//------------------------------------------------------------------------------
// Includes

#include "Adc/Adc.h"
#include "definitions.h"
#include "ResetCause/ResetCause.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "Timer/Timer.h"
#include "Uart/Uart1.h"

//------------------------------------------------------------------------------
// Functions

int main(void) {
    SYS_Initialize(NULL);

    // Initialise modules
    TimerInitialise();
    Uart1Initialise(&uartSettingsDefault);
    AdcInitialise();

    // Print start up message
    TimerDelayMilliseconds(500);
    ResetCausePrint(ResetCauseGet());
    printf("x-IMU3-SA-A8 v1.0.6\n");

    // Main program loop
    while (true) {
        SYS_Tasks();

        // Send ADC data
        static AdcData previousData;
        AdcData data;
        AdcGetData(&data);
        if (data.counter != previousData.counter) {
            previousData = data;
            printf("%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f\n", data.ch1, data.ch2, data.ch3, data.ch4, data.ch5, data.ch6, data.ch7, data.ch8);
        }

        // Enable idle mode (will wake up on next interrupt)
        POWER_LowPowerModeEnter(LOW_POWER_IDLE_MODE);
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
