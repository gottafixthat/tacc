####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -O2 -fno-strength-reduce -I/usr/X11R6/include -I. -I../include/
CFLAGS	+= -I../lib -DADBQT
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

# CFLAGS += -DDBDEBUG

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	LogVoiceMail.cpp\
			ProcessVoiceMail.cpp

# HEADERS =	../include/BlargDB.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_LogVoiceMail.cpp\
			moc_ProcessVoiceMail.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libvoicemail.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) db-lib

db-lib: $(OBJECTS)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)
	# mv libblarg.a lib

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_LogVoiceMail.cpp: LogVoiceMail.h
	$(MOC) LogVoiceMail.h -o moc_LogVoiceMail.cpp

moc_ProcessVoiceMail.cpp: ProcessVoiceMail.h
	$(MOC) ProcessVoiceMail.h -o moc_ProcessVoiceMail.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

LogVoiceMail.o: /usr/include/stdio.h /usr/include/features.h
LogVoiceMail.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
LogVoiceMail.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
LogVoiceMail.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
LogVoiceMail.o: /usr/include/bits/typesizes.h /usr/include/libio.h
LogVoiceMail.o: /usr/include/_G_config.h /usr/include/wchar.h
LogVoiceMail.o: /usr/include/bits/wchar.h /usr/include/gconv.h
LogVoiceMail.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
LogVoiceMail.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
LogVoiceMail.o: /usr/include/stdlib.h LogVoiceMail.h
LogVoiceMail.o: /usr/qt/3/include/qwidget.h /usr/qt/3/include/qwindowdefs.h
LogVoiceMail.o: /usr/qt/3/include/qobjectdefs.h /usr/qt/3/include/qglobal.h
LogVoiceMail.o: /usr/qt/3/include/qstring.h /usr/qt/3/include/qcstring.h
LogVoiceMail.o: /usr/qt/3/include/qmemarray.h /usr/qt/3/include/qgarray.h
LogVoiceMail.o: /usr/qt/3/include/qshared.h /usr/qt/3/include/qwinexport.h
LogVoiceMail.o: /usr/include/string.h /usr/include/limits.h
LogVoiceMail.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
LogVoiceMail.o: /usr/qt/3/include/qnamespace.h /usr/qt/3/include/qobject.h
LogVoiceMail.o: /usr/qt/3/include/qevent.h /usr/qt/3/include/qregion.h
LogVoiceMail.o: /usr/qt/3/include/qrect.h /usr/qt/3/include/qsize.h
LogVoiceMail.o: /usr/qt/3/include/qpoint.h /usr/qt/3/include/qmime.h
LogVoiceMail.o: /usr/qt/3/include/qmap.h /usr/qt/3/include/qdatastream.h
LogVoiceMail.o: /usr/qt/3/include/qiodevice.h /usr/qt/3/include/qpair.h
LogVoiceMail.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
LogVoiceMail.o: /usr/qt/3/include/qtextstream.h
LogVoiceMail.o: /usr/qt/3/include/qpaintdevice.h /usr/qt/3/include/qpalette.h
LogVoiceMail.o: /usr/qt/3/include/qcolor.h /usr/qt/3/include/qstringlist.h
LogVoiceMail.o: /usr/qt/3/include/qstrlist.h /usr/qt/3/include/qptrlist.h
LogVoiceMail.o: /usr/qt/3/include/qglist.h /usr/qt/3/include/qptrcollection.h
LogVoiceMail.o: /usr/qt/3/include/qbrush.h /usr/qt/3/include/qfont.h
LogVoiceMail.o: /usr/qt/3/include/qfontmetrics.h
LogVoiceMail.o: /usr/qt/3/include/qfontinfo.h /usr/qt/3/include/qsizepolicy.h
LogVoiceMail.o: /usr/qt/3/include/qlabel.h /usr/qt/3/include/qframe.h
LogVoiceMail.o: /usr/qt/3/include/qpushbutton.h /usr/qt/3/include/qbutton.h
LogVoiceMail.o: /usr/qt/3/include/qkeysequence.h /usr/qt/3/include/qiconset.h
LogVoiceMail.o: /usr/qt/3/include/qpixmap.h /usr/qt/3/include/qmultilinedit.h
LogVoiceMail.o: /usr/qt/3/include/qmultilineedit.h
LogVoiceMail.o: /usr/qt/3/include/qtextedit.h /usr/qt/3/include/qscrollview.h
LogVoiceMail.o: /usr/qt/3/include/qscrollbar.h
LogVoiceMail.o: /usr/qt/3/include/qrangecontrol.h
LogVoiceMail.o: /usr/qt/3/include/qstylesheet.h
LogVoiceMail.o: /usr/qt/3/include/qptrvector.h /usr/qt/3/include/qgvector.h
LogVoiceMail.o: /usr/qt/3/include/qdict.h /usr/qt/3/include/qgdict.h
LogVoiceMail.o: /usr/qt/3/include/qdatetimeedit.h
LogVoiceMail.o: /usr/qt/3/include/qdatetime.h /usr/qt/3/include/qlayout.h
ProcessVoiceMail.o: /usr/include/stdio.h /usr/include/features.h
ProcessVoiceMail.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ProcessVoiceMail.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
ProcessVoiceMail.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
ProcessVoiceMail.o: /usr/include/bits/typesizes.h /usr/include/libio.h
ProcessVoiceMail.o: /usr/include/_G_config.h /usr/include/wchar.h
ProcessVoiceMail.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ProcessVoiceMail.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
ProcessVoiceMail.o: /usr/include/bits/stdio_lim.h
ProcessVoiceMail.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
ProcessVoiceMail.o: /usr/qt/3/include/qdatetm.h /usr/qt/3/include/qdatetime.h
ProcessVoiceMail.o: /usr/qt/3/include/qstring.h /usr/qt/3/include/qcstring.h
ProcessVoiceMail.o: /usr/qt/3/include/qmemarray.h /usr/qt/3/include/qgarray.h
ProcessVoiceMail.o: /usr/qt/3/include/qshared.h /usr/qt/3/include/qglobal.h
ProcessVoiceMail.o: /usr/qt/3/include/qwinexport.h /usr/include/string.h
ProcessVoiceMail.o: /usr/include/limits.h
ProcessVoiceMail.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/limits.h
ProcessVoiceMail.o: /usr/qt/3/include/qnamespace.h
ProcessVoiceMail.o: /usr/qt/3/include/qlayout.h /usr/qt/3/include/qobject.h
ProcessVoiceMail.o: /usr/qt/3/include/qobjectdefs.h
ProcessVoiceMail.o: /usr/qt/3/include/qwindowdefs.h
ProcessVoiceMail.o: /usr/qt/3/include/qevent.h /usr/qt/3/include/qregion.h
ProcessVoiceMail.o: /usr/qt/3/include/qrect.h /usr/qt/3/include/qsize.h
ProcessVoiceMail.o: /usr/qt/3/include/qpoint.h /usr/qt/3/include/qmime.h
ProcessVoiceMail.o: /usr/qt/3/include/qmap.h /usr/qt/3/include/qdatastream.h
ProcessVoiceMail.o: /usr/qt/3/include/qiodevice.h /usr/qt/3/include/qpair.h
ProcessVoiceMail.o: /usr/qt/3/include/qvaluelist.h /usr/qt/3/include/qtl.h
ProcessVoiceMail.o: /usr/qt/3/include/qtextstream.h
ProcessVoiceMail.o: /usr/qt/3/include/qsizepolicy.h
ProcessVoiceMail.o: /usr/qt/3/include/qwidget.h
ProcessVoiceMail.o: /usr/qt/3/include/qpaintdevice.h
ProcessVoiceMail.o: /usr/qt/3/include/qpalette.h /usr/qt/3/include/qcolor.h
ProcessVoiceMail.o: /usr/qt/3/include/qstringlist.h
ProcessVoiceMail.o: /usr/qt/3/include/qstrlist.h /usr/qt/3/include/qptrlist.h
ProcessVoiceMail.o: /usr/qt/3/include/qglist.h
ProcessVoiceMail.o: /usr/qt/3/include/qptrcollection.h
ProcessVoiceMail.o: /usr/qt/3/include/qbrush.h /usr/qt/3/include/qfont.h
ProcessVoiceMail.o: /usr/qt/3/include/qfontmetrics.h
ProcessVoiceMail.o: /usr/qt/3/include/qfontinfo.h /usr/qt/3/include/qtimer.h
ProcessVoiceMail.o: /usr/qt/3/include/qapplication.h
ProcessVoiceMail.o: /usr/qt/3/include/qdesktopwidget.h
ProcessVoiceMail.o: /usr/qt/3/include/qasciidict.h /usr/qt/3/include/qgdict.h
ProcessVoiceMail.o: /usr/qt/3/include/qtranslator.h ProcessVoiceMail.h
ProcessVoiceMail.o: /usr/qt/3/include/qlabel.h /usr/qt/3/include/qframe.h
ProcessVoiceMail.o: /usr/qt/3/include/qcheckbox.h /usr/qt/3/include/qbutton.h
ProcessVoiceMail.o: /usr/qt/3/include/qkeysequence.h
ProcessVoiceMail.o: /usr/qt/3/include/qpushbutton.h
ProcessVoiceMail.o: /usr/qt/3/include/qiconset.h /usr/qt/3/include/qpixmap.h
ProcessVoiceMail.o: /usr/qt/3/include/qlistview.h
ProcessVoiceMail.o: /usr/qt/3/include/qscrollview.h
ProcessVoiceMail.o: /usr/qt/3/include/qscrollbar.h
ProcessVoiceMail.o: /usr/qt/3/include/qrangecontrol.h
ProcessVoiceMail.o: /usr/qt/3/include/qcombobox.h
ProcessVoiceMail.o: /usr/qt/3/include/qmultilineedit.h
ProcessVoiceMail.o: /usr/qt/3/include/qtextedit.h
ProcessVoiceMail.o: /usr/qt/3/include/qstylesheet.h
ProcessVoiceMail.o: /usr/qt/3/include/qptrvector.h
ProcessVoiceMail.o: /usr/qt/3/include/qgvector.h /usr/qt/3/include/qdict.h
ProcessVoiceMail.o: /usr/qt/3/include/qtextview.h
ProcessVoiceMail.o: /usr/qt/3/include/qpopupmenu.h
ProcessVoiceMail.o: /usr/qt/3/include/qmenudata.h /usr/qt/3/include/qsignal.h
ProcessVoiceMail.o: /usr/qt/3/include/qvariant.h LogVoiceMail.h
ProcessVoiceMail.o: /usr/qt/3/include/qmultilinedit.h
ProcessVoiceMail.o: /usr/qt/3/include/qdatetimeedit.h
