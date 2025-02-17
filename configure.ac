dnl  Copyright (C) 2025 Advanced Software Production Line, S.L.
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
dnl
dnl  For commercial support on build XML enabled solutions contact us:
dnl          
dnl      Postal address:
dnl         Advanced Software Production Line, S.L.
dnl         Edificio Alius A, Oficina 102,
dnl         C/ Antonio Suarez Nº 10,
dnl         Alcalá de Henares 28802 Madrid
dnl         Spain
dnl
dnl      Email address:
dnl         info@aspl.es - http://www.aspl.es/xml
dnl
AC_INIT(src/axl.h)

configure_dir=`dirname "$0"`
AXL_VERSION=`cat $configure_dir/VERSION`
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

compiler_options=""
STRICT_PROTOTYPES=""
if test "$compiler" = "gcc" ; then
   compiler_options="-Wstrict-prototypes -Wall -Werror -g -ansi -fstack-protector-all -Wstack-protector"
   echo "Detected gcc compiler: $compiler, adding options: $compiler_options"
fi
AC_SUBST(compiler_options)

dnl check for py-axl support
AC_ARG_ENABLE(py-axl,   [   --disable-py-axl       Makes Axl Library to be build without PyAxl: python bindings support],
              enable_py_axl_support="$enableval",
	      enable_py_axl_support=yes)
AM_CONDITIONAL(ENABLE_PY_AXL_SUPPORT, test "x$enable_py_axl_support" = "xyes")
AM_PATH_PYTHON([2.3],, enable_py_axl_support=no)
if test "$enable_py_axl_support" = "yes" ; then
   AM_PATH_PYTHON([2.3])

   # find python-config
   # AC_SUBST(PYTHON_CONFIG, m4_esyscmd([./get-python-config]))
   if test -f "/bin/bash" ; then
      PYTHON_CONFIG=`./get-python-config`
   else
      bash_location=`which bash`
      echo "Bash location found: $bash_location"
      PYTHON_CONFIG=`${bash_location} ./get-python-config`
   fi 

   echo "Python pythondir is: ${pythondir}"
   echo "Python found: ${PYTHON}"

   # get site_packages
   # AC_SUBST(python_dir_check, m4_esyscmd([./check-python-dir.py $pythondir]))
   if test "${PYTHON}" = "/usr/bin/python" ; then
      python_dir_check=`./check-python-dir.py ${pythondir}`
   else
      if test "${PYTHON}" = "/usr/bin/python2" ; then
         python_dir_check=`/usr/bin/python2 ./check-python-dir.py ${pythondir}`
      else
        if test "${PYTHON}" = "/usr/bin/python2.7" ; then
           python_dir_check=`/usr/bin/python2.7 ./check-python-dir.py ${pythondir}`
        else
           python_dir_check=`python ./check-python-dir.py ${pythondir}`
        fi
      fi
   fi     

   # get python includes 
   PYTHON_INCLUDES=`$PYTHON_CONFIG --includes 2>/dev/null`
   old_CPPFLAGS=$CPPFLAGS
   CPPFLAGS="$PYTHON_INCLUDES"
   echo "Python includes: $PYTHON_INCLUDES"
   AC_SUBST(PYTHON_INCLUDES)

   AC_TRY_COMPILE([#include <Python.h>],
      [Py_InitModule3(NULL, NULL, NULL);],
      python_devel_found=yes,
      python_devel_found=no)
   AC_MSG_RESULT([Python development headers status: $python_devel_found])
   if test "x$python_devel_found" = "xno"; then
       AC_MSG_WARN([Cannot find Python.h header or current python development environment do not compile symbols required. On debian try to install python-dev package.])
   fi

   # get python flags and libs
   PYTHON_CFLAGS=`$PYTHON_CONFIG --cflags 2>/dev/null | sed -r 's/\-arch (\d|\w)+ //g'`
   PYTHON_LIBS=`$PYTHON_CONFIG --ldflags 2>/dev/null`
   AC_SUBST(PYTHON_CFLAGS)
   AC_SUBST(PYTHON_LIBS)

   # check python_install_dir and pythondir maches
   python_dir_temp=`eval echo ${pythondir}`
   # check python_install_dir and pythondir maches
   if test "$python_dir_check" != "ok" ; then
      AC_MSG_ERROR([ERROR: pythondir variable is poiting to an unusable location: $python_dir_temp. Error detected: $python_dir_check. Fix your autoconf install or disable python building, for that, pass --disable-py-axl to configure. This usually indicates a problem with automake package which provides the _AM_PATH_PYTHON_ macro.])
   fi
   echo "Python install dir OK: $python_dir_temp"
fi
AM_CONDITIONAL(ENABLE_PY_AXL, test "x$python_devel_found" = "xyes")

compiler_options=""
STRICT_PROTOTYPES=""
if test "$compiler" = "gcc" ; then
   compiler_options="-Wstrict-prototypes -Wall -Werror -g -ansi"
   echo "Detected gcc compiler: $compiler, adding options: $compiler_options"
fi
AC_SUBST(compiler_options)

if test "$enable_py_axl_support" = "yes" ; then
  AC_CHECK_PROG(PKG_CONFIG, pkg-config, "yes", "no")
  if test "$PKG_CONFIG" = "no" ; then
    dnl now check pkgconf which seems to be a replacement in free-bsd
    AC_CHECK_PROG(PKG_CONFIG, pkgconf, "yes", "no")
    if test "$PKG_CONFIG" = "no" ; then
       AC_MSG_ERROR([You need to install pkg-config/pkgconf to compile python-axl. See: http://pkgconfig.freedesktop.org/releases/ or https://github.com/pkgconf/pkgconf])
    fi  			      	 	 
  fi
fi


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

dnl check to build axl knife tool 
AC_ARG_ENABLE(axl-knife, [  --enable-axl-knife     Enable building Axl Knife tool [default=yes]], enable_axl_knife="$enableval", enable_axl_knife=yes)
AM_CONDITIONAL(ENABLE_AXL_KNIFE, test "x$enable_axl_knife" = "xyes")

dnl check to build axl babel support
AC_ARG_ENABLE(axl-babel, [  --enable-axl-babel     Enable building Axl Babel tool [default=yes]], enable_axl_babel="$enableval", enable_axl_babel=yes)
AM_CONDITIONAL(ENABLE_AXL_BABEL, test "x$enable_axl_babel" = "xyes")

# The following command also comes to produce the axl_config.h file
# required by the tool. If you update this, remember to update the
# af-arch main configure.ac
AC_TRY_LINK([#define _GNU_SOURCE
#include <stdio.h>], 
[
  char * result;
  va_list args;
  return vasprintf (&result, "This is a test: %d", args);
], [have_vasprintf=yes],[have_vasprintf=no])
echo "Checking vasprintf support: $have_vasprintf"

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
 * This is a generated file.  Please modify 'configure.ac'
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
#define INT_TO_PTR(integer)   ((axlPointer) ${pti_cast} ((int)integer))

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
#define PTR_TO_INT(ptr) ((int) ${pti_cast} (ptr))

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
$ac_cv_sizeof_int)  	pti_cast=''        ptui_cast=''                ;;
$ac_cv_sizeof_long) 	pti_cast='(long)'  ptui_cast='(unsigned long)' ;;
*) 			pti_unknown_void_p=yes                         ;;
esac

# Get current configure for the platform and the os we are running.
echo "Host detected: $host"
case $host in
  *-*-beos*)
    axl_platform="#define AXL_OS_BEOS (1)"
    ;;
  *-*-cygwin*)
    axl_platform="#define AXL_OS_UNIX (1)
#define AXL_PLATFORM_WIN32 (1)
#define AXL_WITH_CYGWIN (1)"
    ;;
  *-*-mingw*)
    axl_platform="#define AXL_OS_WIN32 (1)
#define AXL_PLATFORM_WIN32 (1)"
    ;;
  *)
    axl_platform="#define AXL_OS_UNIX (1)"
    ;;
esac

case $have_vasprintf in
yes)
     vasprintf_status="/**
 * @internal Allows to now if the platform support vasprintf
 * function. Do not use this macro as it is supposed to be for
 * internal use.
 */
#define AXL_HAVE_VASPRINTF (1)"
     ;;
no)
     vasprintf_status=""
     ;;
esac

])

dnl The following lines are for the embeded support of exarg inside axl-knife.
dnl support for vasprintf
case $have_vasprintf in
yes)
	EXARG_FLAGS="-DHAVE_VASPRINTF $EXARG_FLAGS"
	echo "vasprintf support found"
     ;;
esac

dnl check for windows platform
case $host in
*-*-mingw*)
	EXARG_FLAGS="-DOS_WIN32 $EXARG_FLAGS"
	echo "windows platform found"
	;;	
*)
	echo "platform found: $host"
    ;;
esac
AC_SUBST(EXARG_FLAGS)

dnl get site dinamic library extension
SHARED_EXTENSION=$shrext_cmds
AC_SUBST(SHARED_EXTENSION)


AC_OUTPUT([
Makefile
src/Makefile
ns/Makefile
doc/Makefile
doc/axl-doc.doxygen
babel/Makefile
test/Makefile
knife/Makefile
py-axl/Makefile
py-axl/__init__.py
py-axl/doc/conf.py
py-axl/doc/Makefile
axl.pc
axl-ns.pc
axl-babel.pc])

echo "------------------------------------------"
echo "--       LibAXL LIBRARY SETTINGS        --"
echo "------------------------------------------"
echo "   Installation prefix:          [$prefix]"
echo "   Compiler:                     [$compiler]"
echo "   Build Axl Namespace support:  [$enable_axl_ns]"
echo "   Build Axl Library tests:      [$enable_axl_test]"
echo "   Build Axl Knife:              [$enable_axl_knife]"
echo "   Build Axl Library debug log:  [$enable_axl_log]"
echo "   Build PyAxl:                  [$python_devel_found]"
if test x$DOXYGEN = xyes ; then
   echo "   Build Axl Library doc:        enabled"
else	    
   echo "   Build Axl Library doc:        disabled"
   echo "     NOTE: To enable Axl documentation you will"
   echo "     have to install doxygen tool. Then run again"
   echo "     configure process."
fi
echo "   vasprintf support:            [$have_vasprintf]"
echo
echo "   To build without optimizer use: "
echo "   >> make CFLAGS='-g -O0' CXXFLAGS='-g -O0' clean all"
echo
echo "------------------------------------------"
echo "--     NOW TYPE: make; make install     --"
echo "------------------------------------------"
