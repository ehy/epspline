#! /usr/bin/make -f
# POSIX makefile for epspline
# 1st non-comment, 1st target w/o dependencies or commands:
.POSIX:

RM  = rm -f
CP  = cp -f
CAT = cat
# Hmm, had forgotten MKDIR, but it was def'd on all but NetBSD
MKDIR = mkdir -p
# posix make defines YACC and LEX
#YACC =  yacc
#YACC =  byacc
#YACC =  bison -y
#LEX  =  lex
#LEX  =  flex -l
#LEX  =  flex
# Linux special: try to get new flex to produce working code:
#LEXGLNX  =  flex -l
#LEXGLNX  =  flex -X
CCLEX=  $(CXX)
#CCLEX=  c++
LEX_STEM = lex.yy
GREP = grep
SORT = sort
UNIQ = uniq

# Maintain this file in the install target
# for use in the uninstall target
UNINST_LIST = ../uninstall/src_uninstall-list

# parser will not build on some systems, e.g. OpenSolaris;
# add the uname of such systems here, and working (hopefully)
# files will be copied into place rather than generated
# Linux is included because lex is likely to be a symlink
# to flex, and new flex generates code that causes infinite loops,
# or segfaults; old flex works (e.g. Debian package 'flex-old'),
# but it seems unlikely to be the installed version.
#LEXYACCFAIL = SunOS
# New try of new flex (flex -l||-X); if n.g. then re-add Linux
# (Dec 28, 2012) -- N.G. segfault w/ -l, infinite loop w/ -X
# (Mar 5, 2015) -- N.G. NetBSD 7.0beta (last tried was 5.0): spins
LEXYACCFAIL = SunOS Linux
LEXYACCOK   = OpenBSD FreeBSD NetBSD

# PARSERCFLAGS are used in the parser.o
# compile; the _LINUX def is inherited with the lexer and parser
# and wxExio code pulled from an old version of wxWindows --
# it's not actually only used on GNU/Linux here.
PARSERCFLAGS = $(CCFLAGS) -D_LINUX=1

OBJDIR = .
SRCDIR = .
HDRDIR = .
COMMDIR = .
GENSRCDIR = $(SRCDIR)/../wxxrc
CDATADIR  = $(SRCDIR)/../cdata
# License header, e.g. to prepend generated files
LICENSEHEAD = $(CDATADIR)/license.c.head

# prebuilt files
OKLEXER = $(SRCDIR)/ms_lexer.c
OKPARSE = $(SRCDIR)/ms_parse.c

HDRS = $(HDRDIR)/a_canvas.h $(HDRDIR)/a_frame.h $(HDRDIR)/a_ruler.h \
	$(HDRDIR)/cfg.h $(HDRDIR)/splines.h $(HDRDIR)/io.h \
	$(HDRDIR)/stdcc.h $(HDRDIR)/util.h $(HDRDIR)/epspline.h \
	$(HDRDIR)/wxutil.h $(HDRDIR)/a_tabwnd.h $(HDRDIR)/a_tabpage.h \
	$(HDRDIR)/a_zoomdlg.h $(HDRDIR)/a_spintextdlg.h \
	$(HDRDIR)/a_textctldlg.h \
	$(HDRDIR)/a_aboutbox.h \
	$(HDRDIR)/a_prefs_manager.h \
	$(HDRDIR)/a_glbprefsdlg.h \
	$(HDRDIR)/spline_props.h \
	$(HDRDIR)/globlal_prefs.h \
	$(HDRDIR)/bg_image_dlg.h \
	$(HDRDIR)/a_bgimg_manager.h \
	$(HDRDIR)/wxexio.h

SRCS = $(SRCDIR)/epspline.cpp $(SRCDIR)/io.cpp $(SRCDIR)/a_canvas.cpp \
	$(SRCDIR)/splines.cpp $(SRCDIR)/splineprops.cpp $(SRCDIR)/util.cpp \
	$(SRCDIR)/a_frame.cpp $(SRCDIR)/a_ruler.cpp $(SRCDIR)/povdemo.cpp \
	$(SRCDIR)/a_tabwnd.cpp $(SRCDIR)/a_tabpage.cpp \
	$(SRCDIR)/a_zoomdlg.cpp $(SRCDIR)/a_spintextdlg.cpp \
	$(SRCDIR)/a_textctldlg.cpp \
	$(SRCDIR)/a_aboutbox.cpp \
	$(SRCDIR)/a_prefs_manager.cpp \
	$(SRCDIR)/a_glbprefsdlg.cpp \
	$(SRCDIR)/spline_props.cpp \
	$(SRCDIR)/globlal_prefs.cpp \
	$(SRCDIR)/bg_image_dlg.cpp \
	$(SRCDIR)/a_bgimg_manager.cpp \
	$(SRCDIR)/wxexio.cpp

OBJECTS = $(OBJDIR)/epspline.o $(OBJDIR)/io.o $(OBJDIR)/a_canvas.o \
	$(OBJDIR)/splines.o $(OBJDIR)/splineprops.o $(OBJDIR)/util.o \
	$(OBJDIR)/a_frame.o $(OBJDIR)/a_ruler.o $(OBJDIR)/povdemo.o \
	$(OBJDIR)/a_tabwnd.o $(OBJDIR)/a_tabpage.o $(OBJDIR)/a_zoomdlg.o \
	$(OBJDIR)/a_textctldlg.o \
	$(OBJDIR)/a_spintextdlg.o \
	$(OBJDIR)/a_aboutbox.o \
	$(OBJDIR)/a_prefs_manager.o \
	$(OBJDIR)/a_glbprefsdlg.o \
	$(OBJDIR)/spline_props.o \
	$(OBJDIR)/globlal_prefs.o \
	$(OBJDIR)/bg_image_dlg.o \
	$(OBJDIR)/a_bgimg_manager.o \
	$(OBJDIR)/wxexio.o \
	$(OBJDIR)/parser.o

all: $(OBJDIR) epspline$(EXESUFFIX)

$(APPLIB) applib:
	@exit 0

$(OBJDIR):
	@test -d $(OBJDIR) || mkdir $(OBJDIR)

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) $(CCFLAGS) -c -o $@ $<


$(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/lexer.c
	$(CCLEX) -c $(PARSERCFLAGS) -I. -o $@ $(SRCDIR)/parser.c

$(SRCDIR)/parser.c: $(COMMDIR)/parser.y $(SRCDIR)/lexer.c
	@$(RM) y.tab.c
	@if ! echo $(LEXYACCOK) | $(GREP) -q `uname`; then \
	 echo Using pre-built $@ ; \
	 $(CP) $(OKPARSE) $@ ; \
	else \
	 echo generating $@ ; \
	 $(YACC) $(COMMDIR)/parser.y && \
	 sed -e "s;$(COMMDIR)/y.tab.c;parser.y;g" < y.tab.c | \
	 sed -e "s/BUFSIZ/5000/g"            | \
	 sed -e "s/YYLMAX 200/YYLMAX 5000/g" | \
	 sed -e "s/yy/EPSIO_yy/g"            | \
	 sed -e "s/input/EPSIO_input/g"      | \
	 sed -e "s/unput/EPSIO_unput/g"      > $@ && \
	 echo Succeeded with $@ ; \
	fi

$(SRCDIR)/lexer.c: $(COMMDIR)/lexer.l
	@$(RM) $(LEX_STEM).c
	@if ! echo $(LEXYACCOK) | $(GREP) -q `uname`; then \
	 echo Using pre-built $@ ; \
	 $(CP) $(OKLEXER) $@ ; \
	else \
	 echo generating $@ ; \
	 test XLinux = X"`uname`" && LX="$(LEXGLNX)" || LX="$(LEX)" ; \
	 echo using lexer command \""$$LX"\" ; \
	 $$LX $(COMMDIR)/lexer.l && \
	 sed -e "s;$(COMMDIR)/$(LEX_STEM).c;lexer.l;g" < $(LEX_STEM).c | \
	 sed -e "s/yy/EPSIO_yy/g"            | \
	 sed -e "s/input/EPSIO_input/g"      | \
	 sed -e "s/unput/EPSIO_unput/g"      > $@ && \
	 echo Succeeded with $@ ; \
	fi

# Sources generated with e.g. a GUI dialog editor, kept in other
# directory, concatenated with licence to make file here
$(HDRDIR)/spline_props.h : $(LICENSEHEAD) $(GENSRCDIR)/spline_props.h
	$(CAT) $(LICENSEHEAD) $(GENSRCDIR)/spline_props.h \
		> $(HDRDIR)/spline_props.h

$(SRCDIR)/spline_props.cpp : $(LICENSEHEAD) $(GENSRCDIR)/spline_props.cpp
	$(CAT) $(LICENSEHEAD) $(GENSRCDIR)/spline_props.cpp \
		> $(SRCDIR)/spline_props.cpp

$(OBJDIR)/spline_props.o : $(SRCDIR)/spline_props.cpp $(HDRDIR)/spline_props.h

$(HDRDIR)/globlal_prefs.h : $(LICENSEHEAD) $(GENSRCDIR)/globlal_prefs.h
	$(CAT) $(LICENSEHEAD) $(GENSRCDIR)/globlal_prefs.h \
		> $(HDRDIR)/globlal_prefs.h

$(SRCDIR)/globlal_prefs.cpp : $(LICENSEHEAD) $(GENSRCDIR)/globlal_prefs.cpp
	$(CAT) $(LICENSEHEAD) $(GENSRCDIR)/globlal_prefs.cpp \
		> $(SRCDIR)/globlal_prefs.cpp

$(OBJDIR)/globlal_prefs.o : $(SRCDIR)/globlal_prefs.cpp $(HDRDIR)/globlal_prefs.h

$(HDRDIR)/bg_image_dlg.h : $(LICENSEHEAD) $(GENSRCDIR)/bg_image_dlg.h
	$(CAT) $(LICENSEHEAD) $(GENSRCDIR)/bg_image_dlg.h \
		> $(HDRDIR)/bg_image_dlg.h

$(SRCDIR)/bg_image_dlg.cpp : $(LICENSEHEAD) $(GENSRCDIR)/bg_image_dlg.cpp
	$(CAT) $(LICENSEHEAD) $(GENSRCDIR)/bg_image_dlg.cpp \
		> $(SRCDIR)/bg_image_dlg.cpp

$(OBJDIR)/bg_image_dlg.o : $(SRCDIR)/bg_image_dlg.cpp $(HDRDIR)/bg_image_dlg.h

# GNU gperf is used to make a hashed set of pov reserved words
# which have been gathered from pov docs.  These words are in a
# file, one word per line, named pov_reserved_words.text, gperf's
# output is pov_reserved_words.cpp (subject to change of course)
# note that pov_reserved_words.cpp is not compiled alone, it is
# included in util.cpp
POV_WORDS_NAM = pov_reserved_words
POV_WORDS_3_6 = $(SRCDIR)/pov-3.6_reserved_words.text
POV_WORDS_3_7 = $(SRCDIR)/pov-3.7_reserved_words.text
POV_WORDS_SRC = $(SRCDIR)/$(POV_WORDS_NAM).text
POV_WORDS_CPP = $(SRCDIR)/$(POV_WORDS_NAM).cpp
$(POV_WORDS_CPP) : $(POV_WORDS_SRC)
	GP=`which gperf` && { $(RM) $(POV_WORDS_CPP); \
	$$GP --output-file=$(POV_WORDS_CPP) --language=C++ \
	--lookup-function-name=pov_reserved_words_hash \
	--class-name=pov_reserved_words --seven-bit --compare-lengths \
	--readonly-tables --enum --includes $(POV_WORDS_SRC) || exit 1; }
	@echo Succeeded with $@

$(POV_WORDS_SRC) : $(POV_WORDS_3_6) $(POV_WORDS_3_7)
	$(CAT) $(POV_WORDS_3_6) $(POV_WORDS_3_7) \
		| $(SORT) | $(UNIQ) \
		> $(POV_WORDS_SRC)
	@echo Succeeded with $@

# An additional dependency:
util.o : $(POV_WORDS_CPP)

# Changes to sources imply possible changes to strings subject to i18n.
# Only the .pot file is made here; merging, compilation to .mo files
# is done elsewhere.  The GNU gettext programs are used, but since
# a user compiling this source might not have them installed this
# target will not result in failure; only a helpful message.
# Depend on "$(SRCS) $(HDRS)" but argue "$(SRCDIR)/*.h $(SRCDIR)/*.cpp"
# to xgettext, since sources macros may include e.g. parser .c files.
MESSAGES_POT = $(SRCDIR)/epspline.pot
XGETTEXT     = xgettext
$(MESSAGES_POT) : $(SRCS) $(HDRS)
	@echo Invoking $(XGETTEXT) to make $(MESSAGES_POT).
	@$(XGETTEXT) --output=$(MESSAGES_POT) --debug --add-comments \
	--keyword=_ --keyword=_HKS --c++ $(SRCDIR)/*.h $(SRCDIR)/*.cpp && \
	echo Succeeded with $@ || \
	{ echo FAILED to make the i18n template $(MESSAGES_POT); \
	echo If you care about translations then check that \
	GNU gettext package is installed; exit 0; }

# target for binaries:
# Note $(EXESUFFIX) here is *not* for MSW .exe suffix, it is for
# _dbg or so on.
epspline$(EXESUFFIX):  $(APPLIB) $(OBJECTS) $(MESSAGES_POT)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(APPLIB) $(LIBS)

# This is for MSW binary -- assuming build with mingw cross tools,
# which work much like GCC.
# The difference here is that a resource binary needs compiling,
# and that file must also be linked in.  (also, mingw linker does
# *not* accept the .def file wanted by MSW linkers)
epspline$(EXESUFFIX).exe:  $(OBJDIR) mingw_l_y $(APPLIB) $(OBJECTS) $(OBJDIR)/epspline.res $(MESSAGES_POT)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(OBJDIR)/epspline.res \
		$(APPLIB) $(LIBS)
	@echo Succeeded building MSW binary $@

# A convenience target for the above
mingw: epspline$(EXESUFFIX).exe

# make MSW resource binary -- note the attempt to find the resource
# compiler if $(MSWRCC) is empty assumes tool name pattern of
# typical mingw installations
# (Note *BSD make will not expand '$<', posix or not)
$(OBJDIR)/epspline.res:  $(SRCDIR)/epspline.rc
	@RCC="$(MSWRCC)"; RC="$(SRCDIR)/epspline.rc"; \
	if test X"$$RCC" = X ; then \
		{ C="$(CXX)"; RCC="$${C%-*}-windres"; \
		type "$$RCC" || { echo FAILED trying \"$$RCC\"; exit 1; } ; \
		} ; \
		echo "$$RCC" --output-format=coff $$($(CF) --cppflags) $$RC $@ ; \
		"$$RCC" --output-format=coff $$($(CF) --cppflags) $$RC $@ ; \
	else \
		echo $(MSWRCC) --output-format=coff $$RC $@ ; \
		$(MSWRCC) --output-format=coff $$RC $@ ; \
	fi
	@echo Succeeded compiling MSW binary resource $@

# another hack for mingw: do not use lex/yacc even on systems
# on which they work for native builds -- they are not linking
# on FreeBSD -- so place this before $(OBJECTS) dependency
mingw_l_y:
	@test -f "$(SRCDIR)/parser.c" || { \
		echo Using pre-built "$(SRCDIR)/parser.c" for mingw ; \
		$(CP) $(OKPARSE) "$(SRCDIR)/parser.c"; }
	@test -f "$(SRCDIR)/lexer.c" || { \
		echo Using pre-built "$(SRCDIR)/lexer.c" for mingw ; \
		$(CP) $(OKPARSE) "$(SRCDIR)/lexer.c"; }



# More dirty hackery: Sun CC will accept some GCC options, although
# CC(1) does not mention it. E.g., accepts -O2 (vs. -xO2), and,
# -MM to make dependencies.  But, Sun CC -MM *compiles* along with
# the deps, unlike GCC which just prints dependencies on stdout.
# It seems -xM1 is the Sun CC option wanted here, so there is some
# shell code in the rule to check if CXX == CC and `uname` == sUNos
# else fall back to -MM. Setting variable DEPSOPT is the best bet.
# One more bit of help: CXXDEPS can be overridden on commandline.
CXXDEPS = $(CXX)
# depend-raw: $(SRCS) $(HDRS)
depend-raw:
	MM="$(DEPSOPT)"; test X = X"$$MM" && \
	{ CC="$(CXXDEPS)"; CC=$${CC##*/}; CC="$${CC%% *}"; \
		test XSunOS = X"$$(uname)" && \
		test X"$$CC" = XCC && MM="-xM1" || MM="-MM"; } ; \
	$(CXXDEPS) $(CCFLAGS) $$MM $(SRCS) | \
	sed 's/^[^:[:space:]]\{1,\}[[:space:]]*:/$$(OBJDIR)\/&/' > $@

# No capable tools on MSW to make dependency file.  Use Unix tools.
# Actually, GNU tools, grep and sed are used below; word-edge-anchors
# '\<' and '\>' are used and they are not POSIX -- failure likely
# except on GNU system or with GNU tools.
dep depend dep.msw depend.msw: depend-raw
	grep -E '(^\$$.+:| \<[^ /]+\.*(h|xpm|inc)\>)' < depend-raw | \
	sed -e 's;^\$$;\?\?&;' \
	-e 's;[ ]/[^ ]\{1,\}\.[^\. /]\{1,\}\>;;g' | tr '?' '\n' \
	> depend && \
	grep -E '(^\$$.+:| \<[^ /]+\.*(h|xpm|inc)\>)' < depend | \
	sed -e 's;^\$$;\?\?&;' \
	-e 's;[ ]/[^ ]\{1,\}\.[^\. /]\{1,\}\>;;g' \
	-e 's;\.o\>;\.obj;g' -e 's;/;\\;g' | tr '?' '\n' > depend.msw


# This target written for ``Exuberant Ctags.'' [new versions]
# See http://ctags.sourceforge.net
exuberant-tags : $(SRCS) $(HDRS)
	ctags --c-types=cdefgmnpstuv-x --excmd=pattern *

# This target written for ``Exuberant Ctags.'' -p no longer recognized,
# nor --c-types=*+A+C
# See http://ctags.sourceforge.net
old-exuberant-tags : $(SRCS) $(HDRS)
	ctags -p $(SRCDIR) -p $(HDRDIR) --c-types=cdefgmnpstuv-x+A+C \
	--excmd=pattern *

# This target written for BSD ctags
tags : $(SRCS) $(HDRS)
	$(RM) tags
	find $(SRCDIR) \( -name \*.cpp -o -name \*.h \) -print | \
		xargs ctags -adtw

# This target written for ``Exuberant Ctags.''
# See http://ctags.sourceforge.net
TAGS : $(SRCS) $(HDRS)
	ctags -e -p $(SRCDIR) -p $(HDRDIR) --c-types=cdefgmnpstuv-x+A+C *

# This target for GNU idutils
ID : $(SRCS) $(HDRS)
	mkid --prune tmp --output=$@


install: epspline$(EXESUFFIX)
	@$(RM) $(UNINST_LIST)-tmp || exit 0
	@if test -e $(UNINST_LIST); then \
		$(CP) -p $(UNINST_LIST) $(UNINST_LIST)-tmp; \
	fi
	@td="$(INSTALLDIR)"; IF="epspline$(EXESUFFIX)"; F="$$td/$$IF"; \
	test -e "$$td" && test ! -d "$$td" && \
		{ echo Found \""$$td"\" is not a directory; \
		echo NOT installing \""$$F"\"; exit 1; } ; \
	test -d "$$td" || $(MKDIR) "$$td" || \
		{ echo CANNOT $(MKDIR) \""$$td"\"; exit 1; } ; \
	echo DOING $(INSTALLCMD) $(INSTALLOPTS) "$$IF" "$$td"; \
	$(INSTALLCMD) $(INSTALLOPTS) "$$IF" "$$td" && \
		echo "$$F" >> $(UNINST_LIST)-tmp || \
		{ echo FAILED installing "$$F"; exit 1; } ; \
	$(SORT) < $(UNINST_LIST)-tmp | $(UNIQ) > $(UNINST_LIST) && \
		{ $(RM) $(UNINST_LIST)-tmp; exit 0; }
	@echo "Succeeded $@"

# Remove files listed in $(UNINST_LIST)
uninstall:
	@test -f $(UNINST_LIST) || \
		{ echo Uninstall list \"$(UNINST_LIST)\" is missing; exit 1; }
	@while read -r F; do \
		echo Uninstalling '(removing)' \""$$F"\"; \
		$(RM) "$$F"; \
	done < $(UNINST_LIST) && \
		:> $(UNINST_LIST)


clean:
	-$(RM) $(OBJDIR)/*.o \
	epspline$(EXESUFFIX) \
	epspline$(EXESUFFIXDBG) \
	epspline$(EXESUFFIX).exe \
	epspline$(EXESUFFIXDBG).exe \
	$(OBJDIR)/epspline.res \
	$(SRCDIR)/parser.c $(SRCDIR)/lexer.c \
	*core

cleandepend cleandepends:
	-$(RM) depend .depend depend-raw depend.msw

cleangenerated: clean
	-$(RM) $(SRCDIR)/lex.yy.c $(SRCDIR)/y.tab.c \
	$(SRCDIR)/parser.c $(SRCDIR)/lexer.c \
	$(POV_WORDS_CPP) $(MESSAGES_POT)

cleanall: clean cleandepends
	-$(RM) $(SRCDIR)/lex.yy.c $(SRCDIR)/y.tab.c tags TAGS ID

distclean: clean
	-$(RM) $(SRCDIR)/lex.yy.c $(SRCDIR)/y.tab.c tags TAGS ID
	:> $(UNINST_LIST)


#include depend
# posix neglected any 'include' facility, so to use generated
# dependencies give the depency file with -f, like:
#	make -f Makefile -f depend
#
