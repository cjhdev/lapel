/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_TOKENIZER_H
#define LAPEL_TOKENIZER_H

#include <stddef.h>

namespace Lapel {

    class Tokenizer {

    public:

        Tokenizer(char *buffer);

        static size_t tokenize(char *buffer, char **argv, size_t max_argv);

        char *next_token();

    private:

        char *buffer;
        size_t end;

        size_t in_offset;
        size_t out_offset;

        char *arg;

        bool quote;
        bool escape;
        char quote_char;

        void put_char(char c);
        char get_char();
        char peek_char();
        void push_arg();
        char *pop_arg();
    };
};

#endif
