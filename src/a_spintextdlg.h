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

#ifndef _A_SPINTEXTDLG_H_
#define _A_SPINTEXTDLG_H_

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>

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
