#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdbit.h>
#include <time.h>
#include <stdarg.h>

#include "handler.h"

void
debug_msg(const char* fmt, ...)
{
    char     buff[CLOCK_BUFFER_SIZE] = {'0'};
    timespec ts                      = {.tv_nsec = 0x00L, .tv_sec = 0x00L};
    int      is_correct              = 0x00;

    is_correct = timespec_get(&ts, TIME_UTC);
    if (is_correct == 0x00)
    {
        perror("timespec_get failed");
        return;
    }

    
    is_correct = (int)strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
    if (is_correct == 0x00)
    {
        perror("strftime failed");
        return;
    }

    va_list args1;
    va_start(args1, fmt);
    va_list args2;
    va_copy(args2, args1);
    char buf[0x01+vsnprintf(NULL, 0x00UL, fmt, args1)];
    va_end(args1);
    vsnprintf(buf, sizeof buf, fmt, args2);
    va_end(args2);
 
    fprintf(stderr, "%s.%09ld ERROR: %s\n", buff, ts.tv_nsec, buf);

}

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
        Display help message and exit.\n", (char*) pname);
  exit(0x00);
}

static const char*
decimal_converter(uint64_t value, uint8_t base)
{
    static char extracted_octal[OCTAL_BUFFER_SIZE] = {'0'};
    static char extracted_hex[HEX_BUFFER_SIZE]     = {'0'};
    uint64_t    check                              = 0x00U;
    switch (base)
    {
    case OCTAL_BASE:
    {
        const char octal_reference[] = "01234567";
        size_t     idx               = 0x0BU;
        for (size_t i = 0x00UL; i < 0x20UL; i += 0x03UL)
        {
            check = (OCTAL_MASK & (value >> i));
            extracted_octal[idx] = octal_reference[check];
            if (idx == 0x00UL)break;
            idx--;
        }
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
    case HEX_BASE:
    {
        const char hex_reference[] = "0123456789ABCDEF";
        size_t     idx             = 0x08U;
        for (size_t i = 0x00UL; i < 0x20UL; i += 0x04UL)
        {
            check = (HEX_MASK & (value >> i));
            extracted_hex[idx] = hex_reference[check];
            if (idx == 0x00UL)break;
            idx--;
        }
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
        debug_msg("base is %d not valid.", base);
        return "0";
    }
    }
    return "";
}

static const char*
bin_u32(uint32_t value)
{
    static char buf[bits_num(value) * CHAR_BIT + 0x01] = {0};
    for (uint32_t i = 0x00U, mask = ((uint32_t)(0x01U) << (bits_num(value) - 0x01)); mask; mask >>= 0x01)
    {
        buf[i++] = value & mask ? '1' : '0';
    }
    buf[bits_num(value)] = '\0';

    char* ret = strpbrk(buf, "1");
    if (ret == NULL)
    {
        debug_msg("bit conversion failed");
        return "0";
    }
    else
    {
        return ret;
    }
}

static int
convert_hex_number(converter_config* config)
{
    uint64_t extracted_num = 0x00UL;
    char*    remain        = NULL;

    for (; config->idx < config->argc; config->idx++)
    {
        if (strlen(config->argv[config->idx]) < MAX_HEX_LENGTH && strncmp(config->argv[config->idx], "0x", FLAG_LENGTH) == 0x00)
        {
            extracted_num = (uint64_t)strtol(config->argv[config->idx], &remain, HEX_BASE);
            if (extracted_num == 0x00UL || extracted_num > MAX_DECIMAL_NUM || remain[0x00UL] > 0x00)
            {
                debug_msg("extract hex string to number failed because is zero or out of range.");
                return ERROR;
            }
            // TODO: add constant distance between each base for each input number with different num count
            printf("HEX: %s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", config->argv[config->idx], extracted_num, decimal_converter(extracted_num, OCTAL_BASE), bin_u32((uint32_t)extracted_num));
        }
        else if (strncmp(config->argv[config->idx], "-", 0x01UL) == 0x00)
        {
            break;
        }
        else
        {
            debug_msg("this arg '%s' not valid, you must use '0x' preffix.", config->argv[config->idx]);
            return ERROR;
        }
    }
    return 0;
}

static int
convert_binary_number(converter_config* config)
{
    uint64_t extracted_num = 0x00UL;
    char*    remain        = NULL;

    for (; config->idx < config->argc; config->idx++)
    {
        if (strlen(config->argv[config->idx]) < 0x23UL && strncmp(config->argv[config->idx], "0b", FLAG_LENGTH) == 0x00)
        {
            config->argv[config->idx] += 0x02;
            extracted_num = (uint64_t)strtol(config->argv[config->idx], &remain, 0x02);
            if (extracted_num == 0x00UL || extracted_num > 4294967295UL || remain[0x00UL] > 0x00)
            {
                debug_msg("extract binary string to number failed because is zero or out of range.");
                return ERROR;
            }
            printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, HEX_BASE), extracted_num, decimal_converter(extracted_num, OCTAL_BASE), config->argv[config->idx]);
        }
        else if (strncmp(config->argv[config->idx], "-", 0x01UL) == 0x00)
        {
            break;
        }
        else
        {
            debug_msg("this arg '%s' not valid, you must use '0b' preffix.", config->argv[config->idx]);
            return ERROR;
        }
    }
    return 0;
}

static int
convert_decimal_number(converter_config* config)
{
    uint64_t extracted_num = 0x00UL;
    char*    remain        = NULL;

    for (; config->idx < config->argc; config->idx++)
    {
        if (strlen(config->argv[config->idx]) < 0x0dUL && strncmp(config->argv[config->idx], "0d", FLAG_LENGTH) == 0x00)
        {
            config->argv[config->idx] += 2;
            extracted_num = (uint64_t)strtol(config->argv[config->idx], &remain, 0x0a);
            if (extracted_num == 0x00UL || extracted_num > 4294967295UL || remain[0x00UL] > 0x00)
            {
                debug_msg("extract decimal string to number failed because is zero or out of range.");
                return ERROR;
            }
            printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, HEX_BASE), extracted_num, decimal_converter(extracted_num, OCTAL_BASE), bin_u32((uint32_t)extracted_num));
        }
        else if (strncmp(config->argv[config->idx], "-", 0x01UL) == 0x00)
        {
            break;
        }
        else
        {
            debug_msg("this arg '%s' not valid, you must use '0d' preffix.", config->argv[config->idx]);
            return ERROR;
        }
    }
    return 0;
}

static int
convert_octal_number(converter_config* config)
{
    uint64_t extracted_num = 0x00UL;
    char*    remain        = NULL;

    for (; config->idx < config->argc; config->idx++)
    {
        if (strlen(config->argv[config->idx]) < 0x0eUL && strncmp(config->argv[config->idx], "0o", FLAG_LENGTH) == 0x00)
        {
            config->argv[config->idx] += 0x02;
            extracted_num = (uint64_t)strtol(config->argv[config->idx], &remain, OCTAL_BASE);
            if (extracted_num == 0x00UL || extracted_num > 4294967295UL || remain[0x00UL] > 0x00)
            {
                debug_msg("extract octal string to number failed because is zero or out of range.");
                return ERROR;
            }
            printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", decimal_converter(extracted_num, HEX_BASE), extracted_num, decimal_converter(extracted_num, OCTAL_BASE), bin_u32((uint32_t)extracted_num));
        }
        else if (strncmp(config->argv[config->idx], "-", 0x01UL) == 0x00)
        {
            break;
        }
        else
        {
            debug_msg("this arg '%s' not valid, you must use '0o' preffix.", config->argv[config->idx]);
            return ERROR;
        }
    }
    return 0;
}

int
parse_args(int argc, char* argv[])
{
    int              option_index = 0x00;
    converter_config config       = {.argc = (size_t)(argc),
                                     .argv = argv,
                                     .idx  = 0x00UL
                                    };
    int              c            = 0x00;

    while (true)
    {
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
        {
            config.idx  = (strncmp(argv[optind-0x01], "-x", FLAG_LENGTH) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_hex_number(&config) == -1)
            {
                debug_msg("hex conversion failed.");
                return ERROR;
            }
            break;
        }
        case 'o':
        {
            config.idx  = (strncmp(argv[optind-0x01], "-o", FLAG_LENGTH) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_octal_number(&config) == -1)
            {
                debug_msg("octal conversion failed.");
                return ERROR;
            }
            break;
        }
        case 'd':
        {
            config.idx  = (strncmp(argv[optind-0x01], "-d", FLAG_LENGTH) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_decimal_number(&config) == -1)
            {
                debug_msg("decimal conversion failed.");
                return ERROR;
            }
            break;
        }
        case 'b':
        {
            config.idx  = (strncmp(argv[optind-0x01], "-b", FLAG_LENGTH) == 0x00) ? (size_t)(optind) : (size_t)(optind-0x01);
            if (convert_binary_number(&config) == -1)
            {
                debug_msg("binary conversion failed.");
                return ERROR;
            }
            break;
        }
        case '?':
            help(argv[optind-0x01]);
            break;
        default:
            debug_msg("argument parser returned character code 0%o is NOT valid!!!", c);
        }
    }

    if (argc == 1){help("no args");}

    return SUCCESS;
}