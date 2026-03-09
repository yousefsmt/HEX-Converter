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

#define bits_num(value) (sizeof(value) * CHAR_BIT)
 
typedef struct
{
    size_t       argc;
    const char** argv;
    size_t       idx;
}converter_config;

void debug_msg(const char* fmt, ...);
int  parse_args(int argc, char* argv[]);

#endif // !HANDLER_H