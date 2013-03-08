
#include "ornaments.inc"
#include "colors.inc"
#include "metals.inc"
#include "glass.inc"
#include "textures.inc"

global_settings {
	max_trace_level 12
}

#declare DoOrnScene = 1;

#macro OrnPatternSelect(num)
	#switch ( num )
	#case ( 0 )
		bozo
		#break
	#case ( 1 )
		spiral1 4 rotate x*-90
		#break
	#case ( 2 )
		gradient y
		#break
	#end
#end
#macro OrnCmapSelect(num)
	#switch ( num )
	#case ( 0 )
		colour_map {
			[0.0 0.5 colour red 0.75 green 0.5 filter 0.25
			colour red 0.3 blue 0.75 filter 0.75]
			[0.3 0.8 colour red 0.3 blue 0.75 filter 0.75
			colour green 0.3 blue 0.75 filter 0.5]
			[0.6 1.0 colour green 0.3 blue 0.75 filter 0.5
			colour red 0.75 green 0.5 filter 0.25]
		}
		#break
	#case ( 1 )
		colour_map {
			[0.0 0.5 colour red 0.25 green 0.5 filter 0.75
			colour red 0.7 blue 0.25 filter 0.25]
			[0.3 0.8 colour red 0.7 blue 0.25 filter 0.25
			colour green 0.7 blue 0.25 filter 0.5]
			[0.6 1.0 colour green 0.7 blue 0.25 filter 0.5
			colour red 0.25 green 0.5 filter 0.75]
		}
		#break
	#case ( 2 )
		colour_map {
			[0.0 0.5 colour red 0.75 green 0.5 filter 0.25
			colour red 0.3 green 0.75 filter 0.75]
			[0.3 0.8 colour red 0.3 blue 0.75 filter 0.75
			colour green 0.7 blue 0.25 filter 0.5]
			[0.6 1.0 colour green 0.3 blue 0.75 filter 0.5
			colour red 0.75 green 0.7 filter 0.25]
		}
		#break
	#end
#end

#macro OrnBlueMac(idxpatt, turb)
	pigment {
		OrnPatternSelect(idxpatt)
		turbulence turb
		OrnCmapSelect(0)
	}
#end
#declare OrnBluemap = texture { OrnBlueMac(0, 0.5) }
#declare OrnBlueSpi = texture { OrnBlueMac(1, 0.001) }
#declare OrnBlueSpiWild = texture { OrnBlueMac(1, 0.8) }

#macro OrnXmasMac(idxpatt, turb)
	pigment {
		OrnPatternSelect(idxpatt)
		turbulence turb
		OrnCmapSelect(1)
	}
#end
#declare OrnXmasTxr = texture { OrnXmasMac(0, 0.5) }
#declare OrnXmasSpi = texture { OrnXmasMac(1, 0.001) }
#declare OrnXmasSpiWild = texture { OrnXmasMac(1, 0.8) }

#macro OrnPigmentMacInner(idxpatt, idxcmap, turb)
		OrnPatternSelect(idxpatt)
		turbulence turb
		OrnCmapSelect(idxcmap)
#end
#macro OrnPigmentMac(idxpatt, idxcmap, turb)
	pigment { OrnPigmentMacInner(idxpatt, idxcmap, turb) }
#end
#macro OrnTextureMac(idxpatt, idxcmap, turb)
	texture { OrnPigmentMac(idxpatt, idxcmap, turb) }
#end
#declare OrnT7 = OrnTextureMac(1,1,0.25)
#declare OrnT8 = OrnTextureMac(1,0,0.25)
#declare OrnT9 = OrnTextureMac(0,1,0.05)
#declare OrnT10 = OrnTextureMac(0,2,0.05)
#declare OrnT11 = OrnTextureMac(2,2,0.05)
#declare OrnT12 = OrnTextureMac(1,2,0.05)
#declare OrnT13 = OrnTextureMac(2,1,0.05)
#declare OrnT14 = OrnTextureMac(2,0,0.05)

#local test_ornaments = 0;
#ifndef ( DoOrnScene )
#declare DoOrnScene = 0;
#end
#if ( DoOrnScene )
#local test_ornaments = 0;
#end

#declare OrnMat = array[16] {
	// Pretty Red&Green bozo pattern semi translucent Xmas ornament
	// i 0
	material {
		texture {
			OrnXmasTxr scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// As above, but blue and gold
	// i 1
	material {
		texture {
			OrnBluemap scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Nice dark glass with spiral twist
	// i 2
	material {
		texture {
			T_Old_Glass
			finish {
				irid { 0.20
					turbulence 0.0001
					thickness 0.00002
				}
				specular 0.7
				ambient 0.3
				diffuse 0.2
			}
			normal { spiral1 4, 1.0
				turbulence 0.010
				rotate x*-90
			}
		}
		interior {
			I_Glass
		}
	},
	// Copper w/ bands of normals
	// i 3
	material {
		texture {
			T_Copper_4A
			finish {
				ambient .2
				diffuse .6
			}
			normal { gradient y
				frequency 1
				turbulence 0.001
				normal_map {
					[-.50 spiral1 20, 1.75 rotate x*-90]
					[0.25 spiral1 20, 1.75 rotate x*-90]
					[0.50 spiral2 20, 1.75 rotate x*-90]
					[0.75 spiral1 20, 1.75 scale <+1,-1,+1>
						rotate x*-90]
					[1.50 spiral1 20, 1.75 scale <+1,-1,+1>
						rotate x*-90]
				}
			}
		}
	},
	// Pretty blue&gold spiral pattern semi translucent Xmas ornament
	// Also spiral normal
	// i 4
	material {
		texture {
			OrnBlueSpi scale 1.0
			finish {
				specular 0.7
				ambient 0.8
			}
			normal { spiral1 4, 1.0
				turbulence 0.010
				rotate x*-90
			}
		}
		interior {
			I_Glass
		}
	},
	// Red & Green Xmas spiral with turbulence
	// i 5
	material {
		texture {
			OrnXmasSpiWild scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Pretty blue&gold spiral pattern semi translucent Xmas ornament
	// i 6
	material {
		texture {
			OrnBlueSpi scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Red&Green turbulent spiral
	// i 7
	material {
		texture {
			OrnT7  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Blue & Gold turbulent spiral
	// i 8
	material {
		texture {
			OrnT8  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Red & Green bozo low turbulence
	// i 9
	material {
		texture {
			OrnT9  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Green & Gold bozo
	// i 10
	material {
		texture {
			OrnT10  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Green&Gold Y gradient
	// i 11
	material {
		texture {
			OrnT11  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Green&Gold spiral
	// i 12
	material {
		texture {
			OrnT12  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Red & Green gradient Y
	// i 13
	material {
		texture {
			OrnT13  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Blue & Gold gradient Y
	// i 14
	material {
		texture {
			OrnT14  scale 0.25
			finish {
				specular 0.7
				ambient 0.8
			}
		}
		interior {
			I_Glass
		}
	},
	// Red & Greeb Spiral
	// Pretty red&green spiral pattern semi translucent Xmas ornament
	// Also spiral normal
	// i 15
	material {
		texture {
			OrnXmasSpi  scale 1.0
			finish {
				specular 0.7
				ambient 0.8
			}
			normal { spiral1 4, 1.0
				turbulence 0.010
				rotate x*-90
			}
		}
		interior {
			I_Glass
		}
	},
} // End #declare OrnMat = array[]

#local OrnCap = object { OrnCap0 translate <0, -OrnCap0_top, 0>
	texture { T_Gold_5C }
}

#macro Orn0(index)
object {
	union {
		object { OrnCap
		}
		object { OrnA0 translate <0, -OrnA0_top, 0>
			material { OrnMat[index]
				scale <-OrnA0_height,OrnA0_height,-OrnA0_height>
			}
		}
	}
	scale -1.0/OrnA0_height
	translate y*+0.5
}
#end
#macro Orn1(index)
object {
	union {
		object { OrnCap
		}
		object { OrnA1 translate <0, -OrnA1_top, 0>
			material { OrnMat[index]
				scale <-OrnA1_height,OrnA1_height,-OrnA1_height>
			}
		}
	}
	scale -1.0/OrnA1_height
	translate y*+0.5
}
#end
#macro Orn2(index, band_index)
#if ( 0 )
#local OrnA2_Band = object { OrnA2_Band0 }
#local OrnA2_Band_height = OrnA2_Band0_height;
#local OrnA2_Band_top = OrnA2_Band0_top;
#else
#local OrnA2_Band = object { OrnA2_Band1 }
#local OrnA2_Band_height = OrnA2_Band1_height;
#local OrnA2_Band_top = OrnA2_Band1_top;
#end
object {
	union {
		object { OrnCap
		}
		object { OrnA2_Band translate <0, -OrnA2_top, 0>
			material { OrnMat[band_index]
				scale OrnA2_Band_height
				translate y*+(OrnA2_Band_top-OrnA2_top)
			}
		}
		object { OrnA2 translate <0, -OrnA2_top, 0>
			material { OrnMat[index]
				scale <-OrnA2_height,OrnA2_height,-OrnA2_height>
			}
		}
	}
	scale -1.0/OrnA2_height
	translate y*+0.5
}
#end
#macro Orn3(index)
object {
	union {
		object { OrnCap
			scale 2/3
		}
		object { OrnA3 translate <0, -OrnA3_top, 0>
			material { OrnMat[index]
				scale <-OrnA3_height,OrnA3_height,-OrnA3_height>
			}
		}
	}
	scale -1.0/OrnA3_height
	translate y*+0.5
}
#end
#macro Orn4(index)
object {
	union {
		object { OrnCap
			scale 2/3
		}
		object { OrnA4 translate <0, -OrnA4_top, 0>
			material { OrnMat[index]
				scale <-OrnA4_height,OrnA4_height,-OrnA4_height>
			}
		}
	}
	scale -1.0/OrnA4_height
	translate y*+0.5
}
#end
#macro Orn5(index)
object {
	union {
		object { OrnCap
			scale 2/3
		}
		object { OrnA5 translate <0, -OrnA5_top, 0>
			material { OrnMat[index]
				scale <-OrnA5_height,OrnA5_height,-OrnA5_height>
			}
		}
	}
	scale -1.0/OrnA5_height
	translate y*+0.5
}
#end

/*
 * Name some choice ornaments
 */
#declare TopBozoRG = object { Orn0(0) }
#declare TopBozoBY = object { Orn0(1) }
#declare AppleBozoBY = object { Orn3(1) }
#declare BandedGlass = object { Orn2(2, 3) }
#declare AlmondSpiralBY = object { Orn1(4) }
#declare AlmondSpiralRG = object { Orn1(15) }
#declare AlmondTurbBozoRG = object { Orn1(5) }
#declare AlmondTightSpiralBY = object { Orn1(6) }
#declare TopTurbSpiralRG = object { Orn0(7) }
#declare TopTurbSpiralBY = object { Orn0(8) }
#declare AppleLowTurbBozoRG = object { Orn3(9) }
#declare AcornTightSpiralBY = object { Orn4(6) }
#declare AcornSpiralGY = object { Orn4(12) }
#declare AcornGradientGY = object { Orn4(11) }
#declare BellBozoGY = object { Orn5(10) }
#declare BellGradientRG = object { Orn5(13) }
#declare BellGradientBY = object { Orn5(14) }
#declare AcornBozoGY = object { Orn4(10) }

/* Array:
 * this array is intended for ``random'' selections.
 */
#declare OrnCount = 18 ;
#declare OrnArray = array[OrnCount] {
	object { TopBozoRG },
	object { TopBozoBY },
	object { AppleBozoBY },
	object { BandedGlass },
	object { AlmondSpiralBY },
	object { AlmondSpiralRG },
	object { AlmondTurbBozoRG },
	object { AlmondTightSpiralBY },
	object { TopTurbSpiralRG },
	object { TopTurbSpiralBY },
	object { AppleLowTurbBozoRG },
	object { AcornTightSpiralBY },
	object { AcornSpiralGY },
	object { AcornGradientGY },
	object { BellBozoGY },
	object { BellGradientRG },
	object { BellGradientBY },
	object { AcornBozoGY },
}

#if ( test_ornaments )

camera {
	location <0, 0, -3>
	right 4/3*x
	up y
	direction z
	look_at <0, 0.0, 0>
}

light_source {
	<50, 50, -90>
	color White
	fade_distance 55
	fade_power 1
}

plane { z, 30
	pigment { color White }
	hollow
}

object {
	TopBozoRG
	translate y*0.7
	texture { pigment { color Yellow } }
}

object {
	AppleBozoBY
	rotate y*90
	translate y*-0.7
}

object {
	BandedGlass
	translate x*-0.7
}

object {
	AlmondSpiralBY
	translate x*+0.7
}

object {
	AcornGradientGY
	translate y*-0.7
	translate x*+1.4
}

object {
	AlmondSpiralRG
	translate y*-0.7
	translate x*-1.4
}

object {
	BellGradientBY
	translate y*+0.7
	translate x*+1.4
}

object {
	AcornSpiralGY
	translate y*+0.7
	translate x*-1.4
}

#end // test_ornaments

// This declaration, before file include, will enable a transform
// that scales the object to 1 unit on greatest dimension, and
// centers the object on the origin.
#declare DinnerPlate0_USE_NORMAL_TRANSFORM = 1;
// include the plate object
#include "plate.inc"
// wrap object in case other attributes are wanted
#declare DinnerPlate = object {
	DinnerPlate0
	translate <0.0, 0.05, 0.0>
}

#declare TableTop = object {
#local TableTopTop = -DinnerPlate0_height/2/DinnerPlate0_max_extent+0.05;
	box { <-12,TableTopTop-1,-9>, <12,TableTopTop,9>
		texture { Yellow_Pine
			scale 5.950
			rotate <2, -2.5, 2>
			//translate <5, -4, -12>
		}
	}
}

#declare OrnDisplay = union {
	object { TableTop
	}
	object { DinnerPlate
		scale 5.0
	}
	object { AcornGradientGY
		rotate <60, 20,0>
		translate <-1.0, 0.5, 1.0>
	}
	object { AcornTightSpiralBY
		rotate <60, -55,0>
		translate <-1.2, 0.5, -1.1>
	}
	object { TopBozoRG
		rotate <60, +80,0>
		translate <+0.3, 0.5, 0.0>
	}
	object { TopBozoBY // TopTurbSpiralBY
		rotate <60, -120,0>
		translate <-0.4, 0.5, -.55>
	}
	object { AlmondSpiralBY
		rotate <80, -120,0>
		translate <+1.35, 0.5, -1.1>
	}
	object { AlmondSpiralRG
		rotate <-80, +010,0>
		translate <-0.3, 0.5, -1.6>
	}
	object { BandedGlass
		rotate <+15, +005,0>
		translate <+0.4, 0.5, -1.0>
	}
	object { BandedGlass
		rotate <-15, +005,0>
		translate <-0.05, 0.5, +0.75>
	}
	object { BellGradientRG
		rotate <110, -110,0>
		translate <-1.4, 0.5, -0.0>
	}
	object { BellBozoGY
		rotate <110, -120,0>
		translate <+1.2, 0.5, -0.0>
	}
	object { AppleBozoBY
		rotate <-10, -040,0>
		translate <+1.0, 0.5, +1.0>
	}
	object { AppleLowTurbBozoRG
		rotate <+10, -090,0>
		translate <+0.1, 0.5, +1.7>
	}
}

#if (DoOrnScene)

object {
	OrnDisplay
	scale 01.00
}

camera {
	location <2.2, 4, -2.5>
	up y
	direction z
	look_at <0.09, 0.0, 0.04>
	right x * image_width / image_height
}

light_source {
	<00, 50, -90>
	color White
	fade_distance 55
}

#end // #if (DoOrnScene)


