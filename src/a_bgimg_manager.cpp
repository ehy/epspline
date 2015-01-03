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
#include "wxutil.h"
// help topic IDs
#include "epspline_helpids.h"

#include "a_bgimg_manager.h"

// bg image manager methods
bgimg_manager::bgimg_manager()
	: dlg(0), img(0), mods_img(0)
	, cb_func(0), cb_arg(0)
	, mods_current(false)
{
	set_rotbg();
	set_default_params();
}

bgimg_manager::bgimg_manager(unsigned long p)
	: dlg(0), img(0), mods_img(0)
	, cb_func(0), cb_arg(0)
	, mods_current(false)
{
	set_rotbg();
	set_params(p);
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
	data_std.modswidth  =
		width == data_std.origwidth ? 0 : width;
	data_std.modsheight =
		height == data_std.origheight ? 0 : height;
	data_std.off_x = xo;
	data_std.off_y = yo;
}

void
bgimg_manager::get_dimensions(
	dim_type& width, dim_type& height, off_type& xo, off_type& yo)
{
	width  =
		data_std.modswidth ? data_std.modswidth : data_std.origwidth;
	height =
		data_std.modsheight ? data_std.modsheight : data_std.origheight;
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

	//if ( data_std == data_save ) {
	if ( is_current() ) {
		// no transforms pending,
		// give what we got
		// if we got it
		if ( mods_img ) {
			return mods_img;
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
			return 0;
		}
	}

	delete mods_img;
	mods_img = new wxImage(img->Copy());

	data_std.origwidth = img->GetWidth();
	data_std.origheight = img->GetHeight();

	if ( ! has_transform() ) {
		return mods_img;
	}

	if ( get_conv_hsvh() || get_conv_hsvs() || get_conv_hsvv() ) {
		mods_img = wximg_adjhsv(mods_img,
			int(data_std.hsv_h),
			int(data_std.hsv_s),
			int(data_std.hsv_v)
		);
	}
	if ( get_conv_band_comp() ) {
		int v = data_std.band_comp;
		bool lighten = (v > 0);
		double band = 1.0 - double(std::abs(v)) / 255.0;
		mods_img = wximg_bandcomp(mods_img, band, lighten);
	}
	if ( get_conv_grey() ) {
		wxImage* t = mods_img;
		mods_img = wximg_get_greyscale(t);
		delete t;
	}

	if ( get_flip_horz() ) {
		wxImage* t = new wxImage(mods_img->Mirror(true));
		delete mods_img;
		mods_img = t;
	}
	if ( get_flip_vert() ) {
		wxImage* t = new wxImage(mods_img->Mirror(false));
		delete mods_img;
		mods_img = t;
	}
	if ( get_arg_rotate() ) {
		mods_img = wximg_rotate(
			mods_img,
			data_std.degrotate,
			true, rotbg_r, rotbg_g, rotbg_b
		);
	}

	if ( get_arg_rotate() || get_arg_width() || get_arg_height() ) {
		// TODO: make scale type an option
		const
		#if wxCHECK_VERSION(2, 9, 0)
		wxImageResizeQuality
		#else
		int
		#endif
		//qual = wxIMAGE_QUALITY_NORMAL;
		qual = wxIMAGE_QUALITY_HIGH;

		int w = static_cast<int>(
			data_std.modswidth ?
			data_std.modswidth :
			data_std.origwidth);
		int h = static_cast<int>(
			data_std.modsheight ?
			data_std.modsheight :
			data_std.origheight);

		if ( w != mods_img->GetWidth() || h != mods_img->GetHeight() ) {
			mods_img->Rescale(w, h, qual);
		}
	}

	mods_current = true;
	return mods_img;
}

void
bgimg_manager::remove_image(bool update)
{
	hide_dialog();
	data_save = data_std;
	data_std = datastruct();
	
	delete mods_img;
	mods_img = 0;
	delete img;
	img = 0;

	if ( update ) {
		force_updates();
	}
}

void
bgimg_manager::force_updates()
{
	if ( cb_func != 0 ) {
		cb_func(cb_arg);
	}
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
	dat.hsv_h = p->hsv_h->GetValue();
	dat.hsv_s = p->hsv_s->GetValue();
	dat.hsv_v = p->hsv_v->GetValue();
	dat.img_fname = p->selector_file->GetPath();
}

void
bgimg_manager::update__to__dialog(const datastruct& dat)
{
	
	dialog_type* p = get_dlg();
	if ( !p ) {
		return;
	}

	p->chk_flhorz->SetValue(get_flip_horz());
	p->chk_flvert->SetValue(get_flip_vert());
	p->chk_greyscale->SetValue(get_conv_grey());
	p->spin_wi->SetValue(dat.modswidth);
	p->spin_hi->SetValue(dat.modsheight);
	p->spin_offsx->SetValue(dat.off_x);
	p->spin_offsy->SetValue(dat.off_y);
	p->hsv_h->SetValue(dat.hsv_h);
	p->hsv_s->SetValue(dat.hsv_s);
	p->hsv_v->SetValue(dat.hsv_v);
	p->selector_file->SetPath(dat.img_fname);

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
	mods_current = false;

	if ( dialog_type* p = get_dlg() ) {
		p->set_preview(get_mod_image());
	}

	force_updates();
}

void
bgimg_manager::on_cancel(wxCommandEvent& event)
{
	data_std = data_save;

	delete mods_img;
	mods_img = 0;
	delete img;
	img = 0;

	mods_current = false;

	force_updates();
}

void
bgimg_manager::on_OK(wxCommandEvent& event)
{
	delete mods_img;
	mods_img = 0;
	delete img;
	img = 0;

	update_from_dialog(data_std);
	mods_current = false;

	force_updates();
}

void
bgimg_manager::on_help(wxCommandEvent& event)
{
	wxGetApp().ShowHelp(IDI_BackgroundImage);
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

	if ( hsv_h->GetValue() || hsv_s->GetValue() || hsv_v->GetValue() ) {
		wximg_adjhsv(
			&i,
			hsv_h->GetValue(),
			hsv_s->GetValue(),
			hsv_v->GetValue()
		);
	}
	if ( int v = band_comp->GetValue() ) {
		bool lighten = (v > 0);
		double band = 1.0 - double(std::abs(v)) / 255.0;
		wximg_bandcomp(&i, band, lighten);
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
	if ( int v = spin_ro->GetValue() ) {
		wximg_rotate(&i, v, true,
			mng->rotbg_r, mng->rotbg_g, mng->rotbg_b);
	}
	if ( spin_wi->GetValue() > 0 || spin_hi->GetValue() > 0 ) {
		i.Rescale(
		  spin_wi->GetValue() > 0 ? spin_wi->GetValue() : i.GetWidth(),
		  spin_hi->GetValue() > 0 ? spin_hi->GetValue() : i.GetHeight()
		);
	}
	// next block moved to set_preview() because
	// app does not add offset to image
	//if ( spin_offsx->GetValue() || spin_offsy->GetValue() ) {

	set_preview(&i);
}

void
bg_img_dlg::set_preview(wxImage* i)
{
	wxSize sz = bmp_preview->GetClientSize();

	if ( i && i->GetHeight() > 0 ) {
		wxImage itmp(i->Copy());
		i = &itmp;

		// this block moved to here because
		// app does not add offset to image
		if ( spin_offsx->GetValue() || spin_offsy->GetValue() ) {
			i->Resize(
				wxSize(i->GetWidth(), i->GetHeight()),
				wxPoint(spin_offsx->GetValue(), spin_offsy->GetValue()),
				mng->rotbg_r, mng->rotbg_g, mng->rotbg_b
			);
		}

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
				).Size(sz, off,
					mng->rotbg_r, mng->rotbg_g, mng->rotbg_b
				)
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
	switch ( opt_save->GetSelection() ) {
		case 0:
			mng->set_copy_orig(true);
			break;
		case 1:
			mng->set_copy_changes(true);
			break;
		case 2:
		default:
			mng->set_copy_none(true);
			break;
	}
}

void
bg_img_dlg::on_flip_horz(wxCommandEvent& event)
{
	mng->set_flip_horz(chk_flhorz->GetValue());

	put_preview();
}

void
bg_img_dlg::on_flip_vert(wxCommandEvent& event)
{
	mng->set_flip_vert(chk_flvert->GetValue());

	put_preview();
}

void
bg_img_dlg::on_greyscale(wxCommandEvent& event)
{
	mng->set_conv_grey(chk_greyscale->GetValue());

	put_preview();
}

void
bg_img_dlg::on_rotate(wxCommandEvent& event)
{
	int i = spin_ro->GetValue();
	mng->data_std.degrotate = (bgimg_manager::dim_type)i;
	mng->set_arg_rotate(i != 0);

	put_preview();
}

void
bg_img_dlg::on_width(wxCommandEvent& event)
{
	int i = spin_wi->GetValue();
	mng->data_std.modswidth = (bgimg_manager::dim_type)i;
	mng->set_arg_width(i != 0);

	put_preview();
}

void
bg_img_dlg::on_height(wxCommandEvent& event)
{
	int i = spin_hi->GetValue();
	mng->data_std.modsheight = (bgimg_manager::dim_type)i;
	mng->set_arg_height(i != 0);

	put_preview();
}

void
bg_img_dlg::on_offs_x(wxCommandEvent& event)
{
	int i = spin_offsx->GetValue();
	mng->data_std.off_x = (bgimg_manager::off_type)i;
	mng->set_arg_offsx(i != 0);

	put_preview();
}

void
bg_img_dlg::on_offs_y(wxCommandEvent& event)
{
	int i = spin_offsy->GetValue();
	mng->data_std.off_y = (bgimg_manager::off_type)i;
	mng->set_arg_offsy(i != 0);

	put_preview();
}

void
bg_img_dlg::on_hsv_h_scroll(wxScrollEvent& event)
{
	int i = hsv_h->GetValue();
	mng->data_std.hsv_h = (bgimg_manager::hsv_type)i;
	mng->set_conv_hsvh(i != 0);

	put_preview();
}

void
bg_img_dlg::on_hsv_s_scroll(wxScrollEvent& event)
{
	int i = hsv_s->GetValue();
	mng->data_std.hsv_s = (bgimg_manager::hsv_type)i;
	mng->set_conv_hsvs(i != 0);

	put_preview();
}

void
bg_img_dlg::on_hsv_v_scroll(wxScrollEvent& event)
{
	int i = hsv_v->GetValue();
	mng->data_std.hsv_v = (bgimg_manager::hsv_type)i;
	mng->set_conv_hsvv(i != 0);

	put_preview();
}

void
bg_img_dlg::on_band_comp_scroll(wxScrollEvent& event)
{
	int i = band_comp->GetValue();
	mng->data_std.band_comp = (bgimg_manager::cmp_type)i;
	mng->set_conv_band_comp(i != 0);

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
