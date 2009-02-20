####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -O2 -Wall -fno-strength-reduce -I/usr/X11R6/include -I. -I../include/ -I../adb -I../bdes
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

SOURCES =	StrTools.cpp Cfg.cpp CCValidate.cpp FParse.cpp\
			IClient.cpp BString.cpp TAAWidget.cpp\
			QSqlDbPool.cpp\
			StatementEngine.cpp\
			CustomerContactsDB.cpp\
			CSVParser.cpp\
			CustomerSearch.cpp\
			TAATools.cpp BlargDB.cpp\
			TAAConfig.cpp\
			TAAPixmaps.cpp\
			DNSUtils.cpp\
			VHostUtils.cpp\
			GenLedger.cpp AcctsRecv.cpp BrassClient.cpp\
			AddressEditor.cpp AddressEditorData.cpp\
			ParseFile.cpp SelectionList.cpp\
			TAALogin.cpp buildnum.cpp\
			calendar.cpp holiday.cpp worktime.cpp\
			RListViewItem.cpp\
			ColorListViewItem.cpp\
			loadKey.cpp\
			DNSTemplatePicker.cpp\
			DNSManager.cpp\
			ServerGroups.cpp\
			CollReport.cpp\
			wtpl2.cpp

# HEADERS =	../include/BlargDB.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_TAAWidget.cpp\
			moc_QSqlDbPool.cpp\
			moc_CustomerSearch.cpp\
			moc_AddressEditor.cpp moc_AddressEditorData.cpp\
			moc_SelectionList.cpp\
			moc_TAALogin.cpp\
			moc_calendar.cpp\
			moc_RListViewItem.cpp\
			moc_ColorListViewItem.cpp\
			moc_DNSTemplatePicker.cpp\
			moc_DNSManager.cpp\
			moc_ServerGroups.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

# TARGET	=	qtacct

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) db-lib

db-lib: $(OBJECTS)
	ar r libtaa.a $(OBJECTS) $(OBJMETA)
	ranlib libtaa.a
	# mv libblarg.a lib

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

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

moc_QSqlDbPool.cpp: QSqlDbPool.h
	$(MOC) QSqlDbPool.h -o moc_QSqlDbPool.cpp

moc_AddressEditor.cpp: AddressEditor.h
	$(MOC) AddressEditor.h -o moc_AddressEditor.cpp

moc_AddressEditorData.cpp: AddressEditorData.h
	$(MOC) AddressEditorData.h -o moc_AddressEditorData.cpp

moc_SelectionList.cpp: SelectionList.h
	$(MOC) SelectionList.h -o moc_SelectionList.cpp

moc_TAALogin.cpp: TAALogin.h
	$(MOC) TAALogin.h -o moc_TAALogin.cpp

moc_RListViewItem.cpp: RListViewItem.h
	$(MOC) RListViewItem.h -o moc_RListViewItem.cpp

moc_ColorListViewItem.cpp: ColorListViewItem.h
	$(MOC) ColorListViewItem.h -o moc_ColorListViewItem.cpp

moc_calendar.cpp: ../include/calendar.h
	$(MOC) ../include/calendar.h -o moc_calendar.cpp

moc_DNSTemplatePicker.cpp: DNSTemplatePicker.h
	$(MOC) DNSTemplatePicker.h -o moc_DNSTemplatePicker.cpp

moc_DNSManager.cpp: ../include/DNSManager.h
	$(MOC) ../include/DNSManager.h -o moc_DNSManager.cpp

moc_ServerGroups.cpp: ServerGroups.h
	$(MOC) ServerGroups.h -o moc_ServerGroups.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

