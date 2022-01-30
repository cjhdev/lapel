/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_LINE_H
#define LAPEL_LINE_H

#include <stddef.h>
#include <stdint.h>

namespace Lapel {

    class TTY;
    class Shell;
    class DecoderSymbol;

    class Line {

    public:

        enum Mode {

            MODE_USERNAME,
            MODE_PASSWORD,
            MODE_SESSION
        };

        enum Event {

            EVENT_NONE,
            EVENT_EXEC,
            EVENT_COMPLETE_1,
            EVENT_COMPLETE_2,
            EVENT_HISTORY_UP,
            EVENT_HISTORY_DOWN,
            EVENT_CLEAR
        };

        /** Create a Line
         *
         * @param[in] max       maximum size of line buffer (including null-terminator)
         * @param[in] tty
         * @param[in] shell
         *
         * */
        Line(size_t max, TTY &tty);

        virtual ~Line();

        void set_mode(Mode mode);
        Mode get_mode();

        Event process(const DecoderSymbol &decoder);

        void set_size(uint32_t cols);
        uint32_t get_size();

        void set_prompt(const char *s);

        void refresh();

        void clear();

        void read_buffer(char *buffer, size_t max);
        void write_buffer(const char *buffer);

    protected:

        enum State {

            STATE_IDLE,
            STATE_TAB
        };

        State state;

        TTY &tty;

        char *buffer;
        size_t cursor;
        size_t display_cursor;
        size_t end;
        size_t max;

        uint32_t cols;

        bool process_pending;

        const char *prompt;
        size_t prompt_size;

        Mode mode;

        Event event;

        State process_idle(const DecoderSymbol &symbol);
        State process_tab(const DecoderSymbol &symbol);

        void do_backspace();
        void do_delete();
        void do_home();
        void do_end();
        void do_back();
        void do_forward();
        void do_insert(char c);
        void do_delete_from_cursor();
        void do_delete_prev_word();
        void do_delete_line();
        void do_full_refresh();
        void do_refresh_from_cursor();
        void do_refresh(bool from_cursor);

        void clear_buffer();
        void clear_buffer_from_end();

        size_t get_prompt_size();
        size_t get_cursor();
        size_t get_line_size();
        size_t get_cursor_line();
        size_t get_display_line();
        size_t get_last_line();

        char get_raw_char(size_t offset);

        bool character_is_allowed(char c);
    };

};

#endif
