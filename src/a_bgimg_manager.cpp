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

#include <cstdlib>
#include <cstring>
#include <cerrno>
#include "cfg.h"
#include "stdcc.h"
#include "epspline.h"
#include "a_frame.h"
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

// display error message; e.g., image file open failure
void
bgimg_manager::error_msg(wxString msg, wxString addl)
{
	// main window handles the error dialog
	if ( A_Frame* p = dynamic_cast<A_Frame*>(parent_wnd()) ) {
		p->ErrorBox(msg, addl);
	} else {
		std::fprintf(stderr, "%s: %s\n", wxs2ch(addl), wxs2ch(msg));
	}
}

bgimg_manager::dialog_type*
bgimg_manager::get_dlg() {
	if ( dlg == 0 ) {
		dlg = new dialog_type(this);
	}
	return dlg;
}

wxWindow*
bgimg_manager::parent_wnd()
{
	return wxGetApp().GetTopWindow();
}

void
bgimg_manager::set_default_params()
{
	clear_params();
	data_std.zero();
	set_copy_orig();
	update__to__dialog(data_std);
}

void
bgimg_manager::reset_default_params()
{
	// save offsets: they are for using code, and are
	// not applied to image; therefore they are not cumulatively
	// reapplied to image, and should not be lost
	off_type off_x, off_y;
	off_x = data_std.off_x;
	off_y = data_std.off_y;
	// save copy params too; should not be lost here
	params copopt = params(parms() & copy_bitsmask);
	clear_params();
	data_std.zero();
	set_copy_orig();
	// restore saved items
	data_std.off_x = off_x;
	data_std.off_y = off_y;
	parms() = params((parms() & ~copy_bitsmask) | copopt);
	// update
	update__to__dialog(data_std);
}

void
bgimg_manager::set_file(wxString name)
{
	delete img;
	img = 0;
	delete mods_img;
	mods_img = 0;
	img_fname() = name;
	update__to__dialog(data_std);
}

void
bgimg_manager::set_file_reset(wxString name)
{
	data_save = data_std;
	reset_default_params();
	set_file(name);
}

void
bgimg_manager::get_file(wxString& name)
{
	name = img_fname();
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
	#if 0
	width  =
		data_std.modswidth ? data_std.modswidth :
		((is_current() && mods_img) ? dim_type(mods_img->GetWidth()):
			data_std.origwidth);
	height =
		data_std.modsheight ? data_std.modsheight :
		((is_current() && mods_img) ? dim_type(mods_img->GetHeight()):
			data_std.origheight);
	#elif 0
	width  =
		data_std.modswidth ? data_std.modswidth : data_std.origwidth;
	height =
		data_std.modsheight ? data_std.modsheight : data_std.origheight;
	#else
	width  =
		data_std.modswidth ? data_std.modswidth : 0;
	height =
		data_std.modsheight ? data_std.modsheight : 0;
	#endif
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
bgimg_manager::set_hsv_vals(hsv_type hue, hsv_type sat, hsv_type val)
{
	data_std.hsv_h = hue;
	data_std.hsv_s = sat;
	data_std.hsv_v = val;
}

void
bgimg_manager::get_hsv_vals(hsv_type& hue, hsv_type& sat, hsv_type& val)
{
	hue = data_std.hsv_h;
	sat = data_std.hsv_s;
	val = data_std.hsv_v;
}

void
bgimg_manager::set_compression(cmp_type comp)
{
	data_std.band_comp = comp;
}

void
bgimg_manager::get_compression(cmp_type& comp)
{
	comp = data_std.band_comp;
}

void
bgimg_manager::set_rotation(off_type rot)
{
	data_std.degrotate = rot;
}

void
bgimg_manager::get_rotation(off_type& rot)
{
	rot = data_std.degrotate;
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

void
bgimg_manager::cancel_dialog()
{
	dialog_type* p = get_dlg();

	if ( !p || ! p->IsShown() ) {
		return;
	}

	wxControl* c = dynamic_cast<wxControl*>(p->FindWindow(wxID_CANCEL));
	if ( c ) {
		wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED);
		c->Command(e);
	}

	hide_dialog();
}

// saving copies of images:
bool
bgimg_manager::SaveOrigTo(const wxString& name, img_save_type_t type)
{
	// no image
	if ( img_fname().IsEmpty() ) {
		return false;
	}

	// orig not present
	if ( img == 0 ) {
		img = new wxImage(img_fname());
		if ( img == 0 || ! img->IsOk() ) {
			delete img;
			img = 0;
			return false;
		}
	}

	return img->SaveFile(name, type);
}

// saving copies of images:
bool
bgimg_manager::SaveModsTo(const wxString& name, img_save_type_t type)
{
	wxImage* tmp = get_mod_image();

	if ( tmp == 0 ) {
		return false;
	}

	return tmp->SaveFile(name, type);
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
		errno = 0;
		img = new wxImage(img_fname());

		if ( errno && (img == 0 || ! img->IsOk()) ) {
			int etmp = errno;
			// not so good . . .
			delete img;
			img = 0;

			wxString err;
			// TRANSLATORS: this is a failure to open a raster
			// image file specified by the user through a
			// dialog box, meant for a image on the background
			// of the working area ("canvas"). This error is most likely
			// at system level, like does not exist, or permissions.
			err.Printf(
				_("Cannot open backgroung image file \"%s\""),
				img_fname().c_str());

			// don't fail to empty file string: else inf recurse!
			img_fname().Empty();
			error_msg(err, ch2wxs(strerror(etmp)));

			return 0;
		} else if ( img == 0 || ! img->IsOk() ) {
			// not so good . . .
			delete img;
			img = 0;

			wxString err;
			// TRANSLATORS: this is a failure to open a raster
			// image file specified by the user through a
			// dialog box, meant for a image on the background
			// of the working area ("canvas"). This error is most likely
			// at the library level, like a corrupt image or
			// unsupported type.
			err.Printf(
				_("Failure opening backgroung image file \"%s\""),
				img_fname().c_str());

			// don't fail to empty file string: else inf recurse!
			img_fname().Empty();
			error_msg(err, _("Backgroung Image Error"));

			return 0;
		}

		data_std.origwidth = img->GetWidth();
		data_std.origheight = img->GetHeight();
	}

	delete mods_img;
	if ( false ) {
		mods_img = wximg_get_alphaconv(img);
	} else {
		mods_img = new wxImage(img->Copy());
	}

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

	if ( get_arg_width() || get_arg_height() ) {
		// TODO: make scale type an option
		const
#		if wxCHECK_VERSION(2, 9, 0)
		wxImageResizeQuality
#		else
		int
#		endif
		qual = wxIMAGE_QUALITY_HIGH; //qual = wxIMAGE_QUALITY_NORMAL;

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
	delete mods_img;
	mods_img = 0;
	delete img;
	img = 0;

	clear_params();
	data_std.zero();
	update__to__dialog(data_std);
	
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

	switch ( p->opt_save->GetSelection() ) {
		case 0:
			set_copy_orig(true);
			break;
		case 1:
			set_copy_changes(true);
			break;
		case 2:
		default:
			set_copy_none(true);
			break;
	}

	set_flip_horz(p->chk_flhorz->GetValue());
	set_flip_vert(p->chk_flvert->GetValue());
	set_conv_grey(p->chk_greyscale->GetValue());
	dat.degrotate = p->spin_ro->GetValue();
	dat.modswidth = p->spin_wi->GetValue();
	dat.modsheight = p->spin_hi->GetValue();
	dat.off_x = p->spin_offsx->GetValue();
	dat.off_y = p->spin_offsy->GetValue();
	dat.hsv_h = p->hsv_h->GetValue();
	dat.hsv_s = p->hsv_s->GetValue();
	dat.hsv_v = p->hsv_v->GetValue();
	dat.band_comp = p->band_comp->GetValue();
	dat.img_fname = p->selector_file->GetPath();
}

void
bgimg_manager::update__to__dialog(const datastruct& dat)
{
	
	dialog_type* p = get_dlg();
	if ( !p ) {
		return;
	}

	int copyopt;
	if ( get_copy_orig() ) {
		copyopt = 0;
	} else if ( get_copy_changes() ) {
		copyopt = 1;
	} else {
		copyopt = 2;
	}
	p->opt_save->SetSelection(copyopt);
		
	p->chk_flhorz->SetValue(get_flip_horz());
	p->chk_flvert->SetValue(get_flip_vert());
	p->chk_greyscale->SetValue(get_conv_grey());
	p->spin_ro->SetValue(dat.degrotate);
	p->spin_wi->SetValue(dat.modswidth);
	p->spin_hi->SetValue(dat.modsheight);
	p->spin_offsx->SetValue(dat.off_x);
	p->spin_offsy->SetValue(dat.off_y);
	p->hsv_h->SetValue(dat.hsv_h);
	p->hsv_s->SetValue(dat.hsv_s);
	p->hsv_v->SetValue(dat.hsv_v);
	p->band_comp->SetValue(dat.band_comp);
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
	, new_filename(false)
{
}

bg_img_dlg::~bg_img_dlg()
{
}

void
bg_img_dlg::put_preview()
{
	wxString s = mng->img_fname();

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
		wximg_adjhsv(&i,
			hsv_h->GetValue(), hsv_s->GetValue(), hsv_v->GetValue()
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

	if ( i && i->GetWidth() > 0 && i->GetHeight() > 0 ) {
		wxImage itmp(i->Copy());
		i = &itmp;

		int x = sz.GetWidth(), y = sz.GetHeight();
		int ix = spin_wi->GetValue() ? spin_wi->GetValue()
			: i->GetWidth();
		int iy = spin_hi->GetValue() ? spin_hi->GetValue()
			: i->GetHeight();
		int ox = spin_offsx->GetValue();
		int oy = spin_offsy->GetValue();

		float rn = float(x)  / float(y);
		float ro = float(ix) / float(iy);

		if ( ro < rn ) {
			x = int(float(y) * ro + 0.5);
			ox = ox * x / ix;
			oy = oy * x / ix;
		} else {
			y = int(float(x) / ro + 0.5);
			ox = ox * y / iy;
			oy = oy * y / iy;
		}

#		if wxCHECK_VERSION(3, 0, 0)
		// wx 3.0 does not have bug described below
		wxPoint off(
			ox + (sz.GetWidth() - x) / 2, oy + (sz.GetHeight() - y) / 2
		);
#		else
		// with 2.8.12, wxImage::Size(sz, off) fails as soon as
		// non-masked area passes size limit: image simply
		// disappears rather than get clipped -- so no scaled offset
		wxPoint off(
			(sz.GetWidth() - x) / 2, (sz.GetHeight() - y) / 2
		);
#		endif

		if ( i->HasMask() || i->HasAlpha() ) {
			bmp_preview->SetBitmap(
				wxBitmap(
					i->Scale(
						x, y, wxIMAGE_QUALITY_NORMAL
					).Size(sz, off)
				)
			);
		} else {
			bmp_preview->SetBitmap(
				wxBitmap(
					i->Scale(
						x, y, wxIMAGE_QUALITY_NORMAL
					).Size(sz, off,
						mng->rotbg_r, mng->rotbg_g, mng->rotbg_b
					)
				)
			);
		}
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
	event.Skip();

	// already working on it
	if ( new_filename ) {
		return;
	}

	// wx 3.0 bug: this is getting called *for each char entered*
	// in contradiction to the docs which say that it is only
	// called if file exists when control has wxFLP_FILE_MUST_EXIST
	// (and it does). Result: hand text entry is ++ungood.
	// Workaround: check file existence here; do nothing if
	// non-existent.
	if ( !::wxFileExists(selector_file->GetPath()) ) {
		return;
	}

	// handle existing file in idle handler; doing it here
	// gets infinite recursion if a error dialog is shown.
	new_filename = true;
}

void
bg_img_dlg::on_close_event(wxCloseEvent& event)
{
	mng->on_close_event(event);
	event.Skip();
}

void
bg_img_dlg::on_init_dlg(wxInitDialogEvent& event)
{
	mng->on_init_dlg(event);
	event.Skip();
}

void
bg_img_dlg::on_idle_dlg(wxIdleEvent& event)
{
	if ( new_filename ) {
		mng->img_fname().Empty();
		mng->get_mod_image();
		mng->img_fname() = selector_file->GetPath();
	
		put_preview();
		new_filename = false;
	}

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
