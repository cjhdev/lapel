/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef SINGLE_USER_H
#define SINGLE_USER_H

#include "serial_tty.h"
#include "lapel/lapel_system.h"
#include "lapel/lapel_default_history.h"
#include "lapel/lapel_shell.h"
#include "lapel/lapel_command_heap.h"
#include "shell_wrapper.h"

#include <Arduino.h>

class HardwareSerial;

namespace ArduinoLapel {

    class SingleUser : public ShellWrapper {

    public:

        SingleUser(HardwareSerial &serial, size_t max_line = 80, size_t max_argv = 5, size_t max_history = 1);

        void refresh();
        void process();

        void add_command(const char *name, void *ctx, Lapel::Command::command_fn fn);
        void add_command(const char *name, Lapel::Role role, void *ctx, Lapel::Command::command_fn fn);

    private:

        class System : public Lapel::System {

        public:

            System()
            {
            }

            uint32_t get_random()
            {
                return rand();
            }

            const char *get_hostname()
            {
                return "ardu";
            }
        };

        Lapel::User user;
        System system;
        SerialTTY tty;
        Lapel::DefaultHistory history;
        Lapel::Shell shell;
        Lapel::CommandHeap user_commands;
    };
};

#endif
