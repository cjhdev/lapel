/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_USER_H
#define LAPEL_USER_H

#include "lapel_role.h"

#include <stdint.h>
#include <stddef.h>

namespace Lapel {

    class User {

    public:

        User(const char *name, Role role);
        User();

        const char *get_name() const;

        operator bool() const;

        bool operator==(const char *name) const;

        Role get_role() const;

    private:

        char name[20];
        bool valid;
        Role role;
    };
};

#endif
