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

/**
 * To manage the application settings/preferences/options;
 * should receive stored configuration, and changes, and
 * where appropriate signal code elsewhere of a change.
 * 
 * Also, accomadate the preferences dialog window which is
 * designed to be modeless, and has an "Apply" button which should
 * show changes immediately, but such changes should be stored
 * in a 'tentative' state because the dialog has a "Cancel" button
 * that should do as it says. "OK" button should remove tentative
 * state from changes. Dialog also has restore app defaults, and
 * restore initial saved configuration, and these should be provided
 * for in the settings manager.
 * 
 * The class is not virtual nor are its methods: it should have
 * one instance that stands on its own and if more functionality
 * is needed then edit this class rather than subclass.
 **/

#ifndef _A_PREFSMANAGER_H_
#define _A_PREFSMANAGER_H_

#include "cfg.h"
#include "stdcc.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/intl.h>
#include <wx/config.h>
#include <wx/filename.h>
#endif // WX_PRECOMP

// common data, e.g. version
#include "cdata.h"

// Forward decl. of preferences dialog class
class a_global_pref_dialog;

// C-like struct for the set of preferences
struct prefs_set {
	// for this struct:
	bool is_set;
	// colors
	wxString canvas_background_color;
	wxString canvas_guides_color;
	wxString canvas_grid_color;
	// bools, toggles
	bool canvas_grid_show;
	// strings
	wxString xsuffix;
	wxString povexec;
	wxString povopts;
};

class A_Prefs_Manager {
public:
	A_Prefs_Manager(wxConfigBase* pconfig);
	~A_Prefs_Manager();

protected:
private:
	A_Prefs_Manager()  {}

	wxConfigBase*			pcfg;
	a_global_pref_dialog*	pdlg;

	void read_config();
	void update_from_dialog(prefs_set& pset);
	void update__to__dialog(prefs_set& pset);

	friend class a_global_pref_dialog;
	// event handler overides in friend dialog call these
	void on_close_event(wxCloseEvent& event);
	void on_init_dlg(wxInitDialogEvent& event);
	void on_restore_defs(wxCommandEvent& event);
	void on_restore_conf(wxCommandEvent& event);
	void on_apply(wxCommandEvent& event);
	void on_cancel(wxCommandEvent& event);
	void on_OK(wxCommandEvent& event);
	
public:
	void show_prefs_dialog(bool show);
	void delete_prefs_dialog();
	void force_updates();
	
	// for app classes to get current prefs
	static const prefs_set* get_prefs_set();
};

#endif  // _A_PREFSMANAGER_H_
