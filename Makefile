##
## $Id: Makefile,v 1.4 2004/08/21 15:45:01 marc Exp $
##

.SILENT:
VERSION     = 4
PATCHLEVEL  = 0
SUBLEVEL    = 0

## Define this to include triple DES encryption for credit card, electronic
## check, and secure connection encryption to be enabled.
USEDES  = 1
USEQTW  = 0
EXPERIMENTAL = 1

## include file and library file path definitions
INCDIR	=  /usr/lib/qt/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I./include/
#CFLAGS += -I/usr/include/g++-3
LFLAGS  = -lqt -lmysqlclient -lblargdb -L./lib
SHELL   = /bin/sh
CC	= gcc

####### Files

export QTDIR=/usr/include/qt4
export QTMOC=/usr/bin/moc
export QTLIB=/usr/lib/qt4/
export MYSQLLIB=/usr/lib/mysql
export MYSQLINC=/usr/include/mysql
BUILDNUM=scripts/buildnum
#SUBDIRS	= qgtimespinbox lib
SUBDIRS	= bdes
SUBDIRS	+= taccCore taccGui lib
ifdef USEDES
# SUBDIRS	+= libdes bdes
	export USEDES=1
endif

SUBDIRS += voip voicemail ccards asterisk customers
SUBDIRS += reports tickets admin settings custcare
SUBDIRS += taccupgrade
SUBDIRS += taamaild
SUBDIRS += ccexpired
SUBDIRS += cccimport
SUBDIRS += modembilling
SUBDIRS += radimport
SUBDIRS += ccget
SUBDIRS += wasync
SUBDIRS += backend

SOURCES =	

HEADERS =	

OBJECTS =	

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SUBDIRS)

$(SUBDIRS): FORCE
	$(BUILDNUM)
	echo Entering directory $@
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

#depend:
#	set -e; for i in $(SUBDIRS); do cd $$i ; $(MAKE) depend ; cd ..; done
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	set -e; for i in $(SUBDIRS); do cd $$i ; echo make clean in $$i ; $(MAKE) clean ; cd ..; done
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET) include/version.h

FORCE: include/version.h

include/version.h: ./Makefile
	@echo \#ifndef VERSION > .ver
	@echo \#define VERSION \"$(VERSION).$(PATCHLEVEL).$(SUBLEVEL)\" >> .ver
	@echo \#endif >> .ver
	@mv -f .ver $@

rpms: clean
	$(BUILDNUM)
	scripts/makerpm

####### Meta objects


# DO NOT DELETE THIS LINE -- make depend depends on it.
