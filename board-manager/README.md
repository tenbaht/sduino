# Automatic update of the package_index.json file for release management

`assemble.sh`: Insert all objects from parts/ directory in the platform and
tools field of the json template file to assemble the final package file.

`gen_platform_entry.sh`: Generate a platform entry for a core archive.

`gen_tools_entry.sh`: Generate a tools entry for a sdcc or STM8Tools
archive.

Makefile.core
Makefile.sdcc
Makefile.tools

`minimize-sdcc.sh`: Repack a [SDCC snapshot build]
(http://sdcc.sourceforge.net/snap.php) into a minimal compiler package by
removing all non-STM8 files. The total unpacked size shrinks from 250MB to
6MB.

`package_sduino_stm8_index.json`: Generated package index file.

`package_template.json`: Empty template for generating the package index
file.

`README.md`: This file.

`parts/`: Contains all platform and tools entries that should go into the
final package index file.

`release/`: The generated archive files for the release.



## Package URLs

If using attached files to github releases:

URLs of the auto-generated repository archive files: 

- https://github.com/tenbaht/sduino/archive/<tagname>.zip
- https://github.com/tenbaht/sduino/archive/<tagname>.tar.gz

URL of files attached to github releases:

https://github.com/tenbaht/sduino/releases/download/<tagname>/<filename>
