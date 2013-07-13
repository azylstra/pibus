/**
  * Example modbus server for the Raspberry Pi
  * Listens for a modbus connection and then communicates with
  * the client. Currently only allows setting the GPIO pins
  * in output mode.
  *
  * @author Alex Zylstra
  * @date 2013/07/13
  * @license MIT
  *
  */

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>

#ifndef _MSC_VER
    #include <unistd.h>
#endif

#include <modbus.h>
#include <wiringPi.h>

// define some networking stuff
static const short port = 1502;
static const std::string ip = "127.0.0.1";
// how many pins are available
static const short pins = 17;

void update_gpio(modbus_mapping_t * mb_mapping)
{
    // loop over pins
    for(int i=0; i < pins; i++)
    {
        digitalWrite(i, (mb_mapping->tab_registers[i] != 0) );
    }
}

int main(int argc, char* argv [])
{
    std::cout << "PiBus Server starting..." << std::endl;

    // check for verbosity and debug flags:
    bool verbose = false;
    bool debug = false;
    if( argc >= 2 )
    {
        for(int i=1; i < argc; i++)
        {
            std::string flag(argv[i]);
            if(flag == "--verbose")
                verbose = true;
            if(flag == "--debug")
                debug = true;
        }
    }

    // GPIO initialization
    if (wiringPiSetup () == -1)
        return 1;

    // set pin modes:
    for(int i=0; i < pins; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, 0);
    }

    int s = -1;
    // pointers to modbus stuff:
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;

    // open new modbus connection
    ctx = modbus_new_tcp(ip.c_str(), port);
    // turn on debug output if requested:
    if(debug)
        modbus_set_debug(ctx, TRUE);

    // allocate mapping for modbus:
    mb_mapping = modbus_mapping_new(500, 500, 500, 500);
    if (mb_mapping == NULL) {
        std::cerr << "Failed to allocate the mapping: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // listen for TCP connections:
    if(verbose)
        std::cout << "Listening on IP " << ip << ", port " << port << std::endl;
    s = modbus_tcp_listen(ctx, 1);
    modbus_tcp_accept(ctx, &s);

    // loop indefinitely
    for (;;)
    {
        // receive a command via TCP:
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        int rc;
        rc = modbus_receive(ctx, query);

        // if the command is OK:
        if (rc > 0) // rc is the query size
        {
            // output current state if desired
            if(verbose)
            {
                for(int i=0; i<pins; i++)
                    std::cout << "Reg " << i << " = " << mb_mapping->tab_registers[i] << std::endl;
                std::cout << "---------------" << std::endl;
            }

            // reply
            modbus_reply(ctx, query, rc, mb_mapping);

            // update GPIO settings:
            update_gpio(mb_mapping);
        } 

        // check for error or close state
        // see modbus doc
        else if (rc == -1)
            break;
    }

    std::cout << "Quit the loop: " << modbus_strerror(errno) << std::endl;

    // stop listening for TCP if necessary
    if (s != -1)
        close(s);

    // garbage collection for modbus:
    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}