#! /bin/sh
#
# script to make 'translation' for en_US from epspline.pot
# not a real translation; just copy msgid to msgstr, adding
# typographical quotes, etc. -- the output is UTF-8, and
# this script contains UTF-8 in sed args
#
# this is not useful for real (non-English) translations
#
# Note that date "+%Y-%m-%d %H:%M %Z", as used below, is close
# to but not exactly the format requested in the xgettext
# generated .pot file.  The numeric time-zone offset would
# require `date "+%Y-%m-%d %H:%M%z"`, but posix lacks the
# (lowercase) '%z' spec.. See:
#
#	http://pubs.opengroup.org/onlinepubs/009695399/
#

PROG=${0##*/}

e2 () { echo ${PROG}: ${1+"$@"} 1>&2; }
fail () { e2 ${1+"$@"}; exit 1; }

mkdate () { date "+%Y-%m-%d %H:%M %Z"; }

TF=$(mktemp "${PROG}.XXXXXXXX") || fail cannot make temp file
cleanup () {
	test -f "$TF" && rm "$TF"
}
trap cleanup 0

IF=${1:-"epspline.pot"}
OF=${2:-"en_US/epspline.po"}

test -d en_US || mkdir en_US || fail cannot mkdir en_US

:>"$TF"; while read -r L; do
	case "$L" in
	"msgstr \""* )
		sed \
		-e 's/^msgid/msgstr/' \
		-e 's/\([a-zA-Z0-9]\)'\''\([a-zA-Z0-9]\)/\1’\2/g' \
		-e 's/\([a-zA-Z0-9\.]\)\\"/\1”/g' \
		-e 's/'\''\\n/’\\n/g' \
		-e 's/\\"\\n/”\\n/g' \
		-e 's/\([ 	]\)\\"\("[ 	]*\)$/\1“\2/g' \
		-e 's/\([a-zA-Z0-9\.]\)'\''/\1’/g' \
		-e 's/\\"\([a-zA-Z0-9\%]\)/“\1/g' \
		-e 's/^\(msg[^ 	]\{1,\}[ 	]\{1,\}"\)\\"\([ 	]\)/\1”\2/g' \
		-e 's/^"\\"\([ 	]\)/"”\1/g' \
		-e 's/'\''\([a-zA-Z0-9]\)/‘\1/g' \
		-e 's/\([^!]\)--\([^>]\)/\1—\2/g' \
		-e 's/^--\([^>]\)/—\1/g' \
		-e 's/\([^!]\)--$/\1—/g' \
		-e 's/\([Bb]\)ezier/\1ézier/g' \
		-e 's/"_empty_field_"/"_none_"/' \
		< "$TF"
		:>"$TF"
		continue
		;;
	esac
	test -s "$TF" && printf '%s\n' "$L" >> "$TF"
	printf '%s\n' "$L"
	case "$L" in
	"msgid \""* )
		printf '%s\n' "$L" > "$TF"
		;;
	esac
done < "$IF" | sed \
	-e 's/^\([ 	]*"PO-Revision-Date: \).*\\n"[ 	]*$/\1'"$(mkdate)"'\\n"/' \
	-e 's/^\([ 	]*"Language: \)\\n"[ 	]*$/\1en_US\\n"/' \
	-e 's/^\("Content-Type: text\/plain; charset=\)CHARSET\\n"$/\1UTF-8\\n"/' \
	> "$OF"

IF="$OF"
OF="${OF%.*}".mo

msgfmt -o "$OF" -v "$IF" || fail FAILED making "\"$OF\""

