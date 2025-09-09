/* Source File: fhash.c */

#include "c-utils.h"

int main (int argc, char *argv[])
{
    int ret = 0;
    char *shsum;
    file_dat _fd;
    file_dat *fdp = &_fd;

    if (argc < 2) {
        printf ("\nUsage: %s file\n", argv[0]);
        return -1;
    }

    memset (fdp, 0, sizeof (file_dat));

    ret = get_stat_data (argv[1], fdp);
    if (ret)
        return -1;

    if (fdp->mode[0] != 'd')
        fdp->crc  = crc64sum (argv[1]);

    shsum = (char *)sha256sum(argv[1]);
    if (shsum == 0)
        return -1;
    strncpy ((char*)fdp->sha256, (char*)shsum, 2*SHA256_DIGEST_LENGTH);

    printf ("%s|%s|%s|%ld|%s|%s|%llx|%s\n",
            fdp->path,
            fdp->owner,
            fdp->group,
            fdp->size,
            fdp->mode,
            fdp->dt,
            fdp->crc,
            fdp->sha256
           );

    return 0;
}
