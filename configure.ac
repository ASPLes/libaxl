dnl  Copyright (C) 2006 Advanced Software Production Line, S.L.
dnl
dnl  This program is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU Lesser General Public License
dnl  as published by the Free Software Foundation; either version 2.1 of
dnl  the License, or (at your option) any later version.
dnl
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of 
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
dnl  GNU Lesser General Public License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public
dnl  License along with this program; if not, write to the Free
dnl  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
dnl  02111-1307 USA
dnl  
dnl  You may find a copy of the license under this software is released
dnl  at COPYING file. This is LGPL software: you are wellcome to
dnl  develop propietary applications using this library withtout any
dnl  royalty or fee but returning back any change, improvement or
dnl  addition in the form of source code, project image, documentation
dnl  patches, etc. 
AC_INIT(src/axl.h)

AXL_VERSION=`cat VERSION`
AC_SUBST(AXL_VERSION)

AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE(axl, $AXL_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_PROG_CC
AC_ISC_POSIX
AC_HEADER_STDC
AM_PROG_LIBTOOL

AC_OUTPUT([
Makefile
src/Makefile
doc/Makefile
test/Makefile
axl.pc])

echo "------------------------------------------"
echo "--       LibAXL LIBRARY SETTINGS        --"
echo "------------------------------------------"
echo "   Installation prefix:          [$prefix]"
echo
echo "------------------------------------------"
echo "--     NOW TYPE: make; make install     --"
echo "------------------------------------------"
