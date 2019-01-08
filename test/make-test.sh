#!/bin/bash

CURDIR=$(pwd)
SRCREV=$(git describe --tags --match="v*")
LOGFILE=${CURDIR}/logs/test-${SRCREV}.log
BASEDIR=../sduino/hardware/sduino/stm8/libraries/

# count the number of CPUs and use them for parallel compiling
THREADS=$(grep processor  /proc/cpuinfo |wc -l)

# make sure $COLUMNS is set
shopt -s checkwinsize

print_status() {
	local status
	if [ $1 -eq 0 ]; then
		status="\e[32m ok \e[0m"
	else
		status="\e[31mfail\e[0m"
	fi

	# Get the number of columns, but subtact 8 to leave space for the status.
	local columns=$((COLUMNS-8))
	echo -e "\r\e[${columns}C[${status}]"
}


(
	echo "compile log for all tests."
	echo "source revision: $SRCREV"
) | tee "$LOGFILE"

# removing *all* old build directories for a clear start
find "$BASEDIR" -name "build-*" -type d|xargs rm -r

#find "$BASEDIR" -name Makefile|sort|while read line; do
for line in $(find "$BASEDIR" -name Makefile|sort); do
	dir=$(dirname $line)
	shortdir=${dir#$BASEDIR}
	echo -n "$shortdir"
	cd $dir
	make -j$THREADS > build-test.log 2>&1
	status=$?
#	echo $status
	print_status $status
	echo "$status	$shortdir" >> "$LOGFILE"
	if [ $status -eq 0 ]; then
		sdsize.sh build-*/*.map|tail -1 >> "$LOGFILE"
	fi
	cd - > /dev/null
done


# output some statistics
(
# sum up the total memory usage
	TOTAL=$(awk '/^ / {
			text+=$1; data+=$2; bss+=$3
		}
		END {
			printf "total text/data/bss/flash (text+data):\t%i\t%i\t%i\t%i\n",text,data,bss,text+data
		}' "$LOGFILE")
	echo $TOTAL

	echo "Failed tests:"
	grep "^[1-9]" "$LOGFILE"
) | tee -a "$LOGFILE"
