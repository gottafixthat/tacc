####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)/include
CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. 
CFLAGS += -I../include
CFLAGS += -I../customers
CFLAGS += -I../qgtimespinbox
CFLAGS += -I../lib
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

SOURCES =	CCBatch.cpp CCMonetra.cpp

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_CCBatch.cpp moc_CCMonetra.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libccards.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% *.a #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_CCBatch.cpp: CCBatch.h
	$(MOC) CCBatch.h -o moc_CCBatch.cpp

moc_CCMonetra.cpp: CCMonetra.h
	$(MOC) CCMonetra.h -o moc_CCMonetra.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

CCBatch.o: CCBatch.h /usr/qt/3/include/qwidget.h
CCBatch.o: /usr/qt/3/include/qwindowdefs.h /usr/qt/3/include/qobjectdefs.h
CCBatch.o: /usr/qt/3/include/qglobal.h /usr/qt/3/include/qstring.h
CCBatch.o: /usr/qt/3/include/qcstring.h /usr/qt/3/include/qmemarray.h
CCBatch.o: /usr/qt/3/include/qgarray.h /usr/qt/3/include/qshared.h
CCBatch.o: /usr/qt/3/include/qwinexport.h /usr/include/string.h
CCBatch.o: /usr/include/features.h /usr/include/sys/cdefs.h
CCBatch.o: /usr/include/gnu/stubs.h
CCBatch.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
CCBatch.o: /usr/include/limits.h
CCBatch.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
CCBatch.o: /usr/qt/3/include/qnamespace.h /usr/qt/3/include/qobject.h
CCBatch.o: /usr/qt/3/include/qevent.h /usr/qt/3/include/qregion.h
CCBatch.o: /usr/qt/3/include/qrect.h /usr/qt/3/include/qsize.h
CCBatch.o: /usr/qt/3/include/qpoint.h /usr/qt/3/include/qmime.h
CCBatch.o: /usr/qt/3/include/qmap.h /usr/qt/3/include/qdatastream.h
CCBatch.o: /usr/qt/3/include/qiodevice.h /usr/qt/3/include/qpair.h
CCBatch.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
CCBatch.o: /usr/qt/3/include/qtextstream.h /usr/include/stdio.h
CCBatch.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CCBatch.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CCBatch.o: /usr/include/_G_config.h /usr/include/wchar.h
CCBatch.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CCBatch.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
CCBatch.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
CCBatch.o: /usr/qt/3/include/qpaintdevice.h /usr/qt/3/include/qpalette.h
CCBatch.o: /usr/qt/3/include/qcolor.h /usr/qt/3/include/qstringlist.h
CCBatch.o: /usr/qt/3/include/qstrlist.h /usr/qt/3/include/qptrlist.h
CCBatch.o: /usr/qt/3/include/qglist.h /usr/qt/3/include/qptrcollection.h
CCBatch.o: /usr/qt/3/include/qbrush.h /usr/qt/3/include/qfont.h
CCBatch.o: /usr/qt/3/include/qfontmetrics.h /usr/qt/3/include/qfontinfo.h
CCBatch.o: /usr/qt/3/include/qsizepolicy.h /usr/qt/3/include/qlabel.h
CCBatch.o: /usr/qt/3/include/qframe.h /usr/qt/3/include/qlistview.h
CCBatch.o: /usr/qt/3/include/qscrollview.h /usr/qt/3/include/qscrollbar.h
CCBatch.o: /usr/qt/3/include/qrangecontrol.h /usr/qt/3/include/qpushbutton.h
CCBatch.o: /usr/qt/3/include/qbutton.h /usr/qt/3/include/qkeysequence.h
CCBatch.o: /usr/qt/3/include/qiconset.h /usr/qt/3/include/qpixmap.h
CCBatch.o: /usr/include/stdlib.h /usr/include/ctype.h /usr/include/endian.h
CCBatch.o: /usr/include/bits/endian.h /usr/qt/3/include/qmessagebox.h
CCBatch.o: /usr/qt/3/include/qdialog.h /usr/qt/3/include/qprogressdialog.h
CCBatch.o: /usr/qt/3/include/qsemimodal.h /usr/qt/3/include/qprogressbar.h
CCBatch.o: /usr/qt/3/include/qfiledialog.h /usr/qt/3/include/qdir.h
CCBatch.o: /usr/qt/3/include/qfileinfo.h /usr/qt/3/include/qfile.h
CCBatch.o: /usr/qt/3/include/qdatetime.h /usr/qt/3/include/qurloperator.h
CCBatch.o: /usr/qt/3/include/qurl.h /usr/qt/3/include/qnetworkprotocol.h
CCBatch.o: /usr/qt/3/include/qurlinfo.h /usr/qt/3/include/qdict.h
CCBatch.o: /usr/qt/3/include/qgdict.h /usr/qt/3/include/qapplication.h
CCBatch.o: /usr/qt/3/include/qdesktopwidget.h /usr/qt/3/include/qasciidict.h
CCBatch.o: /usr/qt/3/include/qtranslator.h /usr/qt/3/include/qregexp.h
CCBatch.o: /usr/qt/3/include/qlayout.h
CCMonetra.o: CCMonetra.h /usr/qt/3/include/qwidget.h
CCMonetra.o: /usr/qt/3/include/qwindowdefs.h /usr/qt/3/include/qobjectdefs.h
CCMonetra.o: /usr/qt/3/include/qglobal.h /usr/qt/3/include/qstring.h
CCMonetra.o: /usr/qt/3/include/qcstring.h /usr/qt/3/include/qmemarray.h
CCMonetra.o: /usr/qt/3/include/qgarray.h /usr/qt/3/include/qshared.h
CCMonetra.o: /usr/qt/3/include/qwinexport.h /usr/include/string.h
CCMonetra.o: /usr/include/features.h /usr/include/sys/cdefs.h
CCMonetra.o: /usr/include/gnu/stubs.h
CCMonetra.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
CCMonetra.o: /usr/include/limits.h
CCMonetra.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
CCMonetra.o: /usr/qt/3/include/qnamespace.h /usr/qt/3/include/qobject.h
CCMonetra.o: /usr/qt/3/include/qevent.h /usr/qt/3/include/qregion.h
CCMonetra.o: /usr/qt/3/include/qrect.h /usr/qt/3/include/qsize.h
CCMonetra.o: /usr/qt/3/include/qpoint.h /usr/qt/3/include/qmime.h
CCMonetra.o: /usr/qt/3/include/qmap.h /usr/qt/3/include/qdatastream.h
CCMonetra.o: /usr/qt/3/include/qiodevice.h /usr/qt/3/include/qpair.h
CCMonetra.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
CCMonetra.o: /usr/qt/3/include/qtextstream.h /usr/include/stdio.h
CCMonetra.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CCMonetra.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CCMonetra.o: /usr/include/_G_config.h /usr/include/wchar.h
CCMonetra.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CCMonetra.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
CCMonetra.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
CCMonetra.o: /usr/qt/3/include/qpaintdevice.h /usr/qt/3/include/qpalette.h
CCMonetra.o: /usr/qt/3/include/qcolor.h /usr/qt/3/include/qstringlist.h
CCMonetra.o: /usr/qt/3/include/qstrlist.h /usr/qt/3/include/qptrlist.h
CCMonetra.o: /usr/qt/3/include/qglist.h /usr/qt/3/include/qptrcollection.h
CCMonetra.o: /usr/qt/3/include/qbrush.h /usr/qt/3/include/qfont.h
CCMonetra.o: /usr/qt/3/include/qfontmetrics.h /usr/qt/3/include/qfontinfo.h
CCMonetra.o: /usr/qt/3/include/qsizepolicy.h /usr/qt/3/include/qlabel.h
CCMonetra.o: /usr/qt/3/include/qframe.h /usr/qt/3/include/qlistview.h
CCMonetra.o: /usr/qt/3/include/qscrollview.h /usr/qt/3/include/qscrollbar.h
CCMonetra.o: /usr/qt/3/include/qrangecontrol.h
CCMonetra.o: /usr/qt/3/include/qpushbutton.h /usr/qt/3/include/qbutton.h
CCMonetra.o: /usr/qt/3/include/qkeysequence.h /usr/qt/3/include/qiconset.h
CCMonetra.o: /usr/qt/3/include/qpixmap.h /usr/qt/3/include/qtimer.h
CCMonetra.o: /usr/include/stdlib.h /usr/include/ctype.h /usr/include/endian.h
CCMonetra.o: /usr/include/bits/endian.h /usr/qt/3/include/qmessagebox.h
CCMonetra.o: /usr/qt/3/include/qdialog.h /usr/qt/3/include/qprogressdialog.h
CCMonetra.o: /usr/qt/3/include/qsemimodal.h /usr/qt/3/include/qprogressbar.h
CCMonetra.o: /usr/qt/3/include/qfiledialog.h /usr/qt/3/include/qdir.h
CCMonetra.o: /usr/qt/3/include/qfileinfo.h /usr/qt/3/include/qfile.h
CCMonetra.o: /usr/qt/3/include/qdatetime.h /usr/qt/3/include/qurloperator.h
CCMonetra.o: /usr/qt/3/include/qurl.h /usr/qt/3/include/qnetworkprotocol.h
CCMonetra.o: /usr/qt/3/include/qurlinfo.h /usr/qt/3/include/qdict.h
CCMonetra.o: /usr/qt/3/include/qgdict.h /usr/qt/3/include/qeventloop.h
CCMonetra.o: /usr/qt/3/include/qsocketnotifier.h /usr/qt/3/include/qregexp.h
CCMonetra.o: /usr/qt/3/include/qlayout.h
