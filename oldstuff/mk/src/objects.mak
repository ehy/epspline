# make include file, hoped to be portable for mat system's make(1)

HDRS = $(HDRDIR)/a_canvas.h $(HDRDIR)/a_frame.h $(HDRDIR)/a_ruler.h \
	$(HDRDIR)/cfg.h $(HDRDIR)/splines.h \
	$(HDRDIR)/stdcc.h $(HDRDIR)/util.h $(HDRDIR)/epspline.h \
	$(HDRDIR)/wxutil.h $(HDRDIR)/a_tabwnd.h $(HDRDIR)/a_tabpage.h \
	$(HDRDIR)/a_zoomdlg.h $(HDRDIR)/a_spintextdlg.h \
	$(HDRDIR)/a_textctldlg.h \
	$(HDRDIR)/a_aboutbox.h \
	$(HDRDIR)/spline_props.h \
	$(HDRDIR)/wxexio.h

SRCS = $(SRCDIR)/epspline.cpp $(SRCDIR)/io.cpp $(SRCDIR)/a_canvas.cpp \
	$(SRCDIR)/splines.cpp $(SRCDIR)/splineprops.cpp $(SRCDIR)/util.cpp \
	$(SRCDIR)/a_frame.cpp $(SRCDIR)/a_ruler.cpp $(SRCDIR)/povdemo.cpp \
	$(SRCDIR)/a_tabwnd.cpp $(SRCDIR)/a_tabpage.cpp \
	$(SRCDIR)/a_zoomdlg.cpp $(SRCDIR)/a_spintextdlg.cpp \
	$(SRCDIR)/a_textctldlg.cpp \
	$(SRCDIR)/a_aboutbox.cpp \
	$(SRCDIR)/spline_props.cpp \
	$(SRCDIR)/wxexio.cpp

OBJECTS = $(OBJDIR)/epspline.o $(OBJDIR)/io.o $(OBJDIR)/a_canvas.o \
	$(OBJDIR)/splines.o $(OBJDIR)/splineprops.o $(OBJDIR)/util.o \
	$(OBJDIR)/a_frame.o $(OBJDIR)/a_ruler.o $(OBJDIR)/povdemo.o \
	$(OBJDIR)/a_tabwnd.o $(OBJDIR)/a_tabpage.o $(OBJDIR)/a_zoomdlg.o \
	$(OBJDIR)/a_textctldlg.o \
	$(OBJDIR)/a_spintextdlg.o \
	$(OBJDIR)/a_aboutbox.o \
	$(OBJDIR)/wxexio.o \
	$(OBJDIR)/spline_props.o \
	$(OBJDIR)/parser.o

