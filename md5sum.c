#include "c-utils.h"

unsigned char *md5sum (char *fname)
{
    FILE *fp;
    static unsigned char buf[BUFSIZE];
    int fd, i;
    MD5_CTX c;
    unsigned char md5[MD5_DIGEST_LENGTH];
    static char md5out[2 * MD5_DIGEST_LENGTH + 1];
    struct stat statbuf;

    if (lstat (fname, &statbuf) != 0) {
            fprintf (stdout, "***Error in sha1sum(), line %d, processing '%s': \n", __LINE__, fname);
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
    MD5_Init (&c);
    for (;;) {
        i = read (fd, buf, BUFSIZE);
        if (i <= 0)
            break;
        MD5_Update (&c, buf, (unsigned long) i);
    }
    MD5_Final (&(md5[0]), &c);
    fclose (fp);
    memset (md5out, 0, 2 * MD5_DIGEST_LENGTH + 1);
    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf (&(md5out[2 * i]), "%02x", md5[i]);
    return (unsigned char *) md5out;
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

    cp = md5sum (argv[1]);
    printf ("%s  %s\n", cp, argv[1]);
    return 0;
}
#endif /* TEST*/

