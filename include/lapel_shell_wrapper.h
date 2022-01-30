/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_SHELL_WRAPPER_H
#define LAPEL_SHELL_WRAPPER_H

#include "lapel_shell.h"
#include "lapel_decoder.h"

namespace Lapel {

    class ShellWrapper {

    public:

        ShellWrapper(Shell &shell)
            :
            shell(shell)
        {
        }

        void logout()
        {
            shell.cmd_logout();
        }

        ReturnValue dsr_resize()
        {
            return shell.cmd_resize();
        }

        void set_size(uint32_t rows, uint32_t cols)
        {
            shell.set_size(rows, cols);
        }

        void clear()
        {
            shell.cmd_clear();
        }

        const char *get_version()
        {
            return shell.get_version();
        }

        bool get_input(DecoderSymbol &symbol)
        {
            return shell.cmd_get_input(symbol);
        }

        void print(const char *s)
        {
            shell.cmd_print(s);
        }

        void println(const char *s)
        {
            shell.cmd_print(s);
            shell.cmd_print("\r\n");
        }

        bool is_yielding()
        {
            return shell.state == Shell::STATE_YIELD;
        }

        void *get_ctx()
        {
            return shell.command.get_ctx();
        }

    private:

        Shell &shell;
    };
};

#endif
