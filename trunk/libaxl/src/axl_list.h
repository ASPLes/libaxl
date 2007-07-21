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
#ifndef __AXL_LIST_H__
#define __AXL_LIST_H__

#include <axl_decl.h>

BEGIN_C_DECLS

axlList  * axl_list_new    (axlEqualFunc are_equal, axlDestroyFunc destroy_data);

int        axl_list_equal_string (axlPointer a, axlPointer b);

int        axl_list_equal_int    (axlPointer a, axlPointer b);

int        axl_list_always_return_1 (axlPointer a, axlPointer b);

axlList  * axl_list_copy   (axlList * list, axlDuplicateFunc func);

void       axl_list_add    (axlList * list, axlPointer pointer);

void       axl_list_add_at (axlList * list, axlPointer pointer, int position);

void       axl_list_prepend (axlList * list, axlPointer pointer);

void       axl_list_append  (axlList * list, axlPointer pointer);

void       axl_list_remove (axlList * list, axlPointer pointer);

void       axl_list_unlink (axlList * list, axlPointer pointer);

void       axl_list_remove_first (axlList * list);

void       axl_list_unlink_first (axlList * list);

void       axl_list_remove_last (axlList * list);

void       axl_list_unlink_last (axlList * list);

bool       axl_list_exists (axlList * list, axlPointer pointer);

bool       axl_list_exists_at (axlList * list, axlPointer pointer, int position);

bool       axl_list_is_empty  (axlList * list);

axlPointer axl_list_get_first (axlList * list);

axlPointer axl_list_get_last  (axlList * list);

axlPointer axl_list_get_nth   (axlList * list, int position);

axlPointer axl_list_lookup    (axlList * list, axlLookupFunc func, axlPointer data);

bool       axl_list_find_string (axlPointer element, axlPointer data);

int        axl_list_length (axlList * list);

void       axl_list_free (axlList * list);

/* cursor interface */
axlListCursor * axl_list_cursor_new          (axlList * list);

void            axl_list_cursor_first        (axlListCursor * cursor);

void            axl_list_cursor_last         (axlListCursor * cursor);

void            axl_list_cursor_next         (axlListCursor * cursor);

void            axl_list_cursor_previous     (axlListCursor * cursor);

bool            axl_list_cursor_has_next     (axlListCursor * cursor);

bool            axl_list_cursor_has_previous (axlListCursor * cursor);

bool            axl_list_cursor_has_item     (axlListCursor * cursor);

void            axl_list_cursor_unlink       (axlListCursor * cursor);

void            axl_list_cursor_remove       (axlListCursor * cursor);

axlPointer      axl_list_cursor_get          (axlListCursor * cursor);

axlList       * axl_list_cursor_list         (axlListCursor * cursor);

void            axl_list_cursor_free         (axlListCursor * cursor);

END_C_DECLS

#endif
