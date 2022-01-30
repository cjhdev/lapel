/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_COMMAND_HEAP_H
#define LAPEL_COMMAND_HEAP_H

#include "lapel_command_interface.h"

namespace Lapel {


    class CommandHeap : public CommandInterface {

    public:

        CommandHeap();
        ~CommandHeap();

        void add_command(const char *name, Role role, void *ctx, Command::command_fn fn);
        void remove_command(const char *name);

        Command lookup(const char *name);

        bool complete_partial_match(int argc, const char **argv, char *buffer, size_t max, size_t offset = 0);
        size_t count_partial_match(int argc, const char **argv);

    private:

        struct Record {

            Record *next;

            char *name;
            void *ctx;
            Role role;
            Command::command_fn fn;
        };

        Record *head;

        static Command convert_record(const Record &record);
    };
};


#endif
