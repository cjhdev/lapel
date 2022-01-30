/* Copyright 2022 Cameron Harper
 *
 * */
#include "serial_tty.h"

#include <Arduino.h>

using namespace ArduinoLapel;

SerialTTY::SerialTTY(HardwareSerial &serial)
    :
    serial(serial)
{
}

void
SerialTTY::put_char(char c)
{
    serial.print(c);
}

void
SerialTTY::flush()
{
    serial.flush();
}

bool
SerialTTY::get_char(char &c)
{
    bool retval;

    if(serial.available() > 0){

        c = serial.read();

        retval = true;
    }
    else{

        retval = false;
    }

    return retval;
}
