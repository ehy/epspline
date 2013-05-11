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

// Spline objects
//

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/dcmemory.h"
#include "wx/bitmap.h"
#include "wx/rawbmp.h"
#endif // WX_PRECOMP

#ifdef __GNUG__
    #pragma implementation
#endif

// for setting "C" locale in a block
#include "clocnumeric.h"

// TODO: double check need for this conditional:
// is it only dmc compiler that needs it?
// IAC it should be harmless
#ifdef __WXMSW__
#include <float.h>
#else
#include <cfloat>
#endif

#include <limits>
#include <cmath>

#include "splines.h"
#include "wxutil.h"


// namespace for functor classes, as necessary
namespace eps_funcs {

class cp_isflag {
public:
	cp_isflag() {}
	inline bool operator () (const CurvePt& p) {
		return p.isflag();
	}
};

inline void DrawLines(wxDC* dc, unsigned cnt, wxPoint* p)
{
	dc->DrawLines(cnt, p);
}

template<class T> void
SplineBaseDrawCache(
	wxDC* dc,
	const T& cc,
	const wxRect* bound
	)
{
	if ( dc && cc.size() > 1 ) {
	    const int inlst = INT_MIN;
		wxPoint plast(inlst, inlst);
		typename T::const_iterator b = cc.begin(), e = cc.end();
#if ! NO_STATIC_BUFFER
		// This is conditional, with use of temporary automatic
		// buffers if ! NO_STATIC_BUFFER, because static data
		// can be very ungood in a multithreaded program; *but*,
		// 1) presently this program is single threaded, and
		// 2) in multithreaded wx programs most library calls,
		//    such as calls on wxDC, must occur in the main thread
		//    and should be sync'd by use of wx inter-thread
		//    message handling.
		// IAC, if it is a problem use one of the stack buffers
		// below (but they will be slower; probably not noticeably).
		static wxPoint* wxp = 0;
		static size_t sz_wxp = 0;
		const size_t wxp_minalloc = 4096; // 64k if sizeof(wxPoint)==16
		
		if ( sz_wxp < cc.size() || wxp == 0 ) {
			sz_wxp = std::max(cc.size(), wxp_minalloc);
			delete[] wxp;
			wxp = new wxPoint[sz_wxp];
		}
#elif NO_TRAWBUFFER
        tbuffer<wxPoint> wxp(cc.size());
#else
        // As stated in util.h, trawbuffer must only be used for
        // simple classes that do not truly need their default
        // constructors.  As of wx 2.8, the wxPoint is such a class,
        // but this remains DANGEROUS because who knows what
        // changes might be in later versions.  Check this point
        // on segfaults, bus error and such.
        //
        // The reason trawbuffer is desirable is that the array
        // size may be large, and this code will be called repeatedly
        // and rapidly during object transforms such as moves and
        // rotates, and the default ctor calls will add up.
        trawbuffer<wxPoint> wxp(cc.size());
#endif // ! NO_STATIC_BUFFER

        //int cnt = 0;     // wxDC::DrawLines takes an int count . . .
        unsigned int cnt = 0; // . . . but I want to use unsigned
        while ( b < e ) {
            if ( (*b).isflag() ) {
                ++b;
                if ( cnt > 1 ) {
                    DrawLines(dc, cnt, wxp);
                }
                cnt = 0;
                plast = wxPoint(inlst, inlst);
                continue;
            }
            if ( (wxp[cnt] = *b++) == plast ) {
                continue;
            }
            if ( bound && ! InRect(*bound, wxp[cnt]) ) {
				if ( cnt && InRect(*bound, wxp[cnt-1]) ) {
					DrawLines(dc, cnt + 1, wxp);
				}
				if ( cnt ) {
					wxp[0] = wxp[cnt];
					cnt = 0;
				}
			}
            plast = wxp[cnt++];
        }
        if ( cnt > 1 ) {
            DrawLines(dc, cnt, wxp);
        }
	}
}

// END namespace eps_funcs
};

void
SplineBase::DrawCache(
	wxDC* dc,
	const SplineBase::curve_cache& cc,
	const wxRect* bound
	)
{
	eps_funcs::SplineBaseDrawCache(dc, cc, bound);
}

const t_ch  SplineBase::DefaultName [] = wxT("SplineObject");

const t_ch* SplineBase::PropNames [] = {
	wxT("Name"),
	wxT("Transform"),
	wxT("Texture"),
	wxT("Interior"),
	wxT("UserStr")
};

const size_t SplineBase::PropCount = A_SIZE(SplineBase::PropNames);

// empty(-ish) test for the several string fields, to avoid
// so many in-line if() tests
/* static */
wxString
SplineBase::EmptyFieldString() // translated, like '_none_'
{
	// TRANSLATORS: this MUST consist of ASCII alpha-numeric,
	// and optionally '_', due to the strings use in dialog fields
	// which are constrained to those characters due to the
	// requirements of the povray program.  This string indicates
	// that a user-value has not been given; it should not be something
	// that a user would be likely to use as a label in povray
	// scene description language. If the string has characters
	// other than [a-zA-Z0-9_] (shown as a regex pattern) it might
	// cause errors in this program or in povray.
	return _("_empty_field_");
}
/* static */
bool
SplineBase::StringFieldEmpty(const wxString& s)
{
	return ( s == wxT("") || s == EmptyFieldString() );
}

SplineBase::~SplineBase()
{
}

wxString
SplineBase::GetDescription() const
{
	wxString sw(wxT(""));
	wxString des(wxT(""));

	/*
	 * Considering i18n, strings from the code below might well be
	 * incomprehensible to a translator, . . .
	if ( objt == prism ) {
		sw.Printf(_(", %s sweep %f, %f"),
			sweept == conic ? _("conic") : _("linear"),
			sweep_min, sweep_max);
	}

	des.Printf(_("%s, %s %s%s"),
		ObNam.c_str(),
		splinet == linear ? _("linear") :
			(splinet == quadratic ? _("quadratic") :
				(splinet == cubic ? _("cubic") : _("bezier") ) ),
		objt == prism ? _("prism") :
			(objt == lathe ? _("lathe") : _("undefined")),
		sw.c_str());
	 * . . . so use something with more complete strings:
	 */

	des = ObNam;
	if ( splinet == linear ) {
		if ( objt == prism ) {
			if ( sweept == conic ) {
				sw.Printf(_(", linear prism, conic sweep %g, %g"),
					sweep_min, sweep_max);
			} else {
				sw.Printf(_(", linear prism, linear sweep %g, %g"),
					sweep_min, sweep_max);
			}
		} else if ( objt == lathe ) {
			sw = _(", linear lathe");
		} else {
			sw = _(", linear, undefined type");
		}
	} else if ( splinet == quadratic ) {
		if ( objt == prism ) {
			if ( sweept == conic ) {
				sw.Printf(_(", quadratic prism, conic sweep %g, %g"),
					sweep_min, sweep_max);
			} else {
				sw.Printf(_(", quadratic prism, linear sweep %g, %g"),
					sweep_min, sweep_max);
			}
		} else if ( objt == lathe ) {
			sw = _(", quadratic lathe");
		} else {
			sw = _(", quadratic, undefined type");
		}
	} else if ( splinet == cubic ) {
		if ( objt == prism ) {
			if ( sweept == conic ) {
				sw.Printf(_(", cubic prism, conic sweep %g, %g"),
					sweep_min, sweep_max);
			} else {
				sw.Printf(_(", cubic prism, linear sweep %g, %g"),
					sweep_min, sweep_max);
			}
		} else if ( objt == lathe ) {
			sw = _(", cubic lathe");
		} else {
			sw = _(", cubic, undefined type");
		}
	} else if ( splinet == bezier ) {
		if ( objt == prism ) {
			if ( sweept == conic ) {
				sw.Printf(_(", bezier prism, conic sweep %g, %g"),
					sweep_min, sweep_max);
			} else {
				sw.Printf(_(", bezier prism, linear sweep %g, %g"),
					sweep_min, sweep_max);
			}
		} else if ( objt == lathe ) {
			sw = _(", bezier lathe");
		} else {
			sw = _(", bezier, undefined type");
		}
	}

	des += sw;

	return des;
}

void
SplineBase::AddPts(int n, SplinePoint pts[])
{
	for ( int i = 0; i < n; i++ )
		push_back(pts[i]);
	SetDirty();
}

void
SplineBase::AddPts(int n, int pts[])
{
	if ( n & 1 ) return;

	for ( int i = 0; i < n; i += 2 )
		push_back(SplinePoint(pts[i],pts[i+1]));
	SetDirty();
}

void
SplineBase::SetPts(int n, SplinePoint pts[])
{
	::clear(*this);
	AddPts(n, pts);
}

void
SplineBase::SetPts(int n, int pts[])
{
	::clear(*this);
	AddPts(n, pts);
}

bool
SplineBase::MovePoint(SplinePoint& pt, double xoff, double yoff)
{
	if ( PointIndex(pt) < 0 )
		return false;
	pt.x += xoff;
	pt.y += yoff;
    SetDirty();
	return true;
}

// quick hack, for use in SplineBase::MoveSpline; g++ would not
// match the for_each template with classes def'd inside function
namespace eps_funcs {
    class sp_offset {
        SplineBase& sr;
        int xo, yo;
    public:
        sp_offset(SplineBase& s, double x, double y)
            : sr(s), xo(irint(x)), yo(irint(y)) {
            sr.InitBbox();
        }
        inline void operator () (SplinePoint& p) {
            p.x += xo; p.y += yo;
            sr.UpdateBbox(p);
        }
    };

    class cp_offset {
        SplineBase& sr;
        double xo, yo;
    public:
        cp_offset(SplineBase& s, double x, double y)
            : sr(s), xo(x), yo(y) {
            sr.InitCbox();
        }
        inline void operator () (CurvePt& p) {
            if ( p.isflag() ) return;
            p.addX(xo); p.addY(yo);
            sr.UpdateBbox(wxPoint() = p);
            sr.UpdateCbox(p);
        }
    };
};

bool
SplineBase::MoveSpline(double xoff, double yoff)
{
	std::for_each(begin(), end()
               , eps_funcs::sp_offset(*this, xoff, yoff));

	if ( dirty ) {
		CalcCurveCache();
		SetDirty(false);
		return true;
	}

	std::for_each(ccache.begin(), ccache.end()
               , eps_funcs::cp_offset(*this, xoff, yoff));

	return true;
}

void
SplineBase::BBox(wxPoint& least, wxPoint& most)
{
	least.x = bbox.x;
	least.y = bbox.y;
	most.x = least.x + bbox.width;
	most.y = least.y + bbox.height;
}

void
SplineBase::CBox(double& mnx, double& mny, double& mxx, double& mxy)
{
	if ( dirty ) {
		CalcCurveCache();
		SetDirty(false);
	}
	if ( objt == lathe ) {
		// because lathe rotates about axis:
		mxx = std::max(std::abs(cboxM.x), std::abs(cboxL.x));
		mxy = cboxM.y;
		mnx = -mxx;
		mny = cboxL.y;
	} else {
		mnx = cboxL.x; mny = cboxL.y;
		mxx = cboxM.x; mxy = cboxM.y;
	}
}

bool
SplineBase::PtInRect(const wxPoint& p) const
{
	return InRect(*BBox(), p);
}

bool
SplineBase::PtInRect(const wxPoint& p, unsigned pad) const
{
	wxRect t(*BBox());
	t.Inflate(pad);
	return InRect(t, p);
}

int
SplineBase::PointIndex(const SplinePoint& pt, int dtol) const
{
	int i = 0;
	const_iterator it = begin();
	while ( it != end() ) {
		const SplinePoint& p = *it++;
		if ( std::abs(p.x-pt.x) <= dtol && std::abs(p.y-pt.y) <= dtol )
			return i;
		++i;
	}
	return -1;
}

SplineBaseBase::iterator
SplineBase::PointIndexIt(const SplinePoint& pt, int dtol)
{
	iterator it = begin();
	while ( it != end() ) {
		SplinePoint& p = *it;
		if ( std::abs(p.x-pt.x) <= dtol && std::abs(p.y-pt.y) <= dtol )
			break;
		++it;
	}
	return it;
}

bool
SplineBase::Okay() const
{
	return size() ? true : false;
}

void
SplineBase::SetType(obj_t type)
{
	switch ( type ) {
		case prism:
		case lathe:
			objt = type;
			break;
		case undef:
		default:
			objt = undef;
			break;
	}
}

// bad hack:
static int spbx = INT_MIN;
static int spby = INT_MIN;

void
SplineBase::InitBbox()
{
	bbox.x = bbox.y = INT_MAX;
	bbox.width = bbox.height = 0;
	spbx = spby = INT_MIN;
}

void
SplineBase::InitCbox()
{
	cboxL.x = cboxL.y = DBL_MAX;
	cboxM.x = cboxM.y = DBL_MIN;
}

void
SplineBase::UpdateBbox(const wxRealPoint& P)
{
	wxPoint p(irint(P.x), irint(P.y));
	UpdateBbox(p);
}

void
SplineBase::UpdateCbox(double x, double y)
{
	cboxL.x = std::min(cboxL.x, x);
	cboxL.y = std::min(cboxL.y, y);
	cboxM.x = std::max(cboxM.x, x);
	cboxM.y = std::max(cboxM.y, y);
}

void
SplineBase::UpdateBbox(const wxPoint& p)
{
	bbox.x = std::min(bbox.x,p.x);
	bbox.y = std::min(bbox.y,p.y);
	spbx = std::max(spbx,p.x);
	spby = std::max(spby,p.y);
	bbox.width = spbx - bbox.x;
	bbox.height = spby - bbox.y;
}

void
SplineBase::DrawDots(wxDC& dc, wxBrush br, unsigned size)
{
#if wxCHECK_VERSION(2, 6, 0)
	const wxBrush& b = dc.GetBrush();
#else
	wxBrush& b = dc.GetBrush();
#endif
	dc.SetBrush(br);
	DrawDots(dc, size);
	dc.SetBrush(b);
}

void
SplineBase::DrawDots(wxDC& dc, unsigned size)
{
	iterator it = begin();
	while ( it != end() ) {
		SplinePoint& p = *it++;
		dc.DrawCircle(IRINT(p.x), IRINT(p.y), size);
	}
}

void
SplineBase::DrawSelectedDot(wxDC& dc, const SplinePoint& pt, unsigned size)
{
	dc.DrawRectangle(IRINT(pt.x)-(size>>1), IRINT(pt.y)-(size>>1)
		, size, size);
}

void
SplineBase::DrawSelectedDot(wxDC& dc
	, wxBrush br, const SplinePoint& pt, unsigned size)
{
#if wxCHECK_VERSION(2, 6, 0)
	const wxBrush& b = dc.GetBrush();
#else
	wxBrush& b = dc.GetBrush();
#endif
	dc.SetBrush(br);
	DrawSelectedDot(dc, pt, size);
	dc.SetBrush(b);
}

void
SplineBase::DrawDotsSelectedDot(wxDC& dc
	, const SplinePoint& pt, wxBrush selectedbr
	, unsigned size, unsigned selsize)
{
	DrawDots(dc, size);
	DrawSelectedDot(dc, selectedbr, pt, selsize);
}

bool
SplineBase::Export(FILE* f, int n,
	double& lx, double& ly, double& mx, double& my, bool indemo)
{
	if ( !POkay() ) {
		return false;
	}

	// this makes sure real number formatting is in "C" locale
	cnumtmp c_tmp;

	// get min and max for this object
	double Lx = DBL_MAX, Ly = DBL_MAX;
	double Mx = DBL_MIN, My = DBL_MIN;
	CBox(Lx, Ly, Mx, My);

	// wxString not really needed here; but keep for ease of use
	wxString t(ObNam), sps, sws;
	if ( t == DefaultName )
		t << n;
	switch ( splinet ) {
		case linear: sps = wxT("linear_spline"); break;
		case quadratic: sps = wxT("quadratic_spline"); break;
		case cubic: sps = wxT("cubic_spline"); break;
		case bezier: sps = wxT("bezier_spline"); break;
	};

	if ( sweept == slinear ) sws = wxT("linear_sweep");
	else sws = wxT("conic_sweep");

	// auto char bufs, to avoid repeated conversions in UNICODE builds
	const tmp_cstr Ct(wxs2ch(t)), Csps(wxs2ch(sps)), Csws(wxs2ch(sws));

	// first put size declarations
	fprintf(f, "#declare %s_width = %f;\n#declare %s_height = %f;\n"
		, Ct.p(), double(Mx-Lx), Ct.p(), double(My-Ly));
	fprintf(f, "#declare %s_left = %f;\n#declare %s_right = %f;\n"
		, Ct.p(), double(Lx), Ct.p(), double(Mx));
	fprintf(f, "#declare %s_top = %f;\n#declare %s_bottom = %f;\n"
		, Ct.p(), double(Ly), Ct.p(), double(My));
	fprintf(f, "#declare %s_max_extent = max(%s_width, %s_height);\n"
		, Ct.p(), Ct.p(), Ct.p());
	fputc('\n', f);

	// declare a 'transform' object that can be used to normalize
	// the spline object, in this sense: one unit extent of greatest
	// dimension, and centered on origin -- the undefined type is
	// neglected here.
	if ( objt == prism ) {
		const char t[] = "#declare %s_NORMAL_TRANSFORM = transform {\n"
		"  translate"
		" <-%s_left - %s_width / 2, 0, -%s_top - %s_height / 2>\n"
		"  scale"
		" <1 / %s_max_extent, 1, 1 / %s_max_extent>\n"
		"  scale <1, 1, -1>\n"
		"}\n\n";
		fprintf(f, t, Ct.p(),
			Ct.p(), Ct.p(), Ct.p(), Ct.p(),
			Ct.p(), Ct.p()
		);
	} else if ( objt == lathe ) {
		const char t[] = "#declare %s_NORMAL_TRANSFORM = transform {\n"
		"  translate <0, -%s_top - %s_height / 2, 0>\n"
		"  scale (1 / %s_max_extent)\n"
		"  scale <1, -1, 1>\n"
		"}\n\n";
		fprintf(f, t, Ct.p(),
			Ct.p(), Ct.p(),
			Ct.p()
		);
	}

	// begin object decl.
	switch ( objt ) {
		case prism:
			fprintf(f, "#declare %s =\nprism { %s %s %f, %f\n  , "
				, Ct.p(), Csps.p(), Csws.p()
				, sweep_min, sweep_max);
			break;
		case lathe:
			fprintf(f, "#declare %s =\nlathe { %s\n  "
				, Ct.p(), Csps.p());
			break;
		case undef:
		default:
			fprintf(f, "/*\n"
					   "  Undefined type \"%s\" was defined as a %s\n"
					   "  with sweep (min, max) %f and %f.\n"
					   "  It is declared as an array here.\n"
					   "*/\n"
				, Ct.p(), Csps.p(), sweep_min, sweep_max);
			fprintf(f, "#declare %s_ARRAY_COUNT = %u;\n"
				, Ct.p(), unsigned(size()));
			fprintf(f, "#declare %s_SWEEP_MIN = %f;\n"
				, Ct.p(), sweep_min);
			fprintf(f, "#declare %s_SWEEP_MAX = %f;\n"
				, Ct.p(), sweep_max);
			fprintf(f, "#declare %s =\narray[%s_ARRAY_COUNT] {\n  "
				, Ct.p(), Ct.p());
			break;
	}

	if ( objt == undef ) {
		// put control points
		iterator b = begin(), e = end();
		for ( iterator i = b; i != e; i++ ) {
			SplinePoint& P = *i;
			if ( i != b ) fputs(",\n  ", f);
			fprintf(f, "<%f, %f>", double(P.x), double(P.y));
		}
		fputc('\n', f);
	} else {
		// put control point count
		fprintf(f, "%u\n", unsigned(size()));
	
		// put control points
		iterator i = begin();
		while ( i != end() ) {
			SplinePoint& P = *i++;
			fprintf(f, "  , <%f, %f>\n", double(P.x), double(P.y));
		}
	
		// 'open', if used, must be first after <u,v> points
		if ( use_open && objt == prism ) {
			fputs("  open\n", f);
		}
		if ( use_sturm ) {
			fputs("  sturm\n", f);
		}

		fprintf(f,
			"#ifdef ( %s_USE_NORMAL_TRANSFORM )\n"
			"  transform { %s_NORMAL_TRANSFORM }\n"
			"#end\n",
			Ct.p(), Ct.p());

		// finally, put user-optional attributes
		if ( NotStringFieldEmpty(Texture) )
			fprintf(f, "  texture { %s }\n", wxs2ch(Texture));
		if ( NotStringFieldEmpty(Interior) )
			fprintf(f, "  interior { %s }\n", wxs2ch(Interior));
		// demo code cannot make dummies for these
		if ( !indemo ) {
			if ( NotStringFieldEmpty(Transform) )
				fprintf(f, "  transform { %s }\n", wxs2ch(Transform));
			if ( NotStringFieldEmpty(UserStr) )
				fprintf(f, "  %s\n", wxs2ch(UserStr));
		}
	}

	// put object close and blank line
	fprintf(f, "}\n\n");

	// for the caller: tracking min & max
	lx = std::min(lx, Lx);
	ly = std::min(ly, Ly);
	mx = std::max(mx, Mx);
	my = std::max(my, My);

	return true;
}

void
SplineBase::InitPropsDialog(spline_properties& dlg)
{
	switch ( splinet ) {
		case linear:
			dlg.spline_type_label->SetLabel(_("Linear Spline")); 
			break;
		case quadratic:
			dlg.spline_type_label->SetLabel(_("Quadratic Spline")); 
			break;
		case cubic:
			dlg.spline_type_label->SetLabel(_("Cubic Spline")); 
			break;
		case bezier:
			dlg.spline_type_label->SetLabel(_("Bezier Spline")); 
			break;
	}
      
	// POVRay 3.5 docs state that identifiers are:
	//
	//     Up to 127 characters, starting with a letter,
	//     consisting of letters, digits and/or the underscore.
	//
	// and 'letters' and 'digits' are clearly meant as ASCII,
	// although the doc does not actually say so (non-ascii causes
	// povray to fail -- check again with POVRay 4.x), and IAC
	// the file parser will have to be rewritten for mbstrings
	// (currently segfaults)
	static wxArrayString* pwas = 0;
	if ( pwas == 0 ) {
		pwas = new wxArrayString();

		for ( size_t i = 0; i < pov_chars_OK_count; i++ ) {
			pwas->Add(wxString(pov_chars_OK[i]));
		}
	}
	
	wxTextValidator* ttv;
	ttv =
	dynamic_cast<wxTextValidator*>(dlg.txt_objname->GetValidator());
	ttv->SetIncludes(*pwas);
	dlg.txt_objname_validator = ObNam;

	ttv =
	dynamic_cast<wxTextValidator*>(dlg.txt_texture->GetValidator());
	ttv->SetIncludes(*pwas);
	dlg.txt_texture_validator = Texture;

	ttv =
	dynamic_cast<wxTextValidator*>(dlg.txt_interior->GetValidator());
	ttv->SetIncludes(*pwas);
	dlg.txt_interior_validator = Interior;

	ttv =
	dynamic_cast<wxTextValidator*>(dlg.txt_transform->GetValidator());
	ttv->SetIncludes(*pwas);
	dlg.txt_transform_validator = Transform;
      
	int t = 0;
	switch ( objt ) {
		case lathe:
			t = 0;
			break;
		case prism:
			t = 1;
			break;
		case undef:
		default:
			t = 2;
			break;
	}
	dlg.pov_type->SetSelection(t);      
      
	switch ( sweept ) {
		case slinear:
			t = 0;
			break;
		case conic:
		default:
			t = 1;
			break;
	}
	dlg.sweep_type->SetSelection(t);      

	dlg.check_sturm->SetValue(use_sturm);      
	dlg.check_open->SetValue(use_open);      
      
	wxString ts;
	ts = wxT(""); ts.Printf(wxT("%f"), sweep_min);
	dlg.txt_sweep_min_validator = ts;
	ts = wxT(""); ts.Printf(wxT("%f"), sweep_max);
	dlg.txt_sweep_max_validator = ts;
	
	// using ASCII validator
	dlg.txt_predefined_validator = UserStr;
}

void
SplineBase::SetProps(spline_properties& dlg)
{
	wxString ts;

	// User supplied IDs are checked for collision with
	// POV-Ray reserved words
	ts = dlg.txt_objname_validator;
	ts.Trim(true); ts.Trim(false);
	if ( check_identifier(ts) ) ObNam = ts;
	ts = dlg.txt_texture_validator;
	ts.Trim(true); ts.Trim(false);
	if ( check_identifier(ts) ) Texture = ts;
	ts = dlg.txt_interior_validator;
	ts.Trim(true); ts.Trim(false);
	if ( check_identifier(ts) ) Interior = ts;
	ts = dlg.txt_transform_validator;
	ts.Trim(true); ts.Trim(false);
	if ( check_identifier(ts) ) Transform = ts;
 	ts = dlg.txt_predefined_validator;
 	// UserStr value is different -- only constraint is to ASCII,
 	// and may user beware.
	UserStr = ts;
	
	if ( ObNam == wxT("") ) {
		ObNam = DefaultName;
	}
      
	int t = dlg.pov_type->GetSelection();
	switch ( t ) {
		case 0:
			objt = lathe;
			break;
		case 1:
			objt = prism;
			break;
		case 2:
		default:
			objt = undef;
			break;
	}
      
	t = dlg.sweep_type->GetSelection();
	switch ( t ) {
		case 0:
			sweept = slinear;
			break;
		case 1:
		default:
			sweept = conic;
			break;
	}
	
	use_sturm = dlg.check_sturm->GetValue();
	use_open = dlg.check_open->GetValue();
      
	ts = dlg.txt_sweep_min_validator;
	ts.ToDouble(&sweep_min);
	ts = dlg.txt_sweep_max_validator;
	ts.ToDouble(&sweep_max);

	// sanity
	if ( sweep_max < sweep_min ) {
		double d = sweep_min;
		sweep_min = sweep_max;
		sweep_max = d;
	}
}

void
SplineBase::Rotate(unsigned type, wxPoint p0, wxPoint p1, wxPoint ctr)
{
	if ( type == or_not ) return;

	double angle = FindAngle(p0, p1, ctr);

	wxRealPoint p;
	p.x = ctr.x; p.y = ctr.y;
	Rotate(angle, p);
}

void
SplineBase::Rotate(double angle, const wxRealPoint& ctr)
{
	iterator i = begin(), e = end();
	while ( i != e ) {
		SplinePoint& p = *i++;
		p.x -= ctr.x; p.y -= ctr.y;
		bool flip = (p.y < 0.0);
		if ( flip ) {p.y = -p.y; p.x = -p.x;}

		double r = LineLength(p.x, p.y, 0.0, 0.0);
		if ( r == 0.0 ) {
			if ( flip ) {p.y = -p.y; p.x = -p.x;}
			p.x += ctr.x; p.y += ctr.y;
			continue;
		}
		double A = acos(double(p.x)/r) + angle;

		p.x = cos(A) * r;
		p.y = sin(A) * r;
		if ( flip ) {p.y = -p.y; p.x = -p.x;}
		p.x += ctr.x; p.y += ctr.y;
	}

	// try rotating cache, rather than recalulating
	#if 1
	curve_cache::iterator ci = ccache.begin(), ce = ccache.end();
	while ( ci != ce ) {
		CurvePt& p = *ci++;
		p -= ctr;
		bool flip = (p.getY() < 0.0);
		if ( flip ) {p.negate();}

		double r = LineLength(p.getX(), p.getY(), 0.0, 0.0);
		if ( r == 0.0 ) {
            if ( flip ) {p.negate();}
            p += ctr;
			continue;
		}
		double A = acos(p.getX()/r) + angle;

		p.setX(cos(A) * r);
		p.setY(sin(A) * r);
		if ( flip ) {p.negate();}
        p += ctr;
	}
	#else
	SetDirty();
	#endif
}

void
SplineBase::Shear(unsigned type, wxPoint p0, wxPoint p1, wxPoint ctr)
{
	const wxRect& r = *BBox();
	double rat, div;
	bool   neg = false, hor = false;

	switch ( type ) {
		case or_n: case or_en:
			div = double(ctr.y - r.y);
			if ( std::abs(div) < 0.000000001 ) return;
			rat = double(p1.x-p0.x) / div;
			neg = true;
			break;
		case or_s: case or_ws:
			div = double(r.height) - double(ctr.y - r.y);
			if ( std::abs(div) < 0.000000001 ) return;
			rat = double(p1.x-p0.x) / div;
			break;
		case or_w: case or_nw:
			div = double(ctr.x - r.x);
			if ( std::abs(div) < 0.000000001 ) return;
			rat = double(p1.y-p0.y) / div;
			neg = true;
			hor = true;
			break;
		case or_e: case or_se:
			div = double(r.width) - double(ctr.x - r.x);
			if ( std::abs(div) < 0.000000001 ) return;
			rat = double(p1.y-p0.y) / div;
			hor = true;
			break;
		default:
			return;
	}

	if ( neg ) rat = -rat;

	iterator i = begin(), e = end();
	while ( i != e ) {
		SplinePoint& p = *i++;

		if ( hor ) {
			p.y += (p.x - ctr.x) * rat;
		} else {
			p.x += (p.y - ctr.y) * rat;
		}
	}

	SetDirty();
}

void
SplineBase::Flip(unsigned type)
{
	const wxRect& r = *BBox();
	int xs = r.width * 2, ys = r.height * 2;

	switch ( type ) {
		case or_ws: case or_s: ys = -ys; break;
		case or_en: case or_e: xs = -xs; break;
		case or_se: xs = -xs; ys = -ys; break;
		default: break;
	}

	Scale(type, xs, ys);
    SetDirty();
}

void
SplineBase::Scale(unsigned type, int xs, int ys, bool proportional)
{
	const wxRect& r = *BBox();

	if ( !r.width || !r.height )
		return;

	double W = r.width, H = r.height, X = r.x, Y = r.y;
	double SX = xs, SY = ys;
	double t1, t2;
	iterator i = begin(), e = end();

	while ( i != e ) {
		SplinePoint& p = *i++;

		switch ( type ) {
			case or_e:
				p.x += (
					(SX * (p.x-X)) / W
				);
				break;
			case or_s:
				p.y += (
					(SY * (p.y-Y)) / H
				);
				break;
			case or_w:
				p.x += (
					SX - ((SX * (p.x-X)) / W)
				);
				break;
			case or_n:
				p.y += (
					SY - ((SY * (p.y-Y)) / H)
				);
				break;
			case or_se:
				p.x += (
					(SX * (p.x-X)) / W
				);
				t1 = proportional ? SX : SY;
				t2 = proportional ? W : H;
				p.y += (
					(t1 * (p.y-Y)) / t2
				);
				break;
			case or_nw:
				t1 = proportional ? SY : SX;
				t2 = proportional ? H : W;
				p.x += (
					t1 - ((t1 * (p.x-X)) / t2)
				);
				t1 = SY;
				t2 = H;
				p.y += (
					t1 - ((t1 * (p.y-Y)) / t2)
				);
				break;
			case or_ws:
				p.x += (
					SX - ((SX * (p.x-X)) / W)
				);
				t1 = proportional ? SX : SY;
				t2 = proportional ? W : H;
				p.y += (
					(t1 * (p.y-Y)) / t2
				);
				break;
			case or_en:
				p.x += (
					(SX * (p.x-X)) / W
				);
				t1 = proportional ? SX : SY;
				t2 = H;
				p.y += (
					t1 - ((t1 * (p.y-Y)) / t2)
				);
				break;
			default:
				break;
		}
	}

    SetDirty();
}


SplineBase*
LinearSpline::CopySelf() const
{
	return new LinearSpline(*this);
}

bool
LinearSpline::Okay() const
{
	return (size() >= 2) ? true : false;
}

bool
LinearSpline::POkay(const_subcurve_list* ps) const
{
	if ( size() > 2 ) {
		const_iterator i = begin();
		const_iterator e = end();
		const_iterator j = i; // 1nd point and last must match
		const_subcurve c;
		c.first = j;
		++i; ++i;
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// discontinuity
				c.first = j = i++;
				if ( i++ == e ) return false;
				if ( i == e ) return false;
			}
		}
		if ( *j == *--i ) {
			if ( ps ) {
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}
	return false;
}

bool
LinearSpline::POkay(subcurve_list* ps)
{
	if ( size() > 2 ) {
		iterator i = begin();
		iterator e = end();
		iterator j = i; // 1nd point and last must match
		subcurve c;
		c.first = j;
		++i; ++i;
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// discontinuity
				c.first = j = i++;
				if ( i++ == e ) return false;
				if ( i == e ) return false;
			}
		}
		if ( *j == *--i ) {
			if ( ps ) {
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}
	return false;
}

bool
LinearSpline::DelPoint(const SplinePoint& p)
{
	iterator i = PointIndexIt(p, 0);
	iterator j = end();
	if ( i == j ) {
		return false;
	}

	size_type sz = size();
	if ( sz > 2 ) {
		if ( i == begin() ) {
			iterator t = i;
			*++t = *--j;
		} else if ( --j == i ) {
			iterator t = begin();
			if ( *j == *t ) {
				*--j = *begin();
			}
		}
	}

	erase(i);
	SetDirty();
	return true;

}

bool
LinearSpline::AddPoint(const SplinePoint& p)
{
	SplinePoint a, b;
	iterator it = begin();

	a = *it++;
	long Px = LRINT(p.x), Py = LRINT(p.y);

	while ( it != end() ) {
		b = *it;

		if ( PointOnSegPQ(LRINT(a.x), LRINT(a.y)
			, LRINT(b.x), LRINT(b.y), Px, Py) ) {
			insert(it, p);
			SetDirty();
			return true;
		}
		a = b;
		++it;
	}

	return false;
}

// calculate curve points from current control points,
// and store in cache vector
void
LinearSpline::CalcCurveCache()
{

	InitBbox();
	InitCbox();

	iterator it = begin();
	SplinePoint p, p1, b;
	p = *it++; b = p;
	UpdateBbox(p);

	#ifdef LINEAR_SPLINE_INCREMENT
	const unsigned inc = LINEAR_SPLINE_INCREMENT ;
	#else
	const unsigned inc = 1;
	#endif
	double I(inc);

	curve_cache& L = ccache;
	::clear(L);

	bool close = false;

	while ( it != end() ) {
		// Close flag: take steps to end this
		// sub-curve and start next.
		if ( close ) {
			close = false;

			p = *it++;
			UpdateBbox(p);
			if ( it == end() ) break;

			L.push_back(CurvePt(true)); // add flag for line break
			b = p;
			continue;
		}

		// Get last (current) in set of points.
		p1 = *it++;

		// Condition for closure of current sub-curve.
		if ( (std::abs(p1.x-b.x) < tol) && (std::abs(p1.y-b.y) < tol) ) {
			if ( doclose ) {
				iterator t = it;
				*--t = p1 = b;
			}
			close = true;
		}
		UpdateBbox(p1);

		// Generate interpolation coefficients.

		double A[2], B[2], C[2], D[2];
		A[X] = 0.0;
		B[X] = 0.0;
		C[X] = -1.0 * double(p.x) + 1.0 * double(p1.x);
		D[X] = 1.0 * double(p.x);

		A[Y] = 0.0;
		B[Y] = 0.0;
		C[Y] = -1.0 * double(p.y) + 1.0 * double(p1.y);
		D[Y] = 1.0 * double(p.y);

#		if ! FORCE_CONSTANT_SPLINE_INCREMENT
		// *very* crude estimate of suitable increment
		I = std::ceil((LineLength(p.x, p.y, p1.x, p1.y)) / 8.0);
#		endif // ! FORCE_CONSTANT_SPLINE_INCREMENT
		unsigned incr_u = std::max(0u, unsigned(I));

		// Shift point vars for next loop.
		p = p1;

		// Generate and store line endpoint approximation of segment.
		CurvePt P(D[X], D[Y]);
		wxPoint wxpt = P;
		UpdateBbox(wxpt);
		UpdateCbox(P);
		L.push_back(P);

		for ( unsigned T = 1; T <= incr_u; T++ ) {
			double t  = double(T) / I;
			double t2 = t * t;
			double t3 = t * t2;
			CurvePt p(A[X]*t3+B[X]*t2+C[X]*t+D[X],
				A[Y]*t3+B[Y]*t2+C[Y]*t+D[Y]);
#			if 0
			if ( p == P ) continue;
#			else
			if ( std::abs(p.getX() - P.getX()) < 1.0 &&
			     std::abs(p.getY() - P.getY()) < 1.0)
				continue;
#			endif
			UpdateBbox(wxpt = p);
			UpdateCbox(p);
			L.push_back(p);
		}
	}
}

/* informed by POVRay's prism.c */
void
LinearSpline::Draw(wxDC* dc, const wxRect* bound)
{
	if ( size() < 2 ) return;
	if ( dirty ) {
        CalcCurveCache();
        SetDirty(false);
	}
	DrawCache(dc, ccache, bound);
}

SplineBase*
QuadraticSpline::CopySelf() const
{
	return new QuadraticSpline(*this);
}

bool
QuadraticSpline::Okay() const
{
	return (size() >= 3) ? true : false;
}

bool
QuadraticSpline::POkay(const_subcurve_list* ps) const
{
	if ( size() > 2 ) { // epspline will draw the curve with 3
		if ( objt == undef )
			return true; // no check, this is up to user
	}

	if ( size() > 4 ) { // POV requires 5
		const_iterator i = begin();
		const_iterator e = end();
		const_iterator j = i; // 2nd point and last must match
		const_subcurve c;
		c.first = j;
		i = ++j; ++i;
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// discontinuity
				c.first = j = i; ++j;
				if ( j == e ) return false;
				i = j; ++i;
				if ( i == e ) return false;
			}
		}
		if ( *j == *--i ) {
			if ( ps ) {
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}
	return false;
}

bool
QuadraticSpline::POkay(subcurve_list* ps)
{
	if ( size() > 2 ) { // epspline will draw the curve with 3
		if ( objt == undef )
			return true; // no check, this is up to user
	}

	if ( size() > 4 ) { // POV requires 5
		iterator i = begin();
		iterator e = end();
		iterator j = i; // 2nd point and last must match
		subcurve c;
		c.first = j;
		i = ++j; ++i;
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// discontinuity
				c.first = j = i; ++j;
				if ( j == e ) return false;
				i = j; ++i;
				if ( i == e ) return false;
			}
		}
		if ( *j == *--i ) {
			if ( ps ) {
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}
	return false;
}

bool
QuadraticSpline::DelPoint(const SplinePoint& p)
{
#	if 1
	iterator i = PointIndexIt(p, 0);
	iterator e = end();
	
	if ( i == e ) {
		return false;
	}

	erase(i);
	SetDirty();
	return true;
#	else
	size_type sz = size();
	if ( sz < 6 ) return false;

	iterator i = PointIndexIt(p, 0);
	iterator j = end();
	iterator k = begin();
	if ( i != j && i != --j && i != k && i != ++k ) {
		erase(i);
		SetDirty();
		return true;
	}

	return false;
#	endif
}

bool
QuadraticSpline::AddPoint(const SplinePoint& point)
{
	if ( size() < 3 ) return false;

	wxPoint T;  T.x = irint(point.x); T.y = irint(point.y);
	iterator it = begin();
	SplinePoint p, p1, p2, b;
	bool close = false;
	p = *it++;
	p1= *it++; b = p1;

	#ifdef QUADRATIC_SPLINE_INCREMENT
	const unsigned inc = QUADRATIC_SPLINE_INCREMENT ;
	#else
	const unsigned inc = 30;
	#endif
	double I(inc);

	while ( it != end() ) {
		// Close flag: take steps to end this
		// sub-curve and start next.
		if ( close ) {
			close = false;

			p = *it++;
			if ( it == end() ) break;

			p1 = *it++;
			if ( it == end() ) break;

			b = p1;
			continue;
		}

		// Get last (current) in set of points.
		p2 = *it;

		// Condition for closure of current sub-curve.
		if ( (std::abs(p2.x-b.x) < tol) && (std::abs(p2.y-b.y) < tol) ) {
			close = true;
		}

		// Generate interpolation coefficients.

		double A[2], B[2], C[2], D[2];
		A[X] = 0.0;
		B[X] = 0.5 * double(p.x)
			- 1.0 * double(p1.x) + 0.5 * double(p2.x);
		C[X] = -0.5 * double(p.x) + 0.5 * double(p2.x);
		D[X] = double(p1.x);

		A[Y] = 0.0;
		B[Y] = 0.5 * double(p.y)
			- 1.0 * double(p1.y) + 0.5 * double(p2.y);
		C[Y] = -0.5 * double(p.y) + 0.5 * double(p2.y);
		D[Y] = double(p1.y);

		// Test for point on generated approximation segments
		wxPoint P; // (int(irint(D[X])), int(irint(D[Y])));
		P.x = int(irint(D[X]));
		P.y = int(irint(D[Y]));
		for ( unsigned J = 1; J <= inc; J++ ) {
			wxPoint Q;
			double t  = double(J) / I;
			double t2 = t * t;
			double t3 = t * t2;
			Q.x = int(irint(A[X]*t3+B[X]*t2+C[X]*t+D[X]));
			Q.y = int(irint(A[Y]*t3+B[Y]*t2+C[Y]*t+D[Y]));
			if ( P == Q ) continue;
			if ( PointOnSegPQ(P, Q, T) ) {
				insert(it, point);
				SetDirty();
				return true;
			}
			P = Q;
		}

		// Shift point vars for next loop.
		p = p1;
		p1= p2;
		++it;
	}

	return false;
}

// calculate curve points from current control points,
// and store in cache vector
void
QuadraticSpline::CalcCurveCache()
{
	if ( size() < 3 ) return;
	InitBbox();
	InitCbox();

	iterator it = begin();
	SplinePoint p, p1, p2, b;
	p = *it++;
	p1= *it++; b = p1;
	UpdateBbox(p);
	UpdateBbox(p1);

	#ifdef QUADRATIC_SPLINE_INCREMENT
	const unsigned inc = QUADRATIC_SPLINE_INCREMENT ;
	#else
	const unsigned inc = 30;
	#endif
	double I(inc);

	curve_cache& L = ccache;
	::clear(L);

	bool close = false;

	while ( it != end() ) {
		// Close flag: take steps to end this
		// sub-curve and start next.
		if ( close ) {
			close = false;

			p = *it++;
			UpdateBbox(p);
			if ( it == end() ) break;

			p1 = *it++;
			UpdateBbox(p1);
			if ( it == end() ) break;

			L.push_back(CurvePt(true)); // add flag for line break
			b = p1;
			continue;
		}

		// Get last (current) in set of points.
		p2 = *it++;

		// Condition for closure of current sub-curve.
		if ( (std::abs(p2.x-b.x) < tol) && (std::abs(p2.y-b.y) < tol) ) {
			if ( doclose ) {
				iterator t = it;
				*--t = p2 = b;
			}
			close = true;
		}
		UpdateBbox(p2);

		// Generate interpolation coefficients.

		double A[2], B[2], C[2], D[2];
		A[X] = 0.0;
		B[X] = 0.5 * double(p.x)
			- 1.0 * double(p1.x) + 0.5 * double(p2.x);
		C[X] = -0.5 * double(p.x) + 0.5 * double(p2.x);
		D[X] = double(p1.x);

		A[Y] = 0.0;
		B[Y] = 0.5 * double(p.y)
			- 1.0 * double(p1.y) + 0.5 * double(p2.y);
		C[Y] = -0.5 * double(p.y) + 0.5 * double(p2.y);
		D[Y] = double(p1.y);

#		if ! FORCE_CONSTANT_SPLINE_INCREMENT
		// *very* crude estimate of suitable increment
		I = std::ceil((LineLength(p.x, p.y, p1.x, p1.y)) * 0.5);
#		endif // ! FORCE_CONSTANT_SPLINE_INCREMENT
		unsigned incr_u = std::max(0u, unsigned(I));

		// Shift point vars for next loop.
		p = p1;
		p1= p2;

		// Generate and store line endpoint approximation of segment.
		CurvePt P(D[X], D[Y]);
		wxPoint wxpt = P;
		UpdateBbox(wxpt);
		UpdateCbox(P);
		L.push_back(P);

		for ( unsigned T = 1; T <= incr_u; T++ ) {
			double t  = double(T) / I;
			double t2 = t * t;
			double t3 = t * t2;
			CurvePt p(A[X]*t3+B[X]*t2+C[X]*t+D[X],
				A[Y]*t3+B[Y]*t2+C[Y]*t+D[Y]);
#			if 0
			if ( p == P ) continue;
#			else
			if ( std::abs(p.getX() - P.getX()) < 1.0 &&
			     std::abs(p.getY() - P.getY()) < 1.0)
				continue;
#			endif
			UpdateBbox(wxpt = p);
			UpdateCbox(p);
			L.push_back(p);
		}
	}
}

/* informed by POVRay's prism.c */
void
QuadraticSpline::Draw(wxDC* dc, const wxRect* bound)
{
	if ( size() < 3 ) return;
	if ( dirty ) {
        CalcCurveCache();
        SetDirty(false);
	}
	DrawCache(dc, ccache, bound);
}

SplineBase*
CubicSpline::CopySelf() const
{
	return new CubicSpline(*this);
}

bool
CubicSpline::Okay() const
{
	return (size() >= 4) ? true : false;
}

bool
CubicSpline::POkay(const_subcurve_list* ps) const
{
	if ( size() > 3 ) { // epspline will draw the curve with 4
		if ( objt == undef )
			return true; // no check, this is up to user
	}

	if ( size() > 5 ) { // POV requires 6
		const_iterator i = begin();
		const_iterator e = end();
		const_iterator j = i; // 2nd point and last must match
		const_subcurve c;
		c.first = j;
		i = ++j; ++i;
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					return false;
				}
				if ( ++i == e ) {
					--i;
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// discontinuity
				c.first = j = i; ++j;
				if ( j == e ) return false;
				i = j; ++i;
				if ( i == e ) return false;
			}
		}
		if ( *j == *--i ) {
			if ( ps ) {
				++i;
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}
	return false;
}

bool
CubicSpline::POkay(subcurve_list* ps)
{
	if ( size() > 3 ) { // epspline will draw the curve with 4
		if ( objt == undef )
			return true; // no check, this is up to user
	}

	if ( size() > 5 ) { // POV requires 6
		iterator i = begin();
		iterator e = end();
		iterator j = i; // 2nd point and last must match
		subcurve c;
		c.first = j;
		i = ++j; ++i;
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					return false;
				}
				if ( ++i == e ) {
					--i;
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// discontinuity
				c.first = j = i; ++j;
				if ( j == e ) return false;
				i = j; ++i;
				if ( i == e ) return false;
			}
		}
		if ( *j == *--i ) {
			if ( ps ) {
				++i;
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}
	return false;
}

bool
CubicSpline::DelPoint(const SplinePoint& p)
{
	iterator i = PointIndexIt(p, 0);
	iterator e = end();
	
	if ( i == e ) {
		return false;
	}
	
	erase(i);
	SetDirty();
	return true;
}

bool
CubicSpline::AddPoint(const SplinePoint& point)
{
	if ( size() < 4 ) return false;

	wxPoint T;  T.x = irint(point.x); T.y = irint(point.y);
	iterator it = begin();
	SplinePoint p, p1, p2, p3, b, e;
	p = *it++;
	p1= *it++; b = p1;
	p2= *it++; e = p2;

	bool close = false;

	#ifdef CUBIC_SPLINE_INCREMENT
	const unsigned inc = CUBIC_SPLINE_INCREMENT ;
	#else
	const unsigned inc = 30;
	#endif
	double I(inc);

	while ( it != end() ) {
		// Close flag: take steps to end this
		// sub-curve and start next.
		if ( close ) {
			close = false;

			p = *it++;
			if ( it == end() ) break;

			p1 = *it++;
			if ( it == end() ) break;

			p2 = *it++;
			if ( it == end() ) break;

			b = p1; e = p2;
			continue;
		}

		// Condition for closure of current sub-curve.
		if ( (std::abs(e.x-b.x) < tol) && (std::abs(e.y-b.y) < tol) ) {
			close = true;
		}

		// Get last (current) in set of points.
		p3= *it;

		// Generate interpolation coefficients.

		double A[2], B[2], C[2], D[2];
		A[X] = -0.5 * double(p.x) + 1.5 * double(p1.x)
			- 1.5 * double(p2.x) + 0.5 * double(p3.x);
		B[X] = double(p.x) - 2.5 * double(p1.x)
			+ 2.0 * double(p2.x) - 0.5 * double(p3.x);
		C[X] = -0.5 * double(p.x) + 0.5 * double(p2.x);
		D[X] = double(p1.x);

		A[Y] = -0.5 * double(p.y) + 1.5 * double(p1.y)
			- 1.5 * double(p2.y) + 0.5 * double(p3.y);
		B[Y] = double(p.y) - 2.5 * double(p1.y)
			+ 2.0 * double(p2.y) - 0.5 * double(p3.y);
		C[Y] = -0.5 * double(p.y) + 0.5 * double(p2.y);
		D[Y] = double(p1.y);

		wxPoint P; // (int(irint(D[X])), int(irint(D[Y])));
		P.x = int(irint(D[X]));
		P.y = int(irint(D[Y]));
		for ( unsigned J = 1; J <= inc; J++ ) {
			wxPoint Q;
			double t  = double(J) / I;
			double t2 = t * t;
			double t3 = t * t2;
			Q.x = int(irint(A[X]*t3+B[X]*t2+C[X]*t+D[X]));
			Q.y = int(irint(A[Y]*t3+B[Y]*t2+C[Y]*t+D[Y]));
			if ( P == Q ) continue;
			if ( PointOnSegPQ(P, Q, T) ) {
				insert(--it, point);
				SetDirty();
				return true;
			}
			P = Q;
		}

		++it;

		// Shift point vars for next loop.
		p = p1;
		p1= p2;
		p2= p3; e = p2;
	}

	return false;
}

// calculate curve points from current control points,
// and store in cache vector
void
CubicSpline::CalcCurveCache()
{
	if ( size() < 4 ) return;
	InitBbox();
	InitCbox();

	iterator it = begin();
	SplinePoint p, p1, p2, p3, b, e;
	p = *it++;
	p1= *it++; b = p1;
	p2= *it++; e = p2;
	UpdateBbox(p);
	UpdateBbox(p1);
	UpdateBbox(p2);

	#ifdef CUBIC_SPLINE_INCREMENT
	const unsigned inc = CUBIC_SPLINE_INCREMENT ;
	#else
	const unsigned inc = 30;
	#endif
	double I(inc);

	curve_cache& L = ccache;
	::clear(L);

	bool close = false;

	while ( it != end() ) {
		// Close flag: take steps to end this
		// sub-curve and start next.
		if ( close ) {
			close = false;

			p = *it++;
			UpdateBbox(p);
			if ( it == end() ) break;

			p1 = *it++;
			UpdateBbox(p1);
			if ( it == end() ) break;

			p2 = *it++;
			UpdateBbox(p2);
			if ( it == end() ) break;

			L.push_back(CurvePt(true)); // add flag for line break
			b = p1; e = p2;
			continue;
		}

		// Condition for closure of current sub-curve.
		if ( (std::abs(e.x-b.x) < tol) && (std::abs(e.y-b.y) < tol) ) {
			if ( doclose ) {
				iterator t = it;
				*--t = p2 = b;
			}
			close = true;
		}

		// Get last (current) in set of points.
		p3 = *it++;
		UpdateBbox(p3);

		// Generate interpolation coefficients.

		double A[2], B[2], C[2], D[2];
		A[X] = -0.5 * double(p.x) + 1.5 * double(p1.x)
			- 1.5 * double(p2.x) + 0.5 * double(p3.x);
		B[X] = double(p.x) - 2.5 * double(p1.x)
			+ 2.0 * double(p2.x) - 0.5 * double(p3.x);
		C[X] = -0.5 * double(p.x) + 0.5 * double(p2.x);
		D[X] = double(p1.x);

		A[Y] = -0.5 * double(p.y) + 1.5 * double(p1.y)
			- 1.5 * double(p2.y) + 0.5 * double(p3.y);
		B[Y] = double(p.y) - 2.5 * double(p1.y)
			+ 2.0 * double(p2.y) - 0.5 * double(p3.y);
		C[Y] = -0.5 * double(p.y) + 0.5 * double(p2.y);
		D[Y] = double(p1.y);

#		if ! FORCE_CONSTANT_SPLINE_INCREMENT
		// *very* crude estimate of suitable increment
		I = std::ceil((LineLength(p1.x, p1.y, p2.x, p2.y)) * 0.5);
#		endif // ! FORCE_CONSTANT_SPLINE_INCREMENT
		unsigned incr_u = std::max(0u, unsigned(I));

		// Shift point vars for next loop.
		p = p1;
		p1= p2;
		p2= p3; e = p2;

		// Generate and store line endpoint approximation of segment.
		CurvePt P(D[X], D[Y]);
		wxPoint wxpt = P;
		UpdateBbox(wxpt);
		UpdateCbox(P);
		L.push_back(P);

		for ( unsigned T = 1; T <= incr_u; T++ ) {
			double t  = double(T) / I;
			double t2 = t * t;
			double t3 = t * t2;
			CurvePt p(A[X]*t3+B[X]*t2+C[X]*t+D[X],
				A[Y]*t3+B[Y]*t2+C[Y]*t+D[Y]);
#			if 0
			if ( p == P ) continue;
#			else
			if ( std::abs(p.getX() - P.getX()) < 1.0 &&
			     std::abs(p.getY() - P.getY()) < 1.0)
				continue;
#			endif
			UpdateBbox(wxpt = p);
			UpdateCbox(p);
			L.push_back(p);
		}
	}
}

/* informed by POVRay's prism.c */
void
CubicSpline::Draw(wxDC* dc, const wxRect* bound)
{
	if ( size() < 4 ) return;
	if ( dirty ) {
        CalcCurveCache();
        SetDirty(false);
	}
	DrawCache(dc, ccache, bound);
}

SplineBase*
BezierSpline::CopySelf() const
{
	return new BezierSpline(*this);
}

bool
BezierSpline::Okay() const
{
	return (size() >= 4) ? true : false;
}

bool
BezierSpline::POkay(const_subcurve_list* ps) const
{
	const size_type sz = size();
	const size_type bitsng = 3u;
	
	if ( sz > 3 ) { // epspline will draw the curve with 4
		if ( objt == undef )
			return true; // no check, this is up to user
	}

	if ( ! ps && (sz < 4 || (sz & bitsng)) ) {
		return false;
	} else if ( sz == 4 ) {
		const_iterator i = begin();
		const_iterator e = end();
		const_subcurve c(i, e);
		if ( *i == *--e ) {
			// ensure 2 dimensions
			const_iterator p1 = i; ++p1;
			const_iterator p2 = p1; ++p2;
			if ( *i != *p1 && *i != *p2 && *p1 != *p2 ) {
				if ( ps ) {
					ps->push_back(c);
				}
				return true;
			}
		}
	} else { // sz >= 8
		const_iterator i = begin();
		const_iterator e = end();
		const_iterator j = i; // 1st point and last must match
		const_subcurve c(i, e);
		++i; ++i; // advance to 3rd here, while loop advances to 4th
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// valid discontinuity: set j 1st of next segment
				c.first = j = i--;
			}
			// advance to 3rd in next segment
			// test not needed since mult. of 4 is confirmed,
			// but leave defensively against code changes.
			if ( ++i == e ) return false;
			if ( ++i == e ) return false;
			if ( ++i == e ) return false;
		}
		
		// closed path: OK
		if ( *j == *--i ) {
			if ( ps ) {
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}

	return false;
}

bool
BezierSpline::POkay(subcurve_list* ps)
{
	const size_type sz = size();
	const size_type bitsng = 3u;

	if ( sz > 3 ) { // epspline will draw the curve with 4
		if ( objt == undef )
			return true; // no check, this is up to user
	}

	if ( ! ps && (sz < 4 || (sz & bitsng)) ) {
		return false;
	} else if ( sz < 5 ) {
		iterator i = begin();
		iterator e = end();
		subcurve c(i, e);
		if ( *i == *--e ) {
			// ensure 2 dimensions
			iterator p1 = i; ++p1;
			iterator p2 = p1; ++p2;
			if ( *i != *p1 && *i != *p2 && *p1 != *p2 ) {
				if ( ps ) {
					ps->push_back(c);
				}
				return true;
			}
		}
	} else { // sz >= 8
		iterator i = begin();
		iterator e = end();
		iterator j = i; // 1st point and last must match
		subcurve c(i, e);
		++i; ++i; // advance to 3rd here, while loop advances to 4th
		while ( ++i != e ) {
			if ( *i == *j ) {
				if ( ++i == e ) {
					break;
				}
				if ( ps ) {
					c.second = i;
					ps->push_back(c);
				}
				// valid discontinuity: set j 1st of next segment
				c.first = j = i--;
			}
			// advance to 3rd in next segment
			// test not needed since mult. of 4 is confirmed,
			// but leave defensively against code changes.
			if ( ++i == e ) return false;
			if ( ++i == e ) return false;
			if ( ++i == e ) return false;
		}
		
		// closed path: OK
		if ( *j == *--i ) {
			if ( ps ) {
				c.second = ++i;
				ps->push_back(c);
			}
			return true;
		}
	}

	return false;
}

bool
BezierSpline::DelPoint(const SplinePoint& p)
{
	iterator pti = PointIndexIt(p);
	
	if ( pti == end() ) {
		return false;
	}

	iterator i = begin(), l = end();
	subcurve_list sl;
	bool allok = get_subcurves(&sl);

	if ( allok == false ) {
		iterator c = i;
		// point count is not a multiple of 4,
		// so curve is not currently valid;
		// get_subcurves will return false, but
		// not before collecting subset of subcurves
		subcurve_list::iterator b = sl.begin(), e = sl.end();
		while ( b != e ) {
			subcurve& sc = *b++;
			c = std::find(sc.first, sc.second, p);
			if ( c != sc.second ) {
				// found in valid sub
				i = sc.first;
				l = sc.second;
				goto BezierSplineDelPoint_delseg;
			}
		}
		// not found, it's in remainder
		sl.clear();
		while ( (end() - c) > 3 ) {
			iterator tmpc = std::find(c + 2, end(), *c);
			if ( tmpc++ != end() && ((tmpc - c) & 3) == 0 ) {
				sl.push_back(subcurve(c, tmpc));
				c = tmpc;
				continue;
			}
			++c;
		}
		// try again with subcurve list
		b = sl.begin(), e = sl.end();
		while ( b != e ) {
			subcurve& sc = *b++;
			c = std::find(sc.first, sc.second, p);
			if ( c != sc.second ) {
				// found in valid sub
				i = sc.first;
				l = sc.second;
				goto BezierSplineDelPoint_delseg;
			}
		}
		// just remove the point and let user
		// work on it.
		erase(pti);
		SetDirty();
		return true;
	} else {
		subcurve_list::iterator b = sl.begin(), e = sl.end();
		while ( b != e ) {
			subcurve& sc = *b++;
			iterator c = std::find(sc.first, sc.second, p);
			if ( c != sc.second ) {
				// found in valid sub
				i = sc.first;
				l = sc.second;
				goto BezierSplineDelPoint_delseg;
			}
		}
		return false;
	}

BezierSplineDelPoint_delseg:
	// one seg subcurve?
	if ( (i + 4) == l ) {
		erase(i, l);
	    SetDirty();
		return true;
	}

	int n = pti - i;
	int m = n % 4;
	iterator b = i;
	i += (n - m);                     // point to start of seg w/ pti
	iterator ni = i + 4;              // ni == start of next seg

	iterator berase = i, eerase = ni;
	// erasing 1st seg, last seg, or in-between
	if ( i == b ) {
		iterator e = l - 1;
		SplinePoint& t0 = *ni, & t1 = *e;
		SplinePoint tp((t0.x+t1.x)/2.0, (t0.y+t1.y)/2.0);
		t0 = t1 = tp;
	} else if ( ni == l ) {
		iterator e = l - 5;
		SplinePoint& t0 = *b, & t1 = *e;
		SplinePoint tp((t0.x+t1.x)/2.0, (t0.y+t1.y)/2.0);
		t0 = t1 = tp;
	} else {
		iterator s = i - 1;
		SplinePoint& t0 = *s, & t1 = *ni;
		SplinePoint tp((t0.x+t1.x)/2.0, (t0.y+t1.y)/2.0);
		t0 = t1 = tp;
	}

	erase(berase, eerase);
    SetDirty();
	return true;
}

bool
BezierSpline::AddPoint(const SplinePoint& point)
{
	const size_type sz = size();
	const size_type bitsng = 3u;
	
	if ( sz < 4 ) return false;

	// if curve isn't POV-Okay assume edit/fixing,
	if ( (sz & bitsng) || ! POkay() ) {
		// let caller handle it
		return false;
	}

	// from here: try to add segment for point selection on path
	#ifdef BEZIER_SPLINE_INCREMENT
	const unsigned inc = BEZIER_SPLINE_INCREMENT ;
	#else
	const unsigned inc = 60;
	#endif
	double I(inc);

	wxPoint T;  T.x = irint(point.x); T.y = irint(point.y);
	iterator it = begin();
	SplinePoint p, p1, p2, p3, b;

	bool close = true;

	while ( it != end() ) {
		p = *it++;
		if ( it == end() ) break;
		// Close flag: take steps to end this
		// sub-curve and start next.
		if ( close ) {
			close = false;
			b = p;
		}

		p1= *it++;
		if ( it == end() ) break;
		p2= *it++;
		if ( it == end() ) break;
		p3= *it;

		// Condition for closure of current sub-curve.
		if ( (std::abs(p3.x-b.x) < tol) && (std::abs(p3.y-b.y) < tol) ) {
			close = true;
		}

		// Generate interpolation coefficients.

		double A[2], B[2], C[2], D[2];
		A[X] = double(p3.x) -
			3.0 * double(p2.x) + 3.0 * double(p1.x) - double(p.x);
		B[X] = 3.0 * double(p2.x) -
			6.0 * double(p1.x) + 3.0 * double(p.x);
		C[X] = 3.0 * double(p1.x) - 3.0 * double(p.x);
		D[X] = p.x;

		A[Y] = double(p3.y) -
			3.0 * double(p2.y) + 3.0 * double(p1.y) - double(p.y);
		B[Y] = 3.0 * double(p2.y) -
			6.0 * double(p1.y) + 3.0 * double(p.y);
		C[Y] = 3.0 * double(p1.y) - 3.0 * double(p.y);
		D[Y] = p.y;

		wxPoint P;
		// Generate and store line endpoint approximation of segment.
		P.x = int(irint(D[X]));
		P.y = int(irint(D[Y]));
		for ( unsigned J = 1; J <= inc; J++ ) {
			wxPoint Q;
			double t  = double(J) / I;
			double t2 = t * t;
			double t3 = t * t2;
			Q.x = int(irint(A[X]*t3+B[X]*t2+C[X]*t+D[X]));
			Q.y = int(irint(A[Y]*t3+B[Y]*t2+C[Y]*t+D[Y]));
			if ( P == Q ) continue;
			if ( PointOnSegPQ(P, Q, T) ) {
				// Can't insert one point,
				// must create new segment!
				// Adjust iterator to p1
				--it; --it;
				// This is a terribly crude way to
				// adjust old control points and
				// generate new ones.
				// For now it will have to do.
				SplinePoint tp(P.x, P.y), tq(Q.x, Q.y);
				tp.x = p1.x - ((p1.x - p.x) * 2) / 3;
				tp.y = p1.y - ((p1.y - p.y) * 2) / 3;
				*it++ = tp;
				tp.x = p1.x - ((p1.x - p.x) * 1) / 3;
				tp.y = p1.y - ((p1.y - p.y) * 1) / 3;

				tq.x = p2.x + ((p3.x - p2.x) * 2) / 3;
				tq.y = p2.y + ((p3.y - p2.y) * 2) / 3;
				*it = tq;
				tq.x = p2.x + ((p3.x - p2.x) * 1) / 3;
				tq.y = p2.y + ((p3.y - p2.y) * 1) / 3;

				it = insert(it, tq);
				it = insert(it, point);
				it = insert(it, point);
				insert(it, tp);

				SetDirty();
				return true;
			}
			P = Q;
		}

		++it;
	}

	return false;
}

// calculate curve points from current control points,
// and store in cache vector
void
BezierSpline::CalcCurveCache()
{
	if ( size() < 4 ) return;
	InitBbox();
	InitCbox();

	iterator it = begin();
	iterator l3 = begin();
	SplinePoint p, p1, p2, p3, b, last;

	#ifdef BEZIER_SPLINE_INCREMENT
	const unsigned inc = BEZIER_SPLINE_INCREMENT ;
	#else
	const unsigned inc = 60;
	#endif
	double I(inc);

	curve_cache& L = ccache;
	::clear(L);

	bool close = true;
	unsigned c = 0;

	while ( it != end() ) {
		p = *it++;
		UpdateBbox(p);
		if ( it == end() ) break;
		// Close flag: take steps to end this
		// sub-curve and start next.
		if ( close ) {
			close = false;
			L.push_back(CurvePt(true)); // add flag for line break
			b = p;
		}

		p1 = *it++;
		UpdateBbox(p1);
		if ( it == end() ) break;
		p2 = *it++;
		UpdateBbox(p2);
		if ( it == end() ) break;
		p3 = *it++;
		UpdateBbox(p3);

		// Condition for closure of current sub-curve.
		if ( (std::abs(p3.x-b.x) < tol)
			&& (std::abs(p3.y-b.y) < tol) ) {
			if ( doclose ) {
				iterator t = it;
				*--t = p3 = b;
			}
			close = true;
		}

		// Also close at last segment endpoint
		if ( doclose && c
			&& (std::abs(p.x-last.x) < tol)
			&& (std::abs(p.y-last.y) < tol) ) {
			*l3 = p;
		}
		++c;
		l3 = it;
		--l3;
		last = p3;

		// Generate interpolation coefficients.

		double A[2], B[2], C[2], D[2];
		A[X] = double(p3.x) -
			3.0 * double(p2.x) + 3.0 * double(p1.x) - double(p.x);
		B[X] = 3.0 * double(p2.x) -
			6.0 * double(p1.x) + 3.0 * double(p.x);
		C[X] = 3.0 * double(p1.x) - 3.0 * double(p.x);
		D[X] = p.x;

		A[Y] = double(p3.y) -
			3.0 * double(p2.y) + 3.0 * double(p1.y) - double(p.y);
		B[Y] = 3.0 * double(p2.y) -
			6.0 * double(p1.y) + 3.0 * double(p.y);
		C[Y] = 3.0 * double(p1.y) - 3.0 * double(p.y);
		D[Y] = p.y;

		// Generate and store line endpoint approximation of segment.
		CurvePt P(D[X], D[Y]);
		wxPoint wxpt = P;
		UpdateBbox(wxpt);
		UpdateCbox(P);
		L.push_back(P);

#		if ! FORCE_CONSTANT_SPLINE_INCREMENT
		// crude estimate of suitable increment
		// (tried divisor 10.0: too much)
		I = std::ceil((LineLength(p.x, p.y, p3.x, p3.y)
			+ LineLength(p1.x, p1.y, p2.x, p2.y)) / 5.0);
#		endif // ! FORCE_CONSTANT_SPLINE_INCREMENT
		unsigned incr_u = std::max(0u, unsigned(I));

		for ( unsigned T = 1; T <= incr_u; T++ ) {
			double t  = double(T) / I;
			double t2 = t * t;
			double t3 = t * t2;
			CurvePt p(A[X]*t3+B[X]*t2+C[X]*t+D[X],
				A[Y]*t3+B[Y]*t2+C[Y]*t+D[Y]);
#			if 0
			if ( p == P ) continue;
#			else
			if ( std::abs(p.getX() - P.getX()) < 1.0 &&
			     std::abs(p.getY() - P.getY()) < 1.0)
				continue;
#			endif
			UpdateBbox(wxpt = p);
			UpdateCbox(p);
			L.push_back(p);
			P = p;
		}
	}
}

/* informed by POVRay's prism.c */
void
BezierSpline::Draw(wxDC* dc, const wxRect* bound)
{
	if ( size() < 4 ) return;
	if ( dirty ) {
        CalcCurveCache();
        SetDirty(false);
	}
	DrawCache(dc, ccache, bound);
}

void
BezierSpline::DrawDots(wxDC& dc, wxBrush br, unsigned size)
{
#if wxCHECK_VERSION(2, 6, 0)
	const wxBrush& b = dc.GetBrush();
#else
	wxBrush& b = dc.GetBrush();
#endif
	dc.SetBrush(br);
	DrawDots(dc, size);
	dc.SetBrush(b);
}

void
BezierSpline::DrawDots(wxDC& dc, unsigned size)
{
	iterator it = begin();
	while ( it != end() ) {
		SplinePoint& p = *it++;
		dc.DrawCircle(IRINT(p.x), IRINT(p.y), size);
	}
}

void
BezierSpline::DrawSelectedDot(wxDC& dc, const SplinePoint& pt, unsigned dsize)
{
	int index = PointIndex(pt, 0);
	if ( index < 0 ) {
		// Not ours, but what the hell
		dc.DrawRectangle(IRINT(pt.x)-(dsize>>1), IRINT(pt.y)-(dsize>>1)
			, dsize, dsize);
		return;
	}

	iterator it = begin();
	unsigned c0, e0, c1, e1, sz = size(), idx(index);
	unsigned mod = idx % 4;
	switch ( mod ) {
		case 0: // first endpoint of segment
			e0 = idx;
			c0 = e0 + 1;
			break;
		case 1: // first control of segment
			c0 = idx;
			e0 = c0 - 1;
			break;
		case 2: // second control of segment
			c0 = idx;
			e0 = c0 + 1;
			break;
		default:// second endpoint of segment
			e0 = idx;
			c0 = e0 - 1;
	}

	SplinePoint& E0 = it[e0];
	// forward search for companion
	for ( e1 = e0 + (mod & 2 ? 1 : 3); e1 < sz; e1 += 4 )
		if ( it[e1] == E0 )
			break;
	if ( e1 >= sz ) {
		// backward search for companion
		signed t;
		for ( e1 = e0 - (mod & 2 ? 3 : 1), t = e1;
			t >= 0; e1 -= 4, t -= 4 )
			if ( it[e1] == E0 )
				break;
		if ( t < 0 ) {
			//fprintf(stderr, wxT("unclosed bezier\n"));
			dc.DrawRectangle(IRINT(pt.x)-(dsize>>1)
				, IRINT(pt.y)-(dsize>>1)
				, dsize, dsize);
			return;
		}
	}
	c1 = (mod & 2 ? e1 + 1 : e1 - 1);

	unsigned csz = dsize >> 1;
	SplinePoint& C0 = it[c0];
	SplinePoint& C1 = it[c1], & E1 = it[e1];

	wxPen orig = dc.GetPen();
	wxPen npen(*wxGREY_PEN);
	#if defined(__WXMSW__)
	npen.SetStyle(wxDOT);
	#else
	npen.SetStyle(wxSHORT_DASH);
	#endif
	dc.SetPen(npen);

	dc.DrawLine(IRINT(C0.x), IRINT(C0.y), IRINT(E0.x), IRINT(E0.y));
	dc.DrawLine(IRINT(C1.x), IRINT(C1.y), IRINT(E1.x), IRINT(E1.y));

	dc.SetPen(orig);

	dc.DrawCircle(IRINT(C0.x), IRINT(C0.y), csz);
	dc.DrawCircle(IRINT(E0.x), IRINT(E0.y), csz);
	dc.DrawCircle(IRINT(C1.x), IRINT(C1.y), csz);
	dc.DrawCircle(IRINT(E1.x), IRINT(E1.y), csz);

	dc.DrawRectangle(IRINT(pt.x)-csz, IRINT(pt.y)-csz, dsize, dsize);
}

bool
BezierSpline::MovePoint(SplinePoint& pt, double xoff, double yoff)
{
	int index = PointIndex(pt, 0);
	if ( index < 0 ) {
		// Not ours
		return false;
	}

	iterator it = begin();
	unsigned c0, e0, c1, e1, sz = size(), idx(index);
	unsigned mod = idx % 4;
	switch ( mod ) {
		case 0: // first endpoint of segment
			e0 = idx;
			c0 = e0 + 1;
			break;
		case 1: // first control of segment
			c0 = idx;
			e0 = c0 - 1;
			break;
		case 2: // second control of segment
			c0 = idx;
			e0 = c0 + 1;
			break;
		default:// second endpoint of segment
			e0 = idx;
			c0 = e0 - 1;
	}

	SplinePoint& E0 = it[e0];
	// forward search for companion
	for ( e1 = e0 + (mod & 2 ? 1 : 3); e1 < sz; e1 += 4 )
		if ( it[e1] == E0 )
			break;
	if ( e1 >= sz ) {
		// backward search for companion
		signed t;
		for ( e1 = e0 - (mod & 2 ? 3 : 1), t = e1;
			t >= 0; e1 -= 4, t -= 4 )
			if ( it[e1] == E0 )
				break;
		if ( t < 0 ) {
#			if 1
			//fprintf(stderr, "unclosed bezier\n");
			it[index].x += xoff;
			it[index].y += yoff;
			SetDirty();
			return true;
#			else
			//fprintf(stderr, "unclosed bezier\n");
			return false;
#			endif
		}
	}
	c1 = (mod & 2 ? e1 + 1 : e1 - 1);

	SplinePoint& C0 = it[c0];
	SplinePoint& C1 = it[c1], & E1 = it[e1];

	if ( dotlock && (mod == 1 || mod == 2) ) {
		C0.x += xoff; C0.y += yoff;
		double Qx = C1.x, Qy = C1.y;
		FindOppositeRayEnd(C0.x, C0.y, E0.x, E0.y, Qx, Qy);
		C1.x = Qx;
		C1.y = Qy;
	} else if ( dotlock ) {
		E0.x += xoff; E0.y += yoff;
		C0.x += xoff; C0.y += yoff;
		E1.x += xoff; E1.y += yoff;
		C1.x += xoff; C1.y += yoff;
	} else {
		pt.x += xoff;
		pt.y += yoff;
		if ( mod == 0 || mod == 3 ) {
			E1.x += xoff;
			E1.y += yoff;
		}
	}

    SetDirty();
	return true;
}

void
BezierSpline::DrawSelectedDot(wxDC& dc
	, wxBrush br, const SplinePoint& pt, unsigned size)
{
#if wxCHECK_VERSION(2, 6, 0)
	const wxBrush& b = dc.GetBrush();
#else
	wxBrush& b = dc.GetBrush();
#endif
	dc.SetBrush(br);
	DrawSelectedDot(dc, pt, size);
	dc.SetBrush(b);
}

void
BezierSpline::DrawDotsSelectedDot(wxDC& dc
	, const SplinePoint& pt, wxBrush selectedbr
	, unsigned size, unsigned selsize)
{
	DrawDots(dc, size);
	DrawSelectedDot(dc, selectedbr, pt, selsize);
}

