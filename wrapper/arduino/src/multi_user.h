/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef MULTI_USER_H
#define MULTI_USER_H

#include "serial_tty.h"
#include "lapel/lapel_default_user_list.h"
#include "lapel/lapel_system.h"
#include "lapel/lapel_default_history.h"
#include "lapel/lapel_shell.h"
#include "lapel/lapel_command_heap.h"
#include "shell_wrapper.h"

#include <Arduino.h>

class HardwareSerial;

namespace ArduinoLapel {

    class MultiUser : public ShellWrapper {

    public:

        MultiUser(HardwareSerial &serial, size_t max_line = 80, size_t max_argv = 10, size_t max_history = 5);

        void refresh();
        void process();
        bool add_user(const char *name, const char *password, Lapel::Role role = Lapel::ROLE_ALL);
        void remove_user(const char *name);

        void add_command(const char *name, void *ctx, Lapel::Command::command_fn fn);
        void add_command(const char *name, Lapel::Role role, void *ctx, Lapel::Command::command_fn fn);

    private:

        class System : public Lapel::System {

        public:

            System(Lapel::DefaultUserList &user_list)
                :
                user_list(user_list)
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

            Lapel::User user_authenticate(const char *username, const char *password)
            {
                return user_list.authenticate(username, password);
            }

            bool user_add(const char *username, const char *password, Lapel::Role role)
            {
                return user_list.add_user(username, password, role);
            }

            void user_remove(const char *username)
            {
                user_list.remove_user(username);
            }

        private:

            Lapel::DefaultUserList &user_list;
        };

        System system;
        SerialTTY tty;
        Lapel::DefaultHistory history;
        Lapel::DefaultUserList user_list;
        Lapel::Shell shell;
        Lapel::CommandHeap user_commands;
    };
};

#endif
