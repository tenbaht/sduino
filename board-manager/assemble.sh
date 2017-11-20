#!/bin/bash

# This is a very basic template "engine" to generate a full platform index
# file.


if [ $# -ne 1 ]; then
	echo "
usage: $0 packagefile

Insert all objects from parts/ directory in the platform and tools field
of the json template file to assemble the final package file.
"
	exit 1
fi

PACKAGEFILE=$1

cp -a package_template.json ${PACKAGEFILE} 

for i in parts/platform*.txt; do
	sed -ie "/\"platforms\" : \[/r $i" ${PACKAGEFILE};
done;

for i in parts/tools*.txt; do
	sed -ie "/\"tools\" : \[/r $i" ${PACKAGEFILE};
done;

# The template contains an empty {} as a 'end of list' marker. We need to
# remove this marker and the comma right before it. This can be done by some
# sed magic:
#
# compact: sed -e '/},/{N;s/},\s*{}/}/}' package_sduino_stm8_index.json
# (slightly) more readable:
sed -ie '/},/{
	N;
	s/},\s*{}/}/
}' ${PACKAGEFILE}
