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

// A Window for horizontal or vertical rulers.

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef __GNUG__
    #pragma implementation
#endif
#include "cfg.h"
#include "stdcc.h"
#include "a_ruler.h"
#include "util.h"

// A_Ruler
//

BEGIN_EVENT_TABLE(A_Ruler, ruler_parent_class)
	EVT_PAINT        (A_Ruler::OnPaint)
	EVT_LEFT_DOWN    (A_Ruler::OnMouseLDown)
	EVT_LEFT_UP      (A_Ruler::OnMouseLUp)
	EVT_ENTER_WINDOW (A_Ruler::OnMouseEnter)
	EVT_LEAVE_WINDOW (A_Ruler::OnMouseLeave)
	EVT_MOTION       (A_Ruler::OnMouseMove)
	EVT_KEY_DOWN     (A_Ruler::OnKeyDown)
	EVT_KEY_UP       (A_Ruler::OnKeyUp)
END_EVENT_TABLE()


A_Ruler::A_Ruler(wxWindow* parent, wxWindowID id
		, const wxFont& GradFont, wxColor FontColor, wxColor TickColor
		, const wxPoint& pos , const wxSize& size, long style)
	: ruler_parent_class(parent, id, pos, size, style)
	, type(rlr_horz), initoffs(1), linepos(0), scalepct(100)
	, gotmldown(false), shift_down(false)
{
	fnt = GradFont;
	fclr = FontColor;
	clr = TickColor;
	DragCallbackWnd = 0;
	DragCallbackFptr = 0;
}

A_Ruler::~A_Ruler()
{
}

void
A_Ruler::SetDragHandler(DragFptr fptr, wxWindow* wndCB)
{
	DragCallbackWnd = wndCB;
	DragCallbackFptr = fptr;
}

void
A_Ruler::OnMouseMove(wxMouseEvent& event)
{
	if ( DragCallbackFptr && event.Dragging() )
		DragCallbackFptr(DragCallbackWnd, event, this);
	event.Skip();
}

void
A_Ruler::OnMouseLUp(wxMouseEvent& event)
{
	if ( DragCallbackFptr )
		DragCallbackFptr(DragCallbackWnd, event, this);
	gotmldown = false;
	shift_down = false;
	event.Skip();
}

void
A_Ruler::OnMouseLDown(wxMouseEvent& event)
{
	gotmldown = true;
	event.Skip();
}

void
A_Ruler::OnMouseEnter(wxMouseEvent& event)
{
	Refresh(); Update();
	//linepos = 0;
	if ( DragCallbackFptr )
		DragCallbackFptr(DragCallbackWnd, event, this);
	event.Skip();
}

void
A_Ruler::OnMouseLeave(wxMouseEvent& event)
{
	Refresh(); Update();
	//linepos = 0;
	if ( DragCallbackFptr )
		DragCallbackFptr(DragCallbackWnd, event, this);
	event.Skip();
}

void
A_Ruler::OnKeyDown(wxKeyEvent& e)
{
#if defined(__WXX11__) || defined(__WXMOTIF__)
	if ( e.GetKeyCode() == WXK_SHIFT && e.GetEventObject() == this ) {
		shift_down = true;
		return;
	}
#endif
	e.Skip();
}

void
A_Ruler::OnKeyUp(wxKeyEvent& e)
{
#if defined(__WXX11__) || defined(__WXMOTIF__)
	if ( e.GetKeyCode() == WXK_SHIFT && e.GetEventObject() == this ) {
		shift_down = false;
		return;
	}
#endif
	e.Skip();
}

void
A_Ruler::ClearTickLine()
{
	if ( !linepos )
		return;

	wxClientDC dc(this);
	ClearTickLine(dc);
}

void
A_Ruler::ClearTickLine(wxDC& dc)
{
	if ( !linepos )
		return;

	int w0, h0, w1, h1;
	GetSize(&w1, &h1);
	GetClientSize(&w0, &h0);
	w1 -= w0; w1 /= 2;
	h1 -= h0; h1 /= 2;

	w0 = dc.DeviceToLogicalXRel(w0);
	h0 = dc.DeviceToLogicalYRel(h0);

	wxPen op = dc.GetPen();
#	if wxCHECK_VERSION(2, 9, 0)
	wxRasterOperationMode of = dc.GetLogicalFunction();
#	else
	int of = dc.GetLogicalFunction();
#	endif

	dc.SetPen(*wxGREY_PEN);
	dc.SetLogicalFunction(wxINVERT);

	if ( type == rlr_horz ) {
		int p = dc.DeviceToLogicalXRel(linepos - w1);
		dc.DrawLine(p, 0, p, h0);
	} else {
		int p = dc.DeviceToLogicalYRel(linepos - h1);
		dc.DrawLine(0, p, w0, p);
	}

	dc.SetPen(op);
	dc.SetLogicalFunction(of);

	linepos = 0;
}

void
A_Ruler::DrawTickLine(int w)
{
	wxClientDC dc(this);
	DrawTickLine(dc, w);
}

void
A_Ruler::DrawTickLine(wxDC& dc, int w)
{
	if ( linepos == w ) {
		return;
	}

	// Get Rid of any previous line
	if ( linepos ) {
		ClearTickLine(dc);  // Sets linepos 0;
	}

	// Trick Clear...() into drawing.
	linepos = w;
	ClearTickLine(dc);
	// And this beacuse Clear...() sets linepos 0.
	linepos = w;
}

void
A_Ruler::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);
	PrepareDC(dc);

	wxRegion re = GetUpdateRegion();
	wxRegionIterator ri(re);
	bool cl = (linepos != 0);
	int l = (type == rlr_horz) ?
		dc.DeviceToLogicalXRel(linepos) :
		dc.DeviceToLogicalYRel(linepos);

	while ( ri ) {
		const wxRect& r = ri.GetRect();
		DrawRules(dc, r);
		if ( cl && type == rlr_horz ) {
			if ( l >= r.x && l <= (r.x + r.width) ) {
				cl = false;
				linepos = 0;
			}
		} else if ( cl ) {
			if ( l >= r.y && l <= (r.y + r.height) ) {
				cl = false;
				linepos = 0;
			}
		}
		ri++;
	}

	if ( cl )
		ClearTickLine(dc);
}

void
A_Ruler::DrawRules(wxDC& dc, const wxRect& R)
{
	const int wid = 5;
	int xs = dc.DeviceToLogicalXRel(wid);
	int ys = dc.DeviceToLogicalYRel(wid);
	wxRect r(R);

	r.width = dc.DeviceToLogicalX(r.x + r.width);  
	r.x = dc.DeviceToLogicalX(r.x);
	r.height = dc.DeviceToLogicalY(r.y + r.height);
	r.y = dc.DeviceToLogicalY(r.y);
	r.width -= r.x; r.height -= r.y;

	// If there is a border, it must be accounted for.
	int w0, w1, h0, h1;
	GetSize(&w0, &h0);
	GetClientSize(&w1, &h1);
	w0 -= w1; w0 /= 2;
	h0 -= h1; h0 /= 2;

	wxPen op = dc.GetPen();
	wxFont of = dc.GetFont();
	wxColour otfg = dc.GetTextForeground();

	wxPen np(clr, 1, wxSOLID);
	dc.SetPen(np);
	dc.SetFont(fnt);
	dc.SetTextForeground(fclr);

	if ( type == rlr_horz ) {
		int adj = w0 + initoffs;
		int offs = r.x - (r.x % xs) - adj;
		int cnt = ((r.width + r.x) - offs) / xs;
		int ltop = h1 / 2;
		wxString n;
		while ( cnt-- > 0 ) {
			offs += xs;
			int t = offs + adj;
			int tt = t % 100;
			switch ( tt ) {
				case 75: case 50: case 25: case  0:
					n = wxT(""); n << (t * 100 / scalepct);
					dc.GetTextExtent(n, &w0, &h0);
					w1 = offs - w0 / 2;
					dc.DrawLine(offs, ltop+(ltop/2),
						offs, r.y+ltop*2);
					if ( tt == 25 || tt == 75 )
						break;
					dc.DrawText(n, w1, 1);
					#ifdef __WXMOTIF__
					dc.SetPen(np);
					#endif
					break;
				default:
					switch ( t % 25 ) {
						case 10: case 15:
						dc.DrawLine(offs, ltop/2,
							offs, r.y+ltop*2);
						break;
						default:
						dc.DrawLine(offs, ltop,
							offs, r.y+ltop*2);
						break;
					}
					break;
			}
		}
	} else if ( type == rlr_vert ) {
		int adj = h0 + initoffs;
		int offs = r.y - (r.y % ys) - adj;
		int cnt = ((r.height + r.y) - offs) / ys;
		int ltop = w1 / 2;
		wxString n;
		while ( cnt-- > 0 ) {
			offs += ys;
			int t = offs + adj;
			int tt = t % 100;
			switch ( tt ) {
				case 75: case 50: case 25: case  0:
					n = wxT(""); n << (t * 100 / scalepct);
					dc.GetTextExtent(n, &w0, &h0);
					if ( tt == 25 || tt == 75 ) {
						dc.DrawLine(ltop+ltop/2,
							offs, r.x+ltop*2, offs);
						break;
					}
					w0 = std::max(ltop+ltop/2, w0 + 1);
					dc.DrawLine(w0,
						offs, r.x+ltop*2, offs);
					h1 = offs - h0 / 2 + 1;//
					dc.DrawText(n, 1, h1);
					#ifdef __WXMOTIF__
					dc.SetPen(np);
					#endif
					break;
				default:
					switch ( t % 25 ) {
						case 10: case 15:
						dc.DrawLine(ltop/2, offs,
							r.y+ltop*2, offs);
						break;
						default:
						dc.DrawLine(ltop, offs,
							r.y+ltop*2, offs);
						break;
					}
					break;
			}
		}
	}

	dc.SetPen(op);
	dc.SetFont(of);
	dc.SetTextForeground(otfg);
}

void
A_Ruler::PrepareDC(wxDC& dc)
{
	dc.SetMapMode(wxMM_TEXT);
}

