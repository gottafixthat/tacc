####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
CFLAGS += -I../customers
CFLAGS += -I../qgtimespinbox
CFLAGS += -I../lib
CFLAGS += -I../adb
CFLAGS += -DADBQT
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib -shared
LFLAGS += -L../lib -ltaa
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

# CFLAGS += -DDBDEBUG

ifdef USEDES
	CFLAGS	+= -DUSEDES -I../bdes
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	Ticket.cpp\
			TicketManager.cpp\
			CustomerTickets.cpp\
			CreateCustTicket.cpp\
			TicketEditor.cpp

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_TicketManager.cpp\
			moc_CustomerTickets.cpp\
			moc_CreateCustTicket.cpp\
			moc_TicketEditor.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libtickets.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) report-lib

report-lib: $(OBJECTS)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% *.a #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_TicketManager.cpp: TicketManager.h
	$(MOC) TicketManager.h -o moc_TicketManager.cpp

moc_CustomerTickets.cpp: CustomerTickets.h
	$(MOC) CustomerTickets.h -o moc_CustomerTickets.cpp

moc_CreateCustTicket.cpp: CreateCustTicket.h
	$(MOC) CreateCustTicket.h -o moc_CreateCustTicket.cpp

moc_TicketEditor.cpp: TicketEditor.h
	$(MOC) TicketEditor.h -o moc_TicketEditor.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.
