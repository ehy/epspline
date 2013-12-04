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

#include "a_glbprefsdlg.h"
#include "a_prefs_manager.h"
#include "a_frame.h"

a_global_pref_dialog::a_global_pref_dialog(
	A_Frame* parent,
	A_Prefs_Manager* prefs_manager,
	wxWindowID id)
	: global_pref_dialog(parent, id), pm(prefs_manager)
{
}

a_global_pref_dialog::~a_global_pref_dialog()
{
}

void
a_global_pref_dialog::on_close_event(wxCloseEvent& event)
{
	pm->on_close_event(event);
	// let pm decide on Skip()
	//event.Skip();
}
void
a_global_pref_dialog::on_init_dlg(wxInitDialogEvent& event)
{
	pm->on_init_dlg(event);
	event.Skip();
}
void
a_global_pref_dialog::on_restore_defs(wxCommandEvent& event)
{
	pm->on_restore_defs(event);
	event.Skip();
}
void
a_global_pref_dialog::on_restore_conf(wxCommandEvent& event)
{
	pm->on_restore_conf(event);
	event.Skip();
}
void
a_global_pref_dialog::on_apply(wxCommandEvent& event)
{
	pm->on_apply(event);
	event.Skip();
}
void
a_global_pref_dialog::on_cancel(wxCommandEvent& event)
{
	pm->on_cancel(event);
	event.Skip();
}
void
a_global_pref_dialog::on_OK(wxCommandEvent& event)
{
	pm->on_OK(event);
	event.Skip();
}
