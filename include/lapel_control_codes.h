/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_CONTROL_CODES_H
#define LAPEL_CONTROL_CODES_H

namespace Lapel {

    enum Control : char {

        NUL = 0,    // CTRL-@
        SOH = 1,    // CTRL-A
        STX = 2,    // CTRL-B
        ETX = 3,    // CTRL-C
        EOT = 4,    // CTRL-D
        ENQ = 5,    // CTRL-E
        ACK = 6,    // CTRL-F
        BEL = 7,    // CTRL-G
        BS = 8,     // CTRL-H
        HT = 9,     // CTRL-I
        LF = 10,    // CTRL-J
        VT = 11,    // CTRL-K
        FF = 12,    // CTRL-L
        CR = 13,    // CTRL-M
        SO = 14,    // CTRL-N
        SI = 15,    // CTRL-O
        DLE = 16,   // CTRL-P
        DC1 = 17,   // CTRL-Q
        DC2 = 18,   // CTRL-R
        DC3 = 19,   // CTRL-S
        DC4 = 20,   // CTRL-T
        NAK = 21,   // CTRL-U
        SYN = 22,   // CTRL-V
        ETB = 23,   // CTRL-W
        CAN = 24,   // CTRL-X
        EM = 25,    // CTRL-Y
        SUB = 26,   // CTRL-Z
        ESC = 27,   // CTRL-[
        FS = 28,    // CTRL-slash
        GS = 29,    // CTRL-]
        RS = 30,    // CTRL-^
        US = 31,    // CTRL-_
        DEL = 127
    };

};

#endif
