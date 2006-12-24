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
#include <axl_ns.h>

#define LOG_DOMAIN "axl-ns-node"

/** 
 * @brief Allows to perform a node name (tag) checking using XML 1.0
 * Namespace rules.
 * 
 * @param node The node that will be checked.
 *
 * @param ns The namespace to use to match the node name.
 *
 * @param name The node name to be checked (without namespace
 * prefixes).
 * 
 * @return \ref true if the node name is the one expected, binded by
 * the namespace declaration, otherwise \ref false is returned. The
 * function returns \ref false in the of some paremeter is provided
 * null.
 */
bool axl_ns_node_cmp (axlNode    * node, 
		      const char * ns, 
		      const char * name)
{
	int    position;
	char * _name;

	axl_return_val_if_fail (node, false);
	axl_return_val_if_fail (ns, false);
	axl_return_val_if_fail (name, false);

	/* check the name is namespace prefixed */
	if (axl_ns_node_is_prefixed (node, &position)) {

		/* namespace bound */
		_name           = (char*) axl_node_get_name (node);
		_name[position] = 0;

		/* call to check */
		if (! axl_ns_doc_node_check (node, _name, ns)) {
			/* restore the value */
			_name[position] = ':';
			return false;
		}

		/* restore the value */
		_name[position] = ':';

		/* now check the node name (now we know the namespace is checked) */
		return axl_cmp (name, _name + position + 1);
	} /* end if */

	/* look for the default namespace */
	if (! axl_ns_doc_check_default (node, ns)) {
		/* default namespace for the current node doesn't match */
		return false;
	}
	
	/* default namespace match, check the node name */
	return axl_cmp (name, axl_node_get_name (node));
}

/** 
 * @brief Allows to check if the provided node, has a name (tag) which is namespace prefixed. 
 *
 * The following name is considered namespace prefixed
 * <b>edi:price</b> while using <b>edi</b> is not.
 *
 * @param node The node to be checked to have a name prefixed.
 *
 * @param position Optional variable where the position of the ":"
 * inside the xml node name is returned. On \ref false, the value is
 * configured to -1.
 * 
 * @return \ref true if the node is prefixed, otherwise \ref false is
 * returned. The function also returns \ref false if the node
 * reference received is NULL.
 */
bool axl_ns_node_is_prefixed  (axlNode * node, int * position)
{
	int          iterator = 0;
	const char * name;

	axl_return_val_if_fail (node, false);

	/* configure default position */
	if (position)
		*position = -1;

	/* get the node name and check each character */
	name = axl_node_get_name (node);
	while (name[iterator] != 0) {

		/* if found, notify this */
		if (name[iterator] == ':') {
			/* configure default position */
			if (position)
				*position = iterator;

			return true;
		} /* end if */

		/* go to the next character */
		iterator++;
	} /* end while */
	
	/* reached this point, false must be returned */
	return false;
}
