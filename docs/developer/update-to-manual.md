# Update to Manual Installation

[Using the Arduino Board Manager](board-manager-install) is now the
recommended way of installation. Updating to a manual install is only useful
if you are planning to use development or experimental branches of Sduino or
if you outgrow the possibilities of the Arduino IDE and you are looking for
more control by using a proper Makefile controlled build process.

Let's compare:

|		| Board Manager	|manual install
|----		|-----		|------
|Difficulty	|very easy	|easy on Linux and MacOS, more difficult on Windows.
|Used Packages	|latest release	|release, development or experimental
|IDE/GUI based builds|yes		|yes
|Makefile based builds|no		|yes
|Compiling without Arduino libraries|no	|yes


## Ok, let's start.

Start by cloning or downloading and extracting the
[repository](https://github.com/tenbaht/sduino) to a convenient location.

Now we need to copy some files from the automatic install into the
hardware/sduino/tools folder of this repository to end with a folder
structure like this:

```bash
    + hardware/
      + sduino    (this is a link)
        + stm8/   core, variants, etc.
        + tools/
          + wrapper/
          + win/       stm8flash, libusb.dll, bash, ls, make etc.
          + linux      (link)
          + linux32/   stm8flash, stmgal
          + linux64/   stm8flash, stmgal
          + sdcc       can be a link to a full sdcc install
```

After copying you **MUST NOT FORGET** to use the Board Manager to remove the
auto-installed package. Otherwise your are going to use different versions
of the sduino core for IDE and Makefile builds. Confusion would be
guaranteed!



## Linux

Your local hardware folder is usually in `~/sketchbook` or `~/Arduino`.
Create the hardware folder, if not already present.

```bash
	cd ~/Arduino
	mkdir hardware
```

Link the hardware/sduino folder of the downloaded repository to your local
Arduino hardware folder:

```bash
	ln -s [the-extracted-repository]/sduino/hardware/sduino hardware
```

The needed files from the Board Manager install are in
`~/.arduino15/packages/sduino/tools`. We want to copy `sdcc` to `/opt` and the
content of `STM8STools` into `tools`:

```bash
	sudo mkdir /opt
	sudo cp -av ~/.arduino15/packages/sduino/tools/sdcc/<version> /opt/sdcc
	cp -av ~/.arduino15/packages/sduino/tools/STM8STools/<version>/* tools/
```

Alternatively, you could download the lastest [SDCC snapshot
  build](http://sdcc.sourceforge.net/snap.php) and extract that into
`/opt/sdcc` instead of copying the packaged version.

Create a link to SDCC from the tools directory of the sduino repository
(only needed for IDE builds):

```bash
	ln -s /opt/sdcc .
```

`stm8flash` and `stm8gal` are both included as precompiled binaries in this
tools directory. Copy them to a convient place in your path, e.g.
`/usr/local/bin`:

```bash
	sudo cp -av linux/* /usr/local/bin
```

`make` and libusb-1.0 (needed by stm8flash) are part of the standard
repository for any Linux distribution. Example for Debian-type systems (like
Ubuntu, Mint, Elementary etc.):

```bash
	sudo apt-get install make libusb-1.0-0
```

Now use the Board Manager to remove the auto-installed Sduino package and
you are ready.




## MacOS

Quite similar to the Linux install. [Install Homebrew](https://brew.sh/) if
you haven't done it yet, than install make and unpack the sdcc snapshot (if
not re-using the packaged version):

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

Don't despair of the stock Windows terminal window. Installing e.g.
[console2](https://sourceforge.net/projects/console/) will make your life on
the command line so much more enjoyable.


The needed Board Manager install files are in your user folder in
`AppData/Local/Arduino/packages/sduino/tools`. Copy `sdcc/<version>` and the
content of `STM8STools/<version` into `hardware/sduino/tools`. Mind the
slightly different folder structure.

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

Now use the Board Manager to remove the auto-installed Sduino package and
you are ready.

(Keep the [Known problems using
Windows](limitations#known-problems-using-windows) in mind)

