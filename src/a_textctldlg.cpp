// A Simple dialg box presenting a static text,
// zero or more wxTextCtrl (spinner widget + text entry widget),
// and the usual OK and Cancel buttons.

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __GNUG__
    #pragma implementation
#endif
#include "wx/spinbutt.h"
#include "wx/textctrl.h"
#include "wx/spinctrl.h"
#include "a_textctldlg.h"

// A_Textctldialog
//

A_Textctldialog::A_Textctldialog(wxWindow* parent, int id
		, const wxString& title, unsigned num_control
		, const wxArrayString& txttxts, const wxArrayString& initxts
		, const wxString& prompt_text)
	: wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize
		, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
	, nctrl(num_control), text(prompt_text)
{
#if wxCHECK_VERSION(2, 4, 0)
	int fll = wxALIGN_LEFT|wxALIGN_TOP;
	int flr = wxALIGN_RIGHT|wxALIGN_TOP;
	int flc = wxALIGN_CENTER;
	int fl  = wxALIGN_LEFT|wxALIGN_RIGHT|wxALIGN_TOP;
	ptxctl = new wxTextCtrl*[nctrl];

	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);
	wxStaticText* pstxt = new wxStaticText(this, -1, text);
	
	wxBoxSizer* szrTxt = new wxBoxSizer(wxHORIZONTAL);
	szrTxt->Add(20, 20);
	szrTxt->Add(pstxt, 0, fll, 0);
	szrTxt->Add(20, 20);
	szrMain->Add(20, 20);
	szrMain->Add(szrTxt, 0, fll, 0);
	szrMain->Add(20, 20);
	
	wxFlexGridSizer* szrG = new wxFlexGridSizer(2, 20, 20);
	szrG->AddGrowableCol(1);
	for ( unsigned n = 0; n < nctrl; n++ ) {
		wxTextCtrl* pt = new wxTextCtrl(this, -1, initxts[n]);
		wxStaticText* ps = new wxStaticText(this, -1, txttxts[n]);
		ps->Fit();
		szrG->Add(ps, 1, flr, 0);
		pt->Fit();
		szrG->Add(pt, 1, wxGROW|fl, 0);
		ptxctl[n] = pt;
	}
	wxBoxSizer* szrH = new wxBoxSizer(wxHORIZONTAL);
	szrH->Add(20, 0);
	szrH->Add(szrG, 1, wxGROW|wxALL, 0);
	szrH->Add(20, 0);
	szrMain->Add(szrH, 0, wxGROW|wxALL, 0);
	szrMain->Add(0, 20);

	#if wxCHECK_VERSION(2, 6, 0) && 0 // Needs work
	wxStdDialogButtonSizer* szrBtn =
		CreateStdDialogButtonSizer(wxID_OK);
	szrBtn->SetNegativeButton(
		new wxButton(this, wxID_CANCEL, _("&Cancel")));
	szrBtn->Realize();
	#else
	wxButton*      pbok = new wxButton(this, wxID_OK, _("&OK"));
	wxButton*      pbca = new wxButton(this, wxID_CANCEL, _("&Cancel"));
	wxBoxSizer* szrBtn = new wxBoxSizer(wxHORIZONTAL);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbca, 0, flc, 0);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbok, 0, flc, 0);
	szrBtn->Add(20, 20);
	// Some GTK themes draw default button border too large:
	// obscures text -- let user tab to button or use accel. or mouse
	#if !defined(__WXGTK__) || 1 // do it anyway!?
	pbok->SetDefault();
	#endif
	#endif
	szrMain->Add(szrBtn, 1, flr, 0);
	szrMain->Add(0, 20);
	
	SetSizer(szrMain);
	szrMain->Layout();
	Fit();
#else // version < 2.4 untested!
	int fl = wxGROW|wxALIGN_LEFT|wxALIGN_TOP|wxALIGN_BOTTOM;
	ptxctl = new wxTextCtrl*[nctrl];
	
	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);
	wxStaticText* pstxt = new wxStaticText(this, -1, text);
	wxButton*      pbok = new wxButton(this, wxID_OK, _("&OK"));
	wxButton*      pbca = new wxButton(this, wxID_CANCEL, _("&Cancel"));
	
	wxBoxSizer* szrTxt = new wxBoxSizer(wxHORIZONTAL);
	szrTxt->Add(20, 20);
	szrTxt->Add(pstxt, 0, fl, 0);
	szrTxt->Add(20, 20);
	szrMain->Add(20, 20);
	szrMain->Add(szrTxt, 0, fl, 0);
	szrMain->Add(20, 20);
	
	for ( unsigned n = 0; n < nctrl; n++ ) {
		wxTextCtrl* pt = new wxTextCtrl(this, -1, initxts[n]);
		wxStaticText* ps = new wxStaticText(this, -1, txttxts[n]);
		wxBoxSizer* psz = new wxBoxSizer(wxHORIZONTAL);
		psz->Add(20, 20);
		psz->Add(ps, 0, fl, 0);
		psz->Add(20, 20);
		psz->Add(pt, 0, fl, 0);
		psz->Add(20, 20);
		szrMain->Add(psz, 0, fl, 0);
		szrMain->Add(20, 20);
		ptxctl[n] = pt;
	}

	wxBoxSizer* szrBtn = new wxBoxSizer(wxHORIZONTAL);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbca, 0, fl, 0);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbok, 0, fl, 0);
	szrBtn->Add(20, 20);
	szrMain->Add(szrBtn, 0, fl, 0);
	szrMain->Add(20, 20);
	
	SetSizer(szrMain);
	pbok->SetDefault();
	szrMain->Layout();
	Fit();
#endif // version
	if ( nctrl )
		ptxctl[0]->SetFocus();
	SetAutoLayout(true);
}

A_Textctldialog::~A_Textctldialog()
{
	delete[] ptxctl;
}
