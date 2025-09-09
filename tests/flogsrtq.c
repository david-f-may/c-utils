/*	Source File:	flogsrtq.c	*/

/*
 * [BeginDoc]
 *
 * \subsection{flogsrtq.c}
 *
 * Program: flogsrtq.c
 * Script: flogsrtq.sh
 *
 * The purpose of this program is to test the nsort_qsort() function.
 * This function uses a fast non-recursive qsort to sort data and then
 * creates a shell from the sorted data.  Overall, an nsort sort is typically
 * faster than a system provided qsort.  These tend to be recursive
 * implementations and the overhead of recursively calling qsort tends to chew
 * up some time.  The qsort provided here with the nsort code, however, is
 * very fast indeed.  For those instances where you are initially sorting many
 * random items (like 500K-1Million items), it may make sense to use this fast qsort
 * implementation rather than nsort.
 *
 * [EndDoc]
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sorthdr.h"


#define DATASIZE 40
#define ERROR_SIZE 256

#define HASH_STR(x)  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+(x)[3])

 int qtestCompare (void *p1, void *p2)
{
  unsigned int v1 = HASH_STR((char*)p1),
               v2 = HASH_STR((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strncmp ((char *)p1, (char *)p2, DATASIZE));
}

// #ifdef __TINYC__
// #include "sort.c"
// #endif

int main (int argc, char *argv[])
{
  FILE *fp;
  char *data;
  char *ln, *cp;
  char **qsortArray;
  double t1, t2;
  nsort_t *srt;
  int numItems = 0;
  register int counter;
  int status = 0;
  char str[ERROR_SIZE+1];
  nsort_link_t *lnk;

  if (status) {}

  if (argc != 5) {
    printf ("\nUsage: %s num <file> <file.srt> <file.rev.srt>", argv[0]);
    printf ("\twhere num is the number of items to work with\n");
    printf ("\t  and <file> is the name of the file to read\n");
    return 1;
  }

  numItems = atoi (argv[1]);
  if (numItems == 0 || numItems == -1) {
    printf ("\n***Error: couldn't convert %s to an integer value\n", argv[1]);
    return _ERROR_;
  }
  printf ("\nUsing %d items\n", numItems);
  /*
   * Set up the arrays
   */
  qsortArray = (char**)malloc (numItems * sizeof (char *));
  if (0 == qsortArray) {
    printf ("\n***Error: memory exhausted\n");
    return _ERROR_;
  }
  memset (qsortArray, 0, numItems * sizeof (char *));
  check_pointer (qsortArray);

  /*
   * File 1 - the unsorted file
   */
  fp = fopen (argv[2], "r");
  if (fp == 0) {
    printf ("\n***Error: couldn't open %s\n", argv[2]);
    return _ERROR_;
  }
  data = (char*)malloc (512);
  if (0 == data) {
    printf ("\n***Error: memory exhausted\n");
    free (qsortArray);
    return _ERROR_;
  }
  check_pointer (data);
  /*
   * Note: the time here is not reflective of the raw speed of the qsort algorithm.
   * This should probably be redesigned to read the entire file at once - the speed
   * would be better.  I will work on that as soon as time permits.
   */
  ln = (char*)malloc (numItems * DATASIZE);
  if (0 == ln) {
    printf ("\n\n***Error: allocating space for the strings\n");
    free (qsortArray);
    return _ERROR_;
  }
  counter = 0;
  while (!feof (fp)) {
    cp = fgets (data, 80, fp);
    if (0 == cp)
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    strncpy (ln+counter*DATASIZE, data, DATASIZE);
    qsortArray[counter] = ln+counter*DATASIZE;

    if (++counter >= numItems)
      break;
  }
  fclose (fp);

  /*
   * [BeginDoc]
   *
   * The following is the set up and and call to the nsort_qsort() function.
   * You need to create the sort object using nsort_create() or by declaring it
   * an automatic variable.  Then, pass the argument as part of the function call.
   * There is a need for two function pointers because one is for the qsort function
   * and the other becomes srt->compare.  In this case, as in most concievable cases,
   * the two can be the same function.
   *
   * [Verbatim] */

  nsort_elapsed (&t1);
  srt = nsort_create ();
  if (srt == 0) {
    nsort_show_error (str, ERROR_SIZE);
    printf ("\n\n***Error: creating nsort: %s\n", str);
    return _ERROR_;
  }
  status = nsort_qsort (srt, qsortArray, numItems, sizeof (char *),
		  qtestCompare, qtestCompare);

  /* [EndDoc] */
  nsort_elapsed (&t2);
  if (srt->sortError != SORT_NOERROR) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: populating sort with nsort_qsort: %s\n",
	    str);
    free (qsortArray);
    free (srt);
    free (data);
    return _ERROR_;
  }
  srt->manageAllocs = FALSE;
  printf ("\n\nTime to sort %zu items: %f sec.\n", srt->lh->number, t2 - t1);

  // BUGBUG - I desperately need to compare the resultant sorted list
  // with the original data set.
  srt->lh->current = srt->lh->head->next;
  lnk = nsort_list_remove_link(srt->lh);
  while (lnk != 0) {
    free (lnk);
    lnk = nsort_list_remove_link (srt->lh);
  }
  free (ln);
  nsort_del (srt, 0);
  free (srt);
  free (data);
  free (qsortArray);
  print_block_list();
  return _OK_;
}
