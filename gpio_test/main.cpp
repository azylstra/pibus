/**
  * A test program, demonstrating use of the wiringPi API
  * must be run locally on the Raspberry Pi.
  * @author Alex Zylstra
  * @date 2013/06/29
  * @license MIT
  */

#include <stdio.h>
#include <iostream>
#include <cmath>

#include <wiringPi.h>

int main (void)
{
    std::cout << "piCount test program" << std::endl;
    std::cout << "Alex Zylstra" << std::endl;

    // initialization
    if (wiringPiSetup () == -1)
        return 1;

    // number of GPIO pins wired for counting
    int n = 5;

    // set up the pins:
    for(int i=0; i < n; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, 0);
    }

    // counting
    for(int i=0; i < exp2(n); i++)
    {
        // set pins using bitwise logic
        for(int j=0; j < n; j++)
            digitalWrite( j, (i & (int)exp2(j)) );

        // wait 500ms
        delay(500);
    }

    // clear the pins:
    for(int i=0; i < n; i++)
        digitalWrite(i, 0);

    return 0;
}
