#!/bin/bash

# usage: sdcc-link [options] [.lib and .rel files] re5 [other flags and files]
#
# possible script options (options in this order only):
# -v:	verbose
# -d:	debug mode (more verbose), includes coloring the output
# -c:	color the output


### local functions ######################################################

# verbose print
#
# usage: vprint min_level msg
#
# prints a message if the verbosity level is equal or higher than the required
# min_level
#
vprint ()
{
	local level=$1

	shift
	if [ $VERBOSE -ge $level ]; then
		echo -e "$@"
	fi
}


VERBOSE=0
USE_COLOR=0
if [ "$1" == "-v" ]; then
	VERBOSE=1;
	shift
elif [ "$1" == "-d" ]; then
	VERBOSE=2
	USE_COLOR=1
	set -x
	shift
fi
if [ "$1" == "-c" ]; then
	USE_COLOR=1;
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

# check if cp is in the path using 'command -v' (a builtin POSIX function)
if ! command -v cp > /dev/null; then
	# Ok, this means we are on a Windows system and we have to find a
	# way to access cp and rm in ../win. A simple 'cd ../win' or
	# '../win/cp' does't work, as the current working directory is still
	# the Arduino binary directory.
	#
	# This looks ok, but it doesn't work on some Windows systems:
	# (No idea why)
	# PATH="${0%/wrapper/*}"/win:$PATH
	#
	# This is technically wrong, but surprisingly it works with Windows:
	# cd $0/../..
	# PATH=$(pwd)/win:$PATH
	#
	# Use cd/pwd as a replacement for 'realpath' using only builtins.
	# It has the positive side effect of converting from Windows to Unix
	# path syntax avoiding all these backslash issues.
	cd "${0%/wrapper/*}"
	PATH=$(pwd)/win:$PATH
fi

SDCC="$1"
shift

# echo the full command line in cyan on stderr:
>&2 vprint 1 "${CYAN}${@}${OFF}"


# The arduino system insists on a *.a file for a library, but sdar requires
# them to be named *.lib.
#
# Workaround: copy all *.lib files into *.a files.
#
# Iterate over all positional parameters with a for loop.
# The pattern match for filename is easy for bash and dash, but busybox ash
# requires the use of the 'expr' command:
#
# bash, dash: if [[ "$FILE" == *.a ]]; then
# ash uses 'expr': expr "$FILE" : ".*\.a$"; then

declare -a OBJS
while [ $# -gt 0 ]; do
	echo $1
	FILE=$1
#	if [[ "$FILE" == *.a ]]; then		# easy, but bash and dash only
	if expr "$FILE" : ".*\.a$"; then	# bash, dash, busybox ash
		FILE=${FILE%.a}.lib
		cp -a "$1" "$FILE"
	fi
	if [[ "$FILE" == *.o ]]; then
		FILE=${FILE%.o}.rel
	fi
	OBJS+=("${FILE}")
	shift
done

vprint 1 "cmd: ${ORANGE}$SDCC $line${OFF}"
"$SDCC" "${OBJS[@]}"

# propagate the sdcc exit code
exit $?
