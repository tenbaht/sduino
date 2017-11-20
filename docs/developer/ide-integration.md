# IDE integration

The IDE uses arduino-builder as a build tool. This tool is tightly build
around the gcc. It always assumes that the main sketch is a cpp file and it
includes hardcoded assumptions about the file suffixes '.o' for object files
and '.a' for libraries.

As SDCC is not flexible about file suffix' as well we need some wrapper
scripts to convert the different suffixes before and after the compiler run:

file type	|gcc file suffix	|SDCC file suffix
-----------	|-------------		|-----------
source file	|any			|.c
object file	|.o			|.rel
library archive	|.a			|.lib
dependency file	|.d			|.d


## Linux and MacOS

I like minimal approaches and usually prefer using dash over bash. Less
features => less problems. You know.

The linker wrapper sdcc-link.sh is tricky, as we need to parse all
arguments, modify them if an object file or a library is found and call sdcc
with the full list. The required array handling is not implemented in dash,
so we are stuck with the full bash.


## bash on Windows

Using the bash.exe from the MinGW project we can use the same script files
even for Windows.


### Figuring out the path

The hard part is locating the executeables without requiring the user to
modify the PATH variable. Relative to the location of the wrapper scripts
the executeables for `cp` and `rm` are in `../win`. But a simple simple `cd
../win` or `../win/cp` does't work, as the current working directory at this
point is still the Arduino binary directory.

Since the script is called with a full absolute path, we can extract the
needed path from there. This is the obvious solution:

	PATH="${0%/wrapper/*}"/win:$PATH

It looks ok, but it doesn't work on some Windows systems. It will result
in an absolute path like
`C:\Users\michael\AppData\Local\Arduino15\packages\sduino\tools\STM8Tools/win`.
On some systems this works, on some it doesn't. Surprisingly, the mingw
system somehow decides to set the `tools/STM8Tools` directory as the root
directory. On some systems absolute paths above that point are ok, on some
systems they are not. No idea why. And no idea where this root base is
defined (or how to influence it). How can the bash.exe know that it was
extracted from a tar file below the `tools/STM8Tools` directory?

So we need a plan B.

This is technically wrong, but surprisingly it works with Windows:

	cd $0/../..
	PATH=$(pwd)/win:$PATH

The `pwd` has the positive side effect of converting the path from Windows
to Unix syntax avoiding all these backslash issues.

Finally, this is syntactally correct and works on all systems:

	cd "${0%/wrapper/*}"
	PATH=$(pwd)/win:$PATH

All together it leds to this solution:

```bash
# check if cp is in the path
if ! command -v cp > /dev/null; then
	cd "${0%/wrapper/*}"
	PATH=$(pwd)/win:$PATH
fi
```

Now we are ready to go even on Windows. All used functions `command`, `cd`
and `pwd` and the pattern matching are POSIX-conformant builtin shell
functions without any external dependencies resulting in almost no overhead
and high portability.


### Using only cmd.exe

It might be possible to get away with straight cmd.exe batch programming.
But my ambitions of getting into Windows programming are very limited, so
maybe somebody else would like to investigate this.
