#ifndef HANDLER_H
#define HANDLER_H

/*
 * HEx Converter 
 * Copyright (C) 2026 Yousef.smt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <limits.h>

#define SUCCESS (0x00) /*!< Success state mean everything that's ok */
#define ERROR   (-1)   /*!< Error state mean something wrong! */

#define CLOCK_BUFFER_SIZE (0x64UL) /*!< Size for store clock and date */
#define OCTAL_BUFFER_SIZE (0x0cUL) /*!< Size for store octal string converted */
#define HEX_BUFFER_SIZE   (0x09UL) /*!< Size for store hex string converted */

#define OCTAL_MASK (0x07UL) /*!< This mask use for extract octal base */
#define HEX_MASK   (0x0fUL) /*!< This mask use for extract hex base */

#define OCTAL_BASE (0x08U) /*!< Octal base number */
#define HEX_BASE   (0x10U) /*!< Hex base number */

#define FLAG_LENGTH (0x02UL) /*!< This check for each flag does true */

#define MAX_HEX_LENGTH  (0x0bUL)       /*!< Each hex string must be below length */
#define MAX_DECIMAL_NUM (4294967295UL) /*!< Max decimal number can converted (32Bit) */

#define bits_num(value) (sizeof(value) * CHAR_BIT)

typedef struct timespec timespec;

/* config for each converter function */
typedef struct
{
    size_t argc;
    char** argv;
    size_t idx;
}converter_config;

/**
 * @brief Debug information
 * @param fmt formatted message for print debug info
 * @return none
 * This function generates debug message for better mechanism bug fix.
 */
void debug_msg(const char* fmt, ...);

/**
 * @brief Argument Parser
 * @param argc number of argument init to program
 * @param argv all args store to string array
 * @return none
 * This function give argc and argv then parse each base number and print it.
 */
int  parse_args(int argc, char* argv[]);

#endif // !HANDLER_H