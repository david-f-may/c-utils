/* Source File: floghash_l.c */

/*
 * Put the hash code through the ringer.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

#define MAXDATA		1000000
#define ERROR_LEN 256

int main (int argc, char *argv[])
{
  FILE *fp;
  struct stat statbuf;
  char *cp;
  char *data;
  char **cpp;
  nsort_link_t *lnk;
  char str[ERROR_LEN+1];
  nsort_list_t lh;
  int status;
  double t1, t2;
  nsort_hash_t hsh;
  int i, num;
  char *tmp;
  int counter;
  int totalcount = 0;
  int min = MAXDATA,
      max = 0;

  if (totalcount) {}
  if (argc != 2) {
    printf ("\nUsage: %s <file> <srch>\n", argv[0]);
    printf ("\t<file> is the name of the file to process.\n");
    return 1;
  }

  Assert (argv[1] != NULL && argv[1][0] != '\0');

  fp = fopen (argv[1], "r");
  if (fp == NULL) {
    printf ("\n***Error: couldn't open %s\n", argv[1]);
    return _ERROR_;
  }
  stat (argv[1], &statbuf);
  cp = malloc ((size_t)statbuf.st_size+1);
  if (0 == cp) {
    printf ("\n\n***Error: critical memory error allocating file buffer\n");
    return _ERROR_;
  }
  fread (cp, (size_t)statbuf.st_size, 1, fp);
  check_pointer (cp);
  fclose (fp);
  cpp = malloc (MAXDATA*sizeof(char*));
  if (0 == cpp) {
    printf ("\n\n***Error: memory error allocating cpp\n");
    free (cp);
    return _ERROR_;
  }
  memset (cpp, 0, MAXDATA*sizeof(char*));
  check_pointer (cpp);
  totalcount = nsort_text_file_split (cpp, MAXDATA, cp, '\n');
  nsort_hash_init (&hsh, testCompare, 0);
  if (hsh.hashError != SORT_NOERROR) {
    nsort_show_error(str, ERROR_LEN);
    printf ("\n\n***Error: creating hash: %s\n", str);
    return _ERROR_;
  }

  status = nsort_list_init (&lh);
  if (status == _ERROR_) {
    nsort_show_list_error(&lh, str, ERROR_LEN);
    printf ("\n\n***Error: nsort_list_create(): %s\n", str);
    free (cp);
    free (cpp);
    nsort_hash_del (&hsh);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  num = 0;
  for (i = 0; cpp[i] && i < MAXDATA; i++) {
    status = nsort_hash_add_item (&hsh, cpp[i]);
    if (status == _ERROR_) {
      if (hsh.hashError == SORT_UNIQUE) {
        hsh.hashError = SORT_NOERROR;
	      continue;
      }
      else {
        nsort_show_error(str, ERROR_LEN);
        printf ("\n\n***Error: nsort_hash_add_item (&hsh, %s): %s",
            cpp[i], str);
        nsort_hash_del (&hsh);
        nsort_hash_destroy (&hsh);
        free (cp);
        free (cpp);
        return _ERROR_;
      }
    }
    if (hsh.numCompares > num)
      num = hsh.numCompares;
    lnk = malloc (sizeof (nsort_link_t));
    if (0 == lnk) {
      printf ("\n\n***Error: allocating a link\n");
      nsort_hash_del (&hsh);
      nsort_hash_destroy (&hsh);
      free (cpp);
      free (cp);
      return _ERROR_;
    }
    data = malloc (strlen (cpp[i])+1);
    if (0 == data) {
      printf ("\n\n***Error: allocating a string\n");
      free (lnk);
      nsort_hash_del (&hsh);
      nsort_hash_destroy (&hsh);
      free (cpp);
      free (cp);
      return _ERROR_;
    }
    strcpy (data, cpp[i]);
    check_pointer (data);
    lnk->data = data;
    check_pointer (lnk);
    nsort_list_insert_link (&lh, lnk);
  }
  nsort_elapsed (&t2);
  free (cp);
  free (cpp);

  printf ("\nAdd time: %f\n", t2 - t1);
  printf ("Succeeded...%d items stored in buckets\n", hsh.number);
  printf ("Max compares on adds: %d\n", num);

  /*
   * Open test file and search for each item in the hash
   */
  for (i = 0; i < NSORT_HASH_SIZE; i++) {
    status = nsort_restructure_nodes (hsh.srts[i]);
    if (_ERROR_ == status) {
      nsort_show_sort_error(hsh.srts[i], str, ERROR_LEN);
      printf ("\n\n***Error: nsort_restructure_nodes(): %s\n", str);
      return _ERROR_;
    }
    num = hsh.srts[i]->lh->number;
    if (num < min)
      min = num;
    if (num > max)
      max = num;
  }
  printf ("There are %d shells, min num = %d, max num = %d\n",
      NSORT_HASH_SIZE, min, max);
  num = 0;
  nsort_elapsed (&t1);
  lnk = lh.head->next;
  while (lnk != lh.tail) {
    data = nsort_hash_find_item (&hsh, (const char *)lnk->data);
    if (hsh.numCompares > num)
      num = hsh.numCompares;
    if (0 == data)
      printf ("Dind't find \"%s\" and should have\n", (char *)lnk->data);
    lnk = lnk->next;
  }
  nsort_elapsed (&t2);

  printf ("Search Time: %f\n", t2 - t1);
  printf ("Max number of compares: %d\n", num);

  /*
   * OK, now look for bogus data.
   */
  printf ("Now searching for bogus data\n");
  tmp = malloc (100);
  if (0 == tmp) {
    printf ("\n\n***Error: fatal error allocating tmp buffer (100 bytes)\n");
    lnk = nsort_list_remove_link (&lh);
    while (lnk != 0) {
      free (lnk->data);
      free (lnk);
      lnk = nsort_list_remove_link (&lh);
    }
    nsort_list_del (&lh);
    nsort_hash_del (&hsh);
    return _ERROR_;
  }
  counter = 0;
  num = 0;
  nsort_elapsed (&t1);
  lnk = lh.tail->prev;
  while (lnk != lh.head) {
    strncpy (tmp, (char *)lnk->data, 3);
    tmp[3] = '\0';
    strcat (tmp, "ThisIsBogus");
    data = nsort_hash_find_item (&hsh, (const char *)tmp);
    if (hsh.numCompares > num)
      num = hsh.numCompares;
    if (data != 0)
      printf ("Actually found bogus data %s\n", tmp);
    counter ++;
    if (counter % 5000 == 0) {
      printf (".");
      fflush (stdout);
    }
    lnk = lnk->prev;
  }
  printf ("\n");
  nsort_elapsed (&t2);

  printf ("Search Time for %d bogus data items: %f\n", counter, t2 - t1);
  printf ("Maximum number of compares: %d\n", num);
  printf ("\n\n");

  free (tmp);
  nsort_list_clear (&lh);
  nsort_list_del (&lh);
  nsort_hash_del (&hsh);
  print_block_list ();
  return 0;
}
