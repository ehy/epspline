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
#include "splinepath.inc"

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
	#local iu = vec.u + (-Path_1_left - Path_1_width / 2);
	#local iv = vec.v + (-Path_1_top - Path_1_height / 2);
	< iu * Path_scale / Path_1_max_extent, -iv * Path_scale / Path_1_max_extent >
#end

// Make the spline from the array (using above macro)
#declare SplinePath_1 =
spline {
	cubic_spline
	#local ix = 0;
	#while ( ix < Path_1_ARRAY_COUNT )
		ix , mk_spline_point(Path_1[ix])
		#local ix = ix + 1;
	#end
}

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
#declare r_ang = -90;
#declare bcup = 0.75;
#declare ecup = Path_1_ARRAY_COUNT - 1.0;
#declare ncup = 1111;
#declare incr = (ecup - bcup) / ncup;
#declare spc = 1.601;
#declare ix = incr;
#declare lvec = SplinePath_1(ix);
#while ( ix <= ecup )
	#local t_vec = SplinePath_1(ix);
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

