/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_command_table.h"
#include "lapel_debug.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "CommandTable";

CommandTable::CommandTable(const Record *table, size_t size, void *ctx)
    :
    table(table),
    size(size),
    ctx(ctx)
{
}

Command
CommandTable::lookup(const char *name)
{
    Command retval;

    for(size_t i=0; i < size; i++){

        if(strcmp(table[i].name, name) == 0){

            retval = convert_record(ctx, table[i]);
            break;
        }
    }

    return retval;
}

bool
CommandTable::complete_partial_match(int argc, const char **argv, char *buffer, size_t max, size_t offset)
{
    bool retval = false;

    size_t n = 0;

    for(size_t i = 0; i < size; i++){

        auto cmd = convert_record(ctx, table[i]);

        if(cmd.partial_match(argc, argv)){

            if(offset == n){

                retval = cmd.complete(argc, argv, buffer, max);
            }
            else{

                n++;
            }
        }
    }

    return retval;
}

size_t
CommandTable::count_partial_match(int argc, const char **argv)
{
    size_t retval = 0;

    for(size_t i = 0; i < size; i++){

        retval += convert_record(ctx, table[i]).partial_match(argc, argv) ? 1 : 0;
    }

    return retval;
}

Command
CommandTable::convert_record(void *ctx, const Record &record)
{
    return Command(ctx, record.fn, record.name, record.role);
}
