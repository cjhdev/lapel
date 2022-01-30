/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_BUILTIN_H
#define LAPEL_BUILTIN_H

#include "lapel_command_table.h"

namespace Lapel {

    class Builtin {

    public:

        static CommandTable& command();
    };
};

#endif
