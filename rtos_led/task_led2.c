#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "task_led2.h"
#include "task_led1.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// The stack size for the LED2 task.
#define LED2TASKSTACKSIZE	128         // Stack size in words

// This task blinks LED2 with a period of 125ms
static void LED2Task(void *pvParameters)
{
    while(1)
    {
    	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    	ROM_SysCtlDelay(ROM_SysCtlClockGet()/24);
    	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    	ROM_SysCtlDelay(ROM_SysCtlClockGet()/24);
    }
}

// Initializes the LED2 task.
uint32_t LED2TaskInit(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

	// Create the switch task.
    if(xTaskCreate(LED2Task, (signed portCHAR *)"LED2",
                   LED2TASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_LED2_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}
