/**
 *  LibAxl:  Another XML library 
 *  Copyright (C) 2006 Advanced Software Production Line, S.L.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to
 *  develop proprietary applications using this library without any
 *  royalty or fee but returning back any change, improvement or
 *  addition in the form of source code, project image, documentation
 *  patches, etc. 
 *
 *  For commercial support on build XML enabled solutions contact us:
 *          
 *      Postal address:
 *         Advanced Software Production Line, S.L.
 *         C/ Dr. Michavila Nº 14
 *         Coslada 28820 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://fact.aspl.es
 */
#include <axl.h>
#include <stdarg.h>
#include <stdio.h>

/** 
 * \mainpage AXL: Another Library implementation (XML 1.0 standard)
 * 
 * \section intro Introduction
 *
 * AXL is an small library which aims to implement the XML 1.0
 * standard, as defined at this time, inside the XML 1.0 third edition
 * recomendation found at: http://www.w3.org/TR/REC-xml/
 * 
 * It was implemented for the Af-Arch project to avoid the LibXML2
 * dependency (http://www.xmlsoft.org).
 *
 * Main objectives for this library is to provide:
 * 
 * - A clean implementation, that only includes, those elements
 * defined, and only those, inside the XML 1.0 standard. As of this
 * writing, that standard is the third edition.
 *
 * - Ensure that the library is implemented using abstract data types,
 * commonly known as opaque types, to avoid exposing details to the
 * user space code that is consuming the API.
 *
 * - To be small and efficient, ensuring on every stable release that
 * the library do not leak, not only while using the library in a
 * proper manner but also when errors were found. This point is really
 * important for us because Af-Arch server applications needs to be
 * working for a long time. The other issue is that the Af-Arch client
 * platform should be easily embeded, so, footprint is a requirement.
 *
 * 
 *
 * 
 *
 * 
 *
 */


/**
 * \defgroup axl_module Axl main: Init functions for the library
 */

/** 
 * \addtogroup axl_module
 * @{
 */

/** 
 * @brief Initializes Axl library.
 *
 * Currently this function is not required to be executed because
 * libaxl implementation don't use it. The implementation will try to
 * make no used for internal variable initialization.
 *
 * However the API is provided for future usage.
 * 
 * @return The function returns AXL_TRUE if it was properly
 * initialized or AXL_FALSE if something fails.  if fails.
 */
bool axl_init ()
{
	/* nothing to initialize dude */
	return AXL_TRUE;
}

/** 
 *
 * @brief Allows to terminate libaxl library function.
 *
 * Currently this function is not required becasue nothing is done to
 * stop libaxl function. The implementation will try to keep this,
 * however, if the future this could be required.
 */
void axl_end ()
{
	/* nothing to do jack */
	return;
}


/* @} */
