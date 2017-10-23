#!/bin/bash

# usage: sdcc-link [.lib and .rel files] re5 [other flags and files]

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

SDCC="$1"
shift

# echo the full command line in cyan:
>&2 echo -e "${CYAN}${@}${OFF}"

declare -a OBJS
while [ $# -gt 0 ]; do
	echo $1
	FILE=$1
	if [[ $FILE == *.a ]]; then
		FILE=${FILE%.a}.lib
		cp -a "$1" "$FILE"
	fi
	if [[ $FILE == *.o ]]; then
		FILE=${FILE%.o}.rel
	fi
	OBJS+=(${FILE})
	shift
done
echo "$SDCC" "${OBJS[@]}"
"$SDCC" "${OBJS[@]}"

# propagate the sdcc exit code
exit $?
