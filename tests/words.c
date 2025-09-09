/* Source File: words.c */

/*
 * This is a very simple program that creates psuedo-random strings so that
 * other programs can thrash around with the data in a test environment.
 * It takes one or two command line parameters.  The first parameter is
 * a count, which indicates how many words to create.  The second is a
 * length, which indicates the maximum length to create the words.  This
 * program basically creates words of length 5-len.  If you don't specify
 * len, it is 50 by default.  The words created by this program are all
 * lower case.
 */
#include	<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int ac,  char *av[])
{
	char	buf[BUFSIZ];
	int	todo;
	int	maxlen = 50;
	int	len;
	long	junk;
	int	x;
	int	y;

	(void)time(&junk);
#ifndef __CINT__
	(void)srandom((int)junk);
#endif

	if(ac < 3) {
		(void)fprintf(stderr,"usage: words count [len]\n");
		exit(1);
	}

	/* how many inserts to perform */
	todo = atoi(av[1]);

	if(ac > 2) {
		maxlen = atoi(av[2]);
		if(maxlen < 1) {
			(void)fprintf(stderr,"max len less than 1 is not acceptable!\n");
			exit(1);
		}
	}

	for(x = 0; x < todo; x++) {
		if (x != 0)
		  printf ("\n");
		memset (buf, 0, BUFSIZ);
		len = ((int)random() % maxlen) + 1;
		if (len < (int) maxlen / 5)
		  len = maxlen / 5;
		if (len < 5 && maxlen > 5)
		  len = 5;
		for(y = 0; y < len; y++)	{
			buf[y] = ((int)random() % 26) + 'a';
		}

		buf[len] = '\0';
		(void)printf("%s",buf);
	}
	printf ("\n");
	exit(0);
}
