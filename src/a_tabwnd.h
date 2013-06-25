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

// main wxWindows frame class

#ifndef _A_TABWND_H_
#define _A_TABWND_H_

#include <wx/wx.h>

// no aui in wx 2.6
#if wxCHECK_VERSION(2, 8, 0)
#	ifndef NO_AUI_NOTEBOOK
#		define USE_AUI_NOTEBOOK 1
#	endif
#endif

#if USE_AUI_NOTEBOOK
#	include <wx/aui/auibook.h>
	typedef	wxAuiNotebook	NoteBook_type;
#	define	TABWND_EVENT_T	wxAuiNotebookEvent
#	define	TABWND_PAGE_T	wxWindow
#		if wxCHECK_VERSION(2, 9, 0)
#	define	TABWND_CMD_EVENT_T	wxAuiNotebookEvent
#		else
#	define	TABWND_CMD_EVENT_T	wxCommandEvent
#		endif
	const int wxAuiBaseTabCtrlId = 5380;
#else
#	include <wx/notebook.h>
	typedef	wxNotebook	NoteBook_type;
#	define	TABWND_EVENT_T	wxNotebookEvent
#	define	TABWND_PAGE_T	wxNotebookPage
#endif

#include "a_ruler.h"
#include "a_canvas.h"

#ifdef __GNUG__
    #pragma interface
#endif

// fordecl
class A_Frame;

const int TabWindowID = 1777;

// Main frame
class A_Tabwnd : public NoteBook_type {
public:
	A_Tabwnd(A_Frame* parent, wxWindowID id
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0);
	~A_Tabwnd();

	void OnPageChanged(TABWND_EVENT_T& e);
	//void OnPageChanging(TABWND_EVENT_T& e);
#	if USE_AUI_NOTEBOOK
	void OnButton(TABWND_CMD_EVENT_T& e);
#	endif

	bool AddPage(TABWND_PAGE_T* page
		, const wxString& text
		, bool select = FALSE
		, int imageId = -1);

	void SetCurPageText(const wxString& text) {
		SetPageText(GetSelection(), text);
	}

	void UpdateCurPageText();
protected:
	A_Frame* owner;
private:
	DECLARE_EVENT_TABLE()
};

#endif  // _A_TABWND_H_

