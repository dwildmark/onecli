#include <one_cli.h>
#include <stdio.h>
#include <string.h>

static int placehldr(const char *param_string,
                     char *write_buffer,
                     size_t write_buffer_size)
{
    (void)param_string;
    snprintf(write_buffer, write_buffer_size, "HEJSAN\r\n");

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
            .handler = placehldr,
            .exp_nof_params = 0,
        },
    };

    cli_register_command(&command[0]);
    cli_register_command(&command[1]);
    while (1) {
        memset(outbuff, 0, 1000);
        fgets(buff, 1000, stdin);

        buff[strcspn(buff, "\r\n")] = 0;
        printf("%s:%s:%d Entered %s\r\n", __FILE__, __FUNCTION__, __LINE__, buff);

        cli_dispatch_command(buff, outbuff, 1000);

        printf("%s\r\n", outbuff);
    }
}

