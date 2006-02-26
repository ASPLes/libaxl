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
	bool                  choice;
	
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
	axlDtdElementList   * list;

};

struct _axlDtd {
	axlList * elements;
};

/**
 * \defgroup axl_dtd_module Axl DTD Functions: Document type declaration functions, validation, and DTD parsing
 */


/** 
 * \addtogroup axl_dtd_module
 * @{
 */


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
	return NULL;
}

/** 
 * @brief Allows to destroy the provided \ref axlDtd  document.
 * 
 * @param dtd The \ref axlDtd document to destroy.
 */
void       axl_dtd_free  (axlDtd * dtd)
{
	return;
}


/* @} */

