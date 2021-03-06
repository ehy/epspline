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

#ifndef _A_ABOUTBOX_H_
#define _A_ABOUTBOX_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/notebook.h>
#endif // WX_PRECOMP

#ifdef __GNUG__
    #pragma interface
#endif

class A_Aboutbox : public wxDialog {
public:
	A_Aboutbox(wxWindow* parent, int id, const wxString& title);
	~A_Aboutbox();
};

#endif  // _A_ABOUTBOX_H_
