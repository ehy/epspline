// bezier interpolated curve functions and macros
// Ed Hynan -- 2014
//
// Convention: functions in all lower case, macros
// mixed case with initial capital
//

#include "userfunc.inc"

#declare bezier_interp_factor_0 = function(b) { cub(b) }
#declare bezier_interp_factor_1 = function(b, a) { 3 * squ(b) * a }
#declare bezier_interp_factor_2 = function(b, a) { 3 * b * squ(a) }
#declare bezier_interp_factor_3 = function(a) { cub(a) }
#macro BezierInterpolationFactors(a, b)
	<
	bezier_interp_factor_0(b),
	bezier_interp_factor_1(b, a),
	bezier_interp_factor_2(b, a),
	bezier_interp_factor_3(a)
	>
#end

// macro for internal interpolation loop
// to calculate 2d <u,v> coords of point
// at interpolation increment 'i' --
// A, B, C, D are segment end and control
// points, as <u,v> coords
#macro BezierInterpAtIncr(A, B, C, D, i)
	#local a = i;
	#local b =  1 - a;
	#local c = BezierInterpolationFactors(a, b);
	<
	A.u * c.x + B.u * c.y + C.u * c.z + D.u * c.t,
	A.v * c.x + B.v * c.y + C.v * c.z + D.v * c.t
	>
#end

// this can be used much like the spline{} object:
// calculate a <u,v> coord from Bezier points
// in array Arr with Cnt members for position
// along path Pos, where 0<=Pos<Cnt
// Cnt is currently unused
#macro BezierFindPos(Arr, Cnt, Pos)
	#local ipos = int(Pos);
	#local mpos = mod(ipos, 4);
	#local seg  = ipos - mpos;
	#local i = (mpos + (Pos - ipos)) / 4;
	BezierInterpAtIncr(
		Arr[seg + 0],
		Arr[seg + 1],
		Arr[seg + 2],
		Arr[seg + 3],
		i)
#end

// place objects along a cubic bezier path:
// Arr is an array with a member count that is
// a multiple of 4, since each set of 4 <u,v>
// members compose one bezier segment;
// Cnt is the number of members in Arr;
// Inc is the the increment value for the
// interpolation from 0 to 1;
// Obj is SDL that can be placed within
// and object {} statement with a translation
#macro BezierPlaceObj(Arr, Cnt, Inc, Obj)
	#local i = 0;
	#while ( i < Cnt )
		#local tr = BezierFindPos(Arr, Cnt, i);
		object {
			Obj
			translate <tr.u, 0, tr.v>
		}
		#local i = i + Inc;
	#end
#end
// alternative, more like a usual bezier spline algorithm,
// where increment Inc applies to the interpolation
// of each segment
// if this is invoked with same args as BezierPlaceObj above,
// except with the Inc arg at 1/4 the Inc passed to
// BezierPlaceObj, the results are the same -- speed
// and memory comparison tests would be interesting
#macro BezierPlaceObj2(Arr, Cnt, Inc, Obj)
	#local j = 0;
	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = 0;
		#while ( i <= 1 )
			#local tr = BezierInterpAtIncr(A, B, C, D, i);
			object {
				Obj
				translate <tr.u, 0, tr.v>
			}
			#local i = i + Inc;
		#end
		#local j = j + 4; // bezier: 4 per segment
	#end
#end

