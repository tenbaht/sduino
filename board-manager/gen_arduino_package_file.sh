#!/bin/bash

PACKAGE_NAME=Sduino-test

packages_begin() {
cat << EOF
{
    "packages": [
        {
            "name": "$PACKAGE_NAME",
            "maintainer": "Michael Mayer",
            "websiteURL": "https://tenbaht.github.io/sduino/",
            "email": "michael-mayer@gmx.de",
            "help": {
                "online": "http://www.stm32duino.com/viewforum.php?f=52"
            },
EOF
}

packages_end() {
cat << EOF
        }
    ]
}
EOF
}



platforms_begin() {
cat << EOF
            "platforms" : [
                {
                    "name": "Board manager test core",
                    "architecture": "stm8",
                    "version": "0.3.0",
                    "category": "Contributed",
EOF
}

platforms_end() {
cat << EOF
                }
            ],
EOF
}




# list of supported boards
list_boards() {
	echo "                    \"boards\": ["
	n=0
	sed -n "s/.*\.name=//p" ../sduino/hardware/sduino/stm8/boards.txt |\
	while read line; do
		if [ $n -ne 0 ]; then 
			echo "                        },"
		fi
		echo "                        {"
		echo "                            \"name\": \"$line\""
	n=$((n+1))
	done
	echo "                        }"
	echo "                    ],"
}



print_filedata() {
	URL=file://$(realpath $1)
	FILENAME=$1
	SIZE=$(stat --printf="%s" $FILENAME)
	CHKSUM=$(shasum -a 256 $FILENAME|cut "-d " -f1)
cat << EOF
                    "url": "$URL",
                    "archiveFileName": "$FILENAME",
                    "checksum": "SHA-256:$CHKSUM",
                    "size": "$SIZE"
EOF
}


# tools section

list_tools() {
cat << EOF
            "tools" : [
                {
                    "name": "STM8Tools",
                    "version": "2017.10.20",
                    "systems": [
                        {
                            "host": "x86_64-pc-linux-gnu",
EOF
	print_filedata test-tools-ohne.tar.gz
cat << EOF
                        }
                    ]
                },
                {
                    "name": "sdcc",
                    "version": "build.9998",
                    "systems": [
                        {
                            "host": "x86_64-pc-linux-gnu",
EOF
	print_filedata sdcc-stm8-amd64-unknown-linux2.5-20170919-9998.tar.bz2
cat << EOF
                        }
                    ]
                }
            ]
EOF
}



print_footer() {
cat << EOF
            ]
        }
    ]
}
EOF
}

list_dependencies() {
cat << EOF
                    "toolsDependencies": [
                        {
                            "name": "STM8Tools",
                            "version": "2017.10.20",
                            "packager": "$PACKAGE_NAME"
                        },
                        {
                            "name": "sdcc",
                            "version": "build.9998",
                            "packager": "$PACKAGE_NAME"
                        }
                    ],
EOF
}


#  shasum -a 256 test-core.tar.gz

packages_begin

platforms_begin
list_boards
list_dependencies
print_filedata test-core.tar.gz
platforms_end

list_tools
packages_end
