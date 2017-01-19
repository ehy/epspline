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

//
// Build on Unix/POSIX systems with something like:
//	make CXXFLAGS="$CXXFLAGS $(freetype-config --cflags)" \
//		LDLIBS="$(freetype-config --libs)" t1char2pse
// (On SunOS try 'CCFLAGS=...', and so on.)
//
// This can also be built for MSW systems, although there are a few
// issues, mostly how to build and link against freetype.
// The following description of building with the cost-free
// Digital Mars `dmc' compiler might be a rough guide for other
// MSW compilers too.
//
// The dmc compiler can be downloaded at:
//	http://www.digitalmars.com/download/dmcpp.html
// Not all links are free downloads (the IDDE is worth buying)
// but the commandline C++ compiler .zip *and* ``STLport'' .zip
// are cost-free, and both are needed.
//
// The freetype headers and dll and a zlib dll can be had in
// binary packages at:
//	http://gnuwin32.sourceforge.net/packages/freetype.htm
// Get the links ``Binaries'' (freetype-bin-zip) and
// ``Dependencies'' (freetype-dep-zip)
//
// unzip freetype-bin-zip, then freetype-dep-zip. With this file
// in the same working directory where the .zips were unzipped,
// and named ``t1char2pse.cc,'' this should build it:
//	wine dmc -cpp -o+space -mn -Aa -Ab -Ar -Ae -wx -p \
//		-IC:\\dm\\stlport\\stlport \
//		-I.\\include\\freetype2 \
//		-I.\\include \
//		t1char2pse.cc \
//		.\\lib\\freetype-bcc.lib
//
// Note the '\' at line ends mean continuation: the command
// should be on one line (without '\'). Also note the file
// .\\lib\\freetype-bcc.lib on the command; the `bcc' suggests
// it is meant for the Borland compiler, but DMC will use it.
// Further note that the example is of using wine under a
// Unix system, but it should work on MSW with the obvious
// changes to the command. Additionally note that the command
// assumes DMC installed in C:\dm, ymmv. Finally note that if
// not using DMC, edit this file at the ``#if __DMC__'' line
// to add support for your tool.
//
// The needed DLLs are (from the unzipped dir)
// bin\\freetype6.dll bin\\zlib1.dll and of course they must be put
// where the loader can find them; probably the same place
// t1char2pse.exe gets put. The cost-free DMC will static-link
// the DMC C-library (at least that has been my experience).
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cerrno>
#include <clocale>

// MSWindows build.
// Add what you need for your MSW development tools.
#if defined(_WIN32) || defined(__WIN32__) || defined(_Win32) \
	|| defined(__MINGW__) || defined(__DMC__)
#	include <io.h>		// for setmode()
#	include <fcntl.h>	// for O_BINARY
#	if __DMC__
#		define snprintf _snprintf
#	endif
#	ifndef _WIN32
#		define _WIN32
#	endif
#endif
// Anything needed to detect MS; set _MSSYS_ 1
#if defined(_WIN32)
#	define _MSSYS_ 1
#endif

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#if defined(__OpenBSD__) // OBSD 'xenocara' has all in one dir
#	include <t1tables.h>
#else
#	include <freetype/t1tables.h>
#endif
} // extern "C"

// for -x high precision format option
#ifndef FMT_HIGHPREC
#	define FMT_HIGHPREC 8 // lucky number
#endif // FMT_HIGHPREC

// use mbrtowc, or mbtowc?
#ifndef USE_MBRTOWC
#	define USE_MBRTOWC 1
#endif // USE_MBRTOWC

// Unix vs. DOS, sigh
#if _MSSYS_
#	include <windows.h>
#	ifndef CP_SYMBOL // e.g., dmc
#		define CP_SYMBOL 42
#	endif
const char dirsep = '\\';
typedef UINT cpagetype;
cpagetype cur_cpage = CP_ACP;
#else // _MSSYS_
const char dirsep = '/';
typedef const char* cpagetype;
cpagetype cur_cpage = 0;
#endif // _MSSYS_

// class to be instantiated on stack in blocks that need
// a temporary change of a locale category so that dtor
// will automatically restore the original when block is
// left -- used herein for LC_NUMERIC to ensure portable
// "C" locale printf formatting of real numbers
template<int C> class C_loc_Temp {
protected:
        std::string orig;
public:
        C_loc_Temp(const char* loc = "C") {
                const char* p = std::setlocale(C, NULL);
                orig = p ? p : "";
                std::setlocale(C, loc);
        }
        ~C_loc_Temp() {
                std::setlocale(C, orig.c_str());
        }
};

typedef C_loc_Temp<LC_NUMERIC> cnumtmp;
typedef C_loc_Temp<LC_ALL>     cloctmp;


typedef double flt_t;
template <typename T> struct TplPtCoord {
	T x, y;
};

typedef TplPtCoord<flt_t> PtCoord;
struct ccont {
	typedef std::vector<PtCoord> ptvec;
	ptvec v;
	std::string comment;
	ccont() {}
	ccont(const std::string& s) : comment(s) {}
	ccont(const ccont& o) { *this = o; }
	
	ccont& operator = (const ccont& o) {
		v = o.v;
		comment = o.comment;
		return *this;
	}
};

struct iarg {
	std::string s;
	FT_ULong    v;
};

std::ostream&
prn_contour(short p0, short pN, const FT_Outline& outline,
			std::ostream& o, bool& result);
bool
get_contour(short p0, short pN, const FT_Outline& outline, ccont& o);
void
scale_pts(ccont& c, flt_t scale);
bool
collect_pts(flt_t xshift, flt_t yshift, ccont& ccr,
	const FT_Face face, const FT_GlyphSlot slot,
	const FT_Outline& outline);
void
prn_prnobj(ccont& c, unsigned obj_num);
void
prn_openf(unsigned cnt);
void
prn_comment(const std::string& out);
std::string&
sanitise_string(std::string& in, std::string& out);
void
get_envvars();
void
get_options(int ac, char* av[], std::vector<iarg>& ia);

// utility for common bits; as a template so that
// args may be PtCoord type herein, or FreeType's
// FT_Vector, both of which have members x and y
// -- types T and U are the points from which
// midpoint is taken, and R& pR is assigned result
template<class T, class U, class R> inline void
midpoint_assign(const T& p1, const U& p2, R& pR)
{
	pR.x = (p1.x + p2.x) / 2.0;
	pR.y = (p1.y + p2.y) / 2.0;
}

// similar args to template midpoint_assign(),
// this is to calculate cubic bezier control
// points from 'conic' (quadratic) bezier data
// -- pC is the quadratic control point, and
// pE is an end point -- invoke this once for
// *each* quadratic end point to get two cubic
// control points (end points do not change in
// the conversion)
template<class T, class U, class R> inline void
conic_to_cubic_cpts(const T& pC, const U& pE, R& pR)
{
	pR.x = (2.0 * pC.x + pE.x) / 3.0;
	pR.y = (2.0 * pC.y + pE.y) / 3.0;
}

std::vector<FT_Library> ftlib_vec;
extern "C" {
static void f_atexit(void)
{
	for (
		std::vector<FT_Library>::iterator b = ftlib_vec.begin();
		b != ftlib_vec.end();
		b++
	) {
		FT_Done_FreeType(*b);
	}
}
} // extern "C"

const char default_prog[] = "ttchar2pse";
const char* prog = default_prog;
const char* lc_all_found = 0; // for a locale check
// typeface file
const char* fontfile = 0;
// approx. epspline virtual x
const flt_t Xmax = 3000.0;
// prism sweep values; set with option args
double sweep_min = -1.0;
double sweep_max = 1.0;
// point fmt precision: '%g' or '%.8f'
bool extra_prec = false;
// write all args as one object; option -1
bool one_object = false;
// lots of info output: will obscure warnings and errors
bool very_verbose = false;
// include character bounding box around chars
bool box_chars = false;
// if option -1, then box the whole
bool box_allchars = false;

// although using std::cerr for messages,
// C streams are used for data output
std::FILE* out_fp;

int
main(int argc, char* argv[])
{
	if ( argv[0] && *argv[0] ) {
		std::string s(argv[0]);
		std::string::size_type p = s.rfind(dirsep);
		if ( p != std::string::npos && (p+1) < s.length() ) {
			prog = &argv[0][++p];
		} else {
			prog = argv[0];
		}
	}

#if _MSSYS_
	// diddle standard output to stop the \r on line ends
	// because, although the MSW epspline will open files
	// w/o \r, the Unix epspline fails on files with \r.
	if ( ! isatty(fileno(stdout)) ) {
		int nofd = dup(fileno(stdout));
		setmode(nofd, O_BINARY);
		if ( (out_fp = ::fdopen(nofd, "wb")) == 0 ) {
			std::cerr << "Failed to dup standard output\n"
				<< "output will have \\r\n";
			::close(nofd);
			out_fp = stdout;
		} else {
			std::fclose(stdout);
		}
	} else {
		out_fp = stdout;
	}
#else
	out_fp = stdout;
#endif

	get_envvars();

	std::setlocale(LC_ALL, "");

	std::vector<iarg> iargs;

	get_options(argc, argv, iargs);

	if ( very_verbose && lc_all_found ) {
		std::cerr << "Found LC_ALL||LANG" << " == \""
			<< lc_all_found << "\"" << std::endl;
	} else if ( very_verbose ) {
		std::cerr << "Found LC_ALL||LANG" << " not set" << std::endl;
	}

	FT_Library library;
	int error = FT_Init_FreeType(&library);
	if ( error ) {
		std::cerr << "Failed FT_Init_FreeType()\n";
		return 1;
	}

	std::atexit(f_atexit);
	ftlib_vec.push_back(library);

	FT_Face face;      /* handle to face object */
	const char* ffile = fontfile;
	error = FT_New_Face(library, ffile, 0, &face);
	if ( error == FT_Err_Unknown_File_Format ) {
		std::cerr << "Failed on \"" << ffile << 
			"\": FT_Err_Unknown_File_Format\n";
		return 1;
	} else if ( error ) {
		std::cerr << "Failed on \"" << ffile << "\": unknown error\n";
		return 1;
	}

	bool has_glyph_names = FT_HAS_GLYPH_NAMES(face) != 0;
	flt_t xshift = 0.0;
	flt_t xshift_1st = 0.0;
	flt_t yshift = face->bbox.yMax;
	bool xshift_init = false;
	std::vector<ccont> c_all;
	c_all.reserve(iargs.size());

	for ( int i = 0; i < iargs.size(); i++ ) {
		FT_ULong ftul = iargs[i].v;
		std::string& as = iargs[i].s;
	
		FT_UInt glyph_index = FT_Get_Char_Index(face, ftul);
		if ( glyph_index == 0 ) {
			std::cerr << "Found no glyph for \"" << as
				<< "\" " << ftul << std::endl;
			continue;
		}
	
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_SCALE);
		if ( error == FT_Err_Invalid_Argument ) {
			std::cerr << 
				"Error \"FT_Err_Invalid_Argument\" "
				"from FT_Load_Glyph()\n";
			return 1;
		} else if ( error ) {
			std::cerr << "Error " << error << " from FT_Load_Glyph()\n";
			return 1;
		}

		FT_GlyphSlot slot = face->glyph;

		xshift_1st = slot->metrics.horiBearingX;
		if ( !xshift_init ) {
			xshift -= xshift_1st;
			xshift_init = true;
		}

		FT_Outline& outline = slot->outline;

		if ( outline.n_contours > 0 ) {
			c_all.push_back(ccont());
			ccont& ccr = c_all.back();
			bool res = collect_pts(xshift, yshift, ccr,
				face, slot, outline);
			if ( !res ) {
				c_all.pop_back();
				std::cerr << "Failed on arg " << i <<
					": \"" << as << "\"\n";
				continue;
			}

			if ( box_chars ) {
				FT_Glyph_Metrics& gm = slot->metrics;
				flt_t mnX = xshift + gm.horiBearingX;
				flt_t mxX = mnX + gm.width;
				flt_t mxY = yshift - gm.horiBearingY + gm.height;
				flt_t mnY = mxY - gm.height;
				PtCoord pt1, pt2;
				
				pt1.x = mnX; pt1.y = mnY;
				pt2.x = mnX; pt2.y = mxY;
				ccr.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt2);
				
				pt1.x = mnX; pt1.y = mxY;
				pt2.x = mxX; pt2.y = mxY;
				ccr.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt2);
				
				pt1.x = mxX; pt1.y = mxY;
				pt2.x = mxX; pt2.y = mnY;
				ccr.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt2);
				
				pt1.x = mxX; pt1.y = mnY;
				pt2.x = mnX; pt2.y = mnY;
				ccr.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt1);
				ccr.v.push_back(pt2);
			}

			std::ostringstream so;
	
			so << "Glyph data for argument " << as << ":\n";
			if ( has_glyph_names ) {
				const FT_UInt sz = 64;
				char buf[sz];
				if ( FT_Get_Glyph_Name(face, glyph_index, buf, sz)
					 == 0 ) {
					buf[sz - 1] = '\0';
					so << " Glyph name: \"" << buf << "\"\n";
				}
			}
			so << " Number of contours: " << outline.n_contours << '\n';
			so << " Number of points: " << outline.n_points << '\n';
			so << " Number of points in POV-Ray conversion: " <<
				ccr.v.size() << '\n';
			// use double for segments so fraction will show
			// if points count is not a multiple of four
			so << " Number of segments in POV-Ray conversion: " <<
				(double(ccr.v.size()) / 4.0) << '\n';
	
			FT_Glyph_Metrics& gm = slot->metrics;
			so << " glyph metrics:\n\t" <<
				"width " << gm.width << ", height " << gm.height <<
				std::endl <<
				"\thoriBearingX " << gm.horiBearingX <<
				", horiBearingY " << gm.horiBearingY <<
				", horiAdvance " << gm.horiAdvance <<
				std::endl <<
				"\tvertBearingX " << gm.vertBearingX <<
				", vertBearingY " << gm.vertBearingY <<
				", vertAdvance " << gm.vertAdvance <<
				std::endl;
	
			std::cerr << "outline: n_points == " << outline.n_points
				<< ", n_contours == " << outline.n_contours <<
				std::endl;
		
			ccr.comment = so.str();
		}

		xshift += slot->metrics.horiAdvance;
	}

	if ( c_all.size() < 1 ) {
		std::cerr << "No valid outlines collected\n";
		return 1;
	}

	prn_openf(one_object ? 1u : c_all.size());

	flt_t scl = Xmax / xshift;
	if ( very_verbose ) {
		std::cerr << "Using scale factor " <<
			Xmax << "/" << xshift << " (" <<
			scl << ")" << std::endl;
	}

	if ( scl > 1.0 ) { // temporary: check for Y later
		scl = 1.0;
	}
	{
		ccont oneobj;
		for ( unsigned  i = 0; i < c_all.size(); i++ ) {
			if ( ! one_object ) {
				scale_pts(c_all[i], scl);
				prn_prnobj(c_all[i], i);
			} else {
				const ccont& tcc = c_all[i];
				for ( unsigned  i = 0; i < tcc.v.size(); i++ ) {
					oneobj.v.push_back(tcc.v[i]);
				}
				oneobj.comment += tcc.comment;
			}
		}
		if ( one_object ) {
			if ( box_allchars ) {
				flt_t mnX = face->bbox.xMin;
				flt_t mxX = 0.0 + xshift + xshift_1st;
;
				flt_t mnY = yshift - face->bbox.yMin;
				flt_t mxY = yshift - face->bbox.yMax;
				PtCoord pt1, pt2;
				
				pt1.x = mnX; pt1.y = mnY;
				pt2.x = mnX; pt2.y = mxY;
				oneobj.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt2);
				
				pt1.x = mnX; pt1.y = mxY;
				pt2.x = mxX; pt2.y = mxY;
				oneobj.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt2);
				
				pt1.x = mxX; pt1.y = mxY;
				pt2.x = mxX; pt2.y = mnY;
				oneobj.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt2);
				
				pt1.x = mxX; pt1.y = mnY;
				pt2.x = mnX; pt2.y = mnY;
				oneobj.v.push_back(pt1);
				midpoint_assign(pt1, pt2, pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt1);
				oneobj.v.push_back(pt2);
			}

			scale_pts(oneobj, scl);
			prn_prnobj(oneobj, 0);
		}
	}

	std::ostringstream so;
	so << "Bezier spline characters converted from \n\"" <<
		fontfile << "\"\n";
	if ( const char* p = FT_Get_Postscript_Name(face) ) {
		so << "Postscript name: " << p << '\n';
	}
	if ( face->family_name && *face->family_name ) {
		so << "Typeface family name: " << face->family_name << '\n';
	}
	if ( face->style_name && *face->style_name ) {
		so << "Typeface style name: " << face->style_name << '\n';
	}

	{
		PS_FontInfoRec fir;
		if ( FT_Get_PS_Font_Info(face, &fir) == 0 ) {
			so << "Font info version: " << fir.version << '\n';
			so << "Font info full name: " << fir.full_name << '\n';
			so << "Font info family name: " << fir.family_name << '\n';
			so << "Font info weight: " << fir.weight << '\n';
			so << "Font info italic angle: " <<
				fir.italic_angle << '\n';
			// cast char to int to ensure conversion:
			so << "Font info fixed pitch boolean: " <<
				int(fir.is_fixed_pitch) << '\n';
			so << "Font info underline_position: " <<
				fir.underline_position << '\n';
			so << "Font info underline_thickness: " <<
				fir.underline_thickness << '\n';
		}
	}

	so << "Data conversion scaling factor: " << scl << '\n';
	so << "Baseline, scaled and translated for epspline: " <<
		double(scl * yshift) << '\n';
	so << "Typeface unscaled data:\n";
	so << " bounding box: "
		<< face->bbox.xMin << ", "
		<< face->bbox.yMin << ", "
		<< face->bbox.xMax << ", "
		<< face->bbox.yMax << '\n';
	so << " units-per-em: " << face->units_per_EM << '\n';
	so << " ascender: " << face->ascender << '\n';
	so << " descender: " << face->descender << '\n';
	so << " height: " << face->height << '\n';
	so << " max_advance_width: " <<
		face->max_advance_width << '\n';
	so << " max_advance_height: " <<
		face->max_advance_height << '\n';
	so << " underline_position: " <<
		face->underline_position << '\n';
	so << " underline_thickness: " <<
		face->underline_thickness << '\n';

	std::string c("/*\n");
	std::string cm = so.str();
	prn_comment(c + sanitise_string(cm, cm) + "*/");

	return 0;
}

void
get_envvars()
{
	if ( const char* p = std::getenv("EPSPL_HIGH_PREC") ) {
		std::string s(p);
		if ( s != "no" && s != "false" && s !=  "0" ) {
			extra_prec = true;
		}
	}
	if ( const char* p = std::getenv("EPSPL_ONE_OBJECT") ) {
		std::string s(p);
		if ( s != "no" && s != "false" && s !=  "0" ) {
			one_object = true;
		}
	}
	if ( const char* p = std::getenv("EPSPL_VERY_VERBOSE") ) {
		std::string s(p);
		if ( s != "no" && s != "false" && s !=  "0" ) {
			very_verbose = true;
		}
	}
	// local env check
	if ( const char* p = std::getenv("LC_ALL") ) {
		lc_all_found = p;
	} else if ( const char* p = std::getenv("LANG") ) {
		lc_all_found = p;
	}
}

#if _MSSYS_
template <typename C> struct temp_buff {
	C* ptr;
	temp_buff<C>(size_t sz) : ptr(new C[sz]) {}
	~temp_buff<C>() { delete[] ptr; }
private:
	temp_buff<C>() {}
};

// get char string for _one_ wide char
bool
wchar2mbs(wchar_t c, std::string& outstr, cpagetype cp)
{
	char    ncbuf[12];
	wchar_t wcbuf[2];
	int len;

	wcbuf[0] = c;
	wcbuf[1] = L'\0';
	len = WideCharToMultiByte(cp, 0, wcbuf, 1,
				  ncbuf, sizeof(ncbuf), 0, 0);

	if ( len < 1 ) {
		return false;
	}

	ncbuf[len] = '\0';
	outstr = ncbuf;

	return true;
}

int
handle_string_arg(const char* s, std::vector<iarg>& ia,
	cpagetype cp)
{
	if ( ! s ) {
		return -1;
	}
	
	int lenmb = MultiByteToWideChar(cp, 0, s, -1, NULL,  0);

	// returned count includes null terminator
	if ( lenmb < 2 ) {
		return -1;
	}

	temp_buff<wchar_t> wcbuf(lenmb);
	int lenmbb = MultiByteToWideChar(cp, 0, s, -1, wcbuf.ptr, lenmb);

	if ( lenmb != lenmbb ) {
		return -1;
	}

	int num = 0;

	// exclude null terminator
	lenmbb--;

	for ( int i = 0; i < lenmbb; i++, num++ ) {
		iarg t;
		wchar_t c = wcbuf.ptr[i];

		t.v = FT_ULong(c);
		ia.push_back(t);

		std::string cs;
		if ( ! wchar2mbs(c, cs, cp) ) {
			cs = "?";
		}
		ia.back().s.append(cs);
	}

	return num;
}
#else
int
handle_string_arg(const char* s, std::vector<iarg>& ia,
	cpagetype cp)
{
	if ( ! s ) {
		return -1;
	}
	
	cloctmp loctmp(cp ? cp : "");
	int num = 0;
	size_t slen = std::strlen(s) + 1;
	
	int wr;

#if USE_MBRTOWC // || defined(__MINGW32__) || defined(__MINGW64__)
	// e.g., bug in MinGW: mbtowc fails, mbrtowc works
	std::mbstate_t state;
	std::memset(&state, 0, sizeof(state));
#else
	// init state
	wr = std::mbtowc(0, 0, slen);
	if ( wr ) {
		std::cerr << prog << ": found state-dependent mb chars\n";
	}
#endif
	
	while ( slen > 0 ) {
		wchar_t c;

#if USE_MBRTOWC
		wr = std::mbrtowc(&c, s, slen, &state);
#else
		wr = std::mbtowc(&c, s, slen);
#endif
		// returns 0 at '\0'
		if ( wr == 0 ) {
			break;
		}
		// -1 on error
		if ( wr < 0 ) {
			std::cerr << prog
				<< ": found a bad mb char at start of:\n\t\""
				<< s << "\"\n\terror: "
				<< ::strerror(errno) << std::endl;
			return -1;
		}
		// perverse return
		if ( wr > slen ) {
			std::cerr << prog
				<< ": got perverse result from start of:\n\t\""
				<< s << "\"\n\treturn from mbtowc() "
				<< wr << " greater than maximum " << slen << std::endl;
			return -1;
		}
		
		// success
		iarg t;
		t.v = FT_ULong(c);
		ia.push_back(t);
		ia.back().s.append(s, std::string::size_type(wr));
		slen -= wr;
		s += wr;
		num++;
	}
	
	return num;
}
#endif

int
handle_file_arg(const char* s, std::vector<iarg>& ia,
	cpagetype cp)
{
	std::ifstream f(s);

	if ( ! f.good() ) {
		std::cerr << "Bad file argument: " <<
			s << std::endl;
		return -1;
	}

	std::string t;
	int cnt = 0;

	while ( std::getline(f, t) ) {
		if ( t.length() < 1 ) {
			continue;
		}

		int cur = handle_string_arg(t.c_str(), ia, cp);

		if ( cur < 0 ) {
			return -1;
		} else if ( cur == 0 ) {
			continue;
		}

		cnt += cur;
	}

	return cnt;
}

void
usage(int status)
{
	std::cerr << "Usage: " << prog
	<<
	" -f file [-o file] [-1xbBvh -mM number -Carg -s string]"
	    " [unicode indices]\n"
	"\n"
	"Get curves from a type-1 or TrueType font file and convert for epspline.\n"
	"\n"
	"-f file    the font (typeface) file: required\n"
	"-o file    the output file: optional, else stdout\n"
	"-1         ('one') make all characters be one object\n"
	"-x         use extra precision reals ('%."
		    << FMT_HIGHPREC << "f' vs. '%g')\n"
	"-b         include character bounds boxes\n"
	"-B         with option -1, add box around whole object\n"
	"-v         very verbose; will obscure warnings and errors\n"
	"-m real    set minimum sweep of the prism objects\n"
	"-M real    set maximum sweep of the prism objects\n"
	"-s string  use characters from string argument\n"
	"-F file    use characters from file argument\n"
	"-C <a|m|o|s|7|8>(MSWindows) or <locale>(elsewhere)\n"
	"           set character set used for following arguments\n"
	"-h         print this usage help and succeed\n"
	"\n"
	"Arguments that are not introduced by option switches are\n"
	"taken to be unicode charmap indices and will be converted\n"
	"with strtoul(3), so a base prefix may be present.\n"
	"\n"
	"The -C option, except on MSWindows, sets the current locale\n"
	"to the given sub-argument which must be a string naming\n"
	"a valid locale or charset installed on the system; the empty\n"
	"string \"\" will reset the environmental LANG or LC_ALL.\n"
	"On MSWindows a \"codepage\" is selected per sub-argument\n"
	"a|m|o|s|7|8 (a, 'ANSI' is the default):\n"
	"a, ANSI; m, MAC; o, OEM; s, SYMBOL; 7, UTF-7; 8, UTF-8.\n"
	"The option may be repeated and applies to all following\n"
	"arguments.\n"
	"\n"
	"Arguments may appear in any order, and may be repeated;\n"
	"so, the following example works as expected:\n"
	"\n"
	"% " << prog <<
	" -f foo.pfb -s single 32 0x2018 -s\"quotes\" 0x2019 > foo-q.pse\n"
	"\n"
	;

	std::exit(status);
}

void
get_options(int ac, char* av[], std::vector<iarg>& ia)
{	
	const char* outname = 0;
	int nargs = 0;

	for ( int i = 1; i < ac; i++ ) {
		const char* p = av[i];
		if ( *p == '-' ) {
			const char* p2 = p + 1;
			if ( *p2 == '\0' ) {
				// need at least one
				std::cerr << prog << ": got '-' without character\n";
				usage(1);
			}
			while ( char cur = *p2++ ) {
				switch ( cur ) {
					case 'f':
						if ( *p2 != '\0' ) {
							fontfile = p2;
						} else if ( ++i == ac ) {
							usage(1);
						} else {
							fontfile = av[i];
						}
						p2 = ""; // stop while()
						break;
					case 'o':
						if ( *p2 != '\0' ) {
							outname = p2;
						} else if ( ++i == ac ) {
							usage(1);
						} else {
							outname = av[i];
						}
						p2 = ""; // stop while()
						break;
					case 'M':
					case 'm': {
							const char* s;
							if ( *p2 != '\0' ) {
								s = p2;
							} else if ( ++i == ac ) {
								usage(1);
							} else {
								s = av[i];
							}
							char* ep = 0;
							errno = 0;
							if ( cur == 'M' )
								sweep_max = std::strtod(s, &ep);
							else
								sweep_min = std::strtod(s, &ep);
							if ( errno ) {
								std::cerr << prog
									<< "; bad sweep argument \"" << s
									<< "\", error \""
									<< ::strerror(errno) << "\"\n";
								usage(1);
							}
							if ( ep && *ep ) {
								std::cerr << prog
									<< "; bad sweep argument \"" << s
									<< "\": characters \""
									<< ep << "\"\n";
								usage(1);
							}
						}
						p2 = ""; // stop while()
						break;
					case 'x':
						extra_prec = true;
						break;
					case 'b':
						box_chars = true;
						break;
					case 'B':
						box_allchars = true;
						break;
					case 'v':
						very_verbose = true;
						break;
					case '1':
						one_object = true;
						break;
					case 'C': {
							const char* a;
							if ( *p2 != '\0' ) {
								a = p2;
							} else if ( ++i == ac ) {
								usage(1);
							} else {
								a = av[i];
							}

						#if ! _MSSYS_
							cur_cpage = a;
						#else // ! _MSSYS_
							switch ( *a ) {
							case 'a':
								cur_cpage = CP_ACP;
								break;
							case 'm':
								cur_cpage = CP_MACCP;
								break;
							case 'o':
								cur_cpage = CP_OEMCP;
								break;
							case 's':
								cur_cpage = CP_SYMBOL;
								break;
							case '7':
								cur_cpage = CP_UTF7;
								break;
							case '8':
								cur_cpage = CP_UTF8;
								break;
							default:
								std::cerr << prog
								<< ": bad sub-argument to -C: "
								<< a << std::endl;
								usage(1);
								break;
							}
						#endif // ! _MSSYS_
						}
						p2 = ""; // stop while()
						break;
					case 's':
					case 'F': {
							const char* s;
							int n;
							if ( *p2 != '\0' ) {
								s = p2;
							} else if ( ++i == ac ) {
								usage(1);
							} else {
								s = av[i];
							}

							int (*hf)(const char*,
								  std::vector<iarg>&,
								  cpagetype);
							if ( cur == 'F' ) {
								hf = handle_file_arg;
							} else {
								hf = handle_string_arg;
							}
							if ( (n = hf(s, ia, cur_cpage)) < 1 ) {
								std::cerr << prog
								<< ": bad string argument to -s: '"
								<< s << "'" << std::endl;
								usage(1);
							}

							nargs += n;
						}
						p2 = ""; // stop while()
						break;
					case 'h':
						usage(0);
						break;
					default:
						std::cerr << "arg \"" << p << "\" no good: "
							<< ::strerror(errno) << std::endl;
						usage(1);
				}
			} // while ( cur = ... )
		} else {
			char* ep = 0;
			errno = 0;
			FT_ULong ftul = std::strtoul(p, &ep, 0);
			if ( errno ) {
				std::cerr << "arg \"" << p << "\" no good: "
					<< ::strerror(errno) << std::endl;
				usage(1);
			}
			if ( ep && *ep ) {
				std::cerr << "arg \"" << p << "\" no good, chars: "
					<< ep << std::endl;
				usage(1);
			}
			iarg t;
			t.v = ftul;
			ia.push_back(t);
			ia.back().s = p;
			nargs++;
		}
	}

	if ( fontfile == 0 || nargs == 0 ) {
		usage(1);
	}

	if ( outname ) {
		std::fclose(out_fp);
		out_fp = std::fopen(outname, "wb");
		if ( out_fp == NULL ) {
			std::cerr << prog << ":  failed opening '"
				<< outname << "' for output, error \""
				<< ::strerror(errno) << "\""
				<< std::endl;

			usage(1);
		}
	}
}

std::string&
sanitise_string(std::string& in, std::string& out)
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
		// problem: std::iscntrl(c) is true for '\t'
		if ( c != '\t' && (uc.u > 127 || std::iscntrl(c)
				|| ! (std::isprint(c) || std::isspace(c)) ) ) {
			// put double backslash!
			so << "\\\\x" << std::setw(2) << unsigned(uc.u);
			continue;
		}
		switch ( c ) {
			case '\\':
			case '"':
			/* case '\'': NOT single quote */
				so.put('\\');
				break;
			default:
				break;
		}
		so.put(c);
	}
	
	out = so.str();
	std::string::size_type p;
	while ( (p = out.find("*/")) != std::string::npos ) {
		out.replace(p, 2, "**");
	}
	while ( (p = out.find("/*")) != std::string::npos ) {
		out.replace(p, 2, "**");
	}

	return out;
}

void
scale_pts(ccont& c, flt_t scale)
{
	for ( unsigned i = 0; i < c.v.size(); i++ ) {
		c.v[i].x *= scale;
		c.v[i].y *= scale;
	}
}

// Freetype docs at
// http://www.freetype.org/freetype2/docs/glyphs/glyphs-6.html
// describes its 2nd-order, or "conic" arcs with considerable
// difference from POV-Ray's quadratic prisms or lathes.
// Conversion is for another day, and so too are TrueType fonts,
// which use the "conic" type.
// The cubic bezier curves used in Type 1 fonts should translate
// to POV-Ray beziers with little work -- in fact no mathematical
// work, just handling differing storage and other mechanics.
// Freetype docs also say cubic and conic could even be mixed,
// but that presently no font types do so.
// /* From Freetype header:
//    typedef struct  FT_Outline_
//    {
//      short       n_contours;      /* number of contours in glyph        */
//      short       n_points;        /* number of points in the glyph      */
// 
//      FT_Vector*  points;          /* the outline's points               */
//      char*       tags;            /* the points flags                   */
//      short*      contours;        /* the contour end points             */
// 
//      int         flags; /* outline masks */
// 
//    }
// FT_Outline;
// */
bool
get_contour(short p0, short pN, const FT_Outline& outline, ccont& o)
{
	if ( (pN - p0 + 1) < 3 ) {
		std::cerr << "CONTOUR with " << (pN - p0 + 1)
			<< " points\n";
		return false;
	}

	char fl0 = FT_CURVE_TAG(outline.tags[p0]);

	for ( short i = p0; i <= pN; i++ ) {
		const FT_Vector& v = outline.points[i];
		PtCoord pc;
		pc.x = v.x; pc.y = v.y;
		char fl = FT_CURVE_TAG(outline.tags[i]);

		if ( fl == FT_CURVE_TAG_ON && i > p0 ) {
			// 2 endpoints in seq. means straight line segment
			short ix0 = i - 1;
			int tag0 = FT_CURVE_TAG(outline.tags[ix0]);

			if ( tag0 == FT_CURVE_TAG_ON ) {
				const FT_Vector& v2 = outline.points[ix0];
				PtCoord pc2;

				midpoint_assign(v2, pc, pc2);

				o.v.push_back(pc2);
				o.v.push_back(pc2);
			}

			// end last seg.; POV-Ray does not do so implicitely
			o.v.push_back(pc);
		} else if ( fl == FT_CURVE_TAG_CONIC ) {
			short ix0 = i > p0 ? i - 1 : pN;
			short ix3 = i < pN ? i + 1 : p0;
			int tag0 = FT_CURVE_TAG(outline.tags[ix0]);
			int tag3 = FT_CURVE_TAG(outline.tags[ix3]);
			const FT_Vector& v0 = outline.points[ix0];
			const FT_Vector& v3 = outline.points[ix3];
			PtCoord pc0, pc1, pc2, pc3;

			fl0 = fl;

			if ( tag3 == FT_CURVE_TAG_CONIC ) {
				midpoint_assign(v3, pc, pc3);
			} else {
				pc3.x = v3.x; pc3.y = v3.y;
			}

			if ( tag0 == FT_CURVE_TAG_CONIC ) {
				midpoint_assign(v0, pc, pc0);

				conic_to_cubic_cpts(pc, pc0, pc1);

				conic_to_cubic_cpts(pc, pc3, pc2);

				o.v.push_back(pc0);
				o.v.push_back(pc1);
				o.v.push_back(pc2);

				continue;
			} else {
				pc0.x = v0.x; pc0.y = v0.y;
			}

			if ( tag3 == FT_CURVE_TAG_CONIC ) {
				conic_to_cubic_cpts(pc, pc0, pc1);

				conic_to_cubic_cpts(pc, pc3, pc2);

				o.v.push_back(pc1);
				o.v.push_back(pc2);
				o.v.push_back(pc3);

				continue;
			}

			conic_to_cubic_cpts(pc, pc0, pc1);

			conic_to_cubic_cpts(pc, pc3, pc2);

			o.v.push_back(pc1);
			o.v.push_back(pc2);

			continue;
		}

		o.v.push_back(pc);
		fl0 = fl;
	} // end for loop

	if ( fl0 == FT_CURVE_TAG_ON ) {
		int tag0 = FT_CURVE_TAG(outline.tags[p0]);

		// last was endpoint; make straight seg to 1st
		if ( tag0 == FT_CURVE_TAG_ON ) {
			const FT_Vector& v  = outline.points[pN];
			const FT_Vector& v2 = outline.points[p0];
			PtCoord pc;

			midpoint_assign(v, v2, pc);

			o.v.push_back(pc);
			o.v.push_back(pc);
		}
	}

	// POV wants first point repeated to close curve
	const FT_Vector& v = outline.points[p0];
	PtCoord pc;
	pc.x = v.x; pc.y = v.y;
	o.v.push_back(pc);

	return true;
}

std::ostream&
prn_contour(short p0, short pN, const FT_Outline& outline,
			std::ostream& o, bool& result)
{
	if ( (pN - p0) < 3 ) {
		std::cerr << "CONTOUR with " << (pN - p0) << " points\n";
		result = false;
		return o;
	}

	char fl0 = FT_CURVE_TAG(outline.tags[p0]);

	for ( short i = p0; i <= pN; i++ ) {
		const FT_Vector& v = outline.points[i];
		char fl = FT_CURVE_TAG(outline.tags[i]);
		const char* pc = 0;

		// this tag is 0; cannot bit test
		if ( fl == FT_CURVE_TAG_CONIC ) {
			pc = "/* got quadratic/conic segment point FT_CURVE_TAG_CONIC */\n";
		}
		// using eq. rather than bit tests: the constants are 1 and 2
		if ( fl == FT_CURVE_TAG_ON ) {
			pc = "/* on end-point FT_CURVE_TAG_ON follows */\n";
		}
		if ( fl == FT_CURVE_TAG_CUBIC ) {
			pc = "/* off control point FT_CURVE_TAG_CUBIC follows */\n";
		}
		// should not have both bits
		if ( fl == (FT_CURVE_TAG_ON | FT_CURVE_TAG_CUBIC) ) {
			pc = "/* error, got both FT_CURVE_TAG_ON | FT_CURVE_TAG_CUBIC */\n";
		}

		o << pc << "<" << v.x << ", " << v.y << ">\n";

		fl0 = fl;
	}

	result = true;
	return o;
}

void
prn_openf(unsigned cnt)
{
	fprintf(out_fp, 
	" 'FileData'('FileFormatVersion' = 2,\n"
	"   'ObjectCount' = %u,\n"
	"   'HGuideCount' = 0,\n"
	"   'VGuideCount' = 0).\n"
	" \n"
	, cnt);
}

void
prn_comment(const std::string& out)
{
	fprintf(out_fp, 
	" 'FileComments'('CommentCount' = 1,\n"
	"   'Comment1' = \"%s\").\n"
	" \n"
	, out.c_str());
}

void
prn_prnobj(ccont& c, unsigned obj_num)
{
	cnumtmp c_num;

	std::string cm("/*\n");
	cm += sanitise_string(c.comment, c.comment) + "*/";

	std::fprintf(out_fp, 
	" 'Object%u'(objname = \"SplineObject\",\n"
	"   splinetype = 3,\n"
	"   povtype = 1,\n"
	"   sweeptype = 0,\n"
	"   use_sturm = 0,\n"
	"   use_open = 0,\n"
	"   sweep_min = \"%f\",\n"
	"   sweep_max = \"%f\",\n"
	"   'UserStr' = \"%s\",\n"
	, obj_num, sweep_min, sweep_max, cm.c_str());

	unsigned nuv = 0;
	if ( extra_prec ) {
		const int prec = FMT_HIGHPREC;

		for ( ; nuv < c.v.size(); nuv++ ) {
			std::fprintf(out_fp,
				"  'U%u' = \"%.*f\",\n",
				nuv, prec, double(c.v[nuv].x));
			std::fprintf(out_fp,
				"  'V%u' = \"%.*f\",\n",
				nuv, prec, double(c.v[nuv].y));
		}
	} else {
		for ( ; nuv < c.v.size(); nuv++ ) {
			std::fprintf(out_fp,
				"  'U%u' = \"%g\",\n", nuv, double(c.v[nuv].x));
			std::fprintf(out_fp,
				"  'V%u' = \"%g\",\n", nuv, double(c.v[nuv].y));
		}
	}

	std::fprintf(out_fp, "  'UVcount' = %u).\n\n", nuv);
}

bool
collect_pts(flt_t xshift, flt_t yshift, ccont& ccr,
	const FT_Face face, const FT_GlyphSlot slot,
	const FT_Outline& outline)
{
	// Sanity check:
	if ( outline.n_points
	 != (outline.contours[outline.n_contours-1]+1) ) {
		std::cerr << "First sanity check FAILED\n";
		return false;
	}

	ccr.v.reserve(2u * outline.n_points);

	short p0 = 0;
	for ( short c0 = 0; c0 < outline.n_contours; c0++ ) {
		short pN = outline.contours[c0];

		bool tbool = get_contour(p0, pN, outline, ccr);
		if ( tbool == false ) {
			std::cerr << "FAILED on contour: " << c0 << std::endl;
			return false;
		}

		if ( very_verbose ) {
			prn_contour(p0, pN, outline, std::cerr, tbool);
		}

		p0 = pN + 1;
	}

	for ( size_t i = 0; i < ccr.v.size(); i++ ) {
		ccr.v[i].x += xshift;
		ccr.v[i].y = yshift - ccr.v[i].y;
	}
	
	return true;
}
