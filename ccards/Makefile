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

CCBatch.o: CCBatch.h /usr/lib/qt3/include/qwidget.h
CCBatch.o: /usr/lib/qt3/include/qwindowdefs.h
CCBatch.o: /usr/lib/qt3/include/qobjectdefs.h /usr/lib/qt3/include/qglobal.h
CCBatch.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
CCBatch.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
CCBatch.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qwinexport.h
CCBatch.o: /usr/include/string.h /usr/include/features.h
CCBatch.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
CCBatch.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
CCBatch.o: /usr/include/limits.h
CCBatch.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
CCBatch.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qobject.h
CCBatch.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
CCBatch.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
CCBatch.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
CCBatch.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
CCBatch.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
CCBatch.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
CCBatch.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
CCBatch.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CCBatch.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CCBatch.o: /usr/include/_G_config.h /usr/include/wchar.h
CCBatch.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CCBatch.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
CCBatch.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
CCBatch.o: /usr/lib/qt3/include/qpaintdevice.h
CCBatch.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
CCBatch.o: /usr/lib/qt3/include/qstringlist.h /usr/lib/qt3/include/qstrlist.h
CCBatch.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
CCBatch.o: /usr/lib/qt3/include/qptrcollection.h
CCBatch.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
CCBatch.o: /usr/lib/qt3/include/qfontmetrics.h
CCBatch.o: /usr/lib/qt3/include/qfontinfo.h
CCBatch.o: /usr/lib/qt3/include/qsizepolicy.h /usr/lib/qt3/include/qlabel.h
CCBatch.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qlistview.h
CCBatch.o: /usr/lib/qt3/include/qscrollview.h
CCBatch.o: /usr/lib/qt3/include/qscrollbar.h
CCBatch.o: /usr/lib/qt3/include/qrangecontrol.h
CCBatch.o: /usr/lib/qt3/include/qpushbutton.h /usr/lib/qt3/include/qbutton.h
CCBatch.o: /usr/lib/qt3/include/qkeysequence.h
CCBatch.o: /usr/lib/qt3/include/qiconset.h /usr/lib/qt3/include/qpixmap.h
CCBatch.o: /usr/include/stdlib.h /usr/include/ctype.h /usr/include/endian.h
CCBatch.o: /usr/include/bits/endian.h /usr/lib/qt3/include/qmessagebox.h
CCBatch.o: /usr/lib/qt3/include/qdialog.h
CCBatch.o: /usr/lib/qt3/include/qprogressdialog.h
CCBatch.o: /usr/lib/qt3/include/qsemimodal.h
CCBatch.o: /usr/lib/qt3/include/qprogressbar.h
CCBatch.o: /usr/lib/qt3/include/qfiledialog.h /usr/lib/qt3/include/qdir.h
CCBatch.o: /usr/lib/qt3/include/qfileinfo.h /usr/lib/qt3/include/qfile.h
CCBatch.o: /usr/lib/qt3/include/qdatetime.h
CCBatch.o: /usr/lib/qt3/include/qurloperator.h /usr/lib/qt3/include/qurl.h
CCBatch.o: /usr/lib/qt3/include/qnetworkprotocol.h
CCBatch.o: /usr/lib/qt3/include/qurlinfo.h /usr/lib/qt3/include/qdict.h
CCBatch.o: /usr/lib/qt3/include/qgdict.h /usr/lib/qt3/include/qapplication.h
CCBatch.o: /usr/lib/qt3/include/qdesktopwidget.h
CCBatch.o: /usr/lib/qt3/include/qasciidict.h
CCBatch.o: /usr/lib/qt3/include/qtranslator.h /usr/lib/qt3/include/qregexp.h
CCBatch.o: /usr/lib/qt3/include/qlayout.h
CCMonetra.o: CCMonetra.h /usr/lib/qt3/include/qwidget.h
CCMonetra.o: /usr/lib/qt3/include/qwindowdefs.h
CCMonetra.o: /usr/lib/qt3/include/qobjectdefs.h
CCMonetra.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qstring.h
CCMonetra.o: /usr/lib/qt3/include/qcstring.h /usr/lib/qt3/include/qmemarray.h
CCMonetra.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
CCMonetra.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
CCMonetra.o: /usr/include/features.h /usr/include/sys/cdefs.h
CCMonetra.o: /usr/include/gnu/stubs.h
CCMonetra.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
CCMonetra.o: /usr/include/limits.h
CCMonetra.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
CCMonetra.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qobject.h
CCMonetra.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
CCMonetra.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
CCMonetra.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
CCMonetra.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
CCMonetra.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
CCMonetra.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
CCMonetra.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
CCMonetra.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CCMonetra.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CCMonetra.o: /usr/include/_G_config.h /usr/include/wchar.h
CCMonetra.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CCMonetra.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
CCMonetra.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
CCMonetra.o: /usr/lib/qt3/include/qpaintdevice.h
CCMonetra.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
CCMonetra.o: /usr/lib/qt3/include/qstringlist.h
CCMonetra.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
CCMonetra.o: /usr/lib/qt3/include/qglist.h
CCMonetra.o: /usr/lib/qt3/include/qptrcollection.h
CCMonetra.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
CCMonetra.o: /usr/lib/qt3/include/qfontmetrics.h
CCMonetra.o: /usr/lib/qt3/include/qfontinfo.h
CCMonetra.o: /usr/lib/qt3/include/qsizepolicy.h /usr/lib/qt3/include/qlabel.h
CCMonetra.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qlistview.h
CCMonetra.o: /usr/lib/qt3/include/qscrollview.h
CCMonetra.o: /usr/lib/qt3/include/qscrollbar.h
CCMonetra.o: /usr/lib/qt3/include/qrangecontrol.h
CCMonetra.o: /usr/lib/qt3/include/qpushbutton.h
CCMonetra.o: /usr/lib/qt3/include/qbutton.h
CCMonetra.o: /usr/lib/qt3/include/qkeysequence.h
CCMonetra.o: /usr/lib/qt3/include/qiconset.h /usr/lib/qt3/include/qpixmap.h
CCMonetra.o: /usr/lib/qt3/include/qtimer.h /usr/include/stdlib.h
CCMonetra.o: /usr/include/ctype.h /usr/include/endian.h
CCMonetra.o: /usr/include/bits/endian.h /usr/lib/qt3/include/qmessagebox.h
CCMonetra.o: /usr/lib/qt3/include/qdialog.h
CCMonetra.o: /usr/lib/qt3/include/qprogressdialog.h
CCMonetra.o: /usr/lib/qt3/include/qsemimodal.h
CCMonetra.o: /usr/lib/qt3/include/qprogressbar.h
CCMonetra.o: /usr/lib/qt3/include/qfiledialog.h /usr/lib/qt3/include/qdir.h
CCMonetra.o: /usr/lib/qt3/include/qfileinfo.h /usr/lib/qt3/include/qfile.h
CCMonetra.o: /usr/lib/qt3/include/qdatetime.h
CCMonetra.o: /usr/lib/qt3/include/qurloperator.h /usr/lib/qt3/include/qurl.h
CCMonetra.o: /usr/lib/qt3/include/qnetworkprotocol.h
CCMonetra.o: /usr/lib/qt3/include/qurlinfo.h /usr/lib/qt3/include/qdict.h
CCMonetra.o: /usr/lib/qt3/include/qgdict.h /usr/lib/qt3/include/qeventloop.h
CCMonetra.o: /usr/lib/qt3/include/qsocketnotifier.h
CCMonetra.o: /usr/lib/qt3/include/qregexp.h /usr/lib/qt3/include/qlayout.h
