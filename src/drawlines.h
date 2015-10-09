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

/*
 * drawlines.h -- C++ template header providing an adaptation
 *                of the Xiaolin Wu line algorithm
 */

#ifndef _ANTI_A_LINE_H_
#define _ANTI_A_LINE_H_  0x0010

#ifndef   ANTI_ALIAS_LINE_NAMESPACE_NAME
#define   ANTI_ALIAS_LINE_NAMESPACE_NAME	anti_aliasing_lines
#endif // ANTI_ALIAS_LINE_NAMESPACE_NAME

namespace ANTI_ALIAS_LINE_NAMESPACE_NAME {

using namespace std;

typedef unsigned char clrcmp; // color component
const clrcmp ccmp_max = UCHAR_MAX;
const clrcmp ccmp_min = 0;

// sum of fractions p1 and p2 should not exceed 1.0
// unless colorful effects are wanted
template<typename T, typename P> inline T
mul_t2(T v1, P p1)
{
	return T( (p1 * v1) );
}
template<typename T, typename P> inline T
mixpct(T v1, P p1, T v2, P p2)
{
	return mul_t2(v1, p1) + mul_t2(v2, p2);
}

// pixel template
template<typename T, T TI = 0>
struct tpl_pxl {
	typedef T cmp_t;
	T r, g, b;

	tpl_pxl(T rc = TI, T gc = TI, T bc = TI)
		: r(rc), g(gc), b(bc) {}
	tpl_pxl(const tpl_pxl<T, TI>& o) { *this = o; }

	inline tpl_pxl<T, TI>& swap(tpl_pxl<T, TI>& o) {
		swap_vals(r, o.r);
		swap_vals(g, o.g);
		swap_vals(b, o.b);
		return *this;
	}

	inline bool operator == (const tpl_pxl<T, TI>& o) {
		return r == o.r && g == o.g && b == o.b;
	}

	inline bool operator != (const tpl_pxl<T, TI>& o) {
		return ! (*this == o);
	}

	inline tpl_pxl<T, TI>& operator = (const tpl_pxl<T, TI>& o) {
		r = o.r; g = o.g; b = o.b; return *this;
	}

	template<typename fT> inline tpl_pxl<T, TI>&
	mix(const tpl_pxl<T, TI>& o, fT s) {
		const fT t = fT(1) - s;
		r = mixpct(o.r, s, r, t);
		g = mixpct(o.g, s, g, t);
		b = mixpct(o.b, s, b, t);
		return *this;
	}
};

// pixel template xor subs =
template<typename T, T TI = 0>
struct tpl_pxl_xor {
	typedef T cmp_t;
	T r, g, b;

	tpl_pxl_xor(T rc = TI, T gc = TI, T bc = TI)
		: r(rc), g(gc), b(bc) {}
	tpl_pxl_xor(const tpl_pxl_xor<T, TI>& o) { *this = o; }

	inline tpl_pxl_xor<T, TI>& swap(tpl_pxl_xor<T, TI>& o) {
		swap_vals(r, o.r);
		swap_vals(g, o.g);
		swap_vals(b, o.b);
		return *this;
	}

	inline bool operator == (const tpl_pxl_xor<T, TI>& o) {
		return r == o.r && g == o.g && b == o.b;
	}

	inline bool operator != (const tpl_pxl_xor<T, TI>& o) {
		return ! (*this == o);
	}

	inline tpl_pxl_xor<T, TI>&
	operator = (const tpl_pxl_xor<T, TI>& o) {
		r ^= o.r; g ^= o.g; b ^= o.b;
		return *this;
	}

	template<typename fT> inline tpl_pxl_xor<T, TI>&
	mix(const tpl_pxl_xor<T, TI>& o, fT s) {
		//const fT t = fT(1) - s;
		r ^= T(o.r * s);
		g ^= T(o.g * s);
		b ^= T(o.b * s);
		return *this;
	}
};

// pixel struct
typedef tpl_pxl<clrcmp, ccmp_max> pxl;
typedef tpl_pxl_xor<clrcmp, ccmp_max> pxl_xor;


// adapt draw ops to image type;
// not using a virtual interface
// because speed is needed, so alter
// this as necessary
class draw_device {
	draw_device();
#ifdef __WXWINDOWS__
public:
	typedef wxImage rdat;
	typedef unsigned char dtype;
protected:
	rdat& R;
	dtype* pd;
	int w, h;
	int pixwidth;
	int rowlen;
public:
	draw_device(rdat& pimg)
		: R(pimg), pd(R.GetData())
		, w(R.GetWidth()), h(R.GetHeight())
		, pixwidth(R.HasAlpha() ? 4 : 3)
		, rowlen(w * pixwidth)
	{
	}
	~draw_device() {}

	inline int Width() const { return w; }
	inline int Height() const { return h; }
	inline int Rowlen() const { return rowlen; }
	inline int Pixsize() const { return pixwidth; }
	inline const rdat& Img() const { return R; }
#else // wx
#	error "class draw_device must be specialized by hand!"
#endif // __WXWINDOWS__

	inline
	dtype* at(int x, int y)
	{
		return pd + (rowlen * y + x * pixwidth);
	}

	template<typename T> inline
	T* t_cast_at(int x, int y)
	{
		return (T*)at(x, y);
	}

	template<typename fT, typename T> inline
	void mix(int x, int y, const T& clr, fT intensity)
	{
		t_cast_at<T>(x, y)->mix(clr, intensity);
	}

	template<typename T> inline
	void put(int x, int y, const T& clr)
	{
		*t_cast_at<T>(x, y) = clr;
	}
};

template<typename fT, typename T> inline void
mix_pixel(
	draw_device& dr,
	int x, int y, fT intensity,
	int cols, int rows, const T& clr)
{
	if ( x >= 0 && x < cols && y >= 0 && y < rows )
		dr.mix(x, y, clr, intensity);
}

template<typename T> inline void
put_pixel(
	draw_device& dr,
	int x, int y,
	int cols, int rows, const T& clr)
{
	if ( x >= 0 && x < cols && y >= 0 && y < rows )
		dr.put(x, y, clr);
}

template<typename fT, typename pixT> void
drawline_wu(
	fT thickness,
	fT x0, fT y0, fT x1, fT y1,
	draw_device& drdev,
	long cols, long rows,
	const pixT& fg,
	// bool: maintain apparent thickness regardless of line angle
	bool const_thickness)
{
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if ( steep ) {
		swap_vals(x0, y0);
		swap_vals(x1, y1);
	}
	if ( x0 > x1 ) {
		swap_vals(x1, x0);
		swap_vals(y1, y0);
	}

	fT dx = x1 - x0;
	fT dy = y1 - y0;
	fT gradient = dy / dx;

	// reusable: when using modf for integer and/or fraction
	fT intpart;
	// Added EH: for line thickness
	long ithick;
	fT fthick;
	if ( const_thickness ) {
		fT agrad = abs(gradient);
		//agrad *= 0.8;
		agrad = agrad > 1.0 ? 1.0 / agrad : agrad;
		fT hslop = hypot(1.0, agrad);
		fT hvthick = thickness * hslop; // horz. or vert. extent
#		if 1
		const fT fudge = 0.0;
		fthick = modf(hvthick + fudge, &intpart);
		ithick = long(intpart);
		fthick -= fudge;
		fthick /= 2.0;
#		else
		//ithick = lround(hvthick);
		ithick = floor(hvthick);
		fthick = 0;
#		endif
	} else {
		ithick = lround(thickness);
		fthick = 0;
	}

	// 1st endpoint data setup
	fT xend = round(x0);
	fT yend = y0 + gradient * (xend - x0);

	fT xgap = 1.0 - modf(x0 + 0.5, &intpart);

	int xpxl1 = (int)xend;
	int ypxl1; // assigned next modf()

	fT yadd = fthick;
	//fT yadd = 0.0;
#define DO_ENDPTS 1
	// 1st endpoint
	fT yefrac = modf(yend + yadd, &intpart);
	ypxl1 = (int)intpart;
#	if DO_ENDPTS
	if ( steep ) {
		// put fractional thickness
		long t = ithick; t >>= 1;
		ypxl1 -= t;
		// 1st fraction
		if ( ypxl1 >= 0 && ypxl1 < cols ) {
			mix_pixel(drdev, ypxl1, xpxl1, (1.0 - yefrac) * xgap,
				cols, rows, fg);
		}
		// loop over thickness; leave count out of loop scope
		for ( t = 1; t < ithick; t++ ) {
			int dit = ypxl1 + t;
			if ( dit < 0 || dit >= cols )
				continue;
			put_pixel(drdev, dit, xpxl1, cols, rows, fg);
		}
		// last fraction
		t += ypxl1;
		if ( t < cols && t >= 0 ) {
			mix_pixel(drdev, t, xpxl1, yefrac * xgap,
				cols, rows, fg);
		}
	} else {
		// put fractional thickness
		long t = ithick; t >>= 1;
		ypxl1 -= t;
		// 1st fraction
		if ( ypxl1 >= 0 && ypxl1 < rows ) {
			mix_pixel(drdev, xpxl1, ypxl1, (1.0 - yefrac) * xgap,
				cols, rows, fg);
		}
		// loop over thickness; leave count out of loop scope
		for ( t = 1; t < ithick; t++ ) {
			int dit = ypxl1 + t;
			if ( dit < 0 || dit >= rows )
				continue;
			put_pixel(drdev, xpxl1, dit, cols, rows, fg);
		}
		// last fraction
		t += ypxl1;
		if ( t < rows && t >= 0 ) {
			mix_pixel(drdev, xpxl1, t, yefrac * xgap,
				cols, rows, fg);
		}
	}
#	endif // if DO_ENDPTS

	// 1st y-intersect for main loop
	fT intery = yend + gradient;
	// hack: near 45 deg aa needs hack so intery is not integral
	if ( false /* && fp_eq(gradient, 1.0) */ ) {
		intery += 0.1;
	}

	// 2nd endpoint data setup
	xend = round(x1);
	//xend = floor(x1);
	yend = y1 + gradient * (xend - x1);

	xgap = modf(x1 + 0.5, &intpart);

	int xpxl2 = (int)xend;
	int ypxl2; // assigned next modf()

	// 2nd endpoint
	yefrac = modf(yend + yadd, &intpart);
	ypxl2 = (int)intpart;
#	if DO_ENDPTS
	if ( steep ) {
		// put fractional thickness
		long t = ithick; t >>= 1;
		ypxl2 -= t;
		// 1st fraction
		if ( ypxl2 >= 0 && ypxl2 < cols ) {
			mix_pixel(drdev, ypxl2, xpxl2, (1.0 - yefrac) * xgap,
				cols, rows, fg);
		}
		// loop over thickness; leave count out of loop scope
		for ( t = 1; t < ithick; t++ ) {
			int dit = ypxl2 + t;
			if ( dit < 0 || dit >= cols )
				continue;
			put_pixel(drdev, dit, xpxl2, cols, rows, fg);
		}
		// last fraction
		t += ypxl2;
		if ( t < cols && t >= 0 ) {
			mix_pixel(drdev, t, xpxl2, yefrac * xgap,
				cols, rows, fg);
		}
	} else {
		// put fractional thickness
		long t = ithick; t >>= 1;
		ypxl2 -= t;
		// 1st fraction
		if ( ypxl2 >= 0 && ypxl2 < rows ) {
			mix_pixel(drdev, xpxl2, ypxl2, (1.0 - yefrac) * xgap,
				cols, rows, fg);
		}
		// loop over thickness; leave count out of loop scope
		for ( t = 1; t < ithick; t++ ) {
			int dit = ypxl2 + t;
			if ( dit < 0 || dit >= rows )
				continue;
			put_pixel(drdev, xpxl2, dit, cols, rows, fg);
		}
		// last fraction
		t += ypxl2;
		if ( t < rows && t >= 0 ) {
			mix_pixel(drdev, xpxl2, t, yefrac * xgap,
				cols, rows, fg);
		}
	}
#	endif // if DO_ENDPTS

	// line between endpoints
#	if DO_ENDPTS
	const int p1off = 1;
	const int p2off = 0;
#	else  // if DO_ENDPTS
	const int p1off = 0;
	const int p2off = 0;
#	endif // if DO_ENDPTS
	if ( steep ) {
		for ( int x = xpxl1+p1off;
			  x < (xpxl2-p2off);
			  x++, intery += gradient ) {
			yefrac = modf(intery + yadd, &intpart);
			// put fractional thickness
			long t = ithick; t >>= 1;
			int ipart = (int)intpart - t;
			// 1st fraction
			if ( ipart >= 0 && ipart < cols ) {
				mix_pixel(drdev, ipart, x, (1.0 - yefrac),
					cols, rows, fg);
			}
			// loop over thickness; leave count out of loop scope
			for ( t = 1; t < ithick; t++ ) {
				int dit = ipart + t;
				if ( dit < 0 || dit >= cols )
					continue;
				put_pixel(drdev, dit, x, cols, rows, fg);
			}
			// last fraction
			t += ipart;
			if ( t < cols && t >= 0 ) {
				mix_pixel(drdev, t, x, yefrac,
					cols, rows, fg);
			}
			// end loop contents
		}
	} else {
		for ( int x = xpxl1+p1off;
			  x < (xpxl2-p2off);
			  x++, intery += gradient ) {
			yefrac = modf(intery + yadd, &intpart);
			// put fractional thickness
			long t = ithick; t >>= 1;
			int ipart = (int)intpart - t;
			// 1st fraction
			if ( ipart >= 0 && ipart < rows ) {
				mix_pixel(drdev, x, ipart, (1.0 - yefrac),
					cols, rows, fg);
			}
			// loop over thickness; leave count out of loop scope
			for ( t = 1; t < ithick; t++ ) {
				int dit = ipart + t;
				if ( dit < 0 || dit >= rows )
					continue;
				put_pixel(drdev, x, dit, cols, rows, fg);
			}
			// last fraction
			t += ipart;
			if ( t < rows && t >= 0 ) {
				mix_pixel(drdev, x, t, yefrac,
					cols, rows, fg);
			}
			// end loop contents
		}
	}
}

}; // namespace anti_aliasing_pixels

#endif // _ANTI_A_LINE_H_
