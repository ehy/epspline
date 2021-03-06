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

// the wx app class

#ifndef _EPSPLINE_H_
#define _EPSPLINE_H_

#include "cfg.h"
#include "stdcc.h"
#include "util.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/app.h>
#include <wx/intl.h>
#include <wx/log.h>
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/html/helpctrl.h>
#include <wx/snglinst.h>
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

// window icons
extern wxIconBundle iconBundle;

#ifdef __GNUG__
    #pragma interface
#endif

// the preferences manager class
class A_Prefs_Manager;

// app class
//

// wxApp derived application class.
class AnApp : public wxApp {

	// WX macro to setup event handling
	// Added 2013/12/04 for MSW shutdown event handling with
	// EVT_END_SESSION() -- w/o this MSW7 queries and barfs
	DECLARE_EVENT_TABLE()

	// These On*EndSession seem to be mostly for MSW's shutdown message
	// handling; there doesn't seem to be an equivalent in the
	// GTK build, or generally a X Window System desktop environment
	void OnQueryEndSession(wxCloseEvent& e);
	void OnEndSession(wxCloseEvent& e);

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

	// get the preferences manager class
	A_Prefs_Manager* GetPrefsManager() { return pprefsmng; }

	// Show wx help window, if help_ok indicates setup is OK.
	bool  IsHelpOK() const { return help_ok; }

	bool ShowHelp() {
		if ( help_ok ) {
			help->DisplayContents();
			help_display_hack();
		}
		return help_ok;
	}

	bool ShowHelp(int id) {
		if ( help_ok ) {
			help->Display(id);
			help_display_hack();
		}
		return help_ok;
	}

	bool ShowHelp(const wxString& s) {
		if ( help_ok ) {
			help->Display(s);
			help_display_hack();
		}
		return help_ok;
	}

	bool SearchHelpGeneral(const wxString& s) {
		if ( help_ok ) {
			return help->KeywordSearch(s);
		}
		return help_ok;
	}

	void QuitHelp() {
		if ( help_ok ) {
			if ( wxConfigBase* pConfig = GetCfgPtr() ) {
				help->WriteCustomization(pConfig);
			}
			help->Quit();
		}
	}

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
	// Help viewer bug: on link#anchor click, viewer often fails
	// to scroll view to #anchor, and will continue to fail to
	// do so until frame/sash resize, after which it will find
	// anchors (at least for a while).
	// This proc is the place to seek work-around solutions.
	void		help_display_hack();

	// Added v 0.0.4.5: MSW will open an instance per file selected
	// in explorer shell (selecting open from right-click menu); it
	// seems that only way to get multiple args passed to one instance
	// is to set up a DropTarget, with UUID and all registry entries,
	// etc., which must wait for the future.
	// For now refuse 2nd instance using wxSingleInstanceChecker
	wxSingleInstanceChecker* single_instance_check;

	// setup the the Display data commented above
	void GetDisplayData();

	// Manager for app-global preferences. Instance of this owns an
	// instance of a modeless preferences dialog window. The dialog
	// window should be deleted in its parent top window dtor, and not
	// after; hence, delete_prefs_dialog() for the main window's use.
	A_Prefs_Manager*		pprefsmng;
public:
	void					delete_prefs_dialog();
	// this is for the main window to show the prefs dialog
	void					show_prefs_dialog(bool show);

};

DECLARE_APP(AnApp)

#endif  // _EPSPLINE_H_

