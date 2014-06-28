#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "driverlib/sysctl.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "task_led1.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


// The stack size for the LED1  task.
#define LED1TASKSTACKSIZE        128         // Stack size in words

// This task blinks LED1 with a period of 250ms
static void LED1Task(void *pvParameters)
{

    while(1)
    {
    	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    	ROM_SysCtlDelay(ROM_SysCtlClockGet()/12);
    	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
    	ROM_SysCtlDelay(ROM_SysCtlClockGet()/12);
    }
}

// Initializes the LED1 task.
uint32_t LED1TaskInit(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

	// Create the LED task.
    if(xTaskCreate(LED1Task, (signed portCHAR *)"LED1", LED1TASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LED1_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    // Success.
    return(0);
}
