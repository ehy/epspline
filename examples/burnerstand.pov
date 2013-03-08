
#include "colors.inc"
#include "metals.inc"
#include "woods.inc"
#include "textures.inc"

// Declare textures before including epspline .inc file.
#declare BurnerStandBandTexture = texture { T_Chrome_2C }

#declare BurnerStandLegTexture = texture {
	gradient x
	texture_map {
		[0.001 T_Chrome_2A]
		[0.1 T_Chrome_1A]
		[0.15 T_Chrome_2A]
		[0.2 T_Chrome_1A]
		[0.25 T_Chrome_1A]
		[0.3 T_Chrome_2A]
		[0.35 T_Chrome_1A]
		[0.4 T_Chrome_1A]
		[0.45 T_Chrome_2A]
		[0.5 T_Chrome_1A]
		[0.55 T_Chrome_2A]
		[0.6 T_Chrome_1A]
		[0.65 T_Chrome_2A]
		[0.7 T_Chrome_1A]
		[0.75 T_Chrome_1A]
		[0.8 T_Chrome_2A]
		[0.85 T_Chrome_1A]
		[0.9 T_Chrome_1A]
		[0.95 T_Chrome_1A]
	}
}

#declare BurnerStandCupTexture = texture {
	T_Brass_5C
}

// After textures include epspline .inc file.
#include "BurnerStand_pse.inc"


#declare BurnerStand = union {
#local BLegDepth = 33.333;
	object {
		BurnerStandCup
	}
	object {
		BurnerStandLeg
		scale <1, 1*BLegDepth, 1>
		rotate x*-90
		rotate y*30
	}
	object {
		BurnerStandLeg
		scale <1, 1*BLegDepth, 1>
		rotate x*-90
		rotate y*150
	}
	object {
		BurnerStandLeg
		scale <1, 1*BLegDepth, 1>
		rotate x*-90
		rotate y*270
	}
	object {
		BurnerStandBandUpper
	}
	object {
		BurnerStandBandMiddle
	}
	object {
		BurnerStandBandLower
	}
	translate <0, -(BurnerStandLeg_bottom-BurnerStandCup_top)/2, 0>
	scale 1/(BurnerStandLeg_bottom-BurnerStandCup_top)
	scale <1, -1, 1>
}


#if ( 1 ) // testing
#include "skies.inc"

global_settings {
	max_trace_level 12
}

camera {
	location <0, 2, -3>
	look_at  <0, 0,  0>
	right x * image_width / image_height
}

//light_source { <-2, 4, -3> color White }
light_source { <-30, 40, -33> color White }

// Place some BurnerStands
object {
	BurnerStand
	scale 1.333
	scale <1, 1, 1>
	translate <-.75, 0, 0>
	rotate x*00
	translate <0, 0, .0>
}
object {
	BurnerStand
	scale 1.333
	scale <1, -1, 1>
	translate <.75, 0, 0>
	rotate x*00
	translate <0, 0, .0>
}

#declare DoFire = 1;
#if ( DoFire ) // DoFire

#declare TestingFire = 0;
#declare UseFireLightSource = 1;
#include "fire.inc"

#declare BurningStand = union {
	object {
		BurnerStand
	}
	object {
		Fire
		scale 1/17
		translate <0, .20, 0>
	}
}

object {
	BurningStand
	scale <1, 1, 1>
	translate <0, 0.5, 0>
	scale 1.75
	translate <4.0, -2.5, 6.25>
}
object {
	BurningStand
	scale <-1, 1, 1>
	translate <0, 0.5, 0>
	scale 1.75
	translate <-4.0, -2.5, 6.25>
}
#end // DoFire

plane { y, -2.5
	//texture { pigment { Red_Marble } }
	texture {
		T_Wood4
		rotate <2, 88, 0>
		scale <3, 3, 3>
	}
}

sky_sphere {
	S_Cloud2
}

#end // testing
