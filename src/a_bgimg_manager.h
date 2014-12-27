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

#ifndef _A_BGIMG_MANAGER_H_
#define _A_BGIMG_MANAGER_H_

#include "bg_image_dlg.h"

// canvas background image manager for use in app;
// also manages bg img dialog
class bgimg_manager {
	public:
	
	wxWindow* parent_wnd() { return wxGetApp().GetTopWindow(); }
};

namespace ns_bg_img_dlg {
	class bg_img_dlg : public bg_image {
	protected:
		bgimg_manager* mng,

	public:
		bg_img_dlg(
			bgimg_manager* manager,
			wxWindowID id = wxID_ANY,
			const wxString& title = _("Background Image"),
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE);
		~bg_img_dlg();
		
		// Virtual event handler overides
		virtual void on_copy_opt(wxCommandEvent& event);
		virtual void on_flip_horz(wxCommandEvent& event);
		virtual void on_flip_vert(wxCommandEvent& event);
		virtual void on_greyscale(wxCommandEvent& event);
		virtual void on_width(wxCommandEvent& event);
		virtual void on_height(wxCommandEvent& event);
		virtual void on_offs_x(wxCommandEvent& event);
		virtual void on_offs_y(wxCommandEvent& event);
		virtual void on_file select(wxFileDirPickerEvent& event);
		virtual void on_apply(wxCommandEvent& event);
		virtual void on_cancel(wxCommandEvent& event);
		virtual void on_help(wxCommandEvent& event);
		virtual void on_ok(wxCommandEvent& event);
	}; // END class bg_img_dlg
}; // END namespace ns_bg_img_dlg

/*
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class bg_image -- REFRENCE, TEMPORARY
///////////////////////////////////////////////////////////////////////////////
class bg_image : public wxDialog 
{
	private:
	
	protected:
		
		wxStaticBitmap* bmp_preview;
		wxStaticLine* m_staticline1;
		wxStaticLine* m_staticline2;
		wxCheckBox* chk_flhorz;
		wxCheckBox* chk_flvert;
		wxStaticLine* m_staticline8;
		wxStaticLine* m_staticline4;
		wxStaticText* m_staticText3;
		wxSpinCtrl* spin_wi;
		wxStaticLine* m_staticline5;
		wxStaticText* m_staticText4;
		wxSpinCtrl* spin_hi;
		wxStaticLine* m_staticline6;
		wxStaticLine* m_staticline18;
		wxStaticText* m_staticText31;
		wxSpinCtrl* spin_offsx;
		wxStaticLine* m_staticline51;
		wxStaticText* m_staticText311;
		wxSpinCtrl* spin_offsy;
		wxStaticLine* m_staticline511;
		wxFilePickerCtrl* selector_file;
		wxStaticLine* m_staticline7;
		wxStdDialogButtonSizer* button_sizer1;
		wxButton* button_sizer1OK;
		wxButton* button_sizer1Apply;
		wxButton* button_sizer1Cancel;
		wxButton* button_sizer1Help;
		
		
		
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void on_copy_opt( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_flip_horz( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_flip_vert( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_greyscale( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_width( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_height( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_offs_x( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_offs_y( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_file select( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void on_apply( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_cancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_help( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_ok( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxStaticText* bg_ing_label;
		wxRadioBox* opt_save;
		wxCheckBox* chk_greyscale;
		
		bg_image( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Background Image"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~bg_image();
	
};
*/

#endif //_A_BGIMG_MANAGER_H_
