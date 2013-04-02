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

// A Frame for GUI application.

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
#include "a_tabpage.h"

// A_Tabpage
//

#if 0
BEGIN_EVENT_TABLE(A_Tabpage, wxNotebook)
	EVT_NOTEBOOK_PAGE_CHANGED  (TabWindowID, A_Tabpage::OnPageChanged)
	EVT_NOTEBOOK_PAGE_CHANGING (TabWindowID, A_Tabpage::OnPageChanging)
END_EVENT_TABLE()
#endif


A_Tabpage::A_Tabpage(wxWindow* parent, wxWindowID id
	, A_Canvas* can, A_Ruler* hr, A_Ruler* vr
	, const wxPoint& pos, const wxSize& size
	, long style, const wxString& name)
       : tabpage_parent_class(parent, id, pos, size, style, name)
       , canvas(can), hrule(hr), vrule(vr)
{
}

A_Tabpage::~A_Tabpage()
{
}
