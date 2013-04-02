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
#include "a_zoomdlg.h"

A_Zoomdlg::A_Zoomdlg(wxWindow* parent, wxWindowID id)
	: A_Spintextdialog(parent, id, _("Set Scale"), 2
	, get_as(), _("Set scale (zoom) as percent:"))
{

}

A_Zoomdlg::~A_Zoomdlg()
{

}

wxArrayString	A_Zoomdlg::as;

const wxArrayString&
A_Zoomdlg::get_as()
{
	static int init;
	
	if ( !init ) {
		as.Add(_("Scale X: "));
		as.Add(_("Scale Y: "));
		init = 1;
	}
	
	return as;
}
