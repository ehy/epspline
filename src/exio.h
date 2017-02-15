/*
 * *MODIFICATIONS ONLY* Copyright (c) 2013 Ed Hynan
 *
 * This file is part of Epspline
 *
 * This file has been copied from a distibution of the wxWidgets
 * library, formerly called wxWindows, because it was no longer
 * supported by wxWidgets.
 *
 * This file is covered by the original wxWindows license, or any
 * that the original authors or copyright holders would apply. The
 * original comment appears below.
 *
 * For the "wxWindows Licence" see:
 *	http://www.wxwidgets.org/about/newlicen.htm
 */

////////////////////////////////////////////////////////////////////////// */
// Name:        expr.h
// Purpose:     C helper defines and functions for wxExpr class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: expr.h,v 1.10 2001/02/16 18:36:17 vadz Exp $
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
const char *epsio_cons(const char *, const char *);
const char * iomake_integer(const char *);
const char * iomake_word(const char *);
const char * iomake_string(const char *);
const char * iomake_real(const char *, const char *);
const char * iomake_exp(const char *, const char *);
const char * iomake_exp2(const char *, const char *, const char*);
void add_expr(const char *);
void process_eps_cmd(const char *);
void error_syntax(const char *);
}
#else
#if defined(__BORLANDC__) || defined(__VISAGECPP__)
const char *epsio_cons(const char *, const char *);
const char * iomake_integer(const char *);
const char * iomake_word(const char *);
const char * iomake_string(const char *);
const char * iomake_real(const char *, const char *);
const char * iomake_exp(const char *, const char *);
const char * iomake_exp2(const char *, const char *, const char*);
void add_expr(const char *);
void process_eps_cmd(const char *);
void error_syntax(const char *);
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
