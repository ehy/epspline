
// A Simple dialg box presenting a static text,
// zero or more wxTextCtrl (spinner widget + text entry widget),
// and the usual OK and Cancel buttons.

#ifndef _A_TEXTCTLDLG_H_
#define _A_TEXTCTLDLG_H

#include "wx/wx.h"
#include "wx/dialog.h"
#include "wx/spinctrl.h"

#include "cfg.h"
#include "stdcc.h"

#ifdef __GNUG__
    #pragma interface
#endif

class A_Textctldialog : public wxDialog {
	unsigned nctrl;
	wxString text;
	wxTextCtrl** ptxctl;
public:
	A_Textctldialog(wxWindow* parent, int id
		, const wxString& title, unsigned num_control
		, const wxArrayString& txttxts, const wxArrayString& initxts
		, const wxString& prompt_text);
	~A_Textctldialog();

	wxString GetValue(unsigned index) {
		return (index < nctrl) ? ptxctl[index]->GetValue() : wxString(wxT(""));
	}
	void SetValue(unsigned index, wxString value) {
		if ( index < nctrl )
			ptxctl[index]->SetValue(value);
	}
};

#endif  // _A_TEXTCTLDLG_H_
