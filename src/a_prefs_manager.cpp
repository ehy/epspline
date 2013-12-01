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

#include "a_prefs_manager.h"
#include "a_glbprefsdlg.h"
#include "epspline.h"
#include "a_frame.h"

#ifndef WX_PRECOMP
#include <wx/gdicmn.h>
#endif // WX_PRECOMP


struct prefs_set {
	// for this struct:
	bool is_set;
	// colors
	unsigned long canvas_background_color;
	unsigned long canvas_guides_color;
	unsigned long canvas_grid_color;
	// bools, toggles
	bool canvas_grid_show;
	// strings
	wxString povexec;
};

// A_Prefs_Manager will need to work with several data, but
// they needn't be class members, not even static; global
// data limited to this translation unit by anon. namespace
// will keep the class definition smaller. Note there should
// be only one instance of A_Prefs_Manager, and if ever this
// app employs threads or multiple processes this one instance
// should only be used from the main thread/process.
namespace {
	// app defaults
	prefs_set defs = {
		true,
		0x00FFFFFFlu,
		0x00FF0000lu,
		0x00E0E0FFlu,
		true,
#ifdef __WXMSW__
		wxT("pvengine.exe")
#else
		wxT("povray")
#endif
	};
	// from config at start
	prefs_set cfgs;
	// last from prefs dialog
	prefs_set last;
	// tentative, after dialog "Apply"
	prefs_set aply;
};

A_Prefs_Manager::A_Prefs_Manager(wxConfigBase* pconfig)
	: pcfg(pconfig), pdlg(0)
{
	cfgs = defs;
	cfgs.is_set = false;
	
	wxString ent,	s_res;
	long			l_res;
	bool			b_res;
	
	ent = wxT("iface/colors/canvas_bg");
	if ( pcfg->Read(ent, &l_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_background_color = (unsigned long)l_res;
	}
	ent = wxT("iface/colors/canvas_guides");
	if ( pcfg->Read(ent, &l_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_guides_color = (unsigned long)l_res;
	}
	ent = wxT("iface/colors/canvas_grid");
	if ( pcfg->Read(ent, &l_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_grid_color = (unsigned long)l_res;
	}
	ent = wxT("iface/opts/draw_show");
	if ( pcfg->Read(ent, &b_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_grid_show = b_res;
	}
	ent = wxT("preview/povray_exec");
	if ( pcfg->Read(ent, &s_res) ) {
		cfgs.is_set = true;
		cfgs.povexec = s_res;
	}

	last = cfgs;
	last.is_set = false;

	aply = cfgs;
	aply.is_set = false;
}

A_Prefs_Manager::~A_Prefs_Manager()
{
	if ( aply.is_set ) {
		last = aply;
	}

	if ( last.is_set  ) {
		cfgs = last;
	}

	if ( cfgs.is_set  ) {
		wxString ent;
		
		ent = wxT("iface/colors/canvas_bg");
		pcfg->Write(ent, long(cfgs.canvas_background_color));
		ent = wxT("iface/colors/canvas_guides");
		pcfg->Write(ent, long(cfgs.canvas_guides_color));
		ent = wxT("iface/colors/canvas_grid");
		pcfg->Write(ent, long(cfgs.canvas_grid_color));
		ent = wxT("iface/opts/draw_show");
		pcfg->Write(ent, cfgs.canvas_grid_show);
		ent = wxT("preview/povray_exec");
		pcfg->Write(ent, cfgs.povexec);
	}
}

void
A_Prefs_Manager::update_from_dialog()
{
	if ( pdlg == 0 ) {
		return;
	}

	wxString item;
	// the wx docs discourage taking a value from the text control
	// part of a file (or color) picker as that will allow user error,
	// which is good advice. but for the povray preference the user
	// is allowed to enter a simple filename w/o slash that will
	// be searched in $PATH, or on MSW used to replace the path
	// returned by the mime-type manager for ".POV".
	//item = pdlg->glb_pov_picker->GetPath();
	item = pdlg->glb_pov_picker->GetTextCtrl()->GetValue();
	item.Trim(true); item.Trim(false);
	if ( item.IsEmpty() ) {
		item = defs.povexec;
	}
	if ( ! cfgs.is_set ) {
		cfgs.is_set = (cfgs.povexec != item);
	}
	cfgs.povexec = item;
}

void
A_Prefs_Manager::delete_prefs_dialog()
{
	if ( pdlg ) {
		update_from_dialog();
		pdlg->Show(false);
		delete pdlg;
		pdlg = 0;
	}
}

void
A_Prefs_Manager::show_prefs_dialog(bool show)
{
	if ( pdlg == 0 ) {
		A_Frame* pw =
			dynamic_cast<A_Frame*>(wxGetApp().GetTopWindow());
		if ( pw == 0 ) {
			return;
		}
		pdlg = new A_Prefs_dlg(pw, this);
		pdlg->glb_pov_picker->SetPath(cfgs.povexec);
	}
	
	pdlg->Show(show);
}
