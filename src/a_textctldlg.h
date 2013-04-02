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
