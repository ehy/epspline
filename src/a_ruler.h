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

// main wxWindows ruler class

#ifndef _A_RULER_H_
#define _A_RULER_H_

#include <wx/wx.h>
#include <wx/window.h>

#ifdef __GNUG__
    #pragma interface
#endif

// --------------------------------------------------------------------
// constants
// --------------------------------------------------------------------
enum ruler_t {
	rlr_horz, rlr_vert
};

// Window intended to show graduated ticks, like a ruler
typedef wxWindow ruler_parent_class;

class A_Ruler : public ruler_parent_class {
public:
	A_Ruler(wxWindow* parent, wxWindowID id
		, const wxFont& GradFont, wxColor FontColor, wxColor TickColor
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0);

	~A_Ruler();

	void OnPaint     (wxPaintEvent& event);
	void OnMouseLDown(wxMouseEvent& event);
	void OnMouseLUp  (wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseMove (wxMouseEvent& event);
	void OnKeyDown   (wxKeyEvent& event);
	void OnKeyUp     (wxKeyEvent& event);

	void     PrepareDC(wxDC& dc);
	void     DrawRules(wxDC& dc, const wxRect& r);

	void     SetGradFont(const wxFont& GradFont) { fnt = GradFont; }
	void     SetFontColor(wxColor FontColor) { fclr = FontColor; }
	void     SetTickColor(wxColor TickColor) { clr = TickColor; }
	void     DrawTickLine(int where);
	void     DrawTickLine(wxDC& dc, int where);
	void     ClearTickLine();
	void     ClearTickLine(wxDC&);
	inline   void SetType(ruler_t t) {type = t;}
	inline   int  GetInitOffs() const {return initoffs;}
	inline   void SetInitOffs(int offs) {initoffs = offs;}
	inline   void OffsRefresh(int offs) {
				SetInitOffs(offs); Refresh();
	}
	inline   void SetScalePercent(unsigned pct) {
		scalepct = pct;
		Refresh();
	}
	inline   unsigned GetScalePercent() { return scalepct; }

	inline   bool GetMLDown() const {return gotmldown;}
	inline   void ClearMLDown() {gotmldown = false;}
	inline   bool GetShiftDown() const {return shift_down;}

	typedef void (*DragFptr)(wxWindow* wndCB, wxMouseEvent& e, A_Ruler* r);
	void     SetDragHandler(DragFptr fptr, wxWindow* wndCB);

protected:
	ruler_t    type;
	int        initoffs, linepos;
	unsigned   scalepct;
	bool       gotmldown;
	bool       shift_down;
	wxFont     fnt;
	wxColour   fclr;
	wxColour   clr;
	wxWindow*  DragCallbackWnd;
	DragFptr   DragCallbackFptr;

private:
	DECLARE_EVENT_TABLE()
};

#endif  // _A_RULER_H_
