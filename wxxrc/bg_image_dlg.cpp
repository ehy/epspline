///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "bg_image_dlg.h"

///////////////////////////////////////////////////////////////////////////
using namespace ns_bg_img_dlg;

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
	
	bg_img_label = new wxStaticText( this, wxID_ANY, _("Image Preview"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	bg_img_label->Wrap( -1 );
	bg_img_label->SetToolTip( _("Image Preview") );
	bg_img_label->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer1->Add( bg_img_label, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	bmp_preview = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 192,128 ), 0 );
	bSizer1->Add( bmp_preview, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxString opt_saveChoices[] = { _("Copy Original"), _("Copy Changes"), _("No Copy") };
	int opt_saveNChoices = sizeof( opt_saveChoices ) / sizeof( wxString );
	opt_save = new wxRadioBox( this, wxID_ANY, _("Copy Options:"), wxDefaultPosition, wxDefaultSize, opt_saveNChoices, opt_saveChoices, 1, wxRA_SPECIFY_ROWS );
	opt_save->SetSelection( 0 );
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
	spin_wi->SetToolTip( _("Set the width of display on background.") );
	
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
	spin_hi->SetToolTip( _("Set the height of display on background.") );
	
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
	spin_offsx->SetToolTip( _("Set the horizontal offset of display on background.") );
	
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
	spin_offsy->SetToolTip( _("Set the vertical offset of display on background.") );
	
	sizer_offsx1->Add( spin_offsy, 0, wxALL, 5 );
	
	m_staticline511 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizer_offsx1->Add( m_staticline511, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_offsx1, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("Choose Image File:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer8->Add( m_staticText6, 0, wxALL, 5 );
	
	selector_file = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Choose an image file"), wxT("(*.bmp;*.png;*.jpg;*.jpeg;*.jpe;*.tif;*.tiff;*.gif;*.pnm;*.ppm;*.pgm;*.pbm;*.pcx;*.iff;*.ico;*.cur;*.ani;*.tga;*.tpic;*.xpm)|*.bmp;*.png;*.jpg;*.jpeg;*.jpe;*.tif;*.tiff;*.gif;*.pnm;*.ppm;*.pgm;*.pbm;*.pcx;*.iff;*.ico;*.cur;*.ani;*.tga;*.tpic;*.xpm"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	selector_file->SetToolTip( _("Choose a background image file.") );
	
	bSizer8->Add( selector_file, 0, wxALL|wxEXPAND, 5 );
	
	gbSizer1->Add( bSizer8, wxGBPosition( 2, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
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
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( bg_image::on_close_event ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( bg_image::on_init_dlg ) );
	opt_save->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( bg_image::on_copy_opt ), NULL, this );
	chk_flhorz->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_horz ), NULL, this );
	chk_flvert->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_vert ), NULL, this );
	chk_greyscale->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_greyscale ), NULL, this );
	spin_wi->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_width ), NULL, this );
	spin_hi->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_height ), NULL, this );
	spin_offsx->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_x ), NULL, this );
	spin_offsy->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_y ), NULL, this );
	selector_file->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( bg_image::on_file_select ), NULL, this );
	button_sizer1Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_apply ), NULL, this );
	button_sizer1Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_cancel ), NULL, this );
	button_sizer1Help->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_help ), NULL, this );
	button_sizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_ok ), NULL, this );
}

bg_image::~bg_image()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( bg_image::on_close_event ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( bg_image::on_init_dlg ) );
	opt_save->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( bg_image::on_copy_opt ), NULL, this );
	chk_flhorz->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_horz ), NULL, this );
	chk_flvert->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_vert ), NULL, this );
	chk_greyscale->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_greyscale ), NULL, this );
	spin_wi->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_width ), NULL, this );
	spin_hi->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_height ), NULL, this );
	spin_offsx->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_x ), NULL, this );
	spin_offsy->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_y ), NULL, this );
	selector_file->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( bg_image::on_file_select ), NULL, this );
	button_sizer1Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_apply ), NULL, this );
	button_sizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_cancel ), NULL, this );
	button_sizer1Help->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_help ), NULL, this );
	button_sizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_ok ), NULL, this );
}
