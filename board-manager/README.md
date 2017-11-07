# Automatic update of the package_index.json file for release management

Tools for a simple release management system.

Mimimizing the amount of manual work needed for generating the board manager
release files hopefully minimizes the number of errors and allows for
frequent releases.


## Quick checklist for release

- Update the version number for the updated components in `Makefile`.
- `make`
- `make upload`: test on local test server
- Check Changelog. No need to update the version number.
- `make release`: Update version number in changelog, commit generated
  files, tag the new release.
- release message on github and attach the files in release/<VERSION>/



## How to prepare a release

The main `Makefile` defines the version numbers for the core, sdcc and the
tools. Update the version number for the component you would like to
re-generate and run `make`.

It is helpful to really edit the Makefile and save the new version numbers
to the repository on every release. This will make sure that all
dependencies are up-to-date the next time and will prevent the system from
using references to old versions.

After testing, check in the newly generated files and update the Changelog:

	make release


For testing purposes only (!!), it is possible to use the simpler systax
using makefile variables on the command line:

	make COREVERSION=0.3.2 core

will generate a fresh archive for the core files and update the package file
accordingly.


## Supporting tools

`assemble.sh`: Insert all objects from parts/ directory in the platform and
tools field of the json template file to assemble the final package file.

`gen_platform_entry.sh`: Generate a platform entry for a core archive.

`gen_tools_entry.sh`: Generate a tools entry for a sdcc or STM8Tools
archive.

Makefile.core
Makefile.sdcc
Makefile.tools

`minimize-sdcc.sh`: Repack a [SDCC snapshot
build](http://sdcc.sourceforge.net/snap.php) into a minimal compiler package
by removing all non-STM8 files. The total unpacked size shrinks from 250MB
to 6MB.

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
