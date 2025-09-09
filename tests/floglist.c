/* Source File: floglist.c */

/*
 * [BeginDoc]
 *
 * \subsection{floglist.c}
 *
 * Program: floglist.c
 * Script: floglist.sh
 *
 * This program tests the doubly linked list component of the nsort
 * functions.  Overall, this file creates a list, populates it from a
 * file of randomly generated strings, compares it to the original file,
 * saves it to disk, retrieves it from disk, compares it again and then
 * cleans up.  The script associated with this program 
 * executes it repeatedly for a certain number of times or until an error
 * occurs.
 *
 * The first line of a program that uses nsort functionality is as follows:
 * [Verbatim] */

#include "sorthdr.h"

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * This includes all the object types and prototypes that are needed to
 * use the nsort functions.  Most programs that use nsort will need to
 * create a compare function that is passed as a parameter to the sort
 * routines.  However, this program uses the linked list functionality only
 * and doesn't need a compare function.
 *
 * [EndDoc]
 */

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
  nsort_list_t *lh;
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
  /*
   * [BeginDoc]
   *
   * First, we open the first file given on the command line, read it into memory
   * and parse it by the new line character.  The error handling and checking of
   * return values may seem like overkill, but \emph{it is not}.  It is important.
   * [Verbatim] */

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
  cp = (char*)malloc ((size_t) statbuf.st_size+1);
  if (cp == 0) {
    printf ("\n\n***Error: memory error allocating file buffer\n");
    fclose (fp);
    return _ERROR_;
  }
  fread (cp, (size_t)statbuf.st_size, 1, fp);
  fclose (fp);
  cpp = (char**)malloc (LIST_MAX_FILE*sizeof (char*));
  if (cpp == 0) {
    printf ("\n\n***Error: memory error allocating pointer list\n");
    free (cp);
    return _ERROR_;
  }
  memset (cpp, 0, LIST_MAX_FILE*sizeof(char *));
  totalcount = nsort_text_file_split (cpp, LIST_MAX_FILE, cp, '\n');

  /* [EndDoc] */
  /*
   * [BeginDoc]
   *
   * Next, we create a list, which involves two steps.  First, create a list
   * header object and then initialize it.  This is in two steps to allow the
   * user to either create a list object on the heap (through a malloc or calloc
   * allocation) or as an automatic variable.  The floglist_l.c program will
   * demonstrate that.  In this case we create a list object on the heap with a
   * call to nsort_list_create(). \index{nsort_list_create}
   *
   * Secondly, we have to initilize the list object with a call to nsort_list_init().
   * \index{nsort_list_init}
   * [Verbatim] */

  lh = nsort_list_create();
  if (lh == 0) {
    free (cp);
    free (cpp);
    nsort_show_error(str, ERROR_LEN);
    printf ("\n\n***Error: memory creating list: %s\n", str);
    return _ERROR_;
  }
  status = nsort_list_init (lh);
  if (status == _ERROR_) {
    if (nsort_check_error())
      nsort_show_error(str, ERROR_LEN);
    else
      nsort_show_list_error(lh, str, ERROR_LEN);
    printf ("\n\n***Error: initializing list: %s\n", str);
    return _ERROR_;
  }

  /* [EndDoc] */
  /*
   * [BeginDoc]
   *
   * Next, we populate the list.  Each item in the list is stored in
   * a link object, a container of type nsort_link_t.  This is defined
   * \index{nsort_link_t}
   * below, but simply put contains information that allows it to be
   * linked in the list (with next and previous pointers) and a data
   * pointer.  There is also a number member of a link that can be used
   * in the application, if necessary.
   *
   * Essentially, you allocate the link, allocate the data object and
   * populate it with data, set link->data = data, and insert it into
   * the list with nsort_list_insert_link().  Everything else is error
   * \index{nsort_list_insert_link}
   * handling in case something goes wrong.  \emph{Do not take short cuts
   * with error handling.}  If you call a function and it has diagnostic
   * \index{diagnostics} \index{error checking}
   * \index{error checking!importance of}
   * information so you can check for errors, check for errors.  \emph{Do
   * this \emph{EVERY} time.}  I can't tell you how many errors I have helped
   * others find in their code that would never have been problems if they
   * simply followed this rule.
   *
   * If there is an error in the call to nsort_show_list_error(), it will return
   * _ERROR_, or a -1.  In that case, you use nsort_show_list_error() to get
   * \index{nsort_show_list_error}
   * the error into a string and print that to the screen.  Typically, if an
   * error occurs here it is something systemic and you should bale out of the
   * program.
   * [Verbatim] */

  printf ("Populating the list initially\n");
  for (i = 0; cpp[i] != 0 && i < totalcount; i++) {
    lnk = (nsort_link_t*)malloc (sizeof (nsort_link_t));
    if (lnk == 0) {
      printf ("\n\n***Error: fatal memory error "
          "allocating a link\n");
      nsort_list_clear (lh);
      nsort_list_del (lh);
      nsort_list_destroy (lh);
      return _ERROR_;
    }
    lnk->number = i;
    data = (char*)malloc (DATASIZE);
    if (data == 0) {
      printf ("\n\n***Error: fatal memory error "
          "allocating a data item\n");
      free (lnk);
      nsort_list_clear (lh);
      nsort_list_del (lh);
      nsort_list_destroy (lh);
      return _ERROR_;
    }
    memset (data, 0, DATASIZE);
    strcpy (data, cpp[i]);
    lnk->data = data;
    status = nsort_list_insert_link (lh, lnk);
    if (status == _ERROR_) {
      nsort_show_list_error (lh, str, ERROR_LEN);
      printf ("\n\n***Error: adding item to list: %s\n", str);
      nsort_list_clear (lh);
      nsort_list_del (lh);
      nsort_list_destroy (lh);
      return _ERROR_;
    }
  }

  /* [EndDoc] */

  free (cp);
  free (cpp);
  nsort_elapsed (&t2);
  printf ("  Populated the list in %6.4f seconds\n", t2-t1);
  printf ("  The list has %zu items in it\n", lh->number);
  /*
   * [BeginDoc]
   *
   * Next, we are going to open the original file from which we filled the
   * list and compare it to the contents of the list.
   * [Verbatim] */

  fp = fopen (argv[1], "r");
  if (0 == fp) {
#if defined(__solaris__)
    strncat (str, strerror(errno), ERROR_LEN);
#else
    strerror_r (errno, str, ERROR_LEN);
#endif
    printf ("\n\n***Error: opening %s: %s",
        argv[1], str);
    return _ERROR_;
  }
  nsort_elapsed (&t1);
  isWarning = FALSE;
  lnk = lh->head->next;
  while (lnk != lh->tail) {
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
      printf ("***Warning: %s and %s should not be different\n",
          str, (char*)lnk->data);
    }
    if (feof(fp))
      break;
    lnk = lnk->next;
  }
  nsort_elapsed (&t2);
  fclose (fp);

  /* [EndDoc] */
  /*
   * [BeginDoc]
   *
   * To understand this code, you have to understand a little about the
   * make up of a list.  The ``lh'' variable here is a list header, which
   * contains information about the list, including a head and tail, which
   * demarcates where the list begins and ends, respectively, and a current
   * pointer which points to the ``current'' item.  Each of the elements
   * between the head and tail are elements of time nsort_link_t, or links.
   * \index{nsort_link_t}
   * In this case, we have a variable of type nsort_link_t called lnk.  We
   * set lnk to the first item in the list (which is lh->head->next).  Then,
   * we iterate through the list, setting lnk to lnk->next until lnk is
   * lnk->tail, at which point we have iterated through the contents of the
   * list.  On each iteration, we are comparing what is pointed to by the
   * data pointer (lnk->data) with the contents of the file.
   *
   * If we wented to traverse the list backwards (which we will when we are
   * doing reverse sorted compares), we set lnk to the one before the tail
   * (lnk = lh->tail->prev), on each iteration we go backwards (lnk = lnk->prev)
   * until we are on the head (lh->head), at which point we are through.
   *
   * [EndDoc]
   */
  printf ("  Compared the list in %6.4f seconds\n", t2-t1);
  if (!isWarning)
    printf ("  A comparison between the list and the file succeeded\n");
  else
    printf ("  ***Warning: the list and the file are different.\n");

  printf ("Saving the list to a file...\n");
  nsort_elapsed (&t1);
  /*
   * [BeginDoc]
   *
   * Next, we store the list in a file with nsort_list_save(), destroy it
   * in memory with nsort_list_clear() and nsort_list_del(), retrieve the
   * list once again from file with nsort_list_get() and compare it again
   * to the contents of the the original file, the same as we did above.
   * \index{nsort_list_save} \index{nsort_list_clear} \index{nsort_list_del}
   * \index{nsort_list_get}
   * [Verbatim] */

  status = nsort_list_save (lh, "testing nsort_list_save()", DATASIZE,
      "floglist.dat");
  if (status == _ERROR_) {
    nsort_show_list_error(lh, str, ERROR_LEN);
    printf ("\n\n***Error: nsort_list_save(): %s\n", str);
    return _ERROR_;
  }
  nsort_elapsed (&t2);
  printf ("  Stored the list in %6.4f seconds\n", t2-t1);
  printf ("Deleting the in-memory copy...\n");
  nsort_list_clear (lh);
  nsort_list_del (lh);
  printf ("Getting the list from file...\n");
  nsort_elapsed (&t1);
  status = nsort_list_get (lh, "floglist.dat");
  if (status == _ERROR_) {
    nsort_show_list_error(lh, str, ERROR_LEN);
    printf ("\n\n***Error: nsort_list_get(): %s\n", str);
    return _ERROR_;
  }
  nsort_elapsed (&t2);
  printf ("  Reread the list in %6.4f seconds\n", t2-t1);
  printf ("  Our new list has %zu items in it\n", lh->number);
  lnk = lh->head->next;
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
  while (lnk != lh->tail) {
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
      printf ("***Warning: %s and %s should not be different\n",
          str, (char*)lnk->data);
    }
    if (feof(fp))
      break;
    lnk = lnk->next;
  }
  fclose (fp);
  nsort_elapsed (&t2);
  printf ("  Recompared the list in %6.4f seconds\n", t2-t1);
  if (!isWarning)
    printf ("  A comparison between the new "
        "list and the file succeeded\n");
  else
    printf ("  ***Warning: the new list and "
        "the file are different\n");
  printf ("Cleaning up now...\n");

  /* [EndDoc] */
  /*
   * [BeginDoc]
   *
   * Finally, we clean up.  This involves clearing out the list with
   * nsort_list_clear(), destroying the elements of the list with
   * nsort_list_del() and finally destroying the list object with
   * nsort_list_destroy().
   * \index{nsort_list_destroy}
   * [Verbatim] */

  nsort_list_clear (lh);
  nsort_list_del (lh);
  if (nsort_list_destroy(lh) == _ERROR_)
    return _ERROR_;

  /* [EndDoc] */

#ifdef MALLOC_DEBUGGING
  dmalloc_shutdown();
#endif
  print_block_list();
  return 0;
}
