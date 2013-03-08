#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
#include "woods.inc"


#ifndef ( TestingFire )
#declare TestingFire = 1;
#end
#ifndef ( DoFire )
#declare DoFire = 1;
#end
#ifndef ( DoFuelLogs )
#declare DoFuelLogs = 1;
#end
#ifndef ( UseFireLightSource )
#declare UseFireLightSource = 0;
#end

#if ( TestingFire )
light_source {  
    <50, 50, -90>
    color White
    fade_distance 55
    fade_power 1
}

light_source {
    <23.0, 30.0, -20>
    color White
    fade_distance 25
    fade_power 2
    shadowless
}

#if ( 1 )
camera {
	location <0, 8.675, -23>
	look_at <0, 8.85, 0>
}
#else
camera {
	location <-3.5, -2.25, 18.5>
	look_at <-2.35, -2.00, 20.0>
}
#end

#end // #if ( TestingFire )


#if ( DoFire )
#declare FireMedia = media {
	emission rgbt 5
	scattering {3, 0.5}
	intervals 5
	samples 1, 10
	confidence 0.8
	variance 1/100
	density {
		gradient x
		sine_wave
		color_map {
			[0.0 rgbf <0, 0, 05.9, 5.7>]
			[0.4 rgbf <0, 0, 04.9, 5.7>]
			[0.5 rgbf <0.0, 0.0, 2.0, 5.7>]
			[0.6 rgbf <0.0, 0.0, 1.0, 5.7>]
			[0.7 rgbf <0.0, 0.0, 0.5, 5.7>]
			[1.0 rgbf <0.0, 0.0, 0.0, 5.7>]
		} 
		warp { turbulence <.4,.8,.4> octaves 4 lambda 3.5 omega 0.3 }
		scale 50
		translate <37, 0, 0>
	}
	// Multiple densities in a media are multiplied
	density {
		gradient y
		ramp_wave
		density_map {
			[0.000 rgb 0.00]
			[0.187 rgb 0.01]
			[0.333 rgb 0.09]
			[0.500 rgb 0.50]
			[0.667 rgb 0.90]
			[1.000 rgb 1.00]
		}
		warp { turbulence <.3,.1,.3> octaves 3 lambda 1.5 omega 0.3 }
		scale 52
		translate <0, 10, 0>
	}
}

#if ( DoFuelLogs )
#declare FireWood0 =
object {
	difference {
		blob {
		  threshold 1.029
			cylinder {<-3.0,2.0,-1.0>,<-0.25,1.5,-0.5>, 0.75, 1.7
				/* texture { T_Wood2 } */
			}
			cylinder {<-2.5,1.75,-1.3>,<1.05,2.5,-0.9>, 0.95, 1.7
				/* texture { T_Wood2 } */
			}
			cylinder {<-0.5,1.7,-0.7>,<2.75,1.35,-0.0>, 0.65, 1.7
				/* texture { T_Wood2 } */
			}
			cylinder {<0.95,2.35,-0.7>,<3.0,1.5,-0.3>, 0.75, 1.7
				/* texture { T_Wood2 } */
			}
			sturm
		}
		plane {<1,0,0>, -2.5 rotate <0,5,7> /* texture { T_Wood2 } */ }
		plane {<-1,0,0>, -2.8 rotate <0,-5,-7> /* texture { T_Wood2 } */ }
	}
	texture { T_Wood2 finish { ambient .50 diffuse .2 } }
}
#declare FireWood1 =
object {
	difference {
		blob {
		  threshold 1.029
			cylinder {<-2.8,1.0,1.0>,<0.45,0.5,0.75>, 0.85, 1.7
				/* texture { T_Wood2 } */
			}
			cylinder {<-2.9,1.25,0.9>,<0.25,0.7,0.9>, 0.75, 1.7
				/* texture { T_Wood2 } */
			}
			cylinder {<0.25,0.6,0.7>,<2.95,1.35,-0.5>, 0.95, 1.7
				/* texture { T_Wood2 } */
			}
			cylinder {<0.35,0.4,0.65>,<3.0,1.5,-0.3>, 0.80, 1.7
				/* texture { T_Wood2 } */
			}
			sturm
		}
		plane {<1,0,0>, -2.5 rotate <0,5,7> /* texture { T_Wood2 } */ }
		plane {<-1,0,0>, -2.6 rotate <0,5,7> /* texture { T_Wood2 } */ }
	}
	texture { T_Wood2 finish { ambient .50 diffuse .2 } }
}
#declare FireWood =
union {
	object { FireWood0 translate <1.0,0,1.0> }
	object { FireWood1 translate <-0.5,0,-0.5> }
}
#end // DoFuelLogs

#include "fireblob.inc"

#declare FireMedium =
object {
	FireBlob
	#if ( 0 )
	pigment { color Blue }
	#else
	pigment { color Clear }
	finish { ambient 0 diffuse 0 }
	interior {
		media {
			FireMedia
		}
		media {
			emission rgbt <5.8 5.8, 0.0, 0.9>
		}
	}
	hollow on
	#end
	no_shadow
	scale 0.27
	#if ( UseFireLightSource )
	translate y*0
	#else
	translate y*11.00
	#end
}

#if ( DoFuelLogs )
#declare Fire =
union {
	#if ( UseFireLightSource )
	light_source {
		<0,11,0>
		color <1.0, 1.0, 0.5>
		looks_like { FireMedium }
		fade_distance 2
		fade_power 1
	}
	#else
	object {
		FireMedium
	}
	#end
	object {
		FireWood
		scale 2.55
		translate y*0.1
	}
}
#else // DoFuelLogs
#declare Fire =
object {
	#if ( UseFireLightSource )
	light_source {
		<0,11,0>
		color <1.0, 1.0, 0.5>
		looks_like { FireMedium }
		fade_distance 2
		fade_power 1
	}
	#else
	object {
		FireMedium
	}
	#end
}
#end // DoFuelLogs

#end // #if ( DoFire )

#if ( TestingFire )

object {
	Fire
	scale <1.0, 1.0, 1.0>
	translate <0.0, 0.0, 0.0>
}

plane { <0, 1, 0>, 0
    pigment {
        checker color Red, color Blue
    }
	translate y*-0.00
	hollow
}
plane { <0, 0, -1>, 0 translate z*50
    pigment {
        checker color Yellow, color Green
    }
	hollow
}
#end // TestingFire

