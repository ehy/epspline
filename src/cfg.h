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

#ifndef _CFG_H_
#define _CFG_H_

#include "stdcc.h"

#if ! defined(__UNIX__)  // is this a Unix-like system?
#if defined(__unix__) || defined(__unix) || defined(sun) || \
	defined(__sun) || defined(__LINUX__) || defined(__linux__) || \
	defined(linux) || defined(__OpenBSD__) || defined(__NetBSD__) || \
	defined(__FreeBSD__) || defined(SYSV) || defined(SVR4) || \
	defined(__SUNPRO_CC) // And so on as nec.
#	define __UNIX__ 1
#endif
#endif // __UNIX__

// This would detect old Symantec C++, distinct from more
// recent Digital Mars C++ -- surely the code as it stands
// cannot compile with old SC, but I'll leave this for
// (my own) historical interest.
#if defined(__WXMSW__) && __SC__ && !__DMC__
#define HAVE_OLD_HEADERS 1
#endif

#if 1
#define SEPC wxFileName::GetPathSeparator()
#define SEPS wxString(SEPC, size_t(1))
#else
#if defined(__WXMSW__)
const wxChar SEPC = wxT('\\');
#define    SEPS wxT("\\")
#else
const wxChar SEPC = wxT('/');
#define    SEPS wxT("/")
#endif
#endif // 1

// Defaults that may be changed at compile time.
// (Some of these *should* have runtime configuration too.)

// splines.cpp:
#ifndef FORCE_CONSTANT_SPLINE_INCREMENT
#define FORCE_CONSTANT_SPLINE_INCREMENT   0
#endif
// increment constants below are used only if
// macro above is non-zero
#ifndef DEFAULT_SPLINE_INCREMENT
#define DEFAULT_SPLINE_INCREMENT		64
#endif
#ifndef LINEAR_SPLINE_INCREMENT
#define LINEAR_SPLINE_INCREMENT			16
#endif
#ifndef QUADRATIC_SPLINE_INCREMENT
#define QUADRATIC_SPLINE_INCREMENT		DEFAULT_SPLINE_INCREMENT
#endif
#ifndef CUBIC_SPLINE_INCREMENT
#define CUBIC_SPLINE_INCREMENT			DEFAULT_SPLINE_INCREMENT
#endif
#ifndef BEZIER_SPLINE_INCREMENT
#define BEZIER_SPLINE_INCREMENT			DEFAULT_SPLINE_INCREMENT
#endif

// a_canvas.cpp
// Canvas has undo and re-do stacks
#ifndef DEFAULT_UNDO_SIZE
#define DEFAULT_UNDO_SIZE	64
#endif
#ifndef TRANSFORM_HANDLE_SIZE
#define TRANSFORM_HANDLE_SIZE	10
#endif
#ifndef TRANSFORM_SCALE_CONTRAINT
#define TRANSFORM_SCALE_CONTRAINT	15
#endif
#ifndef TRANSFORM_SHEAR_CONTRAINT
#define TRANSFORM_SHEAR_CONTRAINT	15
#endif
#ifndef TRANSFORM_ROTATE_CONTRAINT
#define TRANSFORM_ROTATE_CONTRAINT	15.0 * M_PI / 180.0
#endif

#endif // _CFG_H_

