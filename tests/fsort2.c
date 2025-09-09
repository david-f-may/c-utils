/* Source File: fsort.c */

/*
 * [BeginDoc]
 *
 * \subsection{fsort.c}
 * \index{fsort}
 *
 * Program: fsort.c
 * Shell: fsort.sh
 *
 * The fsort program is the implementation of a combination of techniques
 * that I have wanted to combine for some time.  The result is a sort program
 * that sorts strings faster than the fastest qsort that I am aware of (30-40
 * percent faster).  Basically, the steps for sorting are as follows:
 *
 * \begin{enumerate}
 *
 * \item First, we use a hashing technique that results in the strings being
 * ordered by the first three characters.  This is the technique that is used
 * by rough_sort.c to sort strings in a rough way.
 *
 * \item Then, for each bucket that is populated with items, we use an nsort
 * sort to sort the strings in that bucket.
 *
 * \item Finally, we take the links that were sorted from the bucket and add
 * them to the end of a list (by default, these are in sorted order).
 * 
 * \end{emumerate}
 *
 * When we are done, we have a list that is sorted.  We could put the data
 * from this list into an nsort object and search on it, we could save it
 * to disk, or whatever our application requires.
 *
 * [EndDoc]
 *
 * XXX - finish up with the documentation as the source is cleaned up and made
 * presentable.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sorthdr.h"

// #ifdef __TINYC__
// #include "sort.c"
// #endif

#define HASH_INC_MAX ((256<<15)+(256<<10)+(256<<5)+256)
#define HASH(x)  (((x)<<15)+((x)<<10)+((x)<<5)+(x))
#define HASH_STR(x)  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+(x)[3])
#define HASH_INC ((HASH('z')) - (HASH('A')))
#define MAX_DATA 10000000 // 10 Million lines.
#define MAX_LEN 4096
#define ERROR_LEN 256

typedef struct _hash_link {
  char *data;
  struct _hash_link *next;
} hashLink;

 int fsort_compare (void *p1, void *p2)
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

int main (int argc, char *argv[])
{
  register unsigned int i;
  register int counter;
  register int numlink;
  /*static hashLink *hash_ary[HASH_INC];*/
  static hashLink **hash_ary;
  hashLink **lnks;
  hashLink *lnk;
  hashLink *lnkp;
  nsort_link_t *first, *last;
  nsort_link_t **nlnks;
  nsort_link_t *nlnkp;
  nsort_list_t flh;
  nsort_t srt;
  unsigned int hash_val;
  unsigned int sort_min = 0, sort_max = 0;
  FILE *fp;
  char *cp;
  char *chp;
  char data[MAX_LEN+1];
  char **cpp;
  double t1, t2;
  struct stat statbuf;
  int status;
  int numitems, len;
  char str[ERROR_LEN+1];

  /*
   * Read the whole file into a memory buffer and split it by lines.
   */
  if (argc != 6) {
    printf ("\n\n***Usage: %s num len file file.srt file.rev.srt\n", argv[0]);
    printf ("\twhere num is the number of items to sort,\n");
    printf ("\tlen is the max length of the items,\n");
    printf ("\tfile is a file to sort,\n");
    printf ("\tfile.srt is the presorted file to compare with, and\n");
    printf ("\tfile.rev.srt is a reverse sorted file to compare with.\n");
    return _ERROR_;
  }
  numitems = atoi (argv[1]);
  if (numitems < 0 || numitems > MAX_DATA) {
    printf ("\n\n***Error: invalid \"num\" value: %d\n", numitems);
    return _ERROR_;
  }
  len = atoi (argv[2]);
  if (len < 0 || len > MAX_LEN) {
    printf ("\n\n***Error: invalid \"len\" value: %d\n", len);
    return _ERROR_;
  }
  fp = fopen (argv[3], "r");
  if (fp == NULL) {
    printf ("\n\n***Error: Could not open %s: ", argv[3]);
    perror (" ");
    return _ERROR_;
  }
  stat (argv[3], &statbuf);
  cp = (char*)malloc ((size_t) statbuf.st_size+1);
  if (0 == cp) {
    fprintf (stderr, "\n\n***Error: could not allocate file buffer for %s\n", argv[3]);
    return _ERROR_;
  }
  fread (cp, (size_t)statbuf.st_size, 1, fp);
  fclose (fp);
  hash_ary = (hashLink **)malloc ((HASH_INC)*sizeof(hashLink));
  if (0 == hash_ary) {
    printf ("\n\n***Error: couldn't allocate hash_ary\n");
    free (cp);
    return _ERROR_;
  }
  memset (hash_ary, 0, (HASH_INC)*sizeof(hashLink*));
  check_pointer (hash_ary);
  cpp = (char**)malloc ((numitems+1)*sizeof(char*));
  if (0 == cpp) {
    printf ("\n\n***Error: couldn't allocate cpp\n");
    free (hash_ary);
    free (cp);
    return _ERROR_;
  }
  memset (cpp, 0, (numitems+1)*sizeof(char*));
  check_pointer (cpp);
  status =  nsort_text_file_split (cpp, numitems, cp, '\n');
  check_pointer (cpp);
  if (status > numitems) {
    printf ("\n\n***Warning: the number of lines in %s is greater than %d\n",
	argv[3], numitems);
    printf ("\tWe will only process %d items\n", numitems);
  }
  lnks = (hashLink**)malloc (numitems*sizeof(hashLink*));
  if (0 == lnks) {
    printf ("\n\n***Error: couldn't allocate lnks\n");
    free (hash_ary);
    free (cp);
    free (cpp);
    return _ERROR_;
  }
  memset (lnks, 0, numitems*sizeof(hashLink*));
  check_pointer (lnks);
  lnkp = (hashLink*)malloc (numitems*sizeof(hashLink));
  if (0 == lnkp) {
    printf ("\n\n***Error: could not allocate array of links\n");
    free (hash_ary);
    free (cp);
    free (cpp);
    free (lnks);
    return _ERROR_;
  }
  memset (lnkp, 0, numitems*sizeof(hashLink));
  nlnkp = (nsort_link_t*)malloc (numitems*sizeof (nsort_link_t));
  if (0 == nlnkp) {
    printf ("\n\n***Error: allocating nlnkp\n");
    free (hash_ary);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    return _ERROR_;
  }
  memset (nlnkp, 0, numitems*sizeof(nsort_link_t));
  nlnks = (nsort_link_t**)malloc (numitems*sizeof (nsort_link_t*));
  if (0 == nlnkp) {
    printf ("\n\n***Error: allocating nlnkp\n");
    free (hash_ary);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    return _ERROR_;
  }
  memset (nlnks, 0, numitems*sizeof(nsort_link_t*));
  for (i = 0; i < (unsigned)numitems; i++)
    nlnks[i] = (nsort_link_t *)(((char*)nlnkp)+i*sizeof(nsort_link_t));
  memset (&srt, 0, sizeof (nsort_t));
  srt.head = (nsort_node_t*)malloc (sizeof (nsort_node_t));
  if (0 == srt.head) {
    printf ("\n\n***Error: setting up nsort head\n");
    free (hash_ary);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    free (nlnks);
    return _ERROR_;
  }
  memset (srt.head, 0, sizeof (nsort_node_t));
  srt.tail = (nsort_node_t*)malloc (sizeof (nsort_node_t));
  if (0 == srt.tail) {
    printf ("\n\n***Error: setting up nsort tail\n");
    free (hash_ary);
    free (srt.head);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    free (nlnks);
    return _ERROR_;
  }
  memset (srt.tail, 0, sizeof (nsort_node_t));
  srt.head->prev = 0;
  srt.head->next = srt.tail;
  srt.tail->prev = srt.head;
  srt.tail->next = 0;
  srt.current = srt.head;
  srt.isUnique = FALSE;
  srt.manageAllocs = FALSE;
  srt.compare = fsort_compare;
  srt.lh = nsort_list_create ();
  if (srt.lh == 0) {
    printf ("\n\n***Error: allocating up nsort list\n");
    free (hash_ary);
    free (srt.head);
    free (srt.tail);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    free (nlnks);
    return _ERROR_;
  }
  memset (srt.lh, 0, sizeof (nsort_list_t));
  if (nsort_list_init (srt.lh) == _ERROR_) {
    printf ("\n\n***Error: setting up nsort list\n");
    free (hash_ary);
    free (srt.head);
    free (srt.tail);
    free (srt.lh);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    free (nlnks);
    return _ERROR_;
  }
  check_pointer (lnkp);
  for (i = 0; i < (unsigned) numitems; i++)
    lnks[i] = (hashLink*)(((char*)lnkp)+i*sizeof(hashLink));

  nsort_elapsed (&t1);
  counter = 0;
  for (i = 0; i < (unsigned)numitems && cpp[i]; i++) {
    hash_val = (((unsigned char**)cpp)[i][0]<<15)+
       (((unsigned char**)cpp)[i][1]<<10)+
       (((unsigned char**)cpp)[i][2]<<5)+
       (((unsigned char**)cpp)[i][3])- HASH('A');
    if (hash_val > HASH_INC) {
      printf ("\n\n***Error: invalid value \"%s\" produced hash of %d\n",
	  cpp[i], hash_val);
      free (hash_ary);
      free (srt.head);
      free (srt.tail);
      free (srt.lh);
      free (cp);
      free (cpp);
      free (lnks);
      free (lnkp);
      return _ERROR_;
    }
    // initialize sort_min & sort_max if necessary.
    if (sort_min == 0)
      sort_min = hash_val;
    if (sort_max == 0)
      sort_max = hash_val;
    if (sort_min > hash_val)
      sort_min = hash_val;
    if (sort_max < hash_val)
      sort_max = hash_val;
    lnk = lnks[i];
    lnk->data = cpp[i];
    if (hash_ary[hash_val] == 0) {
      lnk->next = 0;
      hash_ary[hash_val] = lnk;
    }
    else {
      lnk->next = hash_ary[hash_val];
      hash_ary[hash_val] = lnk;
    }
    counter++;
  }
  if (nsort_list_init (&flh) == _ERROR_) {
    printf ("\n\n***Error: initializing flh\n");
    free (hash_ary);
    free (srt.head);
    free (srt.tail);
    nsort_list_del (srt.lh);
    free (srt.lh);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    free (nlnks);
    return _ERROR_;
  }
  flh.current = flh.head;
  // Now, do some magic.
  for (numlink = 0, i = sort_min; i <= sort_max; i++) {
    if (hash_ary[i] != 0) {
      while (hash_ary[i] != 0) {
        lnk = hash_ary[i];
       	hash_ary[i] = hash_ary[i]->next;
        // printf ("%s\n", lnk->data);
        nlnks[numlink]->data = (void*)lnk->data;
        if (nsort_add_item (&srt, nlnks[numlink]) == _ERROR_) {
          nsort_show_sort_error (&srt, str, ERROR_LEN);
          printf ("\n\n***Error: nsort_add_item (&srt, %s\n): %s\n",
              lnk->data, str);
          while (nsort_list_remove_link(srt.lh) != 0)
            ;
          nsort_list_del (srt.lh);
          free (hash_ary);
          free (srt.head);
          free (srt.tail);
          free (srt.lh);
          flh.head->next = flh.tail;
          flh.tail->prev = flh.head;
          flh.number = 0;
          nsort_list_del (&flh);
          free (cp);
          free (cpp);
          free (lnks);
          free (lnkp);
          free (nlnkp);
          free (nlnks);
          return _ERROR_;
        }
        numlink++;
      }
      // Grab the list items and stuff them into another list.
      if (srt.lh->number > 0) {
        first = srt.lh->head->next;
        last = srt.lh->tail->prev;
        srt.lh->head->next = srt.lh->tail;
        srt.lh->tail->prev = srt.lh->head;
        srt.lh->number = 0;
        first->prev = flh.current;
        flh.current->next = first;
        flh.tail->prev = last;
        last->next = flh.tail;
        flh.current = last;
        if (srt.head->next != srt.tail) {
          nsort_node_t *node, *nextNode;
          node = srt.head->next;
          nextNode = node->next;
          while (TRUE) {
            free (node);
            if (nextNode == srt.tail)
              break;
            node = nextNode;
            nextNode = node->next;
          }
          srt.head->next = srt.tail;
          srt.tail->prev = srt.head;
          srt.numNodes = srt.nodeLevel = srt.thresh = srt.numRestruct = 0;
        }
      }
    }
  }
  flh.number = counter;
  nsort_elapsed (&t2);

  // OK, at this point you can save the data in an nsort sort or whatever.
  // At any rate, we will test it in this context by comparing the list
  // with sorted and reverse sorted copies of the original file.

  fp = fopen (argv[4], "r");
  if (0 == fp) {
    while (nsort_list_remove_link(srt.lh) != 0)
      ;
    nsort_list_del (srt.lh);
    free (hash_ary);
    free (srt.head);
    free (srt.tail);
    free (srt.lh);
    flh.head->next = flh.tail;
    flh.tail->prev = flh.head;
    flh.number = 0;
    nsort_list_del (&flh);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    free (nlnks);
    return _ERROR_;
  }
  first = flh.head->next;
  while (first != flh.tail) {
    chp = fgets (data, MAX_LEN, fp);
    if (0 == chp)
      break;
    if (data[0] == '\0')
      break;
    chp = strchr (data, '\n');
    if (0 != chp)
      *chp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *)first->data)) {
      printf ("Bad compare: \"%s\" <> \"%s\"\n", data, (char*)first->data);
    }
    first=first->next;
  }
  fclose (fp);
  fp = fopen (argv[5], "r");
  if (0 == fp) {
    while (nsort_list_remove_link(srt.lh) != 0)
      ;
    nsort_list_del (srt.lh);
    free (hash_ary);
    free (srt.head);
    free (srt.tail);
    free (srt.lh);
    flh.head->next = flh.tail;
    flh.tail->prev = flh.head;
    flh.number = 0;
    nsort_list_del (&flh);
    free (cp);
    free (cpp);
    free (lnks);
    free (lnkp);
    free (nlnkp);
    free (nlnks);
    return _ERROR_;
  }
  first = flh.tail->prev;
  while (first != flh.head) {
    chp = fgets (data, MAX_LEN, fp);
    if (0 == chp)
      break;
    if (data[0] == '\0')
      break;
    chp = strchr (data, '\n');
    if (0 != chp)
      *chp = '\0';
    if (strlen (data) == 0)
      continue;
    if (strcmp (data, (char *)first->data)) {
      printf ("Bad compare: \"%s\" <> \"%s\"\n", data, (char*)first->data);
    }
    first=first->prev;
  }
  fclose (fp);

  while (nsort_list_remove_link(srt.lh) != 0)
    ;
  nsort_list_del (srt.lh);
  free (hash_ary);
  free (srt.head);
  free (srt.tail);
  free (srt.lh);
  flh.head->next = flh.tail;
  flh.tail->prev = flh.head;
  flh.number = 0;
  nsort_list_del (&flh);
  free (cp);
  free (cpp);
  free (lnks);
  free (lnkp);
  free (nlnkp);
  free (nlnks);

  fprintf (stderr, "Sorted %d items in %f seconds\n", counter, t2-t1);
  print_block_list();
  return _OK_;
}
