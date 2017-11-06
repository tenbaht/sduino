#!/bin/bash

# usage: sdcc-link [.lib and .rel files] re5 [other flags and files]

VERBOSE=0
USE_COLOR=0
if [ "$1" == "-v" ]; then
	VERBOSE=1;
	shift
elif [ "$1" == "-vv" ]; then
	VERBOSE=2
#	USE_COLOR=1
	set -x
	shift
fi


if [ $USE_COLOR -gt 0 ]; then
# ANSI color codes to beautify the output:
BLACK='\033[0;30m'
RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
LGRAY='\033[0;37m'
DGRAY='\033[1;30m'
LRED='\033[1;31m'
LGREEN='\033[1;32m'
YELLOW='\033[1;33m'
LBLUE='\033[1;34m'
LPURPLE='\033[1;35m'
LCYAN='\033[1;36m'
WHITE='\033[1;37m'
OFF='\033[0m'
fi

SDCC="$1"
shift

if [ $VERBOSE ]; then
	# echo the full command line in cyan:
	>&2 echo -ne "${CYAN}"
	>&2 echo -n "${@}"
	>&2 echo -e "${OFF}"
fi

declare -a OBJS
while [ $# -gt 0 ]; do
	echo $1
	FILE=$1
	if [[ "$FILE" == *.a ]]; then
		FILE=${FILE%.a}.lib
		cp -a "$1" "$FILE"
	fi
	if [[ "$FILE" == *.o ]]; then
		FILE=${FILE%.o}.rel
	fi
	OBJS+=("${FILE}")
	shift
done

if [ $VERBOSE ]; then
	echo -ne "cmd: ${ORANGE}"
	echo -n "$SDCC" "${OBJS[@]}"
	echo -e "${OFF}"
fi
"$SDCC" "${OBJS[@]}"

# propagate the sdcc exit code
exit $?
