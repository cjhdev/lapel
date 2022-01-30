/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_DEFAULT_USER_LIST_H
#define LAPEL_DEFAULT_USER_LIST_H

#include "lapel_user.h"
#include "lapel_role.h"

namespace Lapel {

    class System;

    class DefaultUserList {

    public:

        DefaultUserList(System &system, size_t max_records = 1);
        ~DefaultUserList();

        User authenticate(const char *name, const char *password);
        bool add_user(const char *name, const char *password, Role role);
        void remove_user(const char *name);

    private:

        struct Record {

            char name[32];
            Role role;
            uint8_t salt[32];
            uint8_t hash[32];
        };

        System &system;

        Record *records;
        size_t max_records;

        void hash_password(const char *password, const void *salt, size_t salt_size, uint8_t *hash, size_t max);

        bool save_user(const char *name, Role role, const void *salt, size_t salt_size, const void *hash, size_t hash_size);
        bool restore_user(const char *name, Role &role);
        bool restore_user_salt(const char *name, void *buffer, size_t max, size_t &actual);
        bool restore_user_hash(const char *name, void *buffer, size_t max, size_t &actual);
        void delete_user(const char *name);
        Record *lookup_record(const char *name);

    };
};

#endif
