/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_COMMAND_TABLE_H
#define LAPEL_COMMAND_TABLE_H

#include "lapel_command_interface.h"

namespace Lapel {

    class CommandTable : public CommandInterface {

    public:

        struct Record {

            const char *name;
            Role role;
            Command::command_fn fn;
        };

        CommandTable(const Record *table, size_t size, void *ctx);

        Command lookup(const char *name);

        bool complete_partial_match(int argc, const char **argv, char *buffer, size_t max, size_t offset = 0);
        size_t count_partial_match(int argc, const char **argv);

    private:

        const Record *table;
        const size_t size;
        void *ctx;

        static Command convert_record(void *ctx, const Record &record);
    };
};

#endif
