ArduinoLapel: Lapel on Arduino
============================

Only ArduinoLapel::SingleUser will fit on the classic Arduino Uno style targets.
Larger memory Arduino targets are recommended for anything more than a demo.

## Example: Multi user shell over serial

``` C++
ArduinoLapel::MultiUser shell(Serial);

void setup()
{
    Serial.begin(115200);

    shell.add_user("admin", "");

    shell.refresh();
}

void loop()
{
    shell.process();
}

```

## License

See LICENSE and NOTICE files.

