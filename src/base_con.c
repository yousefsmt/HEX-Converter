#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "linked_list.h"
#include "base_con.h"
#include "log.h"

#define bits_num(value) (sizeof(value) * CHAR_BIT)

static const char *bin_u32(uint32_t value)
{
    static char buf[bits_num(value) * CHAR_BIT + 1] = {0};
    for (uint32_t i = 0, mask =
        ((uint32_t)(1) << (bits_num(value) - 1)); mask; mask >>= 1) {
        buf[i++] = value & mask ? '1' : '0';
    }
    buf[bits_num(value)] = '\0';
    char* ret = strpbrk(buf, "1");
    if (ret == NULL) {
        debug_msg("bit conversion failed");
        return "0";
    } else {
        return ret;
    }
    return ret;
}

static const char* decimal_converter(uint64_t value, uint8_t base)
{
    static char extracted_octal[12] = {'0'};
    static char extracted_hex[9] = {'0'};
    const char octal_reference[] = "01234567";
    const char hex_reference[] = "0123456789ABCDEF";
    const uint64_t hex_mask = 0x0fUL;
    const uint64_t octal_mask = 0x07UL;
    size_t idx;
    uint64_t check;

    switch (base) {
    case 0x08U:
        idx = 11;
        check = 0;
        for (size_t i = 0; i < 32; i += 3) {
            check = (octal_mask & (value >> i));
            extracted_octal[idx] = octal_reference[check];
            if (idx == 0)
                break;
            idx--;
        }
        for (size_t i = 0; i < 12; i++) {
            if (extracted_octal[i] != '0') {
                idx = i;
                break;
            }
        }
        return &extracted_octal[idx];
    case 16:
        idx = 8;
        check = 0;
        for (size_t i = 0x00UL; i < 0x20UL; i += 0x04UL) {
            check = (hex_mask & (value >> i));
            extracted_hex[idx] = hex_reference[check];
            if (idx == 0x00UL)
                break;
            idx--;
        }
        for (size_t i = 0x00UL; i < 0x09UL; i++) {
            if (extracted_hex[i] != '0') {
                idx = i;
                break;
            }
        }
        return &extracted_hex[idx];
    default:
        debug_msg("base is %d not valid.", base);
        return "0";
    }
    return "";
}

static void hex_dump(const char *str)
{
    uint64_t extracted_num = 0;
    char* remain;

    extracted_num = (uint64_t)strtol(str, &remain, 0x10);
    if (extracted_num == 0x00UL
        || extracted_num > 4294967295UL || remain[0x00UL] > 0x00) {
        debug_msg("extract hex string to number failed because is zero or out of range.");
        return;
    }
    printf("HEX: %s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n", str, extracted_num,
            decimal_converter(extracted_num, 8), bin_u32((uint32_t)extracted_num));
}

static void oct_dump(const char *str)
{
    uint64_t extracted_num = 0;
    char* remain;

    extracted_num = (uint64_t)strtol(&str[2], &remain, 8);
    if (extracted_num == 0
        || extracted_num > 4294967295UL || remain[0] > 0) {
        debug_msg("extract octal string to number failed because is zero or out of range.");
        return;
    }
    printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n",
        decimal_converter(extracted_num, 16),
        extracted_num, decimal_converter(extracted_num, 8), bin_u32((uint32_t)extracted_num));
}

static void dec_dump(const char *str)
{
    uint64_t extracted_num = 0;
    char* remain;

    extracted_num = (uint64_t)strtol(&str[2], &remain, 10);
    if (extracted_num == 0
        || extracted_num > 4294967295UL || remain[0] > 0) {
        debug_msg("extract decimal string to number failed because is zero or out of range.");
        return;
    }
    printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n",
            decimal_converter(extracted_num, 16),
            extracted_num, decimal_converter(extracted_num, 8), bin_u32((uint32_t)extracted_num));
}

static void bin_dump(const char *str)
{
    uint64_t extracted_num = 0;
    char* remain;
    extracted_num = (uint64_t)strtol(&str[2], &remain, 2);
    if (extracted_num == 0
        || extracted_num > 4294967295UL || remain[0] > 0x00) {
        debug_msg("extract binary string to number failed because is zero or out of range.");
        return;
    }
    printf("HEX: 0x%s     DEC: 0d%ld     OCT: 0o%s     BIN: 0b%s\n",
        decimal_converter(extracted_num, 16),
        extracted_num, decimal_converter(extracted_num, 8), str);
}

void num_dumps(struct config_t *config)
{
    list_trav(&config->hex_nums, hex_dump);
    list_trav(&config->oct_nums, oct_dump);
    list_trav(&config->dec_nums, dec_dump);
    list_trav(&config->bin_nums, bin_dump);
}
