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
	char    * defaultNs;
} axlNsTable;

/** 
 * @internal Function that validates the node against current status
 * of the namespace support.
 */
bool __axl_ns_node_validate (axlNode * node, axlError ** error)
{
	/* check if the node has a namespace declaration */
	
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
