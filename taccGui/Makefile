####### This section is automatically generated

USEDES	= 1

INCDIR	=  -I. -I../bdes -I../taccCore
# FIXME:  These will go away...
INCDIR  += -I../lib -I../include
INCDIR  += -I/usr/local/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W
LFLAGS  = -L. -L../bdes -L/usr/lib/mysql -lmysqlclient -lcrypt
# LFLAGS  += -lefence
#LFLAGS += -static
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTMOC)

CFLAGS += -I$(QTDIR)
LFLAGS += -L$(QTDIR)/lib -lQtCore -lQt3Support
TARGET	= libtaccGui

# Uncomment this to remove Qt support for date fields.
# CFLAGS += -DNOADBQT

ifdef USEDES
	CFLAGS	+= -DUSEDES
    LFLAGS  += -lbdes
endif
CFLAGS += -DQT3_SUPPORT

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	TAAWidget.cpp\
		   	PrintTreeWidget.cpp\
		   	EmailTreeWidget.cpp\
			TACCReport.cpp

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_TAAWidget.cpp\
			moc_PrintTreeWidget.cpp\
			moc_EmailTreeWidget.cpp\
			moc_TACCReport.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)


####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) $(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) db-lib

db-lib: $(OBJECTS)
	ar r $(TARGET).a $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET).a
#$(CC) -shared -o $(TARGET).so $(OBJECTS) $(OBJMETA)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET).a: $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET).a $(LFLAGS)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET).a

# FIXME
install: $(TARGET).a
	install -m 644 ADB.h /usr/local/include/
	install -m 644 libadb.a /usr/local/lib
	install -m 644 libadbqt.a /usr/local/lib

FORCE:

####### Meta objects

moc_TAAWidget.cpp: TAAWidget.h
	$(MOC) TAAWidget.h -o moc_TAAWidget.cpp

moc_PrintTreeWidget.cpp: PrintTreeWidget.h
	$(MOC) PrintTreeWidget.h -o moc_PrintTreeWidget.cpp

moc_EmailTreeWidget.cpp: EmailTreeWidget.h
	$(MOC) EmailTreeWidget.h -o moc_EmailTreeWidget.cpp

moc_TACCReport.cpp: TACCReport.h
	$(MOC) TACCReport.h -o moc_TACCReport.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

