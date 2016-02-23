#!/bin/bash

set -x

# gen axl_product_version.nsh
echo -n "!define PRODUCT_VERSION \"" > axl_product_version.nsh
value=`cat VERSION`
echo -n $value >> axl_product_version.nsh
echo "\"" >> axl_product_version.nsh
echo "!define PLATFORM_BITS \"$1\"" >> axl_product_version.nsh
echo "InstallDir \"\$PROGRAMFILES$1\AxlW$1\"" >> axl_product_version.nsh



