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

/**
 * To manage the application settings/preferences/options;
 * should receive stored configuration, and changes, and
 * where appropriate signal code elsewhere of a change.
 * 
 * Also, accomadate the preferences dialog window which is
 * designed to modeless, and has an "Apply" button which should
 * show changes immediately, but such changes should be stored
 * in a 'tentative' state because the dialog has a "Cancel" button
 * that should do as it says. "OK" button should remove tentative
 * state from changes. Dialog also has restore app defaults, and
 * restore initial saved configuration, and these should be provided
 * for in the settings manager.
 **/

#ifndef _A_PREFSMANAGER_H_
#define _A_PREFSMANAGER_H_

// Enumaration of preference settings
enum {
	global_prefs_first,

	// colors
	canvas_background_color,
	canvas_guides_color,
	canvas_grid_color,
	// bools, toggles
	canvas_grid_show,

	global_prefs_last
};

class A_Prefs_Manager {
public:

protected:

private:
	
public:
	
};

#endif  // _A_PREFSMANAGER_H_
