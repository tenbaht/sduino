# Manual Installation

This is the more flexible, but more complicated way to install Sduino.
The recommended way of installation is now [using the Arduino Board
Manager](board-manager-install). [Updating to a manual
installation](update-to-manual) from there is not complicated.

The manual install is only useful if you are planning to use development or
experimental branches of Sduino or if you prefer a GUI-less build system
controlled by a Makefile.

Let's compare:

|		| Board Manager	|manual install
|----		|-----		|------
|Difficulty	|very easy	|easy on Linux and MacOS, more difficult on Windows.
|Used Packages	|latest release	|release, development or experimental
|IDE/GUI based builds|yes		|yes
|Makefile based builds|no		|yes
|Compiling without Arduino libraries|no	|yes





- Clone or download and extract the
  [repository](https://github.com/tenbaht/sduino) to a convenient location.
- Download the lastest [SDCC snapshot
  build](http://sdcc.sourceforge.net/snap.php) for your system. Support for
  the STM8 improves significantly from version to version. Do not use the
  older version of SDCC that might be included in your distribution (Version
  3.6 is definitely too old).

Later in the process you will unpack the SDCC archive. You only need support
for the stm8. You can safely delete all the files related to the PIC CPUs in
the `pic*` and `non-free` directories. That safes more than 90% of the used
disk space and leaves only 20MB out of 240MB.



## Linux

Link the hardware/sduino folder of the downloaded repository to your local
Arduino hardware folder (usually in `~/sketchbook` or `~/Arduino`). Create
the hardware folder, if not already present:

```bash
	cd ~/Arduino
	mkdir hardware
	cd hardware
	ln -s [the-extracted-repository]/sduino/hardware/sduino .
```

Extract the [downloaded SDCC archive](http://sdcc.sourceforge.net/snap.php)
under `/opt`:

```bash
	sudo mkdir /opt
	sudo tar xvjf ~/Downloads/sdcc-snapshot* -C /opt
```

Create a link to SDCC from the tools directory of the sduino repository
(only needed for IDE builds):

```bash
	cd [the-extracted-repository]/sduino/hardware/sduino/tools
	ln -s /opt/sdcc .
```

`stm8flash` and `stm8gal` are both included as precompiled binaries in this
tools directory. Create a link `linux` to the linux tools directory that
matches your system and copy the binaries to a convient place in your path,
e.g. `/usr/local/bin`:

```bash
	ln -s linux64 linux
	cp -av linux/* /usr/local/bin
```


`make` and libusb-1.0 (needed by stm8flash) are part of the standard
repository for any Linux distribution. Example for Debian-type systems (like
Ubuntu, Mint, Elementary etc.):

```bash
	sudo apt-get install make libusb-1.0-0
```

Finally, add a udev rule in order to access the USB port for your flash
tool. Save this as root in in `/etc/udev/rules.d/99-stlink.rules`:

```bash
	# ST-Link/V2 programming adapter

	# ST-Link V1, if using a STM8S discovery board
	# important: It needs a special entry in /etc/modprobe.d
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3744", MODE="0666", GROUP="plugdev"

	# ST-Link/V2, the china adapter with the green plastic housing
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev"
```

Using the STM8S-Discovery board as a flash programmer requires a [special
modprobe configuration
entry](../hardware/stm8sdiscovery#usage-with-linux-and-stm8flash).



## MacOS

Quite similar to the Linux install. [Install Homebrew](https://brew.sh/) if
you haven't done it yet, than install make and unpack the sdcc snapshot:

```bash
	brew make
	mkdir /opt
	tar xvjf sdcc-snapshot* -C /opt
```

Adopt the other steps from the Linux install. The arduino hardware folder is
in your Documents folder at `/Users/<username>/Documents/Arduino/hardware`.

Not sure about stm8flash, I didn't try it. Have a look at the [stm8flash
repository](https://github.com/vdudouyt/stm8flash). It would be great if
someone could summarize the procedure and send it to me together with a
compiled binary for the repository.



## Windows

There is more work to do here, as it is lacking all the tools for the build
system, a package system to install them and even a file system concept
where to store them.

Link (not copy!) the hardware/sduino folder of the downloaded repository to
your local Arduino hardware folder (in `Documents/Arduino/hardware`). Create
the hardware folder, if not already present.

Expected folder structure:

```bash
   Documents/
     + Arduino/
       + hardware/
         + sduino    (this is a link)
           + stm8/   core, variants, etc.
           + tools/
             + wrapper/
             + win/  stm8flash, libusb.dll, bash, ls, make etc.
             + sdcc  can be a link to a full sdcc install
```

>Don't despair of the stock Windows terminal window. Installing e.g.
[console2](https://sourceforge.net/projects/console/) will make your life on
the command line so much more enjoyable.



### The easy way to the command line

Unpack the latest board manager tools archive
[sduino-tools_mingw32*.tar.bz2](https://github.com/tenbaht/sduino/releases)
manually into the hardware/sduino/tools folder. As a workarount for
unpacking a tar.bz2 file (again, Windows is lacking even the most basic
tools...) you could use the board manager and copy the needed files out of
Appdata/Arduino/packages/sduino. But mind the different directory layout and
don't forget to remove the package again. Otherwise your are going to use
different versions of the sduino core for IDE and Makefile builds. Confusion
would be guaranteed!


Update your `%PATH%`:

- Add a link `c:\sdcc` to your SDCC installation directory and add this to
  your `%PATH%`. It is very important, that this name does not contain any
  spaces.
- Add the `sduino/tools/win` directory to your path or move/copy/link the
  files in there to a directory that is already in your path.

Open a terminal window and make sure that the binaries are found:

```bash
	$ ls
	$ sdcc -v
```

If you have installed [console2](https://sourceforge.net/projects/console/)
before, you could even configure it to use `bash` as your default command
shell instead of `cmd.exe`.





### The complete command line

Instead of using the stripped down convinience packages you could do a proper
MinGW, Msys, Msys2 or cygwin install. The result feels almost like a real
UNIX system.



#### MinGW or cygwin

[MinGW/MSys](http://www.mingw.org/wiki/MSYS) and
[cygwin](https://www.cygwin.com/) are both fine. cygwin aims to be an almost
complete POSIX environment (which is nice, but we don't need that function).
MinGW wants to be more compact and works with the native Windows API. That
is perfect for this purpose.

1. Check the
  [MinGW Installation Notes](http://www.mingw.org/wiki/Getting_Started)
2. Download
  [mingw-get-setup.exe](https://sourceforge.net/projects/mingw/files/Installer/)
3. Start it. You can safely deactivate the graphical option.
4. Add this at the end to your path: `;c:\mingw\bin;c:\mingw\msys\1.0\bin`
  (follow the instructions in "Environment Setting" on the [Installation
  Notes page](http://www.mingw.org/wiki/Getting_Started))
5. Open a command line and install the package msys-base by issuing this
  command: `mingw-get install msys-base`
6. Now `ls` or `make` should work.

For efficiency, the Makefile is configured to use dash instead of bash as a
shell. `egrep` is replaced by `grep -E`. The bare minimum of tools you will
need to run the Arduino.mk makefile:

```bash
	dash make
	awk cat cut expr grep head ls mkdir sed tr uname which
```

For IDE builds add bash to the list of required binaries and link them into
hardware/sduino/tools/win.



#### SDCC 

Unpack the downloaded SDCC archive into a location with an easy path without
spaces. `c:/sdcc` is fine or inside MinGW `/opt/sdcc` would be great. Add
`/opt/sdcc/bin` to your `$PATH`. 

For IDE builds link the sdcc directory into `hardware/sduino/tools`.




#### ST-Link, stm8flash

If not already done, install the ST-Link driver from the ST website for
hardware access.

You are not bound to `stm8flash`, you can use the regular flash tool by ST
if you prefer.

A recent precompiled binary for `stm8flash` is part of the Board Manager
tools archive file with the
[releases](https://github.com/tenbaht/sduino/releases). Or compile your own
from the [stm8flash repository](https://github.com/vdudouyt/stm8flash) using
either MinGW or cygwin. For compiling, you will need the
[libusb](http://libusb.info/) windows binary (I used the MinGW32 dll).


### Limitations

(Keep in mind the [Known problems using
Windows](limitations#known-problems-using-windows))
