#! /bin/sh
#
# simple helper for the case in which a TeXlive
# installation is present, but not in PATHs
# by default. Nothing fancy.
#

PROG=${0##*/}

e2 () { printf '%s: %s\n' "$PROG" "$*" 1>&2; }
fail () { e2 ${1+"$@"}; exit 1; }

# inst under home, or sys dir, or whatever
TD="$HOME"

# AFAIK the releases use year as name
TXREL=2012
TXROOT=texlive
TXARCH=x86_64
TXHOST=linux

TXBINDIR="$TD/$TXROOT/$TXREL/bin/$TXARCH-$TXHOST"
test -d "$TXBINDIR" || \
	fail edit script params, because \"$TXBINDIR\" is wrong

NPATH="PATH=\"$TXBINDIR:$PATH\""

TXMANDIR="$TD/$TXROOT/$TXREL/texmf/doc/man"
TXINFODIR="$TD/$TXROOT/$TXREL/texmf/doc/info"

NMPATH="MANPATH=\"$TXMANDIR"
NIPATH="INFOPATH=\"$TXINFODIR"

test X = X"$MANPATH" || NMPATH="$NMPATH:$MANPATH"
test X = X"$INFOPATH" || NIPATH="$NMPATH:$INFOPATH"

NMPATH="$NMPATH\""
NIPATH="$NIPATH\""

printf \
	'ORIGPATH="%s"; %s; ORIGMANPATH="%s"; %s; ORIGINFOPATH="%s"; %s' \
	"$PATH" "$NPATH" \
	"$MANPATH" "$NMPATH" \
	"$INFOPATH" "$NIPATH"

