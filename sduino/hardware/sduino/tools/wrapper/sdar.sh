#!/bin/bash

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

# check if cp is in the path, add our binaries to the PATH if needed
cp --version >/dev/null 2>&1 || PATH=${0%/wrapper/*}/win:$PATH


# echo the full command line in cyan:
>&2 echo -ne "${CYAN}"
>&2 echo -n "${@}"
>&2 echo -e "${OFF}"

# echo the mark id in green and the compiler call in white:
SDAR=$1
LIB=$2
OBJ=${3%.o}.rel
MARK=$4
shift 4

>&2 echo -ne "${GREEN}Mark $MARK:${OFF}"
>&2 echo "$SDAR" "$@" "$LIB" "$OBJ"

"$SDAR" "$@" "$LIB" "$OBJ"
ERR=$?
cp -a "$LIB" "${LIB%.a}.lib"

# propagate the sdar exit code
exit $ERR
