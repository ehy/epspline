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

#ifndef _WXUTIL_H_
#define _WXUTIL_H_

#include <wx/image.h>
#include "splines.h"
#include "util.h"

inline bool InRect(const wxRect& r, const wxPoint& p) {
return(p.x>=r.x && p.y>=r.y && (p.x-r.x)<r.width && (p.y-r.y)<r.height);
}

// simplistc image {light,dark}ening with linear compression
// of pixel components to 'band' width (0.1, 1.0)
// NOTE: returns *same* image (i.e., source is edited)
wxImage*
wximg_bandcomp(wxImage* img, double band, bool lighten = true);

// simplistc HSV adjustment, originally for background image
// double args are -1.0,1.0
// NOTE: returns *same* image (i.e., source is edited)
wxImage* wximg_adjhsv(wxImage* img, double h, double s, double v);

// helper for above proc: take int args in range -100, 100 incl.
// -- useful e.g. in dialog boxes where a control might only
// accept integer ranges
inline wxImage*
wximg_adjhsv(wxImage* img, int _h, int _s, int _v)
{
	return wximg_adjhsv(img,
		double(_h) / 100.0, double(_s) / 100.0, double(_v) / 100.0);
}

// simple conversion to greyscale -- may be replaced
// with something, time permitting
// originally for background image
// NOTE: returns operator new'd image (i.e., source not changed)
wxImage* wximg_get_greyscale(wxImage* src, bool use_alt = false);

// rotate a wxImage by arg in degrees around center
// NOTE: returns *same* image (i.e., source is edited)
// the setbg and color args are to set wxImafe mask, the
// way provided to control background color when rotated
// non-multiple of 90 deg.; wx default is black
const unsigned char wximg_rotmask_default_r = 240;
const unsigned char wximg_rotmask_default_g = 240;
const unsigned char wximg_rotmask_default_b = 240;
wxImage* wximg_rotate(wxImage* img, double rot,
	bool setbg = true,
	unsigned char r = wximg_rotmask_default_r,
	unsigned char g = wximg_rotmask_default_g,
	unsigned char b = wximg_rotmask_default_b);

// test for any intersection of wx rectangles
bool IntersectRect(const wxRect& r0, const wxRect& r1);

// test if point is on rect line, return enum indicating which
enum or_enum {
	or_not,
	or_n, or_w, or_s, or_e,
	or_nw, or_ws, or_se, or_en
};
or_enum PointOnRect(const wxRect& r, const wxPoint& p);

// func obj tests if point is in SplineBase bounding box
class PtInBBox : public std::unary_function<SplineBase*, bool> {
	wxPoint p;
public:
	explicit PtInBBox(const wxPoint& pt) : p(pt) {}
	bool operator () (SplineBase*& s)
		{return (s && InRect(s->bbox,p));}
	bool operator () (const SplineBase*& s) const
		{return (s && InRect(s->bbox,p));}
};

#if 0
// func obj offsetting points in a SplineBase
class point_offset {
	int xo, yo;
public:
	point_offset(int x, int y) : xo(x), yo(y) {}
	inline void operator () (SplinePoint& p) {p.x+=xo; p.y+=yo;}
};
// ``move'' a SplineBase
inline void move_spline(SplineBase& s, int xoff, int yoff)
{
	std::for_each(s.begin(), s.end(), point_offset(xoff, yoff));
}
#else
inline void move_spline(SplineBase& s, int xoff, int yoff)
{
	s.MoveSpline(xoff, yoff);
}
#endif // #if 0

// How does point relate to line?
/* From Graphics Gems 1 p 50 (p 77 in .pdf version (?)) */
/* For points P and Q on line PQ, and test point T:
 * returns
 * 0 if T is not on infinite line PQ,
 * 1 if T is on ray P,
 * 2 if T is on segment PQ,
 * 3 if T is on ray Q.
 */
inline
int PointOnLine(const wxPoint& P, const wxPoint& Q, const wxPoint& T)
{
	return PointOnLine(P.x, P.y, Q.x, Q.y, T.x, T.y);
}

inline
bool PointOnSegPQ(const wxPoint& P, const wxPoint& Q, const wxPoint& T)
{
	return PointOnSegPQ(P.x, P.y, Q.x, Q.y, T.x, T.y);
}

inline
int PointOnLine
(const wxRealPoint& P, const wxRealPoint& Q, const wxRealPoint& T)
{
	wxPoint p, q, t;
	p.x = irint(P.x); p.y = irint(P.y);
	q.x = irint(Q.x); q.y = irint(Q.y);
	t.x = irint(T.x); t.y = irint(T.y);
	return PointOnLine(p, q, t);
}

inline
bool PointOnSegPQ
(const wxRealPoint& P, const wxRealPoint& Q, const wxRealPoint& T)
{
	wxPoint p, q, t;
	p.x = irint(P.x); p.y = irint(P.y);
	q.x = irint(Q.x); q.y = irint(Q.y);
	t.x = irint(T.x); t.y = irint(T.y);
	return PointOnSegPQ(p, q, t);
}

//LineLength
inline double LineLength(const wxPoint& P, const wxPoint& Q)
{
	return LineLength(double(P.x), double(P.y)
					, double(Q.x), double(Q.y));
}

// Angle between points P and Q at center C
inline double
FindAngle(const wxRealPoint& P, const wxRealPoint& Q, const wxRealPoint& C)
{
	return FindAngle(P.x, P.y, Q.x, Q.y, C.x, C.y);
}
inline double
FindAngle(const wxPoint& P, const wxPoint& Q, const wxPoint& C)
{
	return FindAngle(P.x, P.y, Q.x, Q.y, C.x, C.y);
}

// properties
int SetSplineProps(SplineBase* s, wxWindow* parent);

// func obj to draw spline
class draw_spline {
	wxDC& dc;
public:
	draw_spline(wxDC& wxdc) : dc(wxdc) {}
	inline void operator () (SplineBase*& p) {p->Draw(dc);}
};
// Draw spline list
inline void draw_spline_list(std::list<SplineBase*>& l, wxDC& dc)
{
	for_each(l.begin(), l.end(), draw_spline(dc));
}

// If a wxRect is to be used for Refresh(), then its x or y
// should not be less than 0 -- adjust as nec.
inline void FixupRect(wxRect& r) {
	if ( r.x < 0 ) { r.width += r.x; r.x = 0; }
	if ( r.y < 0 ) { r.height += r.y; r.y = 0; }
}

// For unicode builds: (NOTE 18.10.2012: had used wxConvLibc, which uses
// C library conversion if locale in environment is set well -- and
// they do indeed work, verified elsewhere -- but for some reason 
// wxConvLibc causes null pointers to be returned when wxString
// has non-ascii content [originally from utf-8 input] -- the converter
// wxConvUTF8 is working, but I am not sure it is entirely correct;
// so use wxConvCurrent which is a pointer to wx's decision on what is
// suitable, but at a point long ago I had some problem with it, cannot
// recall what that was.)
// Update: use wxMBConv* app_global_mbconv; declared in epspline.cpp,
// to maintain more control in app::OnInit()
extern wxMBConv* app_global_mbconv;
#define wxs2ch(wxs) ((const char*)(wxs).mb_str(*app_global_mbconv))
// do not use wxString::fn_str(), as it might yield wchar_t strings,
// I think for MSW's UNICODE API, but this program is using C library
// f*() functions, so it needs char*, even if multi-byte
//#define wxs2fn(wxs) ((const char*)(wxs).fn_str())
#define wxs2fn(wxs) wxs2ch(wxs)

inline wxString ch2wxs(const char* str)
{
	return app_global_mbconv->cMB2WX(str);
}

// wx version > 2.8: lots of incompatible wxString changes --
// macro for 'cast' of wxString arg to variadic wxString::Format
// of wxString::Printf
#if wxCHECK_VERSION(2, 9, 0)
#define fmtcst(wxs) (wxs)
#else
#define fmtcst(wxs) (const wxChar*)(wxs)
#endif

// This checks its argument against the set of POV-Ray reserved words,
// as found in the v. 3.6 html documentation.  Those words are all
// in ASCII.  The argument "len" is the string length of "word" or the
// number of initial characters to check, and if it is 0, length
// will by found within the function, e.g. by strlen().
bool check_identifier(const char* word, size_t len = 0);
// This version will prompt for a new string if need be, check the
// new string, and if OK assign it to the wxString referred to by "word"
// and return true, or if the user cancels then return false.
// Recurse as necessary.
bool check_identifier(wxString& word);

// This string is the subset of ASCII accepted in POV-Ray IDs
extern const wxChar pov_chars_OK[];
// number if wxChars in above
extern const size_t pov_chars_OK_count;

// Take a wxString reference that is meant as (or to be included in)
// a POV-Ray identifier and 'sanitise' it against pov_chars_OK[] (above)
// albeit in a simplistic manner.  The result is returned in the
// (non-const) reference argument.  Return true if a change was made,
// else false
bool sanitise_for_pov(wxString& str);

// replace C comments "/*" and "*/" with r
inline wxString unccom_wxs(const wxString& s, const char* r = "")
{
	std::string ss(wxs2ch(s));
	return ch2wxs(unccom_string(ss, ss, r).c_str());
}

// char string: anything that is _not_ ascii, printable or space,
// is changed to hex in the '\xNN' convention; also backslash
// "escapes" are put before '\', '"', and '\'' if esc == true
inline wxString&
sanitise_wxs(wxString& in, wxString& out, bool esc = false)
{
	std::string t(wxs2ch(in));
	sanitise_string(t, t, esc);
	out = ch2wxs(t.c_str());
	return out;
}

inline wxString&
unsanitise_wxs(wxString& in, wxString& out, bool esc = false)
{
	std::string t(wxs2ch(in));
	unsanitise_string(t, t, esc);
	out = ch2wxs(t.c_str());
	return out;
}

#endif  // _WXUTIL_H_

