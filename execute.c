/*
 * execute.c -- fetch and execute EDSAC orders
 *
 * LW   04/21/89
 * LW   09/14/21 -- function headers updated to C89
 * AJH  10/03/22 -- Order limit added
 */
#include "edsac.h"
#include "proto.h"
#include <stdio.h>
#include <stdlib.h>

void
execute(void)
{
    for (EDSAC_status = RUNNING; EDSAC_status != STOPPED; ) {
      if ( Order_limit ) {
	if ( Order_limit > 0 ) Order_limit--;
	fetch_order();
	exec_order();
      } else {
	fputs("\n ***Order execution limit reached***\n\n", stderr);
	exit(1);
      }
    }
}
