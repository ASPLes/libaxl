#!/bin/bash

# cleanup sources to avoid reporting generated content
make clean
rm -rf doc/html
rm -rf debian/libaxl-doc
rm -rf debian/libaxlbabel-dev
rm -rf debian/libaxl1
rm -rf debian/python-axl
rm -rf debian/axl-knife
rm -rf debian/python-axl-dev
rm -rf debian/libaxl-dev
rm -rf debian/libaxlns-dev
rm -rf debian/libaxlns1
rm -rf debian/libaxlbabel1
rm -rf debian/tmp

# find all files that have copy right declaration associated to Aspl that don't have 
# the following declaration year
current_year="2025"
LANG=C rgrep -i "Copyright" babel debian-files knife ns py-axl rpm src test web VERSION README README.win32 AUTHORS autogen.sh configure.ac doc 2>&1 | grep "Advanced" | grep -v "Permission denied" | grep -v '~:' | grep -v '/\.svn/' | grep -v "${current_year}"
