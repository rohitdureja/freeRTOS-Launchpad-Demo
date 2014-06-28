/* Demo application running FreeRTOS
 * on the Tiva C Series Launchpad
 *
 * The application blinks an LED at the user defined rate
 * configured over UART.
 *
 * Rohit Dureja
 * University of Pennsylvania
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "task_led1.h"
#include "task_led2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif


// This hook is called by FreeRTOS when an stack overflow error is detected.
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


// Configure the UART and its pins.  This must be called before UARTprintf().
void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 9600, 16000000);
}


// Initialize FreeRTOS and start the initial set of tasks.
int main(void)
{
    // Set the clocking to run at 50 MHz from the PLL.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);

    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    ConfigureUART();

    // Print demo introduction.
    UARTprintf("\n\nWelcome to the EK-TM4C123GXL FreeRTOS Demo!\n");

    // Create the LED1 task.
    if(LED1TaskInit() != 0)
    {

        while(1)
        {
        }
    }

    // Create the LED2 task.
    if(LED2TaskInit() != 0)
    {

        while(1)
        {
        }
    }

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    // In case the scheduler returns for some reason, print an error and loop forever.
    UARTprintf("\n\nScheduler error!\n");

    while(1)
    {
    }
}
