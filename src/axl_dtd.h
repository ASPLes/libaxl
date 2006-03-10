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
#ifndef __AXL_DTD_H__
#define __AXL_DTD_H__

#include <axl_decl.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_dtd_module
 * @{
 */

axlDtd             * axl_dtd_parse            (char      * entity, 
					       int         entity_size,
					       axlError ** error);

axlDtd             * axl_dtd_parse_from_file  (char * file_path,
					       axlError ** error);

bool                 axl_dtd_validate         (axlDoc * doc, 
					       axlDtd * dtd);

axlDtdElement      * axl_dtd_get_root         (axlDtd * dtd); 

char               * axl_dtd_get_element_name (axlDtdElement * element);

AxlDtdElementType    axl_dtd_get_element_type (axlDtdElement * element);

void                 axl_dtd_free             (axlDtd * dtd);

void                 axl_dtd_element_free     (axlDtdElement * element);

/* @} */

END_C_DECLS

#endif
