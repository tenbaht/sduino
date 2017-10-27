#!/bin/bash

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

for i in parts/platform*; do
	sed -ie "/\"platforms\" : \[/r $i" ${PACKAGEFILE};
done;

for i in parts/tools*; do
	sed -ie "/\"tools\" : \[/r $i" ${PACKAGEFILE};
done;

# The template contains an empty {} as a 'end of list' marker. We need to
# remove the trailing comma after the last platform and tools entry and the
# marker. This is asking for some sed magic:
#
# compact: sed -e '/},/{N;s/},\s*{}/}/}' package_sduino_stm8_index.json
# (slightly) more readable:
sed -ie '/},/{
	N;
	s/},\s*{}/}/
}' package_sduino_stm8_index.json
