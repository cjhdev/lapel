/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_encoder.h"
#include "lapel_tty.h"
#include "lapel_control_codes.h"

#include <stddef.h>

using namespace Lapel;

static const char Tag[] = "Encoder";

Encoder::Encoder(TTY &tty)
    :
    tty(tty)
{}

void
Encoder::put_uint(uint32_t n)
{
    char buf[10];

    uint32_t nn = n;
    size_t i;

    for(i=sizeof(buf); i; i--){

        buf[i-1] = '0' + (nn % 10);
        nn /= 10;
    }

    tty.print(buf, sizeof(buf)-i);
}

void
Encoder::put_cuf(uint32_t n)
{
    if(n > 0){

        tty.put_char(ESC);
        tty.put_char('[');

        if(n > 1){

            put_uint(n);
        }

        tty.put_char('C');
    }
}

void
Encoder::put_cuu(uint32_t n)
{
    if(n > 0){

        tty.put_char(ESC);
        tty.put_char('[');

        if(n > 1){

            put_uint(n);
        }

        tty.put_char('A');
    }
}

void
Encoder::put_cup(uint32_t n, uint32_t m)
{
    if((n > 0) && (m > 0)){

        tty.put_char(ESC);
        tty.put_char('[');

        if(n > 1){

            put_uint(n);
        }

        tty.put_char(';');

        if(m > 1){

            put_uint(n);
        }

        tty.put_char('H');
    }
}

void
Encoder::put_dsr()
{
    tty.put_char(ESC);
    tty.put_char('[');
    tty.put_char('6');
    tty.put_char('n');
}

void
Encoder::put_ed_all()
{
    tty.put_char(ESC);
    tty.put_char('[');
    tty.put_char('2');
    tty.put_char('J');
}

void
Encoder::put_ed_right()
{
    tty.put_char(ESC);
    tty.put_char('[');
    tty.put_char('J');
}

void
Encoder::put_hide_cursor()
{
    tty.put_char(ESC);
    tty.put_char('[');
    tty.put_char('?');
    tty.put_char('2');
    tty.put_char('5');
    tty.put_char('l');
}

void
Encoder::put_show_cursor()
{
    tty.put_char(ESC);
    tty.put_char('[');
    tty.put_char('?');
    tty.put_char('2');
    tty.put_char('5');
    tty.put_char('h');
}
