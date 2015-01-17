#! /usr/bin/make -f
# POSIX makefile for epspline anim-example:
# Serves for [un]installation; default
# target is a NOP (actually not, it echo's a message).
# 1st non-comment, 1st target w/o dependencies or commands:
.POSIX:

# catalog name: $(NAME).{pot,po,mo}
NAME = epspline

# Maintain this file in the install target
# for use in the uninstall target
UNINST_LIST = ../uninstall/anim-example_uninstall-list

# some utilities used in this script:
# (cp and rm are always forced)
RM = rm -f
CP = cp -f
MV = mv
GREP = grep -F
SORT = sort
UNIQ = uniq
MKDIR = mkdir

# the general root of an installtion -- frequently (but not
# necessarily) /usr/local on Unix-like systems
PREFIX = /usr/local
# an installer might prefer different prefixes for, e.g., data
BINPREFIX = $(PREFIX)
DATAPREFIX = $(PREFIX)
HELPDATAPREFIX = $(DATAPREFIX)

INSTALLCMD = install
#INSTALLOPTS = -c -m 444 -o 0 -g 0
INSTALLOPTS = -c -m 444
INSTALLDIR = $(DATAPREFIX)/share/$(NAME)/anim-example

# all does nothing; nothing is built, Makefile is presently
# {,un}install
all:
	@P="`pwd`"; D="$${P##*/}"; echo $@ made in "$$D" ; exit 0

INSTALL_EXCLUDES = Makefile
INST_LIST = *
# Maintain $(UNINST_LIST)
install:
	@$(RM) $(UNINST_LIST)-tmp || exit 0
	@if test -e $(UNINST_LIST); then \
		$(CP) -p $(UNINST_LIST) $(UNINST_LIST)-tmp; \
	fi
	@for f in $(INST_LIST) ; do \
		test -e "$$f" || continue ; \
		case " $(INSTALL_EXCLUDES) " in \
			*" $$f "* ) continue;; \
		esac; \
		td="$(INSTALLDIR)" ; \
		test -e "$$td" && test ! -d "$$td" && \
			{ echo Found \""$$td"\" is not a directory; \
			echo CANNOT install in \""$$td"\"; \
			exit 1; } ; \
		test -d "$$td" || $(MKDIR) -p "$$td" || \
			{ echo CANNOT $(MKDIR) \""$$td"\"; exit 1; } ; \
		test -d "$$f" && { $(MKDIR) -p "$$td/$$f" || \
				{ echo CANNOT $(MKDIR) \""$$td/$$f"\"; exit 1; } ; \
			continue ; } ; \
		echo DOING $(INSTALLCMD) $(INSTALLOPTS) "$$f" "$$td"; \
		$(INSTALLCMD) $(INSTALLOPTS) "$$f" "$$td" && \
			echo "$$td/$$f" >> $(UNINST_LIST)-tmp || \
			{ echo FAILED installing "$$td/$$f"; exit 1; }; \
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

clean :

cleanall : clean

distclean: cleanall
	:> $(UNINST_LIST)
