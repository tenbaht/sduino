#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Minimize a SDCC snapshot build by leaving out all non-STM8 files."
	echo
	echo "usage: $0 sdcc-snapshot-filename"
	exit 1
fi


#FILE=~/Downloads/sdcc-snapshot-amd64-unknown-linux2.5-20170919-9998.tar.bz2
FILE=$(realpath $1)

case "$FILE" in
	*.tar.bz2 | *.tbz)
		TARFLAG=j
		;;
	*.tar.gz | *.tgz)
		TARFLAG=z
		;;
	*.zip)
		TARFLAG=zip
		;;
esac

# set to v for verbose mode
VERBOSE=

# transform the filename from "*-snapshot-*" to "*-stm8-*"
NAME=$(basename "$FILE")
NAME=${NAME/snapshot/stm8}

# patterns to exclude all unneeded files from unpacking
TAR_EXCLUDE='--exclude=doc	--exclude=src 	--exclude=non-free
--exclude=stlcs		--exclude=ds80c390.h	--exclude=pic*
--exclude=*00		--exclude=*08		--exclude=*2k
--exclude=*51		--exclude=*80		--exclude=*90
--exclude=*gb		--exclude=*ka		--exclude=*.info
--exclude=large*	--exclude=medium	--exclude=small*'

ZIP_EXCLUDE='-x */non-free/* */src/* *stlcs* */pic*
*00* *08* *80* *90* *51* *2k* *gb* *ka* */info/*
*/large* */medium/* */small*'

TMP=$(mktemp -d sdcc-repack-XXXXXX --tmpdir)

echo "Unpacking into $TMP..."
if [ $TARFLAG == zip ]; then
#	unzip "$FILE" -d "$TMP" $ZIP_EXCLUDE
	cd "$TMP"
	unzip "$FILE" $ZIP_EXCLUDE
	cd -
else
	tar x${VERBOSE}${TARFLAG}f "$FILE" -C "$TMP" $TAR_EXCLUDE
fi

echo "Repacking into file $NAME"
tar c${VERBOSE}${TARFLAG}f "$NAME" -C "$TMP" sdcc

echo "cleaning up temporary files"
rm -rf "$TMP"

echo "done."
