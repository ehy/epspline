#version 3.5;
#include "colors.inc"

light_source { <20, 20, -20> color White }

camera {
	location <0, 0.75, -4>
	look_at <0, 0.25, 0>
	right x * image_width / image_height
	angle 45
}

#declare CupTexture = texture { pigment { color LightBlue } }
#declare BoxTexture = texture { pigment { color DarkGreen } }

#include "cup.inc"

#declare CoffeeCupRaw = union {
	object { Handle
		rotate x * -90
	}
	object { Cup }
}

#declare CoffeeCup = object {
	CoffeeCupRaw
	translate <0, -Cup_top - Cup_height / 2, 0>
	scale 1 / Cup_height
	scale <1, -1, 1>
	texture { CupTexture }
}

object { CoffeeCup translate <0, 0.5, 0> }
// somewhere to put the cup down:
box { <-1, -0.25, -0.75> <1, 0, 0.75>
	texture { BoxTexture }
}

