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

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __globlal_prefs__
#define __globlal_prefs__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/clrpicker.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/filepicker.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class global_pref_dialog
///////////////////////////////////////////////////////////////////////////////
class global_pref_dialog : public wxDialog 
{
	private:
	
	protected:
		wxFlexGridSizer* dlg_base_sizer;
		
		wxNotebook* dlg_notebook;
		wxPanel* tab_global_prefs;
		
		
		
		
		
		wxStaticText* m_staticText9;
		wxTextCtrl* glb_def_suffix;
		
		
		wxStaticText* m_staticText4;
		wxCheckBox* glb_draw_grid;
		
		wxStaticLine* m_staticline1;
		
		
		
		
		
		wxStaticText* m_staticText2;
		wxColourPickerCtrl* glb_gridcolor_picker;
		
		
		wxStaticText* m_staticText21;
		wxColourPickerCtrl* glb_guidescolor_picker;
		
		
		wxStaticText* m_staticText22;
		wxColourPickerCtrl* glb_backgroundcolor_picker;
		
		wxPanel* tab_pov_prefs;
		
		
		
		
		
		wxStaticText* m_staticText3;
		wxFilePickerCtrl* glb_pov_picker;
		
		
		wxStaticText* m_staticText31;
		wxTextCtrl* glb_pov_opts;
		
		
		
		wxButton* glb_restore_defs;
		wxButton* glb_restore_conf;
		wxStdDialogButtonSizer* dlg_button_sizer;
		wxButton* dlg_button_sizerOK;
		wxButton* dlg_button_sizerApply;
		wxButton* dlg_button_sizerCancel;
		wxButton* dlg_button_sizerHelp;
		
		
		
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void on_close_event( wxCloseEvent& event ) { event.Skip(); }
		virtual void on_init_dlg( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void on_def_object_name( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_def_object_name_overflow( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_grid_color_select( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void on_POV_file_select( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void on_restore_defs( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_restore_conf( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_apply( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_cancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_help( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_OK( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString glb_def_suffix_validator; 
		
		global_pref_dialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Epspline Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,605 ), long style = wxCAPTION|wxCLOSE_BOX, const wxString& name = wxT("global_pref_dialog") );
		~global_pref_dialog();
	
};

#endif //__globlal_prefs__
