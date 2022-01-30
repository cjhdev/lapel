/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_DEFAULT_HISTORY_H
#define LAPEL_DEFAULT_HISTORY_H

#include "lapel_history.h"

#include <stdint.h>

namespace Lapel {

    // heap based history store
    class DefaultHistory : public History {

    public:

        DefaultHistory(size_t max_buffer = 0, size_t max_line = 0);
        ~DefaultHistory();

        bool get(size_t index, char *buffer, size_t max);
        void push();
        void set(size_t index, const char *line);

    private:

        const size_t max_line;
        const size_t max_records;

        size_t size;
        size_t append_count;

        char *buffer;

        char *get_buffer(size_t index);
    };
};

#endif
