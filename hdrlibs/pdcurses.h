/* pdcurses.h */

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

/*----------------------------------------------------------------------*
 *                              PDCurses                                *
 *----------------------------------------------------------------------*/

#ifndef __PDCURSES__
#define __PDCURSES__ 1

#define PDC_NCMOUSE 1
#define HAVE_ASSUME_DEFAULT_COLORS 1


typedef unsigned int chtype;

/* The following is curspriv.h. I am hoping I don't need any of this. */

// #if 0
/* Private definitions and declarations for use within PDCurses.
   These should generally not be referenced by applications. */

#ifndef __CURSES_INTERNALS__
#define __CURSES_INTERNALS__ 1

#define CURSES_LIBRARY
// #include <curses.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__TURBOC__) || defined(__EMX__) || defined(__DJGPP__) || \
    defined(PDC_99) || defined(__WATCOMC__)
# ifndef HAVE_VSSCANF
#  define HAVE_VSSCANF 1     /* have vsscanf() */
# endif
#endif

#if defined(PDC_99) || defined(__WATCOMC__)
# ifndef HAVE_SNPRINTF
#  define HAVE_SNPRINTF 1   /* have snprintf() */
# endif
# ifndef HAVE_VSNPRINTF
#  define HAVE_VSNPRINTF 1   /* have vsnprintf() */
# endif
#endif

/*----------------------------------------------------------------------*/

typedef struct _win WINDOW;


typedef struct           /* structure for ripped off lines */
{
    int line;
    int (*init)(WINDOW *, int);
} RIPPEDOFFLINE;

/* Window properties */

#define _SUBWIN    0x01  /* window is a subwindow */
#define _PAD       0x10  /* X/Open Pad. */
#define _SUBPAD    0x20  /* X/Open subpad. */

/* Miscellaneous */

#define _NO_CHANGE -1    /* flags line edge unchanged */

#define _ECHAR     0x08  /* Erase char       (^H) */
#define _DWCHAR    0x17  /* Delete Word char (^W) */
#define _DLCHAR    0x15  /* Delete Line char (^U) */

/*----------------------------------------------------------------------*/

/* Platform implementation functions */

void    PDC_beep(void);
bool    PDC_can_change_color(void);
int     PDC_color_content(short, short *, short *, short *);
bool    PDC_check_key(void);
int     PDC_curs_set(int);
void    PDC_doupdate(void);
void    PDC_flushinp(void);
int     PDC_get_columns(void);
int     PDC_get_cursor_mode(void);
int     PDC_get_key(void);
int     PDC_get_rows(void);
void    PDC_gotoyx(int, int);
bool    PDC_has_mouse(void);
int     PDC_init_color(short, short, short, short);
int     PDC_modifiers_set(void);
int     PDC_mouse_set(void);
void    PDC_napms(int);
void    PDC_reset_prog_mode(void);
void    PDC_reset_shell_mode(void);
int     PDC_resize_screen(int, int);
void    PDC_restore_screen_mode(int);
void    PDC_save_screen_mode(int);
#ifdef XCURSES
void    PDC_set_args(int, char **);
#endif
void    PDC_scr_close(void);
void    PDC_scr_free(void);
int     PDC_scr_open(void);
void    PDC_set_keyboard_binary(bool);
void    PDC_transform_line(int, int, int, const chtype *);
const char *PDC_sysname(void);

/* Internal cross-module functions */

void    PDC_init_atrtab(void);
WINDOW *PDC_makelines(WINDOW *);
WINDOW *PDC_makenew(int, int, int, int);
int     PDC_mouse_in_slk(int, int);
void    PDC_slk_free(void);
void    PDC_slk_initialize(void);
void    PDC_sync(WINDOW *);

#ifdef PDC_WIDE
int     PDC_mbtowc(wchar_t *, const char *, size_t);
size_t  PDC_mbstowcs(wchar_t *, const char *, size_t);
size_t  PDC_wcstombs(char *, const wchar_t *, size_t);
#endif

#ifdef PDCDEBUG
# define PDC_LOG(x) if (SP && SP->dbfp) PDC_debug x
#else
# define PDC_LOG(x)
#endif

/* Internal macros for attributes */

#ifndef max
# define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
# define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define DIVROUND(num, divisor) ((num) + ((divisor) >> 1)) / (divisor)

#define PDC_CLICK_PERIOD 150  /* time to wait for a click, if
                                 not set by mouseinterval() */
#define PDC_COLOR_PAIRS  256
#define PDC_MAXCOL       768  /* maximum possible COLORS; may be less */

#define _INBUFSIZ        512  /* size of terminal input buffer */
#define NUNGETCH         256  /* max # chars to ungetch() */

#ifdef __cplusplus
}
#endif

#endif /* __CURSES_INTERNALS__ */

// #endif /* curspriv.h */


/*man-start**************************************************************

Define before inclusion (only those needed):

   Macro         | Meaning / value
   :-------------|-------------------------------------------------
   XCURSES       | if building / built for X11
   PDC_RGB       | RGB color (Red = 1, Green = 2, Blue = 4) vs. BGR
   PDC_WIDE      | if building / built with wide-character support
   PDC_DLL_BUILD | if building / built as a Windows DLL
   PDC_NCMOUSE   | use ncurses mouse API vs. traditional PDCurses

Defined by this header:

   Macro         | Meaning / value
   :-------------|-------------------------------------------------
   PDCURSES      | PDCurses-only features are available
   PDC_BUILD     | API build version
   PDC_VER_MAJOR | major version number
   PDC_VER_MINOR | minor version number
   PDC_VERDOT    | version string

**man-end****************************************************************/

#define PDCURSES        1
#define PDC_BUILD    3908
#define PDC_VER_MAJOR   3
#define PDC_VER_MINOR   9
#define PDC_VERDOT   "3.9"

#define CHTYPE_LONG     1      /* chtype >= 32 bits */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# define PDC_99         1
#endif

#if defined(__cplusplus) && __cplusplus >= 199711L
# define PDC_PP98       1
#endif

/*----------------------------------------------------------------------*/

#ifdef PDC_WIDE
# include <wchar.h>
#endif

#if defined(PDC_99) && !defined(__bool_true_false_are_defined)
# include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
# ifndef PDC_PP98
#  define bool _bool
# endif
#endif

/*----------------------------------------------------------------------
 *
 *  Constants and Types
 *
 */

#undef FALSE
#define FALSE 0

#undef TRUE
#define TRUE 1

#undef ERR
#define ERR (-1)

#undef OK
#define OK 0

#if !defined(PDC_PP98) && !defined(__bool_true_false_are_defined)
typedef unsigned char bool;
#endif

#if _LP64
typedef unsigned int chtype;
#else
typedef unsigned long chtype;  /* 16-bit attr + 16-bit char */
#endif

#ifdef PDC_WIDE
typedef chtype cchar_t;
#endif

typedef chtype attr_t;

/*----------------------------------------------------------------------
 *
 *  Version Info
 *
 */

/* Use this structure with PDC_get_version() for run-time info about the
   way the library was built, in case it doesn't match the header. */

typedef struct
{
    short flags;          /* flags OR'd together (see below) */
    short build;          /* PDC_BUILD at compile time */
    unsigned char major;  /* PDC_VER_MAJOR */
    unsigned char minor;  /* PDC_VER_MINOR */
    unsigned char csize;  /* sizeof chtype */
    unsigned char bsize;  /* sizeof bool */
} PDC_VERSION;

enum
{
    PDC_VFLAG_DEBUG = 1,  /* set if built with -DPDCDEBUG */
    PDC_VFLAG_WIDE  = 2,  /* -DPDC_WIDE */
    PDC_VFLAG_UTF8  = 4,  /* -DPDC_FORCE_UTF8 */
    PDC_VFLAG_DLL   = 8,  /* -DPDC_DLL_BUILD */
    PDC_VFLAG_RGB   = 16  /* -DPDC_RGB */
};

/*----------------------------------------------------------------------
 *
 *  Mouse Interface
 *
 */

#if _LP64
typedef unsigned int mmask_t;
#else
typedef unsigned long mmask_t;
#endif

typedef struct
{
    int x;           /* absolute column, 0 based, measured in characters */
    int y;           /* absolute row, 0 based, measured in characters */
    short button[3]; /* state of each button */
    int changes;     /* flags indicating what has changed with the mouse */
} MOUSE_STATUS;

#define BUTTON_RELEASED         0x0000
#define BUTTON_PRESSED          0x0001
#define BUTTON_CLICKED          0x0002
#define BUTTON_DOUBLE_CLICKED   0x0003
#define BUTTON_TRIPLE_CLICKED   0x0004
#define BUTTON_MOVED            0x0005  /* PDCurses */
#define WHEEL_SCROLLED          0x0006  /* PDCurses */
#define BUTTON_ACTION_MASK      0x0007  /* PDCurses */

#define PDC_BUTTON_SHIFT        0x0008  /* PDCurses */
#define PDC_BUTTON_CONTROL      0x0010  /* PDCurses */
#define PDC_BUTTON_ALT          0x0020  /* PDCurses */
#define BUTTON_MODIFIER_MASK    0x0038  /* PDCurses */

#define MOUSE_X_POS             (Mouse_status.x)
#define MOUSE_Y_POS             (Mouse_status.y)

/*
 * Bits associated with the .changes field:
 *   3         2         1         0
 * 210987654321098765432109876543210
 *                                 1 <- button 1 has changed
 *                                10 <- button 2 has changed
 *                               100 <- button 3 has changed
 *                              1000 <- mouse has moved
 *                             10000 <- mouse position report
 *                            100000 <- mouse wheel up
 *                           1000000 <- mouse wheel down
 *                          10000000 <- mouse wheel left
 *                         100000000 <- mouse wheel right
 */

#define PDC_MOUSE_MOVED         0x0008
#define PDC_MOUSE_POSITION      0x0010
#define PDC_MOUSE_WHEEL_UP      0x0020
#define PDC_MOUSE_WHEEL_DOWN    0x0040
#define PDC_MOUSE_WHEEL_LEFT    0x0080
#define PDC_MOUSE_WHEEL_RIGHT   0x0100

#define A_BUTTON_CHANGED        (Mouse_status.changes & 7)
#define MOUSE_MOVED             (Mouse_status.changes & PDC_MOUSE_MOVED)
#define MOUSE_POS_REPORT        (Mouse_status.changes & PDC_MOUSE_POSITION)
#define BUTTON_CHANGED(x)       (Mouse_status.changes & (1 << ((x) - 1)))
#define BUTTON_STATUS(x)        (Mouse_status.button[(x) - 1])
#define MOUSE_WHEEL_UP          (Mouse_status.changes & PDC_MOUSE_WHEEL_UP)
#define MOUSE_WHEEL_DOWN        (Mouse_status.changes & PDC_MOUSE_WHEEL_DOWN)
#define MOUSE_WHEEL_LEFT        (Mouse_status.changes & PDC_MOUSE_WHEEL_LEFT)
#define MOUSE_WHEEL_RIGHT       (Mouse_status.changes & PDC_MOUSE_WHEEL_RIGHT)

/* mouse bit-masks */

#define BUTTON1_RELEASED        0x00000001L
#define BUTTON1_PRESSED         0x00000002L
#define BUTTON1_CLICKED         0x00000004L
#define BUTTON1_DOUBLE_CLICKED  0x00000008L
#define BUTTON1_TRIPLE_CLICKED  0x00000010L
#define BUTTON1_MOVED           0x00000010L /* PDCurses */

#define BUTTON2_RELEASED        0x00000020L
#define BUTTON2_PRESSED         0x00000040L
#define BUTTON2_CLICKED         0x00000080L
#define BUTTON2_DOUBLE_CLICKED  0x00000100L
#define BUTTON2_TRIPLE_CLICKED  0x00000200L
#define BUTTON2_MOVED           0x00000200L /* PDCurses */

#define BUTTON3_RELEASED        0x00000400L
#define BUTTON3_PRESSED         0x00000800L
#define BUTTON3_CLICKED         0x00001000L
#define BUTTON3_DOUBLE_CLICKED  0x00002000L
#define BUTTON3_TRIPLE_CLICKED  0x00004000L
#define BUTTON3_MOVED           0x00004000L /* PDCurses */

/* For the ncurses-compatible functions only, BUTTON4_PRESSED and
   BUTTON5_PRESSED are returned for mouse scroll wheel up and down;
   otherwise PDCurses doesn't support buttons 4 and 5 */

#define BUTTON4_RELEASED        0x00008000L
#define BUTTON4_PRESSED         0x00010000L
#define BUTTON4_CLICKED         0x00020000L
#define BUTTON4_DOUBLE_CLICKED  0x00040000L
#define BUTTON4_TRIPLE_CLICKED  0x00080000L

#define BUTTON5_RELEASED        0x00100000L
#define BUTTON5_PRESSED         0x00200000L
#define BUTTON5_CLICKED         0x00400000L
#define BUTTON5_DOUBLE_CLICKED  0x00800000L
#define BUTTON5_TRIPLE_CLICKED  0x01000000L

#define MOUSE_WHEEL_SCROLL      0x02000000L /* PDCurses */
#define BUTTON_MODIFIER_SHIFT   0x04000000L /* PDCurses */
#define BUTTON_MODIFIER_CONTROL 0x08000000L /* PDCurses */
#define BUTTON_MODIFIER_ALT     0x10000000L /* PDCurses */

#define ALL_MOUSE_EVENTS        0x1fffffffL
#define REPORT_MOUSE_POSITION   0x20000000L

/* ncurses mouse interface */

typedef struct
{
    short id;       /* unused, always 0 */
    int x, y, z;    /* x, y same as MOUSE_STATUS; z unused */
    mmask_t bstate; /* equivalent to changes + button[], but
                       in the same format as used for mousemask() */
} MEVENT;

#if defined(PDC_NCMOUSE) && !defined(NCURSES_MOUSE_VERSION)
# define NCURSES_MOUSE_VERSION 2
#endif

#ifdef NCURSES_MOUSE_VERSION
# define BUTTON_SHIFT   BUTTON_MODIFIER_SHIFT
# define BUTTON_CONTROL BUTTON_MODIFIER_CONTROL
# define BUTTON_CTRL    BUTTON_MODIFIER_CONTROL
# define BUTTON_ALT     BUTTON_MODIFIER_ALT
#else
# define BUTTON_SHIFT   PDC_BUTTON_SHIFT
# define BUTTON_CONTROL PDC_BUTTON_CONTROL
# define BUTTON_ALT     PDC_BUTTON_ALT
#endif

/*----------------------------------------------------------------------
 *
 *  Window and Screen Structures
 *
 */

typedef struct _win       /* definition of a window */
{
    int   _cury;          /* current pseudo-cursor */
    int   _curx;
    int   _maxy;          /* max window coordinates */
    int   _maxx;
    int   _begy;          /* origin on screen */
    int   _begx;
    int   _flags;         /* window properties */
    chtype _attrs;        /* standard attributes and colors */
    chtype _bkgd;         /* background, normally blank */
    bool  _clear;         /* causes clear at next refresh */
    bool  _leaveit;       /* leaves cursor where it is */
    bool  _scroll;        /* allows window scrolling */
    bool  _nodelay;       /* input character wait flag */
    bool  _immed;         /* immediate update flag */
    bool  _sync;          /* synchronise window ancestors */
    bool  _use_keypad;    /* flags keypad key mode active */
    chtype **_y;          /* pointer to line pointer array */
    int   *_firstch;      /* first changed character in line */
    int   *_lastch;       /* last changed character in line */
    int   _tmarg;         /* top of scrolling region */
    int   _bmarg;         /* bottom of scrolling region */
    int   _delayms;       /* milliseconds of delay for getch() */
    int   _parx, _pary;   /* coords relative to parent (0,0) */
    struct _win *_parent; /* subwin's pointer to parent win */

    /* these are used only if this is a pad */
    struct pdat
    {
        int _pad_y;
        int _pad_x;
        int _pad_top;
        int _pad_left;
        int _pad_bottom;
        int _pad_right;
    } _pad;               /* Pad-properties structure */
} WINDOW;

/* Color pair structure */

typedef struct
{
    short f;              /* foreground color */
    short b;              /* background color */
    int   count;          /* allocation order */
    bool  set;            /* pair has been set */
} PDC_PAIR;

/* Avoid using the SCREEN struct directly -- use the corresponding
   functions if possible. This struct may eventually be made private. */

typedef struct
{
    bool  alive;          /* if initscr() called, and not endwin() */
    bool  autocr;         /* if cr -> lf */
    bool  cbreak;         /* if terminal unbuffered */
    bool  echo;           /* if terminal echo */
    bool  raw_inp;        /* raw input mode (v. cooked input) */
    bool  raw_out;        /* raw output mode (7 v. 8 bits) */
    bool  audible;        /* FALSE if the bell is visual */
    bool  mono;           /* TRUE if current screen is mono */
    bool  resized;        /* TRUE if TERM has been resized */
    bool  orig_attr;      /* TRUE if we have the original colors */
    short orig_fore;      /* original screen foreground color */
    short orig_back;      /* original screen foreground color */
    int   cursrow;        /* position of physical cursor */
    int   curscol;        /* position of physical cursor */
    int   visibility;     /* visibility of cursor */
    int   orig_cursor;    /* original cursor size */
    int   lines;          /* new value for LINES */
    int   cols;           /* new value for COLS */
    mmask_t _trap_mbe;             /* trap these mouse button events */
    int   mouse_wait;              /* time to wait (in ms) for a
                                      button release after a press, in
                                      order to count it as a click */
    int   slklines;                /* lines in use by slk_init() */
    WINDOW *slk_winptr;            /* window for slk */
    int   linesrippedoff;          /* lines ripped off via ripoffline() */
    int   linesrippedoffontop;     /* lines ripped off on
                                      top via ripoffline() */
    int   delaytenths;             /* 1/10ths second to wait block
                                      getch() for */
    bool  _preserve;               /* TRUE if screen background
                                      to be preserved */
    int   _restore;                /* specifies if screen background
                                      to be restored, and how */
    unsigned long key_modifiers;   /* key modifiers (SHIFT, CONTROL, etc.)
                                      on last key press */
    bool  return_key_modifiers;    /* TRUE if modifier keys are
                                      returned as "real" keys */
    bool  key_code;                /* TRUE if last key is a special key;
                                      used internally by get_wch() */
    MOUSE_STATUS mouse_status;     /* last returned mouse status */
    short line_color;     /* color of line attributes - default -1 */
    attr_t termattrs;     /* attribute capabilities */
    WINDOW *lastscr;      /* the last screen image */
    FILE *dbfp;           /* debug trace file pointer */
    bool  color_started;  /* TRUE after start_color() */
    bool  dirty;          /* redraw on napms() after init_color() */
    int   sel_start;      /* start of selection (y * COLS + x) */
    int   sel_end;        /* end of selection */
    int  *c_buffer;       /* character buffer */
    int   c_pindex;       /* putter index */
    int   c_gindex;       /* getter index */
    int  *c_ungch;        /* array of ungotten chars */
    int   c_ungind;       /* ungetch() push index */
    int   c_ungmax;       /* allocated size of ungetch() buffer */
    PDC_PAIR *atrtab;     /* table of color pairs */
} SCREEN;

/*----------------------------------------------------------------------
 *
 *  External Variables
 *
 */

#ifdef PDC_DLL_BUILD
# ifdef CURSES_LIBRARY
#  define PDCEX __declspec(dllexport) extern
# else
#  define PDCEX __declspec(dllimport)
# endif
#else
# define PDCEX extern
#endif

PDCEX  int          LINES;        /* terminal height */
PDCEX  int          COLS;         /* terminal width */
PDCEX  WINDOW       *stdscr;      /* the default screen window */
PDCEX  WINDOW       *curscr;      /* the current screen image */
PDCEX  SCREEN       *SP;          /* curses variables */
PDCEX  MOUSE_STATUS Mouse_status;
PDCEX  int          COLORS;
PDCEX  int          COLOR_PAIRS;
PDCEX  int          TABSIZE;
PDCEX  chtype       acs_map[];    /* alternate character set map */
PDCEX  char         ttytype[];    /* terminal name/description */

/*man-start**************************************************************

Text Attributes
===============

PDCurses uses a 32-bit integer for its chtype:

   color pair             | modifiers             | character eg 'a'
   -----------------------|-----------------------|--------------------
   31 30 29 28 27 26 25 24|23 22 21 20 19 18 17 16|15 14 13 ..  2  1  0

There are 256 color pairs (8 bits), 8 bits for modifiers, and 16 bits
for character data. The modifiers are bold, underline, right-line,
left-line, italic, reverse and blink, plus the alternate character set
indicator.

**man-end****************************************************************/

/*** Video attribute macros ***/

#define A_NORMAL      (chtype)0

#define A_ALTCHARSET  (chtype)0x00010000
#define A_RIGHT       (chtype)0x00020000
#define A_LEFT        (chtype)0x00040000
#define A_ITALIC      (chtype)0x00080000
#define A_UNDERLINE   (chtype)0x00100000
#define A_REVERSE     (chtype)0x00200000
#define A_BLINK       (chtype)0x00400000
#define A_BOLD        (chtype)0x00800000

#define A_ATTRIBUTES  (chtype)0xffff0000
#define A_CHARTEXT    (chtype)0x0000ffff
#define A_COLOR       (chtype)0xff000000

#define PDC_COLOR_SHIFT 24

#define A_LEFTLINE    A_LEFT
#define A_RIGHTLINE   A_RIGHT
#define A_STANDOUT    (A_REVERSE | A_BOLD) /* X/Open */

#define A_DIM         A_NORMAL
#define A_INVIS       A_NORMAL
#define A_PROTECT     A_NORMAL

#define A_HORIZONTAL  A_NORMAL
#define A_LOW         A_NORMAL
#define A_TOP         A_NORMAL
#define A_VERTICAL    A_NORMAL

#define CHR_MSK       A_CHARTEXT           /* Obsolete */
#define ATR_MSK       A_ATTRIBUTES         /* Obsolete */
#define ATR_NRM       A_NORMAL             /* Obsolete */

/* For use with attr_t -- X/Open says, "these shall be distinct", so
   this is a non-conforming implementation. */

#define WA_NORMAL     A_NORMAL

#define WA_ALTCHARSET A_ALTCHARSET
#define WA_BLINK      A_BLINK
#define WA_BOLD       A_BOLD
#define WA_DIM        A_DIM
#define WA_INVIS      A_INVIS
#define WA_ITALIC     A_ITALIC
#define WA_LEFT       A_LEFT
#define WA_PROTECT    A_PROTECT
#define WA_REVERSE    A_REVERSE
#define WA_RIGHT      A_RIGHT
#define WA_STANDOUT   A_STANDOUT
#define WA_UNDERLINE  A_UNDERLINE

#define WA_HORIZONTAL A_HORIZONTAL
#define WA_LOW        A_LOW
#define WA_TOP        A_TOP
#define WA_VERTICAL   A_VERTICAL

#define WA_ATTRIBUTES A_ATTRIBUTES

/*** Alternate character set macros ***/

#define PDC_ACS(w) ((chtype)w | A_ALTCHARSET)

/* VT100-compatible symbols -- box chars */

#define ACS_ULCORNER  PDC_ACS('l')
#define ACS_LLCORNER  PDC_ACS('m')
#define ACS_URCORNER  PDC_ACS('k')
#define ACS_LRCORNER  PDC_ACS('j')
#define ACS_RTEE      PDC_ACS('u')
#define ACS_LTEE      PDC_ACS('t')
#define ACS_BTEE      PDC_ACS('v')
#define ACS_TTEE      PDC_ACS('w')
#define ACS_HLINE     PDC_ACS('q')
#define ACS_VLINE     PDC_ACS('x')
#define ACS_PLUS      PDC_ACS('n')

/* VT100-compatible symbols -- other */

#define ACS_S1        PDC_ACS('o')
#define ACS_S9        PDC_ACS('s')
#define ACS_DIAMOND   PDC_ACS('`')
#define ACS_CKBOARD   PDC_ACS('a')
#define ACS_DEGREE    PDC_ACS('f')
#define ACS_PLMINUS   PDC_ACS('g')
#define ACS_BULLET    PDC_ACS('~')

/* Teletype 5410v1 symbols -- these are defined in SysV curses, but
   are not well-supported by most terminals. Stick to VT100 characters
   for optimum portability. */

#define ACS_LARROW    PDC_ACS(',')
#define ACS_RARROW    PDC_ACS('+')
#define ACS_DARROW    PDC_ACS('.')
#define ACS_UARROW    PDC_ACS('-')
#define ACS_BOARD     PDC_ACS('h')
#define ACS_LANTERN   PDC_ACS('i')
#define ACS_BLOCK     PDC_ACS('0')

/* That goes double for these -- undocumented SysV symbols. Don't use
   them. */

#define ACS_S3        PDC_ACS('p')
#define ACS_S7        PDC_ACS('r')
#define ACS_LEQUAL    PDC_ACS('y')
#define ACS_GEQUAL    PDC_ACS('z')
#define ACS_PI        PDC_ACS('{')
#define ACS_NEQUAL    PDC_ACS('|')
#define ACS_STERLING  PDC_ACS('}')

/* Box char aliases */

#define ACS_BSSB      ACS_ULCORNER
#define ACS_SSBB      ACS_LLCORNER
#define ACS_BBSS      ACS_URCORNER
#define ACS_SBBS      ACS_LRCORNER
#define ACS_SBSS      ACS_RTEE
#define ACS_SSSB      ACS_LTEE
#define ACS_SSBS      ACS_BTEE
#define ACS_BSSS      ACS_TTEE
#define ACS_BSBS      ACS_HLINE
#define ACS_SBSB      ACS_VLINE
#define ACS_SSSS      ACS_PLUS

/* cchar_t aliases */

#ifdef PDC_WIDE
# define WACS_ULCORNER (&(acs_map['l']))
# define WACS_LLCORNER (&(acs_map['m']))
# define WACS_URCORNER (&(acs_map['k']))
# define WACS_LRCORNER (&(acs_map['j']))
# define WACS_RTEE     (&(acs_map['u']))
# define WACS_LTEE     (&(acs_map['t']))
# define WACS_BTEE     (&(acs_map['v']))
# define WACS_TTEE     (&(acs_map['w']))
# define WACS_HLINE    (&(acs_map['q']))
# define WACS_VLINE    (&(acs_map['x']))
# define WACS_PLUS     (&(acs_map['n']))

# define WACS_S1       (&(acs_map['o']))
# define WACS_S9       (&(acs_map['s']))
# define WACS_DIAMOND  (&(acs_map['`']))
# define WACS_CKBOARD  (&(acs_map['a']))
# define WACS_DEGREE   (&(acs_map['f']))
# define WACS_PLMINUS  (&(acs_map['g']))
# define WACS_BULLET   (&(acs_map['~']))

# define WACS_LARROW   (&(acs_map[',']))
# define WACS_RARROW   (&(acs_map['+']))
# define WACS_DARROW   (&(acs_map['.']))
# define WACS_UARROW   (&(acs_map['-']))
# define WACS_BOARD    (&(acs_map['h']))
# define WACS_LANTERN  (&(acs_map['i']))
# define WACS_BLOCK    (&(acs_map['0']))

# define WACS_S3       (&(acs_map['p']))
# define WACS_S7       (&(acs_map['r']))
# define WACS_LEQUAL   (&(acs_map['y']))
# define WACS_GEQUAL   (&(acs_map['z']))
# define WACS_PI       (&(acs_map['{']))
# define WACS_NEQUAL   (&(acs_map['|']))
# define WACS_STERLING (&(acs_map['}']))

# define WACS_BSSB     WACS_ULCORNER
# define WACS_SSBB     WACS_LLCORNER
# define WACS_BBSS     WACS_URCORNER
# define WACS_SBBS     WACS_LRCORNER
# define WACS_SBSS     WACS_RTEE
# define WACS_SSSB     WACS_LTEE
# define WACS_SSBS     WACS_BTEE
# define WACS_BSSS     WACS_TTEE
# define WACS_BSBS     WACS_HLINE
# define WACS_SBSB     WACS_VLINE
# define WACS_SSSS     WACS_PLUS
#endif

/*** Color macros ***/

#define COLOR_BLACK   0

#ifdef PDC_RGB        /* RGB */
# define COLOR_RED    1
# define COLOR_GREEN  2
# define COLOR_BLUE   4
#else                 /* BGR */
# define COLOR_BLUE   1
# define COLOR_GREEN  2
# define COLOR_RED    4
#endif

#define COLOR_CYAN    (COLOR_BLUE | COLOR_GREEN)
#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)
#define COLOR_YELLOW  (COLOR_RED | COLOR_GREEN)

#define COLOR_WHITE   7

/*----------------------------------------------------------------------
 *
 *  Function and Keypad Key Definitions
 *  Many are just for compatibility
 *
 */

#define KEY_CODE_YES  0x100  /* If get_wch() gives a key code */

#define KEY_BREAK     0x101  /* Not on PC KBD */
#define KEY_DOWN      0x102  /* Down arrow key */
#define KEY_UP        0x103  /* Up arrow key */
#define KEY_LEFT      0x104  /* Left arrow key */
#define KEY_RIGHT     0x105  /* Right arrow key */
#define KEY_HOME      0x106  /* home key */
#define KEY_BACKSPACE 0x107  /* not on pc */
#define KEY_F0        0x108  /* function keys; 64 reserved */

#define KEY_DL        0x148  /* delete line */
#define KEY_IL        0x149  /* insert line */
#define KEY_DC        0x14a  /* delete character */
#define KEY_IC        0x14b  /* insert char or enter ins mode */
#define KEY_EIC       0x14c  /* exit insert char mode */
#define KEY_CLEAR     0x14d  /* clear screen */
#define KEY_EOS       0x14e  /* clear to end of screen */
#define KEY_EOL       0x14f  /* clear to end of line */
#define KEY_SF        0x150  /* scroll 1 line forward */
#define KEY_SR        0x151  /* scroll 1 line back (reverse) */
#define KEY_NPAGE     0x152  /* next page */
#define KEY_PPAGE     0x153  /* previous page */
#define KEY_STAB      0x154  /* set tab */
#define KEY_CTAB      0x155  /* clear tab */
#define KEY_CATAB     0x156  /* clear all tabs */
#define KEY_ENTER     0x157  /* enter or send (unreliable) */
#define KEY_SRESET    0x158  /* soft/reset (partial/unreliable) */
#define KEY_RESET     0x159  /* reset/hard reset (unreliable) */
#define KEY_PRINT     0x15a  /* print/copy */
#define KEY_LL        0x15b  /* home down/bottom (lower left) */
#define KEY_ABORT     0x15c  /* abort/terminate key (any) */
#define KEY_SHELP     0x15d  /* short help */
#define KEY_LHELP     0x15e  /* long help */
#define KEY_BTAB      0x15f  /* Back tab key */
#define KEY_BEG       0x160  /* beg(inning) key */
#define KEY_CANCEL    0x161  /* cancel key */
#define KEY_CLOSE     0x162  /* close key */
#define KEY_COMMAND   0x163  /* cmd (command) key */
#define KEY_COPY      0x164  /* copy key */
#define KEY_CREATE    0x165  /* create key */
#define KEY_END       0x166  /* end key */
#define KEY_EXIT      0x167  /* exit key */
#define KEY_FIND      0x168  /* find key */
#define KEY_HELP      0x169  /* help key */
#define KEY_MARK      0x16a  /* mark key */
#define KEY_MESSAGE   0x16b  /* message key */
#define KEY_MOVE      0x16c  /* move key */
#define KEY_NEXT      0x16d  /* next object key */
#define KEY_OPEN      0x16e  /* open key */
#define KEY_OPTIONS   0x16f  /* options key */
#define KEY_PREVIOUS  0x170  /* previous object key */
#define KEY_REDO      0x171  /* redo key */
#define KEY_REFERENCE 0x172  /* ref(erence) key */
#define KEY_REFRESH   0x173  /* refresh key */
#define KEY_REPLACE   0x174  /* replace key */
#define KEY_RESTART   0x175  /* restart key */
#define KEY_RESUME    0x176  /* resume key */
#define KEY_SAVE      0x177  /* save key */
#define KEY_SBEG      0x178  /* shifted beginning key */
#define KEY_SCANCEL   0x179  /* shifted cancel key */
#define KEY_SCOMMAND  0x17a  /* shifted command key */
#define KEY_SCOPY     0x17b  /* shifted copy key */
#define KEY_SCREATE   0x17c  /* shifted create key */
#define KEY_SDC       0x17d  /* shifted delete char key */
#define KEY_SDL       0x17e  /* shifted delete line key */
#define KEY_SELECT    0x17f  /* select key */
#define KEY_SEND      0x180  /* shifted end key */
#define KEY_SEOL      0x181  /* shifted clear line key */
#define KEY_SEXIT     0x182  /* shifted exit key */
#define KEY_SFIND     0x183  /* shifted find key */
#define KEY_SHOME     0x184  /* shifted home key */
#define KEY_SIC       0x185  /* shifted input key */

#define KEY_SLEFT     0x187  /* shifted left arrow key */
#define KEY_SMESSAGE  0x188  /* shifted message key */
#define KEY_SMOVE     0x189  /* shifted move key */
#define KEY_SNEXT     0x18a  /* shifted next key */
#define KEY_SOPTIONS  0x18b  /* shifted options key */
#define KEY_SPREVIOUS 0x18c  /* shifted prev key */
#define KEY_SPRINT    0x18d  /* shifted print key */
#define KEY_SREDO     0x18e  /* shifted redo key */
#define KEY_SREPLACE  0x18f  /* shifted replace key */
#define KEY_SRIGHT    0x190  /* shifted right arrow */
#define KEY_SRSUME    0x191  /* shifted resume key */
#define KEY_SSAVE     0x192  /* shifted save key */
#define KEY_SSUSPEND  0x193  /* shifted suspend key */
#define KEY_SUNDO     0x194  /* shifted undo key */
#define KEY_SUSPEND   0x195  /* suspend key */
#define KEY_UNDO      0x196  /* undo key */

/* PDCurses-specific key definitions -- PC only */

#define ALT_0         0x197
#define ALT_1         0x198
#define ALT_2         0x199
#define ALT_3         0x19a
#define ALT_4         0x19b
#define ALT_5         0x19c
#define ALT_6         0x19d
#define ALT_7         0x19e
#define ALT_8         0x19f
#define ALT_9         0x1a0
#define ALT_A         0x1a1
#define ALT_B         0x1a2
#define ALT_C         0x1a3
#define ALT_D         0x1a4
#define ALT_E         0x1a5
#define ALT_F         0x1a6
#define ALT_G         0x1a7
#define ALT_H         0x1a8
#define ALT_I         0x1a9
#define ALT_J         0x1aa
#define ALT_K         0x1ab
#define ALT_L         0x1ac
#define ALT_M         0x1ad
#define ALT_N         0x1ae
#define ALT_O         0x1af
#define ALT_P         0x1b0
#define ALT_Q         0x1b1
#define ALT_R         0x1b2
#define ALT_S         0x1b3
#define ALT_T         0x1b4
#define ALT_U         0x1b5
#define ALT_V         0x1b6
#define ALT_W         0x1b7
#define ALT_X         0x1b8
#define ALT_Y         0x1b9
#define ALT_Z         0x1ba

#define CTL_LEFT      0x1bb  /* Control-Left-Arrow */
#define CTL_RIGHT     0x1bc
#define CTL_PGUP      0x1bd
#define CTL_PGDN      0x1be
#define CTL_HOME      0x1bf
#define CTL_END       0x1c0

#define KEY_A1        0x1c1  /* upper left on Virtual keypad */
#define KEY_A2        0x1c2  /* upper middle on Virt. keypad */
#define KEY_A3        0x1c3  /* upper right on Vir. keypad */
#define KEY_B1        0x1c4  /* middle left on Virt. keypad */
#define KEY_B2        0x1c5  /* center on Virt. keypad */
#define KEY_B3        0x1c6  /* middle right on Vir. keypad */
#define KEY_C1        0x1c7  /* lower left on Virt. keypad */
#define KEY_C2        0x1c8  /* lower middle on Virt. keypad */
#define KEY_C3        0x1c9  /* lower right on Vir. keypad */

#define PADSLASH      0x1ca  /* slash on keypad */
#define PADENTER      0x1cb  /* enter on keypad */
#define CTL_PADENTER  0x1cc  /* ctl-enter on keypad */
#define ALT_PADENTER  0x1cd  /* alt-enter on keypad */
#define PADSTOP       0x1ce  /* stop on keypad */
#define PADSTAR       0x1cf  /* star on keypad */
#define PADMINUS      0x1d0  /* minus on keypad */
#define PADPLUS       0x1d1  /* plus on keypad */
#define CTL_PADSTOP   0x1d2  /* ctl-stop on keypad */
#define CTL_PADCENTER 0x1d3  /* ctl-enter on keypad */
#define CTL_PADPLUS   0x1d4  /* ctl-plus on keypad */
#define CTL_PADMINUS  0x1d5  /* ctl-minus on keypad */
#define CTL_PADSLASH  0x1d6  /* ctl-slash on keypad */
#define CTL_PADSTAR   0x1d7  /* ctl-star on keypad */
#define ALT_PADPLUS   0x1d8  /* alt-plus on keypad */
#define ALT_PADMINUS  0x1d9  /* alt-minus on keypad */
#define ALT_PADSLASH  0x1da  /* alt-slash on keypad */
#define ALT_PADSTAR   0x1db  /* alt-star on keypad */
#define ALT_PADSTOP   0x1dc  /* alt-stop on keypad */
#define CTL_INS       0x1dd  /* ctl-insert */
#define ALT_DEL       0x1de  /* alt-delete */
#define ALT_INS       0x1df  /* alt-insert */
#define CTL_UP        0x1e0  /* ctl-up arrow */
#define CTL_DOWN      0x1e1  /* ctl-down arrow */
#define CTL_TAB       0x1e2  /* ctl-tab */
#define ALT_TAB       0x1e3
#define ALT_MINUS     0x1e4
#define ALT_EQUAL     0x1e5
#define ALT_HOME      0x1e6
#define ALT_PGUP      0x1e7
#define ALT_PGDN      0x1e8
#define ALT_END       0x1e9
#define ALT_UP        0x1ea  /* alt-up arrow */
#define ALT_DOWN      0x1eb  /* alt-down arrow */
#define ALT_RIGHT     0x1ec  /* alt-right arrow */
#define ALT_LEFT      0x1ed  /* alt-left arrow */
#define ALT_ENTER     0x1ee  /* alt-enter */
#define ALT_ESC       0x1ef  /* alt-escape */
#define ALT_BQUOTE    0x1f0  /* alt-back quote */
#define ALT_LBRACKET  0x1f1  /* alt-left bracket */
#define ALT_RBRACKET  0x1f2  /* alt-right bracket */
#define ALT_SEMICOLON 0x1f3  /* alt-semi-colon */
#define ALT_FQUOTE    0x1f4  /* alt-forward quote */
#define ALT_COMMA     0x1f5  /* alt-comma */
#define ALT_STOP      0x1f6  /* alt-stop */
#define ALT_FSLASH    0x1f7  /* alt-forward slash */
#define ALT_BKSP      0x1f8  /* alt-backspace */
#define CTL_BKSP      0x1f9  /* ctl-backspace */
#define PAD0          0x1fa  /* keypad 0 */

#define CTL_PAD0      0x1fb  /* ctl-keypad 0 */
#define CTL_PAD1      0x1fc
#define CTL_PAD2      0x1fd
#define CTL_PAD3      0x1fe
#define CTL_PAD4      0x1ff
#define CTL_PAD5      0x200
#define CTL_PAD6      0x201
#define CTL_PAD7      0x202
#define CTL_PAD8      0x203
#define CTL_PAD9      0x204

#define ALT_PAD0      0x205  /* alt-keypad 0 */
#define ALT_PAD1      0x206
#define ALT_PAD2      0x207
#define ALT_PAD3      0x208
#define ALT_PAD4      0x209
#define ALT_PAD5      0x20a
#define ALT_PAD6      0x20b
#define ALT_PAD7      0x20c
#define ALT_PAD8      0x20d
#define ALT_PAD9      0x20e

#define CTL_DEL       0x20f  /* clt-delete */
#define ALT_BSLASH    0x210  /* alt-back slash */
#define CTL_ENTER     0x211  /* ctl-enter */

#define SHF_PADENTER  0x212  /* shift-enter on keypad */
#define SHF_PADSLASH  0x213  /* shift-slash on keypad */
#define SHF_PADSTAR   0x214  /* shift-star  on keypad */
#define SHF_PADPLUS   0x215  /* shift-plus  on keypad */
#define SHF_PADMINUS  0x216  /* shift-minus on keypad */
#define SHF_UP        0x217  /* shift-up on keypad */
#define SHF_DOWN      0x218  /* shift-down on keypad */
#define SHF_IC        0x219  /* shift-insert on keypad */
#define SHF_DC        0x21a  /* shift-delete on keypad */

#define KEY_MOUSE     0x21b  /* "mouse" key */
#define KEY_SHIFT_L   0x21c  /* Left-shift */
#define KEY_SHIFT_R   0x21d  /* Right-shift */
#define KEY_CONTROL_L 0x21e  /* Left-control */
#define KEY_CONTROL_R 0x21f  /* Right-control */
#define KEY_ALT_L     0x220  /* Left-alt */
#define KEY_ALT_R     0x221  /* Right-alt */
#define KEY_RESIZE    0x222  /* Window resize */
#define KEY_SUP       0x223  /* Shifted up arrow */
#define KEY_SDOWN     0x224  /* Shifted down arrow */

#define KEY_MIN       KEY_BREAK      /* Minimum curses key value */
#define KEY_MAX       KEY_SDOWN      /* Maximum curses key */

#define KEY_F(n)      (KEY_F0 + (n))

/*----------------------------------------------------------------------
 *
 *  Functions
 *
 */

/* Standard */

PDCEX  int     addch(const chtype);
PDCEX  int     addchnstr(const chtype *, int);
PDCEX  int     addchstr(const chtype *);
PDCEX  int     addnstr(const char *, int);
PDCEX  int     addstr(const char *);
PDCEX  int     attroff(chtype);
PDCEX  int     attron(chtype);
PDCEX  int     attrset(chtype);
PDCEX  int     attr_get(attr_t *, short *, void *);
PDCEX  int     attr_off(attr_t, void *);
PDCEX  int     attr_on(attr_t, void *);
PDCEX  int     attr_set(attr_t, short, void *);
PDCEX  int     baudrate(void);
PDCEX  int     beep(void);
PDCEX  int     bkgd(chtype);
PDCEX  void    bkgdset(chtype);
PDCEX  int     border(chtype, chtype, chtype, chtype,
                      chtype, chtype, chtype, chtype);
PDCEX  int     box(WINDOW *, chtype, chtype);
PDCEX  bool    can_change_color(void);
PDCEX  int     cbreak(void);
PDCEX  int     chgat(int, attr_t, short, const void *);
PDCEX  int     clearok(WINDOW *, bool);
PDCEX  int     clear(void);
PDCEX  int     clrtobot(void);
PDCEX  int     clrtoeol(void);
PDCEX  int     color_content(short, short *, short *, short *);
PDCEX  int     color_set(short, void *);
PDCEX  int     copywin(const WINDOW *, WINDOW *, int, int, int,
                       int, int, int, int);
PDCEX  int     curs_set(int);
PDCEX  int     def_prog_mode(void);
PDCEX  int     def_shell_mode(void);
PDCEX  int     delay_output(int);
PDCEX  int     delch(void);
PDCEX  int     deleteln(void);
PDCEX  void    delscreen(SCREEN *);
PDCEX  int     delwin(WINDOW *);
PDCEX  WINDOW *derwin(WINDOW *, int, int, int, int);
PDCEX  int     doupdate(void);
PDCEX  WINDOW *dupwin(WINDOW *);
PDCEX  int     echochar(const chtype);
PDCEX  int     echo(void);
PDCEX  int     endwin(void);
PDCEX  char    erasechar(void);
PDCEX  int     erase(void);
PDCEX  void    filter(void);
PDCEX  int     flash(void);
PDCEX  int     flushinp(void);
PDCEX  chtype  getbkgd(WINDOW *);
PDCEX  int     getnstr(char *, int);
PDCEX  int     getstr(char *);
PDCEX  WINDOW *getwin(FILE *);
PDCEX  int     halfdelay(int);
PDCEX  bool    has_colors(void);
PDCEX  bool    has_ic(void);
PDCEX  bool    has_il(void);
PDCEX  int     hline(chtype, int);
PDCEX  void    idcok(WINDOW *, bool);
PDCEX  int     idlok(WINDOW *, bool);
PDCEX  void    immedok(WINDOW *, bool);
PDCEX  int     inchnstr(chtype *, int);
PDCEX  int     inchstr(chtype *);
PDCEX  chtype  inch(void);
PDCEX  int     init_color(short, short, short, short);
PDCEX  int     init_pair(short, short, short);
PDCEX  WINDOW *initscr(void);
PDCEX  int     innstr(char *, int);
PDCEX  int     insch(chtype);
PDCEX  int     insdelln(int);
PDCEX  int     insertln(void);
PDCEX  int     insnstr(const char *, int);
PDCEX  int     insstr(const char *);
PDCEX  int     instr(char *);
PDCEX  int     intrflush(WINDOW *, bool);
PDCEX  bool    isendwin(void);
PDCEX  bool    is_linetouched(WINDOW *, int);
PDCEX  bool    is_wintouched(WINDOW *);
PDCEX  char   *keyname(int);
PDCEX  int     keypad(WINDOW *, bool);
PDCEX  char    killchar(void);
PDCEX  int     leaveok(WINDOW *, bool);
PDCEX  char   *longname(void);
PDCEX  int     meta(WINDOW *, bool);
PDCEX  int     move(int, int);
PDCEX  int     mvaddch(int, int, const chtype);
PDCEX  int     mvaddchnstr(int, int, const chtype *, int);
PDCEX  int     mvaddchstr(int, int, const chtype *);
PDCEX  int     mvaddnstr(int, int, const char *, int);
PDCEX  int     mvaddstr(int, int, const char *);
PDCEX  int     mvchgat(int, int, int, attr_t, short, const void *);
PDCEX  int     mvcur(int, int, int, int);
PDCEX  int     mvdelch(int, int);
PDCEX  int     mvderwin(WINDOW *, int, int);
PDCEX  int     mvgetch(int, int);
PDCEX  int     mvgetnstr(int, int, char *, int);
PDCEX  int     mvgetstr(int, int, char *);
PDCEX  int     mvhline(int, int, chtype, int);
PDCEX  chtype  mvinch(int, int);
PDCEX  int     mvinchnstr(int, int, chtype *, int);
PDCEX  int     mvinchstr(int, int, chtype *);
PDCEX  int     mvinnstr(int, int, char *, int);
PDCEX  int     mvinsch(int, int, chtype);
PDCEX  int     mvinsnstr(int, int, const char *, int);
PDCEX  int     mvinsstr(int, int, const char *);
PDCEX  int     mvinstr(int, int, char *);
PDCEX  int     mvprintw(int, int, const char *, ...);
PDCEX  int     mvscanw(int, int, const char *, ...);
PDCEX  int     mvvline(int, int, chtype, int);
PDCEX  int     mvwaddchnstr(WINDOW *, int, int, const chtype *, int);
PDCEX  int     mvwaddchstr(WINDOW *, int, int, const chtype *);
PDCEX  int     mvwaddch(WINDOW *, int, int, const chtype);
PDCEX  int     mvwaddnstr(WINDOW *, int, int, const char *, int);
PDCEX  int     mvwaddstr(WINDOW *, int, int, const char *);
PDCEX  int     mvwchgat(WINDOW *, int, int, int, attr_t, short, const void *);
PDCEX  int     mvwdelch(WINDOW *, int, int);
PDCEX  int     mvwgetch(WINDOW *, int, int);
PDCEX  int     mvwgetnstr(WINDOW *, int, int, char *, int);
PDCEX  int     mvwgetstr(WINDOW *, int, int, char *);
PDCEX  int     mvwhline(WINDOW *, int, int, chtype, int);
PDCEX  int     mvwinchnstr(WINDOW *, int, int, chtype *, int);
PDCEX  int     mvwinchstr(WINDOW *, int, int, chtype *);
PDCEX  chtype  mvwinch(WINDOW *, int, int);
PDCEX  int     mvwinnstr(WINDOW *, int, int, char *, int);
PDCEX  int     mvwinsch(WINDOW *, int, int, chtype);
PDCEX  int     mvwinsnstr(WINDOW *, int, int, const char *, int);
PDCEX  int     mvwinsstr(WINDOW *, int, int, const char *);
PDCEX  int     mvwinstr(WINDOW *, int, int, char *);
PDCEX  int     mvwin(WINDOW *, int, int);
PDCEX  int     mvwprintw(WINDOW *, int, int, const char *, ...);
PDCEX  int     mvwscanw(WINDOW *, int, int, const char *, ...);
PDCEX  int     mvwvline(WINDOW *, int, int, chtype, int);
PDCEX  int     napms(int);
PDCEX  WINDOW *newpad(int, int);
PDCEX  SCREEN *newterm(const char *, FILE *, FILE *);
PDCEX  WINDOW *newwin(int, int, int, int);
PDCEX  int     nl(void);
PDCEX  int     nocbreak(void);
PDCEX  int     nodelay(WINDOW *, bool);
PDCEX  int     noecho(void);
PDCEX  int     nonl(void);
PDCEX  void    noqiflush(void);
PDCEX  int     noraw(void);
PDCEX  int     notimeout(WINDOW *, bool);
PDCEX  int     overlay(const WINDOW *, WINDOW *);
PDCEX  int     overwrite(const WINDOW *, WINDOW *);
PDCEX  int     pair_content(short, short *, short *);
PDCEX  int     pechochar(WINDOW *, chtype);
PDCEX  int     pnoutrefresh(WINDOW *, int, int, int, int, int, int);
PDCEX  int     prefresh(WINDOW *, int, int, int, int, int, int);
PDCEX  int     printw(const char *, ...);
PDCEX  int     putwin(WINDOW *, FILE *);
PDCEX  void    qiflush(void);
PDCEX  int     raw(void);
PDCEX  int     redrawwin(WINDOW *);
PDCEX  int     refresh(void);
PDCEX  int     reset_prog_mode(void);
PDCEX  int     reset_shell_mode(void);
PDCEX  int     resetty(void);
PDCEX  int     ripoffline(int, int (*)(WINDOW *, int));
PDCEX  int     savetty(void);
PDCEX  int     scanw(const char *, ...);
PDCEX  int     scr_dump(const char *);
PDCEX  int     scr_init(const char *);
PDCEX  int     scr_restore(const char *);
PDCEX  int     scr_set(const char *);
PDCEX  int     scrl(int);
PDCEX  int     scroll(WINDOW *);
PDCEX  int     scrollok(WINDOW *, bool);
PDCEX  SCREEN *set_term(SCREEN *);
PDCEX  int     setscrreg(int, int);
PDCEX  int     slk_attroff(const chtype);
PDCEX  int     slk_attr_off(const attr_t, void *);
PDCEX  int     slk_attron(const chtype);
PDCEX  int     slk_attr_on(const attr_t, void *);
PDCEX  int     slk_attrset(const chtype);
PDCEX  int     slk_attr_set(const attr_t, short, void *);
PDCEX  int     slk_clear(void);
PDCEX  int     slk_color(short);
PDCEX  int     slk_init(int);
PDCEX  char   *slk_label(int);
PDCEX  int     slk_noutrefresh(void);
PDCEX  int     slk_refresh(void);
PDCEX  int     slk_restore(void);
PDCEX  int     slk_set(int, const char *, int);
PDCEX  int     slk_touch(void);
PDCEX  int     standend(void);
PDCEX  int     standout(void);
PDCEX  int     start_color(void);
PDCEX  WINDOW *subpad(WINDOW *, int, int, int, int);
PDCEX  WINDOW *subwin(WINDOW *, int, int, int, int);
PDCEX  int     syncok(WINDOW *, bool);
PDCEX  chtype  termattrs(void);
PDCEX  attr_t  term_attrs(void);
PDCEX  char   *termname(void);
PDCEX  void    timeout(int);
PDCEX  int     touchline(WINDOW *, int, int);
PDCEX  int     touchwin(WINDOW *);
PDCEX  int     typeahead(int);
PDCEX  int     untouchwin(WINDOW *);
PDCEX  void    use_env(bool);
PDCEX  int     vidattr(chtype);
PDCEX  int     vid_attr(attr_t, short, void *);
PDCEX  int     vidputs(chtype, int (*)(int));
PDCEX  int     vid_puts(attr_t, short, void *, int (*)(int));
PDCEX  int     vline(chtype, int);
PDCEX  int     vw_printw(WINDOW *, const char *, va_list);
PDCEX  int     vwprintw(WINDOW *, const char *, va_list);
PDCEX  int     vw_scanw(WINDOW *, const char *, va_list);
PDCEX  int     vwscanw(WINDOW *, const char *, va_list);
PDCEX  int     waddchnstr(WINDOW *, const chtype *, int);
PDCEX  int     waddchstr(WINDOW *, const chtype *);
PDCEX  int     waddch(WINDOW *, const chtype);
PDCEX  int     waddnstr(WINDOW *, const char *, int);
PDCEX  int     waddstr(WINDOW *, const char *);
PDCEX  int     wattroff(WINDOW *, chtype);
PDCEX  int     wattron(WINDOW *, chtype);
PDCEX  int     wattrset(WINDOW *, chtype);
PDCEX  int     wattr_get(WINDOW *, attr_t *, short *, void *);
PDCEX  int     wattr_off(WINDOW *, attr_t, void *);
PDCEX  int     wattr_on(WINDOW *, attr_t, void *);
PDCEX  int     wattr_set(WINDOW *, attr_t, short, void *);
PDCEX  void    wbkgdset(WINDOW *, chtype);
PDCEX  int     wbkgd(WINDOW *, chtype);
PDCEX  int     wborder(WINDOW *, chtype, chtype, chtype, chtype,
                       chtype, chtype, chtype, chtype);
PDCEX  int     wchgat(WINDOW *, int, attr_t, short, const void *);
PDCEX  int     wclear(WINDOW *);
PDCEX  int     wclrtobot(WINDOW *);
PDCEX  int     wclrtoeol(WINDOW *);
PDCEX  int     wcolor_set(WINDOW *, short, void *);
PDCEX  void    wcursyncup(WINDOW *);
PDCEX  int     wdelch(WINDOW *);
PDCEX  int     wdeleteln(WINDOW *);
PDCEX  int     wechochar(WINDOW *, const chtype);
PDCEX  int     werase(WINDOW *);
PDCEX  int     wgetch(WINDOW *);
PDCEX  int     wgetnstr(WINDOW *, char *, int);
PDCEX  int     wgetstr(WINDOW *, char *);
PDCEX  int     whline(WINDOW *, chtype, int);
PDCEX  int     winchnstr(WINDOW *, chtype *, int);
PDCEX  int     winchstr(WINDOW *, chtype *);
PDCEX  chtype  winch(WINDOW *);
PDCEX  int     winnstr(WINDOW *, char *, int);
PDCEX  int     winsch(WINDOW *, chtype);
PDCEX  int     winsdelln(WINDOW *, int);
PDCEX  int     winsertln(WINDOW *);
PDCEX  int     winsnstr(WINDOW *, const char *, int);
PDCEX  int     winsstr(WINDOW *, const char *);
PDCEX  int     winstr(WINDOW *, char *);
PDCEX  int     wmove(WINDOW *, int, int);
PDCEX  int     wnoutrefresh(WINDOW *);
PDCEX  int     wprintw(WINDOW *, const char *, ...);
PDCEX  int     wredrawln(WINDOW *, int, int);
PDCEX  int     wrefresh(WINDOW *);
PDCEX  int     wscanw(WINDOW *, const char *, ...);
PDCEX  int     wscrl(WINDOW *, int);
PDCEX  int     wsetscrreg(WINDOW *, int, int);
PDCEX  int     wstandend(WINDOW *);
PDCEX  int     wstandout(WINDOW *);
PDCEX  void    wsyncdown(WINDOW *);
PDCEX  void    wsyncup(WINDOW *);
PDCEX  void    wtimeout(WINDOW *, int);
PDCEX  int     wtouchln(WINDOW *, int, int, int);
PDCEX  int     wvline(WINDOW *, chtype, int);

/* Wide-character functions */

#ifdef PDC_WIDE
PDCEX  int     addnwstr(const wchar_t *, int);
PDCEX  int     addwstr(const wchar_t *);
PDCEX  int     add_wch(const cchar_t *);
PDCEX  int     add_wchnstr(const cchar_t *, int);
PDCEX  int     add_wchstr(const cchar_t *);
PDCEX  int     bkgrnd(const cchar_t *);
PDCEX  void    bkgrndset(const cchar_t *);
PDCEX  int     border_set(const cchar_t *, const cchar_t *, const cchar_t *,
                          const cchar_t *, const cchar_t *, const cchar_t *,
                          const cchar_t *, const cchar_t *);
PDCEX  int     box_set(WINDOW *, const cchar_t *, const cchar_t *);
PDCEX  int     echo_wchar(const cchar_t *);
PDCEX  int     erasewchar(wchar_t *);
PDCEX  int     getbkgrnd(cchar_t *);
PDCEX  int     getcchar(const cchar_t *, wchar_t *, attr_t *, short *, void *);
PDCEX  int     getn_wstr(wint_t *, int);
PDCEX  int     get_wch(wint_t *);
PDCEX  int     get_wstr(wint_t *);
PDCEX  int     hline_set(const cchar_t *, int);
PDCEX  int     innwstr(wchar_t *, int);
PDCEX  int     ins_nwstr(const wchar_t *, int);
PDCEX  int     ins_wch(const cchar_t *);
PDCEX  int     ins_wstr(const wchar_t *);
PDCEX  int     inwstr(wchar_t *);
PDCEX  int     in_wch(cchar_t *);
PDCEX  int     in_wchnstr(cchar_t *, int);
PDCEX  int     in_wchstr(cchar_t *);
PDCEX  char   *key_name(wchar_t);
PDCEX  int     killwchar(wchar_t *);
PDCEX  int     mvaddnwstr(int, int, const wchar_t *, int);
PDCEX  int     mvaddwstr(int, int, const wchar_t *);
PDCEX  int     mvadd_wch(int, int, const cchar_t *);
PDCEX  int     mvadd_wchnstr(int, int, const cchar_t *, int);
PDCEX  int     mvadd_wchstr(int, int, const cchar_t *);
PDCEX  int     mvgetn_wstr(int, int, wint_t *, int);
PDCEX  int     mvget_wch(int, int, wint_t *);
PDCEX  int     mvget_wstr(int, int, wint_t *);
PDCEX  int     mvhline_set(int, int, const cchar_t *, int);
PDCEX  int     mvinnwstr(int, int, wchar_t *, int);
PDCEX  int     mvins_nwstr(int, int, const wchar_t *, int);
PDCEX  int     mvins_wch(int, int, const cchar_t *);
PDCEX  int     mvins_wstr(int, int, const wchar_t *);
PDCEX  int     mvinwstr(int, int, wchar_t *);
PDCEX  int     mvin_wch(int, int, cchar_t *);
PDCEX  int     mvin_wchnstr(int, int, cchar_t *, int);
PDCEX  int     mvin_wchstr(int, int, cchar_t *);
PDCEX  int     mvvline_set(int, int, const cchar_t *, int);
PDCEX  int     mvwaddnwstr(WINDOW *, int, int, const wchar_t *, int);
PDCEX  int     mvwaddwstr(WINDOW *, int, int, const wchar_t *);
PDCEX  int     mvwadd_wch(WINDOW *, int, int, const cchar_t *);
PDCEX  int     mvwadd_wchnstr(WINDOW *, int, int, const cchar_t *, int);
PDCEX  int     mvwadd_wchstr(WINDOW *, int, int, const cchar_t *);
PDCEX  int     mvwgetn_wstr(WINDOW *, int, int, wint_t *, int);
PDCEX  int     mvwget_wch(WINDOW *, int, int, wint_t *);
PDCEX  int     mvwget_wstr(WINDOW *, int, int, wint_t *);
PDCEX  int     mvwhline_set(WINDOW *, int, int, const cchar_t *, int);
PDCEX  int     mvwinnwstr(WINDOW *, int, int, wchar_t *, int);
PDCEX  int     mvwins_nwstr(WINDOW *, int, int, const wchar_t *, int);
PDCEX  int     mvwins_wch(WINDOW *, int, int, const cchar_t *);
PDCEX  int     mvwins_wstr(WINDOW *, int, int, const wchar_t *);
PDCEX  int     mvwin_wch(WINDOW *, int, int, cchar_t *);
PDCEX  int     mvwin_wchnstr(WINDOW *, int, int, cchar_t *, int);
PDCEX  int     mvwin_wchstr(WINDOW *, int, int, cchar_t *);
PDCEX  int     mvwinwstr(WINDOW *, int, int, wchar_t *);
PDCEX  int     mvwvline_set(WINDOW *, int, int, const cchar_t *, int);
PDCEX  int     pecho_wchar(WINDOW *, const cchar_t*);
PDCEX  int     setcchar(cchar_t*, const wchar_t*, const attr_t,
                        short, const void*);
PDCEX  int     slk_wset(int, const wchar_t *, int);
PDCEX  int     unget_wch(const wchar_t);
PDCEX  int     vline_set(const cchar_t *, int);
PDCEX  int     waddnwstr(WINDOW *, const wchar_t *, int);
PDCEX  int     waddwstr(WINDOW *, const wchar_t *);
PDCEX  int     wadd_wch(WINDOW *, const cchar_t *);
PDCEX  int     wadd_wchnstr(WINDOW *, const cchar_t *, int);
PDCEX  int     wadd_wchstr(WINDOW *, const cchar_t *);
PDCEX  int     wbkgrnd(WINDOW *, const cchar_t *);
PDCEX  void    wbkgrndset(WINDOW *, const cchar_t *);
PDCEX  int     wborder_set(WINDOW *, const cchar_t *, const cchar_t *,
                           const cchar_t *, const cchar_t *, const cchar_t *,
                           const cchar_t *, const cchar_t *, const cchar_t *);
PDCEX  int     wecho_wchar(WINDOW *, const cchar_t *);
PDCEX  int     wgetbkgrnd(WINDOW *, cchar_t *);
PDCEX  int     wgetn_wstr(WINDOW *, wint_t *, int);
PDCEX  int     wget_wch(WINDOW *, wint_t *);
PDCEX  int     wget_wstr(WINDOW *, wint_t *);
PDCEX  int     whline_set(WINDOW *, const cchar_t *, int);
PDCEX  int     winnwstr(WINDOW *, wchar_t *, int);
PDCEX  int     wins_nwstr(WINDOW *, const wchar_t *, int);
PDCEX  int     wins_wch(WINDOW *, const cchar_t *);
PDCEX  int     wins_wstr(WINDOW *, const wchar_t *);
PDCEX  int     winwstr(WINDOW *, wchar_t *);
PDCEX  int     win_wch(WINDOW *, cchar_t *);
PDCEX  int     win_wchnstr(WINDOW *, cchar_t *, int);
PDCEX  int     win_wchstr(WINDOW *, cchar_t *);
PDCEX  wchar_t *wunctrl(cchar_t *);
PDCEX  int     wvline_set(WINDOW *, const cchar_t *, int);
#endif

/* Quasi-standard */

PDCEX  chtype  getattrs(WINDOW *);
PDCEX  int     getbegx(WINDOW *);
PDCEX  int     getbegy(WINDOW *);
PDCEX  int     getmaxx(WINDOW *);
PDCEX  int     getmaxy(WINDOW *);
PDCEX  int     getparx(WINDOW *);
PDCEX  int     getpary(WINDOW *);
PDCEX  int     getcurx(WINDOW *);
PDCEX  int     getcury(WINDOW *);
PDCEX  void    traceoff(void);
PDCEX  void    traceon(void);
PDCEX  char   *unctrl(chtype);

PDCEX  int     crmode(void);
PDCEX  int     nocrmode(void);
PDCEX  int     draino(int);
PDCEX  int     resetterm(void);
PDCEX  int     fixterm(void);
PDCEX  int     saveterm(void);
PDCEX  void    setsyx(int, int);

PDCEX  int     mouse_set(mmask_t);
PDCEX  int     mouse_on(mmask_t);
PDCEX  int     mouse_off(mmask_t);
PDCEX  int     request_mouse_pos(void);
PDCEX  void    wmouse_position(WINDOW *, int *, int *);
PDCEX  mmask_t getmouse(void);

/* ncurses */

PDCEX  int     alloc_pair(int, int);
PDCEX  int     assume_default_colors(int, int);
PDCEX  const char *curses_version(void);
PDCEX  int     find_pair(int, int);
PDCEX  int     free_pair(int);
PDCEX  bool    has_key(int);
PDCEX  bool    is_cleared(const WINDOW *);
PDCEX  bool    is_idcok(const WINDOW *);
PDCEX  bool    is_idlok(const WINDOW *);
PDCEX  bool    is_immedok(const WINDOW *);
PDCEX  bool    is_keypad(const WINDOW *);
PDCEX  bool    is_leaveok(const WINDOW *);
PDCEX  bool    is_nodelay(const WINDOW *);
PDCEX  bool    is_notimeout(const WINDOW *);
PDCEX  bool    is_pad(const WINDOW *);
PDCEX  bool    is_scrollok(const WINDOW *);
PDCEX  bool    is_subwin(const WINDOW *);
PDCEX  bool    is_syncok(const WINDOW *);
PDCEX  int     set_tabsize(int);
PDCEX  int     use_default_colors(void);
PDCEX  int     wgetdelay(const WINDOW *);
PDCEX  WINDOW *wgetparent(const WINDOW *);
PDCEX  int     wgetscrreg(const WINDOW *, int *, int *);
PDCEX  int     wresize(WINDOW *, int, int);

PDCEX  bool    has_mouse(void);
PDCEX  int     mouseinterval(int);
PDCEX  mmask_t mousemask(mmask_t, mmask_t *);
PDCEX  bool    mouse_trafo(int *, int *, bool);
PDCEX  int     nc_getmouse(MEVENT *);
PDCEX  int     ungetmouse(MEVENT *);
PDCEX  bool    wenclose(const WINDOW *, int, int);
PDCEX  bool    wmouse_trafo(const WINDOW *, int *, int *, bool);

/* PDCurses */

PDCEX  int     addrawch(chtype);
PDCEX  int     insrawch(chtype);
PDCEX  bool    is_termresized(void);
PDCEX  int     mvaddrawch(int, int, chtype);
PDCEX  int     mvdeleteln(int, int);
PDCEX  int     mvinsertln(int, int);
PDCEX  int     mvinsrawch(int, int, chtype);
PDCEX  int     mvwaddrawch(WINDOW *, int, int, chtype);
PDCEX  int     mvwdeleteln(WINDOW *, int, int);
PDCEX  int     mvwinsertln(WINDOW *, int, int);
PDCEX  int     mvwinsrawch(WINDOW *, int, int, chtype);
PDCEX  int     raw_output(bool);
PDCEX  int     resize_term(int, int);
PDCEX  WINDOW *resize_window(WINDOW *, int, int);
PDCEX  int     waddrawch(WINDOW *, chtype);
PDCEX  int     winsrawch(WINDOW *, chtype);
PDCEX  char    wordchar(void);

#ifdef PDC_WIDE
PDCEX  wchar_t *slk_wlabel(int);
#endif

PDCEX  void    PDC_debug(const char *, ...);
PDCEX  void    PDC_get_version(PDC_VERSION *);
PDCEX  int     PDC_ungetch(int);
PDCEX  int     PDC_set_blink(bool);
PDCEX  int     PDC_set_bold(bool);
PDCEX  int     PDC_set_line_color(short);
PDCEX  void    PDC_set_title(const char *);

PDCEX  int     PDC_clearclipboard(void);
PDCEX  int     PDC_freeclipboard(char *);
PDCEX  int     PDC_getclipboard(char **, long *);
PDCEX  int     PDC_setclipboard(const char *, long);

PDCEX  unsigned long PDC_get_key_modifiers(void);
PDCEX  int     PDC_return_key_modifiers(bool);

#ifdef XCURSES
PDCEX  WINDOW *Xinitscr(int, char **);
PDCEX  void    XCursesExit(void);
PDCEX  int     sb_init(void);
PDCEX  int     sb_set_horz(int, int, int);
PDCEX  int     sb_set_vert(int, int, int);
PDCEX  int     sb_get_horz(int *, int *, int *);
PDCEX  int     sb_get_vert(int *, int *, int *);
PDCEX  int     sb_refresh(void);
#endif

/* NetBSD */

PDCEX  int     touchoverlap(const WINDOW *, WINDOW *);
PDCEX  int     underend(void);
PDCEX  int     underscore(void);
PDCEX  int     wunderend(WINDOW *);
PDCEX  int     wunderscore(WINDOW *);

/*** Functions defined as macros ***/

/* getch() and ungetch() conflict with some DOS libraries */

#define getch()            wgetch(stdscr)
#define ungetch(ch)        PDC_ungetch(ch)

#define COLOR_PAIR(n)      (((chtype)(n) << PDC_COLOR_SHIFT) & A_COLOR)
#define PAIR_NUMBER(n)     (((n) & A_COLOR) >> PDC_COLOR_SHIFT)

/* These will _only_ work as macros */

#define getbegyx(w, y, x)  (y = getbegy(w), x = getbegx(w))
#define getmaxyx(w, y, x)  (y = getmaxy(w), x = getmaxx(w))
#define getparyx(w, y, x)  (y = getpary(w), x = getparx(w))
#define getyx(w, y, x)     (y = getcury(w), x = getcurx(w))

#define getsyx(y, x)       { if (curscr->_leaveit) (y)=(x)=-1; \
                             else getyx(curscr,(y),(x)); }

#ifdef NCURSES_MOUSE_VERSION
# define getmouse(x) nc_getmouse(x)
#endif

/* Deprecated */

#define PDC_save_key_modifiers(x)  (OK)
#define PDC_get_input_fd()         0

/* return codes from PDC_getclipboard() and PDC_setclipboard() calls */

#define PDC_CLIP_SUCCESS         0
#define PDC_CLIP_ACCESS_ERROR    1
#define PDC_CLIP_EMPTY           2
#define PDC_CLIP_MEMORY_ERROR    3

/* PDCurses key modifier masks */

#define PDC_KEY_MODIFIER_SHIFT   1
#define PDC_KEY_MODIFIER_CONTROL 2
#define PDC_KEY_MODIFIER_ALT     4
#define PDC_KEY_MODIFIER_NUMLOCK 8

#ifdef __cplusplus
# ifndef PDC_PP98
#  undef bool
# endif
}
#endif

/*----------------------------------------------------------------------*
 *                         Panels for PDCurses                          *
 *----------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct panelobs
{
    struct panelobs *above;
    struct panel *pan;
} PANELOBS;

typedef struct panel
{
    WINDOW *win;
    int wstarty;
    int wendy;
    int wstartx;
    int wendx;
    struct panel *below;
    struct panel *above;
    const void *user;
    struct panelobs *obscure;
} PANEL;

PDCEX  int     bottom_panel(PANEL *pan);
PDCEX  int     del_panel(PANEL *pan);
PDCEX  int     hide_panel(PANEL *pan);
PDCEX  int     move_panel(PANEL *pan, int starty, int startx);
PDCEX  PANEL  *new_panel(WINDOW *win);
PDCEX  PANEL  *panel_above(const PANEL *pan);
PDCEX  PANEL  *panel_below(const PANEL *pan);
PDCEX  int     panel_hidden(const PANEL *pan);
PDCEX  const void *panel_userptr(const PANEL *pan);
PDCEX  WINDOW *panel_window(const PANEL *pan);
PDCEX  int     replace_panel(PANEL *pan, WINDOW *win);
PDCEX  int     set_panel_userptr(PANEL *pan, const void *uptr);
PDCEX  int     show_panel(PANEL *pan);
PDCEX  int     top_panel(PANEL *pan);
PDCEX  void    update_panels(void);

#ifdef __cplusplus
}
#endif


/* PDCurses */

/*man-start**************************************************************

addch
-----

### Synopsis

    int addch(const chtype ch);
    int waddch(WINDOW *win, const chtype ch);
    int mvaddch(int y, int x, const chtype ch);
    int mvwaddch(WINDOW *win, int y, int x, const chtype ch);
    int echochar(const chtype ch);
    int wechochar(WINDOW *win, const chtype ch);

    int addrawch(chtype ch);
    int waddrawch(WINDOW *win, chtype ch);
    int mvaddrawch(int y, int x, chtype ch);
    int mvwaddrawch(WINDOW *win, int y, int x, chtype ch);

    int add_wch(const cchar_t *wch);
    int wadd_wch(WINDOW *win, const cchar_t *wch);
    int mvadd_wch(int y, int x, const cchar_t *wch);
    int mvwadd_wch(WINDOW *win, int y, int x, const cchar_t *wch);
    int echo_wchar(const cchar_t *wch);
    int wecho_wchar(WINDOW *win, const cchar_t *wch);

### Description

   addch() adds the chtype ch to the default window (stdscr) at the
   current cursor position, and advances the cursor. Note that chtypes
   can convey both text (a single character) and attributes, including a
   color pair. add_wch() is the wide-character version of this function,
   taking a pointer to a cchar_t instead of a chtype.

   waddch() is like addch(), but also lets you specify the window. (This
   is in fact the core output routine.) wadd_wch() is the wide version.

   mvaddch() moves the cursor to the specified (y, x) position, and adds
   ch to stdscr. mvadd_wch() is the wide version.

   mvwaddch() moves the cursor to the specified position and adds ch to
   the specified window. mvwadd_wch() is the wide version.

   echochar() adds ch to stdscr at the current cursor position and calls
   refresh(). echo_wchar() is the wide version.

   wechochar() adds ch to the specified window and calls wrefresh().
   wecho_wchar() is the wide version.

   addrawch(), waddrawch(), mvaddrawch() and mvwaddrawch() are PDCurses-
   specific wrappers for addch() etc. that disable the translation of
   control characters.

   The following applies to all these functions:

   If the cursor moves on to the right margin, an automatic newline is
   performed. If scrollok is enabled, and a character is added to the
   bottom right corner of the window, the scrolling region will be
   scrolled up one line. If scrolling is not allowed, ERR will be
   returned.

   If ch is a tab, newline, or backspace, the cursor will be moved
   appropriately within the window. If ch is a newline, the clrtoeol
   routine is called before the cursor is moved to the beginning of the
   next line. If newline mapping is off, the cursor will be moved to
   the next line, but the x coordinate will be unchanged. If ch is a
   tab the cursor is moved to the next tab position within the window.
   If ch is another control character, it will be drawn in the ^X
   notation. Calling the inch() routine after adding a control
   character returns the representation of the control character, not
   the control character.

   Video attributes can be combined with a character by ORing them into
   the parameter. Text, including attributes, can be copied from one
   place to another by using inch() and addch().

   Note that in PDCurses, for now, a cchar_t and a chtype are the same.
   The text field is 16 bits wide, and is treated as Unicode (UCS-2)
   when PDCurses is built with wide-character support (define PDC_WIDE).
   So, in functions that take a chtype, like addch(), both the wide and
   narrow versions will handle Unicode. But for portability, you should
   use the wide functions.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   addch                 |    Y   |    Y    |   Y
   waddch                |    Y   |    Y    |   Y
   mvaddch               |    Y   |    Y    |   Y
   mvwaddch              |    Y   |    Y    |   Y
   echochar              |    Y   |    Y    |   Y
   wechochar             |    Y   |    Y    |   Y
   add_wch               |    Y   |    Y    |   Y
   wadd_wch              |    Y   |    Y    |   Y
   mvadd_wch             |    Y   |    Y    |   Y
   mvwadd_wch            |    Y   |    Y    |   Y
   echo_wchar            |    Y   |    Y    |   Y
   wecho_wchar           |    Y   |    Y    |   Y
   addrawch              |    -   |    -    |   -
   waddrawch             |    -   |    -    |   -
   mvaddrawch            |    -   |    -    |   -
   mvwaddrawch           |    -   |    -    |   -

**man-end****************************************************************/

int waddch(WINDOW *win, const chtype ch)
{
    int x, y;
    chtype text, attr;
    bool xlat;

    PDC_LOG(("waddch() - called: win=%p ch=%x (text=%c attr=0x%x)\n",
             win, ch, ch & A_CHARTEXT, ch & A_ATTRIBUTES));

    if (!win || !SP)
        return ERR;

    x = win->_curx;
    y = win->_cury;

    if (y > win->_maxy || x > win->_maxx || y < 0 || x < 0)
        return ERR;

    xlat = !SP->raw_out && !(ch & A_ALTCHARSET);
    text = ch & A_CHARTEXT;
    attr = ch & A_ATTRIBUTES;

    if (xlat && (text < ' ' || text == 0x7f))
    {
        int x2;

        switch (text)
        {
        case '\t':
            for (x2 = ((x / TABSIZE) + 1) * TABSIZE; x < x2; x++)
            {
                if (waddch(win, attr | ' ') == ERR)
                    return ERR;

                /* if tab to next line, exit the loop */

                if (!win->_curx)
                    break;
            }
            return OK;

        case '\n':
            /* if lf -> crlf */

            if (!SP->raw_out)
                x = 0;

            wclrtoeol(win);

            if (++y > win->_bmarg)
            {
                y--;

                if (wscrl(win, 1) == ERR)
                    return ERR;
            }

            break;

        case '\b':
            /* don't back over left margin */

            if (--x < 0)
        case '\r':
                x = 0;

            break;

        case 0x7f:
            if (waddch(win, attr | '^') == ERR)
                return ERR;

            return waddch(win, attr | '?');

        default:
            /* handle control chars */

            if (waddch(win, attr | '^') == ERR)
                return ERR;

            return waddch(win, ch + '@');
        }
    }
    else
    {
        /* If the incoming character doesn't have its own attribute,
           then use the current attributes for the window. If it has
           attributes but not a color component, OR the attributes to
           the current attributes for the window. If it has a color
           component, use the attributes solely from the incoming
           character. */

        if (!(attr & A_COLOR))
            attr |= win->_attrs;

        /* wrs (4/10/93): Apply the same sort of logic for the window
           background, in that it only takes precedence if other color
           attributes are not there and that the background character
           will only print if the printing character is blank. */

        if (!(attr & A_COLOR))
            attr |= win->_bkgd & A_ATTRIBUTES;
        else
            attr |= win->_bkgd & (A_ATTRIBUTES ^ A_COLOR);

        if (text == ' ')
            text = win->_bkgd & A_CHARTEXT;

        /* Add the attribute back into the character. */

        text |= attr;

        /* Only change _firstch/_lastch if the character to be added is
           different from the character/attribute that is already in
           that position in the window. */

        if (win->_y[y][x] != text)
        {
            if (win->_firstch[y] == _NO_CHANGE)
                win->_firstch[y] = win->_lastch[y] = x;
            else
                if (x < win->_firstch[y])
                    win->_firstch[y] = x;
                else
                    if (x > win->_lastch[y])
                        win->_lastch[y] = x;

            win->_y[y][x] = text;
        }

        if (++x >= win->_maxx)
        {
            /* wrap around test */

            x = 0;

            if (++y > win->_bmarg)
            {
                y--;

                if (wscrl(win, 1) == ERR)
                {
                    PDC_sync(win);
                    return ERR;
                }
            }
        }
    }

    win->_curx = x;
    win->_cury = y;

    if (win->_immed)
        wrefresh(win);
    if (win->_sync)
        wsyncup(win);

    return OK;
}

int addch(const chtype ch)
{
    PDC_LOG(("addch() - called: ch=%x\n", ch));

    return waddch(stdscr, ch);
}

int mvaddch(int y, int x, const chtype ch)
{
    PDC_LOG(("mvaddch() - called: y=%d x=%d ch=%x\n", y, x, ch));

    if (move(y,x) == ERR)
        return ERR;

    return waddch(stdscr, ch);
}

int mvwaddch(WINDOW *win, int y, int x, const chtype ch)
{
    PDC_LOG(("mvwaddch() - called: win=%p y=%d x=%d ch=%d\n", win, y, x, ch));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddch(win, ch);
}

int echochar(const chtype ch)
{
    PDC_LOG(("echochar() - called: ch=%x\n", ch));

    return wechochar(stdscr, ch);
}

int wechochar(WINDOW *win, const chtype ch)
{
    PDC_LOG(("wechochar() - called: win=%p ch=%x\n", win, ch));

    if (waddch(win, ch) == ERR)
        return ERR;

    return wrefresh(win);
}

int waddrawch(WINDOW *win, chtype ch)
{
    PDC_LOG(("waddrawch() - called: win=%p ch=%x (text=%c attr=0x%x)\n",
             win, ch, ch & A_CHARTEXT, ch & A_ATTRIBUTES));

    if ((ch & A_CHARTEXT) < ' ' || (ch & A_CHARTEXT) == 0x7f)
        ch |= A_ALTCHARSET;

    return waddch(win, ch);
}

int addrawch(chtype ch)
{
    PDC_LOG(("addrawch() - called: ch=%x\n", ch));

    return waddrawch(stdscr, ch);
}

int mvaddrawch(int y, int x, chtype ch)
{
    PDC_LOG(("mvaddrawch() - called: y=%d x=%d ch=%d\n", y, x, ch));

    if (move(y, x) == ERR)
        return ERR;

    return waddrawch(stdscr, ch);
}

int mvwaddrawch(WINDOW *win, int y, int x, chtype ch)
{
    PDC_LOG(("mvwaddrawch() - called: win=%p y=%d x=%d ch=%d\n",
             win, y, x, ch));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddrawch(win, ch);
}

#ifdef PDC_WIDE
int wadd_wch(WINDOW *win, const cchar_t *wch)
{
    PDC_LOG(("wadd_wch() - called: win=%p wch=%x\n", win, *wch));

    return wch ? waddch(win, *wch) : ERR;
}

int add_wch(const cchar_t *wch)
{
    PDC_LOG(("add_wch() - called: wch=%x\n", *wch));

    return wadd_wch(stdscr, wch);
}

int mvadd_wch(int y, int x, const cchar_t *wch)
{
    PDC_LOG(("mvaddch() - called: y=%d x=%d wch=%x\n", y, x, *wch));

    if (move(y,x) == ERR)
        return ERR;

    return wadd_wch(stdscr, wch);
}

int mvwadd_wch(WINDOW *win, int y, int x, const cchar_t *wch)
{
    PDC_LOG(("mvwaddch() - called: win=%p y=%d x=%d wch=%d\n",
             win, y, x, *wch));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wadd_wch(win, wch);
}

int echo_wchar(const cchar_t *wch)
{
    PDC_LOG(("echo_wchar() - called: wch=%x\n", *wch));

    return wecho_wchar(stdscr, wch);
}

int wecho_wchar(WINDOW *win, const cchar_t *wch)
{
    PDC_LOG(("wecho_wchar() - called: win=%p wch=%x\n", win, *wch));

    if (!wch || (wadd_wch(win, wch) == ERR))
        return ERR;

    return wrefresh(win);
}
#endif
/* PDCurses */

/*man-start**************************************************************

addchstr
--------

### Synopsis

    int addchstr(const chtype *ch);
    int addchnstr(const chtype *ch, int n);
    int waddchstr(WINDOW *win, const chtype *ch);
    int waddchnstr(WINDOW *win, const chtype *ch, int n);
    int mvaddchstr(int y, int x, const chtype *ch);
    int mvaddchnstr(int y, int x, const chtype *ch, int n);
    int mvwaddchstr(WINDOW *, int y, int x, const chtype *ch);
    int mvwaddchnstr(WINDOW *, int y, int x, const chtype *ch, int n);

    int add_wchstr(const cchar_t *wch);
    int add_wchnstr(const cchar_t *wch, int n);
    int wadd_wchstr(WINDOW *win, const cchar_t *wch);
    int wadd_wchnstr(WINDOW *win, const cchar_t *wch, int n);
    int mvadd_wchstr(int y, int x, const cchar_t *wch);
    int mvadd_wchnstr(int y, int x, const cchar_t *wch, int n);
    int mvwadd_wchstr(WINDOW *win, int y, int x, const cchar_t *wch);
    int mvwadd_wchnstr(WINDOW *win, int y, int x, const cchar_t *wch,
                       int n);

### Description

   These routines write a chtype or cchar_t string directly into the
   window structure, starting at the current or specified position. The
   four routines with n as the last argument copy at most n elements,
   but no more than will fit on the line. If n == -1 then the whole
   string is copied, up to the maximum number that will fit on the line.

   The cursor position is not advanced. These routines do not check for
   newline or other special characters, nor does any line wrapping
   occur.

### Return Value

   All functions return OK or ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   addchstr              |    Y   |    Y    |   Y
   waddchstr             |    Y   |    Y    |   Y
   mvaddchstr            |    Y   |    Y    |   Y
   mvwaddchstr           |    Y   |    Y    |   Y
   addchnstr             |    Y   |    Y    |   Y
   waddchnstr            |    Y   |    Y    |   Y
   mvaddchnstr           |    Y   |    Y    |   Y
   mvwaddchnstr          |    Y   |    Y    |   Y
   add_wchstr            |    Y   |    Y    |   Y
   wadd_wchstr           |    Y   |    Y    |   Y
   mvadd_wchstr          |    Y   |    Y    |   Y
   mvwadd_wchstr         |    Y   |    Y    |   Y
   add_wchnstr           |    Y   |    Y    |   Y
   wadd_wchnstr          |    Y   |    Y    |   Y
   mvadd_wchnstr         |    Y   |    Y    |   Y
   mvwadd_wchnstr        |    Y   |    Y    |   Y

**man-end****************************************************************/

int waddchnstr(WINDOW *win, const chtype *ch, int n)
{
    int y, x, maxx, minx;
    chtype *ptr;

    PDC_LOG(("waddchnstr() - called: win=%p n=%d\n", win, n));

    if (!win || !ch || !n || n < -1)
        return ERR;

    x = win->_curx;
    y = win->_cury;
    ptr = &(win->_y[y][x]);

    if (n == -1 || n > win->_maxx - x)
        n = win->_maxx - x;

    minx = win->_firstch[y];
    maxx = win->_lastch[y];

    for (; n && *ch; n--, x++, ptr++, ch++)
    {
        if (*ptr != *ch)
        {
            if (x < minx || minx == _NO_CHANGE)
                minx = x;

            if (x > maxx)
                maxx = x;

            PDC_LOG(("y %d x %d minx %d maxx %d *ptr %x *ch"
                     " %x firstch: %d lastch: %d\n",
                     y, x, minx, maxx, *ptr, *ch,
                     win->_firstch[y], win->_lastch[y]));

            *ptr = *ch;
        }
    }

    win->_firstch[y] = minx;
    win->_lastch[y] = maxx;

    return OK;
}

int addchstr(const chtype *ch)
{
    PDC_LOG(("addchstr() - called\n"));

    return waddchnstr(stdscr, ch, -1);
}

int addchnstr(const chtype *ch, int n)
{
    PDC_LOG(("addchnstr() - called\n"));

    return waddchnstr(stdscr, ch, n);
}

int waddchstr(WINDOW *win, const chtype *ch)
{
    PDC_LOG(("waddchstr() - called: win=%p\n", win));

    return waddchnstr(win, ch, -1);
}

int mvaddchstr(int y, int x, const chtype *ch)
{
    PDC_LOG(("mvaddchstr() - called: y %d x %d\n", y, x));

    if (move(y, x) == ERR)
        return ERR;

    return waddchnstr(stdscr, ch, -1);
}

int mvaddchnstr(int y, int x, const chtype *ch, int n)
{
    PDC_LOG(("mvaddchnstr() - called: y %d x %d n %d\n", y, x, n));

    if (move(y, x) == ERR)
        return ERR;

    return waddchnstr(stdscr, ch, n);
}

int mvwaddchstr(WINDOW *win, int y, int x, const chtype *ch)
{
    PDC_LOG(("mvwaddchstr() - called:\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddchnstr(win, ch, -1);
}

int mvwaddchnstr(WINDOW *win, int y, int x, const chtype *ch, int n)
{
    PDC_LOG(("mvwaddchnstr() - called: y %d x %d n %d \n", y, x, n));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddchnstr(win, ch, n);
}

#ifdef PDC_WIDE
int wadd_wchnstr(WINDOW *win, const cchar_t *wch, int n)
{
    PDC_LOG(("wadd_wchnstr() - called: win=%p n=%d\n", win, n));

    return waddchnstr(win, wch, n);
}

int add_wchstr(const cchar_t *wch)
{
    PDC_LOG(("add_wchstr() - called\n"));

    return wadd_wchnstr(stdscr, wch, -1);
}

int add_wchnstr(const cchar_t *wch, int n)
{
    PDC_LOG(("add_wchnstr() - called\n"));

    return wadd_wchnstr(stdscr, wch, n);
}

int wadd_wchstr(WINDOW *win, const cchar_t *wch)
{
    PDC_LOG(("wadd_wchstr() - called: win=%p\n", win));

    return wadd_wchnstr(win, wch, -1);
}

int mvadd_wchstr(int y, int x, const cchar_t *wch)
{
    PDC_LOG(("mvadd_wchstr() - called: y %d x %d\n", y, x));

    if (move(y, x) == ERR)
        return ERR;

    return wadd_wchnstr(stdscr, wch, -1);
}

int mvadd_wchnstr(int y, int x, const cchar_t *wch, int n)
{
    PDC_LOG(("mvadd_wchnstr() - called: y %d x %d n %d\n", y, x, n));

    if (move(y, x) == ERR)
        return ERR;

    return wadd_wchnstr(stdscr, wch, n);
}

int mvwadd_wchstr(WINDOW *win, int y, int x, const cchar_t *wch)
{
    PDC_LOG(("mvwadd_wchstr() - called:\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wadd_wchnstr(win, wch, -1);
}

int mvwadd_wchnstr(WINDOW *win, int y, int x, const cchar_t *wch, int n)
{
    PDC_LOG(("mvwadd_wchnstr() - called: y %d x %d n %d \n", y, x, n));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wadd_wchnstr(win, wch, n);
}
#endif
/* PDCurses */

/*man-start**************************************************************

addstr
------

### Synopsis

    int addstr(const char *str);
    int addnstr(const char *str, int n);
    int waddstr(WINDOW *win, const char *str);
    int waddnstr(WINDOW *win, const char *str, int n);
    int mvaddstr(int y, int x, const char *str);
    int mvaddnstr(int y, int x, const char *str, int n);
    int mvwaddstr(WINDOW *win, int y, int x, const char *str);
    int mvwaddnstr(WINDOW *win, int y, int x, const char *str, int n);

    int addwstr(const wchar_t *wstr);
    int addnwstr(const wchar_t *wstr, int n);
    int waddwstr(WINDOW *win, const wchar_t *wstr);
    int waddnwstr(WINDOW *win, const wchar_t *wstr, int n);
    int mvaddwstr(int y, int x, const wchar_t *wstr);
    int mvaddnwstr(int y, int x, const wchar_t *wstr, int n);
    int mvwaddwstr(WINDOW *win, int y, int x, const wchar_t *wstr);
    int mvwaddnwstr(WINDOW *win, int y, int x, const wchar_t *wstr, int n);

### Description

   These routines write all the characters of the null-terminated string
   str or wide-character string wstr to the given window. The
   functionality is similar to calling waddch() once for each character
   in the string; except that, when PDCurses is built with wide-
   character support enabled, the narrow-character functions treat the
   string as a multibyte string in the current locale, and convert it.
   The routines with n as the last argument write at most n characters;
   if n is negative, then the entire string will be added.

### Return Value

   All functions return OK or ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   addstr                |    Y   |    Y    |   Y
   waddstr               |    Y   |    Y    |   Y
   mvaddstr              |    Y   |    Y    |   Y
   mvwaddstr             |    Y   |    Y    |   Y
   addnstr               |    Y   |    Y    |   Y
   waddnstr              |    Y   |    Y    |   Y
   mvaddnstr             |    Y   |    Y    |   Y
   mvwaddnstr            |    Y   |    Y    |   Y
   addwstr               |    Y   |    Y    |   Y
   waddwstr              |    Y   |    Y    |   Y
   mvaddwstr             |    Y   |    Y    |   Y
   mvwaddwstr            |    Y   |    Y    |   Y
   addnwstr              |    Y   |    Y    |   Y
   waddnwstr             |    Y   |    Y    |   Y
   mvaddnwstr            |    Y   |    Y    |   Y
   mvwaddnwstr           |    Y   |    Y    |   Y

**man-end****************************************************************/

int waddnstr(WINDOW *win, const char *str, int n)
{
    int i = 0;

    PDC_LOG(("waddnstr() - called: string=\"%s\" n %d \n", str, n));

    if (!win || !str)
        return ERR;

    while (str[i] && (i < n || n < 0))
    {
#ifdef PDC_WIDE
        wchar_t wch;
        int retval = PDC_mbtowc(&wch, str + i, n >= 0 ? n - i : 6);

        if (retval <= 0)
            return OK;

        i += retval;
#else
        chtype wch = (unsigned char)(str[i++]);
#endif
        if (waddch(win, wch) == ERR)
            return ERR;
    }

    return OK;
}

int addstr(const char *str)
{
    PDC_LOG(("addstr() - called: string=\"%s\"\n", str));

    return waddnstr(stdscr, str, -1);
}

int addnstr(const char *str, int n)
{
    PDC_LOG(("addnstr() - called: string=\"%s\" n %d \n", str, n));

    return waddnstr(stdscr, str, n);
}

int waddstr(WINDOW *win, const char *str)
{
    PDC_LOG(("waddstr() - called: string=\"%s\"\n", str));

    return waddnstr(win, str, -1);
}

int mvaddstr(int y, int x, const char *str)
{
    PDC_LOG(("mvaddstr() - called: y %d x %d string=\"%s\"\n", y, x, str));

    if (move(y, x) == ERR)
        return ERR;

    return waddnstr(stdscr, str, -1);
}

int mvaddnstr(int y, int x, const char *str, int n)
{
    PDC_LOG(("mvaddnstr() - called: y %d x %d string=\"%s\" n %d \n",
             y, x, str, n));

    if (move(y, x) == ERR)
        return ERR;

    return waddnstr(stdscr, str, n);
}

int mvwaddstr(WINDOW *win, int y, int x, const char *str)
{
    PDC_LOG(("mvwaddstr() - called: string=\"%s\"\n", str));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddnstr(win, str, -1);
}

int mvwaddnstr(WINDOW *win, int y, int x, const char *str, int n)
{
    PDC_LOG(("mvwaddnstr() - called: y %d x %d string=\"%s\" n %d \n",
             y, x, str, n));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddnstr(win, str, n);
}

#ifdef PDC_WIDE
int waddnwstr(WINDOW *win, const wchar_t *wstr, int n)
{
    int i = 0;

    PDC_LOG(("waddnwstr() - called\n"));

    if (!win || !wstr)
        return ERR;

    while (wstr[i] && (i < n || n < 0))
    {
        chtype wch = wstr[i++];

        if (waddch(win, wch) == ERR)
            return ERR;
    }

    return OK;
}

int addwstr(const wchar_t *wstr)
{
    PDC_LOG(("addwstr() - called\n"));

    return waddnwstr(stdscr, wstr, -1);
}

int addnwstr(const wchar_t *wstr, int n)
{
    PDC_LOG(("addnwstr() - called\n"));

    return waddnwstr(stdscr, wstr, n);
}

int waddwstr(WINDOW *win, const wchar_t *wstr)
{
    PDC_LOG(("waddwstr() - called\n"));

    return waddnwstr(win, wstr, -1);
}

int mvaddwstr(int y, int x, const wchar_t *wstr)
{
    PDC_LOG(("mvaddstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return waddnwstr(stdscr, wstr, -1);
}

int mvaddnwstr(int y, int x, const wchar_t *wstr, int n)
{
    PDC_LOG(("mvaddnstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return waddnwstr(stdscr, wstr, n);
}

int mvwaddwstr(WINDOW *win, int y, int x, const wchar_t *wstr)
{
    PDC_LOG(("mvwaddstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddnwstr(win, wstr, -1);
}

int mvwaddnwstr(WINDOW *win, int y, int x, const wchar_t *wstr, int n)
{
    PDC_LOG(("mvwaddnstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return waddnwstr(win, wstr, n);
}
#endif
/* PDCurses */

/*man-start**************************************************************

attr
----

### Synopsis

    int attroff(chtype attrs);
    int wattroff(WINDOW *win, chtype attrs);
    int attron(chtype attrs);
    int wattron(WINDOW *win, chtype attrs);
    int attrset(chtype attrs);
    int wattrset(WINDOW *win, chtype attrs);
    int standend(void);
    int wstandend(WINDOW *win);
    int standout(void);
    int wstandout(WINDOW *win);

    int color_set(short color_pair, void *opts);
    int wcolor_set(WINDOW *win, short color_pair, void *opts);

    int attr_get(attr_t *attrs, short *color_pair, void *opts);
    int attr_off(attr_t attrs, void *opts);
    int attr_on(attr_t attrs, void *opts);
    int attr_set(attr_t attrs, short color_pair, void *opts);
    int wattr_get(WINDOW *win, attr_t *attrs, short *color_pair,
                  void *opts);
    int wattr_off(WINDOW *win, attr_t attrs, void *opts);
    int wattr_on(WINDOW *win, attr_t attrs, void *opts);
    int wattr_set(WINDOW *win, attr_t attrs, short color_pair,
                  void *opts);

    int chgat(int n, attr_t attr, short color, const void *opts);
    int mvchgat(int y, int x, int n, attr_t attr, short color,
                const void *opts);
    int mvwchgat(WINDOW *win, int y, int x, int n, attr_t attr,
                 short color, const void *opts);
    int wchgat(WINDOW *win, int n, attr_t attr, short color,
               const void *opts);

    chtype getattrs(WINDOW *win);

    int underend(void);
    int wunderend(WINDOW *win);
    int underscore(void);
    int wunderscore(WINDOW *win);

### Description

   These functions manipulate the current attributes and/or colors of
   the named window. These attributes can be any combination of
   A_STANDOUT, A_REVERSE, A_BOLD, A_DIM, A_BLINK, A_UNDERLINE. These
   constants are defined in <curses.h> and can be combined with the
   bitwise-OR operator (|).

   The current attributes of a window are applied to all chtypes that
   are written into the window with waddch(). Attributes are a property
   of the chtype, and move with the character through any scrolling or
   insert/delete operations.

   wattrset() sets the current attributes of the given window to attrs.
   attrset() is the stdscr version.

   wattroff() turns off the named attributes without affecting any other
   attributes; wattron() turns them on.

   wcolor_set() sets the window color to the value of color_pair. opts
   is unused.

   standout() is the same as attron(A_STANDOUT). standend() is the same
   as attrset(A_NORMAL); that is, it turns off all attributes.

   The attr_* and wattr_* functions are intended for use with the WA_*
   attributes. In PDCurses, these are the same as A_*, and there is no
   difference in bevahior from the chtype-based functions. In all cases,
   opts is unused.

   wattr_get() retrieves the attributes and color pair for the specified
   window.

   wchgat() sets the color pair and attributes for the next n cells on
   the current line of a given window, without changing the existing
   text, or alterting the window's attributes. An n of -1 extends the
   change to the edge of the window. The changes take effect
   immediately. opts is unused.

   wunderscore() turns on the A_UNDERLINE attribute; wunderend() turns
   it off. underscore() and underend() are the stdscr versions.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   attroff               |    Y   |    Y    |   Y
   wattroff              |    Y   |    Y    |   Y
   attron                |    Y   |    Y    |   Y
   wattron               |    Y   |    Y    |   Y
   attrset               |    Y   |    Y    |   Y
   wattrset              |    Y   |    Y    |   Y
   standend              |    Y   |    Y    |   Y
   wstandend             |    Y   |    Y    |   Y
   standout              |    Y   |    Y    |   Y
   wstandout             |    Y   |    Y    |   Y
   color_set             |    Y   |    Y    |   Y
   wcolor_set            |    Y   |    Y    |   Y
   attr_get              |    Y   |    Y    |   Y
   wattr_get             |    Y   |    Y    |   Y
   attr_on               |    Y   |    Y    |   Y
   wattr_on              |    Y   |    Y    |   Y
   attr_off              |    Y   |    Y    |   Y
   wattr_off             |    Y   |    Y    |   Y
   attr_set              |    Y   |    Y    |   Y
   wattr_set             |    Y   |    Y    |   Y
   chgat                 |    Y   |    Y    |   Y
   wchgat                |    Y   |    Y    |   Y
   mvchgat               |    Y   |    Y    |   Y
   mvwchgat              |    Y   |    Y    |   Y
   getattrs              |    -   |    Y    |   Y
   underend              |    -   |    -    |   Y
   wunderend             |    -   |    -    |   Y
   underscore            |    -   |    -    |   Y
   wunderscore           |    -   |    -    |   Y

**man-end****************************************************************/

int wattroff(WINDOW *win, chtype attrs)
{
    PDC_LOG(("wattroff() - called\n"));

    if (!win)
        return ERR;

    win->_attrs &= (~attrs & A_ATTRIBUTES);

    return OK;
}

int attroff(chtype attrs)
{
    PDC_LOG(("attroff() - called\n"));

    return wattroff(stdscr, attrs);
}

int wattron(WINDOW *win, chtype attrs)
{
    chtype newcolr, oldcolr, newattr, oldattr;

    PDC_LOG(("wattron() - called\n"));

    if (!win)
        return ERR;

    if ((win->_attrs & A_COLOR) && (attrs & A_COLOR))
    {
        oldcolr = win->_attrs & A_COLOR;
        oldattr = win->_attrs ^ oldcolr;
        newcolr = attrs & A_COLOR;
        newattr = (attrs & A_ATTRIBUTES) ^ newcolr;
        newattr |= oldattr;
        win->_attrs = newattr | newcolr;
    }
    else
        win->_attrs |= (attrs & A_ATTRIBUTES);

    return OK;
}

int attron(chtype attrs)
{
    PDC_LOG(("attron() - called\n"));

    return wattron(stdscr, attrs);
}

int wattrset(WINDOW *win, chtype attrs)
{
    PDC_LOG(("wattrset() - called\n"));

    if (!win)
        return ERR;

    win->_attrs = attrs & A_ATTRIBUTES;

    return OK;
}

int attrset(chtype attrs)
{
    PDC_LOG(("attrset() - called\n"));

    return wattrset(stdscr, attrs);
}

int standend(void)
{
    PDC_LOG(("standend() - called\n"));

    return wattrset(stdscr, A_NORMAL);
}

int standout(void)
{
    PDC_LOG(("standout() - called\n"));

    return wattrset(stdscr, A_STANDOUT);
}

int wstandend(WINDOW *win)
{
    PDC_LOG(("wstandend() - called\n"));

    return wattrset(win, A_NORMAL);
}

int wstandout(WINDOW *win)
{
    PDC_LOG(("wstandout() - called\n"));

    return wattrset(win, A_STANDOUT);
}

chtype getattrs(WINDOW *win)
{
    return win ? win->_attrs : 0;
}

int wcolor_set(WINDOW *win, short color_pair, void *opts)
{
    PDC_LOG(("wcolor_set() - called\n"));

    if (!win)
        return ERR;

    win->_attrs = (win->_attrs & ~A_COLOR) | COLOR_PAIR(color_pair);

    return OK;
}

int color_set(short color_pair, void *opts)
{
    PDC_LOG(("color_set() - called\n"));

    return wcolor_set(stdscr, color_pair, opts);
}

int wattr_get(WINDOW *win, attr_t *attrs, short *color_pair, void *opts)
{
    PDC_LOG(("wattr_get() - called\n"));

    if (!win)
        return ERR;

    if (attrs)
        *attrs = win->_attrs & (A_ATTRIBUTES & ~A_COLOR);

    if (color_pair)
        *color_pair = PAIR_NUMBER(win->_attrs);

    return OK;
}

int attr_get(attr_t *attrs, short *color_pair, void *opts)
{
    PDC_LOG(("attr_get() - called\n"));

    return wattr_get(stdscr, attrs, color_pair, opts);
}

int wattr_off(WINDOW *win, attr_t attrs, void *opts)
{
    PDC_LOG(("wattr_off() - called\n"));

    return wattroff(win, attrs);
}

int attr_off(attr_t attrs, void *opts)
{
    PDC_LOG(("attr_off() - called\n"));

    return wattroff(stdscr, attrs);
}

int wattr_on(WINDOW *win, attr_t attrs, void *opts)
{
    PDC_LOG(("wattr_off() - called\n"));

    return wattron(win, attrs);
}

int attr_on(attr_t attrs, void *opts)
{
    PDC_LOG(("attr_on() - called\n"));

    return wattron(stdscr, attrs);
}

int wattr_set(WINDOW *win, attr_t attrs, short color_pair, void *opts)
{
    PDC_LOG(("wattr_set() - called\n"));

    if (!win)
        return ERR;

    win->_attrs = (attrs & (A_ATTRIBUTES & ~A_COLOR)) | COLOR_PAIR(color_pair);

    return OK;
}

int attr_set(attr_t attrs, short color_pair, void *opts)
{
    PDC_LOG(("attr_get() - called\n"));

    return wattr_set(stdscr, attrs, color_pair, opts);
}

int wchgat(WINDOW *win, int n, attr_t attr, short color, const void *opts)
{
    chtype *dest, newattr;
    int startpos, endpos;

    PDC_LOG(("wchgat() - called\n"));

    if (!win)
        return ERR;

    newattr = (attr & A_ATTRIBUTES) | COLOR_PAIR(color);

    startpos = win->_curx;
    endpos = ((n < 0) ? win->_maxx : min(startpos + n, win->_maxx)) - 1;
    dest = win->_y[win->_cury];

    for (n = startpos; n <= endpos; n++)
        dest[n] = (dest[n] & A_CHARTEXT) | newattr;

    n = win->_cury;

    if (startpos < win->_firstch[n] || win->_firstch[n] == _NO_CHANGE)
        win->_firstch[n] = startpos;

    if (endpos > win->_lastch[n])
        win->_lastch[n] = endpos;

    PDC_sync(win);

    return OK;
}

int chgat(int n, attr_t attr, short color, const void *opts)
{
    PDC_LOG(("chgat() - called\n"));

    return wchgat(stdscr, n, attr, color, opts);
}

int mvchgat(int y, int x, int n, attr_t attr, short color, const void *opts)
{
    PDC_LOG(("mvchgat() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wchgat(stdscr, n, attr, color, opts);
}

int mvwchgat(WINDOW *win, int y, int x, int n, attr_t attr, short color,
             const void *opts)
{
    PDC_LOG(("mvwchgat() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wchgat(win, n, attr, color, opts);
}

int underend(void)
{
    PDC_LOG(("underend() - called\n"));

    return wattroff(stdscr, A_UNDERLINE);
}

int wunderend(WINDOW *win)
{
    PDC_LOG(("wunderend() - called\n"));

    return wattroff(win, A_UNDERLINE);
}

int underscore(void)
{
    PDC_LOG(("underscore() - called\n"));

    return wattron(stdscr, A_UNDERLINE);
}

int wunderscore(WINDOW *win)
{
    PDC_LOG(("wunderscore() - called\n"));

    return wattron(win, A_UNDERLINE);
}
/* PDCurses */

/*man-start**************************************************************

beep
----

### Synopsis

    int beep(void);
    int flash(void);

### Description

   beep() sounds the audible bell on the terminal, if possible; if not,
   it calls flash().

   flash() "flashes" the screen, by inverting the foreground and
   background of every cell, pausing, and then restoring the original
   attributes.

### Return Value

   These functions return ERR if called before initscr(), otherwise OK.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   beep                  |    Y   |    Y    |   Y
   flash                 |    Y   |    Y    |   Y

**man-end****************************************************************/

int beep(void)
{
    PDC_LOG(("beep() - called\n"));

    if (!SP)
        return ERR;

    if (SP->audible)
        PDC_beep();
    else
        flash();

    return OK;
}

int flash(void)
{
    int z, y, x;

    PDC_LOG(("flash() - called\n"));

    if (!curscr)
        return ERR;

    /* Reverse each cell; wait; restore the screen */

    for (z = 0; z < 2; z++)
    {
        for (y = 0; y < LINES; y++)
            for (x = 0; x < COLS; x++)
                curscr->_y[y][x] ^= A_REVERSE;

        wrefresh(curscr);

        if (!z)
            napms(50);
    }

    return OK;
}
/* PDCurses */

/*man-start**************************************************************

bkgd
----

### Synopsis

    int bkgd(chtype ch);
    void bkgdset(chtype ch);
    chtype getbkgd(WINDOW *win);
    int wbkgd(WINDOW *win, chtype ch);
    void wbkgdset(WINDOW *win, chtype ch);

    int bkgrnd(const cchar_t *wch);
    void bkgrndset(const cchar_t *wch);
    int getbkgrnd(cchar_t *wch);
    int wbkgrnd(WINDOW *win, const cchar_t *wch);
    void wbkgrndset(WINDOW *win, const cchar_t *wch);
    int wgetbkgrnd(WINDOW *win, cchar_t *wch);

### Description

   bkgdset() and wbkgdset() manipulate the background of a window. The
   background is a chtype consisting of any combination of attributes
   and a character; it is combined with each chtype added or inserted to
   the window by waddch() or winsch(). Only the attribute part is used
   to set the background of non-blank characters, while both character
   and attributes are used for blank positions.

   bkgd() and wbkgd() not only change the background, but apply it
   immediately to every cell in the window.

   wbkgrnd(), wbkgrndset() and wgetbkgrnd() are the "wide-character"
   versions of these functions, taking a pointer to a cchar_t instead of
   a chtype. However, in PDCurses, cchar_t and chtype are the same.

   The attributes that are defined with the attrset()/attron() set of
   functions take precedence over the background attributes if there is
   a conflict (e.g., different color pairs).

### Return Value

   bkgd() and wbkgd() return OK, unless the window is NULL, in which
   case they return ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   bkgd                  |    Y   |    Y    |   Y
   bkgdset               |    Y   |    Y    |   Y
   getbkgd               |    Y   |    Y    |   Y
   wbkgd                 |    Y   |    Y    |   Y
   wbkgdset              |    Y   |    Y    |   Y
   bkgrnd                |    Y   |    Y    |   Y
   bkgrndset             |    Y   |    Y    |   Y
   getbkgrnd             |    Y   |    Y    |   Y
   wbkgrnd               |    Y   |    Y    |   Y
   wbkgrndset            |    Y   |    Y    |   Y
   wgetbkgrnd            |    Y   |    Y    |   Y

**man-end****************************************************************/

int wbkgd(WINDOW *win, chtype ch)
{
    int x, y;
    chtype oldcolr, oldch, newcolr, newch, colr, attr;
    chtype oldattr = 0, newattr = 0;
    chtype *winptr;

    PDC_LOG(("wbkgd() - called\n"));

    if (!win)
        return ERR;

    if (win->_bkgd == ch)
        return OK;

    oldcolr = win->_bkgd & A_COLOR;
    if (oldcolr)
        oldattr = (win->_bkgd & A_ATTRIBUTES) ^ oldcolr;

    oldch = win->_bkgd & A_CHARTEXT;

    wbkgdset(win, ch);

    newcolr = win->_bkgd & A_COLOR;
    if (newcolr)
        newattr = (win->_bkgd & A_ATTRIBUTES) ^ newcolr;

    newch = win->_bkgd & A_CHARTEXT;

    /* what follows is what seems to occur in the System V
       implementation of this routine */

    for (y = 0; y < win->_maxy; y++)
    {
        for (x = 0; x < win->_maxx; x++)
        {
            winptr = win->_y[y] + x;

            ch = *winptr;

            /* determine the colors and attributes of the character read
               from the window */

            colr = ch & A_COLOR;
            attr = ch & (A_ATTRIBUTES ^ A_COLOR);

            /* if the color is the same as the old background color,
               then make it the new background color, otherwise leave it */

            if (colr == oldcolr)
                colr = newcolr;

            /* remove any attributes (non color) from the character that
               were part of the old background, then combine the
               remaining ones with the new background */

            attr ^= oldattr;
            attr |= newattr;

            /* change character if it is there because it was the old
               background character */

            ch &= A_CHARTEXT;
            if (ch == oldch)
                ch = newch;

            ch |= (attr | colr);

            *winptr = ch;

        }
    }

    touchwin(win);
    PDC_sync(win);
    return OK;
}

int bkgd(chtype ch)
{
    PDC_LOG(("bkgd() - called\n"));

    return wbkgd(stdscr, ch);
}

void wbkgdset(WINDOW *win, chtype ch)
{
    PDC_LOG(("wbkgdset() - called\n"));

    if (win)
    {
        if (!(ch & A_CHARTEXT))
            ch |= ' ';

        win->_bkgd = ch;
    }
}

void bkgdset(chtype ch)
{
    PDC_LOG(("bkgdset() - called\n"));

    wbkgdset(stdscr, ch);
}

chtype getbkgd(WINDOW *win)
{
    PDC_LOG(("getbkgd() - called\n"));

    return win ? win->_bkgd : (chtype)ERR;
}

#ifdef PDC_WIDE
int wbkgrnd(WINDOW *win, const cchar_t *wch)
{
    PDC_LOG(("wbkgrnd() - called\n"));

    return wch ? wbkgd(win, *wch) : ERR;
}

int bkgrnd(const cchar_t *wch)
{
    PDC_LOG(("bkgrnd() - called\n"));

    return wbkgrnd(stdscr, wch);
}

void wbkgrndset(WINDOW *win, const cchar_t *wch)
{
    PDC_LOG(("wbkgdset() - called\n"));

    if (wch)
        wbkgdset(win, *wch);
}

void bkgrndset(const cchar_t *wch)
{
    PDC_LOG(("bkgrndset() - called\n"));

    wbkgrndset(stdscr, wch);
}

int wgetbkgrnd(WINDOW *win, cchar_t *wch)
{
    PDC_LOG(("wgetbkgrnd() - called\n"));

    if (!win || !wch)
        return ERR;

    *wch = win->_bkgd;

    return OK;
}

int getbkgrnd(cchar_t *wch)
{
    PDC_LOG(("getbkgrnd() - called\n"));

    return wgetbkgrnd(stdscr, wch);
}
#endif
/* PDCurses */

/*man-start**************************************************************

border
------

### Synopsis

    int border(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl,
               chtype tr, chtype bl, chtype br);
    int wborder(WINDOW *win, chtype ls, chtype rs, chtype ts,
                chtype bs, chtype tl, chtype tr, chtype bl, chtype br);
    int box(WINDOW *win, chtype verch, chtype horch);
    int hline(chtype ch, int n);
    int vline(chtype ch, int n);
    int whline(WINDOW *win, chtype ch, int n);
    int wvline(WINDOW *win, chtype ch, int n);
    int mvhline(int y, int x, chtype ch, int n);
    int mvvline(int y, int x, chtype ch, int n);
    int mvwhline(WINDOW *win, int y, int x, chtype ch, int n);
    int mvwvline(WINDOW *win, int y, int x, chtype ch, int n);

    int border_set(const cchar_t *ls, const cchar_t *rs,
                   const cchar_t *ts, const cchar_t *bs,
                   const cchar_t *tl, const cchar_t *tr,
                const cchar_t *bl, const cchar_t *br);
    int wborder_set(WINDOW *win, const cchar_t *ls, const cchar_t *rs,
                    const cchar_t *ts, const cchar_t *bs,
                    const cchar_t *tl, const cchar_t *tr,
                    const cchar_t *bl, const cchar_t *br);
    int box_set(WINDOW *win, const cchar_t *verch, const cchar_t *horch);
    int hline_set(const cchar_t *wch, int n);
    int vline_set(const cchar_t *wch, int n);
    int whline_set(WINDOW *win, const cchar_t *wch, int n);
    int wvline_set(WINDOW *win, const cchar_t *wch, int n);
    int mvhline_set(int y, int x, const cchar_t *wch, int n);
    int mvvline_set(int y, int x, const cchar_t *wch, int n);
    int mvwhline_set(WINDOW *win, int y, int x, const cchar_t *wch, int n);
    int mvwvline_set(WINDOW *win, int y, int x, const cchar_t *wch, int n);

### Description

   border(), wborder(), and box() draw a border around the edge of the
   window. If any argument is zero, an appropriate default is used:

   Name | Element                       | Default
   :----|:------------------------------|:------------
   ls   | left side of border           | ACS_VLINE
   rs   | right side of border          | ACS_VLINE
   ts   | top side of border            | ACS_HLINE
   bs   | bottom side of border         | ACS_HLINE
   tl   | top left corner of border     | ACS_ULCORNER
   tr   | top right corner of border    | ACS_URCORNER
   bl   | bottom left corner of border  | ACS_LLCORNER
   br   | bottom right corner of border | ACS_LRCORNER

   hline() and whline() draw a horizontal line, using ch, starting from
   the current cursor position. The cursor position does not change. The
   line is at most n characters long, or as many as will fit in the
   window.

   vline() and wvline() draw a vertical line, using ch, starting from
   the current cursor position. The cursor position does not change. The
   line is at most n characters long, or as many as will fit in the
   window.

   The *_set functions are the "wide-character" versions, taking
   pointers to cchar_t instead of chtype. Note that in PDCurses, chtype
   and cchar_t are the same.

### Return Value

   These functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   border                |    Y   |    Y    |   Y
   wborder               |    Y   |    Y    |   Y
   box                   |    Y   |    Y    |   Y
   hline                 |    Y   |    Y    |   Y
   vline                 |    Y   |    Y    |   Y
   whline                |    Y   |    Y    |   Y
   wvline                |    Y   |    Y    |   Y
   mvhline               |    Y   |    Y    |   Y
   mvvline               |    Y   |    Y    |   Y
   mvwhline              |    Y   |    Y    |   Y
   mvwvline              |    Y   |    Y    |   Y
   border_set            |    Y   |    Y    |   Y
   wborder_set           |    Y   |    Y    |   Y
   box_set               |    Y   |    Y    |   Y
   hline_set             |    Y   |    Y    |   Y
   vline_set             |    Y   |    Y    |   Y
   whline_set            |    Y   |    Y    |   Y
   wvline_set            |    Y   |    Y    |   Y
   mvhline_set           |    Y   |    Y    |   Y
   mvvline_set           |    Y   |    Y    |   Y
   mvwhline_set          |    Y   |    Y    |   Y
   mvwvline_set          |    Y   |    Y    |   Y

**man-end****************************************************************/

/* _attr_passthru() -- Takes a single chtype 'ch' and checks if the
   current attribute of window 'win', as set by wattrset(), and/or the
   current background of win, as set by wbkgd(), should by combined with
   it. Attributes set explicitly in ch take precedence. */

static chtype _attr_passthru(WINDOW *win, chtype ch)
{
    chtype attr;

    /* If the incoming character doesn't have its own attribute, then
       use the current attributes for the window. If the incoming
       character has attributes, but not a color component, OR the
       attributes to the current attributes for the window. If the
       incoming character has a color component, use only the attributes
       from the incoming character. */

    attr = ch & A_ATTRIBUTES;
    if (!(attr & A_COLOR))
        attr |= win->_attrs;

    /* wrs (4/10/93) -- Apply the same sort of logic for the window
       background, in that it only takes precedence if other color
       attributes are not there. */

    if (!(attr & A_COLOR))
        attr |= win->_bkgd & A_ATTRIBUTES;
    else
        attr |= win->_bkgd & (A_ATTRIBUTES ^ A_COLOR);

    ch = (ch & A_CHARTEXT) | attr;

    return ch;
}

int wborder(WINDOW *win, chtype ls, chtype rs, chtype ts, chtype bs,
            chtype tl, chtype tr, chtype bl, chtype br)
{
    int i, ymax, xmax;

    PDC_LOG(("wborder() - called\n"));

    if (!win)
        return ERR;

    ymax = win->_maxy - 1;
    xmax = win->_maxx - 1;

    ls = _attr_passthru(win, ls ? ls : ACS_VLINE);
    rs = _attr_passthru(win, rs ? rs : ACS_VLINE);
    ts = _attr_passthru(win, ts ? ts : ACS_HLINE);
    bs = _attr_passthru(win, bs ? bs : ACS_HLINE);
    tl = _attr_passthru(win, tl ? tl : ACS_ULCORNER);
    tr = _attr_passthru(win, tr ? tr : ACS_URCORNER);
    bl = _attr_passthru(win, bl ? bl : ACS_LLCORNER);
    br = _attr_passthru(win, br ? br : ACS_LRCORNER);

    for (i = 1; i < xmax; i++)
    {
        win->_y[0][i] = ts;
        win->_y[ymax][i] = bs;
    }

    for (i = 1; i < ymax; i++)
    {
        win->_y[i][0] = ls;
        win->_y[i][xmax] = rs;
    }

    win->_y[0][0] = tl;
    win->_y[0][xmax] = tr;
    win->_y[ymax][0] = bl;
    win->_y[ymax][xmax] = br;

    for (i = 0; i <= ymax; i++)
    {
        win->_firstch[i] = 0;
        win->_lastch[i] = xmax;
    }

    PDC_sync(win);

    return OK;
}

int border(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl,
           chtype tr, chtype bl, chtype br)
{
    PDC_LOG(("border() - called\n"));

    return wborder(stdscr, ls, rs, ts, bs, tl, tr, bl, br);
}

int box(WINDOW *win, chtype verch, chtype horch)
{
    PDC_LOG(("box() - called\n"));

    return wborder(win, verch, verch, horch, horch, 0, 0, 0, 0);
}

int whline(WINDOW *win, chtype ch, int n)
{
    chtype *dest;
    int startpos, endpos;

    PDC_LOG(("whline() - called\n"));

    if (!win || n < 1)
        return ERR;

    startpos = win->_curx;
    endpos = min(startpos + n, win->_maxx) - 1;
    dest = win->_y[win->_cury];
    ch = _attr_passthru(win, ch ? ch : ACS_HLINE);

    for (n = startpos; n <= endpos; n++)
        dest[n] = ch;

    n = win->_cury;

    if (startpos < win->_firstch[n] || win->_firstch[n] == _NO_CHANGE)
        win->_firstch[n] = startpos;

    if (endpos > win->_lastch[n])
        win->_lastch[n] = endpos;

    PDC_sync(win);

    return OK;
}

int hline(chtype ch, int n)
{
    PDC_LOG(("hline() - called\n"));

    return whline(stdscr, ch, n);
}

int mvhline(int y, int x, chtype ch, int n)
{
    PDC_LOG(("mvhline() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return whline(stdscr, ch, n);
}

int mvwhline(WINDOW *win, int y, int x, chtype ch, int n)
{
    PDC_LOG(("mvwhline() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return whline(win, ch, n);
}

int wvline(WINDOW *win, chtype ch, int n)
{
    int endpos, x;

    PDC_LOG(("wvline() - called\n"));

    if (!win || n < 1)
        return ERR;

    endpos = min(win->_cury + n, win->_maxy);
    x = win->_curx;

    ch = _attr_passthru(win, ch ? ch : ACS_VLINE);

    for (n = win->_cury; n < endpos; n++)
    {
        win->_y[n][x] = ch;

        if (x < win->_firstch[n] || win->_firstch[n] == _NO_CHANGE)
            win->_firstch[n] = x;

        if (x > win->_lastch[n])
            win->_lastch[n] = x;
    }

    PDC_sync(win);

    return OK;
}

int vline(chtype ch, int n)
{
    PDC_LOG(("vline() - called\n"));

    return wvline(stdscr, ch, n);
}

int mvvline(int y, int x, chtype ch, int n)
{
    PDC_LOG(("mvvline() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wvline(stdscr, ch, n);
}

int mvwvline(WINDOW *win, int y, int x, chtype ch, int n)
{
    PDC_LOG(("mvwvline() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wvline(win, ch, n);
}

#ifdef PDC_WIDE
int wborder_set(WINDOW *win, const cchar_t *ls, const cchar_t *rs,
                const cchar_t *ts, const cchar_t *bs, const cchar_t *tl,
                const cchar_t *tr, const cchar_t *bl, const cchar_t *br)
{
    PDC_LOG(("wborder_set() - called\n"));

    return wborder(win, ls ? *ls : 0, rs ? *rs : 0, ts ? *ts : 0,
                        bs ? *bs : 0, tl ? *tl : 0, tr ? *tr : 0,
                        bl ? *bl : 0, br ? *br : 0);
}

int border_set(const cchar_t *ls, const cchar_t *rs, const cchar_t *ts,
               const cchar_t *bs, const cchar_t *tl, const cchar_t *tr,
               const cchar_t *bl, const cchar_t *br)
{
    PDC_LOG(("border_set() - called\n"));

    return wborder_set(stdscr, ls, rs, ts, bs, tl, tr, bl, br);
}

int box_set(WINDOW *win, const cchar_t *verch, const cchar_t *horch)
{
    PDC_LOG(("box_set() - called\n"));

    return wborder_set(win, verch, verch, horch, horch,
                       (const cchar_t *)NULL, (const cchar_t *)NULL,
                       (const cchar_t *)NULL, (const cchar_t *)NULL);
}

int whline_set(WINDOW *win, const cchar_t *wch, int n)
{
    PDC_LOG(("whline_set() - called\n"));

    return wch ? whline(win, *wch, n) : ERR;
}

int hline_set(const cchar_t *wch, int n)
{
    PDC_LOG(("hline_set() - called\n"));

    return whline_set(stdscr, wch, n);
}

int mvhline_set(int y, int x, const cchar_t *wch, int n)
{
    PDC_LOG(("mvhline_set() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return whline_set(stdscr, wch, n);
}

int mvwhline_set(WINDOW *win, int y, int x, const cchar_t *wch, int n)
{
    PDC_LOG(("mvwhline_set() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return whline_set(win, wch, n);
}

int wvline_set(WINDOW *win, const cchar_t *wch, int n)
{
    PDC_LOG(("wvline_set() - called\n"));

    return wch ? wvline(win, *wch, n) : ERR;
}

int vline_set(const cchar_t *wch, int n)
{
    PDC_LOG(("vline_set() - called\n"));

    return wvline_set(stdscr, wch, n);
}

int mvvline_set(int y, int x, const cchar_t *wch, int n)
{
    PDC_LOG(("mvvline_set() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wvline_set(stdscr, wch, n);
}

int mvwvline_set(WINDOW *win, int y, int x, const cchar_t *wch, int n)
{
    PDC_LOG(("mvwvline_set() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wvline_set(win, wch, n);
}
#endif
/* PDCurses */


/*man-start**************************************************************

clear
-----

### Synopsis

    int clear(void);
    int wclear(WINDOW *win);
    int erase(void);
    int werase(WINDOW *win);
    int clrtobot(void);
    int wclrtobot(WINDOW *win);
    int clrtoeol(void);
    int wclrtoeol(WINDOW *win);

### Description

   erase() and werase() copy blanks (i.e. the background chtype) to
   every cell of the window.

   clear() and wclear() are similar to erase() and werase(), but they
   also call clearok() to ensure that the the window is cleared on the
   next wrefresh().

   clrtobot() and wclrtobot() clear the window from the current cursor
   position to the end of the window.

   clrtoeol() and wclrtoeol() clear the window from the current cursor
   position to the end of the current line.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   clear                 |    Y   |    Y    |   Y
   wclear                |    Y   |    Y    |   Y
   erase                 |    Y   |    Y    |   Y
   werase                |    Y   |    Y    |   Y
   clrtobot              |    Y   |    Y    |   Y
   wclrtobot             |    Y   |    Y    |   Y
   clrtoeol              |    Y   |    Y    |   Y
   wclrtoeol             |    Y   |    Y    |   Y

**man-end****************************************************************/

int wclrtoeol(WINDOW *win)
{
    int x, y, minx;
    chtype blank, *ptr;

    PDC_LOG(("wclrtoeol() - called: Row: %d Col: %d\n",
             win->_cury, win->_curx));

    if (!win)
        return ERR;

    y = win->_cury;
    x = win->_curx;

    /* wrs (4/10/93) account for window background */

    blank = win->_bkgd;

    for (minx = x, ptr = &win->_y[y][x]; minx < win->_maxx; minx++, ptr++)
        *ptr = blank;

    if (x < win->_firstch[y] || win->_firstch[y] == _NO_CHANGE)
        win->_firstch[y] = x;

    win->_lastch[y] = win->_maxx - 1;

    PDC_sync(win);
    return OK;
}

int clrtoeol(void)
{
    PDC_LOG(("clrtoeol() - called\n"));

    return wclrtoeol(stdscr);
}

int wclrtobot(WINDOW *win)
{
    int savey, savex;

    PDC_LOG(("wclrtobot() - called\n"));

    if (!win)
        return ERR;

    savey = win->_cury;
    savex = win->_curx;

    /* should this involve scrolling region somehow ? */

    if (win->_cury + 1 < win->_maxy)
    {
        win->_curx = 0;
        win->_cury++;
        for (; win->_maxy > win->_cury; win->_cury++)
            wclrtoeol(win);
        win->_cury = savey;
        win->_curx = savex;
    }
    wclrtoeol(win);

    PDC_sync(win);
    return OK;
}

int clrtobot(void)
{
    PDC_LOG(("clrtobot() - called\n"));

    return wclrtobot(stdscr);
}

int werase(WINDOW *win)
{
    PDC_LOG(("werase() - called\n"));

    if (wmove(win, 0, 0) == ERR)
        return ERR;

    return wclrtobot(win);
}

int erase(void)
{
    PDC_LOG(("erase() - called\n"));

    return werase(stdscr);
}

int wclear(WINDOW *win)
{
    PDC_LOG(("wclear() - called\n"));

    if (!win)
        return ERR;

    win->_clear = TRUE;
    return werase(win);
}

int clear(void)
{
    PDC_LOG(("clear() - called\n"));

    return wclear(stdscr);
}
/* PDCurses */

/*man-start**************************************************************

color
-----

### Synopsis

    bool has_colors(void);
    int start_color(void);
    int init_pair(short pair, short fg, short bg);
    int pair_content(short pair, short *fg, short *bg);
    bool can_change_color(void);
    int init_color(short color, short red, short green, short blue);
    int color_content(short color, short *red, short *green, short *blue);

    int alloc_pair(int fg, int bg);
    int assume_default_colors(int f, int b);
    int find_pair(int fg, int bg);
    int free_pair(int pair);
    int use_default_colors(void);

    int PDC_set_line_color(short color);

### Description

   To use these routines, first, call start_color(). Colors are always
   used in pairs, referred to as color-pairs. A color-pair is created by
   init_pair(), and consists of a foreground color and a background
   color. After initialization, COLOR_PAIR(n) can be used like any other
   video attribute.

   has_colors() reports whether the terminal supports color.

   start_color() initializes eight basic colors (black, red, green,
   yellow, blue, magenta, cyan, and white), and two global variables:
   COLORS and COLOR_PAIRS (respectively defining the maximum number of
   colors and color-pairs the terminal is capable of displaying).

   init_pair() changes the definition of a color-pair. It takes three
   arguments: the number of the color-pair to be redefined, and the new
   values of the foreground and background colors. The pair number must
   be between 0 and COLOR_PAIRS - 1, inclusive. The foreground and
   background must be between 0 and COLORS - 1, inclusive. If the color
   pair was previously initialized, the screen is refreshed, and all
   occurrences of that color-pair are changed to the new definition.

   pair_content() is used to determine what the colors of a given color-
   pair consist of.

   can_change_color() indicates if the terminal has the capability to
   change the definition of its colors.

   init_color() is used to redefine a color, if possible. Each of the
   components -- red, green, and blue -- is specified in a range from 0
   to 1000, inclusive.

   color_content() reports the current definition of a color in the same
   format as used by init_color().

   assume_default_colors() and use_default_colors() emulate the ncurses
   extensions of the same names. assume_default_colors(f, b) is
   essentially the same as init_pair(0, f, b) (which isn't allowed); it
   redefines the default colors. use_default_colors() allows the use of
   -1 as a foreground or background color with init_pair(), and calls
   assume_default_colors(-1, -1); -1 represents the foreground or
   background color that the terminal had at startup. If the environment
   variable PDC_ORIGINAL_COLORS is set at the time start_color() is
   called, that's equivalent to calling use_default_colors().

   alloc_pair(), find_pair() and free_pair() are also from ncurses.
   free_pair() marks a pair as unused; find_pair() returns an existing
   pair with the specified foreground and background colors, if one
   exists. And alloc_pair() returns such a pair whether or not it was
   previously set, overwriting the oldest initialized pair if there are
   no free pairs.

   PDC_set_line_color() is used to set the color, globally, for the
   color of the lines drawn for the attributes: A_UNDERLINE, A_LEFT and
   A_RIGHT. A value of -1 (the default) indicates that the current
   foreground color should be used.

   NOTE: COLOR_PAIR() and PAIR_NUMBER() are implemented as macros.

### Return Value

   Most functions return OK on success and ERR on error. has_colors()
   and can_change_colors() return TRUE or FALSE. alloc_pair() and
   find_pair() return a pair number, or -1 on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   has_colors            |    Y   |    Y    |   Y
   start_color           |    Y   |    Y    |   Y
   init_pair             |    Y   |    Y    |   Y
   pair_content          |    Y   |    Y    |   Y
   can_change_color      |    Y   |    Y    |   Y
   init_color            |    Y   |    Y    |   Y
   color_content         |    Y   |    Y    |   Y
   alloc_pair            |    -   |    Y    |   -
   assume_default_colors |    -   |    Y    |   Y
   find_pair             |    -   |    Y    |   -
   free_pair             |    -   |    Y    |   -
   use_default_colors    |    -   |    Y    |   Y
   PDC_set_line_color    |    -   |    -    |   -

**man-end****************************************************************/

int COLORS = 0;
int COLOR_PAIRS = PDC_COLOR_PAIRS;

static bool default_colors = FALSE;
static short first_col = 0;
static int allocnum = 0;

int start_color(void)
{
    PDC_LOG(("start_color() - called\n"));

    if (!SP || SP->mono)
        return ERR;

    SP->color_started = TRUE;

    PDC_set_blink(FALSE);   /* Also sets COLORS */

    if (!default_colors && SP->orig_attr && getenv("PDC_ORIGINAL_COLORS"))
        default_colors = TRUE;

    PDC_init_atrtab();

    return OK;
}

static void _normalize(short *fg, short *bg)
{
    if (*fg == -1)
        *fg = SP->orig_attr ? SP->orig_fore : COLOR_WHITE;

    if (*bg == -1)
        *bg = SP->orig_attr ? SP->orig_back : COLOR_BLACK;
}

static void _init_pair_core(short pair, short fg, short bg)
{
    PDC_PAIR *p = SP->atrtab + pair;

    _normalize(&fg, &bg);

    /* To allow the PDC_PRESERVE_SCREEN option to work, we only reset
       curscr if this call to init_pair() alters a color pair created by
       the user. */

    if (p->set)
    {
        if (p->f != fg || p->b != bg)
            curscr->_clear = TRUE;
    }

    p->f = fg;
    p->b = bg;
    p->count = allocnum++;
    p->set = TRUE;
}

int init_pair(short pair, short fg, short bg)
{
    PDC_LOG(("init_pair() - called: pair %d fg %d bg %d\n", pair, fg, bg));

    if (!SP || !SP->color_started || pair < 1 || pair >= COLOR_PAIRS ||
        fg < first_col || fg >= COLORS || bg < first_col || bg >= COLORS)
        return ERR;

    _init_pair_core(pair, fg, bg);

    return OK;
}

bool has_colors(void)
{
    PDC_LOG(("has_colors() - called\n"));

    return SP ? !(SP->mono) : FALSE;
}

int init_color(short color, short red, short green, short blue)
{
    PDC_LOG(("init_color() - called\n"));

    if (!SP || color < 0 || color >= COLORS || !PDC_can_change_color() ||
        red < -1 || red > 1000 || green < -1 || green > 1000 ||
        blue < -1 || blue > 1000)
        return ERR;

    SP->dirty = TRUE;

    return PDC_init_color(color, red, green, blue);
}

int color_content(short color, short *red, short *green, short *blue)
{
    PDC_LOG(("color_content() - called\n"));

    if (color < 0 || color >= COLORS || !red || !green || !blue)
        return ERR;

    if (PDC_can_change_color())
        return PDC_color_content(color, red, green, blue);
    else
    {
        /* Simulated values for platforms that don't support palette
           changing */

        short maxval = (color & 8) ? 1000 : 680;

        *red = (color & COLOR_RED) ? maxval : 0;
        *green = (color & COLOR_GREEN) ? maxval : 0;
        *blue = (color & COLOR_BLUE) ? maxval : 0;

        return OK;
    }
}

bool can_change_color(void)
{
    PDC_LOG(("can_change_color() - called\n"));

    return PDC_can_change_color();
}

int pair_content(short pair, short *fg, short *bg)
{
    PDC_LOG(("pair_content() - called\n"));

    if (pair < 0 || pair >= COLOR_PAIRS || !fg || !bg)
        return ERR;

    *fg = SP->atrtab[pair].f;
    *bg = SP->atrtab[pair].b;

    return OK;
}

int assume_default_colors(int f, int b)
{
    PDC_LOG(("assume_default_colors() - called: f %d b %d\n", f, b));

    if (f < -1 || f >= COLORS || b < -1 || b >= COLORS)
        return ERR;

    if (SP->color_started)
        _init_pair_core(0, f, b);

    return OK;
}

int use_default_colors(void)
{
    PDC_LOG(("use_default_colors() - called\n"));

    default_colors = TRUE;
    first_col = -1;

    return assume_default_colors(-1, -1);
}

int PDC_set_line_color(short color)
{
    PDC_LOG(("PDC_set_line_color() - called: %d\n", color));

    if (!SP || color < -1 || color >= COLORS)
        return ERR;

    SP->line_color = color;

    return OK;
}

void PDC_init_atrtab(void)
{
    PDC_PAIR *p = SP->atrtab;
    short i, fg, bg;

    if (SP->color_started && !default_colors)
    {
        fg = COLOR_WHITE;
        bg = COLOR_BLACK;
    }
    else
        fg = bg = -1;

    _normalize(&fg, &bg);

    for (i = 0; i < PDC_COLOR_PAIRS; i++)
    {
        p[i].f = fg;
        p[i].b = bg;
        p[i].set = FALSE;
    }
}

int free_pair(int pair)
{
    if (pair < 1 || pair >= PDC_COLOR_PAIRS || !(SP->atrtab[pair].set))
        return ERR;

    SP->atrtab[pair].set = FALSE;
    return OK;
}

int find_pair(int fg, int bg)
{
    int i;
    PDC_PAIR *p = SP->atrtab;

    for (i = 0; i < PDC_COLOR_PAIRS; i++)
        if (p[i].set && p[i].f == fg && p[i].b == bg)
            return i;

    return -1;
}

static int _find_oldest()
{
    int i, lowind = 0, lowval = 0;
    PDC_PAIR *p = SP->atrtab;

    for (i = 1; i < PDC_COLOR_PAIRS; i++)
    {
        if (!p[i].set)
            return i;

        if (!lowval || (p[i].count < lowval))
        {
            lowind = i;
            lowval = p[i].count;
        }
    }

    return lowind;
}

int alloc_pair(int fg, int bg)
{
    int i = find_pair(fg, bg);

    if (-1 == i)
    {
        i = _find_oldest();

        if (ERR == init_pair(i, fg, bg))
            return -1;
    }

    return i;
}
/* PDCurses */

/*man-start**************************************************************

debug
-----

### Synopsis

    void traceon(void);
    void traceoff(void);
    void PDC_debug(const char *, ...);

### Description

   traceon() and traceoff() toggle the recording of debugging
   information to the file "trace". Although not standard, similar
   functions are in some other curses implementations.

   PDC_debug() is the function that writes to the file, based on whether
   traceon() has been called. It's used from the PDC_LOG() macro.

   The environment variable PDC_TRACE_FLUSH controls whether the trace
   file contents are fflushed after each write. The default is not. Set
   it to enable this (may affect performance).

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   traceon               |    -   |    -    |   -
   traceoff              |    -   |    -    |   -
   PDC_debug             |    -   |    -    |   -

**man-end****************************************************************/

static bool want_fflush = FALSE;

void PDC_debug(const char *fmt, ...)
{
    va_list args;
    char hms[9];
    time_t now;

    if (!SP || !SP->dbfp)
        return;

    time(&now);
    strftime(hms, 9, "%H:%M:%S", localtime(&now));
    fprintf(SP->dbfp, "At: %8.8ld - %s ", (long) clock(), hms);

    va_start(args, fmt);
    vfprintf(SP->dbfp, fmt, args);
    va_end(args);

    /* If you are crashing and losing debugging information, enable this
       by setting the environment variable PDC_TRACE_FLUSH. This may
       impact performance. */

    if (want_fflush)
        fflush(SP->dbfp);

    /* If with PDC_TRACE_FLUSH enabled you are still losing logging in
       crashes, you may need to add a platform-dependent mechanism to
       flush the OS buffers as well (such as fsync() on POSIX) -- but
       expect terrible performance. */
}

void traceon(void)
{
    if (!SP)
        return;

    if (SP->dbfp)
        fclose(SP->dbfp);

    /* open debug log file append */
    SP->dbfp = fopen("trace", "a");
    if (!SP->dbfp)
    {
        fprintf(stderr, "PDC_debug(): Unable to open debug log file\n");
        return;
    }

    if (getenv("PDC_TRACE_FLUSH"))
        want_fflush = TRUE;

    PDC_LOG(("traceon() - called\n"));
}

void traceoff(void)
{
    if (!SP || !SP->dbfp)
        return;

    PDC_LOG(("traceoff() - called\n"));

    fclose(SP->dbfp);
    SP->dbfp = NULL;
    want_fflush = FALSE;
}
/* PDCurses */

/*man-start**************************************************************

delch
-----

### Synopsis

    int delch(void);
    int wdelch(WINDOW *win);
    int mvdelch(int y, int x);
    int mvwdelch(WINDOW *win, int y, int x);

### Description

   The character under the cursor in the window is deleted. All
   characters to the right on the same line are moved to the left one
   position and the last character on the line is filled with a blank.
   The cursor position does not change (after moving to y, x if
   coordinates are specified).

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   delch                 |    Y   |    Y    |   Y
   wdelch                |    Y   |    Y    |   Y
   mvdelch               |    Y   |    Y    |   Y
   mvwdelch              |    Y   |    Y    |   Y

**man-end****************************************************************/

int wdelch(WINDOW *win)
{
    int y, x, maxx;
    chtype *temp1;

    PDC_LOG(("wdelch() - called\n"));

    if (!win)
        return ERR;

    y = win->_cury;
    x = win->_curx;
    maxx = win->_maxx - 1;
    temp1 = &win->_y[y][x];

    memmove(temp1, temp1 + 1, (maxx - x) * sizeof(chtype));

    /* wrs (4/10/93) account for window background */

    win->_y[y][maxx] = win->_bkgd;

    win->_lastch[y] = maxx;

    if ((win->_firstch[y] == _NO_CHANGE) || (win->_firstch[y] > x))
        win->_firstch[y] = x;

    PDC_sync(win);

    return OK;
}

int delch(void)
{
    PDC_LOG(("delch() - called\n"));

    return wdelch(stdscr);
}

int mvdelch(int y, int x)
{
    PDC_LOG(("mvdelch() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wdelch(stdscr);
}

int mvwdelch(WINDOW *win, int y, int x)
{
    PDC_LOG(("mvwdelch() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wdelch(win);
}
/* PDCurses */

/*man-start**************************************************************

deleteln
--------

### Synopsis

    int deleteln(void);
    int wdeleteln(WINDOW *win);
    int insdelln(int n);
    int winsdelln(WINDOW *win, int n);
    int insertln(void);
    int winsertln(WINDOW *win);

    int mvdeleteln(int y, int x);
    int mvwdeleteln(WINDOW *win, int y, int x);
    int mvinsertln(int y, int x);
    int mvwinsertln(WINDOW *win, int y, int x);

### Description

   With the deleteln() and wdeleteln() functions, the line under the
   cursor in the window is deleted. All lines below the current line are
   moved up one line. The bottom line of the window is cleared. The
   cursor position does not change.

   With the insertln() and winsertn() functions, a blank line is
   inserted above the current line and the bottom line is lost.

   mvdeleteln(), mvwdeleteln(), mvinsertln() and mvwinsertln() allow
   moving the cursor and inserting/deleting in one call.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   deleteln              |    Y   |    Y    |   Y
   wdeleteln             |    Y   |    Y    |   Y
   mvdeleteln            |    -   |    -    |   -
   mvwdeleteln           |    -   |    -    |   -
   insdelln              |    Y   |    Y    |   Y
   winsdelln             |    Y   |    Y    |   Y
   insertln              |    Y   |    Y    |   Y
   winsertln             |    Y   |    Y    |   Y
   mvinsertln            |    -   |    -    |   -
   mvwinsertln           |    -   |    -    |   -

**man-end****************************************************************/

int wdeleteln(WINDOW *win)
{
    chtype blank, *temp, *ptr;
    int y;

    PDC_LOG(("wdeleteln() - called\n"));

    if (!win)
        return ERR;

    /* wrs (4/10/93) account for window background */

    blank = win->_bkgd;

    temp = win->_y[win->_cury];

    for (y = win->_cury; y < win->_bmarg; y++)
    {
        win->_y[y] = win->_y[y + 1];
        win->_firstch[y] = 0;
        win->_lastch[y] = win->_maxx - 1;
    }

    for (ptr = temp; (ptr - temp < win->_maxx); ptr++)
        *ptr = blank;           /* make a blank line */

    if (win->_cury <= win->_bmarg)
    {
        win->_firstch[win->_bmarg] = 0;
        win->_lastch[win->_bmarg] = win->_maxx - 1;
        win->_y[win->_bmarg] = temp;
    }

    return OK;
}

int deleteln(void)
{
    PDC_LOG(("deleteln() - called\n"));

    return wdeleteln(stdscr);
}

int mvdeleteln(int y, int x)
{
    PDC_LOG(("mvdeleteln() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wdeleteln(stdscr);
}

int mvwdeleteln(WINDOW *win, int y, int x)
{
    PDC_LOG(("mvwdeleteln() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wdeleteln(win);
}

int winsdelln(WINDOW *win, int n)
{
    int i;

    PDC_LOG(("winsdelln() - called\n"));

    if (!win)
        return ERR;

    if (n > 0)
    {
        for (i = 0; i < n; i++)
            if (winsertln(win) == ERR)
                return ERR;
    }
    else if (n < 0)
    {
        n = -n;
        for (i = 0; i < n; i++)
            if (wdeleteln(win) == ERR)
                return ERR;
    }

    return OK;
}

int insdelln(int n)
{
    PDC_LOG(("insdelln() - called\n"));

    return winsdelln(stdscr, n);
}

int winsertln(WINDOW *win)
{
    chtype blank, *temp, *end;
    int y;

    PDC_LOG(("winsertln() - called\n"));

    if (!win)
        return ERR;

    /* wrs (4/10/93) account for window background */

    blank = win->_bkgd;

    temp = win->_y[win->_maxy - 1];

    for (y = win->_maxy - 1; y > win->_cury; y--)
    {
        win->_y[y] = win->_y[y - 1];
        win->_firstch[y] = 0;
        win->_lastch[y] = win->_maxx - 1;
    }

    win->_y[win->_cury] = temp;

    for (end = &temp[win->_maxx - 1]; temp <= end; temp++)
        *temp = blank;

    win->_firstch[win->_cury] = 0;
    win->_lastch[win->_cury] = win->_maxx - 1;

    return OK;
}

int insertln(void)
{
    PDC_LOG(("insertln() - called\n"));

    return winsertln(stdscr);
}

int mvinsertln(int y, int x)
{
    PDC_LOG(("mvinsertln() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return winsertln(stdscr);
}

int mvwinsertln(WINDOW *win, int y, int x)
{
    PDC_LOG(("mvwinsertln() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winsertln(win);
}
/* PDCurses */

/*man-start**************************************************************

getch
-----

### Synopsis

    int getch(void);
    int wgetch(WINDOW *win);
    int mvgetch(int y, int x);
    int mvwgetch(WINDOW *win, int y, int x);
    int ungetch(int ch);
    int flushinp(void);

    int get_wch(wint_t *wch);
    int wget_wch(WINDOW *win, wint_t *wch);
    int mvget_wch(int y, int x, wint_t *wch);
    int mvwget_wch(WINDOW *win, int y, int x, wint_t *wch);
    int unget_wch(const wchar_t wch);

    unsigned long PDC_get_key_modifiers(void);
    int PDC_return_key_modifiers(bool flag);

### Description

   With the getch(), wgetch(), mvgetch(), and mvwgetch() functions, a
   character is read from the terminal associated with the window. In
   nodelay mode, if there is no input waiting, the value ERR is
   returned. In delay mode, the program will hang until the system
   passes text through to the program. Depending on the setting of
   cbreak(), this will be after one character or after the first
   newline. Unless noecho() has been set, the character will also be
   echoed into the designated window.

   If keypad() is TRUE, and a function key is pressed, the token for
   that function key will be returned instead of the raw characters.
   Possible function keys are defined in <curses.h> with integers
   beginning with 0401, whose names begin with KEY_.

   If nodelay(win, TRUE) has been called on the window and no input is
   waiting, the value ERR is returned.

   ungetch() places ch back onto the input queue to be returned by the
   next call to wgetch().

   flushinp() throws away any type-ahead that has been typed by the user
   and has not yet been read by the program.

   wget_wch() is the wide-character version of wgetch(), available when
   PDCurses is built with the PDC_WIDE option. It takes a pointer to a
   wint_t rather than returning the key as an int, and instead returns
   KEY_CODE_YES if the key is a function key. Otherwise, it returns OK
   or ERR. It's important to check for KEY_CODE_YES, since regular wide
   characters can have the same values as function key codes.

   unget_wch() puts a wide character on the input queue.

   PDC_get_key_modifiers() returns the keyboard modifiers (shift,
   control, alt, numlock) effective at the time of the last getch()
   call. Use the macros PDC_KEY_MODIFIER_* to determine which
   modifier(s) were set. PDC_return_key_modifiers() tells getch() to
   return modifier keys pressed alone as keystrokes (KEY_ALT_L, etc.).
   These may not work on all platforms.

   NOTE: getch() and ungetch() are implemented as macros, to avoid
   conflict with many DOS compiler's runtime libraries.

### Return Value

   These functions return ERR or the value of the character, meta
   character or function key token.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   getch                 |    Y   |    Y    |   Y
   wgetch                |    Y   |    Y    |   Y
   mvgetch               |    Y   |    Y    |   Y
   mvwgetch              |    Y   |    Y    |   Y
   ungetch               |    Y   |    Y    |   Y
   flushinp              |    Y   |    Y    |   Y
   get_wch               |    Y   |    Y    |   Y
   wget_wch              |    Y   |    Y    |   Y
   mvget_wch             |    Y   |    Y    |   Y
   mvwget_wch            |    Y   |    Y    |   Y
   unget_wch             |    Y   |    Y    |   Y
   PDC_get_key_modifiers |    -   |    -    |   -

**man-end****************************************************************/

static int _get_box(int *y_start, int *y_end, int *x_start, int *x_end)
{
    int start, end;

    if (SP->sel_start < SP->sel_end)
    {
        start = SP->sel_start;
        end = SP->sel_end;
    }
    else
    {
        start = SP->sel_end;
        end = SP->sel_start;
    }

    *y_start = start / COLS;
    *x_start = start % COLS;

    *y_end = end / COLS;
    *x_end = end % COLS;

    return (end - start) + (*y_end - *y_start);
}

static void _highlight(void)
{
    int i, j, y_start, y_end, x_start, x_end;

    if (-1 == SP->sel_start)
        return;

    _get_box(&y_start, &y_end, &x_start, &x_end);

    for (j = y_start; j <= y_end; j++)
        for (i = (j == y_start ? x_start : 0);
             i < (j == y_end ? x_end : COLS); i++)
            curscr->_y[j][i] ^= A_REVERSE;

    wrefresh(curscr);
}

static void _copy(void)
{
#ifdef PDC_WIDE
    wchar_t *wtmp;
# define TMP wtmp
# define MASK A_CHARTEXT
#else
# define TMP tmp
# define MASK 0xff
#endif
    char *tmp;
    long pos;
    int i, j, y_start, y_end, x_start, x_end, len;

    if (-1 == SP->sel_start)
        return;

    len = _get_box(&y_start, &y_end, &x_start, &x_end);

    if (!len)
        return;

#ifdef PDC_WIDE
    wtmp = malloc((len + 1) * sizeof(wchar_t));
    len *= 4;
#endif
    tmp = malloc(len + 1);

    for (j = y_start, pos = 0; j <= y_end; j++)
    {
        for (i = (j == y_start ? x_start : 0);
             i < (j == y_end ? x_end : COLS); i++)
            TMP[pos++] = curscr->_y[j][i] & MASK;

        while (y_start != y_end && pos > 0 && TMP[pos - 1] == 32)
            pos--;

        if (j < y_end)
            TMP[pos++] = 10;
    }
    TMP[pos] = 0;

#ifdef PDC_WIDE
    pos = PDC_wcstombs(tmp, wtmp, len);
#endif

    PDC_setclipboard(tmp, pos);
    free(tmp);
#ifdef PDC_WIDE
    free(wtmp);
#endif
}

static int _paste(void)
{
#ifdef PDC_WIDE
    wchar_t *wpaste;
# define PASTE wpaste
#else
# define PASTE paste
#endif
    char *paste;
    long len, newmax;
    int key;

    key = PDC_getclipboard(&paste, &len);
    if (PDC_CLIP_SUCCESS != key || !len)
        return -1;

#ifdef PDC_WIDE
    wpaste = malloc(len * sizeof(wchar_t));
    len = PDC_mbstowcs(wpaste, paste, len);
#endif
    newmax = len + SP->c_ungind;
    if (newmax > SP->c_ungmax)
    {
        SP->c_ungch = realloc(SP->c_ungch, newmax * sizeof(int));
        if (!SP->c_ungch)
            return -1;
        SP->c_ungmax = newmax;
    }
    while (len > 1)
        PDC_ungetch(PASTE[--len]);
    key = *PASTE;
#ifdef PDC_WIDE
    free(wpaste);
#endif
    PDC_freeclipboard(paste);
    SP->key_modifiers = 0;

    return key;
}

static int _mouse_key(void)
{
    int i, key = KEY_MOUSE, changes = SP->mouse_status.changes;
    unsigned long mbe = SP->_trap_mbe;

    /* Selection highlighting? */

    if ((!mbe || SP->mouse_status.button[0] & BUTTON_SHIFT) && changes & 1)
    {
        i = SP->mouse_status.y * COLS + SP->mouse_status.x;
        switch (SP->mouse_status.button[0] & BUTTON_ACTION_MASK)
        {
        case BUTTON_PRESSED:
            _highlight();
            SP->sel_start = SP->sel_end = i;
            return -1;
        case BUTTON_MOVED:
            _highlight();
            SP->sel_end = i;
            _highlight();
            return -1;
        case BUTTON_RELEASED:
            _copy();
            return -1;
        }
    }
    else if ((!mbe || SP->mouse_status.button[1] & BUTTON_SHIFT) &&
             changes & 2 && (SP->mouse_status.button[1] &
             BUTTON_ACTION_MASK) == BUTTON_CLICKED)
    {
        SP->key_code = FALSE;
        return _paste();
    }

    /* Filter unwanted mouse events */

    for (i = 0; i < 3; i++)
    {
        if (changes & (1 << i))
        {
            int shf = i * 5;
            short button = SP->mouse_status.button[i] & BUTTON_ACTION_MASK;

            if (   (!(mbe & (BUTTON1_PRESSED << shf)) &&
                    (button == BUTTON_PRESSED))

                || (!(mbe & (BUTTON1_CLICKED << shf)) &&
                    (button == BUTTON_CLICKED))

                || (!(mbe & (BUTTON1_DOUBLE_CLICKED << shf)) &&
                    (button == BUTTON_DOUBLE_CLICKED))

                || (!(mbe & (BUTTON1_MOVED << shf)) &&
                    (button == BUTTON_MOVED))

                || (!(mbe & (BUTTON1_RELEASED << shf)) &&
                    (button == BUTTON_RELEASED))
            )
                SP->mouse_status.changes ^= (1 << i);
        }
    }

    if (changes & PDC_MOUSE_MOVED)
    {
        if (!(mbe & (BUTTON1_MOVED|BUTTON2_MOVED|BUTTON3_MOVED)))
            SP->mouse_status.changes ^= PDC_MOUSE_MOVED;
    }

    if (changes & (PDC_MOUSE_WHEEL_UP|PDC_MOUSE_WHEEL_DOWN))
    {
        if (!(mbe & MOUSE_WHEEL_SCROLL))
            SP->mouse_status.changes &=
                ~(PDC_MOUSE_WHEEL_UP|PDC_MOUSE_WHEEL_DOWN);
    }

    if (!changes)
        return -1;

    /* Check for click in slk area */

    i = PDC_mouse_in_slk(SP->mouse_status.y, SP->mouse_status.x);

    if (i)
    {
        if (SP->mouse_status.button[0] & (BUTTON_PRESSED|BUTTON_CLICKED))
            key = KEY_F(i);
        else
            key = -1;
    }

    return key;
}

int wgetch(WINDOW *win)
{
    int key, waitcount;

    PDC_LOG(("wgetch() - called\n"));

    if (!win || !SP)
        return ERR;

    waitcount = 0;

    /* set the number of 1/20th second napms() calls */

    if (SP->delaytenths)
        waitcount = 2 * SP->delaytenths;
    else
        if (win->_delayms)
        {
            /* Can't really do millisecond intervals, so delay in
               1/20ths of a second (50ms) */

            waitcount = win->_delayms / 50;
            if (!waitcount)
                waitcount = 1;
        }

    /* refresh window when wgetch is called if there have been changes
       to it and it is not a pad */

    if (!(win->_flags & _PAD) && ((!win->_leaveit &&
         (win->_begx + win->_curx != SP->curscol ||
          win->_begy + win->_cury != SP->cursrow)) || is_wintouched(win)))
        wrefresh(win);

    /* if ungotten char exists, remove and return it */

    if (SP->c_ungind)
        return SP->c_ungch[--(SP->c_ungind)];

    /* if normal and data in buffer */

    if ((!SP->raw_inp && !SP->cbreak) && (SP->c_gindex < SP->c_pindex))
        return SP->c_buffer[SP->c_gindex++];

    /* prepare to buffer data */

    SP->c_pindex = 0;
    SP->c_gindex = 0;

    /* to get here, no keys are buffered. go and get one. */

    for (;;)            /* loop for any buffering */
    {
        /* is there a keystroke ready? */

        if (!PDC_check_key())
        {
            /* if not, handle timeout() and halfdelay() */

            if (SP->delaytenths || win->_delayms)
            {
                if (!waitcount)
                    return ERR;

                waitcount--;
            }
            else
                if (win->_nodelay)
                    return ERR;

            napms(50);  /* sleep for 1/20th second */
            continue;   /* then check again */
        }

        /* if there is, fetch it */

        key = PDC_get_key();

        /* copy or paste? */

        if (SP->key_modifiers & PDC_KEY_MODIFIER_SHIFT)
        {
            if (0x03 == key)
            {
                _copy();
                continue;
            }
            else if (0x16 == key)
                key = _paste();
        }

        /* filter mouse events; translate mouse clicks in the slk
           area to function keys */

        if (SP->key_code && key == KEY_MOUSE)
            key = _mouse_key();

        /* filter special keys if not in keypad mode */

        if (SP->key_code && !win->_use_keypad)
            key = -1;

        /* unwanted key? loop back */

        if (key == -1)
            continue;

        _highlight();
        SP->sel_start = SP->sel_end = -1;

        /* translate CR */

        if (key == '\r' && SP->autocr && !SP->raw_inp)
            key = '\n';

        /* if echo is enabled */

        if (SP->echo && !SP->key_code)
        {
            waddch(win, key);
            wrefresh(win);
        }

        /* if no buffering */

        if (SP->raw_inp || SP->cbreak)
            return key;

        /* if no overflow, put data in buffer */

        if (key == '\b')
        {
            if (SP->c_pindex > SP->c_gindex)
                SP->c_pindex--;
        }
        else
            if (SP->c_pindex < _INBUFSIZ - 2)
                SP->c_buffer[SP->c_pindex++] = key;

        /* if we got a line */

        if (key == '\n' || key == '\r')
            return SP->c_buffer[SP->c_gindex++];
    }
}

int mvgetch(int y, int x)
{
    PDC_LOG(("mvgetch() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wgetch(stdscr);
}

int mvwgetch(WINDOW *win, int y, int x)
{
    PDC_LOG(("mvwgetch() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wgetch(win);
}

int PDC_ungetch(int ch)
{
    PDC_LOG(("ungetch() - called\n"));

    if (SP->c_ungind >= SP->c_ungmax)   /* pushback stack full */
        return ERR;

    SP->c_ungch[SP->c_ungind++] = ch;

    return OK;
}

int flushinp(void)
{
    PDC_LOG(("flushinp() - called\n"));

    if (!SP)
        return ERR;

    PDC_flushinp();

    SP->c_gindex = 1;       /* set indices to kill buffer */
    SP->c_pindex = 0;
    SP->c_ungind = 0;       /* clear SP->c_ungch array */

    return OK;
}

unsigned long PDC_get_key_modifiers(void)
{
    PDC_LOG(("PDC_get_key_modifiers() - called\n"));

    if (!SP)
        return ERR;

    return SP->key_modifiers;
}

int PDC_return_key_modifiers(bool flag)
{
    PDC_LOG(("PDC_return_key_modifiers() - called\n"));

    if (!SP)
        return ERR;

    SP->return_key_modifiers = flag;
    return PDC_modifiers_set();
}

#ifdef PDC_WIDE
int wget_wch(WINDOW *win, wint_t *wch)
{
    int key;

    PDC_LOG(("wget_wch() - called\n"));

    if (!wch)
        return ERR;

    key = wgetch(win);

    if (key == ERR)
        return ERR;

    *wch = key;

    return SP->key_code ? KEY_CODE_YES : OK;
}

int get_wch(wint_t *wch)
{
    PDC_LOG(("get_wch() - called\n"));

    return wget_wch(stdscr, wch);
}

int mvget_wch(int y, int x, wint_t *wch)
{
    PDC_LOG(("mvget_wch() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wget_wch(stdscr, wch);
}

int mvwget_wch(WINDOW *win, int y, int x, wint_t *wch)
{
    PDC_LOG(("mvwget_wch() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wget_wch(win, wch);
}

int unget_wch(const wchar_t wch)
{
    return PDC_ungetch(wch);
}
#endif
/* PDCurses */

/*man-start**************************************************************

getstr
------

### Synopsis

    int getstr(char *str);
    int wgetstr(WINDOW *win, char *str);
    int mvgetstr(int y, int x, char *str);
    int mvwgetstr(WINDOW *win, int y, int x, char *str);
    int getnstr(char *str, int n);
    int wgetnstr(WINDOW *win, char *str, int n);
    int mvgetnstr(int y, int x, char *str, int n);
    int mvwgetnstr(WINDOW *win, int y, int x, char *str, int n);

    int get_wstr(wint_t *wstr);
    int wget_wstr(WINDOW *win, wint_t *wstr);
    int mvget_wstr(int y, int x, wint_t *wstr);
    int mvwget_wstr(WINDOW *win, int, int, wint_t *wstr);
    int getn_wstr(wint_t *wstr, int n);
    int wgetn_wstr(WINDOW *win, wint_t *wstr, int n);
    int mvgetn_wstr(int y, int x, wint_t *wstr, int n);
    int mvwgetn_wstr(WINDOW *win, int y, int x, wint_t *wstr, int n);

### Description

   These routines call wgetch() repeatedly to build a string,
   interpreting erase and kill characters along the way, until a newline
   or carriage return is received. When PDCurses is built with wide-
   character support enabled, the narrow-character functions convert the
   wgetch()'d values into a multibyte string in the current locale
   before returning it. The resulting string is placed in the area
   pointed to by *str. The routines with n as the last argument read at
   most n characters.

   Note that there's no way to know how long the buffer passed to
   wgetstr() is, so use wgetnstr() to avoid buffer overflows.

### Return Value

   These functions return ERR on failure or any other value on success.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   getstr                |    Y   |    Y    |   Y
   wgetstr               |    Y   |    Y    |   Y
   mvgetstr              |    Y   |    Y    |   Y
   mvwgetstr             |    Y   |    Y    |   Y
   getnstr               |    Y   |    Y    |   Y
   wgetnstr              |    Y   |    Y    |   Y
   mvgetnstr             |    Y   |    Y    |   Y
   mvwgetnstr            |    Y   |    Y    |   Y
   get_wstr              |    Y   |    Y    |   Y
   wget_wstr             |    Y   |    Y    |   Y
   mvget_wstr            |    Y   |    Y    |   Y
   mvwget_wstr           |    Y   |    Y    |   Y
   getn_wstr             |    Y   |    Y    |   Y
   wgetn_wstr            |    Y   |    Y    |   Y
   mvgetn_wstr           |    Y   |    Y    |   Y
   mvwgetn_wstr          |    Y   |    Y    |   Y

**man-end****************************************************************/

#define MAXLINE 255

int wgetnstr(WINDOW *win, char *str, int n)
{
#ifdef PDC_WIDE
    wchar_t wstr[MAXLINE + 1];
    wint_t wintstr[MAXLINE + 1];
    int i;

    if (n < 0 || n > MAXLINE)
        n = MAXLINE;

    if (wgetn_wstr(win, wintstr, n) == ERR)
        return ERR;
    for (i = 0; i < n; ++i) {
        wstr[i] = (wchar_t)wintstr[i];
    }

    return PDC_wcstombs(str, wstr, n);
#else
    int ch, i, num, x, chars;
    char *p;
    bool stop, oldecho, oldcbreak, oldnodelay;

    PDC_LOG(("wgetnstr() - called\n"));

    if (!win || !str)
        return ERR;

    chars = 0;
    p = str;
    stop = FALSE;

    x = win->_curx;

    oldcbreak = SP->cbreak; /* remember states */
    oldecho = SP->echo;
    oldnodelay = win->_nodelay;

    SP->echo = FALSE;       /* we do echo ourselves */
    cbreak();               /* ensure each key is returned immediately */
    win->_nodelay = FALSE;  /* don't return -1 */

    wrefresh(win);

    while (!stop)
    {
        ch = wgetch(win);

        switch (ch)
        {

        case '\t':
            ch = ' ';
            num = TABSIZE - (win->_curx - x) % TABSIZE;
            for (i = 0; i < num; i++)
            {
                if (chars < n)
                {
                    if (oldecho)
                        waddch(win, ch);
                    *p++ = ch;
                    ++chars;
                }
                else
                    beep();
            }
            break;

        case _ECHAR:        /* CTRL-H -- Delete character */
            if (p > str)
            {
                if (oldecho)
                    waddstr(win, "\b \b");
                ch = (unsigned char)(*--p);
                if ((ch < ' ') && (oldecho))
                    waddstr(win, "\b \b");
                chars--;
            }
            break;

        case _DLCHAR:       /* CTRL-U -- Delete line */
            while (p > str)
            {
                if (oldecho)
                    waddstr(win, "\b \b");
                ch = (unsigned char)(*--p);
                if ((ch < ' ') && (oldecho))
                    waddstr(win, "\b \b");
            }
            chars = 0;
            break;

        case _DWCHAR:       /* CTRL-W -- Delete word */

            while ((p > str) && (*(p - 1) == ' '))
            {
                if (oldecho)
                    waddstr(win, "\b \b");

                --p;        /* remove space */
                chars--;
            }
            while ((p > str) && (*(p - 1) != ' '))
            {
                if (oldecho)
                    waddstr(win, "\b \b");

                ch = (unsigned char)(*--p);
                if ((ch < ' ') && (oldecho))
                    waddstr(win, "\b \b");
                chars--;
            }
            break;

        case '\n':
        case '\r':
            stop = TRUE;
            if (oldecho)
                waddch(win, '\n');
            break;

        default:
            if (chars < n)
            {
                if (!SP->key_code && ch < 0x100)
                {
                    *p++ = ch;
                    if (oldecho)
                        waddch(win, ch);
                    chars++;
                }
            }
            else
                beep();

            break;

        }

        wrefresh(win);
    }

    *p = '\0';

    SP->echo = oldecho;     /* restore old settings */
    SP->cbreak = oldcbreak;
    win->_nodelay = oldnodelay;

    return OK;
#endif
}

int getstr(char *str)
{
    PDC_LOG(("getstr() - called\n"));

    return wgetnstr(stdscr, str, MAXLINE);
}

int wgetstr(WINDOW *win, char *str)
{
    PDC_LOG(("wgetstr() - called\n"));

    return wgetnstr(win, str, MAXLINE);
}

int mvgetstr(int y, int x, char *str)
{
    PDC_LOG(("mvgetstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wgetnstr(stdscr, str, MAXLINE);
}

int mvwgetstr(WINDOW *win, int y, int x, char *str)
{
    PDC_LOG(("mvwgetstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wgetnstr(win, str, MAXLINE);
}

int getnstr(char *str, int n)
{
    PDC_LOG(("getnstr() - called\n"));

    return wgetnstr(stdscr, str, n);
}

int mvgetnstr(int y, int x, char *str, int n)
{
    PDC_LOG(("mvgetnstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wgetnstr(stdscr, str, n);
}

int mvwgetnstr(WINDOW *win, int y, int x, char *str, int n)
{
    PDC_LOG(("mvwgetnstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wgetnstr(win, str, n);
}

#ifdef PDC_WIDE
int wgetn_wstr(WINDOW *win, wint_t *wstr, int n)
{
    int ch, i, num, x, chars;
    wint_t *p;
    bool stop, oldecho, oldcbreak, oldnodelay;

    PDC_LOG(("wgetn_wstr() - called\n"));

    if (!win || !wstr)
        return ERR;

    chars = 0;
    p = wstr;
    stop = FALSE;

    x = win->_curx;

    oldcbreak = SP->cbreak; /* remember states */
    oldecho = SP->echo;
    oldnodelay = win->_nodelay;

    SP->echo = FALSE;       /* we do echo ourselves */
    cbreak();               /* ensure each key is returned immediately */
    win->_nodelay = FALSE;  /* don't return -1 */

    wrefresh(win);

    while (!stop)
    {
        ch = wgetch(win);

        switch (ch)
        {

        case '\t':
            ch = ' ';
            num = TABSIZE - (win->_curx - x) % TABSIZE;
            for (i = 0; i < num; i++)
            {
                if (chars < n)
                {
                    if (oldecho)
                        waddch(win, ch);
                    *p++ = ch;
                    ++chars;
                }
                else
                    beep();
            }
            break;

        case _ECHAR:        /* CTRL-H -- Delete character */
            if (p > wstr)
            {
                if (oldecho)
                    waddstr(win, "\b \b");
                ch = *--p;
                if ((ch < ' ') && (oldecho))
                    waddstr(win, "\b \b");
                chars--;
            }
            break;

        case _DLCHAR:       /* CTRL-U -- Delete line */
            while (p > wstr)
            {
                if (oldecho)
                    waddstr(win, "\b \b");
                ch = *--p;
                if ((ch < ' ') && (oldecho))
                    waddstr(win, "\b \b");
            }
            chars = 0;
            break;

        case _DWCHAR:       /* CTRL-W -- Delete word */

            while ((p > wstr) && (*(p - 1) == ' '))
            {
                if (oldecho)
                    waddstr(win, "\b \b");

                --p;        /* remove space */
                chars--;
            }
            while ((p > wstr) && (*(p - 1) != ' '))
            {
                if (oldecho)
                    waddstr(win, "\b \b");

                ch = *--p;
                if ((ch < ' ') && (oldecho))
                    waddstr(win, "\b \b");
                chars--;
            }
            break;

        case '\n':
        case '\r':
            stop = TRUE;
            if (oldecho)
                waddch(win, '\n');
            break;

        default:
            if (chars < n)
            {
                if (!SP->key_code)
                {
                    *p++ = ch;
                    if (oldecho)
                        waddch(win, ch);
                    chars++;
                }
            }
            else
                beep();

            break;

        }

        wrefresh(win);
    }

    *p = '\0';

    SP->echo = oldecho;     /* restore old settings */
    SP->cbreak = oldcbreak;
    win->_nodelay = oldnodelay;

    return OK;
}

int get_wstr(wint_t *wstr)
{
    PDC_LOG(("get_wstr() - called\n"));

    return wgetn_wstr(stdscr, wstr, MAXLINE);
}

int wget_wstr(WINDOW *win, wint_t *wstr)
{
    PDC_LOG(("wget_wstr() - called\n"));

    return wgetn_wstr(win, wstr, MAXLINE);
}

int mvget_wstr(int y, int x, wint_t *wstr)
{
    PDC_LOG(("mvget_wstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wgetn_wstr(stdscr, wstr, MAXLINE);
}

int mvwget_wstr(WINDOW *win, int y, int x, wint_t *wstr)
{
    PDC_LOG(("mvwget_wstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wgetn_wstr(win, wstr, MAXLINE);
}

int getn_wstr(wint_t *wstr, int n)
{
    PDC_LOG(("getn_wstr() - called\n"));

    return wgetn_wstr(stdscr, wstr, n);
}

int mvgetn_wstr(int y, int x, wint_t *wstr, int n)
{
    PDC_LOG(("mvgetn_wstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wgetn_wstr(stdscr, wstr, n);
}

int mvwgetn_wstr(WINDOW *win, int y, int x, wint_t *wstr, int n)
{
    PDC_LOG(("mvwgetn_wstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wgetn_wstr(win, wstr, n);
}
#endif
/* PDCurses */

/*man-start**************************************************************

getyx
-----

### Synopsis

    void getyx(WINDOW *win, int y, int x);
    void getparyx(WINDOW *win, int y, int x);
    void getbegyx(WINDOW *win, int y, int x);
    void getmaxyx(WINDOW *win, int y, int x);

    void getsyx(int y, int x);
    void setsyx(int y, int x);

    int getbegy(WINDOW *win);
    int getbegx(WINDOW *win);
    int getcury(WINDOW *win);
    int getcurx(WINDOW *win);
    int getpary(WINDOW *win);
    int getparx(WINDOW *win);
    int getmaxy(WINDOW *win);
    int getmaxx(WINDOW *win);

### Description

   The getyx() macro (defined in curses.h -- the prototypes here are
   merely illustrative) puts the current cursor position of the
   specified window into y and x. getbegyx() and getmaxyx() return the
   starting coordinates and size of the specified window, respectively.
   getparyx() returns the starting coordinates of the parent's window,
   if the specified window is a subwindow; otherwise it sets y and x to
   -1. These are all macros.

   getsyx() gets the coordinates of the virtual screen cursor, and
   stores them in y and x. If leaveok() is TRUE, it returns -1, -1. If
   lines have been removed with ripoffline(), then getsyx() includes
   these lines in its count; so, the returned y and x values should only
   be used with setsyx().

   setsyx() sets the virtual screen cursor to the y, x coordinates. If
   either y or x is -1, leaveok() is set TRUE, else it's set FALSE.

   getsyx() and setsyx() are meant to be used by a library routine that
   manipulates curses windows without altering the position of the
   cursor. Note that getsyx() is defined only as a macro.

   getbegy(), getbegx(), getcurx(), getcury(), getmaxy(), getmaxx(),
   getpary(), and getparx() return the appropriate coordinate or size
   values, or ERR in the case of a NULL window.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   getyx                 |    Y   |    Y    |   Y
   getparyx              |    Y   |    Y    |   Y
   getbegyx              |    Y   |    Y    |   Y
   getmaxyx              |    Y   |    Y    |   Y
   getsyx                |    -   |    Y    |   Y
   setsyx                |    -   |    Y    |   Y
   getbegy               |    -   |    Y    |   Y
   getbegx               |    -   |    Y    |   Y
   getcury               |    -   |    Y    |   Y
   getcurx               |    -   |    Y    |   Y
   getpary               |    -   |    Y    |   Y
   getparx               |    -   |    Y    |   Y
   getmaxy               |    -   |    Y    |   Y
   getmaxx               |    -   |    Y    |   Y

**man-end****************************************************************/

int getbegy(WINDOW *win)
{
    PDC_LOG(("getbegy() - called\n"));

    return win ? win->_begy : ERR;
}

int getbegx(WINDOW *win)
{
    PDC_LOG(("getbegx() - called\n"));

    return win ? win->_begx : ERR;
}

int getcury(WINDOW *win)
{
    PDC_LOG(("getcury() - called\n"));

    return win ? win->_cury : ERR;
}

int getcurx(WINDOW *win)
{
    PDC_LOG(("getcurx() - called\n"));

    return win ? win->_curx : ERR;
}

int getpary(WINDOW *win)
{
    PDC_LOG(("getpary() - called\n"));

    return win ? win->_pary : ERR;
}

int getparx(WINDOW *win)
{
    PDC_LOG(("getparx() - called\n"));

    return win ? win->_parx : ERR;
}

int getmaxy(WINDOW *win)
{
    PDC_LOG(("getmaxy() - called\n"));

    return win ? win->_maxy : ERR;
}

int getmaxx(WINDOW *win)
{
    PDC_LOG(("getmaxx() - called\n"));

    return win ? win->_maxx : ERR;
}

void setsyx(int y, int x)
{
    PDC_LOG(("setsyx() - called\n"));

    if (curscr)
    {
        curscr->_leaveit = y == -1 || x == -1;

        if (!curscr->_leaveit)
            wmove(curscr, y, x);
    }
}
/* PDCurses */

/*man-start**************************************************************

inch
----

### Synopsis

    chtype inch(void);
    chtype winch(WINDOW *win);
    chtype mvinch(int y, int x);
    chtype mvwinch(WINDOW *win, int y, int x);

    int in_wch(cchar_t *wcval);
    int win_wch(WINDOW *win, cchar_t *wcval);
    int mvin_wch(int y, int x, cchar_t *wcval);
    int mvwin_wch(WINDOW *win, int y, int x, cchar_t *wcval);

### Description

   The inch() functions retrieve the character and attribute from the
   current or specified window position, in the form of a chtype. If a
   NULL window is specified, (chtype)ERR is returned.

   The in_wch() functions are the wide-character versions; instead of
   returning a chtype, they store a cchar_t at the address specified by
   wcval, and return OK or ERR. (No value is stored when ERR is
   returned.) Note that in PDCurses, chtype and cchar_t are the same.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   inch                  |    Y   |    Y    |   Y
   winch                 |    Y   |    Y    |   Y
   mvinch                |    Y   |    Y    |   Y
   mvwinch               |    Y   |    Y    |   Y
   in_wch                |    Y   |    Y    |   Y
   win_wch               |    Y   |    Y    |   Y
   mvin_wch              |    Y   |    Y    |   Y
   mvwin_wch             |    Y   |    Y    |   Y

**man-end****************************************************************/

chtype winch(WINDOW *win)
{
    PDC_LOG(("winch() - called\n"));

    if (!win)
        return (chtype)ERR;

    return win->_y[win->_cury][win->_curx];
}

chtype inch(void)
{
    PDC_LOG(("inch() - called\n"));

    return winch(stdscr);
}

chtype mvinch(int y, int x)
{
    PDC_LOG(("mvinch() - called\n"));

    if (move(y, x) == ERR)
        return (chtype)ERR;

    return stdscr->_y[stdscr->_cury][stdscr->_curx];
}

chtype mvwinch(WINDOW *win, int y, int x)
{
    PDC_LOG(("mvwinch() - called\n"));

    if (wmove(win, y, x) == ERR)
        return (chtype)ERR;

    return win->_y[win->_cury][win->_curx];
}

#ifdef PDC_WIDE
int win_wch(WINDOW *win, cchar_t *wcval)
{
    PDC_LOG(("win_wch() - called\n"));

    if (!win || !wcval)
        return ERR;

    *wcval = win->_y[win->_cury][win->_curx];

    return OK;
}

int in_wch(cchar_t *wcval)
{
    PDC_LOG(("in_wch() - called\n"));

    return win_wch(stdscr, wcval);
}

int mvin_wch(int y, int x, cchar_t *wcval)
{
    PDC_LOG(("mvin_wch() - called\n"));

    if (!wcval || (move(y, x) == ERR))
        return ERR;

    *wcval = stdscr->_y[stdscr->_cury][stdscr->_curx];

    return OK;
}

int mvwin_wch(WINDOW *win, int y, int x, cchar_t *wcval)
{
    PDC_LOG(("mvwin_wch() - called\n"));

    if (!wcval || (wmove(win, y, x) == ERR))
        return ERR;

    *wcval = win->_y[win->_cury][win->_curx];

    return OK;
}
#endif
/* PDCurses */

/*man-start**************************************************************

inchstr
-------

### Synopsis

    int inchstr(chtype *ch);
    int inchnstr(chtype *ch, int n);
    int winchstr(WINDOW *win, chtype *ch);
    int winchnstr(WINDOW *win, chtype *ch, int n);
    int mvinchstr(int y, int x, chtype *ch);
    int mvinchnstr(int y, int x, chtype *ch, int n);
    int mvwinchstr(WINDOW *, int y, int x, chtype *ch);
    int mvwinchnstr(WINDOW *, int y, int x, chtype *ch, int n);

    int in_wchstr(cchar_t *wch);
    int in_wchnstr(cchar_t *wch, int n);
    int win_wchstr(WINDOW *win, cchar_t *wch);
    int win_wchnstr(WINDOW *win, cchar_t *wch, int n);
    int mvin_wchstr(int y, int x, cchar_t *wch);
    int mvin_wchnstr(int y, int x, cchar_t *wch, int n);
    int mvwin_wchstr(WINDOW *win, int y, int x, cchar_t *wch);
    int mvwin_wchnstr(WINDOW *win, int y, int x, cchar_t *wch, int n);

### Description

   These routines read a chtype or cchar_t string from the window,
   starting at the current or specified position, and ending at the
   right margin, or after n elements, whichever is less.

### Return Value

   All functions return the number of elements read, or ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   inchstr               |    Y   |    Y    |   Y
   winchstr              |    Y   |    Y    |   Y
   mvinchstr             |    Y   |    Y    |   Y
   mvwinchstr            |    Y   |    Y    |   Y
   inchnstr              |    Y   |    Y    |   Y
   winchnstr             |    Y   |    Y    |   Y
   mvinchnstr            |    Y   |    Y    |   Y
   mvwinchnstr           |    Y   |    Y    |   Y
   in_wchstr             |    Y   |    Y    |   Y
   win_wchstr            |    Y   |    Y    |   Y
   mvin_wchstr           |    Y   |    Y    |   Y
   mvwin_wchstr          |    Y   |    Y    |   Y
   in_wchnstr            |    Y   |    Y    |   Y
   win_wchnstr           |    Y   |    Y    |   Y
   mvin_wchnstr          |    Y   |    Y    |   Y
   mvwin_wchnstr         |    Y   |    Y    |   Y

**man-end****************************************************************/

int winchnstr(WINDOW *win, chtype *ch, int n)
{
    chtype *src;
    int i;

    PDC_LOG(("winchnstr() - called\n"));

    if (!win || !ch || n < 0)
        return ERR;

    if ((win->_curx + n) > win->_maxx)
        n = win->_maxx - win->_curx;

    src = win->_y[win->_cury] + win->_curx;

    for (i = 0; i < n; i++)
        *ch++ = *src++;

    *ch = (chtype)0;

    return OK;
}

int inchstr(chtype *ch)
{
    PDC_LOG(("inchstr() - called\n"));

    return winchnstr(stdscr, ch, stdscr->_maxx - stdscr->_curx);
}

int winchstr(WINDOW *win, chtype *ch)
{
    PDC_LOG(("winchstr() - called\n"));

    return winchnstr(win, ch, win->_maxx - win->_curx);
}

int mvinchstr(int y, int x, chtype *ch)
{
    PDC_LOG(("mvinchstr() - called: y %d x %d\n", y, x));

    if (move(y, x) == ERR)
        return ERR;

    return winchnstr(stdscr, ch, stdscr->_maxx - stdscr->_curx);
}

int mvwinchstr(WINDOW *win, int y, int x, chtype *ch)
{
    PDC_LOG(("mvwinchstr() - called:\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winchnstr(win, ch, win->_maxx - win->_curx);
}

int inchnstr(chtype *ch, int n)
{
    PDC_LOG(("inchnstr() - called\n"));

    return winchnstr(stdscr, ch, n);
}

int mvinchnstr(int y, int x, chtype *ch, int n)
{
    PDC_LOG(("mvinchnstr() - called: y %d x %d n %d\n", y, x, n));

    if (move(y, x) == ERR)
        return ERR;

    return winchnstr(stdscr, ch, n);
}

int mvwinchnstr(WINDOW *win, int y, int x, chtype *ch, int n)
{
    PDC_LOG(("mvwinchnstr() - called: y %d x %d n %d \n", y, x, n));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winchnstr(win, ch, n);
}

#ifdef PDC_WIDE
int win_wchnstr(WINDOW *win, cchar_t *wch, int n)
{
    PDC_LOG(("win_wchnstr() - called\n"));

    return winchnstr(win, wch, n);
}

int in_wchstr(cchar_t *wch)
{
    PDC_LOG(("in_wchstr() - called\n"));

    return win_wchnstr(stdscr, wch, stdscr->_maxx - stdscr->_curx);
}

int win_wchstr(WINDOW *win, cchar_t *wch)
{
    PDC_LOG(("win_wchstr() - called\n"));

    return win_wchnstr(win, wch, win->_maxx - win->_curx);
}

int mvin_wchstr(int y, int x, cchar_t *wch)
{
    PDC_LOG(("mvin_wchstr() - called: y %d x %d\n", y, x));

    if (move(y, x) == ERR)
        return ERR;

    return win_wchnstr(stdscr, wch, stdscr->_maxx - stdscr->_curx);
}

int mvwin_wchstr(WINDOW *win, int y, int x, cchar_t *wch)
{
    PDC_LOG(("mvwin_wchstr() - called:\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return win_wchnstr(win, wch, win->_maxx - win->_curx);
}

int in_wchnstr(cchar_t *wch, int n)
{
    PDC_LOG(("in_wchnstr() - called\n"));

    return win_wchnstr(stdscr, wch, n);
}

int mvin_wchnstr(int y, int x, cchar_t *wch, int n)
{
    PDC_LOG(("mvin_wchnstr() - called: y %d x %d n %d\n", y, x, n));

    if (move(y, x) == ERR)
        return ERR;

    return win_wchnstr(stdscr, wch, n);
}

int mvwin_wchnstr(WINDOW *win, int y, int x, cchar_t *wch, int n)
{
    PDC_LOG(("mvwinchnstr() - called: y %d x %d n %d \n", y, x, n));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return win_wchnstr(win, wch, n);
}
#endif
/* PDCurses */

/*man-start**************************************************************

initscr
-------

### Synopsis

    WINDOW *initscr(void);
    WINDOW *Xinitscr(int argc, char **argv);
    int endwin(void);
    bool isendwin(void);
    SCREEN *newterm(const char *type, FILE *outfd, FILE *infd);
    SCREEN *set_term(SCREEN *new);
    void delscreen(SCREEN *sp);

    int resize_term(int nlines, int ncols);
    bool is_termresized(void);
    const char *curses_version(void);
    void PDC_get_version(PDC_VERSION *ver);

    int set_tabsize(int tabsize);

### Description

   initscr() should be the first curses routine called. It will
   initialize all curses data structures, and arrange that the first
   call to refresh() will clear the screen. In case of error, initscr()
   will write a message to standard error and end the program.

   endwin() should be called before exiting or escaping from curses mode
   temporarily. It will restore tty modes, move the cursor to the lower
   left corner of the screen and reset the terminal into the proper
   non-visual mode. To resume curses after a temporary escape, call
   refresh() or doupdate().

   isendwin() returns TRUE if endwin() has been called without a
   subsequent refresh, unless SP is NULL.

   In some implementations of curses, newterm() allows the use of
   multiple terminals. Here, it's just an alternative interface for
   initscr(). It always returns SP, or NULL.

   delscreen() frees the memory allocated by newterm() or initscr(),
   since it's not freed by endwin(). This function is usually not
   needed. In PDCurses, the parameter must be the value of SP, and
   delscreen() sets SP to NULL.

   set_term() does nothing meaningful in PDCurses, but is included for
   compatibility with other curses implementations.

   resize_term() is effectively two functions: When called with nonzero
   values for nlines and ncols, it attempts to resize the screen to the
   given size. When called with (0, 0), it merely adjusts the internal
   structures to match the current size after the screen is resized by
   the user. On the currently supported platforms, SDL, Windows console,
   and X11 allow user resizing, while DOS, OS/2, SDL and Windows console
   allow programmatic resizing. If you want to support user resizing,
   you should check for getch() returning KEY_RESIZE, and/or call
   is_termresized() at appropriate times; if either condition occurs,
   call resize_term(0, 0). Then, with either user or programmatic
   resizing, you'll have to resize any windows you've created, as
   appropriate; resize_term() only handles stdscr and curscr.

   is_termresized() returns TRUE if the curses screen has been resized
   by the user, and a call to resize_term() is needed. Checking for
   KEY_RESIZE is generally preferable, unless you're not handling the
   keyboard.

   curses_version() returns a string describing the version of PDCurses.

   PDC_get_version() fills a PDC_VERSION structure provided by the user
   with more detailed version info (see curses.h).

   set_tabsize() sets the tab interval, stored in TABSIZE.

### Return Value

   All functions return NULL on error, except endwin(), which always
   returns OK, and resize_term(), which returns either OK or ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   initscr               |    Y   |    Y    |   Y
   endwin                |    Y   |    Y    |   Y
   isendwin              |    Y   |    Y    |   Y
   newterm               |    Y   |    Y    |   Y
   set_term              |    Y   |    Y    |   Y
   delscreen             |    Y   |    Y    |   Y
   resize_term           |    -   |    Y    |   Y
   set_tabsize           |    -   |    Y    |   Y
   curses_version        |    -   |    Y    |   -
   is_termresized        |    -   |    -    |   -

**man-end****************************************************************/

char ttytype[128];

const char *_curses_notice = "PDCurses " PDC_VERDOT " - " __DATE__;

SCREEN *SP = (SCREEN*)NULL;           /* curses variables */
WINDOW *curscr = (WINDOW *)NULL;      /* the current screen image */
WINDOW *stdscr = (WINDOW *)NULL;      /* the default screen window */

int LINES = 0;                        /* current terminal height */
int COLS = 0;                         /* current terminal width */
int TABSIZE = 8;

MOUSE_STATUS Mouse_status;

extern RIPPEDOFFLINE linesripped[5];
extern char linesrippedoff;

WINDOW *initscr(void)
{
    int i;

    PDC_LOG(("initscr() - called\n"));

    if (SP && SP->alive)
        return NULL;

    SP = calloc(1, sizeof(SCREEN));
    if (!SP)
        return NULL;

    if (PDC_scr_open() == ERR)
    {
        fprintf(stderr, "initscr(): Unable to create SP\n");
        exit(8);
    }

    SP->autocr = TRUE;       /* cr -> lf by default */
    SP->raw_out = FALSE;     /* tty I/O modes */
    SP->raw_inp = FALSE;     /* tty I/O modes */
    SP->cbreak = TRUE;
    SP->key_modifiers = 0L;
    SP->return_key_modifiers = FALSE;
    SP->echo = TRUE;
    SP->visibility = 1;
    SP->resized = FALSE;
    SP->_trap_mbe = 0L;
    SP->linesrippedoff = 0;
    SP->linesrippedoffontop = 0;
    SP->delaytenths = 0;
    SP->line_color = -1;
    SP->lastscr = (WINDOW *)NULL;
    SP->dbfp = NULL;
    SP->color_started = FALSE;
    SP->dirty = FALSE;
    SP->sel_start = -1;
    SP->sel_end = -1;

    SP->orig_cursor = PDC_get_cursor_mode();

    LINES = SP->lines = PDC_get_rows();
    COLS = SP->cols = PDC_get_columns();

    if (LINES < 2 || COLS < 2)
    {
        fprintf(stderr, "initscr(): LINES=%d COLS=%d: too small.\n",
                LINES, COLS);
        exit(4);
    }

    curscr = newwin(LINES, COLS, 0, 0);
    if (!curscr)
    {
        fprintf(stderr, "initscr(): Unable to create curscr.\n");
        exit(2);
    }

    SP->lastscr = newwin(LINES, COLS, 0, 0);
    if (!SP->lastscr)
    {
        fprintf(stderr, "initscr(): Unable to create SP->lastscr.\n");
        exit(2);
    }

    wattrset(SP->lastscr, (chtype)(-1));
    werase(SP->lastscr);

    PDC_slk_initialize();
    LINES -= SP->slklines;

    /* We have to sort out ripped off lines here, and reduce the height
       of stdscr by the number of lines ripped off */

    for (i = 0; i < linesrippedoff; i++)
    {
        if (linesripped[i].line < 0)
            (*linesripped[i].init)(newwin(1, COLS, LINES - 1, 0), COLS);
        else
            (*linesripped[i].init)(newwin(1, COLS,
                                   SP->linesrippedoffontop++, 0), COLS);

        SP->linesrippedoff++;
        LINES--;
    }

    linesrippedoff = 0;

    stdscr = newwin(LINES, COLS, SP->linesrippedoffontop, 0);
    if (!stdscr)
    {
        fprintf(stderr, "initscr(): Unable to create stdscr.\n");
        exit(1);
    }

    wclrtobot(stdscr);

    /* If preserving the existing screen, don't allow a screen clear */

    if (SP->_preserve)
    {
        untouchwin(curscr);
        untouchwin(stdscr);
        stdscr->_clear = FALSE;
        curscr->_clear = FALSE;
    }
    else
        curscr->_clear = TRUE;

    SP->atrtab = calloc(PDC_COLOR_PAIRS, sizeof(PDC_PAIR));
    if (!SP->atrtab)
        return NULL;
    PDC_init_atrtab();  /* set up default colors */

    MOUSE_X_POS = MOUSE_Y_POS = -1;
    BUTTON_STATUS(1) = BUTTON_RELEASED;
    BUTTON_STATUS(2) = BUTTON_RELEASED;
    BUTTON_STATUS(3) = BUTTON_RELEASED;
    Mouse_status.changes = 0;

    SP->alive = TRUE;

    def_shell_mode();

    sprintf(ttytype, "pdcurses|PDCurses for %s", PDC_sysname());

    SP->c_buffer = malloc(_INBUFSIZ * sizeof(int));
    if (!SP->c_buffer)
        return NULL;
    SP->c_pindex = 0;
    SP->c_gindex = 1;

    SP->c_ungch = malloc(NUNGETCH * sizeof(int));
    if (!SP->c_ungch)
        return NULL;
    SP->c_ungind = 0;
    SP->c_ungmax = NUNGETCH;

    return stdscr;
}

#ifdef XCURSES
WINDOW *Xinitscr(int argc, char **argv)
{
    PDC_LOG(("Xinitscr() - called\n"));

    PDC_set_args(argc, argv);
    return initscr();
}
#endif

int endwin(void)
{
    PDC_LOG(("endwin() - called\n"));

    /* Allow temporary exit from curses using endwin() */

    def_prog_mode();
    PDC_scr_close();

    SP->alive = FALSE;

    return OK;
}

bool isendwin(void)
{
    PDC_LOG(("isendwin() - called\n"));

    return SP ? !(SP->alive) : FALSE;
}

SCREEN *newterm(const char *type, FILE *outfd, FILE *infd)
{
    PDC_LOG(("newterm() - called\n"));

    return initscr() ? SP : NULL;
}

SCREEN *set_term(SCREEN *new)
{
    PDC_LOG(("set_term() - called\n"));

    /* We only support one screen */

    return (new == SP) ? SP : NULL;
}

void delscreen(SCREEN *sp)
{
    PDC_LOG(("delscreen() - called\n"));

    if (!SP || sp != SP)
        return;

    free(SP->c_ungch);
    free(SP->c_buffer);
    free(SP->atrtab);

    PDC_slk_free();     /* free the soft label keys, if needed */

    delwin(stdscr);
    delwin(curscr);
    delwin(SP->lastscr);
    stdscr = (WINDOW *)NULL;
    curscr = (WINDOW *)NULL;
    SP->lastscr = (WINDOW *)NULL;

    SP->alive = FALSE;

    PDC_scr_free();

    free(SP);
    SP = (SCREEN *)NULL;
}

int resize_term(int nlines, int ncols)
{
    PDC_LOG(("resize_term() - called: nlines %d\n", nlines));

    if (!stdscr || PDC_resize_screen(nlines, ncols) == ERR)
        return ERR;

    SP->resized = FALSE;

    SP->lines = PDC_get_rows();
    LINES = SP->lines - SP->linesrippedoff - SP->slklines;
    SP->cols = COLS = PDC_get_columns();

    if (SP->cursrow >= SP->lines)
        SP->cursrow = SP->lines - 1;
    if (SP->curscol >= SP->cols)
        SP->curscol = SP->cols - 1;

    if (wresize(curscr, SP->lines, SP->cols) == ERR ||
        wresize(stdscr, LINES, COLS) == ERR ||
        wresize(SP->lastscr, SP->lines, SP->cols) == ERR)
        return ERR;

    werase(SP->lastscr);
    curscr->_clear = TRUE;

    if (SP->slk_winptr)
    {
        if (wresize(SP->slk_winptr, SP->slklines, COLS) == ERR)
            return ERR;

        wmove(SP->slk_winptr, 0, 0);
        wclrtobot(SP->slk_winptr);
        PDC_slk_initialize();
        slk_noutrefresh();
    }

    touchwin(stdscr);
    wnoutrefresh(stdscr);

    return OK;
}

bool is_termresized(void)
{
    PDC_LOG(("is_termresized() - called\n"));

    return SP->resized;
}

const char *curses_version(void)
{
    return _curses_notice;
}

void PDC_get_version(PDC_VERSION *ver)
{
    if (!ver)
        return;

    ver->flags = 0
#ifdef PDCDEBUG
        | PDC_VFLAG_DEBUG
#endif
#ifdef PDC_WIDE
        | PDC_VFLAG_WIDE
#endif
#ifdef PDC_FORCE_UTF8
        | PDC_VFLAG_UTF8
#endif
#ifdef PDC_DLL_BUILD
        | PDC_VFLAG_DLL
#endif
#ifdef PDC_RGB
        | PDC_VFLAG_RGB
#endif
        ;

    ver->build = PDC_BUILD;
    ver->major = PDC_VER_MAJOR;
    ver->minor = PDC_VER_MINOR;
    ver->csize = sizeof(chtype);
    ver->bsize = sizeof(bool);
}

int set_tabsize(int tabsize)
{
    PDC_LOG(("set_tabsize() - called: tabsize %d\n", tabsize));

    if (tabsize < 1)
        return ERR;

    TABSIZE = tabsize;

    return OK;
}
/* PDCurses */

/*man-start**************************************************************

inopts
------

### Synopsis

    int cbreak(void);
    int nocbreak(void);
    int echo(void);
    int noecho(void);
    int halfdelay(int tenths);
    int intrflush(WINDOW *win, bool bf);
    int keypad(WINDOW *win, bool bf);
    int meta(WINDOW *win, bool bf);
    int nl(void);
    int nonl(void);
    int nodelay(WINDOW *win, bool bf);
    int notimeout(WINDOW *win, bool bf);
    int raw(void);
    int noraw(void);
    void noqiflush(void);
    void qiflush(void);
    void timeout(int delay);
    void wtimeout(WINDOW *win, int delay);
    int wgetdelay(const WINDOW *win);
    int typeahead(int fildes);

    int crmode(void);
    int nocrmode(void);

    bool is_keypad(const WINDOW *win);
    bool is_nodelay(const WINDOW *win);
    bool is_notimeout(const WINDOW *win);

### Description

   cbreak() and nocbreak() toggle cbreak mode. In cbreak mode,
   characters typed by the user are made available immediately, and
   erase/kill character processing is not performed. In nocbreak mode,
   typed characters are buffered until a newline or carriage return.
   Interrupt and flow control characters are unaffected by this mode.
   PDCurses always starts in cbreak mode.

   echo() and noecho() control whether typed characters are echoed by
   the input routine. Initially, input characters are echoed. Subsequent
   calls to echo() and noecho() do not flush type-ahead.

   halfdelay() is similar to cbreak(), but allows for a time limit to be
   specified, in tenths of a second. This causes getch() to block for
   that period before returning ERR if no key has been received. tenths
   must be between 1 and 255.

   keypad() controls whether getch() returns function/special keys as
   single key codes (e.g., the left arrow key as KEY_LEFT). Per X/Open,
   the default for keypad mode is OFF. You'll probably want it on. With
   keypad mode off, if a special key is pressed, getch() does nothing or
   returns ERR.

   nodelay() controls whether wgetch() is a non-blocking call. If the
   option is enabled, and no input is ready, wgetch() will return ERR.
   If disabled, wgetch() will hang until input is ready.

   nl() enables the translation of a carriage return into a newline on
   input. nonl() disables this. Initially, the translation does occur.

   raw() and noraw() toggle raw mode. Raw mode is similar to cbreak
   mode, in that characters typed are immediately passed through to the
   user program. The difference is that in raw mode, the INTR, QUIT,
   SUSP, and STOP characters are passed through without being
   interpreted, and without generating a signal.

   In PDCurses, the meta() function sets raw mode on or off.

   timeout() and wtimeout() set blocking or non-blocking reads for the
   specified window. If the delay is negative, a blocking read is used;
   if zero, then non-blocking reads are done -- if no input is waiting,
   ERR is returned immediately. If the delay is positive, the read
   blocks for the delay period; if the period expires, ERR is returned.
   The delay is given in milliseconds, but this is rounded down to 50ms
   (1/20th sec) intervals, with a minimum of one interval if a postive
   delay is given; i.e., 1-99 will wait 50ms, 100-149 will wait 100ms,
   etc.

   wgetdelay() returns the delay timeout as set in wtimeout().

   intrflush(), notimeout(), noqiflush(), qiflush() and typeahead() do
   nothing in PDCurses, but are included for compatibility with other
   curses implementations.

   crmode() and nocrmode() are archaic equivalents to cbreak() and
   nocbreak(), respectively.

   is_keypad() reports whether the specified window is in keypad mode.

   is_nodelay() reports whether the specified window is in nodelay mode.

### Return Value

   is_keypad() and is_nodelay() return TRUE or FALSE. is_notimeout() is
   provided for compatibility with other curses implementations, and
   always returns FALSE. All others return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   cbreak                |    Y   |    Y    |   Y
   nocbreak              |    Y   |    Y    |   Y
   echo                  |    Y   |    Y    |   Y
   noecho                |    Y   |    Y    |   Y
   halfdelay             |    Y   |    Y    |   Y
   intrflush             |    Y   |    Y    |   Y
   keypad                |    Y   |    Y    |   Y
   meta                  |    Y   |    Y    |   Y
   nl                    |    Y   |    Y    |   Y
   nonl                  |    Y   |    Y    |   Y
   nodelay               |    Y   |    Y    |   Y
   notimeout             |    Y   |    Y    |   Y
   raw                   |    Y   |    Y    |   Y
   noraw                 |    Y   |    Y    |   Y
   noqiflush             |    Y   |    Y    |   Y
   qiflush               |    Y   |    Y    |   Y
   timeout               |    Y   |    Y    |   Y
   wtimeout              |    Y   |    Y    |   Y
   wgetdelay             |    -   |    Y    |   -
   typeahead             |    Y   |    Y    |   Y
   crmode                |    Y   |    Y    |   Y
   nocrmode              |    Y   |    Y    |   Y
   is_keypad             |    -   |    Y    |   Y
   is_nodelay            |    -   |    Y    |   -
   is_notimeout          |    -   |    Y    |   -

**man-end****************************************************************/

int cbreak(void)
{
    PDC_LOG(("cbreak() - called\n"));

    if (!SP)
        return ERR;

    SP->cbreak = TRUE;

    return OK;
}

int nocbreak(void)
{
    PDC_LOG(("nocbreak() - called\n"));

    if (!SP)
        return ERR;

    SP->cbreak = FALSE;
    SP->delaytenths = 0;

    return OK;
}

int echo(void)
{
    PDC_LOG(("echo() - called\n"));

    if (!SP)
        return ERR;

    SP->echo = TRUE;

    return OK;
}

int noecho(void)
{
    PDC_LOG(("noecho() - called\n"));

    if (!SP)
        return ERR;

    SP->echo = FALSE;

    return OK;
}

int halfdelay(int tenths)
{
    PDC_LOG(("halfdelay() - called\n"));

    if (!SP || tenths < 1 || tenths > 255)
        return ERR;

    SP->delaytenths = tenths;

    return OK;
}

int intrflush(WINDOW *win, bool bf)
{
    PDC_LOG(("intrflush() - called\n"));

    return OK;
}

int keypad(WINDOW *win, bool bf)
{
    PDC_LOG(("keypad() - called\n"));

    if (!win)
        return ERR;

    win->_use_keypad = bf;

    return OK;
}

int meta(WINDOW *win, bool bf)
{
    PDC_LOG(("meta() - called\n"));

    if (!SP)
        return ERR;

    SP->raw_inp = bf;

    return OK;
}

int nl(void)
{
    PDC_LOG(("nl() - called\n"));

    if (!SP)
        return ERR;

    SP->autocr = TRUE;

    return OK;
}

int nonl(void)
{
    PDC_LOG(("nonl() - called\n"));

    if (!SP)
        return ERR;

    SP->autocr = FALSE;

    return OK;
}

int nodelay(WINDOW *win, bool flag)
{
    PDC_LOG(("nodelay() - called\n"));

    if (!win)
        return ERR;

    win->_nodelay = flag;

    return OK;
}

int notimeout(WINDOW *win, bool flag)
{
    PDC_LOG(("notimeout() - called\n"));

    return OK;
}

int raw(void)
{
    PDC_LOG(("raw() - called\n"));

    if (!SP)
        return ERR;

    PDC_set_keyboard_binary(TRUE);
    SP->raw_inp = TRUE;

    return OK;
}

int noraw(void)
{
    PDC_LOG(("noraw() - called\n"));

    if (!SP)
        return ERR;

    PDC_set_keyboard_binary(FALSE);
    SP->raw_inp = FALSE;

    return OK;
}

void noqiflush(void)
{
    PDC_LOG(("noqiflush() - called\n"));
}

void qiflush(void)
{
    PDC_LOG(("qiflush() - called\n"));
}

void timeout(int delay)
{
    PDC_LOG(("timeout() - called\n"));

    wtimeout(stdscr, delay);
}

void wtimeout(WINDOW *win, int delay)
{
    PDC_LOG(("wtimeout() - called\n"));

    if (!win)
        return;

    if (delay < 0)
    {
        /* This causes a blocking read on the window, so turn on delay
           mode */

        win->_nodelay = FALSE;
        win->_delayms = 0;
    }
    else if (!delay)
    {
        /* This causes a non-blocking read on the window, so turn off
           delay mode */

        win->_nodelay = TRUE;
        win->_delayms = 0;
    }
    else
    {
        /* This causes the read on the window to delay for the number of
           milliseconds. Also forces the window into non-blocking read
           mode */

        /*win->_nodelay = TRUE;*/
        win->_delayms = delay;
    }
}

int wgetdelay(const WINDOW *win)
{
    PDC_LOG(("wgetdelay() - called\n"));

    if (!win)
        return 0;

    return win->_delayms;
}

int typeahead(int fildes)
{
    PDC_LOG(("typeahead() - called\n"));

    return OK;
}

int crmode(void)
{
    PDC_LOG(("crmode() - called\n"));

    return cbreak();
}

int nocrmode(void)
{
    PDC_LOG(("nocrmode() - called\n"));

    return nocbreak();
}

bool is_keypad(const WINDOW *win)
{
    PDC_LOG(("is_keypad() - called\n"));

    if (!win)
        return FALSE;

    return win->_use_keypad;
}

bool is_nodelay(const WINDOW *win)
{
    PDC_LOG(("is_nodelay() - called\n"));

    if (!win)
        return FALSE;

    return win->_nodelay;
}

bool is_notimeout(const WINDOW *win)
{
    (void) win;

    PDC_LOG(("is_notimeout() - called - returning FALSE...\n"));

    return FALSE;
}
/* PDCurses */

/*man-start**************************************************************

insch
-----

### Synopsis

    int insch(chtype ch);
    int winsch(WINDOW *win, chtype ch);
    int mvinsch(int y, int x, chtype ch);
    int mvwinsch(WINDOW *win, int y, int x, chtype ch);

    int insrawch(chtype ch);
    int winsrawch(WINDOW *win, chtype ch);
    int mvinsrawch(int y, int x, chtype ch);
    int mvwinsrawch(WINDOW *win, int y, int x, chtype ch);

    int ins_wch(const cchar_t *wch);
    int wins_wch(WINDOW *win, const cchar_t *wch);
    int mvins_wch(int y, int x, const cchar_t *wch);
    int mvwins_wch(WINDOW *win, int y, int x, const cchar_t *wch);

### Description

   The insch() functions insert a chtype into the window at the current
   or specified cursor position. The cursor is NOT advanced. A newline
   is equivalent to clrtoeol(); tabs are expanded; other control
   characters are converted as with unctrl().

   The ins_wch() functions are the wide-character equivalents, taking
   cchar_t pointers rather than chtypes.

   Video attributes can be combined with a character by ORing them into
   the parameter. Text, including attributes, can be copied from one
   place to another using inch() and insch().

   insrawch() etc. are PDCurses-specific wrappers for insch() etc. that
   disable the translation of control characters.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   insch                 |    Y   |    Y    |   Y
   winsch                |    Y   |    Y    |   Y
   mvinsch               |    Y   |    Y    |   Y
   mvwinsch              |    Y   |    Y    |   Y
   ins_wch               |    Y   |    Y    |   Y
   wins_wch              |    Y   |    Y    |   Y
   mvins_wch             |    Y   |    Y    |   Y
   mvwins_wch            |    Y   |    Y    |   Y
   insrawch              |    -   |    -    |   -
   winsrawch             |    -   |    -    |   -

**man-end****************************************************************/

int winsch(WINDOW *win, chtype ch)
{
    int x, y;
    chtype attr;
    bool xlat;

    PDC_LOG(("winsch() - called: win=%p ch=%x (text=%c attr=0x%x)\n",
             win, ch, ch & A_CHARTEXT, ch & A_ATTRIBUTES));

    if (!win)
        return ERR;

    x = win->_curx;
    y = win->_cury;

    if (y > win->_maxy || x > win->_maxx || y < 0 || x < 0)
        return ERR;

    xlat = !SP->raw_out && !(ch & A_ALTCHARSET);
    attr = ch & A_ATTRIBUTES;
    ch &= A_CHARTEXT;

    if (xlat && (ch < ' ' || ch == 0x7f))
    {
        int x2;

        switch (ch)
        {
        case '\t':
            for (x2 = ((x / TABSIZE) + 1) * TABSIZE; x < x2; x++)
            {
                if (winsch(win, attr | ' ') == ERR)
                    return ERR;
            }
            return OK;

        case '\n':
            wclrtoeol(win);
            break;

        case 0x7f:
            if (winsch(win, attr | '?') == ERR)
                return ERR;

            return winsch(win, attr | '^');

        default:
            /* handle control chars */

            if (winsch(win, attr | (ch + '@')) == ERR)
                return ERR;

            return winsch(win, attr | '^');
        }
    }
    else
    {
        int maxx;
        chtype *temp;

        /* If the incoming character doesn't have its own attribute,
           then use the current attributes for the window. If it has
           attributes but not a color component, OR the attributes to
           the current attributes for the window. If it has a color
           component, use the attributes solely from the incoming
           character. */

        if (!(attr & A_COLOR))
            attr |= win->_attrs;

        /* wrs (4/10/93): Apply the same sort of logic for the window
           background, in that it only takes precedence if other color
           attributes are not there and that the background character
           will only print if the printing character is blank. */

        if (!(attr & A_COLOR))
            attr |= win->_bkgd & A_ATTRIBUTES;
        else
            attr |= win->_bkgd & (A_ATTRIBUTES ^ A_COLOR);

        if (ch == ' ')
            ch = win->_bkgd & A_CHARTEXT;

        /* Add the attribute back into the character. */

        ch |= attr;

        maxx = win->_maxx;
        temp = &win->_y[y][x];

        memmove(temp + 1, temp, (maxx - x - 1) * sizeof(chtype));

        win->_lastch[y] = maxx - 1;

        if ((win->_firstch[y] == _NO_CHANGE) || (win->_firstch[y] > x))
            win->_firstch[y] = x;

        *temp = ch;
    }

    PDC_sync(win);

    return OK;
}

int insch(chtype ch)
{
    PDC_LOG(("insch() - called\n"));

    return winsch(stdscr, ch);
}

int mvinsch(int y, int x, chtype ch)
{
    PDC_LOG(("mvinsch() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return winsch(stdscr, ch);
}

int mvwinsch(WINDOW *win, int y, int x, chtype ch)
{
    PDC_LOG(("mvwinsch() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winsch(win, ch);
}

int winsrawch(WINDOW *win, chtype ch)
{
    PDC_LOG(("winsrawch() - called: win=%p ch=%x "
             "(char=%c attr=0x%x)\n", win, ch,
             ch & A_CHARTEXT, ch & A_ATTRIBUTES));

    if ((ch & A_CHARTEXT) < ' ' || (ch & A_CHARTEXT) == 0x7f)
        ch |= A_ALTCHARSET;

    return winsch(win, ch);
}

int insrawch(chtype ch)
{
    PDC_LOG(("insrawch() - called\n"));

    return winsrawch(stdscr, ch);
}

int mvinsrawch(int y, int x, chtype ch)
{
    PDC_LOG(("mvinsrawch() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return winsrawch(stdscr, ch);
}

int mvwinsrawch(WINDOW *win, int y, int x, chtype ch)
{
    PDC_LOG(("mvwinsrawch() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winsrawch(win, ch);
}

#ifdef PDC_WIDE
int wins_wch(WINDOW *win, const cchar_t *wch)
{
    PDC_LOG(("wins_wch() - called\n"));

    return wch ? winsch(win, *wch) : ERR;
}

int ins_wch(const cchar_t *wch)
{
    PDC_LOG(("ins_wch() - called\n"));

    return wins_wch(stdscr, wch);
}

int mvins_wch(int y, int x, const cchar_t *wch)
{
    PDC_LOG(("mvins_wch() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wins_wch(stdscr, wch);
}

int mvwins_wch(WINDOW *win, int y, int x, const cchar_t *wch)
{
    PDC_LOG(("mvwins_wch() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wins_wch(win, wch);
}
#endif
/* PDCurses */

/*man-start**************************************************************

insstr
------

### Synopsis

    int insstr(const char *str);
    int insnstr(const char *str, int n);
    int winsstr(WINDOW *win, const char *str);
    int winsnstr(WINDOW *win, const char *str, int n);
    int mvinsstr(int y, int x, const char *str);
    int mvinsnstr(int y, int x, const char *str, int n);
    int mvwinsstr(WINDOW *win, int y, int x, const char *str);
    int mvwinsnstr(WINDOW *win, int y, int x, const char *str, int n);

    int ins_wstr(const wchar_t *wstr);
    int ins_nwstr(const wchar_t *wstr, int n);
    int wins_wstr(WINDOW *win, const wchar_t *wstr);
    int wins_nwstr(WINDOW *win, const wchar_t *wstr, int n);
    int mvins_wstr(int y, int x, const wchar_t *wstr);
    int mvins_nwstr(int y, int x, const wchar_t *wstr, int n);
    int mvwins_wstr(WINDOW *win, int y, int x, const wchar_t *wstr);
    int mvwins_nwstr(WINDOW *win, int y, int x, const wchar_t *wstr, int n);

### Description

   The insstr() functions insert a character string into a window at the
   current cursor position, by repeatedly calling winsch(). When
   PDCurses is built with wide-character support enabled, the narrow-
   character functions treat the string as a multibyte string in the
   current locale, and convert it first. All characters to the right of
   the cursor are moved to the right, with the possibility of the
   rightmost characters on the line being lost. The cursor position
   does not change (after moving to y, x, if specified). The routines
   with n as the last argument insert at most n characters; if n is
   negative, then the entire string is inserted.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   insstr                |    Y   |    Y    |   Y
   winsstr               |    Y   |    Y    |   Y
   mvinsstr              |    Y   |    Y    |   Y
   mvwinsstr             |    Y   |    Y    |   Y
   insnstr               |    Y   |    Y    |   Y
   winsnstr              |    Y   |    Y    |   Y
   mvinsnstr             |    Y   |    Y    |   Y
   mvwinsnstr            |    Y   |    Y    |   Y
   ins_wstr              |    Y   |    Y    |   Y
   wins_wstr             |    Y   |    Y    |   Y
   mvins_wstr            |    Y   |    Y    |   Y
   mvwins_wstr           |    Y   |    Y    |   Y
   ins_nwstr             |    Y   |    Y    |   Y
   wins_nwstr            |    Y   |    Y    |   Y
   mvins_nwstr           |    Y   |    Y    |   Y
   mvwins_nwstr          |    Y   |    Y    |   Y

**man-end****************************************************************/

int winsnstr(WINDOW *win, const char *str, int n)
{
#ifdef PDC_WIDE
    wchar_t wstr[513], *p;
    int i;
#endif
    int len;

    PDC_LOG(("winsnstr() - called: string=\"%s\" n %d \n", str, n));

    if (!win || !str)
        return ERR;

    len = strlen(str);

    if (n < 0 || n > len)
        n = len;

#ifdef PDC_WIDE
    if (n > 512)
        n = 512;

    p = wstr;
    i = 0;

    while (str[i] && i < n)
    {
        int retval = PDC_mbtowc(p, str + i, n - i);

        if (retval <= 0)
            break;
        p++;
        i += retval;
    }

    while (p > wstr)
        if (winsch(win, *--p) == ERR)
#else
    while (n)
        if (winsch(win, (unsigned char)(str[--n])) == ERR)
#endif
            return ERR;

    return OK;
}

int insstr(const char *str)
{
    PDC_LOG(("insstr() - called: string=\"%s\"\n", str));

    return winsnstr(stdscr, str, -1);
}

int winsstr(WINDOW *win, const char *str)
{
    PDC_LOG(("winsstr() - called: string=\"%s\"\n", str));

    return winsnstr(win, str, -1);
}

int mvinsstr(int y, int x, const char *str)
{
    PDC_LOG(("mvinsstr() - called: y %d x %d string=\"%s\"\n", y, x, str));

    if (move(y, x) == ERR)
        return ERR;

    return winsnstr(stdscr, str, -1);
}

int mvwinsstr(WINDOW *win, int y, int x, const char *str)
{
    PDC_LOG(("mvwinsstr() - called: string=\"%s\"\n", str));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winsnstr(win, str, -1);
}

int insnstr(const char *str, int n)
{
    PDC_LOG(("insnstr() - called: string=\"%s\" n %d \n", str, n));

    return winsnstr(stdscr, str, n);
}

int mvinsnstr(int y, int x, const char *str, int n)
{
    PDC_LOG(("mvinsnstr() - called: y %d x %d string=\"%s\" n %d \n",
             y, x, str, n));

    if (move(y, x) == ERR)
        return ERR;

    return winsnstr(stdscr, str, n);
}

int mvwinsnstr(WINDOW *win, int y, int x, const char *str, int n)
{
    PDC_LOG(("mvwinsnstr() - called: y %d x %d string=\"%s\" n %d \n",
             y, x, str, n));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winsnstr(win, str, n);
}

#ifdef PDC_WIDE
int wins_nwstr(WINDOW *win, const wchar_t *wstr, int n)
{
    const wchar_t *p;
    int len;

    PDC_LOG(("wins_nwstr() - called\n"));

    if (!win || !wstr)
        return ERR;

    for (len = 0, p = wstr; *p; p++)
        len++;

    if (n < 0 || n > len)
        n = len;

    while (n)
        if (winsch(win, wstr[--n]) == ERR)
            return ERR;

    return OK;
}

int ins_wstr(const wchar_t *wstr)
{
    PDC_LOG(("ins_wstr() - called\n"));

    return wins_nwstr(stdscr, wstr, -1);
}

int wins_wstr(WINDOW *win, const wchar_t *wstr)
{
    PDC_LOG(("wins_wstr() - called\n"));

    return wins_nwstr(win, wstr, -1);
}

int mvins_wstr(int y, int x, const wchar_t *wstr)
{
    PDC_LOG(("mvins_wstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wins_nwstr(stdscr, wstr, -1);
}

int mvwins_wstr(WINDOW *win, int y, int x, const wchar_t *wstr)
{
    PDC_LOG(("mvwinsstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wins_nwstr(win, wstr, -1);
}

int ins_nwstr(const wchar_t *wstr, int n)
{
    PDC_LOG(("ins_nwstr() - called\n"));

    return wins_nwstr(stdscr, wstr, n);
}

int mvins_nwstr(int y, int x, const wchar_t *wstr, int n)
{
    PDC_LOG(("mvinsnstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return wins_nwstr(stdscr, wstr, n);
}

int mvwins_nwstr(WINDOW *win, int y, int x, const wchar_t *wstr, int n)
{
    PDC_LOG(("mvwinsnstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return wins_nwstr(win, wstr, n);
}
#endif
/* PDCurses */

/*man-start**************************************************************

instr
-----

### Synopsis

    int instr(char *str);
    int innstr(char *str, int n);
    int winstr(WINDOW *win, char *str);
    int winnstr(WINDOW *win, char *str, int n);
    int mvinstr(int y, int x, char *str);
    int mvinnstr(int y, int x, char *str, int n);
    int mvwinstr(WINDOW *win, int y, int x, char *str);
    int mvwinnstr(WINDOW *win, int y, int x, char *str, int n);

    int inwstr(wchar_t *wstr);
    int innwstr(wchar_t *wstr, int n);
    int winwstr(WINDOW *win, wchar_t *wstr);
    int winnwstr(WINDOW *win, wchar_t *wstr, int n);
    int mvinwstr(int y, int x, wchar_t *wstr);
    int mvinnwstr(int y, int x, wchar_t *wstr, int n);
    int mvwinwstr(WINDOW *win, int y, int x, wchar_t *wstr);
    int mvwinnwstr(WINDOW *win, int y, int x, wchar_t *wstr, int n);

### Description

   These functions take characters (or wide characters) from the current
   or specified position in the window, and return them as a string in
   str (or wstr). Attributes are ignored. The functions with n as the
   last argument return a string at most n characters long.

### Return Value

   Upon successful completion, innstr(), mvinnstr(), mvwinnstr() and
   winnstr() return the number of characters actually read into the
   string; instr(), mvinstr(), mvwinstr() and winstr() return OK.
   Otherwise, all these functions return ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   instr                 |    Y   |    Y    |   Y
   winstr                |    Y   |    Y    |   Y
   mvinstr               |    Y   |    Y    |   Y
   mvwinstr              |    Y   |    Y    |   Y
   innstr                |    Y   |    Y    |   Y
   winnstr               |    Y   |    Y    |   Y
   mvinnstr              |    Y   |    Y    |   Y
   mvwinnstr             |    Y   |    Y    |   Y
   inwstr                |    Y   |    Y    |   Y
   winwstr               |    Y   |    Y    |   Y
   mvinwstr              |    Y   |    Y    |   Y
   mvwinwstr             |    Y   |    Y    |   Y
   innwstr               |    Y   |    Y    |   Y
   winnwstr              |    Y   |    Y    |   Y
   mvinnwstr             |    Y   |    Y    |   Y
   mvwinnwstr            |    Y   |    Y    |   Y

**man-end****************************************************************/

int winnstr(WINDOW *win, char *str, int n)
{
#ifdef PDC_WIDE
    wchar_t wstr[513];

    if (n < 0 || n > 512)
        n = 512;

    if (winnwstr(win, wstr, n) == ERR)
        return ERR;

    return PDC_wcstombs(str, wstr, n);
#else
    chtype *src;
    int i;

    PDC_LOG(("winnstr() - called: n %d \n", n));

    if (!win || !str)
        return ERR;

    if (n < 0 || (win->_curx + n) > win->_maxx)
        n = win->_maxx - win->_curx;

    src = win->_y[win->_cury] + win->_curx;

    for (i = 0; i < n; i++)
        str[i] = src[i] & A_CHARTEXT;

    str[i] = '\0';

    return i;
#endif
}

int instr(char *str)
{
    PDC_LOG(("instr() - called: string=\"%s\"\n", str));

    return (ERR == winnstr(stdscr, str, stdscr->_maxx)) ? ERR : OK;
}

int winstr(WINDOW *win, char *str)
{
    PDC_LOG(("winstr() - called: \n"));

    return (ERR == winnstr(win, str, win->_maxx)) ? ERR : OK;
}

int mvinstr(int y, int x, char *str)
{
    PDC_LOG(("mvinstr() - called: y %d x %d \n", y, x));

    if (move(y, x) == ERR)
        return ERR;

    return (ERR == winnstr(stdscr, str, stdscr->_maxx)) ? ERR : OK;
}

int mvwinstr(WINDOW *win, int y, int x, char *str)
{
    PDC_LOG(("mvwinstr() - called: y %d x %d \n", y, x));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return (ERR == winnstr(win, str, win->_maxx)) ? ERR : OK;
}

int innstr(char *str, int n)
{
    PDC_LOG(("innstr() - called: n %d \n", n));

    return winnstr(stdscr, str, n);
}

int mvinnstr(int y, int x, char *str, int n)
{
    PDC_LOG(("mvinnstr() - called: y %d x %d n %d \n", y, x, n));

    if (move(y, x) == ERR)
        return ERR;

    return winnstr(stdscr, str, n);
}

int mvwinnstr(WINDOW *win, int y, int x, char *str, int n)
{
    PDC_LOG(("mvwinnstr() - called: y %d x %d n %d \n", y, x, n));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winnstr(win, str, n);
}

#ifdef PDC_WIDE
int winnwstr(WINDOW *win, wchar_t *wstr, int n)
{
    chtype *src;
    int i;

    PDC_LOG(("winnstr() - called: n %d \n", n));

    if (!win || !wstr)
        return ERR;

    if (n < 0 || (win->_curx + n) > win->_maxx)
        n = win->_maxx - win->_curx;

    src = win->_y[win->_cury] + win->_curx;

    for (i = 0; i < n; i++)
        wstr[i] = src[i] & A_CHARTEXT;

    wstr[i] = L'\0';

    return i;
}

int inwstr(wchar_t *wstr)
{
    PDC_LOG(("inwstr() - called\n"));

    return (ERR == winnwstr(stdscr, wstr, stdscr->_maxx)) ? ERR : OK;
}

int winwstr(WINDOW *win, wchar_t *wstr)
{
    PDC_LOG(("winwstr() - called\n"));

    return (ERR == winnwstr(win, wstr, win->_maxx)) ? ERR : OK;
}

int mvinwstr(int y, int x, wchar_t *wstr)
{
    PDC_LOG(("mvinwstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return (ERR == winnwstr(stdscr, wstr, stdscr->_maxx)) ? ERR : OK;
}

int mvwinwstr(WINDOW *win, int y, int x, wchar_t *wstr)
{
    PDC_LOG(("mvwinstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return (ERR == winnwstr(win, wstr, win->_maxx)) ? ERR : OK;
}

int innwstr(wchar_t *wstr, int n)
{
    PDC_LOG(("innwstr() - called\n"));

    return winnwstr(stdscr, wstr, n);
}

int mvinnwstr(int y, int x, wchar_t *wstr, int n)
{
    PDC_LOG(("mvinnstr() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    return winnwstr(stdscr, wstr, n);
}

int mvwinnwstr(WINDOW *win, int y, int x, wchar_t *wstr, int n)
{
    PDC_LOG(("mvwinnwstr() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    return winnwstr(win, wstr, n);
}
#endif
/* PDCurses */

/*man-start**************************************************************

kernel
------

### Synopsis

    int def_prog_mode(void);
    int def_shell_mode(void);
    int reset_prog_mode(void);
    int reset_shell_mode(void);
    int resetty(void);
    int savetty(void);
    int ripoffline(int line, int (*init)(WINDOW *, int));
    int curs_set(int visibility);
    int napms(int ms);

    int draino(int ms);
    int resetterm(void);
    int fixterm(void);
    int saveterm(void);

### Description

   def_prog_mode() and def_shell_mode() save the current terminal modes
   as the "program" (in curses) or "shell" (not in curses) state for use
   by the reset_prog_mode() and reset_shell_mode() functions. This is
   done automatically by initscr().

   reset_prog_mode() and reset_shell_mode() restore the terminal to
   "program" (in curses) or "shell" (not in curses) state. These are
   done automatically by endwin() and doupdate() after an endwin(), so
   they would normally not be called before these functions.

   savetty() and resetty() save and restore the state of the terminal
   modes. savetty() saves the current state in a buffer, and resetty()
   restores the state to what it was at the last call to savetty().

   curs_set() alters the appearance of the cursor. A visibility of 0
   makes it disappear; 1 makes it appear "normal" (usually an underline)
   and 2 makes it "highly visible" (usually a block).

   ripoffline() reduces the size of stdscr by one line. If the "line"
   parameter is positive, the line is removed from the top of the
   screen; if negative, from the bottom. Up to 5 lines can be ripped off
   stdscr by calling ripoffline() repeatedly. The function argument,
   init, is called from within initscr() or newterm(), so ripoffline()
   must be called before either of these functions. The init function
   receives a pointer to a one-line WINDOW, and the width of the window.
   Calling ripoffline() with a NULL init function pointer is an error.

   napms() suspends the program for the specified number of
   milliseconds. draino() is an archaic equivalent. Note that since
   napms() attempts to give up a time slice and yield control back to
   the OS, all times are approximate. (In DOS, the delay is actually
   rounded down to 50ms (1/20th sec) intervals, with a minimum of one
   interval; i.e., 1-99 will wait 50ms, 100-149 will wait 100ms, etc.)
   0 returns immediately.

   resetterm(), fixterm() and saveterm() are archaic equivalents for
   reset_shell_mode(), reset_prog_mode() and def_prog_mode(),
   respectively.

### Return Value

   All functions return OK on success and ERR on error, except
   curs_set(), which returns the previous visibility.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   def_prog_mode         |    Y   |    Y    |   Y
   def_shell_mode        |    Y   |    Y    |   Y
   reset_prog_mode       |    Y   |    Y    |   Y
   reset_shell_mode      |    Y   |    Y    |   Y
   resetty               |    Y   |    Y    |   Y
   savetty               |    Y   |    Y    |   Y
   ripoffline            |    Y   |    Y    |   Y
   curs_set              |    Y   |    Y    |   Y
   napms                 |    Y   |    Y    |   Y
   fixterm               |    -   |    Y    |   -
   resetterm             |    -   |    Y    |   -
   saveterm              |    -   |    Y    |   -
   draino                |    -   |    -    |   -

**man-end****************************************************************/

RIPPEDOFFLINE linesripped[5];
char linesrippedoff = 0;

static struct cttyset
{
    bool been_set;
    SCREEN saved;
} ctty[3];

enum { PDC_SH_TTY, PDC_PR_TTY, PDC_SAVE_TTY };

static void _save_mode(int i)
{
    ctty[i].been_set = TRUE;

    memcpy(&(ctty[i].saved), SP, sizeof(SCREEN));

    PDC_save_screen_mode(i);
}

static int _restore_mode(int i)
{
    if (ctty[i].been_set == TRUE)
    {
        memcpy(SP, &(ctty[i].saved), sizeof(SCREEN));

        if (ctty[i].saved.raw_out)
            raw();

        PDC_restore_screen_mode(i);

        if ((LINES != ctty[i].saved.lines) ||
            (COLS != ctty[i].saved.cols))
            resize_term(ctty[i].saved.lines, ctty[i].saved.cols);

        PDC_curs_set(ctty[i].saved.visibility);

        PDC_gotoyx(ctty[i].saved.cursrow, ctty[i].saved.curscol);
    }

    return ctty[i].been_set ? OK : ERR;
}

int def_prog_mode(void)
{
    PDC_LOG(("def_prog_mode() - called\n"));

    if (!SP)
        return ERR;

    _save_mode(PDC_PR_TTY);

    return OK;
}

int def_shell_mode(void)
{
    PDC_LOG(("def_shell_mode() - called\n"));

    if (!SP)
        return ERR;

    _save_mode(PDC_SH_TTY);

    return OK;
}

int reset_prog_mode(void)
{
    PDC_LOG(("reset_prog_mode() - called\n"));

    if (!SP)
        return ERR;

    _restore_mode(PDC_PR_TTY);
    PDC_reset_prog_mode();

    return OK;
}

int reset_shell_mode(void)
{
    PDC_LOG(("reset_shell_mode() - called\n"));

    if (!SP)
        return ERR;

    _restore_mode(PDC_SH_TTY);
    PDC_reset_shell_mode();

    return OK;
}

int resetty(void)
{
    PDC_LOG(("resetty() - called\n"));

    if (!SP)
        return ERR;

    return _restore_mode(PDC_SAVE_TTY);
}

int savetty(void)
{
    PDC_LOG(("savetty() - called\n"));

    if (!SP)
        return ERR;

    _save_mode(PDC_SAVE_TTY);

    return OK;
}

int curs_set(int visibility)
{
    int ret_vis;

    PDC_LOG(("curs_set() - called: visibility=%d\n", visibility));

    if (!SP || visibility < 0 || visibility > 2)
        return ERR;

    ret_vis = PDC_curs_set(visibility);

    /* If the cursor is changing from invisible to visible, update
       its position */

    if (visibility && !ret_vis)
        PDC_gotoyx(SP->cursrow, SP->curscol);

    return ret_vis;
}

int napms(int ms)
{
    PDC_LOG(("napms() - called: ms=%d\n", ms));

    if (!SP)
        return ERR;

    if (SP->dirty)
    {
        int curs_state = SP->visibility;
        bool leave_state = is_leaveok(curscr);

        SP->dirty = FALSE;

        leaveok(curscr, TRUE);

        wrefresh(curscr);

        leaveok(curscr, leave_state);
        curs_set(curs_state);
    }

    if (ms)
        PDC_napms(ms);

    return OK;
}

int ripoffline(int line, int (*init)(WINDOW *, int))
{
    PDC_LOG(("ripoffline() - called: line=%d\n", line));

    if (linesrippedoff < 5 && line && init)
    {
        linesripped[(int)linesrippedoff].line = line;
        linesripped[(int)linesrippedoff++].init = init;

        return OK;
    }

    return ERR;
}

int draino(int ms)
{
    PDC_LOG(("draino() - called\n"));

    return napms(ms);
}

int resetterm(void)
{
    PDC_LOG(("resetterm() - called\n"));

    return reset_shell_mode();
}

int fixterm(void)
{
    PDC_LOG(("fixterm() - called\n"));

    return reset_prog_mode();
}

int saveterm(void)
{
    PDC_LOG(("saveterm() - called\n"));

    return def_prog_mode();
}
/* PDCurses */

/*man-start**************************************************************

keyname
-------

### Synopsis

    char *keyname(int key);

    char *key_name(wchar_t c);

    bool has_key(int key);

### Description

   keyname() returns a string corresponding to the argument key. key may
   be any key returned by wgetch().

   key_name() is the wide-character version. It takes a wchar_t
   parameter, but still returns a char *.

   has_key() returns TRUE for recognized keys, FALSE otherwise. This
   function is an ncurses extension.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   keyname               |    Y   |    Y    |   Y
   key_name              |    Y   |    Y    |   Y
   has_key               |    -   |    Y    |   Y

**man-end****************************************************************/

static const char *names[] =
{
    "KEY_BREAK", "KEY_DOWN", "KEY_UP", "KEY_LEFT", "KEY_RIGHT",
    "KEY_HOME", "KEY_BACKSPACE", "KEY_F0", "KEY_F(1)", "KEY_F(2)",
    "KEY_F(3)", "KEY_F(4)", "KEY_F(5)", "KEY_F(6)", "KEY_F(7)",
    "KEY_F(8)", "KEY_F(9)", "KEY_F(10)", "KEY_F(11)", "KEY_F(12)",
    "KEY_F(13)", "KEY_F(14)", "KEY_F(15)", "KEY_F(16)", "KEY_F(17)",
    "KEY_F(18)", "KEY_F(19)", "KEY_F(20)", "KEY_F(21)", "KEY_F(22)",
    "KEY_F(23)", "KEY_F(24)", "KEY_F(25)", "KEY_F(26)", "KEY_F(27)",
    "KEY_F(28)", "KEY_F(29)", "KEY_F(30)", "KEY_F(31)", "KEY_F(32)",
    "KEY_F(33)", "KEY_F(34)", "KEY_F(35)", "KEY_F(36)", "KEY_F(37)",
    "KEY_F(38)", "KEY_F(39)", "KEY_F(40)", "KEY_F(41)", "KEY_F(42)",
    "KEY_F(43)", "KEY_F(44)", "KEY_F(45)", "KEY_F(46)", "KEY_F(47)",
    "KEY_F(48)", "KEY_F(49)", "KEY_F(50)", "KEY_F(51)", "KEY_F(52)",
    "KEY_F(53)", "KEY_F(54)", "KEY_F(55)", "KEY_F(56)", "KEY_F(57)",
    "KEY_F(58)", "KEY_F(59)", "KEY_F(60)", "KEY_F(61)", "KEY_F(62)",
    "KEY_F(63)", "KEY_DL", "KEY_IL", "KEY_DC", "KEY_IC", "KEY_EIC",
    "KEY_CLEAR", "KEY_EOS", "KEY_EOL", "KEY_SF", "KEY_SR", "KEY_NPAGE",
    "KEY_PPAGE", "KEY_STAB", "KEY_CTAB", "KEY_CATAB", "KEY_ENTER",
    "KEY_SRESET", "KEY_RESET", "KEY_PRINT", "KEY_LL", "KEY_ABORT",
    "KEY_SHELP", "KEY_LHELP", "KEY_BTAB", "KEY_BEG", "KEY_CANCEL",
    "KEY_CLOSE", "KEY_COMMAND", "KEY_COPY", "KEY_CREATE", "KEY_END",
    "KEY_EXIT", "KEY_FIND", "KEY_HELP", "KEY_MARK", "KEY_MESSAGE",
    "KEY_MOVE", "KEY_NEXT", "KEY_OPEN", "KEY_OPTIONS", "KEY_PREVIOUS",
    "KEY_REDO", "KEY_REFERENCE", "KEY_REFRESH", "KEY_REPLACE",
    "KEY_RESTART", "KEY_RESUME", "KEY_SAVE", "KEY_SBEG", "KEY_SCANCEL",
    "KEY_SCOMMAND", "KEY_SCOPY", "KEY_SCREATE", "KEY_SDC", "KEY_SDL",
    "KEY_SELECT", "KEY_SEND", "KEY_SEOL", "KEY_SEXIT", "KEY_SFIND",
    "KEY_SHOME", "KEY_SIC", "UNKNOWN KEY", "KEY_SLEFT", "KEY_SMESSAGE",
    "KEY_SMOVE", "KEY_SNEXT", "KEY_SOPTIONS", "KEY_SPREVIOUS",
    "KEY_SPRINT", "KEY_SREDO", "KEY_SREPLACE", "KEY_SRIGHT",
    "KEY_SRSUME", "KEY_SSAVE", "KEY_SSUSPEND", "KEY_SUNDO",
    "KEY_SUSPEND", "KEY_UNDO", "ALT_0", "ALT_1", "ALT_2", "ALT_3",
    "ALT_4", "ALT_5", "ALT_6", "ALT_7", "ALT_8", "ALT_9", "ALT_A",
    "ALT_B", "ALT_C", "ALT_D", "ALT_E", "ALT_F", "ALT_G", "ALT_H",
    "ALT_I", "ALT_J", "ALT_K", "ALT_L", "ALT_M", "ALT_N", "ALT_O",
    "ALT_P", "ALT_Q", "ALT_R", "ALT_S", "ALT_T", "ALT_U", "ALT_V",
    "ALT_W", "ALT_X", "ALT_Y", "ALT_Z", "CTL_LEFT", "CTL_RIGHT",
    "CTL_PGUP", "CTL_PGDN", "CTL_HOME", "CTL_END", "KEY_A1", "KEY_A2",
    "KEY_A3", "KEY_B1", "KEY_B2", "KEY_B3", "KEY_C1", "KEY_C2",
    "KEY_C3", "PADSLASH", "PADENTER", "CTL_PADENTER", "ALT_PADENTER",
    "PADSTOP", "PADSTAR", "PADMINUS", "PADPLUS", "CTL_PADSTOP",
    "CTL_PADCENTER", "CTL_PADPLUS", "CTL_PADMINUS", "CTL_PADSLASH",
    "CTL_PADSTAR", "ALT_PADPLUS", "ALT_PADMINUS", "ALT_PADSLASH",
    "ALT_PADSTAR", "ALT_PADSTOP", "CTL_INS", "ALT_DEL", "ALT_INS",
    "CTL_UP", "CTL_DOWN", "CTL_TAB", "ALT_TAB", "ALT_MINUS",
    "ALT_EQUAL", "ALT_HOME", "ALT_PGUP", "ALT_PGDN", "ALT_END",
    "ALT_UP", "ALT_DOWN", "ALT_RIGHT", "ALT_LEFT", "ALT_ENTER",
    "ALT_ESC", "ALT_BQUOTE", "ALT_LBRACKET", "ALT_RBRACKET",
    "ALT_SEMICOLON", "ALT_FQUOTE", "ALT_COMMA", "ALT_STOP",
    "ALT_FSLASH", "ALT_BKSP", "CTL_BKSP", "PAD0", "CTL_PAD0",
    "CTL_PAD1", "CTL_PAD2", "CTL_PAD3", "CTL_PAD4", "CTL_PAD5",
    "CTL_PAD6", "CTL_PAD7","CTL_PAD8", "CTL_PAD9", "ALT_PAD0",
    "ALT_PAD1", "ALT_PAD2", "ALT_PAD3", "ALT_PAD4", "ALT_PAD5",
    "ALT_PAD6", "ALT_PAD7", "ALT_PAD8", "ALT_PAD9", "CTL_DEL",
    "ALT_BSLASH", "CTL_ENTER", "SHF_PADENTER", "SHF_PADSLASH",
    "SHF_PADSTAR", "SHF_PADPLUS", "SHF_PADMINUS", "SHF_UP", "SHF_DOWN",
    "SHF_IC", "SHF_DC", "KEY_MOUSE", "KEY_SHIFT_L", "KEY_SHIFT_R",
    "KEY_CONTROL_L", "KEY_CONTROL_R", "KEY_ALT_L", "KEY_ALT_R",
    "KEY_RESIZE", "KEY_SUP", "KEY_SDOWN"
};

char *keyname(int key)
{
    static char _keyname[14];

    /* Key names must be in exactly the same order as in curses.h */

    PDC_LOG(("keyname() - called: key %d\n", key));

    strcpy(_keyname, ((key >= 0) && (key < 0x80)) ? unctrl((chtype)key) :
           has_key(key) ? names[key - KEY_MIN] : "UNKNOWN KEY");

    return _keyname;
}

bool has_key(int key)
{
    PDC_LOG(("has_key() - called: key %d\n", key));

    return (key >= KEY_MIN && key <= KEY_MAX);
}

#ifdef PDC_WIDE
char *key_name(wchar_t c)
{
    PDC_LOG(("key_name() - called\n"));

    return keyname((int)c);
}
#endif
/* PDCurses */

/*man-start**************************************************************

mouse
-----

### Synopsis

    int mouse_set(mmask_t mbe);
    int mouse_on(mmask_t mbe);
    int mouse_off(mmask_t mbe);
    int request_mouse_pos(void);
    void wmouse_position(WINDOW *win, int *y, int *x);
    mmask_t getmouse(void);

    int mouseinterval(int wait);
    bool wenclose(const WINDOW *win, int y, int x);
    bool wmouse_trafo(const WINDOW *win, int *y, int *x, bool to_screen);
    bool mouse_trafo(int *y, int *x, bool to_screen);
    mmask_t mousemask(mmask_t mask, mmask_t *oldmask);
    int nc_getmouse(MEVENT *event);
    int ungetmouse(MEVENT *event);
    bool has_mouse(void);

### Description

   As of PDCurses 3.0, there are two separate mouse interfaces: the
   classic interface, which is based on the undocumented Sys V mouse
   functions; and an ncurses-compatible interface. Both are active at
   all times, and you can mix and match functions from each, though it's
   not recommended. The ncurses interface is essentially an emulation
   layer built on top of the classic interface; it's here to allow
   easier porting of ncurses apps.

   The classic interface: mouse_set(), mouse_on(), mouse_off(),
   request_mouse_pos(), wmouse_position(), and getmouse(). An
   application using this interface would start by calling mouse_set()
   or mouse_on() with a non-zero value, often ALL_MOUSE_EVENTS. Then it
   would check for a KEY_MOUSE return from getch(). If found, it would
   call request_mouse_pos() to get the current mouse status.

   mouse_set(), mouse_on() and mouse_off() are analagous to attrset(),
   attron() and attroff(). These functions set the mouse button events
   to trap. The button masks used in these functions are defined in
   curses.h and can be or'ed together. They are the group of masks
   starting with BUTTON1_RELEASED.

   request_mouse_pos() requests curses to fill in the Mouse_status
   structure with the current state of the mouse.

   wmouse_position() determines if the current mouse position is within
   the window passed as an argument. If the mouse is outside the current
   window, -1 is returned in the y and x arguments; otherwise the y and
   x coordinates of the mouse (relative to the top left corner of the
   window) are returned in y and x.

   getmouse() returns the current status of the trapped mouse buttons as
   set by mouse_set() or mouse_on().

   The ncurses interface: mouseinterval(), wenclose(), wmouse_trafo(),
   mouse_trafo(), mousemask(), nc_getmouse(), ungetmouse() and
   has_mouse(). A typical application using this interface would start
   by calling mousemask() with a non-zero value, often ALL_MOUSE_EVENTS.
   Then it would check for a KEY_MOUSE return from getch(). If found, it
   would call nc_getmouse() to get the current mouse status.

   mouseinterval() sets the timeout for a mouse click. On all current
   platforms, PDCurses receives mouse button press and release events,
   but must synthesize click events. It does this by checking whether a
   release event is queued up after a press event. If it gets a press
   event, and there are no more events waiting, it will wait for the
   timeout interval, then check again for a release. A press followed by
   a release is reported as BUTTON_CLICKED; otherwise it's passed
   through as BUTTON_PRESSED. The default timeout is 150ms; valid values
   are 0 (no clicks reported) through 1000ms. In x11, the timeout can
   also be set via the clickPeriod resource. The return value from
   mouseinterval() is the old timeout. To check the old value without
   setting a new one, call it with a parameter of -1. Note that although
   there's no classic equivalent for this function (apart from the
   clickPeriod resource), the value set applies in both interfaces.

   wenclose() reports whether the given screen-relative y, x coordinates
   fall within the given window.

   wmouse_trafo() converts between screen-relative and window-relative
   coordinates. A to_screen parameter of TRUE means to convert from
   window to screen; otherwise the reverse. The function returns FALSE
   if the coordinates aren't within the window, or if any of the
   parameters are NULL. The coordinates have been converted when the
   function returns TRUE.

   mouse_trafo() is the stdscr version of wmouse_trafo().

   mousemask() is nearly equivalent to mouse_set(), but instead of
   OK/ERR, it returns the value of the mask after setting it. (This
   isn't necessarily the same value passed in, since the mask could be
   altered on some platforms.) And if the second parameter is a non-null
   pointer, mousemask() stores the previous mask value there. Also,
   since the ncurses interface doesn't work with PDCurses' BUTTON_MOVED
   events, mousemask() filters them out.

   nc_getmouse() returns the current mouse status in an MEVENT struct.
   This is equivalent to ncurses' getmouse(), renamed to avoid conflict
   with PDCurses' getmouse(). But if you define PDC_NCMOUSE before
   including curses.h, it defines getmouse() to nc_getmouse(), along
   with a few other redefintions needed for compatibility with ncurses
   code. nc_getmouse() calls request_mouse_pos(), which (not getmouse())
   is the classic equivalent.

   ungetmouse() is the mouse equivalent of ungetch(). However, PDCurses
   doesn't maintain a queue of mouse events; only one can be pushed
   back, and it can overwrite or be overwritten by real mouse events.

   has_mouse() reports whether the mouse is available at all on the
   current platform.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   mouse_set             |    -   |    -    |   -
   mouse_on              |    -   |    -    |   -
   mouse_off             |    -   |    -    |   -
   request_mouse_pos     |    -   |    -    |   -
   wmouse_position       |    -   |    -    |   -
   getmouse              |    -   |    *    |   -
   mouseinterval         |    -   |    Y    |   -
   wenclose              |    -   |    Y    |   -
   wmouse_trafo          |    -   |    Y    |   -
   mouse_trafo           |    -   |    Y    |   -
   mousemask             |    -   |    Y    |   -
   nc_getmouse           |    -   |    *    |   -
   ungetmouse            |    -   |    Y    |   -
   has_mouse             |    -   |    Y    |   -

   \* See above, under Description

**man-end****************************************************************/

static bool ungot = FALSE;

int mouse_set(mmask_t mbe)
{
    PDC_LOG(("mouse_set() - called: event %x\n", mbe));

    if (!SP)
        return ERR;

    SP->_trap_mbe = mbe;
    return PDC_mouse_set();
}

int mouse_on(mmask_t mbe)
{
    PDC_LOG(("mouse_on() - called: event %x\n", mbe));

    if (!SP)
        return ERR;

    SP->_trap_mbe |= mbe;
    return PDC_mouse_set();
}

int mouse_off(mmask_t mbe)
{
    PDC_LOG(("mouse_off() - called: event %x\n", mbe));

    if (!SP)
        return ERR;

    SP->_trap_mbe &= ~mbe;
    return PDC_mouse_set();
}

int request_mouse_pos(void)
{
    PDC_LOG(("request_mouse_pos() - called\n"));

    Mouse_status = SP->mouse_status;

    return OK;
}

void wmouse_position(WINDOW *win, int *y, int *x)
{
    PDC_LOG(("wmouse_position() - called\n"));

    if (win && wenclose(win, MOUSE_Y_POS, MOUSE_X_POS))
    {
        if (y)
            *y = MOUSE_Y_POS - win->_begy;
        if (x)
            *x = MOUSE_X_POS - win->_begx;
    }
    else
    {
        if (y)
            *y = -1;
        if (x)
            *x = -1;
    }
}

#ifndef PDC_NCMOUSE
mmask_t getmouse(void)
{
    PDC_LOG(("getmouse() - called\n"));

    return SP ? SP->_trap_mbe : (mmask_t)0;
}
#endif

/* ncurses mouse interface */

int mouseinterval(int wait)
{
    int old_wait;

    PDC_LOG(("mouseinterval() - called: %d\n", wait));

    if (!SP)
        return ERR;

    old_wait = SP->mouse_wait;

    if (wait >= 0 && wait <= 1000)
        SP->mouse_wait = wait;

    return old_wait;
}

bool wenclose(const WINDOW *win, int y, int x)
{
    PDC_LOG(("wenclose() - called: %p %d %d\n", win, y, x));

    return (win && y >= win->_begy && y < win->_begy + win->_maxy
                && x >= win->_begx && x < win->_begx + win->_maxx);
}

bool wmouse_trafo(const WINDOW *win, int *y, int *x, bool to_screen)
{
    int newy, newx;

    PDC_LOG(("wmouse_trafo() - called\n"));

    if (!win || !y || !x)
        return FALSE;

    newy = *y;
    newx = *x;

    if (to_screen)
    {
        newy += win->_begy;
        newx += win->_begx;

        if (!wenclose(win, newy, newx))
            return FALSE;
    }
    else
    {
        if (wenclose(win, newy, newx))
        {
            newy -= win->_begy;
            newx -= win->_begx;
        }
        else
            return FALSE;
    }

    *y = newy;
    *x = newx;

    return TRUE;
}

bool mouse_trafo(int *y, int *x, bool to_screen)
{
    PDC_LOG(("mouse_trafo() - called\n"));

    return wmouse_trafo(stdscr, y, x, to_screen);
}

mmask_t mousemask(mmask_t mask, mmask_t *oldmask)
{
    PDC_LOG(("mousemask() - called\n"));

    if (!SP)
        return (mmask_t)0;

    if (oldmask)
        *oldmask = SP->_trap_mbe;

    /* The ncurses interface doesn't work with our move events, so
       filter them here */

    mask &= ~(BUTTON1_MOVED | BUTTON2_MOVED | BUTTON3_MOVED);

    mouse_set(mask);

    return SP->_trap_mbe;
}

int nc_getmouse(MEVENT *event)
{
    int i;
    mmask_t bstate = 0;

    PDC_LOG(("nc_getmouse() - called\n"));

    if (!event || !SP)
        return ERR;

    ungot = FALSE;

    request_mouse_pos();

    event->id = 0;

    event->x = Mouse_status.x;
    event->y = Mouse_status.y;
    event->z = 0;

    for (i = 0; i < 3; i++)
    {
        if (Mouse_status.changes & (1 << i))
        {
            int shf = i * 5;
            short button = Mouse_status.button[i] & BUTTON_ACTION_MASK;

            if (button == BUTTON_RELEASED)
                bstate |= (BUTTON1_RELEASED << shf);
            else if (button == BUTTON_PRESSED)
                bstate |= (BUTTON1_PRESSED << shf);
            else if (button == BUTTON_CLICKED)
                bstate |= (BUTTON1_CLICKED << shf);
            else if (button == BUTTON_DOUBLE_CLICKED)
                bstate |= (BUTTON1_DOUBLE_CLICKED << shf);

            button = Mouse_status.button[i] & BUTTON_MODIFIER_MASK;

            if (button & PDC_BUTTON_SHIFT)
                bstate |= BUTTON_MODIFIER_SHIFT;
            if (button & PDC_BUTTON_CONTROL)
                bstate |= BUTTON_MODIFIER_CONTROL;
            if (button & PDC_BUTTON_ALT)
                bstate |= BUTTON_MODIFIER_ALT;
        }
    }

    if (MOUSE_WHEEL_UP)
        bstate |= BUTTON4_PRESSED;
    else if (MOUSE_WHEEL_DOWN)
        bstate |= BUTTON5_PRESSED;

    /* extra filter pass -- mainly for button modifiers */

    event->bstate = bstate & SP->_trap_mbe;

    return OK;
}

int ungetmouse(MEVENT *event)
{
    int i;
    mmask_t bstate;

    PDC_LOG(("ungetmouse() - called\n"));

    if (!event || ungot)
        return ERR;

    ungot = TRUE;

    SP->mouse_status.x = event->x;
    SP->mouse_status.y = event->y;

    SP->mouse_status.changes = 0;
    bstate = event->bstate;

    for (i = 0; i < 3; i++)
    {
        int shf = i * 5;
        short button = 0;

        if (bstate & ((BUTTON1_RELEASED | BUTTON1_PRESSED |
            BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED) << shf))
        {
            SP->mouse_status.changes |= 1 << i;

            if (bstate & (BUTTON1_PRESSED << shf))
                button = BUTTON_PRESSED;
            if (bstate & (BUTTON1_CLICKED << shf))
                button = BUTTON_CLICKED;
            if (bstate & (BUTTON1_DOUBLE_CLICKED << shf))
                button = BUTTON_DOUBLE_CLICKED;

            if (bstate & BUTTON_MODIFIER_SHIFT)
                button |= PDC_BUTTON_SHIFT;
            if (bstate & BUTTON_MODIFIER_CONTROL)
                button |= PDC_BUTTON_CONTROL;
            if (bstate & BUTTON_MODIFIER_ALT)
                button |= PDC_BUTTON_ALT;
        }

        SP->mouse_status.button[i] = button;
    }

    if (bstate & BUTTON4_PRESSED)
        SP->mouse_status.changes |= PDC_MOUSE_WHEEL_UP;
    else if (bstate & BUTTON5_PRESSED)
        SP->mouse_status.changes |= PDC_MOUSE_WHEEL_DOWN;

    return PDC_ungetch(KEY_MOUSE);
}

bool has_mouse(void)
{
    return PDC_has_mouse();
}
/* PDCurses */

/*man-start**************************************************************

move
----

### Synopsis

    int move(int y, int x);
    int mvcur(int oldrow, int oldcol, int newrow, int newcol);
    int wmove(WINDOW *win, int y, int x);

### Description

   move() and wmove() move the cursor associated with the window to the
   given location. This does not move the physical cursor of the
   terminal until refresh() is called. The position specified is
   relative to the upper left corner of the window, which is (0,0).

   mvcur() moves the physical cursor without updating any window cursor
   positions.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   move                  |    Y   |    Y    |   Y
   mvcur                 |    Y   |    Y    |   Y
   wmove                 |    Y   |    Y    |   Y

**man-end****************************************************************/

int move(int y, int x)
{
    PDC_LOG(("move() - called: y=%d x=%d\n", y, x));

    if (!stdscr || x < 0 || y < 0 || x >= stdscr->_maxx || y >= stdscr->_maxy)
        return ERR;

    stdscr->_curx = x;
    stdscr->_cury = y;

    return OK;
}

int mvcur(int oldrow, int oldcol, int newrow, int newcol)
{
    PDC_LOG(("mvcur() - called: oldrow %d oldcol %d newrow %d newcol %d\n",
             oldrow, oldcol, newrow, newcol));

    if (!SP || newrow < 0 || newrow >= LINES || newcol < 0 || newcol >= COLS)
        return ERR;

    PDC_gotoyx(newrow, newcol);
    SP->cursrow = newrow;
    SP->curscol = newcol;

    return OK;
}

int wmove(WINDOW *win, int y, int x)
{
    PDC_LOG(("wmove() - called: y=%d x=%d\n", y, x));

    if (!win || x < 0 || y < 0 || x >= win->_maxx || y >= win->_maxy)
        return ERR;

    win->_curx = x;
    win->_cury = y;

    return OK;
}
/* PDCurses */

/*man-start**************************************************************

outopts
-------

### Synopsis

    int clearok(WINDOW *win, bool bf);
    int idlok(WINDOW *win, bool bf);
    void idcok(WINDOW *win, bool bf);
    void immedok(WINDOW *win, bool bf);
    int leaveok(WINDOW *win, bool bf);
    int setscrreg(int top, int bot);
    int wsetscrreg(WINDOW *win, int top, int bot);
    int wgetscrreg(const WINDOW *win, int *top, int *bot);
    int scrollok(WINDOW *win, bool bf);

    int raw_output(bool bf);

    bool is_cleared(const WINDOW *win);
    bool is_idlok(const WINDOW *win);
    bool is_idcok(const WINDOW *win);
    bool is_immedok(const WINDOW *win);
    bool is_leaveok(const WINDOW *win);
    bool is_scrollok(const WINDOW *win);

### Description

   With clearok(), if bf is TRUE, the next call to wrefresh() with this
   window will clear the screen completely and redraw the entire screen.

   immedok(), called with a second argument of TRUE, causes an automatic
   wrefresh() every time a change is made to the specified window.

   Normally, the hardware cursor is left at the location of the window
   being refreshed. leaveok() allows the cursor to be left wherever the
   update happens to leave it. It's useful for applications where the
   cursor is not used, since it reduces the need for cursor motions. If
   possible, the cursor is made invisible when this option is enabled.

   wsetscrreg() sets a scrolling region in a window; "top" and "bot" are
   the line numbers for the top and bottom margins. If this option and
   scrollok() are enabled, any attempt to move off the bottom margin
   will cause all lines in the scrolling region to scroll up one line.
   setscrreg() is the stdscr version.

   wgetscrreg() gets the top and bottom margins as set in wsetscrreg().

   idlok() and idcok() do nothing in PDCurses, but are provided for
   compatibility with other curses implementations, likewise is_idlok()
   and is_idcok().

   raw_output() enables the output of raw characters using the standard
   *add* and *ins* curses functions (that is, it disables translation of
   control characters).

   is_cleared() reports whether the specified window causes clear at next
   refresh.

   is_immedok() reports whether the specified window is in immedok mode.

   is_leaveok() reports whether the specified window is in leaveok mode.

   is_scrollok() reports whether the specified window allows scrolling.

### Return Value

   is_cleared(), is_immedok(), is_leaveok() and is_scrollok() return TRUE
   or FALSE. is_idlok() and is_idcok() are provided for compatibility with
   other curses implementations, and always return FALSE. All others
   return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   clearok               |    Y   |    Y    |   Y
   idlok                 |    Y   |    Y    |   Y
   idcok                 |    Y   |    Y    |   Y
   immedok               |    Y   |    Y    |   Y
   leaveok               |    Y   |    Y    |   Y
   setscrreg             |    Y   |    Y    |   Y
   wsetscrreg            |    Y   |    Y    |   Y
   wgetscrreg            |    -   |    Y    |   -
   scrollok              |    Y   |    Y    |   Y
   is_cleared            |    -   |    Y    |   -
   is_idlok              |    -   |    Y    |   -
   is_idcok              |    -   |    Y    |   -
   is_immedok            |    -   |    Y    |   -
   is_leaveok            |    -   |    Y    |   Y
   is_scrollok           |    -   |    Y    |   -
   raw_output            |    -   |    -    |   -

**man-end****************************************************************/

int clearok(WINDOW *win, bool bf)
{
    PDC_LOG(("clearok() - called\n"));

    if (!win)
        return ERR;

    win->_clear = bf;

    return OK;
}

int idlok(WINDOW *win, bool bf)
{
    PDC_LOG(("idlok() - called\n"));

    return OK;
}

void idcok(WINDOW *win, bool bf)
{
    PDC_LOG(("idcok() - called\n"));
}

void immedok(WINDOW *win, bool bf)
{
    PDC_LOG(("immedok() - called\n"));

    if (win)
        win->_immed = bf;
}

int leaveok(WINDOW *win, bool bf)
{
    PDC_LOG(("leaveok() - called\n"));

    if (!win)
        return ERR;

    win->_leaveit = bf;

    curs_set(!bf);

    return OK;
}

int setscrreg(int top, int bottom)
{
    PDC_LOG(("setscrreg() - called: top %d bottom %d\n", top, bottom));

    return wsetscrreg(stdscr, top, bottom);
}

int wsetscrreg(WINDOW *win, int top, int bottom)
{
    PDC_LOG(("wsetscrreg() - called: top %d bottom %d\n", top, bottom));

    if (win && 0 <= top && top <= win->_cury &&
        win->_cury <= bottom && bottom < win->_maxy)
    {
        win->_tmarg = top;
        win->_bmarg = bottom;

        return OK;
    }
    else
        return ERR;
}

int wgetscrreg(const WINDOW *win, int *top, int *bot)
{
    PDC_LOG(("wgetscrreg() - called\n"));

    if (!win || !top || !bot)
        return ERR;

    *top = win->_tmarg;
    *bot = win->_bmarg;

    return OK;
}

int scrollok(WINDOW *win, bool bf)
{
    PDC_LOG(("scrollok() - called\n"));

    if (!win)
        return ERR;

    win->_scroll = bf;

    return OK;
}

int raw_output(bool bf)
{
    PDC_LOG(("raw_output() - called\n"));

    if (!SP)
        return ERR;

    SP->raw_out = bf;

    return OK;
}

bool is_cleared(const WINDOW *win)
{
    PDC_LOG(("is_cleared() - called\n"));

    if (!win)
        return FALSE;

    return win->_clear;
}

bool is_idlok(const WINDOW *win)
{
    (void) win;

    PDC_LOG(("is_idlok() - called\n"));

    return FALSE;
}

bool is_idcok(const WINDOW *win)
{
    (void) win;

    PDC_LOG(("is_idcok() - called\n"));

    return FALSE;
}

bool is_immedok(const WINDOW *win)
{
    PDC_LOG(("is_immedok() - called\n"));

    if (!win)
        return FALSE;

    return win->_immed;
}

bool is_leaveok(const WINDOW *win)
{
    PDC_LOG(("is_leaveok() - called\n"));

    if (!win)
        return FALSE;

    return win->_leaveit;
}

bool is_scrollok(const WINDOW *win)
{
    PDC_LOG(("is_scrollok() - called\n"));

    if (!win)
        return FALSE;

    return win->_scroll;
}
/* PDCurses */

/*man-start**************************************************************

overlay
-------

### Synopsis

    int overlay(const WINDOW *src_w, WINDOW *dst_w)
    int overwrite(const WINDOW *src_w, WINDOW *dst_w)
    int copywin(const WINDOW *src_w, WINDOW *dst_w, int src_tr,
                int src_tc, int dst_tr, int dst_tc, int dst_br,
                int dst_bc, int _overlay)

### Description

   overlay() and overwrite() copy all the text from src_w into dst_w.
   The windows need not be the same size. Those characters in the source
   window that intersect with the destination window are copied, so that
   the characters appear in the same physical position on the screen.
   The difference between the two functions is that overlay() is non-
   destructive (blanks are not copied) while overwrite() is destructive
   (blanks are copied).

   copywin() is similar, but doesn't require that the two windows
   overlap. The arguments src_tc and src_tr specify the top left corner
   of the region to be copied. dst_tc, dst_tr, dst_br, and dst_bc
   specify the region within the destination window to copy to. The
   argument "overlay", if TRUE, indicates that the copy is done non-
   destructively (as in overlay()); blanks in the source window are not
   copied to the destination window. When overlay is FALSE, blanks are
   copied.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   overlay               |    Y   |    Y    |   Y
   overwrite             |    Y   |    Y    |   Y
   copywin               |    Y   |    Y    |   Y

**man-end****************************************************************/

/* Thanks to Andreas Otte <venn@@uni-paderborn.de> for the
   corrected overlay()/overwrite() behavior. */

static int _copy_win(const WINDOW *src_w, WINDOW *dst_w, int src_tr,
                     int src_tc, int src_br, int src_bc, int dst_tr,
                     int dst_tc, bool _overlay)
{
    int col, line, y1, fc, *minchng, *maxchng;
    chtype *w1ptr, *w2ptr;

    int lc = 0;
    int xdiff = src_bc - src_tc;
    int ydiff = src_br - src_tr;

    if (!src_w || !dst_w)
        return ERR;

    minchng = dst_w->_firstch;
    maxchng = dst_w->_lastch;

    for (y1 = 0; y1 < dst_tr; y1++)
    {
        minchng++;
        maxchng++;
    }

    for (line = 0; line < ydiff; line++)
    {
        w1ptr = src_w->_y[line + src_tr] + src_tc;
        w2ptr = dst_w->_y[line + dst_tr] + dst_tc;

        fc = _NO_CHANGE;

        for (col = 0; col < xdiff; col++)
        {
            if ((*w1ptr) != (*w2ptr) &&
                !((*w1ptr & A_CHARTEXT) == ' ' && _overlay))
            {
                *w2ptr = *w1ptr;

                if (fc == _NO_CHANGE)
                    fc = col + dst_tc;

                lc = col + dst_tc;
            }

            w1ptr++;
            w2ptr++;
        }

        if (*minchng == _NO_CHANGE)
        {
            *minchng = fc;
            *maxchng = lc;
        }
        else if (fc != _NO_CHANGE)
        {
            if (fc < *minchng)
                *minchng = fc;
            if (lc > *maxchng)
                *maxchng = lc;
        }

        minchng++;
        maxchng++;
    }

    return OK;
}

int _copy_overlap(const WINDOW *src_w, WINDOW *dst_w, bool overlay)
{
    int first_line, first_col, last_line, last_col;
    int src_start_x, src_start_y, dst_start_x, dst_start_y;
    int xdiff, ydiff;

    if (!src_w || !dst_w)
        return ERR;

    first_col = max(dst_w->_begx, src_w->_begx);
    first_line = max(dst_w->_begy, src_w->_begy);

    last_col = min(src_w->_begx + src_w->_maxx, dst_w->_begx + dst_w->_maxx);
    last_line = min(src_w->_begy + src_w->_maxy, dst_w->_begy + dst_w->_maxy);

    /* determine the overlapping region of the two windows in real
       coordinates */

    /* if no overlapping region, do nothing */

    if ((last_col < first_col) || (last_line < first_line))
        return OK;

    /* size of overlapping region */

    xdiff = last_col - first_col;
    ydiff = last_line - first_line;

    if (src_w->_begx <= dst_w->_begx)
    {
        src_start_x = dst_w->_begx - src_w->_begx;
        dst_start_x = 0;
    }
    else
    {
        dst_start_x = src_w->_begx - dst_w->_begx;
        src_start_x = 0;
    }

    if (src_w->_begy <= dst_w->_begy)
    {
        src_start_y = dst_w->_begy - src_w->_begy;
        dst_start_y = 0;
    }
    else
    {
        dst_start_y = src_w->_begy - dst_w->_begy;
        src_start_y = 0;
    }

    return _copy_win(src_w, dst_w, src_start_y, src_start_x,
                     src_start_y + ydiff, src_start_x + xdiff,
                     dst_start_y, dst_start_x, overlay);
}

int overlay(const WINDOW *src_w, WINDOW *dst_w)
{
    PDC_LOG(("overlay() - called\n"));

    return _copy_overlap(src_w, dst_w, TRUE);
}

int overwrite(const WINDOW *src_w, WINDOW *dst_w)
{
    PDC_LOG(("overwrite() - called\n"));

    return _copy_overlap(src_w, dst_w, FALSE);
}

int copywin(const WINDOW *src_w, WINDOW *dst_w, int src_tr, int src_tc,
            int dst_tr, int dst_tc, int dst_br, int dst_bc, int _overlay)
{
    int src_end_x, src_end_y;
    int src_rows, src_cols, dst_rows, dst_cols;
    int min_rows, min_cols;

    PDC_LOG(("copywin() - called\n"));

    if (!src_w || !dst_w || dst_w == curscr || dst_br >= dst_w->_maxy
        || dst_bc >= dst_w->_maxx || dst_tr < 0 || dst_tc < 0)
        return ERR;

    src_rows = src_w->_maxy - src_tr;
    src_cols = src_w->_maxx - src_tc;
    dst_rows = dst_br - dst_tr + 1;
    dst_cols = dst_bc - dst_tc + 1;

    min_rows = min(src_rows, dst_rows);
    min_cols = min(src_cols, dst_cols);

    src_end_y = src_tr + min_rows;
    src_end_x = src_tc + min_cols;

    return _copy_win(src_w, dst_w, src_tr, src_tc, src_end_y, src_end_x,
                     dst_tr, dst_tc, _overlay);
}
/* PDCurses */

/*man-start**************************************************************

pad
---

### Synopsis

    WINDOW *newpad(int nlines, int ncols);
    WINDOW *subpad(WINDOW *orig, int nlines, int ncols,
                   int begy, int begx);
    int prefresh(WINDOW *win, int py, int px, int sy1, int sx1,
                 int sy2, int sx2);
    int pnoutrefresh(WINDOW *w, int py, int px, int sy1, int sx1,
                     int sy2, int sx2);
    int pechochar(WINDOW *pad, chtype ch);
    int pecho_wchar(WINDOW *pad, const cchar_t *wch);

    bool is_pad(const WINDOW *pad);

### Description

   A pad is a special kind of window, which is not restricted by the
   screen size, and is not necessarily associated with a particular part
   of the screen. You can use a pad when you need a large window, and
   only a part of the window will be on the screen at one time. Pads are
   not refreshed automatically (e.g., from scrolling or echoing of
   input). You can't call wrefresh() with a pad as an argument; use
   prefresh() or pnoutrefresh() instead. Note that these routines
   require additional parameters to specify the part of the pad to be
   displayed, and the location to use on the screen.

   newpad() creates a new pad data structure.

   subpad() creates a new sub-pad within a pad, at position (begy,
   begx), with dimensions of nlines lines and ncols columns. This
   position is relative to the pad, and not to the screen as with
   subwin. Changes to either the parent pad or sub-pad will affect both.
   When using sub-pads, you may need to call touchwin() before calling
   prefresh().

   pnoutrefresh() copies the specified pad to the virtual screen.

   prefresh() calls pnoutrefresh(), followed by doupdate().

   These routines are analogous to wnoutrefresh() and wrefresh(). (py,
   px) specifies the upper left corner of the part of the pad to be
   displayed; (sy1, sx1) and (sy2, sx2) describe the screen rectangle
   that will contain the selected part of the pad.

   pechochar() is functionally equivalent to addch() followed by a call
   to prefresh(), with the last-used coordinates and dimensions.
   pecho_wchar() is the wide-character version.

   is_pad() reports whether the specified window is a pad.

### Return Value

   All functions except is_pad() return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   newpad                |    Y   |    Y    |   Y
   subpad                |    Y   |    Y    |   Y
   prefresh              |    Y   |    Y    |   Y
   pnoutrefresh          |    Y   |    Y    |   Y
   pechochar             |    Y   |    Y    |   Y
   pecho_wchar           |    Y   |    Y    |   Y
   is_pad                |    -   |    Y    |   Y

**man-end****************************************************************/

WINDOW *newpad(int nlines, int ncols)
{
    WINDOW *win;

    PDC_LOG(("newpad() - called: lines=%d cols=%d\n", nlines, ncols));

    win = PDC_makenew(nlines, ncols, 0, 0);
    if (win)
        win = PDC_makelines(win);

    if (!win)
        return (WINDOW *)NULL;

    werase(win);

    win->_flags = _PAD;
    win->_pad._pad_y = 0;
    win->_pad._pad_x = 0;
    win->_pad._pad_top = 0;
    win->_pad._pad_left = 0;
    win->_pad._pad_bottom = min(LINES, nlines) - 1;
    win->_pad._pad_right = min(COLS, ncols) - 1;

    return win;
}

WINDOW *subpad(WINDOW *orig, int nlines, int ncols, int begy, int begx)
{
    WINDOW *win;
    int i;

    PDC_LOG(("subpad() - called: lines=%d cols=%d begy=%d begx=%d\n",
             nlines, ncols, begy, begx));

    if (!orig || !(orig->_flags & _PAD))
        return (WINDOW *)NULL;

    /* make sure window fits inside the original one */

    if (begy < 0 || begx < 0 ||
        (begy + nlines) > orig->_maxy ||
        (begx + ncols)  > orig->_maxx)
        return (WINDOW *)NULL;

    if (!nlines)
        nlines = orig->_maxy - begy;

    if (!ncols)
        ncols = orig->_maxx - begx;

    win = PDC_makenew(nlines, ncols, begy, begx);
    if (!win)
        return (WINDOW *)NULL;

    /* initialize window variables */

    win->_attrs = orig->_attrs;
    win->_leaveit = orig->_leaveit;
    win->_scroll = orig->_scroll;
    win->_nodelay = orig->_nodelay;
    win->_use_keypad = orig->_use_keypad;
    win->_parent = orig;

    for (i = 0; i < nlines; i++)
        win->_y[i] = orig->_y[begy + i] + begx;

    win->_flags = _SUBPAD;
    win->_pad._pad_y = 0;
    win->_pad._pad_x = 0;
    win->_pad._pad_top = 0;
    win->_pad._pad_left = 0;
    win->_pad._pad_bottom = min(LINES, nlines) - 1;
    win->_pad._pad_right = min(COLS, ncols) - 1;

    return win;
}

int prefresh(WINDOW *win, int py, int px, int sy1, int sx1, int sy2, int sx2)
{
    PDC_LOG(("prefresh() - called\n"));

    if (pnoutrefresh(win, py, px, sy1, sx1, sy2, sx2) == ERR)
        return ERR;

    doupdate();
    return OK;
}

int pnoutrefresh(WINDOW *w, int py, int px, int sy1, int sx1, int sy2, int sx2)
{
    int num_cols;
    int sline;
    int pline;

    PDC_LOG(("pnoutrefresh() - called\n"));

    if (py < 0)
        py = 0;
    if (px < 0)
        px = 0;
    if (sy1 < 0)
        sy1 = 0;
    if (sx1 < 0)
        sx1 = 0;

    if ((!w || !(w->_flags & (_PAD|_SUBPAD)) ||
        (sy2 >= LINES) || (sx2 >= COLS)) ||
        (sy2 < sy1) || (sx2 < sx1))
        return ERR;

    sline = sy1;
    pline = py;

    num_cols = min((sx2 - sx1 + 1), (w->_maxx - px));

    while (sline <= sy2)
    {
        if (pline < w->_maxy)
        {
            memcpy(curscr->_y[sline] + sx1, w->_y[pline] + px,
                   num_cols * sizeof(chtype));

            if ((curscr->_firstch[sline] == _NO_CHANGE)
                || (curscr->_firstch[sline] > sx1))
                curscr->_firstch[sline] = sx1;

            if (sx2 > curscr->_lastch[sline])
                curscr->_lastch[sline] = sx2;

            w->_firstch[pline] = _NO_CHANGE; /* updated now */
            w->_lastch[pline] = _NO_CHANGE;  /* updated now */
        }

        sline++;
        pline++;
    }

    if (w->_clear)
    {
        w->_clear = FALSE;
        curscr->_clear = TRUE;
    }

    /* position the cursor to the pad's current position if possible --
       is the pad current position going to end up displayed? if not,
       then don't move the cursor; if so, move it to the correct place */

    if (!w->_leaveit && w->_cury >= py && w->_curx >= px &&
         w->_cury <= py + (sy2 - sy1) && w->_curx <= px + (sx2 - sx1))
    {
        curscr->_cury = (w->_cury - py) + sy1;
        curscr->_curx = (w->_curx - px) + sx1;
    }

    w->_pad._pad_y = py;
    w->_pad._pad_x = px;
    w->_pad._pad_top = sy1;
    w->_pad._pad_left = sx1;
    w->_pad._pad_bottom = sy2;
    w->_pad._pad_right = sx2;

    return OK;
}

int pechochar(WINDOW *pad, chtype ch)
{
    PDC_LOG(("pechochar() - called\n"));

    if (waddch(pad, ch) == ERR)
        return ERR;

    return prefresh(pad, pad->_pad._pad_y, pad->_pad._pad_x, pad->_pad._pad_top,
                    pad->_pad._pad_left, pad->_pad._pad_bottom, pad->_pad._pad_right);
}

#ifdef PDC_WIDE
int pecho_wchar(WINDOW *pad, const cchar_t *wch)
{
    PDC_LOG(("pecho_wchar() - called\n"));

    if (!wch || (waddch(pad, *wch) == ERR))
        return ERR;

    return prefresh(pad, pad->_pad._pad_y, pad->_pad._pad_x, pad->_pad._pad_top,
                    pad->_pad._pad_left, pad->_pad._pad_bottom, pad->_pad._pad_right);
}
#endif

bool is_pad(const WINDOW *pad)
{
    PDC_LOG(("is_pad() - called\n"));

    if (!pad)
        return FALSE;

    return (pad->_flags & _PAD) ? TRUE : FALSE;
}
/* PDCurses */

/*man-start**************************************************************

panel
-----

### Synopsis

    int bottom_panel(PANEL *pan);
    int del_panel(PANEL *pan);
    int hide_panel(PANEL *pan);
    int move_panel(PANEL *pan, int starty, int startx);
    PANEL *new_panel(WINDOW *win);
    PANEL *panel_above(const PANEL *pan);
    PANEL *panel_below(const PANEL *pan);
    int panel_hidden(const PANEL *pan);
    const void *panel_userptr(const PANEL *pan);
    WINDOW *panel_window(const PANEL *pan);
    int replace_panel(PANEL *pan, WINDOW *win);
    int set_panel_userptr(PANEL *pan, const void *uptr);
    int show_panel(PANEL *pan);
    int top_panel(PANEL *pan);
    void update_panels(void);

### Description

   For historic reasons, and for compatibility with other versions of
   curses, the panel functions are prototyped in a separate header,
   panel.h. In many implementations, they're also in a separate library,
   but PDCurses incorporates them.

   The panel functions provide a way to have depth relationships between
   curses windows. Panels can overlap without making visible the
   overlapped portions of underlying windows. The initial curses window,
   stdscr, lies beneath all panels. The set of currently visible panels
   is the 'deck' of panels.

   You can create panels, fetch and set their associated windows,
   shuffle panels in the deck, and manipulate them in other ways.

   bottom_panel() places pan at the bottom of the deck. The size,
   location and contents of the panel are unchanged.

   del_panel() deletes pan, but not its associated winwow.

   hide_panel() removes a panel from the deck and thus hides it from
   view.

   move_panel() moves the curses window associated with pan, so that its
   upper lefthand corner is at the supplied coordinates. (Don't use
   mvwin() on the window.)

   new_panel() creates a new panel associated with win and returns the
   panel pointer. The new panel is placed at the top of the deck.

   panel_above() returns a pointer to the panel in the deck above pan,
   or NULL if pan is the top panel. If the value of pan passed is NULL,
   this function returns a pointer to the bottom panel in the deck.

   panel_below() returns a pointer to the panel in the deck below pan,
   or NULL if pan is the bottom panel. If the value of pan passed is
   NULL, this function returns a pointer to the top panel in the deck.

   panel_hidden() returns OK if pan is hidden and ERR if it is not.

   panel_userptr() - Each panel has a user pointer available for
   maintaining relevant information. This function returns a pointer to
   that information previously set up by set_panel_userptr().

   panel_window() returns a pointer to the curses window associated with
   the panel.

   replace_panel() replaces the current window of pan with win.

   set_panel_userptr() - Each panel has a user pointer available for
   maintaining relevant information. This function sets the value of
   that information.

   show_panel() makes a previously hidden panel visible and places it
   back in the deck on top.

   top_panel() places pan on the top of the deck. The size, location and
   contents of the panel are unchanged.

   update_panels() refreshes the virtual screen to reflect the depth
   relationships between the panels in the deck. The user must use
   doupdate() to refresh the physical screen.

### Return Value

   Each routine that returns a pointer to an object returns NULL if an
   error occurs. Each panel routine that returns an integer, returns OK
   if it executes successfully and ERR if it does not.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   bottom_panel          |    -   |    Y    |   Y
   del_panel             |    -   |    Y    |   Y
   hide_panel            |    -   |    Y    |   Y
   move_panel            |    -   |    Y    |   Y
   new_panel             |    -   |    Y    |   Y
   panel_above           |    -   |    Y    |   Y
   panel_below           |    -   |    Y    |   Y
   panel_hidden          |    -   |    Y    |   Y
   panel_userptr         |    -   |    Y    |   Y
   panel_window          |    -   |    Y    |   Y
   replace_panel         |    -   |    Y    |   Y
   set_panel_userptr     |    -   |    Y    |   Y
   show_panel            |    -   |    Y    |   Y
   top_panel             |    -   |    Y    |   Y
   update_panels         |    -   |    Y    |   Y

### Credits

   Original Author - Warren Tucker <wht@n4hgf.mt-park.ga.us>

**man-end****************************************************************/

PANEL *_bottom_panel = (PANEL *)0;
PANEL *_top_panel = (PANEL *)0;
PANEL _stdscr_pseudo_panel = { (WINDOW *)0 };

#ifdef PANEL_DEBUG

static void dPanel(char *text, PANEL *pan)
{
    PDC_LOG(("%s id=%s b=%s a=%s y=%d x=%d", text, pan->user,
             pan->below ? pan->below->user : "--",
             pan->above ? pan->above->user : "--",
             pan->wstarty, pan->wstartx));
}

static void dStack(char *fmt, int num, PANEL *pan)
{
    char s80[80];

    sprintf(s80, fmt, num, pan);
    PDC_LOG(("%s b=%s t=%s", s80, _bottom_panel ? _bottom_panel->user : "--",
             _top_panel    ? _top_panel->user    : "--"));

    if (pan)
        PDC_LOG(("pan id=%s", pan->user));

    pan = _bottom_panel;

    while (pan)
    {
        dPanel("stk", pan);
        pan = pan->above;
    }
}

/* debugging hook for wnoutrefresh */

static void Wnoutrefresh(PANEL *pan)
{
    dPanel("wnoutrefresh", pan);
    wnoutrefresh(pan->win);
}

static void Touchpan(PANEL *pan)
{
    dPanel("Touchpan", pan);
    touchwin(pan->win);
}

static void Touchline(PANEL *pan, int start, int count)
{
    char s80[80];

    sprintf(s80, "Touchline s=%d c=%d", start, count);
    dPanel(s80, pan);
    touchline(pan->win, start, count);
}

#else   /* PANEL_DEBUG */

#define dPanel(text, pan)
#define dStack(fmt, num, pan)
#define Wnoutrefresh(pan) wnoutrefresh((pan)->win)
#define Touchpan(pan) touchwin((pan)->win)
#define Touchline(pan, start, count) touchline((pan)->win, start, count)

#endif  /* PANEL_DEBUG */

static bool _panels_overlapped(PANEL *pan1, PANEL *pan2)
{
    if (!pan1 || !pan2)
        return FALSE;

    return ((pan1->wstarty >= pan2->wstarty && pan1->wstarty < pan2->wendy)
         || (pan2->wstarty >= pan1->wstarty && pan2->wstarty < pan1->wendy))
        && ((pan1->wstartx >= pan2->wstartx && pan1->wstartx < pan2->wendx)
         || (pan2->wstartx >= pan1->wstartx && pan2->wstartx < pan1->wendx));
}

static void _free_obscure(PANEL *pan)
{
    PANELOBS *tobs = pan->obscure;  /* "this" one */
    PANELOBS *nobs;                 /* "next" one */

    while (tobs)
    {
        nobs = tobs->above;
        free((char *)tobs);
        tobs = nobs;
    }
    pan->obscure = (PANELOBS *)0;
}

static void _pairwise_override(PANEL *pan, PANEL *pan2)
{
    const int sy1 = pan->wstarty, sy2 = pan2->wstarty;
    const int end_y = min(pan->wendy, pan2->wendy);
    int y = max(sy1, sy2);

    while (y < end_y)
    {
        if (is_linetouched(pan->win, y - sy1))
            Touchline(pan2, y - sy2, 1);
        y++;
    }
}

static void _override(PANEL *pan, int show)
{
    PANEL *pan2;
    PANELOBS *tobs = pan->obscure;      /* "this" one */

    if (show == 1)
        Touchpan(pan);
    else if (!show)
    {
        Touchpan(pan);
        Touchpan(&_stdscr_pseudo_panel);
    }
    else if (show == -1)
        while (tobs && (tobs->pan != pan))
            tobs = tobs->above;

    while (tobs)
    {
        if ((pan2 = tobs->pan) != pan)
            _pairwise_override(pan, pan2);

        tobs = tobs->above;
    }
    _pairwise_override(&_stdscr_pseudo_panel, pan);
}

static void _calculate_obscure(void)
{
    PANEL *pan, *pan2;
    PANELOBS *tobs;     /* "this" one */
    PANELOBS *lobs;     /* last one */

    pan = _bottom_panel;

    while (pan)
    {
        if (pan->obscure)
            _free_obscure(pan);

        lobs = (PANELOBS *)0;
        pan2 = _bottom_panel;

        while (pan2)
        {
            if (_panels_overlapped(pan, pan2))
            {
                if ((tobs = malloc(sizeof(PANELOBS))) == NULL)
                    return;

                tobs->pan = pan2;
                dPanel("obscured", pan2);
                tobs->above = (PANELOBS *)0;

                if (lobs)
                    lobs->above = tobs;
                else
                    pan->obscure = tobs;

                lobs  = tobs;
            }

            pan2 = pan2->above;
        }

        _override(pan, 1);
        pan = pan->above;
    }
}

/* check to see if panel is in the stack */

static bool _panel_is_linked(const PANEL *pan)
{
    PANEL *pan2 = _bottom_panel;

    while (pan2)
    {
        if (pan2 == pan)
            return TRUE;

        pan2 = pan2->above;
    }

    return FALSE;
}

/* link panel into stack at top */

static void _panel_link_top(PANEL *pan)
{
#ifdef PANEL_DEBUG
    dStack("<lt%d>", 1, pan);
    if (_panel_is_linked(pan))
        return;
#endif
    pan->above = (PANEL *)0;
    pan->below = (PANEL *)0;

    if (_top_panel)
    {
        _top_panel->above = pan;
        pan->below = _top_panel;
    }

    _top_panel = pan;

    if (!_bottom_panel)
        _bottom_panel = pan;

    _calculate_obscure();
    dStack("<lt%d>", 9, pan);
}

/* link panel into stack at bottom */

static void _panel_link_bottom(PANEL *pan)
{
#ifdef PANEL_DEBUG
    dStack("<lb%d>", 1, pan);
    if (_panel_is_linked(pan))
        return;
#endif
    pan->above = (PANEL *)0;
    pan->below = (PANEL *)0;

    if (_bottom_panel)
    {
        _bottom_panel->below = pan;
        pan->above = _bottom_panel;
    }

    _bottom_panel = pan;

    if (!_top_panel)
        _top_panel = pan;

    _calculate_obscure();
    dStack("<lb%d>", 9, pan);
}

static void _panel_unlink(PANEL *pan)
{
    PANEL *prev;
    PANEL *next;

#ifdef PANEL_DEBUG
    dStack("<u%d>", 1, pan);
    if (!_panel_is_linked(pan))
        return;
#endif
    _override(pan, 0);
    _free_obscure(pan);

    prev = pan->below;
    next = pan->above;

    /* if non-zero, we will not update the list head */

    if (prev)
    {
        prev->above = next;
        if(next)
            next->below = prev;
    }
    else if (next)
        next->below = prev;

    if (pan == _bottom_panel)
        _bottom_panel = next;

    if (pan == _top_panel)
        _top_panel = prev;

    _calculate_obscure();

    pan->above = (PANEL *)0;
    pan->below = (PANEL *)0;
    dStack("<u%d>", 9, pan);

}

/************************************************************************
 *   The following are the public functions for the panels library.     *
 ************************************************************************/

int bottom_panel(PANEL *pan)
{
    if (!pan)
        return ERR;

    if (pan == _bottom_panel)
        return OK;

    if (_panel_is_linked(pan))
        hide_panel(pan);

    _panel_link_bottom(pan);

    return OK;
}

int del_panel(PANEL *pan)
{
    if (pan)
    {
        if (_panel_is_linked(pan))
            hide_panel(pan);

        free((char *)pan);
        return OK;
    }

    return ERR;
}

int hide_panel(PANEL *pan)
{
    if (!pan)
        return ERR;

    if (!_panel_is_linked(pan))
    {
        pan->above = (PANEL *)0;
        pan->below = (PANEL *)0;
        return ERR;
    }

    _panel_unlink(pan);

    return OK;
}

int move_panel(PANEL *pan, int starty, int startx)
{
    WINDOW *win;
    int maxy, maxx;

    if (!pan)
        return ERR;

    if (_panel_is_linked(pan))
        _override(pan, 0);

    win = pan->win;

    if (mvwin(win, starty, startx) == ERR)
        return ERR;

    getbegyx(win, pan->wstarty, pan->wstartx);
    getmaxyx(win, maxy, maxx);
    pan->wendy = pan->wstarty + maxy;
    pan->wendx = pan->wstartx + maxx;

    if (_panel_is_linked(pan))
        _calculate_obscure();

    return OK;
}

PANEL *new_panel(WINDOW *win)
{
    PANEL *pan;

    if (!win)
        return (PANEL *)NULL;

    pan  = malloc(sizeof(PANEL));

    if (!_stdscr_pseudo_panel.win)
    {
        _stdscr_pseudo_panel.win = stdscr;
        _stdscr_pseudo_panel.wstarty = 0;
        _stdscr_pseudo_panel.wstartx = 0;
        _stdscr_pseudo_panel.wendy = LINES;
        _stdscr_pseudo_panel.wendx = COLS;
        _stdscr_pseudo_panel.user = "stdscr";
        _stdscr_pseudo_panel.obscure = (PANELOBS *)0;
    }

    if (pan)
    {
        int maxy, maxx;

        pan->win = win;
        pan->above = (PANEL *)0;
        pan->below = (PANEL *)0;
        getbegyx(win, pan->wstarty, pan->wstartx);
        getmaxyx(win, maxy, maxx);
        pan->wendy = pan->wstarty + maxy;
        pan->wendx = pan->wstartx + maxx;
#ifdef PANEL_DEBUG
        pan->user = "new";
#else
        pan->user = (char *)0;
#endif
        pan->obscure = (PANELOBS *)0;
        show_panel(pan);
    }

    return pan;
}

PANEL *panel_above(const PANEL *pan)
{
    return pan ? pan->above : _bottom_panel;
}

PANEL *panel_below(const PANEL *pan)
{
    return pan ? pan->below : _top_panel;
}

int panel_hidden(const PANEL *pan)
{
    if (!pan)
        return ERR;

    return _panel_is_linked(pan) ? ERR : OK;
}

const void *panel_userptr(const PANEL *pan)
{
    return pan ? pan->user : NULL;
}

WINDOW *panel_window(const PANEL *pan)
{
    PDC_LOG(("panel_window() - called\n"));

    if (!pan)
        return (WINDOW *)NULL;

    return pan->win;
}

int replace_panel(PANEL *pan, WINDOW *win)
{
    int maxy, maxx;

    if (!pan)
        return ERR;

    if (_panel_is_linked(pan))
        _override(pan, 0);

    pan->win = win;
    getbegyx(win, pan->wstarty, pan->wstartx);
    getmaxyx(win, maxy, maxx);
    pan->wendy = pan->wstarty + maxy;
    pan->wendx = pan->wstartx + maxx;

    if (_panel_is_linked(pan))
        _calculate_obscure();

    return OK;
}

int set_panel_userptr(PANEL *pan, const void *uptr)
{
    if (!pan)
        return ERR;

    pan->user = uptr;
    return OK;
}

int show_panel(PANEL *pan)
{
    if (!pan)
        return ERR;

    if (pan == _top_panel)
        return OK;

    if (_panel_is_linked(pan))
        hide_panel(pan);

    _panel_link_top(pan);

    return OK;
}

int top_panel(PANEL *pan)
{
    return show_panel(pan);
}

void update_panels(void)
{
    PANEL *pan;

    PDC_LOG(("update_panels() - called\n"));

    pan = _bottom_panel;

    while (pan)
    {
        _override(pan, -1);
        pan = pan->above;
    }

    if (is_wintouched(stdscr))
        Wnoutrefresh(&_stdscr_pseudo_panel);

    pan = _bottom_panel;

    while (pan)
    {
        if (is_wintouched(pan->win) || !pan->above)
            Wnoutrefresh(pan);

        pan = pan->above;
    }
}
/* PDCurses */

/*man-start**************************************************************

printw
------

### Synopsis

    int printw(const char *fmt, ...);
    int wprintw(WINDOW *win, const char *fmt, ...);
    int mvprintw(int y, int x, const char *fmt, ...);
    int mvwprintw(WINDOW *win, int y, int x, const char *fmt,...);
    int vwprintw(WINDOW *win, const char *fmt, va_list varglist);
    int vw_printw(WINDOW *win, const char *fmt, va_list varglist);

### Description

   The printw() functions add a formatted string to the window at the
   current or specified cursor position. The format strings are the same
   as used in the standard C library's printf(). (printw() can be used
   as a drop-in replacement for printf().)

   The duplication between vwprintw() and vw_printw() is for historic
   reasons. In PDCurses, they're the same.

### Return Value

   All functions return the number of characters printed, or ERR on
   error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   printw                |    Y   |    Y    |   Y
   wprintw               |    Y   |    Y    |   Y
   mvprintw              |    Y   |    Y    |   Y
   mvwprintw             |    Y   |    Y    |   Y
   vwprintw              |    Y   |    Y    |   Y
   vw_printw             |    Y   |    Y    |   Y

**man-end****************************************************************/

int vwprintw(WINDOW *win, const char *fmt, va_list varglist)
{
    char printbuf[513];
    int len;

    PDC_LOG(("vwprintw() - called\n"));

#ifdef HAVE_VSNPRINTF
    len = vsnprintf(printbuf, 512, fmt, varglist);
#else
    len = vsprintf(printbuf, fmt, varglist);
#endif
    return (waddstr(win, printbuf) == ERR) ? ERR : len;
}

int printw(const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("printw() - called\n"));

    va_start(args, fmt);
    retval = vwprintw(stdscr, fmt, args);
    va_end(args);

    return retval;
}

int wprintw(WINDOW *win, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("wprintw() - called\n"));

    va_start(args, fmt);
    retval = vwprintw(win, fmt, args);
    va_end(args);

    return retval;
}

int mvprintw(int y, int x, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("mvprintw() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    va_start(args, fmt);
    retval = vwprintw(stdscr, fmt, args);
    va_end(args);

    return retval;
}

int mvwprintw(WINDOW *win, int y, int x, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("mvwprintw() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    va_start(args, fmt);
    retval = vwprintw(win, fmt, args);
    va_end(args);

    return retval;
}

int vw_printw(WINDOW *win, const char *fmt, va_list varglist)
{
    PDC_LOG(("vw_printw() - called\n"));

    return vwprintw(win, fmt, varglist);
}
/* PDCurses */

/*man-start**************************************************************

refresh
-------

### Synopsis

    int refresh(void);
    int wrefresh(WINDOW *win);
    int wnoutrefresh(WINDOW *win);
    int doupdate(void);
    int redrawwin(WINDOW *win);
    int wredrawln(WINDOW *win, int beg_line, int num_lines);

### Description

   wrefresh() copies the named window to the physical terminal screen,
   taking into account what is already there in order to optimize cursor
   movement. refresh() does the same, using stdscr. These routines must
   be called to get any output on the terminal, as other routines only
   manipulate data structures. Unless leaveok() has been enabled, the
   physical cursor of the terminal is left at the location of the
   window's cursor.

   wnoutrefresh() and doupdate() allow multiple updates with more
   efficiency than wrefresh() alone. wrefresh() works by first calling
   wnoutrefresh(), which copies the named window to the virtual screen.
   It then calls doupdate(), which compares the virtual screen to the
   physical screen and does the actual update. A series of calls to
   wrefresh() will result in alternating calls to wnoutrefresh() and
   doupdate(), causing several bursts of output to the screen. By first
   calling wnoutrefresh() for each window, it is then possible to call
   doupdate() only once.

   In PDCurses, redrawwin() is equivalent to touchwin(), and wredrawln()
   is the same as touchline(). In some other curses implementations,
   there's a subtle distinction, but it has no meaning in PDCurses.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   refresh               |    Y   |    Y    |   Y
   wrefresh              |    Y   |    Y    |   Y
   wnoutrefresh          |    Y   |    Y    |   Y
   doupdate              |    Y   |    Y    |   Y
   redrawwin             |    Y   |    Y    |   Y
   wredrawln             |    Y   |    Y    |   Y

**man-end****************************************************************/

int wnoutrefresh(WINDOW *win)
{
    int begy, begx;     /* window's place on screen   */
    int i, j;

    PDC_LOG(("wnoutrefresh() - called: win=%p\n", win));

    if (!win)
        return ERR;
    if (is_pad(win))
        return pnoutrefresh(win,
				win->_pad._pad_y,
				win->_pad._pad_x,
				win->_pad._pad_top,
				win->_pad._pad_left,
				win->_pad._pad_bottom,
				win->_pad._pad_right);

    begy = win->_begy;
    begx = win->_begx;

    for (i = 0, j = begy; i < win->_maxy; i++, j++)
    {
        if (win->_firstch[i] != _NO_CHANGE)
        {
            chtype *src = win->_y[i];
            chtype *dest = curscr->_y[j] + begx;

            int first = win->_firstch[i]; /* first changed */
            int last = win->_lastch[i];   /* last changed */

            /* ignore areas on the outside that are marked as changed,
               but really aren't */

            while (first <= last && src[first] == dest[first])
                first++;

            while (last >= first && src[last] == dest[last])
                last--;

            /* if any have really changed... */

            if (first <= last)
            {
                memcpy(dest + first, src + first,
                       (last - first + 1) * sizeof(chtype));

                first += begx;
                last += begx;

                if (first < curscr->_firstch[j] ||
                    curscr->_firstch[j] == _NO_CHANGE)
                    curscr->_firstch[j] = first;

                if (last > curscr->_lastch[j])
                    curscr->_lastch[j] = last;
            }

            win->_firstch[i] = _NO_CHANGE;  /* updated now */
        }

        win->_lastch[i] = _NO_CHANGE;       /* updated now */
    }

    if (win->_clear)
        win->_clear = FALSE;

    if (!win->_leaveit)
    {
        curscr->_cury = win->_cury + begy;
        curscr->_curx = win->_curx + begx;
    }

    return OK;
}

int doupdate(void)
{
    int y;
    bool clearall;

    PDC_LOG(("doupdate() - called\n"));

    if (!SP || !curscr)
        return ERR;

    if (isendwin())         /* coming back after endwin() called */
    {
        reset_prog_mode();
        clearall = TRUE;
        SP->alive = TRUE;   /* so isendwin() result is correct */
    }
    else
        clearall = curscr->_clear;

    for (y = 0; y < SP->lines; y++)
    {
        PDC_LOG(("doupdate() - Transforming line %d of %d: %s\n",
                 y, SP->lines, (curscr->_firstch[y] != _NO_CHANGE) ?
                 "Yes" : "No"));

        if (clearall || curscr->_firstch[y] != _NO_CHANGE)
        {
            int first, last;

            chtype *src = curscr->_y[y];
            chtype *dest = SP->lastscr->_y[y];

            if (clearall)
            {
                first = 0;
                last = COLS - 1;
            }
            else
            {
                first = curscr->_firstch[y];
                last = curscr->_lastch[y];
            }

            while (first <= last)
            {
                int len = 0;

                /* build up a run of changed cells; if two runs are
                   separated by a single unchanged cell, ignore the
                   break */

                if (clearall)
                    len = last - first + 1;
                else
                    while (first + len <= last &&
                           (src[first + len] != dest[first + len] ||
                            (len && first + len < last &&
                             src[first + len + 1] != dest[first + len + 1])
                           )
                          )
                        len++;

                /* update the screen, and SP->lastscr */

                if (len)
                {
                    PDC_transform_line(y, first, len, src + first);
                    memcpy(dest + first, src + first, len * sizeof(chtype));
                    first += len;
                }

                /* skip over runs of unchanged cells */

                while (first <= last && src[first] == dest[first])
                    first++;
            }

            curscr->_firstch[y] = _NO_CHANGE;
            curscr->_lastch[y] = _NO_CHANGE;
        }
    }

    curscr->_clear = FALSE;

    if (SP->visibility)
        PDC_gotoyx(curscr->_cury, curscr->_curx);

    SP->cursrow = curscr->_cury;
    SP->curscol = curscr->_curx;

    PDC_doupdate();

    return OK;
}

int wrefresh(WINDOW *win)
{
    bool save_clear;

    PDC_LOG(("wrefresh() - called\n"));

    if ( !win || (win->_flags & (_PAD|_SUBPAD)) )
        return ERR;

    save_clear = win->_clear;

    if (win == curscr)
        curscr->_clear = TRUE;
    else
        wnoutrefresh(win);

    if (save_clear && win->_maxy == SP->lines && win->_maxx == SP->cols)
        curscr->_clear = TRUE;

    return doupdate();
}

int refresh(void)
{
    PDC_LOG(("refresh() - called\n"));

    return wrefresh(stdscr);
}

int wredrawln(WINDOW *win, int start, int num)
{
    int i;

    PDC_LOG(("wredrawln() - called: win=%p start=%d num=%d\n",
        win, start, num));

    if (!win || start > win->_maxy || start + num > win->_maxy)
        return ERR;

    for (i = start; i < start + num; i++)
    {
        win->_firstch[i] = 0;
        win->_lastch[i] = win->_maxx - 1;
    }

    return OK;
}

int redrawwin(WINDOW *win)
{
    PDC_LOG(("redrawwin() - called: win=%p\n", win));

    if (!win)
        return ERR;

    return wredrawln(win, 0, win->_maxy);
}
/* PDCurses */

/*man-start**************************************************************

scanw
-----

### Synopsis

    int scanw(const char *fmt, ...);
    int wscanw(WINDOW *win, const char *fmt, ...);
    int mvscanw(int y, int x, const char *fmt, ...);
    int mvwscanw(WINDOW *win, int y, int x, const char *fmt, ...);
    int vwscanw(WINDOW *win, const char *fmt, va_list varglist);
    int vw_scanw(WINDOW *win, const char *fmt, va_list varglist);

### Description

   These routines correspond to the standard C library's scanf() family.
   Each gets a string from the window via wgetnstr(), and uses the
   resulting line as input for the scan.

   The duplication between vwscanw() and vw_scanw() is for historic
   reasons. In PDCurses, they're the same.

### Return Value

   On successful completion, these functions return the number of items
   successfully matched. Otherwise they return ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   scanw                 |    Y   |    Y    |   Y
   wscanw                |    Y   |    Y    |   Y
   mvscanw               |    Y   |    Y    |   Y
   mvwscanw              |    Y   |    Y    |   Y
   vwscanw               |    Y   |    Y    |   Y
   vw_scanw              |    Y   |    Y    |   Y

**man-end****************************************************************/

static int _pdc_vsscanf(const char *, const char *, va_list);

# define vsscanf _pdc_vsscanf

int vwscanw(WINDOW *win, const char *fmt, va_list varglist)
{
    char scanbuf[256];

    PDC_LOG(("vwscanw() - called\n"));

    if (wgetnstr(win, scanbuf, 255) == ERR)
        return ERR;

    return vsscanf(scanbuf, fmt, varglist);
}

int scanw(const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("scanw() - called\n"));

    va_start(args, fmt);
    retval = vwscanw(stdscr, fmt, args);
    va_end(args);

    return retval;
}

int wscanw(WINDOW *win, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("wscanw() - called\n"));

    va_start(args, fmt);
    retval = vwscanw(win, fmt, args);
    va_end(args);

    return retval;
}

int mvscanw(int y, int x, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("mvscanw() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    va_start(args, fmt);
    retval = vwscanw(stdscr, fmt, args);
    va_end(args);

    return retval;
}

int mvwscanw(WINDOW *win, int y, int x, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("mvscanw() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    va_start(args, fmt);
    retval = vwscanw(win, fmt, args);
    va_end(args);

    return retval;
}

int vw_scanw(WINDOW *win, const char *fmt, va_list varglist)
{
    PDC_LOG(("vw_scanw() - called\n"));

    return vwscanw(win, fmt, varglist);
}

/* _pdc_vsscanf() - Internal routine to parse and format an input
   buffer. It scans a series of input fields; each field is formatted
   according to a supplied format string and the formatted input is
   stored in the variable number of addresses passed. Returns the number
   of input fields or EOF on error.

   Don't compile this unless required. Some compilers (at least Borland
   C++ 3.0) have to link with math libraries due to the use of floats.

   Based on vsscanf.c and input.c from emx 0.8f library source,
   Copyright (c) 1990-1992 by Eberhard Mattes, who has kindly agreed to
   its inclusion in PDCurses. */

#define WHITE(x) ((x) == ' ' || (x) == '\t' || (x) == '\n')

#define NEXT(x) \
        do { \
            x = *buf++; \
            if (!x) \
               return (count ? count : EOF); \
            ++chars; \
           } while (0)

#define UNGETC() \
        do { \
            --buf; --chars; \
           } while (0)

static int _pdc_vsscanf(const char *buf, const char *fmt, va_list arg_ptr)
{
    int count, chars, c, width, radix, d, i;
    int *int_ptr;
    long *long_ptr;
    short *short_ptr;
    char *char_ptr;
    unsigned char f;
    char neg, assign, ok, size;
    long n;
    char map[256], end;
    double dx, dd, *dbl_ptr;
    float *flt_ptr;
    int exp;
    char eneg;

    count = 0;
    chars = 0;
    c = 0;
    while ((f = *fmt) != 0)
    {
        if (WHITE(f))
        {
            do
            {
                ++fmt;
                f = *fmt;
            }
            while (WHITE(f));
            do
            {
                c = *buf++;
                if (!c)
                {
                    if (!f || count)
                        return count;
                    else
                        return EOF;
                } else
                    ++chars;
            }
            while (WHITE(c));
            UNGETC();
        } else if (f != '%')
        {
            NEXT(c);
            if (c != f)
                return count;
            ++fmt;
        } else
        {
            assign = TRUE;
            width = INT_MAX;
            char_ptr = NULL;
            ++fmt;
            if (*fmt == '*')
            {
                assign = FALSE;
                ++fmt;
            }
            if (isdigit(*fmt))
            {
                width = 0;
                while (isdigit(*fmt))
                    width = width * 10 + (*fmt++ - '0');
                if (!width)
                    width = INT_MAX;
            }
            size = 0;
            if (*fmt == 'h' || *fmt == 'l')
                size = *fmt++;
            f = *fmt;
            switch (f)
            {
            case 'c':
                if (width == INT_MAX)
                    width = 1;
                if (assign)
                    char_ptr = va_arg(arg_ptr, char *);
                while (width > 0)
                {
                    --width;
                    NEXT(c);
                    if (assign)
                    {
                        *char_ptr++ = (char) c;
                        ++count;
                    }
                }
                break;
            case '[':
                memset(map, 0, 256);
                end = 0;
                ++fmt;
                if (*fmt == '^')
                {
                    ++fmt;
                    end = 1;
                }
                i = 0;
                for (;;)
                {
                    f = (unsigned char) *fmt;
                    switch (f)
                    {
                    case 0:
                        /* avoid skipping past 0 */
                        --fmt;
                        NEXT(c);
                        goto string;
                    case ']':
                        if (i > 0)
                        {
                            NEXT(c);
                            goto string;
                        }
                        /* no break */
                    default:
                        if (fmt[1] == '-' && fmt[2]
                            && f < (unsigned char)fmt[2])
                        {
                            memset(map + f, 1, (unsigned char)fmt[2] - f);
                            fmt += 2;
                        }
                        else
                            map[f] = 1;
                        break;
                    }
                    ++fmt;
                    ++i;
                }
            case 's':
                memset(map, 0, 256);
                map[' '] = 1;
                map['\n'] = 1;
                map['\r'] = 1;
                map['\t'] = 1;
                end = 1;
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
            string:
                if (assign)
                    char_ptr = va_arg(arg_ptr, char *);
                while (width > 0 && map[(unsigned char) c] != end)
                {
                    --width;
                    if (assign)
                        *char_ptr++ = (char) c;
                    c = *buf++;
                    if (!c)
                        break;
                    else
                        ++chars;
                }
                if (assign)
                {
                    *char_ptr = 0;
                    ++count;
                }
                if (!c)
                    return count;
                else
                    UNGETC();
                break;
            case 'f':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                neg = ok = FALSE;
                dx = 0.0;
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
                if (c == '+')
                {
                    NEXT(c);
                    --width;
                } else if (c == '-')
                {
                    neg = TRUE;
                    NEXT(c);
                    --width;
                }
                while (width > 0 && isdigit(c))
                {
                    --width;
                    dx = dx * 10.0 + (double) (c - '0');
                    ok = TRUE;
                    c = *buf++;
                    if (!c)
                        break;
                    else
                        ++chars;
                }
                if (width > 0 && c == '.')
                {
                    --width;
                    dd = 10.0;
                    NEXT(c);
                    while (width > 0 && isdigit(c))
                    {
                        --width;
                        dx += (double) (c - '0') / dd;
                        dd *= 10.0;
                        ok = TRUE;
                        c = *buf++;
                        if (!c)
                            break;
                        else
                            ++chars;
                    }
                }
                if (!ok)
                    return count;
                if (width > 0 && (c == 'e' || c == 'E'))
                {
                    eneg = FALSE;
                    exp = 0;
                    NEXT(c);
                    --width;
                    if (width > 0 && c == '+')
                    {
                        NEXT(c);
                        --width;
                    } else if (width > 0 && c == '-')
                    {
                        eneg = TRUE;
                        NEXT(c);
                        --width;
                    }
                    if (!(width > 0 && isdigit(c)))
                    {
                        UNGETC();
                        return count;
                    }
                    while (width > 0 && isdigit(c))
                    {
                        --width;
                        exp = exp * 10 + (c - '0');
                        c = *buf++;
                        if (!c)
                            break;
                        else
                            ++chars;
                    }
                    if (eneg)
                        exp = -exp;
                    while (exp > 0)
                    {
                        dx *= 10.0;
                        --exp;
                    }
                    while (exp < 0)
                    {
                        dx /= 10.0;
                        ++exp;
                    }
                }
                if (assign)
                {
                    if (neg)
                        dx = -dx;
                    if (size == 'l')
                    {
                        dbl_ptr = va_arg(arg_ptr, double *);
                        *dbl_ptr = dx;
                    }
                    else
                    {
                        flt_ptr = va_arg(arg_ptr, float *);
                        *flt_ptr = (float)dx;
                    }
                    ++count;
                }
                if (!c)
                    return count;
                else
                    UNGETC();
                break;
            case 'i':
                neg = FALSE;
                radix = 10;
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
                if (!(width > 0 && c == '0'))
                    goto scan_complete_number;
                NEXT(c);
                --width;
                if (width > 0 && (c == 'x' || c == 'X'))
                {
                    NEXT(c);
                    radix = 16;
                    --width;
                }
                else if (width > 0 && (c >= '0' && c <= '7'))
                    radix = 8;
                goto scan_unsigned_number;
            case 'd':
            case 'u':
            case 'o':
            case 'x':
            case 'X':
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
                switch (f)
                {
                case 'o':
                    radix = 8;
                    break;
                case 'x':
                case 'X':
                    radix = 16;
                    break;
                default:
                    radix = 10;
                    break;
                }
            scan_complete_number:
                neg = FALSE;
                if (width > 0 && c == '+')
                {
                    NEXT(c);
                    --width;
                }
                else if (width > 0 && c == '-' && radix == 10)
                {
                    neg = TRUE;
                    NEXT(c);
                    --width;
                }
            scan_unsigned_number:
                n = 0;
                ok = FALSE;
                while (width > 0)
                {
                    --width;
                    if (isdigit(c))
                        d = c - '0';
                    else if (isupper(c))
                        d = c - 'A' + 10;
                    else if (islower(c))
                        d = c - 'a' + 10;
                    else
                        break;
                    if (d < 0 || d >= radix)
                        break;
                    ok = TRUE;
                    n = n * radix + d;
                    c = *buf++;
                    if (!c)
                        break;
                    else
                        ++chars;
                }
                if (!ok)
                    return count;
                if (assign)
                {
                    if (neg)
                        n = -n;
                    switch (size)
                    {
                    case 'h':
                        short_ptr = va_arg(arg_ptr, short *);
                        *short_ptr = (short) n;
                        break;
                    case 'l':
                        long_ptr = va_arg(arg_ptr, long *);
                        *long_ptr = (long) n;
                        break;
                    default:
                        int_ptr = va_arg(arg_ptr, int *);
                        *int_ptr = (int) n;
                    }
                    ++count;
                }
                if (!c)
                    return count;
                else
                    UNGETC();
                break;
            case 'n':
                if (assign)
                {
                    int_ptr = va_arg(arg_ptr, int *);
                    *int_ptr = chars;
                    ++count;
                }
                break;
            default:
                if (!f) /* % at end of string */
                    return count;
                NEXT(c);
                if (c != f)
                    return count;
                break;
            }
            ++fmt;
        }
    }
    return count;
}

/* PDCurses */

/*man-start**************************************************************

scr_dump
--------

### Synopsis

    int putwin(WINDOW *win, FILE *filep);
    WINDOW *getwin(FILE *filep);
    int scr_dump(const char *filename);
    int scr_init(const char *filename);
    int scr_restore(const char *filename);
    int scr_set(const char *filename);

### Description

   getwin() reads window-related data previously stored in a file by
   putwin(). It then creates and initialises a new window using that
   data.

   putwin() writes all data associated with a window into a file, using
   an unspecified format. This information can be retrieved later using
   getwin().

   scr_dump() writes the current contents of the virtual screen to the
   file named by filename in an unspecified format.

   scr_restore() function sets the virtual screen to the contents of the
   file named by filename, which must have been written using
   scr_dump(). The next refresh operation restores the screen to the way
   it looked in the dump file.

   In PDCurses, scr_init() does nothing, and scr_set() is a synonym for
   scr_restore(). Also, scr_dump() and scr_restore() save and load from
   curscr. This differs from some other implementations, where
   scr_init() works with curscr, and scr_restore() works with newscr;
   but the effect should be the same. (PDCurses has no newscr.)

### Return Value

   On successful completion, getwin() returns a pointer to the window it
   created. Otherwise, it returns a null pointer. Other functions return
   OK or ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   putwin                |    Y   |    Y    |   Y
   getwin                |    Y   |    Y    |   Y
   scr_dump              |    Y   |    Y    |   -
   scr_init              |    Y   |    Y    |   -
   scr_restore           |    Y   |    Y    |   -
   scr_set               |    Y   |    Y    |   -

**man-end****************************************************************/

#define DUMPVER 1   /* Should be updated whenever the WINDOW struct is
                       changed */

int putwin(WINDOW *win, FILE *filep)
{
    static const char *marker = "PDC";
    static const unsigned char version = DUMPVER;

    PDC_LOG(("putwin() - called\n"));

    /* write the marker and the WINDOW struct */

    if (filep && fwrite(marker, strlen(marker), 1, filep)
              && fwrite(&version, 1, 1, filep)
              && fwrite(win, sizeof(WINDOW), 1, filep))
    {
        int i;

        /* write each line */

        for (i = 0; i < win->_maxy && win->_y[i]; i++)
            if (!fwrite(win->_y[i], win->_maxx * sizeof(chtype), 1, filep))
                return ERR;

        return OK;
    }

    return ERR;
}

WINDOW *getwin(FILE *filep)
{
    WINDOW *win;
    char marker[4];
    int i, nlines, ncols;

    PDC_LOG(("getwin() - called\n"));

    win = malloc(sizeof(WINDOW));
    if (!win)
        return (WINDOW *)NULL;

    /* check for the marker, and load the WINDOW struct */

    if (!filep || !fread(marker, 4, 1, filep) || strncmp(marker, "PDC", 3)
        || marker[3] != DUMPVER || !fread(win, sizeof(WINDOW), 1, filep))
    {
        free(win);
        return (WINDOW *)NULL;
    }

    nlines = win->_maxy;
    ncols = win->_maxx;

    /* allocate the line pointer array */

    win->_y = malloc(nlines * sizeof(chtype *));
    if (!win->_y)
    {
        free(win);
        return (WINDOW *)NULL;
    }

    /* allocate the minchng and maxchng arrays */

    win->_firstch = malloc(nlines * sizeof(int));
    if (!win->_firstch)
    {
        free(win->_y);
        free(win);
        return (WINDOW *)NULL;
    }

    win->_lastch = malloc(nlines * sizeof(int));
    if (!win->_lastch)
    {
        free(win->_firstch);
        free(win->_y);
        free(win);
        return (WINDOW *)NULL;
    }

    /* allocate the lines */

    win = PDC_makelines(win);
    if (!win)
        return (WINDOW *)NULL;

    /* read them */

    for (i = 0; i < nlines; i++)
    {
        if (!fread(win->_y[i], ncols * sizeof(chtype), 1, filep))
        {
            delwin(win);
            return (WINDOW *)NULL;
        }
    }

    touchwin(win);

    return win;
}

int scr_dump(const char *filename)
{
    FILE *filep;

    PDC_LOG(("scr_dump() - called: filename %s\n", filename));

    if (filename && (filep = fopen(filename, "wb")) != NULL)
    {
        int result = putwin(curscr, filep);
        fclose(filep);
        return result;
    }

    return ERR;
}

int scr_init(const char *filename)
{
    PDC_LOG(("scr_init() - called: filename %s\n", filename));

    return OK;
}

int scr_restore(const char *filename)
{
    FILE *filep;

    PDC_LOG(("scr_restore() - called: filename %s\n", filename));

    if (filename && (filep = fopen(filename, "rb")) != NULL)
    {
        WINDOW *replacement = getwin(filep);
        fclose(filep);

        if (replacement)
        {
            int result = overwrite(replacement, curscr);
            delwin(replacement);
            return result;
        }
    }

    return ERR;
}

int scr_set(const char *filename)
{
    PDC_LOG(("scr_set() - called: filename %s\n", filename));

    return scr_restore(filename);
}
/* PDCurses */

/*man-start**************************************************************

scroll
------

### Synopsis

    int scroll(WINDOW *win);
    int scrl(int n);
    int wscrl(WINDOW *win, int n);

### Description

   scroll() causes the window to scroll up one line. This involves
   moving the lines in the window data strcture.

   With a positive n, scrl() and wscrl() scroll the window up n lines
   (line i + n becomes i); otherwise they scroll the window down n
   lines.

   For these functions to work, scrolling must be enabled via
   scrollok(). Note also that scrolling is not allowed if the supplied
   window is a pad.

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   scroll                |    Y   |    Y    |   Y
   scrl                  |    Y   |    Y    |   Y
   wscrl                 |    Y   |    Y    |   Y

**man-end****************************************************************/

int wscrl(WINDOW *win, int n)
{
    int i, l, dir, start, end;
    chtype blank, *temp;

    /* Check if window scrolls. Valid for window AND pad */

    if (!win || !win->_scroll || !n)
        return ERR;

    blank = win->_bkgd;

    if (n > 0)
    {
        start = win->_tmarg;
        end = win->_bmarg;
        dir = 1;
    }
    else
    {
        start = win->_bmarg;
        end = win->_tmarg;
        dir = -1;
    }

    for (l = 0; l < (n * dir); l++)
    {
        temp = win->_y[start];

        /* re-arrange line pointers */

        for (i = start; i != end; i += dir)
            win->_y[i] = win->_y[i + dir];

        win->_y[end] = temp;

        /* make a blank line */

        for (i = 0; i < win->_maxx; i++)
            *temp++ = blank;
    }

    touchline(win, win->_tmarg, win->_bmarg - win->_tmarg + 1);

    PDC_sync(win);
    return OK;
}

int scrl(int n)
{
    PDC_LOG(("scrl() - called\n"));

    return wscrl(stdscr, n);
}

int scroll(WINDOW *win)
{
    PDC_LOG(("scroll() - called\n"));

    return wscrl(win, 1);
}
/* PDCurses */

/*man-start**************************************************************

slk
---

### Synopsis

    int slk_init(int fmt);
    int slk_set(int labnum, const char *label, int justify);
    int slk_refresh(void);
    int slk_noutrefresh(void);
    char *slk_label(int labnum);
    int slk_clear(void);
    int slk_restore(void);
    int slk_touch(void);
    int slk_attron(const chtype attrs);
    int slk_attr_on(const attr_t attrs, void *opts);
    int slk_attrset(const chtype attrs);
    int slk_attr_set(const attr_t attrs, short color_pair, void *opts);
    int slk_attroff(const chtype attrs);
    int slk_attr_off(const attr_t attrs, void *opts);
    int slk_color(short color_pair);

    int slk_wset(int labnum, const wchar_t *label, int justify);

    int PDC_mouse_in_slk(int y, int x);
    void PDC_slk_free(void);
    void PDC_slk_initialize(void);

    wchar_t *slk_wlabel(int labnum)

### Description

   These functions manipulate a window that contain Soft Label Keys
   (SLK). To use the SLK functions, a call to slk_init() must be made
   BEFORE initscr() or newterm(). slk_init() removes 1 or 2 lines from
   the useable screen, depending on the format selected.

   The line(s) removed from the screen are used as a separate window, in
   which SLKs are displayed.

   slk_init() requires a single parameter which describes the format of
   the SLKs as follows:

    0       3-2-3 format
    1       4-4 format
    2       4-4-4 format (ncurses extension)
    3       4-4-4 format with index line (ncurses extension)
    2 lines used
    55      5-5 format (pdcurses format)

   slk_refresh(), slk_noutrefresh() and slk_touch() are analogous to
   refresh(), noutrefresh() and touch().

### Return Value

   All functions return OK on success and ERR on error.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   slk_init              |    Y   |    Y    |   Y
   slk_set               |    Y   |    Y    |   Y
   slk_refresh           |    Y   |    Y    |   Y
   slk_noutrefresh       |    Y   |    Y    |   Y
   slk_label             |    Y   |    Y    |   Y
   slk_clear             |    Y   |    Y    |   Y
   slk_restore           |    Y   |    Y    |   Y
   slk_touch             |    Y   |    Y    |   Y
   slk_attron            |    Y   |    Y    |   Y
   slk_attrset           |    Y   |    Y    |   Y
   slk_attroff           |    Y   |    Y    |   Y
   slk_attr_on           |    Y   |    Y    |   Y
   slk_attr_set          |    Y   |    Y    |   Y
   slk_attr_off          |    Y   |    Y    |   Y
   slk_wset              |    Y   |    Y    |   Y
   PDC_mouse_in_slk      |    -   |    -    |   -
   PDC_slk_free          |    -   |    -    |   -
   PDC_slk_initialize    |    -   |    -    |   -
   slk_wlabel            |    -   |    -    |   -

**man-end****************************************************************/

enum { LABEL_NORMAL = 8, LABEL_EXTENDED = 10, LABEL_NCURSES_EXTENDED = 12 };

static int label_length = 0;
static int labels = 0;
static int label_fmt = 0;
static int label_line = 0;
static bool hidden = FALSE;

static struct SLK {
    chtype label[32];
    int len;
    int format;
    int start_col;
} *slk = (struct SLK *)NULL;

/* slk_init() is the slk initialization routine.
   This must be called before initscr().

   label_fmt = 0, 1 or 55.
       0 = 3-2-3 format
       1 = 4 - 4 format
       2 = 4-4-4 format (ncurses extension for PC 12 function keys)
       3 = 4-4-4 format (ncurses extension for PC 12 function keys -
    with index line)
      55 = 5 - 5 format (extended for PC, 10 function keys) */

int slk_init(int fmt)
{
    PDC_LOG(("slk_init() - called\n"));

    if (SP)
        return ERR;

    switch (fmt)
    {
    case 0:  /* 3 - 2 - 3 */
        labels = LABEL_NORMAL;
        break;

    case 1:   /* 4 - 4 */
        labels = LABEL_NORMAL;
        break;

    case 2:   /* 4 4 4 */
        labels = LABEL_NCURSES_EXTENDED;
        break;

    case 3:   /* 4 4 4  with index */
        labels = LABEL_NCURSES_EXTENDED;
        break;

    case 55:  /* 5 - 5 */
        labels = LABEL_EXTENDED;
        break;

    default:
        return ERR;
    }

    label_fmt = fmt;

    slk = calloc(labels, sizeof(struct SLK));

    if (!slk)
        labels = 0;

    return slk ? OK : ERR;
}

/* draw a single button */

static void _drawone(int num)
{
    int i, col, slen;

    if (hidden)
        return;

    slen = slk[num].len;

    switch (slk[num].format)
    {
    case 0:  /* LEFT */
        col = 0;
        break;

    case 1:  /* CENTER */
        col = (label_length - slen) / 2;

        if (col + slen > label_length)
            --col;
        break;

    default:  /* RIGHT */
        col = label_length - slen;
    }

    wmove(SP->slk_winptr, label_line, slk[num].start_col);

    for (i = 0; i < label_length; ++i)
        waddch(SP->slk_winptr, (i >= col && i < (col + slen)) ?
               slk[num].label[i - col] : ' ');
}

/* redraw each button */

static void _redraw(void)
{
    int i;

    for (i = 0; i < labels; ++i)
        _drawone(i);
}

/* slk_set() Used to set a slk label to a string.

   labnum  = 1 - 8 (or 10) (number of the label)
   label   = string (8 or 7 bytes total), or NULL
   justify = 0 : left, 1 : center, 2 : right  */

int slk_set(int labnum, const char *label, int justify)
{
#ifdef PDC_WIDE
    wchar_t wlabel[32];

    PDC_mbstowcs(wlabel, label, 31);
    return slk_wset(labnum, wlabel, justify);
#else
    PDC_LOG(("slk_set() - called\n"));

    if (labnum < 1 || labnum > labels || justify < 0 || justify > 2)
        return ERR;

    labnum--;

    if (!label || !(*label))
    {
        /* Clear the label */

        *slk[labnum].label = 0;
        slk[labnum].format = 0;
        slk[labnum].len = 0;
    }
    else
    {
        int i, j = 0;

        /* Skip leading spaces */

        while (label[j] == ' ')
            j++;

        /* Copy it */

        for (i = 0; i < label_length; i++)
        {
            chtype ch = label[i + j];

            slk[labnum].label[i] = ch;

            if (!ch)
                break;
        }

        /* Drop trailing spaces */

        while ((i + j) && (label[i + j - 1] == ' '))
            i--;

        slk[labnum].label[i] = 0;
        slk[labnum].format = justify;
        slk[labnum].len = i;
    }

    _drawone(labnum);

    return OK;
#endif
}

int slk_refresh(void)
{
    PDC_LOG(("slk_refresh() - called\n"));

    return (slk_noutrefresh() == ERR) ? ERR : doupdate();
}

int slk_noutrefresh(void)
{
    PDC_LOG(("slk_noutrefresh() - called\n"));

    if (!SP)
        return ERR;

    return wnoutrefresh(SP->slk_winptr);
}

char *slk_label(int labnum)
{
    static char temp[33];
#ifdef PDC_WIDE
    wchar_t *wtemp = slk_wlabel(labnum);

    PDC_wcstombs(temp, wtemp, 32);
#else
    chtype *p;
    int i;

    PDC_LOG(("slk_label() - called\n"));

    if (labnum < 1 || labnum > labels)
        return (char *)0;

    for (i = 0, p = slk[labnum - 1].label; *p; i++)
        temp[i] = *p++;

    temp[i] = '\0';
#endif
    return temp;
}

int slk_clear(void)
{
    PDC_LOG(("slk_clear() - called\n"));

    if (!SP)
        return ERR;

    hidden = TRUE;
    werase(SP->slk_winptr);
    return wrefresh(SP->slk_winptr);
}

int slk_restore(void)
{
    PDC_LOG(("slk_restore() - called\n"));

    if (!SP)
        return ERR;

    hidden = FALSE;
    _redraw();
    return wrefresh(SP->slk_winptr);
}

int slk_touch(void)
{
    PDC_LOG(("slk_touch() - called\n"));

    if (!SP)
        return ERR;

    return touchwin(SP->slk_winptr);
}

int slk_attron(const chtype attrs)
{
    int rc;

    PDC_LOG(("slk_attron() - called\n"));

    if (!SP)
        return ERR;

    rc = wattron(SP->slk_winptr, attrs);
    _redraw();

    return rc;
}

int slk_attr_on(const attr_t attrs, void *opts)
{
    PDC_LOG(("slk_attr_on() - called\n"));

    return slk_attron(attrs);
}

int slk_attroff(const chtype attrs)
{
    int rc;

    PDC_LOG(("slk_attroff() - called\n"));

    if (!SP)
        return ERR;

    rc = wattroff(SP->slk_winptr, attrs);
    _redraw();

    return rc;
}

int slk_attr_off(const attr_t attrs, void *opts)
{
    PDC_LOG(("slk_attr_off() - called\n"));

    return slk_attroff(attrs);
}

int slk_attrset(const chtype attrs)
{
    int rc;

    PDC_LOG(("slk_attrset() - called\n"));

    if (!SP)
        return ERR;

    rc = wattrset(SP->slk_winptr, attrs);
    _redraw();

    return rc;
}

int slk_color(short color_pair)
{
    int rc;

    PDC_LOG(("slk_color() - called\n"));

    if (!SP)
        return ERR;

    rc = wcolor_set(SP->slk_winptr, color_pair, NULL);
    _redraw();

    return rc;
}

int slk_attr_set(const attr_t attrs, short color_pair, void *opts)
{
    PDC_LOG(("slk_attr_set() - called\n"));

    return slk_attrset(attrs | COLOR_PAIR(color_pair));
}

static void _slk_calc(void)
{
    int i, center, col = 0;
    label_length = COLS / labels;

    if (label_length > 31)
        label_length = 31;

    switch (label_fmt)
    {
    case 0:     /* 3 - 2 - 3 F-Key layout */

        --label_length;

        slk[0].start_col = col;
        slk[1].start_col = (col += label_length);
        slk[2].start_col = (col += label_length);

        center = COLS / 2;

        slk[3].start_col = center - label_length + 1;
        slk[4].start_col = center + 1;

        col = COLS - (label_length * 3) + 1;

        slk[5].start_col = col;
        slk[6].start_col = (col += label_length);
        slk[7].start_col = (col += label_length);
        break;

    case 1:     /* 4 - 4 F-Key layout */

        for (i = 0; i < 8; i++)
        {
            slk[i].start_col = col;
            col += label_length;

            if (i == 3)
                col = COLS - (label_length * 4) + 1;
        }

        break;

    case 2:     /* 4 4 4 F-Key layout */
    case 3:     /* 4 4 4 F-Key layout with index */

        for (i = 0; i < 4; i++)
        {
            slk[i].start_col = col;
            col += label_length;
        }

        center = COLS / 2;

        slk[4].start_col = center - (label_length * 2) + 1;
        slk[5].start_col = center - label_length + 1;
        slk[6].start_col = center + 1;
        slk[7].start_col = center + label_length + 1;

        col = COLS - (label_length * 4) + 1;

        for (i = 8; i < 12; i++)
        {
            slk[i].start_col = col;
            col += label_length;
        }

        break;

    default:    /* 5 - 5 F-Key layout */

        for (i = 0; i < 10; i++)
        {
            slk[i].start_col = col;
            col += label_length;

            if (i == 4)
                col = COLS - (label_length * 5) + 1;
        }
    }

    --label_length;

    /* make sure labels are all in window */

    _redraw();
}

void PDC_slk_initialize(void)
{
    if (slk)
    {
        if (label_fmt == 3)
        {
            SP->slklines = 2;
            label_line = 1;
        }
        else
            SP->slklines = 1;

        if (!SP->slk_winptr)
        {
            SP->slk_winptr = newwin(SP->slklines, COLS,
                                    LINES - SP->slklines, 0);
            if (!SP->slk_winptr)
                return;

            wattrset(SP->slk_winptr, A_REVERSE);
        }

        _slk_calc();

        /* if we have an index line, display it now */

        if (label_fmt == 3)
        {
            chtype save_attr;
            int i;

            save_attr = SP->slk_winptr->_attrs;
            wattrset(SP->slk_winptr, A_NORMAL);
            wmove(SP->slk_winptr, 0, 0);
            whline(SP->slk_winptr, 0, COLS);

            for (i = 0; i < labels; i++)
                mvwprintw(SP->slk_winptr, 0, slk[i].start_col, "F%d", i + 1);

            SP->slk_winptr->_attrs = save_attr;
        }

        touchwin(SP->slk_winptr);
    }
}

void PDC_slk_free(void)
{
    if (slk)
    {
        if (SP->slk_winptr)
        {
            delwin(SP->slk_winptr);
            SP->slk_winptr = (WINDOW *)NULL;
        }

        free(slk);
        slk = (struct SLK *)NULL;

        label_length = 0;
        labels = 0;
        label_fmt = 0;
        label_line = 0;
        hidden = FALSE;
    }
}

int PDC_mouse_in_slk(int y, int x)
{
    int i;

    PDC_LOG(("PDC_mouse_in_slk() - called: y->%d x->%d\n", y, x));

    /* If the line on which the mouse was clicked is NOT the last line
       of the screen, we are not interested in it. */

    if (!slk || !SP->slk_winptr || (y != SP->slk_winptr->_begy + label_line))
        return 0;

    for (i = 0; i < labels; i++)
        if (x >= slk[i].start_col && x < (slk[i].start_col + label_length))
            return i + 1;

    return 0;
}

#ifdef PDC_WIDE
int slk_wset(int labnum, const wchar_t *label, int justify)
{
    PDC_LOG(("slk_wset() - called\n"));

    if (labnum < 1 || labnum > labels || justify < 0 || justify > 2)
        return ERR;

    labnum--;

    if (!label || !(*label))
    {
        /* Clear the label */

        *slk[labnum].label = 0;
        slk[labnum].format = 0;
        slk[labnum].len = 0;
    }
    else
    {
        int i, j = 0;

        /* Skip leading spaces */

        while (label[j] == L' ')
            j++;

        /* Copy it */

        for (i = 0; i < label_length; i++)
        {
            chtype ch = label[i + j];

            slk[labnum].label[i] = ch;

            if (!ch)
                break;
        }

        /* Drop trailing spaces */

        while ((i + j) && (label[i + j - 1] == L' '))
            i--;

        slk[labnum].label[i] = 0;
        slk[labnum].format = justify;
        slk[labnum].len = i;
    }

    _drawone(labnum);

    return OK;
}

wchar_t *slk_wlabel(int labnum)
{
    static wchar_t temp[33];
    chtype *p;
    int i;

    PDC_LOG(("slk_wlabel() - called\n"));

    if (labnum < 1 || labnum > labels)
        return (wchar_t *)0;

    for (i = 0, p = slk[labnum - 1].label; *p; i++)
        temp[i] = *p++;

    temp[i] = '\0';

    return temp;
}
#endif
/* PDCurses */

/*man-start**************************************************************

termattr
--------

### Synopsis

    int baudrate(void);
    char erasechar(void);
    bool has_ic(void);
    bool has_il(void);
    char killchar(void);
    char *longname(void);
    chtype termattrs(void);
    attr_t term_attrs(void);
    char *termname(void);

    int erasewchar(wchar_t *ch);
    int killwchar(wchar_t *ch);

    char wordchar(void);

### Description

   baudrate() is supposed to return the output speed of the terminal. In
   PDCurses, it simply returns INT_MAX.

   has_ic and has_il() return TRUE. These functions have meaning in some
   other implementations of curses.

   erasechar() and killchar() return ^H and ^U, respectively -- the
   ERASE and KILL characters. In other curses implementations, these may
   vary by terminal type. erasewchar() and killwchar() are the wide-
   character versions; they take a pointer to a location in which to
   store the character, and return OK or ERR.

   longname() returns a pointer to a static area containing a verbose
   description of the current terminal. The maximum length of the string
   is 128 characters. It is defined only after the call to initscr() or
   newterm().

   termname() returns a pointer to a static area containing a short
   description of the current terminal (14 characters).

   termattrs() returns a logical OR of all video attributes supported by
   the terminal.

   wordchar() is a PDCurses extension of the concept behind the
   functions erasechar() and killchar(), returning the "delete word"
   character, ^W.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   baudrate              |    Y   |    Y    |   Y
   erasechar             |    Y   |    Y    |   Y
   has_ic                |    Y   |    Y    |   Y
   has_il                |    Y   |    Y    |   Y
   killchar              |    Y   |    Y    |   Y
   longname              |    Y   |    Y    |   Y
   termattrs             |    Y   |    Y    |   Y
   termname              |    Y   |    Y    |   Y
   erasewchar            |    Y   |    Y    |   Y
   killwchar             |    Y   |    Y    |   Y
   term_attrs            |    Y   |    Y    |   Y
   wordchar              |    -   |    -    |   -

**man-end****************************************************************/


int baudrate(void)
{
    PDC_LOG(("baudrate() - called\n"));

    return INT_MAX;
}

char erasechar(void)
{
    PDC_LOG(("erasechar() - called\n"));

    return _ECHAR;      /* character delete char (^H) */
}

bool has_ic(void)
{
    PDC_LOG(("has_ic() - called\n"));

    return TRUE;
}

bool has_il(void)
{
    PDC_LOG(("has_il() - called\n"));

    return TRUE;
}

char killchar(void)
{
    PDC_LOG(("killchar() - called\n"));

    return _DLCHAR;     /* line delete char (^U) */
}

char *longname(void)
{
    PDC_LOG(("longname() - called\n"));

    return ttytype + 9; /* skip "pdcurses|" */
}

chtype termattrs(void)
{
    PDC_LOG(("termattrs() - called\n"));

    return SP ? SP->termattrs : (chtype)0;
}

attr_t term_attrs(void)
{
    PDC_LOG(("term_attrs() - called\n"));

    return SP ? SP->termattrs : (attr_t)0;
}

char *termname(void)
{
    static char _termname[14] = "pdcurses";

    PDC_LOG(("termname() - called\n"));

    return _termname;
}

char wordchar(void)
{
    PDC_LOG(("wordchar() - called\n"));

    return _DWCHAR;         /* word delete char */
}

#ifdef PDC_WIDE
int erasewchar(wchar_t *ch)
{
    PDC_LOG(("erasewchar() - called\n"));

    if (!ch)
        return ERR;

    *ch = (wchar_t)_ECHAR;

    return OK;
}

int killwchar(wchar_t *ch)
{
    PDC_LOG(("killwchar() - called\n"));

    if (!ch)
        return ERR;

    *ch = (wchar_t)_DLCHAR;

    return OK;
}
#endif
/* PDCurses */

/*man-start**************************************************************

touch
-----

### Synopsis

    int touchwin(WINDOW *win);
    int touchline(WINDOW *win, int start, int count);
    int untouchwin(WINDOW *win);
    int wtouchln(WINDOW *win, int y, int n, int changed);
    bool is_linetouched(WINDOW *win, int line);
    bool is_wintouched(WINDOW *win);

    int touchoverlap(const WINDOW *win1, WINDOW *win2);

### Description

   touchwin() and touchline() throw away all information about which
   parts of the window have been touched, pretending that the entire
   window has been drawn on. This is sometimes necessary when using
   overlapping windows, since a change to one window will affect the
   other window, but the records of which lines have been changed in the
   other window will not reflect the change.

   untouchwin() marks all lines in the window as unchanged since the
   last call to wrefresh().

   wtouchln() makes n lines in the window, starting at line y, look as
   if they have (changed == 1) or have not (changed == 0) been changed
   since the last call to wrefresh().

   is_linetouched() returns TRUE if the specified line in the specified
   window has been changed since the last call to wrefresh().

   is_wintouched() returns TRUE if the specified window has been changed
   since the last call to wrefresh().

   touchoverlap(win1, win2) marks the portion of win2 which overlaps
   with win1 as modified.

### Return Value

   All functions return OK on success and ERR on error except
   is_wintouched() and is_linetouched().

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   touchwin              |    Y   |    Y    |   Y
   touchline             |    Y   |    Y    |   Y
   untouchwin            |    Y   |    Y    |   Y
   wtouchln              |    Y   |    Y    |   Y
   is_linetouched        |    Y   |    Y    |   Y
   is_wintouched         |    Y   |    Y    |   Y
   touchoverlap          |    -   |    -    |   Y

**man-end****************************************************************/

int touchwin(WINDOW *win)
{
    int i;

    PDC_LOG(("touchwin() - called: Win=%x\n", win));

    if (!win)
        return ERR;

    for (i = 0; i < win->_maxy; i++)
    {
        win->_firstch[i] = 0;
        win->_lastch[i] = win->_maxx - 1;
    }

    return OK;
}

int touchline(WINDOW *win, int start, int count)
{
    int i;

    PDC_LOG(("touchline() - called: win=%p start %d count %d\n",
             win, start, count));

    if (!win || start > win->_maxy || start + count > win->_maxy)
        return ERR;

    for (i = start; i < start + count; i++)
    {
        win->_firstch[i] = 0;
        win->_lastch[i] = win->_maxx - 1;
    }

    return OK;
}

int untouchwin(WINDOW *win)
{
    int i;

    PDC_LOG(("untouchwin() - called: win=%p", win));

    if (!win)
        return ERR;

    for (i = 0; i < win->_maxy; i++)
    {
        win->_firstch[i] = _NO_CHANGE;
        win->_lastch[i] = _NO_CHANGE;
    }

    return OK;
}

int wtouchln(WINDOW *win, int y, int n, int changed)
{
    int i;

    PDC_LOG(("wtouchln() - called: win=%p y=%d n=%d changed=%d\n",
             win, y, n, changed));

    if (!win || y > win->_maxy || y + n > win->_maxy)
        return ERR;

    for (i = y; i < y + n; i++)
    {
        if (changed)
        {
            win->_firstch[i] = 0;
            win->_lastch[i] = win->_maxx - 1;
        }
        else
        {
            win->_firstch[i] = _NO_CHANGE;
            win->_lastch[i] = _NO_CHANGE;
        }
    }

    return OK;
}

bool is_linetouched(WINDOW *win, int line)
{
    PDC_LOG(("is_linetouched() - called: win=%p line=%d\n", win, line));

    if (!win || line > win->_maxy || line < 0)
        return FALSE;

    return (win->_firstch[line] != _NO_CHANGE) ? TRUE : FALSE;
}

bool is_wintouched(WINDOW *win)
{
    int i;

    PDC_LOG(("is_wintouched() - called: win=%p\n", win));

    if (win)
        for (i = 0; i < win->_maxy; i++)
            if (win->_firstch[i] != _NO_CHANGE)
                return TRUE;

    return FALSE;
}

int touchoverlap(const WINDOW *win1, WINDOW *win2)
{
    int y, endy, endx, starty, startx, begy1, begx1, begy2, begx2;

    PDC_LOG(("touchoverlap() - called: win1=%p win2=%p\n", win1, win2));

    if (!win1 || !win2)
        return ERR;

    begy1 = win1->_begy;
    begx1 = win1->_begx;
    begy2 = win2->_begy;
    begx2 = win2->_begy;

    starty = max(begy1, begy2);
    startx = max(begx1, begx2);
    endy = min(win1->_maxy + begy1, win2->_maxy + begy2);
    endx = min(win1->_maxx + begx1, win2->_maxx + begx2);

    if (starty >= endy || startx >= endx)
        return OK;

    starty -= begy2;
    startx -= begx2;
    endy -= begy2;
    endx -= begx2;
    endx -= 1;

    for (y = starty; y < endy; y++)
    {
        int first = win2->_firstch[y];

        if (first == _NO_CHANGE || win2->_lastch[y] < endx)
            win2->_lastch[y] = endx;
        if (first == _NO_CHANGE || first > startx)
            win2->_firstch[y] = startx;
    }

    return OK;
}
/* PDCurses */

/*man-start**************************************************************

util
----

### Synopsis

    char *unctrl(chtype c);
    void filter(void);
    void use_env(bool x);
    int delay_output(int ms);

    int getcchar(const cchar_t *wcval, wchar_t *wch, attr_t *attrs,
                 short *color_pair, void *opts);
    int setcchar(cchar_t *wcval, const wchar_t *wch, const attr_t attrs,
                 short color_pair, const void *opts);
    wchar_t *wunctrl(cchar_t *wc);

    int PDC_mbtowc(wchar_t *pwc, const char *s, size_t n);
    size_t PDC_mbstowcs(wchar_t *dest, const char *src, size_t n);
    size_t PDC_wcstombs(char *dest, const wchar_t *src, size_t n);

### Description

   unctrl() expands the text portion of the chtype c into a printable
   string. Control characters are changed to the "^X" notation; others
   are passed through. wunctrl() is the wide-character version of the
   function.

   filter() and use_env() are no-ops in PDCurses.

   delay_output() inserts an ms millisecond pause in output.

   getcchar() works in two modes: When wch is not NULL, it reads the
   cchar_t pointed to by wcval and stores the attributes in attrs, the
   color pair in color_pair, and the text in the wide-character string
   wch. When wch is NULL, getcchar() merely returns the number of wide
   characters in wcval. In either mode, the opts argument is unused.

   setcchar constructs a cchar_t at wcval from the wide-character text
   at wch, the attributes in attr and the color pair in color_pair. The
   opts argument is unused.

   Currently, the length returned by getcchar() is always 1 or 0.
   Similarly, setcchar() will only take the first wide character from
   wch, and ignore any others that it "should" take (i.e., combining
   characters). Nor will it correctly handle any character outside the
   basic multilingual plane (UCS-2).

### Return Value

   wunctrl() returns NULL on failure. delay_output() always returns OK.

   getcchar() returns the number of wide characters wcval points to when
   wch is NULL; when it's not, getcchar() returns OK or ERR.

   setcchar() returns OK or ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   unctrl                |    Y   |    Y    |   Y
   filter                |    Y   |    Y    |   Y
   use_env               |    Y   |    Y    |   Y
   delay_output          |    Y   |    Y    |   Y
   getcchar              |    Y   |    Y    |   Y
   setcchar              |    Y   |    Y    |   Y
   wunctrl               |    Y   |    Y    |   Y
   PDC_mbtowc            |    -   |    -    |   -
   PDC_mbstowcs          |    -   |    -    |   -
   PDC_wcstombs          |    -   |    -    |   -

**man-end****************************************************************/

char *unctrl(chtype c)
{
    static char strbuf[3] = {0, 0, 0};

    chtype ic;

    PDC_LOG(("unctrl() - called\n"));

    ic = c & A_CHARTEXT;

    if (ic >= 0x20 && ic != 0x7f)       /* normal characters */
    {
        strbuf[0] = (char)ic;
        strbuf[1] = '\0';
        return strbuf;
    }

    strbuf[0] = '^';            /* '^' prefix */

    if (ic == 0x7f)             /* 0x7f == DEL */
        strbuf[1] = '?';
    else                    /* other control */
        strbuf[1] = (char)(ic + '@');

    return strbuf;
}

void filter(void)
{
    PDC_LOG(("filter() - called\n"));
}

void use_env(bool x)
{
    PDC_LOG(("use_env() - called: x %d\n", x));
}

int delay_output(int ms)
{
    PDC_LOG(("delay_output() - called: ms %d\n", ms));

    return napms(ms);
}

#ifdef PDC_WIDE
int getcchar(const cchar_t *wcval, wchar_t *wch, attr_t *attrs,
             short *color_pair, void *opts)
{
    if (!wcval)
        return ERR;

    if (wch)
    {
        if (!attrs || !color_pair)
            return ERR;

        *wch = (*wcval & A_CHARTEXT);
        *attrs = (*wcval & (A_ATTRIBUTES & ~A_COLOR));
        *color_pair = PAIR_NUMBER(*wcval & A_COLOR);

        if (*wch)
            *++wch = L'\0';

        return OK;
    }
    else
        return ((*wcval & A_CHARTEXT) != L'\0');
}

int setcchar(cchar_t *wcval, const wchar_t *wch, const attr_t attrs,
             short color_pair, const void *opts)
{
    if (!wcval || !wch)
        return ERR;

    *wcval = *wch | attrs | COLOR_PAIR(color_pair);

    return OK;
}

wchar_t *wunctrl(cchar_t *wc)
{
    static wchar_t strbuf[3] = {0, 0, 0};

    cchar_t ic;

    PDC_LOG(("wunctrl() - called\n"));

    if (!wc)
        return NULL;

    ic = *wc & A_CHARTEXT;

    if (ic >= 0x20 && ic != 0x7f)       /* normal characters */
    {
        strbuf[0] = (wchar_t)ic;
        strbuf[1] = L'\0';
        return strbuf;
    }

    strbuf[0] = '^';            /* '^' prefix */

    if (ic == 0x7f)             /* 0x7f == DEL */
        strbuf[1] = '?';
    else                    /* other control */
        strbuf[1] = (wchar_t)(ic + '@');

    return strbuf;
}

int PDC_mbtowc(wchar_t *pwc, const char *s, size_t n)
{
# ifdef PDC_FORCE_UTF8
    wchar_t key;
    int i = -1;
    const unsigned char *string;

    if (!s || (n < 1))
        return -1;

    if (!*s)
        return 0;

    string = (const unsigned char *)s;

    key = string[0];

    /* Simplistic UTF-8 decoder -- only does the BMP, minimal validation */

    if (key & 0x80)
    {
        if ((key & 0xe0) == 0xc0)
        {
            if (1 < n)
            {
                key = ((key & 0x1f) << 6) | (string[1] & 0x3f);
                i = 2;
            }
        }
        else if ((key & 0xe0) == 0xe0)
        {
            if (2 < n)
            {
                key = ((key & 0x0f) << 12) | ((string[1] & 0x3f) << 6) |
                      (string[2] & 0x3f);
                i = 3;
            }
        }
    }
    else
        i = 1;

    if (i)
        *pwc = key;

    return i;
# else
    return mbtowc(pwc, s, n);
# endif
}

size_t PDC_mbstowcs(wchar_t *dest, const char *src, size_t n)
{
# ifdef PDC_FORCE_UTF8
    size_t i = 0, len;

    if (!src || !dest)
        return 0;

    len = strlen(src);

    while (*src && i < n)
    {
        int retval = PDC_mbtowc(dest + i, src, len);

        if (retval < 1)
            return -1;

        src += retval;
        len -= retval;
        i++;
    }
# else
    size_t i = mbstowcs(dest, src, n);
# endif
    dest[i] = 0;
    return i;
}

size_t PDC_wcstombs(char *dest, const wchar_t *src, size_t n)
{
# ifdef PDC_FORCE_UTF8
    size_t i = 0;

    if (!src || !dest)
        return 0;

    while (*src && i < n)
    {
        chtype code = *src++;

        if (code < 0x80)
        {
            dest[i] = code;
            i++;
        }
        else
            if (code < 0x800)
            {
                dest[i] = ((code & 0x07c0) >> 6) | 0xc0;
                dest[i + 1] = (code & 0x003f) | 0x80;
                i += 2;
            }
            else
            {
                dest[i] = ((code & 0xf000) >> 12) | 0xe0;
                dest[i + 1] = ((code & 0x0fc0) >> 6) | 0x80;
                dest[i + 2] = (code & 0x003f) | 0x80;
                i += 3;
            }
    }
# else
    size_t i = wcstombs(dest, src, n);
# endif
    dest[i] = '\0';
    return i;
}
#endif
/* PDCurses */

/*man-start**************************************************************

window
------

### Synopsis

    WINDOW *newwin(int nlines, int ncols, int begy, int begx);
    WINDOW *derwin(WINDOW* orig, int nlines, int ncols,
                   int begy, int begx);
    WINDOW *subwin(WINDOW* orig, int nlines, int ncols,
                   int begy, int begx);
    WINDOW *dupwin(WINDOW *win);
    WINDOW *wgetparent(const WINDOW *win);
    int delwin(WINDOW *win);
    int mvwin(WINDOW *win, int y, int x);
    int mvderwin(WINDOW *win, int pary, int parx);
    int syncok(WINDOW *win, bool bf);
    bool is_subwin(const WINDOW *win);
    bool is_syncok(const WINDOW *win);
    void wsyncup(WINDOW *win);
    void wcursyncup(WINDOW *win);
    void wsyncdown(WINDOW *win);

    WINDOW *resize_window(WINDOW *win, int nlines, int ncols);
    int wresize(WINDOW *win, int nlines, int ncols);
    WINDOW *PDC_makelines(WINDOW *win);
    WINDOW *PDC_makenew(int nlines, int ncols, int begy, int begx);
    void PDC_sync(WINDOW *win);

### Description

   newwin() creates a new window with the given number of lines, nlines
   and columns, ncols. The upper left corner of the window is at line
   begy, column begx. If nlines is zero, it defaults to LINES - begy;
   ncols to COLS - begx. Create a new full-screen window by calling
   newwin(0, 0, 0, 0).

   delwin() deletes the named window, freeing all associated memory. In
   the case of overlapping windows, subwindows should be deleted before
   the main window.

   mvwin() moves the window so that the upper left-hand corner is at
   position (y,x). If the move would cause the window to be off the
   screen, it is an error and the window is not moved. Moving subwindows
   is allowed.

   subwin() creates a new subwindow within a window. The dimensions of
   the subwindow are nlines lines and ncols columns. The subwindow is at
   position (begy, begx) on the screen. This position is relative to the
   screen, and not to the window orig. Changes made to either window
   will affect both. When using this routine, you will often need to
   call touchwin() before calling wrefresh().

   derwin() is the same as subwin(), except that begy and begx are
   relative to the origin of the window orig rather than the screen.
   There is no difference between subwindows and derived windows.

   mvderwin() moves a derived window (or subwindow) inside its parent
   window. The screen-relative parameters of the window are not changed.
   This routine is used to display different parts of the parent window
   at the same physical position on the screen.

   dupwin() creates an exact duplicate of the window win.

   wgetparent() returns the parent WINDOW pointer for subwindows, or NULL
   for windows having no parent.

   wsyncup() causes a touchwin() of all of the window's parents.

   If syncok() is called with a second argument of TRUE, this causes a
   wsyncup() to be called every time the window is changed.

   is_subwin() reports whether the specified window is a subwindow,
   created by subwin() or derwin().

   is_syncok() reports whether the specified window is in syncok mode.

   wcursyncup() causes the current cursor position of all of a window's
   ancestors to reflect the current cursor position of the current
   window.

   wsyncdown() causes a touchwin() of the current window if any of its
   parent's windows have been touched.

   resize_window() allows the user to resize an existing window. It
   returns the pointer to the new window, or NULL on failure.

   wresize() is an ncurses-compatible wrapper for resize_window(). Note
   that, unlike ncurses, it will NOT process any subwindows of the
   window. (However, you still can call it _on_ subwindows.) It returns
   OK or ERR.

   PDC_makenew() allocates all data for a new WINDOW * except the actual
   lines themselves. If it's unable to allocate memory for the window
   structure, it will free all allocated memory and return a NULL
   pointer.

   PDC_makelines() allocates the memory for the lines.

   PDC_sync() handles wrefresh() and wsyncup() calls when a window is
   changed.

### Return Value

   newwin(), subwin(), derwin() and dupwin() return a pointer to the new
   window, or NULL on failure. delwin(), mvwin(), mvderwin() and
   syncok() return OK or ERR. wsyncup(), wcursyncup() and wsyncdown()
   return nothing.

   is_subwin() and is_syncok() returns TRUE or FALSE.

### Errors

   It is an error to call resize_window() before calling initscr().
   Also, an error will be generated if we fail to create a newly sized
   replacement window for curscr, or stdscr. This could happen when
   increasing the window size. NOTE: If this happens, the previously
   successfully allocated windows are left alone; i.e., the resize is
   NOT cancelled for those windows.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   newwin                |    Y   |    Y    |   Y
   delwin                |    Y   |    Y    |   Y
   mvwin                 |    Y   |    Y    |   Y
   subwin                |    Y   |    Y    |   Y
   derwin                |    Y   |    Y    |   Y
   mvderwin              |    Y   |    Y    |   Y
   dupwin                |    Y   |    Y    |   Y
   wgetparent            |    -   |    Y    |   -
   wsyncup               |    Y   |    Y    |   Y
   syncok                |    Y   |    Y    |   Y
   is_subwin             |    -   |    Y    |   -
   is_syncok             |    -   |    Y    |   -
   wcursyncup            |    Y   |    Y    |   Y
   wsyncdown             |    Y   |    Y    |   Y
   wresize               |    -   |    Y    |   Y
   resize_window         |    -   |    -    |   -
   PDC_makelines         |    -   |    -    |   -
   PDC_makenew           |    -   |    -    |   -
   PDC_sync              |    -   |    -    |   -

**man-end****************************************************************/

WINDOW *PDC_makenew(int nlines, int ncols, int begy, int begx)
{
    WINDOW *win;

    PDC_LOG(("PDC_makenew() - called: lines %d cols %d begy %d begx %d\n",
             nlines, ncols, begy, begx));

    /* allocate the window structure itself */

    win = calloc(1, sizeof(WINDOW));
    if (!win)
        return win;

    /* allocate the line pointer array */

    win->_y = malloc(nlines * sizeof(chtype *));
    if (!win->_y)
    {
        free(win);
        return (WINDOW *)NULL;
    }

    /* allocate the minchng and maxchng arrays */

    win->_firstch = malloc(nlines * sizeof(int));
    if (!win->_firstch)
    {
        free(win->_y);
        free(win);
        return (WINDOW *)NULL;
    }

    win->_lastch = malloc(nlines * sizeof(int));
    if (!win->_lastch)
    {
        free(win->_firstch);
        free(win->_y);
        free(win);
        return (WINDOW *)NULL;
    }

    /* initialize window variables */

    win->_maxy = nlines;  /* real max screen size */
    win->_maxx = ncols;   /* real max screen size */
    win->_begy = begy;
    win->_begx = begx;
    win->_bkgd = ' ';     /* wrs 4/10/93 -- initialize background to blank */
    win->_clear = (bool) ((nlines == LINES) && (ncols == COLS));
    win->_bmarg = nlines - 1;
    win->_parx = win->_pary = -1;

    /* initialize pad variables*/

    win->_pad._pad_y = -1;
    win->_pad._pad_x = -1;
    win->_pad._pad_top = -1;
    win->_pad._pad_left = -1;
    win->_pad._pad_bottom = -1;
    win->_pad._pad_right = -1;

    /* init to say window all changed */

    touchwin(win);

    return win;
}

WINDOW *PDC_makelines(WINDOW *win)
{
    int i, j, nlines, ncols;

    PDC_LOG(("PDC_makelines() - called\n"));

    if (!win)
        return (WINDOW *)NULL;

    nlines = win->_maxy;
    ncols = win->_maxx;

    for (i = 0; i < nlines; i++)
    {
        win->_y[i] = malloc(ncols * sizeof(chtype));
        if (!win->_y[i])
        {
            /* if error, free all the data */

            for (j = 0; j < i; j++)
                free(win->_y[j]);

            free(win->_firstch);
            free(win->_lastch);
            free(win->_y);
            free(win);

            return (WINDOW *)NULL;
        }
    }

    return win;
}

void PDC_sync(WINDOW *win)
{
    PDC_LOG(("PDC_sync() - called:\n"));

    if (win->_immed)
        wrefresh(win);
    if (win->_sync)
        wsyncup(win);
}

WINDOW *newwin(int nlines, int ncols, int begy, int begx)
{
    WINDOW *win;

    PDC_LOG(("newwin() - called:lines=%d cols=%d begy=%d begx=%d\n",
             nlines, ncols, begy, begx));

    if (!nlines)
        nlines = LINES - begy;
    if (!ncols)
        ncols  = COLS  - begx;

    if (!SP || begy + nlines > SP->lines || begx + ncols > SP->cols)
        return (WINDOW *)NULL;

    win = PDC_makenew(nlines, ncols, begy, begx);
    if (win)
        win = PDC_makelines(win);

    if (win)
        werase(win);

    return win;
}

int delwin(WINDOW *win)
{
    int i;

    PDC_LOG(("delwin() - called\n"));

    if (!win)
        return ERR;

    /* subwindows use parents' lines */

    if (!(win->_flags & (_SUBWIN|_SUBPAD)))
        for (i = 0; i < win->_maxy && win->_y[i]; i++)
            if (win->_y[i])
                free(win->_y[i]);

    free(win->_firstch);
    free(win->_lastch);
    free(win->_y);
    free(win);

    return OK;
}

int mvwin(WINDOW *win, int y, int x)
{
    PDC_LOG(("mvwin() - called\n"));

    if (!win || (y + win->_maxy > LINES || y < 0)
             || (x + win->_maxx > COLS || x < 0))
        return ERR;

    win->_begy = y;
    win->_begx = x;
    touchwin(win);

    return OK;
}

WINDOW *subwin(WINDOW *orig, int nlines, int ncols, int begy, int begx)
{
    WINDOW *win;
    int i, j, k;

    PDC_LOG(("subwin() - called: lines %d cols %d begy %d begx %d\n",
             nlines, ncols, begy, begx));

    /* make sure window fits inside the original one */

    if (!orig || (begy < orig->_begy) || (begx < orig->_begx) ||
        (begy + nlines) > (orig->_begy + orig->_maxy) ||
        (begx + ncols) > (orig->_begx + orig->_maxx))
        return (WINDOW *)NULL;

    j = begy - orig->_begy;
    k = begx - orig->_begx;

    if (!nlines)
        nlines = orig->_maxy - j;
    if (!ncols)
        ncols  = orig->_maxx - k;

    win = PDC_makenew(nlines, ncols, begy, begx);
    if (!win)
        return (WINDOW *)NULL;

    /* initialize window variables */

    win->_attrs = orig->_attrs;
    win->_bkgd = orig->_bkgd;
    win->_leaveit = orig->_leaveit;
    win->_scroll = orig->_scroll;
    win->_nodelay = orig->_nodelay;
    win->_delayms = orig->_delayms;
    win->_use_keypad = orig->_use_keypad;
    win->_immed = orig->_immed;
    win->_sync = orig->_sync;
    win->_pary = j;
    win->_parx = k;
    win->_parent = orig;

    for (i = 0; i < nlines; i++, j++)
        win->_y[i] = orig->_y[j] + k;

    win->_flags |= _SUBWIN;

    return win;
}

WINDOW *derwin(WINDOW *orig, int nlines, int ncols, int begy, int begx)
{
    return subwin(orig, nlines, ncols, begy + orig->_begy, begx + orig->_begx);
}

int mvderwin(WINDOW *win, int pary, int parx)
{
    int i, j;
    WINDOW *mypar;

    if (!win || !(win->_parent))
        return ERR;

    mypar = win->_parent;

    if (pary < 0 || parx < 0 || (pary + win->_maxy) > mypar->_maxy ||
                                (parx + win->_maxx) > mypar->_maxx)
        return ERR;

    j = pary;

    for (i = 0; i < win->_maxy; i++)
        win->_y[i] = (mypar->_y[j++]) + parx;

    win->_pary = pary;
    win->_parx = parx;

    return OK;
}

WINDOW *dupwin(WINDOW *win)
{
    WINDOW *new;
    chtype *ptr, *ptr1;
    int nlines, ncols, begy, begx, i;

    if (!win)
        return (WINDOW *)NULL;

    nlines = win->_maxy;
    ncols = win->_maxx;
    begy = win->_begy;
    begx = win->_begx;

    new = PDC_makenew(nlines, ncols, begy, begx);
    if (new)
        new = PDC_makelines(new);

    if (!new)
        return (WINDOW *)NULL;

    /* copy the contents of win into new */

    for (i = 0; i < nlines; i++)
    {
        for (ptr = new->_y[i], ptr1 = win->_y[i];
             ptr < new->_y[i] + ncols; ptr++, ptr1++)
            *ptr = *ptr1;

        new->_firstch[i] = 0;
        new->_lastch[i] = ncols - 1;
    }

    new->_curx = win->_curx;
    new->_cury = win->_cury;
    new->_maxy = win->_maxy;
    new->_maxx = win->_maxx;
    new->_begy = win->_begy;
    new->_begx = win->_begx;
    new->_flags = win->_flags;
    new->_attrs = win->_attrs;
    new->_clear = win->_clear;
    new->_leaveit = win->_leaveit;
    new->_scroll = win->_scroll;
    new->_nodelay = win->_nodelay;
    new->_delayms = win->_delayms;
    new->_use_keypad = win->_use_keypad;
    new->_tmarg = win->_tmarg;
    new->_bmarg = win->_bmarg;
    new->_parx = win->_parx;
    new->_pary = win->_pary;
    new->_parent = win->_parent;
    new->_bkgd = win->_bkgd;
    new->_flags = win->_flags;

    return new;
}

WINDOW *wgetparent(const WINDOW *win)
{
    PDC_LOG(("wgetparent() - called\n"));

    if (!win || !win->_parent)
        return NULL;

    return win->_parent;
}

WINDOW *resize_window(WINDOW *win, int nlines, int ncols)
{
    WINDOW *new;
    int i, save_cury, save_curx, new_begy, new_begx;

    PDC_LOG(("resize_window() - called: nlines %d ncols %d\n",
             nlines, ncols));

    if (!win || !SP)
        return (WINDOW *)NULL;

    if (win->_flags & _SUBPAD)
    {
        new = subpad(win->_parent, nlines, ncols, win->_begy, win->_begx);
        if (!new)
            return (WINDOW *)NULL;
    }
    else if (win->_flags & _SUBWIN)
    {
        new = subwin(win->_parent, nlines, ncols, win->_begy, win->_begx);
        if (!new)
            return (WINDOW *)NULL;
    }
    else
    {
        if (win == SP->slk_winptr)
        {
            new_begy = SP->lines - SP->slklines;
            new_begx = 0;
        }
        else
        {
            new_begy = win->_begy;
            new_begx = win->_begx;
        }

        new = PDC_makenew(nlines, ncols, new_begy, new_begx);
        if (!new)
            return (WINDOW *)NULL;
    }

    save_curx = min(win->_curx, (new->_maxx - 1));
    save_cury = min(win->_cury, (new->_maxy - 1));

    if (!(win->_flags & (_SUBPAD|_SUBWIN)))
    {
        new = PDC_makelines(new);
        if (!new)
            return (WINDOW *)NULL;

        new->_bkgd = win->_bkgd;
        werase(new);

        copywin(win, new, 0, 0, 0, 0, min(win->_maxy, new->_maxy) - 1,
                min(win->_maxx, new->_maxx) - 1, FALSE);

        for (i = 0; i < win->_maxy && win->_y[i]; i++)
            if (win->_y[i])
                free(win->_y[i]);
    }

    new->_flags = win->_flags;
    new->_attrs = win->_attrs;
    new->_clear = win->_clear;
    new->_leaveit = win->_leaveit;
    new->_scroll = win->_scroll;
    new->_nodelay = win->_nodelay;
    new->_delayms = win->_delayms;
    new->_use_keypad = win->_use_keypad;
    new->_tmarg = (win->_tmarg > new->_maxy - 1) ? 0 : win->_tmarg;
    new->_bmarg = (win->_bmarg == win->_maxy - 1) ?
                  new->_maxy - 1 : min(win->_bmarg, (new->_maxy - 1));
    new->_parent = win->_parent;
    new->_immed = win->_immed;
    new->_sync = win->_sync;
    new->_bkgd = win->_bkgd;

    new->_curx = save_curx;
    new->_cury = save_cury;

    free(win->_firstch);
    free(win->_lastch);
    free(win->_y);

    *win = *new;
    free(new);

    return win;
}

int wresize(WINDOW *win, int nlines, int ncols)
{
    return (resize_window(win, nlines, ncols) ? OK : ERR);
}

void wsyncup(WINDOW *win)
{
    WINDOW *tmp;

    PDC_LOG(("wsyncup() - called\n"));

    for (tmp = win; tmp; tmp = tmp->_parent)
        touchwin(tmp);
}

int syncok(WINDOW *win, bool bf)
{
    PDC_LOG(("syncok() - called\n"));

    if (!win)
        return ERR;

    win->_sync = bf;

    return OK;
}

bool is_subwin(const WINDOW *win)
{
    PDC_LOG(("is_subwin() - called\n"));

    if (!win)
        return FALSE;

    return ((win->_flags & _SUBWIN) ? TRUE : FALSE);
}

bool is_syncok(const WINDOW *win)
{
    PDC_LOG(("is_syncok() - called\n"));

    if (!win)
        return FALSE;

    return win->_sync;
}

void wcursyncup(WINDOW *win)
{
    WINDOW *tmp;

    PDC_LOG(("wcursyncup() - called\n"));

    for (tmp = win; tmp && tmp->_parent; tmp = tmp->_parent)
        wmove(tmp->_parent, tmp->_pary + tmp->_cury, tmp->_parx + tmp->_curx);
}

void wsyncdown(WINDOW *win)
{
    WINDOW *tmp;

    PDC_LOG(("wsyncdown() - called\n"));

    for (tmp = win; tmp; tmp = tmp->_parent)
    {
        if (is_wintouched(tmp))
        {
            touchwin(win);
            break;
        }
    }
}
/* PDCurses */

/*man-start**************************************************************

clipboard
---------

### Synopsis

    int PDC_getclipboard(char **contents, long *length);
    int PDC_setclipboard(const char *contents, long length);
    int PDC_freeclipboard(char *contents);
    int PDC_clearclipboard(void);

### Description

   PDC_getclipboard() gets the textual contents of the system's
   clipboard. This function returns the contents of the clipboard in the
   contents argument. It is the responsibility of the caller to free the
   memory returned, via PDC_freeclipboard(). The length of the clipboard
   contents is returned in the length argument.

   PDC_setclipboard() copies the supplied text into the system's
   clipboard, emptying the clipboard prior to the copy.

   PDC_clearclipboard() clears the internal clipboard.

### Return Values

    PDC_CLIP_SUCCESS        the call was successful
    PDC_CLIP_MEMORY_ERROR   unable to allocate sufficient memory for
                            the clipboard contents
    PDC_CLIP_EMPTY          the clipboard contains no text
    PDC_CLIP_ACCESS_ERROR   no clipboard support

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   PDC_getclipboard      |    -   |    -    |   -
   PDC_setclipboard      |    -   |    -    |   -
   PDC_freeclipboard     |    -   |    -    |   -
   PDC_clearclipboard    |    -   |    -    |   -

**man-end****************************************************************/

int PDC_getclipboard(char **contents, long *length)
{
    PDC_LOG(("PDC_getclipboard() - called\n"));

    if (SDL_HasClipboardText() == SDL_FALSE)
        return PDC_CLIP_EMPTY;
    *contents = SDL_GetClipboardText();
    *length = strlen(*contents);

    return PDC_CLIP_SUCCESS;
}

int PDC_setclipboard(const char *contents, long length)
{
    PDC_LOG(("PDC_setclipboard() - called\n"));

    if (SDL_SetClipboardText(contents) != 0)
        return PDC_CLIP_ACCESS_ERROR;

    return PDC_CLIP_SUCCESS;
}

int PDC_freeclipboard(char *contents)
{
    PDC_LOG(("PDC_freeclipboard() - called\n"));

    SDL_free(contents);

    return PDC_CLIP_SUCCESS;
}

int PDC_clearclipboard(void)
{
    PDC_LOG(("PDC_clearclipboard() - called\n"));

    if (SDL_HasClipboardText() == SDL_TRUE)
    {
        SDL_SetClipboardText(NULL);
    }

    return PDC_CLIP_SUCCESS;
}
/* PDCurses */

#ifdef PDC_WIDE
// # include "../common/acsgr.h"
/* ACS Unicode mapping with punchouts for box characters et al. */

chtype acs_map[128] =
{
    PDC_ACS(0), PDC_ACS(1), PDC_ACS(2), PDC_ACS(3), PDC_ACS(4),
    PDC_ACS(5), PDC_ACS(6), PDC_ACS(7), PDC_ACS(8), PDC_ACS(9),
    PDC_ACS(10), PDC_ACS(11), PDC_ACS(12), PDC_ACS(13), PDC_ACS(14),
    PDC_ACS(15), PDC_ACS(16), PDC_ACS(17), PDC_ACS(18), PDC_ACS(19),
    PDC_ACS(20), PDC_ACS(21), PDC_ACS(22), PDC_ACS(23), PDC_ACS(24),
    PDC_ACS(25), PDC_ACS(26), PDC_ACS(27), PDC_ACS(28), PDC_ACS(29),
    PDC_ACS(30), PDC_ACS(31), ' ', '!', '"', '#', '$', '%', '&', '\'',
    '(', ')', '*',

    0x2192, 0x2190, 0x2191, 0x2193,

    '/',

    ACS_BLOCK,

    '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=',
    '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '[', '\\', ']', '^', '_',

    0x2666, 0x2592,

    'b', 'c', 'd', 'e',

    0x00b0, 0x00b1, 0x2591, 0x00a4, ACS_LRCORNER, ACS_URCORNER,
    ACS_ULCORNER, ACS_LLCORNER, ACS_PLUS, ACS_S1, ACS_S3, ACS_HLINE,
    ACS_S7, ACS_S9, ACS_LTEE, ACS_RTEE, ACS_BTEE, ACS_TTEE, ACS_VLINE,
    0x2264, 0x2265, 0x03c0, 0x2260, 0x00a3, 0x00b7,

    PDC_ACS(127)
};

#else
// # include "../common/acs437.h"
/* ACS definitions originally by jshumate@wrdis01.robins.af.mil -- these
   match code page 437 and compatible pages (CP850, CP852, etc.) */

chtype acs_map[128] =
{
    PDC_ACS(0), PDC_ACS(1), PDC_ACS(2), PDC_ACS(3), PDC_ACS(4),
    PDC_ACS(5), PDC_ACS(6), PDC_ACS(7), PDC_ACS(8), PDC_ACS(9),
    PDC_ACS(10), PDC_ACS(11), PDC_ACS(12), PDC_ACS(13), PDC_ACS(14),
    PDC_ACS(15), PDC_ACS(16), PDC_ACS(17), PDC_ACS(18), PDC_ACS(19),
    PDC_ACS(20), PDC_ACS(21), PDC_ACS(22), PDC_ACS(23), PDC_ACS(24),
    PDC_ACS(25), PDC_ACS(26), PDC_ACS(27), PDC_ACS(28), PDC_ACS(29),
    PDC_ACS(30), PDC_ACS(31), ' ', '!', '"', '#', '$', '%', '&', '\'',
    '(', ')', '*',

    PDC_ACS(0x1a), PDC_ACS(0x1b), PDC_ACS(0x18), PDC_ACS(0x19),

    '/',

    0xdb,

    '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=',
    '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '[', '\\', ']', '^', '_',

    PDC_ACS(0x04), 0xb1,

    'b', 'c', 'd', 'e',

    0xf8, 0xf1, 0xb0, PDC_ACS(0x0f), 0xd9, 0xbf, 0xda, 0xc0, 0xc5, 0x2d,
    0x2d, 0xc4, 0x2d, 0x5f, 0xc3, 0xb4, 0xc1, 0xc2, 0xb3, 0xf3, 0xf2,
    0xe3, 0xd8, 0x9c, 0xf9,

    PDC_ACS(127)
};

#endif

#define MAXRECT 200     /* maximum number of rects to queue up before
                           an update is forced; the number was chosen
                           arbitrarily */

static SDL_Rect uprect[MAXRECT];       /* table of rects to update */
static chtype oldch = (chtype)(-1);    /* current attribute */
static int rectcount = 0;              /* index into uprect */
static short foregr = -2, backgr = -2; /* current foreground, background */
static bool blinked_off = FALSE;

extern SDL_Window *pdc_window;
extern SDL_Surface *pdc_screen, *pdc_font, *pdc_icon,
            *pdc_back, *pdc_tileback;
int pdc_sheight, pdc_swidth, pdc_yoffset, pdc_xoffset;

SDL_Color pdc_color[PDC_MAXCOL];
Uint32 pdc_mapped[PDC_MAXCOL];
int pdc_fheight, pdc_fwidth, pdc_fthick, pdc_flastc;
bool pdc_own_window;


/* do the real updates on a delay */

void PDC_update_rects(void)
{
    int i;

    if (rectcount)
    {
        /* if the maximum number of rects has been reached, we're
           probably better off doing a full screen update */

        if (rectcount == MAXRECT)
            SDL_UpdateWindowSurface(pdc_window);
        else
        {
            int w = pdc_screen->w;
            int h = pdc_screen->h;

            for (i = 0; i < rectcount; i++)
            {
                if (uprect[i].x > w ||
                    uprect[i].y > h ||
                    !uprect[i].w || !uprect[i].h)
                {
                    if (i + 1 < rectcount)
                    {
                        memmove(uprect + i, uprect + i + 1,
                                (rectcount - i + 1) * sizeof(*uprect));
                        --i;
                    }
                    rectcount--;
                    continue;
                }

                if (uprect[i].x + uprect[i].w > w)
                    uprect[i].w = min(w, w - uprect[i].x);

                if (uprect[i].y + uprect[i].h > h)
                    uprect[i].h = min(h, h - uprect[i].y);
            }

            if (rectcount > 0)
                SDL_UpdateWindowSurfaceRects(pdc_window, uprect, rectcount);
        }

        rectcount = 0;
    }
}

/* set the font colors to match the chtype's attribute */

static void _set_attr(chtype ch)
{
    attr_t sysattrs = SP->termattrs;

#ifdef PDC_WIDE
    TTF_SetFontStyle(pdc_ttffont,
        ( ((ch & A_BOLD) && (sysattrs & A_BOLD)) ?
            TTF_STYLE_BOLD : 0) |
        ( ((ch & A_ITALIC) && (sysattrs & A_ITALIC)) ?
            TTF_STYLE_ITALIC : 0) );
#endif

    ch &= (A_COLOR|A_BOLD|A_BLINK|A_REVERSE);

    if (oldch != ch)
    {
        short newfg, newbg;

        if (SP->mono)
            return;

        pair_content(PAIR_NUMBER(ch), &newfg, &newbg);

        if ((ch & A_BOLD) && !(sysattrs & A_BOLD))
            newfg |= 8;
        if ((ch & A_BLINK) && !(sysattrs & A_BLINK))
            newbg |= 8;

        if (ch & A_REVERSE)
        {
            short tmp = newfg;
            newfg = newbg;
            newbg = tmp;
        }

        if (newfg != foregr)
        {
#ifndef PDC_WIDE
            SDL_SetPaletteColors(pdc_font->format->palette,
                                 pdc_color + newfg, pdc_flastc, 1);
#endif
            foregr = newfg;
        }

        if (newbg != backgr)
        {
#ifndef PDC_WIDE
            if (newbg == -1)
                SDL_SetColorKey(pdc_font, SDL_TRUE, 0);
            else
            {
                if (backgr == -1)
                    SDL_SetColorKey(pdc_font, SDL_FALSE, 0);

                SDL_SetPaletteColors(pdc_font->format->palette,
                                     pdc_color + newbg, 0, 1);
            }
#endif
            backgr = newbg;
        }

        oldch = ch;
    }
}

#ifdef PDC_WIDE

/* Draw some of the ACS_* "graphics" */

bool _grprint(chtype ch, SDL_Rect dest)
{
    Uint32 col = pdc_mapped[foregr];
    int hmid = (pdc_fheight - pdc_fthick) >> 1;
    int wmid = (pdc_fwidth - pdc_fthick) >> 1;

    switch (ch)
    {
    case ACS_ULCORNER:
        dest.h = pdc_fheight - hmid;
        dest.y += hmid;
        dest.w = pdc_fthick;
        dest.x += wmid;
        SDL_FillRect(pdc_screen, &dest, col);
        dest.w = pdc_fwidth - wmid;
        goto S1;
    case ACS_LLCORNER:
        dest.h = hmid;
        dest.w = pdc_fthick;
        dest.x += wmid;
        SDL_FillRect(pdc_screen, &dest, col);
        dest.w = pdc_fwidth - wmid;
        dest.y += hmid;
        goto S1;
    case ACS_URCORNER:
        dest.h = pdc_fheight - hmid;
        dest.w = pdc_fthick;
        dest.y += hmid;
        dest.x += wmid;
        SDL_FillRect(pdc_screen, &dest, col);
        dest.w = wmid;
        dest.x -= wmid;
        goto S1;
    case ACS_LRCORNER:
        dest.h = hmid + pdc_fthick;
        dest.w = pdc_fthick;
        dest.x += wmid;
        SDL_FillRect(pdc_screen, &dest, col);
        dest.w = wmid;
        dest.x -= wmid;
        dest.y += hmid;
        goto S1;
    case ACS_LTEE:
        dest.h = pdc_fthick;
        dest.w = pdc_fwidth - wmid;
        dest.x += wmid;
        dest.y += hmid;
        SDL_FillRect(pdc_screen, &dest, col);
        dest.w = pdc_fthick;
        dest.x -= wmid;
        goto VLINE;
    case ACS_RTEE:
        dest.w = wmid;
    case ACS_PLUS:
        dest.h = pdc_fthick;
        dest.y += hmid;
        SDL_FillRect(pdc_screen, &dest, col);
    VLINE:
        dest.h = pdc_fheight;
        dest.y -= hmid;
    case ACS_VLINE:
        dest.w = pdc_fthick;
        dest.x += wmid;
        goto DRAW;
    case ACS_TTEE:
        dest.h = pdc_fheight - hmid;
        dest.w = pdc_fthick;
        dest.x += wmid;
        dest.y += hmid;
        SDL_FillRect(pdc_screen, &dest, col);
        dest.w = pdc_fwidth;
        dest.x -= wmid;
        goto S1;
    case ACS_BTEE:
        dest.h = hmid;
        dest.w = pdc_fthick;
        dest.x += wmid;
        SDL_FillRect(pdc_screen, &dest, col);
        dest.w = pdc_fwidth;
        dest.x -= wmid;
    case ACS_HLINE:
        dest.y += hmid;
        goto S1;
    case ACS_S3:
        dest.y += hmid >> 1;
        goto S1;
    case ACS_S7:
        dest.y += hmid + (hmid >> 1);
        goto S1;
    case ACS_S9:
        dest.y += pdc_fheight - pdc_fthick;
    case ACS_S1:
    S1:
        dest.h = pdc_fthick;
    case ACS_BLOCK:
    DRAW:
        SDL_FillRect(pdc_screen, &dest, col);
        return TRUE;
    default: ;
    }

    return FALSE;  /* didn't draw it -- fall back to acs_map */
}

#endif

/* draw a cursor at (y, x) */

void PDC_gotoyx(int row, int col)
{
    SDL_Rect src, dest;
    chtype ch;
    int oldrow, oldcol;
#ifdef PDC_WIDE
    Uint16 chstr[2] = {0, 0};
#endif

    PDC_LOG(("PDC_gotoyx() - called: row %d col %d from row %d col %d\n",
             row, col, SP->cursrow, SP->curscol));

    oldrow = SP->cursrow;
    oldcol = SP->curscol;

    /* clear the old cursor */

    PDC_transform_line(oldrow, oldcol, 1, curscr->_y[oldrow] + oldcol);

    if (!SP->visibility)
        return;

    /* draw a new cursor by overprinting the existing character in
       reverse, either the full cell (when visibility == 2) or the
       lowest quarter of it (when visibility == 1) */

    ch = curscr->_y[row][col] ^ A_REVERSE;

    _set_attr(ch);

    src.h = (SP->visibility == 1) ? pdc_fheight >> 2 : pdc_fheight;
    src.w = pdc_fwidth;

    dest.y = (row + 1) * pdc_fheight - src.h + pdc_yoffset;
    dest.x = col * pdc_fwidth + pdc_xoffset;
    dest.h = src.h;
    dest.w = src.w;

#ifdef PDC_WIDE
    SDL_FillRect(pdc_screen, &dest, pdc_mapped[backgr]);

    if (!(SP->visibility == 2 && (ch & A_ALTCHARSET && !(ch & 0xff80)) &&
        _grprint(ch & (0x7f | A_ALTCHARSET), dest)))
    {
        if (ch & A_ALTCHARSET && !(ch & 0xff80))
            ch = acs_map[ch & 0x7f];

        chstr[0] = ch & A_CHARTEXT;

        pdc_font = TTF_RenderUNICODE_Blended(pdc_ttffont, chstr,
                                             pdc_color[foregr]);
        if (pdc_font)
        {
            int center = pdc_fwidth > pdc_font->w ?
                        (pdc_fwidth - pdc_font->w) >> 1 : 0;
            src.x = 0;
            src.y = pdc_fheight - src.h;
            dest.x += center;
            SDL_BlitSurface(pdc_font, &src, pdc_screen, &dest);
            dest.x -= center;
            SDL_FreeSurface(pdc_font);
            pdc_font = NULL;
        }
    }
#else
    if (ch & A_ALTCHARSET && !(ch & 0xff80))
        ch = acs_map[ch & 0x7f];

    src.x = (ch & 0xff) % 32 * pdc_fwidth;
    src.y = (ch & 0xff) / 32 * pdc_fheight + (pdc_fheight - src.h);

    SDL_BlitSurface(pdc_font, &src, pdc_screen, &dest);
#endif

    if (oldrow != row || oldcol != col)
    {
        if (rectcount == MAXRECT)
            PDC_update_rects();

        uprect[rectcount++] = dest;
    }
}

void _new_packet(attr_t attr, int lineno, int x, int len, const chtype *srcp)
{
    SDL_Rect src, dest, lastrect;
    int j;
#ifdef PDC_WIDE
    Uint16 chstr[2] = {0, 0};
#endif
    attr_t sysattrs = SP->termattrs;
    short hcol = SP->line_color;
    bool blink = blinked_off && (attr & A_BLINK) && (sysattrs & A_BLINK);

    if (rectcount == MAXRECT)
        PDC_update_rects();

#ifdef PDC_WIDE
    src.x = 0;
    src.y = 0;
#endif
    src.h = pdc_fheight;
    src.w = pdc_fwidth;

    dest.y = pdc_fheight * lineno + pdc_yoffset;
    dest.x = pdc_fwidth * x + pdc_xoffset;
    dest.h = pdc_fheight;
    dest.w = pdc_fwidth * len;

    /* if the previous rect was just above this one, with the same width
       and horizontal position, then merge the new one with it instead
       of adding a new entry */

    if (rectcount)
        lastrect = uprect[rectcount - 1];

    if (rectcount && lastrect.x == dest.x && lastrect.w == dest.w)
    {
        if (lastrect.y + lastrect.h == dest.y)
            uprect[rectcount - 1].h = lastrect.h + pdc_fheight;
        else
            if (lastrect.y != dest.y)
                uprect[rectcount++] = dest;
    }
    else
        uprect[rectcount++] = dest;

    _set_attr(attr);

    if (backgr == -1)
        SDL_BlitSurface(pdc_tileback, &dest, pdc_screen, &dest);
#ifdef PDC_WIDE
    else
        SDL_FillRect(pdc_screen, &dest, pdc_mapped[backgr]);
#endif

    if (hcol == -1)
        hcol = foregr;

    for (j = 0; j < len; j++)
    {
        chtype ch = srcp[j];

        if (blink)
            ch = ' ';

        dest.w = pdc_fwidth;

        if (ch & A_ALTCHARSET && !(ch & 0xff80))
        {
#ifdef PDC_WIDE
            if (_grprint(ch & (0x7f | A_ALTCHARSET), dest))
            {
                dest.x += pdc_fwidth;
                continue;
            }
#endif
            ch = acs_map[ch & 0x7f];
        }

#ifdef PDC_WIDE
        ch &= A_CHARTEXT;

        if (ch != ' ')
        {
            if (chstr[0] != ch)
            {
                chstr[0] = ch;

                if (pdc_font)
                    SDL_FreeSurface(pdc_font);

                pdc_font = TTF_RenderUNICODE_Blended(pdc_ttffont, chstr,
                                                     pdc_color[foregr]);
            }

            if (pdc_font)
            {
                int center = pdc_fwidth > pdc_font->w ?
                    (pdc_fwidth - pdc_font->w) >> 1 : 0;
                dest.x += center;
                SDL_BlitSurface(pdc_font, &src, pdc_screen, &dest);
                dest.x -= center;
            }
        }
#else
        src.x = (ch & 0xff) % 32 * pdc_fwidth;
        src.y = (ch & 0xff) / 32 * pdc_fheight;

        SDL_BlitSurface(pdc_font, &src, pdc_screen, &dest);
#endif

        if (!blink && (attr & (A_LEFT | A_RIGHT)))
        {
            dest.w = pdc_fthick;

            if (attr & A_LEFT)
                SDL_FillRect(pdc_screen, &dest, pdc_mapped[hcol]);

            if (attr & A_RIGHT)
            {
                dest.x += pdc_fwidth - pdc_fthick;
                SDL_FillRect(pdc_screen, &dest, pdc_mapped[hcol]);
                dest.x -= pdc_fwidth - pdc_fthick;
            }
        }

        dest.x += pdc_fwidth;
    }

#ifdef PDC_WIDE
    if (pdc_font)
    {
        SDL_FreeSurface(pdc_font);
        pdc_font = NULL;
    }
#endif

    if (!blink && (attr & A_UNDERLINE))
    {
        dest.y += pdc_fheight - pdc_fthick;
        dest.x = pdc_fwidth * x + pdc_xoffset;
        dest.h = pdc_fthick;
        dest.w = pdc_fwidth * len;

        SDL_FillRect(pdc_screen, &dest, pdc_mapped[hcol]);
    }
}

/* update the given physical line to look like the corresponding line in
   curscr */

void PDC_transform_line(int lineno, int x, int len, const chtype *srcp)
{
    attr_t old_attr, attr;
    int i, j;

    PDC_LOG(("PDC_transform_line() - called: lineno=%d\n", lineno));

    old_attr = *srcp & (A_ATTRIBUTES ^ A_ALTCHARSET);

    for (i = 1, j = 1; j < len; i++, j++)
    {
        attr = srcp[i] & (A_ATTRIBUTES ^ A_ALTCHARSET);

        if (attr != old_attr)
        {
            _new_packet(old_attr, lineno, x, i, srcp);
            old_attr = attr;
            srcp += i;
            x += i;
            i = 0;
        }
    }

    _new_packet(old_attr, lineno, x, i, srcp);
}

static Uint32 _blink_timer(Uint32 interval, void *param)
{
    SDL_Event event;

    event.type = SDL_USEREVENT;
    SDL_PushEvent(&event);
    return(interval);
}

void PDC_blink_text(void)
{
    static SDL_TimerID blinker_id = 0;
    int i, j, k;

    oldch = (chtype)(-1);

    if (!(SP->termattrs & A_BLINK))
    {
        SDL_RemoveTimer(blinker_id);
        blinker_id = 0;
    }
    else if (!blinker_id)
    {
        blinker_id = SDL_AddTimer(500, _blink_timer, NULL);
        blinked_off = TRUE;
    }

    blinked_off = !blinked_off;

    for (i = 0; i < SP->lines; i++)
    {
        const chtype *srcp = curscr->_y[i];

        for (j = 0; j < SP->cols; j++)
            if (srcp[j] & A_BLINK)
            {
                k = j;
                while (k < SP->cols && (srcp[k] & A_BLINK))
                    k++;
                PDC_transform_line(i, j, k - j, srcp + j);
                j = k;
            }
    }

    oldch = (chtype)(-1);

    PDC_doupdate();
}

void PDC_doupdate(void)
{
    PDC_update_rects();
}

void PDC_pump_and_peep(void)
{
    SDL_Event event;

    if (SDL_PollEvent(&event))
    {
        if (SDL_WINDOWEVENT == event.type &&
            (SDL_WINDOWEVENT_RESTORED == event.window.event ||
             SDL_WINDOWEVENT_EXPOSED == event.window.event ||
             SDL_WINDOWEVENT_SHOWN == event.window.event))
        {
            SDL_UpdateWindowSurface(pdc_window);
            rectcount = 0;
        }
        else
            SDL_PushEvent(&event);
    }
}
/* PDCurses */

/* get the cursor size/shape */

int PDC_get_cursor_mode(void)
{
    PDC_LOG(("PDC_get_cursor_mode() - called\n"));

    return 0;
}

/* return number of screen rows */

int PDC_get_rows(void)
{
    PDC_LOG(("PDC_get_rows() - called\n"));

    return pdc_sheight / pdc_fheight;
}

/* return width of screen/viewport */

int PDC_get_columns(void)
{
    PDC_LOG(("PDC_get_columns() - called\n"));

    return pdc_swidth / pdc_fwidth;
}
/* PDCurses */

static SDL_Event event;
static SDL_Keycode oldkey;
static MOUSE_STATUS old_mouse_status;

static struct
{
    SDL_Keycode keycode;
    bool numkeypad;
    unsigned short normal;
    unsigned short shifted;
    unsigned short control;
    unsigned short alt;
} key_table[] =
{
/* keycode      keypad  normal       shifted       control      alt*/
 {SDLK_LEFT,    FALSE,  KEY_LEFT,    KEY_SLEFT,    CTL_LEFT,    ALT_LEFT},
 {SDLK_RIGHT,   FALSE,  KEY_RIGHT,   KEY_SRIGHT,   CTL_RIGHT,   ALT_RIGHT},
 {SDLK_UP,      FALSE,  KEY_UP,      KEY_SUP,      CTL_UP,      ALT_UP},
 {SDLK_DOWN,    FALSE,  KEY_DOWN,    KEY_SDOWN,    CTL_DOWN,    ALT_DOWN},
 {SDLK_HOME,    FALSE,  KEY_HOME,    KEY_SHOME,    CTL_HOME,    ALT_HOME},
 {SDLK_END,     FALSE,  KEY_END,     KEY_SEND,     CTL_END,     ALT_END},
 {SDLK_PAGEUP,  FALSE,  KEY_PPAGE,   KEY_SPREVIOUS,CTL_PGUP,    ALT_PGUP},
 {SDLK_PAGEDOWN,FALSE,  KEY_NPAGE,   KEY_SNEXT,    CTL_PGDN,    ALT_PGDN},
 {SDLK_INSERT,  FALSE,  KEY_IC,      KEY_SIC,      CTL_INS,     ALT_INS},
 {SDLK_DELETE,  FALSE,  KEY_DC,      KEY_SDC,      CTL_DEL,     ALT_DEL},
 {SDLK_F1,      FALSE,  KEY_F(1),    KEY_F(13),    KEY_F(25),   KEY_F(37)},
 {SDLK_F2,      FALSE,  KEY_F(2),    KEY_F(14),    KEY_F(26),   KEY_F(38)},
 {SDLK_F3,      FALSE,  KEY_F(3),    KEY_F(15),    KEY_F(27),   KEY_F(39)},
 {SDLK_F4,      FALSE,  KEY_F(4),    KEY_F(16),    KEY_F(28),   KEY_F(40)},
 {SDLK_F5,      FALSE,  KEY_F(5),    KEY_F(17),    KEY_F(29),   KEY_F(41)},
 {SDLK_F6,      FALSE,  KEY_F(6),    KEY_F(18),    KEY_F(30),   KEY_F(42)},
 {SDLK_F7,      FALSE,  KEY_F(7),    KEY_F(19),    KEY_F(31),   KEY_F(43)},
 {SDLK_F8,      FALSE,  KEY_F(8),    KEY_F(20),    KEY_F(32),   KEY_F(44)},
 {SDLK_F9,      FALSE,  KEY_F(9),    KEY_F(21),    KEY_F(33),   KEY_F(45)},
 {SDLK_F10,     FALSE,  KEY_F(10),   KEY_F(22),    KEY_F(34),   KEY_F(46)},
 {SDLK_F11,     FALSE,  KEY_F(11),   KEY_F(23),    KEY_F(35),   KEY_F(47)},
 {SDLK_F12,     FALSE,  KEY_F(12),   KEY_F(24),    KEY_F(36),   KEY_F(48)},
 {SDLK_F13,     FALSE,  KEY_F(13),   KEY_F(25),    KEY_F(37),   KEY_F(49)},
 {SDLK_F14,     FALSE,  KEY_F(14),   KEY_F(26),    KEY_F(38),   KEY_F(50)},
 {SDLK_F15,     FALSE,  KEY_F(15),   KEY_F(27),    KEY_F(39),   KEY_F(51)},
 {SDLK_BACKSPACE,FALSE, 0x08,        0x08,         CTL_BKSP,    ALT_BKSP},
 {SDLK_TAB,     FALSE,  0x09,        KEY_BTAB,     CTL_TAB,     ALT_TAB},
 {SDLK_PRINTSCREEN,FALSE,KEY_PRINT,  KEY_SPRINT,   KEY_PRINT,   KEY_PRINT},
 {SDLK_PAUSE,   FALSE,  KEY_SUSPEND, KEY_SSUSPEND, KEY_SUSPEND, KEY_SUSPEND},
 {SDLK_CLEAR,   FALSE,  KEY_CLEAR,   KEY_CLEAR,    KEY_CLEAR,   KEY_CLEAR},
 {SDLK_HELP,    FALSE,  KEY_HELP,    KEY_SHELP,    KEY_LHELP,   KEY_HELP},
 {SDLK_MENU,    FALSE,  KEY_OPTIONS, KEY_SOPTIONS, KEY_OPTIONS, KEY_OPTIONS},
 {SDLK_ESCAPE,  FALSE,  0x1B,        0x1B,         0x1B,        ALT_ESC},
 {SDLK_KP_ENTER,TRUE,   PADENTER,    PADENTER,     CTL_PADENTER,ALT_PADENTER},
 {SDLK_KP_PLUS, TRUE,   PADPLUS,     '+',          CTL_PADPLUS, ALT_PADPLUS},
 {SDLK_KP_MINUS,TRUE,   PADMINUS,    '-',          CTL_PADMINUS,ALT_PADMINUS},
 {SDLK_KP_MULTIPLY,TRUE,PADSTAR,     '*',          CTL_PADSTAR, ALT_PADSTAR},
 {SDLK_KP_DIVIDE,TRUE,  PADSLASH,    '/',          CTL_PADSLASH,ALT_PADSLASH},
 {SDLK_KP_PERIOD,TRUE,  PADSTOP,     '.',          CTL_PADSTOP, ALT_PADSTOP},
 {SDLK_KP_0,    TRUE,   PAD0,        '0',          CTL_PAD0,    ALT_PAD0},
 {SDLK_KP_1,    TRUE,   KEY_C1,      '1',          CTL_PAD1,    ALT_PAD1},
 {SDLK_KP_2,    TRUE,   KEY_C2,      '2',          CTL_PAD2,    ALT_PAD2},
 {SDLK_KP_3,    TRUE,   KEY_C3,      '3',          CTL_PAD3,    ALT_PAD3},
 {SDLK_KP_4,    TRUE,   KEY_B1,      '4',          CTL_PAD4,    ALT_PAD4},
 {SDLK_KP_5,    TRUE,   KEY_B2,      '5',          CTL_PAD5,    ALT_PAD5},
 {SDLK_KP_6,    TRUE,   KEY_B3,      '6',          CTL_PAD6,    ALT_PAD6},
 {SDLK_KP_7,    TRUE,   KEY_A1,      '7',          CTL_PAD7,    ALT_PAD7},
 {SDLK_KP_8,    TRUE,   KEY_A2,      '8',          CTL_PAD8,    ALT_PAD8},
 {SDLK_KP_9,    TRUE,   KEY_A3,      '9',          CTL_PAD9,    ALT_PAD9},
 {0,            0,      0,           0,            0,           0}
};

void PDC_set_keyboard_binary(bool on)
{
    PDC_LOG(("PDC_set_keyboard_binary() - called\n"));
}

/* check if a key or mouse event is waiting */

bool PDC_check_key(void)
{
    int haveevent;

    PDC_pump_and_peep();

    /* SDL_TEXTINPUT can return multiple chars from the IME which we
       should handle before polling for additional events. */

    if (event.type == SDL_TEXTINPUT && event.text.text[0])
        haveevent = 1;
    else
        haveevent = SDL_PollEvent(&event);

    return haveevent;
}

#ifdef PDC_WIDE
static int _utf8_to_unicode(char *chstr, size_t *b)
{
    int i, bytes, unicode;
    unsigned char byte = chstr[0];

    if (byte > 0xf0)
    {
        bytes = 4;
        unicode = byte & 0x7;
    }
    else if (byte > 0xe0)
    {
        bytes = 3;
        unicode = byte & 0xf;
    }
    else if (byte > 0xc0)
    {
        bytes = 2;
        unicode = byte & 0x1f;
    }
    else if (byte > 0x80) {
        /* starts with a continuation byte; invalid character */
        return -1;
    }
    else
    {
        bytes = 1;
        unicode = byte;
    }

    for (i = 1; i < bytes; i++)
        unicode = (unicode << 6) + (chstr[i] & 0x3f);

    *b = bytes;
    return unicode;
}
#endif

/* Handle ALT and CTRL sequences */
static int _handle_alt_keys(int key)
{
    if (key > 0x7f)
        return key;

    if (SP->key_modifiers & PDC_KEY_MODIFIER_CONTROL)
    {
        if (key >= 'A' && key <= 'Z') key -= 64;
        if (key >= 'a' && key <= 'z') key -= 96;
    }
    else if (SP->key_modifiers & PDC_KEY_MODIFIER_ALT)
    {
        if (key >= 'A' && key <= 'Z')
        {
            key += ALT_A - 'A';
            SP->key_code = TRUE;
        } else if (key >= 'a' && key <= 'z')
        {
            key += ALT_A - 'a';
            SP->key_code = TRUE;
        } else if (key >= '0' && key <= '9')
        {
            key += ALT_0 - '0';
            SP->key_code = TRUE;
        }
    }

    return key;
}

static int _process_key_event(void)
{
    int i, key = 0;

#ifdef PDC_WIDE
    size_t bytes;
#endif

    SP->key_code = FALSE;

    if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                SP->key_modifiers &= ~PDC_KEY_MODIFIER_CONTROL;
                break;
            case SDLK_LALT:
            case SDLK_RALT:
                SP->key_modifiers &= ~PDC_KEY_MODIFIER_ALT;
                break;
            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
                SP->key_modifiers &= ~PDC_KEY_MODIFIER_SHIFT;
                break;
        }

        if (!(SDL_GetModState() & KMOD_NUM))
            SP->key_modifiers &= ~PDC_KEY_MODIFIER_NUMLOCK;

        if (SP->return_key_modifiers && event.key.keysym.sym == oldkey)
        {
            SP->key_code = TRUE;
            switch (event.key.keysym.sym)
            {
            case SDLK_RSHIFT:
                return KEY_SHIFT_R;
            case SDLK_LSHIFT:
                return KEY_SHIFT_L;
            case SDLK_RCTRL:
                return KEY_CONTROL_R;
            case SDLK_LCTRL:
                return KEY_CONTROL_L;
            case SDLK_RALT:
                return KEY_ALT_R;
            case SDLK_LALT:
                return KEY_ALT_L;
            default:
                break;
            }
        }

        SP->key_code = FALSE;
        return -1;
    }
    else if (event.type == SDL_TEXTINPUT)
    {
#ifdef PDC_WIDE
        if ((key = _utf8_to_unicode(event.text.text, &bytes)) == -1)
        {
            event.text.text[0] = '\0';
        }
        else
        {
            memmove(event.text.text, event.text.text + bytes,
                    strlen(event.text.text) - bytes + 1);
        }
        return _handle_alt_keys(key);
#else
        key = (unsigned char)event.text.text[0];
        memmove(event.text.text, event.text.text + 1,
                strlen(event.text.text));
        return key > 0x7f ? -1 : _handle_alt_keys(key);
#endif
    }

    oldkey = event.key.keysym.sym;
    if (SDL_GetModState() & KMOD_NUM)
        SP->key_modifiers |= PDC_KEY_MODIFIER_NUMLOCK;

    switch (event.key.keysym.sym)
    {
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            SP->key_modifiers |= PDC_KEY_MODIFIER_CONTROL;
            break;
        case SDLK_LALT:
        case SDLK_RALT:
            SP->key_modifiers |= PDC_KEY_MODIFIER_ALT;
            break;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            SP->key_modifiers |= PDC_KEY_MODIFIER_SHIFT;
            break;
        case SDLK_RETURN:
            return 0x0d;
        default:
            key = event.key.keysym.sym;
    }

    for (i = 0; key_table[i].keycode; i++)
    {
        if (key_table[i].keycode == event.key.keysym.sym)
        {
            if ((event.key.keysym.mod & KMOD_SHIFT) ||
                (key_table[i].numkeypad && (event.key.keysym.mod & KMOD_NUM)))
            {
                key = key_table[i].shifted;
            }
            else if (event.key.keysym.mod & KMOD_CTRL)
            {
                key = key_table[i].control;
            }
            else if (event.key.keysym.mod & KMOD_ALT)
            {
                key = key_table[i].alt;
            }
            else
            {
                /* To get here, we ignore all other modifiers */
                key = key_table[i].normal;
            }

            SP->key_code = (key > 0x100);
            return key;
        }
    }

    /* SDL with TextInput ignores keys with CTRL */
    if (key && SP->key_modifiers & PDC_KEY_MODIFIER_CONTROL)
        return _handle_alt_keys(key);
    return -1;
}

static int _process_mouse_event(void)
{
    SDL_Keymod keymods;
    short shift_flags = 0;

    memset(&SP->mouse_status, 0, sizeof(MOUSE_STATUS));

    keymods = SDL_GetModState();

    if (keymods & KMOD_SHIFT)
        shift_flags |= BUTTON_SHIFT;

    if (keymods & KMOD_CTRL)
        shift_flags |= BUTTON_CONTROL;

    if (keymods & KMOD_ALT)
        shift_flags |= BUTTON_ALT;

    if (event.type == SDL_MOUSEMOTION)
    {
        int i;

        SP->mouse_status.x = (event.motion.x - pdc_xoffset) / pdc_fwidth;
        SP->mouse_status.y = (event.motion.y - pdc_yoffset) / pdc_fheight;

        if (!event.motion.state ||
           (SP->mouse_status.x == old_mouse_status.x &&
            SP->mouse_status.y == old_mouse_status.y))
            return -1;

        SP->mouse_status.changes = PDC_MOUSE_MOVED;

        for (i = 0; i < 3; i++)
        {
            if (event.motion.state & SDL_BUTTON(i + 1))
            {
                SP->mouse_status.button[i] = BUTTON_MOVED | shift_flags;
                SP->mouse_status.changes |= (1 << i);
            }
        }
    }
    else if (event.type == SDL_MOUSEWHEEL)
    {
        int x, y;

        SDL_GetMouseState(&x, &y);
        SP->mouse_status.x = (x - pdc_xoffset) / pdc_fwidth;
        SP->mouse_status.y = (y - pdc_yoffset) / pdc_fheight;

        if (event.wheel.y > 0)
            SP->mouse_status.changes = PDC_MOUSE_WHEEL_UP;
        else if (event.wheel.y < 0)
            SP->mouse_status.changes = PDC_MOUSE_WHEEL_DOWN;
        else if (event.wheel.x > 0)
            SP->mouse_status.changes = PDC_MOUSE_WHEEL_RIGHT;
        else if (event.wheel.x < 0)
            SP->mouse_status.changes = PDC_MOUSE_WHEEL_LEFT;
        else
            return -1;

        SP->key_code = TRUE;
        return KEY_MOUSE;
    }
    else
    {
        short action = (event.button.state == SDL_PRESSED) ?
                       BUTTON_PRESSED : BUTTON_RELEASED;
        Uint8 btn = event.button.button;

        if (btn < 1 || btn > 3)
            return -1;

        /* check for a click -- a press followed immediately by a release */

        if (action == BUTTON_PRESSED && SP->mouse_wait)
        {
            SDL_Event rel;

            napms(SP->mouse_wait);

            if (SDL_PollEvent(&rel))
            {
                if (rel.type == SDL_MOUSEBUTTONUP && rel.button.button == btn)
                    action = BUTTON_CLICKED;
                else
                    SDL_PushEvent(&rel);
            }
        }

        SP->mouse_status.x = (event.button.x - pdc_xoffset) / pdc_fwidth;
        SP->mouse_status.y = (event.button.y - pdc_yoffset) / pdc_fheight;

        btn--;

        SP->mouse_status.button[btn] = action | shift_flags;
        SP->mouse_status.changes = (1 << btn);
    }

    old_mouse_status = SP->mouse_status;

    SP->key_code = TRUE;
    return KEY_MOUSE;
}

/* return the next available key or mouse event */

int PDC_get_key(void)
{
    switch (event.type)
    {
    case SDL_QUIT:
        exit(1);
    case SDL_WINDOWEVENT:
        if (SDL_WINDOWEVENT_SIZE_CHANGED == event.window.event)
        {
            pdc_screen = SDL_GetWindowSurface(pdc_window);
            pdc_sheight = pdc_screen->h - pdc_xoffset;
            pdc_swidth = pdc_screen->w - pdc_yoffset;
            touchwin(curscr);
            wrefresh(curscr);

            if (!SP->resized)
            {
                SP->resized = TRUE;
                SP->key_code = TRUE;
                return KEY_RESIZE;
            }
        }
        break;
    case SDL_MOUSEMOTION:
        SDL_ShowCursor(SDL_ENABLE);
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEWHEEL:
        oldkey = SDLK_SPACE;
        return _process_mouse_event();
    case SDL_KEYUP:
    case SDL_KEYDOWN:
    case SDL_TEXTINPUT:
        PDC_mouse_set();
        return _process_key_event();
    case SDL_USEREVENT:
        PDC_blink_text();
    }

    return -1;
}

/* discard any pending keyboard or mouse input -- this is the core
   routine for flushinp() */

void PDC_flushinp(void)
{
    PDC_LOG(("PDC_flushinp() - called\n"));

    while (PDC_check_key())
        PDC_get_key();
}

bool PDC_has_mouse(void)
{
    return TRUE;
}

int PDC_mouse_set(void)
{
    SDL_ShowCursor(SP->_trap_mbe ? SDL_ENABLE : SDL_DISABLE);

    return OK;
}

int PDC_modifiers_set(void)
{
    return OK;
}
/* PDCurses */

#ifndef PDC_WIDE
// # include "../common/font437.h"
/* Default font for SDL (narrow build) -- this is simply a 256x128x1
   BMP, in #include'able form. The font is 8x16, code page 437, and is
   based on the pc8x16s.bdf font from the vgafonts.tar.gz package, by
   "Myrlin". */

unsigned char font437[] =
{
    0x42, 0x4d, 0x3e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e,
    0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x00, 0x12, 0x0b, 0x00, 0x00, 0x12, 0x0b,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x76, 0xf8, 0xc0, 0x6c, 0xfe, 0x70, 0x7c, 0x18,
    0x7e, 0x38, 0xee, 0x3c, 0x00, 0xc0, 0x1c, 0xc6, 0x00, 0x7e, 0x7e,
    0x7e, 0x18, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00,
    0x00, 0x00, 0xdc, 0xcc, 0xc0, 0x6c, 0xc6, 0xd8, 0x66, 0x18, 0x18,
    0x6c, 0x6c, 0x66, 0x00, 0x60, 0x30, 0xc6, 0xfe, 0x00, 0x00, 0x00,
    0x18, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e,
    0x00, 0xd8, 0xcc, 0xc0, 0x6c, 0x60, 0xd8, 0x66, 0x18, 0x3c, 0xc6,
    0x6c, 0x66, 0x7e, 0x7e, 0x60, 0xc6, 0x00, 0x00, 0x30, 0x0c, 0x18,
    0xd8, 0x18, 0xdc, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x7e, 0x00,
    0xd8, 0xcc, 0xc0, 0x6c, 0x30, 0xd8, 0x66, 0x18, 0x66, 0xc6, 0x6c,
    0x66, 0xdb, 0xf3, 0x60, 0xc6, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x00, 0x76, 0x00, 0x18, 0x00, 0x6c, 0x00, 0x00, 0x7e, 0x00, 0xd8,
    0xc8, 0xc0, 0x6c, 0x18, 0xd8, 0x66, 0x18, 0x66, 0xc6, 0x6c, 0x66,
    0xdb, 0xdb, 0x60, 0xc6, 0xfe, 0x18, 0x0c, 0x30, 0x18, 0x18, 0x7e,
    0x00, 0x00, 0x18, 0x18, 0xec, 0x00, 0x00, 0x7e, 0x00, 0xdc, 0xdc,
    0xc0, 0x6c, 0x18, 0xd8, 0x66, 0x18, 0x66, 0xfe, 0xc6, 0x3e, 0xdb,
    0xdb, 0x7c, 0xc6, 0x00, 0x7e, 0x06, 0x60, 0x18, 0x18, 0x00, 0xdc,
    0x00, 0x00, 0x00, 0x0c, 0x36, 0x7e, 0x7e, 0x00, 0x76, 0xce, 0xc0,
    0xfe, 0x30, 0x7e, 0x66, 0xdc, 0x66, 0xc6, 0xc6, 0x0c, 0x7e, 0x7e,
    0x60, 0xc6, 0x00, 0x18, 0x0c, 0x30, 0x18, 0x18, 0x18, 0x76, 0x00,
    0x00, 0x00, 0x0c, 0x36, 0x32, 0x7e, 0x00, 0x00, 0xc6, 0xc6, 0x00,
    0x60, 0x00, 0x00, 0x76, 0x3c, 0xc6, 0xc6, 0x18, 0x00, 0x06, 0x60,
    0xc6, 0xfe, 0x18, 0x18, 0x18, 0x1b, 0x18, 0x00, 0x00, 0x38, 0x00,
    0x00, 0x0c, 0x36, 0x18, 0x7e, 0x00, 0x00, 0xce, 0xc6, 0x00, 0xc6,
    0x00, 0x00, 0x00, 0x18, 0x6c, 0x6c, 0x30, 0x00, 0x03, 0x30, 0x7c,
    0x00, 0x00, 0x30, 0x0c, 0x1b, 0x18, 0x00, 0x00, 0x6c, 0x00, 0x00,
    0x0c, 0x36, 0x0c, 0x00, 0x00, 0x00, 0x7c, 0xfe, 0x00, 0xfe, 0x00,
    0x00, 0x00, 0x7e, 0x38, 0x38, 0x1e, 0x00, 0x00, 0x1c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0e, 0x18, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x0c,
    0x36, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x38, 0x00, 0x00, 0x0f, 0x6c,
    0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00,
    0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 0x00, 0x18, 0x36, 0x00,
    0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xff, 0xff, 0xf0, 0x0f,
    0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36,
    0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 0x00, 0x18, 0x36, 0x00, 0x00,
    0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xff, 0xff, 0xf0, 0x0f, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00,
    0x36, 0x36, 0x00, 0x36, 0x00, 0x00, 0x18, 0x36, 0x00, 0x00, 0x18,
    0x36, 0x36, 0x18, 0x00, 0x18, 0xff, 0xff, 0xf0, 0x0f, 0x00, 0x00,
    0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36,
    0x36, 0x00, 0x36, 0x00, 0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36,
    0x36, 0x18, 0x00, 0x18, 0xff, 0xff, 0xf0, 0x0f, 0x00, 0x00, 0x00,
    0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36,
    0x00, 0x36, 0x00, 0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36,
    0x18, 0x00, 0x18, 0xff, 0xff, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x18,
    0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00,
    0x36, 0x00, 0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18,
    0x00, 0x18, 0xff, 0xff, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x18, 0x18,
    0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36,
    0x00, 0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00,
    0x18, 0xff, 0xff, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
    0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00,
    0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18,
    0xff, 0xff, 0xf0, 0x0f, 0x00, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0xff,
    0x1f, 0x37, 0x3f, 0x37, 0xff, 0xf7, 0x37, 0xff, 0xf7, 0xff, 0xff,
    0xff, 0xff, 0x3f, 0x1f, 0x1f, 0x3f, 0xff, 0xff, 0xf8, 0x1f, 0xff,
    0xff, 0xf0, 0x0f, 0x00, 0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18,
    0x36, 0x30, 0x30, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x36, 0x00,
    0x00, 0x36, 0x18, 0x18, 0x00, 0x36, 0x18, 0x18, 0x00, 0xff, 0x00,
    0xf0, 0x0f, 0xff, 0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x1f, 0x36,
    0x37, 0x3f, 0xf7, 0xff, 0x37, 0xff, 0xf7, 0xff, 0x36, 0xff, 0x00,
    0x36, 0x1f, 0x1f, 0x00, 0x36, 0xff, 0x18, 0x00, 0xff, 0x00, 0xf0,
    0x0f, 0xff, 0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36,
    0x00, 0x36, 0x00, 0x36, 0x00, 0x36, 0x18, 0x36, 0x00, 0x00, 0x36,
    0x18, 0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0xff, 0x00, 0xf0, 0x0f,
    0xff, 0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00,
    0x36, 0x00, 0x36, 0x00, 0x36, 0x18, 0x36, 0x00, 0x00, 0x36, 0x18,
    0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0xff, 0x00, 0xf0, 0x0f, 0xff,
    0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36,
    0x00, 0x36, 0x00, 0x36, 0x18, 0x36, 0x00, 0x00, 0x36, 0x18, 0x00,
    0x00, 0x36, 0x18, 0x18, 0x00, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x18,
    0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36, 0x00,
    0x36, 0x00, 0x36, 0x18, 0x36, 0x00, 0x00, 0x36, 0x18, 0x00, 0x00,
    0x36, 0x18, 0x18, 0x00, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x18, 0x18,
    0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36, 0x00, 0x36,
    0x00, 0x36, 0x18, 0x36, 0x00, 0x00, 0x36, 0x18, 0x00, 0x00, 0x36,
    0x18, 0x18, 0x00, 0xff, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x44, 0xaa, 0x77, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18,
    0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x11, 0x55, 0xdd, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36,
    0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x06, 0x00, 0x00, 0x00,
    0x44, 0xaa, 0x77, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36,
    0x36, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 0x00, 0x11,
    0x55, 0xdd, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36,
    0x00, 0x00, 0x00, 0x18, 0x76, 0x18, 0x7c, 0x78, 0x66, 0xc6, 0x00,
    0x00, 0x7c, 0x00, 0x00, 0x0c, 0x3f, 0x18, 0x00, 0x00, 0x44, 0xaa,
    0x77, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00,
    0x00, 0x00, 0x18, 0xcc, 0x18, 0xc6, 0xcc, 0x66, 0xc6, 0x00, 0x00,
    0xc6, 0xc0, 0x06, 0x86, 0x9a, 0x3c, 0x00, 0x00, 0x11, 0x55, 0xdd,
    0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00,
    0x00, 0x18, 0xcc, 0x18, 0xc6, 0xcc, 0x66, 0xc6, 0x00, 0x00, 0xc6,
    0xc0, 0x06, 0xdc, 0xce, 0x3c, 0x36, 0xd8, 0x44, 0xaa, 0x77, 0x18,
    0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00,
    0x18, 0xcc, 0x18, 0xc6, 0xcc, 0x66, 0xce, 0x00, 0x00, 0xc0, 0xc0,
    0x06, 0x60, 0x66, 0x3c, 0x6c, 0x6c, 0x11, 0x55, 0xdd, 0x18, 0x18,
    0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18,
    0x7c, 0x18, 0xc6, 0xcc, 0x66, 0xde, 0x7e, 0x7c, 0x60, 0xc0, 0x06,
    0x30, 0x30, 0x18, 0xd8, 0x36, 0x44, 0xaa, 0x77, 0x18, 0xf8, 0xf8,
    0xf6, 0xfe, 0xf8, 0xf6, 0x36, 0xf6, 0xfe, 0xfe, 0xf8, 0xf8, 0x0c,
    0x18, 0xc6, 0xcc, 0x66, 0xfe, 0x00, 0x00, 0x30, 0xfe, 0xfe, 0x18,
    0x18, 0x18, 0x6c, 0x6c, 0x11, 0x55, 0xdd, 0x18, 0x18, 0x18, 0x36,
    0x00, 0x18, 0x06, 0x36, 0x06, 0x06, 0x36, 0x18, 0x00, 0x78, 0x38,
    0x7c, 0xcc, 0x5c, 0xf6, 0x3e, 0x38, 0x30, 0x00, 0x00, 0x6c, 0x6c,
    0x18, 0x36, 0xd8, 0x44, 0xaa, 0x77, 0x18, 0x18, 0xf8, 0x36, 0x00,
    0xf8, 0xf6, 0x36, 0xfe, 0xf6, 0x36, 0xf8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xe6, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x66, 0x66, 0x00,
    0x00, 0x00, 0x11, 0x55, 0xdd, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00,
    0x36, 0x36, 0x00, 0x36, 0x36, 0x18, 0x00, 0x60, 0x30, 0x60, 0x60,
    0xdc, 0xc6, 0x6c, 0x6c, 0x30, 0x00, 0x00, 0x62, 0x62, 0x18, 0x00,
    0x00, 0x44, 0xaa, 0x77, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36,
    0x36, 0x00, 0x36, 0x36, 0x18, 0x00, 0x30, 0x18, 0x30, 0x30, 0x76,
    0x00, 0x3c, 0x38, 0x30, 0x00, 0x00, 0xe0, 0xe0, 0x18, 0x00, 0x00,
    0x11, 0x55, 0xdd, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36,
    0x00, 0x36, 0x36, 0x18, 0x00, 0x18, 0x0c, 0x18, 0x18, 0x00, 0xdc,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x44,
    0xaa, 0x77, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36, 0x00,
    0x36, 0x36, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x55,
    0xdd, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36, 0x00, 0x36,
    0x36, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
    0x78, 0x7c, 0x76, 0x76, 0x76, 0x76, 0x7c, 0x7c, 0x7c, 0x7c, 0x18,
    0x18, 0x18, 0xc6, 0xc6, 0xfc, 0x6e, 0xce, 0x7c, 0x7c, 0x7c, 0x78,
    0x78, 0x76, 0x7c, 0x7c, 0x18, 0xfe, 0x18, 0xc6, 0x70, 0x3c, 0xcc,
    0xc2, 0xcc, 0xcc, 0xcc, 0xcc, 0xc6, 0xc2, 0xc2, 0xc2, 0x18, 0x18,
    0x18, 0xc6, 0xc6, 0xc0, 0xd8, 0xcc, 0xc6, 0xc6, 0xc6, 0xcc, 0xcc,
    0xce, 0xc6, 0xc6, 0x18, 0x60, 0x7e, 0xcc, 0xd8, 0x66, 0xcc, 0xc0,
    0xcc, 0xcc, 0xcc, 0xcc, 0xc0, 0xc0, 0xc0, 0xc0, 0x18, 0x18, 0x18,
    0xc6, 0xc6, 0xc0, 0xd8, 0xcc, 0xc6, 0xc6, 0xc6, 0xcc, 0xcc, 0xc6,
    0xc6, 0xc6, 0x7c, 0x60, 0x18, 0xcc, 0x18, 0xc6, 0xcc, 0xc0, 0xcc,
    0xcc, 0xcc, 0xcc, 0xc0, 0xc0, 0xc0, 0xc0, 0x18, 0x18, 0x18, 0xfe,
    0xc6, 0xc0, 0x7e, 0xcc, 0xc6, 0xc6, 0xc6, 0xcc, 0xcc, 0xc6, 0xc6,
    0xc6, 0xc6, 0x60, 0x7e, 0xcc, 0x18, 0xc0, 0xcc, 0xfe, 0x7c, 0x7c,
    0x7c, 0x7c, 0xc0, 0xfe, 0xfe, 0xfe, 0x18, 0x18, 0x18, 0xc6, 0xfe,
    0xf0, 0x36, 0xcc, 0xc6, 0xc6, 0xc6, 0xcc, 0xcc, 0xc6, 0xc6, 0xc6,
    0xc0, 0x60, 0x18, 0xde, 0x18, 0xc0, 0xcc, 0xc6, 0x0c, 0x0c, 0x0c,
    0x0c, 0xc6, 0xc6, 0xc6, 0xc6, 0x18, 0x18, 0x18, 0xc6, 0xc6, 0xc0,
    0x36, 0xfe, 0xc6, 0xc6, 0xc6, 0xcc, 0xcc, 0xc6, 0xc6, 0xc6, 0xc0,
    0xf8, 0x3c, 0xcc, 0x7e, 0xc0, 0xcc, 0x7c, 0x78, 0x78, 0x78, 0x78,
    0x7c, 0x7c, 0x7c, 0x7c, 0x38, 0x38, 0x38, 0x6c, 0x6c, 0xc0, 0xec,
    0xcc, 0x7c, 0x7c, 0x7c, 0xcc, 0xcc, 0xc6, 0xc6, 0xc6, 0xc0, 0x60,
    0x66, 0xc4, 0x18, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0xc0, 0x00, 0xcc,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x60, 0x66,
    0xf8, 0x18, 0x3c, 0xcc, 0x30, 0x6c, 0x00, 0x18, 0x38, 0x00, 0x6c,
    0x00, 0x18, 0x00, 0x66, 0x18, 0x10, 0x10, 0xfc, 0x00, 0x6c, 0x6c,
    0x00, 0x18, 0xcc, 0x18, 0x00, 0x7c, 0xc6, 0x7c, 0x62, 0x66, 0xcc,
    0x18, 0x00, 0xcc, 0x18, 0x38, 0xcc, 0x30, 0x6c, 0x00, 0x38, 0xc6,
    0x30, 0x66, 0x3c, 0x30, 0x00, 0x38, 0x00, 0x00, 0x3e, 0x38, 0xc6,
    0x30, 0x78, 0x30, 0xc6, 0x00, 0x00, 0x18, 0x3c, 0x66, 0xcc, 0x1b,
    0x00, 0x00, 0x0c, 0x10, 0x00, 0x60, 0x38, 0x00, 0x10, 0x00, 0x60,
    0x00, 0x18, 0x60, 0xc6, 0x6c, 0x30, 0x00, 0x00, 0x10, 0x00, 0x60,
    0x30, 0x60, 0x00, 0xc6, 0xc6, 0x18, 0x00, 0x00, 0xf8, 0x0e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc0, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8c,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0c, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xc0, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xf8, 0x78, 0x7c, 0x7c,
    0x30, 0x7c, 0xcc, 0x18, 0x0c, 0xcc, 0x18, 0xc6, 0xcc, 0x78, 0xf8,
    0x7c, 0xc0, 0x7c, 0x30, 0x78, 0x10, 0x6c, 0xc6, 0x76, 0xfe, 0x0e,
    0x18, 0x70, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xc4, 0xcc, 0xc2, 0x30,
    0xcc, 0xcc, 0x18, 0x0c, 0xcc, 0x18, 0xd6, 0xcc, 0xcc, 0xcc, 0xcc,
    0xc0, 0x86, 0x30, 0xcc, 0x6c, 0xfe, 0x6c, 0xce, 0xc0, 0x18, 0x18,
    0x18, 0x00, 0xfe, 0x00, 0xcc, 0xcc, 0xc0, 0xcc, 0xc0, 0x30, 0xcc,
    0xcc, 0x18, 0x0c, 0xd8, 0x18, 0xd6, 0xcc, 0xcc, 0xcc, 0xcc, 0xc0,
    0x06, 0x30, 0xcc, 0xc6, 0xd6, 0x38, 0xc6, 0x60, 0x18, 0x18, 0x18,
    0x00, 0xc6, 0x00, 0xcc, 0xcc, 0xc0, 0xcc, 0xc0, 0x30, 0xcc, 0xcc,
    0x18, 0x0c, 0xf0, 0x18, 0xd6, 0xcc, 0xcc, 0xcc, 0xcc, 0xc0, 0x1c,
    0x30, 0xcc, 0xc6, 0xd6, 0x38, 0xc6, 0x30, 0x18, 0x18, 0x18, 0x00,
    0xc6, 0x00, 0x7c, 0xcc, 0xc0, 0xcc, 0xfe, 0x30, 0xcc, 0xcc, 0x18,
    0x0c, 0xf0, 0x18, 0xd6, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x70, 0x30,
    0xcc, 0xc6, 0xd6, 0x38, 0xc6, 0x18, 0x18, 0x18, 0x18, 0x00, 0xc6,
    0x00, 0x0c, 0xcc, 0xc4, 0xcc, 0xc6, 0x78, 0xcc, 0xec, 0x18, 0x0c,
    0xd8, 0x18, 0xfe, 0xcc, 0xcc, 0xcc, 0xcc, 0xec, 0xc2, 0x30, 0xcc,
    0xc6, 0xc6, 0x6c, 0xc6, 0x0c, 0x70, 0x18, 0x0e, 0x00, 0x6c, 0x00,
    0x78, 0xf8, 0x78, 0x7c, 0x7c, 0x30, 0x7c, 0xd8, 0x38, 0x0c, 0xcc,
    0x18, 0xec, 0xb8, 0x78, 0xf8, 0x7c, 0xb8, 0x7c, 0xfc, 0xcc, 0xc6,
    0xc6, 0xc6, 0xc6, 0xfe, 0x18, 0x18, 0x18, 0x00, 0x38, 0x00, 0x00,
    0xc0, 0x00, 0x0c, 0x00, 0x32, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x10, 0x0c, 0x00, 0xc0,
    0x00, 0x0c, 0x00, 0x36, 0x00, 0xc0, 0x18, 0x0c, 0xc0, 0x18, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00, 0xc0, 0x00,
    0x0c, 0x00, 0x1c, 0x00, 0xc0, 0x18, 0x0c, 0xc0, 0x38, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0e, 0x18, 0x70, 0xdc, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7c, 0xc6, 0xfc, 0x3c, 0xf8, 0xfc, 0xc0, 0x3e, 0xc6, 0x18, 0x78,
    0xc2, 0xfc, 0xc6, 0xc6, 0x7c, 0xc0, 0x7c, 0xc6, 0x7c, 0x18, 0x7c,
    0x10, 0x6c, 0xc6, 0x18, 0xfe, 0x3c, 0x02, 0x3c, 0x00, 0x00, 0xc0,
    0xc6, 0xc6, 0x66, 0xdc, 0xc0, 0xc0, 0x66, 0xc6, 0x18, 0xcc, 0xc6,
    0xc0, 0xc6, 0xc6, 0xc6, 0xc0, 0xde, 0xc6, 0x86, 0x18, 0xc6, 0x38,
    0xee, 0xc6, 0x18, 0xc0, 0x30, 0x06, 0x0c, 0x00, 0x00, 0xdc, 0xc6,
    0xc6, 0xc2, 0xce, 0xc0, 0xc0, 0xc6, 0xc6, 0x18, 0xcc, 0xcc, 0xc0,
    0xc6, 0xc6, 0xc6, 0xc0, 0xd6, 0xcc, 0x06, 0x18, 0xc6, 0x6c, 0xfe,
    0x6c, 0x18, 0xc0, 0x30, 0x0e, 0x0c, 0x00, 0x00, 0xde, 0xc6, 0xc6,
    0xc0, 0xc6, 0xc0, 0xc0, 0xc6, 0xc6, 0x18, 0xcc, 0xd8, 0xc0, 0xc6,
    0xc6, 0xc6, 0xc0, 0xc6, 0xcc, 0x06, 0x18, 0xc6, 0xc6, 0xd6, 0x7c,
    0x18, 0x60, 0x30, 0x1c, 0x0c, 0x00, 0x00, 0xde, 0xfe, 0xc6, 0xc0,
    0xc6, 0xc0, 0xc0, 0xde, 0xc6, 0x18, 0x0c, 0xf0, 0xc0, 0xc6, 0xce,
    0xc6, 0xc0, 0xc6, 0xd8, 0x0c, 0x18, 0xc6, 0xc6, 0xd6, 0x38, 0x18,
    0x30, 0x30, 0x38, 0x0c, 0x00, 0x00, 0xde, 0xc6, 0xfc, 0xc0, 0xc6,
    0xf8, 0xf8, 0xc0, 0xfe, 0x18, 0x0c, 0xf0, 0xc0, 0xd6, 0xde, 0xc6,
    0xfc, 0xc6, 0xfc, 0x38, 0x18, 0xc6, 0xc6, 0xd6, 0x38, 0x3c, 0x18,
    0x30, 0x70, 0x0c, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc0, 0xc6, 0xc0,
    0xc0, 0xc0, 0xc6, 0x18, 0x0c, 0xd8, 0xc0, 0xfe, 0xfe, 0xc6, 0xc6,
    0xc6, 0xc6, 0x60, 0x18, 0xc6, 0xc6, 0xc6, 0x7c, 0x66, 0x0c, 0x30,
    0xe0, 0x0c, 0x00, 0x00, 0xc6, 0x6c, 0xc6, 0xc2, 0xce, 0xc0, 0xc0,
    0xc0, 0xc6, 0x18, 0x0c, 0xcc, 0xc0, 0xfe, 0xf6, 0xc6, 0xc6, 0xc6,
    0xc6, 0xc0, 0x18, 0xc6, 0xc6, 0xc6, 0x6c, 0x66, 0x06, 0x30, 0xc0,
    0x0c, 0x00, 0x00, 0x7c, 0x38, 0xc6, 0x66, 0xdc, 0xc0, 0xc0, 0x62,
    0xc6, 0x18, 0x0c, 0xc6, 0xc0, 0xee, 0xe6, 0xc6, 0xc6, 0xc6, 0xc6,
    0xc2, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0x66, 0x06, 0x30, 0x80, 0x0c,
    0xc6, 0x00, 0x00, 0x10, 0xfc, 0x3c, 0xf8, 0xfc, 0xfc, 0x3c, 0xc6,
    0x18, 0x1e, 0xc2, 0xc0, 0xc6, 0xc6, 0x7c, 0xfc, 0x7c, 0xfc, 0x7c,
    0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0x66, 0xfe, 0x3c, 0x00, 0x3c, 0x6c,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x6c, 0x7c,
    0x86, 0x76, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x18, 0x00, 0x18, 0x80,
    0x38, 0x18, 0xfe, 0x7c, 0x0c, 0x7c, 0x7c, 0x30, 0x7c, 0x78, 0x00,
    0x30, 0x06, 0x00, 0x60, 0x18, 0x00, 0x18, 0x00, 0x6c, 0xc6, 0xc6,
    0xcc, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00, 0x18, 0xc0, 0x6c,
    0x18, 0xc0, 0x86, 0x0c, 0x86, 0xc6, 0x30, 0xc6, 0x8c, 0x18, 0x18,
    0x0c, 0x00, 0x30, 0x18, 0x00, 0x00, 0x00, 0xfe, 0x86, 0x60, 0xcc,
    0x00, 0x30, 0x0c, 0x66, 0x18, 0x18, 0x00, 0x00, 0x60, 0xc6, 0x18,
    0xc0, 0x06, 0x0c, 0x06, 0xc6, 0x30, 0xc6, 0x06, 0x18, 0x18, 0x18,
    0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x6c, 0x06, 0x30, 0xcc, 0x00,
    0x30, 0x0c, 0x3c, 0x18, 0x00, 0x00, 0x00, 0x30, 0xe6, 0x18, 0x60,
    0x06, 0x0c, 0x06, 0xc6, 0x30, 0xc6, 0x06, 0x00, 0x00, 0x30, 0x7e,
    0x0c, 0x18, 0x00, 0x18, 0x00, 0x6c, 0x06, 0x18, 0xdc, 0x00, 0x30,
    0x0c, 0xff, 0x7e, 0x00, 0xfe, 0x00, 0x18, 0xf6, 0x18, 0x30, 0x06,
    0xfe, 0x06, 0xe6, 0x18, 0xc6, 0x06, 0x00, 0x00, 0x60, 0x00, 0x06,
    0x18, 0x00, 0x18, 0x00, 0x6c, 0x7c, 0x0c, 0x76, 0x00, 0x30, 0x0c,
    0x3c, 0x18, 0x00, 0x00, 0x00, 0x0c, 0xde, 0x18, 0x18, 0x3c, 0xcc,
    0xfc, 0xdc, 0x0c, 0x7c, 0x7e, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x18,
    0x00, 0x3c, 0x00, 0xfe, 0xc0, 0xc6, 0x38, 0x00, 0x30, 0x0c, 0x66,
    0x18, 0x00, 0x00, 0x00, 0x06, 0xce, 0x18, 0x0c, 0x06, 0x6c, 0xc0,
    0xc0, 0x06, 0xc6, 0xc6, 0x18, 0x18, 0x18, 0x7e, 0x18, 0x0c, 0x00,
    0x3c, 0x24, 0x6c, 0xc2, 0xc2, 0x6c, 0x60, 0x30, 0x0c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0xc6, 0x78, 0x06, 0x06, 0x3c, 0xc0, 0xc0,
    0x06, 0xc6, 0xc6, 0x18, 0x18, 0x0c, 0x00, 0x30, 0xc6, 0x00, 0x3c,
    0x66, 0x6c, 0xc6, 0x00, 0x6c, 0x30, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x6c, 0x38, 0xc6, 0x86, 0x1c, 0xc0, 0x60, 0x06,
    0xc6, 0xc6, 0x00, 0x00, 0x06, 0x00, 0x60, 0xc6, 0x00, 0x18, 0x66,
    0x00, 0x7c, 0x00, 0x38, 0x30, 0x0c, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x38, 0x18, 0x7c, 0x7c, 0x0c, 0xfe, 0x3c, 0xfe, 0x7c,
    0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x66, 0x00,
    0x18, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7e, 0x7e, 0x10, 0x00, 0x3c, 0x3c, 0x00, 0xff, 0x00,
    0xff, 0x78, 0x18, 0xe0, 0xe6, 0x18, 0x80, 0x02, 0x00, 0x66, 0x1b,
    0xc6, 0xfe, 0x7e, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x81, 0xff, 0x38, 0x10, 0x18, 0x18, 0x00, 0xff, 0x3c, 0xc3,
    0xcc, 0x18, 0xf0, 0xe7, 0x18, 0xc0, 0x06, 0x18, 0x66, 0x1b, 0x0c,
    0xfe, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x10, 0x00,
    0x81, 0xff, 0x7c, 0x38, 0x18, 0x18, 0x18, 0xe7, 0x66, 0x99, 0xcc,
    0x7e, 0x70, 0x67, 0xdb, 0xe0, 0x0e, 0x3c, 0x00, 0x1b, 0x38, 0xfe,
    0x3c, 0x18, 0x7e, 0x18, 0x30, 0xfe, 0x28, 0xfe, 0x38, 0x00, 0x99,
    0xe7, 0xfe, 0x7c, 0xe7, 0x7e, 0x3c, 0xc3, 0x42, 0xbd, 0xcc, 0x18,
    0x30, 0x63, 0x3c, 0xf0, 0x1e, 0x7e, 0x66, 0x1b, 0x6c, 0xfe, 0x7e,
    0x18, 0x18, 0x0c, 0x60, 0xc0, 0x6c, 0x7c, 0x38, 0x00, 0xbd, 0xc3,
    0xfe, 0xfe, 0xe7, 0xff, 0x3c, 0xc3, 0x42, 0xbd, 0xcc, 0x3c, 0x30,
    0x63, 0xe7, 0xf8, 0x3e, 0x18, 0x66, 0x1b, 0xc6, 0x00, 0x18, 0x18,
    0x18, 0xfe, 0xfe, 0xc0, 0xfe, 0x7c, 0x7c, 0x00, 0x81, 0xff, 0xfe,
    0x7c, 0xe7, 0xff, 0x18, 0xe7, 0x66, 0x99, 0x78, 0x66, 0x30, 0x63,
    0x3c, 0xfe, 0xfe, 0x18, 0x66, 0x7b, 0xc6, 0x00, 0x18, 0x18, 0x18,
    0x0c, 0x60, 0xc0, 0x6c, 0x38, 0x7c, 0x00, 0x81, 0xff, 0xfe, 0x38,
    0x3c, 0x7e, 0x00, 0xff, 0x3c, 0xc3, 0x32, 0x66, 0x30, 0x63, 0xdb,
    0xf8, 0x3e, 0x18, 0x66, 0xdb, 0x6c, 0x00, 0x18, 0x18, 0x18, 0x18,
    0x30, 0x00, 0x28, 0x38, 0xfe, 0x00, 0xa5, 0xdb, 0x6c, 0x10, 0x3c,
    0x3c, 0x00, 0xff, 0x00, 0xff, 0x1a, 0x66, 0x3f, 0x7f, 0x18, 0xf0,
    0x1e, 0x7e, 0x66, 0xdb, 0x38, 0x00, 0x7e, 0x7e, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x10, 0xfe, 0x00, 0x81, 0xff, 0x00, 0x00, 0x18, 0x18,
    0x00, 0xff, 0x00, 0xff, 0x0e, 0x66, 0x33, 0x63, 0x18, 0xe0, 0x0e,
    0x3c, 0x66, 0xdb, 0x60, 0x00, 0x3c, 0x3c, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0xff, 0x1e, 0x3c, 0x3f, 0x7f, 0x00, 0xc0, 0x06, 0x18,
    0x66, 0x7f, 0xc6, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00,
    0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif
// #include "../common/iconbmp.h"
/* The PDCurses logo as #include'able BMP (from icon32.xpm),
   for use by SDL. */

unsigned char iconbmp[] =
{
    0x42, 0x4d, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e,
    0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x13, 0x0b,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xce, 0x6f, 0x9c,
    0xe7, 0xb5, 0xaf, 0x6b, 0x5b, 0xbd, 0xaf, 0xeb, 0xfb, 0xbd, 0xaf,
    0x98, 0xe7, 0xbd, 0xaf, 0x7b, 0x5f, 0xb5, 0xa5, 0x6b, 0x5b, 0xcd,
    0xab, 0x9c, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xcf, 0x03, 0xff, 0xff, 0xce, 0x03, 0xff, 0xff, 0xcc, 0x73,
    0xff, 0xff, 0xcc, 0xf3, 0xff, 0xff, 0xcc, 0xf3, 0xff, 0xff, 0xcc,
    0x73, 0xff, 0xff, 0xc6, 0x33, 0xff, 0xff, 0xc3, 0x13, 0xff, 0xff,
    0xc1, 0x83, 0xff, 0xff, 0xc8, 0xc3, 0xff, 0xff, 0xcc, 0x63, 0xff,
    0xff, 0xce, 0x33, 0xff, 0xff, 0xcf, 0x33, 0xff, 0xff, 0xcf, 0x33,
    0xff, 0xff, 0xce, 0x33, 0xff, 0xff, 0xc0, 0x73, 0xff, 0xff, 0xc0,
    0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff
};


#ifdef PDC_WIDE
# ifndef PDC_FONT_PATH
#  ifdef _WIN32
#   define PDC_FONT_PATH "C:/Windows/Fonts/consola.ttf"
#  elif defined(__APPLE__)
#   define PDC_FONT_PATH "/System/Library/Fonts/Menlo.ttc"
#  else
#   define PDC_FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
#  endif
# endif
TTF_Font *pdc_ttffont = NULL;
int pdc_font_size =
# ifdef _WIN32
 16;
# else
 17;
# endif
#endif

SDL_Window *pdc_window = NULL;
SDL_Surface *pdc_screen = NULL, *pdc_font = NULL, *pdc_icon = NULL,
            *pdc_back = NULL, *pdc_tileback = NULL;
int pdc_sheight = 0, pdc_swidth = 0, pdc_yoffset = 0, pdc_xoffset = 0;

SDL_Color pdc_color[PDC_MAXCOL];
Uint32 pdc_mapped[PDC_MAXCOL];
int pdc_fheight, pdc_fwidth, pdc_fthick, pdc_flastc;
bool pdc_own_window;

static void _clean(void)
{
#ifdef PDC_WIDE
    if (pdc_ttffont)
    {
        TTF_CloseFont(pdc_ttffont);
        TTF_Quit();
    }
#endif
    SDL_FreeSurface(pdc_tileback);
    SDL_FreeSurface(pdc_back);
    SDL_FreeSurface(pdc_icon);
    SDL_FreeSurface(pdc_font);
    SDL_DestroyWindow(pdc_window);
    SDL_Quit();
}

void PDC_retile(void)
{
    if (pdc_tileback)
        SDL_FreeSurface(pdc_tileback);

    pdc_tileback = SDL_ConvertSurface(pdc_screen, pdc_screen->format, 0);
    if (pdc_tileback == NULL)
        return;

    if (pdc_back)
    {
        SDL_Rect dest;

        dest.y = 0;

        while (dest.y < pdc_tileback->h)
        {
            dest.x = 0;

            while (dest.x < pdc_tileback->w)
            {
                SDL_BlitSurface(pdc_back, 0, pdc_tileback, &dest);
                dest.x += pdc_back->w;
            }

            dest.y += pdc_back->h;
        }

        SDL_BlitSurface(pdc_tileback, 0, pdc_screen, 0);
    }
}

void PDC_scr_close(void)
{
    PDC_LOG(("PDC_scr_close() - called\n"));
}

void PDC_scr_free(void)
{
}

static void _initialize_colors(void)
{
    int i, r, g, b;

    for (i = 0; i < 8; i++)
    {
        pdc_color[i].r = (i & COLOR_RED) ? 0xc0 : 0;
        pdc_color[i].g = (i & COLOR_GREEN) ? 0xc0 : 0;
        pdc_color[i].b = (i & COLOR_BLUE) ? 0xc0 : 0;

        pdc_color[i + 8].r = (i & COLOR_RED) ? 0xff : 0x40;
        pdc_color[i + 8].g = (i & COLOR_GREEN) ? 0xff : 0x40;
        pdc_color[i + 8].b = (i & COLOR_BLUE) ? 0xff : 0x40;
    }

    /* 256-color xterm extended palette: 216 colors in a 6x6x6 color
       cube, plus 24 shades of gray */

    for (i = 16, r = 0; r < 6; r++)
        for (g = 0; g < 6; g++)
            for (b = 0; b < 6; b++, i++)
            {
                pdc_color[i].r = (r ? r * 40 + 55 : 0);
                pdc_color[i].g = (g ? g * 40 + 55 : 0);
                pdc_color[i].b = (b ? b * 40 + 55 : 0);
            }

    for (i = 232; i < 256; i++)
        pdc_color[i].r = pdc_color[i].g = pdc_color[i].b = (i - 232) * 10 + 8;

    for (i = 0; i < 256; i++)
        pdc_mapped[i] = SDL_MapRGB(pdc_screen->format, pdc_color[i].r,
                                   pdc_color[i].g, pdc_color[i].b);
}

/* find the display where the mouse pointer is */

int _get_displaynum(void)
{
    SDL_Rect size;
    int i, xpos, ypos, displays;

    displays = SDL_GetNumVideoDisplays();

    if (displays > 1)
    {
        SDL_GetGlobalMouseState(&xpos, &ypos);

        for (i = 0; i < displays; i++)
        {
            SDL_GetDisplayBounds(i, &size);
            if (size.x <= xpos && xpos < size.x + size.w &&
                size.y <= ypos && ypos < size.y + size.h)
                return i;
        }
    }

    return 0;
}

/* open the physical screen -- miscellaneous initialization */

int PDC_scr_open(void)
{
    SDL_Event event;
    int displaynum = 0;

    PDC_LOG(("PDC_scr_open() - called\n"));

    pdc_own_window = !pdc_window;

    if (pdc_own_window)
    {
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) < 0)
        {
            fprintf(stderr, "Could not start SDL: %s\n", SDL_GetError());
            return ERR;
        }

        atexit(_clean);

        displaynum = _get_displaynum();
    }

#ifdef PDC_WIDE
    if (!pdc_ttffont)
    {
        const char *ptsz, *fname;

        if (TTF_Init() == -1)
        {
            fprintf(stderr, "Could not start SDL_TTF: %s\n", SDL_GetError());
            return ERR;
        }

        ptsz = getenv("PDC_FONT_SIZE");
        if (ptsz != NULL)
            pdc_font_size = atoi(ptsz);
        if (pdc_font_size <= 0)
            pdc_font_size = 18;

        fname = getenv("PDC_FONT");
        pdc_ttffont = TTF_OpenFont(fname ? fname : PDC_FONT_PATH,
                                   pdc_font_size);
    }

    if (!pdc_ttffont)
    {
        fprintf(stderr, "Could not load font\n");
        return ERR;
    }

    TTF_SetFontKerning(pdc_ttffont, 0);
    TTF_SetFontHinting(pdc_ttffont, TTF_HINTING_MONO);

    SP->mono = FALSE;
#else
    if (!pdc_font)
    {
        const char *fname = getenv("PDC_FONT");
        pdc_font = SDL_LoadBMP(fname ? fname : "pdcfont.bmp");
    }

    if (!pdc_font)
        pdc_font = SDL_LoadBMP_RW(SDL_RWFromMem(font437, sizeof(font437)), 0);

    if (!pdc_font)
    {
        fprintf(stderr, "Could not load font\n");
        return ERR;
    }

    SP->mono = !pdc_font->format->palette;
#endif

    if (!SP->mono && !pdc_back)
    {
        const char *bname = getenv("PDC_BACKGROUND");
        pdc_back = SDL_LoadBMP(bname ? bname : "pdcback.bmp");
    }

    if (!SP->mono && (pdc_back || !pdc_own_window))
    {
        SP->orig_attr = TRUE;
        SP->orig_fore = COLOR_WHITE;
        SP->orig_back = -1;
    }
    else
        SP->orig_attr = FALSE;

#ifdef PDC_WIDE
    TTF_SizeText(pdc_ttffont, "W", &pdc_fwidth, &pdc_fheight);
    pdc_fthick = pdc_font_size / 20 + 1;
#else
    pdc_fheight = pdc_font->h / 8;
    pdc_fwidth = pdc_font->w / 32;
    pdc_fthick = 1;

    if (!SP->mono)
        pdc_flastc = pdc_font->format->palette->ncolors - 1;
#endif

    if (pdc_own_window && !pdc_icon)
    {
        const char *iname = getenv("PDC_ICON");
        pdc_icon = SDL_LoadBMP(iname ? iname : "pdcicon.bmp");

        if (!pdc_icon)
            pdc_icon = SDL_LoadBMP_RW(SDL_RWFromMem(iconbmp,
                                                    sizeof(iconbmp)), 0);
    }

    if (pdc_own_window)
    {
        const char *env = getenv("PDC_LINES");
        pdc_sheight = (env ? atoi(env) : 25) * pdc_fheight;

        env = getenv("PDC_COLS");
        pdc_swidth = (env ? atoi(env) : 80) * pdc_fwidth;

        pdc_window = SDL_CreateWindow("PDCurses",
            SDL_WINDOWPOS_CENTERED_DISPLAY(displaynum),
            SDL_WINDOWPOS_CENTERED_DISPLAY(displaynum),
            pdc_swidth, pdc_sheight, SDL_WINDOW_RESIZABLE);

        if (pdc_window == NULL)
        {
            fprintf(stderr, "Could not open SDL window: %s\n", SDL_GetError());
            return ERR;
        }

        SDL_SetWindowIcon(pdc_window, pdc_icon);
    }

    /* Events must be pumped before calling SDL_GetWindowSurface, or
       initial modifiers (e.g. numlock) will be ignored and out-of-sync. */

    SDL_PumpEvents();

    /* Wait until window is exposed before getting surface */

    while (SDL_PollEvent(&event))
        if (SDL_WINDOWEVENT == event.type &&
            SDL_WINDOWEVENT_EXPOSED == event.window.event)
            break;

    if (!pdc_screen)
    {
        pdc_screen = SDL_GetWindowSurface(pdc_window);

        if (!pdc_screen)
        {
            fprintf(stderr, "Could not open SDL window surface: %s\n",
                    SDL_GetError());
            return ERR;
        }
    }

    if (!pdc_sheight)
        pdc_sheight = pdc_screen->h - pdc_yoffset;

    if (!pdc_swidth)
        pdc_swidth = pdc_screen->w - pdc_xoffset;

    if (SP->orig_attr)
        PDC_retile();

    _initialize_colors();

    SDL_StartTextInput();

    PDC_mouse_set();

    SP->mouse_wait = PDC_CLICK_PERIOD;
    SP->audible = FALSE;

    SP->termattrs = A_COLOR | A_UNDERLINE | A_LEFT | A_RIGHT | A_REVERSE;
#ifdef PDC_WIDE
    SP->termattrs |= A_ITALIC;
#endif

    PDC_reset_prog_mode();

    return OK;
}

/* the core of resize_term() */

int PDC_resize_screen(int nlines, int ncols)
{
    if (!pdc_own_window)
        return ERR;

    if (nlines && ncols)
    {
#if SDL_VERSION_ATLEAST(2, 0, 5)
        SDL_Rect max;
        int top, left, bottom, right;

        SDL_GetDisplayUsableBounds(0, &max);
        SDL_GetWindowBordersSize(pdc_window, &top, &left, &bottom, &right);
        max.h -= top + bottom;
        max.w -= left + right;

        while (nlines * pdc_fheight > max.h)
            nlines--;
        while (ncols * pdc_fwidth > max.w)
            ncols--;
#endif
        pdc_sheight = nlines * pdc_fheight;
        pdc_swidth = ncols * pdc_fwidth;

        SDL_SetWindowSize(pdc_window, pdc_swidth, pdc_sheight);
        pdc_screen = SDL_GetWindowSurface(pdc_window);
    }

    if (pdc_tileback)
        PDC_retile();

    return OK;
}

void PDC_reset_prog_mode(void)
{
    PDC_LOG(("PDC_reset_prog_mode() - called.\n"));

    PDC_flushinp();
}

void PDC_reset_shell_mode(void)
{
    PDC_LOG(("PDC_reset_shell_mode() - called.\n"));

    PDC_flushinp();
}

void PDC_restore_screen_mode(int i)
{
}

void PDC_save_screen_mode(int i)
{
}

bool PDC_can_change_color(void)
{
    return TRUE;
}

int PDC_color_content(short color, short *red, short *green, short *blue)
{
    *red = DIVROUND(pdc_color[color].r * 1000, 255);
    *green = DIVROUND(pdc_color[color].g * 1000, 255);
    *blue = DIVROUND(pdc_color[color].b * 1000, 255);

    return OK;
}

int PDC_init_color(short color, short red, short green, short blue)
{
    pdc_color[color].r = DIVROUND(red * 255, 1000);
    pdc_color[color].g = DIVROUND(green * 255, 1000);
    pdc_color[color].b = DIVROUND(blue * 255, 1000);

    pdc_mapped[color] = SDL_MapRGB(pdc_screen->format, pdc_color[color].r,
                                   pdc_color[color].g, pdc_color[color].b);

    return OK;
}
/* PDCurses */

/*man-start**************************************************************

pdcsetsc
--------

### Synopsis

    int PDC_set_blink(bool blinkon);
    int PDC_set_bold(bool boldon);
    void PDC_set_title(const char *title);

### Description

   PDC_set_blink() toggles whether the A_BLINK attribute sets an actual
   blink mode (TRUE), or sets the background color to high intensity
   (FALSE). The default is platform-dependent (FALSE in most cases). It
   returns OK if it could set the state to match the given parameter,
   ERR otherwise.

   PDC_set_bold() toggles whether the A_BOLD attribute selects an actual
   bold font (TRUE), or sets the foreground color to high intensity
   (FALSE). It returns OK if it could set the state to match the given
   parameter, ERR otherwise.

   PDC_set_title() sets the title of the window in which the curses
   program is running. This function may not do anything on some
   platforms.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   PDC_set_blink         |    -   |    -    |   -
   PDC_set_bold          |    -   |    -    |   -
   PDC_set_title         |    -   |    -    |   -

**man-end****************************************************************/

int PDC_curs_set(int visibility)
{
    int ret_vis;

    PDC_LOG(("PDC_curs_set() - called: visibility=%d\n", visibility));

    ret_vis = SP->visibility;

    SP->visibility = visibility;

    PDC_gotoyx(SP->cursrow, SP->curscol);

    return ret_vis;
}

void PDC_set_title(const char *title)
{
    PDC_LOG(("PDC_set_title() - called:<%s>\n", title));

    SDL_SetWindowTitle(pdc_window, title);
}

int PDC_set_blink(bool blinkon)
{
    if (!SP)
        return ERR;

    if (SP->color_started)
        COLORS = PDC_MAXCOL;

    if (blinkon)
    {
        if (!(SP->termattrs & A_BLINK))
        {
            SP->termattrs |= A_BLINK;
            PDC_blink_text();
        }
    }
    else
    {
        if (SP->termattrs & A_BLINK)
        {
            SP->termattrs &= ~A_BLINK;
            PDC_blink_text();
        }
    }

    return OK;
}

int PDC_set_bold(bool boldon)
{
    if (!SP)
        return ERR;

#ifdef PDC_WIDE
    if (boldon)
        SP->termattrs |= A_BOLD;
    else
        SP->termattrs &= ~A_BOLD;

    return OK;
#else
    return boldon ? ERR : OK;
#endif
}
/* PDCurses */

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
}

void PDC_napms(int ms)
{
    PDC_LOG(("PDC_napms() - called: ms=%d\n", ms));

    while (ms > 50)
    {
        PDC_pump_and_peep();
        SDL_Delay(50);
        ms -= 50;
    }
    PDC_pump_and_peep();
    SDL_Delay(ms);
}

const char *PDC_sysname(void)
{
    return "SDL2";
}

/* Here's a simple example of combining SDL and PDCurses functionality.
   The top portion of the window is devoted to SDL, with a four-line
   (assuming the default 8x16 font) stdscr at the bottom.
*/

#define SDL_MAIN_HANDLED

/* You could #include pdcsdl.h, or just add the relevant declarations
   here: */

/* PDCurses */

/*man-start**************************************************************

scanw
-----

### Synopsis

    int scanw(const char *fmt, ...);
    int wscanw(WINDOW *win, const char *fmt, ...);
    int mvscanw(int y, int x, const char *fmt, ...);
    int mvwscanw(WINDOW *win, int y, int x, const char *fmt, ...);
    int vwscanw(WINDOW *win, const char *fmt, va_list varglist);
    int vw_scanw(WINDOW *win, const char *fmt, va_list varglist);

### Description

   These routines correspond to the standard C library's scanf() family.
   Each gets a string from the window via wgetnstr(), and uses the
   resulting line as input for the scan.

   The duplication between vwscanw() and vw_scanw() is for historic
   reasons. In PDCurses, they're the same.

### Return Value

   On successful completion, these functions return the number of items
   successfully matched. Otherwise they return ERR.

### Portability

   Function              | X/Open | ncurses | NetBSD
   :---------------------|:------:|:-------:|:------:
   scanw                 |    Y   |    Y    |   Y
   wscanw                |    Y   |    Y    |   Y
   mvscanw               |    Y   |    Y    |   Y
   mvwscanw              |    Y   |    Y    |   Y
   vwscanw               |    Y   |    Y    |   Y
   vw_scanw              |    Y   |    Y    |   Y

**man-end****************************************************************/

static int _pdc_vsscanf(const char *, const char *, va_list);

# define vsscanf _pdc_vsscanf
#endif

/*
int vwscanw(WINDOW *win, const char *fmt, va_list varglist)
{
    char scanbuf[256];

    PDC_LOG(("vwscanw() - called\n"));

    if (wgetnstr(win, scanbuf, 255) == ERR)
        return ERR;

    return vsscanf(scanbuf, fmt, varglist);
}

int scanw(const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("scanw() - called\n"));

    va_start(args, fmt);
    retval = vwscanw(stdscr, fmt, args);
    va_end(args);

    return retval;
}

int wscanw(WINDOW *win, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("wscanw() - called\n"));

    va_start(args, fmt);
    retval = vwscanw(win, fmt, args);
    va_end(args);

    return retval;
}

int mvscanw(int y, int x, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("mvscanw() - called\n"));

    if (move(y, x) == ERR)
        return ERR;

    va_start(args, fmt);
    retval = vwscanw(stdscr, fmt, args);
    va_end(args);

    return retval;
}

int mvwscanw(WINDOW *win, int y, int x, const char *fmt, ...)
{
    va_list args;
    int retval;

    PDC_LOG(("mvscanw() - called\n"));

    if (wmove(win, y, x) == ERR)
        return ERR;

    va_start(args, fmt);
    retval = vwscanw(win, fmt, args);
    va_end(args);

    return retval;
}

int vw_scanw(WINDOW *win, const char *fmt, va_list varglist)
{
    PDC_LOG(("vw_scanw() - called\n"));

    return vwscanw(win, fmt, varglist);
}

*/
#ifndef HAVE_VSSCANF

/* _pdc_vsscanf() - Internal routine to parse and format an input
   buffer. It scans a series of input fields; each field is formatted
   according to a supplied format string and the formatted input is
   stored in the variable number of addresses passed. Returns the number
   of input fields or EOF on error.

   Don't compile this unless required. Some compilers (at least Borland
   C++ 3.0) have to link with math libraries due to the use of floats.

   Based on vsscanf.c and input.c from emx 0.8f library source,
   Copyright (c) 1990-1992 by Eberhard Mattes, who has kindly agreed to
   its inclusion in PDCurses. */

#define WHITE(x) ((x) == ' ' || (x) == '\t' || (x) == '\n')

#define NEXT(x) \
        do { \
            x = *buf++; \
            if (!x) \
               return (count ? count : EOF); \
            ++chars; \
           } while (0)

#define UNGETC() \
        do { \
            --buf; --chars; \
           } while (0)

static int _pdc_vsscanf(const char *buf, const char *fmt, va_list arg_ptr)
{
    int count, chars, c, width, radix, d, i;
    int *int_ptr;
    long *long_ptr;
    short *short_ptr;
    char *char_ptr;
    unsigned char f;
    char neg, assign, ok, size;
    long n;
    char map[256], end;
    double dx, dd, *dbl_ptr;
    float *flt_ptr;
    int exp;
    char eneg;

    count = 0;
    chars = 0;
    c = 0;
    while ((f = *fmt) != 0)
    {
        if (WHITE(f))
        {
            do
            {
                ++fmt;
                f = *fmt;
            }
            while (WHITE(f));
            do
            {
                c = *buf++;
                if (!c)
                {
                    if (!f || count)
                        return count;
                    else
                        return EOF;
                } else
                    ++chars;
            }
            while (WHITE(c));
            UNGETC();
        } else if (f != '%')
        {
            NEXT(c);
            if (c != f)
                return count;
            ++fmt;
        } else
        {
            assign = TRUE;
            width = INT_MAX;
            char_ptr = NULL;
            ++fmt;
            if (*fmt == '*')
            {
                assign = FALSE;
                ++fmt;
            }
            if (isdigit(*fmt))
            {
                width = 0;
                while (isdigit(*fmt))
                    width = width * 10 + (*fmt++ - '0');
                if (!width)
                    width = INT_MAX;
            }
            size = 0;
            if (*fmt == 'h' || *fmt == 'l')
                size = *fmt++;
            f = *fmt;
            switch (f)
            {
            case 'c':
                if (width == INT_MAX)
                    width = 1;
                if (assign)
                    char_ptr = va_arg(arg_ptr, char *);
                while (width > 0)
                {
                    --width;
                    NEXT(c);
                    if (assign)
                    {
                        *char_ptr++ = (char) c;
                        ++count;
                    }
                }
                break;
            case '[':
                memset(map, 0, 256);
                end = 0;
                ++fmt;
                if (*fmt == '^')
                {
                    ++fmt;
                    end = 1;
                }
                i = 0;
                for (;;)
                {
                    f = (unsigned char) *fmt;
                    switch (f)
                    {
                    case 0:
                        /* avoid skipping past 0 */
                        --fmt;
                        NEXT(c);
                        goto string;
                    case ']':
                        if (i > 0)
                        {
                            NEXT(c);
                            goto string;
                        }
                        /* no break */
                    default:
                        if (fmt[1] == '-' && fmt[2]
                            && f < (unsigned char)fmt[2])
                        {
                            memset(map + f, 1, (unsigned char)fmt[2] - f);
                            fmt += 2;
                        }
                        else
                            map[f] = 1;
                        break;
                    }
                    ++fmt;
                    ++i;
                }
            case 's':
                memset(map, 0, 256);
                map[' '] = 1;
                map['\n'] = 1;
                map['\r'] = 1;
                map['\t'] = 1;
                end = 1;
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
            string:
                if (assign)
                    char_ptr = va_arg(arg_ptr, char *);
                while (width > 0 && map[(unsigned char) c] != end)
                {
                    --width;
                    if (assign)
                        *char_ptr++ = (char) c;
                    c = *buf++;
                    if (!c)
                        break;
                    else
                        ++chars;
                }
                if (assign)
                {
                    *char_ptr = 0;
                    ++count;
                }
                if (!c)
                    return count;
                else
                    UNGETC();
                break;
            case 'f':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                neg = ok = FALSE;
                dx = 0.0;
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
                if (c == '+')
                {
                    NEXT(c);
                    --width;
                } else if (c == '-')
                {
                    neg = TRUE;
                    NEXT(c);
                    --width;
                }
                while (width > 0 && isdigit(c))
                {
                    --width;
                    dx = dx * 10.0 + (double) (c - '0');
                    ok = TRUE;
                    c = *buf++;
                    if (!c)
                        break;
                    else
                        ++chars;
                }
                if (width > 0 && c == '.')
                {
                    --width;
                    dd = 10.0;
                    NEXT(c);
                    while (width > 0 && isdigit(c))
                    {
                        --width;
                        dx += (double) (c - '0') / dd;
                        dd *= 10.0;
                        ok = TRUE;
                        c = *buf++;
                        if (!c)
                            break;
                        else
                            ++chars;
                    }
                }
                if (!ok)
                    return count;
                if (width > 0 && (c == 'e' || c == 'E'))
                {
                    eneg = FALSE;
                    exp = 0;
                    NEXT(c);
                    --width;
                    if (width > 0 && c == '+')
                    {
                        NEXT(c);
                        --width;
                    } else if (width > 0 && c == '-')
                    {
                        eneg = TRUE;
                        NEXT(c);
                        --width;
                    }
                    if (!(width > 0 && isdigit(c)))
                    {
                        UNGETC();
                        return count;
                    }
                    while (width > 0 && isdigit(c))
                    {
                        --width;
                        exp = exp * 10 + (c - '0');
                        c = *buf++;
                        if (!c)
                            break;
                        else
                            ++chars;
                    }
                    if (eneg)
                        exp = -exp;
                    while (exp > 0)
                    {
                        dx *= 10.0;
                        --exp;
                    }
                    while (exp < 0)
                    {
                        dx /= 10.0;
                        ++exp;
                    }
                }
                if (assign)
                {
                    if (neg)
                        dx = -dx;
                    if (size == 'l')
                    {
                        dbl_ptr = va_arg(arg_ptr, double *);
                        *dbl_ptr = dx;
                    }
                    else
                    {
                        flt_ptr = va_arg(arg_ptr, float *);
                        *flt_ptr = (float)dx;
                    }
                    ++count;
                }
                if (!c)
                    return count;
                else
                    UNGETC();
                break;
            case 'i':
                neg = FALSE;
                radix = 10;
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
                if (!(width > 0 && c == '0'))
                    goto scan_complete_number;
                NEXT(c);
                --width;
                if (width > 0 && (c == 'x' || c == 'X'))
                {
                    NEXT(c);
                    radix = 16;
                    --width;
                }
                else if (width > 0 && (c >= '0' && c <= '7'))
                    radix = 8;
                goto scan_unsigned_number;
            case 'd':
            case 'u':
            case 'o':
            case 'x':
            case 'X':
                do
                {
                    NEXT(c);
                }
                while (WHITE(c));
                switch (f)
                {
                case 'o':
                    radix = 8;
                    break;
                case 'x':
                case 'X':
                    radix = 16;
                    break;
                default:
                    radix = 10;
                    break;
                }
            scan_complete_number:
                neg = FALSE;
                if (width > 0 && c == '+')
                {
                    NEXT(c);
                    --width;
                }
                else if (width > 0 && c == '-' && radix == 10)
                {
                    neg = TRUE;
                    NEXT(c);
                    --width;
                }
            scan_unsigned_number:
                n = 0;
                ok = FALSE;
                while (width > 0)
                {
                    --width;
                    if (isdigit(c))
                        d = c - '0';
                    else if (isupper(c))
                        d = c - 'A' + 10;
                    else if (islower(c))
                        d = c - 'a' + 10;
                    else
                        break;
                    if (d < 0 || d >= radix)
                        break;
                    ok = TRUE;
                    n = n * radix + d;
                    c = *buf++;
                    if (!c)
                        break;
                    else
                        ++chars;
                }
                if (!ok)
                    return count;
                if (assign)
                {
                    if (neg)
                        n = -n;
                    switch (size)
                    {
                    case 'h':
                        short_ptr = va_arg(arg_ptr, short *);
                        *short_ptr = (short) n;
                        break;
                    case 'l':
                        long_ptr = va_arg(arg_ptr, long *);
                        *long_ptr = (long) n;
                        break;
                    default:
                        int_ptr = va_arg(arg_ptr, int *);
                        *int_ptr = (int) n;
                    }
                    ++count;
                }
                if (!c)
                    return count;
                else
                    UNGETC();
                break;
            case 'n':
                if (assign)
                {
                    int_ptr = va_arg(arg_ptr, int *);
                    *int_ptr = chars;
                    ++count;
                }
                break;
            default:
                if (!f) /* % at end of string */
                    return count;
                NEXT(c);
                if (c != f)
                    return count;
                break;
            }
            ++fmt;
        }
    }
    return count;
}

#ifdef TEST_SDL
PDCEX SDL_Window *pdc_window;
PDCEX SDL_Surface *pdc_screen;
PDCEX int pdc_yoffset;

int main(int argc, char **argv)
{
    char inp[60];
    int i, j, seed;

    seed = time((time_t *)0);
    srand(seed);

    /* Initialize SDL */

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(1);

    atexit(SDL_Quit);

    pdc_window = SDL_CreateWindow("PDCurses for SDL", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    pdc_screen = SDL_GetWindowSurface(pdc_window);

    /* Initialize PDCurses */

    pdc_yoffset = 416;  /* 480 - 4 * 16 */

    initscr();
    start_color();
    scrollok(stdscr, TRUE);

    PDC_set_title("PDCurses for SDL");

    /* Do some SDL stuff */

    for (i = 640, j = 416; j; i -= 2, j -= 2)
    {
        SDL_Rect dest;

        dest.x = (640 - i) / 2;
        dest.y = (416 - j) / 2;
        dest.w = i;
        dest.h = j;

        SDL_FillRect(pdc_screen, &dest,
                     SDL_MapRGB(pdc_screen->format, rand() % 256,
                                rand() % 256, rand() % 256));
    }

    SDL_UpdateWindowSurface(pdc_window);

    /* Do some curses stuff */

    init_pair(1, COLOR_WHITE + 8, COLOR_BLUE);
    bkgd(COLOR_PAIR(1));

    addstr("This is a demo of ");
    attron(A_UNDERLINE);
    addstr("PDCurses for SDL");
    attroff(A_UNDERLINE);
    addstr(".\nYour comments here: ");
    getnstr(inp, 59);
    addstr("Press any key to exit.");

    getch();
    endwin();

    return 0;
}
#endif /* TEST_SDL */

#endif /* __PDCURSES__ */
