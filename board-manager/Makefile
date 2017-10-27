# the required versions for the new build
COREVERSION=0.3.0-pre.1
SDCCVERSION=10088
TOOLSVERSION=2017.10.21

PACKAGEFILE=package_sduino_stm8_index.json

export

.PHONY: core tools sdcc


$(PACKAGEFILE): core tools sdcc
	@echo "Generating the package_index file."
	./assemble.sh $@
	sed 's#file:///home/mmayer/rsync/stm8/board-manager/release#http://knecht.box/sduino#' $(PACKAGEFILE) > package_knecht_index.json

core:
	VERSION=$(COREVERSION) make -f Makefile.core

tools:
	VERSION=$(TOOLSVERSION) make -f Makefile.tools

sdcc:
	VERSION=$(SDCCVERSION) make -f Makefile.sdcc

clean:
	rm -rf *~ *.jsone *.bak *.orig *.rej

