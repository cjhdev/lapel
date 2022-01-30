/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_COMMAND_H
#define LAPEL_COMMAND_H

#include "lapel_role.h"

#include <stdbool.h>
#include <stddef.h>

namespace Lapel {

    class ShellWrapper;
    class User;

    enum ReturnValue {

        RETURN_OK,
        RETURN_ERROR,
        RETURN_YIELD
    };

    class Command {

    public:

        typedef ReturnValue (*command_fn)(ShellWrapper &shell, int argc, char **argv);

        Command(void *ctx, command_fn fn, const char *name, Role role);
        Command();

        ReturnValue operator()(ShellWrapper &shell, int argc, char **argv);

        explicit operator bool() const;

        bool operator==(const char *name);

        bool complete(int argc, const char **argv, char *buffer, size_t max);

        bool partial_match(int argc, const char **argv);

        bool authorize(User &user);

        void *get_ctx();

    private:

        command_fn fn;
        void *ctx;
        const char *name;
        Role role;
        bool valid;
    };
};


#endif
