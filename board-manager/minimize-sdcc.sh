#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Minimize a SDCC snapshot build by leaving out all non-STM8 files."
	echo
	echo "usage: $0 sdcc-snapshot-filename"
	exit 1
fi


#FILE=~/Downloads/sdcc-snapshot-amd64-unknown-linux2.5-20170919-9998.tar.bz2
FILE=$1

case "$FILE" in
	*.tar.bz2 | *.tbz)
		TARFLAG=j
		;;
	*.tar.gz | *.tgz)
		TARFLAG=z
esac

# set to v for verbose mode
VERBOSE=

NAME=$(basename "$FILE")
NAME=${NAME/snapshot/stm8}

TMP=$(mktemp -d sdcc-repack-XXXXXX --tmpdir)

echo "Unpacking into temp. directory $TMP..."
tar x${VERBOSE}${TARFLAG}f "$FILE" -C "$TMP"					\
--exclude=doc		--exclude=src 		--exclude=non-free	\
--exclude=stlcs		--exclude=ds80c390.h				\
"--exclude=pic*"							\
"--exclude=*00"		"--exclude=*08"		"--exclude=*2k"		\
"--exclude=*51"		"--exclude=*80"		"--exclude=*90"		\
"--exclude=*gb"		"--exclude=*ka"		"--exclude=*.info"	\
"--exclude=large*"	"--exclude=medium"	"--exclude=small*"

echo "Repacking into file $NAME"
tar c${VERBOSE}${TARFLAG}f "$NAME" -C "$TMP" sdcc

echo "cleaning up temporary files"
rm -rf "$TMP"

echo "done."
