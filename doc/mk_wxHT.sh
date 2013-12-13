#! /bin/sh
#
# From TeX file comment:
# Coded comment follows '%WX%': string of fields separated by '|', as follows:
#	F1== logical level: 0==N.G., 1==book, 2==chapter, 3==section, etc.
#	F2== name, or label; if "" than take left of '%WX%', between { and }
#	F3== 32 bit integer identifier :  generated incrementally if empty*
#	F4== name for cpp macro, e.g. IDI_$F4; generated if empty
#	F5== name of output file; if "" then override file<->title map
#	*Note on F3: IDs incr from low numbers, so make explicit #s high
#

PROG=${0##*/}
GPAT="%WX%"
: ${DBG:="no"}

: ${PNAME:="epspline"}
: ${HNAME:=${PNAME}.zip}
: ${HHC:=${PNAME}.hhc}
: ${HHK:=${PNAME}.hhk}
: ${HHP:=${PNAME}.hhp}
: ${HDR:=${PNAME}_helpids.h}
: ${TITLE:="Epspline Help"}
: ${DEFTOPIC:=${PNAME}.html}
#: ${DEFTOPIC:=contentsname.html}
# this file must exist; check against override error:
test -f "$DEFTOPIC" || DEFTOPIC="${PNAME}.html"

: ${FMAP:=${PNAME}_fn.map}
: ${CURID:=0}
: ${INCID:=16}

e2 () { printf "$PROG: %s\n" "$*" 1>&2; }
fail () { e2 ${1+"$@"}; exit 1; }
dbg () { test X"$DBG" = Xyes && e2 ${1+"$@"}; true; }

CLVL=0

inc_lvl () { CLVL=$(( CLVL + 1 )); dbg CLVL inc\'d is $CLVL; true; }
dec_lvl () { CLVL=$(( CLVL - 1 )); dbg CLVL dec\'d is $CLVL; true; }

test X"$1" = X || exec <"$1"
exec 3>"$HHC"; HHC_FD=3
exec 4>"$HHK"; HHK_FD=4
exec 5>"$HDR"; HDR_FD=5
POF=$HHC_FD

ptf () {
	printf ${1+"$@"} 1>&$POF
}

put_tabs () {
	n=0
	while test $n -lt $1 ; do
		ptf '\t'
		n=$(( n + 1 ))
	done
}

# includes everything up to body
open_body () {
	ptf '<html>\n<head>\n'
	ptf '\t<title>%s</title>\n' "$TITLE"
	ptf '\t<meta name="date" content="%s">\n' \
		"$(date '+%Y-%m-%d %H:%M:%S')"
	ptf '</head>\n\n<body>\n\n'
}
close_body () {
	ptf '\n</body>\n</html>\n'
}

open_ul () {
	put_tabs $1
	ptf '<ul>\n'
}
close_ul () {
	put_tabs $1
	ptf '</ul>\n'
}
open_li () {
	put_tabs $1
	ptf '<li>\n'
}
close_li () {
	put_tabs $1
	ptf '</li>\n'
}
open_obj () {
	put_tabs $1
	ptf '<object type="text/sitemap">\n'
}
close_obj () {
	put_tabs $1
	ptf '</object>\n'
}
put_param () {
	put_tabs $1
	ptf '<param name="%s" value=%s />\n' "$2" "$3"
}
put_param_Name () {
	put_param $1 "Name" "\"$2\""
}
put_param_ID () {
	put_param $1 "ID" "$2"
}
put_param_Local () {
	put_param $1 "Local" "\"$2\""
}

# get incremented ID value for cpp macros
mk_new_id () {
	CURID=$(( CURID + INCID ))
}

# get & remove head of char separated fields
# $1 is name of var that is assigned head field
# $2 is name of var holding string of fields (from which head is removed)
# $3 is separator character
yankhead () {
	eval $1=\"\${$2%%\\${3}*}\"
	eval $2=\"\${$2#*\\${3}}\"
	eval t1=\"\$"${1}"\"
	eval t2=\"\$"${2}"\"
	test X"$t2" = X"$t1" && eval "${2}=\"\""
}

# set value at tail of char separated fields
# $1 is the value to set
# $2 is name of var holding string of fields
# $3 is separator character
puttail () {
	eval t1=\"\$"${2}"\"
	test X"$t1" = X || t1="${t1}${3}"
	t1="${t1}${1}"
	eval ${2}=\""${t1}"\"
}

mk_fmap () {
	:> "$FMAP"
	for HTF in *.html ; do
		HTIT="$(grep -F '<title>' $HTF | head -n 1)"
		test X = X"$HTIT" || {
			HTIT="${HTIT#*<title>}"; HTIT="${HTIT%%</title>*}"
			test X = X"$HTIT" || \
				printf '%s:%s\n' "$HTF" "$HTIT" >> "$FMAP"
		}
	done
}

mk_fmap_2nd () {
	for HTF2 in *.html ; do
		OIFS="$IFS"; IFS="" 
		grep -E '<h[0-9][[:space:]]+.*<a[[:space:]]name=' "$HTF2" | \
			while read -r MTCH ; do
				PHRZ="${MTCH#*<a name=\"}"
				MTCH="${PHRZ%%\"*}"
				test X = X"$MTCH" && continue
				PHRZ="${PHRZ#*</a>}"; PHRZ="${PHRZ%%</h*}"
				test X = X"$PHRZ" && continue
				printf '%s#%s:%s\n' "$HTF2" "$MTCH" "$PHRZ" >> "$FMAP"
			done
		IFS="$OIFS"
	done
}

# Get a filename name from the file<-->name map.
# Can contain both pretty title string and search regex separated
# by '<R>' as in "Hi: I'm a Pretty Title<r>Hi.*: I'm a .*".
# (The regex is needed if the html might have tags: <b>Hi</b>: I'm ....)
get_fmap () {
	# accept '<r>' at head of pattern to signify it is a regex
	case "$1" in
		*\<R\>* )
			SPAT="${1#*\<R\>}"
			;;
		* )
			SPAT=$(echo "$1" | \
			sed -e 's/<[^><]*>//g' -e 's/[ 	][ 	]*/\[ 	\]\[ 	\]\*/g')
			;;
	esac
	RES=$(grep "$SPAT" "$FMAP") || {
		dbg "no pattern \"$SPAT\" for arg \"$1\" in file \"$FMAP\""
		return 1
	}
	HTF=${RES%%:*};
	dbg "got file " "$HTF" " for pattern " "\"$SPAT\""
	printf '%s' "$HTF"
	return 0
}

strip_tex () {
	t="${1#*\{}"; t="${t%\}*}"
	test X"$t" = X && return 1
	printf '%s' "$t"
	return 0
}

mk_id_name () {
	t="$(strip_tex "$1")" || return $?
	t="$(printf '%s' "$t" | sed 's/[^0-9a-zA-Z]//g')" || return $?
	test X"$t" = X && return 1
	printf '%s' "$t"
	return 0
}

# helper for feed() below; checks for uncommented-out line
# with \input or \include and if regular file is found
# it is printed and the proc succeeds
check_input_line () {
	_locparm0="${1%%\%*}"
	_locparm1=${_locparm0#*\\input\{}
	test X"$_locparm1" = X"$_locparm0" && \
		_locparm1=${_locparm0#*\\include\{}
	test X"$_locparm1" = X"$_locparm0" && \
		return 1
	_locparm1=${_locparm1%%\}*}
	test -f "$_locparm1" || _locparm1="$_locparm1.tex"
	test -f "$_locparm1" || return 1
	printf '%s' "$_locparm1"
	return 0
}

# feed standard input to output, but checking each line
# for an inclusion, and recursing on includes as necessary
# note a line with an include is printed too, not substituted
feed () {
	while read -r L; do
		printf '%s\n' "$L"
		if F=$(check_input_line "$L") ; then
			dbg 'feed()ing inclusion' "\"$F\""
			feed < "$F"
		fi
	done
}


TF=$(mktemp "$PROG.XXXXXX") || fail "cannot make temporary file"
rm_TF () { rm -f "$TF"; }
trap rm_TF 0

cat >"$HHP" <<EOF
Contents file=$HHC
Index file=$HHK
Title=$TITLE
Default topic=$DEFTOPIC
EOF

open_body
POF=$HHK_FD; open_body; open_ul 0
POF=$HDR_FD;
ptf '\n\n#ifndef _%s_INCLUDED \n#define _%s_INCLUDED 1\n\n' "${HDR%.*}" "${HDR%.*}"
cat >&$POF <<EOF
/* name of the wx help "book" file */
#define HELP_BOOK_NAME	"${HNAME}"

/* Help Key String: macro _HKS */
#ifdef _HKS
#error "Help Key String: macro _HKS was already defined"
#endif

#if defined(HELP_KEYS_USE_TRANSLATIONS)
/* NOTE: if help keys are translated,
 * with this macro, then xgettext must
 * be given option -k_HKS
 */
#define _HKS(s)		_(s)
#else /* do not translate help key strings */
#define _HKS(s)		wxT(s)
#endif


EOF

# make map filenames <-> titles
mk_fmap
mk_fmap_2nd
# default output
POF=$HHC_FD
# initial .hhc level
echo $CLVL > "$TF"

# input from stdin (or file arg opened on stdin)
feed | grep -F "$GPAT" | while read -r L; do
	LFT="${L%${GPAT}*}"
	RGT="${L##*${GPAT}}"
	yankhead LL RGT '|'
	case X"$LL" in
		X[0-9] )
		    ;;
		* ) e2 "FOUND field 1 is \"$LL\""
		    e2 "unexpected line is \"$L\""
		    continue ;;
	esac
	dbg "LL is \"$LL\""
	yankhead NM RGT '|'
	test X"$NM" = X && NM="$(strip_tex "$LFT")"
	dbg "NM is \"$NM\""
	yankhead ID RGT '|'
	test X"$ID" = X && { mk_new_id; ID=$CURID; }
	printf '%s' "$ID" | grep '[^0-9]' && {
		e2 "FOUND field 3 is \"$ID\""
		e2 "unexpected line is \"$L\""
		continue
	}
	dbg "ID is \"$ID\""
	# get macro name (if extant); defer check post filename handling
	yankhead IDFN RGT '|'
	# new and improved: if file name is not given, get it from map
	# built from the html files
	yankhead FN RGT '|'
	test X"$FN" = X && {
		FN="$(get_fmap "$NM")" || {
			e2 "cannot find or make filename"
			e2 "FOUND field 5 is \"$ID\""
			e2 "unexpected line is \"$L\""
			continue; }
	}
	dbg "FN is \"$FN\""
	# Now done w/ NM; strip any regex
	NM="${NM%\<R\>*}"
	# check for file; failure not fatal
	test -f "$FN" || e2 "FILE NOT FOUND: \"$FN\""
	# with new and improved file name map, retain name making
	# function to make cpp macro base (mk_id_name was mk_fname)
	test X"$IDFN" = X && IDFN=$(mk_id_name "$NM")
	test X"$IDFN" = X && {
		IDFN=$(mk_id_name "$LFT") || {
			e2 "using filename base for cpp macro name"
			IDFN=${FN%.*}
		}
	}
	dbg "IDFN is \"$IDFN\""

	# start output -- surely this is all obvious!
	# output is to '.hhc' file used for tree control in viewer:
	# it is only *ML hierarchy of <li> and <ul>, and must be sane --
	# LL is intended level for data collected above -- 0 is error --
	# if LL == CLVL (current level) just add <li>
	# if LL == (CLVL + 1), inc CLVL, open <ul>
	# if LL < CLVL, close <ul>, dec CLVL,
	# BUT <ul> may need need closing over several levels, so
	# loop closing <ul> unto equality
	# other differences are logical errors because levels are
	# expected to increment, e.g. book->part->chapter->section->etc.,
	# but obviously it is not required to step down by 1 because
	# e.g. a subsection can be last in its logical group and
	# be followed by a chapter (i.e. several levels above);
	# after handling <ul> levels, add <li> for collected data
	if test $LL -gt $(( CLVL + 1 )) ; then
		fail "level error: object level $LL, current level $CLVL"
	fi
	if test $LL -eq $(( CLVL + 1 )) ; then
		inc_lvl
		open_ul $CLVL
	fi
	while test $LL -lt $CLVL ; do
		close_ul $CLVL
		dec_lvl
	done

	L=$CLVL
	A0=0; A1=1; A2=2; A3=3
	open_li $(( L + $A0 ))
	open_obj $(( L + $A1 ))
	put_param_Name $(( L + $A2 )) "$NM"
	put_param_ID $(( L + $A2 )) "$ID"
	put_param_Local $(( L + $A2 )) "$FN"
	close_obj $(( L + $A1 ))
	# record level for post-loop (in parent)
	echo $CLVL > "$TF"
	
	# additional files
	POF=$HHK_FD
	L=1
	open_li $(( L + $A0 ))
	open_obj $(( L + $A1 ))
	put_param_Name $(( L + $A2 )) "$NM"
	put_param_ID $(( L + $A2 )) "$ID"
	put_param_Local $(( L + $A2 )) "$FN"
	close_obj $(( L + $A1 ))
	POF=$HDR_FD
	ptf '#define IDI_%s\t\t%d\n' $IDFN $ID
	ptf '#define IDS_%s\t\t"%s"\n' $IDFN "$NM"
	ptf '#define IDF_%s\t\t"%s"\n' $IDFN "$FN"
	ptf '\n'
	# reset to default output
	POF=$HHC_FD
done

CLVL=$(cat "$TF")

while test $CLVL -gt 0; do
	close_ul $CLVL
	CLVL=$(( CLVL - 1 ))
done

close_body
POF=$HHK_FD; close_ul 0; close_body
POF=$HDR_FD; ptf '\n#endif /* _%s_INCLUDED */\n\n' "${HDR%.*}"
