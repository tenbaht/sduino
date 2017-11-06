# the required versions for the new build
COREVERSION=0.3.1
SDCCVERSION=10088
TOOLSVERSION=2017.11.06

PACKAGEFILE=package_sduino_stm8_index.json
GITHUB_BASE=https://github.com/tenbaht/sduino/releases/download/v$(COREVERSION)

export

.PHONY: core tools sdcc clean upload


$(PACKAGEFILE): core tools sdcc
	@echo "Generating the package_index file."
	./assemble.sh $@

../$(PACKAGEFILE): $(PACKAGEFILE)
	sed "s#file:///home/mmayer/rsync/stm8/board-manager/release#$(GITHUB_BASE)#" $^ > $@

core:
	VERSION=$(COREVERSION) make -f Makefile.core

tools:
	VERSION=$(TOOLSVERSION) make -f Makefile.tools

sdcc:
	VERSION=$(SDCCVERSION) make -f Makefile.sdcc

clean:
	rm -rf *~ *.jsone *.bak *.orig *.rej


# upload to my private test server, totally useless for anybody else.
upload: $(PACKAGEFILE)
	sed 's#file:///home/mmayer/rsync/stm8/board-manager#http://knecht.fritz.box/sduino#' $^ > package_knecht_index.json
	rsync -rsh=ssh --delete release package_knecht_index.json knecht:/var/www/html/sduino
