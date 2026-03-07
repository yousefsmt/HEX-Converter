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
    static char extracted_octal[12ul] = {'0'};
    static char extracted_hex[9ul]    = {'0'};
    switch (base)
    {
    case 8u:
    {
        const char     octal_refrence[] = "01234567";
        const uint64_t octal_mask       = 0x07UL;
        uint8_t        idx              = 0x0BU;
        uint8_t        check            = 0x00U;
        for (size_t i = 0; i < 32ul; i += 3)
        {
            check = (octal_mask & (value >> i));
            extracted_octal[idx] = octal_refrence[check];
            if (idx == 0)break;
            idx--;
        }
        // extracted_octal[12] = '\0';
        for (size_t i = 0; i < 12UL; i++)
        {
            if (extracted_octal[i] != '0')
            {
                idx = i;
                break;
            }
        }
        return &extracted_octal[idx];
    }
    case 16u:
    {
        const char     hex_refrence[]  = "0123456789ABCDEF";
        const uint64_t hex_mask        = 0x0fUL;
        uint8_t        idx             = 0x08U;
        uint8_t        check           = 0x00U;
        for (size_t i = 0; i < 32ul; i += 4)
        {
            check = (hex_mask & (value >> i));
            extracted_hex[idx] = hex_refrence[check];
            if (idx == 0)break;
            idx--;
        }
        // extracted_hex[9UL] = '\0';
        for (size_t i = 0; i < 9UL; i++)
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
    static char buf[bits_num(value) * CHAR_BIT + 1];
    for (uint32_t i = 0, mask = ((uint32_t)1 << (bits_num(value) - 1)); mask; mask >>= 1)
        buf[i++] = value & mask ? '1' : '0';
    buf[bits_num(value)] = '\0';
    return buf;
}

static int
convert_hex_number(converter_config* config) // Done
{
    uint64_t extracted_num = 0ul;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 11ul && strncmp(config->argv[i], "0x", 2ul) == 0)
        {
            extracted_num = strtol(config->argv[i], NULL, 16);
            if (extracted_num == 0ul){return -1;}
            printf("HEX: %s     DEC: %ld     OCT: 0o%s     BIN: 0b%s\n", config->argv[i], extracted_num, decimal_converter(extracted_num, 8U), bin_u32((uint32_t)extracted_num));
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
    uint64_t extracted_num = 0ul;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 35ul && strncmp(config->argv[i], "0b", 2ul) == 0)
        {
            config->argv[i] += 2;
            extracted_num = strtol(config->argv[i], NULL, 2);
            if (extracted_num == 0ul){return -1;}
            printf("HEX: %s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, 16U), extracted_num, decimal_converter(extracted_num, 8U), config->argv[i]);
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
    uint64_t extracted_num = 0ul;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 10ul && strncmp(config->argv[i], "0x", 2ul) == 0)
        {
            extracted_num = strtol(config->argv[i], NULL, 16);
            if (extracted_num == 0ul || extracted_num > 4294967295UL){return -1;}
            printf("HEX: %s     DEC: %ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, 16U), extracted_num, decimal_converter(extracted_num, 8U), bin_u32((uint32_t)extracted_num));
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
    uint64_t extracted_num = 0ul;

    for (size_t i = config->idx; i < config->argc; i++)
    {
        if (strlen(config->argv[i]) < 13ul && strncmp(config->argv[i], "0o", 2ul) == 0)
        {
            config->argv[i] += 2;
            extracted_num = strtol(config->argv[i], NULL, 8);
            if (extracted_num == 0ul){return -1;}
            printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, 16U), extracted_num, decimal_converter(extracted_num, 8U), bin_u32((uint32_t)extracted_num));
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
    int              option_index = 0;
    converter_config config       = {.argc = 0,
                                     .argv = NULL,
                                     .idx = 0
                                    };
    int              c            = 0;

    while (1) {
        option_index = 0;

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
            config.idx  = (strncmp(argv[optind-1], "-x", 2ul) == 0) ? (size_t)(optind) : (size_t)(optind-1);
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
            config.idx  = (strncmp(argv[optind-1], "-o", 2ul) == 0) ? (size_t)(optind) : (size_t)(optind-1);
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
            config.idx  = (strncmp(argv[optind-1], "-d", 2ul) == 0) ? (size_t)(optind) : (size_t)(optind-1);
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
            config.idx  = (strncmp(argv[optind-1], "-b", 2ul) == 0) ? (size_t)(optind) : (size_t)(optind-1);
            if (convert_binary_number(&config) == -1)
            {
                fprintf(stderr, "binary conversion failed.\n");
                return -1;
            }
            break;
        }
        case '?':
            help(argv[optind-1]);
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