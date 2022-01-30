/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef SERIAL_TTY_H
#define SERIAL_TTY_H

#include "lapel/lapel_tty.h"

class HardwareSerial;

namespace ArduinoLapel {

    class SerialTTY : public Lapel::TTY {

    public:

        SerialTTY(HardwareSerial &serial);

        void put_char(char c);
        void flush();
        bool get_char(char &c);

    private:

        HardwareSerial &serial;
    };
};

#endif
