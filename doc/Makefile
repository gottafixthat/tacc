SUBDIRS = manual

all: $(SUBDIRS)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

clean:
	set -e; for i in $(SUBDIRS); do cd $$i ; $(MAKE) clean ; cd ..; done

FORCE:

