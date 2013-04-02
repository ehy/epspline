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

/**********************************************************************\
  *     stack (auto) object to temporarily set LC_ALL to "C" for     *
  *     suspension of localisation in some block of code             *
\**********************************************************************/

#ifndef __C_LOCAL_ALL_TEMP__
#define __C_LOCAL_ALL_TEMP__

#include <clocale>
#include <string>

template<int C> class C_loc_Temp {
protected:
	std::string orig;
public:
	C_loc_Temp() {
		const char* p = std::setlocale(C, NULL);
		orig = p ? p : "";
		std::setlocale(C, "C");
	}
	~C_loc_Temp() {
		std::setlocale(C, orig.c_str());
	}

	// possibly useful sometime:
	const char* get_orig() const { return orig.c_str(); }
};

typedef C_loc_Temp<LC_ALL> C_All_loc_Temp;

// For brevity where desired:
typedef C_All_loc_Temp c_alltmp;

#endif // __C_LOCAL_ALL_TEMP__
