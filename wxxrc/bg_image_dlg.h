///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __bg_image_dlg__
#define __bg_image_dlg__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/button.h>
#include <wx/gbsizer.h>
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
	
} // namespace ns_bg_img_dlg

#endif //__bg_image_dlg__
