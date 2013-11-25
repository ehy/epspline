#! /usr/bin/make -f
# POSIX makefile for epspline translations
# 1st non-comment, 1st target w/o dependencies or commands:
.POSIX:

# catalog name: $(CATNAME).{pot,po,mo}
CATNAME = epspline
POTFILE = $(CATNAME).pot
POFILE  = $(CATNAME).po
MOFILE  = $(CATNAME).mo
POTSRC  = ../src/$(POTFILE)

# Maintain this file in the install target
# for use in the uninstall target
UNINST_LIST = ../uninstall/po_uninstall-list

# some utilities used in this script:
# (cp and rm are always forced)
MK_EN_US_SH = ./pot2en_US.sh
RM = rm -f
CP = cp -f
MV = mv
CMP = cmp
GREP = grep -F
SORT = sort
UNIQ = uniq
MKDIR = mkdir
MSGFMT = msgfmt -o - -v

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
# the default.
L10_INSTALL_DIR = $(L10NPREFIX)/share/locale

INSTALLCMD = install
#INSTALLOPTS = -c -m 444 -o 0 -g 0
INSTALLOPTS = -c -m 444
INSTALLDIR = $(L10_INSTALL_DIR)
# Will actually under this subdir:
INSTALLSUBD = LC_MESSAGES

# a list of directories that might contain a *.po file, but should
# be excluded in mk_mo_files target; the en_US directory is
# included because it has its own target
# NOTE: all names *must* have trailing slash/ so that grep
# can distinguish e.g. en and en_US (en/ is not a sub-part of en_US/).
MO_EXCLUDE_DIRS = en_US/

# a list of directories that might contain a *.mo file, but should
# be excluded in install target
# NOTE: all names *must* have trailing slash/ so that grep
# can distinguish e.g. en and en_US (en/ is not a sub-part of en_US/).
INSTALL_EXCLUDE_DIRS = 

all: mk_mo_files

# The .mo file is made unconditionally because posix lacks test -ot -nt
mk_mo_files: en_US
	@test -e $@-DONE && exit 0; \
	echo $@ > $@-DONE; \
	for d in * ; do \
		test -d "$$d" || continue ; \
		test -f "$$d/$(POFILE)" || continue ; \
		echo "$(MO_EXCLUDE_DIRS)" | $(GREP) -q "$$d/" && continue ; \
		$(MSGFMT) "$$d/$(POFILE)" > "$$d/$(MOFILE)" || \
			{ echo FAILED making "$$d/$(MOFILE)"; \
			$(RM) $@-DONE; exit 1; } ; \
	done ; \
	d=en ; echo "$(MO_EXCLUDE_DIRS)" | $(GREP) -q $$d/ || \
		{ test -d $$d || $(MKDIR) $$d ; \
			test -f $$d/$(POFILE) || $(CP) en_US/$(MOFILE) $$d; } || \
	{ $(RM) $@-DONE; exit 1; }
	@echo "Succeeded" $@

# en_US is special only because application includes a script
# to generated a minimal (false) translation (just some UTF-8
# typographical glyphs and so on) -- this differs from others
# in that contributions of them are necessary.
en_US: en_US/$(MOFILE)
en_US/$(MOFILE): $(POTFILE)
	@F="$@"; d=$${F%/*}; \
	test -d $$d || $(MKDIR) $$d ; \
	test -e $$d-DONE && exit 0; \
	echo $$d > $$d-DONE; \
	$(MK_EN_US_SH) $(POTFILE) $$d/$(POFILE) || \
	{ $(RM) $$d-DONE; exit 1; }

$(POTFILE): $(POTSRC)
	@$(CMP) $@ $(POTSRC) >/dev/null 2>&1 || \
		{ test -f $(POTSRC) && $(CP) $(POTSRC) $@; }

# Note that install: does not depend on all: or mk_mo_files: --
# that is because those targets will make .mo files unconditionally,
# and since install: is typically invoked with a privileged [eg]id,
# those files might have that [eg]id, which is not nice.
# Therefore this target might install stale files, or fail
# to find files it should install, if all: or mk_mo_files:
# had not been invoked recently.
# Maintain $(UNINST_LIST)
install:
	@$(RM) $(UNINST_LIST)-tmp || exit 0
	@if test -e $(UNINST_LIST); then \
		$(CP) -p $(UNINST_LIST) $(UNINST_LIST)-tmp; \
	fi
	@for d in * ; do \
		test -d "$$d" || continue ; \
		test -f "$$d/$(MOFILE)" || continue ; \
		echo "$(INSTALL_EXCLUDE_DIRS)" | $(GREP) -q "$$d/" && continue; \
		td="$(INSTALLDIR)/$$d/$(INSTALLSUBD)" ; \
		test -e "$$td" && test ! -d "$$td" && \
			{ echo Found \""$$td"\" is not a directory; \
			echo NOT installing \""$$td/$(MOFILE)"\"; \
			continue; } ; \
		test -d "$$td" || $(MKDIR) -p "$$td" || \
			{ echo CANNOT $(MKDIR) \""$$td"\"; exit 1; } ; \
		echo DOING $(INSTALLCMD) $(INSTALLOPTS) "$$d/$(MOFILE)" "$$td"; \
		$(INSTALLCMD) $(INSTALLOPTS) "$$d/$(MOFILE)" "$$td" && \
			echo "$$td/$(MOFILE)" >> $(UNINST_LIST)-tmp || \
			{ echo FAILED installing "$$td/$(MOFILE)"; exit 1; }; \
	done
	@$(SORT) < $(UNINST_LIST)-tmp | $(UNIQ) > $(UNINST_LIST) && \
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

cleanall_en_US :
	@$(RM) en_US/$(MOFILE) en_US-DONE

# Note: clean does not remove .mo files -- the binaries are to be
# included in distribution, since a system might not be able to
# compile them -- the source user can force the build if wanted.
clean :
	@$(RM) *-DONE

cleanall : clean cleanall_en_US
	@for d in * ; do \
		test -d "$$d" || continue ; \
		test -f "$$d/$(MOFILE)" || continue ; \
		echo "$(MO_EXCLUDE_DIRS)" | $(GREP) -q "$$d/" && continue ; \
		$(RM) "$$d/$(MOFILE)" ; \
	done
	@$(RM) en_US/$(POFILE)

distclean: clean
	:> $(UNINST_LIST)
