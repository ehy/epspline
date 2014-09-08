#! /bin/sh

V="0.0.4.3p1"
FMT='%s\n'
test X"$1" = X-s && FMT='"%s"\n'
test X"$1" = X-i && V="1027"
printf "$FMT" "$V"
