/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_DECODER_SYMBOL_H
#define LAPEL_DECODER_SYMBOL_H

namespace Lapel {

    struct DecoderSymbol {

        enum Type {

            TYPE_NUL,
            TYPE_SOH,
            TYPE_STX,
            TYPE_ETX,
            TYPE_EOT,
            TYPE_ENQ,
            TYPE_ACK,
            TYPE_BEL,
            TYPE_BS,
            TYPE_HT,
            TYPE_LF,
            TYPE_VT,
            TYPE_FF,
            TYPE_CR,
            TYPE_SO,
            TYPE_SI,
            TYPE_DLE,
            TYPE_DC1,
            TYPE_DC2,
            TYPE_DC3,
            TYPE_DC4,
            TYPE_NAK,
            TYPE_SYN,
            TYPE_ETB,
            TYPE_CAN,
            TYPE_EM,
            TYPE_SUB,
            TYPE_ESC,
            TYPE_FS,
            TYPE_GS,
            TYPE_RS,
            TYPE_US,
            TYPE_DEL,

            TYPE_CHAR, // non-control character

            TYPE_UP,
            TYPE_DOWN,
            TYPE_LEFT,
            TYPE_RIGHT,

            TYPE_PAGE_UP,
            TYPE_PAGE_DOWN,
            TYPE_HOME,
            TYPE_END,
            TYPE_INSERT,
            TYPE_DELETE,


            TYPE_CPR
        };

        Type type;

        char c;

        uint32_t row;
        uint32_t col;

        const char * type_to_s()
        {
            const char *retval;

            switch(type){
            default:
                retval = "<unknown>";
                break;
            case TYPE_CHAR:
                retval = "CHAR";
                break;
            case TYPE_NUL:
                retval = "NUL";
                break;
            case TYPE_SOH:
                retval = "SOH";
                break;
            case TYPE_STX:
                retval = "STX";
                break;
            case TYPE_ETX:
                retval = "ETX";
                break;
            case TYPE_EOT:
                retval = "EOT";
                break;
            case TYPE_ENQ:
                retval = "ENQ";
                break;
            case TYPE_ACK:
                retval = "ACK";
                break;
            case TYPE_BEL:
                retval = "BEL";
                break;
            case TYPE_BS:
                retval = "BS";
                break;
            case TYPE_HT:
                retval = "HT";
                break;
            case TYPE_LF:
                retval = "LF";
                break;
            case TYPE_VT:
                retval = "VT";
                break;
            case TYPE_FF:
                retval = "FF";
                break;
            case TYPE_CR:
                retval = "CR";
                break;
            case TYPE_SO:
                retval = "SO";
                break;
            case TYPE_SI:
                retval = "SI";
                break;
            case TYPE_DLE:
                retval = "DLE";
                break;
            case TYPE_DC1:
                retval = "DC1";
                break;
            case TYPE_DC2:
                retval = "DC2";
                break;
            case TYPE_DC3:
                retval = "DC3";
                break;
            case TYPE_DC4:
                retval = "DC4";
                break;
            case TYPE_NAK:
                retval = "NAK";
                break;
            case TYPE_SYN:
                retval = "SYN";
                break;
            case TYPE_ETB:
                retval = "ETB";
                break;
            case TYPE_CAN:
                retval = "CAN";
                break;
            case TYPE_EM:
                retval = "EM";
                break;
            case TYPE_SUB:
                retval = "SUB";
                break;
            case TYPE_ESC:
                retval = "ESC";
                break;
            case TYPE_FS:
                retval = "FS";
                break;
            case TYPE_GS:
                retval = "GS";
                break;
            case TYPE_RS:
                retval = "RS";
                break;
            case TYPE_US:
                retval = "US";
                break;
            case TYPE_DEL:
                retval = "DEL";
                break;

            case TYPE_UP:
                retval = "UP";
                break;
            case TYPE_DOWN:
                retval = "DOWN";
                break;
            case TYPE_LEFT:
                retval = "LEFT";
                break;
            case TYPE_RIGHT:
                retval = "RIGHT";
                break;

            case TYPE_PAGE_UP:
                retval = "PAGE_UP";
                break;
            case TYPE_PAGE_DOWN:
                retval = "PAGE_DOWN";
                break;
            case TYPE_HOME:
                retval = "HOME";
                break;
            case TYPE_END:
                retval = "END";
                break;
            case TYPE_INSERT:
                retval=  "INSERT";
                break;
            case TYPE_DELETE:
                retval=  "DELETE";
                break;

            case TYPE_CPR:
                retval = "CPR";
                break;
            }

            return retval;
        }
    };
};

#endif
