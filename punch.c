/*
 * punch.c -- Emulate the EDSAC keyboard perforator.
 *			  Input will be native character set
 *			  (some IBM-PC ASCII hard-coded);
 *			  output will be corresponding EDSAC perforator codes
 *
 *				  Usage:  punch [-xstr1] [file1] [-xstr2] [file2] ...
 *
 *			  Input comes from the files specified on the command line.
 *			  If no files are specified or "-" is used as a filename,
 *			  the standard input will be used.
 *
 *			  Extra characters may be "punched" into the output stream by
 *			  using "-x" followed immediately by the characters to be
 *			  punched.
 *
 *			  Escape codes are provided for those characters that
 *			  may not be in a machine's character set.  The currently
 *			  recognized sequences are \e for the "Erase" punch;
 *			  \s for the "section" symbol; and \p, \h, \f and \d
 *			  for the Greek letters pi, theta, phi and delta, respectively.
 *
 *			  '#', '@', '!' and '&' are also allowed as substitutes for
 *			  the pi, theta, phi and delta. '.' may be used to represent
 *			  blank tape and '*' may used to punch the "erase" character.
 *
 *			  All input that cannot be translated (certain punctuation,
 *			  unknown escape sequences, etc.) is ignored.
 *
 * LW	01/06/90
 * AJH  14/11/21 added int result type to main
 *               removed greek symbols on input
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <locale.h>
#include "punch.h"

#define	BELL		"\a"

int Filecount = 0;		/* number of files punched so far */

int main(argc, argv)
int argc;
char *argv[];
{
	int i;
	FILE *f;

	setlocale(LC_ALL, ""); /* use system locale */

	for (i = 1; i < argc; i++) {

		if (*argv[i] == '-')
			process_option(argv[i] + 1);
		else {
			++Filecount;

			if ((f = fopen(argv[i], "rb")) != NULL)
				punch_file(f);
			else
				fprintf(stderr, "%s:  Can't open file\n", argv[i]);
		}

	}

	if (Filecount == 0)		/* no files specified on command line */
		punch_file(stdin);

}

/*
 * process_option -- process a single command line option
 *					 The option flag ('-') must already have been
 *					 stripped.
 */
void
process_option(option)
char *option;
{
	switch (*option) {

	case '\0':		/* the "null option" -- punch characters from stdin */
		++Filecount;
		punch_file(stdin);
		break;
	case 'x':		/* punch the "extra" characters that follow */
	case 'X':
		punch_string(option + 1);
		break;
	default:
		fprintf(stderr, "%s:  Unknown option\n", option);
		break;
	}
}

/*
 * punch_file -- punch the codes for the characters in file "infile"
 */
void
punch_file(infile)
FILE *infile;
{
        wchar_t c;

	fwide(infile, 1); /* select wide characters */
	while ((c = getwc(infile)) != WEOF)
	  if ( c <= 127 ) {
	    punch_char((int) c);
	    } /* ignore non-ASCII */
}

/*
 * punch_string -- punch the codes for the characters in string "s"
 */
void
punch_string(s)
char *s;
{
	while (*s != '\0')
		punch_char(*s++);
}

/*
 * perforator characters in code order
 *
 * Note that the perforator codes are not the same
 * as the codes used internally by the EDSAC.  The high
 * bit for the perforator code is the opposite of its
 * internal counterpart.  This is so that the character
 * zero will not be represented by blank tape.
 *
 * '~' is used to fill blanks, as it is unused by both the
 * EDSAC perforator and printer
 *
 * The duplicate codes used in "figure_codes" for the single
 * quote and the left parenthesis come directly from Appendix
 * A of WWG and are probably a typographical error but, since
 * most punching is done without using punctuation, it does
 * not appear to cause a problem in practice.
 */
#define DUMMY	'~'

const char letter_codes[] =
		".F~D~HNM~LXGABCV"
		"PQWERTYUIOJ~SZK*";
const char figure_codes[] =
		".~@,!+-'&%~~:?()"
		"0123456789" BELL "#'~(*";

/*
 * punch_char -- "punch" the EDSAC perforator code for the character "c"
 */
void
punch_char(c)
int c;
{
	static int escape_flag = 0;		/* was last char '\\' ? */
	static int comment_flag = 0;            /* was last char '[' ?  */
	char *ptr;

	if (comment_flag && (c = ']')) {
	  comment_flag = 0; /* end of comment reached */
	} else if (escape_flag) {		/* escape sequence? */
		escape_flag = 0;

		switch (c) {
		case 'p':				/* Pi */
			putc(0x1b, stdout);
			break;
		case 's':				/* section symbol */
			putc(0x1d, stdout);
			break;
		case 'e':				/* Erase */
			putc(0x1f, stdout);
			break;
		case 'h':				/* Theta */
			putc(0x02, stdout);
			break;
		case 'f':				/* Phi */
			putc(0x04, stdout);
			break;
		case 'd':				/* Delta */
			putc(0x08, stdout);
			break;
		default:
			/* ignore sequence */
			break;
		}

	} else if (c == '\\') {
		escape_flag = 1; /* start of escape sequence
	} else if (c = '[') {
	        comment_flag = 1; /* start of comment */
	} else if (c != DUMMY && (ptr = strchr(letter_codes, toupper(c))) != NULL) {
		putc(ptr - letter_codes, stdout);
	} else if (c != DUMMY && (ptr = strchr(figure_codes, c)) != NULL) {
		putc(ptr - figure_codes, stdout);
	} /* otherwise, ignore the input character */
}