#!/bin/sh

echo "Preparing sources"
rm -rf rpm/SOURCES/*.tar.gz
rm -rf rpm/BUILD/*
rm -rf rpm/BUILDROOT/*
find rpm/RPMS/ -type f -exec rm {} \;

make dist
cp axl-`cat VERSION`.tar.gz rpm/SOURCES

echo "Calling to compile packages.."
LANG=C rpmbuild -ba --define '_topdir /usr/src/libaxl/rpm' rpm/SPECS/libaxl1.spec
error=$?
if [ $error != 0 ]; then
    echo "ERROR: ***"
    echo "ERROR: rpmbuild command failed, exitcode=$error"
    echo "ERROR: ***"
    exit $error
fi


echo "Output ready at rpm/RPMS"
find rpm/RPMS -type f -name '*.rpm' > rpm/RPMS/files
cat rpm/RPMS/files



