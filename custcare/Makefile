####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR)/include -I. -I../customers -I../include -I../lib -I../tickets -I../reports -I../voicemail -I../admin -I../ccards
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -DADBQT
# CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
LFLAGS  += -ladmin -lcustomers -L../reports -lreports -lvoicemail -ltickets -lccards
LFLAGS  += -L$(QTDIR)/lib -ltaa -ladbqt -lqt-mt -lcrypt -L../lib 
LFLAGS	+= -L/usr/X11R6/lib -lX11 -lXext -lXft -lSM -lICE -L/usr/lib/mysql -lmysqlclient
LFLAGS  += -lmimepp_util -lmimepp_net -lmimepp_core 
LFLAGS  += -lmcve
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

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

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

CallWizard.o: CallWizard.h CallTypeSelector.h /usr/include/stdio.h
CallWizard.o: /usr/include/features.h /usr/include/sys/cdefs.h
CallWizard.o: /usr/include/gnu/stubs.h
CallWizard.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
CallWizard.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CallWizard.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CallWizard.o: /usr/include/_G_config.h /usr/include/wchar.h
CallWizard.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CallWizard.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
CallWizard.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
CallWizard.o: /usr/include/stdlib.h /usr/include/math.h
CallWizard.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
CallWizard.o: /usr/include/bits/mathcalls.h
CallTypeSelector.o: CallTypeSelector.h /usr/include/stdio.h
CallTypeSelector.o: /usr/include/features.h /usr/include/sys/cdefs.h
CallTypeSelector.o: /usr/include/gnu/stubs.h
CallTypeSelector.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stddef.h
CallTypeSelector.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
CallTypeSelector.o: /usr/include/bits/typesizes.h /usr/include/libio.h
CallTypeSelector.o: /usr/include/_G_config.h /usr/include/wchar.h
CallTypeSelector.o: /usr/include/bits/wchar.h /usr/include/gconv.h
CallTypeSelector.o: /usr/lib/gcc-lib/i686-pc-linux-gnu/3.3.4/include/stdarg.h
CallTypeSelector.o: /usr/include/bits/stdio_lim.h
CallTypeSelector.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
