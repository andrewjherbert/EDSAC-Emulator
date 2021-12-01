/*
 * exec_ord.c -- execute the EDSAC order in the order tank
 *
 * LW	03/30/89
 * AJH  28/11/21 -- added tracing of order codes
 */
#include <stdio.h>
#include "edsac.h"
#include "proto.h"

/*
 * jump table -- one C function for each EDSAC function code
 */
typedef void (*EXEC_FUNC)(ADDR, int);

static EXEC_FUNC Optab[] = {
	error,				/* P */
	error,				/* Q */
	error,				/* W */
	pos_branch,			/* E */
	rshift,				/* R */
	transfer,			/* T */
	roundoff,			/* Y */
	u_transfer,			/* U */
	input,				/* I */
	output,				/* O */
	error,				/* J */
	error,				/* Pi */
	subtract,			/* S */
	halt,				/* Z */
	error,				/* K */
	error,				/* Erase */
	error,				/* blank */
	print_check,		        /* F */
	error,				/* Theta */
	error,				/* D */
	error,				/* Phi */
	load_multiplier,	        /* H */
	mult_sub,			/* N */
	error,				/* M */
	error,				/* Delta */
	lshift,				/* L */
	no_operation,		        /* X */
	neg_branch,			/* G */
	add,				/* A */
	error,				/* B */
	collate,			/* C */
	mult_add			/* V */
};

void
exec_order()
{
        static WORD a0 = 0, a1 = 0, a2 = 0, a3 = 0;
	/* call the appropriate function to perform the operation */
        WORD sct = Sequence_control_tank; /* remember sct in order to detect jumps */
	if ( Verbose > 0 ) {
	  fputc(ORDER_CODES[Order_tank.o_func], stderr);
	}
	Optab[Order_tank.o_func](Order_tank.o_addr, Order_tank.o_long);
	if ( (Verbose >=2) &&
	     ( (Accumulator[0] != a0) || (Accumulator[1] != a1)
	        || (Accumulator[2] != a2) || (Accumulator[3] != a3) )) {
	   /* Accumulator has changed */
	   a0 = Accumulator[0]; a1 = Accumulator[1];
	   a2 = Accumulator[2]; a3 = Accumulator[3];
	   for ( int i = 0 ; i < 4; i++ ) {
	     fputc((i == 0) ? '\n' : ' ', stderr);
	     printbits_18(Accumulator[i]);
	     }
	   fputc('\n', stderr);
	 }
	 if ( (Verbose > 0) && Sequence_control_tank != sct )
	   fputc('\n', stderr); /* a jump occured */
}