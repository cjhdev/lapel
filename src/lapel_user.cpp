/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_user.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "User";

User::User()
    :
    valid(false),
    role(0)
{
    (void)memset(name, 0, sizeof(name));
}

User::User(const char *name, Role role)
    :
    role(role)
{
    (void)memset(this->name, 0, sizeof(this->name));

    if(name != nullptr){

        (void)strncpy(this->name, name, sizeof(this->name));
        valid = (*this->name != 0);
    }
    else{

        valid = false;
    }
}

User::operator bool() const
{
    return valid;
}

const char *
User::get_name() const
{
    return name;
}

bool
User::operator==(const char *name) const
{
    return strncmp(this->name, name, sizeof(this->name)) == 0;
}

Role
User::get_role() const
{
    return role;
}
