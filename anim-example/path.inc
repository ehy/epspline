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

// Like atan2, with extra arbitrary handling of adjacent == 0,
// and returning only postive coincident angles when atan2
// returns negative
#macro p_atan2(o, a)
	// take angle as if segment is in 1st quadrant
	#local tt = 0;
	#if ( a != 0 )
		#local tt = atan2(o, a);
	#end
	#if ( tt < 0 )
		#local tt = pi * 2 + tt;
	#end
	tt
#end

// 2D angle of segment between 2 2D vectors;
// result in arctangent 4 quadrant range
// return in units of degrees, as POV rotate transform uses degrees
#ifndef ( rotYAngle )
#macro rotYAngle(ov, nv)
	// wanted: angle of segment between two u,v vectors
	#local tu = nv.u - ov.u;
	#local tv = nv.v - ov.v;
	#if ( 1 )
	#local tr = atan2(tv, tu);
	#else
	#local tr = p_atan2(tv, tu);
	#end // if ( 1 )
	#local ta = degrees(tr);
	// result
	ta
#end
#end // rotYAngle


// 2D angle of segment between 2 2D vectors;
// result in arcsine half cycle range
// return in units of degrees, as POV rotate transform uses degrees
#ifndef ( rotZAngle )
#macro rotZAngle(ov, nv)
	// wanted: angle of segment between two u,v vectors
	#local tu = nv.u - ov.u;
	#local tv = nv.v - ov.v;
	#local th = sqrt(tu * tu + tv * tv);
	#if ( 0 )
		#if ( tu )
		#local tr = acos(tu / th);
		#else // if ( t? )
		#local tr = 0;
		#end // if ( t? )
	#else // if ( 1|0 )
		#if ( th )
		#local tr = asin(tv / th);
		#else // if ( t? )
		#local tr = 0;
		#end // if ( t? )
	#end // if ( 1 )
	#local ta = degrees(tr);
	// result
	ta
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
		#if ( i > max_path )
			#local i = i - max_path;
		#end
		#if ( i < 0 )
			#local i = i + max_path;
		#end
		#if ( ipr > max_path )
			#local ipr = ipr - max_path;
		#end
		#if ( ipr < 0 )
			#local ipr = ipr + max_path;
		#end
	#end
	#local teps = abs(i) - int(abs(i));
	#if ( teps < epsilon )
		#local i = i + select(i, epsilon_add, -epsilon_add);
	#end
	#local teps = abs(ipr) - int(abs(ipr));
	#if ( teps < epsilon )
		#local ipr = ipr + select(ipr, epsilon_add, -epsilon_add);
	#end

	#local t_vec = BezierFindPos(Aobj, 1, i + offs);
	#local lvec = BezierFindPos(Aobj, 1, ipr + offs);

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
