/* 
 * File:   main.c
 * Author: ncabalos
 *
 * Created on November 16, 2013, 2:44 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <p24Fxxxx.h>

#ifdef UNIT_TEST
#include <unity.h>
#include <unit_tests.h>
#else
#include <FreeRTOS.h>
#include <task.h>
#endif

/* Function Protoypes *********************************************************/
static void HardwareInit(void);

#ifndef UNIT_TEST

void HardwareInit(void)
{
}

int main(int argc, char** argv)
{
    HardwareInit();
    vTaskStartScheduler();
    return (EXIT_SUCCESS);
}
#else

int main(int argc, char** argv)
{
    UnityBegin();
    UnityEnd();
    return (EXIT_SUCCESS);
}
#endif

