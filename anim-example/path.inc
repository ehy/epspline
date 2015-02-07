/*
	 path.inc: macros for example boardspace.pov.
	 -
	 Since epspline will export a spline to a POV-Ray
	 array definition  if the type of the spline is set to
	 `undefined,' the point-data can be used in other
	 ways.  The boardspace example uses path_vec2d_rot()
	 here to get, from a position along a bezier path,
	 a set of coords and angles for translating and rotating
	 an object. This include uses cubic_bezier.inc to
	 get <u,v> vectors from interpolation.
	 -
	 Ed Hynan 2014
*/
 
 // include epspline generated cubic spline points,
// and make a path with the (experimental) POV-Ray
// `spline' object'

#ifndef ( BezierFindPos )
#	include "cubic_bezier.inc"
#end

#ifndef ( epsilon )
#declare epsilon = 1.19209290E-07;
#end

#declare epsilon_add = epsilon * 4; // arbitrary

// utility: like the AT&T V3 UNIX, C99, POSIX.1-2001
// math lib hypot(3)
#macro pthHypot(px, py)
	#local _hy = sqrt(px * px + py * py);
	_hy
#end
// utility: length of line with an endpoint at 0,0
#macro pthLineLen(px, py)
	#local len = pthHypot(px, py);
	len
#end
// utility: length of line with endpoints at u,v vecs v1 and v2
#macro pthLineLenUV(v1, v2)
	#local len = pthLineLen(v2.v - v1.v, v2.u - v1.u);
	len
#end

// 2D angle of segment between 2 2D vectors;
// result in arctangent 4 quadrant range
// return in units of degrees, as POV rotate transform uses degrees
#ifndef ( rotYAngle )
#macro rotYAngle(ov, nv)
	// wanted: angle of segment between two u,v vectors
	#local _x = nv.u - ov.u;
	#local _y = nv.v - ov.v;
	#local tr = atan2(_y, _x);
	#local ta = degrees(tr);
	// result
	ta
#end
#end // rotYAngle

#ifndef ( rotZAngle )
#macro rotZAngle(ov, nv)
	rotYAngle(ov, nv)
#end
#end // rotZAngle

// get path data for current and previous indices
#declare path_vec2d_rot_ACNT = 4;
#macro path_vec2d_rot(Aobj, ixcur, ixprev, ixnum, max_path)
	#local iscl = max_path / ixnum;
	#local offs = 0;
	#local i = ixcur * iscl;
	#local ipr = ixprev * iscl;
	#if ( 1 )
		#while ( i >= max_path )
			#local i = i - max_path;
		#end
		#if ( i < 0 )
			#local i = i + max_path;
		#end
		#while ( ipr >= max_path )
			#local ipr = ipr - max_path;
		#end
		#if ( ipr < 0 )
			#local ipr = ipr + max_path;
		#end
	#end

	#local t_vec = BezierFindPos(Aobj, max_path, i   + offs);
	#local lvec  = BezierFindPos(Aobj, max_path, ipr + offs);

	#local av0 = rotYAngle(lvec, t_vec);
	#local av1 = rotYAngle(t_vec, lvec);
	#local av2 = rotZAngle(lvec, t_vec);
	#local av3 = rotZAngle(t_vec, lvec);
	array[path_vec2d_rot_ACNT] {
		< t_vec.u, av0, t_vec.v >,
		< lvec.u,  av1, lvec.v  >,
		< t_vec.u, av2, t_vec.v >,
		< lvec.u,  av3, lvec.v  >
	}
#end // path_vec2d_rot

#macro path_vec2d_rot_dist(Aobj, ixcur, ixprev, ixnum, max_path, clenArr, inc)
	#local clen = clenArr[max_path / 4 - 1];
	#local iscl = clen / ixnum;
	#local i = ixcur * iscl;
	#local ipr = ixprev * iscl;
	#local offs = i - ipr;

	#if ( 1 )
		#while ( i >= clen )
			#local i = i - clen;
		#end
		#if ( i < 0 )
			#local i = i + clen;
		#end
		#while ( ipr >= clen )
			#local ipr = ipr - clen;
		#end
		#if ( ipr < 0 )
			#local ipr = ipr + clen;
		#end
	#end

	#local t_vec = BezierFindPosDistOpti(Aobj, max_path, i  , inc, clenArr);
	#local lvec  = BezierFindPosDistOpti(Aobj, max_path, ipr, inc, clenArr);

	#local av0 = rotYAngle(lvec, t_vec);
	#local av1 = rotYAngle(t_vec, lvec);
	#local av2 = rotZAngle(lvec, t_vec);
	#local av3 = rotZAngle(t_vec, lvec);
	array[path_vec2d_rot_ACNT] {
		< t_vec.u, av0, t_vec.v >,
		< lvec.u,  av1, lvec.v  >,
		< t_vec.u, av2, t_vec.v >,
		< lvec.u,  av3, lvec.v  >
	}
#end // path_vec2d_rot_dist
