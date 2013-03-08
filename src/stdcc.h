// minor C++ portability stuff
//
//
//

#ifndef _STDCC_H_
#define _STDCC_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/config.h>
#endif

// if using wxwidgets work around possible std:: conflicts
// especially with _T in Sun CC's stlport templates
#   if defined(_WX_WX_H_) || defined(_WX_CONFIG_H_BASE_) && \
	wxCHECK_VERSION(2, 6, 0)
#      define wxNEEDS_T 1
#      include <wx/beforestd.h>
#   endif

#   if HAVE_OLD_HEADERS || PRE_STD_CPP
#      include <vector.h>
#      include <list.h>
#      include <deque.h>
#      include <algo.h>
#      include <function.h>
#      include <stdio.h>
#      include <string.h>
#      include <time.h>
#      include <math.h>
#      if __SC__
#         include <fltpnt.h>    //  For double rint(double) proto.
#      endif
#      include <limits.h>
#      include <errno.h>

#      include <ctype.h>
#      include <wctype.h>
#      include <locale.h>

template < class C > inline void clear(C & c)
{
    c.erase(c.begin(), c.end());
}

#      if __SC__ && (__SC__ < 0x750)
                                // Still error w/ 7.60
#         define typename
#         define mutable
#         define explicit
#      endif

// no namespaces
#      define std

#   else                        // HAVE_OLD_HEADERS || PRE_STD_CPP

#      include <vector>
#      include <list>
#      include <deque>
#      include <string>
#      include <algorithm>
#      include <functional>
#      include <exception>
#      include <stdexcept>
#      include <utility>
#      include <memory>
#      include <locale>
#      include <cstdio>
#      include <cstdlib>
#      include <cstring>
#      include <cstdarg>
#      include <ctime>
#      include <cmath>
#      include <climits>
#      include <cassert>
#      include <cerrno>
#      include <cctype>
#      include <cwctype>
#      include <clocale>

// C++ has a to(upper|lower) template in <locale>, and of course it
// uses the C++ locale framework; but, even at this late date
// (12-12) C++ locale is broken on some systems (e.g.
// FreeBSD 9.0, OpenBSD 4.9, BSD->OS X, where setting any locale
// except "C" always throws runtime_error!).  It is necessary,
// therefore, to avoid C++ locale.
namespace eps_funcs {
	inline char  ch_tolower(char c) {
	    return std::tolower(int(c));
	}
	
	inline wchar_t  wc_tolower(wchar_t c) {
	    return std::towlower(c);
	}
	
	inline char  ch_toupper(char c) {
	    return std::toupper(int(c));
	}
	
	inline wchar_t  wc_toupper(wchar_t c) {
	    return std::towupper(c);
	}
} // namespace eps_funcs

// *lousy* hack -- see comment above -- and this will not behave
// as expected for int-meant-as-char, in the C manner; use with
// care, like 'ch_t c = c_tolower(ch_t(some_string[n]))', where
// 'ch_t' may be char or wchar_t per compile-time conditions.
template < class T >
inline T    c_tolower(T c) {
	if ( sizeof(T) == sizeof(char) ) {
		return T(eps_funcs::ch_tolower(char(c)));
	}
    return T(eps_funcs::wc_tolower(wchar_t(c)));
}

template < class T >
inline T    c_toupper(T c) {
	if ( sizeof(T) == sizeof(char) ) {
		return T(eps_funcs::ch_toupper(char(c)));
	}
    return T(eps_funcs::wc_toupper(wchar_t(c)));
}

// put container::clear() as non-member in global space
template <class C> inline void clear(C& c)
{
    c.clear();
}

#      ifdef __DMC__
// the free Digital Mars C++ and 'stlport' do not have std::abs(int),
// anc c++ compiler errors on ambiguity long/double!
#         ifdef _64BIT
#            define ABSINTTYPE long long
#         else
#            define ABSINTTYPE long
#         endif
namespace std {
    inline int abs(int arg) {
        return int (abs((ABSINTTYPE) arg));
    }
};
#      endif

#   endif                        // HAVE_OLD_HEADERS || PRE_STD_CPP

// if using wxwidgets work around possible std:: conflicts
// especially with _T in Sun CC's stlport templates
#   if defined(_WX_WX_H_) || defined(_WX_CONFIG_H_BASE_) && \
	wxCHECK_VERSION(2, 6, 0)
#      include <wx/afterstd.h>
#   endif

#endif                          // _STDCC_H_
