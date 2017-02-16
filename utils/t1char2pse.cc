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
//  make CXXFLAGS="$CXXFLAGS $(freetype-config --cflags)" \
//      LDLIBS="$(freetype-config --libs)" t1char2pse
// (On SunOS try 'CCFLAGS=...', and so on.)
//
// This can also be built for MSW systems, although there are a few
// issues, mostly how to build and link against freetype.
// The following description of building with the cost-free
// Digital Mars `dmc' compiler might be a rough guide for other
// MSW compilers too.
//
// The dmc compiler can be downloaded at:
//  http://www.digitalmars.com/download/dmcpp.html
// Not all links are free downloads (the IDDE is worth buying)
// but the commandline C++ compiler .zip *and* ``STLport'' .zip
// are cost-free, and both are needed.
//
// The freetype headers and dll and a zlib dll can be had in
// binary packages at:
//  http://gnuwin32.sourceforge.net/packages/freetype.htm
// Get the links ``Binaries'' (freetype-bin-zip) and
// ``Dependencies'' (freetype-dep-zip)
//
// unzip freetype-bin-zip, then freetype-dep-zip. With this file
// in the same working directory where the .zips were unzipped,
// and named ``t1char2pse.cc,'' this should build it:
//  wine dmc -cpp -o+space -mn -Aa -Ab -Ar -Ae -wx -p \
//      -IC:\\dm\\stlport\\stlport \
//      -I.\\include\\freetype2 \
//      -I.\\include \
//      t1char2pse.cc \
//      .\\lib\\freetype-bcc.lib
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
#include <cmath>
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
#   include <io.h>      // for setmode()
#   include <fcntl.h>   // for O_BINARY
#   if __DMC__
#       define snprintf _snprintf
#   endif
#   ifndef _WIN32
#       define _WIN32
#   endif
#endif
// Anything needed to detect MS; set _MSSYS_ 1
#if defined(_WIN32)
#   define _MSSYS_ 1
#endif

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#if defined(__OpenBSD__) || defined(__NetBSD__)
#   include <t1tables.h>
#else
#   include <freetype/t1tables.h>
#endif
} // extern "C"

// for -x high precision format option
#ifndef FMT_HIGHPREC
#   define FMT_HIGHPREC 8 // lucky number
#endif // FMT_HIGHPREC

// use mbrtowc, or mbtowc?
#ifndef USE_MBRTOWC
#   define USE_MBRTOWC 1
#endif // USE_MBRTOWC

// Unix vs. DOS, sigh
#if _MSSYS_
#   include <windows.h>
#   ifndef CP_SYMBOL // e.g., dmc
#       define CP_SYMBOL 42
#   endif
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

// what to use as floating point reals
typedef double flt_t;

// for pairs name x and y
template <typename T> struct TplPtCoord {
    T x, y;

    // note comparison is for _same arguments_ rather than
    // equivalence within significant bits
    bool operator == (const TplPtCoord<T>& o) {
        return x == o.x && y == o.y;
    }

    bool operator != (const TplPtCoord<T>& o) {
        return ! (*this == o);
    }
};

// x,y pair
typedef TplPtCoord<flt_t> pair_xy;

// args -p, -P: left, right, top, bottom
template <typename T> struct TplPadOpts {
    T l, r, t, b;

    typedef T dtype;

    TplPadOpts() : l(0), r(0), t(0), b(0) {}
};

// type for args -p, -P: left, right, top, bottom
typedef TplPadOpts<flt_t> pad_opts;
const char pad_opts_sep = ':';

// curve container
struct ccont {
    typedef std::vector<pair_xy> ptvec;
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

// interactive argument
struct iarg {
    std::string s; // copy or indication of original arg
    FT_ULong    v; // value of argument e.g. UTC-16 code
};

std::ostream&
prn_contour(int p0, int pN, const FT_Outline& outline,
            std::ostream& o, bool& result, std::ostream& errstream);
bool
get_contour(int p0, int pN, const FT_Outline& outline,
            ccont& o, std::ostream& errstream);
void
scale_pts(ccont& c, flt_t scale);
bool
collect_pts(flt_t xshift, flt_t yshift, ccont& ccr,
    const FT_Face face, const FT_GlyphSlot slot,
    const FT_Outline& outline, std::ostream& errstream);
// put a cubic bezier line into container
void
add_bezier_line(const pair_xy& pt0, // 1st end point
                const pair_xy& pt3, // 2nd end point
                std::vector<pair_xy>& v);
// put a cubic bezier box into container
void
add_bezier_box(flt_t minX, // 'left' x
               flt_t maxX, // 'right' x
               flt_t minY, // 'top' y
               flt_t maxY, // 'bottom' y
               std::vector<pair_xy>& v);
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
get_options(int ac, char* av[], std::vector<iarg>& ia,
            std::ostream& errstream);

// utility for common bits; as a template so that
// args may be pair_xy type herein, or FreeType's
// FT_Vector, both of which have members x and y
template<class T, class U> inline void
point_assign(const T& src, U& dest)
{
    dest.x = src.x;
    dest.y = src.y;
}

// -- types T and U are the points from which
// midpoint is taken, and R& pR is assigned result
template<class T, class U, class R> inline void
midpoint_assign(const T& p1, const U& p2, R& pR)
{
    pR.x = (p1.x + p2.x) / 2.0;
    pR.y = (p1.y + p2.y) / 2.0;
}

// -- types T and U are the points from which
// intermediate n/m point is taken, and R& pR is assigned result
template<class T, class U, class R> void
midpoint_factor_assign(const T& p1, const U& p2, R& pR, double f)
{
    double dx = double(p2.x) - p1.x;
    double dy = double(p2.y) - p1.y;
    double ho = std::sqrt(dx * dx + dy * dy);
    double hn = ho * f;
    double an = ho == 0.0 ? 0.0 : std::acos(std::abs(dx) / ho);
    double xn = hn * std::cos(an);
    double yn = hn * std::sin(an);

    pR.x = p1.x + (dx < 0.0 ? -xn : xn);
    pR.y = p1.y + (dy < 0.0 ? -yn : yn);
}

// -- types T and U are the points from which
// intermediate 1/3 point is taken, and R& pR is assigned result
template<class T, class U, class R> inline void
midpoint_1_3rd_assign(const T& p1, const U& p2, R& pR)
{
    const double f = 1.0 / 3.0;
    midpoint_factor_assign(p1, p2, pR, f);
}

// -- types T and U are the points from which
// intermediate 2/3 point is taken, and R& pR is assigned result
template<class T, class U, class R> inline void
midpoint_2_3rd_assign(const T& p1, const U& p2, R& pR)
{
    const double f = 2.0 / 3.0;
    midpoint_factor_assign(p1, p2, pR, f);
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
// padding, option -p
pad_opts padding_chars;
// padding, option -P
pad_opts padding_all;

// although using std::cerr for messages,
// C streams are used for data output
std::FILE* out_fp;

int
main(int argc, char* argv[])
{
    std::ostream& errstream = std::cerr;

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
            errstream << "Failed to dup standard output\n"
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

    get_options(argc, argv, iargs, errstream);

    if ( very_verbose && lc_all_found ) {
        errstream << "Found LC_ALL||LANG" << " == \""
            << lc_all_found << "\"" << std::endl;
    } else if ( very_verbose ) {
        errstream << "Found LC_ALL||LANG" << " not set" << std::endl;
    }

    FT_Library library;
    int error = FT_Init_FreeType(&library);
    if ( error ) {
        errstream << "Failed FT_Init_FreeType()\n";
        return 1;
    }

    std::atexit(f_atexit);
    ftlib_vec.push_back(library);

    FT_Face face;      /* handle to face object */
    const char* ffile = fontfile;
    error = FT_New_Face(library, ffile, 0, &face);
    if ( error == FT_Err_Unknown_File_Format ) {
        errstream << "Failed on \"" << ffile << 
            "\": FT_Err_Unknown_File_Format\n";
        return 1;
    } else if ( error ) {
        errstream << "Failed on \"" << ffile << "\": unknown error\n";
        return 1;
    }

    bool has_glyph_names = FT_HAS_GLYPH_NAMES(face) != 0;
    flt_t xshift = 0.0;
    flt_t xshift_1st = 0.0;
    flt_t yshift = face->bbox.yMax;
    bool xshift_init = false;
    // curves (in container structs)
    std::vector<ccont> c_all;
    c_all.reserve(iargs.size());

    for ( int i = 0; i < iargs.size(); i++ ) {
        FT_ULong ftul = iargs[i].v;
        std::string& as = iargs[i].s;
    
        FT_UInt glyph_index = FT_Get_Char_Index(face, ftul);
        if ( glyph_index == 0 ) {
            errstream << "Found no glyph for \"" << as
                << "\" 0x" << std::hex << ftul
                           << std::dec << std::endl;
            continue;
        }
    
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_SCALE);
        if ( error == FT_Err_Invalid_Argument ) {
            errstream << 
                "Error \"FT_Err_Invalid_Argument\" "
                "from FT_Load_Glyph()\n";
            return 1;
        } else if ( error ) {
            errstream << "Error " << error << " from FT_Load_Glyph()\n";
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
                face, slot, outline, errstream);
            if ( !res ) {
                c_all.pop_back();
                errstream << "Failed on arg " << i <<
                    ": \"" << as << "\"\n";
                continue;
            }

            if ( box_chars ) {
                FT_Glyph_Metrics& gm = slot->metrics;
                flt_t mnX = xshift + gm.horiBearingX;
                flt_t mxX = mnX + gm.width;
                flt_t mxY = yshift - gm.horiBearingY + gm.height;
                flt_t mnY = mxY - gm.height;
                
                mnX -= padding_chars.l;
                mxX += padding_chars.r;
                mxY += padding_chars.b;
                mnY -= padding_chars.t;
                
                add_bezier_box(mnX, mxX, mnY, mxY, ccr.v);
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
    
            errstream << "outline: n_points == " << outline.n_points
                << ", n_contours == " << outline.n_contours <<
                std::endl;
        
            ccr.comment = so.str();
        }

        xshift += slot->metrics.horiAdvance;
    }

    if ( c_all.size() < 1 ) {
        errstream << "No valid outlines collected\n";
        return 1;
    }

    prn_openf(one_object ? 1u : c_all.size());

    flt_t scl = Xmax / xshift;
    if ( very_verbose ) {
        errstream << "Using scale factor " <<
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
                // note we draw chars inverted, hence y min/max swapped
                flt_t mnY = yshift - face->bbox.yMax;
                flt_t mxY = yshift - face->bbox.yMin;
                
                if ( very_verbose ) {
                    errstream << "Box all defaults:\n    "
                              << "minX == " << mnX
                              << ", maxX == " << mxX
                              << ", minY == " << mnY
                              << ", maxX == " << mxY << std::endl;
                }

                mnX -= padding_all.l;
                mxX += padding_all.r;
                mnY -= padding_all.t;
                mxY += padding_all.b;

                if ( very_verbose ) {
                    errstream << "Box all with user padding:\n    "
                              << "minX == " << mnX
                              << ", maxX == " << mxX
                              << ", minY == " << mnY
                              << ", maxX == " << mxY << std::endl;
                }

                add_bezier_box(mnX, mxX, mnY, mxY, oneobj.v);
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

// put a cubic bezier line into container
void
add_bezier_line(const pair_xy& pt0,
                const pair_xy& pt3,
                std::vector<pair_xy>& v)
{
    pair_xy pt1, pt2;

    midpoint_1_3rd_assign(pt0, pt3, pt1);
    midpoint_2_3rd_assign(pt0, pt3, pt2);

    v.push_back(pt0);
    v.push_back(pt1);
    v.push_back(pt2);
    v.push_back(pt3);
}

// put a cubic bezier box into container
void
add_bezier_box(flt_t mnX,
               flt_t mxX,
               flt_t mnY,
               flt_t mxY,
               std::vector<pair_xy>& v)
{
    pair_xy pt0, pt3;
    
    pt0.x = mnX; pt0.y = mnY;
    pt3.x = mnX; pt3.y = mxY;
    add_bezier_line(pt0, pt3, v);
    
    pt0.x = mnX; pt0.y = mxY;
    pt3.x = mxX; pt3.y = mxY;
    add_bezier_line(pt0, pt3, v);
    
    pt0.x = mxX; pt0.y = mxY;
    pt3.x = mxX; pt3.y = mnY;
    add_bezier_line(pt0, pt3, v);
    
    pt0.x = mxX; pt0.y = mnY;
    pt3.x = mnX; pt3.y = mnY;
    add_bezier_line(pt0, pt3, v);
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
    std::ostream& errstream, cpagetype cp)
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
    std::ostream& errstream, cpagetype cp)
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
        errstream << prog << ": found state-dependent mb chars\n";
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
            errstream << prog
                << ": found a bad mb char at start of:\n\t\""
                << s << "\"\n\terror: "
                << ::strerror(errno) << std::endl;
            return -1;
        }
        // perverse return
        if ( wr > slen ) {
            errstream << prog
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
    std::ostream& errstream, cpagetype cp)
{
    std::ifstream f(s);

    if ( ! f.good() ) {
        errstream << "Bad file argument: " <<
            s << std::endl;
        return -1;
    }

    std::string t;
    int cnt = 0;

    while ( std::getline(f, t) ) {
        if ( t.length() < 1 ) {
            continue;
        }

        int cur = handle_string_arg(t.c_str(), ia, errstream, cp);

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
usage(std::ostream& errstream, int status)
{
    errstream << "Usage: " << prog
    <<
    " -f file [options] [unicode indices]\n"
    "\n"
    "Get curves from an outline font file and convert for epspline.\n"
    "\n"
    "-f file    the font (typeface) file: required\n"
    "-o file    the output file: optional, else stdout\n"
    "-1         ('one') make all characters be one object\n"
    "-x         use extra precision reals ('%."
            << FMT_HIGHPREC << "f' vs. '%g')\n"
    "-b         include character bounds boxes\n"
    "-B         with option -1, add box around whole object\n"
    "-p l:r:t:b with option -b, padding inside character boxes\n"
    "-P l:r:t:b with options -1B, padding inside whole object box\n"
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

bool
do_padding_arg(const char* arg, pad_opts& o)
{
    pad_opts::dtype last = 0; // Init for SunCC warning
    std::string s(arg);

    for ( unsigned i = 0; i < 4; i++ ) {
        std::string::size_type pos = s.find(pad_opts_sep);
        std::string cur(s.substr(0, pos));
        pad_opts::dtype v;
        
        // last sub-arg given
        if ( pos == std::string::npos ) {
            if ( s.empty() ) {
                if ( i == 0 ) {
                    // did not get one!
                    return false;
                }
                // had one, repeat last
                v = last;
            } else {
                float f;
                char c;
                
                if ( std::sscanf(s.c_str(), "%f%c", &f, &c) != 1 ) {
                    return false;
                }

                v = pad_opts::dtype(f);
                last = v; // record last
                s = ""; // lose old value
            }
       } else {
        // sep found, must be twixt sub-args
            s = s.substr(pos + 1);
            
            // empty part is error
            if ( cur.empty() || s.empty() ) {
                return false;
            }
            
            float f;
            char c;
            
            if ( std::sscanf(cur.c_str(), "%f%c", &f, &c) != 1 ) {
                return false;
            }
                
            v = pad_opts::dtype(f);
            last = v;
        }
        
        if ( i == 0 ) {
            o.l = v;
        } else if ( i == 1 ) {
            o.r = v;
        } else if ( i == 2 ) {
            o.t = v;
        } else if ( i == 3 ) {
            o.b = v;
        } else {
            // WTF
            return false;
        }
    }

    // after loop string should be empty, or there had been extras
    if ( ! s.empty() ) {
        return false;
    }
    
    return true;
}

void
get_options(int ac, char* av[], std::vector<iarg>& ia,
            std::ostream& errstream)
{   
    const char* outname = 0;
    int nargs = 0;

    for ( int i = 1; i < ac; i++ ) {
        const char* p = av[i];
        if ( *p == '-' ) {
            const char* p2 = p + 1;
            if ( *p2 == '\0' ) {
                // need at least one
                errstream << prog << ": got '-' without character\n";
                usage(errstream, 1);
            }
            while ( char cur = *p2++ ) {
                switch ( cur ) {
                    case 'f':
                        if ( *p2 != '\0' ) {
                            fontfile = p2;
                        } else if ( ++i == ac ) {
                            usage(errstream, 1);
                        } else {
                            fontfile = av[i];
                        }
                        p2 = ""; // stop while()
                        break;
                    case 'o':
                        if ( *p2 != '\0' ) {
                            outname = p2;
                        } else if ( ++i == ac ) {
                            usage(errstream, 1);
                        } else {
                            outname = av[i];
                        }
                        p2 = ""; // stop while()
                        break;
                    case 'P':
                    case 'p': {
                            const char* s;
                            if ( *p2 != '\0' ) {
                                s = p2;
                            } else if ( ++i == ac ) {
                                usage(errstream, 1);
                            } else {
                                s = av[i];
                            }
                            bool ret;
                            ret = do_padding_arg(s, cur == 'P'
                                                    ? padding_all
                                                    : padding_chars);
                            if ( ! ret ) {
                                usage(errstream, 1);
                            }
                        }
                        p2 = ""; // stop while()
                        break;
                    case 'M':
                    case 'm': {
                            const char* s;
                            if ( *p2 != '\0' ) {
                                s = p2;
                            } else if ( ++i == ac ) {
                                usage(errstream, 1);
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
                                errstream << prog
                                    << "; bad sweep argument \"" << s
                                    << "\", error \""
                                    << ::strerror(errno) << "\"\n";
                                usage(errstream, 1);
                            }
                            if ( ep && *ep ) {
                                errstream << prog
                                    << "; bad sweep argument \"" << s
                                    << "\": characters \""
                                    << ep << "\"\n";
                                usage(errstream, 1);
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
                                usage(errstream, 1);
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
                                errstream << prog
                                << ": bad sub-argument to -C: "
                                << a << std::endl;
                                usage(errstream, 1);
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
                                usage(errstream, 1);
                            } else {
                                s = av[i];
                            }

                            int (*hf)(const char*,
                                  std::vector<iarg>&,
                                  std::ostream&,
                                  cpagetype);
                            if ( cur == 'F' ) {
                                hf = handle_file_arg;
                            } else {
                                hf = handle_string_arg;
                            }
                            if ( (n = hf(s, ia, errstream, cur_cpage))
                                  < 1 ) {
                                errstream << prog
                                << ": bad string argument to -s: '"
                                << s << "'" << std::endl;
                                usage(errstream, 1);
                            }

                            nargs += n;
                        }
                        p2 = ""; // stop while()
                        break;
                    case 'h':
                        usage(errstream, 0);
                        break;
                    default:
                        errstream << "arg \"" << p << "\" no good: "
                            << ::strerror(errno) << std::endl;
                        usage(errstream, 1);
                }
            } // while ( cur = ... )
        } else {
            char* ep = 0;
            errno = 0;
            FT_ULong ftul = std::strtoul(p, &ep, 0);
            if ( errno ) {
                errstream << "arg \"" << p << "\" no good: "
                    << ::strerror(errno) << std::endl;
                usage(errstream, 1);
            }
            if ( ep && *ep ) {
                errstream << "arg \"" << p << "\" no good, chars: "
                    << ep << std::endl;
                usage(errstream, 1);
            }
            iarg t;
            t.v = ftul;
            ia.push_back(t);
            ia.back().s = p;
            nargs++;
        }
    }

    if ( fontfile == 0 || nargs == 0 ) {
        usage(errstream, 1);
    }

    if ( outname ) {
        std::fclose(out_fp);
        out_fp = std::fopen(outname, "wb");
        if ( out_fp == NULL ) {
            errstream << prog << ":  failed opening '"
                << outname << "' for output, error \""
                << ::strerror(errno) << "\""
                << std::endl;

            usage(errstream, 1);
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
// describe the manner in which interpolated curve points
// are presented by FreeType.  Unsurprisingly, points do not
// occur in the precise manner that POV-Ray (or Epspline)
// expects.
//
// Here, points for a single continuous contour -- which might
// only be a sub-contour of a whole glyph -- are collected
// with adjustment to POV-Ray suitable sequences and placed
// in a standard vector.
//
// Note that the conic (quadratic) type from TTF faces, as
// presented by FreeType, do not correspond directly to the
// quadratic interpolation performed by POV-Ray.  Therefore,
// the single conic control point is converted to two
// cubic points making the equivalent bezier cubic segment.
//
// Implicit straight lines are also converted to bezier cubics.
// Bottom line: all output of this procedure is in bezier
// cubic form, with points that are implicit in the source
// made explicit in the result, as required by POV-Ray.
//
// Freetype docs also say cubic and conic could even be mixed,
// but that presently no font types do so.  This code *should*
// handle that, but has not been tested for that.
bool
get_contour(int p0, int pN, const FT_Outline& outline,
            ccont& o, std::ostream& errstream)
{
    if ( very_verbose ) {
        errstream << "NEW CONTOUR with "
            << (pN - p0 + 1) << " points" << std::endl;
    }

    if ( (pN - p0 + 1) < 3 ) {
        errstream << "CONTOUR with only "
            << (pN - p0 + 1) << " points" << std::endl;
        return false;
    }

    ccont::ptvec::size_type ovb, ove;

    ovb = o.v.size();

    for ( int i = p0; i <= pN; i++ ) {
        int ix0 = i > p0 ? i - 1 : pN;
        int ix3 = i < pN ? i + 1 : p0;
        int tag  = FT_CURVE_TAG(outline.tags[i]);
        int tag0 = FT_CURVE_TAG(outline.tags[ix0]);
        int tag3 = FT_CURVE_TAG(outline.tags[ix3]);
        const FT_Vector& v  = outline.points[i];
        const FT_Vector& v0 = outline.points[ix0];
        const FT_Vector& v3 = outline.points[ix3];
        pair_xy pc0, pc1, pc2, pc3;

        if ( tag == FT_CURVE_TAG_ON ) {
            // implicit straight line?
            // 2 endpoints in seq. means straight line segment
            if ( tag3 == FT_CURVE_TAG_ON ) {
                point_assign(v, pc0);
                point_assign(v3, pc3);
                add_bezier_line(pc0, pc3, o.v);
           }
        } else if ( tag == FT_CURVE_TAG_CUBIC ) {
            // if next is tag cubic, then previous,
            // current, next, and next plus 1 make
            // a cubic segment
            if ( tag3 == FT_CURVE_TAG_CUBIC ) {
                point_assign(v0, pc0);
                point_assign(v, pc1);
                point_assign(v3, pc2);

                int ix = ix3 == pN ? p0 : ix3 + 1;

                const FT_Vector& vt = outline.points[ix];
                point_assign(vt, pc3);

                o.v.push_back(pc0);
                o.v.push_back(pc1);
                o.v.push_back(pc2);
                o.v.push_back(pc3);
            }
        } else if ( tag == FT_CURVE_TAG_CONIC ) {
            if ( tag0 == FT_CURVE_TAG_CONIC ) {
                midpoint_assign(v0, v, pc0);
            } else {
                point_assign(v0, pc0);
            }

            if ( tag3 == FT_CURVE_TAG_CONIC ) {
                midpoint_assign(v3, v, pc3);
            } else {
                point_assign(v3, pc3);
            }

            conic_to_cubic_cpts(v, pc0, pc1);

            conic_to_cubic_cpts(v, pc3, pc2);

            o.v.push_back(pc0);
            o.v.push_back(pc1);
            o.v.push_back(pc2);
            o.v.push_back(pc3);
        } else { // WTF
            errstream << "Error: point indexed at " << i
                      << " has unexpected curve tag "
                      << tag << std::endl;
            return false;
        }
    } // end for loop

    ove = o.v.size();

    // should never happen -- need at least
    // 4 points, one cubic segment, for contour
    if ( (ove - ovb) < 4 ) {
        return false;
    }

    // fixup: add closing line, as needed --
    // should not happen
    if ( o.v[--ove] != o.v[ovb] ) {
        pair_xy pc0, pc3;

        if ( very_verbose ) {
            errstream << "Fixing contour with closing segment ("
                      << ove << " -> " << ovb << ")"
                      << std::endl;
        }

        point_assign(o.v[ove], pc0);
        point_assign(o.v[ovb], pc3);
        add_bezier_line(pc0, pc3, o.v);
    }

    return true;
}

std::ostream&
prn_contour(int p0, int pN, const FT_Outline& outline,
            std::ostream& o, bool& result, std::ostream& errstream)
{
    if ( (pN - p0) < 3 ) {
        errstream << "CONTOUR with " << (pN - p0) << " points\n";
        result = false;
        return o;
    }

    for ( int i = p0; i <= pN; i++ ) {
        const FT_Vector& v = outline.points[i];
        int tag = FT_CURVE_TAG(outline.tags[i]);
        std::ostringstream s;

        // this tag is 0; cannot bit test
        if ( tag == FT_CURVE_TAG_CONIC ) {
            s << "/* off control point FT_CURVE_TAG_CONIC follows */\n";
        } else if ( tag == FT_CURVE_TAG_ON ) {
            s << "/* on end-point FT_CURVE_TAG_ON follows */\n";
        } else if ( tag == FT_CURVE_TAG_CUBIC ) {
            s << "/* off control point FT_CURVE_TAG_CUBIC follows */\n";
        } else {
            // unexpected (newer version of freetype?)
            s << "/* ERROR: got unexpected curve tag == "
              << tag << " */\n";
        }

        o << s.str() << "<" << v.x << ", " << v.y << ">\n";
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
    const FT_Outline& outline, std::ostream& errstream)
{
    // Sanity check:
    if ( outline.n_points
     != (outline.contours[outline.n_contours-1]+1) ) {
        errstream << "First sanity check FAILED\n";
        return false;
    }

    ccr.v.reserve(2u * outline.n_points);

    int p0 = 0;
    for ( int c0 = 0; c0 < outline.n_contours; c0++ ) {
        int pN = outline.contours[c0];

        bool tbool = get_contour(p0, pN, outline, ccr, errstream);
        if ( tbool == false ) {
            errstream << "FAILED on contour: " << c0 << std::endl;
            return false;
        }

        if ( very_verbose ) {
            prn_contour(p0, pN, outline, errstream, tbool, errstream);
        }

        p0 = pN + 1;
    }

    for ( size_t i = 0; i < ccr.v.size(); i++ ) {
        ccr.v[i].x += xshift;
        ccr.v[i].y = yshift - ccr.v[i].y;
    }
    
    return true;
}
