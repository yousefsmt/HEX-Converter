#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H

#include "linked_list.h"

enum base_type_t
{
    __UNSPECIFIED,
    HEX_NUMBER,
    OCT_NUMBER,
    DEC_NUMBER,
    BIN_NUMBER,
    __MAX_BASE_TYPE_T
};

#define MAX_BASE_TYPE (__MAX_BASE_TYPE_T - 1)

struct config_t
{
    struct list_t hex_nums;
    struct list_t oct_nums;
    struct list_t dec_nums;
    struct list_t bin_nums;
};

void init_config(struct config_t *config);
int parse_num_args(struct config_t *config,
                    int argc, char* argv[]);
void free_resource(struct config_t *config);

#endif /* INCLUDE_PARSER_H */