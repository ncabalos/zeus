#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "version.h"

#define MAX_BUFFER_SIZE 64
#define NUMBER_OF_COMMANDS 2
const char * commands[NUMBER_OF_COMMANDS] = {
    "help",
    "version"
};

#define HELP_COMMAND_STRING_NUMBER 3
const char * help_strings[HELP_COMMAND_STRING_NUMBER] = {
    "List of commands:\r\n",
    "help    - Displays this menu\r\n",
    "version - Displays firmware version number\r\n"
};

const char * invalid_string = "Command not found\r\n";
const char * version_string = VERSION_STRING;
const char * sha_string = SHA_STRING;

extern void terminal_output(const char * string);

static void execute_help_command(void);
static void execute_version_command(void);
static void execute_invalid_command(void);

uint16_t parse_command(char * command)
{
    uint16_t i;

    for(i = 0; i < NUMBER_OF_COMMANDS; i++) {
        if(strcmp(commands[i], command) == 0) {
            break;
        }
    }

    switch(i) {
        case 0:
            execute_help_command();
            break;

        case 1:
            execute_version_command();
            break;

        default:
            execute_invalid_command();
            return -1;
    }

    return 0;
}

void execute_help_command(void)
{
    uint16_t i;

    for(i = 0; i < HELP_COMMAND_STRING_NUMBER; i++) {
        terminal_output(help_strings[i]);
    }
}

void execute_version_command(void)
{
    terminal_output(version_string);
    terminal_output("\r\n");
}

void execute_invalid_command(void)
{
    terminal_output(invalid_string);
}
