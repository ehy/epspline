/*
	splinepath.pov; using export from splinepath.pse.
	Sample derived from the simpler sample ``cup.pse.''
	-
	Since epspline will export a spline to a POV-Ray
	array definition  if the type of the spline is set to
	`undefined,' the point-data can be used in other
	ways.  This sample uses the data to make a
	``spline'' (distinct from prism or lathe), which
	performs like a function providing samples along
	the path. Several cups are placed using this spline.
	-
	Note that POV-Ray 3.6 warns that the ``spline'' is
	an experimental feature.
	-
	Ed Hynan 2013
 */

#version 3.5;
#include "colors.inc"

light_source { <20, 20, -20> color White }

camera {
	location <0, 45, -4>
	look_at <0, 0.25, 0>
	right x * image_width / image_height
	angle 45
}

#declare CupTexture = texture { pigment { color LightBlue } }
#declare BoxTexture = texture { pigment { color DarkGreen } }

// In splinepath.pse, from which splinepath.inc was exported,
// a cubic spline was set as an `undefined' type in the
// properties dialog (rather than prism or lathe) and so
// epspline exported the points data as an array; the name
// given (in the properties dialog) was ``Path_1''.
// This sample was based on the cup sample, and so the cup
// and handle are present too.
#include "cubic_bezier.inc"
#include "splinepath.inc"

#declare UseBezPath = 1;
#if ( UseBezPath )
	#declare ThePath = BzPath_1;
	#declare ThePath_left = BzPath_1_left;
	#declare ThePath_top = BzPath_1_top;
	#declare ThePath_width = BzPath_1_width;
	#declare ThePath_height = BzPath_1_height;
	#declare ThePath_max_extent = BzPath_1_max_extent;
	#declare ThePath_ARRAY_COUNT = BzPath_1_ARRAY_COUNT;
#else
	#declare ThePath = Path_1;
	#declare ThePath_left = Path_1_left;
	#declare ThePath_top = Path_1_top;
	#declare ThePath_width = Path_1_width;
	#declare ThePath_height = Path_1_height;
	#declare ThePath_max_extent = Path_1_max_extent;
	#declare ThePath_ARRAY_COUNT = Path_1_ARRAY_COUNT;
#end

#declare CoffeeCupRaw = union {
	object { Handle
		rotate x * -90
	}
	object { Cup }
}

#declare CoffeeCup = object {
	CoffeeCupRaw
	translate <0, -Cup_top - Cup_height / 2, 0>
	scale 1 / Cup_height
	scale <1, -1, 1>
	texture { CupTexture }
}

// The array cannot be subjected to `scale,' `translate,'
// etc. wholesale, so use a macro to transform each point.
#declare Path_scale = 27;
#macro mk_spline_point ( vec )
	#local iu = vec.u + (-ThePath_left - ThePath_width / 2);
	#local iv = vec.v + (-ThePath_top - ThePath_height / 2);
	< iu * Path_scale / ThePath_max_extent, -iv * Path_scale / ThePath_max_extent >
#end

#local curpt = 0;
#while ( curpt < ThePath_ARRAY_COUNT )
	#declare ThePath[curpt] = mk_spline_point(ThePath[curpt]);
	#local curpt = curpt + 1;
#end

#if ( UseBezPath )
#macro SplineThePath(ix)
BezierFindPos(ThePath, ThePath_ARRAY_COUNT, ix)
#end
#else
// Make the spline from the array (using above macro)
#declare SplineThePath =
spline {
	cubic_spline
	#local ix = 0;
	#while ( ix < ThePath_ARRAY_COUNT )
		ix , ThePath[ix]
		#local ix = ix + 1;
	#end
}
#end

// two u,v vectors, taken as endpoints of segment:
// get length
#macro seg_length ( v1, v2 )
	#local iu = v2.u - v1.u;
	#local iv = v2.v - v1.v;
	#local len = sqrt(iu * iu + iv * iv);
	len
#end

// two u,v vectors, taken as endpoints of segment:
// get angle
#macro seg_angle ( ov, nv )
	#local tu = nv.u - ov.u;
	#local tv = nv.v - ov.v;
	#local ang = atan2(tv, tu);
	// radians -> degrees for the `rotate' transform
	#local ang = degrees(ang);
	// result
	ang
#end

// use spline object generated above to place objects;
// also, use two consecutive points to find a rotation
// for the cups
#if ( UseBezPath )
#declare r_ang = -90;
#declare bcup = 0.75;
#declare ecup = ThePath_ARRAY_COUNT - 0.000001;
#declare ncup = 1111;
#declare incr = (ThePath_ARRAY_COUNT - bcup) / ncup;
#declare spc = 1.601;
#declare ix = 0;
#declare lvec = SplineThePath(ThePath_ARRAY_COUNT - 0.00001);
#else
#declare r_ang = -90;
#declare bcup = 0.75;
#declare ecup = ThePath_ARRAY_COUNT - 1.0;
#declare ncup = 1111;
#declare incr = (ecup - bcup) / ncup;
#declare spc = 1.601;
#declare ix = incr;
#declare lvec = SplineThePath(ix);
#end

#while ( ix <= ecup )
	#local t_vec = SplineThePath(ix);
	// Samples from spline may not be evenly spaced,
	// so excess samples are taken and an object is
	// only placed at desired distance
	#local tds = seg_length(t_vec, lvec);
	#if ( tds >= spc )
		#local rot = r_ang - seg_angle(lvec, t_vec);
		#local iu = t_vec.u;
		#local iv = t_vec.v;
		object {
			CoffeeCup
			rotate y * rot
			translate <iu, 0.5, iv>
		}
		#declare lvec = t_vec;
	#end
	#declare ix = ix + incr;
#end

// From original cup sample; left for reference
// somewhere to put the cup down:
box { <-1, -0.25, -0.75> <1, 0, 0.75>
	texture { BoxTexture }
}

