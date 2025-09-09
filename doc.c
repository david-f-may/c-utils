/* Source File:	doc.c */
/* ======================= */

/*
 *
 * ```
 * @DocInclude LICENSE
 * 
 * ```
 * [BeginDoc]
 * 
 * -----------------------------------------------------------------
 * 
 * 
 * # The document program - doc
 * 
 * The document program extracts documentation from source files.
 * This program will look through source files, the names of which
 * will be given on the command line, seeking for the documentation
 * delimiters. If it finds them, it will pull the text between
 * them, non inclusively, assuming it is documentation.
 * 
 * The documentation delimiters are "[BeginDoc]" and "[EndDoc]".
 * These delimiters are case sensitive and must include the '[' and
 * ']' characters, although you should not use '"' where you want
 * the delimiters to be counted.  In other words, if you want to use the
 * delimiters without them being interpreted as delimiters, surround
 * them in double quotes.
 *
 * All the lines that document pulls (the lines between the delimiters)
 * will be placed into a file called "manual.md".  The ability to
 * specify the file name may be added later.  This utility allows any
 * documentation line to begin with "<whitespace>*<whitespace>", which 
 * is removed from the front of the line before the line is written to 
 * the document.  For example, the following are all stripped from the
 * beginning of a line of documentation:
 * 
 * ```
 * =>	"     " just spaces
 * =>	"  *  " any combination of spaces and '*'s
 * =>	"\t*  " any combination of spaces, tabs and '*'s
 * =>	" *\t " 
 * ```
 * 
 * Everything on the line after all the spaces, tabs and '*'s have been
 * stripped is sent as is to the document file, so you can use the technique
 * shown above with these examples to indent.
 * 
 * The stripping away of '*'s at the beginning of the input line is to
 * allow the user to place comments inside of C source files without
 * having to change their comment styles (look at document.c to see
 * how this is used in practice).  The same support is provided for
 * for C and C++ programs (//) and for Assembly
 * programs (;).  So, the following strings will get stripped from
 * the beginning of the line:  * /* // ;
 * 
 * The document program returns a 0 if successful, a 1 if the program was
 * invoked unsuccessfully (bad command line) and a -1 if there was an error.
 * 
 * Document will also look for the string "@DocInclude" in the file.  If
 * it finds it, the document program will assume that the user wants to
 * enter another file in the documentation.  The "@DocInclude" directive
 * is used as follows:
 * 
 * ```
 * "@DocInclude" filename
 * 
 * ```
 * 
 * When this is seen in the source, filename is opened and any
 * documentation in it (text between the document delimiters) is
 * included as if it were part of the original file.
 * 
 * --------------------------------------------------------------------------------
 * 
 * ## Command-line options
 *
 * document takes 2 switches on the command-line, as follows:
 * 
 * ```
 *  -o
 *  -f filename.md
 * 
 * ```
 * -o tells document to overwrite the output file, if one exists with
 * that name.  The default is to append to the output file.
 * 
 * -f filename.md tells document to save the output into "filename.md".
 * The default is to save the data to a file called manual.md.
 * 
 * The document program will interpret wildcard characters on the command
 * line correctly.  So, to run it on all the ".c" files in the current
 * directory, type:
 * 
 * =>	doc *.c
 * 
 * Bug reports, suggestions for enhancement, etc. should be mailed to 
 * david.f.may@gmail.com
  * 
 * [EndDoc]
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>		/* for getopt() */
#ifdef	__CYGWIN__
#ifndef WIN32
#include <getopt.h>
#endif
#endif
#include "doc_config.h"

#ifndef _OK_
#define _OK_ 0
#endif
#ifndef _ERROR_
#define _ERROR_ -1
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define	MANUALFILE	"manual.md"
#define	MANUALMODE	"ab"

int document (char *fname, FILE * outputFP, int included);

#ifdef	__linux__
#include <ctype.h>
int strlwr (char *str);
#endif

#define	EXIT(st)	{status=st;goto CleanUpAndQuit;}

#define SIZE_ARB 4095

#ifndef _OK_
#define _OK_ 0
#endif
#ifndef _ERROR_
#define _ERROR_ -1
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef	HAVE_STRLWR
#include <ctype.h>
int strlwr (char *str);
#endif

#define	EXIT(st)	{status=st;goto CleanUpAndQuit;}
#define	TABSTOP		4	/* redefine this if you have bigger tabs */
#define iswhite(ch) ((ch)==' '||(ch)=='\t')

void emitVerbatimLine (FILE * fp, char *str);
void emitTextLine (FILE * fp, char *str);

int document (char *fname, FILE * outputFP, int included)
{
    char str[SIZE_ARB+1];
    int status;
    FILE *inputFP = NULL;
    char *cp, *cp1, *cpinc;
    char fileName[256];

    /*
     * First inner loop, looking for "[BeginDoc]"
     */
    inputFP = fopen (fname, "rb");
    if (inputFP == NULL) {
        printf ("\n\n** Warning: Could not open %s\n", fname);
        return _OK_;		/* could just exit here, but... */
    }
    strcpy (fileName, fname);
    if (!included) {
        strlwr (fileName);
        fprintf (outputFP, "Source File:\t%s\n", fileName);
        fprintf (outputFP, "======================\n");
        fflush (outputFP);
    }
    while (1 == 1) {
        memset (str, 0, SIZE_ARB+1);
        cp = fgets (str, SIZE_ARB, inputFP);
        if (ferror (inputFP)) {
            printf ("\n\n***Error: Couldn't read %s: terminating.\n", fileName);
            EXIT (-1);
        }
        /*
         * Look for End Of File
         */
        if (feof (inputFP))
            break;
        /*
         * Look for "@DocInclude " within the file.  If it is valid,
         * do the include.
         */
        cp = strstr (str, "@DocInclude ");
        if (cp != NULL) {
            if (*(cp - 1) == '\"')
                continue;
            cpinc = cp + strlen ("@DocInclude ");
            cp1 = strchr (cpinc, (int) '\n');
            if (cp1 != NULL) {
                *cp1 = '\0';
            }
            /*
               while (!isalpha ((int) *cp1) && cp1 != NULL) {
             *cp1 = '\0';
             cp1--;
             }
             */
            fflush (outputFP);
            status = document (cpinc, outputFP, TRUE);
            if (-1 == status)
                return -1;
        }
        /*
         * Look for "[BeginDoc]" and make sure it *is* documentation 
         * and not code
         */
        cp = strstr (str, "[BeginDoc]");
        if (cp == NULL)
            continue;
        if (*(cp - 1) == '\"')
            continue;			/* skip "[BeginDoc]" in quotes */
        /*
         * Found it; start processing documentation
         */
        /*
         * Second inner loop
         */
        while (1 == 1) {
            memset (str, 0, SIZE_ARB+1);
            cp = fgets (str, SIZE_ARB, inputFP);
            if (ferror (inputFP)) {
                printf ("\n\n***Error: Couldn't read %s: terminating.\n", fname);
                EXIT (-1);
            }
            /*
             * This is a problem, but the user will figure it out because her/his
             * documentation is trashed.
             */
            if (feof (inputFP))
                break;
            /*
             * Look for "[EndDoc]" condition - not ending condition if "[EndDoc]" is
             * in double quotes.
             */
            cp = strstr (str, "[EndDoc]");
            if (cp && *(cp - 1) != '\"')
                break;
            /*
             * Look again to see if we bust into verbatim mode.
             */
            cp = strstr (str, "[Verbatim]");
            if (cp != NULL && *(cp - 1) != '\"') {
                while (1 == 1) {
                    memset (str, 0, SIZE_ARB+1);
                    cp = fgets (str, SIZE_ARB, inputFP);
                    if (ferror (inputFP)) {
                        printf ("\n\n***Error: Couldn't read %s: terminating.\n", fname);
                        EXIT (-1);
                    }
                    if (feof (inputFP))
                        break;
                    cp = strstr (str, "[EndDoc]");
                    if (cp && *(cp - 1) != '\"') {
                        cp = fgets (str, SIZE_ARB, inputFP);
                        goto DoneVerbatimMode;
                    }
                    fprintf (outputFP, "%s", str);
                    fflush (outputFP);
                }
            }
            /*
             * We have documentation; print it to the manual and continue
             */
            cp = str;
            while (
                     iswhite (*cp) || 
                     *cp == '*'  ||
                     *cp == ';' ||
                     (*cp == '/' && (*(cp + 1)) == '/') ||
                     (*cp == '/' && (*(cp - 1)) == '/') ||
                     (*cp == '/' && (*(cp + 1)) == '*') ||
                     (*cp == '/' && (*(cp - 1)) == '/') ||
                     (*cp == '-' && (*(cp + 1)) == '-') ||
                     (*cp == '-' && (*(cp - 1)) == '-') &&
                     *cp != '\0'
                  )
                cp++;
            fprintf (outputFP, "%s", cp);
            fflush (outputFP);
        }
DoneVerbatimMode:
        {}; /* accommodate the new iso C standard wrt labels at the end of a block. */
    }
    fclose (inputFP);
    /*
     * Delimit the file divisions
     */
    if (!included) {
        fprintf (outputFP,
                "\n-----------------------------------------------------------------\n\n");
        fflush (outputFP);
    }

    status = 0;			/* everything was fine if we get here */

CleanUpAndQuit:
    printf ("\n");
    fflush (stdout);
    return status;
}

#ifndef HAVE_STRLWR

int strlwr (char *str)
{
    while (*str) {
        *str = tolower (*str);
        str++;
    }
    return _OK_;
}

#endif

void emitVerbatimLine (FILE * fp, char *str)
{
    char *cp;
    int i;
    int pos, depth;

    cp = str;
    pos = 0;
    while (*cp != '\0') {
        if (*cp == '\t') {
            depth = TABSTOP - pos % TABSTOP;
            for (i = 0; i < depth; i++) {
                fputc (' ', fp);
                pos++;
            }
        }
        else {
            fputc (*cp, fp);
            pos++;
        }
        cp++;
    }
}

void emitTextLine (FILE * fp, char *str)
{
    char *cp;

    cp = str;
    while (*cp != '\0') {
        switch (*cp) {
            case '$':			/* math formula delimiter */
                if (*(cp - 1) != '\\') {
                    fputc ('\\', fp);
                    fputc (*cp, fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '%':			/* comment */
                if (*(cp - 1) != '\\') {
                    fputc ('\\', fp);
                    fputc (*cp, fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '&':			/* table alignment mark */
                if (*(cp - 1) != '\\') {
                    fputc ('\\', fp);
                    fputc (*cp, fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '^':			/* math superscript text */
                if (*(cp - 1) != '\\') {
                    fputs ("\\^{}", fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '_':			/* math subscript text */
                if (*(cp - 1) != '\\') {
                    fputs ("\\_{}", fp);
                }
                else
                    fputc (*cp, fp);
                break;
#if 0
            case '#':			/* symbol replacement definition mark */
                if (*(cp - 1) != '\\') {
                    fputc ('\\', fp);
                    fputc (*cp, fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '{':			/* group begin symbol */
                if (*(cp - 1) != '\\') {
                    fputs ("$\\{$", fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '}':			/* group end symbol */
                if (*(cp - 1) != '\\') {
                    fputs ("$\\}$", fp);
                }
                else
                    fputc (*cp, fp);
                break;
#endif
            case '~':			/* unbreakable space */
                if (*(cp - 1) != '\\') {
                    fputs ("\\~{}", fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '>':			/* math symbol */
                if (*(cp - 1) != '$') {
                    fputc ('$', fp);
                    fputc (*cp, fp);
                    fputc ('$', fp);
                }
                else
                    fputc (*cp, fp);
                break;
            case '<':			/* math symbol */
                if (*(cp - 1) != '$') {
                    fputc ('$', fp);
                    fputc (*cp, fp);
                    fputc ('$', fp);
                }
                else
                    fputc (*cp, fp);
                break;
            default:
                fputc (*cp, fp);
                break;
        }
        if (*cp == '\n')
            break;
        cp++;
    }
    return;
}


int main (int argc, char *argv[])
{
    int status = 0;
    FILE *outputFP = NULL;
    int ch;
    int useDefaultManualName = TRUE;
    static char outputFile[255];
    int useDefaultMode = TRUE;
    static char modeFlags[10];

    if (argc == 1 || strcmp (argv[1], "-h") == 0) {
        printf ("\n\nUsage: document [-o] [-f manual] file [file...]\n\n");
        EXIT (1);
    }

    /*
     * Process the command-line
     */
    opterr = 0;
    while ((ch = getopt (argc, argv, "of:")) != EOF) {
        switch (ch) {
            case 'o':
                strcpy (modeFlags, "wb");
                useDefaultMode = FALSE;
                break;
            case 'f':
                strcpy (outputFile, optarg);
                useDefaultManualName = FALSE;
                break;
            case '?':
                printf ("\n\nInvalid command-line\n");
                printf ("Usage: document [-o] [-f manual] file [file...]\n\n");
                EXIT (-1);
        }
    }
    if (useDefaultManualName)
        strcpy (outputFile, MANUALFILE);
    if (useDefaultMode)
        strcpy (modeFlags, MANUALMODE);

    /*
     * Open the output file.
     */
    outputFP = fopen (outputFile, modeFlags);	/* "at" => append, text mode */
    if (outputFP == NULL) {
        printf ("\n\n***Error: Could not open %s\n", outputFile);
        EXIT (-1);
    }

    if (optind == argc) {
        printf ("\n\nNo non-switch arguments on the command-line\n");
        printf ("Usage: document [-o] [-f manual] file [file...]\n\n");
        EXIT (-1);
    }
    for (; optind < argc; optind++) {
        printf ("\nProcessing %s to %s", argv[optind], outputFile);
        status = document (argv[optind], outputFP, FALSE);
        if (-1 == status) {
            EXIT (-1);
        }
    }

CleanUpAndQuit:
    if (NULL != outputFP)
        fclose (outputFP);
    return status;
}
