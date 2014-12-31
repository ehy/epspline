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
	typedef class ns_bg_img_dlg::bg_img_dlg dialog_type;

	// WxH dimensions type
	typedef unsigned long 	dim_type;
	// image offset type
	typedef signed	 short 	off_type;
	// image HSV adj. type
	typedef signed	 short 	hsv_type;

protected:
	friend dialog_type;

	// event handler overides in friend dialog call these
	void on_close_event(wxCloseEvent& event);
	void on_init_dlg(wxInitDialogEvent& event);
	void on_restore_defs(wxCommandEvent& event);
	void on_restore_conf(wxCommandEvent& event);
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
		arg_width = 0x100,
		arg_height = 0x200,
		arg_offsx = 0x400,
		arg_offsy = 0x800,
		// color transforms
		conv_grey = 0x1000,
		conv_hsvs = 0x2000,
		conv_hsvv = 0x4000,
		// test if transform is set
		trans_mask = 0xFFF0,
		// end tag -- just for comma
		end_enum = 0xFFFF
	};

	// structure to hold data
	struct datastruct {
		// params member var
		params parms;
		// on dlg show save here, and restore from here on cancel
		//params parmscancel;
	
		wxString img_fname;

		dim_type origwidth, origheight;
		dim_type modswidth, modsheight;
		off_type off_x, off_y;
		hsv_type hsv_s, hsv_v;

		datastruct() {
			parms = params_null;
			img_fname.Empty();
			origwidth = 0;
			origheight = 0;
			modswidth = 0;
			modsheight = 0;
			off_x = 0;
			off_y = 0;
			hsv_s = 0;
			hsv_v = 0;
		}

		datastruct(const datastruct& o) {
			*this = o;
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
			hsv_s = o.hsv_s;
			hsv_v = o.hsv_v;
			return *this;
		}

		bool operator == (const datastruct& o) {
			return (
				parms == o.parms &&
				img_fname == o.img_fname &&
				origwidth == o.origwidth &&
				origheight == o.origheight &&
				modswidth == o.modswidth &&
				modsheight == o.modsheight &&
				off_x == o.off_x &&
				off_y == o.off_y &&
				hsv_s == o.hsv_s &&
				hsv_v == o.hsv_v
			);
		}
	};

	// data store
	datastruct data_std;
	// data store saved (e.g. cancel dialog))
	datastruct data_save;
	void update_from_dialog(datastruct& dat);
	void update__to__dialog(const datastruct& dat);

	// helpers
	params& parms() { return *(&data_std.parms); }
	wxString& img_fname() { return *(&data_std.img_fname); }

	// this manages the wxImage and copies
	wxImage* img;
	wxImage* mods_img;

	// this manages the dialog box
	dialog_type* dlg;
	dialog_type* get_dlg();

	// params basic ops
	void clear_params() { set_params(0); }
	void set_default_params() {
		clear_params();
		set_copy_changes();
	}

	// tell using code to update
	void force_updates() {}

public:
	bgimg_manager();
	bgimg_manager(unsigned long p);
	~bgimg_manager();

	// set/get: conversion to fundamental type for convenience
	// of using code, e.g. read/write to file
	void set_params(unsigned long p) {
		parms() = static_cast<params>(p);
	}
	unsigned long get_params() {
		return static_cast<unsigned long>(parms());
	}

	// access for using app code
	void set_dimensions(dim_type width, dim_type height,
		off_type xo, off_type yo);
	void get_dimensions(dim_type& width, dim_type& height,
		off_type& xo, off_type& yo);
	void get_dimensions_orig(dim_type& width, dim_type& height);

	// get image for display -- after transforms are applied --
	// and caller should not modify the returned image, but
	// request changes here and then call this again.  Pointer
	// may change from call to call and so don't save.  Would
	// prefer to return const, but need to check that wxWidgets
	// calls to be made can accept const -- meanwhile assign
	// return from this to const pointers and note any problems
	// DO check return for 0! (either failure or not applicable)
	wxImage* get_mod_image();

	void show_dialog(bool show = true);
	void hide_dialog() { show_dialog(false); }

protected:
	// param access wrappers: isolate effects of scheme changes
	bool get_copy_none() { return (parms() & copy_bitsmask) == copy_none; }
	void set_copy_none(bool b = true) {
		set_params(b ? (parms() & ~copy_bitsmask) : (parms() | copy_changes));
	}
	bool get_copy_orig() { return parms() & copy_orig; }
	void set_copy_orig(bool b = true) {
		set_params(b ? (parms() | copy_orig) : (parms() & ~copy_orig));
	}
	bool get_copy_changes() { return parms() & copy_changes; }
	void set_copy_changes(bool b = true) {
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
	bool get_conv_hsvs() { return parms() & conv_hsvs; }
	void set_conv_hsvs(bool b = true) {
		set_params(b ? (parms() | conv_hsvs) : (parms() & ~conv_hsvs));
	}
	bool get_conv_hsvv() { return parms() & conv_hsvv; }
	void set_conv_hsvv(bool b = true) {
		set_params(b ? (parms() | conv_hsvv) : (parms() & ~conv_hsvv));
	}
	bool get_conv_grey() { return parms() & conv_grey; }
	void set_conv_grey(bool b = true) {
		set_params(b ? (parms() | conv_grey) : (parms() & ~conv_grey));
	}
	void toggle_conv_grey() {
		set_conv_grey(!get_conv_grey());
	}

	bool has_transform() { return parms() & trans_mask; }

	wxWindow* parent_wnd() { return wxGetApp().GetTopWindow(); }
};

// here lies the subclass of the generated bg_image dialog class,
// within which virtual methods find their implementation
namespace ns_bg_img_dlg {
	class bg_img_dlg : public bg_image {
	protected:
		bgimg_manager*	mng;
		// for file file selector
		wxString		suffixes;

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
		virtual void on_greyscale(wxCommandEvent& event);
		virtual void on_width(wxCommandEvent& event);
		virtual void on_height(wxCommandEvent& event);
		virtual void on_offs_x(wxCommandEvent& event);
		virtual void on_offs_y(wxCommandEvent& event);
		virtual void on_hsv_s_scroll(wxScrollEvent& event);
		virtual void on_hsv_v_scroll(wxScrollEvent& event);
		virtual void on_file_select(wxFileDirPickerEvent& event);
		virtual void on_close_event(wxCloseEvent& event);
		virtual void on_init_dlg(wxInitDialogEvent& event);
		virtual void on_apply(wxCommandEvent& event);
		virtual void on_cancel(wxCommandEvent& event);
		virtual void on_help(wxCommandEvent& event);
		virtual void on_ok(wxCommandEvent& event);

		void put_preview();
		void set_preview(wxImage* i);

		// for file file selector --
		// Presently suffixes cannot be used due to deficiency
		// in design of wxFilePickerControl, that is lacks
		// any way to change the wildcard string: it can only
		// be set in ctor, and since the ctor is invoked in
		// generated code from a GUI dialog builder, we don't
		// change it.  The argument used now is a string constant
		// taken from static wxImage::GetImageExtWildcard()
		// output from dev's build, but it is not certain to
		// be correct for every build a user might have.
		// So be it. The suffix data and methods remain here out
		// of hope for the future.
		wxString get_suffix_selector() { return suffixes; }
		wxString set_suffix_selector(const wxString& s) {
			wxString t = suffixes;
			suffixes = s;
			return t;
		}
	}; // END class bg_img_dlg
}; // END namespace ns_bg_img_dlg

#endif //_A_BGIMG_MANAGER_H_
