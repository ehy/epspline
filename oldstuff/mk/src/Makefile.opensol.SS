#! /usr/bin/make -f
# GNU ain't UNIX makefile for epspline

RM   =  rm
CP   =  cp
YACC =  yacc
#YACC =  byacc
#YACC =  bison -y
#LEX  =  lex
#LEX  =  flex -l
#LEX  =  flex -X
LEX  =  flex
CCLEX=  c++
#CCLEX=  cc
LEX_STEM = lex.yy
#PICFLAGS = -fPIC

FINAL=1

EXESUFFIX=

#OBJDIR = objects$(EXESUFFIX)
OBJDIR = .
SRCDIR = .
HDRDIR = .
COMMDIR = .

include objects.mak

# OpenSolaris apparently has new flex; as I learned
# under Ubuntu 8.*, it has changed incompatibly and
# 'flex-old' ubuntu package is necessary, nothing
# else works (except *BSD lex).  Try prefab files
# retained for MSW builds
PARSER_SRC = $(SRCDIR)/ms_parse.c
LEXER_SRC =  $(SRCDIR)/ms_lexer.c

all: $(OBJDIR) epspline$(EXESUFFIX)

$(APPLIB) applib:
	true

$(OBJDIR):
	test -d $(OBJDIR) || mkdir $(OBJDIR)

.SUFFIXES: .cpp

$(OBJDIR)/%.o : %.cpp
	`$(CF) --cxx` `$(CF) --cxxflags` $(CCFLAGS) -c -o $@ $<

# OpenSolaris apparently has new flex; as I learned
# under Ubuntu 8.*, it has changed incompatibly and
# 'flex-old' ubuntu package is necessary, nothing
# else works (except *BSD flex).  Try prefab files
# retained for MSW builds
$(OBJDIR)/parser.o: $(PARSER_SRC) $(LEXER_SRC)
	`$(CF) --cxx` `$(CF) --cxxflags` $(CCFLAGS) -c -o $@ $(PARSER_SRC)

#$(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/lexer.c
#	`$(CF) --cxx` `$(CF) --cxxflags` $(CCFLAGS) -c -o $@ parser.c

$(SRCDIR)/parser.c: $(COMMDIR)/parser.y $(SRCDIR)/lexer.c
	$(RM) -f y.tab.c
	$(YACC) $(COMMDIR)/parser.y
	sed -e "s;$(COMMDIR)/y.tab.c;parser.y;g" < y.tab.c | \
	sed -e "s/BUFSIZ/5000/g"            | \
	sed -e "s/YYLMAX 200/YYLMAX 5000/g" | \
	sed -e "s/yy/EPSIO_yy/g"            | \
	sed -e "s/input/EPSIO_input/g"      | \
	sed -e "s/unput/EPSIO_unput/g"      > $(SRCDIR)/parser.c

$(SRCDIR)/lexer.c: $(COMMDIR)/lexer.l
	$(RM) -f $(LEX_STEM).c
	$(LEX) $(COMMDIR)/lexer.l
	sed -e "s;$(COMMDIR)/$(LEX_STEM).c;lexer.l;g" < $(LEX_STEM).c | \
	sed -e "s/yy/EPSIO_yy/g"            | \
	sed -e "s/input/EPSIO_input/g"      | \
	sed -e "s/unput/EPSIO_unput/g"      > $(SRCDIR)/lexer.c

epspline$(EXESUFFIX):  $(APPLIB) $(OBJECTS)
	`$(CF) --cxx` $(LDFLAGS) -o $@ $(OBJECTS) $(APPLINK) `$(CF) --libs`

depend dep :
	rm -f depend
	`$(CF) --cxx` `$(CF) --cflags` $(CCFLAGS) -xM1 $(SRCS) | \
	sed 's/^[^:[:space:]]\{1,\}[[:space:]]*:/$$(OBJDIR)\/&/' > depend

# No capable tools on MSW to make dependency file.  Use Unix tools.
depend.msw dep.msw :
	egrep '(^\$$.+:| \<[^ /]+\.*(h|xpm|inc)\>)' < depend | \
	sed -e 's;^\$$;\?\?&;' \
	-e 's;[ ]/[^ ]\{1,\}\.[^\. /]\{1,\}\>;;g' \
	-e 's;\.o\>;\.obj;g' -e 's;/;\\;g' | tr '?' '\n' > depend.msw
	echo "" >> depend.msw
	echo "" >> depend.msw

# This target written for ``Exuberant Ctags.' [new versions]'
# See http://ctags.sourceforge.net
exuberant-tags : $(SRCS) $(HDRS)
	ctags --c-types=cdefgmnpstuv-x --excmd=pattern *

# This target written for ``Exuberant Ctags.'', old, -p no longer recognized,
# nor --c-types=*+A+C
# See http://ctags.sourceforge.net
old-exuberant-tags : $(SRCS) $(HDRS)
	ctags -p $(SRCDIR) -p $(HDRDIR) --c-types=cdefgmnpstuv-x+A+C \
	--excmd=pattern *

# This target written for BSD ctags; works with GNU ctags
tags : $(SRCS) $(HDRS)
	rm -f tags
	find $(SRCDIR) \( -name \*.cpp -o -name \*.h \) -print | \
		xargs ctags -adtw

# This target written for ``Exuberant Ctags.''
# See http://ctags.sourceforge.net
TAGS : $(SRCS) $(HDRS)
	ctags -e -p $(SRCDIR) -p $(HDRDIR) --c-types=cdefgmnpstuv-x+A+C *

# This target for GNU idutils
ID : $(SRCS) $(HDRS)
	mkid --prune tmp --output=$@

include depend

install: epspline$(EXESUFFIX)
	$(INSTALL)  epspline$(EXESUFFIX) $(INSTALLDIR)


clean:
	-rm -f $(OBJDIR)/*.o $(SRCDIR)/lex.yy.c $(SRCDIR)/y.tab.c \
	$(SRCDIR)/parser.c $(SRCDIR)/lexer.c \
	epspline$(EXESUFFIX) core

cleanall: clean
	-rm -f depend tags TAGS ID
