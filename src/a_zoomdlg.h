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

// dialog with two spin contols, to get user scale values

#ifndef _A_ZOOMDLG_H_
#define _A_ZOOMDLG_H_

#ifdef __GNUG__
    #pragma interface
#endif

#include "a_spintextdlg.h"
#include <wx/window.h>
#include <wx/string.h>


// Main frame
class A_Zoomdlg : public A_Spintextdialog {
	static wxArrayString	as;
	static const wxArrayString& get_as();
public:
	A_Zoomdlg(wxWindow* parent, wxWindowID id);
	~A_Zoomdlg();

	int GetValueX() {
		return A_Spintextdialog::GetValue(0);
	}
	int GetValueY() {
		return A_Spintextdialog::GetValue(1);
	}
	void SetValueX(int value) {
		A_Spintextdialog::SetValue(0, value);
	}
	void SetValueY(int value) {
		A_Spintextdialog::SetValue(1, value);
	}
	void SetRangeX(int min, int max) {
		A_Spintextdialog::SetRange(0, min, max);
	}
	void SetRangeY(int min, int max) {
		A_Spintextdialog::SetRange(1, min, max);
	}
};

#endif  // _A_ZOOMDLG_H_
