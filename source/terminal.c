#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <FreeRTOS.h>
#include <task.h>

#include "terminal.h"

#define MAX_BUFFER_SIZE 64
#define BUFFER_POLL_RATE 5

/* Memory Buffer for storing bytes from UART */
char terminalBuffer[MAX_BUFFER_SIZE];
char *terminalBufferPtr;

xTaskHandle xTerminalTaskHandler;
void vTerminalTaskCode( void * pvParameters );
unsigned char ucTerminalTaskParameter;

void TerminalInit(void)
{
    /* Initialize buffer pointer */
    terminalBufferPtr = terminalBuffer;
    xTaskCreate(
            vTerminalTaskCode,
            (const signed char *)"terminal",
            62,
            &ucTerminalTaskParameter,
            tskIDLE_PRIORITY + 1,
            &xTerminalTaskHandler
            );
}

void vTerminalTaskCode(void * pvParameters) {
    const portTickType xDelay = BUFFER_POLL_RATE / portTICK_RATE_MS;
    for (;;) {
        vTaskDelay( xDelay );
    }
}
