####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR)/include -I. -I../include -I../lib -I../taccCore -I../bdes
INCDIRS += -I/home/marc/src/taa/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W
#LFLAGS  += -lg++ -lstdc++
#LFLAGS  += -lstdc++
LFLAGS  += -L../lib -L../taccCore -L../bdes -ltaa -ltaccCore
LFLAGS  += -L/usr/lib/mysql/ -lmysqlclient
LFLAGS	+= -L$(QTDIR)/lib -lQtCore -lQt3Support
#LFLAGS	+= -static


SHELL   = /bin/sh
CC	= g++
MOC	= /usr/local/bin/moc

ifdef USEDES
	CFLAGS += -DUSEDES -I/usr/local/include
	LFLAGS += -L/usr/local/lib -lbdes
endif
CFLAGS += -DQT3_SUPPORT


####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	wasync.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../bin/wasync

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
