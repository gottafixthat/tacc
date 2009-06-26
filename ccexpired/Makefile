####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR) -I. -I../include -I../lib -I../taccCore -I../bdes
INCDIRS += -I/usr/local/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W
#LFLAGS  += -lg++ -lstdc++
#LFLAGS  += -lstdc++
LFLAGS  += -L/usr/local/lib -L../lib -L../taccCore -L../bdes -ltaccCore -ltaa
LFLAGS  += -L/usr/lib/mysql -lmysqlclient
LFLAGS	+= -L$(QTLIB) -lQtCore
#LFLAGS	+= -static


SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

ifdef USEDES
	CFLAGS += -DUSEDES -I/usr/local/include
	LFLAGS += -L/usr/local/lib -lbdes
endif


####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	ccexpired.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../bin/ccexpired

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCDIRS) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)
	# strip $(TARGET)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

# DO NOT DELETE THIS LINE -- make depend depends on it.
