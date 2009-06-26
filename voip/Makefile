####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -O2 -fno-strength-reduce -I/usr/X11R6/include -I. -I../include/
CFLAGS	+= -I../taccCore -I../taccGui -I../lib
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTMOC)

# CFLAGS += -DDBDEBUG
CFLAGS += -DQT3_SUPPORT

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	VoIPCommon.cpp\
			DIDManager.cpp\
			RateCenterManager.cpp\
			OriginationProviders.cpp\
			VoIPServiceTypes.cpp
#			ProcessVoiceMail.cpp

# HEADERS =	../include/BlargDB.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_DIDManager.cpp\
			moc_RateCenterManager.cpp\
			moc_OriginationProviders.cpp\
			moc_VoIPServiceTypes.cpp
#			moc_ProcessVoiceMail.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libvoip.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
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

moc_DIDManager.cpp: DIDManager.h
	$(MOC) DIDManager.h -o moc_DIDManager.cpp

moc_RateCenterManager.cpp: RateCenterManager.h
	$(MOC) RateCenterManager.h -o moc_RateCenterManager.cpp

moc_OriginationProviders.cpp: OriginationProviders.h
	$(MOC) OriginationProviders.h -o moc_OriginationProviders.cpp

moc_VoIPServiceTypes.cpp: VoIPServiceTypes.h
	$(MOC) VoIPServiceTypes.h -o moc_VoIPServiceTypes.cpp

#moc_ProcessVoiceMail.cpp: ProcessVoiceMail.h
#	$(MOC) ProcessVoiceMail.h -o moc_ProcessVoiceMail.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.
