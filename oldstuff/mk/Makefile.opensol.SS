#! /bin/make -f
# OpenSolaris makefile for epspline
# For Sun CC, not GCC
# Make sure $PATH is suitable; check wx-config --cxx
# SUNWwxwidgets from default repository compatible.
# Good luck.
#

DIRS=src

DEBUG =
OPT = -fast -DNDEBUG=1
#OPT = -xO2 -DNDEBUG=1
GUI = -Dwx_x=1

WARN =

FINAL = 1

EXESUFFIX=

INCLUDES = -I. -I..
# Note:
# Specifically OpenSolaris 0609 w/ package ss-dev installed - but
# probably applies more generally.
# Package SUNWwxwidgets is compiled with Sun CC: wx-config --cxx:
#          /opt/SUNWspro.40/SS12/bin/CC -norunpath
# Package system does not list SUNWspro in default repository (has
# got SUNWsprot - bundled tools). Installing package ss-dev yields
# /opt/SunStudioExpress so I mkdir'd /opt/SUNWspro.40 under which I
# symlinked ../SunStudioExpress SS12 which worked - almost.
# SunStudioExpress includes Rogue Wave standard templates, and those
# use _T as a template parameter. E.g.:
# template <class _T, [...]> foo {
#	[...]
#	operator _T() {[...]}
# }
# Well, wxWidgets defines macro _T(string) for NLS and UNICODE, and
# this program's code uses it. Of course worlds collide.
# Found workaround: Sun CC has '-include <file>' option which
# acts as #include in source. Using this to pre-include templates
# before macro definition allowed successful compilation.
CFL= -include vector -include algorithm \
	$(CXXFLAGS) $(INCLUDES) $(OPT) $(DEFINES)
LFL=

#CF = wxgtk-2.4-config
#CF = wxgtk2-2.4-config
#CF = wxmotif-2.4-config
#CF = wxx11univ-2.4-config
#CF = wx-config --version=2.6
#CF = wx-config --version=2.8
# Seen in Ubuntu 8.10 package: default toolkit
# is not GUI but base!
#CF = wx-config --toolkit=gtk2 --version=2.6
#CF = wx-config --toolkit=gtk2 --version=2.8
#CF = wx-config --toolkit=gtk2
CF = wx-config

CCFLAGS = $(WARN) $(CFL) $(GUI)
LDFLAGS = $(LFL)

all parser.c lexer.c: $(DIRS)
	for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f Makefile.opensol.SS $(MAKEFLAGS) \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done
	@echo "Succeeded making in $(DIRS)"


$(DIRS):
	for d in $(DIRS) ; do \
		test -d $$d || mkdir $$d ; \
	done

INSTALL= ginstall -c -s -m 555 -o 0 -g 0
INSTALLDIR = /usr/local/bin

install: all
	for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f Makefile.opensol.SS $(MAKEFLAGS) \
		INSTALL="$(INSTALL)" INSTALLDIR="$(INSTALLDIR)" $@ ) ; \
	done
	@echo "Succeeded making in $(DIRS)"

clean cleanall depend dep depend.msw dep.msw extags exuberant-tags tags TAGS ID : $(DIRS)
	for d in $(DIRS) ; do \
		(cd $$d ; $(MAKE) -f Makefile.opensol.SS $(MAKEFLAGS) \
		CCFLAGS="`$(CF) --cxxflags` $(CCFLAGS)"  \
		LDFLAGS="$(LDFLAGS)" FINAL="$(FINAL)" CF="$(CF)" \
		EXESUFFIX="$(EXESUFFIX)" $@ ) ; \
	done

