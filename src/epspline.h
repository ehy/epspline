
// the wx app class

#ifndef _EPSPLINE_H_
#define _EPSPLINE_H_

#include "cfg.h"
#include "stdcc.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/intl.h"
#include "wx/log.h"
#include "wx/config.h"
#include "wx/filename.h"
#include "wx/html/helpctrl.h"
#endif // WX_PRECOMP

// common data, e.g. version
#include "cdata.h"

#ifdef HAVE_BOOL
#undef bool
#endif


#define A_SIZE(a) (sizeof(a)/sizeof((a)[0]))

// these should be in cdata.h
#ifndef  APPNAME_IN_ASCII
#	define  APPNAME_IN_ASCII  "epspline"
#	define  APPCLASS_IN_ASCII  "Epspline"
#endif
#ifndef  HELP_DOC_SUBDIR
#	if defined(__WXMSW__)
#		define  HELP_DOC_SUBDIR  "Help Documents"
#	else
#		define  HELP_DOC_SUBDIR  "doc"
#	endif
#endif

#ifdef __GNUG__
    #pragma interface
#endif

// app class
//

// wxApp derived application class.
class AnApp : public wxApp {
public:
	virtual bool OnInit();
	virtual int OnExit();

	// Return copy, a new string.
	wxString GetAppTitle() const {return AppTitle;}
	wxString GetBinName() const {return av0;}
	// Last dir from a file dialog box.
	wxString GetLastDlgDir() const {return LastDlgDir;}
	wxString SetLastDlgDir(const wxString& s) {
		LastDlgDir = s;
		return LastDlgDir;
	}
	// Return copy, a new string.
	wxString GetFileArg(unsigned idx = 0) const {
		if ( idx < FileArgs.GetCount() )
			return FileArgs[idx];
		return wxString(wxT(""));
	}
	unsigned GetNumFiles() const { return FileArgs.GetCount(); }
	const wxArrayString& GetFileArray() const { return FileArgs; }

	wxMBConv* GetAppMBConv() { return app_mbconv; }
	const wxMBConv* GetAppMBConv() const { return app_mbconv; }

	// Show wx help window, if help_ok indicates setup is OK.
	bool  IsHelpOK() const { return help_ok; }
	bool ShowHelp() {
		if ( help_ok ) {
			help->DisplayContents();
		}
		return help_ok;
	}
	bool ShowHelp(int id) {
		if ( help_ok ) {
			help->Display(id);
		}
		return help_ok;
	}
	bool ShowHelp(const wxString& s) {
		if ( help_ok ) {
			help->Display(s);
		}
		return help_ok;
	}
	void QuitHelp() { help->Quit(); }

	// Try to determine if the display/user IO server is on
	// a remote host or the same machine as this process.
	// Presently this just returns true beyond the
	// X Window System, but might be made useful for
	// other contexts (VNC? I simply don't know).
	// Note that even for X this is not reliable;
	// it is merely a hint.
	bool IsDisplayLocal() const;
	// see comment above
	wxString GetDisplayName() const;
	wxString GetDisplayHost() const;

	// get rc/registry etc. object for reading and writing
	// the object is set to use 'path' with SetPath()
	// path is modified with display name where appropriate
	wxConfigBase* GetCfgPtr(wxString path = wxT("/MainWindow")) const;
	
protected:
	wxString      av0;          // argv[0].
	wxString      AppTitle;     // For frame title.
	wxArrayString FileArgs;     // File args from command line.
	wxString      LastDlgDir;   // Last dir from a file dialog box
	wxString      DispName;     // e.g. $DISPLAY in X
	wxString      DispHost;     // in X display host; localhost elswhere
	bool         DispLocal;    // Display is on this machine (just hint)

	// a member pointer to char converter; avoid wx's wxConvCurrent*
	// directly, and get some slack. Access by public inline function
	wxMBConv*     app_mbconv;
	// wxLocale should be preprocessor-optional
#	if ! NO_USE_WXLOCALE
	wxLocale      app_locale;
#	endif

	// This application makes only a few uses of the wx Logging classes
	// (and they might be removed), but the wx library does make use of
	// them, and unfortunately an unnecessary message is sometimes
	// shown when it is *really* not wanted.  For example, writing
	// documentation in TeX, using TeX4ht package (command htlatex)
	// to make html, the helpview sample (uses wxHtmlHelpController)
	// puts up messages about non-existent anchors.  It's true that
	// the anchors are non-existent, but it is suitable to just show
	// the page from the top if the anchor is not found -- and that's
	// what the help controller does, but with the message box. Well,
	// the user doesn't need to be nagged while viewing help, and I
	// don't need to be forced to change tools, or otherwise worry
	// about working, albeit imperfect, html.  Therefore, the logging
	// must be quieted except for DEBUG builds.
#	ifndef DEBUG
	wxLogNull		shush; // redirects log messages to /dev/null
#	endif

	// see wx sample app "helpview"
	wxHtmlHelpController*	help;
	bool		help_ok;	// help docs added successfully or not

	// setup the the Display data commented above
	void GetDisplayData();
};

DECLARE_APP(AnApp)

#endif  // _EPSPLINE_H_

