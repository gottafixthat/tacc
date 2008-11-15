####### This section is automatically generated

USEDES	= 1
ADBQT = 1

INCDIR	=  -I. -I../bdes
INCDIR  += -I/usr/local/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W
LFLAGS  = -L. -L../bdes -L/usr/lib/mysql -lmysqlclient -lcrypt
# LFLAGS  += -lefence
#LFLAGS += -static
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

ifdef ADBQT
    CFLAGS += -DADBQT -I/usr/X11R6/include -I$(QTDIR)/include
    LFLAGS += -L$(QTDIR)/lib -lqt-mt
    TARGET	=	libadbqt
else
    TARGET	=	libadb
endif

# Uncomment this to remove Qt support for date fields.
# CFLAGS += -DNOADBQT

ifdef USEDES
	CFLAGS	+= -DUSEDES
    LFLAGS  += -lbdes
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	ADBColumn.cpp ADBRow.cpp ADB.cpp ADBTable.cpp ADBList.cpp
ifdef ADBQT
    SOURCES += ADBLogin.cpp
endif

OBJECTS =	$(SOURCES:.cpp=.o)

ifdef ADBQT
SRCMETA =	moc_ADB.cpp
endif

OBJMETA =	$(SRCMETA:.cpp=.o)


####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) db-lib adbtest

db-lib: $(OBJECTS)
	ar r $(TARGET).a $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET).a
#$(CC) -shared -o $(TARGET).so $(OBJECTS) $(OBJMETA)

adbtest: adbtest.cpp $(OBJECTS) $(OBJMETA)
	$(CC) -c $(CFLAGS) $(INCDIR) adbtest.cpp
	$(CC) $(OBJECTS) $(OBJMETA) adbtest.o -o adbtest $(LFLAGS)


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
	-rm -f adbtest

install: $(TARGET).a
	install -m 644 ADB.h /usr/local/include/
	install -m 644 libadb.a /usr/local/lib
	install -m 644 libadbqt.a /usr/local/lib

FORCE:

####### Meta objects

moc_ADB.cpp: ADB.h
	$(MOC) ADB.h -o moc_ADB.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

