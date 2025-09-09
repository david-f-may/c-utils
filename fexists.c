#include "c-utils.h"

int fexists (const char *path)
{
    struct stat buf;
    if (lstat (path, &buf) == 0)
        return 1;
    else
        return 0;
}

#ifdef TEST

#define EXIST_FILE "fexists.c"
#define NOEXIST_FILE "aint_no_how_no_way_this_exists.noway.nohow"

int main ()
{
    if (fexists (EXIST_FILE)) {
        printf ("%s exists\n", EXIST_FILE);
    }
    else {
        printf ("%s does not exist\n", EXIST_FILE);
    }
    
    if (fexists (NOEXIST_FILE)) {
        printf ("%s exists\n", NOEXIST_FILE);
    }
    else {
        printf ("%s does not exist\n", NOEXIST_FILE);
    }
#ifdef __MIRC__
    printf ("Compiled with c2m\n");
#endif

#ifdef __TINYC__
    printf ("Compiled with tcc\n");
#endif

}

#endif /* TEST */

