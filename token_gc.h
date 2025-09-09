/*
 * =====================================================================================
 *
 *       Filename:  mpdc.h
 *
 *    Description: Header for mcalc.c 
 *
 *        Version:  1.0
 *        Created:  10/17/2020 03:33:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David F May Jr. (), david.f.may@gmail.com
 *
 * =====================================================================================
 */

/*
 * RCS information.
 *
 * $Id: $
 *
 * $Log: $
 *
 */
/*
**  Constants and macros for the parse engine.
*/

#ifndef __MPDC_H__
#define __MPDC_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <ctype.h>

#define state(s)                s: pi->ch = get_next_char(pi); keep##s
#define state_nk(s)             s: pi->ch = get_next_char(pi)
#define set_state(s)            goto s
#define set_state_keep(s)       goto keep##s
#ifndef iswhite
#define iswhite(ch)             ((ch==' ')||(ch=='\t'))
#endif
#ifndef iswhitenl
#define iswhitenl(ch)           ((ch==' ')||(ch=='\n')||(ch=='\t')||(ch=='\r')||(ch=='\f'))
#endif
#ifdef  TRUE
#undef  TRUE
#endif
#ifdef  FALSE
#undef  FALSE
#endif

#define MAXIDENTSIZE            1024
#define MAXTOKENFILESIZE        4096  // max path on linux


/*
--------------------------------------------------------------------------------
( ) –	Parentheses (function call) (see Note 1)                   left-to-right 
[ ] Brackets (array subscript) 
. Member selection via object name 
-> Member selection via pointer 
++ –- Postfix increment/decrement (see Note 2)
--------------------------------------------------------------------------------
++ –– 	Prefix increment/decrement                                 right-to-left
+ – Unary plus/minus 
! ~ Logical negation/bitwise complement 
(type) Cast (convert value to temporary value of type) 
* Dereference 
& Address (of operand) 
sizeof Determine size in bytes on this implementation
--------------------------------------------------------------------------------
*  /  %	Multiplication/division/modulus	                           left-to-right
--------------------------------------------------------------------------------
+  –  Addition/subtraction                                         left-to-right
--------------------------------------------------------------------------------
<<  >>	Bitwise shift left, Bitwise shift right	                   left-to-right
--------------------------------------------------------------------------------
Relational greater than/greater than or equal to                   left-to-right
<  <= 
Relational less than/less than or equal to 
>  >=
--------------------------------------------------------------------------------
== !=  Relational is equal to/is not equal to                      left-to-right
--------------------------------------------------------------------------------
&   Bitwise AND                                                    left-to-right
--------------------------------------------------------------------------------
^   Bitwise exclusive OR                                           left-to-right
|   Bitwise inclusive OR                                           left-to-right
--------------------------------------------------------------------------------
&&	Logical AND	left-to-right
--------------------------------------------------------------------------------
| |	Logical OR	left-to-right
--------------------------------------------------------------------------------
? : Ternary conditional                                            right-to-left
--------------------------------------------------------------------------------
= 	Assignment 	                                                   right-to-left
+=  -= Addition/subtraction assignment 
*=  /= Multiplication/division assignment 
%=  &= Modulus/bitwise AND assignment 
^=  |= Bitwise exclusive/inclusive OR assignment 
<<=  >>=Bitwise shift left/right assignment
--------------------------------------------------------------------------------
,	Comma (separate expressions)	left-to-right
*/
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

typedef enum {
  PARS_NOERROR,                        /* no error */
  PARS_PARAM,                          /* bad function parameter */
  PARS_NOMEMORY,                       /* out of memory */
  PARS_NEWLINE,                        /* CR in middle of a line - not used anymore */
  PARS_NOPEN,                          /* couldn't open the necessary file */
  PARS_TOOLONG,                        /* item too long */
  PARS_EOF,                            /* EOF in the middle of a string */
  /* new ones here */
  PARS_END                             /* end of parse errors */
} parseErrorType;

extern parseErrorType parseError;
extern char *parseErrorString[];

typedef struct _parse_item {
  char *id;                            /* allocd - this token */
  int idx;
  char *tokenFile;                     /* alloced - file being parsed */
  char *databuf;                       /* allocd buffer for storage of the file */
  int keep;
  char ch;
  CDBTokenType ptoken;
  CDBTokenType ttoken;
  unsigned lnctr;
  size_t datasize;                     /* size of the data in databuf */
  char *datap;                         /* pointer to the data in databuf */
} parseItem;

/*
**  Function interface
*/

void deleteParser (parseItem *pi);
CDBTokenType getToken (parseItem *pi);
parseItem *initParser (const char *fname);
parseItem *initStrParser (const char *parstr);

#ifdef  __cplusplus
}
#endif

#endif          /*  __MPDC_H__   */

