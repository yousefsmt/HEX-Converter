#ifndef HANDLER_H
#define HANDLER_H

#include <stdio.h>
#include <limits.h>
#include <stdint.h>

#define bits_num(value) (sizeof(value) * CHAR_BIT)

typedef struct
{
    size_t       argc;
    const char** argv;
    size_t       idx;
}converter_config;

int parse_args(int argc, char* argv[]);

#endif // !HANDLER_H