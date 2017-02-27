#!/bin/bash


if [ $# -ne 1 ]; then
	cat << EOF
split the library into individual source files for every single function,
compile them seperately and pack them all together into one big library.

usage: $0 cputype

supported cpu types as found in inc/stm8s.h:
EOF
	grep defined inc/stm8s.h|fmt -1|sed -n 's,.*(STM8,\tSTM8,p'|sed 's,).*,,'|sort|uniq
	cat << EOF

These cpus are known to use identical libraries:
	Low density devices:              STM8S003, STM8S103
	Medium density devices:           STM8S005, STM8S105, STM8AF626x
	High density devices without CAN: STM8S007, STM8S207, STM8AF62Ax
	High density devices with CAN:    STM8S208, STM8AF52Ax
	App. specific low density devices:STM8S903
	                                  STM8AF622x
EOF
	exit 1
fi

CPU=$1

HFILES=$(sdcc -mstm8 -Iinc -Isrc -D$CPU "-Wp-MM" -E inc/stm8s.h|fmt -1|sed -n 's, *inc/stm8s_,stm8s_,p')
CFILES=${HFILES//.h/.c}

echo "Needed source code modules for CPU $CPU:"
echo $CFILES
