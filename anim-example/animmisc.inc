/*
	 animmisc.inc.
	 -
	 a few misc support items for animation; included
	 for the Epspline boardspace.pov example.
	 -
	 Ed Hynan 2014
*/

// Frame rates; declare more as necessary for any source video
#declare FILM_RATE = 24;
#declare PAL_RATE  = 25;
#declare NTSC_RATE = 30000 / 1001;
// MUST set animation target frame rate. Do not forget!
// film 24, PAL 25, NTSC 30000/1001 (29.97...), etc
// update: rates read from generated file as 'rate_numerator','rate_denominator'
#declare FRAME_RATE = rate_numerator / rate_denominator;
// for animated image maps, integers to select from numbered files,
// scaled to drop or duplicate per rate difference
#declare IMAP_SELECTOR_FILM = int(CNTF * (FILM_RATE / FRAME_RATE));
#declare IMAP_SELECTOR_PAL  = int(CNTF * (PAL_RATE  / FRAME_RATE));
#declare IMAP_SELECTOR_NTSC = int(CNTF * (NTSC_RATE / FRAME_RATE));

#declare ClockStart    = 0.0;
#declare ClockEnd      = 1.0;
#declare curclock = ClockStart+(ClockEnd-ClockStart)*clock;
// for subset_curclock see comment at declaration of T_FIRST and T_LAST;
// this is probably not generally useful unless a subset of a render
// is meant as a complete section
#declare subset_curclock = T_FIRST+(T_LAST-T_FIRST)*clock;

// macro ampli_y_4: for sine wave of radius 1,
// with cycles 'cycles' over time 'time',
// return y-value for current time 'curt',
// using  cylce offset 'offs' (in degrees)
#macro ampli_y_4(cycles, time, curt, offs)
	#local curdeg = (360 * cycles * (curt / time)) + offs;
	sin(radians(curdeg))
#end

// macro ampli_y_2: like ampli_y_4(cycles, time, curt, offs),
// but taking only arguments 'cycles' and 'offs',
// and using (ClockEnd-ClockStart) and curclock
#macro ampli_y_2(cycles, offs)
	ampli_y_4(cycles, (ClockEnd-ClockStart), curclock, offs)
#end

// macro ampli_y: like ampli_y_2(cycles, offs),
// but taking only arguments 'cycles',
// and using offset 0
#macro ampli_y(cycles)
	ampli_y_2(cycles, 0)
#end

#switch ( TOTF )
#case ( 1800 )
	#declare mult1 = 2;
#break
#case ( 2700 )
	#declare mult1 = 3;
#break
#case ( 3600 )
	#declare mult1 = 4;
#break
#else
	#declare mult1 = 1;
#end
#declare multclock1 = curclock * mult1;

#switch ( CURF )
#range (1, 900)
	#declare nseq = 1;
#break
#range (901, 1800)
	#declare nseq = 2;
#break
#range (1801, 2700)
	#declare nseq = 3;
#break
#range (2701, 3600)
	#declare nseq = 4;
#break
#else
	#declare nseq = 1;
#end

#macro loc_adj2(cycles, offs)
	ampli_y_4(cycles, (ClockEnd-ClockStart), multclock1, offs)
#end
#macro loc_adj1(cycles)
	loc_adj2(cycles, 0)
#end
