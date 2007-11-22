####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR)/include -I. -I../customers -I../include -I../lib -I../tickets -I../reports -I../voicemail -I../voip -I../admin -I../settings -I../ccards -I../asterisk
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -DADBQT
# CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
LFLAGS  += -ladmin -ltaasettings -lcustomers -L../reports -lreports -lvoicemail -lvoip -ltickets -lccards -lasterisk
LFLAGS  += -L$(QTDIR)/lib -ltaa -ladbqt -lqt-mt -lcrypt -L../lib 
LFLAGS	+= -L/usr/X11R6/lib -lX11 -lXext -lXft -lSM -lICE -L/usr/lib64/mysql -L/usr/lib/mysql -lmysqlclient
LFLAGS  += -lmimepp_util -lmimepp_net -lmimepp_core 
LFLAGS  += -lmcve
LFLAGS  += -lpcre
ifdef USEQWT
LFLAGS  += -lqwt
endif
#LFLAGS  += -static
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
	LFLAGS	+= -L/usr/local/lib -lbdes
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	tacc.cpp\
			CallWizard.cpp\
			CallTypeSelector.cpp

HEADERS =	tacc.h\
			CallWizard.h\
			CallTypeSelector.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_tacc.cpp\
			moc_CallWizard.cpp\
			moc_CallTypeSelector.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../bin/tacc

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCDIRS) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA) ../lib/libadmin.a ../lib/libcustomers.a
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

moc_tacc.cpp: tacc.h
	$(MOC) tacc.h -o moc_tacc.cpp

moc_CallWizard.cpp: CallWizard.h
	$(MOC) CallWizard.h -o moc_CallWizard.cpp

moc_CallTypeSelector.cpp: CallTypeSelector.h
	$(MOC) CallTypeSelector.h -o moc_CallTypeSelector.cpp



# DO NOT DELETE THIS LINE -- make depend depends on it.
