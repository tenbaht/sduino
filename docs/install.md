# Installing the needed tools

All tools are available for Linux, MacOS and Windows, most of them even for
Raspbian on a RaspberryPi.

SDCC support for the STM8 is still quite fresh and not very mature. It
improves significantly from version to version. Be sure to use
[the latest snapshot build](http://sdcc.sourceforge.net/snap.php) from the
[project site on sourceforge](http://sdcc.sourceforge.net/), not the older
version that might be included in your distribution.



## Linux

Download the lastest snapshot build for SDCC from
http://sdcc.sourceforge.net/snap.php and extract it under `/opt`.

	mkdir /opt
	tar xvjf ~/Downloads/sdcc-snapshot* -C /opt

`make` is part of the standard repository for any Linux distribution.
Example for Debian-type systems (like Ubuntu, Mint, Elementary etc.):

	apt-get install make

`stm8flash` is included in the tools directory of this repository. Copy it
to a convient place in your path, e.g. `/usr/local/bin`. To use it you might
need to add a udev rule in order to access the USB port. Save this as root
in in `/etc/udev/rules.d/99-stlink.rules`:

	# ST-Link/V2 programming adapter

	# ST-Link V1, if using a STM8S discovery board
	# important: It needs a special entry in /etc/modprob/blacklist
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3744", MODE="0666", GROUP="plugdev"

	# ST-Link/V2, the china adapter with the green plastic housing
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev"



## MacOS

Quite similar. [Install Homebrew](https://brew.sh/) if you haven't done it
yet, than:

	brew make
	mkdir /opt
	tar xvjf sdcc-snapshot* -C /opt

Not sure about stm8flash, I didn't try it. Have a look at the
[stm8flash repository](https://github.com/vdudouyt/stm8flash).




## Windows

Harder, but still possible. It is lacking all the tools, a package system to
install them and even a file system concept where to store them. For
convinience, I collected the bare minimum of needed tools in a
[separate repository](https://github.com/tenbaht/sduino-windowstools).
Download it and copy the directory tree into the sduino
directory. Add the sduino/tools/bin directory to your path or move the files
in there to a directory that is already in your path.

You are not bound to `stm8flash`, you can use the regular flash tool by ST
as well.



### The individual pieces

All the tools in the convinience package are 32 bit. If you need the 64 bit
versions you might have to collect them yourself.

You need SDCC, `make` with some basic tools and maybe `stm8flash`. Get SDCC
from the project website. `make` is a standard tool included in either
MinGW/Msys or cygwin. Both are fine, MinGW/Msys is smaller. `stm8flash` from
the [stm8flash repository](https://github.com/vdudouyt/stm8flash).



#### SDCC

1. Download the lastest snapshot build from http://sdcc.sourceforge.net/snap.php
2. unzip into c:\sdcc (use a path name without whitespace)

You only need support for the stm8. You can safely delete all the files
related to the pic1x CPUs (that safes more than 90% of the used disk space
and leaves only 20MB out of 240MB)


#### MinGW

[MinGW/MSys](http://www.mingw.org/wiki/MSYS) and
[cygwin](https://www.cygwin.com/) are both fine. cygwin aims to be an almost
complete POSIX environment (which is nice, but we don't need it here). MinGW
wants to be more compact and works with the native Windows API. That is good
enough for this purpose.

1. Check the
  [MinGW Installation Notes](http://www.mingw.org/wiki/Getting_Started)
2. Download
  [mingw-get-setup.exe](https://sourceforge.net/projects/mingw/files/Installer/)
  from https://sourceforge.net/projects/mingw/files/Installer/
3. Start it. You can safely deactivate the graphical option.
4. Add this at the end to your path: `;c:\mingw\bin;c:\mingw\msys\1.0\bin`
  (follow the instructions in "Environment Setting" on the [Installation
  Notes page[(http://www.mingw.org/wiki/Getting_Started))
5. Open a command line and install the package msys-base by issuing this
  command: `mingw-get install msys-base`
6. Now `ls` or `make` should work.

For efficiency, the Makefile is configured to use dash instead of bash as a
shell. `egrep` is replaced by `grep -E`. The bare minimum of tools you will
need to run the Arduino.mk makefile:

	dash make
	awk cat cut expr grep head ls mkdir sed tr uname which




#### stm8flash

A precompiled windows binary was in the stm8flash repository for a while but
got removed at one point. I used an
[old version](https://github.com/vdudouyt/stm8flash/raw/39b1a9ec1dd26030065c3e476fc3b7b89626e21d/stm8flash.exe)
of the file. You might prefer to compile your own version from the
[stm8flash repository](https://github.com/vdudouyt/stm8flash) using either
MinGW or cygwin. For compiling, you will need the libusb windows binary:
http://libusb.info/ (I used the MinGW32 dll)




### General problems using Windows

It works, but using the Arduino.mk makefile with Windows is slow. **Very**
slow. Painfully slow. Compiling-the-Blink-example-takes-about-40-seconds
kind of slow. Yes, seriously. No kidding. 40 seconds. Measured on a 3GHz
machine with 4GB RAM.

There is no easy fix, the underlying problem is a fundamental one. It is not
about the compilation itself, it is the way Makefiles are written and
executed. The whole concept relies on forking subprocesses for all the shell
calls. Unfortunately, there is nothing like a fork in Windows and to work
around that is painfully slow.

It would be great if somebody could manage to modify the original Arduino
build system to support non-C++-builds. Or to integrate it somehow into the
STVD IDE. Or replace the painfully slow parameter checking part of the
makefile (that causes the majority of forking) by a single shell script that
gets called by the makefile and delivers the results in no time. Or use
[cmake](www.cmake.org). Or whatever.

Until than the least annoying way out might be using a virtual machine
running a simple Linux system. Ubuntu Mate or a basic Debian install for
example. Virtual Box is great for this purpose and freely available.



## Possible improvements

### All-inclusive packages

One of the big convinience points of the Arduino project are the
easy-to-install all-inclusive packages without any external dependencies.
They don't require any prior knowledge to set up a full cross-compiler tool
chain.

Something like that would be very nice for the STM8 as well, but I didn't go
that far. You still have to do some work yourself. You definitly need the
compiler SDCC. `stm8flash` is a simple tool to transfer the compiled program
to the CPU, but any other flash tool supporting the SWIM protocol will do.
The build process is controlled by `make`. It is a standard tool with any
UNIX system but available for windows as well.

Alternative ways of controlling the build process exist, but then you are
on your own to set it up.
