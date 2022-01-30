/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_command.h"
#include "lapel_user.h"

#include <string.h>

using namespace Lapel;

Command::Command(void *ctx, command_fn fn, const char *name, Role role)
    :
    fn(fn),
    ctx(ctx),
    name(name),
    role(role),
    valid(true)
{
}

Command::Command()
    :
    fn(nullptr),
    ctx(nullptr),
    name(nullptr),
    role(0),
    valid(false)
{
}

Command::operator bool() const
{
    return valid;
}

bool
Command::operator==(const char *name)
{
    return strcmp(this->name, name) == 0;
}

ReturnValue
Command::operator()(ShellWrapper &shell, int argc, char **argv)
{
    ReturnValue retval = RETURN_OK;

    if(fn != nullptr){

        retval = fn(shell, argc, argv);
    }

    return retval;
}

bool
Command::partial_match(int argc, const char **argv)
{
    return complete(argc, argv, nullptr, 0);
}

bool
Command::complete(int argc, const char **argv, char *buffer, size_t max)
{
    bool retval = false;

    if(argc == 1U){

        if(strncmp(name, argv[0], strlen(argv[0])) == 0){

            if(max > 0){

                (void)strncpy(buffer, name, max);
            }

            retval = true;
        }
    }

    return retval;
}

bool
Command::authorize(User &user)
{
    return ((user.get_role() & role) > 0);
}

void *
Command::get_ctx()
{
    return ctx;
}
