/*
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
 *         info@aspl.es - http://www.aspl.es/xml
 */
#include <axl_ns.h>

#define LOG_DOMAIN "axl-ns-node"

/**
 * \defgroup axl_ns_node_module Axl Node Namespace: Xml 1.0 namespace support for XML nodes
 */

/** 
 * \addtogroup axl_ns_node_module
 * @{
 */

/** 
 * @brief Allows to perform a node name (tag) checking using XML 1.0
 * Namespace rules.
 *
 * This function must be used instead of \ref NODE_CMP_NAME if your application must support
 * XML 1.0 namespaces.
 *
 * The idea is that you declares the namespace that you will recognize
 * using a macro definition at some place in your headers, as follows:
 * \code
 * #define YOUR_NS "http://yourdomain.com/somepath/yourapplication"
 * \endcode
 *
 * Previous namespace must be unique. This is crucial to avoid tag
 * clashing. Now, you use this namespace identifier and the local name
 * of your tag to check a particular node to have a particular name as
 * follows:
 * 
 * \code
 * if (axl_ns_node_cmp (node, YOUR_NS, "table")) {
 *     // found table node inside your namespace
 * }
 * \endcode
 *
 * It is required to call first to \ref axl_ns_doc_validate in order
 * to make this function to properly work. 
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
 * @brief Allows to find the first child called as provided inside the
 * childs (including its descendants) hold by the parent provided,
 * with namespace support.
 *
 * This function must be used instead of \ref
 * axl_node_find_called if your application must support
 * XML 1.0 namespaces.
 *
 * This function is similar to \ref axl_ns_node_get_child_called but
 * it will look for a child node called as provided not only in direct
 * childs hold by the parent but also on its all descendants, giving
 * support to configure the namespace where the lookup will be run.
 *
 * If you are looking for a function to search for a particular child
 * node inside direct childs stored for the provided parent, then you
 * must use \ref axl_ns_node_get_child_called.
 *
 * There is also a convenience function that allows to perform a
 * lookup using as a reference a document (using the root node from
 * it): \ref axl_ns_doc_find_called.
 *
 * @param parent The parent where the lookup will be produced.
 *
 * @param ns The namespace where the node will be searched
 *
 * @param name The name of the child to be looked up.
 * 
 * @return A reference to the node found (first instaned matching the
 * name) or NULL if it fails to find a child. 
 */
axlNode * axl_ns_node_find_called  (axlNode    * parent,
				    const char * ns,
				    const char * name)
{
	axlNode * node;
	axlNode * child;

	/* for the first child found */
	node = axl_ns_node_get_child_called (parent, ns, name);
	if (node != NULL)
		return node;

	/* now, for all childs, try to look for the node */
	node = axl_node_get_first_child (parent);
	while (node != NULL) {
		/* make the search */
		child = axl_ns_node_find_called (node, ns, name);
		
		/* child found, return the reference */
		if (child != NULL)
			return child;
		
		/* get next */
		node = axl_node_get_next (node);
	} /* end while */

	/* child note found */
	return NULL;
}

/** 
 * @brief Allows to get a particular child node for the given node
 * (\ref axlNode), inside the provided namespace.
 *
 * This function must be used instead of \ref
 * axl_node_get_child_called if your application must support
 * XML 1.0 namespaces.
 * 
 * @param parent The parent node where the child will be looked up.
 *
 * @param ns The namespace where the lookup will be run.
 *
 * @param name The name for the child to search.
 * 
 * @return A refernce to a \ref axlNode or NULL if no child exists
 * called by the name provided, inside the node provided.
 */
axlNode * axl_ns_node_get_child_called   (axlNode    * parent, 
					  const char * ns,
					  const char * name)
{
	axlNode * node;

	/* for the first child found */
	node = axl_node_get_first_child (parent);
	while (node != NULL) {
		/* check and return the node found */
		if (axl_ns_node_cmp (node, ns, name))
			return node;
		
		/* get next */
		node = axl_node_get_next (node);
	} /* end while */

	return NULL;
}

/** 
 * @brief Allows to get the next node, following to the node provided,
 * matching the given name, inside the namespace configuration
 * provided.
 *
 * This function must be used instead of \ref
 * axl_node_get_next_called if your application must support
 * XML 1.0 namespaces.
 * 
 * @param node The node that is requested to return its next sibling
 * node.
 * 
 * @param ns The namespace value to use to perform the search.
 *
 * @param name The name to match for the next node.
 * 
 * @return A reference to the next node or NULL if it fails. The
 * returned reference mustn't be deallocated.
 */
axlNode * axl_ns_node_get_next_called    (axlNode    * node,
					  const char * ns,
					  const char * name)
{
	axlNode * next;

	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (ns, NULL);
	axl_return_val_if_fail (name, NULL);

	/* while there is a next node */
	next = axl_node_get_next (node);
	while (next != NULL) {
		/* check the node */
		if (axl_ns_node_cmp (next, ns, name))
			return next;

		/* update to the next */
		next = axl_node_get_next (next);
	} /* end while */

	/* no node was found */
	return NULL;
}

/** 
 * @brief Allows to get the previous node, preceding to the node
 * provided, matching the given name, inside the given namespace
 * value.
 * 
 * @param node The node that is requested to return its previous
 * sibling node.
 *
 * @param ns The namespace to be used to perform the lookup.
 *
 * @param name The name to match for the previous node.
 * 
 * @return A reference to the previous node or NULL if it fails. The
 * returned reference mustn't be deallocated.
 */
axlNode * axl_ns_node_get_previous_called (axlNode    * node,
					   const char * ns,
					   const char * name)
{
	axlNode * previous;

	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (ns, NULL);
	axl_return_val_if_fail (name, NULL);

	/* while there is a previous node */
	previous = axl_node_get_previous (node);
	while (previous != NULL) {
		/* check the node */
		if (axl_ns_node_cmp (previous, ns, name))
			return previous;

		/* update to the next */
		previous = axl_node_get_previous (previous);
	} /* end while */

	/* no node was found */
	return NULL;
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

/**
 * @}
 */
