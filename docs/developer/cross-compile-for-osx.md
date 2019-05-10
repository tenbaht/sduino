# Cross-Compile for OSX

Compiling the tools is really easy. Only setting up the cross compilation
environment requires a little more effort. But luckily, the
[osxcross](https://github.com/tpoechtrager/osxcross) project already took
care of this.


## Setting up the cross compilation environment on Linux Mint 19

### Install the needed dependencies

I needed only these (the others were already present):

	sudo apt install clang llvm-dev uuid-dev libssl-dev libbz2-dev

Full list of all dependencies:

	sudo apt install clang llvm-dev libxml2-dev uuid-dev libssl-dev bash patch make  tar xz-utils bzip2 gzip sed cpio libbz2-dev


### Get the MacOSX SDK

#### The official way

Extract the needed files from the official SDKs Xcode 7.3 from the Apple
Developer website using the scripts that come with osxcross:

	sudo apt install cmake libxml2-dev fuse libfuse-dev
	./tools/gen_sdk_package_darling_dmg.sh path/to/SDK-file

On Ubuntu-based distributions with a name not containing the string "ubuntu"
(like Linux Mint) you will get an error message "Required kernel module
'fuse' not loaded". To fix this, "echo out" the line `modinfo fuse
&>/dev/null` in tools/gen_sdk_package_darling_dmg.sh by changing it to `echo
modinfo fuse &>/dev/null` before running it (don't delete it or comment it
out).


```diff
diff --git a/tools/gen_sdk_package_darling_dmg.sh b/tools/gen_sdk_package_darling_dmg.sh
index 8cd23e5..b271450 100755
--- a/tools/gen_sdk_package_darling_dmg.sh
+++ b/tools/gen_sdk_package_darling_dmg.sh
@@ -40,7 +40,7 @@ command -v lsb_release 2>&1 > /dev/null
 if [[ $? -eq 0 ]] && [[ -n $(lsb_release -a 2>&1 | grep -i ubuntu) ]]; then
   echo "Using ubuntu, skipping fuse module check"
 else
-  modinfo fuse &>/dev/null
+  echo modinfo fuse &>/dev/null
 fi
 
 if [ $? -ne 0 ]; then
```

Now move the resulting file `MacOSX10.11.sdk.tar.xz` into the tarball directory.



#### The easy way

Somebody was kind enough to set up a repository with all the [MacOSX
SDKs](https://github.com/phracker/MacOSX-SDKs). This saves you the trouble
of downloading multiple gigabytes of data and running the extraction script.

Download the file
[MacOSX10.11.sdk.tar.xz](https://github.com/phracker/MacOSX-SDKs/releases/download/10.13/MacOSX10.11.sdk.tar.xz)
into the tarball directory.





### Build OSXcross:

	./build.sh
	sudo mv target /opt/osxcross

Add `/opt/osxcross/bin` to your `$PATH` and prepare macports:

	export MACOSX_DEPLOYMENT_TARGET=10.7
	export PATH=$PATH:/opt/osxcross/bin
	osxcross-macports update-cache



## Build stm8flash

	osxcross-macports install libusb-devel
	make CC=o64-clang CXX=o64-clang++ PKGCONFIG=x86_64-apple-darwin15-pkg-config RELEASE=yes

Or with full path if you didn't add the osxcross binary directory do your
PATH:

	make CC=/opt/osxcross/bin/o64-clang CXX=/opt/osxcross/bin/o64-clang++ PKGCONFIG=/opt/osxcross/bin/x86_64-apple-darwin15-pkg-config RELEASE=yes


## Build stm8gal

	make CC=o64-clang

That's all!
