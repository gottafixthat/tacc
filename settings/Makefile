####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR)/include -I. -I../customers -I../include -I../lib
INCDIRS += -I../ccards
INCDIRS += -I../reports -I../config -I../voicemail
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include
CFLAGS += -DADBQT
# CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
LFLAGS  += -lcustomers -lccards -L../reports -lreports -lconfig -lvoicemail -lscheduler
LFLAGS  += -ladbqt -lqt -lmysqlclient -lblarg -lcrypt -L../lib -L$(QTDIR)/lib 
LFLAGS	+= -L/usr/X11R6/lib -lX11 -lXext -L/usr/lib/mysql -lmysqlclient
LFLAGS  += -lBTools -lFParse
LFLAGS  += -lccvs
ifdef USEQWT
LFLAGS  += -lqwt
endif
LFLAGS  += -liserver -lmimepp_util -lmimepp_net -lmimepp_core
# LFLAGS  += -static
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

SOURCES =	SettingsManager.cpp\
		GeneralSettings.cpp

HEADERS =	SettingsManager.h\
		GeneralSettings.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_SettingsManager.cpp\
		moc_GeneralSettings.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libtaasettings.a
# TARGET	=	../bin/admin

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCDIRS) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)
#	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2>/dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_SettingsManager.cpp: SettingsManager.h
	$(MOC) SettingsManager.h -o moc_SettingsManager.cpp

moc_GeneralSettings.cpp: GeneralSettings.h
	$(MOC) GeneralSettings.h -o moc_GeneralSettings.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.
