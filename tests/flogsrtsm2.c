/* Source File: flogsrtsm.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sorthdr.h"

#define HASH_STR(x)  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+(x)[3])


int fexists (const char *path)
{
    struct stat buf;
    if (lstat (path, &buf) == 0)
        return 1;
    else
        return 0;
}


int srtcompare (void *p1, void *p2)
{
  unsigned int v1 = HASH_STR((char*)p1),
               v2 = HASH_STR((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char *)p1, (char*)p2));
}

#define MAX_DATA 1000000 // 1 million lines
#define ERROR_LEN 256
#define ARBITRARY_SIZE 4096
char data[ARBITRARY_SIZE+1];

int main (int argc, char *argv[])
{
  FILE *fp;
  nsort_t *shl;
  nsort_list_t *lh;
  nsort_link_t *lnk;
  nsort_link_t *found;
  struct stat statbuf;
  char *cp;
  char **cpp;
  char *this_cp;
  char str[ERROR_LEN+1];
  int i, numitems;
  int status;
  int counter = 0;
  static int is_problemas = FALSE;

  if (argc != 3) {
    printf ("\n\n***Usage: %s numitems file\n", argv[0]);
    printf ("\twhere numitems is the number of items to sort and\n");
    printf ("\tfile is a file with a list of strings to add to the shell.\n");
    return _ERROR_;
  }
  numitems = atoi (argv[1]);
  if (numitems < 0 || numitems > MAX_DATA) {
    printf ("\n\n***Error: invalid numitems parameter\n");
    return _ERROR_;
  }
  lh = nsort_list_create ();
  if (0 == lh) {
    nsort_show_error (str, ERROR_LEN);
    printf ("\n\n***Error: nsort_list_create(): %s\n", str);
    return _ERROR_;
  }
  status = nsort_list_init (lh);
  if (status == _ERROR_) {
    if (nsort_check_error())
      nsort_show_error(str, ERROR_LEN);
    else
      nsort_show_list_error (lh, str, ERROR_LEN);
    printf ("\n\n***Error: initializing list: %s\n", str);
    return _ERROR_;
  }
  shl = nsort_create ();
  if (shl == 0) {
    nsort_show_error (str, ERROR_LEN);
    printf ("\n\n***Error: memory error allocating nsort_t: %s\n", str);
    return _ERROR_;
  }
  if (fexists ((const char *)"flogsrtsm2.srt")) {
    status = nsort_get (shl, srtcompare, "flogsrtsm2.srt");
      if (status == _ERROR_) {
      nsort_show_sort_error (shl, str, ERROR_LEN);
      printf ("\n\n***Error: nsort_init (): %s\n", str);
      nsort_destroy (shl);
      return _ERROR_;
    }
  }
  else {
    status = nsort_init (shl, srtcompare, TRUE, TRUE);
    if (status == _ERROR_) {
      nsort_show_sort_error (shl, str, ERROR_LEN);
      printf ("\n\n***Error: nsort_init (): %s\n", str);
      nsort_destroy (shl);
      return _ERROR_;
    }
  }
  fp = fopen (argv[2], "r");
  if (fp == NULL) {
    printf ("\n\n***Error: could not open %s: ", argv[2]);
    perror (" ");
    return _ERROR_;
  }
  stat (argv[2], &statbuf);
  cp = malloc ((size_t) statbuf.st_size+1);
  if (0 == cp) {
    printf ("\n\n***Error: Could not alloc %ld bytes from the heap\n", statbuf.st_size+1);
    nsort_del (shl, 0);
    nsort_destroy (shl);
    nsort_list_del (lh);
    return _ERROR_;
  }
  fread (cp, (size_t) statbuf.st_size, 1, fp);
  fclose (fp);
  cpp = malloc ((numitems+3)*sizeof (char*));
  if (0 == cpp) {
    printf ("\n\n***Error: couldn't allocate cpp\n");
    free (cp);
    nsort_del (shl, 0);
    nsort_destroy (shl);
    nsort_list_del (lh);
    return _ERROR_;
  }
  status = nsort_text_file_split (cpp, numitems, cp, '\n');
  check_pointer (cpp);
  if (status > numitems) {
    printf ("\n\n***Warning: the number of lines, %d, in %s, is greater than %d", status, argv[2], numitems);
    printf ("We will only process %d items\n", numitems);
  }
  printf ("Populating the list and shell\n");
  for (i = 0; i < numitems; i++) {
    if (cpp[i] == NULL) break;
    int len = strlen (cpp[i]);
    counter++;
    if (len <= 0)
      continue;
    if (counter % 1000 == 0) {
      printf (".");
    }
    this_cp = malloc (len+1);
    if (cp == 0) {
      printf ("\n\n***Error: malloc error allocating item\n");
      free (cp);
      free (cpp);
      nsort_del (shl, 0);
      nsort_destroy (shl);
      nsort_list_clear (lh);
      nsort_list_del (lh);
      return _ERROR_;
    }
    lnk = malloc (sizeof (nsort_link_t));
    if (0 == lnk) {
      printf ("\n\n***Error malloc error allocating a Link\n");
      free (cp);
      free (cpp);
      free (this_cp);
      nsort_del (shl, 0);
      nsort_destroy (shl);
      nsort_list_clear (lh);
      nsort_list_del (lh);
      return _ERROR_;
    }
    strcpy (this_cp, cpp[i]);
    lnk->data = this_cp;
    nsort_list_insert_link (lh, lnk);

    this_cp = malloc (len+1);
    if (cp == 0) {
      printf ("\n\n***Error: malloc error allocating item\n");
      free (cp);
      free (cpp);
      nsort_del (shl, 0);
      nsort_destroy (shl);
      nsort_list_clear (lh);
      nsort_list_del (lh);
      return _ERROR_;
    }
    lnk = malloc (sizeof (nsort_link_t));
    if (0 == lnk) {
      printf ("\n\n***Error malloc error allocating a Link\n");
      free (cp);
      free (cpp);
      free (this_cp);
      nsort_del (shl, 0);
      nsort_destroy (shl);
      nsort_list_clear (lh);
      nsort_list_del (lh);
      return _ERROR_;
    }
    strcpy (this_cp, cpp[i]);
    lnk->data = this_cp;
    status = nsort_add_item (shl, lnk);
    if (status == _ERROR_) {
      if (shl->sortError == SORT_UNIQUE) {
          shl->sortError = SORT_NOERROR;
          continue;
      }
      else {
        nsort_show_sort_error (shl, str, ERROR_LEN);
        printf ("\n\n***Error: nsort_add_item (): %s\n", str);
        free (cp);
        free (cpp);
        free (this_cp);
        free (lnk);
        nsort_del (shl, 0);
        nsort_destroy (shl);
        nsort_list_clear (lh);
        nsort_list_del (lh);
        return _ERROR_;
      }
    }
    // OK, now search for every item in the shell.
    for (lnk = lh->head->next; lnk != lh->tail; lnk = lnk->next) {
      found = nsort_find_item (shl, lnk);
      if (found == 0) {
        printf ("\n\n***Error: Could not find %s\n", (char *)lnk->data);
        is_problemas = TRUE;
      }
    }
  }
  status = nsort_save (shl, "flogsrtsm2.srt", 100, "flogsrtsm2.srt");
  if (status == _ERROR_) {
      nsort_show_sort_error (shl, str, ERROR_LEN);
      printf ("\n***Error, %d: nsort_save(): %s\n", __LINE__, str);
      return -1;
  }
  printf ("\nDone.\nCleaning up.\n\n");
  free (cp);
  free (cpp);
  nsort_del (shl, 0);
  nsort_destroy (shl);
  nsort_list_clear (lh);
  nsort_list_del (lh);
  if (is_problemas) {
    printf ("\nThere were errors\n");
    return _ERROR_;
  }
  return _OK_;
}
