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
#ifndef __AXL_DOC_H__
#define __AXL_DOC_H__

#include <axl_decl.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_doc_module
 * @{
 */

axlDoc  * axl_doc_create                   (const char     * version, 
					    const char     * encoding,
					    bool   standalone);

axlDoc  * axl_doc_parse                    (const char * entity, 
					    int entity_size, 
					    axlError ** error);

axlDoc  * axl_doc_parse_strings            (axlError ** error,
					    ...);

axlDoc  * axl_doc_parse_from_file          (const char * file_path,
					    axlError  ** error);

bool      axl_doc_dump                     (axlDoc  * doc, 
					    char   ** content, 
					    int     * size);

bool      axl_doc_dump_pretty              (axlDoc  * doc,
					    char   ** content,
					    int     * size,
					    int       tabular);

bool      axl_doc_dump_to_file             (axlDoc  * doc,
					    char    * file_path);

bool      axl_doc_dump_pretty_to_file      (axlDoc  * doc,
					    char    * file_path,
					    int       tabular);

int       axl_doc_get_flat_size            (axlDoc * doc);

bool      axl_doc_are_equal                (axlDoc * doc, 
					    axlDoc * doc2);

bool      axl_doc_are_equal_trimmed        (axlDoc * doc,
					    axlDoc * doc2);

axlNode * axl_doc_get_root                 (axlDoc * doc);

void      axl_doc_set_root                 (axlDoc * doc, 
					    axlNode * root);

axlList * axl_doc_get_list                 (axlDoc * doc, 
					    char * path_to);

axlNode * axl_doc_get                      (axlDoc * doc, 
					    char * path_to);

char    * axl_doc_get_content_at           (axlDoc * doc,
					    char   * path_to,
					    int    * content_size);

char    * axl_doc_get_encoding             (axlDoc * doc);

bool      axl_doc_get_standalone           (axlDoc * doc);

void      axl_doc_free                     (axlDoc * doc);

void      axl_doc_set_child_current_parent (axlDoc * doc, 
					    axlNode * node);

void      axl_doc_pop_current_parent       (axlDoc * doc);

void      axl_doc_add_pi_target            (axlDoc * doc, 
					    char * target, 
					    char * content);

bool      axl_doc_has_pi_target            (axlDoc * doc, 
					    char * pi_target);

char    * axl_doc_get_pi_target_content    (axlDoc * doc, 
					    char * pi_target);

axlList * axl_doc_get_pi_target_list       (axlDoc * doc);

axlPI   * axl_pi_create                    (char * name,
					    char * content);

axlPI   * axl_pi_copy                      (axlPI  * pi);

bool      axl_pi_are_equal                 (axlPI  * pi, 
					    axlPI * pi2);

char    * axl_pi_get_name                  (axlPI  * pi);

char    * axl_pi_get_content               (axlPI  * pi);

void      axl_pi_free                      (axlPI  * pi);

int       axl_pi_get_size                  (axlPI  * pi);

bool      axl_doc_iterate                  (axlDoc           * doc,
					    AxlIterationMode   mode,
					    axlIterationFunc   func,
					    axlPointer         ptr);

bool      axl_doc_iterate_full             (axlDoc           * doc,
					    AxlIterationMode   mode,
					    axlIterationFunc2  func,
					    axlPointer         ptr,
					    axlPointer         ptr2);

bool      axl_doc_iterate_full_from        (axlDoc           * doc,
					    axlNode          * starting_from,
					    AxlIterationMode   mode,
					    axlIterationFunc2  func,
					    axlPointer         ptr,
					    axlPointer         ptr2);

/* private API starts from here, do not use this API */

bool  axl_doc_consume_comments             (axlDoc    * doc,
					    axlStream * stream, 
					    axlError ** error);

bool  axl_doc_consume_pi                   (axlDoc     * doc, 
					    axlNode    * node,
					    axlStream  * stream, 
					    axlError  ** error);

/* @} */

END_C_DECLS

#endif
