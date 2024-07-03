/*
 * exec_ord.c -- execute the EDSAC order in the order tank
 *
 * LW   03/30/89
 * LW   09/14/21 -- function headers updated to C89
 * AJH  10/03/22 -- added tracing or order execution
 * AJH  03/07/24 -- minor tidy re signed/unsigned int
 */
#include "edsac.h"
#include "proto.h"
#include <stdio.h>

/*
 * jump table -- one C function for each EDSAC function code
 */
typedef void (*EXEC_FUNC)(ADDR, int);

void
undefined (ADDR a, int lflag)
{
	error("Undefined order type");
}

static EXEC_FUNC Optab[] = {
    undefined,          /* P */
    undefined,          /* Q */
    undefined,          /* W */
    pos_branch,         /* E */
    rshift,             /* R */
    transfer,           /* T */
    roundoff,           /* Y */
    u_transfer,         /* U */
    input,              /* I */
    output,             /* O */
    undefined,          /* J */
    undefined,          /* Pi */
    subtract,           /* S */
    halt,               /* Z */
    undefined,          /* K */
    undefined,          /* Erase */
    undefined,          /* blank */
    print_check,        /* F */
    undefined,          /* Theta */
    undefined,          /* D */
    undefined,          /* Phi */
    load_multiplier,    /* H */
    mult_sub,           /* N */
    undefined,          /* M */
    undefined,          /* Delta */
    lshift,             /* L */
    no_operation,       /* X */
    neg_branch,         /* G */
    add,                /* A */
    undefined,          /* B */
    collate,            /* C */
    mult_add            /* V */
};

void
exec_order(void)
{
    static WORD a0 = 0, a1 = 0, a2 = 0, a3 = 0;
    int i;
    /* call the appropriate function to perform the operation */
    WORD sct = Sequence_control_tank; /* remember sct in order to detect jumps */
    if ( Verbose > 0 )
      fputc(ORDER_CODES[Order_tank.o_func], stderr);
    /* call the appropriate function to perform the operation */
    Optab[Order_tank.o_func](Order_tank.o_addr, Order_tank.o_long);
    if ( (Verbose >=2) &&
	     ( (Accumulator[0] != a0) || (Accumulator[1] != a1)
	        || (Accumulator[2] != a2) || (Accumulator[3] != a3) )) {
	   /* Accumulator has changed */
	   a0 = Accumulator[0]; a1 = Accumulator[1];
	   a2 = Accumulator[2]; a3 = Accumulator[3];
	   fputc('\n', stderr);
	   printbits_17(Accumulator[0]);
	   for ( i = 1 ; i < 4; i++ ) {
	     fputc(' ', stderr);
	     printbits_18(Accumulator[i]);
	     }
	   fputc('\n', stderr);
    }
    if ( (Verbose > 0) && Sequence_control_tank != sct )
	   fputc('\n', stderr); /* a jump occured */
}
