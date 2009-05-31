###########################################################################
##
## Makefile - cheetahd Makefile
##
###########################################################################
## Written by R. Marc Lewis <marc@blarg.net>
##   (C)opyright 2005, R. Marc Lewis
##   All Rights Reserved.
##
##  Unpublished work.  No portion of this file may be reproduced in whole
##  or in part by any means, electronic or otherwise, without the express
##  written consent of R. Marc Lewis.
###########################################################################

QTDIR=/usr/lib/qt4
QTINC=$(QTDIR)/include
INCDIRS	=  -I. -I../include -I../../lib -I../../taccCore -I/usr/local/include
INCDIRS += -I$(QTINC) -I$(QTINC)/QtCore -I$(QTINC)/QtNetwork -I$(QTINC)/QtSql -I$(QTINC)/QtXml
CFLAGS= -O2 -fno-strength-reduce -Wall -W
CFLAGS += -DQT3_SUPPORT
LFLAGS	+= -L../../lib -L/usr/lib/mysql -L/usr/local/lib -L$(QTDIR)/lib -lQtNetwork -lQtCore -lQtSql -lQtXml
LFLAGS += -L../../taccCore -ltaa -ltaccCore -L/usr/lib64/mysql -L/usr/lib/mysql -lmysqlclient -L../../bdes -lbdes -lQt3Support
# LFLAGS  += -lmysqlclient
# LFLAGS  += -static

SHELL   = /bin/sh
CC	= g++

MOC = /usr/bin/moc-qt4

####### Files

SUBDIRS =	

SOURCES =	ratecdrs.cpp\
			cdrs.cpp

HEADERS =	

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_cdrs.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../../bin/cdrs

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) $(INCDIRS) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	echo Entering directory $@
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	echo Linking $(TARGET)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)
#strip $(TARGET)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

##### Meta Objects

moc_cdrs.cpp: cdrs.h
	$(MOC) cdrs.h -o moc_cdrs.cpp

moc_cbrassworker.cpp: cbrassworker.h
	$(MOC) cbrassworker.h -o moc_cbrassworker.cpp

moc_netserver.cpp: netserver.h
	$(MOC) netserver.h -o moc_netserver.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.
