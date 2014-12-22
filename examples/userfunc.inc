// common functions and macros
// Ed Hynan -- 2014
//
// Convention: functions in all lower case, macros
// mixed case with initial capital
//

// square and cube: *untested assumption*
// that the multiplication here will be faster
// than library pow(val,exp)
#macro Square(Arg)
	(Arg * Arg)
#end
#macro Cube(Arg)
	(Arg * Arg * Arg)
#end
// note on names: square, sqr, and cube
// are reserved; hence, the awkward "squ"
// and "cub"
#declare squ = function(Arg) { Arg * Arg }
#declare cub = function(Arg) { Arg * Arg * Arg }

