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

#if 0 // here C++ is helpful . . .
#	define swap_vals(v1, v2) \
	do { \
		v1 ^= v2; v2 ^= v1; v1 ^= v2; \
	} while ( 0 )
#else // . . . with specialization
template<typename T> inline void swap_vals(T& v1, T& v2)
{
	T t = v1; v1 = v2; v2 = t;
}
template<typename T> inline void swap_vals_int(T& v1, T& v2)
{
	v1 ^= v2; v2 ^= v1; v1 ^= v2;
}
template<> inline void swap_vals(signed char& v1, signed char& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned char& v1, unsigned char& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed short& v1, signed short& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned short& v1, unsigned short& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed int& v1, signed int& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned int& v1, unsigned int& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed long& v1, signed long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned long& v1, unsigned long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed long long& v1, signed long long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned long long& v1, unsigned long long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(bool& v1, bool& v2)
{ swap_vals_int(v1, v2); }
#endif

