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
#include <py_axl.h>
#include <locale.h>

#define LOG_DOMAIN "py-axl"


static PyObject * py_axl_doc_parse (PyObject * self, PyObject * args)
{
	const char * document  = NULL;
	int          size      = -1;
	PyObject   * doc;
	axlDoc     * _doc;
	axlError   * _error     = NULL;
	PyObject   * result;
	PyObject   * error;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s|i", &document, &size))
		return NULL;
	
	/* parse document */
	_doc   = axl_doc_parse (document, size, &_error);
	result = PyTuple_New (2);
	if (_doc == NULL) {
		/* set none document */
		Py_INCREF (Py_None);
		PyTuple_SetItem (result, 0, Py_None);

		/* create error */
		error = py_axl_error_create (_error);
		
		/* set error */
		PyTuple_SetItem (result, 1, error);

		/* return result tuple */
		return result;
	} /* end if */
	
	/* document parsed ok */
	doc = py_axl_doc_create (_doc, axl_true);
	PyTuple_SetItem (result, 0, doc);
	PyTuple_SetItem (result, 1, Py_None);
	Py_INCREF (Py_None);

	return result;
}

static PyObject * py_axl_doc_parse_from_file (PyObject * self, PyObject * args)
{
	const char * path  = NULL;
	PyObject   * doc;
	axlDoc     * _doc;
	axlError   * _error     = NULL;
	PyObject   * result;
	PyObject   * error;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &path))
		return NULL;
	
	/* parse document */
	_doc   = axl_doc_parse_from_file (path, &_error);
	result = PyTuple_New (2);
	if (_doc == NULL) {
		/* set none document */
		Py_INCREF (Py_None);
		PyTuple_SetItem (result, 0, Py_None);

		/* create error */
		error = py_axl_error_create (_error);
		
		/* set error */
		PyTuple_SetItem (result, 1, error);

		/* return result tuple */
		return result;
	} /* end if */
	
	/* document parsed ok */
	doc = py_axl_doc_create (_doc, axl_true);
	PyTuple_SetItem (result, 0, doc);
	PyTuple_SetItem (result, 1, Py_None);
	Py_INCREF (Py_None);

	return result;
}

static PyObject * py_axl_dtd_parse_from_file (PyObject * self, PyObject * args)
{
	const char * path  = NULL;
	PyObject   * dtd;
	axlDtd     * _dtd;
	axlError   * _error     = NULL;
	PyObject   * result;
	PyObject   * error;

	/* parse and check result */
	if (! PyArg_ParseTuple (args, "s", &path))
		return NULL;
	
	/* parse document */
	_dtd   = axl_dtd_parse_from_file (path, &_error);
	result = PyTuple_New (2);
	if (_dtd == NULL) {
		/* set none document */
		Py_INCREF (Py_None);
		PyTuple_SetItem (result, 0, Py_None);

		/* create error */
		error = py_axl_error_create (_error);
		
		/* set error */
		PyTuple_SetItem (result, 1, error);

		/* return result tuple */
		return result;
	} /* end if */
	
	/* document parsed ok */
	dtd = py_axl_dtd_create (_dtd);
	PyTuple_SetItem (result, 0, dtd);
	PyTuple_SetItem (result, 1, Py_None);
	Py_INCREF (Py_None);

	return result;
}

static PyObject * py_axl_version (PyObject * self)
{
	return Py_BuildValue ("s", VERSION);
}

static PyMethodDef py_axl_methods[] = { 
	/* parse */
	{"parse", (PyCFunction) py_axl_doc_parse, METH_VARARGS,
	 "Parse an string provided returning a new reference to (axl.Doc, axl.Error)"},
	/* file_parse */
	{"file_parse", (PyCFunction) py_axl_doc_parse_from_file, METH_VARARGS,
	 "Parse the document found in the file path provided returning a new reference to (axl.Doc, axl.Error)"},
	/* dtd_file_parse */
	{"dtd_file_parse", (PyCFunction) py_axl_dtd_parse_from_file, METH_VARARGS,
	 "Parse the DTD document found in the file path provided returning a new reference to (axl.Dtd, axl.Error)"},
	/* version */
	{"version", (PyCFunction) py_axl_version, METH_NOARGS,
	 "Returns current py-axl version (with is the same as Axl Library)"},
	{NULL, NULL, 0, NULL}   /* sentinel */
}; 

/** 
 * @internal Function that inits all axl modules and classes.
 */ 
PyMODINIT_FUNC  initlibpy_axl (void)
{
	PyObject * module;

	/** 
	 * NOTE: it seems the previous call is not the appropriate way
	 * but there are relevant people that do not think so:
	 *
	 * http://fedoraproject.org/wiki/Features/PythonEncodingUsesSystemLocale
	 *
	 * Our appreciation is that python should take care of the
	 * current system locale to translate unicode content into
	 * const char strings, for those Py_ParseTuple and Py_BuildArg
	 * using s and z, rather forcing people to get into these
	 * hacks which are problematic. 
	 */
	PyUnicode_SetDefaultEncoding ("UTF-8");
	   

	/* call to initilize threading API and to acquire the lock */
	PyEval_InitThreads();

	/* register axl module */
	module = Py_InitModule3 ("libpy_axl", py_axl_methods, 
				 "Base module that implements wrapper functions for base Axl Library");
	if (module == NULL) 
		return;

	/* call to register all axl modules and types */
	init_axl_doc           (module);
	init_axl_error         (module);
	init_axl_node          (module);
	init_axl_attr_cursor   (module);
	init_axl_dtd           (module);
	init_axl_list          (module);
	init_axl_hash          (module);
	init_axl_stack         (module);
	init_axl_stream        (module);
	return;
}

/** 
 * @internal Handler used to notify exception catched and handled by
 * py_axl_handle_and_clear_exception.
 */
PyAxlExceptionHandler py_axl_exception_handler = NULL;

/** 
 * @brief Allows to check, handle and clear exception state.
 */ 
void py_axl_handle_and_clear_exception (void)
{
	PyObject * ptype      = NULL;
	PyObject * pvalue     = NULL;
	PyObject * ptraceback = NULL;
	PyObject * list;
	PyObject * string;
	PyObject * mod;
	int        iterator;
	char     * str;
	char     * str_aux;


	/* check exception */
	if (PyErr_Occurred()) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "found exception...handling..");

		/* fetch exception state */
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		/* import traceback module */
		mod = PyImport_ImportModule("traceback");
		if (! mod) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "failed to import traceback module, printing error to console");
			/* print exception */
			PyErr_Print ();
			goto clean_up;
		} /* end if */

		/* list of backtrace items */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "formating exception: ptype:%p  pvalue:%p  ptraceback:%p",
			       ptype, pvalue, ptraceback);

		/* check ptraceback */
		if (ptraceback == NULL) {
			ptraceback = Py_None;
			Py_INCREF (Py_None);
		} /* end if */

		list     = PyObject_CallMethod (mod, "format_exception", "OOO", ptype,  pvalue, ptraceback);
		iterator = 0;
		if (py_axl_exception_handler)
			str      = axl_strdup ("");
		else
			str      = axl_strdup ("PyAxl found exception inside: \n");
		while (iterator < PyList_Size (list)) {
			/* get the string */
			string  = PyList_GetItem (list, iterator);

			str_aux = str;
			str     = axl_strdup_printf ("%s%s", str_aux, PyString_AsString (string));
			axl_free (str_aux);

			/* next iterator */
			iterator++;
		}

		/* drop a log */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, str);
		if (py_axl_exception_handler) {
			/* remove trailing \n */
			str[strlen (str) - 1] = 0;
			py_axl_exception_handler (str);
		}
		/* free message */
		axl_free (str);

		/* create an empty string \n */
		Py_XDECREF (list);
		Py_DECREF (mod);


	clean_up:
		/* call to finish retrieved vars .. */
		Py_XDECREF (ptype);
		Py_XDECREF (pvalue);
		Py_XDECREF (ptraceback);

	} /* end if */

	/* clear exception */
	PyErr_Clear ();
	return;
}

/** 
 * @brief Allows to configure a handler that will receive the
 * exception message created. 
 *
 * @param handler The handler to configure to receive all exception
 * handling.
 */
void     py_axl_set_exception_handler (PyAxlExceptionHandler handler)
{
	/* configure the handler */
	py_axl_exception_handler = handler;

	return;
}
