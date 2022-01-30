/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_SHELL_H
#define LAPEL_SHELL_H

#include "lapel_builtin.h"
#include "lapel_user.h"
#include "lapel_decoder.h"
#include "lapel_tty.h"
#include "lapel_system.h"
#include "lapel_line.h"

namespace Lapel {

    class CommandInterface;
    class History;
    class UserList;

    class Shell {

        friend class ShellWrapper;

    public:

        static const uint32_t default_rows = 20;
        static const uint32_t default_cols = 120;
        static const uint32_t default_argv = 15;

        enum State {

            STATE_USERNAME,
            STATE_PASSWORD,
            STATE_FAIL,
            STATE_LINE,
            STATE_YIELD
        };

        Shell(
            System &system,
            TTY &tty,
            History &history,
            size_t max_line = default_cols,
            size_t max_argv = default_argv
        );

        virtual ~Shell();

        void refresh();

        void process(char c);
        void process();

        const char *get_version();

        void set_size(uint32_t rows, uint32_t cols);

        void set_user_commands(CommandInterface &commands);

        bool add_user(const char *name, const char *password, Role role);
        void remove_user(const char *name);

        void oob_login(const User &user, bool logout_allowed);
        void oob_logout();

    private:

        System &system;
        TTY &tty;
        History &history;

        // [user, builtin]
        CommandInterface *commands[2];
        Line line;
        size_t history_index;

        size_t max_line;
        size_t max_argv;

        char *buffer;
        char **argv;

        size_t argc;

        State state;

        User user;

        Command command;

        bool logout_permitted;

        char prompt[40];

        bool refresh_pending;
        bool logout_pending;

        Decoder decoder;

        bool input_semaphore;
        DecoderSymbol input_symbol;

        uint32_t rows;
        uint32_t cols;

        unsigned history_id;

        enum ResizeState {

            RESIZE_STATE_IDLE,
            RESIZE_STATE_RX
        };

        ResizeState resize_state;

        void put_username();

        void clear_screen();

        void notify();

        State process_username();
        State process_password();
        State process_fail();
        State process_line();
        State process_yield();

        ReturnValue cmd_resize();
        void cmd_clear();
        void cmd_logout();
        void cmd_refresh();
        void cmd_print(const char *s);
        void cmd_flush();
        bool cmd_get_input(DecoderSymbol &symbol);

        DecoderSymbol *pop_symbol();
        DecoderSymbol *peek_symbol();

        void put_prompt_banner();
        void put_login_banner();

        void complete(Line::Event event);

        void prepare_prompt();

        static const char *state_to_s(State state);
    };
};

#endif
