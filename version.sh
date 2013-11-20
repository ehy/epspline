#! /bin/sh

VMA=0
VMI=0
RMA=4
RMI=3

V="${VMA}.${VMI}.${RMA}.${RMI}"
FMT='%s\n'
test X"$1" = X-s && FMT='"%s"\n'
# 32 bit int: vmajor<<24|vminor<<16|rmajor<<8|rminor
test X"$1" = X-i && {
	get_32 () {
		bc <<OYV
			a=$1; b=$2; c=$3; d=$4;
			define s(v, n) {
				while ( n-- > 0 ) {
					v *= 2;
				}
				return v;
			}
			s(a, 24) + s(b, 16) + s(c, 8) + d;
OYV
	}
	V=$(get_32 ${VMA} ${VMI} ${RMA} ${RMI})
}
printf "$FMT" "$V"
