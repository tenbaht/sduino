#!/bin/dash

# wrapper around SDCC to let the external interface look more gcc-alike
#
# Differences:
# - if a .cpp files is given as input temporarly copy it to .c and compile it
#   as a c file. This will break the dependency check, as it expects the the
#   full original filename, but as this happens only for the original .ino
#   file it is not a big loss.
# - generate .rel files, but copy them as .o files as well to satisfy the
#   dependency checker on following builds


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
SDCC=$1
SRC=$2
OBJ=$3
REL=${OBJ%.o}.rel
MARK=$4
shift 4
>&2 echo "${GREEN}Mark $MARK:${OFF}" "$SDCC" "$@" "$SRC" -o "$OBJ"

case "$SRC" in
	*.cpp)
		# rename .cpp to .c and compile
		>&2 echo "${RED}cpp gefunden${OFF}";
		CSRC="${SRC%pp}"
		cp -av "$SRC" "$CSRC"
		"$SDCC" "$@" "$CSRC" -o "$OBJ"
		ERR=$?
		rm -f "$CSRC"
		;;
	*.c)
		# compile a .c file
		"$SDCC" "$@" "$SRC" -o "$OBJ"
		ERR=$?
		;;
esac

# copy the generated .rel files as an .o file to avoid recompiling the next time
if [ -e "${REL}" ]; then
	cp -a "${REL}" "${OBJ}"
fi

# propagate the sdcc exit code
exit $ERR
