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
// Name:        wxexpr.h
// Purpose:     Prolog-like file I/O, used by resource system.
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: wxexpr.h,v 1.16.2.1 2002/11/04 23:22:44 VZ Exp $
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _APPLOCAL_WXEXPRH__
#define _APPLOCAL_WXEXPRH__

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "wxexio.h"
#endif

// orig: #if wxUSE_PROLOGIO
#if 1

#include <wx/defs.h>
#include <wx/string.h>

#include <wx/list.h>
#include <wx/hash.h>

#include "exio.h"

#include <stdio.h>

#if wxCHECK_VERSION(2,6,0) && 1
#	include <list>
#	define EXIO_USE_STD_CONTAINERS 1
#endif

// Compatibility
#define PrologExpr wxExio
#define PrologDatabase wxExioDatabase
#define proioErrorHandler wxExioErrorHandler
#define EPSIO_ERROR_GENERAL 1
#define EPSIO_ERROR_SYNTAX  2
#define PrologNull wxExioNull
#define PrologInteger wxExioInteger
#define PrologReal wxExioReal
#define PrologWord wxExioWord
#define PrologString wxExioString
#define PrologList wxExioList
#define PrologType wxExioType

// Error types
#define WXEXPR_ERROR_GENERAL 1
#define WXEXPR_ERROR_SYNTAX  2

// Error handler function definition. If app returns TRUE,
// carry on processing.
typedef bool (*wxExioErrorHandler) (int errorType, const char *msg);

WXDLLEXPORT_DATA(extern wxExioErrorHandler) currentwxExioErrorHandler;


typedef enum {
    wxExioNull,
    wxExioInteger,
    wxExioReal,
    wxExioWord,
    wxExioString,
    wxExioList
} wxExioType;

class WXDLLEXPORT wxExioDatabase;

class WXDLLEXPORT wxExio
{
 public:
  wxObject *client_data;
  wxExioType type;
  union {
    long integer;
    wxChar *word;
    wxChar *string;
    double real;
    wxExio *first;  // If is a list expr, points to the first node
    } value;

  wxExio *next;     // If this is a node in a list, points to the next node
  wxExio *last;     // If is a list expr, points to the last node

  wxExio(wxExioType the_type, wxChar *word_or_string, bool allocate);
  wxExio(const wxString& functor);      // Assume this is a new clause - pass functor
  wxExio(wxExioType the_type, const wxString& word_or_string = wxT(""));
  wxExio(long the_integer);
  wxExio(double the_real);
  wxExio(wxList *the_list);
  ~wxExio(void);

  inline wxExioType Type(void) const { return type; }
  inline long IntegerValue(void) const
  {
    if (type == wxExioInteger)
      return value.integer;
    else if (type == wxExioReal)
      return (long)value.real;
    else return 0;
  }

  inline double RealValue(void) const {
    if (type == wxExioReal)
      return value.real;
    else if (type == wxExioInteger)
      return (double)value.integer;
    else return (double)0.0;
  }

  inline wxString WordValue(void) const {
    if (type == wxExioWord)
      return value.word;
    else if (type == wxExioString)
      return wxString(value.string);
    else return wxString(wxT(""));
  }

  inline wxString StringValue(void) const {
    if (type == wxExioString)
      return wxString(value.string);
    else if (type == wxExioWord)
      return wxString(value.word);
    else return wxString(wxT(""));
  }

  // Get nth arg of clause (starting from 1)
  wxExio *Arg(wxExioType type, int arg) const;

  // Return nth argument of a list expression (starting from zero)
  wxExio *Nth(int arg) const;

  // Returns the number of elements in a list expression
  int Number(void) const;

  // Make a clone
  wxExio *Copy(void) const;

  wxExio *GetAttributeValueNode(const wxString& word) const;  // Use only for a clause or list
  wxExio *AttributeValue(const wxString& word) const;  // Use only for a clause
  wxString Functor(void) const;                     // Only for a clause
  bool IsFunctor(const wxString& s) const;                     // Only for a clause
  void WriteClause(FILE* stream);  // Write this expression as a top-level clause
  void WriteExpr(FILE* stream);    // Write as any other subexpression

  // Append an expression to a list
  void Append(wxExio *expr);
  // Insert at beginning of list
  void Insert(wxExio *expr);

  // Get first expr in list
  inline wxExio *GetFirst(void) const { return ((type == wxExioList) ? value.first : (wxExio*)NULL); }

  // Get next expr if this is a node in a list
  inline wxExio *GetNext(void) const { return next; }

  // Get last expr in list
  inline wxExio *GetLast(void) const { return ((type == wxExioList) ? last : (wxExio*)NULL); }

  // This should really be called SetAttributeValue since any existing
  // attribute-value is deleted first.
  void AddAttributeValue(const wxString& attribute, long value);
  void AddAttributeValue(const wxString& attribute, double value);
  void AddAttributeValueWord(const wxString& attribute, const wxString& value);
  void AddAttributeValueString(const wxString& attribute, const wxString& value);
  void AddAttributeValue(const wxString& attribute, wxList *value);
  void AddAttributeValue(const wxString& attribute, wxExio *value);
  void AddAttributeValueStringList(const wxString& attribute, wxList *string_list);

  void DeleteAttributeValue(const wxString& attribute);

  bool GetAttributeValue(const wxString& att, int& var) const;
  bool GetAttributeValue(const wxString& att, long& var) const;
  bool GetAttributeValue(const wxString& att, float& var) const;
  bool GetAttributeValue(const wxString& att, double& var) const;
  bool GetAttributeValue(const wxString& att, wxString& var) const;  // Word OR string -> string
  bool GetAttributeValue(const wxString& att, wxExio **var) const;

//  // Compatibility with old PrologIO
//  inline void AssignAttributeValue(wxChar *att, int *var) const { GetAttributeValue(att, *var); }
//  inline void AssignAttributeValue(wxChar *att, long *var) const { GetAttributeValue(att, *var); }
//  inline void AssignAttributeValue(wxChar *att, float *var) const { GetAttributeValue(att, *var); }
//  inline void AssignAttributeValue(wxChar *att, double *var) const { GetAttributeValue(att, *var); }
//  inline void AssignAttributeValue(wxChar *att, wxExio **var) const { GetAttributeValue(att, var); }
//  void AssignAttributeValue(wxChar *att, wxChar **var) const ;  // Word OR string -> string

  // Add string items to list if the list attribute exists
  bool GetAttributeValueStringList(const wxString& att, wxList *var) const;

  // Associate other data with this expression, e.g. when reading in a
  // number of linked items - store C++ object pointer with the expression
  // so we can index into the wxExio database and fish out the pointer.
  inline void SetClientData(wxObject *data) { client_data = data; }
  inline wxObject *GetClientData(void) const { return client_data; }
};

#	if EXIO_USE_STD_CONTAINERS
class WXExioList: public std::list<wxExio*>, public wxObject {
public:
};
#	else
#if wxUSE_STL
class WXExioList: public wxList, public wxObject {
public:
};
#else
#define WXExioList wxList
#endif
#	endif

class WXDLLEXPORT wxExioDatabase: public WXExioList
{
private:
 #	if EXIO_USE_STD_CONTAINERS
   iterator position;  // Where we are in a search
#	else
#if wxUSE_STL
   wxList::compatibility_iterator position;  // Where we are in a search
#else
    wxNode *position;              // Where we are in a search
#endif
#	endif
    wxHashTable *hash_table;
    wxString attribute_to_hash;

public:
    int noErrors;

    wxExioDatabase(wxExioErrorHandler handler = 0);

    // Use hashing on both the functor, and the attribute of
    // specified type (wxExioString or wxExioInteger) and name.
    // So to find node 45
    // (i.e. match the clause node(id=45, ...))
    // it usually requires 1 look-up: the keys for functor and attribute
    // are added together.
    // Obviously if the attribute was missing in a clause, it would
    // fail to be found by this method, but could be retrieved by a
    // linear search using BeginFind and FindClauseByFunctor,
    // or just searching through the list as per usual.

    wxExioDatabase(wxExioType type, const wxString& attribute, int size = 500,
            wxExioErrorHandler handler = 0);

    ~wxExioDatabase(void);

    void BeginFind(void) ;          // Initialise a search
    wxExio *FindClause(long id) ;  // Find a term based on an integer id attribute
    // e.g. node(id=23, type=rectangle, ....).

    // Find on basis of attribute/value pairs, e.g. type=rectangle
    // This doesn't use hashing; it's a linear search.
    wxExio *FindClause(const wxString& word, const wxString& value);
    wxExio *FindClause(const wxString& word, long value);
    wxExio *FindClause(const wxString& word, double value);
    wxExio *FindClauseByFunctor(const wxString& functor);

    wxExio *HashFind(const wxString& functor, const wxString& value) const;
    wxExio *HashFind(const wxString& functor, long value) const;

    void Append(wxExio *expr);  // Does cleverer things if hashing is on
    void ClearDatabase(void);
    inline int GetErrorCount() const { return noErrors; }
    bool Read(const wxString& filename);
    bool ReadFromString(const wxString& buffer);
    bool Write(const wxString& fileName);
    bool Write(FILE* stream);

//    // Compatibility
//    inline bool ReadProlog(wxChar *filename) { return Read(wxString(filename)); }
//#   if wxCHECK_VERSION(2, 9, 0)
//    inline bool ReadPrologFromString(char *buffer) { return ReadFromString(wxString(buffer, *wxConvCurrent)); }
//#   else
//    inline bool ReadPrologFromString(char *buffer) { return ReadFromString(wxString(buffer, wxConvCurrent)); }
//#   endif
//    inline void WriteProlog(FILE* stream) { Write(stream); }

private:
    DECLARE_DYNAMIC_CLASS(wxExioDatabase)
};

// Function call-style interface - some more convenience wrappers/unwrappers

// Make a call
WXDLLEXPORT wxExio* wxExioMakeCall(const wxString& functor ...);

#define wxExioMakeInteger(x) (new wxExio((long)x))
#define wxExioMakeReal(x) (new wxExio((double)x))
#define wxExioMakeString(x) (new wxExio(wxExioString, x))
#define wxExioMakeWord(x)   (new wxExio(wxExioWord, x))
#define wxExioMake(x)       (new wxExio(x))

// Checks functor
WXDLLEXPORT bool wxExioIsFunctor(wxExio *expr, const wxString& functor);

// Temporary variable for communicating between wxexio.cpp and YACC/LEX
WXDLLEXPORT_DATA(extern wxExioDatabase*) thewxExioDatabase;

// YACC/LEX can leave memory lying around...
extern "C" WXDLLEXPORT int wxExioCleanUp();

#endif // wxUSE_PROLOGIO

#endif // _APPLOCAL_WXEXPRH__

