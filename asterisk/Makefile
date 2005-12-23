####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -O2 -fno-strength-reduce -I/usr/X11R6/include -I. -I../include/
CFLAGS	+= -I../lib -DADBQT
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

# CFLAGS += -DDBDEBUG

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	AsteriskManager.cpp\
			QueueMonitor.cpp\
			AgentStatus.cpp

# HEADERS =	../include/BlargDB.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_AsteriskManager.cpp\
			moc_QueueMonitor.cpp\
			moc_AgentStatus.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libasterisk.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) db-lib

db-lib: $(OBJECTS)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)
	# mv libblarg.a lib

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_AsteriskManager.cpp: AsteriskManager.h
	$(MOC) AsteriskManager.h -o moc_AsteriskManager.cpp

moc_QueueMonitor.cpp: QueueMonitor.h
	$(MOC) QueueMonitor.h -o moc_QueueMonitor.cpp

moc_AgentStatus.cpp: AgentStatus.h
	$(MOC) AgentStatus.h -o moc_AgentStatus.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.
