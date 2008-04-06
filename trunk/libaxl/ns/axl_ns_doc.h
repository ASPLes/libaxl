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
 *         Edificio Alius A, Oficina 102,
 *         C/ Antonio Suarez Nº 10,
 *         Alcalá de Henares 28802 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/xml
 */
#ifndef __AXL_NS_DOC_H__
#define __AXL_NS_DOC_H__

#include <axl_ns.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_ns_doc_module
 * @{
 */

bool axl_ns_doc_validate (axlDoc * doc, axlError ** error);

bool axl_ns_doc_node_check (axlNode    * node, 
			    const char * ns_prefix, 
			    const char * ns);

bool axl_ns_doc_check_default (axlNode    * node, 
			       const char * ns);

/** 
 * @brief Convenience macro that allows to call \ref
 * axl_ns_node_find_called, providing a document, making the parent
 * reference to use the root document node (with namespace support).
 *
 * See \ref axl_ns_node_find_called for more information.
 * 
 * @param doc The document where the node lookup will be produced.
 *
 * @param ns The namespace to use to find the the node using this
 * rules.
 * 
 * @param name The child node name that is looked up.
 * 
 * @return A reference to the node found or NULL if it fails to find
 * the node requested.
 */
#define axl_ns_doc_find_called(doc,ns,name) (axl_ns_node_find_called(axl_doc_get_root(doc),ns,name))

/**
 * @}
 */

END_C_DECLS

#endif


