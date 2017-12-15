/* *****************************************************************************
 * Copyright (c) 2017 Dennis Wildmark
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * ****************************************************************************/

/*
 * Author(s): Dennis Wildmark <d.wildmark@gmail.com>
 */

#include <one_cli.h>
#include <stdio.h>
#include <string.h>

#define ONE_CLI_MAX_NOF_COMMANDS                255
#define ONE_CLI_MAX_COMMAND_LENGTH              40
#define ONE_CLI_MAX_NOF_PARAMETERS              20
#define ONE_CLI_MAX_PARAM_LEN                   20

static struct cli_command cmd_list[ONE_CLI_MAX_NOF_COMMANDS];

static int entry_count = 0;

int cli_register_command(struct cli_command *command)
{
    int i;

    for (i = 0; i < entry_count; i++) {
        if (!strncmp(command->command_string, cmd_list[i].command_string,
                     ONE_CLI_MAX_COMMAND_LENGTH)) {
            printf("COMMAND EXISTS\r\n");
            return -1;
        }

    }

    cmd_list[entry_count] = *command;

    entry_count++;

    printf("registered command\r\n");

    return 0;
}

void cli_dispatch_command(char *command_string, char *output_buffer,
                          size_t output_buffer_len)
{
    int i;
    size_t param_count;
    size_t cmd_len;
    char *cli_cmd;
    char *tmp;
    struct cli_command *cmd = NULL;

    param_count = 0;

    cli_cmd = strtok(command_string, " ");

    if (NULL == cli_cmd) {
        snprintf(output_buffer, output_buffer_len,
                 "oneCLI: Invalid command string\r\n");
        return;
    }

    cmd_len = strlen(cli_cmd);

    tmp = command_string;

    for (i = 0; i < ONE_CLI_MAX_NOF_PARAMETERS && NULL != tmp; i++) {
        tmp += cmd_len + 1;
        tmp = strtok(tmp, " ");

        if (NULL != tmp) {
            param_count++; 
            cmd_len = strlen(tmp);

            if (cmd_len > ONE_CLI_MAX_PARAM_LEN) {
                snprintf(output_buffer, output_buffer_len,
                         "oneCLI: Parameter length too long\r\n");
                return;
            }
        }
    }

    for (i = 0; i < entry_count; i++) {
        if (!strncmp(cli_cmd, cmd_list[i].command_string,
                     ONE_CLI_MAX_COMMAND_LENGTH)) {
            cmd = &cmd_list[i];
        }
    }

    if (NULL == cmd) { 
        snprintf(output_buffer, output_buffer_len,
                 "oneCLI didn't recognize command.\r\n");
        
        return;
    }


    if (cmd->exp_nof_params != param_count) {
        snprintf(output_buffer, output_buffer_len,
                 "Command expected %d parameters. "
                 "Usage: %s", cmd->exp_nof_params, cmd->help_string);
        
        return;
    }
    
    cmd->handler(command_string, output_buffer, output_buffer_len);
}

char *cli_get_parameter(char *param_string, size_t param_number)
{
    int i;
    size_t cmd_len;
    char *param;
    if (param_number > ONE_CLI_MAX_NOF_PARAMETERS)
        return NULL;

    if (NULL == param_string)
        return NULL;

    param = strtok(param_string, " ");

    cmd_len = strlen(param);

    for (i = 0; i < param_number && NULL != param; i++) {
        param += cmd_len + 1;
        param = strtok(param, " ");

        if (NULL != param)
            cmd_len = strlen(param);

    }

    return param;
}

