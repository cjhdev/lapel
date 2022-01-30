/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_ENCODER_H
#define LAPEL_ENCODER_H

#include <stdint.h>

namespace Lapel {

    class TTY;

    class Encoder {

    public:

        Encoder(TTY &tty);

        void clear_screen();

        void put_uint(uint32_t n);

        void put_ed_all();
        void put_ed_right();
        void put_dsr();
        void put_cuu(uint32_t n = 1);
        void put_cuf(uint32_t n = 1);
        void put_cup(uint32_t n = 1, uint32_t m = 1);

        void put_hide_cursor();
        void put_show_cursor();

    private:

        TTY &tty;

    };
};

#endif
