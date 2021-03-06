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

// Utility things:


// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/process.h>
#if wxCHECK_VERSION(2, 4, 0)
#include <wx/filename.h>
#endif
#include <wx/file.h>
#endif // WX_PRECOMP
#include "epspline.h"
#include "util.h"
#include "wxutil.h"

#ifdef __WXMSW__
#include <cstdio>
#include <climits>
#include <io.h>
#include <fcntl.h>
#ifdef __SC__ // actually, DMC: proto missing, definition in C lib
extern "C" int     __CLIB _snprintf(char *,size_t,const char *,...);
#define snprintf _snprintf
#endif
#else
#include <climits>
#include <sys/stat.h>

#ifdef __UNIX__
#include <unistd.h>
#include <sys/types.h>
#endif

#ifdef _BSD_SOURCE
#include <paths.h>
#endif
#endif

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cmath>

#ifndef PATH_MAX
	// Posix
#	ifdef _POSIX_PATH_MAX
#		define PATH_MAX _POSIX_PATH_MAX
#	endif
#endif
#ifndef PATH_MAX
	// BSD
#	ifdef MAXPATHLEN
#		define PATH_MAX MAXPATHLEN
#	endif
#endif
#ifndef PATH_MAX
	// MSDOS
#	ifdef _MAX_PATH
#		define PATH_MAX _MAX_PATH
#	endif
#endif

#ifndef PATH_MAX
#	error "Fix PATH_MAX macro for your platform!"
#endif


// utility

char*
newcstrdup(const char* str)
{
	if ( !str ) return 0;
	char* p = new char[std::strlen(str) + 1];
	return p ? std::strcpy(p, str) : 0;
}

wchar_t*
newwstrdup(const wchar_t* str)
{
	if ( !str ) return 0;
	wchar_t* p = new wchar_t[std::wcslen(str) + 1];
	return p ? std::wcscpy(p, str) : 0;
}

// simplistc image {light,dark}ening with linear compression
// of pixel components to 'band' width (0.1, 1.0)
// NOTE: returns *same* image (i.e., source is edited)
wxImage*
wximg_bandcomp(wxImage* img, double band, bool lighten)
{
	if ( ! img ) {
		return 0;
	}

	const size_t pixwid = 3;
	const int R = 0;
	const int G = 1;
	const int B = 2;

	int wi = img->GetWidth(), hi = img->GetHeight();

	if ( wi < 1 || hi < 1 ) {
		return img;
	}

	unsigned char* p = img->GetData();

	if ( ! p ) {
		return img;
	}

	unsigned char* e = p + (size_t(wi) * pixwid * size_t(hi));

	bool hasmask = img->HasMask();
	unsigned char rm = 0, gm = 0, bm = 0;
		
	if ( hasmask ) {
		rm = img->GetMaskRed();
		gm = img->GetMaskGreen();
		bm = img->GetMaskBlue();
	}
	
	band = std::min(1.0, band);
	band = std::max(0.0, band);

	unsigned char add = lighten ?
		static_cast<unsigned char>(255.0 - band * 255.0) : 0;

	while ( p < e ) {
		p[R] = add + static_cast<unsigned char>(band * p[R]);
		p[G] = add + static_cast<unsigned char>(band * p[G]);
		p[B] = add + static_cast<unsigned char>(band * p[B]);

		p += pixwid;
	}

	if ( hasmask ) {
		rm = add + static_cast<unsigned char>(band * rm);
		gm = add + static_cast<unsigned char>(band * gm);
		bm = add + static_cast<unsigned char>(band * bm);
		
		img->SetMaskColour(rm, gm, bm);
	}

	return img;
}

// simplistc HSV adjustment, originally for background image
// double args are -1.0,1.0
// NOTE: returns *same* image (i.e., source is edited)
// (1st static inlines to support wximg_adjhsv)
static inline double adjhsvH(double v, double a)
{
	a *= 0.5;
	v += a;
	return v > 1.0 ? (v - 1.0) : (v < 0.0 ? (v + 1.0) : v);
}
static inline double adjhsvS(double v, double a)
{
	return v + (a > 0.0 ? (a * (1.0 - v)) : (a * v));
}
static inline double adjhsvV(double v, double a)
{
	return v + (a > 0.0 ? (a * (1.0 - v)) : (a * v));
}
wxImage*
wximg_adjhsv(wxImage* img, double h, double s, double v)
{
	if ( ! img ) {
		return 0;
	}

	const size_t pixwid = 3;
	const int R = 0;
	const int G = 1;
	const int B = 2;

	int wi = img->GetWidth(), hi = img->GetHeight();

	if ( wi < 1 || hi < 1 ) {
		return img;
	}

	unsigned char* p = img->GetData();

	if ( ! p ) {
		return img;
	}

	unsigned char* e = p + (size_t(wi) * pixwid * size_t(hi));

	bool hasmask = img->HasMask();
	unsigned char rm = 0, gm = 0, bm = 0;
		
	if ( hasmask ) {
		rm = img->GetMaskRed();
		gm = img->GetMaskGreen();
		bm = img->GetMaskBlue();
	}

	while ( p < e ) {
		wxImage::HSVValue hsv =
			wxImage::RGBtoHSV(wxImage::RGBValue(p[R], p[G], p[B]));

		wxImage::RGBValue rgb =
			wxImage::HSVtoRGB(wxImage::HSVValue(
				adjhsvH(hsv.hue, h),
				adjhsvS(hsv.saturation, s),
				adjhsvV(hsv.value, v)
			));

		p[R] = rgb.red;
		p[G] = rgb.green;
		p[B] = rgb.blue;

		p += pixwid;
	}

	if ( hasmask ) {
		wxImage::HSVValue hsv =
			wxImage::RGBtoHSV(wxImage::RGBValue(rm, gm, bm));

		wxImage::RGBValue rgb =
			wxImage::HSVtoRGB(wxImage::HSVValue(
				adjhsvH(hsv.hue, h),
				adjhsvS(hsv.saturation, s),
				adjhsvV(hsv.value, v)
			));
		
		img->SetMaskColour(rgb.red, rgb.green, rgb.blue);
	}

	return img;
}

// simple conversion to greyscale -- may be replaced
// with something, time permitting
// originally for background image
// NOTE: returns operator new'd image (i.e., source not changed)
wxImage*
wximg_get_greyscale(wxImage* src, bool use_alt)
{
	const double alt_r = 0.2126;
	const double alt_b = 0.0722;
	const double alt_g = 1.0 - alt_b - alt_r;
	
	wxImage* dest;

	if ( use_alt ) {
		dest = new wxImage(src->ConvertToGreyscale(
			alt_r, alt_g, alt_b
		));
	} else {
		dest = new wxImage(src->ConvertToGreyscale());
	}

	return dest;
}

// rotate a wxImage by arg in degrees around center
// NOTE: returns *same* image (i.e., source is edited)
// the setbg and color args are to set wxImafe mask, the
// way provided to control background color when rotated
// non-multiple of 90 deg.; wx default is black
wxImage*
wximg_rotate(wxImage* img, double rot,
	bool setbg, unsigned char r, unsigned char g, unsigned char b)
{
	if ( setbg && ! img->HasMask() && ! img->HasAlpha() ) {
		img->SetMaskColour(r, g, b);
	}

	// NOTE bool arg interpolate: if image relies on a mask
	// for alpha, interpolate will defeat this by changing
	// masked color; OTOH, an alpha channel will rotate too,
	// so interpolation should be OK
	*img = img->Rotate(
		deg2rad(rot),
		wxPoint(img->GetWidth() / 2, img->GetHeight() / 2),
		(img->HasAlpha() || ! img->HasMask()) ? true : false
	);

	return img;
}

int
PointOnLine(long Px, long Py, long Qx, long Qy, long Tx, long Ty)
{
	long ab0 = std::labs(Qx-Px), ab1 = std::labs(Qy-Py);

	if ( std::labs((Qy-Py)*(Tx-Px)-(Ty-Py)*(Qx-Px)) >= std::max(ab0, ab1) )
		return 0;

	if ( (Qx < Px && Px < Tx) || (Qy < Py && Py < Ty) )
		return 1;
	if ( (Tx < Px && Px < Qx) || (Ty < Py && Py < Qy) )
		return 1;

	if ( (Px < Qx && Qx < Tx) || (Py < Qy && Qy < Ty) )
		return 3;
	if ( (Tx < Qx && Qx < Px) || (Ty < Qy && Qy < Py) )
		return 3;

	return 2;
}

bool
IntersectRect(const wxRect& R0, const wxRect& R1)
{
	int l0 = (R0.width >= 0) ? R0.x : (R0.x + R0.width);
	int t0 = (R0.height >= 0) ? R0.y : (R0.y + R0.height);
	int r0 = l0 + std::abs(R0.width);
	int b0 = t0 + std::abs(R0.height);
	int l1 = (R1.width >= 1) ? R1.x : (R1.x + R1.width);
	int t1 = (R1.height >= 1) ? R1.y : (R1.y + R1.height);
	int r1 = l1 + std::abs(R1.width);
	int b1 = t1 + std::abs(R1.height);

	if ( r0 < l1 || b0 < t1 || l0 > r1 || t0 > b1 )
		return false;

	return true;
}

double
FindAngle(double Px, double Py, double Qx, double Qy, double Cx, double Cy)
{
	double a, b, Pcos, Qcos;

	Px -= Cx; Qx -= Cx;
	Py -= Cy; Qy -= Cy;

	Pcos = Px / LineLength(Px, Py);
	Qcos = Qx / LineLength(Qx, Qy);

	a = std::acos(Pcos);
	b = std::acos(Qcos);

	return (Qy < 0.0) ? -(b - a) : (b - a);
}

void
FindOppositeRayEnd(double Px, double Py, double Cx, double Cy
	, double& Qx, double& Qy)
{
	double run = Px - Cx;
	double rise= Py - Cy;
	double rat = LineLength(Cx, Cy, Qx, Qy) / LineLength(Cx, Cy, Px, Py);

#	if defined(USE_FP_FMA) || defined(FP_FAST_FMA)
	Qx = fma(-run, rat, Cx);
	Qy = fma(-rise, rat, Cy);
#	else
	Qx = Cx - run * rat;
	Qy = Cy - rise * rat;
#	endif
}


or_enum
PointOnRect(const wxRect& r, const wxPoint& p)
{
	wxPoint P, Q;

	P.x = r.x; P.y = r.y; Q.x = r.x + r.width; Q.y = r.y;
	if ( PointOnSegPQ(P, Q, p) ) {
		P = Q; Q.y = r.y + r.height;
		if ( PointOnSegPQ(P, Q, p) )
			return or_en;
		P.x = r.x; P.y = r.y; Q.x = r.x; Q.y = r.y + r.height;
		if ( PointOnSegPQ(P, Q, p) )
			return or_nw;
		return or_n;
	}
	P.x = r.x + r.width; P.y = r.y;
	Q.x = r.x + r.width; Q.y = r.y + r.height;
	if ( PointOnSegPQ(P, Q, p) ) {
		P.x = r.x; P.y = r.y + r.height;
		if ( PointOnSegPQ(P, Q, p) )
			return or_se;
		return or_e;
	}
	P.x = r.x + r.width; P.y = r.y + r.height;
	Q.x = r.x; Q.y = r.y + r.height;
	if ( PointOnSegPQ(P, Q, p) ) {
		P.x = r.x; P.y = r.y;
		if ( PointOnSegPQ(P, Q, p) )
			return or_ws;
		return or_s;
	}
	P.x = r.x; P.y = r.y;
	Q.x = r.x; Q.y = r.y + r.height;
	if ( PointOnSegPQ(P, Q, p) )
		return or_w;
	return or_not;
}

#if defined(__WXMSW__) || defined(__UNIX__) || ! wxCHECK_VERSION(2, 4, 0)
// for Unix, avoid wx to be more in control
char*
gettmpnam()
{
	static char nbuf[PATH_MAX];
	const char tpl[] = "esXXXXXX";
#ifndef __WXMSW__
# ifndef _PATH_TMP
#  define _PATH_TMP "/tmp/"
# endif
#else
# ifndef _PATH_TMP
#  define _PATH_TMP "\\"
# endif
#endif

#define _SEP_STR wxs2ch(SEPS)
	
	const char* p = NULL;
	if ( is_priviliged() == false ) {
		p = std::getenv("TMPDIR");
		#ifdef __WXMSW__
		if ( p == NULL )
			p = std::getenv("TEMP");
		if ( p == NULL )
			p = std::getenv("TMP");
		#endif
	}
	// Don't use this macro: P_tmpdir use is undesirable,
	// e.g. on {Net,Free}BSD (not OpenBSD) it's /var/tmp, not /tmp
	#if defined(P_tmpdir) && 0
	if ( p == NULL )
		p = P_tmpdir;
	#endif
	if ( p == NULL )
		p = _PATH_TMP;

	std::size_t l, tl;
	l = std::strlen(p);
	tl = std::strlen(_SEP_STR);

	if ( tl <= l ) {
		if ( std::strcmp(&p[l - tl], _SEP_STR) ) {
			if ( (l + tl) >= A_SIZE(nbuf) ) {
				std::fprintf(stderr,
					"%s: internal error in gettmpnam()\n",
					wxs2ch(wxGetApp().GetAppName()));
				return NULL;
			}
			std::strcpy(nbuf, p);
			std::strcpy(&nbuf[l], _SEP_STR);
			l += tl;
		} else {
			if ( l >= A_SIZE(nbuf) ) {
				std::fprintf(stderr,
					"%s: internal error in gettmpnam()\n",
					wxs2ch(wxGetApp().GetAppName()));
				return NULL;
			}
			std::strcpy(nbuf, p);
		}
	} else if ( l > 0 ) {
		if ( (l + tl) >= A_SIZE(nbuf) ) {
			std::fprintf(stderr, "%s: internal error in gettmpnam()\n",
				wxs2ch(wxGetApp().GetAppName()));
			return NULL;
		}
		std::strcpy(nbuf, p);
		std::strcpy(&nbuf[l], _SEP_STR);
		l += tl;
	} else {
		l = 0; // use current working dir
	}

	tl = std::strlen(tpl);
	if ( (l + tl) >= A_SIZE(nbuf) ) {
		std::fprintf(stderr, "%s: internal error in gettmpnam()\n",
			wxs2ch(wxGetApp().GetAppName()));
		return NULL;
	}

	std::strcpy(&nbuf[l], tpl);
	return nbuf;
}
#endif

int
gettmpfd(char* buf, size_t len)
{
	int fd;

	// Cannot recall why this code was wanted for MSW && wx >= 2.4 !
	// Update: wxFileName::CreateTempFileName, although OK
	// under wine under Linux, is broken under real MSW!
	// (last check with wx 2.8.10; maybe check again w/ 3.0)
	#if 1 && defined(__WXMSW__) && wxCHECK_VERSION(2, 4, 0)

	char* p, * ptpl = gettmpnam();
	if ( ptpl == NULL ) {
		return -1;
	}
	p = mktemp(ptpl);
	if ( p == NULL ) {
		return -1;
	}
	fd = open(p, O_RDWR|O_CREAT|O_EXCL, 0600);

	#elif !defined(NEED_MKSTEMP) && defined(__UNIX__)

	// for Unix, avoid wx to be more in control
	char* p = gettmpnam();
	if ( p == NULL ) {
		return -1;
	}
	// be paranoid about mode: BSD manual claims mode 0600 w/ mkstemp,
	// but OpenGroup POSIX webpage for mkstemp does not mention mode.
	mode_t um = umask(0177);
	fd = mkstemp(p);
	umask(um);
	if ( fd >= 0 ) {
		fchmod(fd, 0600);
	}

	#elif wxCHECK_VERSION(2, 4, 0)

	wxFile f;
	wxString t = wxFileName::CreateTempFileName(wxT("es"), &f);

	if ( t.IsEmpty() ) {
		return -1;
	}

	fd = f.fd();
	f.Detach();
	const char* p = wxs2ch(t);

	#else

	// before wx 2.4 -- this code hasn't run in a while
	const char* p, * ptpl = gettmpnam();
	wxString tpl(ptpl), dest;
	if ( !wxGetTempFileName(wxPathOnly(tpl), dest) )
		return -1;
	p = wxs2ch(dest);
	fd = open(p, O_RDWR|O_CREAT|O_EXCL, 0600);

	#endif

	if ( fd >= 0 ) {
		if ( buf != NULL ) {
			strncpy(buf, p, len - 1);
			buf[len - 1] = '\0';
		}
	}
	return fd;
}

#ifdef __UNIX__
// There would be no good reason to run this in a priviliged account,
// but provide a test anyway, for some security conditions
// This ideally should be changed per build target appropriately,
// but the default will have to make assumptions: 1) that there
// are uid and gid thresholds below which are system creds, and above
// which are unprivileged real-human accounts, and 2) that although
// that number can't be known universally, we can at least select
// a partly helpful default
#ifndef USER_ID_MIN
#define USER_ID_MIN 100
#endif 
#ifndef GROUP_ID_MIN
#	if defined(__sun) || defined(sun)
		// OpenSolaris (OpenIndiana...) installation-user is given
		// group 'staff', gid 10
#		define GROUP_ID_MIN 10
#	else
#		define GROUP_ID_MIN 100
#	endif
#endif 
bool
is_priviliged()
{
	// we don't care if real or effective
	if ( getuid() < USER_ID_MIN || geteuid() < USER_ID_MIN )
		return true;
	if ( getgid() < GROUP_ID_MIN || getegid() < GROUP_ID_MIN )
		return true;

	return false;
}
#else
bool
is_priviliged()
{
	return false;
}
#endif // __UNIX__

// This string is the subset of ASCII accepted in POV-Ray IDs
const wxChar pov_chars_OK[] =
wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_");
// number if wxChars in above
const size_t pov_chars_OK_count = A_SIZE(pov_chars_OK) - 1;
// This string is the 1st-char prohibited set for POV-Ray IDs
const wxChar pov_char0_NG[] = wxT("0123456789");
// number if wxChars in above
const size_t pov_char0_NG_count = A_SIZE(pov_char0_NG) - 1;

// Take a wxString reference that is meant as (or to be included in)
// a POV-Ray identifier and 'sanitise' it against pov_chars_OK[] (above)
// albeit in a simplistic manner.  The result is returned in the
// (non-const) reference argument.  Return true if a change was made,
// else false
bool sanitise_for_pov(wxString& str)
{
	bool changes = false;
	wxString s_ok(pov_chars_OK);
	for ( size_t p = 0; p < str.length(); p++ ) {
		if ( s_ok.Find(str[p]) == wxNOT_FOUND ) {
			str[p] = wxT('_');
			changes = true;
		}
	}
	for ( size_t p = 0; p < pov_char0_NG_count; p++ ) {
		if ( str[0] == pov_char0_NG[p] ) {
			wxString t(wxT('_'));
			str = t + str;
			changes = true;
			break;
		}
	}
	return changes;
}

// Uses sanitise_for_pov, and if changes were made, prompts user
// with a dialog to say if they are acceptible.
// Like sanitise_for_pov the argument is modified.
// If sanitise_for_pov made changes, the return from the
// prompt dialog is returned: wxYES, or wxNO, or wxCANCEL.
// If no change was made return is wxYES, whic, IAC, means OK.
int sanitise_with_prompt(wxString& str)
{
	if ( sanitise_for_pov(str) ) {
		wxString t = _("Identifier Altered For POV-Ray");
		wxString m = _("The ID word you provided has been altered\n");
		m += _("because it contained characters that cannot\n");
		m += _("be used in an identifier in POV-Ray.\n\n");
		m += _("This is the new ID word: \"");
		m += str + _("\"\n\nIs that word acceptable?");
		
		return wxMessageBox(m, t,
			wxYES_NO | wxCANCEL | wxICON_QUESTION);
	}

	return wxYES;
}

// This checks its argument against the set of POV-Ray reserved words,
// as found in the v. 3.6 html documentation.  Those words are all
// in ASCII.
// Current implementation uses GNU gperf to store the set of words,
// and the file with the code is suitable for inclusion, regardless
// of its .cpp suffix.
// restrict to this unit with namespace.
namespace {
	using namespace std;
	// gperf uses ancient 'register' and apparently has no option to
	// desist with that; meanwhile, compilers such as clang++ complain.
#	define register
#	include "pov_reserved_words.cpp"
#	undef register
}

bool check_identifier(const char* word, size_t len)
{
	if ( word == 0 ) {
		// Either a serious coding error, or a corrupted
		// operating environment, e.g. bad RAM -- bail out.
		// (Note string is not translated)
		wxLogFatalError(wxT("A null pointer was detected in:")
			wxT(" check_identifier() (util.cpp)"));
		// wxLogFatalError is documented to exit, status 3, so
		// this line should not be reached; but, just in case . . .
		// (Does using wxLogNull disable the abort?)
		std::fputs("A null pointer was detected in:"
			" check_identifier() (util.cpp), aborting.\n", stderr);
		abort();
	}

	unsigned int l = (unsigned int)(len ? len : std::strlen(word));
	return pov_reserved_words::pov_reserved_words_hash(word, l) == 0;
}

// This version will prompt for a new string if need be, check the
// new string, and if OK assign it to the wxString refered to by "word"
// and return true, or if the user cancels then return false.
// Recurse as necessary.
// May modify 'word' even if false is returned.
bool check_identifier(wxString& word)
{
	if ( ! check_identifier(wxs2ch(word), word.length()) ) {
		wxString t(_("Identifier Matches a POV-Ray Word"));
		wxString m(_("POV-Ray reserved words cannot be used, but \""));
		m += word;
		m += _("\" is a reserved word.\nPlease provide a new word.");
		wxBell();
		wxString w2 = wxGetTextFromUser(m, t, word);
		// if user cancels the return is empty string ""
		if ( w2.length() == 0 ) {
			return false;
		}
		
		w2.Trim(true); w2.Trim(false);
		int r = sanitise_with_prompt(w2);
		if ( r == wxYES ) {
			word = w2;
		} else if ( r == wxCANCEL ) {
			return false;
		}
		
		return check_identifier(word);
	}

	wxString w2;
	int r;
	do {
		r = sanitise_with_prompt(w2 = word);
		if ( r == wxYES ) {
			if ( w2 == word ) {
				return true;
			}
			break;
		} else if ( r == wxCANCEL ) {
			return false;
		}

		// user selected 'no', get new word
		wxString t(_("New Identifier Needed"));
		wxString m(_("The suggested identifier \""));
		m += w2;
		m += _("\" was rejected.\nPlease provide a new word.");
		wxBell();

		w2 = wxGetTextFromUser(m, t, w2);
		// if user cancels the return is empty string ""
		if ( w2.length() == 0 ) {
			return false;
		}

		break;
	} while ( false );

	word = w2;
	return check_identifier(word);
}

// replace C comments "/*" and "*/" with r
std::string&
unccom_string(std::string& in, std::string& out, const char* r)
{
	std::string t(in);
	std::string::size_type p;

	while ( (p = t.find("*/")) != std::string::npos ) {
		t.replace(p, 2, r);
	}
	while ( (p = t.find("/*")) != std::string::npos ) {
		t.replace(p, 2, r);
	}

	out = t;
	return out;
}

// char string: anything that is _not_ ascii, printable or space,
// is changed to hex in the '\xNN' convention; also backslash
// "escapes" are put before '\', '"', and '\''
std::string&
sanitise_string(std::string& in, std::string& out, bool esc)
{
	std::istringstream si(in);
	std::ostringstream so;

	so << std::setbase(16) << std::uppercase << std::setfill('0');

	char c;
	while ( si.get(c) ) {
		// char and unsigned char (and signed char) are
		// different C++ types even if char is unsigned;
		// use union to get an unsigned type regardless
		// of signedness of char
		union { char c; unsigned char u; } uc;
		uc.c = c;
		// problem: std::iscntrl(c) is true for '\t' etc
		if ( c != '\t' && c != '\n' && c != '\r' ) {
			if ( uc.u > 127 || std::iscntrl(c)
			     || ! (std::isprint(c) || std::isspace(c)) ) {
				so << "\\x" << std::setw(2) << unsigned(uc.u);
				continue;
			}
		}
		if ( esc ) {
			switch ( c ) {
				case '\'':
				case '"':
				case '\\':
					so.put('\\');
					break;
				default:
					break;
			}
		}
		so.put(c);
	}

	out = so.str();

	return out;
}

std::string&
unsanitise_string(std::string& in, std::string& out, bool esc)
{
	std::istringstream si(in);
	std::ostringstream so;

	char c;
	while ( si.get(c) ) {
		if ( c == '\\' ) {
			if ( ! si.get(c) ) {
				so.put('\\');
				continue;
			}

			if ( ! esc && c != 'x' ) {
				so.put('\\');
			} else {
				char buf[3];

				if ( ! si.get(c) ) {
					so.put('x');
					continue;
				}
				if ( ! std::isxdigit(c) ) {
					so.put('x');
					so.put(c);
					continue;
				}

				buf[0] = c;

				if ( ! si.get(c) ) {
					so.put('x');
					so.put(buf[0]);
					continue;
				}
				if ( ! std::isxdigit(c) ) {
					so.put('x');
					so.put(buf[0]);
					so.put(c);
					continue;
				}

				buf[1] = c;
				buf[2] = '\0';

				unsigned us;
				std::istringstream ts(buf);

				ts >> std::setbase(16) >> us;

				union { char c; unsigned char u; } uc;

				uc.u = static_cast<unsigned char>(us);
				c = uc.c;
			}
		}

		so.put(c);
	}

	out = so.str();

	return out;
}
