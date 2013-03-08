/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with EPSIO_yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum EPSIO_yytokentype {
     INTEGER = 1,
     WORD = 2,
     STRING = 3,
     PERIOD = 13,
     OPEN = 4,
     CLOSE = 5,
     COMMA = 6,
     NEWLINE = 7,
     ERROR = 8,
     OPEN_SQUARE = 9,
     CLOSE_SQUARE = 10,
     EQUALS = 11,
     EXP = 14
   };
#endif
/* Tokens.  */
#define INTEGER 1
#define WORD 2
#define STRING 3
#define PERIOD 13
#define OPEN 4
#define CLOSE 5
#define COMMA 6
#define NEWLINE 7
#define ERROR 8
#define OPEN_SQUARE 9
#define CLOSE_SQUARE 10
#define EQUALS 11
#define EXP 14




/* Copy the first part of user declarations.  */
#line 2 "./parser.y"

#include "wx/setup.h"
#include <string.h>
#ifdef __SC__
#include <io.h>
#endif
#ifdef _MSC_VER
#include <io.h>
#endif
#if defined(__GNUWIN32__) && !defined(__TWIN32__)
#include <sys/unistd.h>
#endif

#include "wxexio.h"

#if !defined(__EXTERN_C__)
#define __EXTERN_C__ 1
#endif

/* flex on NetBSD and probably other free BSDs can handle c++ compilation,
 * but *not* within extern C block: it generates __cplusplus conditional
 * prototypes of same name the signatures of which differ only in
 * unsigned int ('size_t') and int parameters: obvious conflict for C-type
 * compilation.
 */
#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(__FreeBSD__)
#undef __EXTERN_C__
#define YY_SKIP_YYWRAP 1
#endif

#if defined(__cplusplus) || defined(__STDC__)
#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#endif
int EPSIO_yylex(void);
int EPSIO_yylook(void);
int EPSIO_yywrap(void);
int EPSIO_yyback(int *, int);
void EPSIO_yyerror(char *);

/* You may need to put /DLEX_SCANNER in your makefile
 * if you're using LEX!
 */
void EPSIO_yyoutput(int);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 50 "./parser.y"
typedef union YYSTYPE {
    char *s;
/*    struct pexpr *expr; */
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 168 "y.tab.c"
# define EPSIO_yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 180 "y.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (EPSIO_yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (EPSIO_yyoverflow) || YYERROR_VERBOSE */


#if (! defined (EPSIO_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union EPSIO_yyalloc
{
  short int EPSIO_yyss;
  YYSTYPE EPSIO_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union EPSIO_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T EPSIO_yyi;				\
	  for (EPSIO_yyi = 0; EPSIO_yyi < (Count); EPSIO_yyi++)	\
	    (To)[EPSIO_yyi] = (From)[EPSIO_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T EPSIO_yynewbytes;						\
	YYCOPY (&EPSIO_yyptr->Stack, Stack, EPSIO_yysize);				\
	Stack = &EPSIO_yyptr->Stack;						\
	EPSIO_yynewbytes = EPSIO_yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	EPSIO_yyptr += EPSIO_yynewbytes / sizeof (*EPSIO_yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char EPSIO_yysigned_char;
#else
   typedef short int EPSIO_yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   32

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  16
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  7
/* YYNRULES -- Number of rules. */
#define YYNRULES  20
/* YYNRULES -- Number of states. */
#define YYNSTATES  33

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   257

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? EPSIO_yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char EPSIO_yytranslate[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    14,     2,     6,    15,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char EPSIO_yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    13,    16,    21,    25,
      26,    28,    32,    36,    38,    40,    42,    44,    48,    52,
      58
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const EPSIO_yysigned_char EPSIO_yyrhs[] =
{
      17,     0,    -1,    -1,    17,    18,    -1,     4,     6,    -1,
      19,     6,    -1,     1,     6,    -1,     4,     7,    20,     8,
      -1,    12,    20,    13,    -1,    -1,    21,    -1,    21,     9,
      20,    -1,     4,    14,    22,    -1,    22,    -1,     4,    -1,
       5,    -1,     3,    -1,     3,     6,     3,    -1,     3,    15,
       3,    -1,     3,     6,     3,    15,     3,    -1,    19,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char EPSIO_yyrline[] =
{
       0,    77,    77,    78,    81,    83,    85,    89,    91,    96,
      97,   100,   104,   107,   111,   113,   115,   117,   119,   122,
     126
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const EPSIO_yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "WORD", "STRING", "PERIOD",
  "OPEN", "CLOSE", "COMMA", "NEWLINE", "ERROR", "OPEN_SQUARE",
  "CLOSE_SQUARE", "EQUALS", "EXP", "$accept", "commands", "command",
  "expr", "arglist", "arg", "arg1", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int EPSIO_yytoknum[] =
{
       0,   256,   257,     1,     2,     3,    13,     4,     5,     6,
       7,     8,     9,    10,    11,    14
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char EPSIO_yyr1[] =
{
       0,    16,    17,    17,    18,    18,    18,    19,    19,    20,
      20,    20,    21,    21,    22,    22,    22,    22,    22,    22,
      22
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char EPSIO_yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     4,     3,     0,
       1,     3,     3,     1,     1,     1,     1,     3,     3,     5,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char EPSIO_yydefact[] =
{
       2,     0,     1,     0,     0,     9,     3,     0,     6,     4,
       9,    16,    14,    15,    20,     0,    10,    13,     5,     0,
       0,     0,     0,     8,     9,     7,    17,    18,    14,    12,
      11,     0,    19
};

/* YYDEFGOTO[NTERM-NUM]. */
static const EPSIO_yysigned_char EPSIO_yydefgoto[] =
{
      -1,     1,     6,    14,    15,    16,    17
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -11
static const EPSIO_yysigned_char EPSIO_yypact[] =
{
     -11,     1,   -11,    -2,    13,     3,   -11,    11,   -11,   -11,
       3,    -3,     9,   -11,   -11,     8,    15,   -11,   -11,    14,
      22,    23,     6,   -11,     3,   -11,    12,   -11,    21,   -11,
     -11,    26,   -11
};

/* YYPGOTO[NTERM-NUM].  */
static const EPSIO_yysigned_char EPSIO_yypgoto[] =
{
     -11,   -11,   -11,    29,   -10,   -11,    10
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char EPSIO_yytable[] =
{
      19,     2,     3,    20,     8,     4,    11,    12,    13,    11,
      28,    13,    21,     5,    30,     5,    10,    18,     5,     9,
      10,    23,    25,    22,    24,    26,    27,    31,    10,    32,
       7,     0,    29
};

static const EPSIO_yysigned_char EPSIO_yycheck[] =
{
      10,     0,     1,     6,     6,     4,     3,     4,     5,     3,
       4,     5,    15,    12,    24,    12,     7,     6,    12,     6,
       7,    13,     8,    14,     9,     3,     3,    15,     7,     3,
       1,    -1,    22
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char EPSIO_yystos[] =
{
       0,    17,     0,     1,     4,    12,    18,    19,     6,     6,
       7,     3,     4,     5,    19,    20,    21,    22,     6,    20,
       6,    15,    14,    13,     9,     8,     3,     3,     4,    22,
      20,    15,     3
};

#define EPSIO_yyerrok		(EPSIO_yyerrstatus = 0)
#define EPSIO_yyclearin	(EPSIO_yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto EPSIO_yyacceptlab
#define YYABORT		goto EPSIO_yyabortlab
#define YYERROR		goto EPSIO_yyerrorlab


/* Like YYERROR except do call EPSIO_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto EPSIO_yyerrlab

#define YYRECOVERING()  (!!EPSIO_yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (EPSIO_yychar == YYEMPTY && EPSIO_yylen == 1)				\
    {								\
      EPSIO_yychar = (Token);						\
      EPSIO_yylval = (Value);						\
      EPSIO_yytoken = YYTRANSLATE (EPSIO_yychar);				\
      YYPOPSTACK;						\
      goto EPSIO_yybackup;						\
    }								\
  else								\
    {								\
      EPSIO_yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `EPSIO_yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX EPSIO_yylex (YYLEX_PARAM)
#else
# define YYLEX EPSIO_yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (EPSIO_yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (EPSIO_yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      EPSIO_yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| EPSIO_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
EPSIO_yy_stack_print (short int *bottom, short int *top)
#else
static void
EPSIO_yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (EPSIO_yydebug)							\
    EPSIO_yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
EPSIO_yy_reduce_print (int EPSIO_yyrule)
#else
static void
EPSIO_yy_reduce_print (EPSIO_yyrule)
    int EPSIO_yyrule;
#endif
{
  int EPSIO_yyi;
  unsigned long int EPSIO_yylno = EPSIO_yyrline[EPSIO_yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             EPSIO_yyrule - 1, EPSIO_yylno);
  /* Print the symbols being reduced, and their result.  */
  for (EPSIO_yyi = EPSIO_yyprhs[EPSIO_yyrule]; 0 <= EPSIO_yyrhs[EPSIO_yyi]; EPSIO_yyi++)
    YYFPRINTF (stderr, "%s ", EPSIO_yytname[EPSIO_yyrhs[EPSIO_yyi]]);
  YYFPRINTF (stderr, "-> %s\n", EPSIO_yytname[EPSIO_yyr1[EPSIO_yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (EPSIO_yydebug)				\
    EPSIO_yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int EPSIO_yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef EPSIO_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define EPSIO_yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
EPSIO_yystrlen (const char *EPSIO_yystr)
#   else
EPSIO_yystrlen (EPSIO_yystr)
     const char *EPSIO_yystr;
#   endif
{
  const char *EPSIO_yys = EPSIO_yystr;

  while (*EPSIO_yys++ != '\0')
    continue;

  return EPSIO_yys - EPSIO_yystr - 1;
}
#  endif
# endif

# ifndef EPSIO_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define EPSIO_yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
EPSIO_yystpcpy (char *EPSIO_yydest, const char *EPSIO_yysrc)
#   else
EPSIO_yystpcpy (EPSIO_yydest, EPSIO_yysrc)
     char *EPSIO_yydest;
     const char *EPSIO_yysrc;
#   endif
{
  char *EPSIO_yyd = EPSIO_yydest;
  const char *EPSIO_yys = EPSIO_yysrc;

  while ((*EPSIO_yyd++ = *EPSIO_yys++) != '\0')
    continue;

  return EPSIO_yyd - 1;
}
#  endif
# endif

# ifndef EPSIO_yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for EPSIO_yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from EPSIO_yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
EPSIO_yytnamerr (char *EPSIO_yyres, const char *EPSIO_yystr)
{
  if (*EPSIO_yystr == '"')
    {
      size_t EPSIO_yyn = 0;
      char const *EPSIO_yyp = EPSIO_yystr;

      for (;;)
	switch (*++EPSIO_yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++EPSIO_yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (EPSIO_yyres)
	      EPSIO_yyres[EPSIO_yyn] = *EPSIO_yyp;
	    EPSIO_yyn++;
	    break;

	  case '"':
	    if (EPSIO_yyres)
	      EPSIO_yyres[EPSIO_yyn] = '\0';
	    return EPSIO_yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! EPSIO_yyres)
    return EPSIO_yystrlen (EPSIO_yystr);

  return EPSIO_yystpcpy (EPSIO_yyres, EPSIO_yystr) - EPSIO_yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
EPSIO_yysymprint (FILE *EPSIO_yyoutput, int EPSIO_yytype, YYSTYPE *EPSIO_yyvaluep)
#else
static void
EPSIO_yysymprint (EPSIO_yyoutput, EPSIO_yytype, EPSIO_yyvaluep)
    FILE *EPSIO_yyoutput;
    int EPSIO_yytype;
    YYSTYPE *EPSIO_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) EPSIO_yyvaluep;

  if (EPSIO_yytype < YYNTOKENS)
    YYFPRINTF (EPSIO_yyoutput, "token %s (", EPSIO_yytname[EPSIO_yytype]);
  else
    YYFPRINTF (EPSIO_yyoutput, "nterm %s (", EPSIO_yytname[EPSIO_yytype]);


# ifdef YYPRINT
  if (EPSIO_yytype < YYNTOKENS)
    YYPRINT (EPSIO_yyoutput, EPSIO_yytoknum[EPSIO_yytype], *EPSIO_yyvaluep);
# endif
  switch (EPSIO_yytype)
    {
      default:
        break;
    }
  YYFPRINTF (EPSIO_yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
EPSIO_yydestruct (const char *EPSIO_yymsg, int EPSIO_yytype, YYSTYPE *EPSIO_yyvaluep)
#else
static void
EPSIO_yydestruct (EPSIO_yymsg, EPSIO_yytype, EPSIO_yyvaluep)
    const char *EPSIO_yymsg;
    int EPSIO_yytype;
    YYSTYPE *EPSIO_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) EPSIO_yyvaluep;

  if (!EPSIO_yymsg)
    EPSIO_yymsg = "Deleting";
  YY_SYMBOL_PRINT (EPSIO_yymsg, EPSIO_yytype, EPSIO_yyvaluep, EPSIO_yylocationp);

  switch (EPSIO_yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int EPSIO_yyparse (void *YYPARSE_PARAM);
# else
int EPSIO_yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int EPSIO_yyparse (void);
#else
int EPSIO_yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int EPSIO_yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE EPSIO_yylval;

/* Number of syntax errors so far.  */
int EPSIO_yynerrs;



/*----------.
| EPSIO_yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int EPSIO_yyparse (void *YYPARSE_PARAM)
# else
int EPSIO_yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
EPSIO_yyparse (void)
#else
int
EPSIO_yyparse ()
    ;
#endif
#endif
{
  
  int EPSIO_yystate;
  int EPSIO_yyn;
  int EPSIO_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int EPSIO_yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int EPSIO_yytoken = 0;

  /* Three stacks and their tools:
     `EPSIO_yyss': related to states,
     `EPSIO_yyvs': related to semantic values,
     `EPSIO_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow EPSIO_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int EPSIO_yyssa[YYINITDEPTH];
  short int *EPSIO_yyss = EPSIO_yyssa;
  short int *EPSIO_yyssp;

  /* The semantic value stack.  */
  YYSTYPE EPSIO_yyvsa[YYINITDEPTH];
  YYSTYPE *EPSIO_yyvs = EPSIO_yyvsa;
  YYSTYPE *EPSIO_yyvsp;



#define YYPOPSTACK   (EPSIO_yyvsp--, EPSIO_yyssp--)

  YYSIZE_T EPSIO_yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE EPSIO_yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int EPSIO_yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  EPSIO_yystate = 0;
  EPSIO_yyerrstatus = 0;
  EPSIO_yynerrs = 0;
  EPSIO_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  EPSIO_yyssp = EPSIO_yyss;
  EPSIO_yyvsp = EPSIO_yyvs;

  goto EPSIO_yysetstate;

/*------------------------------------------------------------.
| EPSIO_yynewstate -- Push a new state, which is found in EPSIO_yystate.  |
`------------------------------------------------------------*/
 EPSIO_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  EPSIO_yyssp++;

 EPSIO_yysetstate:
  *EPSIO_yyssp = EPSIO_yystate;

  if (EPSIO_yyss + EPSIO_yystacksize - 1 <= EPSIO_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T EPSIO_yysize = EPSIO_yyssp - EPSIO_yyss + 1;

#ifdef EPSIO_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *EPSIO_yyvs1 = EPSIO_yyvs;
	short int *EPSIO_yyss1 = EPSIO_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if EPSIO_yyoverflow is a macro.  */
	EPSIO_yyoverflow (YY_("memory exhausted"),
		    &EPSIO_yyss1, EPSIO_yysize * sizeof (*EPSIO_yyssp),
		    &EPSIO_yyvs1, EPSIO_yysize * sizeof (*EPSIO_yyvsp),

		    &EPSIO_yystacksize);

	EPSIO_yyss = EPSIO_yyss1;
	EPSIO_yyvs = EPSIO_yyvs1;
      }
#else /* no EPSIO_yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto EPSIO_yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= EPSIO_yystacksize)
	goto EPSIO_yyexhaustedlab;
      EPSIO_yystacksize *= 2;
      if (YYMAXDEPTH < EPSIO_yystacksize)
	EPSIO_yystacksize = YYMAXDEPTH;

      {
	short int *EPSIO_yyss1 = EPSIO_yyss;
	union EPSIO_yyalloc *EPSIO_yyptr =
	  (union EPSIO_yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (EPSIO_yystacksize));
	if (! EPSIO_yyptr)
	  goto EPSIO_yyexhaustedlab;
	YYSTACK_RELOCATE (EPSIO_yyss);
	YYSTACK_RELOCATE (EPSIO_yyvs);

#  undef YYSTACK_RELOCATE
	if (EPSIO_yyss1 != EPSIO_yyssa)
	  YYSTACK_FREE (EPSIO_yyss1);
      }
# endif
#endif /* no EPSIO_yyoverflow */

      EPSIO_yyssp = EPSIO_yyss + EPSIO_yysize - 1;
      EPSIO_yyvsp = EPSIO_yyvs + EPSIO_yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) EPSIO_yystacksize));

      if (EPSIO_yyss + EPSIO_yystacksize - 1 <= EPSIO_yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", EPSIO_yystate));

  goto EPSIO_yybackup;

/*-----------.
| EPSIO_yybackup.  |
`-----------*/
EPSIO_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* EPSIO_yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  EPSIO_yyn = EPSIO_yypact[EPSIO_yystate];
  if (EPSIO_yyn == YYPACT_NINF)
    goto EPSIO_yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (EPSIO_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      EPSIO_yychar = YYLEX;
    }

  if (EPSIO_yychar <= YYEOF)
    {
      EPSIO_yychar = EPSIO_yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of EPSIO_input.\n"));
    }
  else
    {
      EPSIO_yytoken = YYTRANSLATE (EPSIO_yychar);
      YY_SYMBOL_PRINT ("Next token is", EPSIO_yytoken, &EPSIO_yylval, &EPSIO_yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  EPSIO_yyn += EPSIO_yytoken;
  if (EPSIO_yyn < 0 || YYLAST < EPSIO_yyn || EPSIO_yycheck[EPSIO_yyn] != EPSIO_yytoken)
    goto EPSIO_yydefault;
  EPSIO_yyn = EPSIO_yytable[EPSIO_yyn];
  if (EPSIO_yyn <= 0)
    {
      if (EPSIO_yyn == 0 || EPSIO_yyn == YYTABLE_NINF)
	goto EPSIO_yyerrlab;
      EPSIO_yyn = -EPSIO_yyn;
      goto EPSIO_yyreduce;
    }

  if (EPSIO_yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", EPSIO_yytoken, &EPSIO_yylval, &EPSIO_yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (EPSIO_yychar != YYEOF)
    EPSIO_yychar = YYEMPTY;

  *++EPSIO_yyvsp = EPSIO_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (EPSIO_yyerrstatus)
    EPSIO_yyerrstatus--;

  EPSIO_yystate = EPSIO_yyn;
  goto EPSIO_yynewstate;


/*-----------------------------------------------------------.
| EPSIO_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
EPSIO_yydefault:
  EPSIO_yyn = EPSIO_yydefact[EPSIO_yystate];
  if (EPSIO_yyn == 0)
    goto EPSIO_yyerrlab;
  goto EPSIO_yyreduce;


/*-----------------------------.
| EPSIO_yyreduce -- Do a reduction.  |
`-----------------------------*/
EPSIO_yyreduce:
  /* EPSIO_yyn is the number of a rule to reduce with.  */
  EPSIO_yylen = EPSIO_yyr2[EPSIO_yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  EPSIO_yyval = EPSIO_yyvsp[1-EPSIO_yylen];


  YY_REDUCE_PRINT (EPSIO_yyn);
  switch (EPSIO_yyn)
    {
        case 4:
#line 82 "./parser.y"
    {process_eps_cmd(epsio_cons(iomake_word((EPSIO_yyvsp[-1].s)), NULL)); free((EPSIO_yyvsp[-1].s));}
    break;

  case 5:
#line 84 "./parser.y"
    {process_eps_cmd((EPSIO_yyvsp[-1].s));}
    break;

  case 6:
#line 86 "./parser.y"
    {error_syntax("Unrecognized command.");}
    break;

  case 7:
#line 90 "./parser.y"
    {(EPSIO_yyval.s) = epsio_cons(iomake_word((EPSIO_yyvsp[-3].s)), (EPSIO_yyvsp[-1].s)); free((EPSIO_yyvsp[-3].s));}
    break;

  case 8:
#line 92 "./parser.y"
    {(EPSIO_yyval.s) = (EPSIO_yyvsp[-1].s); }
    break;

  case 9:
#line 96 "./parser.y"
    {(EPSIO_yyval.s) = epsio_cons(NULL, NULL);}
    break;

  case 10:
#line 98 "./parser.y"
    {(EPSIO_yyval.s) = epsio_cons((EPSIO_yyvsp[0].s), NULL);}
    break;

  case 11:
#line 101 "./parser.y"
    {(EPSIO_yyval.s) = epsio_cons((EPSIO_yyvsp[-2].s), (EPSIO_yyvsp[0].s));}
    break;

  case 12:
#line 105 "./parser.y"
    {(EPSIO_yyval.s) = epsio_cons(iomake_word("="), epsio_cons(iomake_word((EPSIO_yyvsp[-2].s)), epsio_cons((EPSIO_yyvsp[0].s), NULL)));
                         free((EPSIO_yyvsp[-2].s)); }
    break;

  case 13:
#line 108 "./parser.y"
    {(EPSIO_yyval.s) = (EPSIO_yyvsp[0].s); }
    break;

  case 14:
#line 112 "./parser.y"
    {(EPSIO_yyval.s) = iomake_word((EPSIO_yyvsp[0].s)); free((EPSIO_yyvsp[0].s));}
    break;

  case 15:
#line 114 "./parser.y"
    {(EPSIO_yyval.s) = iomake_string((EPSIO_yyvsp[0].s)); free((EPSIO_yyvsp[0].s));}
    break;

  case 16:
#line 116 "./parser.y"
    {(EPSIO_yyval.s) = iomake_integer((EPSIO_yyvsp[0].s)); free((EPSIO_yyvsp[0].s));}
    break;

  case 17:
#line 118 "./parser.y"
    {(EPSIO_yyval.s) = iomake_real((EPSIO_yyvsp[-2].s), (EPSIO_yyvsp[0].s)); free((EPSIO_yyvsp[-2].s)); free((EPSIO_yyvsp[0].s)); }
    break;

  case 18:
#line 120 "./parser.y"
    {(EPSIO_yyval.s) = iomake_exp((EPSIO_yyvsp[-2].s), (EPSIO_yyvsp[0].s)); free((EPSIO_yyvsp[-2].s)); free((EPSIO_yyvsp[0].s)); }
    break;

  case 19:
#line 123 "./parser.y"
    {(EPSIO_yyval.s) = iomake_exp2((EPSIO_yyvsp[-4].s), (EPSIO_yyvsp[-2].s), (EPSIO_yyvsp[0].s)); free((EPSIO_yyvsp[-4].s)); free((EPSIO_yyvsp[-2].s));
                                                                  free((EPSIO_yyvsp[0].s)); }
    break;

  case 20:
#line 127 "./parser.y"
    {(EPSIO_yyval.s) = (EPSIO_yyvsp[0].s);}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1271 "y.tab.c"

  EPSIO_yyvsp -= EPSIO_yylen;
  EPSIO_yyssp -= EPSIO_yylen;


  YY_STACK_PRINT (EPSIO_yyss, EPSIO_yyssp);

  *++EPSIO_yyvsp = EPSIO_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  EPSIO_yyn = EPSIO_yyr1[EPSIO_yyn];

  EPSIO_yystate = EPSIO_yypgoto[EPSIO_yyn - YYNTOKENS] + *EPSIO_yyssp;
  if (0 <= EPSIO_yystate && EPSIO_yystate <= YYLAST && EPSIO_yycheck[EPSIO_yystate] == *EPSIO_yyssp)
    EPSIO_yystate = EPSIO_yytable[EPSIO_yystate];
  else
    EPSIO_yystate = EPSIO_yydefgoto[EPSIO_yyn - YYNTOKENS];

  goto EPSIO_yynewstate;


/*------------------------------------.
| EPSIO_yyerrlab -- here on detecting error |
`------------------------------------*/
EPSIO_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!EPSIO_yyerrstatus)
    {
      ++EPSIO_yynerrs;
#if YYERROR_VERBOSE
      EPSIO_yyn = EPSIO_yypact[EPSIO_yystate];

      if (YYPACT_NINF < EPSIO_yyn && EPSIO_yyn < YYLAST)
	{
	  int EPSIO_yytype = YYTRANSLATE (EPSIO_yychar);
	  YYSIZE_T EPSIO_yysize0 = EPSIO_yytnamerr (0, EPSIO_yytname[EPSIO_yytype]);
	  YYSIZE_T EPSIO_yysize = EPSIO_yysize0;
	  YYSIZE_T EPSIO_yysize1;
	  int EPSIO_yysize_overflow = 0;
	  char *EPSIO_yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *EPSIO_yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int EPSIO_yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *EPSIO_yyfmt;
	  char const *EPSIO_yyf;
	  static char const EPSIO_yyunexpected[] = "syntax error, unexpected %s";
	  static char const EPSIO_yyexpecting[] = ", expecting %s";
	  static char const EPSIO_yyor[] = " or %s";
	  char EPSIO_yyformat[sizeof EPSIO_yyunexpected
			+ sizeof EPSIO_yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof EPSIO_yyor - 1))];
	  char const *EPSIO_yyprefix = EPSIO_yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int EPSIO_yyxbegin = EPSIO_yyn < 0 ? -EPSIO_yyn : 0;

	  /* Stay within bounds of both EPSIO_yycheck and EPSIO_yytname.  */
	  int EPSIO_yychecklim = YYLAST - EPSIO_yyn;
	  int EPSIO_yyxend = EPSIO_yychecklim < YYNTOKENS ? EPSIO_yychecklim : YYNTOKENS;
	  int EPSIO_yycount = 1;

	  EPSIO_yyarg[0] = EPSIO_yytname[EPSIO_yytype];
	  EPSIO_yyfmt = EPSIO_yystpcpy (EPSIO_yyformat, EPSIO_yyunexpected);

	  for (EPSIO_yyx = EPSIO_yyxbegin; EPSIO_yyx < EPSIO_yyxend; ++EPSIO_yyx)
	    if (EPSIO_yycheck[EPSIO_yyx + EPSIO_yyn] == EPSIO_yyx && EPSIO_yyx != YYTERROR)
	      {
		if (EPSIO_yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    EPSIO_yycount = 1;
		    EPSIO_yysize = EPSIO_yysize0;
		    EPSIO_yyformat[sizeof EPSIO_yyunexpected - 1] = '\0';
		    break;
		  }
		EPSIO_yyarg[EPSIO_yycount++] = EPSIO_yytname[EPSIO_yyx];
		EPSIO_yysize1 = EPSIO_yysize + EPSIO_yytnamerr (0, EPSIO_yytname[EPSIO_yyx]);
		EPSIO_yysize_overflow |= EPSIO_yysize1 < EPSIO_yysize;
		EPSIO_yysize = EPSIO_yysize1;
		EPSIO_yyfmt = EPSIO_yystpcpy (EPSIO_yyfmt, EPSIO_yyprefix);
		EPSIO_yyprefix = EPSIO_yyor;
	      }

	  EPSIO_yyf = YY_(EPSIO_yyformat);
	  EPSIO_yysize1 = EPSIO_yysize + EPSIO_yystrlen (EPSIO_yyf);
	  EPSIO_yysize_overflow |= EPSIO_yysize1 < EPSIO_yysize;
	  EPSIO_yysize = EPSIO_yysize1;

	  if (!EPSIO_yysize_overflow && EPSIO_yysize <= YYSTACK_ALLOC_MAXIMUM)
	    EPSIO_yymsg = (char *) YYSTACK_ALLOC (EPSIO_yysize);
	  if (EPSIO_yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *EPSIO_yyp = EPSIO_yymsg;
	      int EPSIO_yyi = 0;
	      while ((*EPSIO_yyp = *EPSIO_yyf))
		{
		  if (*EPSIO_yyp == '%' && EPSIO_yyf[1] == 's' && EPSIO_yyi < EPSIO_yycount)
		    {
		      EPSIO_yyp += EPSIO_yytnamerr (EPSIO_yyp, EPSIO_yyarg[EPSIO_yyi++]);
		      EPSIO_yyf += 2;
		    }
		  else
		    {
		      EPSIO_yyp++;
		      EPSIO_yyf++;
		    }
		}
	      EPSIO_yyerror (EPSIO_yymsg);
	      YYSTACK_FREE (EPSIO_yymsg);
	    }
	  else
	    {
	      EPSIO_yyerror (YY_("syntax error"));
	      goto EPSIO_yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	EPSIO_yyerror (YY_("syntax error"));
    }



  if (EPSIO_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (EPSIO_yychar <= YYEOF)
        {
	  /* Return failure if at end of EPSIO_input.  */
	  if (EPSIO_yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  EPSIO_yydestruct ("Error: discarding", EPSIO_yytoken, &EPSIO_yylval);
	  EPSIO_yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto EPSIO_yyerrlab1;


/*---------------------------------------------------.
| EPSIO_yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
EPSIO_yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label EPSIO_yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto EPSIO_yyerrorlab;

EPSIO_yyvsp -= EPSIO_yylen;
  EPSIO_yyssp -= EPSIO_yylen;
  EPSIO_yystate = *EPSIO_yyssp;
  goto EPSIO_yyerrlab1;


/*-------------------------------------------------------------.
| EPSIO_yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
EPSIO_yyerrlab1:
  EPSIO_yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      EPSIO_yyn = EPSIO_yypact[EPSIO_yystate];
      if (EPSIO_yyn != YYPACT_NINF)
	{
	  EPSIO_yyn += YYTERROR;
	  if (0 <= EPSIO_yyn && EPSIO_yyn <= YYLAST && EPSIO_yycheck[EPSIO_yyn] == YYTERROR)
	    {
	      EPSIO_yyn = EPSIO_yytable[EPSIO_yyn];
	      if (0 < EPSIO_yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (EPSIO_yyssp == EPSIO_yyss)
	YYABORT;


      EPSIO_yydestruct ("Error: popping", EPSIO_yystos[EPSIO_yystate], EPSIO_yyvsp);
      YYPOPSTACK;
      EPSIO_yystate = *EPSIO_yyssp;
      YY_STACK_PRINT (EPSIO_yyss, EPSIO_yyssp);
    }

  if (EPSIO_yyn == YYFINAL)
    YYACCEPT;

  *++EPSIO_yyvsp = EPSIO_yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", EPSIO_yystos[EPSIO_yyn], EPSIO_yyvsp, EPSIO_yylsp);

  EPSIO_yystate = EPSIO_yyn;
  goto EPSIO_yynewstate;


/*-------------------------------------.
| EPSIO_yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
EPSIO_yyacceptlab:
  EPSIO_yyresult = 0;
  goto EPSIO_yyreturn;

/*-----------------------------------.
| EPSIO_yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
EPSIO_yyabortlab:
  EPSIO_yyresult = 1;
  goto EPSIO_yyreturn;

#ifndef EPSIO_yyoverflow
/*-------------------------------------------------.
| EPSIO_yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
EPSIO_yyexhaustedlab:
  EPSIO_yyerror (YY_("memory exhausted"));
  EPSIO_yyresult = 2;
  /* Fall through.  */
#endif

EPSIO_yyreturn:
  if (EPSIO_yychar != YYEOF && EPSIO_yychar != YYEMPTY)
     EPSIO_yydestruct ("Cleanup: discarding lookahead",
		 EPSIO_yytoken, &EPSIO_yylval);
  while (EPSIO_yyssp != EPSIO_yyss)
    {
      EPSIO_yydestruct ("Cleanup: popping",
		  EPSIO_yystos[*EPSIO_yyssp], EPSIO_yyvsp);
      YYPOPSTACK;
    }
#ifndef EPSIO_yyoverflow
  if (EPSIO_yyss != EPSIO_yyssa)
    YYSTACK_FREE (EPSIO_yyss);
#endif
  return EPSIO_yyresult;
}


#line 130 "./parser.y"


/* We include ms_lexer.c if we are building for gtk, wine or motif
 * and also whenever we are using configure (marked by __WX_SETUP_H__) for,
 * for example, cross compilation. */
#if (defined(__WXGTK__) || defined(__WXWINE__) || defined(__WXMOTIF__)) || defined(__WX_SETUP_H__) && !defined(NO_CONFIGURE)
#include "ms_lexer.c"
#elif defined(__WXMAC__) && defined(__APPLE__)
#include "ms_lexer.c"
#elif defined(__MWERKS__)
#include "../common/cwlex_EPSIO_yy.c"
#elif defined(__SC__)
#include "ms_lexer.c"
#else
#include "../common/lex_EPSIO_yy.c"
#endif

/*
void EPSIO_yyerror(s)
char *s;
{
  error_syntax(s);
}
*/

/* Ansi prototype. If this doesn't work for you... uncomment
   the above instead.
 */

void EPSIO_yyerror(char *s)
{
  error_syntax(s);
}

/*
 * Unfortunately, my DOS version of FLEX
 * requires EPSIO_yywrap to be #def'ed, whereas
 * the UNIX flex expects a proper function.
 */

/* At least on alphaev6-dec-osf4.0e EPSIO_yywrap() must be #define'd.
 * RL: ... but on Debian/Alpha(linux) it must not, so hopefully
 *     testing for __OSF__ here is what we really want.
 */
#ifdef __OSF__
#ifndef EPSIO_yywrap
#define EPSIO_yywrap() 1
#endif
#else
/* HH: Added test for __WX_SETUP_H__ for gnuwin builds
 * using configure */ 
#if !defined(__SC__) && !defined(__GNUWIN32__) 
#ifdef USE_DEFINE
#ifndef EPSIO_yywrap
#define EPSIO_yywrap() 1
#endif
#elif !defined(__ultrix)
int EPSIO_yywrap() { return 1; }
#elif defined(__VMS__)
int EPSIO_yywrap() { return 1; }
#endif
#elif defined(__WX_SETUP_H__) || defined(__SC__)
int EPSIO_yywrap() { return 1; }
#endif
#endif

#if defined(__cplusplus) || defined(__STDC__)
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif
#endif


