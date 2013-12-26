// Epspline prism and lathe editor sample file portraying
// a marquetry project, not finished.  Don't be harsh on the
// design, it is a mere doodle by a non-artist, but it should
// show an application of POV-Ray prism and lathe objects that
// might be used nicely be a real artist.
//
// 12-2013 Ed Hynan

#include "colors.inc"
#include "metals.inc"
#include "woods.inc"
#include "textures.inc"

#ifndef ( UseAreaLight )
#declare UseAreaLight = 1;
#end

#if ( UseAreaLight )
	light_source {
		<-20, 20, -30>
		color White
		area_light <5, 0, 0>, <0, 0, 5>, 5, 5
		adaptive 1
		jitter
	}
#else
	light_source { <20, 20, -20> color White }
#end

camera {
	location <0, 1.75, -0.25>
	//location <0, 0.25, -1.75>
	look_at <0, 0.25, 0>
	right x * image_width / image_height
	angle 45
}

#macro tint_textureFA(clr, mul, filt, trans, basic)
texture {
	basic
	pigment {
		color (clr * mul) + <0, 0, 0, filt, trans>
	}
}
#end

#declare wood_base_texture_1 = texture {
	T_Wood2
	finish  {
		specular 0.3
		roughness 0.007
		ambient .1
	}
	rotate <2, -02, 9>
	scale 0.1
	translate <3,1,3>
}

#declare outline_texture = 
	tint_textureFA(<.5, .28, .3>, .9, .9, .5, texture{ T_Wood7 rotate <-2, -91, 8> scale 30 })
#declare birdeye_texture =
	tint_textureFA(SlateBlue, .99, .7, .2, texture{ T_Wood7 rotate <2, 91, 3> scale 40 })
#declare face_texture =
	tint_textureFA(White, .9, .6, .2, texture{ T_Wood7 rotate <9, -39, 13> scale 55 })
#declare beak_texture =
	tint_textureFA(Yellow, .9, .6, .3, texture{ T_Wood7 rotate <13, -3, 8> scale 60 })
#declare tongue_texture =
	tint_textureFA(Red, .9, .6, .2, texture{ T_Wood7 rotate <-9, -91, -3> scale 65 })
#declare base_texture =
	tint_textureFA(Clear, .1, .9, .5, wood_base_texture_1)
#declare nostril_texture =
	tint_textureFA(Clear, .2, .6, .2, texture{ T_Wood2 rotate <-8, +89, -3> scale 35 })

#include "bird.inc"

#declare bird_raw = union {
	object { bird_outline }
	object { bird_eye }
	object { face }
	object { beak }
	object { tongue }
	object { nostril }
}

#declare bird_transform = transform {
	translate <-bird_outline_left - bird_outline_width / 2, 0, -bird_outline_top - bird_outline_height / 2>
	#local scl = 1 / bird_outline_max_extent;
	scale <scl, scl * 5, scl>
	scale <1, 1, -1>
	translate <0, -0.001, 0>
}

#declare neck_transform = transform {
	translate <-bird_outline_left - bird_outline_width / 2, 0, -bird_outline_top - bird_outline_height / 2>
	#local scl = 1 / bird_outline_max_extent;
	scale <scl, 1, scl>
	scale <1, 1, -1>
}

#declare bird_assy = object {
	bird_raw
	transform { bird_transform }
}

#declare neck_diff = object {
	bird_neck
	transform { neck_transform }
}

object { bird_assy }
// TODO: something better here
difference {
	box { <-1, -0.25, -0.75> <1, 0, 0.75>
		texture { base_texture }
	}
	object { neck_diff
		texture { base_texture }
	}
}

// Optionally place a chisel or two -- a concept thing.
#ifndef ( UseChisels )
#declare UseChisels = 1;
#end

#if ( UseChisels )
#include "skies.inc"
#declare Chisels_as_include = 1;
#include "chisel0.pov"

#declare ChiselScale = 2 / 3;

object {
	Chisel0
	scale ChiselScale
	translate <.05, 0, 0.4>
	rotate <-90, -83, +03.6>
	translate <0.1, -0.35, .35>
}

object {
	Chisel0
	scale ChiselScale
	translate <.05, 0, 0.4>
	rotate <-90, -83, +03.6>
	translate <0.0, -0.35, .0>
	rotate y * 30
	translate <-0.8, 0.0, .0>
}


sky_sphere {
	S_Cloud2
}

#end

