#!/bin/bash

# helper script for automatic releases.
#
# Prints one platform entry for the given file to be added to the
# package_*.json file.


if [ $# -ne 2 ]; then
	echo "
helper script for automatic releases.

Prints a full tool entry for the given filename stem to be added to the
package_*_index.json file. It will include all OS variants found with the
same filename stem and the same version specifier.

usage: $0 toolfile-stem version

Lists info for all files matching the filename pattern
[toolsfile-stem]*[version]*

Example: $0 release/Sduino-tools 2017-10-21 prints information for all files
matching the filename pattern release/Sduino-tools*2017-10-21*.
"
	exit 1
fi

TRUNK=$1
VERSION=$2


### helper functions #####################################################

# format ID information for a file
print_filedata()
{
	URL=file://$(realpath $1)
	FILENAME=$(basename "$1")
	SIZE=$(stat --printf="%s" $1)
	CHKSUM=$(shasum -a 256 $1|cut "-d " -f1)
cat << EOF
	"url": "$URL",
	"archiveFileName": "$FILENAME",
	"checksum": "SHA-256:$CHKSUM",
	"size": "$SIZE"
EOF
}


# detect the host system type for the given file
detect_hosttype()
{
	case $1 in
		*amd64-unknown-linux* | *linux64* )
			HOST="x86_64-pc-linux-gnu"
			;;
		*mingw32*)
			HOST="i686-mingw32"
			;;
		*i386-unknown-linux* | *linux32* )
			HOST="i686-pc-linux-gnu"
			;;
		*macosx*)
			HOST="x86_64-apple-darwin"
			;;
	esac
}


# detect the tool type/name for the given file
detect_tooltype()
{
	case $1 in
		*Sduino-tools* )
			NAME=STM8Tools
			;;
		*sdcc* )
			NAME=sdcc
			;;
	esac
}



### print a tool entry for the given file ############################



detect_tooltype "$TRUNK"
cat << EOF
                {
                    "name": "$NAME",
                    "version": "$VERSION",
                    "systems": [
                        {
EOF
n=0
for FILE in $TRUNK*$VERSION*; do
	if [ $n -gt 0 ]; then
	echo "                        },{"
	fi
	detect_hosttype "$FILE"
	echo "                            \"host\": \"$HOST\","
	print_filedata "$FILE"
	n=$((n+1))
done
cat << EOF
                        }
                    ]
                },
EOF
