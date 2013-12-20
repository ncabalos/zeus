/****************************************************************************** 
 * Copyright (C) Nathaniel Abalos - All Rights Reserved                       *
 * Contents of this file is proprietary and confidential. Unauthorized        *
 * copying of this file, via any medium is strictly prohibited.               *
 *                                                                            *
 * Contact information: nathaniel_abalos@hotmail.com                          *
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>

#include "serio.h"
#include "terminal.h"

#define MAX_BUFFER_SIZE 64
#define BUFFER_POLL_RATE 5
#define SIMULATOR 1

/* Memory Buffer for storing bytes from UART */
char terminalBuffer[MAX_BUFFER_SIZE];
char * terminalBufferPtr;

const char prompt[] = "zeus:~$ ";

static xTaskHandle xTerminalTaskHandler;
static void vTerminalTaskCode( void * pvParameters );
static unsigned char ucTerminalTaskParameter;
static void display_prompt(bool newline);

void terminal_output(const char * string);
void terminal_output_char(char character);

extern uint16_t parse_command(char * command);

static UART_INFO * terminalUart;

void TerminalInit(UART_INFO * uart_info)
{
    if(uart_info == NULL) {
        return;
    } else {
        terminalUart = uart_info;
    }

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
    display_prompt(true);
}

void vTerminalTaskCode(void * pvParameters)
{
    const portTickType xDelay = BUFFER_POLL_RATE / portTICK_RATE_MS;
    static uint16_t byte_count = 0;
    static uint16_t bytes_read;
    static uint16_t bytes_remaining = 0;
    static uint16_t command_result;
    static char byte_value;
    uint16_t i, length;

    for (;;) {
        byte_count = UartRxDataAvailable(terminalUart);

        if(byte_count > 0) {
            /* Get a character */
            UartReadData(terminalUart, (uint8_t *)&byte_value, 1);
            /* Put character into buffer */
            *terminalBufferPtr = byte_value;

            if(byte_value == 0x0A) {
                terminal_output("\r\n");
                /* Replace line feed with null character */
                *terminalBufferPtr = 0;
                /* Reset buffer pointer */
                terminalBufferPtr = terminalBuffer;
                length = strlen(terminalBuffer);

                /* Remove any control characters */
                for (i = 0; i < length; i++) {
                    if (terminalBuffer[i] < 0x20) {
                        terminalBuffer[i] = 0;
                    }
                }

                if(strlen(terminalBuffer) > 0) {
                    command_result = parse_command(terminalBuffer);
                }

                display_prompt(false);
            } else {
                terminal_output_char(byte_value);
                terminalBufferPtr++;
            }

            bytes_remaining = byte_count - bytes_read;
        }

        if(bytes_remaining == 0) {
            vTaskDelay( xDelay );
        }
    }
}

void display_prompt(bool newline)
{
    if(newline == true) {
        terminal_output("\r\n");
    }

    terminal_output(prompt);
}

void terminal_output(const char * string)
{
    UartWriteData(terminalUart, (uint8_t *)string, strlen(string));
#if SIMULATOR == 1
    fputs(string, stdout);
#endif
}
void terminal_output_char(char character)
{
    UartWriteData(terminalUart, (uint8_t *)&character, 1);
#if SIMULATOR == 1
    putc(character, stdout);
#endif
}