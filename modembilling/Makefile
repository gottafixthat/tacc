####### This section is automatically generated

INCDIRS	=  -I. -I/usr/local/include -I../include -I../lib -I$(QTDIR)/include -I../taccCore -I../bdes
CFLAGS= -O2 -fno-strength-reduce -Wall
LFLAGS  = -L/usr/local/lib -L/usr/lib/mysql -L../lib -L../adb -L../bdes -L$(QTDIR)/lib -L../taccCore
LFLAGS += -ltaa -ltaccCore -lQtCore -lQtSql -lQt3Support -lmysqlclient -lpcre 
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
