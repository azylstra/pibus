/** Simple demonstration client for communicating with a Raspbery Pi
 * over modbus.
 * This client connects to the pi and then counts from 0 to 32 using
 * the GPIO output pins. Physically, the outputs are wired to LEDs
 * on a breadboard so that one can see the Pi counting in binary.
 *
 * @author Alex Zylstra
 * @date 2013/07/13
 * @license MIT
 *
 */

#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <cmath>

#include <chrono>
#include <thread>

#include <modbus.h>

#define LOOP             1
#define SERVER_ID       17
#define ADDRESS_START    0
#define ADDRESS_END      5

int main(void)
{
    // variables for modbus:
    modbus_t *ctx;
    int rc;
    int nb_fail;
    int nb_loop;
    int addr;
    int nb;
    uint16_t *tab_rq_registers;
    uint16_t *tab_rp_registers;

    // TCP 
    ctx = modbus_new_tcp("192.168.1.194", 1502);
    //modbus_set_debug(ctx, TRUE);

    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    /* Allocate and initialize the different memory spaces */
    nb = ADDRESS_END - ADDRESS_START;

    tab_rq_registers = new uint16_t[nb];

    // set the register values:
    for(int i=0; i<=nb; i++)
    	tab_rq_registers[i] = 0;

    tab_rp_registers = new uint16_t[nb];

    // write all registers to initialization value
    for (addr = ADDRESS_START; addr <= ADDRESS_END; addr++) {
        rc = modbus_write_register(ctx, addr, tab_rq_registers[addr]);
    }

    // counting:
    for(int i=0; i < exp2(nb); i++)
    {
        // set pins using bitwise logic
        for(addr = ADDRESS_START; addr <= ADDRESS_END; addr++)
        {
            int j = addr - ADDRESS_START;
            modbus_write_register(ctx, addr, (i & (int)exp2(j)) );
        }

        // wait 500ms
        std::chrono::milliseconds dura( 500 );
        std::this_thread::sleep_for( dura );
    }

    // clear registers
    for (addr = ADDRESS_START; addr <= ADDRESS_END; addr++) {
        rc = modbus_write_register(ctx, addr, 0);
    }

    /* Free the memory */
    delete tab_rq_registers;
    delete tab_rp_registers;

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}