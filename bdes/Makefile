####### This section is automatically generated

INCDIR	=  libdes
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W -I. # -DDBDEBUG
LFLAGS  = -ldes -Llibdes
SHELL   = /bin/sh
CC	= gcc
MOC	= /usr/local/bin/moc

####### End of automatically generated section

####### Files

SUBDIRS =	libdes

SOURCES =	bdes.c

OBJECTS =	$(SOURCES:.c=.o)
EXTRAOBJS =	libdes/*.o

# TARGET	=	qtacct

####### Implicit rules

.SUFFIXES: .c

.c.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: bdes-lib

bdes-lib: $(SUBDIRS) $(OBJECTS)
	ar r libbdes.a libdes/libdes.a $(OBJECTS) $(EXTRAOBJS)
	ranlib libbdes.a
	# mv libblarg.a lib

$(SUBDIRS): FORCE
	echo Entering directory $@
	cd $@; $(MAKE)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LFLAGS)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

install: $(TARGET)
	cp bdes.h /usr/local/include/
	cp libbdes.a /usr/local/lib/

FORCE:

####### Meta objects


# DO NOT DELETE THIS LINE -- make depend depends on it.

