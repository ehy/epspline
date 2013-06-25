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

// A Simple dialg box presenting a static text,
// zero or more wxSpinCtrl (spinner widget + text entry widget),
// and the usual OK and Cancel buttons.

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef __GNUG__
    #pragma implementation
#endif
#include <wx/spinbutt.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include "a_spintextdlg.h"

// A_Spintextdialog
//

A_Spintextdialog::A_Spintextdialog(wxWindow* parent, int id
		, const wxString& title, unsigned num_control
		, const wxArrayString& txttxts, const wxString& prompt_text)
	: wxDialog(parent, id, title), nctrl(num_control), text(prompt_text)
{
#if wxCHECK_VERSION(2, 4, 0)
	int fll = wxALIGN_LEFT|wxALIGN_TOP;
	int flr = wxALIGN_RIGHT|wxALIGN_TOP;
	int flc = wxALIGN_CENTER;
	int fl  = wxALIGN_LEFT|wxALIGN_RIGHT|wxALIGN_TOP;
	pspctl = new wxSpinCtrl*[nctrl];
	
	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);
	wxStaticText* pstxt = new wxStaticText(this, -1, text);
	wxButton*      pbok = new wxButton(this, wxID_OK, _("&OK"));
	wxButton*      pbca = new wxButton(this, wxID_CANCEL, _("&Cancel"));
	
	wxBoxSizer* szrTxt = new wxBoxSizer(wxHORIZONTAL);
	szrTxt->Add(20, 20);
	szrTxt->Add(pstxt, 0, fll, 0);
	szrTxt->Add(20, 20);
	szrMain->Add(20, 20);
	szrMain->Add(szrTxt, 0, fl, 0);
	szrMain->Add(20, 20);
	
	for ( unsigned n = 0; n < nctrl; n++ ) {
		wxSpinCtrl* pt = new wxSpinCtrl(this, -1, wxT("0"));
		wxStaticText* ps = new wxStaticText(this, -1, txttxts[n]);
		wxBoxSizer* psz = new wxBoxSizer(wxHORIZONTAL);
		psz->Add(20, 20);
		psz->Add(ps, 1, wxGROW|fll, 0);
		psz->Add(20, 20);
		psz->Add(pt, 1, wxGROW|flr, 0);
		psz->Add(20, 20);
		szrMain->Add(psz, 1, fl, 0);
		szrMain->Add(20, 20);
		pspctl[n] = pt;
	}

	wxBoxSizer* szrBtn = new wxBoxSizer(wxHORIZONTAL);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbca, 0, flc, 0);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbok, 0, flc, 0);
	szrBtn->Add(20, 20);
	szrMain->Add(szrBtn, 0, flc, 0);
	szrMain->Add(20, 20);
	
	SetSizer(szrMain);
	#if !defined(__WXGTK__) || 1
	// Some themes draw default button border too large:
	// obscures text -- let user tab to button or use accel. or mouse
	pbok->SetDefault();
	#endif
	szrMain->Layout();
	Fit();
#else // version
	int fl = wxGROW|wxALIGN_LEFT|wxALIGN_TOP|wxALIGN_BOTTOM;
	pspctl = new wxSpinCtrl*[nctrl];
	
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
		wxSpinCtrl* pt = new wxSpinCtrl(this, -1, wxT("0"));
		wxStaticText* ps = new wxStaticText(this, -1, txttxts[n]);
		wxBoxSizer* psz = new wxBoxSizer(wxHORIZONTAL);
		psz->Add(20, 20);
		psz->Add(ps, 0, fl, 0);
		psz->Add(20, 20);
		psz->Add(pt, 0, fl, 0);
		psz->Add(20, 20);
		szrMain->Add(psz, 0, fl, 0);
		szrMain->Add(20, 20);
		pspctl[n] = pt;
	}

	wxBoxSizer* szrBtn = new wxBoxSizer(wxHORIZONTAL);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbok, 0, fl, 0);
	szrBtn->Add(20, 20);
	szrBtn->Add(pbca, 0, fl, 0);
	szrBtn->Add(20, 20);
	szrMain->Add(szrBtn, 0, fl, 0);
	szrMain->Add(20, 20);
	
	SetSizer(szrMain);
	pbok->SetDefault();
	szrMain->Layout();
	Fit();
#endif // __WXMSW__
}

A_Spintextdialog::~A_Spintextdialog()
{
	delete[] pspctl;
}
