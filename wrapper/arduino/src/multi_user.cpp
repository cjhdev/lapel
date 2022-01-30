/* Copyright 2022 Cameron Harper
 *
 * */
#include "multi_user.h"

using namespace ArduinoLapel;

MultiUser::MultiUser(
    HardwareSerial &serial,
    size_t max_line,
    size_t max_argv,
    size_t max_history
)
    :
    system(user_list),
    tty(serial),
    history(max_line, max_history),
    user_list(system, 5),
    shell(system, tty, history, max_line, max_argv)
{
    shell.set_user_commands(user_commands);
}

void
MultiUser::process()
{
    char c;

    tty.get_char(c) ? shell.process(c) : shell.process();
}

void
MultiUser::refresh()
{
    shell.refresh();
}

bool
MultiUser::add_user(const char *name, const char *password, Lapel::Role role)
{
    return user_list.add_user(name, password, role);
}

void
MultiUser::remove_user(const char *name)
{
    user_list.remove_user(name);
}

void
MultiUser::add_command(const char *name, Lapel::Role role, void *ctx, Lapel::Command::command_fn fn)
{
    user_commands.add_command(name, role, ctx, fn);
}

void
MultiUser::add_command(const char *name, void *ctx, Lapel::Command::command_fn fn)
{
    add_command(name, Lapel::ROLE_ALL, ctx, fn);
}
