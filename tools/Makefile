
# build all tool binaries on a Linux 64 system
#
# Used system: Linux Mint 19
#
# This assumes that all required cross-compilation environments are properly
# installed.

OSXCC=/opt/osxcross/bin/o64-clang
OSXC++=/opt/osxcross/bin/o64-clang++
OSXPKGCONFIG=/opt/osxcross/bin/x86_64-apple-darwin15-pkg-config
OSXSTRIP=/opt/osxcross/bin/x86_64-apple-darwin15-strip


BINDIR=bin
#BINDIR=../sduino/hardware/sduino/tools

.PHONY: all linux32 linux64 osx windows \
	stm8gal stm8gal-windows stm8gal-linux32 stm8gal-linux64 stm8gal-osx \
	stm8flash stm8flash-windows stm8flash-linux32 stm8flash-linux64 \
	stm8flash-osx



# all tools for all platforms
all: stm8gal stm8flash


# all tools for one platform: Linux 32 bit
linux32: stm8gal-linux32 stm8flash-linux32

# all tools for one platform: Linux 64 bit
linux64: stm8gal-linux64 stm8flash-linux64

# all tools for one platform: OSX 10.11 64 bit
osx: stm8gal-osx stm8flash-osx

# all tools for one platform: Windows 64 bit
windows: stm8gal-windows stm8flash-windows


# stm8gal for all platforms
stm8gal: stm8gal-linux32 stm8gal-linux64 stm8gal-osx stm8gal-windows

# stm8flash for all platforms
stm8flash: stm8flash-windows stm8flash-linux32 stm8flash-linux64 stm8flash-osx

# copy the already compiled binaries into sduino/.../tools
install:
	echo "This rule intentionally does not trigger a re-compile."
	echo "Run 'make all', 'make stm8gal' or 'make stm8flash' if you really"
	echo "want fresh binaries."
	cp -avn bin/* ../sduino/hardware/sduino/tools/

# rules for compiling stm8gal
stm8gal-linux32:
	sed '/FLAGS/ s/ = / += /' stm8gal/Makefile > stm8gal/Makefile.linux32
	CFLAGS=-m32 LDFLAGS=-m32 make -C stm8gal -f Makefile.linux32 clean all
	install -s -D stm8gal/stm8gal $(BINDIR)/linux32/stm8gal
	rm -f stm8gal/Makefile.linux32

stm8gal-linux64:
	make -C stm8gal clean all
	install -s -D stm8gal/stm8gal $(BINDIR)/linux64/stm8gal

stm8gal-osx:
	make -C stm8gal CC=$(OSXCC) clean all
	install -s --strip-program=$(OSXSTRIP) -D stm8gal/stm8gal $(BINDIR)/macosx/stm8gal

stm8gal-windows:
	make -C stm8gal CC=i686-w64-mingw32-gcc OS=Win clean all
	install -s -D stm8gal/stm8gal $(BINDIR)/win/stm8gal.exe


# rules for compiling stm8flash
stm8flash-linux32:
	make -C stm8flash RELEASE=yes CFLAGS=-m32 clean all
	install -s -D stm8flash/stm8flash $(BINDIR)/linux32/stm8flash

stm8flash-linux64:
	make -C stm8flash RELEASE=yes clean all
	install -s -D stm8flash/stm8flash $(BINDIR)/linux64/stm8flash

stm8flash-osx:
	# install dependencies: osxcross-macports install libusb-devel
	make -C stm8flash CC=$(OSXCC) CXX=$(OSXC++) \
		PKGCONFIG=$(OSXPKGCONFIG) \
		RELEASE=yes clean all
	install -s --strip-program=$(OSXSTRIP) -D stm8flash/stm8flash $(BINDIR)/macosx/stm8flash

stm8flash-windows:
	make -C stm8flash CC=i686-w64-mingw32-gcc \
		CFLAGS="-I../lib/windows -L../lib/windows" \
		PLATFORM=w7 \
		RELEASE=yes clean all
	install -s -D stm8flash/stm8flash.exe $(BINDIR)/win/stm8flash.exe
