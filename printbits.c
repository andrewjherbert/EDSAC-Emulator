/*
 * printbits.c -- print binary numbers
 *
 * AH   28/11/21 - tracing and order limit
 */

#include <stdio.h>
#include <stdint.h>
#include "edsac.h"


void
printbits_n(x,n)
WORD x; int n;
{
  for ( int i = n-1; i >= 0 ; i-- )
    fputc('0'|(x>>i)&1,stderr);

}
     
void
printbits_18(x)
WORD x;
{
  printbits_n(x, 18);
}

void
printbits_17(x)
WORD x;
{
  printbits_n(x, 17);
}

