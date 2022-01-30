/* Copyright 2022 Cameron Harper
 *
 * */
#ifndef LAPEL_SYSTEM_H
#define LAPEL_SYSTEM_H

#include "lapel_role.h"
#include "lapel_user.h"

#include <stdint.h>

namespace Lapel {

    class System {

    public:

        virtual ~System();

        virtual uint32_t get_random();
        virtual const char *get_hostname();

        virtual User user_authenticate(const char *name, const char *password);
        virtual bool user_add(const char *name, const char *password, Role role);
        virtual void user_remove(const char *name);
    };
};

#endif
