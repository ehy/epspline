// bezier interpolated curve functions and macros
// Ed Hynan -- 2014
//
// Convention: functions in all lower case, macros
// mixed case with initial capital
//

#declare bzsqua = function(_num) { _num * _num }
#declare bzcube = function(_num) { _num * _num * _num }

#declare bezier_interp_factor_0 = function(b) { bzcube(b) }
#declare bezier_interp_factor_1 = function(b, a, c) { c * bzsqua(b) * a }
#declare bezier_interp_factor_2 = function(b, a, c) { c * b * bzsqua(a) }
#declare bezier_interp_factor_3 = function(a) { bzcube(a) }
#macro BezierInterpolationFactors(a, b, _const)
	#local c = <1,1> * _const;
	<
	bezier_interp_factor_0(b),
	bezier_interp_factor_1(b, a, c.u),
	bezier_interp_factor_2(b, a, c.v),
	bezier_interp_factor_3(a)
	>
#end

// macro for internal interpolation loop
// to calculate 2d <u,v> coords of point
// at interpolation increment 'i' --
// A, B, C, D are segment end and control
// points, as <u,v> coords
// the form with suffix 'Co' allows diddling
// with the constant (3) used in the evaluation
// of the control point terms in the bezier
// interpolation.
#macro BezierInterpAtIncr(A, B, C, D, i)
	BezierInterpAtIncrCo(A, B, C, D, i, 3)
#end
#macro BezierInterpAtIncrCo(A, B, C, D, i, _const)
	#local a = i;
	#local b =  1 - a;
	#local c = BezierInterpolationFactors(a, b, _const);
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
// the form with suffix 'Co' allows diddling
// with the constant (3) used in the evaluation
// of the control point terms in the bezier
// interpolation.
#macro BezierFindPos(Arr, Cnt, Pos)
	BezierFindPosCo(Arr, Cnt, Pos, 3)
#end
#macro BezierFindPosCo(Arr, Cnt, Pos, _const)
	#local ipos = int(Pos);
	#local mpos = mod(ipos, 4);
	#local seg  = ipos - mpos;
	#local i = (mpos + (Pos - ipos)) / 4;
	BezierInterpAtIncrCo(
		Arr[seg + 0],
		Arr[seg + 1],
		Arr[seg + 2],
		Arr[seg + 3],
		i, _const)
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
// the form with suffix 'Co' allows diddling
// with the constant (3) used in the evaluation
// of the control point terms in the bezier
// interpolation.
#macro BezierPlaceObj(Arr, Cnt, Inc, Obj)
	BezierPlaceObjCo(Arr, Cnt, Inc, Obj, 3)
#end
#macro BezierPlaceObjCo(Arr, Cnt, Inc, Obj, _const)
	#local i = 0;
	#while ( i < Cnt )
		#local tr = BezierFindPosCo(Arr, Cnt, i, _const);
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
	BezierPlaceObj2Co(Arr, Cnt, Inc, Obj, 3)
#end
#macro BezierPlaceObj2Co(Arr, Cnt, Inc, Obj, _const)
	#local j = 0;
	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = 0;
		#while ( i <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, i, _const);
			object {
				Obj
				translate <tr.u, 0, tr.v>
			}
			#local i = i + Inc;
		#end
		#local j = j + 4; // bezier: 4 per segment
	#end
#end

// A noticeable problem with placing objects along
// a composite curve of several bezier segments is
// in the differing lengths of each segment; e.g.,
// the macros above use a fixed increment for each
// segment so shorter segments have more 'packed-in'
// objects, and longer segments are more sparse.
// This macro attempts to distribute objects along
// a curve composed of bezier segments, and incurs
// the cost of using more than one pass at interpolation
// and more (parse-time) calculation
// The 'Inc' increment argument will affect accuracy
// of spacing.
// Additional argument 'Num', a positive integer quantity
// of Obj to be placed.
// Also note that if composite curve is discontinuous, e.g.
// has a 'hole', appearance of spacing will be faulty at
// jump from one sub-curve to the next.
#macro BezierSpaceNumObj(Arr, Cnt, Inc, Num, Obj)
	BezierSpaceNumObjCo(Arr, Cnt, Inc, Num, Obj, 3)
#end
#macro BezierSpaceNumObjCo(Arr, Cnt, Inc, Num, Obj, _const)
	#local TotalLen = 0;
	#local LastPt = <Arr[0].u, Arr[0].v>;
	#local j = 0;
	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = 0;
		#while ( i <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, i, _const);
			#local d = sqrt(bzsqua(tr.u-LastPt.u) + bzsqua(tr.v-LastPt.v));
			#local LastPt = tr;
			#local TotalLen = TotalLen + d;
			#local i = i + Inc;
		#end
		#local j = j + 4; // bezier: 4 per segment
	#end
	// now we have a total length; make 2nd pass
	#local LastPt = <Arr[0].u, Arr[0].v>;
	// Space: distance between Obj placement; denominator is
	// fudged slightly to increase Space so that errors in
	// float arithmetic do no allow one additional object
	// before CurLen/Dist comparison fails -- fudge value is
	// arbitrary, but should not have perceptible effect in image.
	#local Space = TotalLen / (Num - 0.01); // distance 'tween
	#local Dist = 0;
	#local CurLen = 0;
	#local j = 0;
	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = 0;
		#while ( i <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, i, _const);
			#local d = sqrt(bzsqua(tr.u-LastPt.u) + bzsqua(tr.v-LastPt.v));
			#local LastPt = tr;
			#local CurLen = CurLen + d;
			#if ( CurLen >= Dist )
				object {
					Obj
					translate <tr.u, 0, tr.v>
				}
				#local Dist = Dist + Space;
			#end
			#local i = i + Inc;
		#end
		#local j = j + 4;
	#end
#end

