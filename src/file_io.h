/*
 * file_io.h
 * 
 * Copyright 2015 Ed Hynan <edhynan@gmail.com>
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

// types and procs for read/write I/O
//
// This is the header associated with io.cpp -- the name
// io.h might conflict with DOS dev tools, hence the name file_io.h

#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include "splines.h"
#include "wxutil.h"
#include "util.h"

// The .PSE file Write/Read routines + struct for additional
// data for per-file section (used by canvas)
class bgimg_manager;
struct IO_AddlData {
	bool init; // remains false if not inited due to old file
	int scrollpos_h;
	int scrollpos_v;
	int scale;
	bgimg_manager* bgm;

	IO_AddlData() {
		init = false;
		scrollpos_h = scrollpos_v = scale = 0;
		bgm = 0;
	}
};

// Write/Read routines
bool WriteData(const wxString& fname, const std::list<SplineBase*>& lst
	, const std::vector<int>& hg, const std::vector<int>& vg
	, const IO_AddlData* addl, const wxString* pcomment = 0);
int  ReadData(const wxString& fname, std::list<SplineBase*>& lst
	, std::vector<int>& hg, std::vector<int>& vg
	, IO_AddlData* addl, wxString* pcomment = 0);

// Must check and sanitize the real numbers when read, as
// extreme values can cause big trouble like infinite loops,
// as dicovered using input from a T1 font conversion, with
// a bug that made values like 5.20004e+08; while fixes
// are needed at the point of the infinite loop (I don't even
// know now if it's in app code or wx code), sanitization
// should happen here anyway.
// Set the function pointer 'io_sanitise_spline_point' to
// something better than the default.
// (App is not presently multi-threaded; if threads are eventually
// employed, the io code had better be used by one thread, likely
// the main thread.)
//
// Return of 0 is OK, failure non-zero is assigned to errno,
// so it should be useful in that role -- probably EINVAL.
extern int (*io_sanitise_spline_point)(void*, SplinePoint*);
extern void* io_sanitise_spline_point_data;

#endif // _FILE_IO_H_
