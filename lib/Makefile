####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -O2 -Wall -fno-strength-reduce -I/usr/X11R6/include -I. -I../include/
#CFLAGS	+= -I/usr/include/c++/3.2.2/ext
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib -shared
SHELL   = /bin/sh
CC	= gcc
MOC	= $(QTDIR)/bin/moc

# CFLAGS += -DDBDEBUG

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
endif

####### End of automatically generated section

####### Files

SUBDIRS =	gl

SOURCES =	IClient.cpp BString.cpp TAAWidget.cpp\
			CustomerSearch.cpp\
			TAATools.cpp BlargDB.cpp\
			TAAConfig.cpp\
			DNSUtils.cpp\
			VHostUtils.cpp\
			GenLedger.cpp AcctsRecv.cpp BrassClient.cpp\
			AddressEditor.cpp AddressEditorData.cpp\
			PhoneEditor.cpp PhoneEditorData.cpp\
			ParseFile.cpp SelectionList.cpp\
			TAALogin.cpp buildnum.cpp\
			calendar.cpp holiday.cpp worktime.cpp\
			RListViewItem.cpp loadKey.cpp\
			DNSTemplatePicker.cpp\
			DNSManager.cpp

# HEADERS =	../include/BlargDB.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_TAAWidget.cpp\
			moc_CustomerSearch.cpp\
			moc_AddressEditor.cpp moc_AddressEditorData.cpp\
			moc_PhoneEditor.cpp moc_PhoneEditorData.cpp\
			moc_SelectionList.cpp\
			moc_TAALogin.cpp\
			moc_calendar.cpp\
			moc_RListViewItem.cpp\
			moc_DNSTemplatePicker.cpp\
			moc_DNSManager.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

# TARGET	=	qtacct

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) db-lib

db-lib: $(OBJECTS)
	ar r libblarg.a $(OBJECTS) $(OBJMETA)
	ranlib libblarg.a
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

moc_TAAWidget.cpp: TAAWidget.h
	$(MOC) TAAWidget.h -o moc_TAAWidget.cpp

moc_CustomerSearch.cpp: CustomerSearch.h
	$(MOC) CustomerSearch.h -o moc_CustomerSearch.cpp

moc_AddressEditor.cpp: AddressEditor.h
	$(MOC) AddressEditor.h -o moc_AddressEditor.cpp

moc_AddressEditorData.cpp: AddressEditorData.h
	$(MOC) AddressEditorData.h -o moc_AddressEditorData.cpp

moc_PhoneEditor.cpp: PhoneEditor.h
	$(MOC) PhoneEditor.h -o moc_PhoneEditor.cpp

moc_PhoneEditorData.cpp: PhoneEditorData.h
	$(MOC) PhoneEditorData.h -o moc_PhoneEditorData.cpp

moc_SelectionList.cpp: SelectionList.h
	$(MOC) SelectionList.h -o moc_SelectionList.cpp

moc_TAALogin.cpp: TAALogin.h
	$(MOC) TAALogin.h -o moc_TAALogin.cpp

moc_RListViewItem.cpp: RListViewItem.h
	$(MOC) RListViewItem.h -o moc_RListViewItem.cpp

moc_calendar.cpp: ../include/calendar.h
	$(MOC) ../include/calendar.h -o moc_calendar.cpp

moc_DNSTemplatePicker.cpp: DNSTemplatePicker.h
	$(MOC) DNSTemplatePicker.h -o moc_DNSTemplatePicker.cpp

moc_DNSManager.cpp: ../include/DNSManager.h
	$(MOC) ../include/DNSManager.h -o moc_DNSManager.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

IClient.o: /usr/include/string.h /usr/include/features.h
IClient.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
IClient.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
IClient.o: /usr/include/stdlib.h /usr/lib/qt3/include/qstring.h
IClient.o: /usr/lib/qt3/include/qcstring.h /usr/lib/qt3/include/qmemarray.h
IClient.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
IClient.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qwinexport.h
IClient.o: /usr/include/limits.h
IClient.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
BString.o: /usr/include/stdio.h /usr/include/features.h
BString.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BString.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BString.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BString.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BString.o: /usr/include/_G_config.h /usr/include/wchar.h
BString.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BString.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BString.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
BString.o: /usr/include/stdlib.h /usr/include/mysql/mysql.h
BString.o: /usr/include/sys/types.h /usr/include/time.h
BString.o: /usr/include/mysql/mysql_com.h /usr/include/mysql/mysql_version.h
BString.o: /usr/include/mysql/my_alloc.h /usr/lib/qt3/include/qstring.h
BString.o: /usr/lib/qt3/include/qcstring.h /usr/lib/qt3/include/qmemarray.h
BString.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
BString.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qwinexport.h
BString.o: /usr/include/string.h /usr/include/limits.h
BString.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
BString.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
BString.o: /usr/lib/qt3/include/qglist.h
BString.o: /usr/lib/qt3/include/qptrcollection.h
BString.o: /usr/lib/qt3/include/qdatastream.h
BString.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qregexp.h
BString.o: /usr/lib/qt3/include/qstringlist.h
BString.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
BString.o: /usr/lib/qt3/include/qtextstream.h /usr/lib/qt3/include/qdatetm.h
BString.o: /usr/lib/qt3/include/qdatetime.h /usr/lib/qt3/include/qnamespace.h
BString.o: /usr/include/ctype.h /usr/include/endian.h
BString.o: /usr/include/bits/endian.h /usr/include/sys/timeb.h
TAAWidget.o: /usr/lib/qt3/include/qapp.h /usr/lib/qt3/include/qapplication.h
TAAWidget.o: /usr/lib/qt3/include/qdesktopwidget.h
TAAWidget.o: /usr/lib/qt3/include/qwidget.h
TAAWidget.o: /usr/lib/qt3/include/qwindowdefs.h
TAAWidget.o: /usr/lib/qt3/include/qobjectdefs.h
TAAWidget.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qstring.h
TAAWidget.o: /usr/lib/qt3/include/qcstring.h /usr/lib/qt3/include/qmemarray.h
TAAWidget.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
TAAWidget.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
TAAWidget.o: /usr/include/features.h /usr/include/sys/cdefs.h
TAAWidget.o: /usr/include/gnu/stubs.h
TAAWidget.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TAAWidget.o: /usr/include/limits.h
TAAWidget.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
TAAWidget.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qobject.h
TAAWidget.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
TAAWidget.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
TAAWidget.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
TAAWidget.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
TAAWidget.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
TAAWidget.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
TAAWidget.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
TAAWidget.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TAAWidget.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TAAWidget.o: /usr/include/_G_config.h /usr/include/wchar.h
TAAWidget.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TAAWidget.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TAAWidget.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TAAWidget.o: /usr/lib/qt3/include/qpaintdevice.h
TAAWidget.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
TAAWidget.o: /usr/lib/qt3/include/qstringlist.h
TAAWidget.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
TAAWidget.o: /usr/lib/qt3/include/qglist.h
TAAWidget.o: /usr/lib/qt3/include/qptrcollection.h
TAAWidget.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
TAAWidget.o: /usr/lib/qt3/include/qfontmetrics.h
TAAWidget.o: /usr/lib/qt3/include/qfontinfo.h
TAAWidget.o: /usr/lib/qt3/include/qsizepolicy.h
TAAWidget.o: /usr/lib/qt3/include/qasciidict.h /usr/lib/qt3/include/qgdict.h
TAAWidget.o: /usr/lib/qt3/include/qtranslator.h TAAWidget.h
TAAWidget.o: /usr/lib/qt3/include/qmainwindow.h
TAAWidget.o: /usr/lib/qt3/include/qtoolbar.h
TAAWidget.o: /usr/lib/qt3/include/qdockwindow.h /usr/lib/qt3/include/qframe.h
TAAWidget.o: /usr/lib/qt3/include/qlabel.h
CustomerSearch.o: CustomerSearch.h TAAWidget.h /usr/lib/qt3/include/qapp.h
CustomerSearch.o: /usr/lib/qt3/include/qapplication.h
CustomerSearch.o: /usr/lib/qt3/include/qdesktopwidget.h
CustomerSearch.o: /usr/lib/qt3/include/qwidget.h
CustomerSearch.o: /usr/lib/qt3/include/qwindowdefs.h
CustomerSearch.o: /usr/lib/qt3/include/qobjectdefs.h
CustomerSearch.o: /usr/lib/qt3/include/qglobal.h
CustomerSearch.o: /usr/lib/qt3/include/qstring.h
CustomerSearch.o: /usr/lib/qt3/include/qcstring.h
CustomerSearch.o: /usr/lib/qt3/include/qmemarray.h
CustomerSearch.o: /usr/lib/qt3/include/qgarray.h
CustomerSearch.o: /usr/lib/qt3/include/qshared.h
CustomerSearch.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
CustomerSearch.o: /usr/include/features.h /usr/include/sys/cdefs.h
CustomerSearch.o: /usr/include/gnu/stubs.h
CustomerSearch.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
CustomerSearch.o: /usr/include/limits.h
CustomerSearch.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
CustomerSearch.o: /usr/lib/qt3/include/qnamespace.h
CustomerSearch.o: /usr/lib/qt3/include/qobject.h
CustomerSearch.o: /usr/lib/qt3/include/qevent.h
CustomerSearch.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qrect.h
CustomerSearch.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
CustomerSearch.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
CustomerSearch.o: /usr/lib/qt3/include/qdatastream.h
CustomerSearch.o: /usr/lib/qt3/include/qiodevice.h
CustomerSearch.o: /usr/lib/qt3/include/qpair.h
CustomerSearch.o: /usr/lib/qt3/include/qvaluelist.h
CustomerSearch.o: /usr/lib/qt3/include/qtl.h
CustomerSearch.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
CustomerSearch.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CustomerSearch.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CustomerSearch.o: /usr/include/_G_config.h /usr/include/wchar.h
CustomerSearch.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CustomerSearch.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
CustomerSearch.o: /usr/include/bits/stdio_lim.h
CustomerSearch.o: /usr/include/bits/sys_errlist.h
CustomerSearch.o: /usr/lib/qt3/include/qpaintdevice.h
CustomerSearch.o: /usr/lib/qt3/include/qpalette.h
CustomerSearch.o: /usr/lib/qt3/include/qcolor.h
CustomerSearch.o: /usr/lib/qt3/include/qstringlist.h
CustomerSearch.o: /usr/lib/qt3/include/qstrlist.h
CustomerSearch.o: /usr/lib/qt3/include/qptrlist.h
CustomerSearch.o: /usr/lib/qt3/include/qglist.h
CustomerSearch.o: /usr/lib/qt3/include/qptrcollection.h
CustomerSearch.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
CustomerSearch.o: /usr/lib/qt3/include/qfontmetrics.h
CustomerSearch.o: /usr/lib/qt3/include/qfontinfo.h
CustomerSearch.o: /usr/lib/qt3/include/qsizepolicy.h
CustomerSearch.o: /usr/lib/qt3/include/qasciidict.h
CustomerSearch.o: /usr/lib/qt3/include/qgdict.h
CustomerSearch.o: /usr/lib/qt3/include/qtranslator.h
CustomerSearch.o: /usr/lib/qt3/include/qmainwindow.h
CustomerSearch.o: /usr/lib/qt3/include/qtoolbar.h
CustomerSearch.o: /usr/lib/qt3/include/qdockwindow.h
CustomerSearch.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qlabel.h
CustomerSearch.o: /usr/lib/qt3/include/qlineedit.h
CustomerSearch.o: /usr/lib/qt3/include/qpushbutton.h
CustomerSearch.o: /usr/lib/qt3/include/qbutton.h
CustomerSearch.o: /usr/lib/qt3/include/qkeysequence.h
CustomerSearch.o: /usr/lib/qt3/include/qiconset.h
CustomerSearch.o: /usr/lib/qt3/include/qpixmap.h
CustomerSearch.o: /usr/lib/qt3/include/qlistview.h
CustomerSearch.o: /usr/lib/qt3/include/qscrollview.h
CustomerSearch.o: /usr/lib/qt3/include/qscrollbar.h
CustomerSearch.o: /usr/lib/qt3/include/qrangecontrol.h /usr/include/stdlib.h
CustomerSearch.o: /usr/lib/qt3/include/qlayout.h
TAATools.o: /usr/include/stdio.h /usr/include/features.h
TAATools.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TAATools.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TAATools.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TAATools.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TAATools.o: /usr/include/_G_config.h /usr/include/wchar.h
TAATools.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TAATools.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TAATools.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TAATools.o: /usr/include/stdlib.h /usr/include/time.h /usr/include/sys/time.h
TAATools.o: /usr/include/bits/time.h /usr/include/sys/select.h
TAATools.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
TAATools.o: /usr/lib/qt3/include/qapp.h /usr/lib/qt3/include/qapplication.h
TAATools.o: /usr/lib/qt3/include/qdesktopwidget.h
TAATools.o: /usr/lib/qt3/include/qwidget.h /usr/lib/qt3/include/qwindowdefs.h
TAATools.o: /usr/lib/qt3/include/qobjectdefs.h /usr/lib/qt3/include/qglobal.h
TAATools.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
TAATools.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
TAATools.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qwinexport.h
TAATools.o: /usr/include/string.h /usr/include/limits.h
TAATools.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
TAATools.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qobject.h
TAATools.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
TAATools.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
TAATools.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
TAATools.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
TAATools.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
TAATools.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
TAATools.o: /usr/lib/qt3/include/qtextstream.h
TAATools.o: /usr/lib/qt3/include/qpaintdevice.h
TAATools.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
TAATools.o: /usr/lib/qt3/include/qstringlist.h
TAATools.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
TAATools.o: /usr/lib/qt3/include/qglist.h
TAATools.o: /usr/lib/qt3/include/qptrcollection.h
TAATools.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
TAATools.o: /usr/lib/qt3/include/qfontmetrics.h
TAATools.o: /usr/lib/qt3/include/qfontinfo.h
TAATools.o: /usr/lib/qt3/include/qsizepolicy.h
TAATools.o: /usr/lib/qt3/include/qasciidict.h /usr/lib/qt3/include/qgdict.h
TAATools.o: /usr/lib/qt3/include/qtranslator.h /usr/lib/qt3/include/qdatetm.h
TAATools.o: /usr/lib/qt3/include/qdatetime.h
TAATools.o: /usr/lib/qt3/include/qmessagebox.h /usr/lib/qt3/include/qdialog.h
TAATools.o: TAATools.h
BlargDB.o: /usr/include/stdio.h /usr/include/features.h
BlargDB.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BlargDB.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BlargDB.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BlargDB.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BlargDB.o: /usr/include/_G_config.h /usr/include/wchar.h
BlargDB.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BlargDB.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BlargDB.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
BlargDB.o: /usr/include/stdlib.h /usr/include/ctype.h /usr/include/endian.h
BlargDB.o: /usr/include/bits/endian.h /usr/include/mysql/mysql.h
BlargDB.o: /usr/include/sys/types.h /usr/include/time.h
BlargDB.o: /usr/include/mysql/mysql_com.h /usr/include/mysql/mysql_version.h
BlargDB.o: /usr/include/mysql/my_alloc.h /usr/lib/qt3/include/qapplication.h
BlargDB.o: /usr/lib/qt3/include/qdesktopwidget.h
BlargDB.o: /usr/lib/qt3/include/qwidget.h /usr/lib/qt3/include/qwindowdefs.h
BlargDB.o: /usr/lib/qt3/include/qobjectdefs.h /usr/lib/qt3/include/qglobal.h
BlargDB.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
BlargDB.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
BlargDB.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qwinexport.h
BlargDB.o: /usr/include/string.h /usr/include/limits.h
BlargDB.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
BlargDB.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qobject.h
BlargDB.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
BlargDB.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
BlargDB.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
BlargDB.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
BlargDB.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
BlargDB.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
BlargDB.o: /usr/lib/qt3/include/qtextstream.h
BlargDB.o: /usr/lib/qt3/include/qpaintdevice.h
BlargDB.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
BlargDB.o: /usr/lib/qt3/include/qstringlist.h /usr/lib/qt3/include/qstrlist.h
BlargDB.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
BlargDB.o: /usr/lib/qt3/include/qptrcollection.h
BlargDB.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
BlargDB.o: /usr/lib/qt3/include/qfontmetrics.h
BlargDB.o: /usr/lib/qt3/include/qfontinfo.h
BlargDB.o: /usr/lib/qt3/include/qsizepolicy.h
BlargDB.o: /usr/lib/qt3/include/qasciidict.h /usr/lib/qt3/include/qgdict.h
BlargDB.o: /usr/lib/qt3/include/qtranslator.h /usr/lib/qt3/include/qregexp.h
BlargDB.o: /usr/lib/qt3/include/qdatetm.h /usr/lib/qt3/include/qdatetime.h
BlargDB.o: /usr/lib/qt3/include/qprinter.h /usr/lib/qt3/include/qpainter.h
BlargDB.o: /usr/lib/qt3/include/qpen.h /usr/lib/qt3/include/qpointarray.h
BlargDB.o: /usr/lib/qt3/include/qwmatrix.h /usr/include/sys/timeb.h BString.h
BlargDB.o: TAATools.h
TAAConfig.o: /usr/include/stdio.h /usr/include/features.h
TAAConfig.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TAAConfig.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TAAConfig.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TAAConfig.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TAAConfig.o: /usr/include/_G_config.h /usr/include/wchar.h
TAAConfig.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TAAConfig.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TAAConfig.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TAAConfig.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qstring.h
TAAConfig.o: /usr/lib/qt3/include/qcstring.h /usr/lib/qt3/include/qmemarray.h
TAAConfig.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
TAAConfig.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qwinexport.h
TAAConfig.o: /usr/include/string.h /usr/include/limits.h
TAAConfig.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
TAAConfig.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
TAAConfig.o: /usr/lib/qt3/include/qptrcollection.h
TAAConfig.o: /usr/lib/qt3/include/qdatastream.h
TAAConfig.o: /usr/lib/qt3/include/qiodevice.h BString.h
TAAConfig.o: /usr/lib/qt3/include/qdatetm.h /usr/lib/qt3/include/qdatetime.h
TAAConfig.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qdict.h
TAAConfig.o: /usr/lib/qt3/include/qgdict.h
DNSUtils.o: TAATools.h /usr/lib/qt3/include/qwidget.h
DNSUtils.o: /usr/lib/qt3/include/qwindowdefs.h
DNSUtils.o: /usr/lib/qt3/include/qobjectdefs.h /usr/lib/qt3/include/qglobal.h
DNSUtils.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
DNSUtils.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
DNSUtils.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qwinexport.h
DNSUtils.o: /usr/include/string.h /usr/include/features.h
DNSUtils.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
DNSUtils.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
DNSUtils.o: /usr/include/limits.h
DNSUtils.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
DNSUtils.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qobject.h
DNSUtils.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
DNSUtils.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
DNSUtils.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
DNSUtils.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
DNSUtils.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
DNSUtils.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
DNSUtils.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
DNSUtils.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
DNSUtils.o: /usr/include/bits/typesizes.h /usr/include/libio.h
DNSUtils.o: /usr/include/_G_config.h /usr/include/wchar.h
DNSUtils.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DNSUtils.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
DNSUtils.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
DNSUtils.o: /usr/lib/qt3/include/qpaintdevice.h
DNSUtils.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
DNSUtils.o: /usr/lib/qt3/include/qstringlist.h
DNSUtils.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
DNSUtils.o: /usr/lib/qt3/include/qglist.h
DNSUtils.o: /usr/lib/qt3/include/qptrcollection.h
DNSUtils.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
DNSUtils.o: /usr/lib/qt3/include/qfontmetrics.h
DNSUtils.o: /usr/lib/qt3/include/qfontinfo.h
DNSUtils.o: /usr/lib/qt3/include/qsizepolicy.h /usr/lib/qt3/include/qdatetm.h
DNSUtils.o: /usr/lib/qt3/include/qdatetime.h /usr/include/time.h
DNSUtils.o: /usr/lib/qt3/include/qregexp.h
VHostUtils.o: TAATools.h /usr/lib/qt3/include/qwidget.h
VHostUtils.o: /usr/lib/qt3/include/qwindowdefs.h
VHostUtils.o: /usr/lib/qt3/include/qobjectdefs.h
VHostUtils.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qstring.h
VHostUtils.o: /usr/lib/qt3/include/qcstring.h
VHostUtils.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
VHostUtils.o: /usr/lib/qt3/include/qshared.h
VHostUtils.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
VHostUtils.o: /usr/include/features.h /usr/include/sys/cdefs.h
VHostUtils.o: /usr/include/gnu/stubs.h
VHostUtils.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
VHostUtils.o: /usr/include/limits.h
VHostUtils.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
VHostUtils.o: /usr/lib/qt3/include/qnamespace.h
VHostUtils.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
VHostUtils.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qrect.h
VHostUtils.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
VHostUtils.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
VHostUtils.o: /usr/lib/qt3/include/qdatastream.h
VHostUtils.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
VHostUtils.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
VHostUtils.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
VHostUtils.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
VHostUtils.o: /usr/include/bits/typesizes.h /usr/include/libio.h
VHostUtils.o: /usr/include/_G_config.h /usr/include/wchar.h
VHostUtils.o: /usr/include/bits/wchar.h /usr/include/gconv.h
VHostUtils.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
VHostUtils.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
VHostUtils.o: /usr/lib/qt3/include/qpaintdevice.h
VHostUtils.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
VHostUtils.o: /usr/lib/qt3/include/qstringlist.h
VHostUtils.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
VHostUtils.o: /usr/lib/qt3/include/qglist.h
VHostUtils.o: /usr/lib/qt3/include/qptrcollection.h
VHostUtils.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
VHostUtils.o: /usr/lib/qt3/include/qfontmetrics.h
VHostUtils.o: /usr/lib/qt3/include/qfontinfo.h
VHostUtils.o: /usr/lib/qt3/include/qsizepolicy.h
VHostUtils.o: /usr/lib/qt3/include/qdatetm.h /usr/lib/qt3/include/qdatetime.h
VHostUtils.o: /usr/include/time.h
GenLedger.o: /usr/include/stdio.h /usr/include/features.h
GenLedger.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
GenLedger.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
GenLedger.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
GenLedger.o: /usr/include/bits/typesizes.h /usr/include/libio.h
GenLedger.o: /usr/include/_G_config.h /usr/include/wchar.h
GenLedger.o: /usr/include/bits/wchar.h /usr/include/gconv.h
GenLedger.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
GenLedger.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
GenLedger.o: /usr/include/stdlib.h /usr/lib/qt3/include/qstrlist.h
GenLedger.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
GenLedger.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
GenLedger.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qglobal.h
GenLedger.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
GenLedger.o: /usr/include/limits.h
GenLedger.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
GenLedger.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
GenLedger.o: /usr/lib/qt3/include/qptrcollection.h
GenLedger.o: /usr/lib/qt3/include/qdatastream.h
GenLedger.o: /usr/lib/qt3/include/qiodevice.h
AcctsRecv.o: /usr/include/stdio.h /usr/include/features.h
AcctsRecv.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
AcctsRecv.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
AcctsRecv.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
AcctsRecv.o: /usr/include/bits/typesizes.h /usr/include/libio.h
AcctsRecv.o: /usr/include/_G_config.h /usr/include/wchar.h
AcctsRecv.o: /usr/include/bits/wchar.h /usr/include/gconv.h
AcctsRecv.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
AcctsRecv.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
AcctsRecv.o: /usr/include/stdlib.h /usr/lib/qt3/include/qstrlist.h
AcctsRecv.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
AcctsRecv.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
AcctsRecv.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qglobal.h
AcctsRecv.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
AcctsRecv.o: /usr/include/limits.h
AcctsRecv.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
AcctsRecv.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
AcctsRecv.o: /usr/lib/qt3/include/qptrcollection.h
AcctsRecv.o: /usr/lib/qt3/include/qdatastream.h
AcctsRecv.o: /usr/lib/qt3/include/qiodevice.h
BrassClient.o: /usr/include/stdio.h /usr/include/features.h
BrassClient.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BrassClient.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BrassClient.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BrassClient.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BrassClient.o: /usr/include/_G_config.h /usr/include/wchar.h
BrassClient.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BrassClient.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BrassClient.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
BrassClient.o: /usr/include/stdlib.h /usr/include/netdb.h
BrassClient.o: /usr/include/netinet/in.h /usr/include/stdint.h
BrassClient.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
BrassClient.o: /usr/include/sys/types.h /usr/include/time.h
BrassClient.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
BrassClient.o: /usr/include/limits.h
BrassClient.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
BrassClient.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
BrassClient.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
BrassClient.o: /usr/include/endian.h /usr/include/bits/endian.h
BrassClient.o: /usr/include/bits/byteswap.h /usr/include/bits/netdb.h
BrassClient.o: /usr/include/string.h /usr/include/unistd.h
BrassClient.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
BrassClient.o: /usr/include/arpa/inet.h /usr/include/sys/time.h
BrassClient.o: /usr/include/bits/time.h /usr/include/sys/select.h
BrassClient.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
BrassClient.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
BrassClient.o: /usr/lib/qt3/include/qmemarray.h
BrassClient.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
BrassClient.o: /usr/lib/qt3/include/qglobal.h
BrassClient.o: /usr/lib/qt3/include/qwinexport.h /usr/include/crypt.h
AddressEditor.o: AddressEditor.h /usr/lib/qt3/include/qdialog.h
AddressEditor.o: /usr/lib/qt3/include/qwidget.h
AddressEditor.o: /usr/lib/qt3/include/qwindowdefs.h
AddressEditor.o: /usr/lib/qt3/include/qobjectdefs.h
AddressEditor.o: /usr/lib/qt3/include/qglobal.h
AddressEditor.o: /usr/lib/qt3/include/qstring.h
AddressEditor.o: /usr/lib/qt3/include/qcstring.h
AddressEditor.o: /usr/lib/qt3/include/qmemarray.h
AddressEditor.o: /usr/lib/qt3/include/qgarray.h
AddressEditor.o: /usr/lib/qt3/include/qshared.h
AddressEditor.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
AddressEditor.o: /usr/include/features.h /usr/include/sys/cdefs.h
AddressEditor.o: /usr/include/gnu/stubs.h
AddressEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
AddressEditor.o: /usr/include/limits.h
AddressEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
AddressEditor.o: /usr/lib/qt3/include/qnamespace.h
AddressEditor.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
AddressEditor.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qrect.h
AddressEditor.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
AddressEditor.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
AddressEditor.o: /usr/lib/qt3/include/qdatastream.h
AddressEditor.o: /usr/lib/qt3/include/qiodevice.h
AddressEditor.o: /usr/lib/qt3/include/qpair.h
AddressEditor.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
AddressEditor.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
AddressEditor.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
AddressEditor.o: /usr/include/bits/typesizes.h /usr/include/libio.h
AddressEditor.o: /usr/include/_G_config.h /usr/include/wchar.h
AddressEditor.o: /usr/include/bits/wchar.h /usr/include/gconv.h
AddressEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
AddressEditor.o: /usr/include/bits/stdio_lim.h
AddressEditor.o: /usr/include/bits/sys_errlist.h
AddressEditor.o: /usr/lib/qt3/include/qpaintdevice.h
AddressEditor.o: /usr/lib/qt3/include/qpalette.h
AddressEditor.o: /usr/lib/qt3/include/qcolor.h
AddressEditor.o: /usr/lib/qt3/include/qstringlist.h
AddressEditor.o: /usr/lib/qt3/include/qstrlist.h
AddressEditor.o: /usr/lib/qt3/include/qptrlist.h
AddressEditor.o: /usr/lib/qt3/include/qglist.h
AddressEditor.o: /usr/lib/qt3/include/qptrcollection.h
AddressEditor.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
AddressEditor.o: /usr/lib/qt3/include/qfontmetrics.h
AddressEditor.o: /usr/lib/qt3/include/qfontinfo.h
AddressEditor.o: /usr/lib/qt3/include/qsizepolicy.h
AddressEditor.o: /usr/lib/qt3/include/qlabel.h /usr/lib/qt3/include/qframe.h
AddressEditor.o: /usr/lib/qt3/include/qpushbutton.h
AddressEditor.o: /usr/lib/qt3/include/qbutton.h
AddressEditor.o: /usr/lib/qt3/include/qkeysequence.h
AddressEditor.o: /usr/lib/qt3/include/qiconset.h
AddressEditor.o: /usr/lib/qt3/include/qpixmap.h
AddressEditor.o: /usr/lib/qt3/include/qcheckbox.h
AddressEditor.o: /usr/lib/qt3/include/qcombobox.h
AddressEditor.o: /usr/lib/qt3/include/qlineedit.h TAAWidget.h
AddressEditor.o: /usr/lib/qt3/include/qapp.h
AddressEditor.o: /usr/lib/qt3/include/qapplication.h
AddressEditor.o: /usr/lib/qt3/include/qdesktopwidget.h
AddressEditor.o: /usr/lib/qt3/include/qasciidict.h
AddressEditor.o: /usr/lib/qt3/include/qgdict.h
AddressEditor.o: /usr/lib/qt3/include/qtranslator.h
AddressEditor.o: /usr/lib/qt3/include/qmainwindow.h
AddressEditor.o: /usr/lib/qt3/include/qtoolbar.h
AddressEditor.o: /usr/lib/qt3/include/qdockwindow.h BString.h
AddressEditor.o: /usr/lib/qt3/include/qdatetm.h
AddressEditor.o: /usr/lib/qt3/include/qdatetime.h TAATools.h
AddressEditor.o: /usr/include/time.h /usr/include/stdlib.h
AddressEditor.o: /usr/lib/qt3/include/qmessagebox.h
AddressEditor.o: /usr/lib/qt3/include/qlayout.h
AddressEditorData.o: /usr/lib/qt3/include/qpixmap.h
AddressEditorData.o: /usr/lib/qt3/include/qpaintdevice.h
AddressEditorData.o: /usr/lib/qt3/include/qwindowdefs.h
AddressEditorData.o: /usr/lib/qt3/include/qobjectdefs.h
AddressEditorData.o: /usr/lib/qt3/include/qglobal.h
AddressEditorData.o: /usr/lib/qt3/include/qstring.h
AddressEditorData.o: /usr/lib/qt3/include/qcstring.h
AddressEditorData.o: /usr/lib/qt3/include/qmemarray.h
AddressEditorData.o: /usr/lib/qt3/include/qgarray.h
AddressEditorData.o: /usr/lib/qt3/include/qshared.h
AddressEditorData.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
AddressEditorData.o: /usr/include/features.h /usr/include/sys/cdefs.h
AddressEditorData.o: /usr/include/gnu/stubs.h
AddressEditorData.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
AddressEditorData.o: /usr/include/limits.h
AddressEditorData.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
AddressEditorData.o: /usr/lib/qt3/include/qnamespace.h
AddressEditorData.o: /usr/lib/qt3/include/qrect.h
AddressEditorData.o: /usr/lib/qt3/include/qsize.h
AddressEditorData.o: /usr/lib/qt3/include/qpoint.h
AddressEditorData.o: /usr/lib/qt3/include/qcolor.h
AddressEditorData.o: /usr/lib/qt3/include/qstringlist.h
AddressEditorData.o: /usr/lib/qt3/include/qvaluelist.h
AddressEditorData.o: /usr/lib/qt3/include/qtl.h
AddressEditorData.o: /usr/lib/qt3/include/qtextstream.h
AddressEditorData.o: /usr/lib/qt3/include/qiodevice.h /usr/include/stdio.h
AddressEditorData.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
AddressEditorData.o: /usr/include/bits/typesizes.h /usr/include/libio.h
AddressEditorData.o: /usr/include/_G_config.h /usr/include/wchar.h
AddressEditorData.o: /usr/include/bits/wchar.h /usr/include/gconv.h
AddressEditorData.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
AddressEditorData.o: /usr/include/bits/stdio_lim.h
AddressEditorData.o: /usr/include/bits/sys_errlist.h
AddressEditorData.o: /usr/lib/qt3/include/qdatastream.h
AddressEditorData.o: /usr/lib/qt3/include/qstrlist.h
AddressEditorData.o: /usr/lib/qt3/include/qptrlist.h
AddressEditorData.o: /usr/lib/qt3/include/qglist.h
AddressEditorData.o: /usr/lib/qt3/include/qptrcollection.h
AddressEditorData.o: /usr/lib/qt3/include/qlayout.h
AddressEditorData.o: /usr/lib/qt3/include/qobject.h
AddressEditorData.o: /usr/lib/qt3/include/qevent.h
AddressEditorData.o: /usr/lib/qt3/include/qregion.h
AddressEditorData.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
AddressEditorData.o: /usr/lib/qt3/include/qpair.h
AddressEditorData.o: /usr/lib/qt3/include/qsizepolicy.h
AddressEditorData.o: /usr/lib/qt3/include/qwidget.h
AddressEditorData.o: /usr/lib/qt3/include/qpalette.h
AddressEditorData.o: /usr/lib/qt3/include/qbrush.h
AddressEditorData.o: /usr/lib/qt3/include/qfont.h
AddressEditorData.o: /usr/lib/qt3/include/qfontmetrics.h
AddressEditorData.o: /usr/lib/qt3/include/qfontinfo.h AddressEditorData.h
AddressEditorData.o: /usr/lib/qt3/include/qdialog.h
AddressEditorData.o: /usr/lib/qt3/include/qlabel.h
AddressEditorData.o: /usr/lib/qt3/include/qframe.h
AddressEditorData.o: /usr/lib/qt3/include/qpushbutton.h
AddressEditorData.o: /usr/lib/qt3/include/qbutton.h
AddressEditorData.o: /usr/lib/qt3/include/qkeysequence.h
AddressEditorData.o: /usr/lib/qt3/include/qiconset.h
AddressEditorData.o: /usr/lib/qt3/include/qcheckbox.h
AddressEditorData.o: /usr/lib/qt3/include/qcombobox.h
AddressEditorData.o: /usr/lib/qt3/include/qlineedit.h
AddressEditorData.o: /usr/lib/qt3/include/qvbuttongroup.h
AddressEditorData.o: /usr/lib/qt3/include/qbuttongroup.h
AddressEditorData.o: /usr/lib/qt3/include/qgroupbox.h
PhoneEditor.o: PhoneEditor.h PhoneEditorData.h /usr/lib/qt3/include/qdialog.h
PhoneEditor.o: /usr/lib/qt3/include/qwidget.h
PhoneEditor.o: /usr/lib/qt3/include/qwindowdefs.h
PhoneEditor.o: /usr/lib/qt3/include/qobjectdefs.h
PhoneEditor.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qstring.h
PhoneEditor.o: /usr/lib/qt3/include/qcstring.h
PhoneEditor.o: /usr/lib/qt3/include/qmemarray.h
PhoneEditor.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
PhoneEditor.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
PhoneEditor.o: /usr/include/features.h /usr/include/sys/cdefs.h
PhoneEditor.o: /usr/include/gnu/stubs.h
PhoneEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PhoneEditor.o: /usr/include/limits.h
PhoneEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
PhoneEditor.o: /usr/lib/qt3/include/qnamespace.h
PhoneEditor.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
PhoneEditor.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qrect.h
PhoneEditor.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
PhoneEditor.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
PhoneEditor.o: /usr/lib/qt3/include/qdatastream.h
PhoneEditor.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
PhoneEditor.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
PhoneEditor.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
PhoneEditor.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PhoneEditor.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PhoneEditor.o: /usr/include/_G_config.h /usr/include/wchar.h
PhoneEditor.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PhoneEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PhoneEditor.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
PhoneEditor.o: /usr/lib/qt3/include/qpaintdevice.h
PhoneEditor.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
PhoneEditor.o: /usr/lib/qt3/include/qstringlist.h
PhoneEditor.o: /usr/lib/qt3/include/qstrlist.h
PhoneEditor.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
PhoneEditor.o: /usr/lib/qt3/include/qptrcollection.h
PhoneEditor.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
PhoneEditor.o: /usr/lib/qt3/include/qfontmetrics.h
PhoneEditor.o: /usr/lib/qt3/include/qfontinfo.h
PhoneEditor.o: /usr/lib/qt3/include/qsizepolicy.h
PhoneEditor.o: /usr/lib/qt3/include/qlabel.h /usr/lib/qt3/include/qframe.h
PhoneEditor.o: /usr/lib/qt3/include/qpushbutton.h
PhoneEditor.o: /usr/lib/qt3/include/qbutton.h
PhoneEditor.o: /usr/lib/qt3/include/qkeysequence.h
PhoneEditor.o: /usr/lib/qt3/include/qiconset.h /usr/lib/qt3/include/qpixmap.h
PhoneEditor.o: /usr/lib/qt3/include/qcheckbox.h
PhoneEditor.o: /usr/lib/qt3/include/qcombobox.h
PhoneEditor.o: /usr/lib/qt3/include/qlineedit.h BString.h
PhoneEditor.o: /usr/lib/qt3/include/qdatetm.h
PhoneEditor.o: /usr/lib/qt3/include/qdatetime.h /usr/include/stdlib.h
PhoneEditor.o: /usr/lib/qt3/include/qmessagebox.h
PhoneEditorData.o: /usr/lib/qt3/include/qpixmap.h
PhoneEditorData.o: /usr/lib/qt3/include/qpaintdevice.h
PhoneEditorData.o: /usr/lib/qt3/include/qwindowdefs.h
PhoneEditorData.o: /usr/lib/qt3/include/qobjectdefs.h
PhoneEditorData.o: /usr/lib/qt3/include/qglobal.h
PhoneEditorData.o: /usr/lib/qt3/include/qstring.h
PhoneEditorData.o: /usr/lib/qt3/include/qcstring.h
PhoneEditorData.o: /usr/lib/qt3/include/qmemarray.h
PhoneEditorData.o: /usr/lib/qt3/include/qgarray.h
PhoneEditorData.o: /usr/lib/qt3/include/qshared.h
PhoneEditorData.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
PhoneEditorData.o: /usr/include/features.h /usr/include/sys/cdefs.h
PhoneEditorData.o: /usr/include/gnu/stubs.h
PhoneEditorData.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PhoneEditorData.o: /usr/include/limits.h
PhoneEditorData.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
PhoneEditorData.o: /usr/lib/qt3/include/qnamespace.h
PhoneEditorData.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
PhoneEditorData.o: /usr/lib/qt3/include/qpoint.h
PhoneEditorData.o: /usr/lib/qt3/include/qcolor.h
PhoneEditorData.o: /usr/lib/qt3/include/qstringlist.h
PhoneEditorData.o: /usr/lib/qt3/include/qvaluelist.h
PhoneEditorData.o: /usr/lib/qt3/include/qtl.h
PhoneEditorData.o: /usr/lib/qt3/include/qtextstream.h
PhoneEditorData.o: /usr/lib/qt3/include/qiodevice.h /usr/include/stdio.h
PhoneEditorData.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PhoneEditorData.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PhoneEditorData.o: /usr/include/_G_config.h /usr/include/wchar.h
PhoneEditorData.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PhoneEditorData.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PhoneEditorData.o: /usr/include/bits/stdio_lim.h
PhoneEditorData.o: /usr/include/bits/sys_errlist.h
PhoneEditorData.o: /usr/lib/qt3/include/qdatastream.h
PhoneEditorData.o: /usr/lib/qt3/include/qstrlist.h
PhoneEditorData.o: /usr/lib/qt3/include/qptrlist.h
PhoneEditorData.o: /usr/lib/qt3/include/qglist.h
PhoneEditorData.o: /usr/lib/qt3/include/qptrcollection.h
PhoneEditorData.o: /usr/lib/qt3/include/qlayout.h
PhoneEditorData.o: /usr/lib/qt3/include/qobject.h
PhoneEditorData.o: /usr/lib/qt3/include/qevent.h
PhoneEditorData.o: /usr/lib/qt3/include/qregion.h
PhoneEditorData.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
PhoneEditorData.o: /usr/lib/qt3/include/qpair.h
PhoneEditorData.o: /usr/lib/qt3/include/qsizepolicy.h
PhoneEditorData.o: /usr/lib/qt3/include/qwidget.h
PhoneEditorData.o: /usr/lib/qt3/include/qpalette.h
PhoneEditorData.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
PhoneEditorData.o: /usr/lib/qt3/include/qfontmetrics.h
PhoneEditorData.o: /usr/lib/qt3/include/qfontinfo.h PhoneEditorData.h
PhoneEditorData.o: /usr/lib/qt3/include/qdialog.h
PhoneEditorData.o: /usr/lib/qt3/include/qlabel.h
PhoneEditorData.o: /usr/lib/qt3/include/qframe.h
PhoneEditorData.o: /usr/lib/qt3/include/qpushbutton.h
PhoneEditorData.o: /usr/lib/qt3/include/qbutton.h
PhoneEditorData.o: /usr/lib/qt3/include/qkeysequence.h
PhoneEditorData.o: /usr/lib/qt3/include/qiconset.h
PhoneEditorData.o: /usr/lib/qt3/include/qcheckbox.h
PhoneEditorData.o: /usr/lib/qt3/include/qcombobox.h
PhoneEditorData.o: /usr/lib/qt3/include/qlineedit.h
PhoneEditorData.o: /usr/lib/qt3/include/qvbuttongroup.h
PhoneEditorData.o: /usr/lib/qt3/include/qbuttongroup.h
PhoneEditorData.o: /usr/lib/qt3/include/qgroupbox.h
ParseFile.o: /usr/include/stdio.h /usr/include/features.h
ParseFile.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ParseFile.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
ParseFile.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
ParseFile.o: /usr/include/bits/typesizes.h /usr/include/libio.h
ParseFile.o: /usr/include/_G_config.h /usr/include/wchar.h
ParseFile.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ParseFile.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
ParseFile.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
ParseFile.o: /usr/include/stdlib.h /usr/include/unistd.h
ParseFile.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
ParseFile.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qstring.h
ParseFile.o: /usr/lib/qt3/include/qcstring.h /usr/lib/qt3/include/qmemarray.h
ParseFile.o: /usr/lib/qt3/include/qgarray.h /usr/lib/qt3/include/qshared.h
ParseFile.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qwinexport.h
ParseFile.o: /usr/include/string.h /usr/include/limits.h
ParseFile.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
ParseFile.o: /usr/lib/qt3/include/qptrlist.h /usr/lib/qt3/include/qglist.h
ParseFile.o: /usr/lib/qt3/include/qptrcollection.h
ParseFile.o: /usr/lib/qt3/include/qdatastream.h
ParseFile.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qregexp.h
ParseFile.o: /usr/lib/qt3/include/qstringlist.h
ParseFile.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
ParseFile.o: /usr/lib/qt3/include/qtextstream.h
SelectionList.o: /usr/lib/qt3/include/qlayout.h
SelectionList.o: /usr/lib/qt3/include/qobject.h
SelectionList.o: /usr/lib/qt3/include/qobjectdefs.h
SelectionList.o: /usr/lib/qt3/include/qglobal.h
SelectionList.o: /usr/lib/qt3/include/qwindowdefs.h
SelectionList.o: /usr/lib/qt3/include/qstring.h
SelectionList.o: /usr/lib/qt3/include/qcstring.h
SelectionList.o: /usr/lib/qt3/include/qmemarray.h
SelectionList.o: /usr/lib/qt3/include/qgarray.h
SelectionList.o: /usr/lib/qt3/include/qshared.h
SelectionList.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
SelectionList.o: /usr/include/features.h /usr/include/sys/cdefs.h
SelectionList.o: /usr/include/gnu/stubs.h
SelectionList.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
SelectionList.o: /usr/include/limits.h
SelectionList.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
SelectionList.o: /usr/lib/qt3/include/qnamespace.h
SelectionList.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
SelectionList.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
SelectionList.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
SelectionList.o: /usr/lib/qt3/include/qmap.h
SelectionList.o: /usr/lib/qt3/include/qdatastream.h
SelectionList.o: /usr/lib/qt3/include/qiodevice.h
SelectionList.o: /usr/lib/qt3/include/qpair.h
SelectionList.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
SelectionList.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
SelectionList.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
SelectionList.o: /usr/include/bits/typesizes.h /usr/include/libio.h
SelectionList.o: /usr/include/_G_config.h /usr/include/wchar.h
SelectionList.o: /usr/include/bits/wchar.h /usr/include/gconv.h
SelectionList.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
SelectionList.o: /usr/include/bits/stdio_lim.h
SelectionList.o: /usr/include/bits/sys_errlist.h
SelectionList.o: /usr/lib/qt3/include/qsizepolicy.h
SelectionList.o: /usr/lib/qt3/include/qwidget.h
SelectionList.o: /usr/lib/qt3/include/qpaintdevice.h
SelectionList.o: /usr/lib/qt3/include/qpalette.h
SelectionList.o: /usr/lib/qt3/include/qcolor.h
SelectionList.o: /usr/lib/qt3/include/qstringlist.h
SelectionList.o: /usr/lib/qt3/include/qstrlist.h
SelectionList.o: /usr/lib/qt3/include/qptrlist.h
SelectionList.o: /usr/lib/qt3/include/qglist.h
SelectionList.o: /usr/lib/qt3/include/qptrcollection.h
SelectionList.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
SelectionList.o: /usr/lib/qt3/include/qfontmetrics.h
SelectionList.o: /usr/lib/qt3/include/qfontinfo.h SelectionList.h
SelectionList.o: /usr/lib/qt3/include/qdialog.h /usr/lib/qt3/include/qlabel.h
SelectionList.o: /usr/lib/qt3/include/qframe.h
SelectionList.o: /usr/lib/qt3/include/qlistview.h
SelectionList.o: /usr/lib/qt3/include/qscrollview.h
SelectionList.o: /usr/lib/qt3/include/qscrollbar.h
SelectionList.o: /usr/lib/qt3/include/qrangecontrol.h
SelectionList.o: /usr/lib/qt3/include/qpushbt.h
SelectionList.o: /usr/lib/qt3/include/qpushbutton.h
SelectionList.o: /usr/lib/qt3/include/qbutton.h
SelectionList.o: /usr/lib/qt3/include/qkeysequence.h
SelectionList.o: /usr/lib/qt3/include/qiconset.h
SelectionList.o: /usr/lib/qt3/include/qpixmap.h
SelectionList.o: /usr/lib/qt3/include/qcombobox.h
TAALogin.o: TAALogin.h /usr/lib/qt3/include/qdialog.h
TAALogin.o: /usr/lib/qt3/include/qwidget.h /usr/lib/qt3/include/qwindowdefs.h
TAALogin.o: /usr/lib/qt3/include/qobjectdefs.h /usr/lib/qt3/include/qglobal.h
TAALogin.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
TAALogin.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
TAALogin.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qwinexport.h
TAALogin.o: /usr/include/string.h /usr/include/features.h
TAALogin.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TAALogin.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TAALogin.o: /usr/include/limits.h
TAALogin.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
TAALogin.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qobject.h
TAALogin.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qregion.h
TAALogin.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
TAALogin.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
TAALogin.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
TAALogin.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
TAALogin.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
TAALogin.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
TAALogin.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TAALogin.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TAALogin.o: /usr/include/_G_config.h /usr/include/wchar.h
TAALogin.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TAALogin.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TAALogin.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TAALogin.o: /usr/lib/qt3/include/qpaintdevice.h
TAALogin.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
TAALogin.o: /usr/lib/qt3/include/qstringlist.h
TAALogin.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
TAALogin.o: /usr/lib/qt3/include/qglist.h
TAALogin.o: /usr/lib/qt3/include/qptrcollection.h
TAALogin.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
TAALogin.o: /usr/lib/qt3/include/qfontmetrics.h
TAALogin.o: /usr/lib/qt3/include/qfontinfo.h
TAALogin.o: /usr/lib/qt3/include/qsizepolicy.h /usr/lib/qt3/include/qlabel.h
TAALogin.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qlineedit.h
TAALogin.o: /usr/lib/qt3/include/qpushbutton.h /usr/lib/qt3/include/qbutton.h
TAALogin.o: /usr/lib/qt3/include/qkeysequence.h
TAALogin.o: /usr/lib/qt3/include/qiconset.h /usr/lib/qt3/include/qpixmap.h
TAALogin.o: /usr/lib/qt3/include/qcombobox.h /usr/lib/qt3/include/qtextview.h
TAALogin.o: /usr/lib/qt3/include/qtextedit.h
TAALogin.o: /usr/lib/qt3/include/qscrollview.h
TAALogin.o: /usr/lib/qt3/include/qscrollbar.h
TAALogin.o: /usr/lib/qt3/include/qrangecontrol.h
TAALogin.o: /usr/lib/qt3/include/qstylesheet.h
TAALogin.o: /usr/lib/qt3/include/qptrvector.h /usr/lib/qt3/include/qgvector.h
TAALogin.o: /usr/lib/qt3/include/qdict.h /usr/lib/qt3/include/qgdict.h
TAALogin.o: TAATools.h /usr/lib/qt3/include/qdatetm.h
TAALogin.o: /usr/lib/qt3/include/qdatetime.h /usr/include/time.h
TAALogin.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
TAALogin.o: /usr/include/bits/confname.h /usr/include/pwd.h
TAALogin.o: /usr/include/stdlib.h /usr/lib/qt3/include/qlayout.h
TAALogin.o: /usr/lib/qt3/include/qapplication.h
TAALogin.o: /usr/lib/qt3/include/qdesktopwidget.h
TAALogin.o: /usr/lib/qt3/include/qasciidict.h
TAALogin.o: /usr/lib/qt3/include/qtranslator.h /usr/include/mysql/mysql.h
TAALogin.o: /usr/include/sys/types.h /usr/include/mysql/mysql_com.h
TAALogin.o: /usr/include/mysql/mysql_version.h /usr/include/mysql/my_alloc.h
calendar.o: /usr/include/ctype.h /usr/include/features.h
calendar.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
calendar.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
calendar.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
calendar.o: /usr/include/bits/typesizes.h /usr/include/endian.h
calendar.o: /usr/include/bits/endian.h /usr/include/stdio.h
calendar.o: /usr/include/libio.h /usr/include/_G_config.h
calendar.o: /usr/include/wchar.h /usr/include/bits/wchar.h
calendar.o: /usr/include/gconv.h
calendar.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
calendar.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
calendar.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/time.h
calendar.o: /usr/lib/qt3/include/qevent.h /usr/lib/qt3/include/qwindowdefs.h
calendar.o: /usr/lib/qt3/include/qobjectdefs.h /usr/lib/qt3/include/qglobal.h
calendar.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
calendar.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
calendar.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qwinexport.h
calendar.o: /usr/include/limits.h
calendar.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
calendar.o: /usr/lib/qt3/include/qnamespace.h /usr/lib/qt3/include/qregion.h
calendar.o: /usr/lib/qt3/include/qrect.h /usr/lib/qt3/include/qsize.h
calendar.o: /usr/lib/qt3/include/qpoint.h /usr/lib/qt3/include/qmime.h
calendar.o: /usr/lib/qt3/include/qmap.h /usr/lib/qt3/include/qdatastream.h
calendar.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
calendar.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
calendar.o: /usr/lib/qt3/include/qtextstream.h /usr/lib/qt3/include/qscrbar.h
calendar.o: /usr/lib/qt3/include/qscrollbar.h /usr/lib/qt3/include/qwidget.h
calendar.o: /usr/lib/qt3/include/qobject.h
calendar.o: /usr/lib/qt3/include/qpaintdevice.h
calendar.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
calendar.o: /usr/lib/qt3/include/qstringlist.h
calendar.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
calendar.o: /usr/lib/qt3/include/qglist.h
calendar.o: /usr/lib/qt3/include/qptrcollection.h
calendar.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
calendar.o: /usr/lib/qt3/include/qfontmetrics.h
calendar.o: /usr/lib/qt3/include/qfontinfo.h
calendar.o: /usr/lib/qt3/include/qsizepolicy.h
calendar.o: /usr/lib/qt3/include/qrangecontrol.h
calendar.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qtimer.h
calendar.o: /usr/lib/qt3/include/qdatetm.h /usr/lib/qt3/include/qdatetime.h
calendar.o: /usr/lib/qt3/include/qpainter.h /usr/lib/qt3/include/qpen.h
calendar.o: /usr/lib/qt3/include/qpointarray.h
calendar.o: /usr/lib/qt3/include/qwmatrix.h /usr/lib/qt3/include/qdrawutil.h
calendar.o: /usr/lib/qt3/include/qstyle.h
holiday.o: /usr/include/stdio.h /usr/include/features.h
holiday.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
holiday.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
holiday.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
holiday.o: /usr/include/bits/typesizes.h /usr/include/libio.h
holiday.o: /usr/include/_G_config.h /usr/include/wchar.h
holiday.o: /usr/include/bits/wchar.h /usr/include/gconv.h
holiday.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
holiday.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
holiday.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/time.h
worktime.o: /usr/include/stdio.h /usr/include/features.h
worktime.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
worktime.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
worktime.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
worktime.o: /usr/include/bits/typesizes.h /usr/include/libio.h
worktime.o: /usr/include/_G_config.h /usr/include/wchar.h
worktime.o: /usr/include/bits/wchar.h /usr/include/gconv.h
worktime.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
worktime.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
worktime.o: /usr/include/time.h
RListViewItem.o: /usr/lib/qt3/include/qapplication.h
RListViewItem.o: /usr/lib/qt3/include/qdesktopwidget.h
RListViewItem.o: /usr/lib/qt3/include/qwidget.h
RListViewItem.o: /usr/lib/qt3/include/qwindowdefs.h
RListViewItem.o: /usr/lib/qt3/include/qobjectdefs.h
RListViewItem.o: /usr/lib/qt3/include/qglobal.h
RListViewItem.o: /usr/lib/qt3/include/qstring.h
RListViewItem.o: /usr/lib/qt3/include/qcstring.h
RListViewItem.o: /usr/lib/qt3/include/qmemarray.h
RListViewItem.o: /usr/lib/qt3/include/qgarray.h
RListViewItem.o: /usr/lib/qt3/include/qshared.h
RListViewItem.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
RListViewItem.o: /usr/include/features.h /usr/include/sys/cdefs.h
RListViewItem.o: /usr/include/gnu/stubs.h
RListViewItem.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
RListViewItem.o: /usr/include/limits.h
RListViewItem.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
RListViewItem.o: /usr/lib/qt3/include/qnamespace.h
RListViewItem.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
RListViewItem.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qrect.h
RListViewItem.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
RListViewItem.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
RListViewItem.o: /usr/lib/qt3/include/qdatastream.h
RListViewItem.o: /usr/lib/qt3/include/qiodevice.h
RListViewItem.o: /usr/lib/qt3/include/qpair.h
RListViewItem.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
RListViewItem.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
RListViewItem.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
RListViewItem.o: /usr/include/bits/typesizes.h /usr/include/libio.h
RListViewItem.o: /usr/include/_G_config.h /usr/include/wchar.h
RListViewItem.o: /usr/include/bits/wchar.h /usr/include/gconv.h
RListViewItem.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
RListViewItem.o: /usr/include/bits/stdio_lim.h
RListViewItem.o: /usr/include/bits/sys_errlist.h
RListViewItem.o: /usr/lib/qt3/include/qpaintdevice.h
RListViewItem.o: /usr/lib/qt3/include/qpalette.h
RListViewItem.o: /usr/lib/qt3/include/qcolor.h
RListViewItem.o: /usr/lib/qt3/include/qstringlist.h
RListViewItem.o: /usr/lib/qt3/include/qstrlist.h
RListViewItem.o: /usr/lib/qt3/include/qptrlist.h
RListViewItem.o: /usr/lib/qt3/include/qglist.h
RListViewItem.o: /usr/lib/qt3/include/qptrcollection.h
RListViewItem.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
RListViewItem.o: /usr/lib/qt3/include/qfontmetrics.h
RListViewItem.o: /usr/lib/qt3/include/qfontinfo.h
RListViewItem.o: /usr/lib/qt3/include/qsizepolicy.h
RListViewItem.o: /usr/lib/qt3/include/qasciidict.h
RListViewItem.o: /usr/lib/qt3/include/qgdict.h
RListViewItem.o: /usr/lib/qt3/include/qtranslator.h
RListViewItem.o: /usr/lib/qt3/include/qpainter.h /usr/lib/qt3/include/qpen.h
RListViewItem.o: /usr/lib/qt3/include/qpointarray.h
RListViewItem.o: /usr/lib/qt3/include/qwmatrix.h RListViewItem.h
RListViewItem.o: /usr/lib/qt3/include/qlistview.h
RListViewItem.o: /usr/lib/qt3/include/qscrollview.h
RListViewItem.o: /usr/lib/qt3/include/qframe.h
RListViewItem.o: /usr/lib/qt3/include/qscrollbar.h
RListViewItem.o: /usr/lib/qt3/include/qrangecontrol.h
loadKey.o: /usr/include/stdio.h /usr/include/features.h
loadKey.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
loadKey.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
loadKey.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
loadKey.o: /usr/include/bits/typesizes.h /usr/include/libio.h
loadKey.o: /usr/include/_G_config.h /usr/include/wchar.h
loadKey.o: /usr/include/bits/wchar.h /usr/include/gconv.h
loadKey.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
loadKey.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
loadKey.o: /usr/include/string.h /usr/include/stdlib.h
loadKey.o: /usr/lib/qt3/include/qdatetm.h /usr/lib/qt3/include/qdatetime.h
loadKey.o: /usr/lib/qt3/include/qstring.h /usr/lib/qt3/include/qcstring.h
loadKey.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
loadKey.o: /usr/lib/qt3/include/qshared.h /usr/lib/qt3/include/qglobal.h
loadKey.o: /usr/lib/qt3/include/qwinexport.h /usr/include/limits.h
loadKey.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
loadKey.o: /usr/lib/qt3/include/qnamespace.h
DNSTemplatePicker.o: DNSTemplatePicker.h /usr/lib/qt3/include/qwidget.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qwindowdefs.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qobjectdefs.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qglobal.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qstring.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qcstring.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qmemarray.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qgarray.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qshared.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
DNSTemplatePicker.o: /usr/include/features.h /usr/include/sys/cdefs.h
DNSTemplatePicker.o: /usr/include/gnu/stubs.h
DNSTemplatePicker.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
DNSTemplatePicker.o: /usr/include/limits.h
DNSTemplatePicker.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qnamespace.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qobject.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qevent.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qregion.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qrect.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qsize.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qpoint.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qdatastream.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qiodevice.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qpair.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qvaluelist.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qtl.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
DNSTemplatePicker.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
DNSTemplatePicker.o: /usr/include/bits/typesizes.h /usr/include/libio.h
DNSTemplatePicker.o: /usr/include/_G_config.h /usr/include/wchar.h
DNSTemplatePicker.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DNSTemplatePicker.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
DNSTemplatePicker.o: /usr/include/bits/stdio_lim.h
DNSTemplatePicker.o: /usr/include/bits/sys_errlist.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qpaintdevice.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qpalette.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qcolor.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qstringlist.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qstrlist.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qptrlist.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qglist.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qptrcollection.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qbrush.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qfont.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qfontmetrics.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qfontinfo.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qsizepolicy.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qdialog.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qlistview.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qscrollview.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qframe.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qscrollbar.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qrangecontrol.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qpushbutton.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qbutton.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qkeysequence.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qiconset.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qpixmap.h /usr/include/stdlib.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qlabel.h
DNSTemplatePicker.o: /usr/lib/qt3/include/qlayout.h
DNSManager.o: TAATools.h /usr/lib/qt3/include/qwidget.h
DNSManager.o: /usr/lib/qt3/include/qwindowdefs.h
DNSManager.o: /usr/lib/qt3/include/qobjectdefs.h
DNSManager.o: /usr/lib/qt3/include/qglobal.h /usr/lib/qt3/include/qstring.h
DNSManager.o: /usr/lib/qt3/include/qcstring.h
DNSManager.o: /usr/lib/qt3/include/qmemarray.h /usr/lib/qt3/include/qgarray.h
DNSManager.o: /usr/lib/qt3/include/qshared.h
DNSManager.o: /usr/lib/qt3/include/qwinexport.h /usr/include/string.h
DNSManager.o: /usr/include/features.h /usr/include/sys/cdefs.h
DNSManager.o: /usr/include/gnu/stubs.h
DNSManager.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
DNSManager.o: /usr/include/limits.h
DNSManager.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/limits.h
DNSManager.o: /usr/lib/qt3/include/qnamespace.h
DNSManager.o: /usr/lib/qt3/include/qobject.h /usr/lib/qt3/include/qevent.h
DNSManager.o: /usr/lib/qt3/include/qregion.h /usr/lib/qt3/include/qrect.h
DNSManager.o: /usr/lib/qt3/include/qsize.h /usr/lib/qt3/include/qpoint.h
DNSManager.o: /usr/lib/qt3/include/qmime.h /usr/lib/qt3/include/qmap.h
DNSManager.o: /usr/lib/qt3/include/qdatastream.h
DNSManager.o: /usr/lib/qt3/include/qiodevice.h /usr/lib/qt3/include/qpair.h
DNSManager.o: /usr/lib/qt3/include/qvaluelist.h /usr/lib/qt3/include/qtl.h
DNSManager.o: /usr/lib/qt3/include/qtextstream.h /usr/include/stdio.h
DNSManager.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
DNSManager.o: /usr/include/bits/typesizes.h /usr/include/libio.h
DNSManager.o: /usr/include/_G_config.h /usr/include/wchar.h
DNSManager.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DNSManager.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
DNSManager.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
DNSManager.o: /usr/lib/qt3/include/qpaintdevice.h
DNSManager.o: /usr/lib/qt3/include/qpalette.h /usr/lib/qt3/include/qcolor.h
DNSManager.o: /usr/lib/qt3/include/qstringlist.h
DNSManager.o: /usr/lib/qt3/include/qstrlist.h /usr/lib/qt3/include/qptrlist.h
DNSManager.o: /usr/lib/qt3/include/qglist.h
DNSManager.o: /usr/lib/qt3/include/qptrcollection.h
DNSManager.o: /usr/lib/qt3/include/qbrush.h /usr/lib/qt3/include/qfont.h
DNSManager.o: /usr/lib/qt3/include/qfontmetrics.h
DNSManager.o: /usr/lib/qt3/include/qfontinfo.h
DNSManager.o: /usr/lib/qt3/include/qsizepolicy.h
DNSManager.o: /usr/lib/qt3/include/qdatetm.h /usr/lib/qt3/include/qdatetime.h
DNSManager.o: /usr/include/time.h TAAWidget.h /usr/lib/qt3/include/qapp.h
DNSManager.o: /usr/lib/qt3/include/qapplication.h
DNSManager.o: /usr/lib/qt3/include/qdesktopwidget.h
DNSManager.o: /usr/lib/qt3/include/qasciidict.h /usr/lib/qt3/include/qgdict.h
DNSManager.o: /usr/lib/qt3/include/qtranslator.h
DNSManager.o: /usr/lib/qt3/include/qmainwindow.h
DNSManager.o: /usr/lib/qt3/include/qtoolbar.h
DNSManager.o: /usr/lib/qt3/include/qdockwindow.h
DNSManager.o: /usr/lib/qt3/include/qframe.h /usr/lib/qt3/include/qlabel.h
DNSManager.o: /usr/lib/qt3/include/qlayout.h
DNSManager.o: /usr/lib/qt3/include/qpushbutton.h
DNSManager.o: /usr/lib/qt3/include/qbutton.h
DNSManager.o: /usr/lib/qt3/include/qkeysequence.h
DNSManager.o: /usr/lib/qt3/include/qiconset.h /usr/lib/qt3/include/qpixmap.h
DNSManager.o: /usr/lib/qt3/include/qmessagebox.h
DNSManager.o: /usr/lib/qt3/include/qdialog.h /usr/lib/qt3/include/qregexp.h
