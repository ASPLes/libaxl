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

#define LOG_DOMAIN "axl-ns-doc"

/** 
 * @internal Name of the key used to access and store the ns table for
 * each node.
 */
#define NS_TABLE "__axl:ns-table"


/** 
 * @internal Table used to hold bindings from prefix declared and
 * namespaces used. It also holds the default namespace declared for a
 * particular node, refering to it and all its content until found
 * another declaration.
 */
typedef struct _axlNsTable {
	/* bindings between prefixes and namespaces */
	axlHash * table;
	
	/* default namespace to be used */
	const char * defaultNs;
} axlNsTable;

void __axl_ns_free_table (axlNsTable * table)
{
	if (table == NULL)
		return;

	/* free table */
	axl_hash_free (table->table);

	/* free node */
	axl_free (table);

	return;
} /* end __axl_ns_free_table */

/* check and install all ns declerations found on this node */
bool __axl_ns_node_check_and_install_ns_decls (axlNode * node, axlAttrCursor * cursor, axlError ** error)
{
	const char    * attr;
	bool            default_found = false;
	axlNsTable    * ns_table = NULL;

	/* for each attribute installed on the node */
	while (axl_node_attr_cursor_has_item (cursor)) {

		/* get the key */
		attr = axl_node_attr_cursor_get_key (cursor);
		if (! axl_memcmp (attr, "xmlns", 5)) {
			/* no xml namespace declaration found, go next */
			axl_node_attr_cursor_next (cursor);
			continue;
		} /* end if */

		/* create the table (without creating the hash for now) */
		if (ns_table == NULL) {
			ns_table = axl_new (axlNsTable, 1);
			
			/* install the table in the node */
			axl_node_annotate_data_full (node, 
						     /* key and its destroy function */
						     NS_TABLE, NULL,
						     /* value and its destroy function */
						     ns_table, (axlDestroyFunc)__axl_ns_free_table);
		} /* end if */
		
		/* check if we have a default xmlns declaration */
		if (strlen (attr) == 5) {
			/* check if there were previous xmlns default declarations */
			if (default_found) {
				/* free cursor */
				axl_node_attr_cursor_free (cursor);
				
				axl_error_new (-1, "Found that the document defines several default xmlns declarations at the same node", NULL, error);
				return false;
			} /* end if */
			default_found = true;
			
			/* copy the default namespace */
			ns_table->defaultNs = axl_node_attr_cursor_get_value (cursor);
			
			/* go next */
			axl_node_attr_cursor_next (cursor);
			continue;
			
		} /* end if */
		
		/* found preffixed namespace declaration */
		if (ns_table->table == NULL) {
			/* create the table with 1 step */
			ns_table->table = axl_hash_new_full (axl_hash_string, axl_hash_equal_string, 1);
		} /* end if */
		
		/* check that the prefix wasn't declared at the same node */
		attr = attr + 6;
		if (axl_hash_exists (ns_table->table, (axlPointer) attr)) {
			/* free cursor */
			axl_node_attr_cursor_free (cursor);
			
			axl_error_new (-1, "Found that the document already declares the same prefix for the same node several times",
				       NULL, error);
			return false;
		} /* end if */
		
		/* install it in the current ns hash table */
		axl_hash_insert (ns_table->table, (axlPointer) attr, (axlPointer) axl_node_attr_cursor_get_value (cursor));

		/* get next attribute */
		axl_node_attr_cursor_next (cursor);

	} /* end while */

	/* all declarations done */
	return true;
	
} /* end __axl_ns_node_check_and_install_ns_decls */

/** 
 * @internal Function that validates the node against current status
 * of the namespace support.
 */
bool __axl_ns_node_validate (axlNode * node, axlError ** error)
{
	int          iterator = 0;
	char       * name;
	bool         found    = false;
	axlNsTable * ns_table;
	axlNode    * parent;
	axlNode    * child;

	/* create the cursor */
	axlAttrCursor * cursor = NULL;

	/* check if the node has a namespace declaration */
	if (axl_node_has_attributes (node)) {

		/* the node has attributes, check if there are some
		 * namespace declaration */
		cursor = axl_node_attr_cursor_new (node);

		/* check and install all ns declerations found on this
		 * node (cursor is dellocated by this function on
		 * error) */
		if (! __axl_ns_node_check_and_install_ns_decls (node, cursor, error))
			return false;

	} /* end if */

	/* now we have all namespace declarations, check that the node
	 * name and its attributes are properly declared */
	name = (char*) axl_node_get_name (node);

	/* check if the node name uses a namespace scope declaration
	 * (prefix:name) */
	while (name[iterator] != 0) {
		if (name[iterator] == ':') {
			/* set the terminator */
			name[iterator] = 0;

			/* namespace node found */
			found = true;
			break;
		}

		/* get next position */
		iterator++;
	} /* end if */
	
	if (found) {
		/* namespace declaration found (this supposes that the
		 * default ns is not used) */
		parent = node;
		while (parent != NULL) {
			/* try to get the ns table having the prefix declaration */
			ns_table = axl_node_annotate_get (parent, NS_TABLE, false);

			/* check the namespace */
			if (ns_table != NULL && axl_hash_exists (ns_table->table, name)) {
				/* prefix found, restore node name */
				name[iterator] = ':';
				break;
			} /* end if */
			
			/* get the parent */
			parent = axl_node_get_parent (parent);
			
			/* no more parents */
			if (parent == NULL) {
				/* prefix not found, restore node name */
				name[iterator] = ':';

				/* free cursor */
				axl_node_attr_cursor_free (cursor);
			
				axl_error_new (-1, "Found prefix declaration for which a namespace binding wasn't found in the node or any parent (xmlns:prefix='ns')",
					       NULL, error);
				return false;
			} /* end if */
		} /* end while */
	} /* end if */

	/* check if the node has a namespace declaration */
	if (axl_node_has_attributes (node)) {
		/* now check attributes */
		axl_node_attr_cursor_first (cursor);
		while (axl_node_attr_cursor_has_item (cursor)) {
			
			/* get the key */
			name = (char*) axl_node_attr_cursor_get_key (cursor);
			if (axl_memcmp (name, "xmlns", 5)) {
				/* skip namespace declarations */
				axl_node_attr_cursor_next (cursor);
				continue;
			} /* end if */
			
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking attribute name='%s'", name);
			
			/* check if the node name uses a namespace scope declaration
			 * (prefix:name) */
			found    = false;
			iterator = 0;
			while (name[iterator] != 0) {
				if (name[iterator] == ':') {
					/* set the terminator */
					name[iterator] = 0;
					
					/* namespace node found */
					found = true;
					break;
				}
				
				/* get next position */
				iterator++;
			} /* end while */
			
			if (found) {
				parent = node;
				while (parent != NULL) {
					/* try to get the ns table having the prefix declaration */
					ns_table = axl_node_annotate_get (parent, NS_TABLE, false);
					
					/* check the namespace */
					if (ns_table != NULL && axl_hash_exists (ns_table->table, name)) {
						/* prefix found, restore attribute name */
						name[iterator] = ':';
						break;
					} /* end if */
					
					/* get the parent */
					parent = axl_node_get_parent (parent);
					
					/* no more parents */
					if (parent == NULL) {
						/* prefix not found, restore attribute name */
						name[iterator] = ':';
						
						/* free cursor */
						axl_node_attr_cursor_free (cursor);
						
						axl_error_new (-1, "Found prefix declaration, for an attribute, for which a namespace binding wasn't found in the node or any parent (xmlns:prefix='ns')",
							       NULL, error);
						return false;
					} /* end if */
				} /* end while */
			} /* end if */
			
			/* next attribute */
			axl_node_attr_cursor_next (cursor);
			
		} /* end while */
	
		/* free cursor */
		axl_node_attr_cursor_free (cursor);

	} /* end if */

	/* do the same for the rest of the child nodes */
	child = axl_node_get_first_child (node);
	while (child != NULL) {
		/* check childs */
		if (! __axl_ns_node_validate (child, error))
			return false;

		/* go next child node */
		child = axl_node_get_next (child);
	} /* end while */

	/* return true, if reached this place */
	return true;
}

/** 
 * @brief Allows to check XML 1.0 namespace rules for the document
 * loaded.
 *
 * XML 1.0 Namespaces definition allows you to define documents that
 * are resistent to tag clashing, making your product to be more
 * usable at any level, being mixed with other products.
 *
 * This function allows you to check a valid xml document, already
 * loaded with the Axl Doc base API (\ref axl_doc_parse_from_file,
 * \ref axl_doc_parse or \ref axl_doc_create), to check if it follows
 * the namespaces contrains. 
 *
 * <i><b>NOTE:</b> providing a document without content (at least one
 * root node configured), will cause the function to return \ref
 * true.</i>
 * 
 * @param doc The document that is being required to be checked
 * against the XML Namespaces 1.0 rules.
 *
 * @param error An optional variable where errors will be reported. If
 * the function returns \ref false, you can call to \ref axl_error_get
 * to get a textual diagnostic.
 * 
 * @return \ref true if the document is namespace-valid, otherwise,
 * \ref false is returned.
 */
bool axl_ns_doc_validate (axlDoc * doc, axlError ** error)
{
	axlNode * node;

	/* check references */
	if (doc == NULL) {
		axl_error_new (-1, "Document provided is a null reference", NULL, error);
		return false;
	} /* end if */

	/* get root document */
	node = axl_doc_get_root (doc);
	if (node == NULL) {
		/* document has no nodes, return true */
		return true;
	} /* end if */
 
	/* call to produce and validate namespace content */
	return __axl_ns_node_validate (node, error);
}

/** 
 * @internal Internal API that allows to check if the provided prefix
 * (ns_prefix) is bound to the provided namespace, using the node
 * provided as a reference for the parent lookup.
 * 
 * @param node The node that is used as reference for the
 * lookup. Starting from this node the lookup will be performed,
 * following with parents.
 *
 * @param ns_prefix The ns prefix to be checked.
 *
 * @param ns The namespace that must be bound the prefix.
 * 
 * @return true if the prefix is bound to the namespace provided,
 * using the node as reference for all declarations found inside the
 * particular node (or its parents).
 */
bool axl_ns_doc_node_check (axlNode    * node, 
			    const char * ns_prefix, 
			    const char * ns)
{
	axlNode    * parent   = node;
	axlNsTable * ns_table = NULL;

	/* foreach node, up to the root parent, starting from the node
	 * provided, do: */
	while (parent != NULL) {
		/* try to get the ns table having the prefix declaration */
		ns_table = axl_node_annotate_get (parent, NS_TABLE, false);
		
		/* check the namespace */
		if (ns_table != NULL && axl_hash_exists (ns_table->table, (axlPointer) ns_prefix)) {
			/* prefix found, check its value */
			return axl_cmp (axl_hash_get (ns_table->table, (axlPointer) ns_prefix), ns);
		} /* end if */
		
		/* not found, get the parent */
		parent = axl_node_get_parent (parent);

	} /* end while */	

	/* reached this point, return false */
	return false;
}

/** 
 * @internal Function that allows to check if the default namespace is
 * the value provided, using as reference, the node received.
 * 
 * @param node The node to check for its default namespace to match
 * the value provided.
 *
 * @param ns The namespace that is provided to match.
 * 
 * @return \ref true if the node has as default namespace the value
 * received. Otherwise \ref false is returned.
 */
bool axl_ns_doc_check_default (axlNode    * node, 
			       const char * ns)
{
	axlNode    * parent   = node;
	axlNsTable * ns_table = NULL;

	/* foreach node, up to the root parent, starting from the node
	 * provided, do: */
	while (parent != NULL) {
		/* try to get the ns table having the prefix declaration */
		ns_table = axl_node_annotate_get (parent, NS_TABLE, false);
		
		/* check the namespace */
		if (ns_table != NULL && ns_table->defaultNs != NULL) {
			/* prefix found, check its value */
			return axl_cmp (ns_table->defaultNs, ns);
		} /* end if */
		
		/* not found, get the parent */
		parent = axl_node_get_parent (parent);

	} /* end while */	

	/* reached this point, return false */
	return false;
}
