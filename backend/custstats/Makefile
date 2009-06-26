####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR) -I. -I../include -I../../lib -I../../taccCore -I../../bdes
INCDIRS += -I/home/marc/src/taa/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include
#LFLAGS  += -lg++ -lstdc++
#LFLAGS  += -lstdc++
LFLAGS  += -L/usr/X11R6/lib -lXext
LFLAGS  += -L../../lib -L../../taccCore -L../../bdes -ltaccCore
LFLAGS  += -L/usr/lib/mysql/ -lmysqlclient
LFLAGS  += -ltaa -lbdes
LFLAGS	+= -L$(QTDIR)/lib -lQtCore -lQt3Support
#LFLAGS	+= -static


SHELL   = /bin/sh
CC	= g++
MOC	= $(QTMOC)

ifdef USEDES
	CFLAGS += -DUSEDES -I/usr/local/include
	LFLAGS += -L/usr/local/lib -lbdes
endif
CFLAGS += -DQT3_SUPPORT


####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	custstats.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../../bin/custstats

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
	strip $(TARGET)

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
