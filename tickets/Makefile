####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
CFLAGS += -I../customers
CFLAGS += -I../qgtimespinbox
CFLAGS += -I../lib
CFLAGS += -DADBQT
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib -shared
LFLAGS += -L../lib -lblarg
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

# CFLAGS += -DDBDEBUG

ifdef USEDES
	CFLAGS	+= -DUSEDES -I../bdes
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	Ticket.cpp\
			TicketManager.cpp\
			CustomerTickets.cpp\
			CreateCustTicket.cpp\
			TicketEditor.cpp

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_TicketManager.cpp\
			moc_CustomerTickets.cpp\
			moc_CreateCustTicket.cpp\
			moc_TicketEditor.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libtickets.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) report-lib

report-lib: $(OBJECTS)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% *.a #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_TicketManager.cpp: TicketManager.h
	$(MOC) TicketManager.h -o moc_TicketManager.cpp

moc_CustomerTickets.cpp: CustomerTickets.h
	$(MOC) CustomerTickets.h -o moc_CustomerTickets.cpp

moc_CreateCustTicket.cpp: CreateCustTicket.h
	$(MOC) CreateCustTicket.h -o moc_CreateCustTicket.cpp

moc_TicketEditor.cpp: TicketEditor.h
	$(MOC) TicketEditor.h -o moc_TicketEditor.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

Ticket.o: Ticket.h /usr/include/sys/types.h /usr/include/features.h
Ticket.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Ticket.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
Ticket.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
Ticket.o: /usr/include/bits/typesizes.h /usr/include/time.h
Ticket.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
Ticket.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
Ticket.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
Ticket.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
Ticket.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
Ticket.o: /usr/include/bits/confname.h /usr/include/string.h
Ticket.o: /usr/include/sys/time.h /usr/include/bits/time.h
Ticket.o: /usr/include/sys/select.h /usr/include/bits/select.h
Ticket.o: /usr/include/bits/sigset.h
TicketManager.o: /usr/include/stdio.h /usr/include/features.h
TicketManager.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TicketManager.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TicketManager.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TicketManager.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TicketManager.o: /usr/include/_G_config.h /usr/include/wchar.h
TicketManager.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TicketManager.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TicketManager.o: /usr/include/bits/stdio_lim.h
TicketManager.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
TicketManager.o: TicketManager.h /usr/lib/qt3/include/qdatetm.h
TicketManager.o: /usr/lib/qt3/include/qdatetime.h
TicketManager.o: /usr/lib/qt3/include/qstring.h
TicketManager.o: /usr/lib/qt3/include/qcstring.h
TicketManager.o: /usr/lib/qt3/include/qmemarray.h
TicketManager.o: /usr/lib/qt3/include/qgarray.h
TicketManager.o: /usr/lib/qt3/include/qshared.h
TicketManager.o: /usr/lib/qt3/include/qglobal.h
TicketManager.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
TicketManager.o: /usr/include/limits.h
TicketManager.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
TicketManager.o: /usr/lib/qt3/include/qnamespace.h
TicketManager.o: /usr/lib/qt3/include/qpainter.h
TicketManager.o: /usr/lib/qt3/include/qcolor.h
TicketManager.o: /usr/lib/qt3/include/qwindowdefs.h
TicketManager.o: /usr/lib/qt3/include/qobjectdefs.h
TicketManager.o: /usr/lib/qt3/include/qstringlist.h
TicketManager.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
TicketManager.o: /usr/lib/qt3/include/qtextstream.h
TicketManager.o: /usr/lib/qt3/include/qiodevice.h
TicketManager.o: /usr/lib/qt3/include/qdatastream.h
TicketManager.o: /usr/lib/qt3/include/qstrlist.h
TicketManager.o: /usr/lib/qt3/include/qptrlist.h
TicketManager.o: /usr/lib/qt3/include/qglist.h
TicketManager.o: /usr/lib/qt3/include/qptrcollection.h
TicketManager.o: /usr/lib/qt3/include/qfontmetrics.h
TicketManager.o: /usr/lib/qt3/include/qfont.h /usr/lib/qt3/include/qrect.h
TicketManager.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
TicketManager.o: /usr/lib/qt3/include/qfontinfo.h
TicketManager.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qpen.h
TicketManager.o: /usr/lib/qt3/include/qbrush.h
TicketManager.o: /usr/lib/qt3/include/qpointarray.h
TicketManager.o: /usr/lib/qt3/include/qwmatrix.h
TicketManager.o: /usr/lib/qt3/include/qlistview.h
TicketManager.o: /usr/lib/qt3/include/qscrollview.h
TicketManager.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qwidget.h
TicketManager.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
TicketManager.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
TicketManager.o: /usr/lib/qt3/include/qpair.h
TicketManager.o: /usr/lib/qt3/include/qpaintdevice.h
TicketManager.o: /usr/lib/qt3/include/qpalette.h
TicketManager.o: /usr/lib/qt3/include/qsizepolicy.h
TicketManager.o: /usr/lib/qt3/include/qscrollbar.h
TicketManager.o: /usr/lib/qt3/include/qrangecontrol.h
TicketManager.o: /usr/lib/qt3/include/qtextview.h
TicketManager.o: /usr/lib/qt3/include/qtextedit.h
TicketManager.o: /usr/lib/qt3/include/qstylesheet.h
TicketManager.o: /usr/lib/qt3/include/qptrvector.h
TicketManager.o: /usr/lib/qt3/include/qgvector.h /usr/lib/qt3/include/qdict.h
TicketManager.o: /usr/lib/qt3/include/qgdict.h
TicketManager.o: /usr/lib/qt3/include/qcombobox.h
TicketManager.o: /usr/lib/qt3/include/qcheckbox.h
TicketManager.o: /usr/lib/qt3/include/qbutton.h
TicketManager.o: /usr/lib/qt3/include/qkeysequence.h
TicketManager.o: /usr/lib/qt3/include/qlabel.h
TicketManager.o: /usr/lib/qt3/include/qpushbutton.h
TicketManager.o: /usr/lib/qt3/include/qiconset.h
TicketManager.o: /usr/lib/qt3/include/qpixmap.h
TicketManager.o: /usr/lib/qt3/include/qpopupmenu.h
TicketManager.o: /usr/lib/qt3/include/qmenudata.h
TicketManager.o: /usr/lib/qt3/include/qsignal.h
TicketManager.o: /usr/lib/qt3/include/qvariant.h TicketsCommon.h
TicketManager.o: /usr/lib/qt3/include/qlayout.h /usr/lib/qt3/include/qtimer.h
TicketManager.o: Ticket.h /usr/include/sys/types.h /usr/include/time.h
TicketManager.o: TicketEditor.h /usr/lib/qt3/include/qmultilineedit.h
CustomerTickets.o: CustomerTickets.h /usr/lib/qt3/include/qdatetm.h
CustomerTickets.o: /usr/lib/qt3/include/qdatetime.h
CustomerTickets.o: /usr/lib/qt3/include/qstring.h
CustomerTickets.o: /usr/lib/qt3/include/qcstring.h
CustomerTickets.o: /usr/lib/qt3/include/qmemarray.h
CustomerTickets.o: /usr/lib/qt3/include/qgarray.h
CustomerTickets.o: /usr/lib/qt3/include/qshared.h
CustomerTickets.o: /usr/lib/qt3/include/qglobal.h
CustomerTickets.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
CustomerTickets.o: /usr/include/features.h /usr/include/sys/cdefs.h
CustomerTickets.o: /usr/include/gnu/stubs.h
CustomerTickets.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
CustomerTickets.o: /usr/include/limits.h
CustomerTickets.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
CustomerTickets.o: /usr/lib/qt3/include/qnamespace.h
CustomerTickets.o: /usr/lib/qt3/include/qpainter.h
CustomerTickets.o: /usr/lib/qt3/include/qcolor.h
CustomerTickets.o: /usr/lib/qt3/include/qwindowdefs.h
CustomerTickets.o: /usr/lib/qt3/include/qobjectdefs.h
CustomerTickets.o: /usr/lib/qt3/include/qstringlist.h
CustomerTickets.o: /usr/lib/qt3/include/qvaluelist.h
CustomerTickets.o: /usr/lib/qt3/include/qtl.h
CustomerTickets.o: /usr/lib/qt3/include/qtextstream.h
CustomerTickets.o: /usr/lib/qt3/include/qiodevice.h /usr/include/stdio.h
CustomerTickets.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CustomerTickets.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CustomerTickets.o: /usr/include/_G_config.h /usr/include/wchar.h
CustomerTickets.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CustomerTickets.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
CustomerTickets.o: /usr/include/bits/stdio_lim.h
CustomerTickets.o: /usr/include/bits/sys_errlist.h
CustomerTickets.o: /usr/lib/qt3/include/qdatastream.h
CustomerTickets.o: /usr/lib/qt3/include/qstrlist.h
CustomerTickets.o: /usr/lib/qt3/include/qptrlist.h
CustomerTickets.o: /usr/lib/qt3/include/qglist.h
CustomerTickets.o: /usr/lib/qt3/include/qptrcollection.h
CustomerTickets.o: /usr/lib/qt3/include/qfontmetrics.h
CustomerTickets.o: /usr/lib/qt3/include/qfont.h /usr/lib/qt3/include/qrect.h
CustomerTickets.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
CustomerTickets.o: /usr/lib/qt3/include/qfontinfo.h
CustomerTickets.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qpen.h
CustomerTickets.o: /usr/lib/qt3/include/qbrush.h
CustomerTickets.o: /usr/lib/qt3/include/qpointarray.h
CustomerTickets.o: /usr/lib/qt3/include/qwmatrix.h
CustomerTickets.o: /usr/lib/qt3/include/qlistview.h
CustomerTickets.o: /usr/lib/qt3/include/qscrollview.h
CustomerTickets.o: /usr/lib/qt3/include/qframe.h
CustomerTickets.o: /usr/lib/qt3/include/qwidget.h
CustomerTickets.o: /usr/lib/qt3/include/qobject.h
CustomerTickets.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qmime.h
CustomerTickets.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qpair.h
CustomerTickets.o: /usr/lib/qt3/include/qpaintdevice.h
CustomerTickets.o: /usr/lib/qt3/include/qpalette.h
CustomerTickets.o: /usr/lib/qt3/include/qsizepolicy.h
CustomerTickets.o: /usr/lib/qt3/include/qscrollbar.h
CustomerTickets.o: /usr/lib/qt3/include/qrangecontrol.h
CustomerTickets.o: /usr/lib/qt3/include/qtextview.h
CustomerTickets.o: /usr/lib/qt3/include/qtextedit.h
CustomerTickets.o: /usr/lib/qt3/include/qstylesheet.h
CustomerTickets.o: /usr/lib/qt3/include/qptrvector.h
CustomerTickets.o: /usr/lib/qt3/include/qgvector.h
CustomerTickets.o: /usr/lib/qt3/include/qdict.h /usr/lib/qt3/include/qgdict.h
CustomerTickets.o: /usr/lib/qt3/include/qcombobox.h
CustomerTickets.o: /usr/lib/qt3/include/qlabel.h
CustomerTickets.o: /usr/lib/qt3/include/qpushbutton.h
CustomerTickets.o: /usr/lib/qt3/include/qbutton.h
CustomerTickets.o: /usr/lib/qt3/include/qkeysequence.h
CustomerTickets.o: /usr/lib/qt3/include/qiconset.h
CustomerTickets.o: /usr/lib/qt3/include/qpixmap.h
CustomerTickets.o: /usr/lib/qt3/include/qmessagebox.h
CustomerTickets.o: /usr/lib/qt3/include/qdialog.h /usr/include/stdlib.h
CustomerTickets.o: /usr/lib/qt3/include/qheader.h
CustomerTickets.o: /usr/lib/qt3/include/qlayout.h
CustomerTickets.o: /usr/lib/qt3/include/qtimer.h Ticket.h
CustomerTickets.o: /usr/include/sys/types.h /usr/include/time.h
CreateCustTicket.o: /usr/include/stdio.h /usr/include/features.h
CreateCustTicket.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
CreateCustTicket.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
CreateCustTicket.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CreateCustTicket.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CreateCustTicket.o: /usr/include/_G_config.h /usr/include/wchar.h
CreateCustTicket.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CreateCustTicket.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
CreateCustTicket.o: /usr/include/bits/stdio_lim.h
CreateCustTicket.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
CreateCustTicket.o: Ticket.h /usr/include/sys/types.h /usr/include/time.h
CreateCustTicket.o: TicketEditor.h /usr/lib/qt3/include/qdatetm.h
CreateCustTicket.o: /usr/lib/qt3/include/qdatetime.h
CreateCustTicket.o: /usr/lib/qt3/include/qstring.h
CreateCustTicket.o: /usr/lib/qt3/include/qcstring.h
CreateCustTicket.o: /usr/lib/qt3/include/qmemarray.h
CreateCustTicket.o: /usr/lib/qt3/include/qgarray.h
CreateCustTicket.o: /usr/lib/qt3/include/qshared.h
CreateCustTicket.o: /usr/lib/qt3/include/qglobal.h
CreateCustTicket.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
CreateCustTicket.o: /usr/include/limits.h
CreateCustTicket.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
CreateCustTicket.o: /usr/lib/qt3/include/qnamespace.h
CreateCustTicket.o: /usr/lib/qt3/include/qpainter.h
CreateCustTicket.o: /usr/lib/qt3/include/qcolor.h
CreateCustTicket.o: /usr/lib/qt3/include/qwindowdefs.h
CreateCustTicket.o: /usr/lib/qt3/include/qobjectdefs.h
CreateCustTicket.o: /usr/lib/qt3/include/qstringlist.h
CreateCustTicket.o: /usr/lib/qt3/include/qvaluelist.h
CreateCustTicket.o: /usr/lib/qt3/include/qtl.h
CreateCustTicket.o: /usr/lib/qt3/include/qtextstream.h
CreateCustTicket.o: /usr/lib/qt3/include/qiodevice.h
CreateCustTicket.o: /usr/lib/qt3/include/qdatastream.h
CreateCustTicket.o: /usr/lib/qt3/include/qstrlist.h
CreateCustTicket.o: /usr/lib/qt3/include/qptrlist.h
CreateCustTicket.o: /usr/lib/qt3/include/qglist.h
CreateCustTicket.o: /usr/lib/qt3/include/qptrcollection.h
CreateCustTicket.o: /usr/lib/qt3/include/qfontmetrics.h
CreateCustTicket.o: /usr/lib/qt3/include/qfont.h /usr/lib/qt3/include/qrect.h
CreateCustTicket.o: /usr/lib/qt3/include/qsize.h
CreateCustTicket.o: /usr/lib/qt3/include/qpoint.h
CreateCustTicket.o: /usr/lib/qt3/include/qfontinfo.h
CreateCustTicket.o: /usr/lib/qt3/include/qregion.h
CreateCustTicket.o: /usr/lib/qt3/include/qpen.h /usr/lib/qt3/include/qbrush.h
CreateCustTicket.o: /usr/lib/qt3/include/qpointarray.h
CreateCustTicket.o: /usr/lib/qt3/include/qwmatrix.h
CreateCustTicket.o: /usr/lib/qt3/include/qlistview.h
CreateCustTicket.o: /usr/lib/qt3/include/qscrollview.h
CreateCustTicket.o: /usr/lib/qt3/include/qframe.h
CreateCustTicket.o: /usr/lib/qt3/include/qwidget.h
CreateCustTicket.o: /usr/lib/qt3/include/qobject.h
CreateCustTicket.o: /usr/lib/qt3/include/qevent.h
CreateCustTicket.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
CreateCustTicket.o: /usr/lib/qt3/include/qpair.h
CreateCustTicket.o: /usr/lib/qt3/include/qpaintdevice.h
CreateCustTicket.o: /usr/lib/qt3/include/qpalette.h
CreateCustTicket.o: /usr/lib/qt3/include/qsizepolicy.h
CreateCustTicket.o: /usr/lib/qt3/include/qscrollbar.h
CreateCustTicket.o: /usr/lib/qt3/include/qrangecontrol.h
CreateCustTicket.o: /usr/lib/qt3/include/qtextview.h
CreateCustTicket.o: /usr/lib/qt3/include/qtextedit.h
CreateCustTicket.o: /usr/lib/qt3/include/qstylesheet.h
CreateCustTicket.o: /usr/lib/qt3/include/qptrvector.h
CreateCustTicket.o: /usr/lib/qt3/include/qgvector.h
CreateCustTicket.o: /usr/lib/qt3/include/qdict.h
CreateCustTicket.o: /usr/lib/qt3/include/qgdict.h
CreateCustTicket.o: /usr/lib/qt3/include/qcombobox.h
CreateCustTicket.o: /usr/lib/qt3/include/qlabel.h
CreateCustTicket.o: /usr/lib/qt3/include/qpushbutton.h
CreateCustTicket.o: /usr/lib/qt3/include/qbutton.h
CreateCustTicket.o: /usr/lib/qt3/include/qkeysequence.h
CreateCustTicket.o: /usr/lib/qt3/include/qiconset.h
CreateCustTicket.o: /usr/lib/qt3/include/qpixmap.h
CreateCustTicket.o: /usr/lib/qt3/include/qmultilineedit.h
CreateCustTicket.o: /usr/lib/qt3/include/qcheckbox.h CreateCustTicket.h
CreateCustTicket.o: /usr/lib/qt3/include/qmessagebox.h
CreateCustTicket.o: /usr/lib/qt3/include/qdialog.h
CreateCustTicket.o: /usr/lib/qt3/include/qheader.h
CreateCustTicket.o: /usr/lib/qt3/include/qlayout.h
TicketEditor.o: /usr/include/stdio.h /usr/include/features.h
TicketEditor.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TicketEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TicketEditor.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TicketEditor.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TicketEditor.o: /usr/include/_G_config.h /usr/include/wchar.h
TicketEditor.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TicketEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TicketEditor.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TicketEditor.o: /usr/include/stdlib.h Ticket.h /usr/include/sys/types.h
TicketEditor.o: /usr/include/time.h TicketEditor.h
TicketEditor.o: /usr/lib/qt3/include/qdatetm.h
TicketEditor.o: /usr/lib/qt3/include/qdatetime.h
TicketEditor.o: /usr/lib/qt3/include/qstring.h
TicketEditor.o: /usr/lib/qt3/include/qcstring.h
TicketEditor.o: /usr/lib/qt3/include/qmemarray.h
TicketEditor.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
TicketEditor.o: /usr/lib/qt3/include/qglobal.h
TicketEditor.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
TicketEditor.o: /usr/include/limits.h
TicketEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
TicketEditor.o: /usr/lib/qt3/include/qnamespace.h
TicketEditor.o: /usr/lib/qt3/include/qpainter.h /usr/lib/qt3/include/qcolor.h
TicketEditor.o: /usr/lib/qt3/include/qwindowdefs.h
TicketEditor.o: /usr/lib/qt3/include/qobjectdefs.h
TicketEditor.o: /usr/lib/qt3/include/qstringlist.h
TicketEditor.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
TicketEditor.o: /usr/lib/qt3/include/qtextstream.h
TicketEditor.o: /usr/lib/qt3/include/qiodevice.h
TicketEditor.o: /usr/lib/qt3/include/qdatastream.h
TicketEditor.o: /usr/lib/qt3/include/qstrlist.h
TicketEditor.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
TicketEditor.o: /usr/lib/qt3/include/qptrcollection.h
TicketEditor.o: /usr/lib/qt3/include/qfontmetrics.h
TicketEditor.o: /usr/lib/qt3/include/qfont.h /usr/lib/qt3/include/qrect.h
TicketEditor.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
TicketEditor.o: /usr/lib/qt3/include/qfontinfo.h
TicketEditor.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qpen.h
TicketEditor.o: /usr/lib/qt3/include/qbrush.h
TicketEditor.o: /usr/lib/qt3/include/qpointarray.h
TicketEditor.o: /usr/lib/qt3/include/qwmatrix.h
TicketEditor.o: /usr/lib/qt3/include/qlistview.h
TicketEditor.o: /usr/lib/qt3/include/qscrollview.h
TicketEditor.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qwidget.h
TicketEditor.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
TicketEditor.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
TicketEditor.o: /usr/lib/qt3/include/qpair.h
TicketEditor.o: /usr/lib/qt3/include/qpaintdevice.h
TicketEditor.o: /usr/lib/qt3/include/qpalette.h
TicketEditor.o: /usr/lib/qt3/include/qsizepolicy.h
TicketEditor.o: /usr/lib/qt3/include/qscrollbar.h
TicketEditor.o: /usr/lib/qt3/include/qrangecontrol.h
TicketEditor.o: /usr/lib/qt3/include/qtextview.h
TicketEditor.o: /usr/lib/qt3/include/qtextedit.h
TicketEditor.o: /usr/lib/qt3/include/qstylesheet.h
TicketEditor.o: /usr/lib/qt3/include/qptrvector.h
TicketEditor.o: /usr/lib/qt3/include/qgvector.h /usr/lib/qt3/include/qdict.h
TicketEditor.o: /usr/lib/qt3/include/qgdict.h
TicketEditor.o: /usr/lib/qt3/include/qcombobox.h
TicketEditor.o: /usr/lib/qt3/include/qlabel.h
TicketEditor.o: /usr/lib/qt3/include/qpushbutton.h
TicketEditor.o: /usr/lib/qt3/include/qbutton.h
TicketEditor.o: /usr/lib/qt3/include/qkeysequence.h
TicketEditor.o: /usr/lib/qt3/include/qiconset.h
TicketEditor.o: /usr/lib/qt3/include/qpixmap.h
TicketEditor.o: /usr/lib/qt3/include/qmultilineedit.h
TicketEditor.o: /usr/lib/qt3/include/qcheckbox.h
TicketEditor.o: /usr/lib/qt3/include/qmessagebox.h
TicketEditor.o: /usr/lib/qt3/include/qdialog.h /usr/lib/qt3/include/qheader.h
TicketEditor.o: /usr/lib/qt3/include/qlayout.h
