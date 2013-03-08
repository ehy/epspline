
// A Simple dialg box presenting a static text,
// zero or more wxSpinCtrl (spinner widget + text entry widget),
// and the usual OK and Cancel buttons.

#ifndef _A_SPINTEXTDLG_H_
#define _A_SPINTEXTDLG_H_

#include "wx/wx.h"
#include "wx/dialog.h"
#include "wx/spinctrl.h"

#include "cfg.h"
#include "stdcc.h"

#ifdef __GNUG__
    #pragma interface
#endif

class A_Spintextdialog : public wxDialog {
	unsigned nctrl;
	wxString text;
	wxSpinCtrl** pspctl;
public:
	A_Spintextdialog(wxWindow* parent, int id
		, const wxString& title, unsigned num_control
		, const wxArrayString& txttxts, const wxString& prompt_text);
	~A_Spintextdialog();

	int GetValue(unsigned index) {
		return (index < nctrl) ? pspctl[index]->GetValue() : INT_MAX;
	}
	void SetValue(unsigned index, int value) {
		if ( index < nctrl )
			pspctl[index]->SetValue(value);
	}
	void SetRange(unsigned index, int min, int max) {
		if ( index < nctrl )
			pspctl[index]->SetRange(min, max);
	}
};

#endif  // _A_SPINTEXTDLG_H_
