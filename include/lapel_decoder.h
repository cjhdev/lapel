/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_DECODER_H
#define LAPEL_DECODER_H

#include <stddef.h>
#include <stdint.h>

#include "lapel_decoder_symbol.h"

namespace Lapel {

    class Decoder {

    public:

        Decoder();

        bool process(char c, DecoderSymbol &symbol);


        static size_t get_uint(const char *c, size_t n, uint32_t &u);


    private:

        enum State {

            STATE_IDLE,

            STATE_ESCAPE,

            STATE_CSI_P,
            STATE_CSI_I,
            STATE_CSI_F,

            STATE_READY
        };

        State state;

        bool process_pending;
        bool symbol_ready;

        DecoderSymbol symbol;

        char parameter[32];
        size_t parameter_len;

        char intermediate[32];
        size_t intermediate_len;

        State process_idle(char c);
        State process_escape(char c);
        State process_csi_p(char c);
        State process_csi_i(char c);
        State process_csi_f(char c);
        State process_tab(char c);

        bool char_to_control(char c, DecoderSymbol::Type &type);
    };
};

#endif
