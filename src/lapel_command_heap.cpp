/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_command_heap.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "CommandHeap";

CommandHeap::CommandHeap()
    :
    head(nullptr)
{
}

CommandHeap::~CommandHeap()
{
    for(auto it = head; it;){

        volatile auto next = it->next;

        delete it->name;
        delete it;

        it = next;
    }

    head = nullptr;
}

Command
CommandHeap::lookup(const char *name)
{
    Command retval;

    for(auto it = head; it; it = it->next){

        if(strcmp(it->name, name) == 0){

            retval = convert_record(*it);
            break;
        }
    }

    return retval;
}

bool
CommandHeap::complete_partial_match(int argc, const char **argv, char *buffer, size_t max, size_t offset)
{
    bool retval = false;

    size_t n = 0;

    for(auto it = head; it; it = it->next){

        auto cmd = convert_record(*it);

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
CommandHeap::count_partial_match(int argc, const char **argv)
{
    size_t retval = 0;

    for(auto it = head; it; it = it->next){

        retval += convert_record(*it).partial_match(argc, argv) ? 1 : 0;
    }

    return retval;
}

void
CommandHeap::add_command(const char *name, Role role, void *ctx, Command::command_fn fn)
{
    if(!lookup(name)){

        Record *ptr = new Record;

        (void)memset(ptr, 0, sizeof(*ptr));

        ptr->next = head;

        ptr->name = new char[strlen(name)+1];

        (void)strcpy(ptr->name, name);

        ptr->role = role;
        ptr->ctx = ctx;
        ptr->fn = fn;

        head = ptr;
    }
}

void
CommandHeap::remove_command(const char *name)
{
    for(auto ptr = head, prev = head; ptr; prev = ptr, ptr = ptr->next){

        if(strcmp(ptr->name, name) == 0){

            if(prev != ptr){

                prev->next = ptr->next;
            }
            else{

                head = ptr->next;
            }

            delete ptr->name;
            delete ptr;

            break;
        }
    }
}

Command
CommandHeap::convert_record(const Record &record)
{
    return Command(record.ctx, record.fn, record.name, record.role);
}
