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

// Dialog for global preferences, with data validation
typedef class a_global_pref_dialog : public global_pref_dialog {
public:
	a_global_pref_dialog(A_Frame* parent, wxWindowID id = wxID_ANY);
	~a_global_pref_dialog();

protected:
	// Virtual event handler overides
	virtual void on_def_object_name( wxCommandEvent& event ) { event.Skip(); }
	virtual void on_def_object_name_overflow( wxCommandEvent& event ) { event.Skip(); }
	virtual void on_grid_color_select( wxColourPickerEvent& event ) { event.Skip(); }
	virtual void on_POV_file_select( wxFileDirPickerEvent& event ) { event.Skip(); }
	virtual void on_init_dlg( wxInitDialogEvent& event ) { event.Skip(); }
	virtual void on_apply( wxCommandEvent& event ) { event.Skip(); }
	virtual void on_cancel( wxCommandEvent& event ) { event.Skip(); }
	virtual void on_OK( wxCommandEvent& event ) { event.Skip(); }
                 
private:
} A_Prefs_dlg;

#endif  // _A_GLBPREFSDLG_H_
