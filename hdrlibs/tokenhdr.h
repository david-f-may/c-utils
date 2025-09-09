/*  Include File:   tokenhdr.h    */

/*
 * [BeginDoc]
 *
 *  # BSD 3-Clause License
 *
 *  Copyright (c) 2024, David May
 *  
 *  Redistribution  and  use  in source  and  binary  forms,  with or
 *  without modification,  are permitted  provided that the following
 *  conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions  in  binary  form  must  reproduce  the  above
 *  copyright notice,  this  list of  conditions  and  the  following
 *  disclaimer in the documentation  and/or  other materials provided
 *  with the distribution.
 *
 *  3. Neither the name of  the copyright holder nor the names of its
 *  contributors  may be used  to endorse or promote products derived
 *  from this software without specific prior written permission.
 *
 *  THIS  SOFTWARE  IS  PROVIDED   BY   THE   COPYRIGHT  HOLDERS  AND
 *  CONTRIBUTORS  “AS IS”  AND  ANY  EXPRESS  OR IMPLIED  WARRANTIES,
 *  INCLUDING,   BUT  NOT  LIMITED  TO,  THE  IMPLIED  WARRANTIES  OF
 *  MERCHANTABILITY  AND   FITNESS   FOR  A  PARTICULAR  PURPOSE  ARE
 *  DISCLAIMED.  IN   NO   EVENT   SHALL   THE  COPYRIGHT  HOLDER  OR
 *  CONTRIBUTORS  BE  LIABLE  FOR  ANY  DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *  USE, DATA, OR PROFITS;  OR BUSINESS  INTERRUPTION) HOWEVER CAUSED
 *  AND ON  ANY  THEORY  OF  LIABILITY,  WHETHER IN  CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF  THIS SOFTWARE,  EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *  
 *  [EndDoc]
 */

/*
**  Constants and macros for the parse engine.
*/

#ifndef __TOKENHDR_H__
#define __TOKENHDR_H__

/*
 * [BeginDoc]
 *
 * # Introduction
 * 
 * This is the Token module.  It provides functions that allow the
 * user to break a file into lexical tokens.  The algorithm used 
 * here is a fairly simple (yet elegant) finite state machine based
 * on goto's.  Three macros are used to implement the FSM in a
 * structured way.  They are as follows:
 * 
 * state(st);
 * set_state(st);
 * set_state_keep(st);
 * 
 * state (st) sets up two labels, st and st##keep.  set_state(st)
 * sends control to the label st, which gets another character from
 * the input stream and continues processing.  set_state_keep (st)
 * sends control to the label st##keep which just continues processing
 * without getting another character.
 * 
 * Consider the following example:
 * 
 * state (OUTSIDE):
 * 
 * This set's up two labels, OUTSIDE: and OUTSIDEkeep:.  A call to
 * set_state (OUTSIDE) does a goto OUTSIDE; where a call to 
 * set_state_keep (OUTSIDE) does a goto OUTSIDEkeep.  Processing
 * then continues from one of those points.
 * 
 * [EndDoc]
 */
#include <stdio.h>
#include <ctype.h>

#define state(s)                s: pi->ch = get_next_char(pi); keep##s
#define state_nk(s)             s: pi->ch = get_next_char(pi)
#define set_state(s)            goto s
#define set_state_keep(s)       goto keep##s
#ifndef	iswhite
#define iswhite(ch)             ((ch==' ')||(ch=='\t'))
#endif
#ifndef	iswhitenl
#define iswhitenl(ch)           ((ch==' ')||(ch=='\n')||(ch=='\t')||(ch=='\r'))
#endif
#ifdef  TRUE
#undef  TRUE
#endif
#ifdef  FALSE
#undef  FALSE
#endif

#define MAXIDENTSIZE        4096
#define	MAXTOKENFILESIZE	4096

/*
 * [BeginDoc]
 *
 * # Tokens
 *
 * The CDBTokenType enumerator lists the different token types. As
 * tokens are being parsed, one of the CDBTokenType enumerations
 * is returned. The normal tokens returned by the token functions
 * are from IDENT to CHAR_CONSTANT. The other token types returned
 * are diagnostics or error tokens.
 *
 * [Verbatim */

    typedef enum {
        TOKFALSE,
        TOKTRUE,
        NONE,
        IDENT,
        NUMBER,
        FLOAT,
        STRING,
        OPERATOR,
        PREPROC,
        CHAR_CONST,
        PARSE_ERROR,
        DONE
    } CDBTokenType;

/* [EndDoc] */

    typedef enum {
        PARS_NOERROR,           /* no error */
        PARS_PARAM,             /* bad function parameter */
        PARS_NOMEMORY,          /* out of memory */
        PARS_NEWLINE,           /* CR in middle of a line - not used anymore */
        PARS_NOPEN,             /* couldn't open the necessary file */
        PARS_TOOLONG,           /* item too long */
        PARS_EOF,               /* EOF in the middle of a string */
        /* new ones here */
        PARS_END                /* end of parse errors */
    } parseErrorType;

    extern parseErrorType parseError;
    extern char *parseErrorString[];

/*
 *
 *  [BeginDoc]
 *
 *  The parseItem data stucture is used for the tokenizing of C-type
 *  languages. It contains data points used to manage tokenizing.
 *  The tokenizer returns a small number of token types from the 
 *  CDBTokenType enumerated type.
 *
 *  [Verbatim] */

    typedef struct _parse_item {
        char *id;               /* allocd - this token */
        int idx;
        char *tokenFile;        /* alloced - file being parsed */
        char *databuf;          /* allocd buffer for storage of the file */
        int keep;
        char ch;
        CDBTokenType ptoken;
        CDBTokenType ttoken;
        unsigned lnctr;
        size_t datasize;        /* size of the data in databuf */
        char *datap;            /* pointer to the data in databuf */
    } parseItem;

/* [EndDoc] */

/*
**  Function interface
*/
    parseItem *initParser(const char *fname);
    void deleteParser(parseItem * pi);
    CDBTokenType getToken(parseItem * pi);
    parseItem *initStrParser(const char *parstr);

/*
 * Implements a Finite State Machine for parsing.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
int get_next_char(parseItem * pi);

/*
 * [BeginDoc]
 * 
 * The Token module has several external variables that are used for
 * different tasks.  They are as follows:
 * 
 * char *id;
 * unsigned int lnctr;
 * char *tokenFile;
 * ErrorType parse_error = PARS_NOERROR;
 * char *ErrorString[];
 * 
 * When GetToken() returns, id will contain a token string
 * or "(EOF)" if processing is done.  lnctr will contain the line
 * number of the file being processed.  tokenFile will contain
 * the name of the file being processed.  If there is a parsing
 * error, parse_error will contain the numerical representation
 * of the error and ErrorString[parse_error] will point to a 
 * string describing the error.
 * 
 * =================================================================
 * [EndDoc]
 */
parseErrorType parseError;
char *parseErrorString[] = {
    "no error",                 /* PARS_NOERROR */
    "bad function parameter",   /* PARS_PARAM */
    "dynamic memory exhausted", /* PARS_NOMEMORY */
    "newline before string termination",    /* PARS_NEWLINE - not used now */
    "couldn't open file for reading",   /* PARS_NOPEN */
    "item too long",            /* PARS_TOOLONG */
    "EOF encountered in the middle of a string constant",   /* PARS_EOF */
    /*
     * new ones here 
     */
    "parse end errors"
};

/*
 * For when you want to parse the entire file and process it for 
 * testing or what-not.
 */

/*
 * [BeginDoc]
 * 
 * int InitParser (const char *file);
 * 
 * InitParser() initializes the Token module.  This function must
 * be called before any other Token routines.  It opens the file
 * to be parsed, allocates a buffer for all of the file contents,
 * then reads the entire file into memory (for speed).  Then it
 * allocates memory for id and tokenFile.  If any of these
 * allocations fail, the Token module fails, and InitParser() returns
 * TOKFALSE.  If InitParser() succeeds, it returns TOKTRUE.  Upon successful
 * completion, other modules in the Token module can be used.
 * 
 * Please note: The result is undefined if you continue using the Token
 * routines if InitParser() fails.  The pointers involved will not
 * necessarily point to valid areas of memory, so it is impossible
 * to say what the result would be, although it is fairly safe to 
 * say that the result would not be good!
 * 
 * =================================================================
 * [EndDoc]
 */
parseItem *initParser(const char *file) {
    parseItem *pi;
    FILE *fp;
    struct stat statbuf;
    size_t len;

    assert(file != NULL);
    if (file == 0) {
        parseError = PARS_PARAM;
        return 0;
    }
    fp = fopen(file, "rb");
    if (fp == NULL) {
        parseError = PARS_NOPEN;
        return 0;
    }
    pi = malloc(sizeof(parseItem));
    if (0 == pi) {
        parseError = PARS_NOMEMORY;
        return 0;
    }
    memset(pi, 0, sizeof(parseItem));
    ////check_pointer (pi);
    stat(file, &statbuf);
    pi->databuf = malloc((size_t) (statbuf.st_size + 1));
    if (NULL == pi->databuf) {
        parseError = PARS_NOMEMORY;
        free(pi);
        return 0;
    }
    memset(pi->databuf, 0, (size_t) (statbuf.st_size + 1));
    //check_pointer (pi->databuf);
    pi->datasize = (size_t) (statbuf.st_size);
    pi->id = malloc(MAXIDENTSIZE);
    if (NULL == pi->id) {
        parseError = PARS_NOMEMORY;
        free(pi->databuf);
        free(pi);
        return 0;
    }
    memset(pi->id, 0, MAXIDENTSIZE);
    //check_pointer (pi->id);
    len = strlen(file) + 1;
    pi->tokenFile = malloc(len);
    if (NULL == pi->tokenFile) {
        parseError = PARS_NOMEMORY;
        free(pi->databuf);
        free(pi->id);
        free(pi);
        return 0;
    }
    memset(pi->tokenFile, 0, len);
    //check_pointer (pi->tokenFile);
    strncpy(pi->tokenFile, file, len - 1);
    pi->tokenFile[len - 1] = '\0';
    //check_pointer (pi->tokenFile);
    /*
     * Read the data into the buffer - the return value from
     * fread is pretty useless!
     */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    (void) fread(pi->databuf, (size_t) statbuf.st_size, 1, fp);
#pragma GCC diagnostic pop
    pi->datap = pi->databuf;    /* initialize the pointer */
    pi->databuf[(size_t) statbuf.st_size] = '\0';   /* close off any string-handling probs. */
    fclose(fp);                 /* don't need it any more */
    /*
     * The file was opened fine, so initialize the rest 
     * of the variables
     */
    parseError = PARS_NOERROR;
    pi->lnctr = 1;
    pi->keep = TOKFALSE;
    pi->ptoken = pi->ttoken = NONE;
    return pi;
}

/*
 * [BeginDoc]
 * 
 * int InitStrParser (char *parstr);
 * 
 * InitStrParser() initializes the Token module as InitParser() does
 * with the exception that it uses the string parstr instead of
 * reading the data from a file.  This allows a user to pass in a
 * string of data to be parsed.  parstr should have been allocated
 * on the stack and will "belong" to the token module after the
 * call to InitStrParser() (i.e., DeleteParser() will free the
 * storage containing the string).
 * 
 * Note:  if there is an error on the call to this function, 
 * parstr is not freed and should be freed by the caller.
 * 
 * =================================================================
 * [EndDoc]
 */
parseItem *initStrParser(const char *parstr) {
    parseItem *pi;
    size_t len;

    if (parstr == 0) {
        parseError = PARS_PARAM;
        return 0;
    }
    pi = malloc(sizeof(parseItem));
    if (0 == pi) {
        parseError = PARS_NOMEMORY;
        return 0;
    }
    memset(pi, 0, sizeof(parseItem));
    //check_pointer (pi);
    len = strlen(parstr) + 1;
    pi->databuf = malloc(len);
    if (NULL == pi->databuf) {
        parseError = PARS_NOMEMORY;
        free(pi);
        return 0;
    }
    pi->datasize = len;
    memset(pi->databuf, 0, pi->datasize);
    //check_pointer (pi->databuf);
    strcpy(pi->databuf, parstr);
    pi->id = malloc(MAXIDENTSIZE);
    if (NULL == pi->id) {
        parseError = PARS_NOMEMORY;
        free(pi->databuf);
        free(pi);
        return 0;
    }
    memset(pi->id, 0, MAXIDENTSIZE);
    //check_pointer (pi->id);
    pi->tokenFile = malloc(MAXTOKENFILESIZE);
    if (NULL == pi->tokenFile) {
        parseError = PARS_NOMEMORY;
        free(pi->id);
        free(pi->databuf);
        free(pi);
        return 0;
    }
    memset(pi->tokenFile, 0, MAXTOKENFILESIZE);
    //check_pointer (pi->tokenFile);
    strcpy(pi->tokenFile, "User String");
    pi->datap = pi->databuf;    /* initialize the pointer */
    /*
     * The file was opened fine, so initialize the rest 
     * of the variables
     */
    parseError = PARS_NOERROR;
    pi->lnctr = 1;
    pi->keep = TOKFALSE;
    pi->ptoken = pi->ttoken = NONE;
    return pi;
}

/*
 * [BeginDoc]
 * 
 * void DeleteParser(void);
 * 
 * DeleteParser() deallocates any heap memory and closes Token
 * processing.
 * 
 * =================================================================
 * [EndDoc]
 */
void deleteParser(parseItem * pi) {
    //check_pointer (pi->databuf);
    free(pi->databuf);
    pi->databuf = NULL;
    pi->datasize = 0;
    //check_pointer (pi->id);
    free(pi->id);
    pi->id = NULL;
    //check_pointer (pi->tokenFile);
    free(pi->tokenFile);
    pi->tokenFile = NULL;
    free(pi);
}

/*
 * [BeginDoc]
 * 
 * CDBTokenType GetToken (void);
 * 
 * GetToken() is the primary user function of the Token module.
 * Typically, the user will call InitParser() with the name of
 * the file to parse as the argument, and will then call GetToken()
 * until it returns DONE or indicates an error occurred.  GetToken()
 * takes no arguments and returns a CDBTokenType, an enumerated value
 * including the following:
 * 
 * TOKFALSE, TOKTRUE, NONE, IDENT, NUMBER, STRING, OPERATOR, PREPROC,
 * CHAR_CONST, PARSE_ERROR, DONE
 * 
 * Under normal circumstances, the user can expect GetToken() to
 * return one of IDENT (identifier), NUMBER (numerical constant),
 * STRING (string constant), OPERATOR (+,-,/,*,+=,etc.), PREPROC
 * (preprocessor directive) or CHAR_CONST (character constant).
 * Any other return values should be considered exceptions and
 * handled accordingly.
 * 
 * =================================================================
 * [EndDoc]
 */
CDBTokenType getToken(parseItem * pi) {
    if (pi->keep) {
        pi->keep = TOKFALSE;
        set_state_keep(OUTSIDE);
    }
    else
        set_state(OUTSIDE);
  state(OUTSIDE):
    if (iswhite(pi->ch))        /* starting state */
        set_state(OUTSIDE);
    if (isalpha((int) pi->ch) || pi->ch == '_') {
        pi->id[0] = pi->ch;
        pi->idx = 1;
        set_state(IDENTIFIER);
    }
    else if (isdigit((int) pi->ch)) {
        pi->id[0] = pi->ch;
        pi->idx = 1;
        set_state(NUMBER);
    }
    else if (pi->ch == '\"') {
        pi->ch = get_next_char(pi);
        /*
         * check for empty string 
         */
        if (pi->ch == '\"') {
            pi->id[0] = '\0';
            pi->idx = 1;
            return (STRING);
        }
        /*
         * Check for \<nl>
         */
        if (pi->ch == '\\') {
            pi->ch = get_next_char(pi);
            if (pi->ch == '\n')
                pi->lnctr++;
        }
        pi->id[0] = pi->ch;
        pi->idx = 1;
        set_state(STRING_CONSTANT);
    }
    else if (pi->ch == '!') {   /* check to see if != or just ! */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char(pi);
        if (pi->ch == '=') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
        else {
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKTRUE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
    }
    else if (pi->ch == '|') {   /* check to see if || or just | */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char(pi);
        if (pi->ch == '|') {
            pi->id[pi->idx++] = '|';
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
        else {
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKTRUE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
    }
    else if (pi->ch == '&') {   /* check to see if && or just & */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char(pi);
        if (pi->ch == '&') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
        else {
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKTRUE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
    }
    else if (pi->ch == '>') {   /*      check for >= */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char(pi);
        if (pi->ch == '=') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
        else {
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKTRUE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
    }
    else if (pi->ch == '<') {   /*      check for <= */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char(pi);
        if (pi->ch == '=') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
        else if (pi->ch == '>') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            return (OPERATOR);
        }
        else {
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKTRUE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
    }
    else if (pi->ch == '^') {   /*      check for <= */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char(pi);
        if (pi->ch == '=') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
        else {
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKTRUE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
    }
    else
        switch (pi->ch) {
        case '#':
            pi->id[0] = pi->ch;
            pi->idx = 1;
            set_state(PREPROCESSOR);
        case '/':
            set_state(AWAIT_ASTERISK);
        case '\'':
            pi->ch = get_next_char(pi);
            if (pi->ch == '\\') {
                pi->id[0] = '\\';
                pi->idx = 1;
                pi->ch = get_next_char(pi);
                if (pi->ch == '\'') {
                    pi->id[pi->idx++] = pi->ch;
                    set_state(CHAR_CONSTANT);
                }
                else
                    set_state_keep(CHAR_CONSTANT);
            }
            else {
                pi->id[0] = pi->ch;
                pi->idx = 1;
            }
            set_state(CHAR_CONSTANT);
        case '\n':
            pi->lnctr++;
            pi->ch = get_next_char(pi);
            if (pi->ch == EOF)
                pi->lnctr--;
            set_state_keep(OUTSIDE);
            /*
             * 20020308 - DFM 
             */
            /*
             * (sigh) Make cygwin work. 
             */
        case '\r':
            pi->ch = get_next_char(pi);
            if (pi->ch == EOF)
                pi->lnctr--;
            set_state_keep(OUTSIDE);
        case EOF:
            set_state(END_OF_FILE);
        case '=':              /* check to see if == or just = */
            pi->id[0] = pi->ch;
            pi->idx = 1;
            pi->ch = get_next_char(pi);
            if (pi->ch == '=') {
                pi->id[pi->idx++] = pi->ch;
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKFALSE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
            else if (pi->ch == '>') {
                pi->id[pi->idx++] = pi->ch;
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKFALSE;
                return (OPERATOR);
            }
            else {
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKTRUE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
        case '+':              /* check to see if ++ or just + */
            pi->id[0] = pi->ch;
            pi->idx = 1;
            pi->ch = get_next_char(pi);
            if (pi->ch == '+' || pi->ch == '=') {
                pi->id[pi->idx++] = pi->ch;
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKFALSE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
            else {
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKTRUE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
        case '-':              /* check to see if --, ->, -= or just - */
            pi->id[0] = pi->ch;
            pi->idx = 1;
            pi->ch = get_next_char(pi);
            if (pi->ch == '-' || pi->ch == '>' || pi->ch == '=') {
                pi->id[pi->idx++] = pi->ch;
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKFALSE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
            else {
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKTRUE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
        case '*':              /* check to see if *= or just */
            pi->id[0] = pi->ch;
            pi->idx = 1;
            pi->ch = get_next_char(pi);
            if (pi->ch == '=') {
                pi->id[pi->idx++] = pi->ch;
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKFALSE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
            else {
                pi->id[pi->idx++] = '\0';
                pi->keep = TOKTRUE;
                //check_pointer (pi->id);
                return (OPERATOR);
            }
        default:
            pi->id[0] = pi->ch;
            pi->idx = 1;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }

    state_nk(IDENTIFIER);
    if (isalnum((int) pi->ch) || pi->ch == '_') {
        if (pi->idx >= MAXIDENTSIZE) {
            parseError = PARS_TOOLONG;
            return (PARSE_ERROR);
        }
        pi->id[pi->idx++] = pi->ch;
        set_state(IDENTIFIER);
    }
    else {
        pi->keep = TOKTRUE;
        pi->id[pi->idx++] = '\0';
        //check_pointer (pi->id);
        return (IDENT);
    }
    state_nk(NUMBER);
    if (isalnum((int) pi->ch)) {
        if (pi->idx >= MAXIDENTSIZE) {
            parseError = PARS_TOOLONG;
            return (PARSE_ERROR);
        }
        pi->id[pi->idx++] = pi->ch;
        set_state(NUMBER);
    }
    else if (pi->ch == '.') {
        if (pi->idx >= MAXIDENTSIZE) {
            parseError = PARS_TOOLONG;
            return (PARSE_ERROR);
        }
        pi->id[pi->idx++] = pi->ch;
        set_state(FLOAT);
    }
    else {
        pi->keep = TOKTRUE;
        pi->id[pi->idx++] = '\0';
        //check_pointer (pi->id);
        return (NUMBER);
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-label"
  state(FLOAT):
    if (pi->ch != '.' && !isalnum((int) pi->ch)) {
        pi->keep = TOKFALSE;
        pi->id[pi->idx++] = '\0';
        return (FLOAT);
    }
    pi->id[pi->idx++] = pi->ch;
    set_state(FLOAT);
#pragma GCC diagnostic pop
  state(CHAR_CONSTANT):
    if (pi->ch == '\'') {
        pi->keep = TOKFALSE;
        pi->id[pi->idx++] = '\0';
        //check_pointer (pi->id);
        return (CHAR_CONST);
    }
    if (pi->idx >= MAXIDENTSIZE) {
        parseError = PARS_TOOLONG;
        return (PARSE_ERROR);
    }
    pi->id[pi->idx++] = pi->ch;
    set_state(CHAR_CONSTANT);
  state(STRING_CONSTANT):
    if (pi->ch == '\"') {
        if (pi->id[pi->idx - 1] == '\\') {
            pi->id[pi->idx++] = pi->ch;
            set_state(STRING_CONSTANT);
        }
        pi->keep = TOKFALSE;
        pi->id[pi->idx++] = '\0';
        //check_pointer (pi->id);
        return (STRING);
    }
    else if (pi->ch == '\n') {
        pi->lnctr++;
        pi->id[pi->idx++] = pi->ch;
        set_state(STRING_CONSTANT);
    }
    else if (pi->ch == EOF) {
        pi->lnctr--;
        parseError = PARS_EOF;
        return (PARSE_ERROR);
    }
    else if (pi->ch == '\\') {
        pi->ch = get_next_char(pi);
        if (pi->ch == '\n') {
            pi->lnctr++;
            pi->ch = get_next_char(pi);
            while (iswhite(pi->ch))
                pi->ch = get_next_char(pi);
            set_state_keep(STRING_CONSTANT);
        }
        else {
            pi->id[pi->idx++] = '\\';
            pi->id[pi->idx++] = pi->ch;
            set_state(STRING_CONSTANT);
        }
    }
    else {
        if (pi->idx >= MAXTOKENFILESIZE) {
            parseError = PARS_TOOLONG;
            return (PARSE_ERROR);
        }
        pi->id[pi->idx++] = pi->ch;
        set_state(STRING_CONSTANT);
    }
    state_nk(AWAIT_ASTERISK);
    if (pi->ch == '*')
        set_state(REMARK);
    else if (pi->ch == '/')
        set_state(CPP_REMARK);
    else {                      /*      check for /= */
        pi->id[0] = '/';
        pi->idx = 1;
        pi->ch = get_next_char(pi);
        if (pi->ch == '=') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            //check_pointer (pi->id);
            return (OPERATOR);
        }
        pi->keep = TOKTRUE;
        pi->id[pi->idx++] = '\0';
        //check_pointer (pi->id);
        return (OPERATOR);
    }
  state(REMARK):
    if (pi->ch == '\n') {
        pi->lnctr++;
        set_state(REMARK);
    }
    else if (pi->ch == '*')
        set_state(AWAIT_SLASH);
    else
        set_state(REMARK);
    state_nk(AWAIT_SLASH);
    if (pi->ch == '/')
        set_state(OUTSIDE);
    else
        set_state_keep(REMARK);
    state_nk(CPP_REMARK);
    if (pi->ch == '\n') {
        pi->lnctr++;            /* increment line counter */
        set_state(OUTSIDE);
    }
    else
        set_state(CPP_REMARK);
    state_nk(PREPROCESSOR);
    if (pi->ch == '\n') {
        pi->lnctr++;
        pi->id[pi->idx++] = '\0';
        pi->keep = TOKFALSE;
        //check_pointer (pi->id);
        return (PREPROC);
    }
    else if (pi->ch == '\\') {
        pi->ch = get_next_char(pi);
        if (pi->ch == '\n') {
            pi->lnctr++;
            pi->ch = get_next_char(pi);
        }
        while (iswhite(pi->ch))
            pi->ch = get_next_char(pi);
    }
    if (pi->idx >= MAXIDENTSIZE) {
        parseError = PARS_TOOLONG;
        return (PARSE_ERROR);
    }
    pi->id[pi->idx++] = pi->ch;
    set_state(PREPROCESSOR);
    state_nk(END_OF_FILE);
    strcpy(pi->id, "(EOF)");
    //check_pointer (pi->id);
    return (DONE);
}

int get_next_char(parseItem * pi) {
    char ch;

    assert(pi->datap >= pi->databuf
           && pi->datap <= pi->databuf + pi->datasize + 1);
    //check_pointer (pi->databuf);
    if (pi->datap < pi->databuf || pi->datap > pi->databuf + pi->datasize)
        return EOF;
    ch = *pi->datap;
    pi->datap++;
    if (ch == '\0')
        return (EOF);
    else
        return ((int) ch);
}

#define ERR_HERE_LEN 10
#define ERR_HERE_MAX (ERR_HERE_LEN*2+7)

char *errorLocation (parseItem *pi)
{
    int soff = 0, eoff = 0, val = 0;
    char sptr[ERR_HERE_LEN+1], eptr[ERR_HERE_LEN+1];
    static char scr[ERR_HERE_MAX];

    /*
     * Initialize return string
     */
    memset (scr, 0, ERR_HERE_MAX-1);
    memset (sptr, 0, ERR_HERE_LEN);
    memset (eptr, 0, ERR_HERE_LEN);

    /*
     * First, get the len we can process behind the current location
     */
    val = pi->datap - pi->databuf - 1;
    if (val > ERR_HERE_LEN)
        soff = ERR_HERE_LEN;
    else
        soff = val;

    /*
     * Now, get the len we can process ahead the current location
     */
    val = (pi->databuf + pi->datasize -1) - pi->datap - 1;
    if (val > ERR_HERE_LEN)
        eoff = ERR_HERE_LEN;
    else
        eoff = val;

    /*
     * Populate and return the return str
     */
    int off = pi->datap - pi->databuf;
    strncat (sptr, &pi->databuf[off-soff], soff);
    sptr[soff] = '\0';
    strncat (eptr, &pi->databuf[off], eoff);
    eptr[eoff] = '\0';
    snprintf (scr, ERR_HERE_MAX-1, "%s ^^^ %s", sptr, eptr);
    return (scr);
}


/*
 * @DocInclude testtokenhdr.c
 */

#endif                          /*  __TOKENHDR_H__   */
