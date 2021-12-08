/*
 * init.c -- Routines to load initial orders into memory and
 *			 to process command-line options.
 *			 
 *
 * LW	01/06/90
 * AJH  28/11/21 - new arguments -v -l -b
 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <langinfo.h>
#include "edsac.h"
#include "orders.h"
#include "proto.h"
#include "inout.h"

#define NUM_SWITCHES	41		/* number of uniselector switches available */

static ORDER Uniselectors[2][NUM_SWITCHES+1] = {
	{	/* original initial orders */
		{T_CODE},	  {H_CODE, 2},	{T_CODE},	{E_CODE, 6},
		{P_CODE, 1},      {P_CODE, 5},	{T_CODE},	{I_CODE},
		{A_CODE},	  {R_CODE, 16}, {T_CODE, 0, 1}, {I_CODE, 2},
		{A_CODE, 2},      {S_CODE, 5},	{E_CODE, 21},	{T_CODE, 3},
		{V_CODE, 1},      {L_CODE, 8},	{A_CODE, 2},	{T_CODE, 1},
		{E_CODE, 11},     {R_CODE, 4},	{A_CODE, 1},	{L_CODE, 0, 1},
		{A_CODE},	  {T_CODE, 31}, {A_CODE, 25},	{A_CODE, 4},
		{U_CODE, 25},     {S_CODE, 31}, {G_CODE, 6},	{-1}
	},
	{	/* final form of initial orders */
		{T_CODE},	{E_CODE, 20},	 {P_CODE, 1},	  {U_CODE, 2},
		{A_CODE, 39},   {R_CODE, 4},	 {V_CODE},	  {L_CODE, 8},
		{T_CODE},	{I_CODE, 1}, 	 {A_CODE, 1},	  {S_CODE, 39},
		{G_CODE, 4},    {L_CODE, 0, 1},	 {S_CODE, 39},	  {E_CODE, 17},
		{S_CODE, 7},    {A_CODE, 35},	 {T_CODE, 20},	  {A_CODE},
		{H_CODE, 8},    {A_CODE, 40},	 {T_CODE, 43},	  {A_CODE, 22},
		{A_CODE, 2},    {T_CODE, 22},	 {E_CODE, 34},	  {A_CODE, 43},
		{E_CODE, 8},    {A_CODE, 42},	 {A_CODE, 40},	  {E_CODE, 25},
		{A_CODE, 22},   {T_CODE, 42},	 {I_CODE, 40, 1}, {A_CODE, 40, 1},
		{R_CODE, 16},   {T_CODE, 40, 1}, {E_CODE, 8},	  {P_CODE, 5, 1},
		{P_CODE, 0, 1}, {-1}
	}
};

/* the set of initial orders to use -- assume the "production" orders */
static int Initial_Orders = 1; /* or 0 or -1 */

#define LOAD_BINARY -1 /* bypass initial orders and load binary direct to store */

/*
 * initialize -- Load the initial orders and clear the sequence control tank.
 */
void
initialize()
{

  if ( Initial_Orders < 0 ) {
    
         read_binary_to_store();
	
  } else {

	for (int i = 0; Uniselectors[Initial_Orders][i].o_func >= 0; i++)
		Store[i] = ((WORD) Uniselectors[Initial_Orders][i].o_func
								<< (ADDR_BITS + FLAG_BITS))
						+ (Uniselectors[Initial_Orders][i].o_addr
								<< (FLAG_BITS))
						+ (Uniselectors[Initial_Orders][i].o_long);
  }

  Sequence_control_tank = 0;

}

WORD
read_18_bits()
{
  int64_t word = 0;
  int ch = fgetc(Tape_reader); 

  while ( ch != EOF ) {
    while ( strchr(" \n\t\r", ch) != NULL ) {
      ch = fgetc(Tape_reader); /* skip white space */
      }
    if ( ch != EOF ) {
      for ( int bit = 1 ; bit <= 18 ; bit++ ) { /* load bits into a word */
  	if ( ch == '1' ) {
	  word = (word << 1) + 1;
	} else if ( ch == '0' ) {
	  word <<= 1;
	} else if ( ch == EOF ) {
	  fputs("Unexpected end of file in binary input\n", stderr);
	  exit(1);
	} else {
	  fprintf(stderr, "Unexpected character %c found in binary input\n", ch);
	  exit(1);
	}
	ch = fgetc(Tape_reader);
      }
      ungetc(ch, Tape_reader);
      return word;
    }
  }
  return -1; /* run out of data */
}
    
void
read_binary_to_store()
{
  int loc = 0, value;
  int ch = fgetc(Tape_reader);
  if (! strcmp(nl_langinfo(CODESET), "UTF-8") ) { /* test for UTF-8 */
    /* dispose of BOM if present */
    if ( ch == 0xEF ) {
      for ( int i = 1 ; i <= 3 ; i++ ) fgetc(Tape_reader);
    } 
  }
  ungetc(ch, Tape_reader);

  if ( loc >= STORE_SIZE ) {
     fputs("Binary input too large to store\n", stderr);
     exit(1);
  }

  while ( (value = read_18_bits()) != -1 ) {
    Store[loc] = value >> 1;
    if ( ((value & 1) == 1) && (loc > 0) ) {
      Store[loc-1] += 131072;
    }
    loc++;
  }
  
}
      

/*
 * process_options -- Process a string of command-line options.
 * It is assumed that the leading '-' has been
 * stripped from the options string.
 */
void
process_options(options)
char *options;
{

	for ( ; *options != '\0' ; options++) {

		switch (tolower(*options)) {
		case '0':
		case '1':		/* choose new initial orders */
			Initial_Orders = *options - '0';
			break;
		case 'b':
		        Initial_Orders = LOAD_BINARY;
		        break;
		        
		case 'c':		/* "Clear" button */
			memset(Store, 0, STORE_SIZE*sizeof(Store[0]));
			memset(Accumulator, 0, ACCUM_SIZE*sizeof(Accumulator[0]));
			memset(Multiplier, 0, MULT_SIZE*sizeof(Multiplier[0]));
			Sequence_control_tank = 0;
			break;
		case 'l':               /* set order execution limit */
		        options++; /* extract number */
		        Order_limit = atoi(options);
		        if ( !Order_limit ) fprintf(stderr, "l option ignored\n");
			while ( isdigit(*options) ) options++; /* skip over digits */
			options--; /* step back to last digit */
			break;
		case 's':		/* "Start" button */
			initialize();
			execute();
			break;
		case 'r':		/* "Reset" button */
			execute();
			break;
		case 'v':               /* set tracing verbosity */
		        options++; /* extract number */
		        Verbose = atoi(options);
		        if ( !Verbose) fprintf(stderr, "v option ignored\n");
			while ( isdigit(*options) ) options++; /* skip over digits */
			options--; /* step back to last digit */
			break;
		default:		/* unknown option */
			fprintf(stderr, "%c:  Unknown option\n", *options);
		}

	}

}
