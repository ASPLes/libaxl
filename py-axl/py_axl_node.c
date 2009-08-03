/** 
 *  PyAxl: Axl Library python bindings
 *  Copyright (C) 2009 Advanced Software Production Line, S.L.
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
#include <py_axl_node.h>

#define LOG_DOMAIN "py-axl-node"

struct _PyAxlNode {
	/* header required to initialize python required bits for
	   every python object */
	PyObject_HEAD

	/* pointer to the axl node */
	axlNode   * node;
	axl_bool    finish_on_gc;
};

static int py_axl_node_init_type (PyAxlNode *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

/** 
 * @brief Function used to allocate memory required by the object
 * axl.Node
 */
static PyObject * py_axl_node_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyAxlNode *self;

	/* create the object */
	self = (PyAxlNode *)type->tp_alloc(type, 0);

	return (PyObject *)self;
}

/** 
 * @brief Function used to finish and dealloc memory used by the
 * object axl.Node
 */
static void py_axl_node_dealloc (PyAxlNode* self)
{

	/* release node */
	if (self->finish_on_gc) 
		axl_node_free (self->node);
	self->node = NULL;

	/* free the node it self */
	self->ob_type->tp_free ((PyObject*)self);

	return;
}

/** 
 * @brief This function implements the generic attribute getting that
 * allows to perform complex member resolution (not merely direct
 * member access).
 */
PyObject * py_axl_node_get_attr (PyObject *o, PyObject *attr_name) {
	const char      * attr = NULL;
	PyObject        * result;
	PyAxlNode       * self = (PyAxlNode *) o; 
	PyObject        * tuple;
	int               size = 0;

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return NULL;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "received request to report channel attr name %s (self: %p)",
		   attr, o);

	if (axl_cmp (attr, "name")) {
		/* name */
		return Py_BuildValue ("s", axl_node_get_name (self->node));
	} else if (axl_cmp (attr, "first_child")) {
		/* first_child */
		if (axl_node_get_first_child (self->node) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		} 
			
		return Py_BuildValue ("O", py_axl_node_create (axl_node_get_first_child (self->node), axl_false));
	} else if (axl_cmp (attr, "next")) {
		/* next */
		if (axl_node_get_next (self->node) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		} 
			
		return Py_BuildValue ("O", py_axl_node_create (axl_node_get_next (self->node), axl_false));
	} else if (axl_cmp (attr, "previous")) {
		/* previous */
		if (axl_node_get_previous (self->node) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		} 
			
		return Py_BuildValue ("O", py_axl_node_create (axl_node_get_previous (self->node), axl_false));
	} else if (axl_cmp (attr, "parent")) {
		/* parent */
		if (axl_node_get_parent (self->node) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		} 
			
		return Py_BuildValue ("O", py_axl_node_create (axl_node_get_parent (self->node), axl_false));

	} else if (axl_cmp (attr, "content")) {
		/* return a tuple with content and size */
		tuple = PyTuple_New (2);
		
		PyTuple_SetItem (tuple, 0, Py_BuildValue ("z", axl_node_get_content (self->node, &size)));
		PyTuple_SetItem (tuple, 1, Py_BuildValue ("i", size));
			
		return tuple;
	}

	/* first implement generic attr already defined */
	result = PyObject_GenericGetAttr (o, attr_name);
	if (result)
		return result;
	
	return NULL;
}

/** 
 * @brief Implements attribute set operation.
 */
int py_axl_node_set_attr (PyObject *o, PyObject *attr_name, PyObject *v)
{
	const char      * attr = NULL;
/*	PyAxlNode        * self = (PyAxlNode *) o; */
/*	axl_bool          boolean_value = axl_false; */

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return -1;

	/* now implement generic setter */
	return PyObject_GenericSetAttr (o, attr_name, v);
}

static PyObject * py_axl_node_next_called (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	const char * node_name = NULL;
	axlNode    * node = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &node_name))
		return NULL;
	
	/* get next called */
	node = axl_node_get_next_called (self->node, node_name);
	if (node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */
	
	/* create node result */
	return py_axl_node_create (node, axl_false);
}

static PyObject * py_axl_node_previous_called (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	const char * node_name = NULL;
	axlNode    * node = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &node_name))
		return NULL;
	
	/* get previous called */
	node = axl_node_get_previous_called (self->node, node_name);
	if (node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */
	
	/* create node result */
	return py_axl_node_create (node, axl_false);
}

static PyObject * py_axl_node_child_called (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	const char * node_name = NULL;
	axlNode    * node = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &node_name))
		return NULL;
	
	/* get previous called */
	node = axl_node_get_child_called (self->node, node_name);
	if (node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */
	
	/* create node result */
	return py_axl_node_create (node, axl_false);
}

static PyObject * py_axl_node_find_called (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	const char * node_name = NULL;
	axlNode    * node = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &node_name))
		return NULL;
	
	/* get previous called */
	node = axl_node_find_called (self->node, node_name);
	if (node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */
	
	/* create node result */
	return py_axl_node_create (node, axl_false);
}

static PyObject * py_axl_node_nth_child (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	int          position  = -1;
	axlNode    * node = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "i", &position))
		return NULL;
	
	/* get previous called */
	node = axl_node_get_child_nth (self->node, position);
	if (node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */
	
	/* create node result */
	return py_axl_node_create (node, axl_false);
}

static PyObject * py_axl_node_has_attr (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	char       * attr_name = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &attr_name))
		return NULL;
	
	return Py_BuildValue ("i", HAS_ATTR (self->node, attr_name));
}

static PyObject * py_axl_node_attr_value (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	char       * attr_name = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &attr_name))
		return NULL;
	
	return Py_BuildValue ("z", ATTR_VALUE (self->node, attr_name));
}

static PyMethodDef py_axl_node_methods[] = { 
	/* next_called */
	{"next_called", (PyCFunction) py_axl_node_next_called, METH_VARARGS,
	 "Gest the xml node found at the same level, but with the name provided."},
	/* previous_called */
	{"previous_called", (PyCFunction) py_axl_node_previous_called, METH_VARARGS,
	 "Gest the xml node found at the same level, but with the name provided."},
	/* child_called */
	{"child_called", (PyCFunction) py_axl_node_child_called, METH_VARARGS,
	 "Gets the xml child node called as provided. The child is looked up only on direct childs."},
	/* find_called */
	{"find_called", (PyCFunction) py_axl_node_find_called, METH_VARARGS,
	 "Gets the xml child node called as provided. The child is looked up in all childs found starting the parent node."},
	/* nth_child */
	{"nth_child", (PyCFunction) py_axl_node_nth_child, METH_VARARGS,
	 "Allows to get a reference to the child node located at the same level at the nth position"},
	/* has_attr */
	{"has_attr", (PyCFunction) py_axl_node_has_attr, METH_VARARGS,
	 "Allows to check if the provided node has the given attribute."},
	/* attr */
	{"attr", (PyCFunction) py_axl_node_attr_value, METH_VARARGS,
	 "Allows to get the given attribute on the provided node."},
 	{NULL}  
}; 

static PyTypeObject PyAxlNodeType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size*/
    "axl.Node",                 /* tp_name*/
    sizeof(PyAxlNode),       /* tp_basicsize*/
    0,                         /* tp_itemsize*/
    (destructor)py_axl_node_dealloc, /* tp_dealloc*/
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
    py_axl_node_get_attr,    /* tp_getattro*/
    py_axl_node_set_attr,    /* tp_setattro*/
    0,                         /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags*/
    "Node object; wrapper of axlNode API type",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    py_axl_node_methods,     /* tp_methods */
    0, /* py_axl_node_members, */     /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)py_axl_node_init_type,      /* tp_init */
    0,                         /* tp_alloc */
    py_axl_node_new,         /* tp_new */

};


/** 
 * @brief Allows to check if the PyObject received represents a
 * PyAxlNode reference.
 */
axl_bool             py_axl_node_check    (PyObject          * obj)
{
	/* check null references */
	if (obj == NULL)
		return axl_false;

	/* return check result */
	return PyObject_TypeCheck (obj, &PyAxlNodeType);
}

/** 
 * @brief Allows to create a new PyAxlNode instance using the copy
 * provided. 
 *
 * @param node The xml node that will represents the python instance.
 *
 * @param finish_on_gc Signal to finish or not the internal copy once
 * the variable gets garbage collected.
 */
PyObject   * py_axl_node_create   (axlNode  * node, 
				   axl_bool   finish_on_gc)
{
	/* return a new instance */
	PyAxlNode * obj = (PyAxlNode *) PyObject_CallObject ((PyObject *) &PyAxlNodeType, NULL); 

	/* check ref created */
	if (obj == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "Failed to create PyAxlNode object, returning NULL");
		return NULL;
	} /* end if */

	/* set node if defined */
	if (node)
		obj->node = node;
	obj->finish_on_gc = finish_on_gc;

	return __PY_OBJECT (obj);
}

void        init_axl_node      (PyObject * module)
{
	/* register type */
	if (PyType_Ready(&PyAxlNodeType) < 0)
		return;
	
	Py_INCREF (&PyAxlNodeType);
	PyModule_AddObject(module, "Node", (PyObject *)&PyAxlNodeType);

}


