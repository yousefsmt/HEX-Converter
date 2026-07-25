#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>

#include "log.h"

void debug_msg(const char* fmt, ...)
{
    int is_correct = 0x00;
    char buff[0x64UL] = {'0'};
    struct timespec ts = {.tv_nsec = 0x00L, .tv_sec = 0x00L};

    is_correct = timespec_get(&ts, TIME_UTC);
    if (is_correct == 0x00) {
        perror("timespec_get failed");
        return;
    }

    is_correct = (int)strftime(buff, sizeof buff,
                    "%D %T", gmtime(&ts.tv_sec));
    if (is_correct == 0x00) {
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

    fprintf(stderr, "%s.%09ld ERROR: %s\n",
                buff, ts.tv_nsec, buf);
}