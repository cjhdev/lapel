/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_TTY_H
#define LAPEL_TTY_H

#include <stddef.h>
#include <stdarg.h>

namespace Lapel {

    class TTY {

    public:

        virtual ~TTY();

        /** Put a character
         *
         * @param[in] c
         *
         * */
        virtual void put_char(char c);

        /** Flush the output buffer (if there is one)
         *
         * */
        virtual void flush();

        /** Print an ASCII string
         *
         * @param[in] s ASCII
         * @param[in] n size of s
         *
         * */
        void print(const char *s, size_t n);

        /** Print a null-terminated ASCII string
         *
         * @param[in] s null-terminate ASCII
         *
         * */
        void print(const char *s);
    };

};

#endif
