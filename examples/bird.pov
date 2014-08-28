// Epspline prism and lathe editor sample file portraying
// a marquetry project, not finished.  Don't be harsh on the
// design, it is a mere doodle by a non-artist, but it should
// show an application of POV-Ray prism and lathe objects that
// might be used nicely be a real artist.
//
// 12-2013 Ed Hynan

// POV-Ray 3.7 makes some new demands, among which are #version
// as a first statement, and an expicit assumed_gamma
#version version;
#if ( version >= 3.7 )
#if ( version > 3.7 )
#version 3.7;
#end
global_settings{ assumed_gamma 1.0 }
#else // 3.7
// FPO. This is the 1st sample made w/ POV 3.7; attempting
// to get similar results w/ 3.6 is a matter for another
// time -- gamma does not do it.
global_settings{ assumed_gamma 2.2 }
#end

#include "colors.inc"
#include "metals.inc"
#include "woods.inc"
#include "textures.inc"

camera {
	location <0, 1.75, -0.25>
	//location <0, 0.25, -1.75>
	look_at <0, 0.25, 0>
	//look_at <0, 04, 0>
	right x * image_width / image_height
	angle 45
}

#declare LightOffs = 10; // 5; // or 10 or 20
// light source iff no lamp
light_source {
	<-LightOffs, LightOffs, -LightOffs * 1.5>
	color White
	area_light <5, 0, 0>, <0, 0, 5>, 5, 5
	adaptive 1
	jitter
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

#end

// Optionally place a lamp or two -- mostly for
// some reflection on chisel blades, so mostly --
// possibly all -- outside of view.
// Set 0 for now: not getting what I wanted, no more time
#ifndef ( UseLamps )
#declare UseLamps = 0;
#end

#if ( UseLamps )

#declare Lamps_as_include = 1;
#declare UseOilLampLightSource = 1;
#declare UseFlame = 0;
#declare TestLamp = 0;
#declare UseLampLightSource = 1;
#declare LampGlobeTexture = texture { Glass3 }
//#declare LampGlobeTexture = texture { pigment { color Red } }
#declare LampBodyTexture = texture { Brass_Metal }
#declare LampKnobTexture = texture { Silver_Metal }
#declare LampFlameTexture = texture {
	pigment { Yellow }
	finish { ambient .8 diffuse .2 }
}

#include "lamp.pov"

object {
	OilLamp
	scale 1
	translate < -0.0, 0.0, -2.25 >
	rotate y * 15
}

#end // Lamp

#ifndef ( UseWalls )
#declare UseWalls = 1;
#end

// Ceiling and walls; not seen directly, but just something
// for reflective surfaces.
#declare UseBrickWall = UseChisels;
#if ( UseBrickWall )
#declare BrickWallDistance = LightOffs * 1.5 + 5;
#declare BrickWallSize = 1.251;
#declare BrickWallAngle = -20;

#declare BrickWallTexture = texture {
	pigment {
		brick Gray60, <136/255, 75/255, 75/255>
		brick_size <8,3,4.5>*BrickWallSize mortar .5*BrickWallSize
	}
	normal {
		//brick 0.5
		brick normal {
			granite 0.125
		}, normal {
			average
			normal_map {
				[0.3  granite .35]
				[0.8  dents .3]
			}
		}
		brick_size <8,3,4.5>*BrickWallSize mortar .5*BrickWallSize
	}
	finish {
		diffuse 0.4
		ambient 0.30
	}
}

#macro BrickWalls_put(num)
	#local cnt = 0;
	#local ang = 360 / num;
	#while ( cnt < num )
		plane { z, BrickWallDistance
			texture {
				BrickWallTexture
			}
			hollow
			rotate y * (BrickWallAngle + ang * cnt)
		}
		#local cnt = cnt + 1;
	#end
#end // #macro BrickWalls_put(num)

BrickWalls_put(4)

#declare CeilingScale = BrickWallDistance * 2;
#declare CeilingAngle = BrickWallAngle;

// simple ceiling for reflection
plane { +y, LightOffs + 25
	texture {
		pigment {
			checker color Tan, color Thistle scale 0.10
		}
		scale <CeilingScale, 1, CeilingScale>
	}
	hollow
	rotate y * CeilingAngle
}

#end // Walls

