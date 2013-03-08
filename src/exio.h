/* //////////////////////////////////////////////////////////////////////////
// Name:        exio.h
// Purpose:     C helper defines and functions for wxExio class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: exio.h,v 1.10 2001/02/16 18:36:17 vadz Exp $
// Copyright:   (c)
// Licence:   	wxWindows licence
////////////////////////////////////////////////////////////////////////// */

#ifndef _WX_EXPRH__
#define _WX_EXPRH__

#include <math.h>
#if defined(__VISAGECPP__) && __IBMCPP__ >= 400
#  undef __BSEXCPT__
#endif
#include <stdlib.h>

#ifdef ____HPUX__
#define alloca malloc
#endif


#ifdef __cplusplus
extern "C" {
char *epsio_cons(char *, char *);
char * iomake_integer(char *);
char * iomake_word(char *);
char * iomake_string(char *);
char * iomake_real(char *, char *);
char * iomake_exp(char *, char *);
char * iomake_exp2(char *, char *, char*);
void add_expr(char *);
void process_eps_cmd(char *);
void error_syntax(char *);
}
#else
#if defined(__BORLANDC__) || defined(__VISAGECPP__)
char *epsio_cons(char *, char *);
char * iomake_integer(char *);
char * iomake_word(char *);
char * iomake_string(char *);
char * iomake_real(char *, char *);
char * iomake_exp(char *, char *);
char * iomake_exp2(char *, char *, char*);
void add_expr(char *);
void process_eps_cmd(char *);
void error_syntax(char *);
int  lex_input(void);
#else
char *epsio_cons();
char * iomake_integer();
char * iomake_word();
char * iomake_string();
char * iomake_real();
char * iomake_exp();
char * iomake_exp2();

void add_expr();
void process_eps_cmd();
void error_syntax();
#endif
#endif

#endif
	/* _WX_EXPRH__ */
