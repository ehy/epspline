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
MKDIR = mkdir

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
# UPDATE for wx 2.9.5: the above 2 sentences were true up to
# 2.9.4, but 2.9.5 is now botching-up the paths and needs this
# set to succeed; it can still be set elsewise on make command line
L10_INSTALL_DEF = -DL10N_INSTALL_PATH=\\\"$(L10_INSTALL_DIR)\\\"
#L10_INSTALL_DEF =

INSTALLCMD = install
#INSTALLOPTS = -c -s -m 555 -o 0 -g 0
INSTALLOPTS = -c -s -m 555
INSTALLDIR = $(BINPREFIX)/bin

# GNU rejects MACRO = $(MACRO)
DEFINES = $(L10_INSTALL_DEF) -DINSTALL_PREFIX=\\\"$(DATAPREFIX)\\\"

YDEBUG = -g -DDEBUG=1
NDEBUG = -DNDEBUG=1
DEBUG = $(NDEBUG)

# Feb  7 2009: -Wno-write-strings
# GCC g++ only (maybe also clang++):
# Hate to disable warnings, but recent g++ (4.x) is
# spamming deprecated conversion warnings from the wx* headers;
# so be it.
# OpenBSD: also, strict-aliasing spam
#WARN = -Wall -Wno-write-strings -Wno-strict-aliasing
# -Wunknown-pragmas added for clang++
# For development, or user may give WARN='$(GCCWARN)'
# Note these are useful for clang too.
GCCWARN = -Wall -Wno-write-strings -Wno-strict-aliasing \
	-Wno-unknown-pragmas -Wnon-virtual-dtor -Wextra \
	-Wno-deprecated-declarations \
	-Wno-unused-parameter -Wno-unused-variable \
	-Wno-potentially-evaluated-expression
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
LIBSARG = std,aui
LIBS = `$(CF) --libs $(LIBSARG)`
# mingw only: MSW binary resource compiler
MSWRCC = `$(CF) --rescomp`

# The parser will need more stack for large single objects
# (revealed by testing with font based output) -- here
# is a macro for YACC definitions as needed
YACCDEFINES = -DYYMAXDEPTH=30000

# For each Makefile that installs something, uninstall
# data is saved in dir $(UNINSTALLER_ROOT), in file
# $${dir}$(UNINSTALLER_BASE); it's desirable that these
# files *not* be owned by priviliged account that is used
# to install, so that user may (re)move this source. So,
# try to ensure they exists at 1st make, which hopefully
# occurs w/o privilege, and during install data is *redirected*
# into extant files, which should not change ownership.
# The target for this setup is 'uninst_setup' ('prog' depends).
UNINSTALLER_ROOT = uninstall
UNINSTALLER_BASE = _uninstall-list
UNINSTALLER_DIRS = doc po examples anim-example src


#
# Targets:
#
all: doc prog po examples

prog: uninst_setup depend
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) -f depend \
		CC="$(CC)" CFLAGS="$(CFLAGS)" \
		CXX="$(CXX)" CCFLAGS="$(CCFLAGS) $(DEBUG)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		YACCDEFINES="$(YACCDEFINES)" \
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
		CC="$(CC)" CFLAGS="$(CFLAGS)" \
		CXX="$(CXX)" CCFLAGS="$(CCFLAGS) $(DEBUG)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		MSWRCC="$(MSWRCC)" \
		YACCDEFINES="$(YACCDEFINES)" \
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
src/depend:
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXXDEPS="$(CXXDEPS)" DEPSOPT="$(DEPSOPT)" \
		CCFLAGS="$(CCFLAGS) $(DEBUG)"  \
		YACCDEFINES="$(YACCDEFINES)" \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIX)" LIBS="$(LIBS)" depend ) ; \
	done
	@echo "Succeeded making" $@

src/dep:
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXXDEPS="$(CXXDEPS)" DEPSOPT="$(DEPSOPT)" \
		CCFLAGS="$(CCFLAGS) $(DEBUG)"  \
		YACCDEFINES="$(YACCDEFINES)" \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIX)" LIBS="$(LIBS)" dep ) ; \
	done
	@echo "Succeeded making" $@

depend: src/depend
	@echo "Succeeded making" $@

dep: src/dep
	@echo "Succeeded making" $@

Release: all

Debug DEBUG debug: depend
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) -f depend \
		CXX="$(CXX)" \
		CCFLAGS="$(CCFLAGS) $(YDEBUG)"  \
		YACCDEFINES="$(YACCDEFINES)" \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIXDBG)" LIBS="$(LIBS)" all ) ; \
	done
	@echo "Succeeded making in $(DIRS)"

junko:
	@echo loaded, partner

# one-time make uninstall data containers: see
# comment by UNINSTALLER_ROOT assignment
uninst_setup:
	@test -d $(UNINSTALLER_ROOT) || $(MKDIR) $(UNINSTALLER_ROOT)
	@for t in $(UNINSTALLER_DIRS); do \
		f="$(UNINSTALLER_ROOT)/$${t}$(UNINSTALLER_BASE)" ; \
		test -f "$$f" || :>"$$f" ; \
	done

# One (un)install task is handled in this top makefile:
# the (now conventional) .desktop file, and icons.
# This *must* be done with commands that know the
# setup and how to install these items.  Therefore,
# this rule proceeds only if the expected commands
# are found by the shell.
# This target should not yield a failure status; it
# can print warnings but should not prevent the
# rest of the installation.
XDG_INST_ICON = xdg-icon-resource
XDG_INST_MENU = xdg-desktop-menu
XDG_INST_MIME = xdg-mime
install_xdg:
	@echo Installing XDG desktop items: menu entry, icons
	@XII=$$(which $(XDG_INST_ICON)) && XIM=$$(which $(XDG_INST_MENU)) \
	&& XMM=$$(which $(XDG_INST_MIME)) \
	|| { echo cannot find $(XDG_INST_ICON) or $(XDG_INST_MENU); \
		echo will not do xdg icon and menu item install; exit 0; } ; \
	$$XMM install --novendor epspline.xdgmime.xml || \
		{ echo failed to install xdg mime xml; true; } ; \
	ARGS="install --noupdate --novendor --size"; \
	for s in 16 24 32 48 64; do \
		I="art/epspline_$${s}x$${s}.png"; test -f "$$I" || \
			{ echo icon "$$I" is missing; continue; } ; \
		$$XII $$ARGS $$s "$$I" epspline || \
			{ echo failed installing icon size $$s; true; } ; \
		$$XII install --context mimetypes --size $$s \
			"$$I" application-x-epspline || \
			{ echo failed to install xdg mime context $$s; true; } ; \
	done; \
	$$XII forceupdate; \
	I="epspline.desktop"; test -f "$$I" || exit 0; \
	$$XIM install --novendor "$$I" || \
	{ echo failed to install "$$I"; true; }

uninstall_xdg:
	@echo Uninstalling XDG desktop items: menu entry, icons
	@XII=$$(which $(XDG_INST_ICON)) && XIM=$$(which $(XDG_INST_MENU)) \
	&& XMM=$$(which $(XDG_INST_MIME)) \
	|| { echo cannot find $(XDG_INST_ICON) or $(XDG_INST_MENU); \
		echo will not do xdg icon and menu item uninstall; exit 0; } ; \
	I="epspline.desktop"; \
	$$XIM uninstall "$$I" || \
	{ echo failed to uninstall "$$I"; true; } ; \
	ARGS="uninstall --noupdate --size"; \
	for s in 16 24 32 48 64; do \
		I="art/epspline_$${s}x$${s}.png"; \
		$$XII uninstall --context mimetypes --size $$s \
			application-x-epspline || \
			{ echo failed to uninstall xdg mime context $$s; true; } ; \
		$$XII $$ARGS $$s epspline || \
			{ echo failed uninstalling icon size $$s; true; } ; \
	done; \
	$$XMM uninstall epspline.xdgmime.xml || \
		{ echo failed to uninstall xdg mime xml; true; } ; \
	$$XII forceupdate || true


# make, or install or clean, the translation message catalogs
# this target always exits success in case current system
# does not have msgfmt program (or any other error) and it
# should not prevent program build and install
po examples anim-example doc: junko
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

install_anim-example uninstall_anim-example clean_anim-example cleanall_anim-example distclean_anim-example:
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
install_base: prog install_po install_examples install_anim-example install_doc
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		INSTALLCMD="$(INSTALLCMD)" INSTALLOPTS="$(INSTALLOPTS)" \
		INSTALLDIR="$(INSTALLDIR)" install ) ; \
	done
	@echo "Succeeded $@ in $(DIRS)"

# Split install into install base -- all but install_xdg --
# and install whole thing.  To avoids repeated xdg stuff
# during development.
install: install_base install_xdg
	@echo "Succeeded $@ in $(DIRS)"

uninstall: uninstall_po uninstall_examples uninstall_anim-example uninstall_doc uninstall_xdg
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

clean: clean_po clean_examples clean_anim-example clean_doc
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

cleanall : cleanall_po cleanall_examples cleanall_anim-example cleanall_doc
	@for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f $(MKFILE) \
		CXX="$(CXX)" \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIXDBG="$(EXESUFFIXDBG)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done

distclean: distclean_po distclean_examples distclean_anim-example distclean_doc
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
MKZIP = itydoodah
EXCLFILES = epspline.geany helpview 3rd_pty oldstuff .git .gitignore \
	examples-working tmp bak excluded
dist archive distarchive: distclean
	V=$$(/bin/sh ./version.sh) || { \
		echo FAILED to get version from ./version.sh; exit 1; } ; \
	TD="epspline-$${V}"; PD="$$(pwd)"; \
	D="$${PD##*/}"; TF="$$TD.tar.gz"; \
	test -e ../"$$TD" && \
		{ echo FAILED: ../"$$TD" exists; ls -ld ../"$$TD"; exit 1; }; \
	test -e ../EXCL_TEMPD && \
		{ echo FAILED: ../EXCL_TEMPD exists; ls -ld ../EXCL_TEMPD; exit 1; }; \
	mkdir ../EXCL_TEMPD || { echo FAILED mkdir ../EXCL_TEMPD; exit 1; }; \
	for f in $(EXCLFILES) ; do \
		test -e "$$f" && $(MV) "$$f" ../EXCL_TEMPD ; \
	done; \
	_ST=0; \
	test 0 -eq $$_ST && { \
		( cd .. && { \
			test X"$$D" = X"$$TD" || \
				{ $(MV) "$$D" "$$TD" && trap "$(MV) $$TD $$D" 0; }; \
			} && { \
				tar cvf - "$$TD" | gzip --best > "$$TF" && \
				ls -l "$$TF" || { echo FAILED making $$TF; exit 1; }; \
				test X"$(MKZIP)" = X || { \
					TZ="$$TD.zip"; \
					$(RM) "$$TZ"; zip -9 -o -r "$$TZ" "$$TD" || \
						{ echo FAILED making $$TZ; exit 1; }; \
				}; \
			}; \
			exit 0; \
		) || \
		{ echo FAILED making "$$TF" in $@; _ST=1; }; \
	}; \
	for f in ../EXCL_TEMPD/* ../EXCL_TEMPD/.??* ; do \
		test -e "$$f" || continue; \
		$(MV) "$$f" . ; \
	done; rmdir ../EXCL_TEMPD; \
	test 0 -eq $$_ST && echo SUCCEEDED $@ || echo FAILED $@; \
	exit $$_ST
	
