/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_COMMAND_INTERFACE_H
#define LAPEL_COMMAND_INTERFACE_H

#include <stdbool.h>
#include <stddef.h>

#include "lapel_command.h"

namespace Lapel {

    class CommandInterface {

    public:

        virtual ~CommandInterface()
        {}

        virtual Command lookup(const char *name) = 0;
        virtual bool complete_partial_match(int argc, const char **argv, char *buffer, size_t max, size_t offset = 0) = 0;
        virtual size_t count_partial_match(int argc, const char **argv) = 0;
    };
};

#endif
