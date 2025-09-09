/* Source File: flogcmp.c */

#include <sorthdr.h>

#if defined(__TINYC__) || defined(__CINT__)
#undef HAVE_PTHREAD_H
// #include "sort.c"
#endif

/*
 * The purpose of this program is to do some testing using hashes to enhance
 * the capability of compare functions.  On my architecture, the HASH_STR_4()
 * macro and compare function gives the best results in terms of speed.
 */

#define HASH_STR_3(x) \
  (((x)[0]<<10)+((x)[1]<<5)+((x)[2]))
#define HASH_STR_4(x) \
  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+((x)[3]))
#define HASH_STR_5(x) \
  (((x)[0]<<20)+((x)[1]<<15)+((x)[2]<<10)+((x)[3]<<5)+((x)[4]))
#define HASH_STR_6(x) \
  (((x)[0]<<25)+((x)[1]<<20)+((x)[2]<<15)+((x)[3]<<10)+((x)[4]<<5)+((x)[5]))

 int str_0_compare (void *p1, void *p2)
{
  return (strcmp ((char*)p1, (char*)p2));
}

 int str_3_compare (void *p1, void *p2)
{
  unsigned long v1 = HASH_STR_3 ((char*)p1),
                v2 = HASH_STR_3 ((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char*)p1+3, (char*)p2+3));
}

 int str_4_compare (void *p1, void *p2)
{
  unsigned long v1 = HASH_STR_4 ((char*)p1),
                v2 = HASH_STR_4 ((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char*)p1+4, (char*)p2+4));
}

 int str_5_compare (void *p1, void *p2)
{
  unsigned long v1 = HASH_STR_5 ((char*)p1),
                v2 = HASH_STR_5 ((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char*)p1+5, (char*)p2+5));
}

 int str_6_compare (void *p1, void *p2)
{
  unsigned long v1 = HASH_STR_6 ((char*)p1),
                v2 = HASH_STR_6 ((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char*)p1+6, (char*)p2+6));
}

int create_link_pool (nsort_link_t ***lnks, nsort_link_t **lnkp, int count)
{
  int i;

  *lnkp = malloc (count*sizeof (nsort_link_t));
  if (0 == lnkp) {
    return _ERROR_;
  }
  memset (*lnkp, 0, count*sizeof (nsort_link_t));
  *lnks = malloc (count*sizeof (nsort_link_t*));
  if (0 == lnks) {
    free (lnkp);
    return _ERROR_;
  }
  memset (*lnks, 0, count*sizeof (nsort_link_t*));
  for (i = 0; i < count; i++)
    (*lnks)[i] = (nsort_link_t*)(((char*)(*lnkp))+i*sizeof(nsort_link_t));
  return _OK_;
}

int destroy_link_pool (nsort_link_t **lnks, nsort_link_t *lnkp)
{
  free (lnks);
  free (lnkp);
  return _OK_;
}

#define DATASIZE 40
#define ERROR_SIZE 512
#define MAXDATA 10000000

int main (int argc, char *argv[])
{
  FILE *fp;
  char *ln, *cp;
  struct stat statbuf;
  char **cpp;
  char data[DATASIZE+1];
  nsort_link_t **lnks;
  nsort_link_t *lnkp;
  nsort_link_t *lnk;
  int isOK;
  nsort_t *srt;
  int status;
  double t1, t2;
  int counter;
  nsort_node_level_t lvl;
  int i;
  int *compares;
  int maxCompares;
  double avgCompares;
  char str[ERROR_SIZE+1];
  int totalcount;

  if (argc != 3) {
    printf ("\n\nUsage: %s <file> <file.srt>\n", argv[0]);
    printf ("\tWhere <file> is the file to sort\n");
    printf ("\tand <file.srt> is the sorted file to compare to.\n\n");
    return 1;
  }
  Assert (argv[1] != NULL && argv[1][0] != '\0');
  if (argv[1] == NULL || argv[1][0] == '\0') {
    printf ("\n\n***argv[1] is invalid\n");
    return _ERROR_;
  }
  fp = fopen (argv[1], "r");
  if (fp == NULL) {
    printf ("\n***Error: couldn't open %s\n", argv[1]);
    return -1;
  }
  stat (argv[1], &statbuf);
  cp = malloc ((size_t) statbuf.st_size+1);
  if (cp == 0) {
    printf ("\n\n***Error: couldn't allocate file buffer\n");
    fclose (fp);
    return _ERROR_;
  }
  fread (cp, (size_t)statbuf.st_size, 1, fp);
  check_pointer (cp);
  fclose (fp);
  cpp = malloc (MAXDATA*sizeof(char*));
  if (cpp == 0) {
    printf ("\n\n***Error: memory error allocating pointer list\n");
    free (cp);
    return _ERROR_;
  }
  memset (cpp, 0, MAXDATA*sizeof(char *));
  check_pointer (cpp);
  totalcount = nsort_text_file_split (cpp, MAXDATA, cp, '\n');

  compares = malloc (totalcount*sizeof(int));
  if (compares == 0) {
    printf ("\n\n***Error: allocating compares array\n");
    free (cp);
    free (cpp);
    return _ERROR_;
  }
  memset (compares, 0, totalcount*sizeof(int));

  status = create_link_pool (&lnks, &lnkp, totalcount);
  if (status == _ERROR_) {
    printf ("\n\n***Error: creating link pool\n");
    free (cp);
    free (cpp);
    free (compares);
    return _ERROR_;
  }
  ln = malloc (totalcount*DATASIZE);
  if (0 == ln) {
    printf ("\n\n***Error: couldn't allocate string store\n");
    free (cp);
    free (cpp);
    free (compares);
    destroy_link_pool (lnks, lnkp);
    return _ERROR_;
  }
  memset (ln, 0, totalcount*DATASIZE);

  srt = nsort_create ();
  if (srt == NULL) {
    nsort_show_error (str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_create(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    return _ERROR_;
  }
  status = nsort_init (srt, str_0_compare, FALSE, FALSE);
  if (status == _ERROR_) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_init(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    nsort_destroy (srt);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  counter = 0;
  i = 0;
  while (cpp[counter] != 0 && counter < totalcount) {
    int len = strlen (cpp[counter]);
    if (len == 0) {
      counter++;
      continue;
    }
    if (len < 7) {
      printf ("***Warning: %s is less than 7 chars in length...skipping\n",
          cpp[counter]);
      counter++;
      continue;
    }
    strncpy (ln+counter*DATASIZE, cpp[counter], DATASIZE-1);
    lnks[i]->data = ln+counter*DATASIZE;
    status = nsort_add_item (srt, lnks[i]);
    if (status == _ERROR_) {
      free (cp);
      free (cpp);
      free (compares);
      nsort_del (srt, 0);
      nsort_destroy (srt);
      destroy_link_pool (lnks, lnkp);
      free (ln);
      return _ERROR_;
    }
    i++;
    compares[counter] = srt->numCompares;
    counter++;
  }
  nsort_elapsed (&t2);
  nsort_node_levels (srt, &lvl);

  printf ("\nHash_0 compare");
  printf ("\nSucceeded...%zu lines stored\n", srt->lh->number);
  printf ("  Total add time: %f\n", t2 - t1);
  printf ("  Number of nodes = %zu\n", srt->numNodes);
  printf ("  Node levels: ");
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    printf ("%d %d, ", i, lvl.lvl[i]);
  printf ("\n");

  maxCompares = 0;
  avgCompares = 0.0;
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
    avgCompares += compares[i];
  }
  avgCompares /= counter;
  printf ("  Compares during adds: max = %d, avg = %f\n",
          maxCompares, avgCompares);
  printf ("  Number of restructures during adds: %zu\n\n",
      srt->numRestruct);

  /*
   * Now, compare to the sorted file
   */
  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }

  lnk = srt->lh->head->next;
  isOK = TRUE;
  while (!feof (fp) && lnk != 0) {
    cp = fgets (data, DATASIZE, fp);
    if (0 == cp) {
      break;
    }
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) lnk->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
              (char *) data, (char *) lnk->data);
      isOK = FALSE;
    }
    lnk = lnk->next;
  }

  if (!isOK) {
    printf ("\n***Problem with sort data during "
        "ordered compare, original filling\n");
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }
  fclose (fp);

  /*
   * HASH_3
   */
  nsort_del (srt, 0);
  nsort_destroy (srt);
  memset (compares, 0, totalcount*sizeof(int));
  memset (ln, 0, totalcount*DATASIZE);

  srt = nsort_create ();
  if (srt == NULL) {
    nsort_show_error (str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_create(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    return _ERROR_;
  }
  status = nsort_init (srt, str_3_compare, FALSE, FALSE);
  if (status == _ERROR_) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_init(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    nsort_destroy (srt);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  counter = 0;
  i = 0;
  while (cpp[counter] != 0 && counter < totalcount) {
    int len = strlen (cpp[counter]);
    if (len == 0) {
      counter++;
      continue;
    }
    if (len < 7) {
      printf ("***Warning: %s is less than 7 chars in length...skipping\n",
          cpp[counter]);
      counter++;
      continue;
    }
    strncpy (ln+counter*DATASIZE, cpp[counter], DATASIZE-1);
    lnks[i]->data = ln+counter*DATASIZE;
    status = nsort_add_item (srt, lnks[i]);
    if (status == _ERROR_) {
      free (cp);
      free (cpp);
      free (compares);
      nsort_del (srt, 0);
      nsort_destroy (srt);
      destroy_link_pool (lnks, lnkp);
      free (ln);
      return _ERROR_;
    }
    i++;
    compares[counter] = srt->numCompares;
    counter++;
  }
  nsort_elapsed (&t2);
  nsort_node_levels (srt, &lvl);

  printf ("\nHash_3 compare");
  printf ("\nSucceeded...%zu lines stored\n", srt->lh->number);
  printf ("  Total add time: %f\n", t2 - t1);
  printf ("  Number of nodes = %zu\n", srt->numNodes);
  printf ("  Node levels: ");
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    printf ("%d %d, ", i, lvl.lvl[i]);
  printf ("\n");

  maxCompares = 0;
  avgCompares = 0.0;
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
    avgCompares += compares[i];
  }
  avgCompares /= counter;
  printf ("  Compares during adds: max = %d, avg = %f\n",
          maxCompares, avgCompares);
  printf ("  Number of restructures during adds: %zu\n\n",
      srt->numRestruct);

  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }


  lnk = srt->lh->head->next;
  isOK = TRUE;
  while (!feof (fp) && lnk != 0) {
    cp = fgets (data, DATASIZE, fp);
    if (0 == cp) {
      break;
    }
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) lnk->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
              (char *) data, (char *) lnk->data);
      isOK = FALSE;
    }
    lnk = lnk->next;
  }

  if (!isOK) {
    printf ("\n***Problem with sort data during compare\n");
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }
  fclose (fp);

  /*
   * HASH_4
   */
  nsort_del (srt, 0);
  nsort_destroy (srt);
  memset (compares, 0, totalcount*sizeof(int));
  memset (ln, 0, totalcount*DATASIZE);

  srt = nsort_create ();
  if (srt == NULL) {
    nsort_show_error (str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_create(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    return _ERROR_;
  }
  status = nsort_init (srt, str_4_compare, FALSE, FALSE);
  if (status == _ERROR_) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_init(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    nsort_destroy (srt);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  counter = 0;
  i = 0;
  while (cpp[counter] != 0 && counter < totalcount) {
    int len = strlen (cpp[counter]);
    if (len == 0) {
      counter++;
      continue;
    }
    if (len < 7) {
      printf ("***Warning: %s is less than 7 chars in length...skipping\n",
          cpp[counter]);
      counter++;
      continue;
    }
    strncpy (ln+counter*DATASIZE, cpp[counter], DATASIZE-1);
    lnks[i]->data = ln+counter*DATASIZE;
    status = nsort_add_item (srt, lnks[i]);
    if (status == _ERROR_) {
      free (cp);
      free (cpp);
      free (compares);
      nsort_del (srt, 0);
      nsort_destroy (srt);
      destroy_link_pool (lnks, lnkp);
      free (ln);
      return _ERROR_;
    }
    compares[counter] = srt->numCompares;
    i++;
    counter++;
  }
  nsort_elapsed (&t2);
  nsort_node_levels (srt, &lvl);

  printf ("\nHash_4 compare");
  printf ("\nSucceeded...%zu lines stored\n", srt->lh->number);
  printf ("  Total add time: %f\n", t2 - t1);
  printf ("  Number of nodes = %zu\n", srt->numNodes);
  printf ("  Node levels: ");
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    printf ("%d %d, ", i, lvl.lvl[i]);
  printf ("\n");

  maxCompares = 0;
  avgCompares = 0.0;
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
    avgCompares += compares[i];
  }
  avgCompares /= counter;
  printf ("  Compares during adds: max = %d, avg = %f\n",
          maxCompares, avgCompares);
  printf ("  Number of restructures during adds: %zu\n\n",
      srt->numRestruct);

  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }

  lnk = srt->lh->head->next;
  isOK = TRUE;
  while (!feof (fp) && lnk != 0) {
    cp = fgets (data, DATASIZE, fp);
    if (0 == cp) {
      break;
    }
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) lnk->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
              (char *) data, (char *) lnk->data);
      isOK = FALSE;
    }
    lnk = lnk->next;
  }

  if (!isOK) {
    printf ("\n***Problem with sort data during "
        "ordered compare, original filling\n");
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }
  fclose (fp);

  /*
   * HASH_5
   */
  nsort_del (srt, 0);
  nsort_destroy (srt);
  memset (compares, 0, totalcount*sizeof(int));
  memset (ln, 0, totalcount*DATASIZE);


  srt = nsort_create ();
  if (srt == NULL) {
    nsort_show_error (str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_create(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    return _ERROR_;
  }
  status = nsort_init (srt, str_5_compare, FALSE, FALSE);
  if (status == _ERROR_) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_init(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    nsort_destroy (srt);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  counter = 0;
  i = 0;
  while (cpp[counter] != 0 && counter < totalcount) {
    int len = strlen (cpp[counter]);
    if (len == 0) {
      counter++;
      continue;
    }
    if (len < 7) {
      printf ("***Warning: %s is less than 7 chars in length...skipping\n",
          cpp[counter]);
      counter++;
      continue;
    }
    strncpy (ln+counter*DATASIZE, cpp[counter], DATASIZE-1);
    lnks[i]->data = ln+counter*DATASIZE;
    status = nsort_add_item (srt, lnks[i]);
    if (status == _ERROR_) {
      free (cp);
      free (cpp);
      free (compares);
      nsort_del (srt, 0);
      nsort_destroy (srt);
      destroy_link_pool (lnks, lnkp);
      free (ln);
      return _ERROR_;
    }
    compares[counter] = srt->numCompares;
    i++;
    counter++;
  }
  nsort_elapsed (&t2);
  nsort_node_levels (srt, &lvl);

  printf ("\nHash_5 compare");
  printf ("\nSucceeded...%zu lines stored\n", srt->lh->number);
  printf ("  Total add time: %f\n", t2 - t1);
  printf ("  Number of nodes = %zu\n", srt->numNodes);
  printf ("  Node levels: ");
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    printf ("%d %d, ", i, lvl.lvl[i]);
  printf ("\n");

  maxCompares = 0;
  avgCompares = 0.0;
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
    avgCompares += compares[i];
  }
  avgCompares /= counter;
  printf ("  Compares during adds: max = %d, avg = %f\n",
          maxCompares, avgCompares);
  printf ("  Number of restructures during adds: %zu\n\n",
      srt->numRestruct);

  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }

  lnk = srt->lh->head->next;
  isOK = TRUE;
  while (!feof (fp) && lnk != 0) {
    cp = fgets (data, DATASIZE, fp);
    if (0 == cp) {
      break;
    }
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) lnk->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
              (char *) data, (char *) lnk->data);
      isOK = FALSE;
    }
    lnk = lnk->next;
  }

  if (!isOK) {
    printf ("\n***Problem with sort data during "
        "ordered compare, original filling\n");
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }
  fclose (fp);

  /*
   * HASH_6
   */
  nsort_del (srt, 0);
  nsort_destroy (srt);
  memset (compares, 0, totalcount*sizeof(int));
  memset (ln, 0, totalcount*DATASIZE);

  srt = nsort_create ();
  if (srt == NULL) {
    nsort_show_error (str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_create(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    return _ERROR_;
  }
  status = nsort_init (srt, str_6_compare, FALSE, FALSE);
  if (status == _ERROR_) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_init(): %s\n", str);
    free (cp);
    free (cpp);
    free (compares);
    nsort_destroy (srt);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  counter = 0;
  i = 0;
  while (cpp[counter] != 0 && counter < totalcount) {
    int len = strlen (cpp[counter]);
    if (len == 0) {
      counter++;
      continue;
    }
    if (len < 7) {
      printf ("***Warning: %s is less than 7 chars in length...skipping\n",
          cpp[counter]);
      counter++;
      continue;
    }
    strncpy (ln+counter*DATASIZE, cpp[counter], DATASIZE-1);
    lnks[i]->data = ln+counter*DATASIZE;
    status = nsort_add_item (srt, lnks[i]);
    if (status == _ERROR_) {
      free (cp);
      free (cpp);
      free (compares);
      nsort_del (srt, 0);
      nsort_destroy (srt);
      destroy_link_pool (lnks, lnkp);
      free (ln);
      return _ERROR_;
    }
    compares[counter] = srt->numCompares;
    i++;
    counter++;
  }
  nsort_elapsed (&t2);
  nsort_node_levels (srt, &lvl);

  printf ("\nHash_6 compare");
  printf ("\nSucceeded...%zu lines stored\n", srt->lh->number);
  printf ("  Total add time: %f\n", t2 - t1);
  printf ("  Number of nodes = %zu\n", srt->numNodes);
  printf ("  Node levels: ");
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    printf ("%d %d, ", i, lvl.lvl[i]);
  printf ("\n");

  maxCompares = 0;
  avgCompares = 0.0;
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
    avgCompares += compares[i];
  }
  avgCompares /= counter;
  printf ("  Compares during adds: max = %d, avg = %f\n",
          maxCompares, avgCompares);
  printf ("  Number of restructures during adds: %zu\n\n",
      srt->numRestruct);

  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }

  lnk = srt->lh->head->next;
  isOK = TRUE;
  while (!feof (fp) && lnk != 0) {
    cp = fgets (data, DATASIZE, fp);
    if (0 == cp) {
      break;
    }
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) lnk->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
              (char *) data, (char *) lnk->data);
      isOK = FALSE;
    }
    lnk = lnk->next;
  }

  if (!isOK) {
    printf ("\n***Problem with sort data during "
        "ordered compare, original filling\n");
    free (cp);
    free (cpp);
    free (compares);
    nsort_del (srt, 0);
    nsort_destroy (srt);
    destroy_link_pool (lnks, lnkp);
    free (ln);
    return _ERROR_;
  }
  fclose (fp);

  free (cp);
  free (cpp);
  free (compares);
  nsort_del (srt, 0);
  nsort_destroy (srt);
  destroy_link_pool (lnks, lnkp);
  free (ln);
  return _OK_;
}

