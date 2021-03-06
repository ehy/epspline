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

#ifndef _A_GLBPREFSDLG_H_
#define _A_GLBPREFSDLG_H_
// wxFormBuilder generated base class for this dialog:
#include "globlal_prefs.h"

// forward decl. of main window class
class A_Frame;
// forward decl. preferences manager class
class A_Prefs_Manager;

// Dialog for global preferences, with data validation
typedef class a_global_pref_dialog : public global_pref_dialog {
	// allow prefs manager any access
	friend	class A_Prefs_Manager;
	A_Prefs_Manager*	pm;

public:
	a_global_pref_dialog(
		A_Frame* parent,
		A_Prefs_Manager* prefs_manager,
		wxWindowID id = wxID_ANY
	);
	~a_global_pref_dialog();

protected:
	// Virtual event handler overides
	virtual void on_close_event(wxCloseEvent& event);
	virtual void on_init_dlg(wxInitDialogEvent& event);
	virtual void on_restore_defs(wxCommandEvent& event);
	virtual void on_restore_conf(wxCommandEvent& event);
	virtual void on_apply(wxCommandEvent& event);
	virtual void on_cancel(wxCommandEvent& event);
	virtual void on_help(wxCommandEvent& event);
	virtual void on_OK(wxCommandEvent& event);
                 
private:
} A_Prefs_dlg;

#endif  // _A_GLBPREFSDLG_H_
