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
// build with something like:
//	make CXXFLAGS="$CXXFLAGS $(freetype-config --cflags)" \
//		LDLIBS="$(freetype-config --libs)" t1char2pse
// (On SunOS try 'CCFLAGS=...', and so on.)
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cerrno>

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/t1tables.h>
}

using namespace std;

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

ostream&
prn_contour(short p0, short pN, const FT_Outline& outline, ostream& o, bool& result);
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

// approx. epspline virtual x
const flt_t Xmax = 3000.0;

int
main(int argc, char* argv[])
{
	if ( argc < 3 ) {
		cerr << "need 2 args: font-file, and glyph-index\n";
		return 1;
	}

	FT_Library library;
	int error = FT_Init_FreeType( &library );
	if ( error ) {
		cerr << "Failed FT_Init_FreeType()\n";
		return 1;
	}

	std::atexit(f_atexit);
	ftlib_vec.push_back(library);

	FT_Face face;      /* handle to face object */
	const char* ffile = argv[1];
	error = FT_New_Face(library, ffile, 0, &face);
	if ( error == FT_Err_Unknown_File_Format ) {
		cerr << "Failed on \"" << ffile << 
			"\": FT_Err_Unknown_File_Format\n";
		return 1;
	} else if ( error ) {
		cerr << "Failed on \"" << ffile << "\": unknown error\n";
		return 1;
	}

	bool has_glyph_names = FT_HAS_GLYPH_NAMES(face) != 0;
	flt_t xshift = 0.0;
	flt_t yshift = face->bbox.yMax;
	bool xshift_init = false;
	vector<ccont> c_all;
	c_all.reserve(argc - 2);

	for ( int i = 2; i < argc; i++ ) {
		errno = 0;
		FT_ULong ftul = strtoul(argv[i], 0, 0);
		if ( errno ) {
			cerr << "arg \"" << argv[i] << "\" no good: "
				<< strerror(errno) << endl;
			continue;
		}
	
		FT_UInt glyph_index = FT_Get_Char_Index(face, ftul);
		if ( glyph_index == 0 ) {
			cerr << "Found no glyph for \"" << argv[i]
				<< "\" (" << ftul <<
			")\n";
			continue;
		}
	
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_SCALE);
		if ( error == FT_Err_Invalid_Argument ) {
			cerr << 
				"Error \"FT_Err_Invalid_Argument\" "
				"from FT_Load_Glyph()\n";
			return 1;
		} else if ( error ) {
			cerr << "Error " << error << " from FT_Load_Glyph()\n";
			return 1;
		}

		FT_GlyphSlot slot = face->glyph;

		if ( !xshift_init ) {
			xshift -= slot->metrics.horiBearingX;
			xshift_init = true;
		}

		std::string sarg(argv[i]);
		std::ostringstream so;

		so << "Glyph data for argument " << sarg << ":\n";
		if ( has_glyph_names ) {
			const FT_UInt sz = 512;
			char buf[sz];
			if ( FT_Get_Glyph_Name(face, glyph_index, buf, sz) == 0 ) {
				buf[sz - 1] = '\0';
				so << " Glyph name: \"" << buf << "\"\n";
			}
		}

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

		FT_Outline& outline = slot->outline;

		if ( outline.n_contours > 0 ) {
			cerr << "outline: n_points == " << outline.n_points
				<< ", n_contours == " << outline.n_contours << endl;
		
			c_all.push_back(ccont(so.str()));
			ccont& ccr = c_all.back();
			bool res = collect_pts(xshift, yshift, ccr,
				face, slot, outline);
			if ( !res ) {
				c_all.pop_back();
				cerr << "Failed on arg " << i << endl;
				continue;
			}
		}

		xshift += slot->metrics.horiAdvance;
	}

	if ( c_all.size() < 1 ) {
		cerr << "No valid outlines collected\n";
		return 1;
	}

	prn_openf(c_all.size());
	flt_t scl = Xmax / xshift;
	if ( scl > 1.0 ) { // temporary: check for Y later
		scl = 1.0;
	}
	for ( unsigned  i = 0; i < c_all.size(); i++ ) {
		scale_pts(c_all[i], scl);
		prn_prnobj(c_all[i], i);
	}

	std::ostringstream so;
	so << "Bezier spline characters converted from \n\"" <<
		argv[1] << "\"\n";
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
			so << "Font info fixed pitch boolean: " <<
				fir.is_fixed_pitch << '\n';
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
	so << " bounding box: " << face->bbox.xMin << ", "
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

std::string&
sanitise_string(std::string& in, std::string& out)
{
	std::istringstream si(in);
	std::ostringstream so;
	
	std::string::value_type c;
	while ( si.get(c) ) {
		if ( ! std::isprint(c) && ! std::isspace(c) ) {
			char buf[8];
			::snprintf(buf, 8, "0x%02X", unsigned(c));
			so << buf;
			continue;
		}
		switch ( c ) {
			case '\'':
			case '"':
			case '\\':
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
	if ( (pN - p0) < 3 ) {
		cerr << "CONTOUR with " << (pN - p0) << " points\n";
		return false;
	}

	char fl0 = FT_CURVE_TAG(outline.tags[p0]);

	for ( short i = p0; i <= pN; i++ ) {
		const FT_Vector& v = outline.points[i];
		PtCoord pc;
		pc.x = v.x; pc.y = v.y;
		char fl = FT_CURVE_TAG(outline.tags[i]);

		if ( fl == FT_CURVE_TAG_ON && i > p0 ) {
			if ( fl0 == FT_CURVE_TAG_ON ) {
				//2 endpoints in seq. means straight line segment
				const FT_Vector& v2 = outline.points[i - 1];
				PtCoord pc2;
				pc2.x = v2.x; pc2.y = v2.y;
				pc2.x += pc.x; pc2.y += pc.y;
				pc2.x /= 2.0; pc2.y /= 2.0;
				o.v.push_back(pc2);
				o.v.push_back(pc2);
			}
		}

		if ( fl == FT_CURVE_TAG_ON && i > p0 ) {
			// end last seg.; POV-Ray does not do so implicitely
			o.v.push_back(pc);
		}

		o.v.push_back(pc);
		fl0 = fl;
	}

	if ( fl0 == FT_CURVE_TAG_ON ) {
		// last was endpoint; make straight seg to 1st
		const FT_Vector& v = outline.points[pN];
		PtCoord pc;
		pc.x = v.x; pc.y = v.y;
		const FT_Vector& v2 = outline.points[p0];
		PtCoord pc2;
		pc2.x = v2.x; pc2.y = v2.y;
		pc2.x += pc.x; pc2.y += pc.y;
		pc2.x /= 2.0; pc2.y /= 2.0;
		o.v.push_back(pc2);
		o.v.push_back(pc2);
	}

	// POV wants first point repeated to close curve
	const FT_Vector& v = outline.points[p0];
	PtCoord pc;
	pc.x = v.x; pc.y = v.y;
	o.v.push_back(pc);

	return true;
}

ostream&
prn_contour(short p0, short pN, const FT_Outline& outline,
			ostream& o, bool& result)
{
	if ( (pN - p0) < 3 ) {
		cerr << "CONTOUR with " << (pN - p0) << " points\n";
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
			pc = "/* error: got conic segment point FT_CURVE_TAG_CONIC follows */\n";
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
	printf(
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
	printf(
	" 'FileComments'('CommentCount' = 1,\n"
	"   'Comment1' = \"%s\").\n"
	" \n"
	, out.c_str());
}

void
prn_prnobj(ccont& c, unsigned obj_num)
{
	std::string cm("/*\n");
	cm += sanitise_string(c.comment, c.comment) + "*/";

	printf(
	" 'Object%u'(objname = \"SplineObject\",\n"
	"   splinetype = 3,\n"
	"   povtype = 1,\n"
	"   sweeptype = 0,\n"
	"   use_sturm = 0,\n"
	"   use_open = 0,\n"
	"   sweep_min = \"-1.00000000\",\n"
	"   sweep_max = \"1.00000000\",\n"
	"   'UserStr' = \"%s\",\n"
	, obj_num, cm.c_str());

	unsigned nuv = 0;
	for ( ; nuv < c.v.size(); nuv++ ) {
		printf("  'U%u' = \"%g\",\n", nuv, double(c.v[nuv].x));
		printf("  'V%u' = \"%g\",\n", nuv, double(c.v[nuv].y));
	}

	printf("  'UVcount' = %u).\n\n", nuv);
}

bool
collect_pts(flt_t xshift, flt_t yshift, ccont& ccr,
	const FT_Face face, const FT_GlyphSlot slot,
	const FT_Outline& outline)
{
	// Sanity check:
	if ( outline.n_points
	 != (outline.contours[outline.n_contours-1]+1) ) {
		cerr << "First sanity check FAILED\n";
		return false;
	}

	ccr.v.reserve(2u * outline.n_points);

	short p0 = 0;
	for ( short c0 = 0; c0 < outline.n_contours; c0++ ) {
		short pN = outline.contours[c0];

		bool tbool = get_contour(p0, pN, outline, ccr);
		if ( tbool == false ) {
			cerr << "FAILED on contour: " << c0 << endl;
			return false;
		}

		prn_contour(p0, pN, outline, cerr, tbool);

		p0 = pN + 1;
	}

	for ( size_t i = 0; i < ccr.v.size(); i++ ) {
		ccr.v[i].x += xshift;
		ccr.v[i].y = yshift - ccr.v[i].y;
	}
	
	return true;
}

