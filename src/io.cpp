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



#include <wx/wx.h>
#include <wx/filename.h>
#include "wxexio.h"
#include "wxutil.h"
#include "a_bgimg_manager.h"

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <limits>
#include <cctype>
#include <cerrno>
#if defined(__WXMSW__) && defined(__SC__) // old.
#include <float.h>  // for FLT_MIN
#else
#include <cfloat>
#endif

// for setting "C" locale in a block
#include "clocnumeric.h"

namespace {
	inline wxString wxsani(const wxString& s)
	{
		std::string ss(wxs2ch(s));
		return ch2wxs(sanitise_string(ss, ss).c_str());
	}
}

// Must check and sanitize the real numbers, as
// extreme values can cause big trouble like infinite loops,
// as dicovered using input from a T1 font conversion, with
// a bug that made values like 5.20004e+08; while fixes
// are needed at the point of the infinite loop (I don't even
// know now if it's in app code or wx code), sanitization
// should happen here anyway.
// Set the function pointer 'io_sanitise_spline_point' to
// something better than the default.
// (App is not presently multi-threaded; if threads are eventually
// employed, the io code had better be used by one thread, likely
// the main thread.)
//
// Return of 0 is OK, failure non-zero is assigned to errno,
// so it should be useful in that role -- probably EINVAL.
int (*io_sanitise_spline_point)(void*, SplinePoint*) = 0;
void* io_sanitise_spline_point_data = 0;

inline int sanitise_spline_point(SplinePoint& pt)
{
	if ( io_sanitise_spline_point != 0 ) {
		return io_sanitise_spline_point(
			io_sanitise_spline_point_data, &pt);
	}

	// arbitrary default maximum; use io_sanitise_spline_point!
	const double maxV = 65535.0;
	if ( std::abs(pt.x) > maxV )
		return EINVAL;
	if ( std::abs(pt.y) > maxV )
		return EINVAL;
	// NOTE: float epsilon, although double is tested; serves as
	// minimum for this default proc.
	const double minV = std::numeric_limits<float>::epsilon();
	if ( std::abs(pt.x) <= minV )
		pt.x = pt.x < 0.0 ? -0.0 : 0.0;
	if ( std::abs(pt.y) <= minV )
		pt.y = pt.y < 0.0 ? -0.0 : 0.0;
	return 0;
}

// Data file format version:
const long format_version = 0x00000003L;

// Real number formatting precision (decimal part)
const int default_real_fmt_precision = 8;

bool
WriteData(const wxString& fname, const std::list<SplineBase*>& lst
	, const std::vector<int>& hg, const std::vector<int>& vg
	, const IO_AddlData* addl, const wxString* pcomment)
{
	errno = 0;
	cnumtmp c_tmp;
	int real_fmt_precision = default_real_fmt_precision;
	wxString real_fmt;
	auto_std_FILE o(wxs2fn(fname), "wb"); // MS reads fail w/ "wt"

	if ( ! o ) {
		int e = errno;
		::perror(wxs2fn(fname));
		errno = e;
		return false;
	}
	
	real_fmt.Printf(wxT("%%.%df"), real_fmt_precision);

	wxExioDatabase db;

	wxExio* cnt = new wxExio(wxT("FileData"));
	cnt->AddAttributeValue(wxT("FileFormatVersion"), format_version);
	cnt->AddAttributeValue(wxT("ObjectCount"), (long)lst.size());
	cnt->AddAttributeValue(wxT("HGuideCount"), (long)hg.size());
	cnt->AddAttributeValue(wxT("VGuideCount"), (long)vg.size());

	if ( format_version >= 0x3 && addl && addl->init ) {
		long tlong = static_cast<long>(addl->scrollpos_h);
		cnt->AddAttributeValue(wxT("CanvasHScroll"), tlong);
		tlong = static_cast<long>(addl->scrollpos_v);
		cnt->AddAttributeValue(wxT("CanvasVScroll"), tlong);
		tlong = static_cast<long>(addl->scale);
		cnt->AddAttributeValue(wxT("CanvasScale"), tlong);
		
		if ( addl->bgm ) {
			wxString name;
			addl->bgm->get_file(name);
			if ( addl->bgm->get_copy_orig() ||
				 addl->bgm->get_copy_changes() ) {
				// in this case do not save directory
				name = wxFileName(name).GetFullName();
			}
			cnt->AddAttributeValueString(
				wxT("CanvasBGFName"), wxsani(name));

			tlong = static_cast<long>(addl->bgm->get_params());
			cnt->AddAttributeValue(wxT("CanvasBGParams"), tlong);

			bgimg_manager::dim_type dtw, dth;
			bgimg_manager::off_type otx, oty;
			addl->bgm->get_dimensions(dtw, dth, otx, oty);
			tlong = static_cast<long>(dtw);
			cnt->AddAttributeValue(wxT("CanvasBGImgWi"), tlong);
			tlong = static_cast<long>(dth);
			cnt->AddAttributeValue(wxT("CanvasBGImgHi"), tlong);
			tlong = static_cast<long>(otx);
			cnt->AddAttributeValue(wxT("CanvasBGImgOffX"), tlong);
			tlong = static_cast<long>(oty);
			cnt->AddAttributeValue(wxT("CanvasBGImgOffY"), tlong);

			bgimg_manager::hsv_type hsvh, hsvs, hsvv;
			addl->bgm->get_hsv_vals(hsvh, hsvs, hsvv);
			tlong = static_cast<long>(hsvh);
			cnt->AddAttributeValue(wxT("CanvasBGImgHueH"), tlong);
			tlong = static_cast<long>(hsvs);
			cnt->AddAttributeValue(wxT("CanvasBGImgHueS"), tlong);
			tlong = static_cast<long>(hsvv);
			cnt->AddAttributeValue(wxT("CanvasBGImgHueV"), tlong);

			bgimg_manager::cmp_type cmp;
			addl->bgm->get_compression(cmp);
			tlong = static_cast<long>(cmp);
			cnt->AddAttributeValue(wxT("CanvasBGImgLightness"), tlong);

			bgimg_manager::off_type rot;
			addl->bgm->get_rotation(rot);
			tlong = static_cast<long>(rot);
			cnt->AddAttributeValue(wxT("CanvasBGImgRotate"), tlong);
		}
	}

	db.Append(cnt);

	int nobj = 0;
	std::list<SplineBase*>::const_iterator it = lst.begin();
	while ( it != lst.end() ) {
		const SplineBase* p = *it++;
		wxString ob(wxT("Object"));

		ob << nobj++;
		wxExio& e  = *new wxExio(ob);
		e.AddAttributeValueString(wxT("objname")
			, wxsani(p->GetObNam()));
		e.AddAttributeValue(wxT("splinetype"), (long)p->Getsplinet());
		e.AddAttributeValue(wxT("povtype"), (long)p->Getobjt());
		e.AddAttributeValue(wxT("sweeptype"), (long)p->Getsweept());
		e.AddAttributeValue(wxT("use_sturm"), (long)p->Getusesturm());
		e.AddAttributeValue(wxT("use_open"), (long)p->Getuseopen());
		wxString swv; swv.Printf(real_fmt, p->Getsweepmin());
		e.AddAttributeValueString(wxT("sweep_min"), wxsani(swv));
		swv = wxT(""); swv.Printf(real_fmt, p->Getsweepmax());
		e.AddAttributeValueString(wxT("sweep_max"), wxsani(swv));
		if ( SplineBase::NotStringFieldEmpty(p->GetTransform()) )
			e.AddAttributeValueString(p->PropNames[p->transform]
				, wxsani(p->GetTransform()));
		if ( SplineBase::NotStringFieldEmpty(p->GetTexture()) )
			e.AddAttributeValueString(p->PropNames[p->texture]
				, wxsani(p->GetTexture()));
		if ( SplineBase::NotStringFieldEmpty(p->GetInterior()) )
			e.AddAttributeValueString(p->PropNames[p->interior]
				, wxsani(p->GetInterior()));
		if ( SplineBase::NotStringFieldEmpty(p->GetUserStr()) )
			e.AddAttributeValueString(p->PropNames[p->userstr]
				, wxsani(p->GetUserStr()));

		int npt = 0;
		SplineBaseBase::const_iterator i = p->begin();
		while ( i != p->end() ) {
			SplinePoint pt = *i++;
			wxString t, t0;

			t = wxT("U");
			t << npt;
			t0 = wxT("");
			t0.Printf(real_fmt, pt.x);
			e.AddAttributeValueString(t, t0);

			t = wxT("V");
			t << npt;
			t0 = wxT("");
			t0.Printf(real_fmt, pt.y);
			e.AddAttributeValueString(t, t0);

			++npt;
		}

		e.AddAttributeValue(wxT("UVcount"), (long)npt);

		db.Append(&e);
	}

	if ( hg.size() > 0 ) {
		wxExio& ex = *new wxExio(wxT("HGuides"));
		int n = 0;
		std::vector<int>::const_iterator i, e;

		i = hg.begin(); e = hg.end();
		while ( i != e ) {
			int g = *i++;
			wxString t(wxT("h"));

			t << n++;
			ex.AddAttributeValue(t, (long)g);
		}
		db.Append(&ex);
	}

	if ( vg.size() > 0 ) {
		wxExio& ex = *new wxExio(wxT("VGuides"));
		int n = 0;
		std::vector<int>::const_iterator i, e;

		i = vg.begin(); e = vg.end();
		while ( i != e ) {
			int g = *i++;
			wxString t(wxT("v"));

			t << n++;
			ex.AddAttributeValue(t, (long)g);
		}
		db.Append(&ex);
	}

	if ( pcomment ) {
		// Regardless of count, allow just one for now
		wxExio* ex = new wxExio(wxT("FileComments"));
		ex->AddAttributeValue(wxT("CommentCount"), 1l);
		ex->AddAttributeValueString(wxT("Comment1"), wxsani(*pcomment));
		db.Append(ex);
	}

	db.Write(o);

	return true;
}

int
ReadData(const wxString& fname, std::list<SplineBase*>& lst
	, std::vector<int>& hg, std::vector<int>& vg
	, IO_AddlData* addl, wxString* pcomment)
{
	cnumtmp c_tmp;
	int n;
	wxExioDatabase db;

	errno = 0;

	if ( !db.Read(fname) ) {
		// int e = errno; cannot rely on errno being set
		// by open(2) or read(2): lexer code uses isatty(2)
		// which sets ENOTTY, and puzzlingly EAGAIN on
		// GNU/Linux and OpenBSD . . .
		fprintf(stderr, "Failed reading %s\n", wxs2fn(fname));
		// errno = e; . . . so just set EINVAL
		errno = EINVAL;
		return -1;
	}
	db.BeginFind();

	wxExio* pe = db.FindClauseByFunctor(wxT("FileData"));
	if ( !pe ) {
		fprintf(stderr, "Failed on %s\n", wxs2ch(fname));
		errno = EINVAL;
		return -1;
	}

	long fvers = 0L;
	pe->GetAttributeValue(wxT("FileFormatVersion"), fvers);
	int nobj = 0;
	pe->GetAttributeValue(wxT("ObjectCount"), nobj);
	int nhg = 0;
	pe->GetAttributeValue(wxT("HGuideCount"), nhg);
	int nvg = 0;
	pe->GetAttributeValue(wxT("VGuideCount"), nvg);

	if ( fvers >= 0x3L && addl ) {
		int tint = 0;
		pe->GetAttributeValue(wxT("CanvasHScroll"), tint);
		addl->scrollpos_h = tint;
		tint = 0;
		pe->GetAttributeValue(wxT("CanvasVScroll"), tint);
		addl->scrollpos_v = tint;
		tint = 0;
		pe->GetAttributeValue(wxT("CanvasScale"), tint);
		addl->scale = tint;
		
		if ( addl->bgm ) {
			long tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGParams"), tlong);
			addl->bgm->set_params(static_cast<unsigned long>(tlong));
			bgimg_manager::dim_type dtw;

			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgWi"), tlong);
			dtw = static_cast<bgimg_manager::dim_type>(tlong);
			bgimg_manager::dim_type dth;
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgHi"), tlong);
			dth = static_cast<bgimg_manager::dim_type>(tlong);
			bgimg_manager::off_type otx;
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgOffX"), tlong);
			otx = static_cast<bgimg_manager::off_type>(tlong);
			bgimg_manager::off_type oty;
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgOffY"), tlong);
			oty = static_cast<bgimg_manager::off_type>(tlong);
			addl->bgm->set_dimensions(dtw, dth, otx, oty);

			bgimg_manager::hsv_type hsvh, hsvs, hsvv;
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgHueH"), tlong);
			hsvh = static_cast<bgimg_manager::hsv_type>(tlong);
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgHueS"), tlong);
			hsvs = static_cast<bgimg_manager::hsv_type>(tlong);
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgHueV"), tlong);
			hsvv = static_cast<bgimg_manager::hsv_type>(tlong);
			addl->bgm->set_hsv_vals(hsvh, hsvs, hsvv);

			bgimg_manager::cmp_type cmp;
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgLightness"), tlong);
			cmp = static_cast<bgimg_manager::cmp_type>(tlong);
			addl->bgm->set_compression(cmp);

			bgimg_manager::off_type rot;
			tlong = 0;
			pe->GetAttributeValue(wxT("CanvasBGImgRotate"), tlong);
			rot = static_cast<bgimg_manager::off_type>(tlong);
			addl->bgm->set_rotation(rot);

			wxString name;
			pe->GetAttributeValue(wxT("CanvasBGFName"), name);
			if ( addl->bgm->get_copy_orig() ||
				 addl->bgm->get_copy_changes() ) {
				// in this case directory was not
				// saved, and file is expected in
				// same dir as .pse file.
				name = wxFileName(
					wxFileName(fname).GetPath(),
					wxFileName(name).GetFullName()
				).GetFullPath();
			}
			addl->bgm->set_file(name);
		}

		addl->init = true;
	}

	for ( n = 0; n < nobj; n++ ) {
		wxString ob(wxT("Object"));

		ob << n;
		pe = db.FindClauseByFunctor(ob);
		if ( !pe ) {
			fprintf(stderr, "Null return on %s\n", wxs2ch(ob));
			continue;
		}
		wxString t(wxT(""));
		pe->GetAttributeValue(wxT("objname"), t);
		if ( t == wxT("") ) continue;

		int ti = -1;
		pe->GetAttributeValue(wxT("splinetype"), ti);

		SplineBase* p;
		switch ( ti ) {
			case SplineBase::linear:
				p = new LinearSpline(t); break;
			case SplineBase::quadratic:
				p = new QuadraticSpline(t); break;
			case SplineBase::cubic:
				p = new CubicSpline(t); break;
			case SplineBase::bezier:
				p = new BezierSpline(t); break;
			default:
				continue;
		}

		ti = -1;
		pe->GetAttributeValue(wxT("povtype"), ti);
		if ( ti == -1 ) {
			delete p;
			continue;
		}

		p->SetType((SplineBase::obj_t)ti);
		lst.push_back(p);

		ti = -1;
		pe->GetAttributeValue(wxT("use_sturm"), ti);
		if ( ti > -1 ) p->Getusesturm() = ti == 0 ? false : true;
		else p->Getusesturm() = false;

		ti = -1;
		pe->GetAttributeValue(wxT("use_open"), ti);
		if ( ti > -1 ) p->Getuseopen() = ti == 0 ? false : true;
		else p->Getuseopen() = false;

		ti = -1;
		pe->GetAttributeValue(wxT("sweeptype"), ti);
		if ( ti > -1 ) p->Getsweept() = (SplineBase::sweep_t)ti;
		else p->Getsweept() = SplineBase::slinear;

		t = wxT("");
		pe->GetAttributeValue(wxT("sweep_min"), t);
		if ( t != wxT("") ) t.ToDouble(&p->Getsweepmin());
		else p->Getsweepmin() = -1.0;

		t = wxT("");
		pe->GetAttributeValue(wxT("sweep_max"), t);
		if ( t != wxT("") ) t.ToDouble(&p->Getsweepmax());
		else p->Getsweepmax() = 1.0;

		t = wxT("");
		pe->GetAttributeValue(p->PropNames[p->transform], t);
		if ( t != wxT("") ) {
			p->GetTransform() = t;
		}

		t = wxT("");
		pe->GetAttributeValue(p->PropNames[p->texture], t);
		if ( t != wxT("") ) {
			p->GetTexture() = t;
		}

		t = wxT("");
		pe->GetAttributeValue(p->PropNames[p->interior], t);
		if ( t != wxT("") ) {
			p->GetInterior() = t;
		}

		t = wxT("");
		pe->GetAttributeValue(p->PropNames[p->userstr], t);
		if ( t != wxT("") ) {
			p->GetUserStr() = t;
		}

		ti = -1;
		pe->GetAttributeValue(wxT("UVcount"), ti);
		if ( fvers >= 0x00000002L ) {
			for ( int i = 0; i < ti; i++ ) {
				SplinePoint pt;
				double d;

				t = wxT("U");
				t << i;
				wxString t2 = wxT("");
				pe->GetAttributeValue(t, t2);
				if ( t2 == wxT("") ) {
					d = 0.0;
				} else {
					t2.ToDouble(&d);
				}
				pt.x = d;

				t = wxT("V");
				t << i;
				t2 = wxT("");
				pe->GetAttributeValue(t, t2);
				if ( t2 == wxT("") ) {
					d = 0.0;
				} else {
					t2.ToDouble(&d);
				}
				pt.y = d;

				int err = sanitise_spline_point(pt);
				if ( err ) {
					errno = err;
					return -1;
				}
				p->push_back(pt);
			}
		} else {
			for ( int i = 0; i < ti; i++ ) {
				SplinePoint pt;

				t = wxT("U");
				t << i;
				float t2 = FLT_MIN;
				pe->GetAttributeValue(t, t2);
				pt.x = t2;

				t = wxT("V");
				t << i;
				t2 = FLT_MIN;
				pe->GetAttributeValue(t, t2);
				pt.y = t2;

				int err = sanitise_spline_point(pt);
				if ( err ) {
					errno = err;
					return -1;
				}
				p->push_back(pt);
			}
		}
	}

	if ( nhg > 0 && (pe = db.FindClauseByFunctor(wxT("HGuides"))) ) {
		for ( int i = 0; i < nhg; i++ ) {
			wxString t(wxT("h"));
			int g = -1;

			t << i;
			pe->GetAttributeValue(t, g);
			if ( g >= 0 )
				hg.push_back(g);
		}
	}

	if ( nvg > 0 && (pe = db.FindClauseByFunctor(wxT("VGuides"))) ) {
		for ( int i = 0; i < nvg; i++ ) {
			wxString t(wxT("v"));
			int g = -1;

			t << i;
			pe->GetAttributeValue(t, g);
			if ( g >= 0 )
				vg.push_back(g);
		}
	}

	if ( pcomment ) {
		// Regardless of count, allow just one for now
		pe = db.FindClauseByFunctor(wxT("FileComments"));
		if ( pe ) {
			long n = 0;
			pe->GetAttributeValue(wxT("CommentCount"), n);
			if ( n > 0 ) {
				wxString t;
				pe->GetAttributeValue(wxT("Comment1"), t);
				*pcomment = wxsani(t);
			} else {
				*pcomment = wxT("");
			}
		} else {
			*pcomment = wxT("");
		}
	}

	return n;
}
