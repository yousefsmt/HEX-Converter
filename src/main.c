#include <stdio.h>
#include <stdint.h>

#include "base_con.h"
#include "parser.h"
#include "log.h"

int main(int argc, char *argv[])
{
    struct config_t config;
    int err;

    init_config(&config);

    err = parse_num_args(&config, argc, argv);
    if (err < 0) {
        debug_msg("parse argument failed!");
        return -1;
    }

    num_dumps(&config);

    free_resource(&config);

    return 0;
}