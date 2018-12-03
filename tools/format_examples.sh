#!/bin/dash

# a simple script to convert the example sketches into valid *.md listings
# ready to be used in the documentation.

for i in *; do
	(echo "# $i.ino"; echo '```c'; cat $i/$i.ino; echo '```') > $i.md;
done
