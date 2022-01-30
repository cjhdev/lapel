/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_shell.h"
#include "lapel_history.h"
#include "lapel_command_interface.h"
#include "lapel_tty.h"
#include "lapel_control_codes.h"
#include "lapel_line.h"
#include "lapel_debug.h"
#include "lapel_builtin.h"
#include "lapel_user.h"
#include "lapel_shell_wrapper.h"
#include "lapel_encoder.h"
#include "lapel_tokenizer.h"
#include "lapel_system.h"

#include <string.h>

#ifndef LAPEL_VERSION
#define LAPEL_VERSION "<unknown>"
#endif

using namespace Lapel;

static const char Tag[] = "Shell";

Shell::Shell(
    System &system,
    TTY &tty,
    History &history,
    size_t max_line,
    size_t max_argv
)
    :
    system(system),
    tty(tty),
    history(history),
    commands{
        nullptr,
        &Builtin::command()
    },
    line(max_line, tty),
    history_index(0),

    max_line(max_line),
    max_argv(max_argv),

    argc(0),

    state(STATE_USERNAME),

    logout_permitted(true),

    refresh_pending(true),
    logout_pending(false),

    input_semaphore(false),

    rows(default_rows),
    cols(default_cols),

    resize_state(RESIZE_STATE_IDLE)
{
    buffer = new char[max_line];
    argv = new char *[max_argv];

    (void)memset(buffer, 0, max_line);
    (void)memset(prompt, 0, sizeof(prompt));

    line.set_prompt("username: ");
    line.set_mode(Line::MODE_USERNAME);
}

Shell::~Shell()
{
    delete buffer;
    delete argv;
}

void
Shell::process()
{
    notify();
}

void
Shell::process(char c)
{
    input_semaphore = decoder.process(c, input_symbol);

    if(input_semaphore){

        notify();
    }
}

void
Shell::notify()
{
    State next_state;

    switch(state){
    default:

        next_state = state;
        break;

    case STATE_USERNAME:

        next_state = process_username();
        break;

    case STATE_PASSWORD:

        next_state = process_password();
        break;

    case STATE_FAIL:

        next_state = process_fail();
        break;

    case STATE_LINE:

        next_state = process_line();
        break;

    case STATE_YIELD:

        next_state = process_yield();
        break;
    }

    if(state != next_state){

        LAPEL_DEBUG(Tag, "state=%s next_state=%s", state_to_s(state), state_to_s(next_state))

        refresh_pending = true;
    }

    state = next_state;

    if(logout_pending){

        logout_pending = false;

        state = STATE_USERNAME;
    }

    if(refresh_pending){

        refresh();
    }
}

Shell::State
Shell::process_username()
{
    State retval = state;

    DecoderSymbol *s = pop_symbol();

    if(s){

        switch(line.process(*s)){
        default:
            break;
        case Line::EVENT_EXEC:

            line.read_buffer(buffer, max_line);

            line.clear();

            argc = Tokenizer::tokenize(buffer, argv, max_argv);

            if(argc > 0U){

                line.set_prompt("password: ");
                line.set_mode(Line::MODE_PASSWORD);

                retval = STATE_PASSWORD;
            }
            break;
        }
    }

    return retval;
}

Shell::State
Shell::process_password()
{
    State retval = state;

    DecoderSymbol *s = pop_symbol();

    Encoder encoder(tty);

    if(s){

        switch(line.process(*s)){
        default:
            break;
        case Line::EVENT_EXEC:

            argv[1] = &(argv[0])[strlen(argv[0]) + 1];

            line.read_buffer(argv[1], max_line - (strlen(argv[0]) + 1));

            user = system.user_authenticate(argv[0], argv[1]);

            if(user){

                prepare_prompt();

                line.clear();

                line.set_prompt(prompt);
                line.set_mode(Line::MODE_SESSION);

                encoder.put_cup();
                encoder.put_ed_all();

                put_login_banner();

                tty.put_char(CR);

                retval = STATE_LINE;
            }
            else{

                retval = STATE_FAIL;
            }
            break;
        }
    }

    return retval;
}

Shell::State
Shell::process_fail()
{
    State retval = state;

    DecoderSymbol *s = pop_symbol();

    if(s){

        line.clear();

        line.set_mode(Line::MODE_USERNAME);
        line.set_prompt("username: ");

        retval = STATE_USERNAME;
    }

    return retval;
}

void
Shell::complete(Line::Event event)
{
    size_t matches = 0;
    const size_t num_commands = sizeof(commands)/sizeof(*commands);

    if(argc > 0){

        for(size_t i=0; i < num_commands; i++){

            matches += (commands[i] ? commands[i]->count_partial_match(argc, (const char **)argv) : 0);
        }

        switch(matches){
        case 0:
            break;

        case 1:

            for(size_t i=0; i < num_commands; i++){

                if(commands[i]){

                    if(commands[i]->complete_partial_match(argc, (const char **)argv, buffer, max_line, 0)){

                        line.write_buffer(buffer);
                        break;
                    }
                }
            }
            break;

        default:

            if(event == Line::EVENT_COMPLETE_2){

                tty.print("\r\nfound many!\r\n");
            }
            break;
        }
    }
}

Shell::State
Shell::process_line()
{
    State retval = state;

    DecoderSymbol *s = pop_symbol();

    if(s){

        Line::Event ev = line.process(*s);

        switch(ev){
        default:
            break;
        case Line::EVENT_COMPLETE_1:
        case Line::EVENT_COMPLETE_2:

            line.read_buffer(buffer, max_line);

            argc = Tokenizer::tokenize(buffer, argv, max_argv);

            complete(ev);

            refresh_pending = true;
            break;

        case Line::EVENT_HISTORY_UP:

            line.read_buffer(buffer, max_line);

            if(history.get(history_index + 1, nullptr, 0)){

                history.set(history_index, buffer);

                if(history.get(history_index + 1, buffer, max_line)){

                    history_index++;

                    line.write_buffer(buffer);
                    refresh_pending = true;
                }
            }
            break;

        case Line::EVENT_HISTORY_DOWN:

            line.read_buffer(buffer, max_line);

            if(history_index > 0){

                if(history.get(history_index - 1, nullptr, 0)){

                    history.set(history_index, buffer);

                    if(history.get(history_index - 1, buffer, max_line)){

                        history_index--;

                        line.write_buffer(buffer);
                        refresh_pending = true;
                    }
                }
            }
            break;

        case Line::EVENT_EXEC:

            tty.print("\r\n");

            line.read_buffer(buffer, max_line);

            line.clear();

            refresh_pending = true;

            history_index = 0;

            history.set(0, buffer);
            history.push();

            argc = Tokenizer::tokenize(buffer, argv, max_argv);

            command = Command();

            if(argc > 0){

                for(size_t i=0; !command && (i < sizeof(commands)/sizeof(*commands)); i++){

                    if(commands[i] != nullptr){

                        command = commands[i]->lookup(argv[0]);
                    }
                }

                ShellWrapper wrapper(*this);

                if(!command){

                    tty.print("lapel: ");
                    tty.print(argv[0]);
                    tty.print(": command not found\r\n");
                }
                else if(!command.authorize(user)){

                    tty.print("lapel: ");
                    tty.print(argv[0]);
                    tty.print(": access denied\r\n");
                }
                else{

                    switch(command(wrapper, argc, argv)){
                    default:
                    case RETURN_OK:
                    case RETURN_ERROR:

                        retval = STATE_LINE;
                        break;

                    case RETURN_YIELD:

                        retval = STATE_YIELD;
                        break;
                    }
                }
            }
            break;

        case Line::EVENT_CLEAR:

            line.clear();
            refresh_pending = true;
            break;
        }
    }

    return retval;
}

Shell::State
Shell::process_yield()
{
    State retval;

    ShellWrapper wrapper(*this);

    DecoderSymbol *s = peek_symbol();

    if(s && s->type == DecoderSymbol::TYPE_ETX){

        (void)pop_symbol();

        tty.print("^C\r\n");

        resize_state = RESIZE_STATE_IDLE;

        retval = STATE_LINE;
    }
    else{

        LAPEL_DEBUG(Tag, "process yield")

        switch(command(wrapper, argc, argv)){
        default:
        case RETURN_OK:
        case RETURN_ERROR:

            retval = STATE_LINE;
            break;

        case RETURN_YIELD:

            retval = STATE_YIELD;
            break;
        }
    }

    return retval;
}

void
Shell::cmd_clear()
{
    Encoder encoder(tty);

    encoder.put_cup();
    encoder.put_ed_all();

    refresh_pending = true;
}

ReturnValue
Shell::cmd_resize()
{
    ReturnValue retval;
    DecoderSymbol *s;

    Encoder encoder(tty);

    s = pop_symbol();

    switch(resize_state){
    default:
    case RESIZE_STATE_IDLE:

        LAPEL_DEBUG(Tag, "emit DSR")

        encoder.put_cup(999, 999);
        encoder.put_dsr();

        resize_state = RESIZE_STATE_RX;
        retval = RETURN_YIELD;
        break;

    case RESIZE_STATE_RX:

        LAPEL_DEBUG(Tag, "check for CPR")

        if(s && (s->type == DecoderSymbol::TYPE_CPR)){

            LAPEL_DEBUG(Tag, "got a CPR")

            encoder.put_cup();

            set_size(s->row, s->col);

            refresh_pending = true;

            resize_state = RESIZE_STATE_IDLE;
            retval = RETURN_OK;
        }
        else{

            LAPEL_DEBUG(Tag, "not a CPR!")

            retval = RETURN_YIELD;
        }
        break;
    }

    return retval;
}

void
Shell::cmd_logout()
{
    if(logout_permitted){

        line.set_mode(Line::MODE_USERNAME);
        line.set_prompt("username: ");

        logout_pending = true;
    }
    else{

        tty.print("lapel: ");
        tty.print(argv[0]);
        tty.print(": not a login shell\r\n");
    }
}

const char *
Shell::get_version()
{
    static const char version[] = LAPEL_VERSION;

    return version;
}

void
Shell::cmd_print(const char *s)
{
    tty.print(s);
}

void
Shell::cmd_flush()
{
    tty.flush();
}

bool
Shell::cmd_get_input(DecoderSymbol &symbol)
{
    DecoderSymbol *s = pop_symbol();

    if(s){

        symbol = *s;
    }

    return (s != nullptr);
}

void
Shell::cmd_refresh()
{
    refresh_pending = true;
}

void
Shell::set_size(uint32_t rows, uint32_t cols)
{
    LAPEL_DEBUG(Tag, "set_size: rows=%" PRIu32 " cols=%" PRIu32, rows, cols)

    this->rows = rows;
    this->cols = cols;

    line.set_size(cols);
}

void
Shell::refresh()
{
    Encoder encoder(tty);

    refresh_pending = false;

    switch(state){
    default:
        break;
    case STATE_USERNAME:

        encoder.put_hide_cursor();

        encoder.put_cup();
        encoder.put_ed_all();

        put_prompt_banner();

        line.refresh();

        encoder.put_show_cursor();

        break;

    case STATE_PASSWORD:

        encoder.put_hide_cursor();

        encoder.put_cup();
        encoder.put_ed_all();

        put_prompt_banner();

        tty.print("username: ");
        tty.print(argv[0]);

        tty.print("\r\n");

        line.refresh();

        encoder.put_show_cursor();

        break;

    case STATE_FAIL:

        encoder.put_hide_cursor();

        encoder.put_cup();
        encoder.put_ed_all();

        put_prompt_banner();

        tty.print("username: ");
        tty.print(argv[0]);
        tty.print("\r\n");
        tty.print("password: ");
        line.refresh();
        tty.print("\r\n");

        tty.print("\r\nbad username or password");

        encoder.put_show_cursor();
        break;

    case STATE_LINE:

        line.refresh();
        break;

    case STATE_YIELD:
        break;
    }
}

void
Shell::set_user_commands(CommandInterface &ci)
{
    commands[0] = &ci;
}

bool
Shell::add_user(const char *name, const char *password, Role role)
{
    return system.user_add(name, password, role);
}

void
Shell::remove_user(const char *name)
{
    system.user_remove(name);
}

void
Shell::oob_login(const User &user, bool logout_permitted)
{
    Encoder encoder(tty);

    if(user){

        this->user = user;
        this->logout_permitted = logout_permitted;

        prepare_prompt();

        line.clear();

        line.set_prompt(prompt);
        line.set_mode(Line::MODE_SESSION);

        encoder.put_cup();
        encoder.put_ed_all();

        put_login_banner();

        tty.put_char(CR);

        state = STATE_LINE;

        refresh();
    }
}

void
Shell::oob_logout()
{
    user = User();

    logout_permitted = true;

    state = STATE_USERNAME;

    refresh();
}

DecoderSymbol *
Shell::pop_symbol()
{
    DecoderSymbol *retval = nullptr;

    if(input_semaphore){

        input_semaphore = false;

        retval = &input_symbol;
    }

    return retval;
}

DecoderSymbol *
Shell::peek_symbol()
{
    return input_semaphore ? &input_symbol : nullptr;
}

void
Shell::put_prompt_banner()
{
    const char *logo[] = {
        " ____ ____ ____ ____ ____ _________ ____ ____ ",
        "||l |||a |||p |||e |||l |||       |||s |||h ||",
        "||__|||__|||__|||__|||__|||_______|||__|||__||",
        "|/__\\|/__\\|/__\\|/__\\|/__\\|/_______\\|/__\\|/__\\|"
    };

    size_t width = strlen(*logo);

    if(cols < width){

        tty.print("\r\n");
        tty.print("LapelShell");
        tty.print("\r\n");
        tty.print("\r\nv");
        tty.print(get_version());
        tty.print("\r\n");
    }
    else{

        tty.print("\r\n");

        size_t offset = (cols - width) / 2;

        if(offset > 40){

            offset = 40;
        }

        for(size_t line=0; line < sizeof(logo)/sizeof(*logo); line++){

            if(line > 0){

                tty.print("\r\n");
            }

            for(size_t i=0; i < offset; i++){

                tty.put_char(' ');
            }

            tty.print(logo[line]);
        }

        tty.print("\r\n");
        tty.print("\r\n");

        width = strlen(get_version()) + 1;

        if(width <= cols){

            for(size_t i=0; i < offset; i++){

                tty.put_char(' ');
            }
        }

        tty.print("v");
        tty.print(get_version());
        tty.print("\r\n");
    }

    tty.print("\r\n");
    tty.print("Login");
    tty.print("\r\n");
    tty.print("\r\n");
}

void
Shell::put_login_banner()
{
}

const char *
Shell::state_to_s(State state)
{
    switch(state){
    default:                return "<unknown>";
    case STATE_USERNAME:    return "USERNAME";
    case STATE_PASSWORD:    return "PASSWORD";
    case STATE_FAIL:        return "FAIL";
    case STATE_LINE:        return "LINE";
    case STATE_YIELD:       return "YIELD";
    }
}

void
Shell::prepare_prompt()
{
    (void)memset(prompt, 0, sizeof(prompt));

    if((strlen(user.get_name()) + strlen("@") + strlen(system.get_hostname()) + strlen("$ ")) < sizeof(prompt)){

        (void)strcpy(prompt, user.get_name());
        (void)strcpy(&prompt[strlen(prompt)], "@");
        (void)strcpy(&prompt[strlen(prompt)], system.get_hostname());
        (void)strcpy(&prompt[strlen(prompt)], "$ ");
    }
    else{

        (void)strcpy(prompt, "$ ");
    }
}
