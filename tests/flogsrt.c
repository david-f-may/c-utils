/* Source File: flogsrt.c */

/*
 * Put the sort code through the ringer.
 */

/*
 * [BeginDoc]
 *
 * \subsection{flogsrt.c}
 *
 * Pogram: flogsrt.c
 * Script: flogsrt.sh
 *
 * The flogsrt program exercises the nsort functionality.  Basically, it does the following:
 *
 * \begin{enumerate}
 *
 * \item Create a sort object on the heap.
 * 
 * \item Read a data file into memory and parse it into lines.
 *
 * \item Add each line to the object, which should result in a sorted file.
 *
 * \item For each line in the original file, search through the sort object and flag
 * it as an error if any of the items aren't found.
 *
 * \item Compare the items in the sort object to a file that has been presorted and then
 * reverse compare the items in the sort object to a file that has been reverse sorted.
 *
 * \item Save the sort object as a file, destroy the memory version and then retrieve it
 * again from the file into memory.
 *
 * \item Do the searches and the forward and reverse compares again as above.
 *
 * \item Clean up memory and exit.
 *
 * \end{enumerate}
 *
 * Again, we need to include the nsort.h header file:
 * [Verbatim] */

#include "sorthdr.h"

/* [EndDoc] */

#ifdef DEBUG_DMALLOC
#include <dmalloc.h>
#endif

// #ifdef __TINYC__
// #include "sort.c"
// #endif /* __TINYC__ */

/*
 * [BeginDoc]
 *
 * Any program that uses the nsort sorting functionality has to define a compare
 * function.  This determines the order of the sort.  The nsort functionality is
 * designed to be as flexible as possible, but part of that is that the user has
 * to define the compares.  This allows you to, for example, have complex structures
 * as data items for a sort.  Then, you can write your compare function so that sorts
 * by last name first, but if the last name is the same, then it will sort by first
 * name.  Also, your application may require a dictionary sort or a case insensitive
 * sort.  At any rate, the compare function allows you to do whatever comparison is
 * necessary in your data object so that it is added to the sort object in ``sorted
 * order'' as determined by the application.
 *
 * The compare function for the flogsrt program is as follows:
 * [Verbatim] */

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

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * This is a simple compare function that does a strcmp() of the items being added to
 * the object.  Whatever your compare function looks at, the return value needs to be
 * as follows:
 *
 * \begin{itemize}
 * \item [less than 0] \ldots if p1 is less than p2
 * \item [equal to 0] \ldots if p1 is the same as p2
 * \item [greater than 0] \ldots if p1 is greater than p2
 * \end{itemize}
 *
 * Again, ``less than'', ``equal to'' and ``greater than'' are determined based on the
 * needs of the application.
 *
 * [EndDoc]
 */
 void printDump (void *data)
{
  printf ("%s\n", (char *) data);
}

#define	DATASIZE	40
#define MAXDATA		1000000
#define ERROR_SIZE 512

int main (int argc, char *argv[])
{
  FILE *fp;
  char *ln, *cp;
  struct stat statbuf;
  char **cpp;
  nsort_link_t *lnk = 0, *found = 0;
  int status;
  double t1, t2;
  int isOK = TRUE;
  nsort_t *srt;
  int counter;
  nsort_node_level_t lvl;
  int i;
  int *compares;
  int maxCompares;
  double avgCompares;
  char data[ERROR_SIZE+1];
  char str[ERROR_SIZE+1];
  int totalcount;
#ifdef NSORT_STATS
  double *traversal_times;
  double maxTraversals;
  double avgTraversals;
#endif

#ifdef DEBUG_MALLOC
  dmalloc_debug_setup("log_stats,log-non-free,check-fence,lockon=20,log=dmalloc.%p");
#endif
  if (argc != 4) {
    printf ("\nUsage: %s <file> <file.srt> <file.rev.srt>\n", argv[0]);
    printf ("\twhere <file> is the name of the file to read\n");
    printf ("\t  and <file.srt> is a sorted version of <file>\n");
    printf ("\t  and <file.rev.srt> is in reverse sorted order.\n\n");
    return 1;
  }

  Assert (argv[1] != NULL && argv[1][0] != '\0');
  if (argv[1] == 0 || argv[1] == 0) {
    printf ("\n\nargv[1] is invalid\n");
    return _ERROR_;
  }
  Assert (argv[2] != NULL && argv[2][0] != '\0');
  if (argv[2] == 0 || argv[2] == 0) {
    printf ("\n\nargv[2] is invalid\n");
    return _ERROR_;
  }
  Assert (argv[3] != NULL && argv[3][0] != '\0');
  if (argv[3] == 0 || argv[3] == 0) {
    printf ("\n\nargv[3] is invalid\n");
    return _ERROR_;
  }

  /*
   * [BeginDoc]
   *
   * Again, a test file is read into memory and parsed by line as follows:
   * [Verbatim] */

  fp = fopen (argv[1], "r");
  if (fp == NULL) {
    printf ("\n***Error: couldn't open %s\n", argv[1]);
    return -1;
  }
  stat (argv[1], &statbuf);
  cp = (char *)malloc ((size_t) statbuf.st_size+1);
  if (cp == 0) {
    printf ("\n\n***Error: couldn't allocate file buffer\n");
    fclose (fp);
    return _ERROR_;
  }
  fread (cp, (size_t)statbuf.st_size, 1, fp);
  check_pointer (cp);
  fclose (fp);
  cpp = (char **)malloc (MAXDATA*sizeof(char*));
  if (cpp == 0) {
    printf ("\n\n***Error: memory error allocating pointer list\n");
    free (cp);
    return _ERROR_;
  }
  memset (cpp, 0, MAXDATA*sizeof(char *));
  check_pointer (cpp);
  totalcount = nsort_text_file_split (cpp, MAXDATA, cp, '\n');

  /* [EndDoc] */

  compares = (int *)malloc (totalcount*sizeof(int));
  if (compares == 0) {
    printf ("\n\n***Error: allocating compares array\n");
    free (cp);
    free (cpp);
    return _ERROR_;
  }
#ifdef NSORT_STATS
  traversal_times = (double*)malloc (totalcount*sizeof(double));
  if (traversal_times == 0) {
    printf ("\n\n***Error: allocating traversal times array\n");
    free (cpp);
    free (cp);
    free (compares);
    return _ERROR_;
  }
#endif


  /*
   * [BeginDoc]
   * An nsort object is created on the heap using the
   * nsort_create() and nsort_init() functions.
   * \index{nsort_create} \index{nsort_init}
   * Then, the object is populated using the nsort_add_item() function.
   * \index{nsort_add_item}
   * [Verbatim] */

  srt = nsort_create ();
  if (srt == NULL) {
    nsort_show_error (str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_create(): %s\n", str);
    return _ERROR_;
  }
  status = nsort_init (srt, testCompare, TRUE, TRUE);
  if (status == _ERROR_) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: nsort_init(): %s\n", str);
    nsort_destroy (srt);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  counter = 0;
  while (cpp[counter] != 0 && counter < totalcount) {
    if (strlen (cpp[counter]) == 0) {
      counter ++;
      continue;
    }
    lnk = (nsort_link_t*)malloc (sizeof (nsort_link_t));
    if (lnk == 0) {
      printf ("\n\n***Error: allocating a link\n");
      free (cp);
      free (cpp);
      free (compares);
#ifdef NSORT_STATS
      free (traversal_times);
#endif
      nsort_del (srt, 0);
      nsort_destroy (srt);
      return _ERROR_;
    }
    ln = (char*)malloc (DATASIZE);
    if (ln == 0) {
      printf ("\n\n***Error: allocating a data block\n");
      free (lnk);
      free (cp);
      free (cpp);
      free (compares);
#ifdef NSORT_STATS
      free (traversal_times);
#endif
      nsort_del (srt, 0);
      nsort_destroy (srt);
      return _ERROR_;
    }
    strncpy (ln, cpp[counter], DATASIZE-1);
    lnk->data = ln;

    status = nsort_add_item (srt, lnk);
    if (status == _ERROR_) {
      if (srt->sortError == SORT_UNIQUE) {
        srt->sortError = SORT_NOERROR;
        free (ln);
        free (lnk);
        counter++;
	      continue;
      }
      free (lnk);
      free (cp);
      free (cpp);
      free (compares);
#ifdef NSORT_STATS
      free (traversal_times);
#endif
      nsort_show_sort_error (srt, str, ERROR_SIZE);
	    printf ("\n\n***Error: nsort_add_item() at %d: %s\n",
          counter, str);
	    nsort_del (srt, 0);
      nsort_destroy (srt);
	    return _ERROR_;
    }
    check_pointer (lnk);
    check_pointer (ln);
    compares[counter] = srt->numCompares;
#ifdef NSORT_STATS
    traversal_times[counter] = srt->traversal_time;
#endif
    counter++;
  }

  /* [EndDoc] */
  /*
   * [BeginDoc]
   * Notice that process is basically the same for adding items to a sorted
   * object as it is for adding items to a list.  Allocate a link  (an item of
   * type nsort_link_t), allocate a data object, have the data member of the
   * link point to the data object (link->data = data), and call nsort_add_item()
   * to add the item to the sorted object.  Again, it is important that the
   * programmer be meticulous in error checking and such.  Otherwise, your program
   * is a time bomb waiting to go off.
   *
   * The error handling for the sort functions is done using the nsort_show_sort_error()
   * function.  It is similar to the nsort_show_list_error() function in the way it
   * is called. \index{nsort_show_sort_error}
   *
   * [EndDoc]
   */
  printf ("\n\nDone\n");
  nsort_elapsed (&t2);
  check_pointer (cp);
  free (cp);
  check_pointer (cpp);
  free (cpp);
  nsort_node_levels (srt, &lvl);

  printf ("\nSucceeded...%zu lines stored\n", srt->lh->number);
  printf ("  Total add time: %f\n", t2 - t1);
  printf ("  Number of nodes = %zu\n", srt->numNodes);
  printf ("  Node levels: ");
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    printf ("%d %d, ", i, lvl.lvl[i]);
  printf ("\n");

  maxCompares = 0;
  avgCompares = 0.0;
#ifdef NSORT_STATS
  maxTraversals = 0.0;
  avgTraversals = 0.0;
#endif
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
#ifdef NSORT_STATS
    if (traversal_times[i] > maxTraversals)
      maxTraversals = traversal_times[i];
#endif
    avgCompares += compares[i];
#ifdef NSORT_STATS
    avgTraversals += traversal_times[i];
#endif
  }
  avgCompares /= counter;
#ifdef NSORT_STATS
  avgTraversals /= counter;
#endif
  printf ("  Compares during adds: max = %d, avg = %f\n",
	  maxCompares, avgCompares);
#ifdef NSORT_STATS
  printf ("  Traversal times furing adds: max = %f, avg = %f\n",
      maxTraversals, avgTraversals);
#endif
  printf ("  Number of restructures during adds: %zu\n\n",
      srt->numRestruct);

  check_pointer (compares);
  printf ("Opening test file and comparing with sort contents\n");

  /*
   * [BeginDoc]
   *
   * Next, we search for each item that we just added to the sort object, as
   * follows:
   * [Verbatim] */

  fp = fopen (argv[1], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n",
        argv[1]);
    return _ERROR_;
  }

  lnk = (nsort_link_t*)malloc (sizeof (nsort_link_t));
  if (0 == lnk) {
    printf ("\n\n***Error: fatal memory error\n");
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }

  ln = (char*)malloc (DATASIZE);
  if (0 == ln) {
    printf ("\n\n***Error: fatal memory error\n");
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }
  nsort_restructure_nodes (srt);
  nsort_elapsed (&t1);
  counter = 0;
  while (!feof (fp)) {
    cp = fgets (data, 80, fp);
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
    strncpy (ln, data, DATASIZE);
    lnk->data = ln;
    found = nsort_find_item (srt, lnk);
    if (0 == found) {
      printf ("Didn't find \"%s\"\n", (char *) ln);
      isOK = FALSE;
    }
    compares[counter] = srt->numCompares;
#ifdef NSORT_STATS
    traversal_times[counter] = srt->traversal_time;
#endif
    counter++;
  }
  nsort_elapsed (&t2);

  /* [EndDoc] */

  if (!isOK) {
    printf ("\n***Problem with sort data, "
        "original filling\n");
    printf ("Could not find some data items "
        "that we should have found\n");
    free (lnk);
    free (ln);
    fclose (fp);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }

  check_pointer (lnk);
  free (lnk);
  check_pointer (ln);
  free (ln);
  fclose (fp);

  printf ("Search Time for searches: %f\n", t2 - t1);

  maxCompares = 0;
  avgCompares = 0.0;
#ifdef NSORT_STATS
  maxTraversals = 0.0;
  avgTraversals = 0.0;
#endif
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
#ifdef NSORT_STATS
    if (traversal_times[i] > maxTraversals)
      maxTraversals = traversal_times[i];
#endif
    avgCompares += compares[i];
#ifdef NSORT_STATS
    avgTraversals += traversal_times[i];
#endif
  }
  avgCompares /= counter;
#ifdef NSORT_STATS
  avgTraversals /= counter;
#endif
  printf ("  Compares during finds: max = %d, avg = %f\n",
	  maxCompares, avgCompares);
#ifdef NSORT_STATS
  printf ("  Traversal times furing finds: max = %f, avg = %f\n",
      maxTraversals, avgTraversals);
#endif
  printf ("  Number of restructures during finds: %zu\n\n",
      srt->numRestruct);


  printf ("Doing sorted and reverse sorted compares\n");

  /*
   * [BeginDoc]
   *
   * Next, we do ordered and reverse ordered compares:
   * [Verbatim] */

  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  found = srt->lh->head->next;
  while (!feof (fp) && found != 0) {
    cp = fgets (data, 80, fp);
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
    if (strcmp (data, (char *) found->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
	      (char *) data, (char *) found->data);
      isOK = FALSE;
    }
    found = found->next;
  }

  if (!isOK) {
    printf ("\n***Problem with sort data during "
        "ordered compare, original filling\n");
    return -1;
  }

  fclose (fp);

  fp = fopen (argv[3], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[3]);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }

  found = srt->lh->tail->prev;
  while (!feof (fp) && found != 0) {
    /*memset (data, 0, 512); */
    cp = fgets (data, 80, fp);
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
    if (strcmp (data, (char *) found->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
	      (char *) data, (char *) found->data);
      isOK = FALSE;
    }
    found = found->prev;
  }
  nsort_elapsed (&t2);

  /* [EndDoc] */

  if (!isOK) {
    printf ("\n***Problem with sort data reverse compares, original filling\n");
    return -1;
  }

  fclose (fp);

  printf ("Compare Time: %f\n", t2 - t1);

  printf ("\nStoring the sorted data\n");

  nsort_elapsed (&t1);

  /*
   * [BeginDoc]
   *
   * Next, we are going to save the sorted object to a file, destroy the
   * memory version of it and retrieve it from file:
   * [Verbatim] */

  status = nsort_save (srt, "Test sort storage functionality",
		      DATASIZE, "sort.dat");
  if (_ERROR_ == status) {
    nsort_show_sort_error(srt, str, ERROR_SIZE);
    printf ("\n***Error: storing data, %s\n", str);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return (_ERROR_);
  }
  nsort_elapsed (&t2);

  printf ("Store Time: %f\n", t2 - t1);

  printf ("\nDeleting and initializing the sort again\n");

  nsort_del (srt, NULL);
  nsort_destroy (srt);

  nsort_elapsed (&t1);
  srt = nsort_create ();
  if (srt == 0) {
    nsort_show_sort_error(srt, str, ERROR_SIZE);
    printf ("\n\n***Error: creating a sort item: %s\n", str);
    return _ERROR_;
  }
  status = nsort_get (srt, testCompare, "sort.dat");
  if (status == _ERROR_) {
    nsort_show_sort_error (srt, str, ERROR_SIZE);
    printf ("\n\n***Error: getting a sort item from file: %s\n",
        str);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    return _ERROR_;
  }

  /* [EndDoc] */
  nsort_elapsed (&t2);

  nsort_node_levels (srt, &lvl);

  printf ("\nTime to re-populate the sort, %f\n", t2 - t1);
  printf ("\nRetrieval of data into sort successful");
  printf ("\n%zu items stored in sort", srt->lh->number);
  printf ("\nNumber of nodes = %zu\n", srt->numNodes);

  printf ("Node levels: ");
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    printf ("%d %d, ", i, lvl.lvl[i]);
  printf ("\n");

  printf ("\nOpening test file and comparing with tree contents\n");

  /*
   * [BeginDoc]
   *
   * Next, we will yet again open the test files and do the searches and compares again.
   * The code isn't shown here, but it is similar to that shown above.
   * 
   * [EndDoc]
   */

  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }

  lnk = (nsort_link_t*)malloc (sizeof (nsort_link_t));
  if (0 == lnk) {
    printf ("\n\n***Error: memory error allocating link\n");
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }
  ln = (char*)malloc (DATASIZE);
  if (0 == ln) {
    printf ("\n\n***Error: critical memory error\n");
    free (lnk);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }
  nsort_elapsed (&t1);

  memset (data, 0, 512);
  counter = 0;
  while (!feof (fp)) {
    cp = fgets (data, 80, fp);
    if (0 == cp) {
      break;
    }
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    if (data[0] == '\0')
      continue;
    strcpy (ln, data);
    lnk->data = ln;
    found = nsort_find_item (srt, lnk);
    if (0 == found) {
      printf ("Didn't find \"%s\"\n", (char *) data);
      isOK = FALSE;
    }
    compares[counter] = srt->numCompares;
#ifdef NSORT_STATS
    traversal_times[counter] = srt->traversal_time;
#endif
    counter++;
  }
  nsort_elapsed (&t2);
  if (!isOK) {
    printf ("\n***Problem with tree data, retieval from data disk\n");
    free (ln);
    free (lnk);
    free (compares);
#ifdef NSORT_STATS
    free (traversal_times);
#endif
    nsort_del (srt, 0);
    nsort_destroy (srt);
    return _ERROR_;
  }
  fclose (fp);
  free (lnk);

  printf ("Search Time: %f\n", t2 - t1);

  maxCompares = 0;
  avgCompares = 0.0;
#ifdef NSORT_STATS
  maxTraversals = 0.0;
  avgTraversals = 0.0;
#endif
  for (i = 0; i < counter; i++) {
    if (compares[i] > maxCompares)
      maxCompares = compares[i];
#ifdef NSORT_STATS
    if (traversal_times[i] > maxTraversals)
      maxTraversals = traversal_times[i];
#endif
    avgCompares += compares[i];
#ifdef NSORT_STATS
    avgTraversals += traversal_times[i];
#endif
  }
  avgCompares /= counter;
#ifdef NSORT_STATS
  avgTraversals /= counter;
#endif
  printf ("  Compares during finds: max = %d, avg = %f\n",
	  maxCompares, avgCompares);
#ifdef NSORT_STATS
  printf ("  Traversal times furing finds: max = %f, avg = %f\n",
      maxTraversals, avgTraversals);
#endif
  printf ("  Number of restructures during finds: %zu\n\n",
      srt->numRestruct);

  printf ("Doing sorted and reverse sorted compares\n");

  /*
   * Do the ordered compare
   */
  fp = fopen (argv[2], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[2]);
    return _ERROR_;
  }

  nsort_elapsed (&t1);
  found = srt->lh->head->next;
  while (!feof (fp) && found != srt->lh->tail) {
    cp = fgets (data, 80, fp);
    if (0 == cp) {
      break;
    }
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    /* skip blank lines */
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) found->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
	      (char *) data, (char *) found->data);
      isOK = FALSE;
    }
    found = found->next;
  }

  if (!isOK) {
    printf ("\n***Problem with tree data, retrieval from disk\n");
    return -1;
  }

  fclose (fp);

  /*
   * Do the reverse ordered compare
   */
  fp = fopen (argv[3], "r");
  if (0 == fp) {
    printf ("\n\n***Error: Couldn't open %s\n", argv[3]);
    return _ERROR_;
  }

  found = srt->lh->tail->prev;
  while (!feof (fp) && found != srt->lh->head) {
    /*memset (data, 0, 512); */
    cp = fgets (data, 80, fp);
    if (0 == cp) {
      break;
    }
    if (data[0] == '\0')
      break;
    cp = strchr (data, '\n');
    if (0 != cp)
      *cp = '\0';
    /* skip blank lines */
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *) found->data)) {
      printf ("Bad compare: \"%s\" <-> \"%s\"\n",
	      (char *) data, (char *) found->data);
      isOK = FALSE;
    }
    found = found->prev;
  }
  nsort_elapsed (&t2);
  free (ln);

  printf ("Search Time: %f\n", t2 - t1);

  if (!isOK) {
    printf ("\n***Problem with tree data, retrieval from disk\n");
    return -1;
  }

  fclose (fp);
  free (compares);
#ifdef NSORT_STATS
  free (traversal_times);
#endif

#if 0
  counter = srt->lh->number / 100;
  printf ("Removing %d items from the sort (each '.' is 100 items)\n", counter);

  {
    unsigned int max_per_restruct = 0, total_restruct = 0;
    unsigned int min_per_restruct = srt->numNodes;
    int dummy = TRUE;
    nsort_elapsed (&t1);
    for (i = 0; i < counter; i++) {
      if (i%100 == 0) {
        printf (".");
        fflush (stdout);
      }
      if (dummy) {
        found = nsort_remove_item (srt, srt->lh->tail->prev->prev);
        dummy = FALSE;
      }
      else {
        found = nsort_remove_item (srt, srt->lh->head->next->next);
        dummy = TRUE;
      }
      if (found == 0) {
        nsort_show_error (str, ERROR_SIZE);
        printf ("\n\n***Error: nsort_remove_item (srt, srt->lh->...): %s\n",
            str);
        return _ERROR_;
      }
      if (srt->numRestruct > max_per_restruct)
        max_per_restruct = srt->numRestruct;
      if (srt->numRestruct < min_per_restruct)
        min_per_restruct = srt->numRestruct;
      total_restruct += srt->numRestruct;
      free (found->data);
      free (found);
    }
    nsort_elapsed (&t2);

    printf ("\nRemoved %d items in %6.4f seconds\n", counter, t2-t1);
    printf ("Maximum number restructs = %d, total restructs = %d\n",
        max_per_restruct, total_restruct);
    printf ("Minimum number restructs = %d\n\n",
        min_per_restruct);
  }
#endif

  /*
   * [BeginDoc]
   *
   * Finally, we destroy the sort object and exit the program:
   * [Verbatim] */

  printf ("Done!\nCleaning up.\n\n");

  nsort_del (srt, NULL);
  nsort_destroy (srt);

  /* [EndDoc] */

  print_block_list();
#ifdef DEBUG_MALLOC
  dmalloc_shutdown();
#endif
  return 0;
}
