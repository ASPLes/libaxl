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
#ifndef __AXL_NS_NODE_H__
#define __AXL_NS_NODE_H__

#include <axl_ns.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_ns_node_module
 * @{
 */

bool      axl_ns_node_cmp          (axlNode * node, 
				    const char * ns, 
				    const char * name);

axlNode * axl_ns_node_find_called  (axlNode    * parent,
				    const char * ns,
				    const char * name);

axlNode * axl_ns_node_get_child_called   (axlNode    * parent, 
					  const char * ns,
					  const char * name);

axlNode * axl_ns_node_get_next_called    (axlNode    * node,
					  const char * ns,
					  const char * name);

axlNode * axl_ns_node_get_previous_called (axlNode    * node,
					   const char * ns,
					   const char * name);

/** 
 * @brief Allows to check if an xml node is prefixed (by a xml
 * namespace declaration).
 *
 * See \ref axl_ns_node_is_prefixed.
 * 
 * @param node The node to check.
 * 
 * @return \ref true if prefixed, otherwise \ref false is returned.
 */
#define AXL_IS_PREFIXED(node) (axl_ns_node_is_prefixed(node, NULL))

bool axl_ns_node_is_prefixed  (axlNode * node, 
			       int     * position);

/**
 * @}
 */

END_C_DECLS

#endif
