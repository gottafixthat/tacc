####### This section is automatically generated

USEDES = 1

MYSQLDIR = /usr/lib/mysql
INCDIRS	=  -I. -I../include -I$(QTDIR) -I../taccCore -I../lib -I../bdes
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include
#LFLAGS  += -lg++ -lstdc++
LFLAGS  += -lstdc++
LFLAGS  += -L/usr/local/lib -L../lib -L../taccCore -L../bdes -ltaa -lbdes
LFLAGS  += -L$(MYSQLDIR) -ltaccCore -lmysqlclient
LFLAGS	+= -L$(QTLIB) -lQtCore
#LFLAGS	+= -static


SHELL   = /bin/sh
CC	= g++
MOC	= /usr/local/bin/moc

ifdef USEDES
	CFLAGS += -DUSEDES -I/usr/local/include
	LFLAGS += -L/usr/local/lib -lbdes
endif


####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	ccget.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../bin/ccget

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) $(INCDIRS) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)
#	strip $(TARGET)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

## moc_brass.cpp: brass.h
##	$(MOC) brass.h -o moc_brass.cpp

# DO NOT DELETE THIS LINE -- make depend depends on it.
