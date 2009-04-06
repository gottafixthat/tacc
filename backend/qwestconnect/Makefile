
USEDES = 1

INCDIRS	=  -I. -I../../include -I../../taccCore -I../../bdes -I../../tickets -I$(QTDIR)/include -I../../lib -I/usr/local/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W

LFLAGS  = -L../../taccCore -L../../bdes -L../../lib -L../../tickets -L$(QTDIR)/lib
LFLAGS	+= -L/usr/local/lib -ltickets -ltaa -ltaccCore
LFLAGS  += -L/usr/lib/mysql -lmysqlclient -lQtCore -lQt3Support
LFLAGS  += -lmimepp_core -lmimepp_util -lmimepp_net
LFLAGS  += -lpcre

SHELL   = /bin/sh
CC	= g++

ifdef USEDES
	CFLAGS += -DUSEDES
	LFLAGS += -lbdes
endif
CFLAGS += -DQT3_SUPPORT

# LFLAGS += -static
CFLAGS	+= $(DEMO)


####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	qwestconnect.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

TARGET	=	../../bin/qwestconnect

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
	# strip $(TARGET)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:


# DO NOT DELETE THIS LINE -- make depend depends on it.
