/*
 * Copyright (c) 2013 Ed Hynan
 *
 * This file is part of Epspline
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

// utils using wx things

#ifndef _WXSTRUTIL_H_
#define _WXSTRUTIL_H_

#include <wx/strconv.h>
#include <wx/string.h>

#include <string>

extern wxMBConv* app_global_mbconv; // in file with app implementation

// wxString to const char*
#define wxs2ch(wxs) ((const char*)(wxs).mb_str(*app_global_mbconv))

// wxString to filename const char*
#define wxs2fn(wxs) wxs2ch(wxs)

// const char* to wxString
inline wxString ch2wxs(const char* str)
{
	return app_global_mbconv->cMB2WX(str);
}

// std::string to wxString
inline wxString ss2wxs(const std::string& str)
{
	return ch2wxs(str.c_str());
}

// wx version > 2.8: lots of incompatible wxString changes --
// macro for 'cast' of wxString arg to variadic wxString::Format
// of wxString::Printf
#if wxCHECK_VERSION(2, 9, 0)
#define fmtcst(wxs) (wxs)
#else
#define fmtcst(wxs) ((const wxChar*)(wxs))
#endif


#endif  // _WXSTRUTIL_H_

