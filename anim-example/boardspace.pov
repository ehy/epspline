/*
	 boardspace.pov.
	 -
	 Since epspline will export a spline to a POV-Ray
	 array definition if the type of the spline is set to
	 `undefined,' the point-data can be used in other
	 ways.  The boardspace example makes a random boardlike
	 thing, and moves an object among obstacles on the board.
	 -
	 Works like this on Unix:
    	 Invoke, if neeeded, the "traceimage" target in the
    	 (GNU make) GNUMakefile, open boardspace-path.pse with
    	 Epspline and check/adjust bezier path. Next run make
    	 (or gmake) with no target and check the available
    	 targets near the top of the help output (try LDdevel
    	 first), peruse the remainder of help text, and try
    	 something like this:
    	 -
    	 time make PREVIEW=true ANI_FILM=true ENCODE_TYPE=ntsc-film \
    		AUDIO_ARGS="-an" MULTIJOBS=true \
    		ffLDdevel
    	 -
    	 Note the target is "ffLDdevel" which will finish with
    	 ffmpeg encoding -- if you don't have ffmpeg just use
    	 "LDdevel"
    	 -

	 Works like this on MSWindows:
	    1) select menu Render->Edit Settings/Render, and in the
	     "INI file" section of the dialog make sure ANIM_MSW.INI,
	     included with this example, will be used and click "Save".
	    2) open the aforementioned ANIM_MSW.INI in the POV-Ray
	     editor, switch to that tab, and READ THE COMMENTS, esp.
	     comments re. "IO and anim opts" and Output_File_Name;
	     because, if you know how to make Pre_Scene_Command
	     work the render can be made more flexible and subdirs
	     can be used for generated files. (I cannot get
	     Pre_Scene_Command to work if the command is given any
	     options at all -- YMMV.)
	    3) invoke, if neeeded, the "traceimage" section in 
    	 ANIM_MSW.INI, open boardspace-path.pse with
    	 Epspline and check/adjust bezier path.
    	4) select a size-named section, which are all for animation,
    	 and run.
    	 -
    	 Finally if you wish, compile the frames into a video with
    	 your preferred tool.
    	 -

	 -
	 Ed Hynan 2015
*/

// should work with 3.6 or 3.7
//#version 3.7;
#version 3.6;

// Source (pixel) aspect
#declare aspect = image_width / image_height;

#include "colors.inc"

//
// animation related setup
//

#declare IS_ANIM = clock_on;
#if ( IS_ANIM )
#	ifdef ( NO_GEN_FILES )
#	declare FFST = 1;
#	declare NF = 900;
#	declare TESTING = 0;
#	declare SINGLE  = 0;
#	declare IsFinal = 1;
#	declare rate_numerator = 24;
#	declare rate_denominator = 1;
#	declare DAR_x = image_width;
#	declare DAR_y = image_height;
#	else
	// values from data-file: Makefile genterated or hand edited, etc..
#	fopen DATF "gen/dat0" read
#	read(DATF, FFST, NF, TESTING, SINGLE)
#	fclose DATF
#	fopen DATF "gen/dat1" read
#	read(DATF, IsFinal, rate_numerator, rate_denominator, DAR_x, DAR_y)
#	fclose DATF
#	end

	// Display aspect:
#	declare DAR = DAR_x / DAR_y;

#	declare TOTF = NF - FFST + 1;
#	declare CURF = frame_number;
#	declare CNTF = CURF - FFST + 1;
#	declare T_TWEEN = clock_delta;
	// NOTE: initial_clock and final_clock and
	// final_frame and initial_frame yield numbers for
	// the *subset* of a render (+SFn, +EFn), and only match +KIn and +KFn
	// on a complete render!
#	declare T_FIRST = initial_clock;
#	declare T_LAST  = final_clock;
#	declare SUBSET_NF = final_frame - initial_frame;

	global_settings {
		assumed_gamma 1.0
		max_trace_level 12
		ambient_light 3
	}

	light_source { <0, 4, 0> color White }

#	include "animmisc.inc"

#else // #if ( IS_ANIM )

#	declare DAR = 1;

	global_settings {
		assumed_gamma 1.0
		max_trace_level 12
		ambient_light 3
	}
	
	light_source { <0, 4, 0> color White }
#end // #if ( IS_ANIM )

//
// decls and macros for the generated board
//
#declare CYLPIG = pigment { color Red }
#declare CYLTEX = texture { CYLPIG }
#declare BOXPIG = pigment { color Blue }
#declare BOXTEX = texture { BOXPIG }

// If changed, bezier path is invalid
#declare MAIN_SEED = 99;

// board rectangular division number and constants based on that
// (N_SIDE should have been N_RECT, etc.)
#declare N_SIDE = 16;
#declare S_SIDE = 1.0 / N_SIDE;
#declare OBSCAL = 0.8;
#declare B_SIDE = S_SIDE * OBSCAL;
#declare B_OMIN = (S_SIDE - B_SIDE) / 2.0;
#declare B_OMAX = S_SIDE - B_OMIN;
#declare CYCNTR = S_SIDE / 2.0;
#declare R_SIDE = (S_SIDE / 2.0) * OBSCAL;
#declare MAXHI = 0.05;
#declare MINHI = 0.0;

#macro RndScale(Robj, Lwv, Upv)
	#local RU = rand(Robj);
	#local Scl = (Upv - Lwv);
	#local RS = (RU * Scl) + Lwv;
	RS
#end

#macro RectThing(Pos, Lo, Hi)
	box {
		<B_OMIN, Lo, B_OMIN>, <B_OMAX, Hi, B_OMAX>
		translate <Pos.u, 0, Pos.v>
	}
#end
#macro CylThing(Pos, Lo, Hi)
	cylinder {
		<CYCNTR, Lo, CYCNTR>, <CYCNTR, Hi, CYCNTR>, R_SIDE
		translate <Pos.u, 0, Pos.v>
	}
#end

#macro IPutRect(Ix, Iz, Lo, Hi)
	RectThing(<Ix * S_SIDE, Iz * S_SIDE>, Lo, Hi)
#end
#macro IPutCyl(Ix, Iz, Lo, Hi)
	CylThing(<Ix * S_SIDE, Iz * S_SIDE>, Lo, Hi)
#end

#macro PutBoard(Lo, Hi, Robj)
	#local Tcnt = 16;
	#local j = 0;
	#local Cnt = N_SIDE;
	#while ( j < Cnt )
		#local i = 0;
		#while ( i < Cnt )
			#local Type = mod(int(RndScale(Robj, 0, Tcnt)), Tcnt);
			#local Top = RndScale(Robj, Lo, Hi);
			#switch ( Type )
			#case ( 0 )
				object {
					IPutRect(i, j, Lo, Top)
					texture { BOXTEX }
				}
			#break
			#range (1, 4)
				object {
					IPutCyl(i, j, Lo, Top)
					texture { CYLTEX }
				}
			#break
			#else
			#end
			#local i = i + 1;
		#end
		#local j = j + 1;
	#end
#end

#declare BoardUnit = union {
	PutBoard(MINHI, MAXHI, seed(MAIN_SEED))
	object {
		box {
			<0, 0 - R_SIDE, 0>, <1, 0, 1>
		}
		texture {
			pigment {
				color Yellow
			}
		}
	}
}

#declare BoardCtr = object {
	BoardUnit
	translate <-0.5, 0, -0.5>
}

#if ( IS_ANIM )
#	include "cubic_bezier.inc"
#	include "boardspace-path.inc"
#	include "path.inc"

#declare path_TWEEN       = T_TWEEN; // 1/(TOTF-0);
#declare pathClockStart   = ClockStart + path_TWEEN;
#declare pathClockEnd     = ClockEnd + path_TWEEN;
#declare pathclock = pathClockStart+(pathClockEnd-pathClockStart)*clock;
#declare PATH_IX = pathclock + path_TWEEN;
//#declare PATH_PREV = PATH_IX - path_TWEEN;
#declare PATH_PREV = pathclock - path_TWEEN;

// for 'banking' object tilt
#declare zangmax = 85;
#declare zangdiv = 1.2;
#declare zangmul = 1 * (TOTF / 900);
#declare zangmin = 0.01;

#macro chk_zmin(a)
	#local r = a;
	#if ( abs(r) < zangmin )
		#local r = 0;
	#end
	r
#end

// Get -- and condition -- z rotation of object to
// approximate banking for a turn. The angles used are the
// result of atan2 and since atan2 can't `spin', i.e. has
// a discontinuity near pi and -pi, using a simple difference
// make an occasional `flipped' result. The first two
// conditional blocks correct for that, imperfectly but
// suitably here. The remainder of the macro just scales and limits.
#macro get_z_rot(rt1, rt2)
	#local a1 = chk_zmin(rt1);
	#local a2 = chk_zmin(rt2);
	#if ( a2 < -90 )
		#if ( a1 > 90 )
			#local a2 = a2 + 360;
		#end
	#end
	#if ( a1 < -90 )
		#if ( a2 > 90 )
			#local a1 = a1 + 360;
		#end
	#end
	#local ang = (a1 - a2) * zangmul / zangdiv;
	#if ( abs(ang) > zangmax )
		#local ang = select(ang, -zangmax, zangmax);
	#end
	ang
#end // get_z_rot


#declare travel_rot_addl = -90;
// see path.inc
#declare traveldataAR =
	path_vec2d_rot(Path_1, PATH_IX, PATH_PREV, 1, Path_1_ARRAY_COUNT);

// scale interpolation result vectors
// Note values added to Path_1_off{x,y} -- notarbitrary:
// y gets difference 'tween board background image top and topmost
// point of drawn path (taken from epspline coord display of mouse
// position, needn't be too accurate). Likewise x, but for left.
// other constants (Path_1_{left,top,max_extent,etc}) are found
// in boardspace-path.inc exported from Epspline.
#local pthinc = 0;
#local Path_1_offx = Path_1_max_extent / 2.0 - 8;
#local Path_1_offy = Path_1_max_extent / 2.0 + 7;
#while ( pthinc < path_vec2d_rot_ACNT )
#	local Unew =
	(traveldataAR[pthinc].x - Path_1_left - Path_1_offx) / Path_1_max_extent;
#	local Vnew =
	(traveldataAR[pthinc].z - Path_1_top - Path_1_offy) / Path_1_max_extent;
#	local Anew = traveldataAR[pthinc].y * -1;
#	declare traveldataAR[pthinc] = <Unew, Anew, Vnew * -1>;
#	local pthinc = pthinc + 1;
#end

#declare traveldata   = traveldataAR[0];
#declare travel_rot_path = traveldata.y;
#declare travel_rot      = -(travel_rot_path + travel_rot_addl);
#macro mk_travelZrot(navg)
	#local ix = 0;
	#local accZ = 0;
	#local incT = path_TWEEN;
	#local T0 = PATH_IX+incT*0;
	#local T1 = PATH_IX+incT*1;
	#while ( ix < navg )
		#local r0 =
			path_vec2d_rot(Path_1, T1, T0, 1, Path_1_ARRAY_COUNT);
		#local r1 =
			path_vec2d_rot(Path_1, T1+incT, T0+incT, 1, Path_1_ARRAY_COUNT);
		#local accZ = accZ + get_z_rot(r1[0].y, r0[0].y);
		#local ix = ix + 1;
		#local T0 = T0 + incT;
		#local T1 = T1 + incT;
	#end
	#local accZ = accZ / navg;
	accZ
#end

#declare travelZrot = mk_travelZrot(6);

// the travelling thingy: a bezier prism
#	declare Travel_1_USE_NORMAL_TRANSFORM = 1;
#	include "travel.pse.inc"
#	declare Travellor_scale = (S_SIDE / 1.5);
#	declare Travellor = object {
		Travel_1
		texture {
			pigment { color Magenta }
		}
		scale Travellor_scale
	}

// see animmisc.inc
#	declare travel_height = 0.15 -
	(0.07 * ampli_y_4(8, (pathClockEnd-pathClockStart), pathclock, 60));
	
	object {
		Travellor
		// Now, the animation motion
		rotate z * travelZrot
		rotate y * travel_rot
	    translate <traveldata.x, travel_height, traveldata.z>
	}

	object {
		BoardCtr
	}

#	if ( DAR = 1 )
#		declare Camera_Y = 11.9950;
#	else
#		if ( DAR = (4 / 3) )
#			declare Camera_Y = 16.00;
#		else
#			if ( DAR = (16 / 9) )
#				declare Camera_Y = 20.80;
#			else
#				declare Camera_Y = 16.00;
#			end
#		end
#	end
	camera {
		location <0, Camera_Y, 0>
		look_at  <0, 0, 0>
		right x * DAR
		angle 5.000000
	}
#else // #if ( IS_ANIM )
	object {
		BoardCtr
		rotate x * -90
	}
	
	camera {
		location <0, 0, -12.5>
		look_at  <0, 0, 0>
		right x * DAR
		angle 5.000000
	}
#end // #if ( IS_ANIM )
