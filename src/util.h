
// utility

#ifndef _UTIL_H_
#define _UTIL_H_
#include "cfg.h"
#include "stdcc.h"

#undef  A_SIZE
#define A_SIZE(a) (sizeof(a)/sizeof((a)[0]))

// quick, dirty automatic buffer w/ closure
template < class T > struct tbuffer {
	T* ptr;
	tbuffer(std::size_t size) : ptr(new T[size]) {}
	~tbuffer()       { delete[] ptr; }
	operator T*()    { return ptr; }
	operator void*() { return ptr; }
	T& operator [] (int i) { return ptr[i]; }
};

struct tmpchbuffer : public tbuffer<char> {
	tmpchbuffer(const char* s)
	: tbuffer<char>(std::strlen(s) + 1) {
		std::strcpy(ptr, s);
	}
};
struct tmpwchbuffer : public tbuffer<wchar_t> {
	tmpwchbuffer(const wchar_t* s)
	: tbuffer<wchar_t>(std::wcslen(s) + 1) {
		std::wcscpy(ptr, s);
	}
};
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
	operator T*()    { return ptr; }
	operator void*() { return ptr; }
	T& operator [] (int i) { return ptr[i]; }
};
#endif // ! NO_TRAWBUFFER

// There would be no good reason to run this in a priviliged account,
// but provide a test anyway, for some security conditions
// *NOTE* presently this is only real for Unix; on MSW it always
// returns false, because I don't know MSW well enough.
bool
is_priviliged();

// copy t_ch string with op new
#if defined(_UNICODE) || defined(UNICODE)
#	define newstrdup newwstrdup
#else
#	define newstrdup newcstrdup
#endif
// copy char string with op new
char* newcstrdup(const char* str);
// copy wide char string with op new
wchar_t* newwstrdup(const wchar_t* str);

// Temporary new[]'d string of char types
template<class T, class AllocF>
class tmpstr_tpl {
	AllocF f;
	T* p;
public:
	tmpstr_tpl(const T* str) : p(f(str)) {}
	~tmpstr_tpl() {delete[] p;}
	inline T* getchar() {return p;}
	inline const T* getcchar() const {return p;}
	inline operator T* () {return getchar();}
	inline operator const T* () const {return getcchar();}
	inline T* c() {return getchar();}
	inline const T* cc() const {return getcchar();}
	inline T& operator [] (int i) { return p[i]; }
	inline const T& operator [] (int i) const { return p[i]; }
	inline T* release() {T* t = p; p = 0; return t;}
};

class char_new_duplicator {
public:
	char_new_duplicator() {}
	inline char* operator () (const char* p) {
	    return newcstrdup(p);
	}
};
typedef tmpstr_tpl<char, char_new_duplicator>		tmp_cstr;

class wchar_t_new_duplicator {
public:
	wchar_t_new_duplicator() {}
	inline wchar_t* operator () (const wchar_t* p) {
	    return newwstrdup(p);
	}
};
typedef tmpstr_tpl<wchar_t, wchar_t_new_duplicator>	tmp_wstr;

#if defined(_UNICODE) || defined(UNICODE)
typedef tmp_wstr	tmp_str;
#else
typedef tmp_cstr	tmp_str;
#endif

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
	return std::sqrt(Qx * Qx + Qy * Qy);
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
template<typename fT> inline long lround(fT v)
{
	return LRINT(v);
}
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

