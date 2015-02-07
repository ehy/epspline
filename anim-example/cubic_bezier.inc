// bezier interpolated curve functions and macros
// Ed Hynan -- 2014
//

#declare bzsqua  = function(_num) { _num * _num }
#declare bzcube  = function(_num) { _num * _num * _num }
#declare bzhypot = function(_y, _x) {
	sqrt(bzsqua(_y) + bzsqua(_x))
}
#macro bzhypuv(p1, p0)
		bzhypot(p1.u - p0.u, p1.v - p0.v)
#end

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

// This macro finds indices into bezier point array Arr,
// each being the start of a point quad that when interpolated
// increases the curve total length to a value, which is stored
// in the array at that indice --
// IOW an array of max lengths to optimize finding a point <u,v>
// for a number less than the curve total length
#macro BezierFindLengthArray(Arr, Cnt, Inc)
	BezierFindLengthArrayCo(Arr, Cnt, Inc, 3)
#end
#macro BezierFindLengthArrayCo(Arr, Cnt, Inc, _const)
	#local TotalLen = 0;
	#local result = array[Cnt / 4];
	#local ji = 0;
	#local j = 0;

	#local A = Arr[j + 0];
	#local B = Arr[j + 1];
	#local C = Arr[j + 2];
	#local D = Arr[j + 3];
	#local LastPt = BezierInterpAtIncrCo(A, B, C, D, 0, _const);

	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = Inc;

		#while ( i <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, i, _const);
			#local d = bzhypuv(tr, LastPt);
			#local LastPt = tr;
			#local TotalLen = TotalLen + d;
			#local i = i + Inc;
		#end

		#local result[ji] = TotalLen;
		#local ji = ji + 1;
		#local j = j + 4; // bezier: 4 per segment
	#end
	result
#end

// This macro finds approximate total length of a curve
// interpolated from an array of bezier segments.
// The 'Inc' increment argument will affect accuracy
// of result.
#macro BezierFindLength(Arr, Cnt, Inc)
	BezierFindLengthCo(Arr, Cnt, Inc, 3)
#end
#macro BezierFindLengthCo(Arr, Cnt, Inc, _const)
	#local TotalLenA = BezierFindLengthArrayCo(Arr, Cnt, Inc, _const);
	#local TotalLen = TotalLenA[dimension_size(TotalLenA,1) - 1];
	TotalLen
#end

// this can be used much like the spline{} object:
// calculate a <u,v> coord from Bezier points
// in array Arr with Cnt members for position
// along path Pos, where 0<=Pos<Cnt
//
// the form with suffix 'Co' allows diddling
// with the constant (3) used in the evaluation
// of the control point terms in the bezier
// interpolation.
#macro BezierFindPos(Arr, Cnt, Pos)
	BezierFindPosCo(Arr, Cnt, Pos, 3)
#end
#macro BezierFindPosCo(Arr, Cnt, Pos, _const)
	#local ipos = int(Pos);
	#local pos  = mod(ipos, Cnt);
	#local mpos = mod(pos, 4);
	#local seg  = pos - mpos;
	#local i = (mpos + (Pos - ipos)) / 4;
	BezierInterpAtIncrCo(
		Arr[seg + 0],
		Arr[seg + 1],
		Arr[seg + 2],
		Arr[seg + 3],
		i, _const)
#end

// like BezierFindPos above but attempts even distribution;
// see comment at BezierSpaceNumObj (below) for details.
// *Very* slow for small Inc (e.g., 0.01 or less)
#macro BezierFindPosDist(Arr, Cnt, Pos, Inc)
	BezierFindPosDistCo(Arr, Cnt, Pos, Inc, 3)
#end
#macro BezierFindPosDistCo(Arr, Cnt, Pos, Inc, _const)
	#local tr = <0, 0>;
	#local CurLen = 0;
	#local j = 0;

	#local A = Arr[j + 0];
	#local B = Arr[j + 1];
	#local C = Arr[j + 2];
	#local D = Arr[j + 3];
	#local LastPt = BezierInterpAtIncrCo(A, B, C, D, 0, _const);

	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = Inc;

		#while ( i <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, i, _const);
			#local d = bzhypuv(tr, LastPt);
			#local LastPt = tr;
			#local CurLen = CurLen + d;
			#if ( CurLen >= Pos )
				#local i = 1;
				#local j = Cnt;
			#end
			#local i = i + Inc;
		#end

		#local j = j + 4;
	#end
	// result: not valid if invoke with wrong parameters
	tr
#end

// like BezierFindPosDist above but but takes additional arg
// Opti, a search optimization array previously made by
// BezierFindLengthArray (above)
#macro BezierFindPosDistOpti(Arr, Cnt, Pos, Inc, Opti)
	BezierFindPosDistOptiCo(Arr, Cnt, Pos, Inc, Opti, 3)
#end
#macro BezierFindPosDistOptiCo(Arr, Cnt, _Pos, Inc, Opti, _const)
	#local Olen = Cnt / 4;
	#local TotLen = Opti[Olen - 1];
	#local CurLen = 0;
	#local Pos = _Pos;
	#while ( Pos > TotLen )
		#local Pos = Pos - TotLen;
	#end
	#local LastPt = <Arr[0].u, Arr[0].v>;

	#local j0 = 0;
	#local j = 0;

	// Find desired Pos in optimization array, and
	// the indice indicates which segment point set in
	// the bezier array should be used
	#while ( j < Olen )
		#if ( Opti[j] >= Pos )
			#if ( j > 0 )
				#local CurLen = Opti[j - 1];
			#else
				#local CurLen = 0;
			#end
			#local j0 = j * 4;
			#local j = Olen; // breaker
		#end

		#local j = j + 1;
	#end

	// pre loop: iter max -- need a judicious value
	#local inc    = Inc;
	#local intinc = max(0.1, inc);
	#local j = j0;

	#local A = Arr[j + 0];
	#local B = Arr[j + 1];
	#local C = Arr[j + 2];
	#local D = Arr[j + 3];
	#local LastPt = BezierInterpAtIncrCo(A, B, C, D, 0, _const);

	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = inc;
		#local k = intinc;
		#local d = CurLen;
		#local tpt = LastPt;

		// pre- loop with larger increment:
		// big speed gain for small Inc arg
		#while ( k <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, k, _const);
			#local dd = d + bzhypuv(tr, LastPt);
			#if ( dd >= Pos )
				#local i = k - intinc;
				#local CurLen = d;
				#local LastPt = tpt;
				#local k = 1;
			#else
				#local i = k;
				#local CurLen = dd;
				#local LastPt = tr;
			#end
			#local tpt = tr;
			#local d = dd;
			#local k = k + intinc;
		#end

		// final find loop with accuracy of Inc argument,
		// starting where pre- loop above broke off
		#while ( i <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, i, _const);
			#local d = bzhypuv(tr, LastPt);
			#local CurLen = CurLen + d;
			#if ( CurLen >= Pos )
				#local i = 1;
				#local j = Cnt;
			#end
			#local LastPt = tr;
			#local i = i + inc;
		#end

		#local j = j + 4;
	#end
	// result: not valid if invoked with wrong parameters
	tr
#end

// place objects along a cubic bezier path:
// Arr is an array with a member count that is
// a multiple of 4, since each set of 4 <u,v>
// members compose one bezier segment;
// Cnt is the number of members in Arr;
// Inc is the the increment value for the
// interpolation from 0 to 1;
// Obj is SDL that can be placed within
// an object {} statement with a translation
//
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
	#local TotalLen = BezierFindLengthCo(Arr, Cnt, Inc, _const);
	// Space: distance between Obj placement; denominator is
	// fudged slightly to increase Space so that errors in
	// float arithmetic do no allow one additional object
	// before CurLen/Dist comparison fails -- fudge value is
	// arbitrary, but should not have perceptible effect in image.
	#local Space = TotalLen / (Num - 0.01); // distance 'tween
	#local Dist = 0;
	#local CurLen = 0;
	#local j = 0;

	#local A = Arr[j + 0];
	#local B = Arr[j + 1];
	#local C = Arr[j + 2];
	#local D = Arr[j + 3];
	#local LastPt = BezierInterpAtIncrCo(A, B, C, D, 0, _const);

	#while ( j < Cnt )
		#local A = Arr[j + 0];
		#local B = Arr[j + 1];
		#local C = Arr[j + 2];
		#local D = Arr[j + 3];
		#local i = Inc;

		#while ( i <= 1 )
			#local tr = BezierInterpAtIncrCo(A, B, C, D, i, _const);
			#local d = bzhypuv(tr, LastPt);
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
