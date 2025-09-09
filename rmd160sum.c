#include "c-utils.h"

unsigned char *rmd160sum (char *fname)
{
    RIPEMD160_CTX c;
    FILE *fp;
    int fd, i;
    unsigned char buf[BUFSIZE];
    unsigned char rmd160[RIPEMD160_DIGEST_LENGTH];
    static char rmd160out[2 * RIPEMD160_DIGEST_LENGTH + 1];
    struct stat statbuf;

    if (lstat (fname, &statbuf) != 0) {
            fprintf (stdout, "***Error in rmd160sum(), line %d, processing '%s': \n", __LINE__, fname);
            fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
        return 0;

    fp = fopen (fname, "r");
    if (0 == fp) {
        return 0;
    }
    fd = fileno (fp);
    RIPEMD160_Init (&c);
    for (;;) {
        i = read (fd, buf, BUFSIZE);
        if (i <= 0)
            break;
        RIPEMD160_Update (&c, buf, (unsigned long) i);
    }
    RIPEMD160_Final (&(rmd160[0]), &c);
    fclose (fp);
    memset (rmd160out, 0, 2 * RIPEMD160_DIGEST_LENGTH + 1);
    for (i = 0; i < RIPEMD160_DIGEST_LENGTH; i++)
        sprintf (&(rmd160out[2 * i]), "%02x", rmd160[i]);
    return (unsigned char *) rmd160out;
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
    unsigned char *cp;

    if (argc < 2) {
        printf ("\nUsage: %s file\n", argv[0]);
        return -1;
    }

    cp = rmd160sum (argv[1]);
    printf ("%s  %s\n", cp, argv[1]);
    return 0;
}
#endif /* TEST*/

