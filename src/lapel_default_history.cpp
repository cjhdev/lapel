/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_default_history.h"
#include "lapel_debug.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "DefaultHistory";

DefaultHistory::DefaultHistory(size_t max_line, size_t max_records)
    :
    max_line(max_line),
    max_records(max_records),
    size(0),
    append_count(0),
    buffer(nullptr)
{
    if((max_records > 0) && (max_line > 0)){

        buffer = new char[max_line*max_records];
    }
}

DefaultHistory::~DefaultHistory()
{
    delete buffer;
}

bool
DefaultHistory::get(size_t index, char *buffer, size_t max)
{
    static const char *empty = "";
    bool retval = false;

    if(index < size){

        strncpy(buffer, get_buffer(append_count - index), max);

        retval = true;
    }
    else{

        if(index == 0){

            strncpy(buffer, empty, max);

            retval = true;
        }
    }

    return retval;
}

void
DefaultHistory::set(size_t index, const char *line)
{
    if(max_records > 0){

        if((size == 0) && (index == 0)){

            size = 1;
        }

        if(index < size){

            (void)strncpy(get_buffer(append_count - index), line, max_line - 1);
        }
    }
}

void
DefaultHistory::push()
{
    if(size > 0){

        // todo strip whitespace
        if((size == 1) || (strcmp(get_buffer(append_count - 1), get_buffer(append_count)) != 0)){

            if(strlen(get_buffer(append_count)) > 0){

                append_count++;

                if(size < max_records){

                    size++;
                }
            }
        }

        *get_buffer(append_count) = 0;
    }
}

char *
DefaultHistory::get_buffer(size_t index)
{
    return (buffer != nullptr) ? &buffer[(index % max_records) * max_line] : nullptr;
}
