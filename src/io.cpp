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
#include "wxexio.h"
#include "wxutil.h"
// for setting "C" locale in a block
#include "clocnumeric.h"

#include <limits>
#include <cerrno>
#ifdef __WXMSW__
#include <float.h>  // for FLT_MIN
#else
#include <cfloat>
#endif


// Must check and sanitize the real numbers, as
// extreme values can cause big trouble like infinite loops,
// as dicovered using input from a T1 font conversion, with
// a bug that made values like 5.20004e+08; while fixes
// are needed at the point of the infinite loop (I don't even
// know now if it's in app code or wx code), sanitization
// should happen here anyway.
// Use the callback for something more reasonable than the default.
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

	// semi-arbitrary default maximum; use callback
	const double maxV = 3300.0;
	if ( std::abs(pt.x) > maxV )
		return EINVAL;
	if ( std::abs(pt.y) > maxV )
		return EINVAL;
	// NOTE: float epsilon, although double is tested; serves as min.
	// for this application.
	const double minV = std::numeric_limits<float>::epsilon();
	if ( std::abs(pt.x) <= minV )
		pt.x = pt.x < 0.0 ? -0.0 : 0.0;
	if ( std::abs(pt.y) <= minV )
		pt.y = pt.y < 0.0 ? -0.0 : 0.0;
	return 0;
}

// Data file format version:
const long format_version = 0x00000002L;

// Real number formatting precision (decimal part)
const int default_real_fmt_precision = 8;

bool
WriteData(const wxString& fname, const std::list<SplineBase*>& lst
	, const std::vector<int>& hg, const std::vector<int>& vg)
{
	cnumtmp c_tmp;
	int real_fmt_precision = default_real_fmt_precision;
	wxString real_fmt;
	FILE* o = fopen(wxs2fn(fname), "wb"); // MS reads fail w/ "wt"

	if ( o == NULL ) {
		::perror(wxs2fn(fname));
		return false;
	}
	
	real_fmt.Printf(wxT("%%.%df"), real_fmt_precision);

	wxExioDatabase db;

	wxExio* cnt = new wxExio(wxT("FileData"));
	cnt->AddAttributeValue(wxT("FileFormatVersion"), format_version);
	cnt->AddAttributeValue(wxT("ObjectCount"), (long)lst.size());
	cnt->AddAttributeValue(wxT("HGuideCount"), (long)hg.size());
	cnt->AddAttributeValue(wxT("VGuideCount"), (long)vg.size());
	db.Append(cnt);

	int nobj = 0;
	std::list<SplineBase*>::const_iterator it = lst.begin();
	while ( it != lst.end() ) {
		const SplineBase* p = *it++;
		wxString ob(wxT("Object"));

		ob << nobj++;
		wxExio& e  = *new wxExio(ob);
		e.AddAttributeValueString(wxT("objname"), p->GetObNam());
		e.AddAttributeValue(wxT("splinetype"), (long)p->Getsplinet());
		e.AddAttributeValue(wxT("povtype"), (long)p->Getobjt());
		e.AddAttributeValue(wxT("sweeptype"), (long)p->Getsweept());
		e.AddAttributeValue(wxT("use_sturm"), (long)p->Getusesturm());
		e.AddAttributeValue(wxT("use_open"), (long)p->Getuseopen());
		wxString swv; swv.Printf(real_fmt, p->Getsweepmin());
		e.AddAttributeValueString(wxT("sweep_min"), swv);
		swv = wxT(""); swv.Printf(real_fmt, p->Getsweepmax());
		e.AddAttributeValueString(wxT("sweep_max"), swv);
		if ( SplineBase::NotStringFieldEmpty(p->GetTransform()) )
			e.AddAttributeValueString(p->PropNames[p->transform]
				, p->GetTransform());
		if ( SplineBase::NotStringFieldEmpty(p->GetTexture()) )
			e.AddAttributeValueString(p->PropNames[p->texture]
				, p->GetTexture());
		if ( SplineBase::NotStringFieldEmpty(p->GetInterior()) )
			e.AddAttributeValueString(p->PropNames[p->interior]
				, p->GetInterior());
		if ( SplineBase::NotStringFieldEmpty(p->GetUserStr()) )
			e.AddAttributeValueString(p->PropNames[p->userstr]
				, p->GetUserStr());

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

	db.Write(o);
	if ( fclose(o) ) {
		::perror(wxs2fn(fname));
		return false;
	}

	return true;
}

int
ReadData(const wxString& fname, std::list<SplineBase*>& lst
	, std::vector<int>& hg, std::vector<int>& vg)
{
	cnumtmp c_tmp;
	int n;
	wxExioDatabase db;

	if ( !db.Read(fname) ) {
		int e = errno;
		fprintf(stderr, "Failed reading %s\n", wxs2fn(fname));
		errno = e;
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
		if ( fvers >= format_version ) {
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

	return n;
}

