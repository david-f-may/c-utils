/* Source File: flogthrd.c */

/*
 * [BeginDoc]
 *
 * \subsection{flogthrd.c}
 *
 * Program: flogthrd.c
 * Script: flogthrd.sh
 *
 * This program was written to show how to write a multi-threaded
 * program using the thread safe nsort sort routines.  In flogthrd,
 * the programmer can determine how many threads to use, limited by
 * the need to have the number of threads equally divide into the number
 * of items being sorted.  If you want to use an nsort in a threaded
 * application, you should study this program as an example of how
 * to do so.
 *
 * [EndDoc]
 */
#include "sorthdr.h"

// #ifdef __TINYC__
// #include "sort.c"
// #endif /* __TINYC__ */

#define NUM_THREAD 16
#define MAX_DATA 1000000
#define ERROR_LEN 256

typedef struct _thread_data {
  nsort_t *srt;                  /* shell to store stuff */
  int threadnum;                 /* number of this thread */
  int number;                    /* number of items in cpp */
  int status;                    /* status of this thread */
  char **cpp;                    /* array of strings to sort */
  nsort_link_t **lpp;            /* pre-allocated array of links */
  nsort_link_t *lnks;            /* pre-allocated links */
} threadData;

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

//
// Populate a shell with the contents of the file given by file
// and return a pointer to the populated shell.
//
void *populateSort (void *tt)
{
  int i;
  threadData *t;
  int status;

  t = (threadData *)tt;
  for (i = 0; i < t->number; i++) {
    if (t->lpp[i]->data == 0)
      break;
    status = nsort_add_item (t->srt, t->lpp[i]);
    if (status == _ERROR_) {
      if (t->srt->sortError == SORT_UNIQUE) {
        t->srt->sortError = SORT_NOERROR;
        continue;
      }
      set_sortError (t->srt->sortError);
      t->status = _ERROR_;
      return (void*)0;
    }
  }
  pthread_exit ((void*)0);

  return (void*)0;
}

//
// OK, so we will open the file, read in all the items, split them up among
// the threadData items and then create the threads and pass on a threadData
// to each one.  The shells will get populated by the populateSort function
// and will be in the threadData items.  Then, I will need to merge those
// back together into single shells.  Hopefully, all of that will be quicker
// than just populating a single shell of that many items.
//
int main (int argc, char *argv[])
{
  threadData *tt[NUM_THREAD];
  void *chk_lnks[NUM_THREAD];
  pthread_t th[NUM_THREAD];
  pthread_attr_t attr;
  char *cp;
  char **cpp;
  double t0, t1, t2;
  int status;
  int i, j;
  int printUsage = FALSE;
  FILE *fp;
  struct stat sbuf;
  int totalcount, itemcount, thnum;
  int count[NUM_THREAD];
  nsort_list_t *lhs[NUM_THREAD];
  nsort_list_t *final_lh, *tmp_lh;
  nsort_t *final;
  nsort_link_t *lnk;
  char str[ERROR_LEN+1];

  if (argc != 2)
    printUsage = TRUE;
  if (argv[1] == 0 || argv[1][0] == '\0')
    printUsage = TRUE;
  if (printUsage == TRUE) {
    printf ("\n\nUsage: %s <file>\n", argv[0]);
    printf ("\twhere <file> is the file to sort\n");
    return 1;
  }
  nsort_elapsed (&t0);
  //
  // Grab the contents of the file and parse it out by lines.
  //
  fp = fopen (argv[1], "r");
  if (fp == NULL) {
    printf ("\n\n***Error: couldn't open %s\n", argv[1]);
    return _ERROR_;
  }
  stat (argv[1], &sbuf);
  cp = malloc ((size_t)sbuf.st_size+1);
  if (0 == cp) {
    printf ("\n\n***Error: critical memory error allocating file buffer\n");
    fclose (fp);
    return _ERROR_;
  }
  fread (cp, (size_t)sbuf.st_size, 1, fp);
  check_pointer (cp);
  fclose (fp);
  cpp = malloc (MAX_DATA * sizeof (char *));
  if (0 == cpp) {
    printf ("\n\n***Error: critical memory error allocating char array\n");
    free (cp);
    return _ERROR_;
  }
  memset (cpp, 0, MAX_DATA*sizeof(char*));
  check_pointer (cpp);
  totalcount = nsort_text_file_split (cpp, MAX_DATA, cp, '\n');
  if (totalcount % NUM_THREAD) {
    printf ("\n\n***Error: the number of items should equally divide"
        " into the number of threads\n");
    free (cp);
    free (cpp);
    return _ERROR_;
  }
  itemcount = totalcount/NUM_THREAD;
  printf ("\nProcessing %d items (%d per thread)\n", totalcount, itemcount);

  //
  // Allocate the data slots and populate them as much as possible.
  //
  for (i = 0; i < NUM_THREAD; i++) {
    tt[i] = malloc (sizeof (threadData));
    if (0 == tt[i]) {
      printf ("\n\n***Error: critical memory error in main\n");
      return _ERROR_;
    }
    memset (tt[i], 0, sizeof(threadData));
    tt[i]->threadnum = i;
    tt[i]->number = itemcount;
    tt[i]->srt = nsort_create ();
    if (tt[i]->srt == 0) {
      nsort_show_sort_error (tt[i]->srt, str, ERROR_LEN);
      printf ("\n\n***Error: nsort_create(): %s\n", str);
      free (tt[i]);
      free (cpp);
      free (cp);
      return _ERROR_;
    }
    status = nsort_init (tt[i]->srt, testCompare, FALSE, FALSE);
    if (status == _ERROR_) {
      nsort_show_sort_error(tt[i]->srt, str, ERROR_LEN);
      printf ("\n\n***Error: nsort_init(): %s\n", str);
      free (tt[i]->srt);
      free (tt[i]);
      free (cpp);
      free (cp);
      return _ERROR_;
    }
    tt[i]->cpp = malloc (itemcount * sizeof (char*));
    if (tt[i]->cpp == 0) {
      printf ("\n\n***Error: critical memory error allocating data array\n");
      nsort_del (tt[i]->srt, 0);
      nsort_destroy (tt[i]->srt);
      free (tt[i]);
      free (cpp);
      free (cp);
      return _ERROR_;
    }
    memset (tt[i]->cpp, 0, itemcount * sizeof (char*));
    tt[i]->lnks = malloc (itemcount*sizeof (nsort_link_t));
    if (tt[i]->lnks == 0) {
      printf ("\n\n***Error: memory error allocating array of links\n");
      free (tt[i]->cpp);
      nsort_del (tt[i]->srt, 0);
      nsort_destroy (tt[i]->srt);
      free (tt[i]);
      free (cpp);
      free (cp);
      return _ERROR_;
    }
    memset (tt[i]->lnks, 0, itemcount*sizeof(nsort_link_t));
    chk_lnks[i] = (void*)tt[i]->lnks;
    tt[i]->lpp = malloc (itemcount * sizeof (nsort_link_t*));
    if (tt[i]->lpp == 0) {
      printf ("\n\n***Error: memory error allocating pointer to array of links\n");
      free (tt[i]->lnks);
      free (tt[i]->cpp);
      nsort_del (tt[i]->srt, 0);
      nsort_destroy (tt[i]->srt);
      free (tt[i]);
      free (cpp);
      free (cp);
      return _ERROR_;
    }
    memset (tt[i]->lpp, 0, itemcount*sizeof(nsort_link_t*));
    for (j = 0; j < itemcount; j++)
      tt[i]->lpp[j] = (nsort_link_t*)((char *)(tt[i]->lnks)+(j*sizeof(nsort_link_t)));
  }
  //
  // This looks complex.  That's because it is.  Here's the skinny:
  // Basically, this should split up cpp between each thread's storage
  // area.  When this is finished, all the items pointed to by cpp
  // should be pointed to by tt[?]->cpp[?] and tt[?]->lpp[?].
  //
  for (i = 0, j = 0; cpp[i] != 0 && j < itemcount; j++) {
    for (thnum = 0; thnum < NUM_THREAD; thnum++) {
      tt[thnum]->cpp[j] = cpp[i];
      i++;
      tt[thnum]->lpp[j]->data = tt[thnum]->cpp[j];
      if (cpp[i] == 0)
        break;
    }
  }
  for (i = 0; i < NUM_THREAD; i++)
    check_pointer (tt[i]->lnks);

  nsort_elapsed (&t1);
  //
  // Looks like we're OK...so proceed.
  // Create the attr required and create the threads.
  //
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for (i = 0; i < NUM_THREAD; i++) {
    status = pthread_create (&th[i], &attr, populateSort, tt[i]);
    if (status) {
      printf ("\n\n***Error: pthread_create returned %d\n", status);
      return _ERROR_;
    }
  }
  pthread_attr_destroy (&attr);

  //
  // OK, connect to the threads and wait for them to finish.
  //
  for (i = 0; i < NUM_THREAD; i++) {
    status = pthread_join (th[i], (void**)&count[i]);
    if (status) {
      printf ("\n\n***Error: joining thread %d\n", i);
      return _ERROR_;
    }
  }
 
  //
  // So, I should have all shells populated through separate threads.  I can
  // now merge them to get a single merged shell.
  //
  for (i = 0; i < NUM_THREAD; i++) {
    lhs[i] = nsort_sort_to_list (tt[i]->srt);
    nsort_destroy (tt[i]->srt);
  }

  if (NUM_THREAD > 2) {
    final_lh = nsort_list_create();
    if (final_lh == 0) {
      nsort_show_error(str, ERROR_LEN);
      printf ("\n\n***Error: creating final list: %s\n", str);
      return _ERROR_;
    }
    status = nsort_list_merge_sorted (final_lh, lhs[0], lhs[1],
        testCompare);
    if (status == _ERROR_) {
      nsort_show_list_error(final_lh, str, ERROR_LEN);
      printf ("\n\n***Error: nsort_list_merge_sorted(): %s\n", str);
      return _ERROR_;
    }
    nsort_list_del (lhs[0]);
    nsort_list_destroy (lhs[0]);
    nsort_list_del (lhs[1]);
    nsort_list_destroy (lhs[1]);
    for (i = 2; i < NUM_THREAD; i++) {
      tmp_lh = final_lh;
      final_lh = nsort_list_create();
      if (final_lh == 0) {
        nsort_show_error(str, ERROR_LEN);
        printf ("\n\n***Error: nsort_list_create(): %s\n", str);
        return _ERROR_;
      }
      status = nsort_list_merge_sorted (final_lh, tmp_lh, lhs[i],
          testCompare);
      if (status == _ERROR_) {
        nsort_show_list_error(final_lh, str, ERROR_LEN);
        printf ("\n\n***Error: nsort_list_merge_sorted(): %s\n", str);
        return _ERROR_;
      }
      nsort_list_del (tmp_lh);
      nsort_list_destroy (tmp_lh);
      nsort_list_del (lhs[i]);
      nsort_list_destroy (lhs[i]);
    }
  }
  else {
    final_lh = nsort_list_create ();
    if (final_lh == 0) {
      nsort_show_error (str, ERROR_LEN);
      printf ("\n\n***Error: nsort_list_create(): %s\n", str);
      return _ERROR_;
    }
    status = nsort_list_merge_sorted (final_lh, lhs[0], lhs[1],
        testCompare);
    if (status == _ERROR_) {
      nsort_show_list_error (final_lh, str, ERROR_LEN);
      printf ("\n\n***Error: nsort_list_merge_sorted(): %s\n", str);
      return _ERROR_;
    }
    nsort_list_del (lhs[0]);
    nsort_list_destroy (lhs[0]);
    nsort_list_del (lhs[1]);
    nsort_list_destroy (lhs[1]);
  }
  final = nsort_create();
  if (final == 0) {
    nsort_show_error(str, ERROR_LEN);
    printf ("\n\n***Error: list2Shell(): %s\n", str);
    return _ERROR_;
  }
  status = nsort_list_to_sort (final, final_lh, testCompare);
  if (status == _ERROR_) {
    nsort_show_sort_error(final, str, ERROR_LEN);
    printf ("\n\n***Error: nsort_list_to_sort(): %s\n", str);
    return _ERROR_;
  }
  nsort_elapsed (&t2);
  for (i = 0; i < NUM_THREAD; i++) {
    if (tt[i]->status == _ERROR_) {
      nsort_show_error(str, ERROR_LEN);
      printf ("\n\n***Error: tt[%d]->status == _ERROR_: %s\n",
        i,str);
      return (_ERROR_);
    }
  }
  printf ("\nThere were no errors\n");
  printf ("\n\nAdded %zu items in %f seconds\n", final->lh->number, t2-t1);
  printf ("\n\nTotal time: %f seconds (including allocating memory)\n", t2-t0);
  fp = fopen ("inputsrt.out", "w");
  if (0 == fp) {
    perror ("\n\n***Error: Could not open inputsrt.out");
    goto cleanupAndQuit;
  }
  lnk = final->lh->head->next;
  while (lnk != final->lh->tail) {
    fprintf (fp, "%s\n", (char*)lnk->data);
    lnk = lnk->next;
  }
  fclose (fp);

cleanupAndQuit:

  lnk = nsort_list_remove_link (final->lh);
  while (lnk != 0)
    lnk = nsort_list_remove_link (final->lh);
  nsort_del (final, 0);
  for (i = 0; i < NUM_THREAD; i++) {
    free (tt[i]->lpp);
    free (tt[i]->lnks);
    if (chk_lnks[i] != tt[i]->lnks) {
      printf ("\n\n***Warning: tt[%d]->lnks != chk_lnks[%d]\n", i, i);
    }
    free (tt[i]->cpp);
    free (tt[i]);
  }
  nsort_destroy (final);
  free (cpp);
  free (cp);
  // That's all, folks.
  print_block_list();
  pthread_exit ((void*)0);

  return 0;
}
