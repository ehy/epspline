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

#include "cfg.h"
#include "stdcc.h"
#include "epspline.h"
#include "splines.h"
#include "spline_props.h"
// help topic IDs
#include "epspline_helpids.h"

#include "a_bgimg_manager.h"

// The generated dialog base is in namespace "ns_bg_img_dlg",
// and derive with that ns.
namespace ns_bg_img_dlg {
bg_img_dlg::bg_img_dlg(
	bgimg_manager* manager,
	wxWindowID id = wxID_ANY,
	const wxString& title = _("Background Image"),
	const wxPoint& pos = wxDefaultPosition,
	const wxSize& size = wxDefaultSize,
	long style = wxDEFAULT_DIALOG_STYLE)
: bg_image(manager->parent_wnd(), id, title, wxPoint& pos, size, style)
, mng(manager)
{
}

bg_img_dlg::~bg_img_dlg()
{
}

void
bg_img_dlg::on_copy_opt(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_flip_horz(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_flip_vert(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_greyscale(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_width(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_height(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_offs_x(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_offs_y(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_file select(wxFileDirPickerEvent& event)
{

}

void
bg_img_dlg::on_apply(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_cancel(wxCommandEvent& event)
{

}

void
bg_img_dlg::on_help(wxCommandEvent& event)
{
	// Should not invoke help viewer from modal window;
	// what grief we might cause the event processor.
	// It seemed to work with wxGTK 2.8, probably
	// accidentally and temporarily; in 3.0.0 the
	// dialog would never show again, who knows what
	// might happen on MSW.
	// So, if modal, just end returning help ID.
	if ( IsModal() ) {
		EndModal(wxID_HELP);
	} else {
		wxGetApp().ShowHelp(_help_sect);
		event.Skip();
	}
}

void
bg_img_dlg::on_ok(wxCommandEvent& event)
{

}
// END class bg_img_dlg methods

}; // END namespace ns_bg_img_dlg

/*
///////////////////////////////////////////////////////////////////////////
// impl. -- REFRENCE, TEMPORARY
bg_image::bg_image( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 3, 3, 0, 15 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 1, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 16, 1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	bg_ing_label = new wxStaticText( this, wxID_ANY, _("Image"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	bg_ing_label->Wrap( -1 );
	bg_ing_label->SetToolTip( _("Image Preview") );
	bg_ing_label->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer1->Add( bg_ing_label, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	bmp_preview = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 192,192 ), 0 );
	bSizer1->Add( bmp_preview, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxString opt_saveChoices[] = { _("Copy Original"), _("Copy Changes"), _("No Copy") };
	int opt_saveNChoices = sizeof( opt_saveChoices ) / sizeof( wxString );
	opt_save = new wxRadioBox( this, wxID_ANY, _("Copy Options:"), wxDefaultPosition, wxDefaultSize, opt_saveNChoices, opt_saveChoices, 1, wxRA_SPECIFY_ROWS );
	opt_save->SetSelection( 1 );
	opt_save->SetToolTip( _("Option to copy image with the .pse file: copy the original without changes (changes will be lost), or copy with changes (changes will be available in future, even if original file is not), or make no copy (changes will be lost).") );
	
	bSizer1->Add( opt_save, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	chk_flhorz = new wxCheckBox( this, wxID_ANY, _("Flip Horizontal"), wxDefaultPosition, wxDefaultSize, 0 );
	chk_flhorz->SetToolTip( _("Flip image left to right.") );
	
	bSizer4->Add( chk_flhorz, 0, wxALL, 5 );
	
	chk_flvert = new wxCheckBox( this, wxID_ANY, _("Flip Vertical"), wxDefaultPosition, wxDefaultSize, 0 );
	chk_flvert->SetToolTip( _("Flip image top to bottom.") );
	
	bSizer4->Add( chk_flvert, 0, wxALL, 5 );
	
	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer4->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	chk_greyscale = new wxCheckBox( this, wxID_ANY, _("Greyscale"), wxDefaultPosition, wxDefaultSize, 0 );
	chk_greyscale->SetToolTip( _("Convert image to greyscale.") );
	
	bSizer13->Add( chk_greyscale, 0, wxALIGN_CENTER|wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer4->Add( bSizer13, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline4, 0, wxALL|wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->AddGrowableCol( 3 );
	gbSizer1->AddGrowableRow( 5 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxBoxSizer* sizer_dimsw;
	sizer_dimsw = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Width:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText3->Wrap( -1 );
	sizer_dimsw->Add( m_staticText3, 0, wxALL, 5 );
	
	spin_wi = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 32767, 0 );
	sizer_dimsw->Add( spin_wi, 0, wxALL, 5 );
	
	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizer_dimsw->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_dimsw, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* sizer_dimsh;
	sizer_dimsh = new wxBoxSizer( wxVERTICAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Height:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText4->Wrap( -1 );
	sizer_dimsh->Add( m_staticText4, 0, wxALL, 5 );
	
	spin_hi = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 32767, 0 );
	sizer_dimsh->Add( spin_hi, 0, wxALL, 5 );
	
	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizer_dimsh->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_dimsh, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_staticline18 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	gbSizer1->Add( m_staticline18, wxGBPosition( 0, 1 ), wxGBSpan( 2, 1 ), wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* sizer_offsx;
	sizer_offsx = new wxBoxSizer( wxVERTICAL );
	
	m_staticText31 = new wxStaticText( this, wxID_ANY, _("Horizontal Offset:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText31->Wrap( -1 );
	sizer_offsx->Add( m_staticText31, 0, wxALL, 5 );
	
	spin_offsx = new wxSpinCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32767, 32766, 0 );
	sizer_offsx->Add( spin_offsx, 0, wxALL, 5 );
	
	m_staticline51 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizer_offsx->Add( m_staticline51, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_offsx, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* sizer_offsx1;
	sizer_offsx1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText311 = new wxStaticText( this, wxID_ANY, _("Vertical Offset:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText311->Wrap( -1 );
	sizer_offsx1->Add( m_staticText311, 0, wxALL, 5 );
	
	spin_offsy = new wxSpinCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32767, 32766, 0 );
	sizer_offsx1->Add( spin_offsy, 0, wxALL, 5 );
	
	m_staticline511 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizer_offsx1->Add( m_staticline511, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_offsx1, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	selector_file = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select an image file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	gbSizer1->Add( selector_file, wxGBPosition( 2, 0 ), wxGBSpan( 1, 3 ), wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	m_staticline7 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizer1->Add( m_staticline7, wxGBPosition( 3, 0 ), wxGBSpan( 1, 3 ), wxEXPAND | wxALL, 5 );
	
	button_sizer1 = new wxStdDialogButtonSizer();
	button_sizer1OK = new wxButton( this, wxID_OK );
	button_sizer1->AddButton( button_sizer1OK );
	button_sizer1Apply = new wxButton( this, wxID_APPLY );
	button_sizer1->AddButton( button_sizer1Apply );
	button_sizer1Cancel = new wxButton( this, wxID_CANCEL );
	button_sizer1->AddButton( button_sizer1Cancel );
	button_sizer1Help = new wxButton( this, wxID_HELP );
	button_sizer1->AddButton( button_sizer1Help );
	button_sizer1->Realize();
	gbSizer1->Add( button_sizer1, wxGBPosition( 4, 0 ), wxGBSpan( 1, 3 ), wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer1->Add( gbSizer1, 1, wxEXPAND, 5 );
	
	fgSizer2->Add( bSizer1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer2->Add( 16, 1, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 1, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND, 5 );
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	fgSizer2->Fit( this );
	
	// Connect Events
	opt_save->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( bg_image::on_copy_opt ), NULL, this );
	chk_flhorz->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_horz ), NULL, this );
	chk_flvert->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_vert ), NULL, this );
	chk_greyscale->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_greyscale ), NULL, this );
	spin_wi->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_width ), NULL, this );
	spin_hi->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_height ), NULL, this );
	spin_offsx->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_x ), NULL, this );
	spin_offsy->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_y ), NULL, this );
	selector_file->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( bg_image::on_file select ), NULL, this );
	button_sizer1Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_apply ), NULL, this );
	button_sizer1Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_cancel ), NULL, this );
	button_sizer1Help->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_help ), NULL, this );
	button_sizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_ok ), NULL, this );
}

bg_image::~bg_image()
{
	// Disconnect Events
	opt_save->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( bg_image::on_copy_opt ), NULL, this );
	chk_flhorz->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_horz ), NULL, this );
	chk_flvert->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_vert ), NULL, this );
	chk_greyscale->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_greyscale ), NULL, this );
	spin_wi->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_width ), NULL, this );
	spin_hi->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_height ), NULL, this );
	spin_offsx->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_x ), NULL, this );
	spin_offsy->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_y ), NULL, this );
	selector_file->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( bg_image::on_file select ), NULL, this );
	button_sizer1Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_apply ), NULL, this );
	button_sizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_cancel ), NULL, this );
	button_sizer1Help->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_help ), NULL, this );
	button_sizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_ok ), NULL, this );
}
*/
