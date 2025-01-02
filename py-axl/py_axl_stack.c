/** 
 *  PyAxl: Axl Library python bindings
 *  Copyright (C) 2025 Advanced Software Production Line, S.L.
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
#include <py_axl_stack.h>

#define LOG_DOMAIN "py-axl-stack"

struct _PyAxlStack {
	/* header required to initialize python required bits for
	   every python object */
	PyObject_HEAD

	/* pointer to the axl stack */
	axlStack * stack;
};

static int py_axl_stack_init_type (PyAxlStack *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

/** 
 * @brief Function used to allocate memory required by the object
 * axl.Stack
 */
static PyObject * py_axl_stack_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyAxlStack *self;

	/* create the object */
	self = (PyAxlStack *)type->tp_alloc(type, 0);

	return (PyObject *)self;
}

/** 
 * @brief Function used to finish and dealloc memory used by the
 * object axl.Stack
 */
static void py_axl_stack_dealloc (PyAxlStack* self)
{
	/* free the stack it self */
	self->ob_type->tp_free ((PyObject*)self);

	return;
}

/** 
 * @brief This function implements the generic attribute getting that
 * allows to perform complex member resolution (not merely direct
 * member access).
 */
PyObject * py_axl_stack_get_attr (PyObject *o, PyObject *attr_name) {
	const char      * attr = NULL;
	PyObject        * result;
/*	PyAxlStack        * self = (PyAxlStack *) o; */

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return NULL;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received request to report stack attr name %s (self: %p)",
		   attr, o);

	/* first implement generic attr already defined */
	result = PyObject_GenericGetAttr (o, attr_name);
	if (result)
		return result;
	
	return NULL;
}

/** 
 * @brief Implements attribute set operation.
 */
int py_axl_stack_set_attr (PyObject *o, PyObject *attr_name, PyObject *v)
{
	const char      * attr = NULL;
/*	PyAxlStack        * self = (PyAxlStack *) o; */
/*	axl_bool          boolean_value = axl_false; */

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return -1;

	/* now implement generic setter */
	return PyObject_GenericSetAttr (o, attr_name, v);
}

static PyMethodDef py_axl_stack_methods[] = { 
 	{NULL}  
}; 

static PyTypeObject PyAxlStackType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size*/
    "axl.Stack",                 /* tp_name*/
    sizeof(PyAxlStack),       /* tp_basicsize*/
    0,                         /* tp_itemsize*/
    (destructor)py_axl_stack_dealloc, /* tp_dealloc*/
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
    py_axl_stack_get_attr,    /* tp_getattro*/
    py_axl_stack_set_attr,    /* tp_setattro*/
    0,                         /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags*/
    "Stack object; wrapper of axlStack API type",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weakstackoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    py_axl_stack_methods,     /* tp_methods */
    0, /* py_axl_stack_members, */     /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)py_axl_stack_init_type,      /* tp_init */
    0,                         /* tp_alloc */
    py_axl_stack_new,         /* tp_new */

};


/** 
 * @brief Allows to check if the PyObject received represents a
 * PyAxlStack reference.
 */
axl_bool             py_axl_stack_check    (PyObject          * obj)
{
	/* check null references */
	if (obj == NULL)
		return axl_false;

	/* return check result */
	return PyObject_TypeCheck (obj, &PyAxlStackType);
}

void        init_axl_stack      (PyObject * module)
{
	/* register type */
	if (PyType_Ready(&PyAxlStackType) < 0)
		return;
	
	Py_INCREF (&PyAxlStackType);
	PyModule_AddObject(module, "Stack", (PyObject *)&PyAxlStackType);

}


