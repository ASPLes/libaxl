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
#include <axl_decl.h>
#include <axl_stream.h>
#include <axl_log.h>
#define LOG_DOMAIN "axl-decl"
/** 
 * @internal
 *
 * @brief Allows to compare two strings provided, s1 and s1 to be
 * equal.
 *
 * In the case both are equal, AXL_TRUE is returned. Otherwise
 * AXL_FALSE.
 * 
 */
bool axl_cmp (char * string, char * string2)
{
	axl_return_val_if_fail (string, AXL_FALSE);
	axl_return_val_if_fail (string2, AXL_FALSE);
	
	if (! axl_stream_cmp ((string != NULL) ? string : "s1",
			      (string2 != NULL) ? string2 : "s2",
			      (string != NULL) ? strlen (string) : 2))
		return AXL_FALSE;

	return axl_stream_cmp ((string != NULL) ? string : "s1",
			       (string2 != NULL) ? string2 : "s2",
			       (string2 != NULL) ? strlen (string2) : 2);
}
