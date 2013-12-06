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

#ifndef __spline_props__
#define __spline_props__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class spline_properties
///////////////////////////////////////////////////////////////////////////////
class spline_properties : public wxDialog 
{
	private:
	
	protected:
		
		
		
		
		wxStaticLine* m_staticline3;
		wxStaticText* m_staticText5;
		wxStaticLine* m_staticline1;
		wxStaticLine* m_staticline2;
		wxStaticLine* m_staticline8;
		wxStaticLine* m_staticline4;
		wxStaticText* m_staticText3;
		wxStaticLine* m_staticline5;
		wxStaticText* m_staticText4;
		wxStaticLine* m_staticline6;
		wxStaticText* m_staticText6;
		wxStaticText* Interior_label;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxStaticLine* m_staticline7;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		
		
		
	
	public:
		wxStaticText* spline_type_label;
		wxTextCtrl* txt_objname;
		wxRadioBox* pov_type;
		wxRadioBox* sweep_type;
		wxCheckBox* check_sturm;
		wxCheckBox* check_open;
		wxTextCtrl* txt_sweep_min;
		wxTextCtrl* txt_sweep_max;
		wxTextCtrl* txt_texture;
		wxTextCtrl* txt_interior;
		wxTextCtrl* txt_transform;
		wxTextCtrl* txt_predefined;
		wxString txt_objname_validator; 
		wxString txt_sweep_min_validator; 
		wxString txt_sweep_max_validator; 
		wxString txt_texture_validator; 
		wxString txt_interior_validator; 
		wxString txt_transform_validator; 
		wxString txt_predefined_validator; 
		
		spline_properties( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Spline Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("spline_properties") );
		~spline_properties();
	
};

#endif //__spline_props__
