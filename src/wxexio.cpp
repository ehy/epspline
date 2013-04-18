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
 *      http://www.wxwidgets.org/about/newlicen.htm
 */
 
/**
***  This is a slightly modified copy of the wxExpr class from the
***  wx(Windows|Widgets) library.  It exists because compilation of
***  wxExpr is optional, and too often it is not included in a
***  OS/distribution package system's build of the library.  So it is
***  to make this program portable without requiring reconfiguration
***  and recompilation of the target system's wx library that this copy
***  is here.  (Too bad the systems don't provide a full featured build;
***  but then there remains the question of the wx project's continued
***  maintenance of the class.)
***
***  The original wxExpr comment appears below:
**/

/////////////////////////////////////////////////////////////////////////////
// Name:        wxexpr.cpp
// Purpose:     wxExpr
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: wxexpr.cpp,v 1.25.2.1 2002/11/04 23:11:23 VZ Exp $
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "wxexio.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
// (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//#if wxUSE_PROLOGIO
#if 1

#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include <wx/utils.h>

#include "wxutil.h"
#include "wxexio.h"

// wxString has been mutilated in wx 2.9 and greater -- breaking
// C++ casting operators, for example. Grrrr.
#if wxCHECK_VERSION(2, 9, 0)
#if		defined(_UNICODE) || defined(UNICODE)
#			define C_STR wc_str()
#		else
#			define C_STR c_str()
#		endif
#else
#	define C_STR c_str()
#endif

#if !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__FreeBSD__)
extern "C" {
#endif
void add_expr(char *);
void LIOFromFile(FILE *fd);
void LIOFromString(char *buf);
#if !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__FreeBSD__)
}
#endif

// for 2.6 deprecations:
#if wxCHECK_VERSION(2, 6, 0)
#define Next GetNext
#define Previous GetPrevious
#define Data GetData
#define First GetFirst
#endif

#include "util.h"
#define copystring newstrdup

/* Rename all YACC/LEX stuff or we'll conflict with other
 * applications
 */

#define yyback EPSIO_yyback
#define yylook EPSIO_yylook
#define yywrap EPSIO_yywrap
#define yyoutput EPSIO_yyoutput
#define yylex EPSIO_yylex
#define yyerror EPSIO_yyerror
#define yyleng EPSIO_yyleng
#define yytext EPSIO_yytext
#define yymorfg EPSIO_yymorfg
#define yylineno EPSIO_yylineno
#define yytchar EPSIO_yytchar
#define yyin EPSIO_yyin
#define yyout EPSIO_yyout
#define yysvf EPSIO_yysvf
#define yyestate EPSIO_yyestate
#define yysvec EPSIO_yysvec
#define yybgin EPSIO_yybgin
#define yyprevious EPSIO_yyprevious
#define yylhs EPSIO_yylhs
#define yylen EPSIO_yylen
#define yydefred EPSIO_yydefred
#define yydgoto EPSIO_yydgoto
#define yysindex EPSIO_yysindex
#define yyrindex EPSIO_yyrindex
#define yygindex EPSIO_yygindex
#define yytable EPSIO_yytable
#define yycheck EPSIO_yycheck
#define yyname EPSIO_yyname
#define yyrule EPSIO_yyrule
#define yydebug EPSIO_yydebug
#define yynerrs EPSIO_yynerrs
#define yyerrflag EPSIO_yyerrflag
#define yychar EPSIO_yychar
#define yyvsp EPSIO_yyvsp
#define yyssp EPSIO_yyssp
#define yyval EPSIO_yyval
#define yylval EPSIO_yylval
#define yyss EPSIO_yyss
#define yyvs EPSIO_yyvs
#define yyparse EPSIO_yyparse

/* +++steve162e: more defines necessary */
#define yy_init_buffer EPSIO_yy_init_buffer
#define yy_create_buffer EPSIO_yy_create_buffer
#define yy_load_buffer_state EPSIO_yy_load_buffer_state
#define yyrestart EPSIO_yyrestart
#define yy_switch_to_buffer EPSIO_yy_switch_to_buffer
#define yy_delete_buffer EPSIO_yy_delete_buffer
/* ---steve162e */

/* WG 1/96: still more for flex 2.5 */
#define yy_scan_buffer EPSIO_scan_buffer
#define yy_scan_string EPSIO_scan_string
#define yy_scan_bytes EPSIO_scan_bytes
#define yy_flex_debug EPSIO_flex_debug
#define yy_flush_buffer EPSIO_flush_buffer
#if !defined(__VISAGECPP__)
/* multiply defined??? */
#define yyleng EPSIO_yyleng
#define yytext EPSIO_yytext
#endif

#if !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__FreeBSD__)
extern "C" {
#endif
extern FILE* yyin;
int yyparse(void);
#if !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__FreeBSD__)
}
#endif
inline int hack_yyparse(void*) { return yyparse(); }


wxExioDatabase *thewxExioDatabase = NULL;
wxExioErrorHandler currentwxExioErrorHandler;

wxExio::wxExio(const wxString& functor)
{
  type = wxExioList;
  next = NULL;
  last = NULL;
  value.first = NULL;

  wxExio *pfunctor = new wxExio(wxExioWord, functor);
  Append(pfunctor);
  client_data = NULL;
}

wxExio::wxExio(wxExioType the_type, const wxString& word_or_string)
{
  type = the_type;

  switch (the_type)
  {
   case wxExioWord:
    value.word = copystring((const wxChar *)word_or_string.C_STR);
    break;
   case wxExioString:
    value.string = copystring((const wxChar *)word_or_string.C_STR);
    break;
   case wxExioList:
    last = NULL;
    value.first = NULL;
    break;
   case wxExioReal:
   case wxExioInteger:
   case wxExioNull: 
    break;
  }
  client_data = NULL;
  next = NULL;
}

wxExio::wxExio(wxExioType the_type, wxChar *word_or_string, bool allocate)
{
  type = the_type;

  switch (the_type)
  {
   case wxExioWord:
    value.word = allocate ? copystring(word_or_string) : word_or_string;
    break;
   case wxExioString:
    value.string = allocate ? copystring(word_or_string) : word_or_string;
    break;
   case wxExioList:
    last = NULL;
    value.first = NULL;
    break;
   case wxExioReal:
   case wxExioInteger:
   case wxExioNull: 
    break;
  }
  client_data = NULL;
  next = NULL;
}

wxExio::wxExio(long the_integer)
{
  type = wxExioInteger;
  value.integer = the_integer;
  client_data = NULL;
  next = NULL;
}

wxExio::wxExio(double the_real)
{
  type = wxExioReal;
  value.real = the_real;
  client_data = NULL;
  next = NULL;
}

wxExio::wxExio(wxList *the_list)
{
  type = wxExioList;
  client_data = NULL;
  last = NULL;
  value.first = NULL;

  wxExio *listExpr = new wxExio(wxExioList);

#if wxUSE_STL
  wxList::compatibility_iterator node = the_list->First();
#else
  wxNode *node = the_list->First();
#endif
  while (node)
  {
    wxExio *expr = (wxExio *)node->Data();
    listExpr->Append(expr);
    node = node->Next();
  }
  Append(listExpr);

  delete the_list;
}

wxExio::~wxExio(void)
{
  switch (type)
  {
    case wxExioInteger:
    case wxExioReal:
    {
     break;
    }
   case wxExioString:
   {
     delete[] value.string;
     break;
   }
   case wxExioWord:
   {
     delete[] value.word;
     break;
   }
   case wxExioList:
   {
     wxExio *expr = value.first;
     while (expr)
     {
       wxExio *expr1 = expr->next;

       delete expr;
       expr = expr1;
     }
     break;
   }
   case wxExioNull: break;
  }
}

void wxExio::Append(wxExio *expr)
{
  if (!value.first)
    value.first = expr;

  if (last)
    last->next = expr;
  last = expr;
}

void wxExio::Insert(wxExio *expr)
{
  expr->next = value.first;
  value.first = expr;

  if (!last)
    last = expr;
}

wxExio *wxExio::Copy(void) const
{
  // This seems to get round an optimizer bug when
  // using Watcom C++ 10a in WIN32 compilation mode.
  // If these lines not present, the type seems to be
  // interpreted wrongly as an integer.
  // I don't want to turn optimization off since it's needed
  // for reading in files quickly.
#if defined(__WATCOMC__)
  char buf[2];
  sprintf(buf, "");
#endif

  switch (type)
  {
    case wxExioInteger:
      return new wxExio(value.integer);
    case wxExioReal:
      return new wxExio(value.real);
    case wxExioString:
      return new wxExio(wxExioString, wxString(value.string));
    case wxExioWord:
      return new wxExio(wxExioWord, wxString(value.word));
    case wxExioList:
    {
      wxExio *expr = value.first;
      wxExio *new_list = new wxExio(wxExioList);
      while (expr)
      {
        wxExio *expr2 = expr->Copy();
        new_list->Append(expr2);
        expr = expr->next;
      }
      return new_list;
    }
   case wxExioNull: 
    break;
  }
  return NULL;
}


// Get the wxExio (containing (= wxExio Value) form) for the given word
//  or string, assuming that we have Attribute=Value, ...
wxExio *wxExio::GetAttributeValueNode(const wxString& word) const // Use only for a clause or list
{
  if (type != wxExioList)
    return NULL;

  wxExio *expr = value.first;
  while (expr)
  {
    if (expr->type == wxExioList)
    {
      wxExio *firstNode = expr->value.first;
      if ((firstNode->type == wxExioWord) && (firstNode->value.word[0] == '='))
      {
        wxExio *secondNode = firstNode->next;
        if ((secondNode->type == wxExioWord) && 
            (wxStrcmp((const wxChar *)word.C_STR, secondNode->value.word) == 0))
        {
          return expr;
        }
      }
    }
    expr = expr->next;
  }
  return NULL;
}

// Get the value (in wxExio form) for the given word or string, assuming
// that we have Attribute=Value, ...
wxExio *wxExio::AttributeValue(const wxString& word) const // Use only for a clause or list
{
  if (type != wxExioList)
    return NULL;

  wxExio *attExpr = GetAttributeValueNode(word);
  if (attExpr && attExpr->value.first && attExpr->value.first->next)
    return attExpr->value.first->next->next;
  else return NULL;
}

wxString wxExio::Functor(void) const // Use only for a clause
{
  if ((type != wxExioList) || !value.first)
    return wxString(wxT(""));

  if (value.first->type == wxExioWord)
    return wxString(value.first->value.word);
  else
    return wxString(wxT(""));
}

bool wxExio::IsFunctor(const wxString& f) const  // Use only for a clause
{
  if ((type != wxExioList) || !value.first)
    return FALSE;

  return (value.first->type == wxExioWord && 
          (wxStrcmp((const wxChar *)f.C_STR, value.first->value.word) == 0));
}

// Return nth argument of a clause (starting from 1)
wxExio *wxExio::Arg(wxExioType theType, int arg) const
{
  wxExio *expr = value.first;
  int i;
  for (i = 1; i < arg; i++)
    if (expr)
      expr = expr->next;

  if (expr && (expr->type == theType))
    return expr;
  else
    return NULL;
}

// Return nth argument of a list expression (starting from zero)
wxExio *wxExio::Nth(int arg) const
{
  if (type != wxExioList)
    return NULL;

  wxExio *expr = value.first;
  int i;
  for (i = 0; i < arg; i++)
    if (expr)
      expr = expr->next;
    else return NULL;

  if (expr)
    return expr;
  else
    return NULL;
}

  // Returns the number of elements in a list expression
int wxExio::Number(void) const
{
  if (type != wxExioList)
    return 0;

  int i = 0;
  wxExio *expr = value.first;
  while (expr)
  {
    expr = expr->next;
    i ++;
  }
  return i;
}

void wxExio::DeleteAttributeValue(const wxString& attribute)
{
  if (type != wxExioList)
    return;

  wxExio *expr = value.first;
  wxExio *lastExpr = this;
  while (expr)
  {
    if (expr->type == wxExioList)
    {
      wxExio *firstNode = expr->value.first;
      if ((firstNode->type == wxExioWord) && (firstNode->value.word[0] == '='))
      {
        wxExio *secondNode = firstNode->next;
        if ((secondNode->type == wxExioWord) && 
            (wxStrcmp((const wxChar *)attribute.C_STR, secondNode->value.word) == 0))
        {
          wxExio *nextExpr = expr->next;
          delete expr;

          lastExpr->next = nextExpr;

          if (last == expr)
            last = lastExpr;

          return;
        }
      }
    }
    lastExpr = expr;
    expr = expr->next;
  }
  return;
}

void wxExio::AddAttributeValue(const wxString& attribute, wxExio *val)
{
  if (type != wxExioList)
  {
//    cout << "Error! tried to add an attribute-value pair to a nonlist wxExio expression\n";
    return;
  }
  // Warning - existing code may assume that any existing value
  // is deleted first. For efficiency, we leave this to the application.
//  DeleteAttributeValue(attribute);

  wxExio *patt = new wxExio(wxExioWord, attribute);
  wxExio *pequals = new wxExio(wxExioWord, wxT("="));

  wxExio *listExpr = new wxExio(wxExioList);

  listExpr->Append(pequals);
  listExpr->Append(patt);
  listExpr->Append(val);

  Append(listExpr);
}

void wxExio::AddAttributeValue(const wxString& attribute, long val)
{
  if (type != wxExioList)
  {
//    cout << "Error! tried to add an attribute-value pair to a nonlist wxExio expression\n";
    return;
  }
  // Warning - existing code may assume that any existing value
  // is deleted first. For efficiency, we leave this to the application.
//  DeleteAttributeValue(attribute);

  wxExio *patt = new wxExio(wxExioWord, attribute);
  wxExio *pval = new wxExio(val);
  wxExio *pequals = new wxExio(wxExioWord, wxT("="));

  wxExio *listExpr = new wxExio(wxExioList);

  listExpr->Append(pequals);
  listExpr->Append(patt);
  listExpr->Append(pval);

  Append(listExpr);
}

void wxExio::AddAttributeValue(const wxString& attribute, double val)
{
  if (type != wxExioList)
  {
//    cout << "Error! tried to add an attribute-value pair to a nonlist wxExio expression\n";
    return;
  }

//  DeleteAttributeValue(attribute);
  wxExio *patt = new wxExio(wxExioWord, attribute);
  wxExio *pval = new wxExio(val);
  wxExio *pequals = new wxExio(wxExioWord, wxT("="));

  wxExio *listExpr = new wxExio(wxExioList);

  listExpr->Append(pequals);
  listExpr->Append(patt);
  listExpr->Append(pval);

  Append(listExpr);
}

void wxExio::AddAttributeValueString(const wxString& attribute, const wxString& val)
{
  if (type != wxExioList)
  {
//    cout << "Error! tried to add an attribute-value pair to a nonlist wxExio expression\n";
    return;
  }

//  DeleteAttributeValue(attribute);

  wxExio *patt = new wxExio(wxExioWord, attribute);
  wxExio *pval = new wxExio(wxExioString, val);
  wxExio *pequals = new wxExio(wxExioWord, wxT("="));

  wxExio *listExpr = new wxExio(wxExioList);

  listExpr->Append(pequals);
  listExpr->Append(patt);
  listExpr->Append(pval);

  Append(listExpr);
}

void wxExio::AddAttributeValueWord(const wxString& attribute, const wxString& val)
{
  if (type != wxExioList)
  {
//    cout << "Error! tried to add an attribute-value pair to a nonlist wxExio expression\n";
    return;
  }

//  DeleteAttributeValue(attribute);

  wxExio *patt = new wxExio(wxExioWord, attribute);
  wxExio *pval = new wxExio(wxExioWord, val);
  wxExio *pequals = new wxExio(wxExioWord, wxT("="));

  wxExio *listExpr = new wxExio(wxExioList);

  listExpr->Append(pequals);
  listExpr->Append(patt);
  listExpr->Append(pval);

  Append(listExpr);
}

void wxExio::AddAttributeValue(const wxString& attribute, wxList *val)
{
  if (type != wxExioList)
  {
//    cout << "Error! tried to add an attribute-value pair to a nonlist wxExio expression\n";
    return;
  }
  if (!val)
    return;

//  DeleteAttributeValue(attribute);

  wxExio *patt = new wxExio(wxExioWord, attribute);
  wxExio *pval = new wxExio(val);
  wxExio *pequals = new wxExio(wxExioWord, wxT("="));

  wxExio *listExpr = new wxExio(wxExioList);

  listExpr->Append(pequals);
  listExpr->Append(patt);
  listExpr->Append(pval);

  Append(listExpr);
}

void wxExio::AddAttributeValueStringList(const wxString& attribute, wxList *string_list)
{
  if (type != wxExioList)
  {
//    cout << "Error! tried to add an attribute-value pair to a nonlist wxExio expression\n";
    return;
  }
  if (!string_list)
    return;

//  DeleteAttributeValue(attribute);

  // First make a list of wxExio strings
  wxExio *listExpr = new wxExio(wxExioList);
#if wxUSE_STL
  wxList::compatibility_iterator node = string_list->First();
#else
  wxNode *node = string_list->First();
#endif
  while (node)
  {
    wxChar *string = (wxChar *)node->Data();
    wxExio *expr = new wxExio(wxExioString, wxString(string));
    listExpr->Append(expr);
    node = node->Next();
  }

  // Now make an (=, Att, Value) triple
  wxExio *patt = new wxExio(wxExioWord, attribute);
  wxExio *pequals = new wxExio(wxExioWord, wxT("="));

  wxExio *listExpr2 = new wxExio(wxExioList);

  listExpr2->Append(pequals);
  listExpr2->Append(patt);
  listExpr2->Append(listExpr);

  Append(listExpr2);
}

bool wxExio::GetAttributeValue(const wxString& att, int& var) const
{
  wxExio *expr = AttributeValue(att);

  if (expr && (expr->Type() == wxExioInteger || expr->Type() == wxExioReal))
  {
    var = (int)(expr->IntegerValue());
    return TRUE;
  }
  else
    return FALSE;
}

bool wxExio::GetAttributeValue(const wxString& att, long& var) const
{
  wxExio *expr = AttributeValue(att);

  if (expr && (expr->Type() == wxExioInteger || expr->Type() == wxExioReal))
  {
    var = expr->IntegerValue();
    return TRUE;
  }
  else
    return FALSE;
}

bool wxExio::GetAttributeValue(const wxString& att, float& var) const
{
  wxExio *expr = AttributeValue(att);
  if (expr && (expr->Type() == wxExioInteger || expr->Type() == wxExioReal))
  {
    var = (float) expr->RealValue();
    return TRUE;
  }
  else
    return FALSE;
}

bool wxExio::GetAttributeValue(const wxString& att, double& var) const
{
  wxExio *expr = AttributeValue(att);
  if (expr && (expr->Type() == wxExioInteger || expr->Type() == wxExioReal))
  {
    var = expr->RealValue();
    return TRUE;
  }
  else
    return FALSE;
}

bool wxExio::GetAttributeValue(const wxString& att, wxString& var)  const // Word OR string -> string
{
  wxExio *expr = AttributeValue(att);
  if (expr && expr->Type() == wxExioWord)
  {
    var = expr->WordValue();
    return TRUE;
  }
  else if (expr && expr->Type() == wxExioString)
  {
    var = expr->StringValue();
    return TRUE;
  }
  else
    return FALSE;
}

bool wxExio::GetAttributeValue(const wxString& att, wxExio **var) const
{
  wxExio *expr = AttributeValue(att);
  if (expr)
  {
    *var = expr;
    return TRUE;
  }
  else
    return FALSE;
}

bool wxExio::GetAttributeValueStringList(const wxString& att, wxList *var) const
{
  wxExio *expr = AttributeValue(att);
  if (expr && expr->Type() == wxExioList)
  {
    wxExio *string_expr = expr->value.first;
    while (string_expr)
    {
      if (string_expr->Type() == wxExioString)
        var->Append((wxObject *)copystring(string_expr->StringValue().C_STR));

      string_expr = string_expr->next;
    }
	return TRUE;
  }
  else
    return FALSE;
}

// Compatibility
void wxExio::AssignAttributeValue(wxChar *att, wxChar **var) const
{
  wxString str;
  if (GetAttributeValue(att, str))
  {
    if (*var)
      delete[] *var;
    *var = copystring((const wxChar *) str.C_STR);
  }
}

void wxExio::WriteClause(FILE* stream)  // Write this expression as a top-level clause
{
  if (type != wxExioList)
    return;

  wxExio *node = value.first;
  if (node)
  {
    node->WriteExpr(stream);
    fprintf( stream, "(" );
    node = node->next;
    bool first = TRUE;
    while (node)
    {
      if (!first)
        fprintf( stream, "  " );
      node->WriteExpr(stream);
      node = node->next;
      if (node) 
        fprintf( stream, ",\n" );
      first = FALSE;
    }
    fprintf( stream, ").\n\n" );
  }
}

void wxExio::WriteExpr(FILE* stream)    // Write as any other subexpression
{
  // This seems to get round an optimizer bug when
  // using Watcom C++ 10a in WIN32 compilation mode.
  // If these lines not present, the type seems to be
  // interpreted wrongly as an integer.
  // I don't want to turn optimization off since it's needed
  // for reading in files quickly.
#if defined(__WATCOMC__)
  char buf[2];
  sprintf(buf, "");
#endif

  switch (type)
  {
    case wxExioInteger:
    {
      fprintf( stream, "%ld", value.integer );
      break;
    }
    case wxExioReal:
    {
      double f = value.real;
      fprintf( stream, "%.6g", f);
      break;
    }
    case wxExioString:
    {
      fprintf( stream, "\"" );
      size_t i;
      // EH: 18.10.2012 -- the commented line below, with wx 2.9.4,
      // would segfault (GNU/Linux) when source text was utf-8 *with*
      // non-ascii content (locale set correctly); the replacement
      // with macro wxs2ch (wxutil.h) uses *wxConvCurrent and works
      // (not that it matters if mb strings can be written: the parser
      // code segfaults reading non-ascii, plus this code pertains to
      // POVRay SDL, which will not accept such symbols as of v. 3.6)
      //const wxWX2MBbuf val = wxConvLibc.cWX2MB(value.string);
      wxString tstr(value.string);
      const tmp_cstr val(wxs2ch(tstr)); // in util.h
      size_t len = val.cnt - 1; // object has count including null
      for (i = 0; i < len; i++)
      {
        char ch = val[i];
        if (ch == '"' || ch == '\\')
        fprintf( stream, "\\" );
        char tmp[2];
        tmp[0] = ch;
        tmp[1] = 0;
        fprintf( stream, "%s", tmp );
      }
      fprintf( stream, "\"" );
      break;
    }
    case wxExioWord:
    {
      bool quote_it = FALSE;
      const wxWX2MBbuf val = wxConvLibc.cWX2MB(value.word);
      size_t len = strlen(val);
      if ((len == 0) || (len > 0 && (val[(size_t) 0] > 64 && val[(size_t) 0] < 91)))
        quote_it = TRUE;
      else
      {
        size_t i;
        for (i = 0; i < len; i++)
          if ((!isalpha(val[i])) && (!isdigit(val[i])) &&
              (val[i] != '_'))
            { quote_it = TRUE; i = len; }
      }

      if (quote_it)
        fprintf( stream ,"'" );

      fprintf( stream, "%s", (const char*)val );

      if (quote_it)
        fprintf( stream, "'" );

      break;
    }
    case wxExioList:
    {
      if (!value.first)
        fprintf( stream, "[]" );
      else
      {
        wxExio *expr = value.first;

        if ((expr->Type() == wxExioWord) && (wxStrcmp(expr->WordValue(), wxT("=")) == 0))
        {
          wxExio *arg1 = expr->next;
          wxExio *arg2 = arg1->next;
          arg1->WriteExpr(stream);
          fprintf( stream, " = " );
          arg2->WriteExpr(stream);
        }
        else
        {
          fprintf( stream, "[" );
          while (expr)
          {
            expr->WriteExpr(stream);
            expr = expr->next;
            if (expr) 
	        fprintf( stream, ", " );
          }
          fprintf( stream, "]" );
        }
      }
      break;
    }
   case wxExioNull: break;
  }
}

/*
 * wxExio 'database' (list of expressions)
 */
 
IMPLEMENT_DYNAMIC_CLASS(wxExioDatabase, WXExioList)

wxExioDatabase::wxExioDatabase(wxExioErrorHandler handler)
{
#if !wxUSE_STL && !EXIO_USE_STD_CONTAINERS
  position = NULL;
#endif
  hash_table = NULL;
  currentwxExioErrorHandler = handler;
  noErrors = 0;
}

wxExioDatabase::wxExioDatabase(wxExioType type, const wxString& attribute, int size,
                               wxExioErrorHandler handler)
{
#	if !wxUSE_STL && !EXIO_USE_STD_CONTAINERS
  position = NULL;
#	endif
  attribute_to_hash = attribute;
  if (type == wxExioString)
    hash_table = new wxHashTable(wxKEY_STRING, size);
  else if (type == wxExioInteger)
    hash_table = new wxHashTable(wxKEY_INTEGER, size);
  else hash_table = NULL;

  currentwxExioErrorHandler = handler;
  noErrors = 0;
}

wxExioDatabase::~wxExioDatabase(void)
{
  ClearDatabase();
  if (hash_table)
    delete hash_table;
}

void wxExioDatabase::BeginFind(void)          // Initialise a search
{
#	if EXIO_USE_STD_CONTAINERS
  position = begin();
#	else
  position = First();
#	endif
}

wxExio *wxExioDatabase::FindClause(long id)  // Find a term based on an integer id attribute
                                 // e.g. node(id=23, type=rectangle, ....).
{
  wxExio *found = NULL;
#	if EXIO_USE_STD_CONTAINERS
  while (position != end() && !found)
  {
    wxExio *term = *position++;
    
    if (term->Type() == wxExioList)
    {
      wxExio *value = term->AttributeValue(wxT("id"));
      if (value->Type() == wxExioInteger && value->IntegerValue() == id)
        found = term;
    }
  }
#	else
  while (position && !found)
  {
    wxExio *term = (wxExio *)position->Data();
    
    if (term->Type() == wxExioList)
    {
      wxExio *value = term->AttributeValue(wxT("id"));
      if (value->Type() == wxExioInteger && value->IntegerValue() == id)
        found = term;
    }
    position = position->Next();
  }
#	endif
  return found;
}

// Find on basis of attribute/value pairs, e.g. type=rectangle
wxExio *wxExioDatabase::FindClause(const wxString& word, const wxString& val)
{
  wxExio *found = NULL;
#	if EXIO_USE_STD_CONTAINERS
  while (position != end() && !found)
  {
    wxExio *term = *position++;
    
    if (term->Type() == wxExioList)
    {
     wxExio *value = term->AttributeValue(word);
      if ((value->Type() == wxExioWord && value->WordValue() == val) ||
          (value->Type() == wxExioString && value->StringValue() == val))
        found = term;
     }
  }
#	else
  while (position && !found)
  {
    wxExio *term = (wxExio *)position->Data();
    
    if (term->Type() == wxExioList)
    {
      wxExio *value = term->AttributeValue(word);
      if ((value->Type() == wxExioWord && value->WordValue() == val) ||
          (value->Type() == wxExioString && value->StringValue() == val))
        found = term;
    }
    position = position->Next();
  }
#	endif
  return found;
}

wxExio *wxExioDatabase::FindClause(const wxString& word, long val)
{
  wxExio *found = NULL;
#	if EXIO_USE_STD_CONTAINERS
  while (position != end() && !found)
  {
    wxExio *term = *position++;
    
    if (term->Type() == wxExioList)
    {
      wxExio *value = term->AttributeValue(word);
      if ((value->Type() == wxExioInteger) && (value->IntegerValue() == val))
        found = term;
    }
  }
#	else
  while (position && !found)
  {
    wxExio *term = (wxExio *)position->Data();
    
    if (term->Type() == wxExioList)
    {
      wxExio *value = term->AttributeValue(word);
      if ((value->Type() == wxExioInteger) && (value->IntegerValue() == val))
        found = term;
    }
    position = position->Next();
  }
#	endif
  return found;
}

wxExio *wxExioDatabase::FindClause(const wxString& word, double val)
{
  wxExio *found = NULL;
#	if EXIO_USE_STD_CONTAINERS
  while (position != end() && !found)
  {
    wxExio *term = *position++;
    
    if (term->Type() == wxExioList)
    {
      wxExio *value = term->AttributeValue(word);
      if ((value->Type() == wxExioReal) && (value->RealValue() == val))
        found = term;
    }
  }
#	else
  while (position && !found)
  {
    wxExio *term = (wxExio *)position->Data();
    
    if (term->Type() == wxExioList)
    {
      wxExio *value = term->AttributeValue(word);
      if ((value->Type() == wxExioReal) && (value->RealValue() == val))
        found = term;
    }
    position = position->Next();
  }
#	endif
  return found;
}

wxExio *wxExioDatabase::FindClauseByFunctor(const wxString& functor)
{
  wxExio *found = NULL;
#	if EXIO_USE_STD_CONTAINERS
  while (position != end() && !found)
  {
    wxExio *term = *position++;
    
    if (term->Type() == wxExioList)
    {
      if (term->Functor() == functor)
        found = term;
    }
  }
#	else
  while (position && !found)
  {
    wxExio *term = (wxExio *)position->Data();
    
    if (term->Type() == wxExioList)
    {
      if (term->Functor() == functor)
        found = term;
    }
    position = position->Next();
  }
#	endif
  return found;
}

// If hashing is on, must store in hash table too
void wxExioDatabase::Append(wxExio *clause)
{
#	if EXIO_USE_STD_CONTAINERS
  push_back(clause);
#	else
  wxList::Append((wxObject *)clause);
#	endif
  if (hash_table)
  {
    wxString functor(clause->Functor());
    wxExio *expr = clause->AttributeValue(attribute_to_hash);
    if (expr)
    {
      long functor_key = hash_table->MakeKey(WXSTRINGCAST functor.C_STR);
      long value_key = 0;
      if (expr && expr->Type() == wxExioString)
      {
        value_key = hash_table->MakeKey(WXSTRINGCAST expr->StringValue().C_STR);
        hash_table->Put(functor_key + value_key, WXSTRINGCAST expr->StringValue().C_STR, (wxObject *)clause);
      }
      else if (expr && expr->Type() == wxExioInteger)
      {
        value_key = expr->IntegerValue();
        hash_table->Put(functor_key + value_key, expr->IntegerValue(), (wxObject *)clause);
      }

    }
  }
}

wxExio *wxExioDatabase::HashFind(const wxString& functor, long value) const
{
  long key = hash_table->MakeKey(WXSTRINGCAST functor.C_STR) + value;

  // The key alone isn't guaranteed to be unique:
  // must supply value too. Let's assume the value of the
  // id is going to be reasonably unique.
  return (wxExio *)hash_table->Get(key, value);
}

wxExio *wxExioDatabase::HashFind(const wxString& functor, const wxString& value) const
{
  long key = hash_table->MakeKey(WXSTRINGCAST functor.C_STR) + hash_table->MakeKey(WXSTRINGCAST value.C_STR);
  return (wxExio *)hash_table->Get(key, WXSTRINGCAST value.C_STR);
}

void wxExioDatabase::ClearDatabase(void)
{
  noErrors = 0;
#	if EXIO_USE_STD_CONTAINERS
  iterator node;
  for ( node = begin(); node != end(); node++ )
  {
    wxExio *expr = *node;
    delete expr;
  }
  clear();
#	else
#if wxUSE_STL
  wxList::compatibility_iterator node = First();
  while (node)
  {
    wxExio *expr = (wxExio *)node->Data();
    delete expr;
    node = node->Next();
  }
  clear();
#else
  wxNode *node = First();
  while (node)
  {
    wxExio *expr = (wxExio *)node->Data();
    delete expr;
    delete node;
    node = First();
  }
#endif
#	endif

  if (hash_table)
    hash_table->Clear();
}

bool wxExioDatabase::Read(const wxString& filename)
{
  noErrors = 0;

  //FILE *f = wxFopen(filename, wxT("r"));
  auto_std_FILE f(wxs2ch(filename), "r");
  if (f)
  {
    thewxExioDatabase = this;

    LIOFromFile(f);
    hack_yyparse(0);

    //fclose(f);

    wxExioCleanUp();
    return (noErrors == 0);
  }
  else
  {
    return FALSE;
  }
}

bool wxExioDatabase::ReadFromString(const wxString& buffer)
{
  noErrors = 0;
  thewxExioDatabase = this;

  const wxWX2MBbuf buf = buffer.mb_str();
  LIOFromString(wxMBSTRINGCAST buf);
  hack_yyparse(0);
  wxExioCleanUp();
  return (noErrors == 0);
}

bool wxExioDatabase::Write(const wxString& fileName)
{
  //FILE *stream = wxFopen( fileName, wxT("w+"));
  auto_std_FILE stream(wxs2ch(fileName), "w+");
  
  if (!stream)
    return FALSE;
    
  bool success = Write(stream);
  //fclose(stream);
  return success;
}

bool wxExioDatabase::Write(FILE *stream)
{
  noErrors = 0;
#	if EXIO_USE_STD_CONTAINERS
	for ( iterator node = begin(); node != end(); node++ ) {
		wxExio *expr = *node;
		expr->WriteClause(stream);
	}
#	else
#if wxUSE_STL
  wxList::compatibility_iterator node = First();
#else
  wxNode *node = First();
#endif
  while (node)
  {
    wxExio *expr = (wxExio *)node->Data();
    expr->WriteClause(stream);
    node = node->Next();
  }
#	endif
  return (noErrors == 0);
}

void add_expr(wxExio * expr)
{
  thewxExioDatabase->Append(expr);
}

// Checks functor
bool wxExioIsFunctor(wxExio *expr, const wxString& functor)
{
  if (expr && (expr->Type() == wxExioList))
  {
    wxExio *first_expr = expr->value.first;

    if (first_expr && (first_expr->Type() == wxExioWord) &&
       (first_expr->WordValue() == functor))
      return TRUE;
    else 
      return FALSE;
  } 
  else 
    return FALSE;
}

/*
 * Called from parser
 *
 */

char *iomake_integer(char *str)
{
  wxExio *x = new wxExio(atol(str));

  return (char *)x;
}

char *iomake_real(char *str1, char *str2)
{
  char buf[50];

  sprintf(buf, "%s.%s", str1, str2);
  double f = (double)atof(buf);
  wxExio *x = new wxExio(f);

  return (char *)x;
}

// extern "C" double exp10(double);

char *iomake_exp(char *str1, char *str2)
{
  double mantissa = (double)atoi(str1);
  double exponent = (double)atoi(str2);

  double d = mantissa * pow(10.0, exponent);

  wxExio *x = new wxExio(d);

  return (char *)x;
}

char *iomake_exp2(char *str1, char *str2, char *str3)
{
  char buf[50];

  sprintf(buf, "%s.%s", str1, str2);
  double mantissa = (double)atof(buf);
  double exponent = (double)atoi(str3);

  double d = mantissa * pow(10.0, exponent);

  wxExio *x = new wxExio(d);

  return (char *)x;
}

char *iomake_word(char *str)
{
  wxExio *x = new wxExio(wxExioWord, wxConvLibc.cMB2WX(str));
  return (char *)x;
}

char *iomake_string(char *str)
{
  wxChar *s, *t;
  size_t len, i;
  const wxMB2WXbuf sbuf = wxConvLibc.cMB2WX(str);

//  str++;			/* skip leading quote */
  len = wxStrlen(sbuf) - 1;	/* ignore trailing quote */
    
  s = new wxChar[len + 1];
    
  t = s;
  for(i=1; i<len; i++) // 1 since we want to skip leading quote
  {
    if (sbuf[i] == wxT('\\') && sbuf[i+1] == wxT('"'))
    {
      *t++ = wxT('"');
      i ++;
    }
    else if (sbuf[i] == wxT('\\') && sbuf[i+1] == wxT('\\'))
    {
      *t++ = wxT('\\');
      i ++;
    }
    else
      *t++ = sbuf[i];
  }

  *t = wxT('\0');

  wxExio *x = new wxExio(wxExioString, s, FALSE);
  return (char *)x;
}

char *epsio_cons(char * ccar, char * ccdr)
{
  wxExio *car = (wxExio *)ccar;
  wxExio *cdr = (wxExio *)ccdr;

  if (cdr == NULL)
  {
    cdr = new wxExio(wxExioList);
  }
  if (car)
    cdr->Insert(car);
  return (char *)cdr;
}

void process_eps_cmd(char * cexpr)
{
  wxExio *expr = (wxExio *)cexpr;
  add_expr(expr);
}

void error_syntax(char *WXUNUSED(s))
{
  if (currentwxExioErrorHandler)
    (void)(*(currentwxExioErrorHandler))(WXEXPR_ERROR_SYNTAX, (char *)"syntax error");
  if (thewxExioDatabase) thewxExioDatabase->noErrors += 1;
}

#if 0
#ifdef _WINDLL
// char *__cdecl strdup(const char *s)
WXDLLEXPORT char *strdup(const char *s)
{
  int len = strlen(s);
  char *new_s = (char *)malloc(sizeof(char)*(len+1));
  strcpy(new_s, s);
  return new_s;
}
#endif
#endif

#endif
  // wxUSE_PROLOGIO
