####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)
CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. 
CFLAGS += -I../include
CFLAGS += -I../customers
CFLAGS += -I../qgtimespinbox
CFLAGS += -I../lib
CFLAGS += -I../taccCore
CFLAGS += -I../taccGui
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib -shared
LFLAGS += -L../lib -lblarg
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTMOC)

# CFLAGS += -DDBDEBUG
CFLAGS += -DQT3_SUPPORT

ifdef USEDES
	CFLAGS	+= -DUSEDES -I../bdes
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	CCTools.cpp CCBatch.cpp CCMonetra.cpp

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_CCTools.cpp moc_CCBatch.cpp moc_CCMonetra.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libccards.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% *.a #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_CCTools.cpp: CCTools.h
	$(MOC) CCTools.h -o moc_CCTools.cpp

moc_CCBatch.cpp: CCBatch.h
	$(MOC) CCBatch.h -o moc_CCBatch.cpp

moc_CCMonetra.cpp: CCMonetra.h
	$(MOC) CCMonetra.h -o moc_CCMonetra.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.
