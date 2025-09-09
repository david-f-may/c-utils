/* Source File: flogsrtsys.c */

/*
 * [BeginDoc]
 *
 * \subsection{flogsrtsys.c}
 *
 * Program: flogsrtsys.c
 * Script: flogsrtsys.sh
 *
 * This program was written to show the performance of the system qsort function
 * so that it can be used in comparison with the nsort and the non-recursive
 * qsort.
 * 
 * [EndDoc]
 */
#include "sorthdr.h"

// #ifdef __TINYC__
// #include "sort.c"
// #endif

#define HASH_STR(x)  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+(x)[3])

 int testCompare (void *p1, void *p2)
{
  unsigned v1 = HASH_STR((char*)p1),
           v2 = HASH_STR((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char *)p1, (char *)p2));
}


#define	DATASIZE	40
#define MAXDATA		1000000
#define ERROR_LEN 256

int main (int argc, char *argv[])
{
  FILE *fp;
  char **cpp;
  char *data;
  char *cp, *sysqsarray;
  int i, number;
  struct stat statbuf;
  double t1, t2;
  int totalcount = 0;

  if (totalcount){}

  if (argc != 3)  {
    printf ("\nUsage: %s <file> <num>\n", argv[0]);
    printf ("\twhere <file> is the name of the file to read\n");
    printf ("\tand <num> is the number of items to sort\n\n");
    return 1;
  }

  number = atoi (argv[2]);
  if (number < 0 || number > MAXDATA) {
    printf ("\n\n***Error: invalid number of items\n");
    return _ERROR_;
  }

  Assert (argv[1] != NULL && argv[1][0] != '\0');
  fp = fopen (argv[1], "r");
  if (fp == NULL) {
    printf ("\n***Error: couldn't open %s\n", argv[1]);
    return -1;
  }

  stat (argv[1], &statbuf);
  data = (char*)malloc ((size_t)statbuf.st_size+1);
  if (0 == data) {
    printf ("\n\n***Error: fatal memory error allocating data\n");
    return _ERROR_;
  }
  fread (data, (size_t)statbuf.st_size, 1, fp);
  fclose (fp);

  cpp = (char**)malloc (MAXDATA * sizeof(char*));
  if (0 == cpp) {
    printf ("\n\n***Error: allocating cpp\n");
    return _ERROR_;
  }
  memset (cpp, 0, MAXDATA*sizeof(char*));
  check_pointer (cpp);
  totalcount = nsort_text_file_split (cpp, MAXDATA, data, '\n');
  /*
   * Now, use qsort to do the sorting.
   */
  sysqsarray = (char*)malloc (number * DATASIZE);
  if (0 == sysqsarray) {
    free (data);
    printf ("\n\n***Error: memory exhausted allocating qsort array\n");
    return _ERROR_;
  }
  memset (sysqsarray, 0, number*DATASIZE);
  check_pointer (sysqsarray);

  nsort_elapsed (&t1);
  for (i = 0; i < number; i++) {
    cp = sysqsarray+i*DATASIZE;
    strncpy (cp, cpp[i], DATASIZE);
    cp[DATASIZE-1] = '\0';
  }
  qsort ((void*)sysqsarray, number, DATASIZE,
      (int(*)(const void*, const void*))testCompare);
  nsort_elapsed (&t2);

  printf ("Added with system qsort in %8.6f seconds\n", t2-t1);

  free (sysqsarray);
  free (data);
  print_block_list();
  return _OK_;
}
