/**
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
#include <axl_decl.h>
#include <axl.h>

#define LOG_DOMAIN "axl-dtd"

typedef enum {
	CHOICE        = 1,
	SEQUENCE      = 2,
	LEAF_NODE     = 3,
	LEAF_PCDATA   = 4
}AxlDtdNestedType;

typedef struct _axlDtdElementList {
	/** 
	 * @brief Allows to configure how is given top level
	 * configuration for nodes to be defined inside the xml
	 * document being configured. As defined in the XML 1.0
	 * Recomendation, available top level choices are: choice or
	 * sequence. 
	 *
	 * They allow to configure allowed nodes to be selected as
	 * childs, from a set of node names, called choice or to
	 * configure which are the set of nodes to be used, in a
	 * particular order, called sequence.
	 *
	 * This variable allows to configure which is the top level
	 * section configuration: either a choice or a sequence.
	 *
	 * Keep in mind that, having only one element inside the
	 * itemList, there is no difference between the sequence and
	 * the choice.
	 */
	AxlDtdNestedType      type;
	
	/** 
	 * @brief Allows to configure how many times is repeated a
	 * selection provided (by this element).
	 */
	AxlDtdTimes           times;

	/** 
	 * @brief Item list, which contains more axlDtdElementList
	 * nodes, configuring elements allowed.
	 */
	axlList             * itemList;
}axlDtdElementList;

struct _axlDtdElement {
	/** 
	 * @brief The document type element declaration name. This is
	 * the name of the xml node being constrained.
	 */
	char                * name;

	/** 
	 * @brief This is the type of the xml node being constrained.
	 */
	AxlDtdElementType     type;
	
	/** 
	 * @brief List of available items.
	 * 
	 * This variable holds current top level list selection. See
	 * axlDetdElementList.choice variable.
	 */
	axlList             * list;

};

struct _axlDtd {
	/** 
	 * @brief All elements inside the DTD declaration.
	 */
	axlList       * elements;
	/** 
	 * @brief The element root, for the given DTD declaration.
	 */
	axlDtdElement * root;
};

/**
 * \defgroup axl_dtd_module Axl DTD Functions: Document type declaration functions, validation, and DTD parsing
 */


/** 
 * \addtogroup axl_dtd_module
 * @{
 */


/** 
 * @internal
 *
 * @brief Support function to \ref axl_dtd_parse which creates a new
 * empty DTD reference.
 * 
 * 
 * @return A newly allocated \ref axlDtd reference.
 */
axlDtd * __axl_dtd_new ()
{
	axlDtd * dtd;
	
	dtd           = axl_new (axlDtd, 1);
	dtd->elements = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_dtd_element_free);

	return dtd;
}

/** 
 * @internal 
 *
 * @brief Adds the axlDtdElement into the given axlDtd definition,
 * checking that everything is properly configured, and ensuring that
 * the root element gets properly configured.
 * 
 * @param dtd The \ref axlDtd object that will receive the
 * axlDtdElement.
 *
 * @param stream The \ref axlStream object that will be destroyed if
 * something wrong is found.
 *
 * @param element The axlDtdElement to be added to the give axlDtd
 * object.
 * 
 * @return AXL_TRUE if the given axlDtdElement is compatible inside
 * the axlDtd declaration or AXL_FALSE if a error is found.
 */
bool __axl_dtd_add_element (axlDtd * dtd, axlStream * stream, axlDtdElement * element)
{
	/* check the basic case */
	if (dtd->root == NULL) {
		/* set as the root node the one received */
		dtd->root     = element;

		/* add the node found */
		axl_list_add (dtd->elements, element);

		return AXL_TRUE;
	}

	/* check that the is not element already named like the
	 * element received. If it is the case drop an error */
	

	/* check that the element receive is not a top level one for
	 * the set already defined */
	return AXL_TRUE;
}

/** 
 * @internal
 *
 * Parses a document type element that it is expected to be found at
 * the given stream.
 * 
 * @param dtd The axlDtd where the element type readed must be added.
 *
 * @param stream The stream where the element type if expected to be found.
 *
 * @param error An axlError, optional, reference where error will be
 * reported.
 * 
 * @return AXL_TRUE if the element was parsed properly, AXL_FALSE if
 * not. The stream associated will be unrefered and the axlError
 * provided will be filled if an error is found.
 */
bool __axl_dtd_parse_element (axlDtd * dtd, axlStream * stream, axlError ** error)
{
	char           * string_aux;
	int              matched_chunk = -1;
	axlDtdElement  * element;

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* get for the first element declaration */
	if (! (axl_stream_inspect (stream, "<!ELEMENT") > 0)) {
		axl_error_new (-1, "Expected to receive a <!ELEMENT, but it wasn't found", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);
	
	/* get the element name */
	string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, AXL_TRUE, 2, "?>", " ");
	if (string_aux == NULL) {
		axl_error_new (-1, "Expected to receive a DTD element name for <!ELEMENT declaration, but not found", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}

	/* check that the DTD have an element name and an element type */
	if (matched_chunk == 0) {
		axl_error_new (-1, "Found a DTD <!ELEMENT declaration, without content specification. Missing value, examples: EMPTY, ANY, (..)", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}
	
	/* create the DTD element */
	element           = axl_new (axlDtdElement, 1);
	element->name     = axl_strdup (string_aux);
	element->list     = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_dtd_element_free);

	/* now, check for the basic cases: ANY and EMPTY */
	if (axl_stream_peek (stream, "EMPTY") > 0) {
		/* accept previous peek */
		axl_stream_accept (stream);

		/* found empty declaration */
		element->type = ELEMENT_TYPE_EMPTY;

	}else if (axl_stream_peek (stream, "ANY") > 0) {
		/* accept previous peek */
		axl_stream_accept (stream);

		/* found any declaration */
		element->type = ELEMENT_TYPE_ANY;
	}else {
		/* complex element type declaration, let's roll */
		/* now get the element content type */
		while (axl_stream_remains (stream)) {
		
			/* consume previous white spaces */
			AXL_CONSUME_SPACES (stream);

			/* case not supported */
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "case not supported");
		}
	}

	/* add element found */
	if (! __axl_dtd_add_element (dtd, stream, element))
		return AXL_FALSE;
	
	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* check for the last DTD declaration */
	if (! (axl_stream_inspect (stream, ">"))) {
		axl_error_new (-1, "Unable to find last, > terminator for the DTD <!ELEMENT declaration", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}
	
	/* element type declaration completely read */
	return AXL_TRUE;
}


/** 
 * @internal
 * 
 * Implements DTD parsing, reading it from a direct buffer, or a file
 * path or a file handle.
 */
axlDtd * __axl_dtd_parse_common (char * entity, int entity_size, 
				 char * file_path, int fd_handle, 
				 axlError ** error)
{
	axlStream * stream;
	axlDtd    * dtd;
	int         iterator;
	
	/* create the stream associated */
	stream = axl_stream_new (entity, entity_size, file_path, fd_handle, error);
	axl_return_val_if_fail (stream, NULL);

	dtd    = __axl_dtd_new ();
	axl_stream_link (stream, dtd, (axlDestroyFunc) axl_dtd_free);

	iterator = 0;
	while (axl_stream_remains (stream)) {
		/* get rid from comments found */
		if (! axl_doc_consume_comments (NULL, stream, error))
			return AXL_FALSE;
		
		/* check for element declaration */
		if (axl_stream_peek (stream, "<!ELEMENT") > 0) {
			/* found element declaration */
			if (! __axl_dtd_parse_element (dtd, stream, error))
				return AXL_FALSE;
			continue;
		}

		
		/* stop the loop */
		if (iterator == 3) {
			axl_error_new (-1, "unable to process DTD content, unable to found expected information", stream, error);
			axl_stream_free (stream);
			return NULL;
		}
		iterator++;
	}

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "DTD load COMPLETE");
	axl_stream_unlink (stream);
	axl_stream_free (stream);
	return dtd;
}

/** 
 * @brief Allows to parse the provided entity, which is expected to
 * contain a DTD (Document Type Definition).
 * 
 * @param entity The document type definition to parse.
 *
 * @param entity_size The document size, or -1 to make the function to
 * figure out current size.
 *
 * @param error An optional \ref axlError where errors will be reported.
 * 
 * @return A newly allocated \ref axlDtd that must be deallocated when
 * no longer need with \ref axl_dtd_free. The function could return
 * NULL on failure detected. On that case, it is requred to check \ref
 * axlError variable, if defined.
 */
axlDtd   * axl_dtd_parse (char      * entity, 
			  int         entity_size,
			  axlError ** error)
{

	return __axl_dtd_parse_common (entity, entity_size, NULL, -1, error);
}

/** 
 * @brief Allows to parse the provided DTD definition, which is found
 * on the provided file path.
 * 
 * @param file_path The file path where it is expected to receive a
 * DTD file.
 *
 * @param error An optional \ref axlError reference where all errors found will be reported.
 * 
 * @return A newly allocated \ref axlDtd instance or NULL if it fails.
 */
axlDtd   * axl_dtd_parse_from_file (char * file_path,
				    axlError ** error)
{
	return __axl_dtd_parse_common (NULL, -1, file_path, -1, error);
}

/** 
 * @brief Allows to destroy the provided \ref axlDtd  document.
 * 
 * @param dtd The \ref axlDtd document to destroy.
 */
void       axl_dtd_free  (axlDtd * dtd)
{
	if (dtd == NULL)
		return;

	axl_list_free (dtd->elements);
	axl_free (dtd);

	return;
}


/** 
 * @internal
 *
 * @brief Allows to release the memory hold by the given
 * axlDtdElement.
 * 
 * @param element The axlDtdElement to release.
 */
void       axl_dtd_element_free (axlDtdElement * element)
{
	if (element == NULL)
		return;

	/* free element name */
	axl_free (element->name);

	/* free element list definitions */
	axl_list_free (element->list);
	
	/* free element itself */
	axl_free (element);

	return;
}

/* @} */

