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

/*
 ~  Simple spline drawing editor to help produce
 ~  POV include files for spline-based POV objects.
 ~
 ~  The ``drawing'' example provided with the wxWindows source,
 ~  by Robert Roebling, provided the starting point.
 ~
 ~  Ed Hynan -- Sat Mar 18 11:15:35 EST 2000
 ~  Ed Hynan -- Tue Aug 26 08:15:27 EDT 2003
 ~  Ed Hynan -- Fri Mar 18 08:43:38 EST 2005
 ~  Ed Hynan -- Sat Jan  6 08:28:35 EST 2007
*/

#include "stdcc.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
// (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <wx/stdpaths.h>
#if wxCHECK_VERSION(2, 4, 0)
#include <wx/filename.h>
#endif
#endif // WX_PRECOMP

#if defined(bool) && 0
#error "bool"
#endif

#ifdef __GNUG__
    #pragma implementation
#endif

#if INVOKE_STD_SETLOCALE || NO_USE_WXLOCALE
#include <clocale>
#endif // ! INVOKE_STD_SETLOCALE
#include <typeinfo>

#include "epspline.h"
#include "a_frame.h"
#include "clocall.h"
#include "a_prefs_manager.h"

#if defined(__WXMSW__)
#if defined(__SC__) || defined(__DMC__)
#include <dos.h> /*For find{first|next}()*/
#endif
static void findwild(int ac, wxChar** av, wxArrayString& dest);
#endif

#if defined(__SC__) || defined(__DMC__) && defined(__WXMSW__)
// Digital Mars C++ build is producing corrupt wxApp:argv vector
// in OnInit causing exception when given args; in fact argv[0] is N.G.
// As workaround try C library globals until problem is known.
#define ARGC _argc
#define ARGV _argv
extern "C" {
	extern int ARGC;
	extern char** ARGV;
}
#else
#define ARGC argc
#define ARGV argv
#endif

// a global pointer to char converter; avoid wx's wxConvCurrent*
// directly, and get some slack.
wxMBConv* app_global_mbconv;

// the application icons
#include "art/epspline_16x16.xpm"
#include "art/epspline_24x24.xpm"
#include "art/epspline_32x32.xpm"
#include "art/epspline_48x48.xpm"
#include "art/epspline_64x64.xpm"
const char** icon_bundle_items[] = {
	epspline_16x16,
	epspline_24x24,
	epspline_32x32,
	epspline_48x48,
	epspline_64x64
};
wxIconBundle iconBundle;

// the application class
//

// App object creation
IMPLEMENT_APP(AnApp)

// `Main program'

	// WX macro to setup event handling
	// Added 2013/12/04 for MSW shutdown event handling with
	// EVT_END_SESSION() -- w/o this MSW7 queries and barfs
BEGIN_EVENT_TABLE		(AnApp, wxApp)
 EVT_QUERY_END_SESSION	(AnApp::OnQueryEndSession)
 EVT_END_SESSION		(AnApp::OnEndSession)
END_EVENT_TABLE()

// These On*EndSession seem to be mostly for MSW's shutdown message
// handling; there doesn't seem to be an equivalent in the
// GTK build, or generally a X Window System desktop environment?
// These are a PITA because I do not know how to get Wine to
// cause these events.
void
AnApp::OnQueryEndSession(wxCloseEvent& e)
{
	A_Frame* pw = dynamic_cast<A_Frame*>(GetTopWindow());
	if ( pw == 0 ) {
		return;
	}

	// do not try to show save query dialogs here: makes a mess
	// of MSW logoff/shutdown; but DO try to shutdown open dialogs:
	pw->CloseDialogs();
	QuitHelp();

	bool dirt = pw->QueryDirtyData();

	if ( e.CanVeto() && dirt ) {
		e.Veto(true);
	}
}

void
AnApp::OnEndSession(wxCloseEvent& e)
{
	if ( A_Frame* pw = dynamic_cast<A_Frame*>(GetTopWindow()) ) {
		pw->Close(true);
	}
}

bool
AnApp::OnInit()
{
	pprefsmng = 0;
	single_instance_check = 0;

	// n.g. unless using wx commandline parser, else fails on args!
	if ( false && !wxApp::OnInit() ) {
		return false;
	}

	// Added v 0.0.4.5: MSW will open an instance per file selected
	// in explorer shell (selecting open from right-click menu); it
	// seems that only way to get multiple args passed to one instance
	// is to set up a DropTarget, with UUID and all registry entries,
	// etc., which must wait for the future.
	// For now refuse 2nd instance using wxSingleInstanceChecker --
	// code lifted from wx docs.
#	if wxCHECK_VERSION(2, 9, 0)
	single_instance_check = new wxSingleInstanceChecker();
#	else
	{
		wxString nchk(wxString(_T("Epspline-")) + wxGetUserId());
		single_instance_check = new wxSingleInstanceChecker(nchk);
	}
#	endif
	if ( single_instance_check->IsAnotherRunning() ) {
		wxLogError(_("Epspline is already running, aborting."));
		delete single_instance_check;
		single_instance_check = 0;
		return false;
	}

	// set the frame icons
	for ( size_t i = 0; i < A_SIZE(icon_bundle_items); i++ ) {
		iconBundle.AddIcon(wxIcon(icon_bundle_items[i]));
	}

	// will need to control some C-library functions,
	// particularly string formatting of numeric data;
	// otherwise let locale from environment be used
	// an instance of wxLocale should do std::setlocale(),
	// but the the following can be used if needed:
#	if INVOKE_STD_SETLOCALE || NO_USE_WXLOCALE
	std::setlocale(LC_ALL, "");
	// NOTE: the C++ locale object(s) are not used here:
	// 1st) that would be the business of wxWidgets; and as of
	// v. 2.9 they _are_ having trouble with it -- AND
	// 2nd) as of now, 12-2012, C++ locale is still broken
	// in at least some stdc++ libraries; e.g. FreeBSD latest release
	// v. 9.0 (still) will not accept any locale string but "C",
	// not even the empty string "" as one would expect from
	// the C setlocale(LC_ALL, "") idiom -- anything but "C" throws
	// runtime_error; a web search shows that similar behavior
	// is being found in Apple OS X (no surprise, given its descent);
	// a recent GNU/Linux system, Ubuntu 12.04, seems better in that
	// no exceptions are thrown and in quick tests at least
	// double formatting is affected re. decimal point character --
	// BUT even in that system usage conflicts with Stroustrup's
	// The C++ Programming Language Third Edition, which shows this:
	//         cin.imbue(std::locale::global()); // reset cin to [...]
	// which fails to compile with the complaint that there is no
	// matching function [re. global(void)] -- only compiles if passed
	// a std::locale object.  Messages from web search indicated
	// additional oddities, e.g. w/ MinGW on MSW.
	// It seems that the only conclusion is that C++ locale is still
	// a mess even at this late date.  I wonder how wx devs will resolve
	// this for the wxUSE_STD build before release of 3.0?
	// Update: comment re. FreeBSD 9.0 -- identical for OpenBSD 4.9.
#	endif

#	if wxCHECK_VERSION(2, 6, 0) && \
		defined(__UNIX__) && defined(INSTALL_PREFIX)
	// In (at least) wx 2.8, [GS]etInstallPrefix() are *not* declared
	// virtual (unlike, e.g., GetDataDir()), and they are members of
	// wxStandardPaths, *not* of wxStandardPathsBase which is what
	// wxStandardPaths::Get() actually returns a ref. to!  So,
	// compile fails with 'no member . . . in wxStandardPathsBase'.
	// Here dynamic_cast can do its thing.
	try {
		dynamic_cast<wxStandardPaths&>
		(wxStandardPaths::Get()).SetInstallPrefix(wxT(INSTALL_PREFIX));
	} catch ( std::bad_cast e ) {
		wxLogError(
			wxT("Cannot set install prefix: unexpected condition"));
		std::fprintf(stderr,
			"Cannot set install prefix -- unexpected condition: "
			"bad_cast: \"%s\"\n", e.what());
		// can continue, but might fail i18n and help setup
	}
#	endif

	// wxLocale should be preprocessor-optional
	// as of 12-2012 this needs real testing; there are
	// no translations yet
#	if ! NO_USE_WXLOCALE
#	ifdef L10N_INSTALL_PATH
	wxLocale::AddCatalogLookupPathPrefix(wxT(L10N_INSTALL_PATH));
#	endif

	// possible args: wxLANGUAGE_DEFAULT, wxLOCALE_DONT_LOAD_DEFAULT
	if ( ! app_locale.Init() ) {
		// do not complain loudly; it will only be a nuisance
		#if DEBUG
		//wxLogError(wxT("(wx) 1st Locale initialization failed"));
		std::fputs(
			"epspline: 1st Locale initialization failed\n", stderr);
		#else  // DEBUG
		;
		#endif // DEBUG
	}

	// Even if Init() above returns false, translations might
	// still work, so keep at it.
#	if wxCHECK_VERSION(2, 9, 1)
	// Sigh. wx 2.9.? wxTranslations has the bizarre logic in
	// AddCatalog() that if the app's string language matches
	// the message cat language, then there is no need to load
	// the catalog, so the method returns true without loading!
	// A comment from the wx source in src/common/translation.cpp:
    //    It is OK to not load catalog if the msgid language and m_language match,
    //    in which case we can directly display the texts embedded in program's
    //    source code:
	//
	// That is misguided, as source code strings are best constrained
	// to ASCII, and so the translation mechanism might be used to load
	// more desirable text even in the same locale as the
	// source code language (by providing a message catalog, of course).
    //
    // Furthermore, the one-arg AddCatalog() calls the two-arg
    // form, passing wxLANGUAGE_ENGLISH_US; therefore, messages
    // for the en_US locale will never be loaded when the one-arg
	// call is used!
    //
    // So, work around the anti-US code by using the two-arg
    // AddCatalog() call to lie to wx that the source strings
	// are *not* US English, so that it will load the messages
	// even in the en_US locale.
    wxLanguage src_lang = wxLanguage(app_locale.GetLanguage());
    if ( src_lang == wxLANGUAGE_ENGLISH_US ) {
#		if DEBUG
		std::fprintf(stderr
		,"Got lang int %d -- base == %d, UK == %d, US == %d, default == %d, unknown == %d\n"
		, int(src_lang)
		, int(wxLANGUAGE_ENGLISH)
		, int(wxLANGUAGE_ENGLISH_UK)
		, int(wxLANGUAGE_ENGLISH_US)
		, int(wxLANGUAGE_DEFAULT)
		, int(wxLANGUAGE_UNKNOWN)
		);
#		endif // DEBUG
		// wxLANGUAGE_ENGLISH seems to be used as equivalent to
		// wxLANGUAGE_ENGLISH_UK; although the enum value differs
		src_lang = wxLANGUAGE_ENGLISH;
	} else {
		// if not in the en_US locale, then tell wx the truth
		src_lang = wxLANGUAGE_ENGLISH_US;
	}
	if ( ! app_locale.AddCatalog(wxT(APPNAME_IN_ASCII), src_lang) ) {
#	else  // wxCHECK_VERSION(2, 9, 1)
	if ( ! app_locale.AddCatalog(wxT(APPNAME_IN_ASCII)) ) {
#	endif // wxCHECK_VERSION(2, 9, 1)
		// do not complain loudly; it will only be a nuisance when
		// the cat really does not exist (rather than another error)
		//wxLogError(wxT("(wx) add initialization catalog failed"));
		#if DEBUG
		std::fprintf(stderr,
			"epspline: AddCatalog(%s) failed\n", APPNAME_IN_ASCII);
		#else  // DEBUG
		;
		#endif // DEBUG
	}
	
	// right from internat sample:
	app_locale.AddCatalog(wxT("wxstd"));
#	endif // ! NO_USE_WXLOCALE

	// set the global wxMBConv* -- this envvar will be undocumented
	if ( const char* p = std::getenv("WXCHARCONV") ) {
		if ( ! std::strcmp(p, "libc") ) {
			app_global_mbconv = &wxConvLibc;
			wxConvCurrent = app_global_mbconv;
		} else if ( ! std::strcmp(p, "wxlocal") ) {
			app_global_mbconv = &wxConvLocal;
			wxConvCurrent = app_global_mbconv;
		} else {
			app_global_mbconv = 0;
		}
	} else {
		app_global_mbconv = 0;
	}

	if ( app_global_mbconv == 0 ) {
		app_global_mbconv = wxConvCurrent ?
			wxConvCurrent :			
#			if ! USE_WX_CHARCONV_LIBC
			&wxConvLocal;
#			else
			&wxConvLibc;
#			endif
	}
	
	app_mbconv = app_global_mbconv;

#	if DEBUG
	std::fprintf(stderr,".GetLocale() == %s\n",
		  wxs2ch(wxString(app_locale.GetLocale())));
	std::fprintf(stderr,".GetSysName() == %s\n",
		  wxs2ch(app_locale.GetSysName()));
	std::fprintf(stderr,".GetCanonicalName() == %s\n",
		  wxs2ch(app_locale.GetCanonicalName()));
#	endif

	wxFileName fn_av0(ARGV[0]);
	av0 = fn_av0.GetName();

	// with char conversion ready, 1st check display
	GetDisplayData();
	if ( ! DispLocal ) {
		std::fprintf(stderr, "%s: using display \"%s\", host \"%s\"\n",
			wxs2ch(av0), wxs2ch(DispName), wxs2ch(DispHost));
	}

#	if wxCHECK_VERSION(2, 7, 0)
	wxString resourcedir(wxStandardPaths::Get().GetResourcesDir());
#	else
	wxString resourcedir(wxStandardPaths::Get().GetDataDir());
#	endif
#	if _DEBUG_RESOURCE_PATHS
	std::fprintf(stderr,
		"wx standard paths - Resources Dir:\n\t%s\n",
		wxs2ch(resourcedir));
#	if wxCHECK_VERSION(2, 7, 0)
	wxString loc_resourcedir;
	loc_resourcedir = wxStandardPaths::Get().GetLocalizedResourcesDir(
		wxT("en"), wxStandardPaths::ResourceCat_Messages);
	std::fprintf(stderr,
		"wx standard paths - Localized Resources Dir:\n\t%s\n",
		wxs2ch(loc_resourcedir));
#	endif
#	endif

	// not in cdata.h; translatable
	//AppTitle = _("POV Spline Editor");
	AppTitle = _("Prism/Lathe Object Editor for POV-Ray");
	
	// rand init has been here a long time -- never did add
	// whatever it was for -- harmless nevertheless
	::srand(::time(NULL));

	// nothing below needs locale processing
	c_alltmp clt;


#	if defined(__WXMSW__)
	// about capitalised ARG?, see comment near top regarding __SC__
	findwild(ARGC, ARGV, FileArgs);
#	else
	for ( int i = 1; i < argc; i++ ) {
		FileArgs.Add(argv[i]);
	}
#	endif

#	if defined(__WXMSW__)
	// hard code appname, with version appended -- this scheme is
	// used in the NSIS binary installer
	wxString fn(wxT(APPNAME_IN_ASCII));
	fn += wxT("\\");
	fn += wxT(APPVERSIONSTR);
#	else // ! __WXMSW__
	// set app name from bin name
#	if wxCHECK_VERSION(2, 4, 0)
	wxString fn(wxFileName(argv[0]).GetName());
#	else
	wxString fn(wxFileNameFromPath(wxString(argv[0])));
#	endif // if wxCHECK_VERSION(2, 4, 0)
	if ( fn.length() < 1 )
		fn = wxT(APPNAME_IN_ASCII);
#	endif // if defined(__WXMSW__)

	SetVendorName(wxT(VENDORNAME));
	SetClassName(wxT(APPCLASS_IN_ASCII));
	SetAppName(fn);

	// these are needed (at least) for the html help viewer --
	// which is created after the wxConfigBase::Get() call,
	// and main window creation
	wxInitAllImageHandlers();
#	if wxCHECK_VERSION(2, 8, 0)
	wxFileSystem::AddHandler(new wxArchiveFSHandler);
#	else
	wxFileSystem::AddHandler(new wxZipFSHandler);
#	endif

#	if ! defined(__UNIX__)
	// not Unix; just guessing that system provides preferred work dir.
	LastDlgDir = wxStandardPaths::Get().GetDocumentsDir();
#	else // ! defined(__UNIX__)
	// assume $PWD is suitable
	LastDlgDir = wxGetCwd();
#	endif // ! defined(__UNIX__)


	// saved state -- per display where applicable
	wxConfigBase* pConfig = GetCfgPtr();
	wxPoint P;
	P.x = int(pConfig->Read(wxT("x"), 20L));
	P.y = int(pConfig->Read(wxT("y"), 40L));
	wxSize S;
	S.x = int(pConfig->Read(wxT("w"), 800L));
	S.y = int(pConfig->Read(wxT("h"), 600L));
	
	pprefsmng = new A_Prefs_Manager(pConfig);

	// this might be useful on some Unix systems
	SetUseBestVisual(true);

	// Main window
	A_Frame* frame = new A_Frame(GetAppTitle(), P, S);

	// Set frame as main window
	frame->Show(TRUE);
	
	frame->Iconize(bool(pConfig->Read(wxT("iconized"), 0L)));
	frame->Maximize(bool(pConfig->Read(wxT("maximized"), 0L)));

	SetTopWindow(frame);

	// make the help controller
	help = new wxHtmlHelpController(
#	if wxCHECK_VERSION(2, 8, 0)
	wxHF_DEFAULT_STYLE, frame);
#	else  // wxCHECK_VERSION(2, 8, 0)
	wxHF_DEFAULT_STYLE|wxHF_OPEN_FILES);
#	endif // wxCHECK_VERSION(2, 8, 0)

	// add help book(s) (zip file[s])
	wxFileName helpbook(resourcedir, wxT(HELP_BOOK_NAME));
	helpbook.AppendDir(wxT(HELP_DOC_SUBDIR));
	help_ok = help->AddBook(helpbook);
	if ( ! help_ok ) {
		wxString h(helpbook.GetFullPath());
		std::fprintf(stderr, "%s: failed loading help \"%s\"\n",
			wxs2ch(fn), wxs2ch(h));
		helpbook.RemoveLastDir();
		help_ok = help->AddBook(helpbook);
		if ( ! help_ok ) {
			h = helpbook.GetFullPath();
			std::fprintf(stderr, "%s: failed loading help \"%s\"\n",
				wxs2ch(fn), wxs2ch(h));
			delete help;
			help = 0;
		}
	}
	
	if ( help_ok ) {
		help->UseConfig(pConfig);
	}

	return TRUE;
}

#ifndef USE_HELP_DISPLAY_HACK
#define USE_HELP_DISPLAY_HACK 1
#endif

#if USE_HELP_DISPLAY_HACK
void
AnApp::help_display_hack()
{
	if ( help_ok ) {
		wxHtmlWindow* hw = help->GetHelpWindow()->GetHtmlWindow();
		if ( hw ) {
			// Problem: frequent failure to scroll anchor link
			// ("foo.html#bar") into view in the html window.
			// Clearly a bug in (re)size handling, but I haven't
			// found where; IAC, size event handler results in
			// call to wxHtmlWindow::CreateLayout() (which is
			// protected, so can't be called directly here),
			// which is where html 'cells' have display size
			// calculated. A fresh wxHtmlWindow::CreateLayout()
			// call fixes the anchor display problem at least
			// until window resize or splitter sash move, when
			// the symptom might reappear -- EH 2016/03/09 09:15:40
			if ( wxEvtHandler* hdlr = hw->GetEventHandler() ) {
				wxSizeEvent ev(wxSize(hw->GetSize()));
				hdlr->ProcessEvent(ev);
			}
		}
	}
}
#else  // USE_HELP_DISPLAY_HACK
void AnApp::help_display_hack() {}
#endif // USE_HELP_DISPLAY_HACK

int
AnApp::OnExit()
{
	delete pprefsmng;
	delete wxConfigBase::Set(0);
	delete single_instance_check;
	//delete help;
	return 0;
}

#if defined(__WXMSW__)
static void
findwild(int ac, wxChar** av, wxArrayString& dest)
{
	for ( int i = 1; i < ac; i++ ) {
#	if defined(__DMC__) || defined(__SC__)
		wxString d(av[i]);
		// although SC will handle forward slashes
		// replace them for consistency
		d.Replace(wxT("/"), wxT("\\"), true);
		int idx = d.Find(wxT('\\'), true);

		if ( idx >= 0 )
			d.Remove(idx);
		else
			d = wxT("");

		for ( struct FIND* fe = findfirst(av[i], 0);
			fe != NULL; fe = findnext() ) {
			wxString f(fe->name);
			if ( f != wxT("") ) {
				if ( d == wxT("") )
					dest.Add(f);
				else if ( f[0] == wxT('\\') )
					dest.Add(d + f);
				else
					dest.Add(d + wxT("\\") + f);
			}
		}
#	else
		for ( wxString f = wxFindFirstFile(av[i], wxFILE);
			! f.IsEmpty(); f = wxFindNextFile() ) {
			if ( f != wxT("") ) {
				f.Replace(wxT("/"), wxT("\\"), true);
				dest.Add(f);
			}
		}
#	endif
	}
}
#endif

wxConfigBase*
AnApp::GetCfgPtr(wxString path) const
{
	if ( wxConfigBase* pConfig = wxConfigBase::Get() ) {
		wxString sfx;
		wxString dname(GetDisplayName());
		if ( DispName != wxT("default") ) {
			sfx = wxString(wxT("@")) + dname;
		}
		pConfig->SetPath(path + sfx);
		return pConfig;
	}

	return 0;
}

void
AnApp::delete_prefs_dialog()
{
	if ( pprefsmng ) {
		pprefsmng->delete_prefs_dialog();
	}
}

void
AnApp::show_prefs_dialog(bool show)
{
	if ( pprefsmng ) {
		pprefsmng->show_prefs_dialog(show);
	}
}

bool
AnApp::IsDisplayLocal() const
{
	return DispLocal;
}

wxString
AnApp::GetDisplayName() const
{
	return DispName;
}

wxString
AnApp::GetDisplayHost() const
{
	return DispHost;
}

// need to know if our display is on local host,
// because if it is not display behavior can be
// selected in consideration of that
#if defined(__X__) || defined(__WXGTK__)

// Since the display name is wanted under the X Window System,
// there is the question of how to get it. AFAIK getenv("DISPLAY")
// should be reliable on any Unix, but X (and, I think, GTK) might
// be used beyond Unix, and it must be more reliable in general
// to get the display with XDisplayName(3); nevertheless, if it
// seems to be a really Unix-like system (*not* wx macro __UNIX_LIKE__)
// use the environment anyway.  The reason is that on some but not
// all systems libX11 symbols will be linkable simply by linking
// with wxGTK.  Those that do require -lX11 in link args cause an
// extra complication in the build process, both makefile and README.
// Still, the README must note that 'USE_XLIB_DISPLAYNAME' and '-lX11'
// *may* be given on make commandline as necessary.
// Update: cannot use __UNIX_LIKE__, it is defined for Unix too
// (e.g. OpenIndiana)
#ifndef USE_XLIB_DISPLAYNAME
#	if ( defined(__UNIX__) || defined(__X__) ) && ! \
		( defined(__VMS__) )
		// ( defined(__UNIX_LIKE__) || defined(__VMS__) )
#		define	USE_XLIB_DISPLAYNAME 0
#	else
#		define	USE_XLIB_DISPLAYNAME 1
#	endif
#endif // #ifndef USE_XLIB_DISPLAYNAME

#if USE_XLIB_DISPLAYNAME
// Just proto this func here so that X* headers are not
// required -- I hope this will be reliable. To date,
// the wxGTK headers are not exposing the X* headers, so
// this should not collide with the real proto.
// NOTE the possible need for -lX11 arg to linker: it
// is not necessary on Ubuntu or *BSD (possibly not with
// any GNU ld[?]), but it is needed on OpenSolaris with
// the SunStudio tools, and probably others.
extern "C" const char* XDisplayName(const char*);
#endif

void
AnApp::GetDisplayData()
{
	DispLocal = true;
	DispName = wxT("default");
	DispHost = wxT("localhost");

	wxString dn;
#	ifdef __X__  // not GTK . .
	// this is not provided by wxGTK, but presumably is for
	// wxUnix and wxMotif
	dn = wxGetDisplayName();
#	else  // . . . GTK . . .
#		if USE_XLIB_DISPLAYNAME
	// get display name with Xlib rather than getenv()
	// because the latter might only be reliable on Unix
	if ( const char* pn = XDisplayName(0) ) {
		dn = ch2wxs(pn);
	}
#		else  // USE_XLIB_DISPLAYNAME
	// assume getenv() will be reliable
	if ( const char* pn = std::getenv("DISPLAY") ) {
		dn = ch2wxs(pn);
	}
#		endif  // USE_XLIB_DISPLAYNAME
#	endif // . . . GTK

	if ( dn.IsEmpty() ) {
		// well, just assume true
		return;
	} else {
		DispName = dn;
	}

	// assuming format of $DISPLAY as with X on Unix --
	// ':' separated -- but I do not now know if that holds
	// elsewhere, e.g. VMS
	int colonpos = dn.Find(wxT(':'));
	if ( colonpos == 0 || colonpos == wxNOT_FOUND ) {
		// well, just assume true
		return;
	}

	wxString svhost = dn.Left(colonpos);

	if ( svhost.IsEmpty() ) {
		// well, must be true
		return;
	}

	// No, the following is wrong. If the diplay name has a host,
	// don't try to determine whether it's the same machine: that
	// would undermine the purpose of localhost, and also X should
	// not be using the non-network optimisations.
#	if 0
	// Now, check host part of display name, BUT:
	// this is not reliable!  E.g., tunneling a remote
	// client app with 'ssh -n -Y "foo"' or the like
	// might put "localhost" in the host-part.
	if ( svhost == wxT("localhost") || svhost == wxT("127.0.0.1") ) {
		return;
	}
	wxString h = wxGetHostName();
	wxString fqh = wxGetFullHostName();
	if ( svhost == h || svhost == fqh ) {
		return;
	}
	// At this point other possibilities still exists, like host name
	// aliases.  A next step might be to resolve host from display
	// name and the current host name and compare, but for now
	// leave it to user to discover the other ways of getting
	// the behavior this function is meant to help determine
	// (environment and menu)
#	endif

	DispLocal = false;
	DispHost = svhost;
}
#else  // defined(__X__) || defined(__WXGTK__)
void
AnApp::GetDisplayData()
{
	DispLocal = true;
	DispName = wxT("default");
	DispHost = wxT("localhost");
}
#endif // defined(__X__) || defined(__WXGTK__)
