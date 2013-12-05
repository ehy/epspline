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
// Display current data in quick and dirty POVRay test.
//


// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>

#include <wx/process.h>
#include <wx/filename.h>
#ifdef __WXMSW__
#include <wx/mimetype.h>
#endif
#endif // WX_PRECOMP

#include "splines.h"
#include "a_canvas.h"
#include "util.h"
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

#include <cmath>

#ifndef __WXMSW__
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#endif
#ifdef wx_x
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#endif
#ifdef _BSD_SOURCE
#include <paths.h>
#endif

// wx 2.9 removed wxStringList;
// a change to std::set is working well.
#include <set>

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

bool GetPovExecutable(wxString& dest);

// process execution class for POVRay demonstration
class PovDemoProc : public wxProcess {
	std::list<SplineBase*>& l;      // For names and types
	A_Canvas&          can;    // For Export() method
	wxString           cmd;
	wxString           itmp, otmp, itmp2;
	wxString           errmsg;
	bool               ok;

	bool SetupTmp();
	bool writepov(FILE* f);
public:
	static const wxString povexec;

	PovDemoProc(std::list<SplineBase*>& lst, A_Canvas& canvas);
	~PovDemoProc();

	wxString GetCommand() const {return cmd;}
	bool Okay() const {return ok;}

	wxString GetErrMsg() const {return errmsg;}

	void OnTerminate(int pid, int status);
};

#ifdef __WXMSW__
const wxString PovDemoProc::povexec(wxT("pvengine.exe"));
#else
const wxString PovDemoProc::povexec(wxT("povray"));
#endif

PovDemoProc::PovDemoProc(std::list<SplineBase*>& lst, A_Canvas& canvas)
	: l(lst), can(canvas)
{
	ok = SetupTmp();
}

PovDemoProc::~PovDemoProc()
{
	if ( ok ) {
		::remove(wxs2ch(itmp));
		::remove(wxs2ch(itmp2));
		::remove(wxs2ch(otmp));
		::remove(wxs2ch(otmp + wxT(".png")));
	}
}

bool
PovDemoProc::writepov(FILE* f)
{
	const char* clrs [] = {
		"<1,0,0>", "<0,1,0>", "<0,0,1>",
		"<1,1,0>", "<1,0,1>", "<0,1,1>",
		"<0.667,0,0>", "<0,0.667,0>", "<0,0,0.667>",
		"<0.667,0.667,0>", "<0.667,0,0.667>", "<0,0.667,0.667>",
		"<0.333,0,0>", "<0,0.333,0>", "<0,0,0.333>",
		"<0.333,0.333,0>", "<0.333,0,0.333>", "<0,0.333,0.333>"
	};
	const char fst [] =
		"#version 3.5;\n"
		"global_settings { assumed_gamma 1.0 }\n"
		"#include \"colors.inc\"\n"
		"#declare aspect = image_width / image_height;\n\n"
		"#if ( aspect > 1.0 )\n"
		"#declare szadj = 1.0 / aspect;\n"
		"#else\n"
		"#declare szadj = aspect;\n"
		"#end\n\n"
		"light_source { <20, 20, -20>\n"
		"  color White\n}\n\n"
		;
	const char stf [] =
		"#include \"%s\"\n\n"
		"camera {\n"
		"  location <0, 0, %f>\n"
		"  look_at <0, 0, 0>\n"
		"  right x * aspect\n"
		"  angle %f\n"
		"}\n\n"
		"#declare testo = union {\n"
		;
	const char uo [] = "  object { %s\n%s    rotate x * %d\n  }\n";
	const char tx [] = "    texture { pigment { color rgb %s } }\n";
	const char ob [] =
		"object { testo\n"
		"  translate <%f, 0, %f>\n"
		"  scale (1 / %f) * szadj\n"
		"  rotate x * 90\n"
		"}\n\n";
	int clr = 0, n = 0, outcnt = 0;
	wxString t(itmp), tn, tt, ts = wxT("");
	const double cam_pos_adj = 0.25; // trial & error adjustment
	const double cam_angle = 45.0; // this may become configurable

	cnumtmp c_tmp; // ensures real number formatting is in "C" locale

	std::fputs(fst, f);

	t.Replace(wxT("."), wxT("_"), true);
	t = wxT("I_") + t.AfterLast(SEPC);

	std::list<SplineBase*>::iterator i = l.begin();
	//wxStringList decl; // gone as of wx 2.9
	std::set<wxString>  decl;
	wxPoint pmin, pmax;
	double Lx = DBL_MAX, Ly = DBL_MAX;
	double Mx = DBL_MIN, My = DBL_MIN, viewmax = 0.0;
	while ( i != l.end() ) {
		SplineBase* p = *i++;
		
		if ( ! p->POkay() ) {
			continue;
		}
		if ( p->Getobjt() == SplineBase::undef ) {
			continue;
		}

		outcnt++;
		// for whole min and max
		double mnx, mny, mxx, mxy;
		p->CBox(mnx, mny, mxx, mxy);
		// because lathes rotate about 0 . . .
		if ( p->Getobjt() == SplineBase::lathe ) {
			viewmax = std::max(viewmax, std::abs(mxx));
		} else {
			double sw = p->Getsweepmax();
			viewmax = std::max(viewmax, sw);
		}
		Lx = std::min(Lx, mnx);
		Ly = std::min(Ly, mny);
		Mx = std::max(Mx, mxx);
		My = std::max(My, mxy);

		// object attributes
		wxString ts;
		ts = p->GetTexture(); ts.Trim(true); ts.Trim(false);
		if ( SplineBase::NotStringFieldEmpty(ts)
			&& (decl.find(ts) == decl.end()) ) { // !decl.Member(ts) ) {
			std::fprintf(f
			, "#declare %s = texture { pigment { color rgb %s } }\n"
			, wxs2ch(ts), clrs[clr++]);
			if ( clr == A_SIZE(clrs) ) clr = 0;
			decl.insert(ts); // decl.Add(ts);
		}
		ts = p->GetInterior(); ts.Trim(true); ts.Trim(false);
		if ( SplineBase::NotStringFieldEmpty(ts)
			&& (decl.find(ts) == decl.end()) ) { // !decl.Member(ts) ) {
			std::fprintf(f, "#declare %s = interior { ior 1 }\n"
				, wxs2ch(ts));
			decl.insert(ts); // decl.Add(ts);
		}
		ts = p->GetTransform(); ts.Trim(true); ts.Trim(false);
		if ( SplineBase::NotStringFieldEmpty(ts)
			&& (decl.find(ts) == decl.end()) ) { // !decl.Member(ts) ) {
			std::fprintf(f, "#declare %s = transform { rotate x*0 }\n"
				, wxs2ch(ts));
			decl.insert(ts); // decl.Add(ts);
		}
	}

	// write any good objects lately?
	if ( outcnt < 1 ) {
		return false;
	}

    double zp, scale;
    scale = std::max(std::abs(My-Ly), std::abs(Mx-Lx));

    // camera z-position, from two data: camera field angle, and
    // extent of field to be visible (x|y) which will have its
    // center at origin.  Since camera is centered and perpendicular,
    // angle/2 is opposite extent/2 which is one side of the 90 degree
    // angle of a right triangle:
    // (the two data are cam_angle:angle, and scale:visible extent)
    // -- take sine of angle and find radius, which is the hypotenuse:
    zp = std::abs(std::sin(cam_angle / 2.0 / (M_PI * 2.0)));
    zp = (scale / 2.0) / zp; // sin(theta)==y/r ergo r==y/sin(theta)
    // -- zp now holds radius|hypotenuse, so follow Pythagorus to the
    //    remaining side, which is negated for the camera z-position:
    zp = -std::sqrt(zp * zp - std::pow(scale / 2.0, int(2)));
    // -- zp now holds the *simplistic* camera z-position; it does not
    //    account for objects with significant -z extent.  To use maths
    //    to make an adjustment would require more processing of the
    //    calculated curve-points to find the most-deeply-clipped, but
    //    simple fudging will be good enough.  The greatest extent of
    //    all lathes or greatest sweep of prisms is subtracted from
    //    camera z-pos; it will help in some cases, but will merely
    //    reduce the size of the image if the point is near x,y center.
    zp -= viewmax;
    // -- finally, apply the render's scale factor
    zp /= scale;

    // print, and allow a bit of trial & error adjustment camera z-pos
	std::fprintf(f, stf, wxs2ch(itmp), zp - cam_pos_adj, cam_angle);

	//decl.erase(decl.begin(), decl.end()); // decl.Clear();
	clear(decl);
	i = l.begin();
	while ( i != l.end() ) {
		SplineBase* p = *i++;
		int rtt = 0;

		if ( ! p->POkay() ) {
			continue;
		}
		
		tn = p->GetObNam();
		if ( tn == SplineBase::DefaultName ) {
			tn << n;
		}
		n++;

		// continue on undef _after_ checking name
		// and inc'ing n, because objects were exported this way
		if ( p->Getobjt() == SplineBase::undef ) {
			continue;
		}
#if 0 // working on this: see A_Canvas::ExportFile()
		int nn = 0;
		wxString ttn(tn);
		while ( decl.find(ttn) != decl.end() ) {
			ttn = tn;
			ttn << ++nn;
		}
		tn = ttn;
		decl.insert(tn);	
#endif // 0

		tt = wxT("");
		if ( SplineBase::StringFieldEmpty(p->GetTexture()) ) {
			tt.Printf(ch2wxs(tx), ch2wxs(clrs[clr++]).c_str());
			if ( clr == A_SIZE(clrs) ) clr = 0;
		}

        // want to see lathes from side view
        if ( p->Getobjt() == SplineBase::lathe ) {
            rtt = 90;
        }

		std::fprintf(f, uo, wxs2ch(tn), wxs2ch(tt), rtt);
	}
	std::fputs("}\n\n", f);

	std::fprintf(f, ob, -(Mx-Lx)/2.0-Lx, -(My-Ly)/2.0-Ly, scale);

	std::fputs(
		"plane { <0,0,-1>, 0 translate z*50\n"
		"  texture { pigment { color White } }\n"
		"  hollow\n"
		"}\n\n",
		f
	);

	return true;
}

namespace {
	// POV-Ray from user prefs -- no init arg, set through `SetPovOpts'
	// externally by e.g., preference manager
	wxString povopts;
};

void
SetPovOpts(wxString cfg)
{
	povopts = cfg;
}

wxString
GetPovOpts()
{
	return povopts;
}

bool
PovDemoProc::SetupTmp()
{
	cnumtmp c_tmp;
	char pc[PATH_MAX];
	auto_std_FILE f;

	int fd = gettmpfd(pc, A_SIZE(pc));
	if ( fd < 0 ) {
		errmsg = _("internal error [SetupTmp() 00]");
		::perror("PovDemoProc::SetupTmp() 00");
		return false;
	}
	otmp = ch2wxs(pc);
	::close(fd);
	wxString outf(otmp + wxT(".png"));
	// we expect that (otmp + ".png") does not exist
#ifdef __UNIX__
	fd = symlink(wxs2fn(otmp), wxs2fn(outf));
#else
	fd = open(wxs2fn(outf), O_RDWR|O_CREAT|O_EXCL, 0600);
#endif
	if ( fd < 0 ) {
		errmsg = _("internal error [SetupTmp() 05]");
		::perror("PovDemoProc::SetupTmp() 05");
		::remove(wxs2fn(otmp));
		return false;
	}
#ifndef __UNIX__
	::close(fd);
#endif

	fd = gettmpfd(pc, A_SIZE(pc));
	f.assign(::fdopen(fd, "r+"));
	if ( ! f ) {
		errmsg = _("internal error [SetupTmp() 10]");
		::perror("PovDemoProc::SetupTmp() 10");
		::remove(wxs2fn(otmp));
		::remove(wxs2fn(outf));
		if ( fd >= 0 ) {
			::remove(pc);
			::close(fd);
		}
		return false;
	}
	itmp = ch2wxs(pc);

	can.ExportFILE(f, itmp.AfterLast(SEPC), true);
	int e0 = ::ferror(f);
	int e1 = ::fclose(f.release());
	if ( e0 || e1 ) {
		errmsg = _("internal error [SetupTmp() 20]");
		::perror("PovDemoProc::SetupTmp() 20");
		::remove(wxs2fn(otmp));
		::remove(wxs2fn(outf));
		::remove(wxs2fn(itmp));
		return false;
	}

	fd = gettmpfd(pc, A_SIZE(pc));
	f.assign(::fdopen(fd, "r+"));
	if ( ! f ) {
		errmsg = _("internal error [SetupTmp() 30]");
		::perror("PovDemoProc::SetupTmp() 30");
		::remove(wxs2fn(otmp));
		::remove(wxs2fn(outf));
		::remove(wxs2fn(itmp));
		if ( fd >= 0 ) {
			::remove(pc);
			::close(fd);
		}
		return false;
	}
	itmp2 = ch2wxs(pc);

	writepov(f);
	e0 = ::ferror(f);
	e1 = ::fclose(f.release());
	if ( e0 || e1 ) {
		errmsg = _("internal error [SetupTmp() 40]");
		::perror("PovDemoProc::SetupTmp() 40");
		::remove(wxs2fn(otmp));
		::remove(wxs2fn(outf));
		::remove(wxs2fn(itmp));
		::remove(wxs2fn(itmp2));
		return false;
	}

	wxString pexec;
	if ( ! GetPovExecutable(pexec) ) {
		errmsg =
		_("POV-Ray not found or not executable -- check preferences");
		::remove(wxs2fn(otmp));
		::remove(wxs2fn(outf));
		::remove(wxs2fn(itmp));
		::remove(wxs2fn(itmp2));
		return false;
	}

	wxString opts = GetPovOpts();

#ifdef __UNIX__
	cmd.Printf(wxT("%s %s +FN +I%s +O%s")
		, fmtcst(pexec), fmtcst(opts), fmtcst(itmp2), fmtcst(outf));

	// Added 10-2012 -- the Unix Povray X preview window under
	// new (last few years) compositing/window managers (e.g.
    // KDE, compiz, etc) has unwanted transparency, at least
    // with typical default visual 'TrueColor' -- a workaround
    // that has been reliable (for me) is to give '-visual DirectColor',
    // but I don't know if it is OK as a default.
    // Update: change to add the arg by default, use of envvar
    // is to refrain from using the arg.
    //
    // Update 2013/11/20: POVRay 3.7 released a few days ago, and
    // the Unix build uses SDL (might be an option) which does not
    // accept the this argument, so the default is made false, and
    // and envar added to make the the arg available -- and for fun
    // the old code is left in place so that the user may use
    // both envars and turn on and then off for no good reason.
    bool use_directcolor = false;
    if ( const char* p = std::getenv("POVRAY_USE_DIRECTCOLOR") ) {
        wxString s(ch2wxs(p));
        if ( s == wxT("1")
            || ! s.CmpNoCase(wxT("yes"))
            || ! s.CmpNoCase(wxT("true"))
            || ! s.CmpNoCase(wxT("on"))
            ) {
			use_directcolor = true;
        }
    }
    if ( const char* p = std::getenv("POVRAY_NO_DIRECTCOLOR") ) {
        wxString s(ch2wxs(p));
        if ( s == wxT("1")
            || ! s.CmpNoCase(wxT("yes"))
            || ! s.CmpNoCase(wxT("true"))
            || ! s.CmpNoCase(wxT("on"))
            ) {
			use_directcolor = false;
        }
    }
	if ( use_directcolor ) {
		cmd += wxT(" -visual DirectColor");
	}

#	ifndef _PATH_BSHELL
#	define _PATH_BSHELL "/bin/sh"
#	endif

#	if 0 // decide: should povray messages go to /dev/null, or not?
	cmd = ch2wxs(_PATH_BSHELL " -c 'exec ") + cmd
        + ch2wxs(" < /dev/null > /dev/null 2>&1'");
#	else
	cmd = ch2wxs(_PATH_BSHELL " -c 'exec ") + cmd
        + ch2wxs(" < /dev/null'");
#	endif

#else // __UNIX__

	cmd.Printf(wxT("%s %s +FN +I%s +O%s /NR")
		, fmtcst(pexec), fmtcst(opts), fmtcst(itmp2), fmtcst(outf));

#endif // __UNIX__

	return true;
}

void
PovDemoProc::OnTerminate(int pid, int status) // const
{
#if 0 // Lockup if dialog is shown at this point
	if ( status ) {
		wxString m(wxT(""));
		m.Printf(_("Povray (%d) failed: status %d"), pid, status);
		wxMessageBox(m, _("Demo Failed"));
	} else {
		wxString m(wxT(""));
		m.Printf(_("Povray (%d) finished"), pid);
		wxMessageBox(m, _("Demo Completed"));
	}
#else
	if ( status ) {
		// FIXME: Need something suitable for GUI app.
		wxString m(_("%s (pid %d) failed: status %d\n"));
		std::fprintf(stderr, wxs2ch(m), wxs2ch(cmd), pid, status);
	}
#endif

	delete this;
}

namespace {
	// POV-Ray from user prefs -- no init arg, set through `SetPovPref'
	// externally by e.g., preference manager
	wxString povcfg;
};

void
SetPovPref(wxString cfg)
{
	povcfg = cfg;
}

wxString
GetPovPref()
{
	return povcfg;
}

bool
GetPovExecutable(wxString& dest)
{
	// POV-Ray from user prefs
	wxString cfgpov = GetPovPref();
	// if it's same as default, empty it as if it was not set;
	// note comparison is case sensitive, but this should ultimately
	// not matter on case-insensitive fs, such as on MSW
	if ( cfgpov == PovDemoProc::povexec ) {
		cfgpov.Empty();
	}
	
	if ( ! cfgpov.IsEmpty() ) {
		wxFileName fn(cfgpov);
		if ( ! fn.IsOk() ) {
			// bad arg
			return false;
		}
		
		// has a path part?
		if ( ! fn.GetPathWithSep().IsEmpty() ) {
			// if path was given user wants a specific executable
			if ( fn.IsFileExecutable() ) {
				dest = cfgpov;
				return true;
			}
			return false;
		}
		// path was not present; arg will be considered name only
		// to be used in OS-specific lookup below
	}

#ifdef __WXMSW__
	// On MS, POVRay puts its type and command in the
	// registry; this should find it
	wxMimeTypesManager* mtm = wxTheMimeTypesManager;
	wxFileType* ft = mtm->GetFileTypeFromExtension(wxT(".POV"));
	if ( ft == NULL ) {
		return false;
	}

	wxString cmd;
	wxFileType::MessageParameters mp;
	bool ftr = ft->GetOpenCommand(&cmd, mp);
	delete ft;
	if ( ftr == false ) {
		return false;
	}

	// The mime-type thingy returns something like:
	// "C:\Program Files\POV-Ray-3.1g\BIN\PVENGINE.EXE" /EDIT "FOO.POV"
	// or "C:\Program Files\POV-Ray-3.1g\BIN\PVENGINE.EXE" /RENDER "FOO.POV"
	// chop off the unwanted stuff (only want exe path).
	// POV 3.7: these are now appearing in lowercase
	if ( int idx = cmd.Find(wxT(" /")) ) {
		cmd.Remove(idx);
		cmd.Trim(true);
	}

	// if user pref was given as name only, consider it a substitute
	// for the exe named in the return from the type-manager above
	if ( ! cfgpov.IsEmpty() ) {
		bool lqu = cmd.StartsWith(wxT("\""), &cmd);
		bool rqu = cmd.EndsWith(wxT("\""), &cmd);

		wxFileName fn(cmd);
		cmd = fn.GetPathWithSep() + cfgpov;

		if ( lqu ) {
			cmd = wxString(wxT("\"")) + cmd;
		}
		if ( rqu ) {
			cmd += wxT("\"");
		}
	}

	dest = cmd;
	return true;

#elif !defined(__WXMSW__) && !defined(__UNIX__)
	// This is for-position-only, for platforms not currently
	// tested, and will certainly need work for any of them
	if ( cfgpov.IsEmpty() ) {
		dest = PovDemoProc::povexec;
	} else {
		dest = cfgpov;
	}

	return true;

#elif defined(__UNIX__)
	// On Unix, POVRay AFAIK registers no mime type during
	// installation.  Do a quick PATH search.
	const char* path = 0;

	if ( is_priviliged() == false ) {
		path = std::getenv("PATH");
	}
	
	if ( path == 0 ) {
#	ifdef _PATH_DEFPATH
		path = _PATH_DEFPATH;
#	else
		// certainly not suitable all-around, but we shouldn't
		// be here in any case
		path = "/usr/bin:/usr/X11R6/bin:/usr/X11/bin:/bin";
#	endif
	}

	wxString px(cfgpov.IsEmpty() ? PovDemoProc::povexec : cfgpov);
	wxString p(ch2wxs(path));

	for (int n = 0; n < 64; n++) { // 64 is arbitrary
		wxString d(p.BeforeFirst(wxT(':')));
		bool brk = (d == p);
		p.Remove(0, d.Len()+1);
		if ( d.Last() == wxT(':') )
			d.RemoveLast();

		struct stat sb;
		if ( !stat(wxs2fn(d+SEPC+px), &sb) ) {
			// S_IX* check is not complete, but
			// if we can't execute it, handle error
			// at execution attempt
			if ( S_ISREG(sb.st_mode) &&
				sb.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH) ) {
				dest = d + SEPC + px;
				return true;
			}
		}

		if ( brk )
			break;
	}

	return false;

#else
#   error "What platform is this? You must port this code!"
#endif
}

// The only public interface: proto in util.h
long
DoPovDemo(A_Canvas& canvas, std::list<SplineBase*>& lst, wxString* e)
{
	long ret = 0;
	int flags = 0;
#ifdef wxEXEC_ASYNC
	flags |= wxEXEC_ASYNC;
#endif

	PovDemoProc* p = new PovDemoProc(lst, canvas);
	if ( p->Okay() ) {
		ret = wxExecute(p->GetCommand(), flags, p);
	} else {
		if ( e ) {
			*e = p->GetErrMsg();
		}
	    //fprintf(stderr, "FAILED: %s\n", wxs2ch(p->GetCommand()));
		delete p;
		return false;
	}
	
	return ret;
}

