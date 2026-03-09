#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdbit.h>

#include "handler.h"

static void
help(void *pname) {
  printf("HEX Converter v0.1.0 Developed by: Yousef.smt\n\
   Usage: %s [OPTION [VALUE]]\n\
  -x, --hex\n\
        Set hex number. [0x00000000-0xFFFFFFFF]\n\
  -o, --octal\n\
        Set octal number. [0o00000000000-0o37777777777]\n\
  -d, --decimal \n\
        Set decimal number. [0-4294967295]\n\
  -b, --binary \n\
        Set binary number. [0b0...00-0b1...11 max 32 Bit]\n\
  -h, --help\n\
        Display help message and exit.\n\
  --license\n\
        Display the license information.\n", (char*) pname);
  exit(0);
}

static const char*
decimal_converter(uint64_t value, uint8_t base)
{
    static char extracted_octal[0x0cUL] = {'0'};
    static char extracted_hex[0x09UL]    = {'0'};
    switch (base)
    {
    case 0x08U:
    {
        const char     octal_refrence[] = "01234567";
        const uint64_t octal_mask       = 0x07UL;
        size_t         idx              = 0x0BU;
        uint64_t       check            = 0x00U;
        for (size_t i = 0x00UL; i < 0x20UL; i += 0x03UL)
        {
            check = (octal_mask & (value >> i));
            extracted_octal[idx] = octal_refrence[check];
            if (idx == 0x00UL)break;
            idx--;
        }
        // extracted_octal[12] = '\0';
        for (size_t i = 0x00UL; i < 0x0cUL; i++)
        {
            if (extracted_octal[i] != '0')
            {
                idx = i;
                break;
            }
        }
        return &extracted_octal[idx];
    }
    case 0x10U:
    {
        const char     hex_refrence[]  = "0123456789ABCDEF";
        const uint64_t hex_mask        = 0x0fUL;
        size_t         idx             = 0x08U;
        uint64_t       check           = 0x00U;
        for (size_t i = 0x00UL; i < 0x20UL; i += 0x04UL)
        {
            check = (hex_mask & (value >> i));
            extracted_hex[idx] = hex_refrence[check];
            if (idx == 0x00UL)break;
            idx--;
        }
        // extracted_hex[9UL] = '\0';
        for (size_t i = 0x00UL; i < 0x09UL; i++)
        {
            if (extracted_hex[i] != '0')
            {
                idx = i;
                break;
            }
        }
        return &extracted_hex[idx];
    }
    default:
    {
        fprintf(stderr, "base not valid.\n");
        return "0";
    }
    }
    return "";
}

static const char*
bin_u32(uint32_t value)
{
    static char buf[bits_num(value) * CHAR_BIT + 0x01];
    for (uint32_t i = 0x00U, mask = ((uint32_t)(0x01U) << (bits_num(value) - 0x01)); mask; mask >>= 0x01)
        buf[i++] = value & mask ? '1' : '0';
    buf[bits_num(value)] = '\0';
    return buf;
}

static int
convert_hex_number(converter_config* config) // Done
{
    uint64_t extracted_num = 0x00UL;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 0x0bUL && strncmp(config->argv[i], "0x", 0x02UL) == 0x00)
        {
            extracted_num = (uint64_t)strtol(config->argv[i], NULL, 0x10);
            if (extracted_num == 0x00UL){return -1;}
            printf("HEX: %s     DEC: %ld     OCT: 0o%s     BIN: 0b%s\n", config->argv[i], extracted_num, decimal_converter(extracted_num, 0x08U), bin_u32((uint32_t)extracted_num));
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

static int
convert_binary_number(converter_config* config)
{
    uint64_t extracted_num = 0x00UL;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 0x23UL && strncmp(config->argv[i], "0b", 0x02UL) == 0x00)
        {
            config->argv[i] += 0x02;
            extracted_num = (uint64_t)strtol(config->argv[i], NULL, 0x02);
            if (extracted_num == 0x00UL){return -1;}
            printf("HEX: %s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, 0x10U), extracted_num, decimal_converter(extracted_num, 0x08U), config->argv[i]);
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

static int
convert_decimal_number(converter_config* config)
{
    uint64_t extracted_num = 0x00UL;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 0x0aUL && strncmp(config->argv[i], "0x", 0x02UL) == 0x00)
        {
            extracted_num = (uint64_t)strtol(config->argv[i], NULL, 0x10);
            if (extracted_num == 0x00UL || extracted_num > 4294967295UL){return -1;}
            printf("HEX: %s     DEC: %ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, 0x10U), extracted_num, decimal_converter(extracted_num, 0x08U), bin_u32((uint32_t)extracted_num));
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

static int
convert_octal_number(converter_config* config)
{
    uint64_t extracted_num = 0x00UL;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 0x0dUL && strncmp(config->argv[i], "0o", 0x02UL) == 0x00)
        {
            config->argv[i] += 0x02;
            extracted_num = (uint64_t)strtol(config->argv[i], NULL, 0x08);
            if (extracted_num == 0x00UL){return -1;}
            printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, 0x10U), extracted_num, decimal_converter(extracted_num, 0x08U), bin_u32((uint32_t)extracted_num));
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

int
parse_args(int argc, char* argv[])
{
    int              option_index = 0x00;
    converter_config config       = {.argc = 0x00UL,
                                     .argv = NULL,
                                     .idx = 0x00UL
                                    };
    int              c            = 0x00;

    while (1) {
        option_index = 0x00;

        static struct option long_options[] = {
            {"hex",     required_argument, 0,  'x'},
            {"octal",   required_argument, 0,  'o'},
            {"decimal", required_argument, 0,  'd'},
            {"binary",  required_argument, 0,  'b'},
            {0,         0,                 0,   0 }
        };
        c = getopt_long(argc, argv, "x", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'x':
        {
            config.argc = (size_t)(argc);
            config.argv = (const char**)argv;
            config.idx  = (strncmp(argv[optind-0x01], "-x", 0x02UL) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_hex_number(&config) == -1)
            {
                fprintf(stderr, "hex conversion failed.\n");
                return -1;
            }
            break;
        }
        case 'o':
        {
            config.argc = (size_t)(argc);
            config.argv = (const char**)argv;
            config.idx  = (strncmp(argv[optind-0x01], "-o", 0x02UL) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_octal_number(&config) == -1)
            {
                fprintf(stderr, "octal conversion failed.\n");
                return -1;
            }
            break;
        }
        case 'd':
        {
            config.argc = (size_t)(argc);
            config.argv = (const char**)argv;
            config.idx  = (strncmp(argv[optind-0x01], "-d", 0x02UL) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_decimal_number(&config) == -1)
            {
                fprintf(stderr, "hex conversion failed.\n");
                return -1;
            }
            break;
        }
        case 'b':
        {
            config.argc = (size_t)(argc);
            config.argv = (const char**)argv;
            config.idx  = (strncmp(argv[optind-0x01], "-b", 0x02UL) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_binary_number(&config) == -1)
            {
                fprintf(stderr, "binary conversion failed.\n");
                return -1;
            }
            break;
        }
        case '?':
            help(argv[optind-0x01]);
            break;
        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    // if (optind < argc)
    // {
    //     help(argv[argc - optind]);
    // }

    return 0;
}