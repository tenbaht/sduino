#!/bin/dash

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


# echo the full command line in cyan:
>&2 echo "${CYAN}${@}${OFF}"

# echo the mark id in green and the compiler call in white:
SDAR=$1
LIB=$2
OBJ=${3%.o}.rel
MARK=$4
shift 4

>&2 echo "${GREEN}Mark $MARK:${OFF}" "$SDAR" "$@" "$LIB" "$OBJ"
"$SDAR" "$@" "$LIB" "$OBJ"
cp -a "$LIB" "${LIB%.a}.lib"
