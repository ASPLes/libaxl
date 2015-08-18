#!/bin/sh

python /usr/src/freebsd-pkg-builder/pkg-builder.py /usr/src/libaxl /usr/src/libaxl/freebsd/freebsd\:9\:x86\:64/ --skip-build --version=`cat /usr/src/libaxl/VERSION` --description="Fast and efficient XML 1.0 implementation"
