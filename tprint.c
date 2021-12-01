/*
 * tprint.c -- Emulate the EDSAC teleprinter.
 *			   Input will be teleprinter codes;
 *			   output will be native character set
 *			   (some ASCII hard-coded).
 *
 *			   Usage:  tprint [filename ... ]
 *
 *			   Input comes from the files specified on
 *			   the command line.  If no files are specified
 *			   or "-" is used as a filename, the standard
 *			   input is used.
 *
 * LW	05/30/89
 * AJH  12/11/21 added int result type to main
 *               input is opened as binary
 *               improved handling of output of £
 */
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <langinfo.h>
#include "tprint.h"
#define	SWITCH			0x100
#define LMODE			0		/* letter printing mode */
#define FMODE			1		/* figure printing mode */

#define isswitch(c)		((c) & SWITCH)

#define CR			'\r'
#define LF			'\n'
#define BELL			'\a'

#define STERLING		L'£'	


int Mode = LMODE;		/* assume teleprinter starts in letter printing mode */

/* translation table for EDSAC teleprinter characters. */
const wint_t transtab[2][32] = {
	/* letter mode */
	{
		'P', 'Q', 'W', 'E',
		'R', 'T', 'Y', 'U',
		'I', 'O', 'J', SWITCH+FMODE,
		'S', 'Z', 'K', SWITCH+LMODE,
		SWITCH+LMODE, 'F', CR,  'D',
		' ', 'H', 'N', 'M',
		LF,  'L', 'X', 'G',
		'A', 'B', 'C', 'V'
	},
	/* figure mode */
	{
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', BELL, SWITCH+FMODE,
		'"', '+', '(', SWITCH+LMODE,
		SWITCH+FMODE, '$', CR, ';',
		' ', STERLING,  ',', '.',
		LF,  ')', '/', '#',
		'-', '?', ':', '='
	}
};
int main(argc, argv)
int argc;
char *argv[];
{
	int i;
	FILE *f;

	/* We wish to use the user's current locale. */
	setlocale(LC_ALL, "");

	/* We intend to use wide functions on standard output. */
	fwide(stdout, 1);

	/* For Windows compatibility, print out a Byte Order Mark ifwhen using UTF-8.
	 * If you save the output to a file, this helps tell Windows  applications 
         * that the file is Unicode. Other systems don't need it nor use it.
	 */
	if (strcmp(nl_langinfo(CODESET), "UTF-8") == 0) { /* test for UTF-8 */
	  fputwc(L'\uFEFF', stdout);
	}

	if (argc == 1) {			/* no files specified */
		print_file(stdin);
	} else {
		for (i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-") == 0)
				print_file(stdin);
			else if ((f = fopen(argv[i], "rb")) != NULL)
				print_file(f);
			else
				fprintf(stderr, "%s:  Can't open file\n", argv[i]);
		}
	}
}

/*
 * print_file -- print the characters for teleprinter codes in file "infile"
 */
void
print_file(infile)
FILE *infile;
{
	int c;

	while ((c = getc(infile)) != WEOF)
		print_char(c);

}

/*
 * print_char -- print the character represented by the EDSAC teleprinter
 * code "c".  "SWITCH" characters change the teleprinter from
 * "letter shift" to "figure shift" and back again
 */
void
print_char(c)
wint_t c;
{
	c = transtab[Mode][c & 0x1f];	/* translate character */

	if (isswitch(c))
		Mode = c & (~SWITCH);	/* switch printer mode */
	else
	        putwchar(c);      	/* or print it */
}
