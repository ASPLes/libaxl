/** 
 *  PyAxl: Axl Library python bindings
 *  Copyright (C) 2013 Advanced Software Production Line, S.L.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  For commercial support for XML enabled solutions contact us:
 *          
 *      Postal address:
 *         Advanced Software Production Line, S.L.
 *         C/ Antonio Suarez Nº 10, 
 *         Edificio Alius A, Despacho 102
 *         Alcalá de Henares 28802 (Madrid)
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/axl
 */

#ifndef __PY_AXL_ATTR_CURSOR_H__
#define __PY_AXL_ATTR_CURSOR_H__

/** 
 * @brief Object definition that represents the Python encapsulation
 * for axlAttrCursor type.
 */
typedef struct _PyAxlAttrCursor PyAxlAttrCursor;

#include <py_axl.h>

axl_bool           py_axl_attr_cursor_check    (PyObject * obj);

PyObject         * py_axl_attr_cursor_create   (axlAttrCursor  * attr_cursor, 
						axl_bool   finish_on_gc);

axlAttrCursor    * py_axl_attr_cursor_get      (PyObject * obj);

void               init_axl_attr_cursor        (PyObject * module);

#endif
