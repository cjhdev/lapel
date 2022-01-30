#include <arduino_lapel.h>

ArduinoLapel::MultiUser shell(Serial);

void setup()
{
    Serial.begin(115200);

    shell.add_user("admin", "");

    shell.add_command("led", nullptr, [](Lapel::ShellWrapper &shell, int argc, char **argv){

        Lapel::ReturnValue retval = Lapel::RETURN_OK;

        if(argc > 1){

            if(strcmp("on", argv[1]) == 0){

                pinMode(LED_BUILTIN, OUTPUT);
                digitalWrite(LED_BUILTIN, HIGH);
            }
            else if(strcmp("off", argv[1]) == 0){

                pinMode(LED_BUILTIN, OUTPUT);
                digitalWrite(LED_BUILTIN, LOW);
            }
            else{

                shell.println("error: unknown argument");
                retval = Lapel::RETURN_ERROR;
            }
        }
        else{

            shell.println("error: expecting an argument");
            retval = Lapel::RETURN_ERROR;
        }

        return retval;
    });

    shell.refresh();
}

void loop()
{
    shell.process();
}
