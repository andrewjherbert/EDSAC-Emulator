/*
 * execute.c -- fetch and execute EDSAC orders
 *
 * LW	04/21/89
 * AJH  28/11/21 -- Order_limit added
 */
#include <stdlib.h>
#include <stdio.h>
#include "edsac.h"
#include "proto.h"

void
execute()
{

	for (EDSAC_status = RUNNING; EDSAC_status != STOPPED; ) {
	  if ( Order_limit ) {
	        Order_limit--;
		fetch_order();
		exec_order();
	  } else {
	    fputs("\n ***Order execution limit reached***\n\n", stderr);
	    exit(1);
	  }
	}
}
