///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "spline_props.h"

///////////////////////////////////////////////////////////////////////////

spline_properties::spline_properties( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 3, 3, 0, 15 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 1, 16, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND|wxALIGN_RIGHT, 5 );
	
	
	fgSizer2->Add( 16, 1, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	spline_type_label = new wxStaticText( this, wxID_ANY, _("Bezier Spline"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	spline_type_label->Wrap( -1 );
	spline_type_label->SetToolTip( _("Spline type: cannot be changed") );
	spline_type_label->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer1->Add( spline_type_label, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, _("Object Name:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText5->Wrap( -1 );
	bSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	txt_objname = new wxTextCtrl( this, wxID_ANY, _("SplineObject"), wxDefaultPosition, wxDefaultSize, 0 );
	txt_objname->SetValidator( wxTextValidator( wxFILTER_INCLUDE_CHAR_LIST, &txt_objname_validator ) );
	txt_objname->SetToolTip( _("Name of object (#declare . . .)") );
	txt_objname->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer1->Add( txt_objname, 0, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxString pov_typeChoices[] = { _("Lathe"), _("Prism"), _("Undefined") };
	int pov_typeNChoices = sizeof( pov_typeChoices ) / sizeof( wxString );
	pov_type = new wxRadioBox( this, wxID_ANY, _("Render Type:"), wxDefaultPosition, wxDefaultSize, pov_typeNChoices, pov_typeChoices, 1, wxRA_SPECIFY_ROWS );
	pov_type->SetSelection( 2 );
	pov_type->SetToolTip( _("Lathe: rotates about axis\nPrism: extruded over 'Sweep'\nUndefined: intended for hand-editing") );
	
	bSizer1->Add( pov_type, 0, wxALL, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	wxString sweep_typeChoices[] = { _("Linear"), _("Conic") };
	int sweep_typeNChoices = sizeof( sweep_typeChoices ) / sizeof( wxString );
	sweep_type = new wxRadioBox( this, wxID_ANY, _("Sweep Type:"), wxDefaultPosition, wxDefaultSize, sweep_typeNChoices, sweep_typeChoices, 1, wxRA_SPECIFY_ROWS );
	sweep_type->SetSelection( 1 );
	sweep_type->SetToolTip( _("Sweep will taper if 'conic', or be uniform if 'linear' (only applies to prism type)") );
	
	bSizer4->Add( sweep_type, 0, wxALL, 5 );
	
	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer4->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );
	
	check_sturm = new wxCheckBox( this, wxID_ANY, _("Use 'sturm'"), wxDefaultPosition, wxDefaultSize, 0 );
	check_sturm->SetToolTip( _("Add the 'sturm' modifier") );
	
	bSizer4->Add( check_sturm, 0, wxALIGN_CENTER|wxALL, 5 );
	
	check_open = new wxCheckBox( this, wxID_ANY, _("Use 'open'"), wxDefaultPosition, wxDefaultSize, 0 );
	check_open->SetValue(true); 
	check_open->SetToolTip( _("Add the 'open' modifier (for prism only)") );
	
	bSizer4->Add( check_open, 0, wxALIGN_CENTER|wxALL, 5 );
	
	bSizer1->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline4, 0, wxALL|wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->AddGrowableCol( 2 );
	gbSizer1->AddGrowableRow( 6 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* sizer_sweep_sub1;
	sizer_sweep_sub1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Start Sweep:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText3->Wrap( -1 );
	sizer_sweep_sub1->Add( m_staticText3, 0, wxALL, 5 );
	
	txt_sweep_min = new wxTextCtrl( this, wxID_ANY, _("-1"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	txt_sweep_min->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &txt_sweep_min_validator ) );
	txt_sweep_min->SetToolTip( _("Start sweep here (float)") );
	
	sizer_sweep_sub1->Add( txt_sweep_min, 0, wxALL, 5 );
	
	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizer_sweep_sub1->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( sizer_sweep_sub1, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* size_sweep_sub2;
	size_sweep_sub2 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("End Sweep:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticText4->Wrap( -1 );
	size_sweep_sub2->Add( m_staticText4, 0, wxALL, 5 );
	
	txt_sweep_max = new wxTextCtrl( this, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	txt_sweep_max->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &txt_sweep_max_validator ) );
	txt_sweep_max->SetToolTip( _("End sweep here (float)") );
	
	size_sweep_sub2->Add( txt_sweep_max, 0, wxALL, 5 );
	
	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	size_sweep_sub2->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );
	
	gbSizer1->Add( size_sweep_sub2, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("Texture:"), wxPoint( -1,-1 ), wxDefaultSize, wxALIGN_LEFT );
	m_staticText6->Wrap( -1 );
	gbSizer1->Add( m_staticText6, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	txt_texture = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( -1,-1 ), wxDefaultSize, 0 );
	txt_texture->SetValidator( wxTextValidator( wxFILTER_INCLUDE_CHAR_LIST, &txt_texture_validator ) );
	txt_texture->SetToolTip( _("Optional pre-defined texture identifier; may be left empty") );
	txt_texture->SetMinSize( wxSize( 200,-1 ) );
	
	gbSizer1->Add( txt_texture, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	Interior_label = new wxStaticText( this, wxID_ANY, _("Interior:"), wxPoint( -1,-1 ), wxDefaultSize, wxALIGN_LEFT );
	Interior_label->Wrap( -1 );
	gbSizer1->Add( Interior_label, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	txt_interior = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( -1,-1 ), wxDefaultSize, 0 );
	txt_interior->SetValidator( wxTextValidator( wxFILTER_INCLUDE_CHAR_LIST, &txt_interior_validator ) );
	txt_interior->SetToolTip( _("Optional pre-defined interior identifier; may be left empty") );
	txt_interior->SetMinSize( wxSize( 200,-1 ) );
	
	gbSizer1->Add( txt_interior, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("Transform:"), wxPoint( -1,-1 ), wxDefaultSize, wxALIGN_LEFT );
	m_staticText8->Wrap( -1 );
	gbSizer1->Add( m_staticText8, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	txt_transform = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( -1,-1 ), wxDefaultSize, 0 );
	txt_transform->SetValidator( wxTextValidator( wxFILTER_INCLUDE_CHAR_LIST, &txt_transform_validator ) );
	txt_transform->SetToolTip( _("Optional pre-defined transform identifier; may be left empty") );
	txt_transform->SetMinSize( wxSize( 200,-1 ) );
	
	gbSizer1->Add( txt_transform, wxGBPosition( 6, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Predefined Identifiers:"), wxPoint( -1,-1 ), wxDefaultSize, wxALIGN_LEFT );
	m_staticText9->Wrap( -1 );
	gbSizer1->Add( m_staticText9, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	txt_predefined = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( -1,-1 ), wxDefaultSize, wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE );
	txt_predefined->SetValidator( wxTextValidator( wxFILTER_ASCII, &txt_predefined_validator ) );
	txt_predefined->SetToolTip( _("Optionally enter arbitrary SDL here; but it must make sense within the object definition, e.g. macros, pigments, etc.\nMay be left empty") );
	txt_predefined->SetMinSize( wxSize( 200,120 ) );
	
	gbSizer1->Add( txt_predefined, wxGBPosition( 2, 1 ), wxGBSpan( 6, 1 ), wxALL, 5 );
	
	m_staticline7 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	gbSizer1->Add( m_staticline7, wxGBPosition( 9, 0 ), wxGBSpan( 1, 2 ), wxEXPAND | wxALL, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	gbSizer1->Add( m_sdbSizer1, wxGBPosition( 10, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	bSizer1->Add( gbSizer1, 1, wxEXPAND, 5 );
	
	fgSizer2->Add( bSizer1, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 16, 1, 1, wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND|wxALIGN_BOTTOM, 5 );
	
	
	fgSizer2->Add( 1, 16, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_BOTTOM, 5 );
	
	
	fgSizer2->Add( 16, 16, 1, wxEXPAND|wxALIGN_RIGHT|wxALIGN_BOTTOM, 5 );
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	fgSizer2->Fit( this );
}

spline_properties::~spline_properties()
{
}
