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

// The Help->About dialog box

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/encconv.h>
#endif

#include "GPL.inc"
#include "FDL.inc"
#include "PERM.inc"

static const wxChar tM[] =
	wxT("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM")
	wxT("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM")
;

#ifdef __GNUG__
    #pragma implementation
#endif
#include "a_aboutbox.h"
#include "epspline.h"
#include "wxutil.h"
#include "cdata.h"

// A_Aboutbox
//

A_Aboutbox::A_Aboutbox(wxWindow* parent, int id, const wxString& title)
	: wxDialog(parent, id, title)
{
	static const wxChar copyright[] = 
		wxT(": an editor for POV-Ray prism and lathe objects\n")
		wxT("Copyright (C) 2000-2013 Edward V. Hynan\n\n")
		wxT("The GNU GENERAL PUBLIC LICENSE version 2 or\n")
		wxT("greater is your license, granted by the author\n")
		wxT("and copyright holder, to use, modify, and redistribute\n")
		wxT("this software.\n\n")
		wxT("The GNU FREE DOCUMENTATION LICENSE version 1.3 or\n")
		wxT("greater is your license, granted by the author\n")
		wxT("and copyright holder, to use, modify, and redistribute\n")
		wxT("the documentation distributed with this software.")
		;
	wxFontEncodingArray fea = wxEncodingConverter::GetPlatformEquivalents(
		wxFONTENCODING_ISO8859_1);
	wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL, false, wxT(""), 
		// Just hoping it's suitable
		fea.GetCount() > 0 ? fea[0] : wxFONTENCODING_DEFAULT
		);
	wxTextAttr tatt(*wxBLACK, *wxWHITE, font);
	//wxString text(wxGetApp().GetAppTitle());
	wxString text(wxT(APPCLASS_IN_ASCII));
	int wi, hi, tw, th;
	::wxDisplaySize(&wi, &hi);
	wi -= 256; hi -= 256;
	GetTextExtent(wxString(tM), &tw, &th, 0, 0, &font);
	th = 3 * tw / 4;
	wxSize sz(std::min(tw, wi), std::min(th, hi));

	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);
	szrMain->Add(20, 20);
	wxButton*      pbok = new wxButton(this, wxID_OK, _("&OK"));
	
	wxNotebook*    book = new wxNotebook(this, -1, wxDefaultPosition
		, wxDefaultSize, 0, wxString(_("About "))+text);
	book->SetAutoLayout(true);
#if wxCHECK_VERSION(2, 6, 0)
	szrMain->Add(book, 1, wxGROW, 1);
#else
	wxNotebookSizer* nbs = new wxNotebookSizer(book);
	szrMain->Add(nbs, 1, wxGROW, 1);
#endif

	szrMain->Add(20, 20);
	szrMain->Add(pbok, 0, wxALIGN_CENTER, 0);
	szrMain->Add(20, 20);
	text += copyright;
	static const wxChar trademarks[] = wxT("\n\n")
		wxT("As stated in the POV-Ray 3.6 for UNIX documentation:\n")
		wxT("\"The terms 'POV-Ray', 'Persistence of Vision Ray Tracer',\n")
		wxT("and 'POV-Team' are trademarks of Persistence of Vision\n")
		wxT("Raytracer Pty. Ltd.\"\n")
		;
	text += trademarks;

	wxPanel* page = new wxPanel(book);
	wxBoxSizer* szr     = new wxBoxSizer(wxVERTICAL);
	wxStaticText* pstxt = new wxStaticText(page, -1, text);
	// Added 2013/04/13
	pstxt->SetFont(font);
	//
	szr->Add(20, 20);
	szr->Add(pstxt, 0, wxALIGN_CENTER
#	if ! wxCHECK_VERSION(2, 9, 0)
		|wxADJUST_MINSIZE, 0);
#	else // ! wxCHECK_VERSION(2, 9, 0)
		, 0);
#	endif // ! wxCHECK_VERSION(2, 9, 0)
	szr->Add(20, 20);
	page->SetSizer(szr);
	book->AddPage(page, _("Copyrights"), true);

	text = ch2wxs(GPL__);
	page = new wxPanel(book);
	wxTextCtrl* ptxtc = new wxTextCtrl(page, -1, text, wxDefaultPosition
		, sz, wxTE_LEFT|wxHSCROLL|wxTE_READONLY|wxTE_MULTILINE);
	ptxtc->SetStyle(0, text.Length(), tatt);
	szr = new wxBoxSizer(wxHORIZONTAL);
	szr->Add(ptxtc, 1, wxGROW, 1);
	page->SetSizer(szr);
	book->AddPage(page, _("Source Code License"), false);

	text = ch2wxs(FDL__);
	page = new wxPanel(book);
	ptxtc = new wxTextCtrl(page, -1, text, wxDefaultPosition
		, sz, wxTE_LEFT|wxHSCROLL|wxTE_READONLY|wxTE_MULTILINE);
	ptxtc->SetStyle(0, text.Length(), tatt);
	szr = new wxBoxSizer(wxHORIZONTAL);
	szr->Add(ptxtc, 1, wxGROW, 1);
	page->SetSizer(szr);
	book->AddPage(page, _("Documentation License"), false);

	text = ch2wxs(PERM__);
	page = new wxPanel(book);
	ptxtc = new wxTextCtrl(page, -1, text, wxDefaultPosition
		, sz, wxTE_LEFT|wxHSCROLL|wxTE_READONLY|wxTE_MULTILINE);
	ptxtc->SetStyle(0, text.Length(), tatt);
	szr = new wxBoxSizer(wxHORIZONTAL);
	szr->Add(ptxtc, 1, wxGROW, 1);
	page->SetSizer(szr);
	book->AddPage(page, _("POV-Team Permission"), false);

	SetSizer(szrMain);
	SetAutoLayout(true);
	Fit();
}

A_Aboutbox::~A_Aboutbox()
{
}
