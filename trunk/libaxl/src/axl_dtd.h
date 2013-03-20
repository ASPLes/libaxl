/*
 *  LibAxl:  Another XML library
 *  Copyright (C) 2013 Advanced Software Production Line, S.L.
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
#ifndef __AXL_DTD_H__
#define __AXL_DTD_H__

#include <axl_decl.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_dtd_module
 * @{
 */

axlDtd             * axl_dtd_parse            (const char * entity, 
					       int          entity_size,
					       axlError ** error);

axlDtd             * axl_dtd_parse_from_file  (const char * file_path,
					       axlError ** error);

axl_bool             axl_dtd_validate         (axlDoc * doc, 
					       axlDtd * dtd,
					       axlError ** error);

axl_bool             axl_dtd_are_equal        (axlDtd * dtd,
					       axlDtd * dtd2);

axlDtdElement      * axl_dtd_get_root         (axlDtd * dtd); 

axlDtdElement      * axl_dtd_get_element      (axlDtd * dtd, 
					       const char * name);

char               * axl_dtd_get_element_name (axlDtdElement * element);

AxlDtdElementType    axl_dtd_get_element_type (axlDtdElement * element);

axlDtdElementList  * axl_dtd_get_item_list    (axlDtdElement * element);

axl_bool             axl_dtd_element_is_toplevel (axlDtd * dtd, axlDtdElement * element);

int                  axl_dtd_item_list_count  (axlDtdElementList * itemList);

AxlDtdNestedType     axl_dtd_item_list_type   (axlDtdElementList * itemList);

AxlDtdTimes          axl_dtd_item_list_repeat (axlDtdElementList * itemList); 

axlDtdElementListNode * axl_dtd_item_list_get_node (axlDtdElementList * itemList, 
						    int position);

NodeType             axl_dtd_item_node_get_type (axlDtdElementListNode * node);

axlDtdElementList  * axl_dtd_item_node_get_list (axlDtdElementListNode * node);

char               * axl_dtd_item_node_get_value (axlDtdElementListNode * node);

AxlDtdTimes          axl_dtd_item_node_get_repeat (axlDtdElementListNode * node);

axl_bool             axl_dtd_entity_exists    (axlDtd            * dtd, 
					       const char        * name,
					       axlDtdEntityType    type);

char               * axl_dtd_entity_value     (axlDtd            * dtd, 
					       const char        * name,
					       axlDtdEntityType    type);

axlDtdAttribute    * axl_dtd_get_attr         (axlDtd * dtd,
					       const char * nodeName);

int                  axl_dtd_get_attr_contraints (axlDtd * dtd,
						  const char * nodeName);

void                 axl_dtd_free             (axlDtd * dtd);

void                 axl_dtd_element_free     (axlDtdElement * element);

void                 axl_dtd_item_list_free   (axlDtdElementList * list);

axl_bool             axl_dtd_check_entity_ref_and_expand  (axlDtdEntityResolver   resolver, 
							   axlPointer             data,
							   axlStream            * stream, 
							   const           char * prefix,
							   axlError            ** error);


/* DON'T USE THIS FUNCTIONS, PROTOTYPES DECLARATIONS */
axl_bool     __axl_dtd_validate_item_list (axlDtdElementList  * itemList, 
					   axlNode            * parent, 
					   int                * child_pos,
					   axlError          ** error,
					   axl_bool                 top_level);


/* @} */

END_C_DECLS

#endif
