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
Ticket.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
Ticket.o: /usr/include/bits/typesizes.h /usr/include/time.h
Ticket.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
Ticket.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
Ticket.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
Ticket.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
Ticket.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
Ticket.o: /usr/include/bits/confname.h /usr/include/string.h
Ticket.o: /usr/include/sys/time.h /usr/include/bits/time.h
Ticket.o: /usr/include/sys/select.h /usr/include/bits/select.h
Ticket.o: /usr/include/bits/sigset.h
TicketManager.o: /usr/include/stdio.h /usr/include/features.h
TicketManager.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TicketManager.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
TicketManager.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TicketManager.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TicketManager.o: /usr/include/_G_config.h /usr/include/wchar.h
TicketManager.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TicketManager.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
TicketManager.o: /usr/include/bits/stdio_lim.h
TicketManager.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
TicketManager.o: TicketManager.h /usr/qt/3/include/qdatetm.h
TicketManager.o: /usr/qt/3/include/qdatetime.h /usr/qt/3/include/qstring.h
TicketManager.o: /usr/qt/3/include/qcstring.h /usr/qt/3/include/qmemarray.h
TicketManager.o: /usr/qt/3/include/qgarray.h /usr/qt/3/include/qshared.h
TicketManager.o: /usr/qt/3/include/qglobal.h /usr/qt/3/include/qwinexport.h
TicketManager.o: /usr/include/string.h /usr/include/limits.h
TicketManager.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
TicketManager.o: /usr/qt/3/include/qnamespace.h /usr/qt/3/include/qpainter.h
TicketManager.o: /usr/qt/3/include/qcolor.h /usr/qt/3/include/qwindowdefs.h
TicketManager.o: /usr/qt/3/include/qobjectdefs.h
TicketManager.o: /usr/qt/3/include/qstringlist.h
TicketManager.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
TicketManager.o: /usr/qt/3/include/qtextstream.h
TicketManager.o: /usr/qt/3/include/qiodevice.h
TicketManager.o: /usr/qt/3/include/qdatastream.h /usr/qt/3/include/qstrlist.h
TicketManager.o: /usr/qt/3/include/qptrlist.h /usr/qt/3/include/qglist.h
TicketManager.o: /usr/qt/3/include/qptrcollection.h
TicketManager.o: /usr/qt/3/include/qfontmetrics.h /usr/qt/3/include/qfont.h
TicketManager.o: /usr/qt/3/include/qrect.h /usr/qt/3/include/qsize.h
TicketManager.o: /usr/qt/3/include/qpoint.h /usr/qt/3/include/qfontinfo.h
TicketManager.o: /usr/qt/3/include/qregion.h /usr/qt/3/include/qpen.h
TicketManager.o: /usr/qt/3/include/qbrush.h /usr/qt/3/include/qpointarray.h
TicketManager.o: /usr/qt/3/include/qwmatrix.h /usr/qt/3/include/qlistview.h
TicketManager.o: /usr/qt/3/include/qscrollview.h /usr/qt/3/include/qframe.h
TicketManager.o: /usr/qt/3/include/qwidget.h /usr/qt/3/include/qobject.h
TicketManager.o: /usr/qt/3/include/qevent.h /usr/qt/3/include/qmime.h
TicketManager.o: /usr/qt/3/include/qmap.h /usr/qt/3/include/qpair.h
TicketManager.o: /usr/qt/3/include/qpaintdevice.h
TicketManager.o: /usr/qt/3/include/qpalette.h /usr/qt/3/include/qsizepolicy.h
TicketManager.o: /usr/qt/3/include/qscrollbar.h
TicketManager.o: /usr/qt/3/include/qrangecontrol.h
TicketManager.o: /usr/qt/3/include/qtextview.h /usr/qt/3/include/qtextedit.h
TicketManager.o: /usr/qt/3/include/qstylesheet.h
TicketManager.o: /usr/qt/3/include/qptrvector.h /usr/qt/3/include/qgvector.h
TicketManager.o: /usr/qt/3/include/qdict.h /usr/qt/3/include/qgdict.h
TicketManager.o: /usr/qt/3/include/qcombobox.h /usr/qt/3/include/qcheckbox.h
TicketManager.o: /usr/qt/3/include/qbutton.h /usr/qt/3/include/qkeysequence.h
TicketManager.o: /usr/qt/3/include/qlabel.h /usr/qt/3/include/qpushbutton.h
TicketManager.o: /usr/qt/3/include/qiconset.h /usr/qt/3/include/qpixmap.h
TicketManager.o: /usr/qt/3/include/qpopupmenu.h /usr/qt/3/include/qmenudata.h
TicketManager.o: /usr/qt/3/include/qsignal.h /usr/qt/3/include/qvariant.h
TicketManager.o: TicketsCommon.h /usr/qt/3/include/qlayout.h
TicketManager.o: /usr/qt/3/include/qtimer.h Ticket.h /usr/include/sys/types.h
TicketManager.o: /usr/include/time.h TicketEditor.h
TicketManager.o: /usr/qt/3/include/qmultilineedit.h
CustomerTickets.o: CustomerTickets.h /usr/qt/3/include/qdatetm.h
CustomerTickets.o: /usr/qt/3/include/qdatetime.h /usr/qt/3/include/qstring.h
CustomerTickets.o: /usr/qt/3/include/qcstring.h /usr/qt/3/include/qmemarray.h
CustomerTickets.o: /usr/qt/3/include/qgarray.h /usr/qt/3/include/qshared.h
CustomerTickets.o: /usr/qt/3/include/qglobal.h /usr/qt/3/include/qwinexport.h
CustomerTickets.o: /usr/include/string.h /usr/include/features.h
CustomerTickets.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
CustomerTickets.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
CustomerTickets.o: /usr/include/limits.h
CustomerTickets.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
CustomerTickets.o: /usr/qt/3/include/qnamespace.h
CustomerTickets.o: /usr/qt/3/include/qpainter.h /usr/qt/3/include/qcolor.h
CustomerTickets.o: /usr/qt/3/include/qwindowdefs.h
CustomerTickets.o: /usr/qt/3/include/qobjectdefs.h
CustomerTickets.o: /usr/qt/3/include/qstringlist.h
CustomerTickets.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
CustomerTickets.o: /usr/qt/3/include/qtextstream.h
CustomerTickets.o: /usr/qt/3/include/qiodevice.h /usr/include/stdio.h
CustomerTickets.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CustomerTickets.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CustomerTickets.o: /usr/include/_G_config.h /usr/include/wchar.h
CustomerTickets.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CustomerTickets.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
CustomerTickets.o: /usr/include/bits/stdio_lim.h
CustomerTickets.o: /usr/include/bits/sys_errlist.h
CustomerTickets.o: /usr/qt/3/include/qdatastream.h
CustomerTickets.o: /usr/qt/3/include/qstrlist.h /usr/qt/3/include/qptrlist.h
CustomerTickets.o: /usr/qt/3/include/qglist.h
CustomerTickets.o: /usr/qt/3/include/qptrcollection.h
CustomerTickets.o: /usr/qt/3/include/qfontmetrics.h /usr/qt/3/include/qfont.h
CustomerTickets.o: /usr/qt/3/include/qrect.h /usr/qt/3/include/qsize.h
CustomerTickets.o: /usr/qt/3/include/qpoint.h /usr/qt/3/include/qfontinfo.h
CustomerTickets.o: /usr/qt/3/include/qregion.h /usr/qt/3/include/qpen.h
CustomerTickets.o: /usr/qt/3/include/qbrush.h /usr/qt/3/include/qpointarray.h
CustomerTickets.o: /usr/qt/3/include/qwmatrix.h /usr/qt/3/include/qlistview.h
CustomerTickets.o: /usr/qt/3/include/qscrollview.h /usr/qt/3/include/qframe.h
CustomerTickets.o: /usr/qt/3/include/qwidget.h /usr/qt/3/include/qobject.h
CustomerTickets.o: /usr/qt/3/include/qevent.h /usr/qt/3/include/qmime.h
CustomerTickets.o: /usr/qt/3/include/qmap.h /usr/qt/3/include/qpair.h
CustomerTickets.o: /usr/qt/3/include/qpaintdevice.h
CustomerTickets.o: /usr/qt/3/include/qpalette.h
CustomerTickets.o: /usr/qt/3/include/qsizepolicy.h
CustomerTickets.o: /usr/qt/3/include/qscrollbar.h
CustomerTickets.o: /usr/qt/3/include/qrangecontrol.h
CustomerTickets.o: /usr/qt/3/include/qtextview.h
CustomerTickets.o: /usr/qt/3/include/qtextedit.h
CustomerTickets.o: /usr/qt/3/include/qstylesheet.h
CustomerTickets.o: /usr/qt/3/include/qptrvector.h
CustomerTickets.o: /usr/qt/3/include/qgvector.h /usr/qt/3/include/qdict.h
CustomerTickets.o: /usr/qt/3/include/qgdict.h /usr/qt/3/include/qcombobox.h
CustomerTickets.o: /usr/qt/3/include/qlabel.h /usr/qt/3/include/qpushbutton.h
CustomerTickets.o: /usr/qt/3/include/qbutton.h
CustomerTickets.o: /usr/qt/3/include/qkeysequence.h
CustomerTickets.o: /usr/qt/3/include/qiconset.h /usr/qt/3/include/qpixmap.h
CustomerTickets.o: /usr/qt/3/include/qmessagebox.h
CustomerTickets.o: /usr/qt/3/include/qdialog.h /usr/include/stdlib.h
CustomerTickets.o: /usr/qt/3/include/qheader.h /usr/qt/3/include/qlayout.h
CustomerTickets.o: /usr/qt/3/include/qtimer.h Ticket.h
CustomerTickets.o: /usr/include/sys/types.h /usr/include/time.h
CreateCustTicket.o: /usr/include/stdio.h /usr/include/features.h
CreateCustTicket.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
CreateCustTicket.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
CreateCustTicket.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CreateCustTicket.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CreateCustTicket.o: /usr/include/_G_config.h /usr/include/wchar.h
CreateCustTicket.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CreateCustTicket.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
CreateCustTicket.o: /usr/include/bits/stdio_lim.h
CreateCustTicket.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
CreateCustTicket.o: Ticket.h /usr/include/sys/types.h /usr/include/time.h
CreateCustTicket.o: TicketEditor.h /usr/qt/3/include/qdatetm.h
CreateCustTicket.o: /usr/qt/3/include/qdatetime.h /usr/qt/3/include/qstring.h
CreateCustTicket.o: /usr/qt/3/include/qcstring.h
CreateCustTicket.o: /usr/qt/3/include/qmemarray.h /usr/qt/3/include/qgarray.h
CreateCustTicket.o: /usr/qt/3/include/qshared.h /usr/qt/3/include/qglobal.h
CreateCustTicket.o: /usr/qt/3/include/qwinexport.h /usr/include/string.h
CreateCustTicket.o: /usr/include/limits.h
CreateCustTicket.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
CreateCustTicket.o: /usr/qt/3/include/qnamespace.h
CreateCustTicket.o: /usr/qt/3/include/qpainter.h /usr/qt/3/include/qcolor.h
CreateCustTicket.o: /usr/qt/3/include/qwindowdefs.h
CreateCustTicket.o: /usr/qt/3/include/qobjectdefs.h
CreateCustTicket.o: /usr/qt/3/include/qstringlist.h
CreateCustTicket.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
CreateCustTicket.o: /usr/qt/3/include/qtextstream.h
CreateCustTicket.o: /usr/qt/3/include/qiodevice.h
CreateCustTicket.o: /usr/qt/3/include/qdatastream.h
CreateCustTicket.o: /usr/qt/3/include/qstrlist.h /usr/qt/3/include/qptrlist.h
CreateCustTicket.o: /usr/qt/3/include/qglist.h
CreateCustTicket.o: /usr/qt/3/include/qptrcollection.h
CreateCustTicket.o: /usr/qt/3/include/qfontmetrics.h
CreateCustTicket.o: /usr/qt/3/include/qfont.h /usr/qt/3/include/qrect.h
CreateCustTicket.o: /usr/qt/3/include/qsize.h /usr/qt/3/include/qpoint.h
CreateCustTicket.o: /usr/qt/3/include/qfontinfo.h /usr/qt/3/include/qregion.h
CreateCustTicket.o: /usr/qt/3/include/qpen.h /usr/qt/3/include/qbrush.h
CreateCustTicket.o: /usr/qt/3/include/qpointarray.h
CreateCustTicket.o: /usr/qt/3/include/qwmatrix.h
CreateCustTicket.o: /usr/qt/3/include/qlistview.h
CreateCustTicket.o: /usr/qt/3/include/qscrollview.h
CreateCustTicket.o: /usr/qt/3/include/qframe.h /usr/qt/3/include/qwidget.h
CreateCustTicket.o: /usr/qt/3/include/qobject.h /usr/qt/3/include/qevent.h
CreateCustTicket.o: /usr/qt/3/include/qmime.h /usr/qt/3/include/qmap.h
CreateCustTicket.o: /usr/qt/3/include/qpair.h
CreateCustTicket.o: /usr/qt/3/include/qpaintdevice.h
CreateCustTicket.o: /usr/qt/3/include/qpalette.h
CreateCustTicket.o: /usr/qt/3/include/qsizepolicy.h
CreateCustTicket.o: /usr/qt/3/include/qscrollbar.h
CreateCustTicket.o: /usr/qt/3/include/qrangecontrol.h
CreateCustTicket.o: /usr/qt/3/include/qtextview.h
CreateCustTicket.o: /usr/qt/3/include/qtextedit.h
CreateCustTicket.o: /usr/qt/3/include/qstylesheet.h
CreateCustTicket.o: /usr/qt/3/include/qptrvector.h
CreateCustTicket.o: /usr/qt/3/include/qgvector.h /usr/qt/3/include/qdict.h
CreateCustTicket.o: /usr/qt/3/include/qgdict.h /usr/qt/3/include/qcombobox.h
CreateCustTicket.o: /usr/qt/3/include/qlabel.h
CreateCustTicket.o: /usr/qt/3/include/qpushbutton.h
CreateCustTicket.o: /usr/qt/3/include/qbutton.h
CreateCustTicket.o: /usr/qt/3/include/qkeysequence.h
CreateCustTicket.o: /usr/qt/3/include/qiconset.h /usr/qt/3/include/qpixmap.h
CreateCustTicket.o: /usr/qt/3/include/qmultilineedit.h
CreateCustTicket.o: /usr/qt/3/include/qcheckbox.h CreateCustTicket.h
CreateCustTicket.o: /usr/qt/3/include/qmessagebox.h
CreateCustTicket.o: /usr/qt/3/include/qdialog.h /usr/qt/3/include/qheader.h
CreateCustTicket.o: /usr/qt/3/include/qlayout.h
TicketEditor.o: /usr/include/stdio.h /usr/include/features.h
TicketEditor.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TicketEditor.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
TicketEditor.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TicketEditor.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TicketEditor.o: /usr/include/_G_config.h /usr/include/wchar.h
TicketEditor.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TicketEditor.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
TicketEditor.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TicketEditor.o: /usr/include/stdlib.h Ticket.h /usr/include/sys/types.h
TicketEditor.o: /usr/include/time.h TicketEditor.h
TicketEditor.o: /usr/qt/3/include/qdatetm.h /usr/qt/3/include/qdatetime.h
TicketEditor.o: /usr/qt/3/include/qstring.h /usr/qt/3/include/qcstring.h
TicketEditor.o: /usr/qt/3/include/qmemarray.h /usr/qt/3/include/qgarray.h
TicketEditor.o: /usr/qt/3/include/qshared.h /usr/qt/3/include/qglobal.h
TicketEditor.o: /usr/qt/3/include/qwinexport.h /usr/include/string.h
TicketEditor.o: /usr/include/limits.h
TicketEditor.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
TicketEditor.o: /usr/qt/3/include/qnamespace.h /usr/qt/3/include/qpainter.h
TicketEditor.o: /usr/qt/3/include/qcolor.h /usr/qt/3/include/qwindowdefs.h
TicketEditor.o: /usr/qt/3/include/qobjectdefs.h
TicketEditor.o: /usr/qt/3/include/qstringlist.h
TicketEditor.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
TicketEditor.o: /usr/qt/3/include/qtextstream.h /usr/qt/3/include/qiodevice.h
TicketEditor.o: /usr/qt/3/include/qdatastream.h /usr/qt/3/include/qstrlist.h
TicketEditor.o: /usr/qt/3/include/qptrlist.h /usr/qt/3/include/qglist.h
TicketEditor.o: /usr/qt/3/include/qptrcollection.h
TicketEditor.o: /usr/qt/3/include/qfontmetrics.h /usr/qt/3/include/qfont.h
TicketEditor.o: /usr/qt/3/include/qrect.h /usr/qt/3/include/qsize.h
TicketEditor.o: /usr/qt/3/include/qpoint.h /usr/qt/3/include/qfontinfo.h
TicketEditor.o: /usr/qt/3/include/qregion.h /usr/qt/3/include/qpen.h
TicketEditor.o: /usr/qt/3/include/qbrush.h /usr/qt/3/include/qpointarray.h
TicketEditor.o: /usr/qt/3/include/qwmatrix.h /usr/qt/3/include/qlistview.h
TicketEditor.o: /usr/qt/3/include/qscrollview.h /usr/qt/3/include/qframe.h
TicketEditor.o: /usr/qt/3/include/qwidget.h /usr/qt/3/include/qobject.h
TicketEditor.o: /usr/qt/3/include/qevent.h /usr/qt/3/include/qmime.h
TicketEditor.o: /usr/qt/3/include/qmap.h /usr/qt/3/include/qpair.h
TicketEditor.o: /usr/qt/3/include/qpaintdevice.h /usr/qt/3/include/qpalette.h
TicketEditor.o: /usr/qt/3/include/qsizepolicy.h
TicketEditor.o: /usr/qt/3/include/qscrollbar.h
TicketEditor.o: /usr/qt/3/include/qrangecontrol.h
TicketEditor.o: /usr/qt/3/include/qtextview.h /usr/qt/3/include/qtextedit.h
TicketEditor.o: /usr/qt/3/include/qstylesheet.h
TicketEditor.o: /usr/qt/3/include/qptrvector.h /usr/qt/3/include/qgvector.h
TicketEditor.o: /usr/qt/3/include/qdict.h /usr/qt/3/include/qgdict.h
TicketEditor.o: /usr/qt/3/include/qcombobox.h /usr/qt/3/include/qlabel.h
TicketEditor.o: /usr/qt/3/include/qpushbutton.h /usr/qt/3/include/qbutton.h
TicketEditor.o: /usr/qt/3/include/qkeysequence.h /usr/qt/3/include/qiconset.h
TicketEditor.o: /usr/qt/3/include/qpixmap.h
TicketEditor.o: /usr/qt/3/include/qmultilineedit.h
TicketEditor.o: /usr/qt/3/include/qcheckbox.h /usr/qt/3/include/qmessagebox.h
TicketEditor.o: /usr/qt/3/include/qdialog.h /usr/qt/3/include/qheader.h
TicketEditor.o: /usr/qt/3/include/qlayout.h
