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

#ifndef WX_PRECOMP
#include <wx/gdicmn.h>
#endif // WX_PRECOMP

#include "a_prefs_manager.h"
#include "a_glbprefsdlg.h"
#include "epspline.h"
#include "a_frame.h"
#include "util.h"
// help topic IDs
#include "epspline_helpids.h"

// hack protos: these are defined in povdemo.cpp
void SetPovPref(wxString cfg);
void SetPovOpts(wxString cfg);

// A_Prefs_Manager will need to work with several data, but
// they needn't be class members, not even static; using global
// data limited to this translation unit by anon. namespace
// will keep the class definition smaller. Note there should
// be only one instance of A_Prefs_Manager, and if ever this
// app employs threads or multiple processes this one instance
// should only be used from the main thread/process.
namespace {
	// app defaults
	prefs_set defs = {
		true,				// is_set -- internal use
		wxT("#FFFFFF"),		// canvas_background_color
		wxT("#FF0000"),		// canvas_guides_color
		wxT("#E0E0FF"),		// canvas_grid_color
		true,				// bool canvas_grid_show
		wxT(".inc"),		// xsuffix (default extension)
#ifdef __WXMSW__
		wxT("pvengine.exe"),// povexec -- pathname or basename
#else
		wxT("povray"),		// see above
#endif
		wxT("+D +P +Q9 +A")	// povopts;
	};
	// from config at start
	prefs_set cfgs;
	// last from prefs dialog
	prefs_set last;
	// tentative, after dialog "Apply"
	prefs_set aply;
	// set to current set. for return from static method
	prefs_set current;
};

A_Prefs_Manager::A_Prefs_Manager(wxConfigBase* pconfig)
	: pcfg(pconfig), pdlg(0)
{
	read_config();

	last = cfgs;
	last.is_set = false;

	aply = cfgs;
	aply.is_set = false;
	
	// for external use:
	current = cfgs.is_set ? cfgs : defs;

	// these are not queried on init code; must be set
	SetPovPref(current.povexec);
	SetPovOpts(current.povopts);
}

A_Prefs_Manager::~A_Prefs_Manager()
{
	if ( last.is_set  ) {
		cfgs = last;
	}

	if ( cfgs.is_set  ) {
		wxString ent;
		
		ent = wxT("iface/colors/canvas_bg");
		pcfg->Write(ent, cfgs.canvas_background_color);
		ent = wxT("iface/colors/canvas_guides");
		pcfg->Write(ent, cfgs.canvas_guides_color);
		ent = wxT("iface/colors/canvas_grid");
		pcfg->Write(ent, cfgs.canvas_grid_color);
		ent = wxT("iface/opts/draw_show");
		pcfg->Write(ent, cfgs.canvas_grid_show);
		ent = wxT("user/export_suffix");
		pcfg->Write(ent, cfgs.xsuffix);
		ent = wxT("preview/povray_exec");
		pcfg->Write(ent, cfgs.povexec);
		ent = wxT("preview/povray_options");
		pcfg->Write(ent, cfgs.povopts);
	}
}

void
A_Prefs_Manager::read_config()
{
	cfgs = defs;
	cfgs.is_set = false;
	
	wxString ent,	s_res;
	bool			b_res;
	
	ent = wxT("iface/colors/canvas_bg");
	if ( pcfg->Read(ent, &s_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_background_color = s_res;
	}
	ent = wxT("iface/colors/canvas_guides");
	if ( pcfg->Read(ent, &s_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_guides_color = s_res;
	}
	ent = wxT("iface/colors/canvas_grid");
	if ( pcfg->Read(ent, &s_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_grid_color = s_res;
	}
	ent = wxT("iface/opts/draw_show");
	if ( pcfg->Read(ent, &b_res) ) {
		cfgs.is_set = true;
		cfgs.canvas_grid_show = b_res;
	}
	ent = wxT("user/export_suffix");
	if ( pcfg->Read(ent, &s_res) ) {
		cfgs.is_set = true;
		cfgs.xsuffix = s_res;
	}
	ent = wxT("preview/povray_exec");
	if ( pcfg->Read(ent, &s_res) ) {
		cfgs.is_set = true;
		cfgs.povexec = s_res;
	}
	ent = wxT("preview/povray_options");
	if ( pcfg->Read(ent, &s_res) ) {
		cfgs.is_set = true;
		cfgs.povopts = s_res;
	}
}

void
A_Prefs_Manager::update_from_dialog(prefs_set& pset)
{
	if ( pdlg == 0 ) {
		return;
	}

	wxString item;
	bool bval;

	// the wx docs discourage taking a value from the text control
	// part of a file (or color) picker as that will allow user error,
	// which is good advice. but for the povray exec preference the user
	// is allowed to enter a simple filename w/o slash that will
	// be searched in $PATH, or on MSW used to replace the path
	// returned by the mime-type manager for ".POV".
	//item = pdlg->glb_pov_picker->GetPath();
	item = pdlg->glb_pov_picker->GetTextCtrl()->GetValue();
	item.Trim(true); item.Trim(false);
	if ( item.IsEmpty() ) {
		item = defs.povexec;
	}
	if ( ! pset.is_set ) {
		pset.is_set = (pset.povexec != item);
	}
	pset.povexec = item;

	// pov options text field
	item = pdlg->glb_pov_opts->GetValue();
	item.Trim(true); item.Trim(false);
	if ( ! pset.is_set ) {
		pset.is_set = (pset.povopts != item);
	}
	pset.povopts = item;
	
	// suffix/extension
	item = pdlg->glb_def_suffix->GetValue();
	item.Trim(true); item.Trim(false);
	if ( item.IsEmpty() ) {
		item = defs.xsuffix;
	}
	if ( ! pset.is_set ) {
		pset.is_set = (pset.xsuffix != item);
	}
	pset.xsuffix = item;
	
	// canvas_grid_show -- draw grid chackbox
	bval = pdlg->glb_draw_grid->GetValue();
	if ( ! pset.is_set ) {
		pset.is_set = (pset.canvas_grid_show != bval);
	}
	pset.canvas_grid_show = bval;
	
	// colors
	wxColour clr;
	long clr_flags;
	#if wxCHECK_VERSION(2, 8, 12)
	clr_flags = wxC2S_NAME|wxC2S_CSS_SYNTAX|wxC2S_HTML_SYNTAX;
	#else
	clr_flags = wxC2S_HTML_SYNTAX;
	#endif

	// grid color picker
	clr = pdlg->glb_gridcolor_picker->GetColour();
	item = clr.IsOk() ? clr.GetAsString(clr_flags) : ch2wxs("");
	item.Trim(true); item.Trim(false);
	if ( item.IsEmpty() ) {
		item = defs.canvas_grid_color;
	}
	if ( ! pset.is_set ) {
		pset.is_set = (pset.canvas_grid_color != item);
	}
	pset.canvas_grid_color = item;

	// guides color picker
	clr = pdlg->glb_guidescolor_picker->GetColour();
	item = clr.IsOk() ? clr.GetAsString(clr_flags) : ch2wxs("");
	item.Trim(true); item.Trim(false);
	if ( item.IsEmpty() ) {
		item = defs.canvas_guides_color;
	}
	if ( ! pset.is_set ) {
		pset.is_set = (pset.canvas_guides_color != item);
	}
	pset.canvas_guides_color = item;

	// background color picker
	clr = pdlg->glb_backgroundcolor_picker->GetColour();
	item = clr.IsOk() ? clr.GetAsString(clr_flags) : ch2wxs("");
	item.Trim(true); item.Trim(false);
	if ( item.IsEmpty() ) {
		item = defs.canvas_background_color;
	}
	if ( ! pset.is_set ) {
		pset.is_set = (pset.canvas_background_color != item);
	}
	pset.canvas_background_color = item;

}

void
A_Prefs_Manager::update__to__dialog(prefs_set& pset)
{
	if ( pdlg == 0 ) {
		return;
	}
	
	// suffix/extension
	pdlg->glb_def_suffix->SetValue(pset.xsuffix);
	
	// canvas_grid_show -- draw grid chackbox
	pdlg->glb_draw_grid->SetValue(pset.canvas_grid_show);
	
	// colors : note we are prefering a color name if possible.
	wxColour clr;
	long clr_flags;
	#if wxCHECK_VERSION(2, 8, 12)
	clr_flags = wxC2S_NAME|wxC2S_CSS_SYNTAX|wxC2S_HTML_SYNTAX;
	#else
	clr_flags = wxC2S_HTML_SYNTAX;
	#endif

	// grid color picker
	clr = pset.canvas_grid_color;
	pdlg->glb_gridcolor_picker->SetColour(clr.GetAsString(clr_flags));

	// guides color picker
	clr = pset.canvas_guides_color;
	pdlg->glb_guidescolor_picker->SetColour(clr.GetAsString(clr_flags));

	// background color picker
	clr = pset.canvas_background_color;
	pdlg->glb_backgroundcolor_picker->SetColour(clr.GetAsString(clr_flags));

	// pov exec is special: the file picker only wants an abspath
	// so only place it directly in the picker control if it is
	// abs, exists, and for good measure, is executable; elsewise
	// put it in the picker's text control part for user's pleasure
	wxFileName fn(pset.povexec);
	if ( fn.IsAbsolute() && fn.IsFileExecutable() ) {
		pdlg->glb_pov_picker->SetPath(pset.povexec);
	} else {
		pdlg->glb_pov_picker->GetTextCtrl()->SetValue(pset.povexec);
	}

	// pov options text field
	pdlg->glb_pov_opts->SetValue(pset.povopts);
}

void
A_Prefs_Manager::delete_prefs_dialog()
{
	if ( pdlg ) {
		a_global_pref_dialog*	tmp = pdlg;
		pdlg = 0;
		tmp->Show(false);
		tmp->Destroy();
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
	}

	if ( ! pdlg->IsShown() && show ) {
		update__to__dialog(last);
	}
	
	if ( pdlg->IsIconized() && show ) {
		pdlg->Iconize(false);
	}
	
	if ( pdlg->IsMaximized() && show ) {
		pdlg->Maximize(false);
	}
	
	pdlg->Show(show);
}


void
A_Prefs_Manager::force_updates()
{
	A_Frame* pw =
		dynamic_cast<A_Frame*>(wxGetApp().GetTopWindow());
	if ( pw == 0 ) {
		return;
	}
	pw->PreferenceChanged();
	
	if ( current.is_set ) {
		SetPovPref(current.povexec);
		SetPovOpts(current.povopts);
	}
}

/*static*/ const prefs_set*
A_Prefs_Manager::get_prefs_set()
{
	return current.is_set ? &current : 0;
}

// The dialog's virtual event handlers call these 'on_*' procs.
void
A_Prefs_Manager::on_close_event(wxCloseEvent& event)
{
	current = last;
	current.is_set = true;
	force_updates();

	if ( event.CanVeto() == false ) {
		fputs("epspline: event.CanVeto() == false\n", stderr);
		delete_prefs_dialog();
	} else {
		update__to__dialog(last);
		event.Skip();
	}
}

void
A_Prefs_Manager::on_init_dlg(wxInitDialogEvent& event)
{
}

void
A_Prefs_Manager::on_restore_defs(wxCommandEvent& event)
{
	update__to__dialog(defs);
}

void
A_Prefs_Manager::on_restore_conf(wxCommandEvent& event)
{
	update__to__dialog(cfgs);
}

void
A_Prefs_Manager::on_apply(wxCommandEvent& event)
{
	update_from_dialog(aply);
	if ( aply.is_set ) {
		current = aply;
		force_updates();
	}
}

void
A_Prefs_Manager::on_cancel(wxCommandEvent& event)
{
	update__to__dialog(last);
	current = last;
	current.is_set = true;
	force_updates();
}

void
A_Prefs_Manager::on_OK(wxCommandEvent& event)
{
	on_apply(event);
	if ( aply.is_set ) {
		last = aply;
		aply.is_set = false;
	}
}

void
A_Prefs_Manager::on_help(wxCommandEvent& event)
{
	wxGetApp().ShowHelp(IDI_Preferences);
}
