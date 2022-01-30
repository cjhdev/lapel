/* Copyright 2022 Cameron Harper
 *
 * */
#include "single_user.h"

using namespace ArduinoLapel;

SingleUser::SingleUser(
    HardwareSerial &serial,
    size_t max_line,
    size_t max_argv,
    size_t max_history
)
    :
    user("admin", Lapel::ROLE_ALL),
    tty(serial),
    history(max_line, max_history),
    shell(system, tty, history, max_line, max_argv)
{
    shell.oob_login(user, false);
    shell.set_user_commands(user_commands);
}

void
SingleUser::process()
{
    char c;

    tty.get_char(c) ? shell.process(c) : shell.process();
}

void
SingleUser::refresh()
{
    shell.refresh();
}

void
SingleUser::add_command(const char *name, Lapel::Role role, void *ctx, Lapel::Command::command_fn fn)
{
    user_commands.add_command(name, role, ctx, fn);
}

void
SingleUser::add_command(const char *name, void *ctx, Lapel::Command::command_fn fn)
{
    add_command(name, Lapel::ROLE_ALL, ctx, fn);
}
