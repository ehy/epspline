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

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "globlal_prefs.h"

///////////////////////////////////////////////////////////////////////////

global_pref_dialog::global_pref_dialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	dlg_base_sizer = new wxFlexGridSizer( 3, 3, 20, 0 );
	dlg_base_sizer->AddGrowableCol( 1 );
	dlg_base_sizer->AddGrowableRow( 0 );
	dlg_base_sizer->SetFlexibleDirection( wxBOTH );
	dlg_base_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	dlg_base_sizer->Add( 0, 0, 1, 0, 5 );
	
	dlg_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	tab_global_prefs = new wxPanel( dlg_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 4, 8, 8 );
	fgSizer3->AddGrowableCol( 2 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 8, 16, 1, wxEXPAND, 5 );
	
	m_staticText9 = new wxStaticText( tab_global_prefs, wxID_ANY, _("Default extension/suffix for exports:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer3->Add( m_staticText9, 0, wxALL, 5 );
	
	glb_def_suffix = new wxTextCtrl( tab_global_prefs, wxID_ANY, _(".inc"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB );
	glb_def_suffix->SetMaxLength( 64 ); 
	glb_def_suffix->SetValidator( wxTextValidator( wxFILTER_EXCLUDE_CHAR_LIST, &glb_def_suffix_validator ) );
	glb_def_suffix->SetToolTip( _("Default extension/suffix used when choosing a filename for export to POV-Ray SDL; note that a leading '.' is not added automatically") );
	
	fgSizer3->Add( glb_def_suffix, 0, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 8, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( tab_global_prefs, wxID_ANY, _("Draw grid lines on canvas background:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer3->Add( m_staticText4, 0, wxALL, 5 );
	
	glb_draw_grid = new wxCheckBox( tab_global_prefs, wxID_ANY, _("Check to draw grid"), wxDefaultPosition, wxDefaultSize, 0 );
	glb_draw_grid->SetValue(true); 
	fgSizer3->Add( glb_draw_grid, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer5->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( tab_global_prefs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer5->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 4, 8, 8 );
	fgSizer5->AddGrowableCol( 2 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 8, 16, 1, wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( tab_global_prefs, wxID_ANY, _("Choose background grid color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer5->Add( m_staticText2, 0, wxALL, 5 );
	
	glb_gridcolor_picker = new wxColourPickerCtrl( tab_global_prefs, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL );
	glb_gridcolor_picker->SetToolTip( _("Choose a color for the canvas background grid lines.") );
	
	fgSizer5->Add( glb_gridcolor_picker, 0, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 8, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 8, 16, 1, wxEXPAND, 5 );
	
	m_staticText21 = new wxStaticText( tab_global_prefs, wxID_ANY, _("Choose guides color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer5->Add( m_staticText21, 0, wxALL, 5 );
	
	glb_guidescolor_picker = new wxColourPickerCtrl( tab_global_prefs, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL );
	glb_guidescolor_picker->SetToolTip( _("Choose a color for the canvas guide lines.") );
	
	fgSizer5->Add( glb_guidescolor_picker, 0, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 8, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 8, 16, 1, wxEXPAND, 5 );
	
	m_staticText22 = new wxStaticText( tab_global_prefs, wxID_ANY, _("Choose background color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer5->Add( m_staticText22, 0, wxALL, 5 );
	
	glb_backgroundcolor_picker = new wxColourPickerCtrl( tab_global_prefs, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_USE_TEXTCTRL );
	glb_backgroundcolor_picker->SetToolTip( _("Choose a color for the canvas background.") );
	
	fgSizer5->Add( glb_backgroundcolor_picker, 0, wxEXPAND, 5 );
	
	
	fgSizer5->Add( 8, 16, 1, wxEXPAND, 5 );
	
	bSizer5->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	tab_global_prefs->SetSizer( bSizer5 );
	tab_global_prefs->Layout();
	bSizer5->Fit( tab_global_prefs );
	dlg_notebook->AddPage( tab_global_prefs, _("Global Preferences"), true );
	tab_pov_prefs = new wxPanel( dlg_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 4, 8, 8 );
	fgSizer2->AddGrowableCol( 2 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 8, 16, 1, wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( tab_pov_prefs, wxID_ANY, _("POV-Ray executable file:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText3->Wrap( -1 );
	fgSizer2->Add( m_staticText3, 0, wxALL, 5 );
	
	glb_pov_picker = new wxFilePickerCtrl( tab_pov_prefs, wxID_ANY, wxT("povray"), _("Select a POV-Ray executable file"), wxT("*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_USE_TEXTCTRL );
	glb_pov_picker->SetToolTip( _("Enter the file name of a POV-Ray executable, with a path if necessary, or select a full path to POV-Ray from a file selection dialog window.") );
	
	fgSizer2->Add( glb_pov_picker, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	
	fgSizer2->Add( 8, 16, 1, wxEXPAND, 5 );
	
	bSizer2->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	tab_pov_prefs->SetSizer( bSizer2 );
	tab_pov_prefs->Layout();
	bSizer2->Fit( tab_pov_prefs );
	dlg_notebook->AddPage( tab_pov_prefs, _("POV-Ray Settings"), false );
	
	dlg_base_sizer->Add( dlg_notebook, 1, wxEXPAND | wxALL, 5 );
	
	
	dlg_base_sizer->Add( 0, 0, 1, 0, 5 );
	
	
	dlg_base_sizer->Add( 20, 0, 1, 0, 5 );
	
	wxFlexGridSizer* fgSizer51;
	fgSizer51 = new wxFlexGridSizer( 1, 5, 8, 8 );
	fgSizer51->AddGrowableCol( 1 );
	fgSizer51->AddGrowableCol( 2 );
	fgSizer51->AddGrowableCol( 3 );
	fgSizer51->SetFlexibleDirection( wxBOTH );
	fgSizer51->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer51->Add( 0, 0, 1, wxEXPAND, 5 );
	
	glb_restore_defs = new wxButton( this, wxID_ANY, _("Restore Defaults"), wxDefaultPosition, wxDefaultSize, 0 );
	glb_restore_defs->SetToolTip( _("Restore values from built-in defaults.") );
	
	fgSizer51->Add( glb_restore_defs, 0, wxALL, 5 );
	
	glb_restore_conf = new wxButton( this, wxID_ANY, _("Restore Configured"), wxDefaultPosition, wxDefaultSize, 0 );
	glb_restore_conf->SetToolTip( _("Restore values from configuration at startup.") );
	
	fgSizer51->Add( glb_restore_conf, 0, wxALL, 5 );
	
	dlg_button_sizer = new wxStdDialogButtonSizer();
	dlg_button_sizerOK = new wxButton( this, wxID_OK );
	dlg_button_sizer->AddButton( dlg_button_sizerOK );
	dlg_button_sizerApply = new wxButton( this, wxID_APPLY );
	dlg_button_sizer->AddButton( dlg_button_sizerApply );
	dlg_button_sizerCancel = new wxButton( this, wxID_CANCEL );
	dlg_button_sizer->AddButton( dlg_button_sizerCancel );
	dlg_button_sizer->Realize();
	fgSizer51->Add( dlg_button_sizer, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxEXPAND|wxRIGHT, 5 );
	
	
	fgSizer51->Add( 0, 0, 1, wxEXPAND, 5 );
	
	dlg_base_sizer->Add( fgSizer51, 1, wxEXPAND, 5 );
	
	
	dlg_base_sizer->Add( 20, 0, 1, 0, 5 );
	
	
	dlg_base_sizer->Add( 0, 0, 1, 0, 5 );
	
	
	dlg_base_sizer->Add( 0, 0, 1, 0, 5 );
	
	
	dlg_base_sizer->Add( 0, 0, 1, 0, 5 );
	
	this->SetSizer( dlg_base_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( global_pref_dialog::on_init_dlg ) );
	glb_def_suffix->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( global_pref_dialog::on_def_object_name ), NULL, this );
	glb_def_suffix->Connect( wxEVT_COMMAND_TEXT_MAXLEN, wxCommandEventHandler( global_pref_dialog::on_def_object_name_overflow ), NULL, this );
	glb_gridcolor_picker->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( global_pref_dialog::on_grid_color_select ), NULL, this );
	glb_guidescolor_picker->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( global_pref_dialog::on_grid_color_select ), NULL, this );
	glb_backgroundcolor_picker->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( global_pref_dialog::on_grid_color_select ), NULL, this );
	glb_pov_picker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( global_pref_dialog::on_POV_file_select ), NULL, this );
	glb_restore_defs->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_restore_defs ), NULL, this );
	glb_restore_conf->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_restore_conf ), NULL, this );
	dlg_button_sizerApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_apply ), NULL, this );
	dlg_button_sizerCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_cancel ), NULL, this );
	dlg_button_sizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_OK ), NULL, this );
}

global_pref_dialog::~global_pref_dialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( global_pref_dialog::on_init_dlg ) );
	glb_def_suffix->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( global_pref_dialog::on_def_object_name ), NULL, this );
	glb_def_suffix->Disconnect( wxEVT_COMMAND_TEXT_MAXLEN, wxCommandEventHandler( global_pref_dialog::on_def_object_name_overflow ), NULL, this );
	glb_gridcolor_picker->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( global_pref_dialog::on_grid_color_select ), NULL, this );
	glb_guidescolor_picker->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( global_pref_dialog::on_grid_color_select ), NULL, this );
	glb_backgroundcolor_picker->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( global_pref_dialog::on_grid_color_select ), NULL, this );
	glb_pov_picker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( global_pref_dialog::on_POV_file_select ), NULL, this );
	glb_restore_defs->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_restore_defs ), NULL, this );
	glb_restore_conf->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_restore_conf ), NULL, this );
	dlg_button_sizerApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_apply ), NULL, this );
	dlg_button_sizerCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_cancel ), NULL, this );
	dlg_button_sizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( global_pref_dialog::on_OK ), NULL, this );
}
