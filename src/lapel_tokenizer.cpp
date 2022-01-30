/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_tokenizer.h"

#include <string.h>

using namespace Lapel;

static const char Tag[] = "Tokenizer";

Tokenizer::Tokenizer(char *buffer)
    :
    buffer(buffer),
    end(strlen(buffer)),

    in_offset(0),
    out_offset(0),

    arg(nullptr),

    quote(false),
    escape(false)
{
}

size_t
Tokenizer::tokenize(char *buffer, char **argv, size_t max_argv)
{
    Tokenizer tokenizer(buffer);

    size_t argc;

    (void)memset(argv, 0, max_argv * sizeof(*argv));

    for(argc=0; argc < max_argv; argc++){

        argv[argc] = tokenizer.next_token();

        if(argv[argc] == nullptr){

            break;
        }
    }

    return argc;
}

void
Tokenizer::put_char(char c)
{
    buffer[out_offset] = c;
    out_offset++;
}

char
Tokenizer::get_char()
{
    char retval = 0;

    if(in_offset < end){

        retval = buffer[in_offset];
        in_offset++;
    }

    return retval;
}

char
Tokenizer::peek_char()
{
    char retval = 0;

    if(in_offset < end){

        retval = buffer[in_offset];
    }

    return retval;
}

void
Tokenizer::push_arg()
{
    if(arg == nullptr){

        arg = &buffer[out_offset];
    }
}

char *
Tokenizer::pop_arg()
{
    char *retval = arg;

    arg = nullptr;

    return retval;
}

char *
Tokenizer::next_token()
{
    char *retval = nullptr;
    char c;

    do{

        c = get_char();

        if(c == 0){

            if(!escape && !quote){

                retval = pop_arg();

                if(retval != nullptr){

                    put_char(0);
                }
            }
        }
        else if(escape){

            escape = false;

            put_char(c);
        }
        else if(quote){

            if(quote_char == c){

                quote = false;

                if((peek_char() == 0) || (peek_char() == ' ')){

                    put_char(0);

                    retval = pop_arg();
                }
            }
            else{

                put_char(c);
            }
        }
        else if(c == '\\'){

            escape = true;

            push_arg();
        }
        else if(c == ' '){

            retval = pop_arg();

            if(retval != nullptr){

                put_char(0);
            }
        }
        else if((c == '\'') || (c == '"')){

            quote = true;
            quote_char = c;

            push_arg();
        }
        else{

            push_arg();

            put_char(c);
        }
    }
    while((c != 0) && (retval == nullptr));

    return retval;
}
