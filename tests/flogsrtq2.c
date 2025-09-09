/*	Source File:	flogsrtq2.c	*/

/*
 * [BeginDoc]
 *
 * \subsection{flogsrtq2.c}
 *
 * Program: flogsrtq2.c
 * Script: flogsrtq2.sh
 *
 * This program is designed to test the nsort_list_qsort() function.
 * [EndDoc]
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sorthdr.h"


// #ifdef __TINYC__
// #include "sort.c"
// #endif

#define HASH_STR(x)  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+(x)[3])

 int testCompare (void *p1, void *p2)
{
  unsigned int v1 = HASH_STR((char*)p1),
               v2 = HASH_STR((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char *) p1, (char *) p2));
}

#define DATASIZE 40
#define ERROR_SIZE 256

int main (int argc, char *argv[])
{
  FILE *fp;
  char *data;
  char *ln, *cp;
  double t1, t2;
  nsort_t *srt;
  int numItems = 0;
  nsort_list_t *lh;
  nsort_link_t *lnk;
  int isOK = TRUE;
  char str[ERROR_SIZE+1];
  int status = 0;

  if (status) {}

  if (argc != 5) {
    printf ("\nUsage: %s num <file> <file.srt> <file.rev.srt>", argv[0]);
    printf ("\twhere num is the number of items to work with\n");
    printf ("\t  and <file> is the name of the file to read\n");
    return 1;
  }

  Assert (argv[1] != 0 && argv[1][0] != '\0');
  if (argv[1] == 0 || argv[1][0] == '\0') {
    printf ("\n\n***Error: argv[1] is invalid\n");
    return _ERROR_;
  }
  Assert (argv[2] != 0 && argv[2][0] != '\0');
  if (argv[2] == 0 || argv[2][0] == '\0') {
    printf ("\n\n***Error: argv[2] is invalid\n");
    return _ERROR_;
  }
  Assert (argv[3] != 0 && argv[3][0] != '\0');
  if (argv[3] == 0 || argv[3][0] == '\0') {
    printf ("\n\n***Error: argv[3] is invalid\n");
    return _ERROR_;
  }
  Assert (argv[4] != 0 && argv[4][0] != '\0');
  if (argv[4] == 0 || argv[4][0] == '\0') {
    printf ("\n\n***Error: argv[4] is invalid\n");
    return _ERROR_;
  }
  numItems = atoi (argv[1]);
  if (numItems == 0 || numItems == -1) {
    printf ("\n***Error: couldn't convert %s to an integer value\n", argv[1]);
    return _ERROR_;
  }
  printf ("\nUsing %d items\n", numItems);
  /*
   * Set up the list
   */
  lh = nsort_list_create ();
  status = nsort_list_init (lh);
  if (lh == 0) {
    nsort_show_list_error (lh, str, ERROR_SIZE);
    printf ("\n\n***Error: creating list, %s\n", str);
    return _ERROR_;
  }
  fp = fopen (argv[2], "r");
  if (fp == 0) {
    printf ("\n***Error: couldn't open %s\n", argv[2]);
    return _ERROR_;
  }
  data = (char*)malloc (512);
  if (0 == data) {
    printf ("\n***Error: memory exhausted\n");
    nsort_list_del (lh);
    free (lh);
    return _ERROR_;
  }
  check_pointer (data);
  while (!feof (fp)) {
    cp = fgets (data, 80, fp);
    if (0 == cp)
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    ln = (char*)malloc (DATASIZE);
    if (0 == ln) {
      printf ("\n***Error: memory exhausted\n");
      /* let the OS unmalloc memory */
      return _ERROR_;
    }
    strncpy (ln, data, DATASIZE);
    ln[DATASIZE - 1] = '\0';
    check_pointer (ln);

    lnk = (nsort_link_t*)malloc (sizeof (nsort_link_t));
    if (0 == lnk) {
      printf ("\n\n***Error: dynamic memory exhausted\n");
      return _ERROR_;
    }
    lnk->data = ln;

    lh->current = lh->tail->prev;
    nsort_list_insert_link (lh, lnk);
  }
  fclose (fp);
  nsort_elapsed (&t1);
  srt = nsort_create();
  if (srt == 0) {
    nsort_show_error(str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_create(): %s\n", str);
    return _ERROR_;
  }
  status = nsort_list_qsort (srt, lh, DATASIZE, testCompare);
  nsort_elapsed (&t2);
  if (srt->sortError != SORT_NOERROR) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: populating sort with nsort_list_qsort(): %s\n",
	    str);
    return _ERROR_;
  }
  printf ("\n\nTime to sort %zu items: %f sec.\n", srt->lh->number, t2 - t1);

  /* do ordered and reverse ordered compares */
  fp = fopen (argv[3], "r");
  if (0 == fp) {
    printf ("\n\n***Error: couldn't open %s\n", argv[3]);
    return _ERROR_;
  }
  nsort_elapsed (&t1);
  lnk = srt->lh->head->next;
  while (!feof (fp) && lnk != 0) {
    cp = fgets (data, 80, fp);
    if (0 == cp)
      break;
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) lnk->data)) {
      printf ("Bad compare: \"%s\" - \"%s\"\n", data, (char *) lnk->data);
      isOK = FALSE;
    }
    lnk = lnk->next;
  }

  if (!isOK) {
    printf ("\n***Problem with forward sort\n");
    return _ERROR_;
  }
  fclose (fp);
  fp = fopen (argv[4], "r");
  if (0 == fp) {
    printf ("\n\n***Error: couln't open %s\n", argv[4]);
    return _ERROR_;
  }
  lnk = srt->lh->tail->prev;
  while (!feof (fp) && lnk != 0) {
    cp = fgets (data, 80, fp);
    if (0 == cp)
      break;
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) lnk->data)) {
      printf ("Bad compare: \"%s\" - \"%s\"\n", data, (char *) lnk->data);
      isOK = FALSE;
    }
    lnk = lnk->prev;
  }
  nsort_elapsed (&t2);

  if (!isOK) {
    printf ("\n***Problem with reverse compare\n");
    return _ERROR_;
  }
  fclose (fp);

  printf ("Compare time: %f\n", t2 - t1);


  free (data);
  nsort_del (srt, 0);
  free (srt);
  print_block_list ();
  return _OK_;
}
