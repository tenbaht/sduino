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


# parse the script options
#
# This is very crude. The options are allowed only as the very first argments
# and they are required to be used in exactly this order.

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


# define color codes

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
#
# This is all pure POSIX, it works for bash, dash and busybox ash
vprint 2 "copy *.a to *.lib"
for FILE; do
	vprint 2 "- checking parameter '$FILE'"
	if expr "$FILE" : ".*\.a$"; then	# bash, dash, busybox ash
		NEW=${FILE%.a}.lib
		vprint 1 "copy $FILE to $NEW"
		cp -a "$FILE" "$NEW"
	fi
done


# replace *.o with *.rel and *.a with *.lib
#
# On bash this is a simple pattern substituiton:
# set -- "${@/.o/.rel}"
# set -- "${@/.a/.lib}"
#
# Unfortunatly, this does not work with dash or ash. dash does not support
# pattern substituition in general. busybox ash does not support arrays and
# shortens the arg list to the first argument, deleting all the rest.
#
# As a workaround we combine the argument list into a single string. By
# using TAB as a field separator we can even deal with spaces, backspaces
# and colons in file names.

# use tab as field separator
IFS=$'\t'


# combine all arguments into a single string with field separator and add a
# TAB at the end. This allows the pattern below to match the last argument
# as well.
vprint 2 "Combine all arguments into a single string"
line="$*	"

# do the filename replacements: (bash and ash, not dash)
# Needs a double slash to replace all occurencies, not only the first one.
#line=${line//.o/.rel}
#line=${line//.a/.lib}
# replace all the occurencies just before a field separator
vprint 2 "- before substitution: $line"
line="${line//.o	/.rel	}"
line="${line//.a	/.lib	}"
vprint 2 "- after substitution: $line"

vprint 1 "cmd: ${ORANGE}$SDCC $line${OFF}"
"$SDCC" $line

# propagate the sdcc exit code
exit $?
