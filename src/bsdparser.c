#include <stdlib.h>
#include <string.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX EPSIO_yylex()
#define YYEMPTY -1
#define EPSIO_yyclearin (EPSIO_yychar=(YYEMPTY))
#define EPSIO_yyerrok (EPSIO_yyerrflag=0)
#define YYRECOVERING() (EPSIO_yyerrflag!=0)
#define YYPREFIX "EPSIO_yy"
#line 3 "./parser.y"
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
void EPSIO_yyerror(const char *);

/* help for adding const */
#undef _free
#define _free(a) free((void*)(a))

/* You may need to put /DLEX_SCANNER in your makefile
 * if you're using LEX!
 */
void EPSIO_yyoutput(int);

#line 54 "./parser.y"
#ifndef YYSTYPE_DEFINED
#define YYSTYPE_DEFINED
typedef union {
    const char *s;
/*    struct pexpr *expr; */
} YYSTYPE;
#endif /* YYSTYPE_DEFINED */
#line 71 "y.tab.c"
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
#define YYERRCODE 256
const short EPSIO_yylhs[] =
	{                                        -1,
    0,    0,    1,    1,    1,    2,    2,    3,    3,    3,
    4,    4,    5,    5,    5,    5,    5,    5,    5,
};
const short EPSIO_yylen[] =
	{                                         2,
    0,    2,    2,    2,    2,    4,    3,    0,    1,    3,
    3,    1,    1,    1,    1,    3,    3,    5,    1,
};
const short EPSIO_yydefred[] =
	{                                      1,
    0,    0,    0,    0,    2,    0,    5,    3,    0,    0,
    0,   14,   19,    0,    0,   12,    4,    0,    0,    0,
    0,    7,    0,    6,    0,   17,    0,   11,   10,    0,
   18,
};
const short EPSIO_yydgoto[] =
	{                                       1,
    5,   13,   14,   15,   16,
};
const short EPSIO_yysindex[] =
	{                                      0,
   -2,  -11,   -1,    7,    0,    1,    0,    0,    7,   -9,
    2,    0,    0,   10,    5,    0,    0,   19,   20,   35,
   16,    0,    7,    0,   12,    0,   34,    0,    0,   39,
    0,};
const short EPSIO_yyrindex[] =
	{                                      0,
    0,    0,    0,   31,    0,    0,    0,    0,   37,   17,
   23,    0,    0,    0,   27,    0,    0,    0,    0,    0,
    0,    0,   29,    0,   25,    0,   23,    0,    0,    0,
    0,};
const short EPSIO_yygindex[] =
	{                                      0,
    0,   42,   -8,    0,   24,
};
#define YYTABLESIZE 254
const short EPSIO_yytable[] =
	{                                       3,
   18,    7,    9,   19,   20,    9,    4,   10,   11,   12,
   23,    8,   21,   17,   29,    4,   10,   27,   12,   22,
   25,   15,   15,   24,    4,   30,   15,   13,   13,   16,
   16,    9,   13,    8,   16,   26,    9,    9,    8,   31,
    8,    8,    6,    0,   28,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    2,
};
const short EPSIO_yycheck[] =
	{                                       2,
    9,   13,    4,   13,   14,    4,    9,    1,    2,    3,
    6,   13,   11,   13,   23,    9,    1,    2,    3,   10,
    1,    5,    6,    5,    9,   14,   10,    5,    6,    5,
    6,    5,   10,    5,   10,    1,   10,    4,   10,    1,
   10,    5,    1,   -1,   21,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  256,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 14
#if YYDEBUG
const char * const EPSIO_yyname[] =
	{
"end-of-file","INTEGER","WORD","STRING","OPEN","CLOSE","COMMA","NEWLINE",
"ERROR","OPEN_SQUARE","CLOSE_SQUARE","EQUALS",0,"PERIOD","EXP",
};
const char * const EPSIO_yyrule[] =
	{"$accept : commands",
"commands :",
"commands : commands command",
"command : WORD PERIOD",
"command : expr PERIOD",
"command : error PERIOD",
"expr : WORD OPEN arglist CLOSE",
"expr : OPEN_SQUARE arglist CLOSE_SQUARE",
"arglist :",
"arglist : arg",
"arglist : arg COMMA arglist",
"arg : WORD EQUALS arg1",
"arg : arg1",
"arg1 : WORD",
"arg1 : STRING",
"arg1 : INTEGER",
"arg1 : INTEGER PERIOD INTEGER",
"arg1 : INTEGER EXP INTEGER",
"arg1 : INTEGER PERIOD INTEGER EXP INTEGER",
"arg1 : expr",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
/* LINTUSED */
int EPSIO_yydebug;
int EPSIO_yynerrs;
int EPSIO_yyerrflag;
int EPSIO_yychar;
short *EPSIO_yyssp;
YYSTYPE *EPSIO_yyvsp;
YYSTYPE EPSIO_yyval;
YYSTYPE EPSIO_yylval;
short *EPSIO_yyss;
short *EPSIO_yysslim;
YYSTYPE *EPSIO_yyvs;
unsigned int EPSIO_yystacksize;
#line 135 "./parser.y"

/*
 * Always include lexer; see comment below for historical
 * interest.  Note that with Epspline prebuilt lexer and parser
 * are included and used everywhere but {Free,Net,Open}BSD.
 * (flex invoked as lex, at least on OpenBSD, does not make
 * width based typedefs that might fail in prebuilt code
 * compiled elsewhere)
 */
#if 1 /* EH, include lexer */
#include "lexer.c"
#else /* EH, include lexer */
/* We include lexer.c if we are building for gtk, wine or motif
 * and also whenever we are using configure (marked by __WX_SETUP_H__) for,
 * for example, cross compilation. */
#if (defined(__WXGTK__) || defined(__WXWINE__) || defined(__WXMOTIF__)) || defined(__WX_SETUP_H__) && !defined(NO_CONFIGURE)
#include "lexer.c"
#elif defined(__WXMAC__) && defined(__APPLE__)
#include "lexer.c"
#elif defined(__MWERKS__)
#include "../common/cwlex_EPSIO_yy.c"
#elif defined(__SC__)
#include "lexer.c"
#else
#include "../common/lex_EPSIO_yy.c"
#endif
#endif /* EH, include lexer */

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

void EPSIO_yyerror(const char *s)
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
#line 316 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int EPSIO_yygrowstack(void)
{
    unsigned int newsize;
    long sslen;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = EPSIO_yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    sslen = EPSIO_yyssp - EPSIO_yyss;
#ifdef SIZE_MAX
#define YY_SIZE_MAX SIZE_MAX
#else
#define YY_SIZE_MAX 0xffffffffU
#endif
    if (newsize && YY_SIZE_MAX / newsize < sizeof *newss)
        goto bail;
    newss = EPSIO_yyss ? (short *)realloc(EPSIO_yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss); /* overflow check above */
    if (newss == NULL)
        goto bail;
    EPSIO_yyss = newss;
    EPSIO_yyssp = newss + sslen;
    if (newsize && YY_SIZE_MAX / newsize < sizeof *newvs)
        goto bail;
    newvs = EPSIO_yyvs ? (YYSTYPE *)realloc(EPSIO_yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs); /* overflow check above */
    if (newvs == NULL)
        goto bail;
    EPSIO_yyvs = newvs;
    EPSIO_yyvsp = newvs + sslen;
    EPSIO_yystacksize = newsize;
    EPSIO_yysslim = EPSIO_yyss + newsize - 1;
    return 0;
bail:
    if (EPSIO_yyss)
            free(EPSIO_yyss);
    if (EPSIO_yyvs)
            free(EPSIO_yyvs);
    EPSIO_yyss = EPSIO_yyssp = NULL;
    EPSIO_yyvs = EPSIO_yyvsp = NULL;
    EPSIO_yystacksize = 0;
    return -1;
}

#define YYABORT goto EPSIO_yyabort
#define YYREJECT goto EPSIO_yyabort
#define YYACCEPT goto EPSIO_yyaccept
#define YYERROR goto EPSIO_yyerrlab
int
EPSIO_yyparse(void)
{
    int EPSIO_yym, EPSIO_yyn, EPSIO_yystate;
#if YYDEBUG
    const char *EPSIO_yys;

    if ((EPSIO_yys = getenv("YYDEBUG")))
    {
        EPSIO_yyn = *EPSIO_yys;
        if (EPSIO_yyn >= '0' && EPSIO_yyn <= '9')
            EPSIO_yydebug = EPSIO_yyn - '0';
    }
#endif /* YYDEBUG */

    EPSIO_yynerrs = 0;
    EPSIO_yyerrflag = 0;
    EPSIO_yychar = (-1);

    if (EPSIO_yyss == NULL && EPSIO_yygrowstack()) goto EPSIO_yyoverflow;
    EPSIO_yyssp = EPSIO_yyss;
    EPSIO_yyvsp = EPSIO_yyvs;
    *EPSIO_yyssp = EPSIO_yystate = 0;

EPSIO_yyloop:
    if ((EPSIO_yyn = EPSIO_yydefred[EPSIO_yystate]) != 0) goto EPSIO_yyreduce;
    if (EPSIO_yychar < 0)
    {
        if ((EPSIO_yychar = EPSIO_yylex()) < 0) EPSIO_yychar = 0;
#if YYDEBUG
        if (EPSIO_yydebug)
        {
            EPSIO_yys = 0;
            if (EPSIO_yychar <= YYMAXTOKEN) EPSIO_yys = EPSIO_yyname[EPSIO_yychar];
            if (!EPSIO_yys) EPSIO_yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, EPSIO_yystate, EPSIO_yychar, EPSIO_yys);
        }
#endif
    }
    if ((EPSIO_yyn = EPSIO_yysindex[EPSIO_yystate]) && (EPSIO_yyn += EPSIO_yychar) >= 0 &&
            EPSIO_yyn <= YYTABLESIZE && EPSIO_yycheck[EPSIO_yyn] == EPSIO_yychar)
    {
#if YYDEBUG
        if (EPSIO_yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, EPSIO_yystate, EPSIO_yytable[EPSIO_yyn]);
#endif
        if (EPSIO_yyssp >= EPSIO_yysslim && EPSIO_yygrowstack())
        {
            goto EPSIO_yyoverflow;
        }
        *++EPSIO_yyssp = EPSIO_yystate = EPSIO_yytable[EPSIO_yyn];
        *++EPSIO_yyvsp = EPSIO_yylval;
        EPSIO_yychar = (-1);
        if (EPSIO_yyerrflag > 0)  --EPSIO_yyerrflag;
        goto EPSIO_yyloop;
    }
    if ((EPSIO_yyn = EPSIO_yyrindex[EPSIO_yystate]) && (EPSIO_yyn += EPSIO_yychar) >= 0 &&
            EPSIO_yyn <= YYTABLESIZE && EPSIO_yycheck[EPSIO_yyn] == EPSIO_yychar)
    {
        EPSIO_yyn = EPSIO_yytable[EPSIO_yyn];
        goto EPSIO_yyreduce;
    }
    if (EPSIO_yyerrflag) goto EPSIO_yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto EPSIO_yynewerror;
#endif
EPSIO_yynewerror:
    EPSIO_yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto EPSIO_yyerrlab;
#endif
EPSIO_yyerrlab:
    ++EPSIO_yynerrs;
EPSIO_yyinrecovery:
    if (EPSIO_yyerrflag < 3)
    {
        EPSIO_yyerrflag = 3;
        for (;;)
        {
            if ((EPSIO_yyn = EPSIO_yysindex[*EPSIO_yyssp]) && (EPSIO_yyn += YYERRCODE) >= 0 &&
                    EPSIO_yyn <= YYTABLESIZE && EPSIO_yycheck[EPSIO_yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (EPSIO_yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *EPSIO_yyssp, EPSIO_yytable[EPSIO_yyn]);
#endif
                if (EPSIO_yyssp >= EPSIO_yysslim && EPSIO_yygrowstack())
                {
                    goto EPSIO_yyoverflow;
                }
                *++EPSIO_yyssp = EPSIO_yystate = EPSIO_yytable[EPSIO_yyn];
                *++EPSIO_yyvsp = EPSIO_yylval;
                goto EPSIO_yyloop;
            }
            else
            {
#if YYDEBUG
                if (EPSIO_yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *EPSIO_yyssp);
#endif
                if (EPSIO_yyssp <= EPSIO_yyss) goto EPSIO_yyabort;
                --EPSIO_yyssp;
                --EPSIO_yyvsp;
            }
        }
    }
    else
    {
        if (EPSIO_yychar == 0) goto EPSIO_yyabort;
#if YYDEBUG
        if (EPSIO_yydebug)
        {
            EPSIO_yys = 0;
            if (EPSIO_yychar <= YYMAXTOKEN) EPSIO_yys = EPSIO_yyname[EPSIO_yychar];
            if (!EPSIO_yys) EPSIO_yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, EPSIO_yystate, EPSIO_yychar, EPSIO_yys);
        }
#endif
        EPSIO_yychar = (-1);
        goto EPSIO_yyloop;
    }
EPSIO_yyreduce:
#if YYDEBUG
    if (EPSIO_yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, EPSIO_yystate, EPSIO_yyn, EPSIO_yyrule[EPSIO_yyn]);
#endif
    EPSIO_yym = EPSIO_yylen[EPSIO_yyn];
    if (EPSIO_yym)
        EPSIO_yyval = EPSIO_yyvsp[1-EPSIO_yym];
    else
        memset(&EPSIO_yyval, 0, sizeof EPSIO_yyval);
    switch (EPSIO_yyn)
    {
case 3:
#line 86 "./parser.y"
{process_eps_cmd(epsio_cons(iomake_word(EPSIO_yyvsp[-1].s), NULL)); _free(EPSIO_yyvsp[-1].s);}
break;
case 4:
#line 88 "./parser.y"
{process_eps_cmd(EPSIO_yyvsp[-1].s);}
break;
case 5:
#line 90 "./parser.y"
{error_syntax("Unrecognized command.");}
break;
case 6:
#line 94 "./parser.y"
{EPSIO_yyval.s = epsio_cons(iomake_word(EPSIO_yyvsp[-3].s), EPSIO_yyvsp[-1].s); _free(EPSIO_yyvsp[-3].s);}
break;
case 7:
#line 96 "./parser.y"
{EPSIO_yyval.s = EPSIO_yyvsp[-1].s; }
break;
case 8:
#line 100 "./parser.y"
{EPSIO_yyval.s = epsio_cons(NULL, NULL);}
break;
case 9:
#line 102 "./parser.y"
{EPSIO_yyval.s = epsio_cons(EPSIO_yyvsp[0].s, NULL);}
break;
case 10:
#line 105 "./parser.y"
{EPSIO_yyval.s = epsio_cons(EPSIO_yyvsp[-2].s, EPSIO_yyvsp[0].s);}
break;
case 11:
#line 109 "./parser.y"
{EPSIO_yyval.s = epsio_cons(iomake_word("="), epsio_cons(iomake_word(EPSIO_yyvsp[-2].s), epsio_cons(EPSIO_yyvsp[0].s, NULL)));
                         _free(EPSIO_yyvsp[-2].s); }
break;
case 12:
#line 112 "./parser.y"
{EPSIO_yyval.s = EPSIO_yyvsp[0].s; }
break;
case 13:
#line 116 "./parser.y"
{EPSIO_yyval.s = iomake_word(EPSIO_yyvsp[0].s); _free(EPSIO_yyvsp[0].s);}
break;
case 14:
#line 118 "./parser.y"
{EPSIO_yyval.s = iomake_string(EPSIO_yyvsp[0].s); _free(EPSIO_yyvsp[0].s);}
break;
case 15:
#line 120 "./parser.y"
{EPSIO_yyval.s = iomake_integer(EPSIO_yyvsp[0].s); _free(EPSIO_yyvsp[0].s);}
break;
case 16:
#line 122 "./parser.y"
{EPSIO_yyval.s = iomake_real(EPSIO_yyvsp[-2].s, EPSIO_yyvsp[0].s); _free(EPSIO_yyvsp[-2].s); _free(EPSIO_yyvsp[0].s); }
break;
case 17:
#line 124 "./parser.y"
{EPSIO_yyval.s = iomake_exp(EPSIO_yyvsp[-2].s, EPSIO_yyvsp[0].s); _free(EPSIO_yyvsp[-2].s); _free(EPSIO_yyvsp[0].s); }
break;
case 18:
#line 127 "./parser.y"
{EPSIO_yyval.s = iomake_exp2(EPSIO_yyvsp[-4].s, EPSIO_yyvsp[-2].s, EPSIO_yyvsp[0].s); _free(EPSIO_yyvsp[-4].s); _free(EPSIO_yyvsp[-2].s);
                                                                  _free(EPSIO_yyvsp[0].s); }
break;
case 19:
#line 131 "./parser.y"
{EPSIO_yyval.s = EPSIO_yyvsp[0].s;}
break;
#line 580 "y.tab.c"
    }
    EPSIO_yyssp -= EPSIO_yym;
    EPSIO_yystate = *EPSIO_yyssp;
    EPSIO_yyvsp -= EPSIO_yym;
    EPSIO_yym = EPSIO_yylhs[EPSIO_yyn];
    if (EPSIO_yystate == 0 && EPSIO_yym == 0)
    {
#if YYDEBUG
        if (EPSIO_yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        EPSIO_yystate = YYFINAL;
        *++EPSIO_yyssp = YYFINAL;
        *++EPSIO_yyvsp = EPSIO_yyval;
        if (EPSIO_yychar < 0)
        {
            if ((EPSIO_yychar = EPSIO_yylex()) < 0) EPSIO_yychar = 0;
#if YYDEBUG
            if (EPSIO_yydebug)
            {
                EPSIO_yys = 0;
                if (EPSIO_yychar <= YYMAXTOKEN) EPSIO_yys = EPSIO_yyname[EPSIO_yychar];
                if (!EPSIO_yys) EPSIO_yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, EPSIO_yychar, EPSIO_yys);
            }
#endif
        }
        if (EPSIO_yychar == 0) goto EPSIO_yyaccept;
        goto EPSIO_yyloop;
    }
    if ((EPSIO_yyn = EPSIO_yygindex[EPSIO_yym]) && (EPSIO_yyn += EPSIO_yystate) >= 0 &&
            EPSIO_yyn <= YYTABLESIZE && EPSIO_yycheck[EPSIO_yyn] == EPSIO_yystate)
        EPSIO_yystate = EPSIO_yytable[EPSIO_yyn];
    else
        EPSIO_yystate = EPSIO_yydgoto[EPSIO_yym];
#if YYDEBUG
    if (EPSIO_yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *EPSIO_yyssp, EPSIO_yystate);
#endif
    if (EPSIO_yyssp >= EPSIO_yysslim && EPSIO_yygrowstack())
    {
        goto EPSIO_yyoverflow;
    }
    *++EPSIO_yyssp = EPSIO_yystate;
    *++EPSIO_yyvsp = EPSIO_yyval;
    goto EPSIO_yyloop;
EPSIO_yyoverflow:
    EPSIO_yyerror("yacc stack overflow");
EPSIO_yyabort:
    if (EPSIO_yyss)
            free(EPSIO_yyss);
    if (EPSIO_yyvs)
            free(EPSIO_yyvs);
    EPSIO_yyss = EPSIO_yyssp = NULL;
    EPSIO_yyvs = EPSIO_yyvsp = NULL;
    EPSIO_yystacksize = 0;
    return (1);
EPSIO_yyaccept:
    if (EPSIO_yyss)
            free(EPSIO_yyss);
    if (EPSIO_yyvs)
            free(EPSIO_yyvs);
    EPSIO_yyss = EPSIO_yyssp = NULL;
    EPSIO_yyvs = EPSIO_yyvsp = NULL;
    EPSIO_yystacksize = 0;
    return (0);
}
