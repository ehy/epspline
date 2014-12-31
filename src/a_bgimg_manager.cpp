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

// simple conversion to greyscale -- may be replaced
// with something, time permitting
static wxImage*
get_grey_wximg(wxImage* src, bool use_alt = false);

// bg image manager methods
bgimg_manager::bgimg_manager()
	: dlg(0), img(0), mods_img(0)
{
	set_default_params();
	//parmscancel = parms;
}

bgimg_manager::bgimg_manager(unsigned long p)
	: dlg(0), img(0), mods_img(0)
{
	set_params(p);
	//parmscancel = parms;
}

bgimg_manager::~bgimg_manager()
{
	delete dlg;
	delete img;
	delete mods_img;
}

bgimg_manager::dialog_type*
bgimg_manager::get_dlg() {
	if ( dlg == 0 ) {
		dlg = new dialog_type(this);
	}
	return dlg;
}

void
bgimg_manager::set_dimensions(
	dim_type width, dim_type height, off_type xo, off_type yo)
{
	data_std.modswidth  = width;
	data_std.modsheight = height;
	data_std.off_x = xo;
	data_std.off_y = yo;
}

void
bgimg_manager::get_dimensions(
	dim_type& width, dim_type& height, off_type& xo, off_type& yo)
{
	width  = data_std.modswidth;
	height = data_std.modsheight;
	xo = data_std.off_x;
	yo = data_std.off_y;
}

void
bgimg_manager::get_dimensions_orig(dim_type& width, dim_type& height)
{
	width  = data_std.origwidth;
	height = data_std.origheight;
}

void
bgimg_manager::show_dialog(bool show)
{
	dialog_type* p = get_dlg();

	if ( p == 0 ) {
		return;
	}

	if ( ! p->IsShown() && show ) {
		data_save = data_std;
		update__to__dialog(data_std);
	}
	
	if ( p->IsIconized() && show ) {
		p->Iconize(false);
	}
	
	if ( p->IsMaximized() && show ) {
		p->Maximize(false);
	}
	
	if ( p->IsShown() && show ) {
		p->Raise();
	}
	
	p->Show(show);
}

wxImage*
bgimg_manager::get_mod_image()
{
	// no image
	if ( img_fname().IsEmpty() ) {
		delete mods_img;
		mods_img = 0;
		delete img;
		img = 0;
		return 0;
	}

	if ( data_std == data_save ) {
		// no transforms pending, give what we got
		if ( mods_img || img ) {
			return mods_img ? mods_img : img;
		}
	}

	// transforms are pending

	// orig not present
	if ( img == 0 ) {
		// have file name but image not created
		// yet; or, error
		img = new wxImage(img_fname());
		if ( img == 0 || ! img->IsOk() ) {
			// not so good . . .
			delete img;
			img = 0;
			return mods_img ? mods_img : 0;
		}
	}

	delete mods_img;
	mods_img = 0;

	data_std.origwidth = img->GetWidth();
	data_std.origheight = img->GetHeight();

	if ( ! has_transform() ) {
		return img;
	}

	if ( get_conv_grey() ) {
		mods_img = get_grey_wximg(img);
	}

	if ( get_flip_horz() ) {
		wxImage* t = new wxImage(
			(mods_img ? mods_img : img)->Mirror(true)
		);
		delete mods_img;
		mods_img = t;
	}
	if ( get_flip_vert() ) {
		wxImage* t = new wxImage(
			(mods_img ? mods_img : img)->Mirror(false)
		);
		delete mods_img;
		mods_img = t;
	}

	if ( get_arg_width() || get_arg_height() ) {
		// TODO: make scale type an option
		const
		#if wxCHECK_VERSION(2, 9, 0)
		wxImageResizeQuality
		#else
		int
		#endif
		//qual = wxIMAGE_QUALITY_NORMAL;
		qual = wxIMAGE_QUALITY_HIGH;
		int ow = static_cast<int>(data_std.origwidth);
		int oh = static_cast<int>(data_std.origheight);
		int w = static_cast<int>(
			data_std.modswidth ?
			data_std.modswidth :
			data_std.origwidth);
		int h = static_cast<int>(
			data_std.modsheight ?
			data_std.modsheight :
			data_std.origheight);

		if ( w != ow || h != oh ) {
			if ( mods_img ) {
				mods_img->Rescale(w, h, qual);
			} else {
				mods_img = new wxImage(img->Scale(w, h, qual));
			}
		}
	}

	return mods_img;
}

void
bgimg_manager::update_from_dialog(datastruct& dat)
{
	dialog_type* p = get_dlg();
	if ( !p ) {
		return;
	}

	set_flip_horz(p->chk_flhorz->GetValue());
	set_flip_vert(p->chk_flvert->GetValue());
	set_conv_grey(p->chk_greyscale->GetValue());
	dat.modswidth = p->spin_wi->GetValue();
	dat.modsheight = p->spin_hi->GetValue();
	dat.off_x = p->spin_offsx->GetValue();
	dat.off_y = p->spin_offsy->GetValue();
	dat.img_fname = p->selector_file->GetPath();
}

void
bgimg_manager::update__to__dialog(const datastruct& dat)
{
	
	dialog_type* p = get_dlg();
	if ( !p ) {
		return;
	}

	/* wxCheckBox* */ p->chk_flhorz->SetValue(get_flip_horz());
	/* wxCheckBox* */ p->chk_flvert->SetValue(get_flip_vert());
	/* wxCheckBox* */ p->chk_greyscale->SetValue(get_conv_grey());
	/* wxSpinCtrl* */ p->spin_wi->SetValue(dat.modswidth);
	/* wxSpinCtrl* */ p->spin_hi->SetValue(dat.modsheight);
	/* wxSpinCtrl* */ p->spin_offsx->SetValue(dat.off_x);
	/* wxSpinCtrl* */ p->spin_offsy->SetValue(dat.off_y);
	/* wxFilePickerCtrl* */ p->selector_file->SetPath(dat.img_fname);

	p->set_preview(get_mod_image());
}

// The dialog's virtual event handlers call these 'on_*' procs.
void
bgimg_manager::on_close_event(wxCloseEvent& event)
{
	if ( event.CanVeto() == false ) {
		dialog_type* t = dlg;
		dlg = 0;
		delete t;
	} else {
		//update__to__dialog(data_std);
		event.Skip();
	}
}

void
bgimg_manager::on_init_dlg(wxInitDialogEvent& event)
{
}

void
bgimg_manager::on_restore_defs(wxCommandEvent& event)
{
	update__to__dialog(data_std);
}

void
bgimg_manager::on_restore_conf(wxCommandEvent& event)
{
	update__to__dialog(data_std);
}

void
bgimg_manager::on_apply(wxCommandEvent& event)
{
	update_from_dialog(data_std);

	if ( dialog_type* p = get_dlg() ) {
		p->set_preview(get_mod_image());
	}

	force_updates();
}

void
bgimg_manager::on_cancel(wxCommandEvent& event)
{
	data_std = data_save;
	update__to__dialog(data_std);
	force_updates();
}

void
bgimg_manager::on_OK(wxCommandEvent& event)
{
	get_mod_image();
	data_save = data_std;
	on_apply(event);
}

void
bgimg_manager::on_help(wxCommandEvent& event)
{
	wxGetApp().ShowHelp(IDI_BackgroundImage);
}

// simple conversion to greyscale -- may be replaced
// with something, time permitting
static wxImage*
get_grey_wximg(wxImage* src, bool use_alt)
{
	const double alt_r = 0.2126;
	const double alt_b = 0.0722;
	const double alt_g = 1.0 - alt_b - alt_r;
	
	wxImage* dest;

	if ( use_alt ) {
		dest = new wxImage(src->ConvertToGreyscale(
			alt_r, alt_g, alt_b
		));
	} else {
		dest = new wxImage(src->ConvertToGreyscale());
	}

	return dest;
}

// The generated dialog base is in namespace "ns_bg_img_dlg",
// and derive with that ns.
namespace ns_bg_img_dlg {
bg_img_dlg::bg_img_dlg(
	bgimg_manager* manager,
	wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style)
	: bg_image(manager->parent_wnd(), id, title, pos, size, style)
	, mng(manager)
	, suffixes(wxImage::GetImageExtWildcard())
{
}

bg_img_dlg::~bg_img_dlg()
{
}

void
bg_img_dlg::put_preview()
{
	wxString s = selector_file->GetPath();

	if ( s.IsEmpty() ) {
		set_preview(0);
		return;
	}

	wxImage i(s);

	if ( ! i.IsOk() ) {
		set_preview(0);
		return;
	}

	if ( chk_greyscale->GetValue() ) {
		i = i.ConvertToGreyscale();
	}
	if ( chk_flhorz->GetValue() ) {
		i = i.Mirror(true);
	}
	if ( chk_flvert->GetValue() ) {
		i = i.Mirror(false);
	}
	if ( spin_wi->GetValue() > 0 || spin_hi->GetValue() > 0 ) {
		i.Rescale(
		  spin_wi->GetValue() > 0 ? spin_wi->GetValue() : i.GetWidth(),
		  spin_hi->GetValue() > 0 ? spin_hi->GetValue() : i.GetHeight()
		);
	}
	if ( spin_offsx->GetValue() || spin_offsy->GetValue() ) {
		i.Resize(
			wxSize(i.GetWidth(), i.GetHeight()),
			wxPoint(spin_offsx->GetValue(), spin_offsy->GetValue()),
			0, 0, 0
		);
	}

	set_preview(&i);
}

void
bg_img_dlg::set_preview(wxImage* i)
{
	wxSize sz = bmp_preview->GetClientSize();

	if ( i && i->GetHeight() > 0 ) {
		int x, y;
		float rn = float(sz.GetWidth())  / float(sz.GetHeight());
		float ro = float(i->GetWidth()) / float(i->GetHeight());

		if ( ro < rn ) {
			x = int(float(sz.GetHeight()) * ro + 0.5);
			y = sz.GetHeight();
		} else {
			x = sz.GetWidth();
			y = int(float(sz.GetWidth()) / ro + 0.5);
		}

		wxPoint off(
			(sz.GetWidth() - x) / 2, (sz.GetHeight() - y) / 2
		);

		bmp_preview->SetBitmap(
			wxBitmap(
				i->Scale(
					x, y, wxIMAGE_QUALITY_NORMAL
				).Size(sz, off, 0, 0, 0)
			)
		);
	} else {
		bmp_preview->SetBitmap(
			wxBitmap(
				wxImage(
					sz.GetWidth(), sz.GetHeight(), true
				)
			)
		);
	}
}

void
bg_img_dlg::on_copy_opt(wxCommandEvent& event)
{
}

void
bg_img_dlg::on_flip_horz(wxCommandEvent& event)
{
	mng->set_flip_horz(chk_flhorz->GetValue() ? true : false);

	put_preview();
}

void
bg_img_dlg::on_flip_vert(wxCommandEvent& event)
{
	mng->set_flip_vert(chk_flvert->GetValue() ? true : false);

	put_preview();
}

void
bg_img_dlg::on_greyscale(wxCommandEvent& event)
{
	mng->set_conv_grey(chk_greyscale->GetValue());

	put_preview();
}

void
bg_img_dlg::on_width(wxCommandEvent& event)
{
	mng->data_std.modswidth =
		(bgimg_manager::dim_type)spin_wi->GetValue();

	put_preview();
}

void
bg_img_dlg::on_height(wxCommandEvent& event)
{
	mng->data_std.modsheight =
		(bgimg_manager::dim_type)spin_hi->GetValue();

	put_preview();
}

void
bg_img_dlg::on_offs_x(wxCommandEvent& event)
{
	mng->data_std.off_x =
		(bgimg_manager::off_type)spin_offsx->GetValue();

	put_preview();
}

void
bg_img_dlg::on_offs_y(wxCommandEvent& event)
{
	mng->data_std.off_y =
		(bgimg_manager::off_type)spin_offsy->GetValue();

	put_preview();
}

void
bg_img_dlg::on_file_select(wxFileDirPickerEvent& event)
{
	mng->data_std.img_fname.Empty();
	mng->get_mod_image();
	mng->data_std.img_fname = selector_file->GetPath();

	put_preview();
}

void
bg_img_dlg::on_close_event(wxCloseEvent& event)
{
	mng->on_close_event(event);
	// let manager decide on Skip()
	//event.Skip();
}

void
bg_img_dlg::on_init_dlg(wxInitDialogEvent& event)
{
	mng->on_init_dlg(event);
	event.Skip();
}

void
bg_img_dlg::on_apply(wxCommandEvent& event)
{
		mng->on_apply(event);
		event.Skip();
}

void
bg_img_dlg::on_cancel(wxCommandEvent& event)
{
		mng->on_cancel(event);
		event.Skip();
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
		mng->on_help(event);
		event.Skip();
	}
}

void
bg_img_dlg::on_ok(wxCommandEvent& event)
{
		mng->on_OK(event);
		event.Skip();
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
