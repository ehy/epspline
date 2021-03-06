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
  *     stack (auto) object to temporarily set LC_NUMERIC to         *
  *     "C" for textual real number formatting and scanning.         *
\**********************************************************************/

#ifndef __C_LOCAL_NUMERIC_TEMP__
#define __C_LOCAL_NUMERIC_TEMP__

#include "clocall.h"

typedef C_loc_Temp<LC_NUMERIC> C_Numeric_Temp;

// For brevity where desired:
typedef C_Numeric_Temp cnumtmp;

#endif // __C_LOCAL_NUMERIC_TEMP__
