/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_default_user_list.h"
#include "lapel_system.h"
#include "lapel_debug.h"

#include "mbed_sha256/mbed_sha256.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "DefaultUserList";

DefaultUserList::DefaultUserList(System &system, size_t max_records)
    :
    system(system),
    max_records(max_records)
{
    records = new Record[max_records];

    (void)memset(records, 0, sizeof(Record) * max_records);
}

DefaultUserList::~DefaultUserList()
{
    delete(records);
    records = nullptr;
}

User
DefaultUserList::authenticate(const char *name, const char *password)
{
    User retval;
    uint8_t expected_hash[32];
    uint8_t hash[32];
    uint8_t salt[32];
    size_t salt_size, hash_size;
    Role role;

    if(
        restore_user(name, role)
        &&
        restore_user_salt(name, salt, sizeof(salt), salt_size)
        &&
        restore_user_hash(name, expected_hash, sizeof(expected_hash), hash_size)
    ){
        hash_password(password, salt, sizeof(salt), hash, sizeof(hash));

        if(memcmp(hash, expected_hash, sizeof(hash)) == 0){

            retval = User(name, role);
        }
    }

    return retval;
}

bool
DefaultUserList::add_user(const char *name, const char *password, Role role)
{
    uint8_t hash[32];
    uint8_t salt[32];

    for(size_t i=0; i < sizeof(salt); i++){

        salt[i] = (uint8_t)system.get_random();
    }

    hash_password(password, salt, sizeof(salt), hash, sizeof(hash));

    return save_user(name, role, salt, sizeof(salt), hash, sizeof(hash));
}

void
DefaultUserList::remove_user(const char *name)
{
    delete_user(name);
}

void
DefaultUserList::hash_password(const char *password, const void *salt, size_t salt_size, uint8_t *hash, size_t max)
{
    (void)max;

    mbedtls_sha256_context ctx;

    mbedtls_sha256_init(&ctx);
    (void)mbedtls_sha256_starts_ret(&ctx, 0);
    (void)mbedtls_sha256_update_ret(&ctx, (uint8_t *)salt, salt_size);
    (void)mbedtls_sha256_update_ret(&ctx, (uint8_t *)password, strlen(password));
    (void)mbedtls_sha256_finish_ret(&ctx, (uint8_t *)hash);
}

bool
DefaultUserList::save_user(const char *name, Role role, const void *salt, size_t salt_size, const void *hash, size_t hash_size)
{
    bool retval;

    if(*name == 0){

        LAPEL_ERROR(Tag, "name cannot be empty")
        retval = false;
    }
    else if(strlen(name) > sizeof(records->name)){

        LAPEL_ERROR(Tag, "name is too large")
        retval = false;
    }
    else if(salt_size > sizeof(records->salt)){

        LAPEL_ERROR(Tag, "salt is too large")
        retval = false;
    }
    else if(hash_size > sizeof(records->hash)){

        LAPEL_ERROR(Tag, "hash is too large")
        retval = false;
    }
    else{

        Record *r = lookup_record(name);

        if(!r){

            r = lookup_record("");
        }

        if(!r){

            LAPEL_ERROR(Tag, "insufficent memory")
            retval = false;
        }
        else{

            (void)memset(r, 0, sizeof(*r));
            (void)strncpy(r->name, name, sizeof(r->name));
            r->role = role;
            (void)memcpy(r->salt, salt, salt_size);
            (void)memcpy(r->hash, hash, hash_size);

            retval = true;
        }
    }

    return retval;
}

bool
DefaultUserList::restore_user(const char *name, Role &role)
{
    bool retval = false;
    Record *r = lookup_record(name);

    if(r && (*name != 0)){

        role = r->role;

        retval = true;
    }

    return retval;
}

bool
DefaultUserList::restore_user_salt(const char *name, void *buffer, size_t max, size_t &actual)
{
    bool retval = false;
    Record *r = lookup_record(name);

    if(r && (*name != 0)){

        actual = sizeof(r->salt);

        (void)memcpy(buffer, r->salt, (actual > max) ? actual : max);

        retval = true;
    }

    return retval;
}

bool
DefaultUserList::restore_user_hash(const char *name, void *buffer, size_t max, size_t &actual)
{
    bool retval = false;
    Record *r = lookup_record(name);

    if(r && (*name != 0)){

        actual = sizeof(r->hash);

        (void)memcpy(buffer, r->hash, (actual > max) ? actual : max);

        retval = true;
    }

    return retval;
}

void
DefaultUserList::delete_user(const char *name)
{
    Record *r = lookup_record(name);

    if(r && (*name != 0)){

        (void)memset(r, 0, sizeof(*r));
    }
}

DefaultUserList::Record *
DefaultUserList::lookup_record(const char *name)
{
    Record *retval = nullptr;

    for(size_t i=0; i < max_records; i++){

        if(strcmp(name, records[i].name) == 0){

            retval = &records[i];
            break;
        }
    }

    return retval;
}
