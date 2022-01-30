/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_get_opt.h"
#include "lapel_debug.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "GetOpt";

GetOpt::GetOpt(unsigned argc, const char **argv, const Option *options, size_t size)
    :
    options(options),
    size(size),
    argc(argc),
    argv(argv),
    index(0),
    current_arg(nullptr),
    current_opt(nullptr),
    state(STATE_IDLE)
{
}

const GetOpt::Option *
GetOpt::lookup_short_option(char c)
{
    const Option *retval = nullptr;

    for(size_t i=0; i < size; i++){

        if(options[i].c == c){

            retval = &options[i];
            break;
        }
    }

    return retval;
}

const GetOpt::Option *
GetOpt::lookup_long_option(const char *s)
{
    const Option *retval = nullptr;

    for(size_t i=0; i < size; i++){

        if(strlen(options[i].s) == long_size(s)){

            if(strncmp(options[i].s, s, long_size(s)) == 0){

                retval = &options[i];
                break;
            }
        }
    }

    return retval;
}

size_t
GetOpt::long_size(const char *token)
{
    size_t retval = 0;

    for(retval=0; retval < strlen(token); retval++){

        if(token[retval] == '='){

            break;
        }
    }

    return retval;
}

bool
GetOpt::next()
{
    State next_state;

    current_arg = nullptr;
    current_opt = nullptr;

    const char *token;

    switch(state){
    default:
    case STATE_IDLE:

        token = (index < argc) ? argv[index] : nullptr;

        if(token == nullptr){

            next_state = STATE_FINISHED;
        }
        else if((*token == 0) || (*token != '-') || (strlen(token) == 1)){

            next_state = STATE_FINISHED;
        }
        // start of short option
        else if(token[1] != '-'){

            offset = 0;

            next_state = parse_short_option(&token[1]);
        }
        // '--' end of options
        else if(strlen(token) == 2){

            index++;

            next_state = STATE_FINISHED;
        }
        // long option
        else{

            next_state = parse_long_option(&token[2]);
        }
        break;

    case STATE_CHAIN:

        LAPEL_ASSERT(index < argc)

        next_state = parse_short_option(&(argv[index])[1]);
        break;

    case STATE_FINISHED:

        next_state = state;
        break;
    }

    state = next_state;

    return (get_result() == OK);
}

GetOpt::State
GetOpt::parse_short_option(const char *token)
{
    State retval = state;

    if((current_opt = lookup_short_option(token[offset]))){

        switch(current_opt->arg){
        default:
        case NONE:
            offset++;
            if(offset < strlen(token)){

                retval = STATE_CHAIN;
            }
            else{

                retval = STATE_IDLE;
                index++;
            }
            break;

        case REQUIRED:

            if(token[offset+1] != 0){

                current_arg = &token[offset+1];
            }
            else if(!is_last_arg(argv[index])){

                current_arg = argv[index+1];
                index++;
            }
            retval = STATE_IDLE;
            index++;
            break;

        case OPTIONAL:

            if(token[offset+1] != 0){

                current_arg = &token[offset+1];
            }
            else if(!is_last_arg(argv[index])){

                current_arg = argv[index+1];
                index++;
            }
            retval = STATE_IDLE;
            index++;
            break;
        }
    }

    return retval;
}

GetOpt::State
GetOpt::parse_long_option(const char *token)
{
    if((current_opt = lookup_long_option(token))){

        size_t opt_size = long_size(token);

        switch(current_opt->arg){
        default:
        case NONE:
            break;

        case REQUIRED:
        case OPTIONAL:

            // --opt=arg
            if(opt_size < strlen(token)){

                current_arg = &token[opt_size+1];
            }
            else if(!is_last_arg(argv[index])){

                current_arg = argv[index+1];
                index++;
            }
            break;
        }

        index++;
    }

    return STATE_IDLE;
}

bool
GetOpt::is_last_arg(const char *token)
{
    return (argv[argc-1] == token);
}

const char *
GetOpt::get_arg()
{
    return current_arg;
}

const GetOpt::Option *
GetOpt::get_option()
{
    return current_opt;
}

GetOpt::Result
GetOpt::get_result()
{
    Result retval;

    if(state == STATE_FINISHED){

        retval = END;
    }
    else if(get_option() == nullptr){

        retval = UNKNOWN;
    }
    else if(get_arg() == nullptr){

        retval = (current_opt->arg == REQUIRED) ? MISSING : OK;
    }
    else{

        retval = OK;
    }

    return retval;
}
