#! /bin/sh
#
# Make a PNG from an example POV file.
#

P=${0##*/}
W=800
H=600

: ${ADDLARG:="-visual DirectColor"}
: ${OTYPE:=".png"}

# POSIX sh getopts
while getopts h:w: opt; do
  case $opt in
    h ) H="$OPTARG" ;;
    w ) W="$OPTARG" ;;
    * ) echo $P: bad arg "\"$OPTARG\"" 1>&2; exit 1 ;;
  esac
done

eval N=\"\$$OPTIND\"

test X = X"$N" && {
  echo $P: need POV file argument 1>&2
  exit 1
}
test -f "$N" && test -r "$N" || {
  echo $P: bad input file argument 1>&2
  exit 1
}

OBN="${N%.*}"

exec povray +D +X3 +P +FN +A0.7 +W$W +H$H -I"$N" -O"$OBN"-${W}x${H}${OTYPE} ${ADDLARG}
