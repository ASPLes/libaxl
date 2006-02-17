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

struct _axlAttribute {
	char * name;
	char * value;
};

struct _axlNode {
	/** 
	 * @internal
	 *
	 * Node name that is the value found at the very begining of
	 * the node definition <name.../>
	 */
	char          * name;

	/** 
	 * @internal
	 * @brief The attributes this node has.
	 */
	axlAttribute ** attributes;

	/** 
	 * @internal
	 *
	 * @brief How many attributes this node has.
	 */
	int             attributes_count;

	/** 
	 * @internal
	 * 
	 * Current configuration for a node to be considered empty.
	 */
	bool            is_empty;
};



/** 
 * @brief Creates a new \ref AxlNode with the provided name.
 *
 * @param name The name to be used for the node be created.
 * 
 * @return 
 */
axlNode * axl_node_create (char * name)
{
	axlNode * node = NULL;

	/* perform some environmental checks */
	axl_return_val_if_fail (name, NULL);
	
	node           = axl_new (axlNode, 1);
	node->name     = axl_strdup (name);

	return node;
}

/** 
 * @brief Allows to configure the given node to be empty.
 *
 * A \ref axlNode is empty when it is known that the node doesn't have
 * any content inside it as a child element.
 *
 * @param node The node to configure as empty.
 *
 * @param empty The value for emptyness to be used. AXL_FALSE will
 * mean that the node is not empty.
 */
void      axl_node_is_empty (axlNode * node, bool empty)
{
	axl_return_if_fail (node);
	
	/* set value received */
	node->is_empty = empty;

	return;
}

/** 
 * @brief Destroy the given node provided by the reference.
 *
 * The function will check for nodes that are null references.
 * 
 * @param node The node to destroy. 
 */
void axl_node_free (axlNode * node) 
{
	axl_return_if_fail (node);

	/* free current node */
	if (node->name != NULL)
		axl_free (node->name);

	/* free attributes */
	axl_free (node);
	
	/* the node to release */
	return;
}


