####### This section is automatically generated

USEDES = 1

MYSQLDIR = /usr/lib/mysql
INCDIRS	=  -I. -I../include
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include
#LFLAGS  += -lg++ -lstdc++
LFLAGS  += -lstdc++
LFLAGS  += -L/usr/local/lib -lBTools
#LFLAGS	+= -L$(QTDIR)/lib -lqt -L/usr/X11R6/lib -lX11 -lXext -lqutil
LFLAGS  += -L$(MYSQLDIR) -ladb -lmysqlclient -lz
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
