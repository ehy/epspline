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
#include "a_tabpage.h"
#include "a_frame.h"

// A_Tabpage
//

unsigned int
A_Tabpage_base::tabpage_count = 0;

A_Tabpage_base::A_Tabpage_base(wxWindow* parent, wxWindowID id
	, const wxPoint& pos, const wxSize& size
	, long style, const wxString& name)
	: tabpage_parent_class(parent, id, pos, size, style, name)
{
	++tabpage_count;
	DBGERR2("Epspline: CTor; tabpage_count == %u\n", tabpage_count);
}

A_Tabpage_base::~A_Tabpage_base()
{
	if ( tabpage_count < 1 ) {
		// bad, bad, bad
		fprintf(stderr,
			"Epspline internal error: tabpage_count == %u\n",
			tabpage_count
		);
	} else {
		--tabpage_count;
	}
	DBGERR2("Epspline: DTor; tabpage_count == %u\n", tabpage_count);
}

#if 0 // not needed so far
BEGIN_EVENT_TABLE(A_Tabpage, A_Tabpage_base)
	EVT_NOTEBOOK_PAGE_CHANGED  (TabWindowID, A_Tabpage::OnPageChanged)
	EVT_NOTEBOOK_PAGE_CHANGING (TabWindowID, A_Tabpage::OnPageChanging)
END_EVENT_TABLE()
#endif

A_Tabpage::A_Tabpage(wxWindow* parent, wxWindowID id
	, A_Canvas* can, A_Ruler* hr, A_Ruler* vr
	, const wxPoint& pos, const wxSize& size
	, long style, const wxString& name)
	: A_Tabpage_base(parent, id, pos, size, style, name)
	, canvas(can), hrule(hr), vrule(vr)
{
}

A_Tabpage::~A_Tabpage()
{
}

A_Tabpage_managed::A_Tabpage_managed(A_Frame* topframe
	, wxWindow* parent, wxWindowID id
	, const wxPoint& pos, const wxSize& size
	, long style, const wxString& name)
	: A_Tabpage(parent, id, 0, 0, 0, pos, size, style, name)
	, frame(topframe)
{
	// allocate the canvas window
	bool aadraw = frame->GetAADraw();
	if ( const char* p = std::getenv("EPSPLINE_BUFFER_CANVAS") ) {	
		if ( ! std::strcmp(p, "1") || ! std::strcmp(p, "true") ) {
			canvas = new A_BUFDCCanvas(frame, this, aadraw);
		} else {
			canvas = new A_Canvas(frame, this, aadraw);
		}
	} else {
		// A platform's own double buffering makes
		// the buffered canvas redundant and *slow*.
		// The platform macro tests were found in dcbuffer.h;
		// so far only GTK2 is tested.
		// Note GTK2 no longer really needs this test since
		// finding 'SetDoubleBuffered(bool)', which is used in
		// buffered canvas ctor; using that, GTK2 performance is
		// not perceptibly different either way.
#		if defined(__WXGTK20__) || \
			defined(__WXMAC__) || defined(__WXDFB__)
			canvas = new A_Canvas(frame, this, aadraw);
#		else // defined(__WXGTK__)
			canvas = new A_BUFDCCanvas(frame, this, aadraw);
#		endif // defined(__WXGTK__)
	}

	// ugly hack to get font and font based size for rulers
	// -- get font based size into wxSize sz
	// according to string that is wide enough --
	wxSize sz;
	wxFont fnt;
	// scope the device context in a block
	{
		wxWindowDC dc(frame);
		dc.SetMapMode(wxMM_TEXT);
		wxFont of = dc.GetFont();
		#if defined(__WXMSW__)
		fnt.SetStyle(wxNORMAL);
		fnt.SetFamily(wxSWISS);
		fnt.SetFaceName(wxT("")); // BUG: Always set to MS Sans Serif, a raster.
		#else
		fnt = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
		fnt.SetWeight(wxLIGHT);
		#endif
		#if !defined(__WXMOTIF__)
		fnt.SetPointSize(7);
		#endif
		dc.SetFont(fnt);
	
		// try "00000" or "MMMMM" (wider)
		dc.GetTextExtent(wxString(wxT("000000")), &sz.x, &sz.y);
		sz.y = sz.x;
	
		// device context should be given back its original resources
		dc.SetFont(of);
	}

	// . . . for these the GetTextExtent hack is insufficient:
	#if defined(__WXMSW__) || defined(__WXX11__)
	sz.x += 6;
	sz.y += 6;
	#elif !defined(__WXGTK__)
	sz.x += 2 * wxSystemSettings::GetMetric(wxSYS_EDGE_Y);
	sz.y += 2 * wxSystemSettings::GetMetric(wxSYS_EDGE_Y);
	#endif

	// ruler colors
	wxColor fclr(0x00, 0x00, 0x00);
	wxColor tclr(0x40, 0x40, 0xFF);

	// Style used in 'rulers': these were chosen long ago, but have
	// been producing nice results until wx 2.9.x, when things began
	// to change: the GTK2 border with wx 3.0.0 is not "raised" but
	// is just a thick white outline taking too much space.
	// The MSW build still displays as wanted.
	#if defined(__WXMSW__)
	// The MS build seems to need this to get the 3d look . . .
	int rstyle = wxDOUBLE_BORDER;
	#elif wxCHECK_VERSION(2, 9, 5) // comment above: 1st seen in 2.9.5
	// no border looks OK after just a moment getting used to it;
	// I think the 'flat look' is the current rage in design, too.
	// Update in 0.0.4.5: change default to wxBORDER_THEME and
	// allow change via macro:
	#	if defined(GRADUATED_RULE_BORDER)
	int rstyle = GRADUATED_RULE_BORDER;
	#	else
	//int rstyle = wxBORDER_NONE; // new names, 'BORDER' comes first
	int rstyle = wxBORDER_THEME;
	#	endif // defined(GRADUATED_RULE_BORDER)
	#else
	int rstyle = wxRAISED_BORDER;
	#endif
	const int ticks_offs = 2;

	// wxWidgets sizers
	wxBoxSizer* szrTop  = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* szrBot  = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);

	// top-left button
	wxWindow* tlbut = new ruler_parent_class
		(this, -1, wxDefaultPosition, sz, rstyle);
	szrTop->Add(tlbut
		, 0, wxALIGN_LEFT|wxALIGN_TOP, 0);

	// top ruler
	A_Ruler* hr = new A_Ruler(this
		, -1, fnt, fclr, tclr
		, wxDefaultPosition, sz, rstyle);
	hr->SetInitOffs(ticks_offs);
	hr->SetType(rlr_horz);
	szrTop->Add(hr, 1
		, wxALIGN_RIGHT|wxALIGN_LEFT|wxALIGN_TOP|wxGROW
		, 0);
	canvas->SetHRule(hr);

	// side ruler
	A_Ruler* vr = new A_Ruler(this
		, -1, fnt, fclr, tclr
		, wxDefaultPosition, sz, rstyle);
	vr->SetInitOffs(ticks_offs);
	vr->SetType(rlr_vert);
	szrBot->Add(vr, 0
		, wxGROW|wxALIGN_LEFT|wxALIGN_TOP|wxALIGN_BOTTOM
		, 0);
	canvas->SetVRule(vr);

	szrBot->Add(canvas, 1, wxGROW|wxALL, 0);

	// add sub-sizers
	szrMain->Add(szrTop, 0
		, wxGROW|wxALIGN_RIGHT|wxALIGN_LEFT|wxALIGN_TOP, 0);
	szrMain->Add(szrBot, 1, wxGROW|wxALL, 0);

#if wxCHECK_VERSION(2, 4, 0)
	bool theme_enab = parent->GetThemeEnabled();
	tlbut->SetThemeEnabled(theme_enab);
	hr->SetThemeEnabled(theme_enab);
	vr->SetThemeEnabled(theme_enab);
#endif

	SetHRule(hr);
	SetVRule(vr);

	// put main sizer
	SetSizer(szrMain);
	SetAutoLayout(true);
}

A_Tabpage_managed::~A_Tabpage_managed()
{
}

