/*
 * tprint.c -- Emulate the EDSAC teleprinter.
 *             Input will be teleprinter codes;
 *             output will be wide char
 *
 *                 Usage:  tprint [filename ... ]
 *
 *             Input comes from the files specified on
 *             the command line.  If no files are specified
 *             or "-" is used as a filename, the standard
 *             input is used.
 *
 * LW   05/30/89
 * LW   09/14/21 -- function headers updated to C89
 *               -- use wide chars for output (needed for £)
 *               -- now print terminating newline at end of main()
 *               -- explicit return of EXIT_SUCCESS
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "tprint.h"

#define SWITCH          0x100
#define LMODE           0       /* letter printing mode */
#define FMODE           1       /* figure printing mode */

#define isswitch(c)     ((c) & SWITCH)

#define CR              '\r'
#define LF              '\n'
#define BELL            '\a'

#define STERLING        L'£'

int Mode = LMODE;       /* assume teleprinter starts in letter printing mode */

/* translation table for EDVAC teleprinter characters. */
const wchar_t transtab[2][32] = {
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

int
main(int argc, char *argv[])
{
    int i;
    FILE *f;

    setlocale(LC_ALL, "");

    if (argc == 1) {            /* no files specified */
        print_file(stdin);
    } else {
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0)
                print_file(stdin);
            else if ((f = fopen(argv[i], "r")) != NULL)
                               /* replace ^ with "rb" if necessary */
                print_file(f);
            else
                fprintf(stderr, "%s:  Can't open file\n", argv[i]);
        }
    }
    putwchar(L'\n');

    return EXIT_SUCCESS;
}

/*
 * print_file -- print the characters for teleprinter codes in file "infile"
 */
void
print_file(FILE *infile)
{
    int c;

    while ((c = getc(infile)) != EOF)
        print_char(c);

}

/*
 * print_char -- print the character represented by the EDVAC teleprinter
 *               code "c".  "SWITCH" characters change the teleprinter from
 *               "letter shift" to "figure shift" and back again
 */
void
print_char(wchar_t c)
{
    c = transtab[Mode][c & 0x1f];   /* translate character */

    if (isswitch(c))
        Mode = c & (~SWITCH);       /* switch printer mode */
    else
        putwchar(c);                /* or print it */
}
