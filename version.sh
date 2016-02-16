#! /bin/sh

V="0.0.4.5"
FMT='%s\n'
test X"$1" = X-s && FMT='"%s"\n'
test X"$1" = X-i && V="1029"
printf "$FMT" "$V"
