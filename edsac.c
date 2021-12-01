/*
 * edsac.c -- Global variables for EDSAC simulator
 *
 * LW	04/21/89
 * AH   28/11/21 - tracing and order limit
 */
#include <stdio.h>
#include "edsac.h"

/*
 * ultrasonic tanks
 */
WORD  Accumulator[4], Multiplier[2];		/* registers */
WORD  Store[STORE_SIZE];			/* main memory */
ADDR  Sequence_control_tank;			/* program counter */
ORDER Order_tank;				/* current order */

/*
 * I/O devices
 */
FILE *Tape_reader;				 /* paper tape reader */
FILE *Teleprinter;				 /* teleprinter output */

/*
 * control information
 */
STATUS EDSAC_status = STOPPED;			 /* is machine running or not? */

/*
 * tracing and order limit
 */
int Verbose = TRACE_NONE;
int Order_limit = -1;                                /* run until error or stopped */
