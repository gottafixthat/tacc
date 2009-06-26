####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR) -I. -I../../include -I../../lib 
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I../../taccCore -I../../taccGui
# CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
LFLAGS  += -L../../lib -ltaa -L../../taccCore -ltaccCore -L../../taccGui -ltaccGui
LFLAGS  += -L$(QTDIR)/lib -lQtCore -lQtSql -lQt3Support -lQtGui
LFLAGS  += -lcrypt -L../../lib -L../../bdes
LFLAGS	+= -L/usr/X11R6/lib -lX11 -lXext -lXft -lSM -lICE -L/usr/lib64/mysql -L/usr/lib/mysql -lmysqlclient
#LFLAGS  += -lmimepp_util -lmimepp_net -lmimepp_core 
#LFLAGS  += -lmimetic
#LFLAGS  += -lmcve
LFLAGS  += -lpcre
ifdef USEQWT
LFLAGS  += -lqwt
endif
#LFLAGS  += -static
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTMOC)

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
	LFLAGS	+= -L/usr/local/lib -lbdes
endif
CFLAGS += -DQT3_SUPPORT

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	cmrimport.cpp

HEADERS =	cmrimport.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_cmrimport.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../../bin/cmrimport

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

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_cmrimport.cpp: cmrimport.h
	$(MOC) cmrimport.h -o moc_cmrimport.cpp



# DO NOT DELETE THIS LINE -- make depend depends on it.
