####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR)/include -I. -I../include -I../lib -I../adb -I../bdes
INCDIRS += -I/usr/local/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -DADBQT
#LFLAGS  += -lg++ -lstdc++
#LFLAGS  += -lstdc++
LFLAGS  += -L../lib -L../adb -L../bdes -ltaa
LFLAGS  += -L/usr/X11R6/lib -lXext
LFLAGS  += -L/usr/local/lib -ladbqt
LFLAGS  += -lpcre
LFLAGS  += -L/usr/lib/mysql -lmysqlclient
LFLAGS	+= -L$(QTDIR)/lib -lqt-mt
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

SOURCES =	cccimport.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../bin/cccimport

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
