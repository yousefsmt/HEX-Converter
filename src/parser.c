#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "parser.h"
#include "log.h"

static void help(void *pname) {
  printf("HEX Converter v0.1.2 Developed by: Yousef.smt\n\
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
        Display help message and exit.\n", (char*) pname);
  exit(0);
}


static int add_mem_arg(struct config_t *config, const uint32_t type,
                        const char *value, const size_t size)
{
    int err = 0;

    if (type > MAX_BASE_TYPE) {
        debug_msg("Requested type is out of range");
        return -1;
    }

    switch (type) {
    case HEX_NUMBER:
        err = list_add_node(&config->hex_nums, value, size);
        if (err == -1) {
            debug_msg("in adding a hex value to a linked list, something is wrong; 'value' and 'size' don't make sense");
            return -1;
        }
        break;
    case BIN_NUMBER:
        list_add_node(&config->bin_nums, value, size);
        if (err == -1) {
            debug_msg("in adding a binary value to a linked list, something is wrong; 'value' and 'size' don't make sense");
            return -1;
        }
        break;
    case OCT_NUMBER:
        list_add_node(&config->oct_nums, value, size);
        if (err == -1) {
            debug_msg("in adding a octal value to a linked list, something is wrong; 'value' and 'size' don't make sense");
            return -1;
        }
        break;
    case DEC_NUMBER:
        list_add_node(&config->dec_nums, value, size);
        if (err == -1) {
            debug_msg("in adding a decimal value to a linked list, something is wrong; 'value' and 'size' don't make sense");
            return -1;
        }
        break;
    default:
        return -1;
    }
    return 0;
}

static int add_number(struct config_t *config, const int idx,
                        const uint32_t type, const int argc, const char **argv)
{
    size_t len;

    for (int i = idx; i < argc; i++) {
        len = strlen(argv[i]);
        switch (type) {
        case HEX_NUMBER:
            if (len < 0x0bUL && strncmp(argv[i], "0x", 0x02UL) == 0x00) {
                add_mem_arg(config, HEX_NUMBER, argv[i], len);
            } else if (strncmp(argv[i], "-", 0x01UL) == 0x00) {
                return 0;
            } else {
                debug_msg("after the hex flag (-x or --hex), each number must contain a '0x' prefix.");
                return -1;
            }
            break;
        case BIN_NUMBER:
            if (len < 0x23UL && strncmp(argv[i], "0b", 0x02UL) == 0x00) {
                add_mem_arg(config, BIN_NUMBER, argv[i], len);
            } else if (strncmp(argv[i], "-", 0x01UL) == 0x00) {
                return 0;
            } else {
                debug_msg("after the binary flag (-b or --binary), each number must contain a '0b' prefix.");
                return -1;
            }
            break;
        case DEC_NUMBER:
            if (len < 0x0dUL && strncmp(argv[i], "0d", 0x02UL) == 0x00) {
                add_mem_arg(config, DEC_NUMBER, argv[i], len);
            } else if (strncmp(argv[i], "-", 0x01UL) == 0x00) {
                return 0;
            } else {
                debug_msg("after the decimal flag (-d or --decimal), each number must contain a '0d' prefix.");
                return -1;
            }
            break;
        case OCT_NUMBER:
            if (len < 0x0eUL && strncmp(argv[i], "0o", 0x02UL) == 0x00) {
                add_mem_arg(config, OCT_NUMBER, argv[i], len);
            } else if (strncmp(argv[i], "-", 0x01UL) == 0x00) {
                return 0;
            } else {
                debug_msg("after the octal flag (-o or --octal), each number must contain a '0o' prefix.");
                return -1;
            }
            break;
        }
    }

    return 0;
}

void init_config(struct config_t *config)
{
    list_init(&config->bin_nums);
    list_init(&config->hex_nums);
    list_init(&config->oct_nums);
    list_init(&config->dec_nums);
}

int parse_num_args(struct config_t *config,
                    int argc, char* argv[])
{
    int option_index = 0;
    int c = 0;
    int err;
    size_t idx;

    while (1) {
        option_index = 0x00;

        static struct option long_options[] = {
            {"hex",     required_argument, 0,  'x'},
            {"octal",   required_argument, 0,  'o'},
            {"decimal", required_argument, 0,  'd'},
            {"binary",  required_argument, 0,  'b'},
            {0,         0,                 0,   0 }
        };
        c = getopt_long(argc, argv, "xodb", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'x':
            idx  = (strncmp(argv[optind-0x01], "-x", 0x02UL) == 0x00)
                    ? (size_t)(optind) : (size_t)(optind-0x01);
            err = add_number(config, idx, HEX_NUMBER, argc, (const char **)argv);
            if (err == -1) {
                debug_msg("you set %d arguments in hex (-x or --hex) part something wrong", argc);
                return -1;
            }
            break;
        case 'o':
            idx = (strncmp(argv[optind-0x01], "-o", 0x02UL) == 0x00)
                    ? (size_t)(optind) : (size_t)(optind-0x01);
            err = add_number(config, idx, OCT_NUMBER, argc, (const char **)argv);
            if (err == -1) {
                debug_msg("you set %d arguments in octal (-o or --octal) part something wrong", argc);
                return -1;
            }
            break;
        case 'd':
            idx = (strncmp(argv[optind-0x01], "-d", 0x02UL) == 0x00)
                    ? (size_t)(optind) : (size_t)(optind-0x01);
            err = add_number(config, idx, DEC_NUMBER, argc, (const char **)argv);
            if (err == -1) {
                debug_msg("you set %d arguments in decimal (-d or --decimal) part something wrong", argc);
                return -1;
            }
            break;
        case 'b':
            idx = (strncmp(argv[optind-0x01], "-b", 0x02UL) == 0x00)
                    ? (size_t)(optind) : (size_t)(optind-0x01);
            err = add_number(config, idx, BIN_NUMBER, argc, (const char **)argv);
            if (err == -1) {
                debug_msg("you set %d arguments in binary (-b or --binary) part something wrong", argc);
                return -1;
            }
            break;
        case '?':
            help(argv[optind-0x01]);
            break;
        default:
            debug_msg("argument parser returned character code 0%o is NOT valid!!!", c);
        }
    }

    if (argc == 1){help("no args");}

    return 0;
}

void free_resource(struct config_t *config)
{
    list_free(&config->bin_nums);
    list_free(&config->hex_nums);
    list_free(&config->dec_nums);
    list_free(&config->oct_nums);
}