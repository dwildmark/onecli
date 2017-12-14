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

static int placehldr(char *param_string,
                     char *write_buffer,
                     size_t write_buffer_size)
{
    snprintf(write_buffer, write_buffer_size, "HEJSAN\r\n");

    return 0;
}

static int test_command_1(char *param_string,
                          char *write_buffer,
                          size_t write_buffer_size)
{
    char *param_1;

    param_1 = cli_get_parameter(param_string, 1);

    snprintf(write_buffer, write_buffer_size, "HEJSAN %s\r\n",
             param_1);

    return 0;
}

int main(void)
{
    char buff[1000];
    char outbuff[1000];
    printf("TEST\r\n");

    struct cli_command command[2] = {
        {
            .command_string = "help",
            .help_string = "hewlp hurr durr",
            .handler = placehldr,
            .exp_nof_params = 0,
        },
        {
            .command_string = "test",
            .help_string = "durr",
            .handler = test_command_1,
            .exp_nof_params = 1,
        },
    };

    cli_register_command(&command[0]);
    cli_register_command(&command[1]);
    while (1) {
        memset(outbuff, 0, 1000);
        fgets(buff, 1000, stdin);

        buff[strcspn(buff, "\r\n")] = 0;
        printf("%s:%d:%s Entered %s\r\n",
               __FILE__, __LINE__, __FUNCTION__, buff);

        cli_dispatch_command(buff, outbuff, 1000);

        printf("%s\r\n", outbuff);
    }
}

