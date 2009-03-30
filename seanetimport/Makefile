####### This section is automatically generated

INCDIRS	=  -I. -I/usr/local/include -I../include -I../lib -I$(QTDIR)/include -I../adb -I../bdes
CFLAGS= -O2 -fno-strength-reduce -Wall
LFLAGS  = -L/usr/local/lib -L/usr/lib/mysql -L../lib -L../adb -L../bdes -L$(QTDIR)/lib -L/usr/X11R6/lib/
LFLAGS += -ltaa -ladbqt -lqt-mt -lmysqlclient -lpcre
#LFLAGS += -lXext -lX11 -lSM -ldl
#LFLAGS += -static
SHELL   = /bin/sh
CC	= g++

USEDES	= 1

ifdef USEDES
	CFLAGS	+= -DUSEDES
	LFLAGS	+= -lbdes
endif

# LFLAGS += -static

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	seanetimport.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

TARGET	=	seanetimport

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) $(INCDIRS) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(TARGET)

FORCE:


# DO NOT DELETE THIS LINE -- make depend depends on it.
