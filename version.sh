#! /bin/sh

V="0.5.0.0"
FMT='%s\n'
test X"$1" = X-s && FMT='"%s"\n'
test X"$1" = X-i && V="327680"
printf "$FMT" "$V"
