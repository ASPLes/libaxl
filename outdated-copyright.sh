#!/bin/bash

# cleanup sources to avoid reporting generated content
make clean > /dev/null 2>&1
rm -rf doc/html > /dev/null 2>&1
rm -rf debian/libaxl-doc > /dev/null 2>&1
rm -rf debian/libaxlbabel-dev > /dev/null 2>&1
rm -rf debian/libaxl1 > /dev/null 2>&1
rm -rf debian/python-axl > /dev/null 2>&1
rm -rf debian/axl-knife > /dev/null 2>&1
rm -rf debian/python-axl-dev > /dev/null 2>&1
rm -rf debian/libaxl-dev > /dev/null 2>&1
rm -rf debian/libaxlns-dev > /dev/null 2>&1
rm -rf debian/libaxlns1 > /dev/null 2>&1
rm -rf debian/libaxlbabel1 > /dev/null 2>&1
rm -rf debian/tmp > /dev/null 2>&1

# find all files that have copy right declaration associated to Aspl that don't have 
# the following declaration year
current_year="2025"
LANG=C rgrep -i "Copyright" babel debian-files knife ns py-axl rpm src test web VERSION README README.win32 AUTHORS autogen.sh configure.ac doc 2>&1 | grep "Advanced" | grep -v "Permission denied" | grep -v '~:' | grep -v '/\.svn/' | grep -v "${current_year}"
