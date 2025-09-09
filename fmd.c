/* Source File: fmd.c */

/*
 * fmd takes a command like
 *
 * fmd add file.txt
 *
 * file.txt should be a list of files on the file system, like all the suid
 * files. fmd should support the following commands:
 *
 * fmd add file.txt
 *
 * fmd checks to see if there is a file.txt.dat file. If so, it reads the
 * sorted data into memory as a radix sort item, and then it adds the files
 * in the file.txt file to that sorted data and saves the file.txt.dat.
 *
 * fmd check file.txt
 *
 * fmd opens the sorted data in file.txt.dat. If that file doesn't exist,
 * fmd returns an error and quits. If it does, for each file name in
 * file.txt, fmd finds the corresponding line in the sorted data and
 * determines if the sha256 check sum is unchanged. If so, it will print
 *
 * <file> ... ok
 *
 * If it has changed, it will print
 *
 * <file> ... changed
 * 
 * If a file name given in file.txt does not exist in file.txt.dat, fmd
 * will show
 *
 * <file> DOES NOT EXIST in sorted list
 *
 */

#include "c-utils.h"
#include "sorthdr.h"

#define PATH_SZ 1000
typedef struct _this_file_dat {
    unsigned char path[PATH_SZ+1];
    unsigned char owner[FIELD_SZ+1];
    unsigned char group[FIELD_SZ+1];
    off_t size;
    unsigned char mode[FIELD_SZ+1];
    char dt[FIELD_SZ+1];
    /* message digest information */
    unsigned char sha256[SHA256_SZ+1];
} this_file_dat;

int get_this_stat_data (const char *file, this_file_dat *fdp)
{
    struct stat sb;
    struct passwd *pw;
    struct group *gp;
    struct tm *tinfo;
    char err[PATH_MAX+1];

    strncpy ((char *)fdp->path, file, PATH_SZ-1);
        if (lstat (file, &sb) == -1) {
            snprintf (err, PATH_MAX, "***Error in get_this_stat_data(), line %d, processing '%s': \n", __LINE__, file);
            perror (err);
            return -1;
        }

    /* Get the mode string */
    fdp->mode[0] = '?';
    if (S_ISBLK (sb.st_mode))
        fdp->mode[0] = 'b';
    if (S_ISCHR (sb.st_mode))
        fdp->mode[0] = 'c';
    if (S_ISDIR (sb.st_mode))
        fdp->mode[0] = 'd';
    if (S_ISREG (sb.st_mode))
        fdp->mode[0] = '-';
    if (S_ISFIFO (sb.st_mode))
        fdp->mode[0] = 'p';
    if (S_ISLNK (sb.st_mode))
        fdp->mode[0] = 'l';
    if (S_ISSOCK (sb.st_mode))
        fdp->mode[0] = 's';

    fdp->mode[1] = sb.st_mode & S_IRUSR ? 'r' : '-';
    fdp->mode[2] = sb.st_mode & S_IWUSR ? 'w' : '-';
    fdp->mode[3] = sb.st_mode & S_IXUSR ? 'x' : '-';

    fdp->mode[4] = sb.st_mode & S_IRGRP ? 'r' : '-';
    fdp->mode[5] = sb.st_mode & S_IWGRP ? 'w' : '-';
    fdp->mode[6] = sb.st_mode & S_IXGRP ? 'x' : '-';

    fdp->mode[7] = sb.st_mode & S_IROTH ? 'r' : '-';
    fdp->mode[8] = sb.st_mode & S_IWOTH ? 'w' : '-';
    fdp->mode[9] = sb.st_mode & S_IXOTH ? 'x' : '-';

    if (sb.st_mode & S_ISUID) {
        if (fdp->mode[3] != 'x')
            fdp->mode[3] = 'S';
        else
            fdp->mode[3] = 's';
    }
    if (sb.st_mode & S_ISGID) {
        if (fdp->mode[6] != 'x')
            fdp->mode[6] = 'S';
        else
            fdp->mode[6] = 's';
    }
    if (sb.st_mode & S_ISVTX) {
        if (fdp->mode[9] != 'x')
            fdp->mode[9] = 'T';
        else
            fdp->mode[9] = 't';
    }
    fdp->mode[10] = '\0';

    /* now, get the owner and group from uid, gid */
    pw = getpwuid (sb.st_uid);
    if (0 == pw)
        snprintf ((char*)fdp->owner, FIELD_SZ, "%d", sb.st_uid);
    else
        strncpy ((char*)fdp->owner, pw->pw_name, FIELD_SZ);

    gp = getgrgid (sb.st_gid);
    if (0 == gp)
        snprintf ((char*)fdp->group, FIELD_SZ, "%d", sb.st_gid);
    else
        strncpy ((char*)fdp->group, gp->gr_name, FIELD_SZ);

    /* Now, get the size and date */
    fdp->size = sb.st_size;

    /*
     * * Format: YYYY/MM/DD-HH:MM:SS
     * */
    tinfo = localtime (&sb.st_ctime);
    strftime (fdp->dt, FIELD_SZ, "%Y/%m/%d-%H:%M:%S",tinfo);

    return 0;
}

#define HASH_STR(x)  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+(x)[3])
#define ERROR_SIZE 512

int fmdCompare (void *p1, void *p2)
{
    this_file_dat *fd1 = (this_file_dat*)p1;
    this_file_dat *fd2 = (this_file_dat*)p2;
  unsigned int v1 = HASH_STR(((char*)(fd1)->path)),
               v2 = HASH_STR(((char*)(fd2)->path));
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char *)(fd1->path), (char *)(fd2->path)));
}

/*
 * 1. Check to see if there is a .dat file.
 *    a. If so, open it and add to it.
 *    b. If not, grab the file names from the text file and put them in a
 *    .dat file.
 */
int fmd_add (char *fname)
{
    char *shsum;
    char *cpnl;
    this_file_dat *fdp;
    char cp[PATH_SZ+1];
    int ctr = 0;
    FILE *fp;
    nsort_link_t *lnk;
    nsort_t *srt;
    char str[ERROR_SIZE+1];
    char save_name[PATH_MAX+1];
    int ret;

    if (! fexists (fname) ) {
        printf ("\n*** Error: fmd_add(), line %d, file %s does not exist\n", __LINE__, fname);
        return -1;
    }
    fp = fopen (fname, "r");
    if (fp == NULL) {
        printf ("\n***Error: fmd_add, line %d, could not open %s to read\n", __LINE__, fname);
        perror (" ");
        return -1;
    }
    /*
     * Check if nsort file exists. If so, read it in; if not, create it.
     */
    srt = nsort_create ();
    if (srt == NULL) {
        nsort_show_error (str, ERROR_SIZE);
        printf ("\n***Error: fmd_add, line %d, nsort_create(): %s\n", __LINE__, str);
        return -1;
    }
    strncpy (save_name, fname, PATH_MAX);
    strncat (save_name, ".dat", PATH_MAX);
    printf ("DEBUG: save_name = %s\n", save_name);
    if (fexists (save_name)) {
        ret = nsort_get (srt, fmdCompare, save_name);
        if (ret == _ERROR_) {
            nsort_show_sort_error (srt, str, ERROR_SIZE);
            printf ("\n\n***Error: fmd_add, line %d, reading sort file: %s\n", __LINE__, str);
            nsort_destroy (srt);
            return -1;
        }
    }
    else {
        ret = nsort_init (srt, fmdCompare, TRUE, TRUE);
        if (ret == _ERROR_) {
            nsort_show_sort_error (srt, str, ERROR_SIZE);
            printf ("\n***Error: fmd_add, line %d, nsort_init(): %s\n", __LINE__, str);
            nsort_destroy (srt);
            return -1;
        }
    }
    printf ("DEBUG: sizeof(this_file_dat) = %zu\n", sizeof(this_file_dat));
    while (1 == 1) {
        if (fgets (cp, PATH_SZ, fp) == NULL)
            break;
        if (cp[0] == '\n')
            continue;
        cpnl = strchr (cp, '\n');
        if (cpnl != NULL)
            *cpnl = '\0';
        if (! fexists (cp)) {
            printf ("\n***Warning: \"%s\" does not exist...ignoring\n", cp);
            continue;
        }
        fdp = malloc (sizeof (this_file_dat));
        if (fdp == NULL) {
            printf ("\n***Error: fmd_add, line %d, could not allocate %zu bites\n", __LINE__, sizeof (this_file_dat));
            return  -1;
        }
        memset (fdp, 0, sizeof (this_file_dat));
        lnk = (nsort_link_t*)malloc (sizeof (nsort_link_t));
        if (lnk == NULL) {
            printf ("\n***Error: fmd_add, line %d, allocating nsort_link_t %zu bites\n", __LINE__, sizeof (nsort_link_t));
            nsort_del (srt, 0);
            nsort_destroy (srt);
            return -1;
        }
        // printf ("DEBUG: cp = %s\n", cp);
        ret = get_this_stat_data (cp, fdp);
        if (ret)
            continue;
        /*
        if (fdp->mode[0] != 'd') {
            fdp->crc = crc64sum (cp);
            if (fdp->crc == 0) continue;
        }
        */
        shsum = (char *)sha256sum(cp);
        if (shsum == NULL)
            return -1;
        strncpy ((char*)fdp->sha256, (char*)shsum, 2*SHA256_DIGEST_LENGTH);
        lnk->data = fdp;
        ret = nsort_add_item (srt, lnk);
        if (ret == _ERROR_) {
            if (srt->sortError == SORT_UNIQUE) {
                srt->sortError = SORT_NOERROR;
                free (fdp);
                free (lnk);
                printf ("N/A: File %s already in sorted list\n", cp);
                continue;
            }
        }
        else {
            printf ("ADD: File %s added\n", cp);
            ctr ++;
        }
    }
    ret = nsort_save (srt, "Store sorted file and MD information", sizeof (this_file_dat), save_name); 
    if (ret == _ERROR_) {
        nsort_show_sort_error (srt, str, ERROR_SIZE);
        printf ("\n***Error: fmd_add, line %d, nsort_save(): %s\n", __LINE__, str);
        return -1;
    }
    printf ("Added %d items to sorted list\n", ctr);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return 0;

}

int fmd_check (char *fname)
{
    char *shsum;
    char *cpnl;
    this_file_dat *fdp;
    this_file_dat *fndfdp;
    char cp[PATH_SZ+1];
    int ctr = 0;
    int ok_ctr = 0;
    int chg_ctr = 0;
    FILE *fp;
    nsort_link_t *lnk;
    nsort_link_t *found;
    nsort_t *srt;
    char str[ERROR_SIZE+1];
    char save_name[PATH_MAX+1];
    int ret;

    strncpy (save_name, fname, PATH_MAX);
    strncat (save_name, ".dat", PATH_MAX);
    if (! fexists (save_name) ) {
        printf ("\n*** Error: fmd_check(), line %d, file %s does not exist\n", __LINE__, save_name);
        return -1;
    }
    fp = fopen (fname, "r");
    if (fp == NULL) {
        printf ("\n***Error: fmd_check, line %d, could not open %s to read\n", __LINE__, fname);
        perror (" ");
        return -1;
    }
    /*
     * Check if nsort file exists. If so, read it.
     */
    srt = nsort_create ();
    if (srt == NULL) {
        nsort_show_error (str, ERROR_SIZE);
        printf ("\n***Error: fmd_check, line %d, nsort_create(): %s\n", __LINE__, str);
        return -1;
    }
    if (fexists (save_name)) {
        ret = nsort_get (srt, fmdCompare, save_name);
        if (ret == _ERROR_) {
            nsort_show_sort_error (srt, str, ERROR_SIZE);
            printf ("\n\n***Error: fmd_check, line %d, reading sort file: %s\n", __LINE__, str);
            nsort_destroy (srt);
            return -1;
        }
    }
    else {
        printf ("\n***Error: fmd_check, line %d, file %s does not exist\n", __LINE__, save_name);
        nsort_destroy (srt);
        return  -1;
    }
    fdp = malloc (sizeof (this_file_dat));
    if (fdp == NULL) {
        printf ("\n***Error: fmd_check, line %d, could not allocate %zu bites\n", __LINE__, sizeof (this_file_dat));
        return  -1;
    }
    lnk = malloc (sizeof (nsort_link_t));
    if (lnk == NULL) {
        printf ("\n***Error: fmd_check, line %d, could not allocate %zu bites\n", __LINE__, sizeof (nsort_link_t));
        return  -1;
    }
    while (1 == 1) {
        if (fgets (cp, PATH_SZ, fp) == NULL)
            break;
        if (cp[0] == '\n')
            continue;
        cpnl = strchr (cp, '\n');
        if (cpnl != NULL)
            *cpnl = '\0';
        if (! fexists (cp)) {
            printf ("\n***Warning: \"%s\" does not exist...ignoring\n", cp);
            continue;
        }
        ret = get_this_stat_data (cp, fdp);
        if (ret)
            continue;
        /*
        if (fdp->mode[0] != 'd') {
            fdp->crc = crc64sum (cp);
            if (fdp->crc == 0) continue;
        }
        */
        shsum = (char *)sha256sum(cp);
        if (shsum == NULL)
            return -1;
        strncpy ((char*)fdp->sha256, (char*)shsum, 2*SHA256_DIGEST_LENGTH);
        lnk->data = fdp;
        found = nsort_find_item (srt, lnk);
        if (0 == found) {
            printf ("\n***Warning: fmd_check, line %d, did not find \"%s\"\n", __LINE__, (char *) cp);
            continue;
        }
        else {
            fndfdp = (this_file_dat *)found->data;
            /*
            printf ("DEBUG: fndfdp:%s|%s|%s|%zu|%s|%s|%s\n",
                    fndfdp->path, fndfdp->owner, fndfdp->group, fndfdp->size, fndfdp->mode, fndfdp->dt, fndfdp->sha256);
            printf ("DEBUG: fdp:%s|%s|%s|%zu|%s|%s|%s\n",
                    fdp->path, fdp->owner, fdp->group, fdp->size, fdp->mode, fdp->dt, fdp->sha256);
            */
            if (strcmp ((char *)fdp->sha256, (char *)fndfdp->sha256)) {
                printf ("%s is changed...\n", cp);
                chg_ctr++;
            }
            else {
                if (! strcmp ((char *)fdp->sha256, (char *)fndfdp->sha256)) {
                    printf ("%s is okay...\n", cp);
                    ok_ctr++;
                }
            }
        }
        ctr++;
    }
    printf ("Checked %d items in sorted list: %d okay, %d changed\n", ctr, ok_ctr, chg_ctr);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return 0;
}

int fmd_list (char *fname)
{
    this_file_dat *fdp;
    int ctr = 0;
    nsort_link_t *lnk;
    nsort_t *srt;
    char str[PATH_MAX];
    char save_name[PATH_MAX+1];
    int ret;

    strncpy (save_name, fname, PATH_MAX);
    strncat (save_name, ".dat", PATH_MAX);
    srt = nsort_create ();
    if (srt == NULL) {
        nsort_show_error (str, ERROR_SIZE);
        printf ("\n***Error: fmd_check, line %d, nsort_create(): %s\n", __LINE__, str);
        return -1;
    }
    if (fexists (save_name)) {
        ret = nsort_get (srt, fmdCompare, save_name);
        if (ret == _ERROR_) {
            nsort_show_sort_error (srt, str, ERROR_SIZE);
            printf ("\n***Error: fmd_list, line %d, reading sort file: %s\n", __LINE__, str);
            nsort_destroy (srt);
            return -1;
        }
        // nsort_restructure_nodes (srt);
    }
    else {
        printf ("\n***Error: fmd_list, line %d, file %s does not exist\n", __LINE__, save_name);
        nsort_destroy (srt);
        return  -1;
    }
    lnk = srt->lh->head->next;
    if (lnk == srt->lh->tail->prev) {
        printf ("\n***Error: line %d, sorted list %s is empty\n", __LINE__, save_name);
        return -1;
    }
    while (lnk != srt->lh->tail) {
        fdp = lnk->data;
        snprintf (str, PATH_MAX, "%s|%s|%s|%zu|%s|%s|%s",
            fdp->path, fdp->owner, fdp->group, fdp->size, fdp->mode, fdp->dt, fdp->sha256);
        puts ((const char *)str);
        lnk = lnk->next;
        ctr++;
    }
    nsort_del (srt, 0);
    nsort_destroy (srt);
    printf ("\nListed %d items\n", ctr);
    return 0;
}

int main (int argc, char *argv[])
{
    int ret = 0;

    if (argc < 3) {
        printf ("\nUsage: %s add|check|list file\n", argv[0]);
        return -1;
    }

    if ( ! strncmp (argv[1], "add", 5) ) {
        ret = fmd_add (argv[2]);
        if (ret)
            return -1;
    }
    else if ( ! strncmp (argv[1], "check", 5) ) {
        ret = fmd_check (argv[2]);
        if (ret)
            return -1;
    }
    else if ( ! strncmp (argv[1], "list", 5) ) {
        ret = fmd_list (argv[2]);
        if (ret)
            return -1;
    }
    else {
        printf ("*** Error: command \"%s\" unrecognized\n", argv[1]);
        return -1;
    }
    return 0;
}
