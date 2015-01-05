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
	
	bmp_preview = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 200,150 ), 0 );
	bSizer1->Add( bmp_preview, 0, wxALIGN_CENTER, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->AddGrowableCol( 4 );
	gbSizer2->AddGrowableRow( 4 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	gbSizer2->SetMinSize( wxSize( -1,30 ) ); 
	m_staticline13 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	gbSizer2->Add( m_staticline13, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxString opt_saveChoices[] = { _("Copy Original"), _("Copy Changes"), _("No Copy") };
	int opt_saveNChoices = sizeof( opt_saveChoices ) / sizeof( wxString );
	opt_save = new wxRadioBox( this, wxID_ANY, _("Copy Options:"), wxDefaultPosition, wxDefaultSize, opt_saveNChoices, opt_saveChoices, 1, wxRA_SPECIFY_ROWS );
	opt_save->SetSelection( 1 );
	opt_save->SetToolTip( _("Option to copy image with the saved .pse file:\ncopy the original without changes (modifications\nmay be reapplied automattically), or save a copy\nwith changes, or make no copy at all.") );
	
	gbSizer2->Add( opt_save, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxBOTTOM|wxTOP, 5 );
	
	chk_greyscale = new wxCheckBox( this, wxID_ANY, _("Greyscale"), wxDefaultPosition, wxDefaultSize, 0 );
	chk_greyscale->SetToolTip( _("Convert image to greyscale.") );
	
	gbSizer2->Add( chk_greyscale, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizer2->Add( m_staticline2, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_staticline181 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizer2->Add( m_staticline181, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	chk_flhorz = new wxCheckBox( this, wxID_ANY, _("Flip Horizontal"), wxDefaultPosition, wxDefaultSize, 0 );
	chk_flhorz->SetToolTip( _("Flip image left to right.") );
	
	bSizer15->Add( chk_flhorz, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	chk_flvert = new wxCheckBox( this, wxID_ANY, _("Flip Vertical"), wxDefaultPosition, wxDefaultSize, 0 );
	chk_flvert->SetToolTip( _("Flip image top to bottom.") );
	
	bSizer15->Add( chk_flvert, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	gbSizer2->Add( bSizer15, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxBOTTOM|wxTOP, 5 );
	
	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	gbSizer2->Add( m_staticline8, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("Rotate:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer111->Add( m_staticText8, 0, wxALL, 5 );
	
	spin_ro = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 75,-1 ), wxSP_ARROW_KEYS|wxSP_WRAP, -359, 359, 0 );
	spin_ro->SetToolTip( _("Rotate the image, in degrees") );
	
	bSizer111->Add( spin_ro, 0, wxALIGN_CENTER, 5 );
	
	gbSizer2->Add( bSizer111, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxBOTTOM|wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizer2->Add( m_staticline4, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxEXPAND | wxALL, 5 );
	
	m_staticline19 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizer2->Add( m_staticline19, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );
	
	bSizer1->Add( gbSizer2, 0, wxALL|wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->AddGrowableCol( 8 );
	gbSizer1->AddGrowableRow( 6 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxBoxSizer* sizer_dimsw;
	sizer_dimsw = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Width:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText3->Wrap( -1 );
	sizer_dimsw->Add( m_staticText3, 0, wxALL, 5 );
	
	spin_wi = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), wxSP_ARROW_KEYS, 0, 32767, 0 );
	spin_wi->SetToolTip( _("Set the width of display on background.\nZero (0) will use default width.") );
	
	sizer_dimsw->Add( spin_wi, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize( 120,-1 ), wxLI_HORIZONTAL );
	sizer_dimsw->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_dimsw, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* sizer_dimsh;
	sizer_dimsh = new wxBoxSizer( wxVERTICAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Height:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText4->Wrap( -1 );
	sizer_dimsh->Add( m_staticText4, 0, wxALL, 5 );
	
	spin_hi = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), wxSP_ARROW_KEYS, 0, 32767, 0 );
	spin_hi->SetToolTip( _("Set the height of display on background.\nZero (0) will use default height.") );
	
	sizer_dimsh->Add( spin_hi, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize( 120,-1 ), wxLI_HORIZONTAL );
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
	spin_offsx->SetMinSize( wxSize( 75,-1 ) );
	
	sizer_offsx->Add( spin_offsx, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	m_staticline51 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize( 120,-1 ), wxLI_HORIZONTAL );
	sizer_offsx->Add( m_staticline51, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_offsx, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* sizer_offsx1;
	sizer_offsx1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText311 = new wxStaticText( this, wxID_ANY, _("Vertical Offset:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText311->Wrap( -1 );
	sizer_offsx1->Add( m_staticText311, 0, wxALL, 5 );
	
	spin_offsy = new wxSpinCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 75,-1 ), wxSP_ARROW_KEYS, -32767, 32766, 0 );
	spin_offsy->SetToolTip( _("Set the vertical offset of display on background.") );
	
	sizer_offsx1->Add( spin_offsy, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	m_staticline511 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize( 120,-1 ), wxLI_HORIZONTAL );
	sizer_offsx1->Add( m_staticline511, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_offsx1, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_staticline11 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	gbSizer1->Add( m_staticline11, wxGBPosition( 0, 3 ), wxGBSpan( 2, 1 ), wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("HSV adjust:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer9->Add( m_staticText7, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	hsv_h = new wxSlider( this, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_INVERSE|wxSL_VERTICAL );
	hsv_h->SetToolTip( _("Adjust HSV (Hue Saturation Value) hue.") );
	
	bSizer11->Add( hsv_h, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	hsv_s = new wxSlider( this, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_INVERSE|wxSL_VERTICAL );
	hsv_s->SetToolTip( _("Adjust HSV (Hue Saturation Value) saturation.") );
	
	bSizer11->Add( hsv_s, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALIGN_TOP|wxEXPAND, 5 );
	
	hsv_v = new wxSlider( this, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_INVERSE|wxSL_VERTICAL );
	hsv_v->SetToolTip( _("Adjust HSV (Hue Saturation Value) value.") );
	
	bSizer11->Add( hsv_v, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	bSizer9->Add( bSizer11, 1, wxALIGN_CENTER|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticline12 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline12, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( bSizer9, wxGBPosition( 0, 4 ), wxGBSpan( 2, 1 ), wxALIGN_CENTER|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticline111 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	gbSizer1->Add( m_staticline111, wxGBPosition( 0, 5 ), wxGBSpan( 3, 1 ), wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, _("Lightness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer91->Add( m_staticText71, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	band_comp = new wxSlider( this, wxID_ANY, 0, -255, 255, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_VERTICAL );
	band_comp->SetToolTip( _("Lighten or darken image.") );
	
	bSizer12->Add( band_comp, 0, wxALIGN_CENTER|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer91->Add( bSizer12, 1, wxALIGN_CENTER|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticline121 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLI_HORIZONTAL );
	bSizer91->Add( m_staticline121, 0, wxALL|wxEXPAND, 5 );
	
	gbSizer1->Add( bSizer91, wxGBPosition( 0, 6 ), wxGBSpan( 2, 1 ), wxALIGN_CENTER|wxALIGN_TOP|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Choose an image file :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer8->Add( m_staticText9, 0, wxALL, 5 );
	
	selector_file = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Choose an image file"), wxT("(*.bmp;*.png;*.jpg;*.jpeg;*.jpe;*.tif;*.tiff;*.gif;*.pnm;*.ppm;*.pgm;*.pbm;*.pcx;*.iff;*.ico;*.cur;*.ani;*.tga;*.tpic;*.xpm)|*.bmp;*.png;*.jpg;*.jpeg;*.jpe;*.tif;*.tiff;*.gif;*.pnm;*.ppm;*.pgm;*.pbm;*.pcx;*.iff;*.ico;*.cur;*.ani;*.tga;*.tpic;*.xpm"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	selector_file->SetToolTip( _("Choose a background image file.") );
	
	bSizer8->Add( selector_file, 0, wxALIGN_CENTER|wxALL|wxBOTTOM|wxEXPAND|wxTOP, 5 );
	
	gbSizer1->Add( bSizer8, wxGBPosition( 3, 0 ), wxGBSpan( 1, 7 ), wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	m_staticline7 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizer1->Add( m_staticline7, wxGBPosition( 3, 0 ), wxGBSpan( 1, 7 ), wxEXPAND | wxALL, 5 );
	
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
	gbSizer1->Add( button_sizer1, wxGBPosition( 4, 0 ), wxGBSpan( 1, 7 ), wxALIGN_CENTER|wxALIGN_TOP|wxALL|wxEXPAND|wxTOP, 5 );
	
	bSizer1->Add( gbSizer1, 1, wxALL|wxEXPAND, 5 );
	
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
	chk_greyscale->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_greyscale ), NULL, this );
	chk_flhorz->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_horz ), NULL, this );
	chk_flvert->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_vert ), NULL, this );
	spin_ro->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_rotate ), NULL, this );
	spin_wi->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_width ), NULL, this );
	spin_hi->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_height ), NULL, this );
	spin_offsx->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_x ), NULL, this );
	spin_offsy->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_y ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
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
	chk_greyscale->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_greyscale ), NULL, this );
	chk_flhorz->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_horz ), NULL, this );
	chk_flvert->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( bg_image::on_flip_vert ), NULL, this );
	spin_ro->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_rotate ), NULL, this );
	spin_wi->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_width ), NULL, this );
	spin_hi->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_height ), NULL, this );
	spin_offsx->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_x ), NULL, this );
	spin_offsy->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( bg_image::on_offs_y ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_h->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_hsv_h_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_s->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_hsv_s_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	hsv_v->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_hsv_v_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	band_comp->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( bg_image::on_band_comp_scroll ), NULL, this );
	selector_file->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( bg_image::on_file_select ), NULL, this );
	button_sizer1Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_apply ), NULL, this );
	button_sizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_cancel ), NULL, this );
	button_sizer1Help->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_help ), NULL, this );
	button_sizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( bg_image::on_ok ), NULL, this );
}
