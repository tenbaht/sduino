# the required versions for the new build
COREVERSION=0.4.0-pre3
SDCCVERSION=10738
TOOLSVERSION=2018.12.06

BASEURL=https://github.com/tenbaht/sduino/releases/download/v$(COREVERSION)

PACKAGEFILE=package_sduino_stm8_index.json

# version for pre-release testing on my private server at home
TESTPACKAGEFILE=package_testserver_index.json
TESTBASEURL=http://knecht.fritz.box/sduino
TESTBASEDIR=knecht:/var/www/html/sduino

# make sure these definitions are visible for the sub-makes
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
	# update the version number in the changelog if not already done:
	grep -q "$(COREVERSION)" ../CHANGELOG.md || \
	sed "/^## \[Unreleased\]/ a\\\n\n## [$(COREVERSION) - $(DATE)]" ../CHANGELOG.md
	git add ../CHANGELOG.md
	git add parts/tools-*-$(SDCCVERSION).txt parts/tools-*-$(TOOLSVERSION).txt parts/platform-*-$(COREVERSION).txt
	git commit
	git tag v$(COREVERSION)
