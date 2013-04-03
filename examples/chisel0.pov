
#include "colors.inc"
#include "metals.inc"
#include "woods.inc"
#include "textures.inc"
#include "skies.inc"

global_settings {
	max_trace_level 12
}

// Declare textures before including epspline .inc file.
#declare ChiselBladeTexture = texture { T_Chrome_2C }

#declare GrindTexture = texture {
	gradient x
	texture_map {
		[0.001 T_Chrome_2C]
		[0.1 T_Chrome_1C]
		[0.15 T_Chrome_2C]
		[0.2 T_Chrome_3C]
		[0.25 T_Chrome_1C]
		[0.3 T_Chrome_2C]
		[0.35 T_Chrome_3C]
		[0.4 T_Chrome_1C]
		[0.45 T_Chrome_2C]
		[0.5 T_Chrome_1C]
		[0.55 T_Chrome_2C]
		[0.6 T_Chrome_3C]
		[0.65 T_Chrome_2C]
		[0.7 T_Chrome_1C]
		[0.75 T_Chrome_3C]
		[0.8 T_Chrome_2C]
		[0.85 T_Chrome_1C]
		[0.9 T_Chrome_3C]
		[0.95 T_Chrome_1C]
	}
}

#declare TangFormTexture = texture {
	GrindTexture
// Hack: numbers copied from .inc file, which follows
#local Hack_width = 185.000000;
#local Hack_height = 465.000000;
#local Hack_left = 385.000000;
#local Hack_top = 0.000000;
	scale <Hack_width / 3.1925, 2, Hack_height>
	translate <Hack_left, 0, Hack_top+Hack_height/2>
}

#declare GrindFormTexture = texture {
	GrindTexture 
// Hack: numbers copied from .inc file, which follows
#local Hack_width = 390.000000;
#local Hack_height = 415.000000;
#local Hack_left = 400.000000;
#local Hack_top = 540.000000;
	scale <Hack_width / 3.38725, 2, Hack_height>
	translate <Hack_left+Hack_width/2, 0, Hack_top+Hack_height/2>
}

#declare ChiselHandleTexture = texture {
	T_Wood7
	finish  {
		specular 0.3
		roughness 0.007
	}
	rotate x*90
	rotate y*9
	rotate z*3
	scale 35
	translate <7,5,4>
}

#declare ChiselFerruleTexture = texture {
	T_Brass_5C
}

// After textures include epspline .inc file.
#include "chisel.inc"

camera {
	location <0, 2, -3>
	look_at  <0, 0,  0>
	right x * (image_width / image_height)
}

light_source { <-2, 4, -3> color White }
light_source { <-30, 40, -33> color White }

#declare blade_unit_raw = object {
	ChiselBladeProfile
	translate <
		-ChiselBladeProfile_left-(ChiselBladeProfile_width/2)
		, 0
		, -ChiselBladeProfile_top-(ChiselBladeProfile_height/2)
	>
	scale <1.0,1.75*ChiselHandle_height,1.0>
	translate y*-1.75*ChiselHandle_height
}

#declare blade_unit_tang_cutter = object {
	TangForm
	translate <
		-TangForm_left-TangForm_width
		, 0
		, -TangForm_top-TangForm_height
	>
	scale <1, ChiselBladeProfile_width, -1>
	rotate x*-90
	translate <-(ChiselBladeProfile_width/6.6667)
		,-TangForm_height*.99
		,0>
}

#declare blade_unit_tang_cut = difference {
	object { blade_unit_raw }
	object {
		blade_unit_tang_cutter
		rotate y*00
	}
	object {
		blade_unit_tang_cutter
		translate x*ChiselBladeProfile_height/2.5
		rotate y*90
	}
	object {
		blade_unit_tang_cutter
		rotate y*180
	}
	object {
		blade_unit_tang_cutter
		translate x*ChiselBladeProfile_height/5
		rotate y*270
	}
}

#declare blade_unit_grind_cutter = object {
	GrindForm
	translate <
		-GrindForm_left-GrindForm_width/2
		, 0
		, -GrindForm_top-GrindForm_height
	>
	scale <1, GrindForm_width*1.5, -1>
	rotate y*180
	rotate x*-15
	translate <0,-2.0*(ChiselHandle_height+GrindForm_height*1.0),0>
}

#declare blade_unit_grind_tang_cut = difference {
	object { blade_unit_tang_cut }
	object { blade_unit_grind_cutter }
}

#declare blade_unit = object {
	blade_unit_grind_tang_cut
	scale 1.5/ChiselBladeProfile_width
		* (ChiselHandle_width/2)/ChiselHandle_height
	translate y*-.42
}

#declare handle_unit_raw = object {
	ChiselHandle
	translate <
		0
		, -ChiselHandle_top-(ChiselHandle_height/2)
		, 0
	>
	scale -1/ChiselHandle_height
}

#declare imprint_unit_raw = object {
	Imprint0
	translate <-Imprint0_left, 0, -Imprint0_top>
}

#declare imprint_unit_sub_union = union {
	object {
		imprint_unit_raw
	}
	object {
		imprint_unit_raw
		scale <-1, 1, 1>
	}
}

#declare imprint_unit_union = union {
	object {
		imprint_unit_sub_union
	}
	object {
		imprint_unit_sub_union
		scale <1, 1, -1>
	}
}

#declare imprint_unit = object {
#local unit_xz = .25/Imprint0_height/5;
	imprint_unit_union
	scale <unit_xz, .3, unit_xz>
	rotate <90, 0, 0>
}

#declare ferrule_cut_unit = object {
	ChiselFerruleChamfer
	translate <
		0
		, -ChiselFerruleChamfer_top-(ChiselFerruleChamfer_height/2)
		, 0
	>
	scale -1/ChiselHandle_height
}

#declare handle_unit = difference {
	object { handle_unit_raw }
	object {
		ferrule_cut_unit
		translate <0, -.45, 0>
	}
	object {
		imprint_unit
		rotate y*0
		translate <0, 0, 1/ChiselHandle_width>
	}
	object {
		imprint_unit
		rotate y*180
		translate <0, 0, -1/ChiselHandle_width>
	}
	object {
		imprint_unit
		rotate y*90
		translate <1/ChiselHandle_width, 0, 0>
	}
	object {
		imprint_unit
		rotate y*-90
		translate <-1/ChiselHandle_width, 0, 0>
	}
}

#declare ferrule_unit = object {
	ChiselFerrule
	translate <
		0
		, -ChiselFerrule_top-(ChiselFerrule_height/2)
		, 0
	>
	scale -1/ChiselHandle_height
}

#declare handle_and_ferrule = union {
	object { handle_unit }
	object {
		ferrule_unit
		translate <0, -.45, 0>
	}
}

#declare Chisel0 = union {
	object {
		handle_and_ferrule
		translate y*.45
	}
	object {
		blade_unit
		translate y*.45
	}
}

// Place some Chisels
object {
	Chisel0
	rotate y*-90
	rotate z*-90
	scale 2
	translate <.0, 1.0, .75>
}

object {
	Chisel0
	rotate y*180
	//rotate x*165
	rotate x*180
	scale 2
	translate <.25, -0.55, 0>
}

object {
	Chisel0
	//rotate x*-15
	scale 2
	translate <-.25, .55, 0>
	rotate x*180
}

object {
	Chisel0
	scale 2
	translate <.75, 0, 0>
	rotate x*-90
	translate <0, 0, .2>
}

object {
	Chisel0
	scale 2
	translate <-.75, 0, 0>
	rotate x*90
	translate <0, 0, .0>
}

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

