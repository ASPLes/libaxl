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

# check type sizes
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(void *)

##########################
# Check for doxygen tool #
##########################
dnl check for doxygen documentation
AC_ARG_ENABLE(axl-doc, [  --enable-axl-doc     Enable building Axl documentation (doxygen required) [default=yes]], enable_axl_doc="$enableval", enable_axl_doc=yes)
if test x$enable_axl_doc = xyes ; then
        AC_CHECK_PROG(DOXYGEN, doxygen, "yes", "no")
fi
AM_CONDITIONAL(ENABLE_AF_ARCH_DOC, test "x$DOXYGEN" = "xyes")

dnl check to build test files
AC_ARG_ENABLE(axl-test, [  --enable-axl-test     Enable building Axl test executables [default=yes]], enable_axl_test="$enableval", enable_axl_test=yes)
AM_CONDITIONAL(ENABLE_AXL_TESTS, test "x$enable_axl_test" = "xyes")

dnl check to build libaxl namespace support
AC_ARG_ENABLE(axl-ns, [  --enable-axl-ns     Enable building Axl Namespace support [default=yes]], enable_axl_ns="$enableval", enable_axl_ns=yes)
AM_CONDITIONAL(ENABLE_AXL_NS, test "x$enable_axl_ns" = "xyes")

AC_ARG_ENABLE(axl-log, [  --enable-axl-log     Enable building Axl with debug log support [default=yes]], enable_axl_log="$enableval", enable_axl_log=yes)
AM_CONDITIONAL(ENABLE_AXL_LOG, test "x$enable_axl_log" = "xyes")

# The following command also comes to produce the axl_config.h file
# required by the tool. If you update this, remember to update the
# af-arch main configure.ac
AC_TRY_LINK([#include <stdio.h>], 
[
  char * result = NULL;
  vasprintf (&result, "This is a test: %d", 10);
], [have_vasprintf=yes],[have_vasprintf=no])

# produce axl_config.h file this section will only be run if
# config.status is invoked with no arguments, or with "axl_config.h"
# as an argument. Remember to update af-arch configure.ac file to
# include the same script building as the following.
AC_CONFIG_COMMANDS([axl_config.h],
[
	outfile=axl_config.h-tmp
	cat > $outfile <<_______EOF
/* 
 * Axl Library axl_config.h
 * Platform dependant definitions.
 *
 * This is a generated file.  Please modify 'configure.in'
 */

#ifndef __AXL_CONFIG_H__
#define __AXL_CONFIG_H__

/** 
 * \addtogroup axl_decl_module
 * @{
 */

/** 
 * @brief Allows to convert integer value (including constant values)
 * into a pointer representation. 
 *
 * Use the oposite function to restore the value from a pointer to a
 * integer: \ref PTR_TO_INT. 
 *
 * @param integer The integer value to cast to pointer.
 *
 * @return A \ref axlPointer reference.
 */
#define INT_TO_PTR(integer)   ((axlPointer) ${pti_cast} integer)

/** 
 * @brief Allows to convert a pointer reference (\ref axlPointer),
 * which stores an integer that was stored using \ref INT_TO_PTR.
 *
 * Use the oposite function to restore the pointer value stored in the
 * integer value.
 * 
 * @param ptr The pointer to cast to a integer value.
 * 
 * @return A int value.
 */
#define PTR_TO_INT(ptr) ((int) ${pti_cast} ptr)

/**
 * @brief Allows to get current platform configuration. This is used
 * by Axl library but could be used by applications built on top of
 * Axl to change its configuration based on the platform information.
 */
$axl_platform

$vasprintf_status

/* @} */

#endif
_______EOF

	# Check if the file exists and it is equal, if not, overwrite it 
	if cmp -s $outfile src/axl_config.h; then
	  AC_MSG_NOTICE([axl_config.h is unchanged])
	  rm -f $outfile
	else
	  mv $outfile src/axl_config.h
	fi
],[

# Check size of void pointer against the size of a single
# integer. This will allow us to know if we can cast directly a
# integer to pointer and viceversa.
case $ac_cv_sizeof_void_p in
$ac_cv_sizeof_int)  	pti_cast=''        pti_cast=''                ;;
$ac_cv_sizeof_long) 	pti_cast='(long)'  pti_cast='(unsigned long)' ;;
*) 			pti_unknown_void_p=yes                        ;;
esac

# Get current configure for the platform and the os we are running.
case $host in
  *-*-beos*)
    axl_platform="#define AXL_OS_BEOS"
    ;;
  *-*-cygwin*)
    axl_platform="#define AXL_OS_UNIX
#define AXL_PLATFORM_WIN32
#define AXL_WITH_CYGWIN"
    ;;
  *-*-mingw*)
    axl_platform="#define AXL_OS_WIN32
#define AXL_PLATFORM_WIN32"
    ;;
  *)
    axl_platform="#define AXL_OS_UNIX"
    ;;
esac

case $have_vasprintf in
yes)
     vasprintf_status="/**
 * @internal Allows to now if the platform support vasprintf
 * function. Do not use this macro as it is supposed to be for
 * internal use.
 */
#define AXL_HAVE_VASPRINTF 1"
     ;;
no)
     vasprintf_status=""
     ;;
esac

])


AC_OUTPUT([
Makefile
src/Makefile
ns/Makefile
doc/Makefile
doc/axl-doc.doxygen
test/Makefile
axl.pc
axl-ns.pc])

echo "------------------------------------------"
echo "--       LibAXL LIBRARY SETTINGS        --"
echo "------------------------------------------"
echo "   Installation prefix:          [$prefix]"
echo
echo "   Build Axl Namespace support:  [$enable_axl_ns]"
echo
echo "   Build Axl Library tests:      [$enable_axl_test]"
echo
echo "   Build Axl Library debug log:  [$enable_axl_log]"
echo
if test x$DOXYGEN = xyes ; then
   echo "   Build Axl Library doc:        enabled"
else	    
   echo "   Build Axl Library doc:        disabled"
   echo  
   echo "     NOTE: To enable Axl documentation you will"
   echo "     have to install doxygen tool. Then run again"
   echo "     configure process."
fi
echo "------------------------------------------"
echo "--     NOW TYPE: make; make install     --"
echo "------------------------------------------"
