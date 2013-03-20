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

#ifndef __PY_AXL_H__
#define __PY_AXL_H__

/* include python devel headers */
#include <Python.h>

/* include axl headers */
#include <axl.h>

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

/** 
 * @brief PyAxl macro used to cast objects to PyObject.
 */
#ifndef __PY_OBJECT
# define __PY_OBJECT(o) ((PyObject *)o)
#endif

void     py_axl_handle_and_clear_exception (void);

typedef void (*PyAxlExceptionHandler) (const char * exception_msg);

void     py_axl_set_exception_handler (PyAxlExceptionHandler handler);

PyMODINIT_FUNC  initlibpy_axl_11 (void);

/* include other modules */
#include <py_axl_doc.h>
#include <py_axl_error.h>
#include <py_axl_node.h>
#include <py_axl_attr_cursor.h>
#include <py_axl_dtd.h>
#include <py_axl_list.h>
#include <py_axl_hash.h>
#include <py_axl_stack.h>
#include <py_axl_stream.h>

#endif
