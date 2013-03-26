#! /usr/bin/make -f
# POSIX makefile for epspline docs
# 1st non-comment, 1st target w/o dependencies or commands:
.POSIX:

# name
NAME = epspline

# Maintain this file in the install target
# for use in the uninstall target
UNINST_LIST = ../uninstall/doc_uninstall-list

# some utilities used in this script:
# (cp and rm are always forced)
CP	    = cp -p -f
CPR	    = $(CP) -R -L
RM	    = rm -r -f
GREP    = grep -F
SORT    = sort
UNIQ    = uniq
CMP     = cmp
MKDIR	= mkdir -p

MKHT 	=	htlatex
# MKHTARG is to make html for wxHtmlHelp viewer
MKHTARG =	"html,info,3.2,index=2,3"
# MKHWARG is to make html for 'real' up-to-date web browser viewer
MKHWARG =	"html,info,index=2,3,sec-filename"
ZIP		=	zip
ZIPARGS	=	-9
UNZIP	=	unzip
UZPARGS	=	-t
TEX		=	tex
TEX4dx	=	'\def \filename{{$(NAME)}{idx}{4dx}{ind}} \input idxmake.4ht'
LATEX	=	latex
D2PDF	=	dvipdfm
D2PS	=	dvips
PDFTOPS	=	pdftops
MKPDF   =	pdflatex
MKPS    =	dvips
MKIDXCMD=	makeindex
MKIDXSTY=	$(NAME).ist
MKIDXARG=	-s $(MKIDXSTY) -o $(NAME).ind
MKIDX	=	$(MKIDXCMD) $(MKIDXARG)
MK_ADDL =	./mk_wxHT.sh
HCHK	=	tidy
HCHKARGS=	-c -i -w 0 -latin1 -m

NCOMP	=	3

SRCD	=	src
INP		=	$(NAME).tex
BSEL	=	buildselect.sty
INPALL	=	$(SRCD)/apphelpdoc.sty $(SRCD)/$(BSEL) \
			$(SRCD)/$(INP) \
			$(SRCD)/ch_introduction.tex \
			$(SRCD)/fdl.tex
WXDOCF	=	help.zip
HWDOCF	=	html.zip
WXDOCA	=	$(NAME).hhp $(NAME).hhk $(NAME).hhc
WXHEADER=	$(NAME)_helpids.h
PDF		=	$(NAME).pdf
PS		=	$(NAME).ps
DVI		=	$(NAME).dvi
LISTING	=	listing.ps
ZIPDOCF	=	$(WXDOCA) *.html *.css *.png
PREPDD	=	prep
TEMPDHL	=	TEMP_HELP
TEMPDHT	=	TEMP_HTML
TEMPDPD	=	TEMP_PDF
TEMPDDV	=	TEMP_DVI
ALLTEMPD=	$(TEMPDHL) $(TEMPDHT) $(TEMPDPD) $(TEMPDDV)
ALLGEN	=	$(WXDOCF) $(WXHEADER) $(HWDOCF) $(PDF) $(PS) $(DVI) $(LISTING)
NO_DIST	=	$(HWDOCF) $(PDF) $(PS) $(DVI) $(LISTING)

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
INSTALLDIR = $(DATAPREFIX)/share/$(NAME)/doc

all: wxdoc

# dvi not working; rest OK
dvi: $(DVI)
pdf: $(PDF)
ps: $(PS)
wxdoc: $(WXDOCF)
html: $(HWDOCF)
listing: $(LISTING)

SELFMTBASE	= '%s\n%s\n%s\n' \
			'\newcommand\SELtrue[1]{\#1}' '\newcommand\SELfalse[1]{}'
WXBSEL	=	$(SELFMTBASE) '\def\SELbuild{1}'
$(WXDOCF): $(INPALL)
	$(RM) $(TEMPDHL)
	$(CPR) $(SRCD) $(TEMPDHL)
	for f in $(PREPDD)/* ; do \
		test -e "$$f" || continue; \
		$(CPR) "$$f" $(TEMPDHL) ; \
		done
	( TIDY=$$(which $(HCHK) ) || TIDY=true; \
		cd $(TEMPDHL) && \
		printf '\\hypersetup{final=true,tex4ht}\n' > hyperref.cfg && \
		printf $(WXBSEL) >> $(BSEL) && \
		$(MKHT) $(INP) $(MKHTARG) && \
		$(TEX) $(TEX4dx) && $(MKIDX) $(NAME).4dx && \
		$(MKHT) $(INP) $(MKHTARG) && \
		DEFTOPIC="epsplineli1.html" PNAME=$(NAME) HNAME=$@ DBG=yes \
			../$(MK_ADDL) $(INP) && \
		{ $$TIDY $(HCHKARGS) *.html; test 2 -gt $$? && true ; } && \
		{ $(CMP) ../$(WXHEADER) $(WXHEADER) || \
			$(CP) $(WXHEADER) .. ; } && \
		$(ZIP) $(ZIPARGS) $@ $(ZIPDOCF) && \
		$(UNZIP) $(UZPARGS) $@ )
	$(RM) $@
	$(CP) $(TEMPDHL)/$@ .
	$(CMP) ./$(WXHEADER) ../src/$(WXHEADER) || \
		$(CP) ./$(WXHEADER) ../src


HWBSEL	=	$(SELFMTBASE) '\def\SELbuild{2}'
$(HWDOCF): $(INPALL)
	$(RM) $(TEMPDHT)
	$(CPR) $(SRCD) $(TEMPDHT)
	for f in $(PREPDD)/* ; do \
		test -e "$$f" || continue; \
		$(CP) -r "$$f" $(TEMPDHT) ; \
		done
	( TIDY=$$(which $(HCHK) ) || TIDY=true; \
		cd $(TEMPDHT) && \
		printf '\\hypersetup{final=true,tex4ht}\n' > hyperref.cfg && \
		printf $(HWBSEL) >> $(BSEL) && \
		$(MKHT) $(INP) $(MKHWARG) && \
		$(TEX) $(TEX4dx) && $(MKIDX) $(NAME).4dx && \
		$(MKHT) $(INP) $(MKHWARG) && \
		{ $$TIDY $(HCHKARGS) *.html; test 2 -gt $$? && true ; } && \
		$(ZIP) $(ZIPARGS) $@ $(ZIPDOCF) && \
		$(UNZIP) $(UZPARGS) $@ )
	$(RM) $@
	$(CP) $(TEMPDHT)/$@ .


$(LISTING):
	cd ../src && \
	a2ps -f 11 -R --columns=1 -T 4 --pretty-print=cxx -o - \
	$$(ls -r *.h *.hh *.hpp *.C *.cc *.cxx *.cpp *.y *.l) > ../doc/$@

# This target (dvi) is broken because it uses latex as compiler,
# and latex cannot handle the .PNG images.
DVBSEL	=	$(SELFMTBASE) '\def\SELbuild{3}'
$(DVI): $(INPALL)
	@echo Target $@ currently broken \(see makefile comment\).; exit 1
	$(RM) $(TEMPDDV)
	$(CPR) $(SRCD) $(TEMPDDV)
	for f in $(PREPDD)/* ; do \
		test -e "$$f" || continue; \
		$(CP) -r "$$f" $(TEMPDDV) ; \
		done
	( COMPN=0; cd $(TEMPDDV) && \
		printf $(DVBSEL) >> $(BSEL) && \
		while test $$COMPN -lt $(NCOMP) ; do \
		COMPN=$$((COMPN + 1)) ; \
		$(LATEX) $(INP) || \
		{ echo FAILED $@ compilation pass $$COMPN; exit 1; } ; \
		{ test 1 -eq $$COMPN && $(MKIDX) $(NAME); true; } \
		done )
	$(RM) $@
	$(CP) $(TEMPDDV)/$@ .

PDBSEL	=	$(SELFMTBASE) '\def\SELbuild{4}'
$(PDF): $(INPALL)
	$(RM) $(TEMPDPD)
	$(CPR) $(SRCD) $(TEMPDPD)
	for f in $(PREPDD)/* ; do \
		test -e "$$f" || continue; \
		$(CP) -r "$$f" $(TEMPDPD) ; \
		done
	( COMPN=0; cd $(TEMPDPD) && \
		printf $(PDBSEL) >> $(BSEL) && \
		while test $$COMPN -lt $(NCOMP) ; do \
		COMPN=$$((COMPN + 1)) ; \
		$(MKPDF) $(INP) || \
		{ echo FAILED $@ compilation pass $$COMPN; exit 1; } ; \
		{ test 1 -eq $$COMPN && $(MKIDX) $(NAME); true; } \
		done )
	$(RM) $@
	$(CP) $(TEMPDPD)/$@ .

$(PS): $(PDF)
	$(PDFTOPS) $(PDF) $@

INSTALL_EXCLUDES =
INST_LIST = $(WXDOCF)
# Maintain $(UNINST_LIST)
install:
	@$(RM) $(UNINST_LIST)-tmp || exit 0
	@if test -e $(UNINST_LIST); then \
		$(CP) -p $(UNINST_LIST) $(UNINST_LIST)-tmp; \
	fi
	@for f in $(INST_LIST) ; do \
		test -f "$$f" || continue ; \
		echo "$(INSTALL_EXCLUDES)" | $(GREP) -q "$$f/" && continue; \
		td="$(INSTALLDIR)" ; \
		test -e "$$td" && test ! -d "$$td" && \
			{ echo Found \""$$td"\" is not a directory; \
			echo CANNOT install in \""$$td"\"; \
			exit 1; } ; \
		test -d "$$td" || $(MKDIR) -p "$$td" || \
			{ echo CANNOT $(MKDIR) \""$$td"\"; exit 1; } ; \
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

clean:
	$(RM) $(ALLTEMPD)

cleanall: clean
	$(RM)  $(ALLGEN)

distclean: clean
	$(RM)  $(NO_DIST)
	:> $(UNINST_LIST)
