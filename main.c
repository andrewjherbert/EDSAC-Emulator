/*
 * main.c -- EDSAC emulator main program
 *
 *			 If no files are specified or "-" is used as an
 *			 argument, the standard input will be read. Allowable
 *			 options are:
 *
 *				 -c		Clear the store (the EDSAC "Clear" button)
 *				 -s		Load the initial orders & start the machine
 *						(EDSAC "Start" button)
 *				 -r             Restart the machine (EDSAC "Reset" button)
 *				 -0		Use original (1949) initial orders
 *				 -1		Use final form of initial orders (default)
 *
 *                               -vn            Set tracing verbosity
 *                                              0 = none
 *                                              1 = orders
 *                               -ln            Set limit on orders executed
 *                               -b             Load binary direct into store
 *
 *			 If there are no command-line arguments, the initial orders
 *			 will be loaded automatically and the program will be started.
 *			 Otherwise, the -s option (or -r) must be used to start the
 *			 machine.
 *
 * LW	02/07/90
 * AJH  14/11/21 - added int result type to main
 * AJH  28/11/21 - 
 */
#include <stdlib.h>
#include <stdio.h>
#include "edsac.h"
#include "inout.h"
#include "proto.h"

int main(argc, argv)
int argc;
char *argv[];
{
	int i;

	Tape_reader = stdin;
	Teleprinter = stdout;

	if (argc == 1) {		/* nothing specified on command line */

		initialize();
		execute();
		
	} else {

		for (i = 1; i < argc; i++) {

			if (argv[i][0] != '-') {
				Tape_reader = fopen(argv[i], "rb");
				if (Tape_reader == NULL) {
					fprintf(stderr, "%s:  Can't open file\n", argv[i]);
				        exit(1);
				}
			} else if (argv[i][1] == '\0')
				Tape_reader = stdin;
			else
				process_options(argv[i]+1);

		}

	}

}
