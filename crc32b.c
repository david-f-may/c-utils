// ----------------------------- crc32b --------------------------------

/* This is the basic CRC-32 calculation with some optimization but no
table lookup. The the byte reversal is avoided by shifting the crc reg
right instead of left and by using a reversed 32-bit word to represent
the polynomial.
   When compiled to Cyclops with GCC, this function executes in 8 + 72n
instructions, where n is the number of bytes in the input message. It
should be doable in 4 + 61n instructions.
   If the inner loop is strung out (approx. 5*8 = 40 instructions),
it would take about 6 + 46n instructions. */

/*
You are free to use, copy, and distribute any of the code on this web site,
whether modified by you or not. You need not give attribution. This includes
the algorithms (some of which appear in Hacker's Delight), the Hacker's
Assistant, and any code submitted by readers. Submitters implicitly agree to
this.

The textural material and pictures are copyright by the author, and the usual
copyright rules apply. E.g., you may store the material on your computer and
make hard or soft copies for your own use. However, you may not incorporate
this material into another publication without written permission from the
author (which the author may give by email).

The author has taken care in the preparation of this material, but makes no
expressed or implied warranty of any kind and assumes no responsibility for
errors or omissions. No liability is assumed for incidental or consequential
damages in connection with or arising out of the use of the information or
programs contained herein.
*/


unsigned int crc32b(unsigned char *message) {
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0) {
      byte = message[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

#ifdef TEST
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    unsigned char *filebuf;
    int fd, i;
    struct stat statbuf;
    unsigned int crc32 = 0;
    char msg[256];

    if (argc < 2) {
        printf ("\nUsage: %s file\n", argv[0]);
        return -1;
    }

    if (lstat (argv[1], &statbuf) != 0) {
        fprintf (stdout, "***Error in crc32sum(), line %d, processing %s: \n", __LINE__, argv[1]);
        fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) )
        return 0;

    fp = fopen (argv[1], "r");
    if (0 == fp) {
            fprintf (stdout, "***Error in crc32min(), line %d, processing %s: \n", __LINE__, argv[1]);
        fflush(stdout);
        perror (0);
        return 0;
    }
    filebuf = malloc ((size_t) (statbuf.st_size + 1));
    if (0 == filebuf) {
            fprintf (stdout, "***Error in crc32sum(), line %d: fatal memory error allocating %d bytes\n", __LINE__, (int) statbuf.st_size+1);
        return 0;
    }
    fd = fileno (fp);
    i = read (fd, filebuf, statbuf.st_size);
    if (i <= 0) {
            fprintf (stdout, "***Error in crc32sum(), line %d: read of '%s' failed\n", __LINE__, argv[1]);
        return 0;
    }
    fclose (fp);

    crc32 = crc32b (filebuf);
    printf ("%x  %s\n", (unsigned int) crc32, argv[1]);
    return 0;
}
#endif

