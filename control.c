/*
 * control.c -- functions to emulate EDSAC's control operations
 *
 * LW   05/30/89
 * LW   09/14/21 -- function headers updated to C89
 * AJH  10/03/22 -- Halt message extended
 */
#include <stdio.h>
#include "edsac.h"
#include "proto.h"

#define ACCUM_IS_NEGATIVE    (Accumulator[0] & SIGN_BIT)

/*
 * halt -- Stop the machine and ring the warning bell
 */
void
halt(ADDR n, int lflag)
{
    EDSAC_status = STOPPED;
    fprintf(stderr, "\n\nExecution Halted @ location %u by Z%d%c\n\n",
		Sequence_control_tank - 1, n, lflag ? 'D' : 'F');
}

/*
 * no_operation -- null instruction
 */
void
no_operation(ADDR n, int lflag)
{}

/*
 * pos_branch -- branch to the order in storage location "n" if
 *               the number in the accumulator is greater than or
 *               equal to zero
 */
void
pos_branch(ADDR n, int lflag)
{
    n = check_addr(n, 0);   /* D code is meaningless for E order */

    if (!ACCUM_IS_NEGATIVE)
        Sequence_control_tank = n;
}

/*
 * neg_branch -- branch to the order in storage location "n" if
 *               the number in the accumulator is less than zero
 */
void
neg_branch(ADDR n, int lflag)
{
    n = check_addr(n, 0);   /* D code is meaningless for G order */

    if (ACCUM_IS_NEGATIVE)
        Sequence_control_tank = n;
}
