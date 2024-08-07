/*
 * error.c -- error handling routines for EDSAC emulator
 *
 * LW   01/03/90
 * LW   09/14/21 -- function headers updated to C89
 */
#include <stdio.h>
#include <stdlib.h>
#include "edsac.h"
#include "proto.h"

#define odd(n)  ((n) & 1)

static const char Codes[] = "PQWERTYUIOJ#SZK*.FhD!HNM&LXGABCV";

/*
 * error -- print an error message and stop
 */
void
error(char* msg)
{

    fprintf(stderr, "%s at location %u\n"
                    "Order = %c %u %c\n\n",
                    msg,
                    Sequence_control_tank - 1,
                    Codes[Order_tank.o_func],
                    Order_tank.o_addr,
                    (Order_tank.o_long ? 'D' : 'F'));
    exit(1);
}

/*
 * check_addr -- check address "n" for validity and return it
 *               (adjusted, if necessary).  If the address is
 *               odd and refers to a long number ("lflag"), it
 *               is forced to be even.  If the address does not
 *               physically exist, it is "wrapped-around".
 */
int
check_addr(ADDR n, int lflag)
{

    if (lflag && odd(n))        /* "long" location not even */
        n &= ~1;

    return(n % STORE_SIZE);

}
