#ifndef MY_SYSTEM_H
#define MY_SYSTEM_H

#include <random>

#include "lapel_system.h"
#include "lapel_default_user_list.h"

class MySystem : public Lapel::System {

public:

    MySystem()
        :
        user_list(*this, 2)
    {
    }

    uint32_t get_random()
    {
        return random(generator);
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

    std::random_device generator;
    std::uniform_int_distribution<uint32_t> random;

    Lapel::DefaultUserList user_list;
};


#endif
