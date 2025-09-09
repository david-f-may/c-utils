/* Source File: floglist_l.c */

/*
 * [BeginDoc]
 *
 * \subsection{floglist_l.c}
 *
 * Program: floglist_l.c
 * Script: floglist_l.sh
 *
 * This program is almost identical to floglist.c except that the list objects
 * are automatic variables rather than allocated on the heap.  The following are
 * the differences when allocating list headers as automatic variables:
 *
 * [EndDoc]
 */
#include "sorthdr.h"

// #if defined(__TINYC__) || defined(__CINT__)
// #include "sort.c"
// #endif

#define DATASIZE 40
#define ERROR_LEN 256
#define LIST_MAX_FILE 1000000

int main (int argc, char *argv[])
{
  FILE *fp;
  char *cp, *data = 0;
  int status;
  nsort_link_t *lnk = 0;
  /*
   * [BeginDoc]
   * [Verbatim] */
  nsort_list_t lh;
  /* [EndDoc] */
  /*
   * [BeginDoc]
   *
   * Notice that the variable is not declared as a pointer here like it was in
   * floglist.c.  This causes enough memory for the entire structure to be allocated
   * on the stack.
   *
   * [EndDoc]
   */
  struct stat statbuf;
  char str[ERROR_LEN+1];
  int isWarning;
  char **cpp;
  int totalcount;
  int i;
  double t1, t2;

  if (argc != 2) {
    printf ("\n\n***Error: incorrect command-line\n");
    printf ("Usage: %s <file>\n", argv[0]);
    printf ("\twhere <file> is the file to process.\n");
    return _ERROR_;
  }
  errno = 0;
  nsort_elapsed (&t1);
  fp = fopen (argv[1], "r");
  if (0 == fp) {
#if defined(__solaris__)
    strncat (str, strerror(errno), ERROR_LEN);
#else
    strerror_r (errno, str, ERROR_LEN);
#endif
    printf ("\n\n***Error: opening %s: %s", argv[1], str);
    return _ERROR_;
  }
  stat (argv[1], &statbuf);
  cp = malloc ((size_t) statbuf.st_size+1);
  if (cp == 0) {
    printf ("\n\n***Error: memory error allocating file buffer\n");
    fclose (fp);
    return _ERROR_;
  }
  fread (cp, (size_t)statbuf.st_size, 1, fp);
  fclose (fp);
  cpp = malloc (LIST_MAX_FILE*sizeof (char*));
  if (cpp == 0) {
    printf ("\n\n***Error: memory error allocating pointer list\n");
    free (cp);
    return _ERROR_;
  }
  memset (cpp, 0, LIST_MAX_FILE*sizeof(char *));
  totalcount = nsort_text_file_split (cpp, LIST_MAX_FILE, cp, '\n');
  /*
   * [BeginDoc]
   *
   * The initialization of the list is slightly different.  We don't call
   * nsort_list_create() because the object is already created on the stack.
   * Also, when we call nsort_list_init(), we have to pass it a pointer
   * to a list header object, as follows:
   * [Verbatim] */

  status = nsort_list_init (&lh);
  if (status == _ERROR_) {
    if (nsort_check_error())
      nsort_show_error(str, ERROR_LEN);
    else
      nsort_show_list_error(&lh, str, ERROR_LEN);
    printf ("\n\n***Error: initializing list: %s\n", str);
    return _ERROR_;
  }

  /* [EndDoc] */
  printf ("Populating the list initially\n");
  for (i = 0; cpp[i] != 0 && i < totalcount; i++) {
    lnk = malloc (sizeof (nsort_link_t));
    if (lnk == 0) {
      printf ("\n\n***Error: fatal memory error allocating a link\n");
      nsort_list_clear (&lh);
      nsort_list_del (&lh);
      return _ERROR_;
    }
    lnk->number = i;
    data = malloc (DATASIZE);
    if (data == 0) {
      printf ("\n\n***Error: fatal memory error allocating a data item\n");
      free (lnk);
      nsort_list_clear (&lh);
      nsort_list_del (&lh);
      return _ERROR_;
    }
    memset (data, 0, DATASIZE);
    strcpy (data, cpp[i]);
    lnk->data = data;

    /*
     * [BeginDoc]
     *
     * Everywhere one of the list functions is called where a pointer to a
     * list header object is required, the parameter should be passed as
     * follows:
     * [Verbatim] */

    status = nsort_list_insert_link (&lh, lnk);
    if (status == _ERROR_) {
      nsort_show_list_error (&lh, str, ERROR_LEN);
      printf ("\n\n***Error: adding item to list: %s\n", str);
      nsort_list_clear (&lh);
      nsort_list_del (&lh);
      return _ERROR_;
    }

    /* [EndDoc] */
  }
  free (cp);
  free (cpp);
  nsort_elapsed (&t2);
  printf ("  Populated the list in %6.4f seconds\n", t2-t1);
  printf ("  The list has %zu items in it\n", lh.number);
  fp = fopen (argv[1], "r");
  if (0 == fp) {
#if defined(__solaris__)
    strncat (str, strerror(errno), ERROR_LEN);
#else
    strerror_r (errno, str, ERROR_LEN);
#endif
    printf ("\n\n***Error: opening %s: %s", argv[1], str);
    return _ERROR_;
  }
  nsort_elapsed (&t1);
  isWarning = FALSE;

  /*
   * [BeginDoc]
   * Since lh is an object of type nsort_list_t and not a pointer to it, we
   * use the `.' notation to access it rather than the `->' notation, as follows:
   * [Verbatim] */

  lnk = lh.head->next;

  /* [EndDoc] */
  while (lnk != lh.tail) {
    cp = fgets (str, ERROR_LEN, fp);
    if (0 == cp)
      break;
    if (str[0] == '\0')
      break;
    cp = strchr (str, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (str) == 0)
      continue;
    if (strcmp (str, (char *)lnk->data)) {
      isWarning = TRUE;
      printf ("***Warning: %s and %s should not be different\n", str,
          (char*)lnk->data);
    }
    if (feof(fp))
      break;
    lnk = lnk->next;
  }
  nsort_elapsed (&t2);
  fclose (fp);
  printf ("  Compared the list in %6.4f seconds\n", t2-t1);
  if (!isWarning)
    printf ("  A comparison between the list and the file succeeded\n");
  else
    printf ("  ***Warning: the list and the file are different.\n");

  printf ("Saving the list to a file...\n");
  nsort_elapsed (&t1);
  status = nsort_list_save (&lh, "testing nsort_list_save()", DATASIZE,
      "floglist.dat");
  if (status == _ERROR_) {
    nsort_show_list_error(&lh, str, ERROR_LEN);
    printf ("\n\n***Error: nsort_list_save(): %s\n", str);
    return _ERROR_;
  }
  nsort_elapsed (&t2);
  printf ("  Stored the list in %6.4f seconds\n", t2-t1);
  printf ("Deleting the in-memory copy...\n");
  nsort_list_clear (&lh);
  nsort_list_del (&lh);
  printf ("Getting the list from file...\n");
  nsort_elapsed (&t1);
  status = nsort_list_get (&lh, "floglist.dat");
  if (status == _ERROR_) {
    nsort_show_list_error(&lh, str, ERROR_LEN);
    printf ("\n\n***Error: nsort_list_get(): %s\n", str);
    return _ERROR_;
  }
  nsort_elapsed (&t2);
  printf ("  Reread the list in %6.4f seconds\n", t2-t1);
  printf ("  Our new list has %zu items in it\n", lh.number);
  lnk = lh.head->next;
  nsort_elapsed (&t1);
  fp = fopen (argv[1], "r");
  if (0 == fp) {
#if defined(__solaris__)
    strncat (str, strerror(errno), ERROR_LEN);
#else
    strerror_r (errno, str, ERROR_LEN);
#endif
    printf ("\n\n***Error: opening %s: %s", argv[1], str);
    return _ERROR_;
  }
  isWarning = FALSE;
  lnk = lh.head->next;
  while (lnk != lh.tail->next) {
    cp = fgets (str, ERROR_LEN, fp);
    if (0 == cp)
      break;
    if (str[0] == '\0')
      break;
    cp = strchr (str, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (str) == 0)
      continue;
    if (strcmp (str, (char *)lnk->data)) {
      isWarning = TRUE;
      printf ("***Warning: %s and %s should not be different\n", str,
          (char*)lnk->data);
    }
    if (feof(fp))
      break;
    lnk = lnk->next;
  }
  fclose (fp);
  nsort_elapsed (&t2);
  printf ("  Recompared the list in %6.4f seconds\n", t2-t1);
  if (!isWarning)
    printf ("  A comparison between the new list and the file succeeded\n");
  else
    printf ("  ***Warning: the new list and the file are different\n");
  printf ("Cleaning up now...\n");

  /*
   * [BeginDoc]
   *
   * Finally, when we clean up the the list, we use the pointer to the
   * object as above and we do not need to call nsort_list_destroy() as
   * the object was allocated on the stack.
   * [Verbatim] */

  nsort_list_clear (&lh);
  nsort_list_del (&lh);

  /* [EndDoc] */
#ifdef MALLOC_DEBUGGING
  dmalloc_shutdown();
#endif
  print_block_list();
  return 0;
}
