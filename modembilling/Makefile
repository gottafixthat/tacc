####### This section is automatically generated

INCDIRS	=  -I. -I/usr/local/include -I../include -I$(QTDIR)/include
CFLAGS= -O2 -fno-strength-reduce -Wall
LFLAGS  = -L/usr/local/lib -L/usr/lib/mysql -L../lib -L$(QTDIR)/lib -L/usr/X11R6/lib/
LFLAGS += -lblarg -ladb -lqt-mt -lmysqlclient -lBTools -lXext -lX11 -lSM -ldl -lICE
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

SOURCES =	modembilling.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

TARGET	=	../bin/modembilling

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
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
