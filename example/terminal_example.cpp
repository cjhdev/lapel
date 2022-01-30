#include "lapel_tty.h"
#include "lapel_shell.h"
#include "lapel_config.h"
#include "my_system.h"
#include "lapel_default_history.h"
#include "lapel_command_heap.h"

#include <iostream>

#include <pty.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>


FILE *logfile;

static void ushell_at_exit(void);

static struct termios orig_termios;
static int rawmode;
static int atexit_registered;
static bool resize_pending;

extern char *optarg;
extern int optind, opterr, optopt;

void sig_handler(int signo)
{
    switch(signo){
    case SIGWINCH:
        resize_pending = true;
        break;
    default:
        break;
    }
}

class MyTTY : public Lapel::TTY {

public:

    MyTTY(int fd_in, int fd_out)
        :
        fd_in(fd_in),
        fd_out(fd_out)
    {
    }

    void put_char(char c)
    {
        (void)write(fd_out, &c, 1);;
    }

    bool get_char(char &c)
    {
        return (read(fd_in, &c, 1) == 1);
    }

private:

    int fd_in;
    int fd_out;
};

// thanks linenoise
static int enable_raw_mode(int fd)
{
    struct termios raw;

    if (!isatty(STDIN_FILENO)) goto fatal;

    if (!atexit_registered) {

        atexit(ushell_at_exit);
        atexit_registered = 1;
    }
    if (tcgetattr(fd,&orig_termios) == -1) goto fatal;

    raw = orig_termios;  /* modify the original mode */

    /* input modes: no break, no CR to NL, no parity check, no strip char,
     * no start/stop output control. */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    /* output modes - disable post processing */
    raw.c_oflag &= ~(OPOST);

    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);

    /* local modes - choing off, canonical off, no extended functions,
     * no signal chars (^Z,^C) */
    //raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);

    /* control chars - set return condition: min number of bytes and timer.
     * We want read to return every single byte, without timeout. */
    raw.c_cc[VMIN] = 1; raw.c_cc[VTIME] = 0; /* 1 byte, no timer */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(fd,TCSAFLUSH,&raw) < 0) goto fatal;

    rawmode = 1;

    return 0;

fatal:
    errno = ENOTTY;
    return -1;
}

// thanks linenoise
static void disable_raw_mode(int fd)
{
    /* Don't even check the return value as it's too late. */
    if (rawmode && tcsetattr(fd,TCSAFLUSH,&orig_termios) != -1)
        rawmode = 0;
}

static void ushell_at_exit(void)
{
    disable_raw_mode(STDIN_FILENO);
}

static bool get_size(size_t &rows, size_t &cols)
{
    bool retval = false;
    struct winsize ws;

    if((ioctl(1, TIOCGWINSZ, &ws) == 0) && (ws.ws_col > 0)){

        rows = ws.ws_row;
        cols = ws.ws_col;

        retval = true;
    }

    return retval;
}

int main(int argc, char **argv)
{
    const char *username = nullptr;
    uint64_t role = 0;
    bool logout_permitted = false;

    int opt;

    while((opt = getopt(argc, argv, "u:r:lh")) != -1){

        switch(opt){
        case 'u':   // oob_login with this user

            username = optarg;
            break;

        case 'r':   // oob_login with this role

            if(sscanf(optarg, "%" PRIu64, &role) != 1){

                fprintf(stderr, "-r <integer>\n");

                exit(EXIT_FAILURE);
            }
            break;

        case 'l':

            logout_permitted = true;
            break;

        case 'h':

            printf("terminal_example\n");
            printf("\n");
            printf("options:\n");
            printf("\n");
            printf("-u <string>     oob username\n");
            printf("-r <uint64>     role for user\n");
            printf("-l              logout permitted\n");
            printf("-h              print this message\n");
            exit(EXIT_SUCCESS);
            break;

        default:

            break;
        }
    }

    signal(SIGWINCH, sig_handler);

    if(enable_raw_mode(STDIN_FILENO) == 0){

        logfile = fopen("debug.log", "w");

        LAPEL_DEBUG("app", "begin")

        MyTTY tty(STDIN_FILENO, STDOUT_FILENO);

        MySystem system;

        Lapel::DefaultHistory history(120, 5);

        Lapel::Shell shell(system, tty, history, 120, 20);

        shell.add_user("root", "root", Lapel::ROLE_ALL);
        shell.add_user("admin", "", Lapel::ROLE_ALL);

        Lapel::CommandHeap commands;

        shell.set_user_commands(commands);

        for(unsigned i=0; i < 100; i++){

            char buffer[20];

            snprintf(buffer, sizeof(buffer), "dummy%u", i);

            commands.add_command(buffer, 0, nullptr, nullptr);
        }

        commands.add_command("nothing", 0, nullptr, nullptr);

        if(username != nullptr){

            Lapel::User user(username, role);

            shell.oob_login(user, logout_permitted);
        }

        {
            size_t rows, cols;

            if(get_size(rows, cols)){

                shell.set_size(rows, cols);
            }
        }

        shell.refresh();

        fd_set readers;

        FD_ZERO(&readers);

        FD_SET(STDIN_FILENO, &readers);

        while(true){

            char c;

            int ret = select(STDIN_FILENO + 1, &readers, nullptr, nullptr, nullptr);

            if(resize_pending){

                size_t rows, cols;

                if(get_size(cols, rows)){

                    LAPEL_DEBUG("app", "resize: rows=%u cols=%u", (unsigned)rows, (unsigned)cols)

                    shell.set_size(rows, cols);
                    shell.refresh();
                }

                resize_pending = false;
            }

            if(ret > 0){

                if(tty.get_char(c)){

                    shell.process(c);
                }
                else{

                    break;
                }
            }
        }

        disable_raw_mode(STDIN_FILENO);
    }

    exit(EXIT_SUCCESS);
}
