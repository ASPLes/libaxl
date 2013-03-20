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
#include <py_axl_attr_cursor.h>

#define LOG_DOMAIN "py-axl-attr-cursor"

struct _PyAxlAttrCursor {
	/* header required to initialize python required bits for
	   every python object */
	PyObject_HEAD

	/* pointer to the axl attr_cursor */
	axlAttrCursor   * attr_cursor;
	axl_bool          finish_on_gc;
};

static int py_axl_attr_cursor_init_type (PyAxlAttrCursor *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

/** 
 * @brief Function used to allocate memory required by the object
 * axl.Attr_Cursor
 */
static PyObject * py_axl_attr_cursor_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyAxlAttrCursor  * self;
	PyObject         * py_node = NULL;

	/* create the object */
	self = (PyAxlAttrCursor *)type->tp_alloc(type, 0);

	/* parse and get attr_cursor name */
	if (! PyArg_ParseTuple (args, "|O", &py_node))
		return NULL;
		
	/* create the attr_cursor if received a defined name */
	if (PyTuple_Size (args) == 1 && py_axl_node_get (py_node)) {
		self->attr_cursor  = axl_node_attr_cursor_new (py_axl_node_get (py_node));
		self->finish_on_gc = axl_true;
	}

	return (PyObject *)self;
}

/** 
 * @brief Function used to finish and dealloc memory used by the
 * object axl.Attr_Cursor
 */
static void py_axl_attr_cursor_dealloc (PyAxlAttrCursor* self)
{

	/* release attr_cursor on if it is signaled to be released and it was
	 * not configured to be inside a document, which means the
	 * document will take care of this */
	if (self->finish_on_gc)
		axl_node_attr_cursor_free (self->attr_cursor);
	self->attr_cursor = NULL;

	/* free the attr_cursor it self */
	self->ob_type->tp_free ((PyObject*)self);

	return;
}

/** 
 * @brief This function implements the generic attribute getting that
 * allows to perform complex member resolution (not merely direct
 * member access).
 */
PyObject * py_axl_attr_cursor_get_attr (PyObject *o, PyObject *attr_name) {
	PyAxlAttrCursor * self   = (PyAxlAttrCursor *) o; 
	const char      * attr   = NULL;
	PyObject        * result = NULL;

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return NULL;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received request to report attr_cursor attr name %s (self: %p)",
		   attr, o);

	if (axl_cmp (attr, "key")) {
		/* cursor current key */
		return Py_BuildValue ("s", axl_node_attr_cursor_get_key (self->attr_cursor));
	} else if (axl_cmp (attr, "value")) {
		/* cursor current value */
		return Py_BuildValue ("s", axl_node_attr_cursor_get_value (self->attr_cursor));
	}

	/* first implement generic attr already defined */
	result = PyObject_GenericGetAttr (o, attr_name);
	if (result)
		return result;
	
	return NULL;
}

static PyObject * py_axl_attr_cursor_first (PyObject * _self, PyObject * args)
{
	PyAxlAttrCursor * self      = (PyAxlAttrCursor *) _self;

	/* call to set first position */
	axl_node_attr_cursor_first (self->attr_cursor);
	
	/* return ok */
	Py_INCREF (Py_None);
	return Py_None;
}

static PyObject * py_axl_attr_cursor_next (PyObject * _self, PyObject * args)
{
	PyAxlAttrCursor * self      = (PyAxlAttrCursor *) _self;

	/* call to set next position */
	axl_node_attr_cursor_next (self->attr_cursor);
	
	/* return ok */
	Py_INCREF (Py_None);
	return Py_None;
}

static PyObject * py_axl_attr_cursor_has_item (PyObject * _self, PyObject * args)
{
	PyAxlAttrCursor * self      = (PyAxlAttrCursor *) _self;

	/* return current status of has_item */
	return Py_BuildValue ("i", axl_node_attr_cursor_has_item (self->attr_cursor));
}

static PyObject * py_axl_attr_cursor_has_next (PyObject * _self, PyObject * args)
{
	PyAxlAttrCursor * self      = (PyAxlAttrCursor *) _self;

	/* return current status of has_next */
	return Py_BuildValue ("i", axl_node_attr_cursor_has_next (self->attr_cursor));
}

static PyMethodDef py_axl_attr_cursor_methods[] = { 
	/* first */
	{"first", (PyCFunction) py_axl_attr_cursor_first, METH_NOARGS,
	 "Set the cursor to the first position."},
	/* next */
	{"next", (PyCFunction) py_axl_attr_cursor_next, METH_NOARGS,
	 "Set the cursor to the next position."},
	/* has_item */
	{"has_item", (PyCFunction) py_axl_attr_cursor_has_item, METH_NOARGS,
	 "Allows to check if the cursor is pointing to an item after calling to next() or first() or if it had iterated over all attributes."},
	/* has_next */
	{"has_next", (PyCFunction) py_axl_attr_cursor_has_next, METH_NOARGS,
	 "Allows to check if the cursor has a next item to check or if it had iterated over all attributes."},
 	{NULL}  
}; 

static PyTypeObject PyAxlAttrCursorType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size*/
    "axl.Attr_Cursor",                 /* tp_name*/
    sizeof(PyAxlAttrCursor),       /* tp_basicsize*/
    0,                         /* tp_itemsize*/
    (destructor)py_axl_attr_cursor_dealloc, /* tp_dealloc*/
    0,                         /* tp_print*/
    0,                         /* tp_getattr*/
    0,                         /* tp_setattr*/
    0,                         /* tp_compare*/
    0,                         /* tp_repr*/
    0,                         /* tp_as_number*/
    0,                         /* tp_as_sequence*/
    0,                         /* tp_as_mapping*/
    0,                         /* tp_hash */
    0,                         /* tp_call*/
    0,                         /* tp_str*/
    py_axl_attr_cursor_get_attr,    /* tp_getattro*/
    0,                         /* tp_setattro*/
    0,                         /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags*/
    "Attr_Cursor object; wrapper of axlAttrCursor API type",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    py_axl_attr_cursor_methods,     /* tp_methods */
    0, /* py_axl_attr_cursor_members, */     /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)py_axl_attr_cursor_init_type,      /* tp_init */
    0,                         /* tp_alloc */
    py_axl_attr_cursor_new,         /* tp_new */

};


/** 
 * @brief Allows to check if the PyObject received represents a
 * PyAxlAttrCursor reference.
 */
axl_bool             py_axl_attr_cursor_check    (PyObject          * obj)
{
	/* check null references */
	if (obj == NULL)
		return axl_false;

	/* return check result */
	return PyObject_TypeCheck (obj, &PyAxlAttrCursorType);
}

/** 
 * @brief Allows to create a new PyAxlAttrCursor instance using the copy
 * provided. 
 *
 * @param attr_cursor The xml attr_cursor that will represents the python instance.
 *
 * @param finish_on_gc Signal to finish or not the internal copy once
 * the variable gets garbage collected.
 */
PyObject   * py_axl_attr_cursor_create   (axlAttrCursor  * attr_cursor, 
					  axl_bool   finish_on_gc)
{
	/* return a new instance */
	PyAxlAttrCursor * obj = (PyAxlAttrCursor *) PyObject_CallObject ((PyObject *) &PyAxlAttrCursorType, NULL); 

	/* check ref created */
	if (obj == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "Failed to create PyAxlAttrCursor object, returning NULL");
		return NULL;
	} /* end if */

	/* set attr_cursor if defined */
	if (attr_cursor)
		obj->attr_cursor = attr_cursor;
	obj->finish_on_gc = finish_on_gc;

	return __PY_OBJECT (obj);
}

/** 
 * @internal Allows to get the internal reference used by the provided
 * PyAxlAttrCursor.
 */
axlAttrCursor    * py_axl_attr_cursor_get      (PyObject * obj)
{
	/* check object received */
	if (! py_axl_attr_cursor_check (obj))
		return NULL;

	/* return the attr_cursor */
	return ((PyAxlAttrCursor *)obj)->attr_cursor;
}

void        init_axl_attr_cursor      (PyObject * module)
{
	/* register type */
	if (PyType_Ready(&PyAxlAttrCursorType) < 0)
		return;
	
	Py_INCREF (&PyAxlAttrCursorType);
	PyModule_AddObject(module, "AttrCursor", (PyObject *)&PyAxlAttrCursorType);
}


