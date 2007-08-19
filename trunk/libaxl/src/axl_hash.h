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
 *         info@aspl.es - http://fact.aspl.es
 */
#ifndef __AXL_HASH_H__
#define __AXL_HASH_H__

#include <axl_decl.h>

BEGIN_C_DECLS

axlHash       * axl_hash_new          (axlHashFunc    hash,
				       axlEqualFunc   equal);

axlHash       * axl_hash_new_full     (axlHashFunc    hash,
				       axlEqualFunc   equal,
				       int            step);

unsigned int    axl_hash_string       (axlPointer key);

int             axl_hash_equal_string (axlPointer keya, 
				       axlPointer keyb);

unsigned int    axl_hash_int          (axlPointer key);

int             axl_hash_equal_int    (axlPointer keya, 
				       axlPointer keyb);

void            axl_hash_insert       (axlHash    * hash, 
				       axlPointer   key,
				       axlPointer   data);

void            axl_hash_insert_full  (axlHash        * hash,
				       axlPointer       key,
				       axlDestroyFunc   destroy_key,
				       axlPointer       data,
				       axlDestroyFunc   destroy_data);

void            axl_hash_remove       (axlHash    * hash,
				       axlPointer   key);

void            axl_hash_delete       (axlHash    * hash,
				       axlPointer   key);

bool            axl_hash_exists       (axlHash   * hash,
				       axlPointer  key);

axlPointer      axl_hash_get          (axlHash * hash, 
				       axlPointer key);

void            axl_hash_foreach      (axlHash            * hash, 
				       axlHashForeachFunc   func, 
				       axlPointer           user_data);

void            axl_hash_foreach2     (axlHash            * hash, 
				       axlHashForeachFunc2  func, 
				       axlPointer           user_data,
				       axlPointer           user_data2);

void            axl_hash_foreach3     (axlHash            * hash, 
				       axlHashForeachFunc3  func, 
				       axlPointer           user_data,
				       axlPointer           user_data2,
				       axlPointer           user_data3);

void            axl_hash_foreach4     (axlHash            * hash, 
				       axlHashForeachFunc4  func, 
				       axlPointer           user_data,
				       axlPointer           user_data2,
				       axlPointer           user_data3,
				       axlPointer           user_data4);
				
int             axl_hash_items        (axlHash * hash);

axlHash       * axl_hash_copy         (axlHash             * hash,
				       axlHashItemCopy       key_copy,
				       axlHashItemCopy       value_copy);
				       

void            axl_hash_show_status  (axlHash * hash);

typedef void (*axlHashPrintKeyData) (axlPointer key, axlPointer data);

void            axl_hash_show_status_full (axlHash * hash, 
					   axlHashPrintKeyData show_item);


void            axl_hash_free         (axlHash * hash);

/* cursor interface */
axlHashCursor * axl_hash_cursor_new          (axlHash * hash);

void            axl_hash_cursor_first        (axlHashCursor * cursor);

void            axl_hash_cursor_last         (axlHashCursor * cursor);

void            axl_hash_cursor_next         (axlHashCursor * cursor);

bool            axl_hash_cursor_has_next     (axlHashCursor * cursor);

bool            axl_hash_cursor_has_item     (axlHashCursor * cursor);

void            axl_hash_cursor_remove       (axlHashCursor * cursor);

axlPointer      axl_hash_cursor_get_key      (axlHashCursor * cursor);

axlPointer      axl_hash_cursor_get_value    (axlHashCursor * cursor);

axlHash       * axl_hash_cursor_hash         (axlHashCursor * cursor);

void            axl_hash_cursor_free         (axlHashCursor * cursor);
			   

END_C_DECLS

#endif
