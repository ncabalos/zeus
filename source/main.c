/* 
 * File:   main.c
 * Author: ncabalos
 *
 * Created on November 16, 2013, 2:44 PM
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef UNIT_TEST
#include <unity.h>
#include <unit_tests.h>
#endif

/*
 * 
 */
#ifndef UNIT_TEST
int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}
#else
int main(int argc, char** argv) {
    UnityBegin();
    UnityEnd();
    return (EXIT_SUCCESS);
}
#endif

