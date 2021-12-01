EDSAC Emulator
==============

This folder contains a slightly updated version of a command line based
emulator for the Cambridge EDSAC computer.

The original was retrieved from the Computer Conservation Society web
site in November 2021 and updated to better support modern character
sets and improve compatibility with the GUI-based emulator from Warwick
University.

The name of the original author is unknown except by the initials "LW".

From aspects of the code it would seem the original was written to run
under the Windows operating system.

The changes made are as follows:

1. the makefile has been edited to use the cproto command to create
header files containing function prototypes and a make clean option added
which deletes the prototype header and object files, forcing a complete rebuild.

2. gcc is specified as the compiler to be used in building the system.

3. the executables no longer have the suffix .exe

4. the punch command has been updated to work with wide characters, although
all but ASCII characters are ignored.

5. The provision for using Greek symbols has been removed, so input must use
the ASCII substitutes and/or escape sequences as explained in printman.txt.

6. The option to use [ ... ] for including comments in programs has been 
added, to be compatible with the Warwick emulator.

7. the example programs have been edited to replace [ ... ] with { ... } as
a consequence of 6.  I am not sure why LW used [ ... ] except perhaps to
distinguish data from orders?

8. the tprint command has been updated to work with wide characters in order
to support output of the pound sign symbol (£).

9. additional options -b -l and -v added to enable loading of binary images
and execution tracing.

10. the original LW sources are kept in the directory "old".

Andrew Herbert 30/11/2021
