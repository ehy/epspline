/* Version: $Id: parser.y,v 1.26 2002/04/19 22:12:37 RL Exp $ */
 %{
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
int yylex(void);
int yylook(void);
int yywrap(void);
int yyback(int *, int);
void yyerror(const char *);

/* You may need to put /DLEX_SCANNER in your makefile
 * if you're using LEX!
 */
void yyoutput(int);

%}

%union {
    const char *s;
/*    struct pexpr *expr; */
}


%start commands

%token <s> INTEGER 1
%token <s> WORD 2
%token <s> STRING 3
%token <s> PERIOD 13
%token OPEN 4
%token CLOSE 5
%token COMMA 6
%token NEWLINE 7
%token ERROR 8
%token OPEN_SQUARE 9
%token CLOSE_SQUARE 10
%token EQUALS 11
%token EXP 14

/* %type <expr> command expr arglist arg arg1 */
%type <s> command expr arglist arg arg1

%%

commands :	/* empty */
	|	commands command
	;

command	:       WORD PERIOD
			{process_eps_cmd(epsio_cons(iomake_word($1), NULL)); free($1);}
        |       expr PERIOD
			{process_eps_cmd($1);}
	|	error PERIOD
			{error_syntax("Unrecognized command.");}
	;

expr	:	WORD OPEN arglist CLOSE 
			{$$ = epsio_cons(iomake_word($1), $3); free($1);}
	|	OPEN_SQUARE arglist CLOSE_SQUARE
			{$$ = $2; }
	;

arglist	:
			{$$ = epsio_cons(NULL, NULL);}
	|	arg
			{$$ = epsio_cons($1, NULL);}
	|
		arg COMMA arglist
			{$$ = epsio_cons($1, $3);}
	;

arg	:	WORD EQUALS arg1
			{$$ = epsio_cons(iomake_word("="), epsio_cons(iomake_word($1), epsio_cons($3, NULL)));
                         free($1); }
	|	arg1
			{$$ = $1; }
	;

arg1	:	WORD
			{$$ = iomake_word($1); free($1);}
	|	STRING
			{$$ = iomake_string($1); free($1);}
	|	INTEGER
			{$$ = iomake_integer($1); free($1);}
	|	INTEGER PERIOD INTEGER
			{$$ = iomake_real($1, $3); free($1); free($3); }
        |       INTEGER EXP INTEGER
                         {$$ = iomake_exp($1, $3); free($1); free($3); }
        |
              INTEGER PERIOD INTEGER EXP INTEGER
                         {$$ = iomake_exp2($1, $3, $5); free($1); free($3);
                                                                  free($5); }

	|	expr
			{$$ = $1;}
	;

%%

/* We include lexer.c if we are building for gtk, wine or motif
 * and also whenever we are using configure (marked by __WX_SETUP_H__) for,
 * for example, cross compilation. */
#if (defined(__WXGTK__) || defined(__WXWINE__) || defined(__WXMOTIF__)) || defined(__WX_SETUP_H__) && !defined(NO_CONFIGURE)
#include "lexer.c"
#elif defined(__WXMAC__) && defined(__APPLE__)
#include "lexer.c"
#elif defined(__MWERKS__)
#include "../common/cwlex_yy.c"
#elif defined(__SC__)
#include "lexer.c"
#else
#include "../common/lex_yy.c"
#endif

/*
void yyerror(s)
char *s;
{
  error_syntax(s);
}
*/

/* Ansi prototype. If this doesn't work for you... uncomment
   the above instead.
 */

void yyerror(const char *s)
{
  error_syntax(s);
}

/*
 * Unfortunately, my DOS version of FLEX
 * requires yywrap to be #def'ed, whereas
 * the UNIX flex expects a proper function.
 */

/* At least on alphaev6-dec-osf4.0e yywrap() must be #define'd.
 * RL: ... but on Debian/Alpha(linux) it must not, so hopefully
 *     testing for __OSF__ here is what we really want.
 */
#ifdef __OSF__
#ifndef yywrap
#define yywrap() 1
#endif
#else
/* HH: Added test for __WX_SETUP_H__ for gnuwin builds
 * using configure */ 
#if !defined(__SC__) && !defined(__GNUWIN32__) 
#ifdef USE_DEFINE
#ifndef yywrap
#define yywrap() 1
#endif
#elif !defined(__ultrix)
int yywrap() { return 1; }
#elif defined(__VMS__)
int yywrap() { return 1; }
#endif
#elif defined(__WX_SETUP_H__) || defined(__SC__)
int yywrap() { return 1; }
#endif
#endif

#if defined(__cplusplus) || defined(__STDC__)
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif
#endif
