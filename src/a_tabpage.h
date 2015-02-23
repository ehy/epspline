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

// window type underlying tab page elements canvas and rulers.

#ifndef _A_TABPAGE_H_
#define _A_TABPAGE_H_

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include "a_ruler.h"
#include "a_canvas.h"

#ifdef __GNUG__
    #pragma interface
#endif

// For the working areas of the tabs of the main interface

// NOTE using wxWindow as parent (wxNotebookPage is typedef of wxWindow)
// does *not* automatically size the page; wxPanel works
//typedef wxNotebookPage tabpage_parent_class;
typedef wxPanel tabpage_parent_class;

// base for pages of the tabbed interface
class A_Tabpage_base : public tabpage_parent_class {
public:
	A_Tabpage_base(wxWindow* parent, wxWindowID id
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0, const wxString& name = wxT("tabpage"));
	virtual ~A_Tabpage_base();

	inline static unsigned int
	get_tabpage_count() {
		return tabpage_count;
	}

protected:
	// keep a tab count -- if this code ever uses threads,
	// wxWidgets GUI code should always execute in main thread only
	// so a simple static integer type should suffice
	static unsigned int tabpage_count;
};

// default tab page with pointers to canvas and rulers
class A_Tabpage : public A_Tabpage_base {
public:
	A_Tabpage(wxWindow* parent, wxWindowID id
		, A_Canvas* canvas, A_Ruler* hr, A_Ruler* vr
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0, const wxString& name = wxT("tabpage"));
	~A_Tabpage();

	void SetCanvas(A_Canvas* can) { canvas = can; }
	void SetHRule(A_Ruler* ph) { hrule = ph; }
	void SetVRule(A_Ruler* pv) { vrule = pv; }
	A_Canvas* GetCanvas() { return canvas; }
	A_Ruler* GetHRule() { return hrule; }
	A_Ruler* GetVRule() { return vrule; }

protected:
	A_Canvas* canvas;
	A_Ruler*  hrule;
	A_Ruler*  vrule;
private:
	//DECLARE_EVENT_TABLE()
};

// tab page that manages (e.g., allocates) canvas and rulers
class A_Tabpage_managed : public A_Tabpage {
public:
	A_Tabpage_managed(A_Frame* topframe, wxWindow* parent, wxWindowID id
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0, const wxString& name = wxT("tabpage"));
	~A_Tabpage_managed();

protected:
	A_Frame* frame;
};


#endif  // _A_TABPAGE_H_
