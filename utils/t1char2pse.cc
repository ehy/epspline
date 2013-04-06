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
#include <vector>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
}

using namespace std;

typedef double realtype;
template <typename T> struct TplPtCoord {
	T x, y;
};

typedef TplPtCoord<realtype> PtCoord;
typedef std::vector<PtCoord> ccont;

ostream&
prn_contour(short p0, short pN, const FT_Outline& outline, ostream& o, bool& result);
bool
get_contour(short p0, short pN, const FT_Outline& outline, ccont& o);
void
scale_pts(ccont& c, double scale);
bool
collect_pts(double xshift, double yshift, ccont& ccr, FT_Face face,
	FT_GlyphSlot slot, FT_Outline& outline);
void
prn_prnobj(ccont& c, unsigned obj_num);
void
prn_openf(unsigned cnt);

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
const double Xmax = 3000.0;

int
main(int argc, char* argv[])
{
	if ( argc < 3 ) {
		cerr << "need 2 args: font-file, and glyph-index\n";
		return 1;
	}

	FT_Library library;
	int error = FT_Init_FreeType( &library );
	if ( error )
	{
		cerr << "Failed FT_Init_FreeType()\n";
		return 1;
	}

	std::atexit(f_atexit);
	ftlib_vec.push_back(library);

	FT_Face face;      /* handle to face object */
	const char* ffile = argv[1];
	error = FT_New_Face(library, ffile, 0, &face);
	if ( error == FT_Err_Unknown_File_Format ) {
		cerr << "Failed on \"" << ffile << "\": FT_Err_Unknown_File_Format\n";
		return 1;
	} else if ( error ) {
		cerr << "Failed on \"" << ffile << "\": unknown error\n";
		return 1;
	}

	double xshift = 0.0;
	double yshift = face->bbox.yMax;
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
			cerr << "Error \"FT_Err_Invalid_Argument\" from FT_Load_Glyph()\n";
			return 1;
		} else if ( error ) {
			cerr << "Error " << error << " from FT_Load_Glyph()\n";
			return 1;
		}
		FT_GlyphSlot slot = face->glyph;

		if ( !xshift_init ) {
			xshift -= slot->metrics.horiBearingX;
		}

		FT_Outline& outline = slot->outline;

		if ( outline.n_contours > 0 ) {
			cerr << "outline: n_points == " << outline.n_points
				<< ", n_contours == " << outline.n_contours << endl;
		
			c_all.push_back(ccont());
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
	double scl = Xmax / xshift;
	for ( unsigned  i = 0; i < c_all.size(); i++ ) {
		if ( scl < 1.0 ) {
			scale_pts(c_all[i], scl);
		}
	
		prn_prnobj(c_all[i], i);
	}

#if 0
	double llx, lly, urx, ury;
	llx = face->bbox.xMin;
	lly = face->bbox.yMin;
	urx = face->bbox.xMax;
	ury = face->bbox.yMax;
	cerr << "face->bbox:\n\t" <<
		"xMin " << llx << ", yMin " << lly <<
		", xMax " << urx << ", yMax " << ury << endl;
	cerr << "face->units_per_EM " << face->units_per_EM << endl;
	cerr << "face->ascender " << face->ascender << endl;
	cerr << "face->descender " << face->descender << endl;
	cerr << "face->height " << face->height << endl;
	cerr << "face->underline_position " << face->underline_position << endl;
	FT_Glyph_Metrics& gm = slot->metrics;
	cerr << "glyph metrics:\n\t" <<
		"width " << gm.width << ", height " << gm.height <<
		endl <<
		"\thoriBearingX " << gm.horiBearingX <<
		", horiBearingY " << gm.horiBearingY <<
		", horiAdvance " << gm.horiAdvance <<
		endl <<
		"\tvertBearingX " << gm.vertBearingX <<
		", vertBearingY " << gm.vertBearingY <<
		", vertAdvance " << gm.vertAdvance <<
		endl;

	return result ? 0 : 1;
#endif

	return 0;
}

void
scale_pts(ccont& c, double scale)
{
	for ( unsigned i = 0; i < c.size(); i++ ) {
		c[i].x *= scale;
		c[i].y *= scale;
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
				o.push_back(pc2);
				o.push_back(pc2);
			}
		}

		if ( fl == FT_CURVE_TAG_ON && i > p0 ) {
			// end last seg.; POV-Ray does not do so implicitely
			o.push_back(pc);
		}

		o.push_back(pc);
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
		o.push_back(pc2);
		o.push_back(pc2);
	}

	// POV wants first point repeated to close curve
	const FT_Vector& v = outline.points[p0];
	PtCoord pc;
	pc.x = v.x; pc.y = v.y;
	o.push_back(pc);

	return true;
}

ostream&
prn_contour(short p0, short pN, const FT_Outline& outline, ostream& o, bool& result)
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
prn_prnobj(ccont& c, unsigned obj_num)
{
	printf(
	" 'Object%u'(objname = \"SplineObject\",\n"
	"   splinetype = 3,\n"
	"   povtype = 1,\n"
	"   sweeptype = 0,\n"
	"   use_sturm = 0,\n"
	"   use_open = 0,\n"
	"   sweep_min = \"-1.00000000\",\n"
	"   sweep_max = \"1.00000000\",\n"
	, obj_num);

	unsigned nuv = 0;
	for ( ; nuv < c.size(); nuv++ ) {
		printf("  'U%u' = \"%g\",\n", nuv, double(c[nuv].x));
		printf("  'V%u' = \"%g\",\n", nuv, double(c[nuv].y));
	}

	printf("  'UVcount' = %u).\n\n", nuv);
}

bool
collect_pts(double xshift, double yshift, ccont& ccr, FT_Face face,
	FT_GlyphSlot slot, FT_Outline& outline)
{
	// Sanity check:
	if ( outline.n_points
	 != (outline.contours[outline.n_contours-1]+1) ) {
		cerr << "First sanity check FAILED\n";
		return false;
	}

	ccr.reserve(2u * outline.n_points);

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

	for ( size_t i = 0; i < ccr.size(); i++ ) {
		ccr[i].x += xshift;
		ccr[i].y = yshift - ccr[i].y;
	}
	
	return true;
}

