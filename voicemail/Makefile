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
LogVoiceMail.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
LogVoiceMail.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
LogVoiceMail.o: /usr/include/bits/typesizes.h /usr/include/libio.h
LogVoiceMail.o: /usr/include/_G_config.h /usr/include/wchar.h
LogVoiceMail.o: /usr/include/bits/wchar.h /usr/include/gconv.h
LogVoiceMail.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
LogVoiceMail.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
LogVoiceMail.o: /usr/include/stdlib.h LogVoiceMail.h
LogVoiceMail.o: /usr/lib/qt3/include/qwidget.h
LogVoiceMail.o: /usr/lib/qt3/include/qwindowdefs.h
LogVoiceMail.o: /usr/lib/qt3/include/qobjectdefs.h
LogVoiceMail.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qstring.h
LogVoiceMail.o: /usr/lib/qt3/include/qcstring.h
LogVoiceMail.o: /usr/lib/qt3/include/qmemarray.h
LogVoiceMail.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
LogVoiceMail.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
LogVoiceMail.o: /usr/include/limits.h
LogVoiceMail.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
LogVoiceMail.o: /usr/lib/qt3/include/qnamespace.h
LogVoiceMail.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
LogVoiceMail.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qrect.h
LogVoiceMail.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
LogVoiceMail.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
LogVoiceMail.o: /usr/lib/qt3/include/qdatastream.h
LogVoiceMail.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
LogVoiceMail.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
LogVoiceMail.o: /usr/lib/qt3/include/qtextstream.h
LogVoiceMail.o: /usr/lib/qt3/include/qpaintdevice.h
LogVoiceMail.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
LogVoiceMail.o: /usr/lib/qt3/include/qstringlist.h
LogVoiceMail.o: /usr/lib/qt3/include/qstrlist.h
LogVoiceMail.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
LogVoiceMail.o: /usr/lib/qt3/include/qptrcollection.h
LogVoiceMail.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
LogVoiceMail.o: /usr/lib/qt3/include/qfontmetrics.h
LogVoiceMail.o: /usr/lib/qt3/include/qfontinfo.h
LogVoiceMail.o: /usr/lib/qt3/include/qsizepolicy.h
LogVoiceMail.o: /usr/lib/qt3/include/qlabel.h /usr/lib/qt3/include/qframe.h
LogVoiceMail.o: /usr/lib/qt3/include/qpushbutton.h
LogVoiceMail.o: /usr/lib/qt3/include/qbutton.h
LogVoiceMail.o: /usr/lib/qt3/include/qkeysequence.h
LogVoiceMail.o: /usr/lib/qt3/include/qiconset.h
LogVoiceMail.o: /usr/lib/qt3/include/qpixmap.h
LogVoiceMail.o: /usr/lib/qt3/include/qmultilinedit.h
LogVoiceMail.o: /usr/lib/qt3/include/qmultilineedit.h
LogVoiceMail.o: /usr/lib/qt3/include/qtextedit.h
LogVoiceMail.o: /usr/lib/qt3/include/qscrollview.h
LogVoiceMail.o: /usr/lib/qt3/include/qscrollbar.h
LogVoiceMail.o: /usr/lib/qt3/include/qrangecontrol.h
LogVoiceMail.o: /usr/lib/qt3/include/qstylesheet.h
LogVoiceMail.o: /usr/lib/qt3/include/qptrvector.h
LogVoiceMail.o: /usr/lib/qt3/include/qgvector.h /usr/lib/qt3/include/qdict.h
LogVoiceMail.o: /usr/lib/qt3/include/qgdict.h
LogVoiceMail.o: /usr/lib/qt3/include/qdatetimeedit.h
LogVoiceMail.o: /usr/lib/qt3/include/qdatetime.h
LogVoiceMail.o: /usr/lib/qt3/include/qlayout.h
ProcessVoiceMail.o: /usr/include/stdio.h /usr/include/features.h
ProcessVoiceMail.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ProcessVoiceMail.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
ProcessVoiceMail.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
ProcessVoiceMail.o: /usr/include/bits/typesizes.h /usr/include/libio.h
ProcessVoiceMail.o: /usr/include/_G_config.h /usr/include/wchar.h
ProcessVoiceMail.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ProcessVoiceMail.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
ProcessVoiceMail.o: /usr/include/bits/stdio_lim.h
ProcessVoiceMail.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qdatetm.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qdatetime.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qstring.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qcstring.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qmemarray.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qgarray.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qshared.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qglobal.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
ProcessVoiceMail.o: /usr/include/limits.h
ProcessVoiceMail.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qnamespace.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qlayout.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qobject.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qobjectdefs.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qwindowdefs.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qevent.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qregion.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qpoint.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qdatastream.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qiodevice.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qpair.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qvaluelist.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qtl.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qtextstream.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qsizepolicy.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qwidget.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qpaintdevice.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qpalette.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qcolor.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qstringlist.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qstrlist.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qptrlist.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qglist.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qptrcollection.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qbrush.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qfont.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qfontmetrics.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qfontinfo.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qtimer.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qapplication.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qdesktopwidget.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qasciidict.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qgdict.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qtranslator.h ProcessVoiceMail.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qlabel.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qframe.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qcheckbox.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qbutton.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qkeysequence.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qpushbutton.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qiconset.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qpixmap.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qlistview.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qscrollview.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qscrollbar.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qrangecontrol.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qcombobox.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qmultilineedit.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qtextedit.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qstylesheet.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qptrvector.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qgvector.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qdict.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qtextview.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qpopupmenu.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qmenudata.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qsignal.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qvariant.h LogVoiceMail.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qmultilinedit.h
ProcessVoiceMail.o: /usr/lib/qt3/include/qdatetimeedit.h
