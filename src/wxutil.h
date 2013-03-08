
// utils using wx things

#ifndef _WXUTIL_H_
#define _WXUTIL_H_

#include "splines.h"
#include "util.h"

bool WriteData(const wxString& fname, const std::list<SplineBase*>& lst
	, const std::vector<int>& hg, const std::vector<int>& vg);
int  ReadData(const wxString& fname, std::list<SplineBase*>& lst
	, std::vector<int>& hg, std::vector<int>& vg);

inline bool InRect(const wxRect& r, const wxPoint& p) {
return(p.x>=r.x && p.y>=r.y && (p.x-r.x)<r.width && (p.y-r.y)<r.height);
}

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

#endif  // _WXUTIL_H_

