// An Oil Lamp
// Created with epspline (POV Spline Editor)

#include "textures.inc"
#include "colors.inc"

#ifndef ( UseOilLampLightSource )
#declare UseOilLampLightSource = 1;
#end
#ifndef ( UseFlame )
#declare UseFlame = 0;
#end
#ifndef ( Use_Radiosity )
#declare Use_Radiosity = 0;
#end

#ifndef ( TestLamp )
#declare TestLamp = 1;
#end

#if ( TestLamp )

#declare LampGlobeTexture = texture { Glass3 }
#declare LampBodyTexture = texture { Brass_Metal }
#declare LampKnobTexture = texture { Silver_Metal }
#declare LampFlameTexture = texture {
  pigment { Yellow }
  finish { ambient .8 diffuse .2 }
}

#end // TestLamp

#include "lamp_pse.inc"

#declare LampKnob = union {
	object {
		LampKnobBase
	}
	object {
		LampKnobShaft
	}
}

#if ( UseFlame )
#declare DoFire = 1;
#declare DoFuelLogs = 0;
#declare UseFireLightSource = UseOilLampLightSource;
#declare TestingFire = 0;
#include "fire.inc"
#end // UseFlame

#if ( UseOilLampLightSource )
#declare LampFlameLightSource = light_source {
	<LampFlame_left+((LampFlame_right-LampFlame_left)/2)
	, 0
	, LampFlame_top+((LampFlame_bottom-LampFlame_top)/2)>
	color <1.0, 1.0, 0.5>
	looks_like { LampFlame }
	fade_distance 1.7
	fade_power 1.4
}
#end // UseOilLampLightSource

#declare OilLamp = union {
	object {
		LampBody
	}
	object {
		LampHandle
		scale <1, 32.0, 1>
		rotate x*-90
	}
	object {
		LampKnob
		rotate z*90
		translate <250+105, LampBody_top+135, -25>
		//rotate y*90
	}
#if ( UseFlame )
	object {
		Fire
		scale LampFlame_height*(1/22)
		scale <1, -1, 1>
		translate <0, LampFlame_bottom+16, 0>
	}
#else // UseFlame
#if ( UseOilLampLightSource )
	object {
		LampFlameLightSource
		scale <1, 2.0, 1>
		rotate x*-90
	}
#end // UseOilLampLightSource
	object {
		LampFlame
		scale <1, 2.0, 1>
		rotate x*-90
		no_shadow
	}
#end // UseFlame
	object {
		LampGlobe
	}
	scale 1/LampBody_bottom
	scale <1, -1, 1>
	translate <0, 1, 0>
}

#if ( TestLamp )
global_settings{
	max_trace_level 7
#if ( Use_Radiosity )
	radiosity {
		brightness 1.2
	}
#end // Use_Radiosity
}

camera {
  location <0, .5, -1.25>
  look_at <0, 0.5, 0>
  right x * image_width / image_height
}

light_source { <10, 10, -20> color rgb<1, 1, 1> }

object { OilLamp }

box { <-.75, 0, .75> <.75, 1.5, .85>
	texture { Rosewood scale .25 }
}
box { <-.75, 0, .75> <.75, -.1, -.75>
	texture { Blood_Marble scale .25 }
}
#end
