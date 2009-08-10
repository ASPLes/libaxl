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
#include <py_axl_doc.h>

#define LOG_DOMAIN "py-axl-doc"

struct _PyAxlDoc {
	/* header required to initialize python required bits for
	   every python object */
	PyObject_HEAD

	/* pointer to the axl doc */
	axlDoc * doc;
	/* release the document on gc */
	axl_bool finish_on_gc;
};

static int py_axl_doc_init_type (PyAxlDoc *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

/** 
 * @brief Function used to allocate memory required by the object
 * axl.Doc
 */
static PyObject * py_axl_doc_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyAxlDoc   * self;
	const char * version     = NULL;
	const char * encoding    = NULL;
	axl_bool     stand_alone = axl_true;

	/* now parse arguments */
	static char *kwlist[] = {"version", "encoding", "standalone", NULL};

	/* create the object */
	self = (PyAxlDoc *)type->tp_alloc(type, 0);

	/* in the case the constructor was called with arguments,
	 * check them */
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|ssi", kwlist, &version, &encoding, &stand_alone))
		return NULL;

	/* create the document */
	self->finish_on_gc = axl_true;
	self->doc          = axl_doc_create (version, encoding, stand_alone);

	/* return reference created */
	return (PyObject *)self;
}

/** 
 * @brief Function used to finish and dealloc memory used by the
 * object axl.Doc
 */
static void py_axl_doc_dealloc (PyAxlDoc* self)
{
	/* free document and nullfy pointer */
	if (self->finish_on_gc)
		axl_doc_free (self->doc);
	self->doc = NULL;

	/* free the node it self */
	self->ob_type->tp_free ((PyObject*)self);

	return;
}

/** 
 * @brief This function implements the generic attribute getting that
 * allows to perform complex member resolution (not merely direct
 * member access).
 */
PyObject * py_axl_doc_get_attr (PyObject *o, PyObject *attr_name) {
	const char      * attr = NULL;
	PyObject        * result;
	PyAxlDoc        * self = (PyAxlDoc *) o; 

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return NULL;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received request to report doc attr name %s (self: %p)",
		   attr, o);
	if (axl_cmp (attr, "encoding")) {
		/* encoding */
		return Py_BuildValue ("s", axl_doc_get_encoding (self->doc));
	} else if (axl_cmp (attr, "standalone") || axl_cmp (attr, "stand_alone")) {
		/* standalone */
		return Py_BuildValue ("i", axl_doc_get_standalone (self->doc));
	} else if (axl_cmp (attr, "root")) {
		if (axl_doc_get_root (self->doc) == NULL) {
			Py_INCREF (Py_None);
			return Py_None;
		}

		/* return root axl node: signaling to not finish the
		 * internal copy once it is collected the reference */
		return py_axl_node_create (axl_doc_get_root (self->doc), axl_false);
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
int py_axl_doc_set_attr (PyObject *o, PyObject *attr_name, PyObject *v)
{
	const char      * attr    = NULL;
	PyAxlDoc        * self    = (PyAxlDoc *) o; 
	PyObject        * py_node = NULL;

	/* now implement other attributes */
	if (! PyArg_Parse (attr_name, "s", &attr))
		return -1;

	if (axl_cmp (attr, "root")) {
		if (! PyArg_Parse (v, "O", &py_node))
			return -1;

		/* configure the node */
		axl_doc_set_root (self->doc, py_axl_node_get (py_node));
		return 0;
	}

	/* now implement generic setter */
	return PyObject_GenericSetAttr (o, attr_name, v);
}

static PyObject * py_axl_doc_get_node (PyObject * _self, PyObject * args)
{
	PyAxlDoc  * self = (PyAxlDoc *) _self;
	const char * path = NULL;
	axlNode    * node = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &path))
		return NULL;
	
	/* get next called */
	node = axl_doc_get (self->doc, path);
	if (node == NULL) {
		Py_INCREF (Py_None);
		return Py_None;
	} /* end if */
	
	/* create node result */
	return py_axl_node_create (node, axl_false);
}

static PyObject * py_axl_doc_has_pi (PyObject * _self, PyObject * args)
{
	PyAxlDoc  * self      = (PyAxlDoc *) _self;
	char      * pi_name   = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &pi_name))
		return NULL;
	
	return Py_BuildValue ("i", axl_doc_has_pi_target (self->doc, pi_name));
}

static PyObject * py_axl_doc_pi_value (PyObject * _self, PyObject * args)
{
	PyAxlDoc  * self    = (PyAxlDoc *) _self;
	char      * pi_name = NULL;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &pi_name))
		return NULL;
	
	return Py_BuildValue ("z", axl_doc_get_pi_target_content (self->doc, pi_name));
}

static PyMethodDef py_axl_doc_methods[] = { 
	/* next_called */
	{"get", (PyCFunction) py_axl_doc_get_node, METH_VARARGS,
	 "Allows to get a node found at a particular path (method wrapper of axl_doc_get)"},
	/* has_pi */
	{"has_pi", (PyCFunction) py_axl_doc_has_pi, METH_VARARGS,
	 "Allows to check if the provided doc has the given process instruction."},
	/* pi */
	{"pi", (PyCFunction) py_axl_doc_pi_value, METH_VARARGS,
	 "Allows to get the value of the given process instruction on the provided doc."},
 	{NULL, NULL, 0, NULL}  
}; 

static PyTypeObject PyAxlDocType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size*/
    "axl.Doc",                 /* tp_name*/
    sizeof(PyAxlDoc),       /* tp_basicsize*/
    0,                         /* tp_itemsize*/
    (destructor)py_axl_doc_dealloc, /* tp_dealloc*/
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
    py_axl_doc_get_attr,    /* tp_getattro*/
    py_axl_doc_set_attr,    /* tp_setattro*/
    0,                         /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags*/
    "Document object; wrapper of axlDoc API type",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    py_axl_doc_methods,     /* tp_methods */
    0, /* py_axl_doc_members, */     /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)py_axl_doc_init_type,      /* tp_init */
    0,                         /* tp_alloc */
    py_axl_doc_new,         /* tp_new */

};


/** 
 * @brief Allows to check if the PyObject received represents a
 * PyAxlDoc reference.
 */
axl_bool             py_axl_doc_check    (PyObject          * obj)
{
	/* check null references */
	if (obj == NULL)
		return axl_false;

	/* return check result */
	return PyObject_TypeCheck (obj, &PyAxlDocType);
}

PyObject   * py_axl_doc_create    (axlDoc * doc, axl_bool finish_on_gc)
{
	/* return a new instance */
	PyAxlDoc * obj = (PyAxlDoc *) PyObject_CallObject ((PyObject *) &PyAxlDocType, NULL); 

	/* check ref created */
	if (obj == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "Failed to create PyAxlDoc object, returning NULL");
		return NULL;
	} /* end if */

	/* configure finish handling */
	obj->finish_on_gc = finish_on_gc;

	/* set doc if defined */
	if (doc) {
		/* clear previous reference */
		axl_doc_free (obj->doc);
		obj->doc = doc;
	}

	return __PY_OBJECT (obj);
}

axlDoc     * py_axl_doc_get      (PyObject * doc)
{
	PyAxlDoc * _doc = (PyAxlDoc *) doc;
	
	if (! py_axl_doc_check (doc))
		return NULL;
	/* return doc reference */
	return _doc->doc;
}

void        init_axl_doc      (PyObject * module)
{
	/* register type */
	if (PyType_Ready(&PyAxlDocType) < 0)
		return;
	
	Py_INCREF (&PyAxlDocType);
	PyModule_AddObject(module, "Doc", (PyObject *)&PyAxlDocType);

}


