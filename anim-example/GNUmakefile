#
# boardspace.pov.
# -
# Since epspline will export a spline to a POV-Ray
# array definition if the type of the spline is set to
# `undefined,' the point-data can be used in other
# ways.  The boardspace example makes a random boardlike
# thing, and moves an object among obstacles on the board.
# -
# Works like this:
# Invoke, if neeeded, the "traceimage" target in the
# (GNU make) Makefile, open boardspace-path.pse with
# Epspline and check/adjust bezier path. Next run make
# (or gmake) with no target and check the available
# targets near the top of the help output (try LDdevel
# first), peruse the remainder of help text, and try
# something like this:
# -
# time make PREVIEW=true ANI_FILM=true ENCODE_TYPE=ntsc-film \
#	AUDIO_ARGS="-an" MULTIJOBS=true \
#	ffLDdevel
# -
# Note the target is "ffLDdevel" which will finish with
# ffmpeg encoding -- if you don't have ffmpeg just use
# "LDdevel"
# -
# Ed Hynan 2014
#

# GMAKE Makefile for POV-Ray animation
# Although portable makefiles are best by far, indulging
# in GNU Make's features reduces the need for complex
# shell commands in rules; at least GNU Make is a portable program
#
# Oct 2012 Ed Hynan
#

# use a ulimit for mem: certain SDL errors can make povray spin
# eating memory -- in Linux this can lock the machine to the point
# of invoking the brilliant OOM process killer, which then proceeds
# to make an unusable botch out of the machine -- so hope that a
# ulimit on memory will prevent this
# NOTE: of the ulimit options, on GNU/Linux (Ubuntu 12.04) only
# -v (virtual mem limit) is effective to stop runaway allocations;
# also Ubuntu (and Debian) default shell, dash, *DOES* implement -v,
# but the manual page (at least on Ubuntu) fails to mention -v.
# Also, this has been tried on OpenBSD 4.9, on which -v is NOT
# supported, but where -m just works well anyway. :-)
#UMEMMAX ?= 500000  # good enough for povray 3.6 . . .
UMEMMAX ?= 5000000 # . . . but 3.7 needs more
ifndef NO_ULIMIT
ULIMIT := ulimit -m $(UMEMMAX); ulimit -v $(UMEMMAX)
else
ULIMIT := :
endif

#NAMEBASE     ?= anim
NAMEBASE     = boardspace
namebase     = $(NAMEBASE)
namepov      = $(namebase)

povext       = pov
nameext      = png
vidext       = mpg
vidextxlg    = mp4
typeopt      = +FN

#pov          = povray
pov          = povray-3.7
#pov          = povray-3.6
ffm          = ffmpeg

MV           = mv
RM           = rm
CP           = cp
MVFORCE      = $(MV) -f
RMFORCE      = $(RM) -f
CPFORCE      = $(CP) -f

ifndef FRAMEDIR
framedir = frames-$(namebase)
else
framedir = $(FRAMEDIR)
endif
nameout = $(framedir)/$(namebase)

# preview window optional for animation
ifdef PREVIEW
dflag36        = -visual DirectColor +D
dflag37        = +D
dflag        = ${dflag37}
else
dflag        = -D
endif

# frame count is best if a multiple of GOP number (or so I assume)
# e.g. 756 == 42 * 18 (18 is max GOP for NTSC)
# 900 frames == 30.03 seconds at rate 30000/1001; and
# 900 == 50 * 18
FRAMECNT ?= 900
frame1st := 1
framecnt := $(FRAMECNT)
# for ffmpeg input format; e.g. -i foo%0$(ffdigits)d.png
ffdigits := $(shell T=$(framecnt); echo $${\#T})
ifdef MULTIJOBS
J1S := $(frame1st)
JINCR := $(shell echo "scale=3; t=($(framecnt)-$(frame1st)+1)/4+0.5; scale=0; t/1" | bc)
J1E := $(shell echo $$(( $(J1S) + $(JINCR) - 1)) )
J2S := $(shell echo $$(( $(J1E) + 1)) )
J2E := $(shell echo $$(( $(J1E) + $(JINCR) )) )
J3S := $(shell echo $$(( $(J2E) + 1)) )
J3E := $(shell echo $$(( $(J2E) + $(JINCR) )) )
J4S := $(shell echo $$(( $(J3E) + 1)) )
J4E := $(framecnt)
# for success tests in rules (because povray exits 0 on error!)
N1S := $(shell printf "%0$(ffdigits)u" $(J1S) )
N1E := $(shell printf "%0$(ffdigits)u" $(J1E) )
N2S := $(shell printf "%0$(ffdigits)u" $(J2S) )
N2E := $(shell printf "%0$(ffdigits)u" $(J2E) )
N3S := $(shell printf "%0$(ffdigits)u" $(J3S) )
N3E := $(shell printf "%0$(ffdigits)u" $(J3E) )
N4S := $(shell printf "%0$(ffdigits)u" $(J4S) )
N4E := $(shell printf "%0$(ffdigits)u" $(J4E) )
endif # MULTIJOBS

# testing mode - passed into POV SDL
TESTING ?= 0

# just make single image
SINGLE ?= 0

# cyclic animation default (+ on, - off):
anicyc = +KC
# cyclic overrides from environment:
ifdef NO_CYCLIC_ANIM
anicyc = -KC
endif

ifndef ANIMOPTS
frameini = +KFI$(frame1st)
framelast = +KFF$(framecnt)
clockini = +KI0.0
clocklast = +KF1.0
aniaddl = $(anicyc)
animopts = $(frameini) $(framelast) $(clockini) $(clocklast) $(aniaddl)
J1animopts= $(animopts) +SF$(J1S) +EF$(J1E)
J2animopts= $(animopts) +SF$(J2S) +EF$(J2E)
J3animopts= $(animopts) +SF$(J3S) +EF$(J3E)
J4animopts= $(animopts) +SF$(J4S) +EF$(J4E)
else
animopts = $(ANIMOPTS)
endif

# render to DVD pixel sizes? (display aspect is maintained)
# use DVD_SOURCE=alt for 704 width, use DVD_SOURCE=pal for 576 rows
# use DVD_SOURCE=alt-pal for both latter and former; anything else
# is 720x480 (or 352x240)
Hsm         = 240
Hmed        = 480
Hlg         = 480
ifdef DVD_SOURCE
Wsm   = 352
Wmed  = 720
Wlg   = 720
ifneq (,$(findstring alt,$(DVD_SOURCE)))
Wmed  = 704
Wlg   = 704
endif
ifneq (,$(findstring pal,$(DVD_SOURCE)))
Hsm         = 288
Hmed        = 576
Hlg         = 576
endif
else
Wsm   = 320
Wmed  = 640
Wlg   = 854
endif

szsm        = +W$(Wsm)   +H$(Hsm)
szmed       = +W$(Wmed)  +H$(Hmed)
szlg        = +W$(Wlg)   +H$(Hlg)
# large MP4, uses only frame rate from ENCODE_TYPE
szxlg       = +W1600     +H900
# "WEBfinal: image with opts $(szweb) $(finalflags)"
szweb       = +W500      +H500

# Opts for optional last stage ffmpeg encode
#
# encoding type (for ff*{deval,final} targets):
# ntsc | ntsc-film | pal | pal-film, where the *-film
# variants encode at 24 fps
Wlo = 352
Wst = 720
ifneq (,$(findstring 704,$(ENCODE_TYPE)))
Wst = 704
endif
ifneq (,$(findstring pal,$(ENCODE_TYPE)))
Hlo         = 288
Hst         = 576
ffrate      = 25
target      = pal-dvd
gop_arg     = -g 15
bf_arg      = -bf 2
else
#default to ntsc type
Hlo         = 240
Hst         = 480
ffrate      = 30000/1001
target      = ntsc-dvd
gop_arg     = -g 18
bf_arg      = -bf 2
endif

VBITRATE ?= 7680k
vbitrate = $(VBITRATE)

ifneq (,$(findstring film,$(ENCODE_TYPE)))
ffrate      = 24
endif

# Povray animation frame rate, and ffmpeg input rate
# default NTSC (note that if this does not match ENCODE_TYPE
# then ffmpeg must convert sizes and frame rate)
RNUMER = 30000
RDENOM = 1001
ffinrate = 30000/1001
# allow override for film
ifdef ANI_FILM
RNUMER = 24
RDENOM = 1
ffinrate = 24
endif
# allow override for PAL
ifdef ANI_PAL
RNUMER = 25
RDENOM = 1
ffinrate = 25
endif

# this build does not handle audio encoding; but,
# a command line override may be give to include
# audio aruments to ffmpeg, e.g.:
# 'make AUDIO_INP="-i foo.wav"  AUDIO_ARGS="-acodec ac3 -ac 2 -ar 48000 -b:a 192k"'
AUDIO_ARGS := -an

ffargs1 = $(AUDIO_INP) -target $(target) -r $(ffrate)
ffargs2 = -b:v $(vbitrate) $(bf_arg) $(gop_arg) $(AUDIO_ARGS)

ifdef SCROLL_TEXT
ifndef SCROLL_LINES
scrll := $(shell wc -l < $(SCROLL_TEXT))
else
scrll = $(SCROLL_LINES)
endif
ifdef SCROLL_PAD
scpad = $(SCROLL_PAD)
else
scpad = 11
endif
ifndef FONTF
FONTF = FreeForm721_black.ttf
endif
TXTCOLOR ?= 0x6F10DF@0.98
txclr = $(TXTCOLOR)
SHADOWCOLOR ?= 0x2A033F@0.4
shclr = $(SHADOWCOLOR)

# Note that parameters in the following might need adjustment per font!
scr_y := ($(scrll)*lh+h+$(scpad)*2)*n/$(framecnt)*-1+h+$(scpad)
txt_x := (w-tw)/2
ffvfmsm       = -vf "drawtext=fontsize=14:fontcolor=$(txclr):shadowcolor=$(shclr):shadowx=1.07:shadowy=1.6:fontfile=$(FONTF):textfile=$(SCROLL_TEXT):x=$(txt_x):y=$(scr_y),scale=$(Wlo):$(Hlo)"
ffvfmmed      = -vf "drawtext=fontsize=28:fontcolor=$(txclr):shadowcolor=$(shclr):shadowx=2.13:shadowy=3.2:fontfile=$(FONTF):textfile=$(SCROLL_TEXT):x=$(txt_x):y=$(scr_y),scale=$(Wst):$(Hst)"
ffvfmlg       = -vf "drawtext=fontsize=28:fontcolor=$(txclr):shadowcolor=$(shclr):shadowx=2.13:shadowy=3.2:fontfile=$(FONTF):textfile=$(SCROLL_TEXT):x=$(txt_x):y=$(scr_y),scale=$(Wst):$(Hst)"
ffvfmxlg      = -vf "drawtext=fontsize=52.5:fontcolor=$(txclr):shadowcolor=$(shclr):shadowx=4:shadowy=6:fontfile=$(FONTF):textfile=$(SCROLL_TEXT):x=$(txt_x):y=$(scr_y)"
ffvfmweb      = -vf "drawtext=fontsize=28:fontcolor=$(txclr):shadowcolor=$(shclr):shadowx=2.13:shadowy=3.2:fontfile=$(FONTF):textfile=$(SCROLL_TEXT):x=$(txt_x):y=$(scr_y),scale=$(Wst):$(Hst)"
else
ffvfmsm       = -vf scale=$(Wlo):$(Hlo)
ffvfmmed      = -vf scale=$(Wst):$(Hst)
ffvfmlg       = -vf scale=$(Wst):$(Hst)
ffvfmxlg      = 
ffvfmweb      =
endif

ffmsm       = $(ffargs1) -s $(Wsm)x$(Hsm) $(ffvfmsm) -aspect 4:3 $(ffargs2)
ffmmed      = $(ffargs1) -s $(Wmed)x$(Hmed) $(ffvfmmed) -aspect 4:3 $(ffargs2)
ffmlg       = $(ffargs1) -s $(Wlg)x$(Hlg) $(ffvfmlg) -aspect 16:9 $(ffargs2)
ffmxlg      = -vcodec libx264 -r $(ffrate) -s 1600x900 $(ffvfmxlg) -aspect 16:9 -b:v 8000k $(bf_arg) -an
ffmweb      = -vcodec libx264 -r $(ffrate) -s 500x500 $(ffvfmweb) -aspect 1 -b:v 2000k $(bf_arg) -an

develflags   = +Q4 -A
finalflags   = +Q9 +A0.1 -J

ifndef ADDLPOVOPTS
# add visual arg for compositing managers that make pov window transparent
# NO +P for animation!
#ADDLPOVOPTS  = +P
ADDLPOVOPTS  =
endif

ifdef FINAL
FINAL = 1
else
FINAL = 0
endif

SRCS         = $(namepov).$(povext)
DEPS         = boardspace-path.inc \
				path.inc \
				animmisc.inc \
				cubic_bezier.inc

# generated data file to be read within POV SDL file
GEN_DATA_D = gen
GEN_DATA_0 = $(GEN_DATA_D)/dat0
GEN_DATA_1 = $(GEN_DATA_D)/dat1
GEN_DATA   = $(GEN_DATA_0) $(GEN_DATA_1)

finalimg     = $(nameout).SD.final.$(nameext)

all: listtargets

listtargets:
	@echo "traceimage: make Epspline background image, 600x600"
	@echo "LDdevel: image with opts $(szsm) $(develflags)"
	@echo "LDfinal: image with opts $(szsm) $(finalflags)"
	@echo "SDdevel: image with opts $(szmed) $(develflags)"
	@echo "SDfinal: image with opts $(szmed) $(finalflags)"
	@echo "WSDdevel: image with opts $(szlg) $(develflags)"
	@echo "WSDfinal: image with opts $(szlg) $(finalflags)"
	@echo "HD1devel: image with opts $(szxlg) $(develflags)"
	@echo "HD1final: image with opts $(szxlg) $(finalflags)"
	@echo "WEBfinal: image with opts $(szweb) $(finalflags)"
	@echo "tst: make test image, tst2: alternative test image"
	@echo "clean: remove frame images, generated data"
	@echo "allclean: clean, plus remove ffmpeg output"
	@echo
	@echo "To have $(ffm) encode the frames (on success),"
	@echo "prefix any of the above with 'ff', e.g. 'ffLDdevel'"
	@echo
	@echo Further control through parameters in the environment
	@echo or on the make command line \(which overrides internal
	@echo assignments\):
	@echo
	@echo UMEMMAX in KiB argument to ulimit -[mv]: "$(UMEMMAX)"
	@echo NO_ULIMIT disable ulimit setting
	@echo NAMEBASE base name of POV SDL input file and output prefix
	@echo FRAMEDIR directory for output of animation frames: \"$(FRAMEDIR)\"
	@echo FRAMECNT number of frames to render: \"$(FRAMECNT)\"
	@echo MULTIJOBS \(ifdef\) whether to use multiple processes
	@echo TESTING passed to SDL
	@echo PREVIEW show preview window '(+D)'
	@echo FINAL - boolean - intended to be final '(finished)' render
	@echo SINGLE make single test image '(not impl.: use tartget tst)'
	@echo NO_CYCLIC_ANIM elide povray -KC option for looping animations
	@echo ADDLPOVOPTS optional additional options for POVRay
	@echo DVD_SOURCE '(= {,{,alt-{pal,ntsc}}})' render to DVD sizes:
	@echo '  ' with alt- use reduces width of 704 '(rather than 720);'
	@echo '  '  default is ntsc
	@echo ANI_FILM or ANI_PAL '(ifdef)' povray animation render frame rate
	@echo ENCODE_TYPE ffmpeg encoding target:
	@echo '  '  'ntsc | ntsc-film | pal | pal-film;' with -film use 24 fps
	@echo VBITRATE ffmpeg encoding video bitrate: "$(VBITRATE)"
	@echo AUDIO_ARGS audio ready for ffmpeg, use these: \"$(AUDIO_ARGS)\"
	@echo AUDIO_INP optional audio input for ffmpeg, e.g.:
	@echo '  ' 'make AUDIO_INP="-i foo.wav"  AUDIO_ARGS="-acodec ac3 -ac 2 -ar 48000 -b:a 192k"'
	@echo SCROLL_TEXT optional text file if animation is meant for
	@echo such, scrolls from bottom to top over length of animation
	@echo SCROLL_PAD some padding in pixels for optional scrolling text
	@echo FONTF font file for optional scrolling text
	@echo TXTCOLOR, SHADOWCOLOR, colors for scrolling text, e.g.:
	@echo '  ' '0x6F10DF@0.98' where alpha scale follows '@'
	@echo

mk_dir:
	test -d $(framedir) || { mkdir $(framedir) || \
		{ echo FAILED mkdir $(framedir) ; exit 1 ; } ; } ; \
	test -d $(GEN_DATA_D) || { mkdir $(GEN_DATA_D) || \
		{ echo FAILED mkdir $(GEN_DATA_D) ; exit 1 ; } ; } ;

$(GEN_DATA) : mk_dir
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0)
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1)
	printf '%s,%s' 4 3 >> $(GEN_DATA_1)

gen_data: $(GEN_DATA)

# Povray exits with status 0 even on error!
# Testing for the presence of output files sucks but will have to do.
ifdef MULTIJOBS
$(nameout).LD.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	($(pov) $(develflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	sleep 1; ($(pov) $(develflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	sleep 1; ($(pov) $(develflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	sleep 1; ($(pov) $(develflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).LD.devel$(N1E).$(nameext) && \
		test -f $(nameout).LD.devel$(N2E).$(nameext) && \
		test -f $(nameout).LD.devel$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).LD.devel$(N4E).$(nameext) $@
else
$(nameout).LD.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(develflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).LD.devel$(framecnt).$(nameext) $@
endif

LDdevel: $(nameout).LD.devel.$(nameext)

$(nameout).LD.devel.$(vidext): $(nameout).LD.devel.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).LD.devel%0$(ffdigits)d.$(nameext) \
		$(ffmsm) -y $@

ffLDdevel: $(nameout).LD.devel.$(vidext)

ifdef MULTIJOBS
$(nameout).SD.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	($(pov) $(develflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(develflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(develflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(develflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).SD.devel$(N1E).$(nameext) && \
		test -f $(nameout).SD.devel$(N2E).$(nameext) && \
		test -f $(nameout).SD.devel$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).SD.devel$(N4E).$(nameext) $@
else
$(nameout).SD.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(develflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).SD.devel$(framecnt).$(nameext) $@
endif

SDdevel: $(nameout).SD.devel.$(nameext)

$(nameout).SD.devel.$(vidext): $(nameout).SD.devel.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).SD.devel%0$(ffdigits)d.$(nameext) \
		$(ffmmed) -y $@

ffSDdevel: $(nameout).SD.devel.$(vidext)

ifdef MULTIJOBS
$(nameout).WSD.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	($(pov) $(develflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(develflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(develflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(develflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).WSD.devel$(N1E).$(nameext) && \
		test -f $(nameout).WSD.devel$(N2E).$(nameext) && \
		test -f $(nameout).WSD.devel$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).WSD.devel$(N4E).$(nameext) $@
else
$(nameout).WSD.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(develflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).WSD.devel$(framecnt).$(nameext) $@
endif

WSDdevel: $(nameout).WSD.devel.$(nameext)

$(nameout).WSD.devel.$(vidext): $(nameout).WSD.devel.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).WSD.devel%0$(ffdigits)d.$(nameext) \
		$(ffmlg) -y $@

ffWSDdevel: $(nameout).WSD.devel.$(vidext)

ifdef MULTIJOBS
$(nameout).HD1.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	($(pov) $(develflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(develflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(develflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(develflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).HD1.devel$(N1E).$(nameext) && \
		test -f $(nameout).HD1.devel$(N2E).$(nameext) && \
		test -f $(nameout).HD1.devel$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).HD1.devel$(N4E).$(nameext) $@
else
$(nameout).HD1.devel.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(develflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).HD1.devel$(framecnt).$(nameext) $@
endif

HD1devel: $(nameout).HD1.devel.$(nameext)

$(nameout).HD1.devel.$(vidextxlg): $(nameout).HD1.devel.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).HD1.devel%0$(ffdigits)d.$(nameext) \
		$(ffmxlg) -y $@

ffHD1devel: $(nameout).HD1.devel.$(vidextxlg)

ifdef MULTIJOBS
$(nameout).LD.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	($(pov) $(finalflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(finalflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(finalflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(finalflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).LD.final$(N1E).$(nameext) && \
		test -f $(nameout).LD.final$(N2E).$(nameext) && \
		test -f $(nameout).LD.final$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).LD.final$(N4E).$(nameext) $@
else
$(nameout).LD.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(finalflags) $(dflag) $(szsm) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).LD.final$(framecnt).$(nameext) $@
endif

LDfinal: $(nameout).LD.final.$(nameext)

$(nameout).LD.final.$(vidext): $(nameout).LD.final.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).LD.final%0$(ffdigits)d.$(nameext) \
		$(ffmsm) -y $@

ffLDfinal: $(nameout).LD.final.$(vidext)

ifdef MULTIJOBS
$(nameout).SD.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	($(pov) $(finalflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(finalflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(finalflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(finalflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).SD.final$(N1E).$(nameext) && \
		test -f $(nameout).SD.final$(N2E).$(nameext) && \
		test -f $(nameout).SD.final$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).SD.final$(N4E).$(nameext) $@
else
$(nameout).SD.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 4 3 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(finalflags) $(dflag) $(szmed) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).SD.final$(framecnt).$(nameext) $@
endif

SDfinal: $(nameout).SD.final.$(nameext)

$(nameout).SD.final.$(vidext): $(nameout).SD.final.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).SD.final%0$(ffdigits)d.$(nameext) \
		$(ffmmed) -y $@

ffSDfinal: $(nameout).SD.final.$(vidext)

ifdef MULTIJOBS
$(nameout).WSD.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	($(pov) $(finalflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(finalflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(finalflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(finalflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).WSD.final$(N1E).$(nameext) && \
		test -f $(nameout).WSD.final$(N2E).$(nameext) && \
		test -f $(nameout).WSD.final$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).WSD.final$(N4E).$(nameext) $@
else
$(nameout).WSD.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(finalflags) $(dflag) $(szlg) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).WSD.final$(framecnt).$(nameext) $@
endif

WSDfinal: $(nameout).WSD.final.$(nameext)

$(nameout).WSD.final.$(vidext): $(nameout).WSD.final.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).WSD.final%0$(ffdigits)d.$(nameext) \
		$(ffmlg) -y $@

ffWSDfinal: $(nameout).WSD.final.$(vidext)

ifdef MULTIJOBS
$(nameout).HD1.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	($(pov) $(finalflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(finalflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(finalflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(finalflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f $(nameout).HD1.final$(N1E).$(nameext) && \
		test -f $(nameout).HD1.final$(N2E).$(nameext) && \
		test -f $(nameout).HD1.final$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).HD1.final$(N4E).$(nameext) $@
else
$(nameout).HD1.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 16 9 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(finalflags) $(dflag) $(szxlg) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).HD1.final$(framecnt).$(nameext) $@
endif

HD1final: $(nameout).HD1.final.$(nameext)

$(nameout).HD1.final.$(vidextxlg): $(nameout).HD1.final.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).HD1.final%0$(ffdigits)d.$(nameext) \
		$(ffmxlg) -y $@

ffHD1final: $(nameout).HD1.final.$(vidextxlg)


ifdef MULTIJOBS
$(nameout).WEB.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	( $(ULIMIT) ; \
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 1 1 >> $(GEN_DATA_1) ; \
	($(pov) $(finalflags) $(dflag) $(szweb) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J1animopts); echo $$? > J1status) & \
	($(pov) $(finalflags) $(dflag) $(szweb) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J2animopts); echo $$? > J2status) & \
	($(pov) $(finalflags) $(dflag) $(szweb) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J3animopts); echo $$? > J3status) & \
	($(pov) $(finalflags) $(dflag) $(szweb) +I$(namepov).$(povext) \
	$(typeopt) +O$@ $(ADDLPOVOPTS) $(J4animopts); echo $$? > J4status) & \
		wait; grep '[^0]' J?status && exit 1; exit 0) && \
		test -f    $(nameout).WEB.final$(N1E).$(nameext) && \
		test -f    $(nameout).WEB.final$(N2E).$(nameext) && \
		test -f    $(nameout).WEB.final$(N3E).$(nameext) && \
		$(CPFORCE) $(nameout).WEB.final$(N4E).$(nameext) $@
else
$(nameout).WEB.final.$(nameext): mk_dir $(SRCS) $(DEPS)
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) $(TESTING) $(SINGLE) \
		> $(GEN_DATA_0) ; \
	printf '%s,%s,%s,' $(FINAL) $(RNUMER) $(RDENOM) > $(GEN_DATA_1) ; \
	printf '%s,%s' 1 1 >> $(GEN_DATA_1) ; \
	$(ULIMIT) ; $(pov) $(finalflags) $(dflag) $(szweb) +I$(namepov).$(povext) \
		$(typeopt) +O$@ $(ADDLPOVOPTS) $(animopts) && \
		$(CPFORCE) $(nameout).WEB.final$(framecnt).$(nameext) $@
endif

WEBfinal: $(nameout).WEB.final.$(nameext)

$(nameout).WEB.final.$(vidextxlg): $(nameout).WEB.final.$(nameext)
	$(ffm) -r $(ffinrate) -i $(nameout).WEB.final%0$(ffdigits)d.$(nameext) \
		$(ffmweb) -y $@

ffWEBfinal: $(nameout).WEB.final.$(vidextxlg)

traceimage:
	$(pov) \
		+P +D +A0.3 +Q9 +W600 +H600 \
		+I${namebase}.pov +O${namebase}-600x600

TSTCMD=+D +X3 +P +FN +A0.7 +W$(Wmed) +H480 -visual DirectColor
TST2CMD=+D +X3 +P +FN +A0.1 +W$(Wlg) +H480 -visual DirectColor

test: tst.png
#	display tst.png
test2: tst2.png
#	display tst2.png
# $(SRCS) $(DEPS)
tst tst.png: mk_dir
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) 1 1 > $(GEN_DATA_0)
	printf '%s,%s,%s,%s,%s' 0 24 1 4 3 > $(GEN_DATA_1)
	$(pov) $(TSTCMD) $(ADDLPOVOPTS) +I$(namepov).$(povext) +O$@
# $(SRCS) $(DEPS)
tst2 tst2.png: mk_dir
	printf '%s,%s,%s,%s' $(frame1st) $(framecnt) 1 1 > $(GEN_DATA_0)
	printf '%s,%s,%s,%s,%s' 1 24 1 16 9 > $(GEN_DATA_1)
	$(pov) $(TST2CMD) $(ADDLPOVOPTS) +I$(namepov).$(povext) +O$@


cleangen:
	$(RMFORCE) $(GEN_DATA)

clean: cleangen
	$(RMFORCE) $(nameout).*.devel*.$(nameext) \
		$(nameout).*.devel*.$(vidext) J?status tst*.png

allclean: clean
	$(RMFORCE) $(nameout).$(nameext) $(nameout).*.*.$(nameext) \
		$(nameout).$(vidext) $(nameout).*.*.$(vidext) \
		$(nameout).$(vidext) $(nameout).*.*.mp4 J?status
