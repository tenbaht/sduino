#!/bin/sh
# works on bash, dash or busybox ash

print_usage ()
{
	cat << EOF
Print the memory usage of a binary compiled by SDCC

usage: $0 mapfile

The display format mimics the output of the size tool. The values are
calculated from the segment definitions in the map file as follows:

text (code in flash)
	- CODE: program code
	- GSINIT: code to copy the globaly initialized data
	- GSFINAL: code after the initialization is done

data (initialized data in flash)
	- HOME: Interrupt vectors etc.
	- CONST: Constants (tables)
	- INITIALIZER: Content of initialized variables (to be copied to
	  INITIALIZED)

bss (uninitialized, zeroed and initialized, total RAM usage)
	- DATA: uninitialized
	- INITIALIZED: initialized, copied from INITIALIZER

dec (total flash usage in decimal)
	- text+data

hex (total flash usage in hex)
	- text+data

filename
	- the filename of the map file

LGPL-2.1, (c) 2018 M. Mayer
EOF
	exit 1
}


VERBOSE=0

while getopts ":hv" opt; do
	case "$opt" in
		v)
			echo "verbose!"
			VERBOSE=1
			;;
		*)
			print_usage
			;;
	esac
done
shift $((OPTIND-1))

if [ $# -lt 1 ]; then
	print_usage;
fi

FILENAME="$1"

awk -v filename="$FILENAME" -v verbose=$VERBOSE '
/^[A-Z]+   / {
	if (verbose) print $1, $3, $5; size[$1]=$5;
}
END	{
	text = 	size["CODE"]+size["GSINIT"]+size["GSFINAL"];
	data =	size["HOME"]+size["CONST"]+size["INITIALIZER"];
	bss  =	size["DATA"]+size["INITIALIZED"];
	print "   text\t   data\t    bss\t    dec\t    hex\tfilename";
#	print	text, data, bss;
	printf "%7d\t%7d\t%7d\t%7d\t%7x\t%s\n", text, data, bss, text+data, text+data, filename;
}' "$FILENAME"
