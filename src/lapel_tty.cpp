/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_tty.h"
#include "lapel_debug.h"

#include <string.h>

using namespace Lapel;

TTY::~TTY()
{
}

void
TTY::put_char(char c)
{
    (void)c;
}

void
TTY::flush()
{
}

void
TTY::print(const char *s, size_t n)
{
    for(size_t i=0; i < n; i++){

        put_char(s[i]);
    }
}

void
TTY::print(const char *s)
{
    for(size_t i=0; i < strlen(s); i++){

        put_char(s[i]);
    }
}

void _putchar(char c)
{
    (void)c;
}
