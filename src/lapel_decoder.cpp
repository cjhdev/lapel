/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_decoder.h"
#include "lapel_control_codes.h"
#include "lapel_debug.h"

using namespace Lapel;

static const char Tag[] = "Decoder";

Decoder::Decoder()
    :
    state(STATE_IDLE)
{
}

bool
Decoder::process(char c, DecoderSymbol &symbol)
{
    State next_state;

    do{

        process_pending = false;
        symbol_ready = false;

        switch(state){
        default:

            next_state = state;
            break;

        case STATE_IDLE:

            next_state = process_idle(c);
            break;

        case STATE_ESCAPE:

            next_state = process_escape(c);
            break;

        case STATE_CSI_P:

            next_state = process_csi_p(c);
            break;

        case STATE_CSI_I:

            next_state = process_csi_i(c);
            break;

        case STATE_CSI_F:

            next_state = process_csi_f(c);
            break;
        }

        state = next_state;
    }
    while(process_pending);

    if(symbol_ready){

        symbol = this->symbol;
    }

    return symbol_ready;
}

bool
Decoder::char_to_control(char c, DecoderSymbol::Type &type)
{
    bool retval = true;

    switch(c){
    case Control::NUL:
        type = DecoderSymbol::TYPE_NUL;
        break;
    case Control::EOT:
        type = DecoderSymbol::TYPE_EOT;
        break;
    case Control::SI:
        type = DecoderSymbol::TYPE_SI;
        break;
    case Control::DC1:
        type = DecoderSymbol::TYPE_DC1;
        break;
    case Control::DC2:
        type = DecoderSymbol::TYPE_DC2;
        break;
    case Control::DC3:
        type = DecoderSymbol::TYPE_DC3;
        break;
    case Control::DC4:
        type = DecoderSymbol::TYPE_DC4;
        break;
    case Control::SYN:
        type = DecoderSymbol::TYPE_SYN;
        break;
    case Control::CAN:
        type = DecoderSymbol::TYPE_CAN;
        break;
    case Control::EM:
        type = DecoderSymbol::TYPE_EM;
        break;
    case Control::SUB:
        type = DecoderSymbol::TYPE_SUB;
        break;
    case Control::FS:
        type = DecoderSymbol::TYPE_FS;
        break;
    case Control::GS:
        type = DecoderSymbol::TYPE_GS;
        break;
    case Control::RS:
        type = DecoderSymbol::TYPE_RS;
        break;
    case Control::US:
        type = DecoderSymbol::TYPE_US;
        break;
    case Control::LF:
        type = DecoderSymbol::TYPE_LF;
        break;
    case Control::BEL:  // CTRL-G
        type = DecoderSymbol::TYPE_BEL;
        break;
    case Control::SOH:  // CTRL-A
        type = DecoderSymbol::TYPE_SOH;
        break;
    case Control::STX:  // CTRL-B
        type = DecoderSymbol::TYPE_STX;
        break;
    case Control::ETX:  // CTRL-C
        type = DecoderSymbol::TYPE_ETX;
        break;
    case Control::ENQ:  // CTRL-E
        type = DecoderSymbol::TYPE_ENQ;
        break;
    case Control::ACK:  // CTRL-F
        type = DecoderSymbol::TYPE_ACK;
        break;
    case Control::VT:   // CTRL-K
        type = DecoderSymbol::TYPE_VT;
        break;
    case Control::FF:   // CTRL-L
        type = DecoderSymbol::TYPE_FF;
        break;
    case Control::SO:   // CTRL-N
        type = DecoderSymbol::TYPE_SO;
        break;
    case Control::DLE:  // CTRL-P
        type = DecoderSymbol::TYPE_DLE;
        break;
    case Control::NAK:  // CTRL-U
        type = DecoderSymbol::TYPE_NAK;
        break;
    case Control::ETB:  // CTRL-W
        type = DecoderSymbol::TYPE_ETB;
        break;
    case Control::CR:
        type = DecoderSymbol::TYPE_CR;
        break;
    case Control::DEL:
        type = DecoderSymbol::TYPE_DEL;
        break;
    case Control::BS:   // CTRL-H
        type = DecoderSymbol::TYPE_BS;
        break;
    case Control::HT:   // CTRL-I / tab
        type = DecoderSymbol::TYPE_HT;
        break;
    default:
        retval = false;
        break;
    }

    return retval;
}

Decoder::State
Decoder::process_idle(char c)
{
    State retval = state;

    if(char_to_control(c, symbol.type)){

        symbol_ready = true;
    }
    else if(c == Control::ESC){

        retval = STATE_ESCAPE;
    }
    else{

        symbol.type = DecoderSymbol::TYPE_CHAR;
        symbol.c = c;

        symbol_ready = true;
    }

    return retval;
}

Decoder::State
Decoder::process_escape(char c)
{
    State retval;

    switch(c){
    case '[':
        retval = STATE_CSI_P;
        parameter_len = 0;
        intermediate_len = 0;
        break;
    default:
        retval = STATE_IDLE;
        break;
    }

    return retval;
}

Decoder::State
Decoder::process_csi_p(char c)
{
    State retval = state;

    if((c >= '0') && (c <= '?')){

        if(parameter_len < sizeof(parameter)){

            parameter[parameter_len] = c;
            parameter_len++;
        }
    }
    else{

        retval = STATE_CSI_I;
        process_pending = true;
    }

    return retval;
}

Decoder::State
Decoder::process_csi_i(char c)
{
    State retval = state;

    if(((c >= '#') && (c <= '/')) || (c == ' ')){

        intermediate[intermediate_len] = c;
        intermediate_len++;
    }
    else{

        retval = STATE_CSI_F;
        process_pending = true;
    }

    return retval;
}

Decoder::State
Decoder::process_csi_f(char c)
{
    State retval = STATE_IDLE;

    switch(c){
    case 'A':
        symbol.type = DecoderSymbol::TYPE_UP;
        symbol_ready = true;
        break;
    case 'B':
        symbol.type = DecoderSymbol::TYPE_DOWN;
        symbol_ready = true;
        break;
    case 'C':
        symbol.type = DecoderSymbol::TYPE_RIGHT;
        symbol_ready = true;
        break;
    case 'D':
        symbol.type = DecoderSymbol::TYPE_LEFT;
        symbol_ready = true;
        break;
    case 'F':
        symbol.type = DecoderSymbol::TYPE_END;
        symbol_ready = true;
        break;
    case 'G':
        symbol.type = DecoderSymbol::TYPE_CHAR;
        symbol.c = '5';
        symbol_ready = true;
        break;
    case 'H':
        symbol.type = DecoderSymbol::TYPE_HOME;
        symbol_ready = true;
        break;
    case 'S':
        symbol.type = DecoderSymbol::TYPE_PAGE_UP;
        symbol_ready = true;
        break;
    case 'T':
        symbol.type = DecoderSymbol::TYPE_PAGE_DOWN;
        symbol_ready = true;
        break;
    case '~':
        if(parameter_len == 1){

            switch(parameter[0]){
            case '1':
            case '7':
                symbol.type = DecoderSymbol::TYPE_HOME;
                symbol_ready = true;
                break;
            case '2':
                symbol.type = DecoderSymbol::TYPE_INSERT;
                symbol_ready = true;
                break;
            case '3':
                symbol.type = DecoderSymbol::TYPE_DELETE;
                symbol_ready = true;
                break;
            case '4':
            case '8':
                symbol.type = DecoderSymbol::TYPE_END;
                symbol_ready = true;
                break;
            case '5':
                symbol.type = DecoderSymbol::TYPE_PAGE_UP;
                symbol_ready = true;
                break;
            case '6':
                symbol.type = DecoderSymbol::TYPE_PAGE_DOWN;
                symbol_ready = true;
                break;
            }
        }
        break;

    case 'R':   // cursor position report
    {
        size_t offset;

        offset = get_uint(parameter, parameter_len, symbol.row);

        if(offset > 0){

            if((parameter_len - offset) > 0){

                if(parameter[offset] == ';'){

                    if(get_uint(&parameter[offset + 1], parameter_len - offset - 1, symbol.col) > 0){

                        symbol.type = DecoderSymbol::TYPE_CPR;
                        symbol_ready = true;
                    }
                }
            }
        }
    }
        break;

    default:
        break;
    }

    return retval;
}

size_t
Decoder::get_uint(const char *c, size_t n, uint32_t &u)
{
    size_t retval = 0;

    u = 0;

    for(size_t i=0; i < n; i++){

        if(c[i] >= '0' && c[i] <= '9'){

            u *= 10;
            u += (c[i] - '0');

            retval++;
        }
        else{

            break;
        }
    }

    return retval;
}
