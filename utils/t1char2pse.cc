#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <cerrno>

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
}

FT_Library  library;
FT_Face     face;      /* handle to face object */

using namespace std;

typedef double realtype;
template <typename T> struct TplPtCoord {
	T x, y;
};

typedef TplPtCoord<realtype> PtCoord;
typedef std::vector<PtCoord> ccont;

ostream&
prn_outline(const FT_Outline& outline, ostream& o, bool& result);
ostream&
prn_contour(short p0, short pN, const FT_Outline& outline, ostream& o, bool& result);
bool
get_contour(short p0, short pN, const FT_Outline& outline, ccont& o);
void
prn_openf();
void
scale_pts(ccont& c);


int
main(int argc, char* argv[])
{
	if ( argc < 3 ) {
		cerr << "need 2 args: font-file, and glyph-index\n";
		return 1;
	}

	int error = FT_Init_FreeType( &library );
	if ( error )
	{
		cerr << "Failed FT_Init_FreeType()\n";
		return 1;
	}

	const char* ffile = argv[1];

	error = FT_New_Face(library, ffile, 0, &face);
	if ( error == FT_Err_Unknown_File_Format ) {
		cerr << "Failed on \"" << ffile << "\": FT_Err_Unknown_File_Format\n";
		return 1;
	} else if ( error ) {
		cerr << "Failed on \"" << ffile << "\": unknown error\n";
		return 1;
	}

	errno = 0;
	FT_ULong ftul = strtoul(argv[2], 0, 0);
	if ( errno ) {
		cerr << "arg \"" << argv[2] << "\" no good: "
			<< strerror(errno) << endl;
		return 1;
	}

	// Will not succeed w/o setting a size
	// if size is too small, some integer values degenerate
	if ( FT_Set_Char_Size(face, 72 * 64, 0, 720, 0) ) {
		cerr << "FAILED FT_Set_Char_Size()\n";
		return 1;
	}

	int glyph_index = FT_Get_Char_Index(face, ftul);
	if ( glyph_index < 1 ) {
		cerr << "Found no glyph for \"" << argv[2] << "\" (" << ftul <<
		")\n";
		return 1;
	}

	error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	if ( error == FT_Err_Invalid_Argument ) {
		cerr << "Error \"FT_Err_Invalid_Argument\" from FT_Load_Glyph()\n";
		return 1;
	} else if ( error ) {
		cerr << "Error " << error << " from FT_Load_Glyph()\n";
		return 1;
	}
	FT_GlyphSlot slot = face->glyph;
	FT_Outline& outline = slot->outline;

	cerr << "outline: n_points == " << outline.n_points
		<< ", n_contours == " << outline.n_contours << endl;

	bool result;
	prn_outline(outline, cerr, result);
	if ( result == false ) {
		cerr << "Failed printing curve points for \"" << argv[1] <<
		"\"\n";
	}

	return result ? 0 : 1;
}

void
scale_pts(ccont& c)
{
	const realtype limY = 600;
	realtype minX, maxX, minY, maxY;

	minX = minY = numeric_limits<realtype>::max();
	maxX = maxY = numeric_limits<realtype>::min();

	for ( unsigned i = 0; i < c.size(); i++ ) {
		minX = min(minX, c[i].x);
		maxX = max(maxX, c[i].x);
		minY = min(minY, c[i].y);
		maxY = max(maxY, c[i].y);
	}

	// flip & shift for epspline
	realtype tmp = maxY;
	minY = numeric_limits<realtype>::max();
	maxY = numeric_limits<realtype>::min();
	for ( unsigned i = 0; i < c.size(); i++ ) {
		c[i].y = tmp - c[i].y;
		minY = min(minY, c[i].y);
		maxY = max(maxY, c[i].y);
	}

	// scale
	realtype sclX = limY / maxX;
	realtype sclY = limY / maxY;
	for ( unsigned i = 0; i < c.size(); i++ ) {
		c[i].x *= tmp;
		c[i].y *= sclY;
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
			// end last seg.; POV-Ray does no do so implicitley
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
prn_openf()
{
	fputs(
	" 'FileData'('FileFormatVersion' = 2,\n"
	"   'ObjectCount' = 1,\n"
	"   'HGuideCount' = 0,\n"
	"   'VGuideCount' = 0).\n"
	" \n"
	" 'Object0'(objname = \"SplineObject\",\n"
	"   splinetype = 3,\n"
	"   povtype = 1,\n"
	"   sweeptype = 0,\n"
	"   use_sturm = 0,\n"
	"   use_open = 0,\n"
	"   sweep_min = \"-1.00000000\",\n"
	"   sweep_max = \"1.00000000\",\n"
	, stdout);
}

ostream&
prn_outline(const FT_Outline& outline, ostream& o, bool& result)
{
	// Sanity check:
	if ( outline.n_points
	 != (outline.contours[outline.n_contours-1]+1) ) {
		result = false;
		cerr << "First sanity check FAILED\n";
		return o;
	}

	prn_openf();

	unsigned nuv = 0;

	short p0 = 0;
	for ( short c0 = 0; c0 < outline.n_contours; c0++ ) {
		short pN = outline.contours[c0];

		o << "/* contour " << c0 << ", " << (pN - p0) << " points */\n";
		ccont c;
		bool tbool = get_contour(p0, pN, outline, c);
		if ( tbool == false ) {
			cerr << "FAILED on contour: " << c0 << endl;
			result = false;
			return o;
		}

		prn_contour(p0, pN, outline, o, tbool);
		if ( tbool == false ) {
			cerr << "FAILED on contour: " << c0 << endl;
			result = false;
			return o;
		}

		scale_pts(c);
		for ( unsigned n = 0; n < c.size(); n++ ) {
			printf("  'U%u' = \"%g\",\n", nuv, double(c[n].x));
			printf("  'V%u' = \"%g\",\n", nuv, double(c[n].y));
			nuv++;
		}

		p0 = pN + 1;
	}

	printf("  'UVcount' = %u).\n", nuv);

	result = true;
	return o;
}

