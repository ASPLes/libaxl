/*
 *  Axl Knife: Console tool on top of Axl Library
 *  Copyright (C) 2007 Advanced Software Production Line, S.L.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  For commercial support on build XML enabled solutions contact us:
 *          
 *      Postal address:
 *         Advanced Software Production Line, S.L.
 *         Edificio Alius A, Oficina 102,
 *         C/ Antonio Suarez Nº 10,
 *         Alcalá de Henares 28802 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/xml
 */
#ifndef __AXL_KNIFE_H__
#define __AXL_KNIFE_H__

/* local includes */
#include <stdarg.h>

/* include axl main headers */
#include <axl.h>

/* include axl namespace support */
#include <axl_ns.h>

/* include exarg support */
#include <exarg.h>

/** 
 * Drop an error msg to the console stderr.
 *
 * To drop an error message use:
 * \code
 *   error ("unable to open file: %s", file);
 * \endcode
 * 
 * @param m The error message to output.
 */
void  axl_knife_error (const char * file, int line, const char * format, ...);

/** 
 * Drop a msg to the console stdout.
 *
 * To drop a message use:
 * \code
 *   msg ("module loaded: %s", module);
 * \endcode
 * 
 * @param m The console message to output.
 */
void  axl_knife_msg   (const char * file, int line, const char * format, ...);

/** 
 * Drop a second level msg to the console stdout.
 *
 * To drop a message use:
 * \code
 *   msg2 ("module loaded: %s", module);
 * \endcode
 * 
 * @param m The console message to output.
 */
void  axl_knife_msg2   (const char * file, int line, const char * format, ...);

/** 
 * Drop a warning msg to the console stdout.
 *
 * To drop a message use:
 * \code
 *   wrn ("module loaded: %s", module);
 * \endcode
 * 
 * @param m The warning message to output.
 */
void  axl_knife_wrn   (const char * file, int line, const char * format, ...);

#if defined(__COMPILING_AXL_KNIFE__)
#define error(m,...)  do{axl_knife_error (__AXL_FILE__, __AXL_LINE__, m, ##__VA_ARGS__);}while(0)
#define msg(m,...)    do{axl_knife_msg (__AXL_FILE__, __AXL_LINE__, m, ##__VA_ARGS__);}while(0)
#define msg2(m,...)   do{axl_knife_msg2 (__AXL_FILE__, __AXL_LINE__, m, ##__VA_ARGS__);}while(0)
#define wrn(m,...)    do{axl_knife_wrn (__AXL_FILE__, __AXL_LINE__, m, ##__VA_ARGS__);}while(0)
#endif

#endif
