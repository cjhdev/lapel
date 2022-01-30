/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_line.h"
#include "lapel_tty.h"
#include "lapel_control_codes.h"
#include "lapel_debug.h"
#include "lapel_decoder.h"
#include "lapel_encoder.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "Line";

Line::Line(size_t max, TTY &tty)
    :
    state(STATE_IDLE),
    tty(tty),
    cursor(0),
    display_cursor(0),
    end(0),
    max((max > 0) ? (max-1) : 0),
    cols(80),
    prompt(nullptr),
    prompt_size(0),
    mode(MODE_SESSION)
{
    buffer = new char[max];

    clear_buffer();
}

Line::~Line()
{
    delete buffer;
}

void
Line::refresh()
{
    do_full_refresh();
}

Line::Event
Line::process(const DecoderSymbol &symbol)
{
    event = EVENT_NONE;

    State next_state;

    do{

        process_pending = false;

        switch(state){
        default:
        case STATE_IDLE:

            next_state = process_idle(symbol);
            break;

        case STATE_TAB:

            next_state = process_tab(symbol);
            break;
        }

        state = next_state;
    }
    while(process_pending);

    return event;
}

Line::State
Line::process_idle(const DecoderSymbol &symbol)
{
    State retval = state;

    switch(symbol.type){
    default:
        break;
    case DecoderSymbol::TYPE_CHAR:
        do_insert(symbol.c);
        break;
    case DecoderSymbol::TYPE_SOH:
        do_home();
        break;
    case DecoderSymbol::TYPE_STX:
        do_back();
        break;
    case DecoderSymbol::TYPE_ENQ:
        do_end();
        break;
    case DecoderSymbol::TYPE_ACK:
        do_forward();
        break;
    case DecoderSymbol::TYPE_BS:
        do_backspace();
        break;
    case DecoderSymbol::TYPE_VT:
        do_delete_from_cursor();
        break;
    case DecoderSymbol::TYPE_FF:
        this->event = EVENT_CLEAR;
        break;
    case DecoderSymbol::TYPE_CR:
        this->event = EVENT_EXEC;
        break;
    case DecoderSymbol::TYPE_SO:
        this->event = EVENT_HISTORY_DOWN;
        break;
    case DecoderSymbol::TYPE_DLE:
        this->event = EVENT_HISTORY_UP;
        break;
    case DecoderSymbol::TYPE_NAK:
        do_delete_line();
        break;
    case DecoderSymbol::TYPE_ETB:
        do_delete_prev_word();
        break;
    case DecoderSymbol::TYPE_DEL:
        do_backspace();
        break;
    case DecoderSymbol::TYPE_UP:
        this->event = EVENT_HISTORY_UP;
        break;
    case DecoderSymbol::TYPE_DOWN:
        this->event = EVENT_HISTORY_DOWN;
        break;
    case DecoderSymbol::TYPE_LEFT:
        do_back();
        break;
    case DecoderSymbol::TYPE_RIGHT:
        do_forward();
        break;
    case DecoderSymbol::TYPE_PAGE_UP:
        break;
    case DecoderSymbol::TYPE_PAGE_DOWN:
        break;
    case DecoderSymbol::TYPE_HOME:
        do_home();
        break;
    case DecoderSymbol::TYPE_END:
        do_end();
        break;
    case DecoderSymbol::TYPE_HT:
        this->event = EVENT_COMPLETE_1;
        retval = STATE_TAB;
        break;
    case DecoderSymbol::TYPE_CPR:
        break;
    }

    return retval;
}

Line::State
Line::process_tab(const DecoderSymbol &symbol)
{
    State retval = state;

    switch(symbol.type){
    default:
        retval = STATE_IDLE;
        process_pending = true;
        break;
    case DecoderSymbol::TYPE_HT:
        this->event = EVENT_COMPLETE_2;
        break;
    }

    return retval;
}

void
Line::do_backspace()
{
    if(cursor > 0){

        (void)memmove(&buffer[cursor-1], &buffer[cursor], end - cursor);

        cursor--;
        end--;

        buffer[end] = 0;

        do_refresh_from_cursor();
    }
}

void
Line::do_delete()
{
    if((end > 0) && (cursor < end)){

        (void)memmove(&buffer[cursor], &buffer[cursor+1], end - (cursor + 1));

        end--;

        do_refresh_from_cursor();
    }
}

void
Line::do_home()
{
    cursor = 0;

    do_refresh_from_cursor();
}

void
Line::do_end()
{
    cursor = end;

    do_refresh_from_cursor();
}

void
Line::do_back()
{
    if(cursor > 0){

        cursor--;

        do_refresh_from_cursor();
    }
}

void
Line::do_forward()
{
    if(cursor != end){

        cursor++;

        do_refresh_from_cursor();
    }
}

void
Line::clear_buffer()
{
    (void)memset(buffer, 0, max+1);

    end = 0;
    cursor = 0;
}

void
Line::clear_buffer_from_end()
{
    (void)memset(&buffer[end], 0, max-end);
}

bool
Line::character_is_allowed(char c)
{
    bool retval;

    switch(mode){
    default:
    case MODE_SESSION:
        retval = true;
        break;
    case MODE_USERNAME:
        retval =
            (c >= '0' && c <= '9')
            ||
            (c >= 'a' && c <= 'z')
            ||
            (c >= 'A' && c <= 'Z');
        break;
    case MODE_PASSWORD:
        retval = (c != ' ');
        break;
    }

    return retval;
}

void
Line::do_insert(char c)
{
    if(character_is_allowed(c)){

        if(end != max){

            if(cursor != end){

                (void)memmove(&buffer[cursor+1], &buffer[cursor], end - cursor);
            }

            end++;

            buffer[cursor] = c;
            buffer[end] = 0;

            cursor++;

            do_refresh_from_cursor();
        }
    }
}

void
Line::do_delete_from_cursor()
{
    end = cursor;

    do_refresh_from_cursor();
}

void
Line::do_delete_prev_word()
{
}

void
Line::do_delete_line()
{
    cursor = 0;
    end = 0;

    do_refresh_from_cursor();
}

void
Line::set_size(uint32_t cols)
{
    this->cols = cols;
}

uint32_t
Line::get_size()
{
    return cols;
}

void
Line::clear()
{
    clear_buffer();
}

void
Line::do_refresh_from_cursor()
{
    do_refresh(true);
}

void
Line::do_full_refresh()
{
    do_refresh(false);
}

void
Line::do_refresh(bool from_cursor)
{
    size_t i = 0;
    Decoder decoder;
    Encoder encoder(tty);
    DecoderSymbol symbol;

    if(from_cursor){

        if(get_cursor_line() < get_display_line()){

            encoder.put_cuu();

            i = get_cursor_line() * cols;
        }
        else{

            i = get_display_line() * cols;
        }
    }
    else{

        encoder.put_cuu(get_display_line());
    }

    tty.put_char(CR);

    encoder.put_ed_right();

    for(size_t n = i, m = 0; get_raw_char(m) > 0; m++){

        char c = get_raw_char(m);

        if(decoder.process(c, symbol) && (symbol.type == DecoderSymbol::TYPE_CHAR)){

            if(n > 0){

                n--;
            }
            else{

                tty.put_char(c);

                i++;

                if((i % cols) == 0){

                    tty.put_char(CR);
                    tty.put_char(LF);
                }
            }
        }
        else if(n == 0){

            tty.put_char(c);
        }
    }

    encoder.put_cuu(get_last_line() - get_cursor_line());

    tty.put_char(CR);

    encoder.put_cuf(get_cursor() % cols);

    tty.flush();

    display_cursor = cursor;
}

void
Line::set_prompt(const char *prompt)
{
    this->prompt = prompt;
    prompt_size = (prompt != nullptr) ? strlen(prompt) : 0;
}

size_t
Line::get_prompt_size()
{
    size_t retval = 0;

    Decoder decoder;
    DecoderSymbol symbol;

    for(size_t i=0; prompt && (i < strlen(prompt)); i++){

        if(decoder.process(prompt[i], symbol) && (symbol.type = DecoderSymbol::TYPE_CHAR)){

            retval++;
        }
    }

    return retval;
}

size_t
Line::get_cursor()
{
    return get_prompt_size() + cursor;
}

size_t
Line::get_line_size()
{
    return get_prompt_size() + end;
}

size_t
Line::get_last_line()
{
    return get_line_size() / cols;
}

size_t
Line::get_cursor_line()
{
    return (get_prompt_size() + cursor) / cols;
}

size_t
Line::get_display_line()
{
    return (get_prompt_size() + display_cursor) / cols;
}

char
Line::get_raw_char(size_t offset)
{
    char retval;

    if(offset < prompt_size){

        retval = prompt[offset];
    }
    else if(offset < (prompt_size + end)){

        retval = (mode == MODE_PASSWORD) ? '*' : buffer[offset - prompt_size];
    }
    else{

        retval = 0;
    }

    return retval;
}

void
Line::set_mode(Mode mode)
{
    this->mode = mode;
}

Line::Mode
Line::get_mode()
{
    return mode;
}

void
Line::read_buffer(char *buffer, size_t max)
{
    strncpy(buffer, this->buffer, max);
}

void
Line::write_buffer(const char *buffer)
{
    strncpy(this->buffer, buffer, max);
    end = (strlen(buffer) > max) ? max : strlen(buffer);
    this->buffer[end] = 0;
    cursor = end;
    clear_buffer_from_end();
}
