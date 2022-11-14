/*
 * fetch.c -- fetch the next order from the store into
 *            the order tank, separating the order into
 *            its components and pointing the sequence
 *            control at the next order to be executed.
 *
 * LW   03/30/89
 * LW   09/14/21 -- function headers updated to C89
 */
#include "edsac.h"
#include "proto.h"
#include <stdio.h>
#include <stdlib.h>

void
fetch_order(void)
{
    WORD curr_order;

    if (Sequence_control_tank < 0 || Sequence_control_tank > 1023) exit(1);

    curr_order = Store[Sequence_control_tank++];

    Order_tank.o_long = curr_order & MAX_FLAG;
    curr_order >>= FLAG_BITS;
    Order_tank.o_addr = curr_order & MAX_ADDR;
    curr_order >>= ADDR_BITS;
    Order_tank.o_func = curr_order & MAX_FUNC;
}
