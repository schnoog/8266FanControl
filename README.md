# 8266FanControl

## Quick and dirty PWM fan control for ESP8266
A quick and dirty version of a PWM control I use to set the speed of a 
Noctua 5V PWM Fan. 

It connects to the local wifi, opens a webserver to set the fan speed
with as slider from 0 to 255. The value is saved in EEPROM and will
be recovered after boot.

## Hardware and Connection
I use an old Wemos D1 mini.
The fan is connected as follow
Pin1 (black) - GND
Pin2 (yellow) - 5V
Pin3 (green) - not connected
Pin4 (blue) - D5

That's all

