# Installing the needed tools

All tools are available for Linux, Windows and MacOS, most of them even for
Raspbian on a RaspberryPi.

D

## Linux


easy.

	apt-get install build-essentials
	mkdir /opt
	tar xvjf sdcc-snapshot* -C /opt

done.



## Windows

Harder, as we need to install some package management first.

You need SDCC,
make and git. SDCC from the project website, make and git are standard tools
included in either MinGW or cygwin. Both are fine, MinGW is smaller.

SDCC:

1. Download the lastes snapshot build from http://sdcc.sourceforge.net/snap.php
2. unzip into c:\\sduino

[MinGW](http://www.mingw.org/wiki/MSYS)

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


git:
https://git-for-windows.github.io/


