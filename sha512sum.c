#include "c-utils.h"

unsigned char *sha512sum (char *fname)
{
  SHA512_CTX c;
  int fd, i;
  unsigned char buf[BUFSIZE];
  FILE *fp;
  unsigned char sha1[SHA512_DIGEST_LENGTH];
  static char sha1out[2 * SHA512_DIGEST_LENGTH + 1];
  struct stat statbuf;

  if (lstat (fname, &statbuf) != 0) {
    fprintf (stdout, "***Error in sha512sum(), line %d, processing '%s': \n", __LINE__, fname);
    fflush(stdout);
    perror (0);
    return 0;
  }
  if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
    return 0;
  fp = fopen (fname, "r");
  if (fp == 0) {
    return 0;
  }
  fd = fileno (fp);
  SHA512_Init (&c);
  for (;;) {
    i = read (fd, buf, BUFSIZE);
    if (i <= 0)
      break;
    SHA512_Update (&c, buf, (unsigned long) i);
  }
  SHA512_Final (&(sha1[0]), &c);
  fclose (fp);
  memset (sha1out, 0, 2 * SHA512_DIGEST_LENGTH + 1);
  for (i = 0; i < SHA512_DIGEST_LENGTH; i++)
    sprintf (&(sha1out[2 * i]), "%02x", sha1[i]);
  return (unsigned char *) sha1out;
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

    cp = sha512sum (argv[1]);
    printf ("%s  %s\n", cp, argv[1]);
    return 0;
}
#endif /* TEST*/


