# Compiling the tools

## stm8gal

Project repository:
[https://github.com/gicking/stm8gal](https://github.com/gicking/stm8gal)

	git clone git@github.com:gicking/stm8gal.git




### For Linux

	cd stm8gal
	make
	cp -av stm8gal /usr/local/bin

### For Linux 32 bit on a 64 bit Linux system

Required packages:

	apt install gcc-multilib libusb-1.0-0-dev:i386

Compiling stm8gal requires a small modification in the Makefile. For CFLAGS
and LDFLAGS change the `=` operator to `+=` to allow for additional
parameters on the command line:


	cd stm8gal
	sed -i '/FLAGS/ s/ = / += /' Makefile

Now start the compiler:

	CFLAGS=-m32 LDFLAGS=-m32 make



### For Windows

Windows does not support an SPI device. Apply [this
patch](../img/no-spi.patch) to disable the compilation of spi_spidev_comm.c
for Windows:

	patch -p1 < no-spi.patch

Now compile:

- on a Windows system: `make`
- using mingw on Linux: `make CC=i686-w64-mingw32-gcc OS=Win`




## stm8flash

Project repository:
[https://github.com/vdudouyt/stm8flash](https://github.com/vdudouyt/stm8flash)

Compiling for Linux and OSX is straight forward, Windows is more
complicated.


###  For Linux

Required packages:

	apt install libusb-1.0-0-dev

Compiling stm8flash:

	git clone git@github.com:vdudouyt/stm8flash.git
	cd stm8flash
	make RELEASE=yes



### For Linux 32 bit on a 64 bit Linux system

Required packages:

	apt install gcc-multilib libusb-1.0-0-dev:i386

Compiling stm8flash:

	git clone git@github.com:vdudouyt/stm8flash.git
	cd stm8flash
	make RELEASE=yes CFLAGS=-m32





### Cross-Compiling for windows on a Linux system

Current versions of stm8flash support the espstlink programmer.
The programmer is conneced via USB and a virtual serial port, not over Wifi
and for serial access the termios library is used. Being a POSIX function,
this is not supported by mingw, only by cygwin. As a workaround apply [this
patch](../img/no-espstlink-10810e88.patch) to disable the espstlink
functionality on Windows systems:

	patch -p1 < no-espstlink-10810e88.patch

Obviously, a better approach would be the use of a simple intermediate
cross-platform layer like
[libserialport](https://sigrok.org/wiki/Libserialport).


Required packages:

	apt install mingw-w64 mingw-w64-tools

Get stm8flash and patch the sources to cut out espstlink code:

	git clone git@github.com:vdudouyt/stm8flash.git
	cd stm8flash
	wget https://tenbaht.github.io/sduino/img/no-espstlink-10810e88.patch

Download the 7z file containing the latest [libusb dll
binary](https://github.com/libusb/libusb/releases). Unpack the archive into
a new directory inside the stm8flash directory:

	7z -owindows x <libusb-xxx.7z>

Compiling stm8flash (as 32 bit binary for XP compatibility):

	make CC=i686-w64-mingw32-gcc RELEASE=yes CFLAGS="-Iwindows/include -Lwindows/MinGW32/dll" PLATFORM=w7



### For Windows on a Windows system


https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2

- Open msys window, execute `pacman -Syuu`, close window.
- repeat the before step until no more updates are found.
- do eigther the full install (700MB):
	pacman -S --needed base-devel mingw-w64-i686-toolchain
- or the reduced install (350MB):
	pacman -S --needed mingw-w64-i686-toolchain make grep sed pkg-config
- minimum install (150MB):
	pacman -S --needed mingw-w64-i686-binutils mingw-w64-i686-crt-git
	mingw-w64-i686-gcc mingw-w64-i686-gcc-libs mingw-w64-i686-headers-git
	mingw-w64-i686-libmangle-git mingw-w64-i686-libwinpthread-git
	mingw-w64-i686-make mingw-w64-i686-pkg-config mingw-w64-i686-tools-git
	mingw-w64-i686-winpthreads-git mingw-w64-i686-winstorecompat-git

- copy `libusb-1.0/libusb.h` and `libusb.dll` into current directory
- `make RELEASE=yes CFLAGS="-I. -L."`


### For Mac OSX


#### stm8flash

Required packages (libusb pulls in gcc if not already installed):

	brew install libusb pkg-config

Change into the stm8flash source directory and change the `#include
"libusb.h"` to "#include "libusb-1.0/libusb.h"` (same as for Windows). Now
it compiles:

        make RELEASE=yes



#### stm8gal

No SPI support, so compiling is similar to Windows and the same modification
of the Makefile is necessary: Remove `spi_spidev_comm.c` from the list of
source files in `SOURCES` in the Makefile.

Now we can compile, but we need to convince the Makefile not to set the
`USE_SPIDEV` define. An easy workaround to do so is to fool the Makefile
into assuming a Windows build:

	make OS=Win

