/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_system.h"

using namespace Lapel;

static const char Tag[] = "System";

System::~System()
{}

uint32_t
System::get_random()
{
    return 0;
}

const char *
System::get_hostname()
{
    return "lapel";
}

User
System::user_authenticate(const char *name, const char *password)
{
    (void)name;
    (void)password;

    return User();
}

bool
System::user_add(const char *name, const char *password, Role role)
{
    (void)name;
    (void)password;
    (void)role;

    return false;
}

void
System::user_remove(const char *name)
{
    (void)name;
}
