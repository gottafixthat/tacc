####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR) -I. -I../include -I../lib -I../taccCore
INCDIRS += -I/usr/local/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -DADBQT
#LFLAGS  += -lg++ -lstdc++
#LFLAGS  += -lstdc++
#LFLAGS  += -L../lib -ltaa
LFLAGS  += -L/usr/local/lib -L../taccCore -ltaccCore
LFLAGS  += -L/usr/lib/mysql -lmysqlclient
LFLAGS  += -L../bdes
LFLAGS  += -lmimepp_net -lmimepp_core -lmimepp_util
LFLAGS	+= -L$(QTLIB) -lQtCore
#LFLAGS	+= -static

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
	LFLAGS	+= -L/usr/local/lib -lbdes
endif


####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	mailwrap.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../bin/taamail

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
