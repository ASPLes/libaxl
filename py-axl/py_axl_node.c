/** 
 *  PyAxl: Axl Library python bindings
 *  Copyright (C) 2022 Advanced Software Production Line, S.L.
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

	/* pointer to the parent document */
	PyObject  * py_doc;
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
	PyAxlNode  * self;
	const char * name = NULL;

	/* create the object */
	self = (PyAxlNode *)type->tp_alloc(type, 0);

	/* parse and get node name */
	if (! PyArg_ParseTuple (args, "|s", &name))
		return NULL;
		
	/* create the node if received a defined name */
	if (name != NULL) {
		self->node         = axl_node_create (name);
		self->finish_on_gc = axl_true;
	}

	return (PyObject *)self;
}

/** 
 * @brief Function used to finish and dealloc memory used by the
 * object axl.Node
 */
static void py_axl_node_dealloc (PyAxlNode* self)

	
{

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Finishing axl.Node reference %p", self);

	/* release node on if it is signaled to be released and it was
	 * not configured to be inside a document, which means the
	 * document will take care of this */
	if (self->finish_on_gc && axl_node_get_doc (self->node) == NULL)
		axl_node_free (self->node);
	self->node = NULL;

	/* decrease reference to the document if defined */
	if (self->py_doc) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "  Finishing axl.Doc reference %p (%d)",
			   self->py_doc, self->py_doc->ob_refcnt);
		Py_DECREF (self->py_doc);
		self->py_doc = NULL;
	}

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

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received request to report node attr name %s (self: %p)",
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
			
		return py_axl_node_create (axl_node_get_first_child (self->node), axl_false, self->py_doc);
	} else if (axl_cmp (attr, "next")) {
		/* next */
		if (axl_node_get_next (self->node) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		} 
			
		return py_axl_node_create (axl_node_get_next (self->node), axl_false, self->py_doc);
	} else if (axl_cmp (attr, "previous")) {
		/* previous */
		if (axl_node_get_previous (self->node) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		} 
			
		return py_axl_node_create (axl_node_get_previous (self->node), axl_false, self->py_doc);
	} else if (axl_cmp (attr, "parent")) {
		/* parent */
		if (axl_node_get_parent (self->node) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		} 
			
		return py_axl_node_create (axl_node_get_parent (self->node), axl_false, self->py_doc);

	} else if (axl_cmp (attr, "content")) {
		/* return a tuple with content and size */
		tuple = PyTuple_New (2);
		
		PyTuple_SetItem (tuple, 0, Py_BuildValue ("z", axl_node_get_content (self->node, &size)));
		PyTuple_SetItem (tuple, 1, Py_BuildValue ("i", size));
			
		return tuple;
	} else if (axl_cmp (attr, "trans")) {
		/* return a tuple with content and size */
		tuple = PyTuple_New (2);
		
		PyTuple_SetItem (tuple, 0, Py_BuildValue ("z", axl_node_get_content_trans (self->node, &size)));
		PyTuple_SetItem (tuple, 1, Py_BuildValue ("i", size));
			
		return tuple;
	} else if (axl_cmp (attr, "doc")) {
		
		/* check if the node has a document configured */
		if (axl_node_get_doc (self->node) == NULL) {
			/* return none */
			Py_INCREF (Py_None);
			return Py_None;
		} /* end if */

		/* ok, return document holding the node */
		return py_axl_doc_create (axl_node_get_doc (self->node), axl_false);
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
	const char      * attr    = NULL;
	const char      * content = NULL;
	PyAxlNode        * self = (PyAxlNode *) o; 
/*	axl_bool          boolean_value = axl_false; */

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return -1;

        if (axl_cmp (attr, "content")) {
		if (! PyArg_Parse (v, "s", &content))
			return -1;		
		/* check if the content has invalid chars */
		axl_node_set_content (self->node, content, -1);
		return 0;
	}

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
	return py_axl_node_create (node, axl_false, self->py_doc);
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
	return py_axl_node_create (node, axl_false, self->py_doc);
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
	return py_axl_node_create (node, axl_false, self->py_doc);
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
	return py_axl_node_create (node, axl_false, self->py_doc);
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
	return py_axl_node_create (node, axl_false, self->py_doc);
}

static PyObject * py_axl_node_has_attr (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	char       * attr_name  = NULL;
	char       * attr_value = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s|s", &attr_name, &attr_value))
		return NULL;

	if (attr_value == NULL)
		return Py_BuildValue ("i", HAS_ATTR (self->node, attr_name));
	return Py_BuildValue ("i", HAS_ATTR_VALUE (self->node, attr_name, attr_value));
}

static PyObject * py_axl_node_attr_value (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	char       * attr_name  = NULL;
	char       * attr_value = NULL;

	if (self->node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s|s", &attr_name, &attr_value))
		return NULL;

	/* check set operation */
	if (PyTuple_Size (args) == 2) {
		/* remove previous attribute if exists */
		axl_node_remove_attribute (self->node, attr_name);

		/* found set operation */
		axl_node_set_attribute (self->node, attr_name, attr_value);
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */
	
	return Py_BuildValue ("z", ATTR_VALUE (self->node, attr_name));
}

static PyObject * py_axl_node_attr_value_trans (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	char       * attr_name  = NULL;
	char       * value;
	PyObject   * result;

	if (self->node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &attr_name))
		return NULL;

	/* get attribute */
	value  = axl_node_get_attribute_value_trans (self->node, attr_name);
	result = Py_BuildValue ("z", value);

	/* free value */
	axl_free (value);
	return result;
}

static PyObject * py_axl_node_str (PyObject * _self)
{
	char      * dump   = NULL;
	PyAxlNode * self   = (PyAxlNode *) _self;
	PyObject  * result = NULL;

	if (self == NULL || self->node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	}
	
	if (! axl_node_dump_pretty (self->node, &dump, NULL, 4)) {
		Py_INCREF (Py_None);
		return Py_None;
	}

	result = Py_BuildValue ("z", dump);
	axl_free (dump);
	return result;
}



static PyObject * py_axl_node_set_child (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	PyObject   * child     = Py_None;

	if (self->node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "O", &child))
		return NULL;

	/* check object received */
	if (! py_axl_node_check (child)) {
		/* set exception */
		PyErr_SetString (PyExc_TypeError, "Expected to receive an axl.Node object but received something different");
		return NULL;
	}

	/* set the node as child */
	axl_node_set_child (self->node, py_axl_node_get (child));

	/* and make the node to not release its internal node when it
	 * is deallocated to avoid the node finishing the reference
	 * that was finished by the document holding */
	if (axl_node_get_doc (py_axl_node_get (_self)) || self->finish_on_gc)
		((PyAxlNode *)(child))->finish_on_gc = axl_false;
	
	/* return ok */
	Py_INCREF (Py_None);
	return Py_None;
}

static PyObject * py_axl_node_set_child_after (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	PyObject   * child     = Py_None;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "O", &child))
		return NULL;

	/* check object received */
	if (! py_axl_node_check (child)) {
		/* set exception */
		PyErr_SetString (PyExc_TypeError, "Expected to receive an axl.Node object but received something different");
		return NULL;
	}

	/* set the node as child */
	axl_node_set_child_after (self->node, py_axl_node_get (child));

	/* and make the node to not release its internal node when it
	 * is deallocated to avoid the node finishing the reference
	 * that was finished by the document holding */
	if (axl_node_get_doc (py_axl_node_get (_self)) || self->finish_on_gc)
		((PyAxlNode *)(child))->finish_on_gc = axl_false;
	
	/* return ok */
	Py_INCREF (Py_None);
	return Py_None;
}

static PyObject * py_axl_node_attr_cursor_new (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	
	/* create node cursor attr */
	return py_axl_attr_cursor_create (axl_node_attr_cursor_new (self->node), axl_true);
}

static PyObject * py_axl_node_remove (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	axl_bool     dealloc   = axl_true;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "|i", &dealloc))
		return NULL;
	
	/* remove the node from the document */
	axl_node_remove (self->node, dealloc);

	/* clean internal reference when dealloc is true */
	self->node = NULL;

	Py_INCREF (Py_None);
	return Py_None;
}

static PyObject * py_axl_node_replace (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;
	PyAxlNode  * new_ref   = NULL;
	axl_bool     dealloc   = axl_true;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "O|i", &new_ref, &dealloc))
		return NULL;

	if (! py_axl_node_check ((PyObject *)new_ref)) {
		/* set exception */
		PyErr_SetString (PyExc_TypeError, "Expected to receive an axl.Node object but received something different");
		return NULL;
	} /* end if */
	
	/* replace node reference */
	axl_node_replace (self->node, new_ref->node, dealloc);

	if (dealloc)
		self->node = NULL;

	/* and make the node to not release its internal node when it
	 * is deallocated to avoid the node finishing the reference
	 * that was finished by the document holding */
	if (axl_node_get_doc (new_ref->node)) 
		new_ref->finish_on_gc = axl_false;

	Py_INCREF (Py_None);
	return Py_None;
}

static PyObject * py_axl_node_deattach (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;

	/* deattach node */
	axl_node_deattach (self->node);
	Py_INCREF (Py_None);
	return Py_None;
}

static PyObject * py_axl_node_set_empty (PyObject * _self, PyObject * args)
{
	PyAxlNode  * self      = (PyAxlNode *) _self;

	/* remove the node from the document */
	axl_node_set_is_empty (self->node, axl_true);
	Py_INCREF (Py_None);
	return Py_None;
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
	 "Allows to get/set the given attribute on the provided node."},
	/* attr_trans */
	{"attr_trans", (PyCFunction) py_axl_node_attr_value_trans, METH_VARARGS,
	 "Allows to get the given attribute on the provided node translate entity references ' < & >."},
	/* set_child */
	{"set_child", (PyCFunction) py_axl_node_set_child, METH_VARARGS,
	 "Allows to configure the provided node as the instance's child."},
	/* set_child_after */
	{"set_child_after", (PyCFunction) py_axl_node_set_child_after, METH_VARARGS,
	 "Allows to configure a new child node placing it after the calling node inside the same level. Both nodes will share parent node."},
	/* attr_cursor_new */
	{"attr_cursor_new", (PyCFunction) py_axl_node_attr_cursor_new, METH_NOARGS,
	 "Allows to create a new attribute cursor object used to iterate over all attributes of a node."},
	/* remove */
	{"remove", (PyCFunction) py_axl_node_remove, METH_VARARGS,
	 "Allows to remove the provided node from its current document. The method also receives an optional Boolean parameter to signal to also finish the internal reference."},
	/* replace */
	{"replace", (PyCFunction) py_axl_node_replace, METH_VARARGS,
	 "Allows to replace current node with the node provided. The method also receives an optional Boolean parameter to signal to also finish the internal reference."},
	/* dettach */
	{"deattach", (PyCFunction) py_axl_node_deattach, METH_NOARGS,
	 "Allows to dettach the current node from its holding document. This operation is useful to prepare node to be moved inside the same document or different documents."},
	/* set_empty */
	{"set_empty", (PyCFunction) py_axl_node_set_empty, METH_NOARGS,
	 "Allows to clear all node content."},
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
    py_axl_node_str,           /* tp_str*/
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
				   axl_bool   finish_on_gc,
				   PyObject * py_doc)
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

	/* get a reference to the document if defined */
	if (py_doc) {
		Py_INCREF (py_doc);
		obj->py_doc = py_doc;
	} /* end if */

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Created axl.Node reference (%p, ref count: %d): %s (doc: %p, refcount: %d)", 
		   obj, obj->ob_refcnt, axl_node_get_name (obj->node), obj->py_doc, obj->py_doc->ob_refcnt);

	return __PY_OBJECT (obj);
}

/** 
 * @internal Allows to get the internal reference used by the provided
 * PyAxlNode.
 */
axlNode    * py_axl_node_get      (PyObject * obj)
{
	/* check object received */
	if (! py_axl_node_check (obj))
		return NULL;

	/* return the node */
	return ((PyAxlNode *)obj)->node;
}

/** 
 * @internal Function used to set the node to be deallocated or not
 * according to the provided dealloc value.
 */
void         py_axl_node_set_dealloc (PyObject * obj, axl_bool dealloc)
{
	PyAxlNode * node = (PyAxlNode *) obj;
	node->finish_on_gc = dealloc;
	return;
}

void        init_axl_node      (PyObject * module)
{
	/* register type */
	if (PyType_Ready(&PyAxlNodeType) < 0)
		return;
	
	Py_INCREF (&PyAxlNodeType);
	PyModule_AddObject(module, "Node", (PyObject *)&PyAxlNodeType);

}


