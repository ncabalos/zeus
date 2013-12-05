/*
 * File:   main.c
 * Author: ncabalos
 *
 * Created on November 16, 2013, 2:44 PM
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <p24Fxxxx.h>

#include <serio.h>
#include <terminal.h>

#ifdef UNIT_TEST
#include <unity.h>
#include <unit_tests.h>
#else
#include <FreeRTOS.h>
#include <task.h>
#endif

/* Global Variables ***********************************************************/
UART_INFO * DebugUART;

/* Function Protoypes *********************************************************/
static void HardwareInit(void);


#ifndef UNIT_TEST

void HardwareInit(void)
{
    DebugUART = UartInit(UART_2, 115200UL, SERIO_1STOPBIT, SERIO_NOPARITY);
}

void ModuleInit(void)
{
    TerminalInit(DebugUART);
}

extern void execute_pwm_command(char * command);
extern uint16_t is_number(char * str);
char test[] = "pwm +1 555";
char test2[] = "-100";
int main(int argc, char ** argv)
{
    HardwareInit();
    ModuleInit();
    vTaskStartScheduler();
    return (EXIT_SUCCESS);
}
#else

int main(int argc, char ** argv)
{
    UnityBegin();
    UnityEnd();
    return (EXIT_SUCCESS);
}
#endif

