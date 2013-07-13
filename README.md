pibus
=====

Communications with a Raspberry Pi via modbus over TCP/IP

Modbus is a common protocol for communications with PLC-type devices in industry and research. This is a simple example program which demonstrates communications using modbus with the Pi, in particular for output via the GPIO pins.

The example client sets the GPIO output to count in binary. When testing this I had a series of LEDs on a breadboard as a demonstration. This was developed for an intended use on a Pi running Raspbian but modifications to other systems ought to be straightforward.

Dependencies:
  * wiringPi library
      http://wiringpi.com/
  * libmodbus
     http://libmodbus.org/
     also available through Raspbian package manager