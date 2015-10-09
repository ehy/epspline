#! /bin/sh

V="0.0.4.4p5"
FMT='%s\n'
test X"$1" = X-s && FMT='"%s"\n'
test X"$1" = X-i && V="1028"
printf "$FMT" "$V"
