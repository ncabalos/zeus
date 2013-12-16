#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "version.h"
#include "servo.h"

enum {
    COMMAND_PWM,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_MAX
};

#define MAX_BUFFER_SIZE 64
const char * commands[COMMAND_MAX] = {
    "pwm",
    "help",
    "version"
};

#define HELP_COMMAND_STRING_NUMBER 4
const char * help_strings[HELP_COMMAND_STRING_NUMBER] = {
    "List of commands:\r\n",
    "pwm     - pwm <channel> <duty cycle>\r\n",
    "help    - Displays this menu\r\n",
    "version - Displays firmware version number\r\n"
};

const char * invalid_string = "Command not found\r\n";
const char * version_string = VERSION_STRING;
const char * sha_string = SHA_STRING;

extern void terminal_output(const char * string);

static uint16_t execute_help_command(void);
static uint16_t execute_version_command(void);
static uint16_t execute_invalid_command(void);
static uint16_t execute_pwm_command(char * command);

uint16_t parse_command(char * command)
{
    uint16_t i;
    uint16_t result;
    char * chptr;
    char * parameters;
    chptr = strtok(command, " ");

    for(i = 0; i < COMMAND_MAX; i++) {
        if(strcmp(commands[i], chptr) == 0) {
            parameters = command + strlen(chptr) + 1;
            break;
        }
    }

    switch(i) {
        case COMMAND_HELP:
            result = execute_help_command();
            break;

        case COMMAND_VERSION:
            result = execute_version_command();
            break;

        case COMMAND_PWM:
            result = execute_pwm_command(parameters);
            break;

        default:
            execute_invalid_command();
            result = -1;
            break;
    }

    return result;
}

uint16_t execute_help_command(void)
{
    uint16_t i;

    for(i = 0; i < HELP_COMMAND_STRING_NUMBER; i++) {
        terminal_output(help_strings[i]);
    }

    return 0;
}

uint16_t execute_version_command(void)
{
    terminal_output(version_string);
    terminal_output("\r\n");
    return 0;
}

uint16_t execute_invalid_command(void)
{
    terminal_output(invalid_string);
    return 0;
}
char parser_string[32];

uint16_t is_number(char * str)
{
    char c;
    /* check first character */
    c = *str++;

    if(c != '-' && c != '+' && (c < '1' || c > '9'))
        return -1;

    while((c = *str++) != 0) {
        if(c < '0' || c > '9') {
            return -1;
        }
    }

    return 0;
}

bool convert_string_to_number(char * str, uint16_t * num)
{
    char * chptr = str;

    if (is_number(chptr) == 0) {
        *num = atoi(chptr);
        return true;
    } else {
        return false;
    }
}

uint16_t execute_pwm_command(char * command)
{
    uint16_t channel;
    uint16_t duty_cycle;
    uint16_t param_num;
    char * chptr;
    chptr = strtok(command, " ");
    param_num = 0;

    while (chptr != NULL) {
        switch (param_num) {
            case 0:
                if(convert_string_to_number(chptr, &channel) == false) {
                    sprintf(parser_string, "parameter 1 is not a number\r\n");
                    terminal_output(parser_string);
                    return -1;
                } else {
                    sprintf(parser_string, "channel: %d\r\n", channel);
                    terminal_output(parser_string);
                }

                break;

            case 1:
                if(convert_string_to_number(chptr, &duty_cycle) == false) {
                    sprintf(parser_string, "parameter 2 is not a number\r\n");
                    terminal_output(parser_string);
                    return -1;
                } else {
                    sprintf(parser_string, "duty: %d\r\n", duty_cycle);
                    terminal_output(parser_string);
                }

                break;
        }

        chptr = strtok(NULL, " ");
        param_num++;
    }
    terminal_output("Setting pwm\r\n");
    ServoSetPulseWidth(channel, duty_cycle);
    return 0;
}
