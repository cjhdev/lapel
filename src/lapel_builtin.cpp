/* Copyright 2022 Cameron Harper
 *
 * */
#include "lapel_builtin.h"
#include "lapel_command_table.h"
#include "lapel_shell_wrapper.h"
#include "lapel_get_opt.h"

#include <string.h>

using namespace Lapel;

CommandTable&
Builtin::command()
{
    static const CommandTable::Record table[] = {

        {
            .name="resize",
            .role=ROLE_ALL,
            .fn = [](ShellWrapper &shell, int argc, char **argv){

                ReturnValue retval;

                static const GetOpt::Option option[] = {
                    {.s="row", .c='r', .arg=GetOpt::REQUIRED},
                    {.s="col", .c='c', .arg=GetOpt::REQUIRED},
                    {.s="help", .c='h', .arg=GetOpt::NONE}
                };

                if(shell.is_yielding()){

                    retval = shell.dsr_resize();
                }
                else{

                    GetOpt parser(argc-1, (const char **)&argv[1], option, sizeof(option)/sizeof(*option));

                    uint32_t rows = Shell::default_rows;
                    uint32_t cols = Shell::default_cols;
                    bool manual = false, help = false;

                    while(parser.next() && !help){

                        switch(parser.get_option()->c){
                        default:
                            break;
                        case 'r':
                            Decoder::get_uint(parser.get_arg(), strlen(parser.get_arg()), rows);
                            manual = true;
                            break;
                        case 'c':
                            Decoder::get_uint(parser.get_arg(), strlen(parser.get_arg()), cols);
                            manual = true;
                            break;
                        case 'h':
                            help = true;
                            break;
                        }
                    }

                    switch(parser.get_result()){
                    case GetOpt::MISSING:

                        shell.println("error: argument missing");
                        retval = RETURN_OK;
                        break;

                    case GetOpt::UNKNOWN:

                        shell.println("error: unknown option");
                        retval = RETURN_OK;
                        break;

                    default:

                        if(help){

                            shell.println("resize [OPTIONS]");
                            shell.println("");
                            shell.println("  If no row/col is specified then this command will query the terminal for size using DSR.");
                            shell.println("");
                            shell.println("options:");
                            shell.println("");
                            shell.println("  -r --row=[UNSIGNED] specify rows");
                            shell.println("  -c --col=[UNSIGNED] specify columns");
                            shell.println("  -h --help           print this message");
                            shell.println("");

                            retval = RETURN_OK;
                        }
                        else if(manual){

                            shell.set_size(rows, cols);

                            retval = RETURN_OK;
                        }
                        else{

                            retval = shell.dsr_resize();
                        }
                        break;
                    }
                }

                return retval;
            }
        },
        {
            .name="clear",
            .role=ROLE_ALL,
            .fn = [](ShellWrapper &shell, int argc, char **argv){

                (void)argc;
                (void)argv;

                shell.clear();

                return RETURN_OK;
            }
        },
        {
            .name="logout",
            .role=ROLE_ALL,
            .fn = [](ShellWrapper &shell, int argc, char **argv){

                (void)argc;
                (void)argv;

                shell.logout();

                return RETURN_OK;
            }
        },
        {
            .name="help",
            .role=ROLE_ALL,
            .fn = [](ShellWrapper &shell, int argc, char **argv){

                (void)argc;
                (void)argv;

                shell.print("Lapel ");
                shell.print(shell.get_version());
                shell.print("\r\n");

                return RETURN_OK;
            }
        },
        {
            .name="echo",
            .role=ROLE_ALL,
            .fn = [](ShellWrapper &shell, int argc, char **argv){

                for(int i=1; i < argc; i++){

                    shell.print(argv[i]);

                    if((i+1) < argc){

                        shell.print(" ");
                    }
                }

                shell.print("\r\n");

                return RETURN_OK;
            }
        }
    };

    static CommandTable command(table, sizeof(table)/sizeof(*table), nullptr);

    return command;
}
