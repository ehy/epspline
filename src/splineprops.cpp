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
 * For user management of spline object properties
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "cfg.h"
#include "stdcc.h"
#include "epspline.h"
#include "splines.h"
#include "spline_props.h"
// help topic IDs
#include "epspline_helpids.h"

namespace {
	const int _help_sect = IDI_ContinuedEditing;
	// Dialog subclass for virtual oaverrides
	class _props_dlg : public spline_properties {
	public:
		_props_dlg(wxWindow* parent) : spline_properties(parent) {}
		~_props_dlg() {}
	
	protected:
		// Virtual event handler overides
		virtual void on_help( wxCommandEvent& event ) {
			// Should not invoke help viewer from modal window;
			// what grief we might cause the event processor.
			// It seemed to work with wxGTK 2.8, probably
			// accidentally and temporarily; in 3.0.0 the
			// dialog would never show again, who knows what
			// might happen on MSW.
			// So, if modal, just end returning help ID.
			if ( IsModal() ) {
				EndModal(wxID_HELP);
			} else {
				wxGetApp().ShowHelp(_help_sect);
				event.Skip();
			}
		}
	                 
	private:
	};
}; // namespace

int
SetSplineProps(SplineBase* s, wxWindow* parent)
{
	_props_dlg pd(parent);

	s->InitPropsDialog(pd);
	//pd.Centre(wxBOTH);

	int r = pd.ShowModal();

	switch ( r ) {
		case wxID_OK:
			s->SetProps(pd);
			break;
		case wxID_HELP:
			wxGetApp().ShowHelp(_help_sect);
			//r = SetSplineProps(s, parent);
			r = pd.ShowModal();
			if ( r == wxID_OK ) {
				s->SetProps(pd);
			} else if ( r == wxID_HELP ) {
				wxGetApp().ShowHelp(_help_sect);
			}
			break;
		case wxID_CANCEL:
		default:
			break;
	}
	
	return r;
}

