/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_GET_OPT_H
#define LAPEL_GET_OPT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace Lapel {

    class GetOpt {

    public:

        enum Arg {

            NONE,
            OPTIONAL,
            REQUIRED
        };

        struct Option {

            const char *s;
            char c;
            Arg arg;
        };

        enum Result {

            OK,       // parsed option (get_arg() returns argument if present)
            UNKNOWN,  // parsed unknown option
            MISSING,  // missing argument
            END       // no more options
        };

        GetOpt(unsigned argc, const char **argv, const Option *options, size_t size);

        bool next();

        // current index (for restarting parsing)
        size_t get_index();

        // current argument
        const char *get_arg();

        // current result
        Result get_result();

        // current option (same as returned by get_next())
        const Option *get_option();

    private:

        const Option *options;
        size_t size;

        unsigned argc;
        const char **argv;

        size_t index;
        size_t offset;

        const char *current_arg;
        const Option *current_opt;

        enum State {

            STATE_IDLE,
            STATE_CHAIN,    // parsing chain of short options
            STATE_FINISHED
        };

        State state;

        const Option *lookup_short_option(char short_name);
        const Option *lookup_long_option(const char *long_name);
        size_t long_size(const char *token);

        State parse_long_option(const char *token);
        State parse_short_option(const char *token);

        bool is_last_arg(const char *token);
    };
};

#endif
