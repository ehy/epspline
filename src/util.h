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

// utility

#ifndef _UTIL_H_
#define _UTIL_H_
#include "cfg.h"
#include "stdcc.h"
// std
#include <string>
#include <stdexcept>
#include <cstdio>
#include <cmath>

#if defined(__DMC__) || defined (__SC__) && defined(__WXMSW__)
#	define snprintf _snprintf // sigh
#endif

#undef  A_SIZE
#define A_SIZE(a) (sizeof(a)/sizeof((a)[0]))

// handy, used in wxutil.h
// real args: float, double, long double
#define PI_HEMI (M_PI / 180.0)
template< class T > inline T deg2rad(T degrees)
{
	return degrees * T(PI_HEMI);
}
template< class T > inline T rad2deg(T radians)
{
	return radians / T(PI_HEMI);
}

// macro and inline func for image alpha overlay blending
// on uchar color component, where alpha is also uchar
// 0(transparent),255(opaque) -- Afor is foreground (top)
// alpha, Aback is alpha of bg (blended into pixel component)
#define ALPHA_OVER(Cfor, Afor, Cback, Aback) \
	( \
		(unsigned char)( \
			(((unsigned)(Cfor) * (Afor) / 255) + \
			((unsigned)(Cback) * (Aback) / 255) * \
			(255 - (Afor)) / 255) \
		) \
	)
inline unsigned char
alpha_over(
	unsigned char Cfor, unsigned char Afor,
	unsigned char Cback, unsigned char Aback = 255)
{
	return static_cast<unsigned char>(
		((unsigned)Cfor * Afor / 255) +
		((unsigned)Cback * Aback / 255) *
		(255 - Afor) / 255
	);
}

// quick, dirty automatic buffer w/ closure
template < class T > struct tbuffer {
	std::size_t cnt;
	T* ptr;
	tbuffer(std::size_t size) : cnt(size), ptr(new T[size]) {}
	~tbuffer()       { delete[] ptr; }
	const T* p() const { return ptr; }
	operator const T*() const { return ptr; }
	operator const void*() const { return ptr; }
	T* p()            { return ptr; }
	operator T*()    { return ptr; }
	operator void*() { return ptr; }
	T& at(std::size_t i) throw(std::range_error)
	{
		if ( i >= cnt ) {
			throw std::range_error("operator [] indice too great");
		}
		return ptr[i];
	}
	const T& at(std::size_t i) const throw(std::range_error)
	{
		if ( i >= cnt ) {
			throw std::range_error("operator [] indice too great");
		}
		return ptr[i];
	}
	T& operator [] (std::size_t i) throw(std::range_error)
	{ return at(i); }
	const T& operator [] (std::size_t i) const throw(std::range_error)
	{ return at(i); }
};

struct tmpchbuffer : public tbuffer<char> {
	tmpchbuffer(const char* s)
	: tbuffer<char>(std::strlen(s) + 1) {
		if ( cnt > 1 ) {
			// not in std::
			::snprintf(ptr, cnt, "%s", s);
		}
		ptr[cnt - 1] = '\0';
	}
};
struct tmpwchbuffer : public tbuffer<wchar_t> {
	tmpwchbuffer(const wchar_t* s)
	: tbuffer<wchar_t>(std::wcslen(s) + 1) {
		if ( cnt > 1 ) {
			std::wcscpy(ptr, s);
		}
		ptr[cnt - 1] = '\0';
	}
};

typedef tmpchbuffer  tmp_cstr;
typedef tmpwchbuffer tmp_wstr;

#if defined(_UNICODE) || defined(UNICODE)
typedef tmp_wstr	tmp_str;
#else
typedef tmp_cstr	tmp_str;
#endif

// copy string with op new
#if defined(_UNICODE) || defined(UNICODE)
#	define newstrdup newwstrdup
#else
#	define newstrdup newcstrdup
#endif
// copy char string with op new
char* newcstrdup(const char* str);
// copy wide char string with op new
wchar_t* newwstrdup(const wchar_t* str);

#if ! NO_TRAWBUFFER
// Like tbuffer above, but tbuffer uses C++ operator new[], and
// therefore will invoke the default constructor over the entire
// array, which might not be small.  This trawbuffer is a hack
// to avoid new[], and thereby avoid the default constructor
// calls over the array; *BUT* that makes this class (struct)
// DANGEROUS -- it must only be used when it is certain that the
// default constructor is truly unneeded, e.g. a class of a few
// ints only that can tolerate uninitialized members due to the
// way it will be used.
//
// NOTE though, that on GNU/Linux with g++ or clang++ it appears
// that memory from std::get_temporary_buffer is initialized to
// zero (but hopefully more quickly overall than operator new[]).
#include <memory> // just a reminder; these are included in "stdcc.h"
#include <new>
template < class T > struct trawbuffer {
	T* ptr;
	trawbuffer(std::ptrdiff_t size) throw(std::bad_alloc) : ptr(0) {
		std::pair <T*, std::ptrdiff_t> pr =
			std::get_temporary_buffer<T>(size);
		ptr = pr.first;
		if ( ptr == 0 || pr.second < size ) {
			throw std::bad_alloc();
		}
	}
	~trawbuffer(){
		if ( ptr ) {
			std::return_temporary_buffer(ptr);
		}
	}
	operator T* ()    { return ptr; }
	operator void* () { return ptr; }
	T& operator [] (int i) { return ptr[i]; }
};
#endif // ! NO_TRAWBUFFER

// Automatic, or "scoped", std FILE, closed in dtor
class auto_std_FILE {
	std::FILE* fp;
	auto_std_FILE(const auto_std_FILE&);
	auto_std_FILE& operator = (const auto_std_FILE&);
public:
	auto_std_FILE() { fp = 0; }
	auto_std_FILE(std::FILE* already_open) { fp = already_open; }
	auto_std_FILE(const char* n, const char* m = "w+b")
	{ fp = std::fopen(n, m); }
	auto_std_FILE(int fd, const char* m = "w+b")
	{ fp = /* not in std */ ::fdopen(fd, m); }
	~auto_std_FILE() { if ( fp ) std::fclose(fp); }

	operator std::FILE* () { return fp; }
	std::FILE* get() { return fp; }
	bool operator == (std::FILE* o) const { return fp == o; }
	bool isopen() const { return fp != 0; }
	operator bool () const { return isopen(); }
	
	std::FILE* release(std::FILE* newfp = 0)
	{ std::FILE* t = fp; fp = newfp; return t; }
	std::FILE* assign(std::FILE* newfp) { return release(newfp); }
};

// There would be no good reason to run this in a priviliged account,
// but provide a test anyway, for some security conditions
// *NOTE* presently this is only real for Unix; on MSW it always
// returns false, because I don't know MSW well enough.
bool
is_priviliged();

// char string: anything that is _not_ ascii, printable or space,
// is changed to hex in the '\xNN' convention; also backslash
// "escapes" are put before '\', '"', and '\'' if esc == true
std::string&
sanitise_string(std::string& in, std::string& out, bool esc = false);
std::string&
unsanitise_string(std::string& in, std::string& out, bool esc = false);
// replace C comments "/*" and "*/" with r
std::string&
unccom_string(std::string& in, std::string& out, const char* r = "");

// delete elements of pointer container
template<class T>
class delete_pointer {
public:
	delete_pointer() {}
	inline void operator () (T& p) {delete p; p = 0;}
};

template<class C> inline void
delete_contained(C& c)
{
for_each(c.begin(), c.end(), delete_pointer<typename C::value_type>());
}
template<class C> inline void
delete_clear_contained(C& c)
{
	delete_contained(c);
	clear(c);
}

// pointer container copy with clearing of destination
template<class C, class D> inline void
delete_clear_copy(C& dest, const D& source)
{
	delete_clear_contained(dest);
	copy(source.begin(), source.end(), back_inserter(dest));
}

// How does point relate to line?
/* From Graphics Gems 1 p 50 (p 77 in .pdf version (?)) */
/* For points P and Q on line PQ, and test point T:
 * returns
 * 0 if T is not on infinite line PQ,
 * 1 if T is on ray P,
 * 2 if T is on segment PQ,
 * 3 if T is on ray Q.
 */
int PointOnLine(long Px, long Py,
                long Qx, long Qy,
                long Tx, long Ty);
// a convenience for PointOnLine returns
enum vals_POLN {
    poln_no = 0, poln_P = 1, poln_PQ = 2, poln_Q = 3
};
inline bool
PointOnSegPQ(long Px, long Py,
            long Qx, long Qy,
            long Tx, long Ty)
{
    return PointOnLine(Px, Py, Qx, Qy, Tx, Ty) == poln_PQ;
}

// Length of Line
inline double LineLength(double Qx, double Qy)
{
#	if NEED_HYPOT
	return std::sqrt(Qx * Qx + Qy * Qy);
#	else
	return ::hypot(Qx, Qy);
#	endif
}
inline double LineLength(double Px, double Py,
                        double Qx, double Qy)
{
	return LineLength(Qx - Px, Qy - Py);
}

// Angle between points P and Q at center C
double
FindAngle(double Px, double Py,
          double Qx, double Qy,
          double Cx, double Cy);

// For two rays sharing an endpoint but not on same line,
// find Qx, Qy on line opposite Px, Py, with same length
// as original Cx,Cy-->Qx,Qy.  Return result in (reference) Qx, Qy.
void
FindOppositeRayEnd(double Px, double Py,
                   double Cx, double Cy,
                   double& Qx, double& Qy);

// Get a temp file name *in static buffer*; like "/tmp/t3762.XXXXXX"
// use for mkstemp, etc..
char* gettmpnam();
// Get open temp file descriptor.  If buf != NULL, copy file name
// to, but at most len - 1 chars
int gettmpfd(char* buf, size_t len);

// Double to well rounded int or long using lrint(double) (math.h)
#ifndef IRINT
#   define IRINT(d) irint(d)
#endif
// Round double to int
// note: had long used rint(3)|lrint(3), but these are subject to
// setable rounding direction where available (e.g. fesetround in glibc)
// but if lround(3) is available it always rounds in the same way
#ifndef LRINT
#	if LROUND_UNAVAILABLE
#		define LRINT(d) long(((d) + (((d) < 0.0) ? -0.5 : 0.5)))
#	else
#		define LRINT(d) lround(d)
#	endif
#endif

#if LROUND_UNAVAILABLE
#	if defined(__DMC__) || defined (__SC__) && defined(__WXMSW__)
// Bummer: dmc will not use a C++ template definition of lround() --
// it is prototyped in math.h, but it is not defined in C-library.
// The compiler prefers the specific proto it has seen over the
// template here, but of course link fails on missing definition.
#		define lround(v)	LRINT(v)
#	else
template<typename fT> inline long lround(fT v)
{
	return LRINT(v);
}
#	endif
#endif

inline int irint(double d) {
    return int(LRINT(d));
}
inline long int lirint(double d) {
    return LRINT(d);
}

// Debug things.  FIXME: not likely working on MS
#if DEBUG
// formatted messages:
#	define FPRINTF(args) \
		do { fprintf args ; } while (0)
#	define DBGERR1(arg) \
		FPRINTF((stderr, (arg)))
#	define DBGERR2(a1, a2) \
		FPRINTF((stderr, (a1), (a2)))
#	define DBGERR3(a1, a2, a3) \
		FPRINTF((stderr, (a1), (a2), (a3)))
#	define DBGERR4(a1, a2, a3, a4) \
		FPRINTF((stderr, (a1), (a2), (a3), (a4)))
#	define DBGERR5(a1, a2, a3, a4, a5) \
		FPRINTF((stderr, (a1), (a2), (a3), (a4), (a5)))
#else
#	define FPRINTF(a1) do { ; } while (0)
#	define DBGERR1(a1) do { ; } while (0)
#	define DBGERR2(a1,a2) do { ; } while (0)
#	define DBGERR3(a1,a2,a3) do { ; } while (0)
#	define DBGERR4(a1,a2,a3,a4) do { ; } while (0)
#	define DBGERR5(a1,a2,a3,a4,a5) do { ; } while (0)
#endif

#endif  // _UTIL_H_

