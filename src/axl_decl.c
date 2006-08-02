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
 * In the case both are equal, true is returned. Otherwise
 * false.
 * 
 */
bool axl_cmp (char * string, char * string2)
{
	int iterator = 0;

	if (string == NULL)
		return false;
	if (string2 == NULL)
		return false;
	
	/* for each item inside the iterator */
	while (string [iterator] != 0 && string2 [iterator] != 0) {
		
		/* check the content */
		if (string [iterator] != string2 [iterator])
			return false;

		/* update the iterator */
		iterator++;
		
	} /* end while */
	
	/* check that both string ends at the same point */
	if (string [iterator] != 0 ||
	    string2 [iterator] != 0)
		return false;
	
	return true;
}


/** 
 * @brief Allows to check if both strings provided are equal on its initial size bytes.
 *
 * This function is more efficient than common memcmp because it
 * doesn't perform the additional work to figure out which are the
 * bytes that differ both strings.
 * 
 * @param string The string to check.
 *
 * @param string2 The second string to check.
 *
 * @param size The size to check for both strings to be equal.
 * 
 * @return \ref true if the both strings are equal for its initial
 * size bytes or \ref false if not.
 */
bool axl_memcmp (char * string, char * string2, int size)
{
	int iterator = 0;

	_memcmp(iterator,string,string2,size);
}



/** 
 * @brief Allows to deallocate memory referenced by <i>ref</i> but
 * checking before that the reference is different from null.
 * 
 * @param ref The reference to clear.
 */
void axl_free (axlPointer ref)
{
	if (ref == NULL)
		return;

	free (ref);
	return;
}

/** 
 *
 * @brief Call to strdup function check if received is a NULL
 * reference
 * 
 * @param string The string to copy.
 * 
 * @return A newly allocated value or NULL.
 */
char * axl_strdup (const char * string)
{
	return (string != NULL) ? (char *) axl_stream_strdup ((char *) string) : NULL;
}
