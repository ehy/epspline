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

#ifndef _A_BGIMG_MANAGER_H_
#define _A_BGIMG_MANAGER_H_

#include "bg_image_dlg.h"
#include <cstring>

// forward ref to dialog subclass
namespace ns_bg_img_dlg {
	class bg_img_dlg;
};

// canvas background image manager for use in app;
// also manages bg img dialog
class bgimg_manager {
public:
	friend class ns_bg_img_dlg::bg_img_dlg;
	typedef class ns_bg_img_dlg::bg_img_dlg dialog_type;

	// WxH dimensions type
	typedef unsigned long 	dim_type;
	// image offset type
	typedef signed	 short 	off_type;
	// image HSV adj. type
	typedef signed	 short 	hsv_type;
	// image compress type
	typedef signed	 short 	cmp_type;

	// for using code update callback
	typedef void*	cb_update_arg;
	typedef void   (*cb_update_func)(cb_update_arg);

protected:
	// event handler overides in friend dialog call these
	void on_close_event(wxCloseEvent& event);
	void on_init_dlg(wxInitDialogEvent& event);
	void on_apply(wxCommandEvent& event);
	void on_cancel(wxCommandEvent& event);
	void on_help(wxCommandEvent& event);
	void on_OK(wxCommandEvent& event);
	
	// params/args user-set in dialog box
	enum params {
		params_null = 0,
		// copy options -- several states
		copy_none = 0,
		copy_orig = 1,
		copy_changes = 2,
		copy_bitsmask = 0x0F,
		// layout transforms
		flip_horz = 0x10,
		flip_vert = 0x20,
		arg_rot   = 0x80,
		arg_width = 0x100,
		arg_height = 0x200,
		arg_offsx = 0x400,
		arg_offsy = 0x800,
		// color transforms
		conv_grey = 0x1000,
		conv_hsvh = 0x2000,
		conv_hsvs = 0x4000,
		conv_hsvv = 0x8000,
		conv_bcmp = 0x10000,
		// test if transform is set
		trans_mask = 0xFFFF0,
		// end tag -- just for comma
		end_enum = 0xFFFFF
	};

	// structure to hold data
#   if defined(__SC__) || defined(__DMC__) // Digital Mars w32
	friend
#   endif
	struct datastruct {
		// params member var
		params parms;
	
		wxString img_fname;

		dim_type origwidth, origheight;
		dim_type modswidth, modsheight;
		off_type off_x, off_y;
		hsv_type hsv_h, hsv_s, hsv_v;
		cmp_type band_comp;
		off_type degrotate;

		datastruct() {
			zero();
		}

		datastruct(const datastruct& o) {
			*this = o;
		}

		void zero() {
			parms = params_null;
			img_fname.Empty();
			origwidth = 0;
			origheight = 0;
			modswidth = 0;
			modsheight = 0;
			off_x = 0;
			off_y = 0;
			hsv_h = 0;
			hsv_s = 0;
			hsv_v = 0;
			band_comp = 0;
			degrotate = 0;
		}

		datastruct& operator = (const datastruct& o) {
			parms = o.parms;
			img_fname = o.img_fname;
			origwidth = o.origwidth;
			origheight = o.origheight;
			modswidth = o.modswidth;
			modsheight = o.modsheight;
			off_x = o.off_x;
			off_y = o.off_y;
			hsv_h = o.hsv_h;
			hsv_s = o.hsv_s;
			hsv_v = o.hsv_v;
			band_comp = o.band_comp;
			degrotate = o.degrotate;
			return *this;
		}

		bool operator == (const datastruct& o) const {
			return (
				parms == o.parms &&
				img_fname == o.img_fname &&
				origwidth == o.origwidth &&
				origheight == o.origheight &&
				modswidth == o.modswidth &&
				modsheight == o.modsheight &&
				off_x == o.off_x &&
				off_y == o.off_y &&
				hsv_h == o.hsv_h &&
				hsv_s == o.hsv_s &&
				hsv_v == o.hsv_v &&
				band_comp == o.band_comp &&
				degrotate == o.degrotate
			);
		}

		bool operator != (const datastruct& o) const {
			return ! (*this == o);
		}
	};

	// data store
	datastruct data_std;
	// data store saved (e.g. cancel dialog))
	datastruct data_save;
	void update_from_dialog(datastruct& dat);
	void update__to__dialog(const datastruct& dat);

	// helpers
	params& parms() { return data_std.parms; }
	wxString& img_fname() { return data_std.img_fname; }

	// this manages the wxImage and copies
	wxImage* img;
	wxImage* mods_img;

	// this manages the dialog box
	dialog_type* dlg;
	dialog_type* get_dlg();

	// params basic ops
	void clear_params() { set_params(0); }
	void set_default_params();
	void reset_default_params();

	// tell using code to update
	void force_updates();

	// display error message; e.g., image file open failure
	void error_msg(wxString msg, wxString addl);

	// for using code update callback
	cb_update_func  cb_func;
	cb_update_arg	cb_arg;

	// mods_img is current
	bool mods_current;

public:
	bgimg_manager();
	bgimg_manager(unsigned long p);
	~bgimg_manager();

	// for using code update callback
	void set_update_callback(cb_update_func f, cb_update_arg a = 0) {
		cb_func = f;
		cb_arg  = a;
	}

	// mods_img is current
	bool is_current() { return mods_current; }

	// set/get: conversion to fundamental type for convenience
	// of using code, e.g. read/write to file
	void set_params(unsigned long p) {
		parms() = static_cast<params>(p);
	}
	unsigned long get_params() {
		return static_cast<unsigned long>(parms());
	}

	// access for using app code
	void set_file(wxString name);
	void get_file(wxString& name);
	void set_dimensions(dim_type width, dim_type height,
		off_type xo, off_type yo);
	// width or height field is empty, zero is return for that
	void get_dimensions(dim_type& width, dim_type& height,
		off_type& xo, off_type& yo);
	// width or height field is empty, original is return for that
	void get_dimensions_noz(dim_type& width, dim_type& height,
		off_type& xo, off_type& yo);
	void get_dimensions_orig(dim_type& width, dim_type& height);
	void set_hsv_vals(hsv_type hue, hsv_type sat, hsv_type val);
	void get_hsv_vals(hsv_type& hue, hsv_type& sat, hsv_type& val);
	void set_compression(cmp_type comp);
	void get_compression(cmp_type& comp);
	void set_rotation(off_type rot);
	void get_rotation(off_type& rot);
	// like set_file() above, plus reset data; e.g., if
	// mod image is set as orig image, so mods are not repeated
	void set_file_reset(wxString name);

	// saving copies of images:
#	if wxCHECK_VERSION(2, 9, 0) // sigh
	typedef wxBitmapType	img_save_type_t;
#	else
	typedef int				img_save_type_t;
#	endif
	bool SaveOrigTo(const wxString& name,
		img_save_type_t type = wxBITMAP_TYPE_PNG);
	bool SaveModsTo(const wxString& name,
		img_save_type_t type = wxBITMAP_TYPE_PNG);

	// get image for display -- after transforms are applied --
	// and caller should not modify the returned image, but
	// request changes here and then call this again.  Pointer
	// may change from call to call and so don't save.  Would
	// prefer to return const, but need to check that wxWidgets
	// calls to be made can accept const -- meanwhile assign
	// return from this to const pointers and note any problems
	// DO check return for 0! (either failure or not applicable)
	wxImage* get_mod_image();

	// remove bg image
	void remove_image(bool update = true);

	void show_dialog(bool show = true);
	void hide_dialog() { show_dialog(false); }
	void cancel_dialog();

	// background color when image is rotated, etc.
	unsigned char rotbg_r;
	unsigned char rotbg_g;
	unsigned char rotbg_b;
	void set_rotbg(
		unsigned char r = wximg_rotmask_default_r,
		unsigned char g = wximg_rotmask_default_g,
		unsigned char b = wximg_rotmask_default_b
	) {
		rotbg_r = r;
		rotbg_g = g;
		rotbg_b = b;
		delete mods_img;
		mods_img = 0;
	}

	// param access wrappers: isolate effects of scheme changes
	bool get_copy_none() { return (parms() & copy_bitsmask) == copy_none; }
	void set_copy_none(bool b = true) {
		set_params(b ? (parms() & ~copy_bitsmask) : (parms() | copy_orig));
	}
	bool get_copy_orig() { return parms() & copy_orig; }
	void set_copy_orig(bool b = true) {
		set_copy_none(true);
		set_params(b ? (parms() | copy_orig) : (parms() & ~copy_orig));
	}
	bool get_copy_changes() { return parms() & copy_changes; }
	void set_copy_changes(bool b = true) {
		set_copy_none(true);
		set_params(b ? (parms() | copy_changes) : (parms() & ~copy_changes));
	}
	bool get_flip_horz() { return parms() & flip_horz; }
	void set_flip_horz(bool b = true) {
		set_params(b ? (parms() | flip_horz) : (parms() & ~flip_horz));
	}
	void toggle_flip_horz() {
		set_flip_horz(!get_flip_horz());
	}
	bool get_flip_vert() { return parms() & flip_vert; }
	void set_flip_vert(bool b = true) {
		set_params(b ? (parms() | flip_vert) : (parms() & ~flip_vert));
	}
	void toggle_flip_vert() {
		set_flip_vert(!get_flip_vert());
	}
	bool get_arg_rotate() { return parms() & arg_rot; }
	void set_arg_rotate(bool b = true) {
		set_params(b ? (parms() | arg_rot) : (parms() & ~arg_rot));
	}
	bool get_arg_width() { return parms() & arg_width; }
	void set_arg_width(bool b = true) {
		set_params(b ? (parms() | arg_width) : (parms() & ~arg_width));
	}
	bool get_arg_height() { return parms() & arg_height; }
	void set_arg_height(bool b = true) {
		set_params(b ? (parms() | arg_height) : (parms() & ~arg_height));
	}
	bool get_arg_offsx() { return parms() & arg_offsx; }
	void set_arg_offsx(bool b = true) {
		set_params(b ? (parms() | arg_offsx) : (parms() & ~arg_offsx));
	}
	bool get_arg_offsy() { return parms() & arg_offsy; }
	void set_arg_offsy(bool b = true) {
		set_params(b ? (parms() | arg_offsy) : (parms() & ~arg_offsy));
	}
	bool get_conv_hsvh() { return parms() & conv_hsvh; }
	void set_conv_hsvh(bool b = true) {
		set_params(b ? (parms() | conv_hsvh) : (parms() & ~conv_hsvh));
	}
	bool get_conv_hsvs() { return parms() & conv_hsvs; }
	void set_conv_hsvs(bool b = true) {
		set_params(b ? (parms() | conv_hsvs) : (parms() & ~conv_hsvs));
	}
	bool get_conv_hsvv() { return parms() & conv_hsvv; }
	void set_conv_hsvv(bool b = true) {
		set_params(b ? (parms() | conv_hsvv) : (parms() & ~conv_hsvv));
	}
	bool get_conv_band_comp() { return parms() & conv_bcmp; }
	void set_conv_band_comp(bool b = true) {
		set_params(b ? (parms() | conv_bcmp) : (parms() & ~conv_bcmp));
	}
	bool get_conv_grey() { return parms() & conv_grey; }
	void set_conv_grey(bool b = true) {
		set_params(b ? (parms() | conv_grey) : (parms() & ~conv_grey));
	}
	void toggle_conv_grey() {
		set_conv_grey(!get_conv_grey());
	}

	bool has_transform() { return parms() & trans_mask; }

	wxWindow* parent_wnd();
};

// here lies the subclass of the generated bg_image dialog class,
// within which virtual methods find their implementation
namespace ns_bg_img_dlg {
	class bg_img_dlg : public bg_image {
	protected:
		bgimg_manager*	mng;
		// set in file picker handler gets name of existing file
		bool new_filename;
		// width x height unlock key up/down
		bool wh_unlock;

	public:
		bg_img_dlg(
			bgimg_manager* manager,
			wxWindowID id = wxID_ANY,
			const wxString& title = _("Background Image"),
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE);
		~bg_img_dlg();
		
		// Virtual event handler overides
		virtual void on_copy_opt(wxCommandEvent& event);
		virtual void on_flip_horz(wxCommandEvent& event);
		virtual void on_flip_vert(wxCommandEvent& event);
		virtual void on_rotate(wxCommandEvent& event);
		virtual void on_greyscale(wxCommandEvent& event);
		virtual void on_width(wxCommandEvent& event);
		virtual void on_height(wxCommandEvent& event);
		virtual void on_offs_x(wxCommandEvent& event);
		virtual void on_offs_y(wxCommandEvent& event);
		virtual void on_hsv_h_scroll(wxScrollEvent& event);
		virtual void on_hsv_s_scroll(wxScrollEvent& event);
		virtual void on_hsv_v_scroll(wxScrollEvent& event);
		virtual void on_band_comp_scroll(wxScrollEvent& event);
		virtual void on_file_select(wxFileDirPickerEvent& event);
		virtual void on_close_event(wxCloseEvent& event);
		virtual void on_init_dlg(wxInitDialogEvent& event);
		virtual void on_key_down( wxKeyEvent& event );
		virtual void on_key_up( wxKeyEvent& event );
		virtual void on_idle_dlg(wxIdleEvent& event);
		virtual void on_apply(wxCommandEvent& event);
		virtual void on_cancel(wxCommandEvent& event);
		virtual void on_help(wxCommandEvent& event);
		virtual void on_ok(wxCommandEvent& event);

		void put_preview();
		void set_preview(wxImage* i);
	}; // END class bg_img_dlg
}; // END namespace ns_bg_img_dlg

#endif //_A_BGIMG_MANAGER_H_
