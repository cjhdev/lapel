/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_HISTORY_H
#define LAPEL_HISTORY_H

#include <stddef.h>

namespace Lapel {

    class History {

    public:

        virtual bool get(size_t index, char *buffer, size_t max) = 0;
        virtual void push() = 0;
        virtual void set(size_t index, const char *line) = 0;
    };
};


#endif
