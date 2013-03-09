#! /usr/bin/make -f
# POSIX makefile for epspline
# 1st non-comment, 1st target w/o dependencies or commands:
.POSIX:

# some utilities used in this script:
SED = sed
CAT = cat
RM = rm -f
CP = cp -f
MV = mv

DIRS=src
# this MUST change if src/Makefile is renamed
MKFILE = Makefile

# the general root of an installtion -- frequently (but not
# necessarily) /usr/local on Unix-like systems
PREFIX = /usr/local
# an installer might prefer different prefixes for, e.g., data
BINPREFIX = $(PREFIX)
DATAPREFIX = $(PREFIX)
HELPDATAPREFIX = $(DATAPREFIX)
L10NPREFIX = $(DATAPREFIX)

# l10n/LC_MESSAGES install directory for system it's running on.
# For wx on Unix-like systems /usr/local/share/locale is
# the default, so this macro is is needed only if different
L10_INSTALL_DIR = $(L10NPREFIX)/share/locale
# wx doesn't mind a redundant optional search directory,
# but leave this empty by default anyway
#L10_INSTALL_DEF = -DL10N_INSTALL_PATH=\\\"$(L10_INSTALL_DIR)\\\"
L10_INSTALL_DEF =

INSTALLCMD = install
#INSTALLOPTS = -c -s -m 555 -o 0 -g 0
INSTALLOPTS = -c -s -m 555
INSTALLDIR = $(BINPREFIX)/bin

# GNU rejects MACRO = $(MACRO)
DEFINES = $(L10_INSTALL_DEF) -DINSTALL_PREFIX=\\\"$(DATAPREFIX)\\\"

YDEBUG = -g -DDEBUG=1
NDEBUG = -DNDEBUG=1
DEBUG = $(NDEBUG)
# wx_x is an ancient relic; this Makefile should be useful for
# MSW build w/, e.g., Mingw, MSYS, depending mostly on usable wx-config.
#GUI = -Dwx_x=1

# Feb  7 2009: -Wno-write-strings
# GCC g++ only (maybe also clang++):
# Hate to disable warnings, but recent g++ (4.x) is
# spamming deprecated conversion warnings from the wx* headers;
# so be it.
# OpenBSD: also, strict-aliasing spam
#WARN = -Wall -Wno-write-strings -Wno-strict-aliasing
# -Wunknown-pragmas added for clang++
# For development, -Wnon-virtual-dtor -Wextra are good:
GCCWARN = -Wall -Wno-write-strings -Wno-strict-aliasing \
	-Wno-unknown-pragmas \
	-Wnon-virtual-dtor -Wextra
WARN =

#FINAL = 0
FINAL = 1
EXESUFFIXDBG = _dbg
EXESUFFIX =

INCLUDES = -I. -I..
CFL = $(INCLUDES) $(OPT) $(CXXFLAGS) $(DEFINES)
#LFL = -Wl,-S
LFL=
LDFLAGS = $(LFL)

#CF = wxgtk-2.4-config
#CF = wxgtk2-2.4-config
#CF = wxmotif-2.4-config
#CF = wxx11univ-2.4-config
#CF = wx-config --version=2.6
CF = wx-config

# By default use compiler suggested by wx
CXX = `$(CF) --cxx`
CCFLAGS = `$(CF) --cxxflags` $(WARN) $(CFL) $(GUI)
LIBS = `$(CF) --libs`
# mingw only: MSW binary resource compiler
MSWRCC = `$(CF) --rescomp`

all: prog po examples doc

prog: depend
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) -f depend \
		CXX="$(CXX)" \
		CCFLAGS="$(CCFLAGS) $(NDEBUG)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIX)" LIBS="$(LIBS)" all ) || \
		{ echo IF build failed with complaints about unknown options; \
		echo such as -Wfoo, then edit the Makefile and comment the; \
		echo WARN = -W... macro, it is for GCC g++; exit 1; } ; \
	done
	@echo "Succeeded making program"

# target for MSW build with mingw
mingw exe msw epspline.exe: depend
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) -f depend \
		CXX="$(CXX)" MSWRCC="$(MSWRCC)" \
		CCFLAGS="$(CCFLAGS) $(NDEBUG)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIX)" LIBS="$(LIBS)" mingw ) || \
		{ echo IF build failed with complaints about unknown options; \
		echo such as -Wfoo, then edit the Makefile and comment the; \
		echo WARN = -W... macro, it is for GCC g++; exit 1; } ; \
	done
	@echo "Succeeded making program"

# Make deps: this target is unreliable since who knows what compiler
# is in use and how it is persuaded to generate depencies?
# One small bit of help: DEPSOPT="$(DEPSOPT)" -- DEPSOPT is
# not set in this Makefile. It must be set on commandline, e.g.:
#	make [...] DEPSOPT="-print_dependencies_but_do_not_compile"
# Even so, it's not certain the rule in src/Makefile has an
# acceptable syntax for any compiler.  User will find out.
# The default option is GCC -MM; the rule has shell code to check
# for CC and uname==SunOS.
# One more bit of help: CXXDEPS can be overridden on commandline.
CXXDEPS = $(CXX)
src/depend :
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXXDEPS="$(CXXDEPS)" DEPSOPT="$(DEPSOPT)" \
		CCFLAGS="$(CCFLAGS) $(NDEBUG)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIX)" LIBS="$(LIBS)" depend ) ; \
	done
	@echo "Succeeded making" $@

depend dep : src/depend
	@echo "Succeeded making" $@

Release: all

Debug: depend
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) -f depend \
		CXX="$(CXX)" \
		CCFLAGS="$(CCFLAGS) $(YDEBUG)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIXDBG)" LIBS="$(LIBS)" all ) ; \
	done
	@echo "Succeeded making in $(DIRS)"

junko:
	@echo loaded

# make, or install or clean, the translation message catalogs
# this target always exits success in case current system
# does not have msgfmt program (or any other error) and it
# should not prevent program build and install
po examples doc: junko
	@cd $@ && \
	$(MAKE) -f $(MKFILE) \
		PREFIX="$(PREFIX)" DATAPREFIX="$(DATAPREFIX)" all || \
		{ echo FAILED making in $@; exit 0; };

install_po uninstall_po clean_po cleanall_po distclean_po:
	@TT=$@; t=$${TT%_*}; D=$${TT##*_}; cd $$D && \
	$(MAKE) -f $(MKFILE) \
		PREFIX="$(PREFIX)" DATAPREFIX="$(DATAPREFIX)" $$t || \
		{ echo FAILED make $@; exit 0; };

install_examples uninstall_examples clean_examples cleanall_examples distclean_examples:
	@TT=$@; t=$${TT%_*}; D=$${TT##*_}; cd $$D && \
	$(MAKE) -f $(MKFILE) \
		PREFIX="$(PREFIX)" DATAPREFIX="$(DATAPREFIX)" $$t || \
		{ echo FAILED make $@; exit 0; };

install_doc uninstall_doc clean_doc cleanall_doc distclean_doc:
	@TT=$@; t=$${TT%_*}; D=$${TT##*_}; cd $$D && \
	$(MAKE) -f $(MKFILE) \
		PREFIX="$(PREFIX)" DATAPREFIX="$(DATAPREFIX)" $$t || \
		{ echo FAILED make $@; exit 0; };

# Note that install depends on prog, not all.  This intentionally
# avoids the po target.  If target all had not already been
# visited, the catalogs will not be installed.
install: prog install_po install_examples install_doc
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		INSTALLCMD="$(INSTALLCMD)" INSTALLOPTS="$(INSTALLOPTS)" \
		INSTALLDIR="$(INSTALLDIR)" $@ ) ; \
	done
	@echo "Succeeded install in $(DIRS)"

uninstall: uninstall_po uninstall_examples uninstall_doc
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		INSTALLCMD="$(INSTALLCMD)" INSTALLOPTS="$(INSTALLOPTS)" \
		INSTALLDIR="$(INSTALLDIR)" $@ ) ; \
	done
	@echo "Succeeded uninstall in $(DIRS)"


depend.msw dep.msw exuberant-tags tags TAGS ID :
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXX="$(CXX)" \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIXDBG="$(EXESUFFIXDBG)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done

clean: clean_po clean_examples clean_doc
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXX="$(CXX)" \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIXDBG="$(EXESUFFIXDBG)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done

cleandepend cleandepends:
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXX="$(CXX)" \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIXDBG="$(EXESUFFIXDBG)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done

cleanall : cleanall_po cleanall_examples cleanall_doc
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXX="$(CXX)" \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIXDBG="$(EXESUFFIXDBG)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done

distclean: distclean_po distclean_examples distclean_doc
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXX="$(CXX)" \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIXDBG="$(EXESUFFIXDBG)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done

# Make distribution archive.  This is not as simple as one might hope,
# because some tar utilities have no option to exclude certain files
# (e.g. OpenBSD).  So get excluded files out of the way within a
# temp directory.
# Also will make zip file (in addition to tar.gz, not instead) if
# "$(MKZIP)" is not empty.
EXCLFILES = epspline.geany helpview 3rd_pty oldstuff .git .gitignore \
	examples-working
dist archive distarchive: distclean
	mkdir ../EXCL_TEMPD
	for f in $(EXCLFILES) ; do \
		test -e "$$f" && $(MV) "$$f" ../EXCL_TEMPD ; \
	done
	V=$$(/bin/sh ./version.sh) || { \
		echo FAILED to get version from ./version.sh; exit 1; } ; \
	TD="epspline-$${V}"; PD="$$(pwd)"; D="$${PD##*/}"; TF="$$TD.tar.gz"; \
	( cd .. && { \
		test X"$$D" = X"$$TD" || \
			{ $(MV) "$$D" "$$TD" && trap "$(MV) $$TD $$D" 0; }; \
		} && {\
		tar cvf - "$$TD" | gzip --best > "$$TF" && \
		ls -l "$$TF" || \
			{ echo FAILED making $$TF; exit 1; }; \
		test X"$(MKZIP)" = X && exit 0; TZ="$$TD.zip"; \
		$(RM) "$$TZ"; zip -9 -db -o -r "$$TZ" "$$TD" || \
			{ echo FAILED making $$TZ; exit 1; }; \
		exit 0; } ; ) || \
	{ echo FAILED making "$$TF" in $@; exit 1; }
	for f in ../EXCL_TEMPD/* ; do $(MV) "$$f" . ; done
	rmdir ../EXCL_TEMPD
