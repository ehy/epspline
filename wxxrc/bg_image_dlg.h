///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __BG_IMAGE_DLG_H__
#define __BG_IMAGE_DLG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/gbsizer.h>
#include <wx/slider.h>
#include <wx/filepicker.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace ns_bg_img_dlg
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class bg_image
	///////////////////////////////////////////////////////////////////////////////
	class bg_image : public wxDialog 
	{
		private:
		
		protected:
			wxBoxSizer* bSizer13;
			wxStaticText* bg_img_label;
			wxStaticBitmap* bmp_preview;
			wxStaticText* bg_img_info;
			wxStaticLine* m_staticline1;
			wxStaticLine* m_staticline13;
			wxRadioBox* opt_save;
			wxCheckBox* chk_greyscale;
			wxStaticLine* m_staticline2;
			wxStaticLine* m_staticline181;
			wxCheckBox* chk_flhorz;
			wxCheckBox* chk_flvert;
			wxStaticLine* m_staticline8;
			wxStaticText* m_staticText8;
			wxSpinCtrl* spin_ro;
			wxStaticLine* m_staticline4;
			wxStaticLine* m_staticline19;
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
			wxStaticLine* m_staticline11;
			wxStaticText* m_staticText7;
			wxSlider* hsv_h;
			wxSlider* hsv_s;
			wxSlider* hsv_v;
			wxStaticLine* m_staticline12;
			wxStaticLine* m_staticline111;
			wxStaticText* m_staticText71;
			wxSlider* band_comp;
			wxStaticLine* m_staticline121;
			wxStaticText* m_staticText9;
			wxFilePickerCtrl* selector_file;
			wxStaticLine* m_staticline7;
			wxStdDialogButtonSizer* button_sizer1;
			wxButton* button_sizer1OK;
			wxButton* button_sizer1Apply;
			wxButton* button_sizer1Cancel;
			wxButton* button_sizer1Help;
			
			// Virtual event handlers, overide them in your derived class
			virtual void on_close_event( wxCloseEvent& event ) { event.Skip(); }
			virtual void on_idle_dlg( wxIdleEvent& event ) { event.Skip(); }
			virtual void on_init_dlg( wxInitDialogEvent& event ) { event.Skip(); }
			virtual void on_key_down( wxKeyEvent& event ) { event.Skip(); }
			virtual void on_key_up( wxKeyEvent& event ) { event.Skip(); }
			virtual void on_copy_opt( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_greyscale( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_flip_horz( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_flip_vert( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_rotate( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_width( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_height( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_offs_x( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_offs_y( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_hsv_h_scroll( wxScrollEvent& event ) { event.Skip(); }
			virtual void on_hsv_s_scroll( wxScrollEvent& event ) { event.Skip(); }
			virtual void on_hsv_v_scroll( wxScrollEvent& event ) { event.Skip(); }
			virtual void on_band_comp_scroll( wxScrollEvent& event ) { event.Skip(); }
			virtual void on_file_select( wxFileDirPickerEvent& event ) { event.Skip(); }
			virtual void on_apply( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_cancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_help( wxCommandEvent& event ) { event.Skip(); }
			virtual void on_ok( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			bg_image( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Background Image"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU ); 
			~bg_image();
		
	};
	
} // namespace ns_bg_img_dlg

#endif //__BG_IMAGE_DLG_H__
