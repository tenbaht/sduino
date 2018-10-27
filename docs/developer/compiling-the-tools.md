# Compiling the tools

## stm8gal

Project page: https://github.com/gicking/stm8gal

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

Windows does not support an SPI device. Apply this patch to disable
compilation of spi_spidev_comm.c for Windows:

```diff
diff --git a/Makefile b/Makefile
index 24b0057..053d17b 100644
--- a/Makefile
+++ b/Makefile
@@ -4,7 +4,7 @@ CC            = gcc
 CFLAGS        = -c -Wall -I./STM8_Routines
 #CFLAGS       += -DDEBUG
 LDFLAGS       = -g3 -lm
-SOURCES       = bootloader.c hexfile.c main.c misc.c serial_comm.c spi_spidev_comm.c spi_Arduino_comm.c
+SOURCES       = bootloader.c hexfile.c main.c misc.c serial_comm.c spi_Arduino_comm.c
 INCLUDES      = globals.h misc.h bootloader.h hexfile.h serial_comm.h spi_spidev_comm.h spi_Arduino_comm.h main.h
 STM8FLASH     = STM8_Routines/E_W_ROUTINEs_128K_ver_2.1.s19 STM8_Routines/E_W_ROUTINEs_128K_ver_2.0.s19 STM8_Routines/E_W_ROUTINEs_256K_ver_1.0.s19 STM8_Routines/E_W_ROUTINEs_32K_ver_1.3.s19 STM8_Routines/E_W_ROUTINEs_32K_ver_1.4.s19 STM8_Routines/E_W_ROUTINEs_128K_ver_2.2.s19 STM8_Routines/E_W_ROUTINEs_32K_ver_1.0.s19 STM8_Routines/E_W_ROUTINEs_128K_ver_2.4.s19 STM8_Routines/E_W_ROUTINEs_32K_ver_1.2.s19  STM8_Routines/E_W_ROUTINEs_32K_verL_1.0.s19 STM8_Routines/E_W_ROUTINEs_8K_verL_1.0.s19
 STM8INCLUDES  = $(STM8FLASH:.s19=.h)
@@ -16,6 +16,7 @@ RM            = rm -fr
 # add optional SPI support via spidev library (Windows not yet supported)
 ifeq (,$(findstring Win,$(OS)))
 	CFLAGS  += -DUSE_SPIDEV
+	SOURCES += spi_spidev_comm.c
 endif
 
 # add optional GPIO reset via wiringPi library (Raspberry only) 
diff --git a/misc.c b/misc.c
index 9fe02a0..67b5c0d 100644
--- a/misc.c
+++ b/misc.c
@@ -22,7 +22,7 @@
 // WIN32 specific
 #if defined(WIN32)
 
-  #include "Windows.h"
+  #include "windows.h"
 
   // forground colours
   #define FG_BLACK      0
```

Now compile:

- on a Windows system: `make`
- using mingw on Linux: `make CC=i686-w64-mingw32-gcc OS=Win`




## stm8flash

Project page: https://github.com/vdudouyt/stm8flash

Compiling for Linux and OSX is straight forward, Windows is more
complicated. Current versions of stm8flash support the espstlink programmer.
The programmer is conneced via USB and a virtual serial port, not over Wifi
and for serial access the termios library is used. Being a POSIX function,
this is not supported by mingw, only by cygwin. As a workaround I use this
patch to disable the espstlink functionality on Windows systems:

```diff
diff --git a/Makefile b/Makefile
index 7ed48d1..e76a69f 100644
--- a/Makefile
+++ b/Makefile
@@ -39,6 +39,7 @@ else
 	LIBS   = -lusb-1.0
 	LIBUSB_CFLAGS =
 	CC	   ?= GCC
+	SKIP_ESP   = yes
 	BIN_SUFFIX =.exe
 endif
 
@@ -47,8 +48,12 @@ override CFLAGS := $(BASE_CFLAGS) $(LIBUSB_CFLAGS) $(CFLAGS)
 
 
 BIN 		=stm8flash
-OBJECTS 	=stlink.o stlinkv2.o espstlink.o main.o byte_utils.o ihex.o srec.o stm8.o libespstlink.o
-
+OBJECTS 	=stlink.o stlinkv2.o main.o byte_utils.o ihex.o srec.o stm8.o
+ifeq ($(SKIP_ESP),yes)
+	override CFLAGS  += -DNO_ESP
+else
+	OBJECTS +=espstlink.o libespstlink.o
+endif
 
 .PHONY: all clean install
 
diff --git a/main.c b/main.c
index ce19e47..29a25d2 100644
--- a/main.c
+++ b/main.c
@@ -54,6 +54,7 @@ programmer_t pgms[] = {
 		stlink2_swim_read_range,
 		stlink2_swim_write_range,
 	},
+#ifndef NO_ESP
     {
 		"espstlink",
 		0,
@@ -64,6 +65,7 @@ programmer_t pgms[] = {
 		espstlink_swim_read_range,
 		espstlink_swim_write_range,
 	},
+#endif
 	{ NULL },
 };
 
@@ -72,9 +74,15 @@ void print_help_and_exit(const char *name, bool err) {
 	fprintf(stream, "Usage: %s [-c programmer] [-S serialno] [-p partno] [-s memtype] [-b bytes] [-r|-w|-v] <filename>\n", name);
 	fprintf(stream, "Options:\n");
 	fprintf(stream, "\t-?             Display this help\n");
-	fprintf(stream, "\t-c programmer  Specify programmer used (stlink, stlinkv2 or espstlink)\n");
+	fprintf(stream, "\t-c programmer  Specify programmer used (stlink, stlinkv2"
+#ifndef NO_ESP
+	" or espstlink"
+#endif
+	")\n");
 	fprintf(stream, "\t-S serialno    Specify programmer's serial number. If not given and more than one programmer is available, they'll be listed.\n");
+#ifndef NO_ESP
 	fprintf(stream, "\t-d port        Specify the serial device for espstlink (default: /dev/ttyUSB0)\n");
+#endif
 	fprintf(stream, "\t-p partno      Specify STM8 device\n");
 	fprintf(stream, "\t-l             List supported STM8 devices\n");
 	fprintf(stream, "\t-s memtype     Specify memory type (flash, eeprom, ram, opt or explicit address)\n");
```

Obviously, a better approach would be the use of a simple intermediate
cross-platform layer like
[libserialport](https://sigrok.org/wiki/Libserialport).




###  For Linux

Required packages:

	apt install libusb-1.0-0-dev`

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

Required packages:

	apt install mingw-w64 mingw-w64-tools

Compiling stm8flash:

	git clone git@github.com:vdudouyt/stm8flash.git
	cd stm8flash
	make CC=i686-w64-mingw32-gcc RELEASE=yes CFLAGS=-I. PLATFORM=w7



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

