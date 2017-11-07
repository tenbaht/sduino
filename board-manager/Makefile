# the required versions for the new build
COREVERSION=0.3.1
SDCCVERSION=10088
TOOLSVERSION=2017.11.06

BASEURL=https://github.com/tenbaht/sduino/releases/download/v$(COREVERSION)

PACKAGEFILE=package_sduino_stm8_index.json

# version for pre-release testing on my private server at home
TESTPACKAGEFILE=package_testserver_index.json
TESTBASEURL=http://knecht.fritz.box/sduino
TESTBASEDIR=knecht:/var/www/html/sduino

export

.PHONY: core tools sdcc clean upload release


$(TESTPACKAGEFILE): ../$(PACKAGEFILE)
	sed 's#http.*/download/#$(TESTBASEURL)/release/#' $^ > $@

../$(PACKAGEFILE): core tools sdcc
	@echo "Generating the package_index file."
	./assemble.sh $@

core tools sdcc:
	make -f Makefile.$@

clean:
	rm -rf *~ *.jsone *.bak *.orig *.rej


# upload to my private test server, totally useless for anybody else.
upload: $(TESTPACKAGEFILE)
	@echo "uploading to $(TESTBASEDIR)"
	rsync -av -rsh=ssh --delete release $(TESTPACKAGEFILE) $(TESTBASEDIR)
	@echo "URL for board manager: $(TESTBASEURL)/$(TESTPACKAGEFILE)"

DATE=$(shell date '+%F')

release:
	echo sed "/^## \[Unreleased\]/ a\\\n\n## [$(COREVERSION) - $(DATE)]" ../CHANGELOG.md
	echo git add ../CHANGELOG.md
	echo git add parts/tools-*  parts/platform-*
	echo git commit
	echo git tag v$(COREVERSION)
