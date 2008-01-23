/*
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
 *         Edificio Alius A, Oficina 102,
 *         C/ Antonio Suarez Nº 10,
 *         Alcalá de Henares 28802 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/xml
 */
#include <axl_decl.h>
#include <axl.h>

#define LOG_DOMAIN "axl-dtd"

struct _axlDtdElementListNode {
	NodeType     type;
	AxlDtdTimes  times;
	axlPointer   data;
};

struct _axlDtdElementList {
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
};

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
	 * axlDtdElementList.type variable.
	 */
	axlDtdElementList   * list;

	/** 
	 * @brief Minimum item list count to be matched while using
	 * this DTD element rule.
	 */
	int                    minimum_match;
};

struct _axlDtdAttributeDecl {
	/** 
	 * @brief Attribute name. This is the attribute value defined
	 * for the node.
	 */
	char                     * name;
	
	/** 
	 * @brief This is the attribute declaration type. It shows if 
	 */
	AxlDtdAttributeType        type;

	/** 
	 * @brief Allows to model whoe is 
	 */
	AxlDtdAttributeDefaults    defaults;
	
	/** 
	 * @brief This is a default value for the <!ATTLIST
	 * declaration received, in the case a FIXED value is required
	 * or a default value is declarted.
	 */
	char                     * default_value;

	/** 
	 * @brief Internal declaration for enum values defined for
	 * this rule. This list is only initialized in the case enum
	 * values are defined.
	 */
	axlList                  * enumvalues;

};

struct _axlDtdAttribute {
	/** 
	 * @brief The document attribute list declaration name. This
	 * is the name of the node that will receive the constrain
	 * defined.
	 */
	char           * name;
	
	/** 
	 * @brief This is the list of constrains defined for the
	 * node. It as list of \ref axlDtdAttributeDecl which defines
	 * the attribute that is declarted, if it is required, and the
	 * type of its content.
	 */
	axlList        * list;
};

struct _axlDtdEntityExternalData {
	/** 
	 * @brief Contains the system literal reference. This is a URI
	 * reference to the resource pointed by the \ref axlDtdEntity
	 * definition.
	 */
	char * system_literal;
	/** 
	 * @brief Contains the public literal information associated
	 * to the entity definition.
	 */
	char * public_literal;
	/** 
	 * @brief Contains the NDATA information (a notation name
	 * reference).
	 */
	char * ndata;
};

struct _axlDtdEntity {
	/** 
	 * @brief Contains the entity name.
	 */
	char           * name;
	
	/** 
	 * @brief Contains the entity type.
	 */
	axlDtdEntityType type;

	/** 
	 * @brief Content of the entity definition ([9] EntityValue).
	 */
	char           * content;
	
	/** 
	 * @brief An entity definition can have a reference to a
	 * external resource. The following pointer contains
	 * information for the external resource pointed.
	 */
	axlDtdEntityExternalData * data;
};

struct _axlDtd {
	/** 
	 * @brief Holds all entity definitions inside the DTD
	 * declaration (<!ENTITY..>).
	 */
	axlList       * entities;

	/** 
	 * @brief All elements inside the DTD declaration
	 * (<!ELEMENT..> ).
	 */
	axlList       * elements;

	/** 
	 * @brief All attribute type declerations inside the DTD
	 * (<!ATTLIST..>)
	 */
	axlList       * attributes;

	/** 
	 * @brief The element root, for the given DTD declaration.
	 */
	axlDtdElement * root;

	/** 
	 * @brief Internal flag that allows to notify that the DTD
	 * contains ID attribute declaration, making the DTD this
	 * references. 
	 */
	bool            haveIdDecl;

	/** 
	 * @brief Flag that the dtd declaration have attributes which
	 * are flaged as IDREF.
	 */
	bool            haveIdRefDecl;
};

/**
 * \defgroup axl_dtd_module Axl DTD: Document type declaration interface (functions, validation, and DTD parsing)
 */


/** 
 * \addtogroup axl_dtd_module
 * @{
 */


/** 
 * @internal
 *
 * Allows to create a new dtd element list item, which represents a
 * content particule inside an item list or a item list. This allows
 * the recursion defined on the XML 1.0 standard.
 *
 * The function receives the node name and a reference list. According
 * to the values the function creates a node which contains a leaf
 * value or a node which contains a reference to the a new list which
 * is nested.
 */
axlDtdElementListNode * __create_axl_dtd_element_list (char * node_name,
						       axlDtdElementList * list)
{
	axlDtdElementListNode * node;

	node = axl_new (axlDtdElementListNode, 1);

	/* create a node element reference */
	if (node_name != NULL) {
		node->data = node_name;
		node->type = AXL_ELEMENT_NODE;
		return node;
	}

	/* create an element list reference */
	if (list != NULL) {
		node->data = list;
		node->type = AXL_ELEMENT_LIST;
		return node;
	}

	/* if another type is requested, return NULL */
	return NULL;
}

/** 
 * @internal
 *
 * Support function used to destroy all items stored on a item list.
 * 
 * @param node 
 */
void __destroy_axl_dtd_element_list (axlDtdElementListNode * node)
{
	if (node == NULL)
		return;
	/* free the reference to the leaf node if defined */
	if (node->type == AXL_ELEMENT_NODE)
		axl_free (node->data);
	
	/* do not do nothing if the reference is not element list */
	if (node->type == AXL_ELEMENT_LIST)
		axl_dtd_item_list_free (node->data);

	/* free de node itself */
	axl_free (node);
	return;
}

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

	/* create the DTD element and nothing else. The rest of items
	 * created on demand */
	dtd           = axl_new (axlDtd, 1);

	return dtd;
}

bool __queue_items (axlPointer data, axlPointer _stack)
{
	axlStack * stack = _stack;

	/* queue the data */
	axl_stack_push (stack, data);

	/* return false to make the function to not stop */
	return false;
}

void __axl_dtd_queue_childs (axlStack * stack, axlNode * parent)
{
	axlNode * child;

	/* get the first child */
	child = axl_node_get_first_child (parent);
	while (child != NULL) {

		/* queue the child */
		axl_stack_push (stack, child);
		
		/* get the next child */
		child = axl_node_get_next (child);
	} /* end while */

	return;
}

/** 
 * @internal
 *
 * Support internal function which allows to queue all items inside an
 * axlDtdElementList to be checked.
 * 
 * @param stack The stack where all data will be placed.
 *
 * @param dtd_element_list The dtd element list where the data will be
 * extracted.
 */
void __axl_dtd_queue_items (axlStack * stack, axlList * list) 
{
	/* call to queue items */
	axl_list_lookup (list, __queue_items, stack);

	/* nothing more */
	return;
}

/** 
 * @internal
 *
 * Support function which allows to check if the provided two dtd
 * elements are in fact, parent and child.
 *
 * DTD element have a parent-child relation based in the fact that the
 * first define top level xml nodes that are followed, in the form of
 * childs nodes, by other DTD elements that define more childs, etc...
 *
 * This function allows to check if the provided parent dtd element
 * have references inside its content specification that proves that
 * it is indeed a parent definition.
 * 
 * @param dtd_element_parent The supposed DTD parent element.
 * @param dtd_element_child  The supposedd DTD child element.
 * 
 * @return true if the function can confirm that the parent-child
 * relation exists, false if not or it could be proved.
 */
bool     __axl_dtd_get_is_parent (axlDtdElement * dtd_element_parent,
				  axlDtdElement * dtd_element_child)
{
	axlStack               * stack;
	axlDtdElementListNode  * node;
	axlDtdElementList      * list;

	/* check for leaf nodes, that, by definition, could be a
	 * parent of nothing. */
	if (dtd_element_parent->list == NULL || dtd_element_parent->list->itemList == NULL) {
		return false;
	}

	/* prepare all elements inside the stack to be checked */
	stack = axl_stack_new (NULL);
	__axl_dtd_queue_items (stack, dtd_element_parent->list->itemList);
	

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "stack size to operate: %d, list: %d", 
		   axl_stack_size (stack),
		   axl_list_length (dtd_element_parent->list->itemList));

	/* now search for a content particule that makes are reference
	 * to the child DTD element */
	do {
		node = axl_stack_pop (stack);
		switch (node->type) {
		case AXL_ELEMENT_NODE:
			/* leaf node case */
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a leaf node, checking it");

			/* seems this is a final node */
			if (axl_cmp (node->data, dtd_element_child->name)) {
				/* seems that the content
				 * specification makes a reference to
				 * the child node. */
				axl_stack_free (stack);
				return true;
			}
			break;
		case AXL_ELEMENT_LIST:
			/* a nested list case */
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a complex node queuing its internal elements, while checking parent=%s for child=%s",
				   dtd_element_parent->name, dtd_element_child->name);
			/* the item list read, is a complex value,
			 * queue all items inside to be inspected */
			list = node->data;
			__axl_dtd_queue_items (stack, list->itemList);
			break;
		case AXL_ELEMENT_NOT_DEFINED:
			/* do nothing */
			break;
		}
		
		/* iterate until all elements are evaluated */
	}while (! axl_stack_is_empty (stack));

	/* deallocates no longer used stack */
	axl_stack_free (stack);
	
	/* either it isn't the parent or it can't be proved. */
	return false;
}


/** 
 * @internal
 * 
 * Support function which allows to get which is the most top root
 * node for the provided set of DTD elements.
 */
axlDtdElement * __axl_dtd_get_new_root (axlDtd * dtd) 
{
	int             iterator;
	bool            change_detected;

	axlDtdElement * dtd_element_aux;
	axlDtdElement * dtd_element_the_root_is_on_fire;

	/* set the very first root node */
	dtd_element_the_root_is_on_fire = axl_list_get_nth (dtd->elements, 0);
	
	do {
		/* check which is the top */
		iterator        = 0;
		change_detected = false;
		while (iterator < axl_list_length (dtd->elements)) {
			
			/* get the next reference */
			dtd_element_aux = axl_list_get_nth (dtd->elements, iterator);
			
			/* check which is the top */
			if (__axl_dtd_get_is_parent (dtd_element_aux,
						     dtd_element_the_root_is_on_fire)) {
				/* it seems that the new element is the root
				 * one, update the reference */
				dtd_element_the_root_is_on_fire = dtd_element_aux;
				change_detected = true;
			}
			
			/* update inner loop iterator */
			iterator ++;
		} /* while end */
	}while (change_detected);

	/* return the root found */
	return dtd_element_the_root_is_on_fire;
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
 * @return true if the given axlDtdElement is compatible inside
 * the axlDtd declaration or false if a error is found.
 */
bool     __axl_dtd_add_element (axlDtd * dtd, axlDtdElement * element, 
			    axlStream * stream, axlError ** error)
{
	int             iterator        = 0;
	axlDtdElement * dtd_element_aux = NULL;

	/* check that there is no element already named like the
	 * element received. If it is the case drop an error */
	while (iterator < axl_list_length (dtd->elements)) {
		dtd_element_aux = axl_list_get_nth (dtd->elements, iterator);
		if (axl_cmp (dtd_element_aux->name, element->name)) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "DTD element for <%s> == <%s> was defined twice", 
				   dtd_element_aux->name, element->name);

			axl_error_new (-1, "Find that an DTD element was defined twice (no more than one time is allowed)", 
				       stream, error);
			axl_stream_free (stream);
			return false;
		}

		/* update current iterator */
		iterator++;
	}
	
	/* add the new DTD element to the list */
	axl_list_add (dtd->elements, element);
	return true;
}

/** 
 * @internal
 * 
 * Internal support function which adds the provided content particule
 * to the dtd item list received. It also perform all operations
 * required for the chunk_matched option received.
 *
 * In the case the function fails to do its work, it will deallocate
 * the stream, filling the error received.
 *
 * According to the chunk matched value, the function will react
 * adding the element and configuring current element list.
 *
 */
bool     __axl_dtd_element_content_particule_add (axlDtdElementList  * dtd_item_list, 
						  char               * string_aux, 
						  int                  chunk_matched, 
						  axlStream          * stream, 
						  axlError          **error)
{
	axlDtdElementListNode * node;

	/* check if the item list was creted or not */
	if (dtd_item_list->itemList == NULL) {
		dtd_item_list->itemList = axl_list_new (axl_list_always_return_1, 
							(axlDestroyFunc) __destroy_axl_dtd_element_list);
	}

	/* create the node to be added */
	node = __create_axl_dtd_element_list (string_aux, NULL);

	/* know add the element found */
	axl_list_add (dtd_item_list->itemList, node);

	/* set configuration for item repetition */
	switch (chunk_matched) {
	case 4:
		/* one or many times */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting one to many repeat pattern: (+)");
		node->times = ONE_OR_MANY;
		break;
	case 5:
		/* zero or many times */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting zero to many repeat pattern: (*)");
		node->times = ZERO_OR_MANY;
		break;
	case 6:
		/* zero or one time */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting one to one repeat pattern: (?)");
		node->times = ZERO_OR_ONE;
		break;
	default:
		/* one and only one time */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting one and only one repeat pattern: ()");
		node->times = ONE_AND_ONLY_ONE;
	}

	/* return that all is ok */
	return true;
}


/** 
 * @internal
 * 
 * @brief Support function which allows to get current repetition
 * configuration.
 * 
 * @param stream The stream where the operation will be performed.
 * 
 * @return Current configuration read, the function will properly work
 * if it is called when it is espected to find a content specification
 * repetition. If not found, the \ref ONE_AND_ONLY_ONE is returned.
 */
AxlDtdTimes __axl_dtd_get_repetition_conf (axlStream * stream)
{
	axl_return_val_if_fail (stream, ONE_AND_ONLY_ONE);

	if (axl_stream_inspect (stream, "?", 1) > 0) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found '?' repetition conf");
		/* seems the content specification could appear zero
		 * or one time */
		return ZERO_OR_ONE;

	} else if (axl_stream_inspect (stream, "+", 1) > 0) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found '+' repetition conf");
		/* seems the content specification must appear one up
		 * to many */
		return ONE_OR_MANY;

	} else if (axl_stream_inspect (stream, "*", 1) > 0) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found '*' repetition conf");
		/* seems the content specification could appear zero
		 * up to many */
		return ZERO_OR_MANY;
	} 

	/* the content specification must appear */
	return ONE_AND_ONLY_ONE;
}

/** 
 * @internal
 *
 * Support function which creates a child item list, insert it to the
 * parent item list received.
 * 
 * @param parent 
 * 
 * @return 
 */
axlDtdElementList * __axl_dtd_create_and_queue (axlDtdElementList * parent)
{
	axlDtdElementList     * child;
	axlDtdElementListNode * node;
	
	/* create the DTD item list */
	child       = axl_new (axlDtdElementList, 1);

	/* make by default the item list to be defined as "not
	 * defined" until the first separator is found */
	child->type = STILL_UNDEF; 
	
	/* create a node that */
	node = __create_axl_dtd_element_list (NULL, child);

	/* create the parent list reference if weren't */
	if (parent->itemList == NULL) {
		parent->itemList = axl_list_new (axl_list_always_return_1, 
						 (axlDestroyFunc) __destroy_axl_dtd_element_list);
	}

	/* add the node */
	axl_list_add (parent->itemList, node);

	/* return the new child created */
	return child;
}

/** 
 * @internal
 *
 * Updates current chunk readed information to allow perform a better
 * code after calling this function.
 *
 */
void __axl_dtd_element_spec_update_chunk_matched (axlStream * stream, 
						  int * chunk_matched)
{
	/* check for for sequence or choice characters */
	if (axl_stream_inspect (stream, ",", 1) > 0) {
		/* flag that we have found a , (choice)
		 * separator */
		(*chunk_matched) = 1;
		
	} else if (axl_stream_inspect (stream, "|", 1) > 0) {
		/* flag that we have found a | (sequence)
		 * separator */
		(*chunk_matched) = 2;

	} else if (axl_stream_inspect (stream, ")", 1) > 0) {
		/* flag that we have found a | (sequence)
		 * separator */
		(*chunk_matched) = 3;
		
	} else if (axl_stream_inspect (stream, "+", 1) > 0) {
		/* flag that we have found a | (sequence)
		 * separator */
		(*chunk_matched) = 4;
		
	} else if (axl_stream_inspect (stream, "*", 1) > 0) {
		/* flag that we have found a | (sequence)
		 * separator */
		(*chunk_matched) = 5;
		
	} else if (axl_stream_inspect (stream, "?", 1) > 0) {
		/* flag that we have found a | (sequence)
		 * separator */
		(*chunk_matched) = 6;
	}

	return;
}

/** 
 * @internal
 *
 * Support function to read the content particule separator once the
 * repeat pattern was found 
 * 
 */
bool     __axl_dtd_element_spec_update_chunk_matched_for_cp_separator (axlStream * stream, 
								   int * chunk_matched)
{
	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* check for for sequence or choice characters */
	if (axl_stream_inspect (stream, ",", 1) > 0) {
		/* flag that we have found a , (choice)
		 * separator */
		(*chunk_matched) = 1;
		return true;
		
	} else if (axl_stream_inspect (stream, "|", 1) > 0) {
		/* flag that we have found a | (sequence)
		 * separator */
		(*chunk_matched) = 2;
		return true;

	} else if (axl_stream_inspect (stream, ")", 1) > 0) {
		/* flag that we have found a | (sequence)
		 * separator */
		(*chunk_matched) = 3;
		return true;
	}
	
	return false;
}

/** 
 * @internal
 *
 * Support function which allows to read the next content particule.
 */
char * __axl_dtd_read_content_particule (axlStream  * stream, 
					 int        * chunk_matched,
					 axlStack   * dtd_item_stack, 
					 axlError  ** error)
{
	char * string_aux;

	/* read the spec particule stopping when a white space
	 * or other character is found */
	string_aux = axl_stream_get_until (stream, NULL, chunk_matched, true, 8, 
					   /* basic, default delimiters: 0, 1, 2, 3 */
					   " ", ",", "|", ")",
					   /* repetition configuration: 4, 5, 6 */
					   "+", "*", "?",
					   /* new dtd item list being opened: 8 */
					   "(");
	if (string_aux == NULL) {
		axl_error_new (-1, "Expected to find a element content specification particule, but it wasn't found",
			       stream, error);
		axl_stack_free (dtd_item_stack);
		axl_stream_free (stream);
		return NULL;
	}
	
	/* check the user doesn't nest item list in a not
	 * proper way */
	if (*chunk_matched == 8) {
		axl_error_new (-1, "Found a not proper nesting item list for a DTD element, before using ( a separator must be used (CHOICE: |, SEQUENCE: ,)",
			       stream, error);
		axl_stack_free (dtd_item_stack);
		axl_stream_free (stream);
		return NULL;
	}

	/* nullify stream internal reference */
	axl_stream_nullify (stream, LAST_CHUNK);
	
	/* return the content particule found */
	return string_aux;
}

/** 
 * @internal
 *
 * Support function which reads current <!ELEMENT specification,
 * configuring it to the received axlDtdElement.
 * 
 * @param stream The stream where the axlDtdElement spec will be read.
 *
 * @param dtd_element The axlDtdElement that will receive the content
 * spec.
 *
 * @param error An optional \ref axlError, where errors will be
 * reported.
 * 
 * @return \ref true if the content spec was properly read or \ref
 * false if not.
 */
bool     __axl_dtd_read_element_spec (axlStream * stream, axlDtdElement * dtd_element, axlError ** error)
{
	char              * string_aux;
	bool                is_pcdata;
	int                 chunk_matched = -1;
	axlStack          * dtd_item_stack;
	axlDtdElementList * dtd_item_list;
	bool                is_empty;
	
	
	/* create the stack used to control which is
	 * the current context for the items read for
	 * the xml DTD especification (pd, pd2, (pr|po), ..) */
	dtd_item_stack = axl_stack_new (NULL);

	/* create the DTD item list */
	dtd_item_list       = axl_new (axlDtdElementList, 1);

	/* by default, set still undef to change it once a separator
	 * is detected or the function ends. This will help to detect
	 * problems produced by people mixing content element
	 * separators. */
	dtd_item_list->type = STILL_UNDEF; 

	/* set the content spec list to the dtd element read */
	dtd_element->list   = dtd_item_list;
	   
	/* push the item created */
	/* axl_stack_push (dtd_item_stack, dtd_item_list); */
	
	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* check that the content specification have an ( */
	if (! (axl_stream_inspect (stream, "(", 1))) {
		axl_error_new (-1, "Expected to find a element content specification opener \"(\", but it wasn't found",
			       stream, error);
		axl_stack_free (dtd_item_stack);
		axl_stream_free (stream);		
		return false;
	}
	
	do {
		/* consume previous white spaces */
		AXL_CONSUME_SPACES (stream);

		/* a new item list have been opened */
		if (axl_stream_inspect (stream, "(", 1) > 0) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a DTD item list openining: %d",
				   axl_stack_size (dtd_item_stack));

			/* a new item list is being defined, we have
			 * to queue current dtd_item_list and create a
			 * new item list */
			axl_stack_push (dtd_item_stack, dtd_item_list);
			
			/* create the DTD item list */
			dtd_item_list        = __axl_dtd_create_and_queue (dtd_item_list);

			/* let's continue at the begining */
			continue;
		}
		

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "iterating again to get a new content particule (item list size: %d)",
			   axl_dtd_item_list_count (dtd_item_list));

		/* read the next content particule: here is the chunk
		 * matched codes found: 
		 * basic, default delimiters: 
		 * 0, 1, 2, 3 -> " ", ",", "|", ")" 
		 * repetition configuration: 
		 * 4, 5, 6  -> "+", "*", "?",
		 * new dtd item list being opened: 
		 * 8 -> "(" */
		string_aux = __axl_dtd_read_content_particule (stream, &chunk_matched, dtd_item_stack, error);
		if (string_aux == NULL)
			return false;
		
		/* check, and record, that the string read is
		 * PCDATA */
		is_pcdata = axl_cmp (string_aux, "#PCDATA");

		/* add the item read if have something defined */

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found content spec particule: (size: %d) '%s'", 
			   strlen (string_aux),
			   string_aux);

		/* check if the have matched a white space: next check is
		 * based on the call to axl_stream_get_until at the caller
		 * function: " " */
		if (chunk_matched == 0) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "found white spaces as delimiter, consuming them (current chunk matched: %d)",
				   chunk_matched);
			
			/* consume previous white spaces */
			AXL_CONSUME_SPACES (stream);

			/* update current chunk_matched to conform to
			 * an stream that have all elements really
			 * close: the following function tries to read
			 * and update chunk_matched variable to point
			 * to the value read for ",", "|", "+", "*",
			 * "?" and ")" because white spaces were found */
			__axl_dtd_element_spec_update_chunk_matched (stream, &chunk_matched);


			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "current chunk matched before update (%d)",
				   chunk_matched);
		}

		/* add the content particule found, this function
		 * already detect that a white space was found and
		 * consumes all white spaces found */
		if (!__axl_dtd_element_content_particule_add (dtd_item_list, string_aux, chunk_matched, stream, error))
			return false;

		if (chunk_matched == 4 || chunk_matched == 5 || chunk_matched == 6) {
			/* found a repetition pattern */
			if (! __axl_dtd_element_spec_update_chunk_matched_for_cp_separator (stream, &chunk_matched)) {
				axl_error_new (-1, "Before a repetition pattern (*,+,?) expected to find a content particule separator",
					       stream, error);
				axl_stack_free (dtd_item_stack);
				axl_stream_free (stream);		
				return false;
			}
		}

		/* set current sequence type accoring to separators
		 * used */
		switch (chunk_matched) {
		case 1:
			if (dtd_item_list->type == CHOICE) {
				axl_error_new (-1, "Detected that the DTD definition is mixing content particules separators at the same level ('|' and ','). First detected a sequence spec (,) but then detected a choice element (|)",
					       stream, error);
				axl_stack_free (dtd_item_stack);
				axl_stream_free (stream);		
				return false;
			}
			dtd_item_list->type = SEQUENCE;
			break;
		case 2:
			if (dtd_item_list->type == SEQUENCE) {
				axl_error_new (-1, "Detected that the DTD definition is mixing content particules separators at the same level ('|' and ','). First detected a choice spec (|) but then detected a sequence element (,)",
					       stream, error);
				axl_stack_free (dtd_item_stack);
				axl_stream_free (stream);		
				return false;
			}
			dtd_item_list->type = CHOICE;
			break;
		}

		/* set element type if a element list terminator was
		 * found ( 3 = ')' = chunk_matched) */
		if ((chunk_matched == 3) && is_pcdata) {
			if (axl_list_length (dtd_item_list->itemList) == 1)
				dtd_element->type = ELEMENT_TYPE_PCDATA;
			else if (axl_list_length (dtd_item_list->itemList) > 1)
				dtd_element->type = ELEMENT_TYPE_MIXED;
		}

		/* pop current element list header */
		if (chunk_matched == 3) {
			do {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a DTD item list termination: stack status: %d",
					   axl_stack_size (dtd_item_stack));
				/* consume previous white spaces */
				AXL_CONSUME_SPACES (stream);
				dtd_item_list->times = __axl_dtd_get_repetition_conf (stream);

				/* consume previous white spaces */
				AXL_CONSUME_SPACES (stream);

				if (axl_stream_inspect (stream, ",", 1) > 0) {
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a sequence (,) separator while reading terminator list");

					chunk_matched = 1;
				}
				else if (axl_stream_inspect (stream, "|", 1) > 0) {
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a choice (|) separator while reading terminator list");
					chunk_matched = 2;
				}
				
				/* this means that a ) was found, we have to
				 * pop current queue */
				is_empty              = axl_stack_is_empty (dtd_item_stack);
				if (! is_empty) {
					dtd_item_list = axl_stack_pop (dtd_item_stack);
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting the next item list in the stack, stack status: %d",
						   axl_stack_size (dtd_item_stack));
				}

				/* special case: check if the next element to
				 * be read is a new ) */
				/* consume previous white spaces */
				AXL_CONSUME_SPACES (stream);

			}while ((axl_stream_inspect (stream, ")", 1) > 0) && !is_empty);

			/* drop a log */
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "terminator sequence status: chunk matched=%d ans stack status: %d",
				   chunk_matched, axl_stack_size (dtd_item_stack));

		}

		/* check if we have finished */
	} while (chunk_matched != 3 || (! axl_stack_is_empty (dtd_item_stack)));


	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "content spec terminated, now lookup for the termination");
		
	/* consume previous white spaces */
	/* AXL_CONSUME_SPACES (stream);*/

	/* read here repetition specification */
	/* dtd_item_list->times = __axl_dtd_get_repetition_conf (stream); */
	
	/* set default content element separator */
	if (dtd_item_list->type == STILL_UNDEF)
		dtd_item_list->type = SEQUENCE;
		
	/* free the stack used */
	axl_stack_free (dtd_item_stack);


	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "DTD content element specification found and parsed ok");


	/* content spec readed properly */
	return true;
}

/** 
 * @internal
 * 
 * Calculates the number of nodes to be matched at minimum for the
 * provided DTD element. 
 * 
 * @param element The DTD element to configure with its minimum item
 * count to be matched.
 */
int __axl_dtd_parse_element_get_compulsory_num (axlDtdElementList * list)
{
	axlDtdElementListNode * itemNode;
	int                     count    = 0;
	int                     iterator = 0;

	/* check for null parameters */
	if (list == NULL)
		return 0;

	/* only count for repetitiong patterns that makes obligatory
	 * to have childs */
	if (list->times == ONE_AND_ONLY_ONE ||
	    list->times == ONE_OR_MANY) {
		
		while (iterator < axl_list_length (list->itemList)) {
			/* get the reference for the item node */
			itemNode = axl_list_get_nth (list->itemList, iterator);
			
			/* check if the repetitiong patter is
			 * compulsory */
			if (itemNode->times == ONE_OR_MANY ||
			    itemNode->times == ONE_AND_ONLY_ONE) {
				/* check if we have an itemNode that has an
				 * Node or a list */
				if (itemNode->type == AXL_ELEMENT_NODE) {
					/* we have an item node */
					count++;
					if (list->type == CHOICE) {
						/* because we have a
						 * choice list, once
						 * validated one item,
						 * it is the minimum
						 * requirement. */
						return count;
					}
				} else {
					/* we have a list */
					count += __axl_dtd_parse_element_get_compulsory_num (itemNode->data);
				}
			}
			
			/* update the index */
			iterator++;
		}
	}
		
	/* return current count */
	return count;
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
 * @return true if the element was parsed properly, false if
 * not. The stream associated will be unrefered and the axlError
 * provided will be filled if an error is found.
 */
bool     __axl_dtd_parse_element (axlDtd * dtd, axlStream * stream, axlError ** error)
{
	char              * string_aux;
	int                 matched_chunk = -1;
	axlDtdElement     * element;

	/* init the dtd element list */
	if (dtd->elements == NULL)
		dtd->elements = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_dtd_element_free);

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* get for the first element declaration */
	if (! (axl_stream_inspect (stream, "<!ELEMENT", 9) > 0)) {
		axl_error_new (-1, "Expected to receive a <!ELEMENT, but it wasn't found", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);
	
	/* get the element name */
	string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, false, 3, ">", "(", " ", "<!ELEMENT");
	if (string_aux == NULL) {
		axl_error_new (-1, "Expected to receive a DTD element name for <!ELEMENT declaration, but not found", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* check that the DTD have an element name and an element type */
	if ((matched_chunk == 0) || (matched_chunk == 3)) {
		axl_error_new (-1, "Found a DTD <!ELEMENT declaration, without content specification. Missing value, examples: EMPTY, ANY, (..)", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* nullify internal stream content */
	axl_stream_nullify (stream, LAST_CHUNK);
	
	/* create the DTD element */
	element           = axl_new (axlDtdElement, 1);
	element->name     = string_aux;

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* now, check for the basic cases: ANY and EMPTY */
	if (axl_stream_peek (stream, "EMPTY", 5) > 0) {
		/* accept previous peek */
		axl_stream_accept (stream);

		/* found empty declaration */
		element->type = ELEMENT_TYPE_EMPTY;

	} else if (axl_stream_peek (stream, "ANY", 3) > 0) {
		/* accept previous peek */
		axl_stream_accept (stream);

		/* found any declaration */
		element->type = ELEMENT_TYPE_ANY;
	} else {
		/* complex element type declaration, let's roll now
		 * get the element content type read current dtd
		 * element spec. 
		 *
		 * By default, any comple element type definition,
		 * have childrens, until PC data definition is found,
		 * which leads to the two possible values: Mixed and
		 * PcData */
		element->type = ELEMENT_TYPE_CHILDREN;
		if (!__axl_dtd_read_element_spec (stream, element, error))
			return false;
	}

	/* add element found */
	if (! __axl_dtd_add_element (dtd, element, stream, error))
		return false;
	
	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* check for the last DTD declaration */
	if (! (axl_stream_inspect (stream, ">", 1))) {
		axl_error_new (-1, "Unable to find last, > terminator for the DTD <!ELEMENT declaration", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* now, count the number of obligatory elements, required for
	 * the validation process */
	element->minimum_match = __axl_dtd_parse_element_get_compulsory_num (element->list);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found DTD element declaration read complete: minimum matching elements: %d",
		   element->minimum_match);

	/* element type declaration completely read */
	return true;
}

/** 
 * @internal
 * 
 * Destroy the provided reference and its associated data.
 * 
 * @param decl The reference declaration.
 */
void axl_dtd_attribute_decl_free (axlDtdAttributeDecl * decl)
{
	/* free the rule name */
	if (decl->name != NULL)
		axl_free (decl->name);

	/* free the default value */
	if (decl->default_value != NULL)
		axl_free (decl->default_value);

	/* free enum declaration list if defined */
	if (decl->enumvalues != NULL)
		axl_list_free (decl->enumvalues);

	/* free the node itself */
	axl_free (decl);
	
	/* nothing more to do */
	return;
}

/** 
 * @internal function to dealloc an single attribute set decleration.
 * 
 * @param attribute The reference to dealloc.
 */
void axl_dtd_attribute_free (axlDtdAttribute * attribute)
{
	/* free the attribute list, name and the node itself */
	axl_free (attribute->name);
	axl_list_free (attribute->list);
	axl_free (attribute);

	return;
}

bool __find_attr_decl (axlPointer _element, axlPointer data)
{
	axlDtdAttributeDecl * decl = _element;
	char                * name = data;

	/* check the name */
	if (axl_cmp (decl->name, name))
		return true;

	/* it is not the element */
	return false;
}

/** 
 * @brief Allows to check if the stream contains a reference to a
 * entity, calling the resolver to get the replacement text to be
 * placed.
 * 
 * @param resolver The function to be called with the replacement
 * text. This function must return the replacement text or NULL if it
 * fails. Failing to return a reference resolution will make the
 * entity reference to appear as is.
 *
 * @param resolver The entity reference resolver function to be called
 * to solve references found.
 *
 * @param data User defined data provided to the function, passed
 * directly to the resolver function once executed.
 *
 * @param stream The stream where the entity reference could appear.
 *
 * @param prefix The reference prefix to recognize. Values allowed
 * are: % (DTD references) and & (general entity references).
 *
 * @param error Optional reference to the axlError to report textual
 * diagnostic errors.
 *
 * @return The function return \ref false if some error while
 * resolving entity references was found. Otherwise the function
 * return true.
 */
bool axl_dtd_check_entity_ref_and_expand (axlDtdEntityResolver   resolver, 
					  axlPointer             data,
					  axlStream            * stream, 
					  const char           * prefix,
					  axlError            ** error)
					  
{
	char       * string_aux;
	char       * new_value;
	int          index;

	/* check if we have an entity reference using the provided prefix */
	index = axl_stream_get_index (stream);
	if (! (axl_stream_inspect (stream, prefix, 1) > 0))
		return true;

	/* get the entity reference until the end */
	string_aux = axl_stream_get_until (stream, NULL, NULL, true, 1, ";");
	if (string_aux == NULL) {
		axl_error_new (-1, "null value received while expecting to find the entity reference to resolve.", stream, error);
		axl_stream_free (stream);
		return false;
	} /* end if */

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found entity reference: %s%s;...resolving", prefix, string_aux);

	/* resolve the reference */
	new_value = (char *) resolver (string_aux, data);
	if (new_value == NULL) {
		axl_stream_move (stream, index);
		return true;
	} /* end if */

	/* accept content consumed */
	axl_stream_accept (stream);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "entity resolved to: %s", new_value);

	/* place the replacement data at the start of the stream */
	new_value = axl_strdup_printf ("%s ", new_value);
	axl_stream_push (stream, new_value, strlen (new_value));
	axl_free (new_value);
	
	return true;
}

/** 
 * @internal Entity resolver used by __axl_dtd_parse_attlist.
 */
const char * __axl_dtd_entity_resolver (const char * entityName, axlPointer data)
{
	/* return the entity resolution */
	return axl_dtd_entity_value ((axlDtd *) data, entityName, PARAMETER_ENTITY);
} /* end if */

axlList * __axl_dtd_parse_enumvalues (const char * _enum_values)
{
	char    ** result;
	int        iterator;
	axlList  * list;

	result   = axl_stream_split (_enum_values, 1, "|");
	iterator = 0;
	list     = axl_list_new (axl_list_always_return_1, axl_free);

	
	while (result[iterator]) {
		/* clean the enum value */
		axl_stream_trim (result[iterator]);

		/* add to the list */
		axl_list_add (list, axl_strdup (result[iterator]));

		/* update the iterator value */
		iterator++;

	} /* end while */
	
	/* free tokens */
	axl_stream_freev (result);
	
	/* return the list */
	return list;
}

/** 
 * @internal function used by \ref axl_dtd_attr_validation function to
 * lookup ATTLIST contraints flaged as unique ID.
 */
bool __find_id_decl (axlPointer _element, axlPointer data)
{
	/* return the comparision */
	return (((axlDtdAttributeDecl *) _element)->type == TOKENIZED_TYPE_ID);
	
} /* end __find_id_decl */


/** 
 * @internal
 * 
 * Parse the <!ATTLIST decleration, registering it into the provided
 * dtd element.
 */
bool __axl_dtd_parse_attlist (axlDtd * dtd, axlStream * stream, axlError ** error)
{
	char                * string_aux    = NULL;
	int                   matched_chunk = -1;
	axlDtdAttribute     * attribute     = NULL;
	axlDtdAttributeDecl * decl          = NULL;
	axlDtdAttributeDecl * declAux       = NULL;
	char                * err_msg;

	/* init the dtd attr list */
	if (dtd->attributes == NULL)
		dtd->attributes = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_dtd_attribute_free);

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* get the element name */
	string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, false, 1, " ");
	if (string_aux == NULL) {
		axl_error_new (-1, "Expected to receive a DTD attribute name for <!ATTLIST declaration, but not found", stream, error);
		axl_stream_free (stream);
		return false;
	}

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found dtd attr declaration for node: <%s>", string_aux);

	/* find the node that holds all attr declarations for the node found */
	attribute         = axl_dtd_get_attr (dtd, string_aux);

	/* check if found */
	if (attribute == NULL) {
		/* create the axlDtdAttribute holder */
		attribute = axl_new (axlDtdAttribute, 1);

		/* record the node to which the list of rules applies */
		axl_stream_nullify (stream, LAST_CHUNK);
		attribute->name   = string_aux;

		/* init the attribute rule list */
		attribute->list   = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_dtd_attribute_decl_free);

		/* now configure this new attribute inside the dtd */
		axl_list_add (dtd->attributes, attribute);
	} /* end if */

	/* now get the list of attributes */
	while (1) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "finding next att declaration");

		/* consume previous white spaces */
		AXL_CONSUME_SPACES (stream);

		/* check if we have finished */
		if (axl_stream_inspect (stream, ">", 1) > 0)
			break;

		/* get the attribute name the rules applies */
		string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, false, 1, " ");
		if (string_aux == NULL) {
			axl_error_new (-1, "Expected to receive an attribute name for <!ATTLIST declaration, but not found", stream, error);
			axl_stream_free (stream);
			return false;
		}

		/* nully the string and store it new rule created */
		axl_stream_nullify (stream, LAST_CHUNK);

		/* create a new attribute single constraint */
		decl            = axl_new (axlDtdAttributeDecl, 1);
		decl->name      = string_aux;

		/* add the attribute constraint to the list */
		axl_list_add (attribute->list, decl);

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "find constraint for attribute name=%s", decl->name);

		/* consume previous white spaces */
		AXL_CONSUME_SPACES (stream);

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking constraint type..");

		/* check for an entity reference and expand the stream
		 * content with its resolution */
		if (! axl_dtd_check_entity_ref_and_expand (__axl_dtd_entity_resolver, dtd, stream, "%", error))
			return false;

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "about to check attr constraint type, stream status: '%s'",
			 axl_stream_get_following (stream, 30));
		
		/* now check the contraint type */
		if (axl_stream_inspect (stream, "NOTATION", 8) > 0) {
			/* parse notation declaration */
		}else if (axl_stream_inspect (stream, "(", 1) > 0) {
			/* parse enum declaration */
			string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, true, 1, ")");
			if (string_aux == NULL) {
				axl_error_new (-1, "expected to find enum declaration but termination caracter ')' was not found", stream, error);
				axl_stream_free (stream);
				return false;
			} /* end if */
			decl->type       = ENUMERATION_TYPE;
			decl->enumvalues = __axl_dtd_parse_enumvalues (string_aux);
		}else {
			/* set the attribute type */
			if (axl_stream_inspect (stream, "CDATA", 5) > 0) {
				decl->type = CDATA_ATTRIBUTE;
			} else if (axl_stream_inspect (stream, "IDREFS", 6) > 0) {
				
				/* flag the type */
				decl->type = TOKENIZED_TYPE_IDREFS;

				/* flag the dtd to have a IDREF declaration */
				dtd->haveIdRefDecl = true;
			} else if (axl_stream_inspect (stream, "IDREF", 5) > 0) {
				/* notify type found */
				decl->type = TOKENIZED_TYPE_IDREF;

				/* flag the dtd to have a IDREF declaration */
				dtd->haveIdRefDecl = true;
				
			} else if (axl_stream_inspect (stream, "ID", 2) > 0) {
				
				/* notify the type found */
				decl->type      = TOKENIZED_TYPE_ID;

				/* flag the dtd to have a ID declaration */
				dtd->haveIdDecl = true;
				
			} else if (axl_stream_inspect (stream, "ENTITY", 6) > 0)
				decl->type = TOKENIZED_TYPE_ENTITY;
			else if (axl_stream_inspect (stream, "ENTITIES", 8) > 0)
				decl->type = TOKENIZED_TYPE_ENTITIES;
			else if (axl_stream_inspect (stream, "NMTOKENS", 8) > 0)
				decl->type = TOKENIZED_TYPE_NMTOKENS;
			else if (axl_stream_inspect (stream, "NMTOKEN", 7) > 0)
				decl->type = TOKENIZED_TYPE_NMTOKEN;
			else {
				axl_error_new (-1, "Unrecognied attr type declaration found, check your <!ATTLIST declaration", stream, error);
				axl_stream_free (stream);
				return false;
			} /* end if */
		} /* end if */

		/* consume previous white spaces */
		AXL_CONSUME_SPACES (stream);

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking default value declaration, stream status: '%s'",
			 axl_stream_get_following (stream, 30));

		/* get default declaration value */
		if (axl_stream_inspect (stream, "#REQUIRED", 9) > 0) {
			decl->defaults = ATT_REQUIRED;
		} else if (axl_stream_inspect (stream, "#IMPLIED", 8) > 0) {
			decl->defaults = ATT_IMPLIED;
		} else {
			decl->defaults = ATT_IMPLIED;
			if (axl_stream_inspect (stream, "#FIXED", 6) > 0) {
				decl->defaults = ATT_FIXED;

				/* consume previous white spaces */
				AXL_CONSUME_SPACES (stream);
			}

			/* check default value for this case */
			if (! (axl_stream_peek (stream, "\"", 1) > 0 ||
			       axl_stream_peek (stream, "'", 1) > 0)) {
				err_msg = axl_strdup_printf ("Unable to find default attribute declaration (#REQUIRED, #IMPLIED, #FIXED)  for attribute %s, node <%s>",
							     decl->name, attribute->name);
				axl_error_new (-1, err_msg, stream, error);
				axl_stream_free (stream);
				axl_free (err_msg);
				return false;
			} /* end if */
		} /* end if */

		/* check constraint for ID types */
		if (decl->type == TOKENIZED_TYPE_ID) {
			/* check that the node doesn't have any unique
			 * id declared */

			/* check if the node have TOKENIZED_TYPE_ID */
			declAux = axl_list_lookup (attribute->list, __find_id_decl, NULL);
			if (declAux != NULL && !axl_cmp (declAux->name, decl->name)) {
				err_msg = axl_strdup_printf ("Found ATTLIST declaration, with several ID declarations <ATTLIST %s %s..",
							     attribute->name, decl->name);
				axl_error_new (-1, err_msg, stream, error);
				axl_stream_free (stream);
				axl_free (err_msg);
				return false;
			} /* end if */
			
			/* check required and implied */
			if (decl->defaults != ATT_REQUIRED && decl->defaults != ATT_IMPLIED) {
				err_msg = axl_strdup_printf ("Found ATTLIST declaration, with ID, that don't have configured either #IMPLICIT or #REQUIRED for attribute %s, node <%s>",
							     decl->name, attribute->name);
				axl_error_new (-1, err_msg, stream, error);
				axl_stream_free (stream);
				axl_free (err_msg);
				return false;
			} /* end if */
		} /* end if */

		/* consume previous white spaces */
		AXL_CONSUME_SPACES (stream);

		/* nullify to check this value later */
		string_aux = NULL;
		if (axl_stream_inspect (stream, "\"", 1) > 0) {
			/* get until */
			string_aux = axl_stream_get_until (stream, NULL, NULL, true, 1, "\"");
		} else if (axl_stream_inspect (stream, "'", 1) > 0) {
			/* get until */
			string_aux = axl_stream_get_until (stream, NULL, NULL, true, 1, "\"");
		} /* end if */

		/* check if default value was found */
		if (string_aux != NULL) {

			/* found default value, check if we have an
			 * enumeration type, enforcing that the value
			 * defined to be inside the enumeration */
			if (decl->type == ENUMERATION_TYPE) {
				if (axl_list_lookup (decl->enumvalues, axl_list_find_string, string_aux) == NULL) {
					axl_error_new (-1, 
						       "Configured a default value for an attribute list which only accepts a set of enum values that do not containt it.",
						       stream, error);
					axl_stream_free (stream);
					return false;
				} /* end if */
			} /* end if */

			/* nullify value and make string_aux to be
			 * owned by the axlDtdAttributeDecl
			 * reference */
			axl_stream_nullify (stream, LAST_CHUNK);
			decl->default_value = string_aux;
		} /* end if */

	} /* end while */
       	
	/* properly parsed */
	return true;
}

/** 
 * @internal
 * 
 * Destroy the provided entity reference and all allocated memory.
 * 
 * @param entity The entity the deallocate.
 */
void axl_dtd_entity_free (axlDtdEntity * entity)
{
	/* the entity reference */
	axl_return_if_fail (entity);

	/* free the entity name */
	if (entity->name)
		axl_free (entity->name);
	
	/* free the content */
	if (entity->content)
		axl_free (entity->content);

	/* free external data if defined */
	if (entity->data) {
		/* free system literal */
		if (entity->data->system_literal)
			axl_free (entity->data->system_literal);
		
		/* free public literal */
		if (entity->data->public_literal)
			axl_free (entity->data->public_literal);

		/* free ndata literal */
		if (entity->data->ndata)
			axl_free (entity->data->ndata);

		/* free the node itself */
		axl_free (entity->data);
	}

	/* free the node */
	axl_free (entity);

	return;
}

/** 
 * @internal
 *
 * Parses an entity definition from the current status of the stream
 * provided.
 */
bool __axl_dtd_parse_entity (axlDtd * dtd, axlStream * stream, axlError ** error)
{
	char         * string_aux;
	int            matched_chunk;
	axlDtdEntity * entity;

	/* init the dtd element list */
	if (dtd->entities == NULL)
		dtd->entities = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_dtd_entity_free);
	
	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* get for the first element declaration */
	if (! (axl_stream_inspect (stream, "<!ENTITY", 8) > 0)) {
		axl_error_new (-1, "Expected to receive a <!ENTITY, but it wasn't found", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* create a new entity */
	entity = axl_new (axlDtdEntity, 1);

	/* set the entity and return true */
	axl_list_add (dtd->entities, entity);

	/* check for parameter entity definition */
	if (axl_stream_inspect (stream, "%", 1) > 0) {
		/* set the entity type */
		entity->type = PARAMETER_ENTITY;
		
		/* consume previous white spaces */
		AXL_CONSUME_SPACES (stream);

	} else
		entity->type = GENERAL_ENTITY;

	/* get the element name */
	string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, false, 1, " ");
	if (string_aux == NULL) {
		axl_error_new (-1, "Expected to receive a DTD entity name for <!ENTITY declaration, but not found", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* set the name */
	axl_stream_nullify (stream, LAST_CHUNK);
	entity->name = string_aux;

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* now check if we have an external reference */
	if (axl_stream_inspect (stream, "PUBLIC", 6) > 0) {
		/* we have a public external resource definition */
		
	}else if (axl_stream_inspect (stream, "SYSTEM", 6) > 0) {
		/* we have a system definition */
		
	}else {
		/* we have a plain value get the content remove next "
		   and ' if defined */
		if (! ((axl_stream_inspect (stream, "\"", 1) > 0))) {
			if (! (axl_stream_inspect (stream, "\'", 1) > 0)) {
				axl_error_new (-2, "Expected to find entity value initiator (\") or ('), every entity value must start with them", 
					       stream, error);
				axl_stream_free (stream);
				return false;
			}
			/* knowing that ' was matched, now get the attribute value */
			string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, true, 1, "'");
		}else {
			/* knowhing that " was matched, now get the attribute value */
			string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, true, 1, "\"");
		}

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "entity value found: [%s]", string_aux);
		
		/* nullify internal reference so we have the
		 * only one reference to entity content value
		 * inside string_aux */
		axl_stream_nullify (stream, LAST_CHUNK);

		/* set the value */
		entity->content = string_aux;
	}

	/* consume previous white spaces */
	AXL_CONSUME_SPACES (stream);

	/* check last item to parse */
	if (! (axl_stream_inspect (stream, ">", 1) > 0)) {
		axl_error_new (-2, "Expected to find entity definition terminator (>), but it wasn't found", 
			       stream, error);
		axl_stream_free (stream);
		return false;
	}

	return true;
}


/** 
 * @internal
 * 
 * Implements DTD parsing, reading it from a direct buffer, or a file
 * path or a file handle.
 */
axlDtd * __axl_dtd_parse_common (const char * entity, int entity_size, 
				 const char * file_path, int fd_handle, 
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
			return NULL;
		
		/* check for element declaration */
		if (axl_stream_peek (stream, "<!ELEMENT", 9) > 0) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found DTD element declaration");
			/* found element declaration */
			if (! __axl_dtd_parse_element (dtd, stream, error))
				return NULL;
			
			continue;

		}

		/* check for attribute list declarations */
		if (axl_stream_inspect (stream, "<!ATTLIST", 9) > 0) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found DTD attribute list declaration");

			/* parse it */
			if (! __axl_dtd_parse_attlist (dtd, stream, error))
				return NULL;
			
			continue;
		}

		/* check for the entity declaration */
		if (axl_stream_peek (stream, "<!ENTITY", 8) > 0) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found DTD entity declaration");

			/* parse the entity definition */
			if (! __axl_dtd_parse_entity (dtd, stream, error))
				return NULL;

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


	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "DTD elements totally loaded, building references..");

	/* update current root reference, the DTD root for the DTD
	 * document already parsed */
	if (dtd->elements != NULL) 
		dtd->root = __axl_dtd_get_new_root (dtd);

	/* check if the DTD has ID declarations if found IDREF
	 * declarations */
	if (! dtd->haveIdDecl && dtd->haveIdRefDecl) {
		axl_error_new (-1, "DTD semantic error, found IDREF attribute declaration but no attribute ID declaration was found.", stream, error);
		axl_stream_free (stream);
		return NULL;
	}
	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "DTD load COMPLETE");

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
axlDtd   * axl_dtd_parse (const char * entity, 
			  int          entity_size,
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
axlDtd   * axl_dtd_parse_from_file (const char * file_path,
				    axlError ** error)
{
	return __axl_dtd_parse_common (NULL, -1, file_path, -1, error);
}


/** 
 * @internal
 * 
 * Support function for axl_dtd_validate which checks if the provided
 * parent have its childs configuration according to the values
 * expresed on the sequenced represented by the itemList.
 *
 * The function return true if the validation was ok, or false
 * if something have failed. It also creates an error, using the
 * optional axlError reference received.
 */
bool     __axl_dtd_validate_sequence (axlNode            * parent, 
				      int                * child_position,
				      axlDtdElementList  * itemList, 
				      axlError          ** error,
				      bool                 try_match,
				      bool                 top_level)
{
	int                      iterator        = 0;
	int                      child_pos       = *child_position;
	axlNode                * node;
	axlDtdElementListNode  * itemNode;
	bool                     status          = false;
	bool                     one_matched;
	AxlDtdTimes              times;
	

	axl_return_val_if_fail (parent, false);
	axl_return_val_if_fail (itemList, false);


	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "validating a sequence list: iterator=%d, item list count=%d, at child position=%d",
		 iterator, axl_dtd_item_list_count (itemList), child_pos);

	/* iterate over the sequence, checking its order */
	while (iterator < axl_dtd_item_list_count (itemList)) {
		
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting next item node from the DTD item list at: %d",
			 iterator);
		
		/* get the item node specification */
		itemNode    = axl_dtd_item_list_get_node (itemList, iterator);
		one_matched = false;
		times       = axl_dtd_item_node_get_repeat (itemNode);

		do {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting node at child position: %d",
				   child_pos);

			/* get the node that is located at the same position
			 * than the sequence */
			if (child_pos < axl_node_get_child_num (parent)) {
				node     = axl_node_get_child_nth (parent, child_pos);
			} else
				node     = NULL;

			/* the node child list have ended, check if
			 * this situation was expected */
			if (node == NULL) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "no more child nodes to validate at %d, for parent: %s, times: %d, iterator: %d, item count: %d",
					   child_pos, axl_node_get_name (parent), times,
					   iterator, axl_dtd_item_list_count (itemList));
				/* check if we were working with a
				 * list, which have matched at least
				 * one item */
				if (times == ONE_OR_MANY && one_matched && status && 
				    ((iterator + 1) == axl_dtd_item_list_count (itemList))) {
					*child_position = child_pos;
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "sequence validated with child position (III): %d", child_pos);
					return true;
				}
				
				/* check that the reset of the
				 * specification item is optional,
				 * including the one used */
				status = true;
				do {
					if (times != ZERO_OR_MANY &&
					    times != ZERO_OR_ONE) {

						__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found item, inside the DTD item list, that is not optional: %d (repeat value: %d)", 
							   iterator, times);
						status = false;
						break;
					}

					/* update index and get the next item */
					iterator++;
					if (iterator < axl_dtd_item_list_count (itemList))
						itemNode = axl_dtd_item_list_get_node (itemList, iterator);
				}while (status && (iterator < axl_dtd_item_list_count (itemList)));

				/* check status before checking the rest of the item spec */
				if (status) {
					*child_position = child_pos;

					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "sequence validated with child position (II): %d", child_pos);

					return true;
				}
				
				/* check if a try match is being runned */
				if (! try_match) {
					axl_error_new (-1, "Found that DTD specifies more nodes to be hold by the parent, but no more childs were found",
						       NULL, error);
				}

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found that no nodes left to satisfy DTD validation operation");
				*child_position = child_pos;
				return false;
			}

			/* check node type */
			if (axl_dtd_item_node_get_type (itemNode) == AXL_ELEMENT_LIST) {

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the item node is an item list, dtd item list position: %d, child position: %d=<%s>",
					 iterator, child_pos, axl_node_get_name (node));

				/* element list found, validate its content */
				if (! __axl_dtd_validate_item_list (axl_dtd_item_node_get_list (itemNode),
								    parent, &child_pos, error, false)) {
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "sub item list validation have failed (not critical)");
					/* check if we are the top
					 * level list and the itemNode
					 * checked is the last one
					 * item on the item list */
					if (top_level && ((iterator + 1) == axl_node_get_child_num (parent))) {
						__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "found that the last item list wasn't matched");
						
					}

					*child_position = child_pos;
					return false;
				}

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "validated item list, child position after: %d",
					   child_pos);
				/* because child position updating and
				 * repeat matching is already handled
				 * by dtd_validate_item_list function
				 * we just continue with the next
				 * iteration */
				break;

			} else if (axl_dtd_item_node_get_type (itemNode) == AXL_ELEMENT_NODE) {
				/* check the name against the spec */

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
					   "the item node is a final content particule definition: %s",
					   axl_dtd_item_node_get_value (itemNode));

				status = NODE_CMP_NAME (node, axl_dtd_item_node_get_value (itemNode));
			}

			/* check previous status */
			if ((times == ONE_AND_ONLY_ONE) || 
			    (times == ONE_OR_MANY && one_matched == false)) {
				if (! status) {
					/* only report an upper level
					 * error if we are not running
					 * a try match */
					if (! try_match) {

						__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, 
							   "Found different node (<%s>) for a sequence expected (<%s>), at child position: %d, item list pos: %d",
							   axl_node_get_name (node), 
							   axl_dtd_item_node_get_value (itemNode),
							   child_pos, iterator);
						axl_error_new (-1, "Found a different node, inside a sequence, than the sequence especification (DTD)",
							       NULL, error);
					}
					/* return that a match wasn't possible */
					*child_position = child_pos;
					return false;			
				}
			}

			/* according to the repetition pattern, update loop indexes */
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "updating child nodes references: %d, repeat type: %d, status=%d",
				   child_pos, times, status);


			/* one only item to match and exactly one */
			if (times == ONE_AND_ONLY_ONE) {
				child_pos++;
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "updated child position to: %d, repeat type: %d, status=%d",
					   child_pos, times, status);
				break;
			}

			/* one or many items to match */
			if (times == ONE_OR_MANY) { 

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "matched one to many item node: status=%d one_matched=%d",
					   status, one_matched);

				/* if the match have failed and
				 * previous matches was ok, it seems
				 * we have reached the next
				 * items. Just break the loop */
				if (status == false && one_matched == true) 
					break;

				child_pos++;
				one_matched = true;
				continue; /* don't break the loop */
			}

			/* zero or optionally one item to match */			
			if (times == ZERO_OR_ONE) {
				/* if previous status was ok, it seems
				 * that we have matched the optional
				 * character. In that case, move the
				 * index to the following value. If
				 * not, just break the loop. */
				if (status == true)
					child_pos++;
				break;
			}

			/* zero or many items to match */
			if (times == ZERO_OR_MANY) {
				if (status == true) {
					one_matched = true;
					child_pos++;
					continue;
				}
				break;
			}


			/* until break the loop */
		}while (true);

		/* update iterator index */
		iterator++;
	}

	/* check if more nodes where specified than the DTD spec */
	times = axl_dtd_item_list_repeat (itemList);
	if ((times == ONE_OR_MANY || times == ONE_AND_ONLY_ONE) && 
	    top_level && (child_pos  < axl_node_get_child_num (parent))) {

		/* drop a log */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "parent node <%s> have more childs=%d than the childs iterated=%d, top_level=%d",
			   axl_node_get_name (parent),
			   axl_node_get_child_num (parent),
			   child_pos, top_level);

		/* do not report an error found if a try match is
		 * being run */
		if (! try_match) {
			axl_error_new (-1, "More childs, than the ones especified in the DTD, were found",
				       NULL, error);
		}
		/* return that the match wasn't possible */
		*child_position = child_pos;
		return false;
	}

	/* return that the sequence has been validated */
	*child_position = child_pos;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "sequence validated with child position (I): %d", child_pos);

	return true;
}

/** 
 * @internal
 * 
 * Internal support function to validate the choice list.
 */
bool     __axl_dtd_validate_choice (axlNode * parent, int * child_position, 
				    axlDtdElementList * itemList, 
				    axlError  ** error,
				    bool     try_match, bool     top_level)
{
	axlNode               * node;
	axlDtdElementListNode * itemNode;
	int                     iterator;
	bool                    status;
	AxlDtdTimes             times;
	bool                    one_match;

	
	if (*child_position < axl_node_get_child_num (parent)) {
		/* get a reference to be matched by the choice list */
		node = axl_node_get_child_nth (parent, *child_position);
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "validated choice list at position: %d=<%s>", *child_position, axl_node_get_name (node));
	} else {
		/* tried to match a choice list with a child index
		 * outside the maximum number of childs */
		if (! try_match) {
			axl_error_new (-1, "Unable to match choice list, it seems that the are not enough childs to validate the choice list",
				       NULL, error);
		}
		return false;
	}

	iterator = 0; 
	while (iterator < axl_dtd_item_list_count (itemList)) {
		/* get the DTD item list to match */
		itemNode = axl_dtd_item_list_get_node   (itemList, iterator);
		times    = axl_dtd_item_node_get_repeat (itemNode);

		if (axl_dtd_item_node_get_type (itemNode) == AXL_ELEMENT_NODE) {
			/* reset match configuration */
			one_match = false;
		repeat_for_node:
			/* a node was found */
			status    = NODE_CMP_NAME (node, axl_dtd_item_node_get_value (itemNode));

			/* know, if the node was matched check it
			 * repetition configuration */
			if (status) {
				/* update child position */
				(*child_position)++;

				if (times == ONE_AND_ONLY_ONE || times == ZERO_OR_ONE) {
					/* the node was matched and
					 * the itemNode has a one and
					 * only one configuration,
					 * just return that the choice
					 * list was matched */
					return true;
				}
				if (times == ONE_OR_MANY || times == ZERO_OR_MANY) {
					/* because the node was matched, but the repetition
					 * pattern allows to match more nodes we have to
					 * iterate a bit more */
					node = axl_node_get_child_nth (parent, *child_position);
					if (node == NULL) {
						/* because we already matched at least one item, 
						 * we can assume that the itemNode was successfully 
						 * matched for both cases (*) and (+). */
						return true;
					}
					/* flag the one match */
					one_match = true;
					
					/* if the node reference is
					 * not NULL, try to match the
					 * next item */
					goto repeat_for_node;
				}
			} /* end if */
			
			/* before returning, that that we have matched
			 * previously, at least, one node for
			 * one-to-many and zero-to-many pattern */
			if ((times == ONE_OR_MANY || times == ZERO_OR_MANY) && one_match) {
				return true;
			}

		} else if (axl_dtd_item_node_get_type (itemNode) == AXL_ELEMENT_LIST) {
			/* an element list was found, call to validate it */
			/* element list found, validate its content */
			if (__axl_dtd_validate_item_list (axl_dtd_item_node_get_list (itemNode),
							  parent, child_position, error, false)) {
				/* item list matched */
				return true;
			}
		}

		/* no item was matched, update iterator indexes */
		iterator++;
	}

	/* seems that the choice list wasn't matched */
	if (! try_match) {
		axl_error_new (-1, "Unable to match choice list, after checking all posibilities, choice list wasn't validated", 
			       NULL, error);
	}
	return false;
}

/** 
 * @internal 
 *
 * Tries to perform a validation, based on the item list received and
 * the repetition configuration.
 * 
 * @param itemList The item list containing DTD content spec
 * information used to validate.
 *
 * @param parent The parent node where the validation process is being
 * applied. The content spec refers to the childs the parent has.
 *
 * @param stack An stack used by the overall process to store the
 * subsequent parents to be validated. This stack must be released if
 * a error is found.
 *
 * @param error An optional axlError reference containing the error
 * textual diagnostic if found.
 * 
 * @return true if the validation was ok, otherwise false is
 * returned.
 */
bool     __axl_dtd_validate_item_list (axlDtdElementList  * itemList,
				       axlNode            * parent, 
				       int                * child_position,
				       axlError          ** error,
				       bool                 top_level)
{
	int      temp_child_pos;
	int      caller_child_pos;
	bool     status;
	bool     already_matched;


	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "validating an item list with repeat pattern: %d, at %d, top level=%d",
		   axl_dtd_item_list_repeat (itemList), *child_position, top_level);

	/* store current caller child position value to check it before */
	caller_child_pos = *child_position;

	/* now check repetition type */
	switch (axl_dtd_item_list_repeat (itemList)) {
	case ONE_AND_ONLY_ONE:
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a (one and only one) spec..");
		if (axl_dtd_item_list_type (itemList) == SEQUENCE) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "using a SEQUENCE form");
			/* it is a choice, so the item list specifies
			 * the nodes that could appear */
			if (!__axl_dtd_validate_sequence (parent, child_position, itemList, error, 
							  false, top_level)) {
				return false;
			}
		}else {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "using a CHOICE form");
			/* it is a sequence, so, item list
			 * specification represents the nodes, in the
			 * order they must appear */
			if (!__axl_dtd_validate_choice (parent, child_position, itemList, error,
							false, top_level)) {
				return false;
			}
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "choice list was properly validated");
		}
		break;
	case ZERO_OR_ONE:

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a (zero or one) spec..");
		if (axl_dtd_item_list_type (itemList) == SEQUENCE) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "using a SEQUENCE form, parent: <%s>",
				   axl_node_get_name (parent));
			/* because we are running a zero or one item
			 * list matching, we don't care if it doesn't
			 * match. In the case it match, the child
			 * position is updated and next calls will be
			 * properly aligned. In the match doesn't
			 * happens, it also don't matter because the
			 * pattern allow to not match */
			temp_child_pos = *child_position;
			if (!__axl_dtd_validate_sequence (parent, child_position, itemList, error, 
							  true, top_level)) {
				/* check that the match wasn't
				 * produced, at any level */
				if (temp_child_pos != *child_position) {
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "item list mismatch (%d != %d)", 
						   temp_child_pos, *child_position);
					axl_error_new (-1, "Found an DTD item list definition, that should be matched entirely or not, zero or one time, but it was matched partially",
						       NULL, error);
					return false;
				}

				return false;
			}
			
		}else {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "    using a CHOICE form");

			/* it is a sequence, so, item list
			 * specification represents the nodes, in the
			 * order they must appear */
			__axl_dtd_validate_choice (parent, child_position, itemList, error,
						   true, top_level);
		}
		break;
	case ZERO_OR_MANY:

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a (zero or many) spec..");
		if (axl_dtd_item_list_type (itemList) == SEQUENCE) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "    using a SEQUENCE (size: %d) form ",
				   axl_dtd_item_list_count (itemList));
			/* one this case, several matches must be
			 * tried, until the validation fails */
			do {
				temp_child_pos = *child_position;
				status         = __axl_dtd_validate_sequence (parent, child_position, itemList, error, 
									      true, top_level);

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "sequence match status=%d", status);
				if (! status) {
					/* check that the match wasn't
					 * produced, at any level */
					if ((temp_child_pos != *child_position)) {

						__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "item list mismatch (%d != %d)", 
							   temp_child_pos, *child_position);
						axl_error_new (-1, "Found an DTD item list definition, that should be matched entirely or not, zero or many times, but it was matched partially",
							       NULL, error);
						return false;
					}
				}
			}while (status);
		}else {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "    using a CHOICE form");
			/* it is a sequence, so, item list
			 * specification represents the nodes, in the
			 * order they must appear */
			do {
				status = __axl_dtd_validate_choice (parent, child_position, itemList, error,
								    true, top_level);
			}while (status);
		}
		break;
	case ONE_OR_MANY:
		/* one or many sequence spec (+) */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a (zero or many) spec..");
		if (axl_dtd_item_list_type (itemList) == SEQUENCE) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "    using a SEQUENCE (size: %d) form ",
				   axl_dtd_item_list_count (itemList));

			/* one this case, several matches must be
			 * tried, until the validation fails */
			already_matched = false;
			do {
				temp_child_pos = *child_position;
				/* try to match the one or many
				   sequence according to the value
				   stored inside already matched */
				status         = __axl_dtd_validate_sequence (parent, child_position, itemList, error, 
									      already_matched, top_level);

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "sequence match status=%d", status);
				if (! status) {
					/* check that the match wasn't
					 * produced, at any level */
					if ((temp_child_pos != *child_position)) {

						__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "item list mismatch, matched partially (%d != %d)", 
							   temp_child_pos, *child_position);
						axl_error_new (-1, 
							       "Found an DTD item list definition, that should be matched entirely or not, one or many times, but it was matched partially",
							       NULL, error);
						return false;
					}
				}else {
					/* set that we have matched, at least, one item */
					already_matched = true;
				}

				

			}while (status);
		}else {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "    using a CHOICE form");
			/* it is a sequence, so, item list
			 * specification represents the nodes, in the
			 * order they must appear */
			already_matched = false;
			do {
				/* the next choice matching is done
				 * according to the value stored in
				 * already matched */
				status = __axl_dtd_validate_choice (parent, child_position, itemList, error,
								    already_matched, top_level);
				/* if the validation successed, set
				 * that next matched are not required
				 * to be successful ones */
				if (status)
					already_matched = true;
			}while (status);
		}
		break;
	default:
		/* this case will never be reached */
#define INTERNAL_ERROR_01 "critical error reached a place that shows the dtd parser is not properly defining the repetition pattern for the current itemList."
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, INTERNAL_ERROR_01);
		axl_error_new (-1, INTERNAL_ERROR_01, NULL, error);
		return false;
	}


	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "validate item list terminated, now check post-conditions, top_level=%d, item list type=%d, child pos=%d, childs num=%d",
		   top_level, axl_dtd_item_list_type (itemList), *child_position, axl_node_get_child_num (parent));

	/* check that, in the case that the choice item list is being
	 * validated, ensure it has validated all nodes, especially if
	 * we are the top level definition */
	if (top_level && (axl_dtd_item_list_type (itemList) == CHOICE)) {
		if (((*child_position) + 1) < axl_node_get_child_num (parent)) {
		    

			__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "found that the choice list didn't cover all childs (%d), while parent=<%s> has: (%d)",
				   (*child_position), axl_node_get_name (parent), axl_node_get_child_num (parent));
			axl_error_new (-1, "Found that the validation process didn't cover all nodes, while using a choice list. This means that the xml document have more content than the DTD spec",
				       NULL, error);
			return false;
		}
	}
	
	/* element type children validated */
	return true;       
}

/** 
 * @internal
 *
 * Support function validate parent nodes which are element type
 * children ones.
 */
bool     __axl_dtd_validate_element_type_children (axlDtdElement  * element, 
						   axlNode        * parent, 
						   bool             top_level,
						   axlError      ** error)
{
	axlDtdElementList * itemList;
	int                 child_pos = 0;
	char              * err_msg;

	/* get a reference to the item list */
	itemList = axl_dtd_get_item_list (element);

	/* check for xml nodes with fewer content than the initially
	 * expected. */
	if (axl_node_get_child_num (parent) < element->minimum_match) {
		err_msg = axl_strdup_printf ("Found that the parent node (<%s>) received doesn't contains enough xml nodes inside to get a proper validation (childs found (%d) != childs that should be found (%d)). This means that the xml document have fewer content than the DTD spec.",
					     axl_node_get_name (parent), 
					     axl_node_get_child_num (parent), 
					     element->minimum_match);
		axl_error_new (-1, err_msg, NULL, error);
		axl_free (err_msg);
		return false;
	}

	/* validate the item list, starting from the child 0 */
	if (__axl_dtd_validate_item_list (itemList, parent, &child_pos, error, top_level)) {
		/* check if, at least, all minimum elements was
		 * matched */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking minimum node match: (%d < min: %d) (%d < childs: %d) node=<%s>", 
			   child_pos, element->minimum_match,
			   child_pos, axl_node_get_child_num (parent),
			   axl_node_get_name (parent));
			
		if (child_pos < axl_node_get_child_num (parent)) {
			axl_error_new (-1, "Found that the validation process didn't cover all nodes. All xml child nodes inside the parent wasn't covered. This means that the xml document have more content than the DTD spec.",
				       NULL, error);
			return false;
		}
		/* seems that the minimum match */
		return true;
	}

	return false;
}

/** 
 * @internal
 * Internal support function to validate #PCDATA nodes.
 */
bool     __axl_dtd_validate_element_type_pcdata (axlDtdElement  * element, 
						 axlNode        * parent, 
						 axlStack       * stack, 
						 axlError      ** error)
{
	/* check for childs */
	if (axl_node_have_childs (parent)) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "node <%s> should be #PCDATA and it contains childs",
			   axl_node_get_name (parent));
		axl_error_new (-1, 
			       "Found a node for which its espeficiation makes it to be a node with only data and no childs, and it currently contains childs",
			       NULL, error);
		return false;
	}

	/* return that the validation was ok */
	return true;
}

/** 
 * @internal
 * 
 * Support function to validate empty nodes.
 */
bool     __axl_dtd_validate_element_type_empty (axlDtdElement  * element,
						axlNode        * parent,
						axlStack       * stack,
						axlError      ** error)
{
	/* check the node is indeed, empty */
	if (! axl_node_is_empty (parent)) {
		axl_error_new (-1, "Found a node that it is especified that must be empty, but it isn't",
			       NULL, error);
		return false;
	}

	/* check the node doesn't have childs */
	if (axl_node_have_childs (parent)) {
		axl_error_new (-1, "Found a node that it is especified that must be empty, but it has childs",
			       NULL, error);
		return false;
	}
	
	/* return that the validation was ok */
	return true;
}

bool __axl_dtd_attr_validate_foreach (const char * key, const char * value, axlPointer data, axlPointer data2)
{
	axlDtdAttribute     * attribute = data;
	axlError           ** error     = data2;
	axlDtdAttributeDecl * decl;
	char                * err_msg;

	/* get declaration associated */
	decl = axl_list_lookup (attribute->list, __find_attr_decl, (axlPointer) key);
	
	if (decl == NULL) {
		/* found an error */
		err_msg = axl_strdup_printf ("Found an attribute (%s) which is not specified by the attribute declaration for <%s>",
					     key, attribute->name);
		axl_error_new (-1, err_msg, NULL, error);
		
		/* free the cursor and the error message */
		axl_free (err_msg);

		/* return true here because we want to stop the process */
		return true;
		
	} /* end if */

	/* if the declaration is found, now check its
	 * contraints */
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking contraint for attribute: %s=%s", decl->name, value);
	if (decl->type == CDATA_ATTRIBUTE) {
		/* found free attribute declaration, go ahed */
	} else if (decl->type == ENUMERATION_TYPE) {
		/* found an enumeration type, check and return */
		if (axl_list_lookup (decl->enumvalues, axl_list_find_string, (char *) value) == NULL) {
			/* found an error */
			err_msg = axl_strdup_printf ("Found an attribute (%s) with a value not allowed by the enum declaration (%s) for the node <%s>",
						     key, value, attribute->name);
			axl_error_new (-1, err_msg, NULL, error);
			
			/* free the cursor and the error message */
			axl_free (err_msg);
			return true;
		}
	} else {
		/* not supported yet */
	}

	/* return false to continue with the process */
	return false;
}

bool __axl_dtd_attr_validate_required (axlPointer element, axlPointer data)
{
	axlNode             * node = data;
	axlDtdAttributeDecl * decl = element;

	switch (decl->defaults) {
	case ATT_REQUIRED:
		/* attribute required */
		return !HAS_ATTR (node, decl->name);
	case ATT_FIXED:
		return !HAS_ATTR_VALUE (node, decl->name, decl->default_value);
	default:
		break;
	} /* end switch */

	/* return false for this because it is not obligatory
	 * to have the attribute defined. */
	return false;
}

/** 
 * @internal Functions which validates the attribute declaration for
 * the node provided, using attribute declarations found.
 * 
 * @param node The node to check for its attributes.
 *
 * @param dtd The dtd used to validate the node provided.
 *
 * @param error A reference to the axlError where the textual
 * diagnostic error will be reported.
 * 
 * @return true if the node is validated, false if not.
 */
bool axl_dtd_attr_validate (axlNode * node, axlDtd * dtd, axlError ** error, axlHash * id_validation, axlList * idref_validation)
{
	axlDtdAttribute     * attribute;
	axlDtdAttributeDecl * decl;
	char                * err_msg;
	int                   iterator;
	axlError            * _error = NULL;

	/* find attribute contraints for the node */
	attribute = axl_dtd_get_attr (dtd, axl_node_get_name (node));
	if (attribute == NULL)
		return true;

	/* we have an especification, run it */

	/* for each attribute found, check against the spec */
	axl_node_attr_foreach (node, __axl_dtd_attr_validate_foreach, attribute, &_error);

	/* check the error */
	if (! axl_error_was_ok (_error)) {
		/* reconfigure error returned */
		if (error != NULL)
			*error = _error;
		return false;
	} /* end if */
		
	
	/* now, for each contraint, check that all required nodes
	 * exists */
	decl = axl_list_lookup (attribute->list, __axl_dtd_attr_validate_required, node);
	if (decl != NULL) {
		if (decl->defaults == ATT_FIXED)
			err_msg = axl_strdup_printf ("attribute required '%s' (or its value), due to #FIXED declaration, not found for node <%s>", 
						     decl->name, attribute->name);
		else 
			err_msg = axl_strdup_printf ("attribute required '%s', due to #REQUIRED declaration, not found for node <%s>", 
						     decl->name, attribute->name);
		axl_error_new (-1, err_msg, NULL, error);
		axl_free (err_msg);
		return false;
	} /* end if */

	/* check declarations */
	if (dtd->haveIdDecl) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found DTD has ID unique attribute declaration..");
		
		/* check if the node have TOKENIZED_TYPE_ID */
		decl = axl_list_lookup (attribute->list, __find_id_decl, NULL);
		
		/* if we have a tokenized */
		if (decl != NULL) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found ID unique attribute declaration %s=\"%s\"..",
				   decl->name, ATTR_VALUE (node, decl->name));

			/* check if the attribute value for the decl that is
			 * flagged as ID is already found at the
			 * id_validation */
			if (axl_hash_exists (id_validation, (axlPointer) ATTR_VALUE (node, decl->name))) {
				err_msg = axl_strdup_printf ("DTD declared the attribute '%s' as unique (ID) for the node %s, but was found used several times",
							     decl->name, attribute->name);
				axl_error_new (-1, err_msg, NULL, error);
				axl_free (err_msg);
				return false;
			} /* end if */
			
			/* seems the attribute was not used, nice!, store it */
			axl_hash_insert (id_validation, (axlPointer) ATTR_VALUE (node, decl->name), (axlPointer) ATTR_VALUE (node, decl->name));
		} /* end if */
	} /* end if */

	if (dtd->haveIdRefDecl) {
		/* find the id ref declaration */
		
		iterator = 0;
		while (iterator < axl_list_length (attribute->list)) {
			
			/* get the attribute declaration at the
			 * particular position */
			decl = axl_list_get_nth (attribute->list, iterator);
			if (decl->type == TOKENIZED_TYPE_IDREF) {
				/* found a reference, but do not check
				 * it at this place becase the
				 * reference could be placed at any
				 * part in the document event after
				 * the reference pointed is
				 * defined. store and check later */
				if (ATTR_VALUE (node, decl->name)) {
					/* store the id ref reference
					 * if defined */
					axl_list_add (idref_validation, (axlPointer) ATTR_VALUE (node, decl->name));
				}
			} /* end if */

			/* get the next */
			iterator++;
			
		} /* end if */
		
	} /* end if */

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "attributes validated for node=<%s>", attribute->name);
	
	return true;
}

/** 
 * @internal Function used by axl_dtd_validate_references to ensure
 * that all references found point to a valid reference defined.
 */
bool __axl_dtd_reference_check (axlPointer _element, axlPointer data)
{
#if defined(SHOW_DEBUG_LOG)
	const char * value = _element;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking id ref: %s", value);
#endif

	return ! axl_hash_exists ((axlHash *) data, _element);
}

/** 
 * @internal Function that validates all references found (from IDREF
 * attribute) to unique references (defined by ID attributes).
 *
 */
bool axl_dtd_validate_references (axlHash * id_validation, axlList * idref_validation, axlError ** error)
{
	char * reference;
	char * err_msg;
	
	/* if no empty at the valiadtion reference list, means not
	 * reference was done, so there is no room for errors */
	if (idref_validation == NULL) 
		return true;
	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "id_validation reference: 0x%x", id_validation);

	/* find first reference not found */
	reference = axl_list_lookup (idref_validation, __axl_dtd_reference_check, id_validation);
	
	if (reference != NULL) {
		/* found a reference not defined, report it to the
		 * application level */
		err_msg = axl_strdup_printf ("Found a reference defined ('%s') which is not found in any ID attribute in the document",
					     reference);
		axl_error_new (-1, err_msg, NULL, error);
		axl_free (err_msg);

		return false;
	} /* end if */
	
	/* validation ok */
	return true;
}

/** 
 * @brief Allows to validate the given XML document (\ref axlDoc)
 * against the given document type definition (DTD, \ref axlDtd).
 *
 * This function allows to validate your XML documents providing the
 * document type definition, that was read using \ref axl_dtd_parse or
 * \ref axl_dtd_parse_from_file.
 *
 * Keep in mind that a document could be well-formed and valid. The
 * only difference is that valid XML document are those that, meet all
 * XML rules, but also are clasified and recognized as XML documents
 * with some particular structure, that is represented (or
 * constrained) with providing a DTD definition.
 *
 * @param doc The \ref axlDoc containing the XML document to be
 * validated.
 *
 * @param dtd The \ref axlDtd containing the DTD definition used to
 * validate the document.
 *
 * @param error An optional reference to a \ref axlError object where
 * validation errors are reported.
 *
 * @return true if the document is valid, false if not.
 */
bool           axl_dtd_validate        (axlDoc * doc, axlDtd * dtd,
					axlError ** error)
{
	axlNode            * parent;
	axlStack           * stack;
	axlHash            * id_validation = NULL;
	axlList            * idref_validation = NULL;
	
	axlDtdElement      * element;
	bool                 top_level;
	char               * err_msg;
	bool                 result;
	
	/* perform some checkings */
	axl_return_val_if_fail (doc, false);
	axl_return_val_if_fail (dtd, false);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "starting DTD validation");

	/* validate the very first root node */
	parent  = axl_doc_get_root (doc);
	element = axl_dtd_get_root (dtd);
	if ((element != NULL) && ! NODE_CMP_NAME (parent, axl_dtd_get_element_name (element))) {

		/* because a DTD document could have several top level
		 * elements, ensure this is not the case */
		element = axl_dtd_get_element (dtd, axl_node_get_name (parent));
		if (element == NULL) { /*  || ! axl_dtd_element_is_toplevel (dtd, element)) { */
			/* root node doesn't match */
			axl_error_new (-1, "Found that root node doesn't match!", NULL, error);
			return false;

		} /* end if */
	} /* end if */

	/* check if the node has DTD element declaration */
	if (element == NULL) {
		err_msg = axl_strdup_printf ("There is not DTD element declaration to validate the node <%s>", 
					     axl_node_get_name (parent));
		axl_error_new (-1, err_msg, NULL, error);
		axl_free (err_msg);
		return false;
	} /* end if */

	/* check if the dtd contains a Id declaration */
	if (dtd->haveIdDecl) {
		/* seems the user have declarted ID attributes init the hash */
		id_validation = axl_hash_new (axl_hash_string, axl_hash_equal_string);
	} /* end if */

	/* check if the dtd contains Id ref declarations */
	if (dtd->haveIdRefDecl) {
		/* create a list that could contain all references done */
		idref_validation = axl_list_new (axl_list_always_return_1, NULL);
	} /* end if */

	/* check empty content spec */
	if (axl_dtd_get_element_type (element) == ELEMENT_TYPE_EMPTY) {
		/* check if the document provided have only one node */
		result = axl_node_is_empty (parent) && !axl_node_have_childs (parent) && axl_dtd_attr_validate (parent, dtd, error, id_validation, idref_validation);

		/* check references */
		if (result)
			result = axl_dtd_validate_references (id_validation, idref_validation, error);
		
		/* free and return */
		axl_hash_free (id_validation);

		/* free the list */
		axl_list_free (idref_validation);
		return result;
	} /* end if */

	/* queue initial nodes to validate */
	stack     = axl_stack_new (NULL);
	

	/* set that the only top level node is the first one */
	top_level = true;

	do {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "doing a DTD iteration: <%s>...",
			   axl_node_get_name (parent));

		/* validate attributes */
		if (! axl_dtd_attr_validate (parent, dtd, error, id_validation, idref_validation)) {
			/* free the stack */
			axl_stack_free (stack);

			/* free id_validation */
			axl_hash_free (id_validation);

			/* free the list */
			axl_list_free (idref_validation);
			return false;
		}

		/* reach this position, the <parent> reference contains
		 * a reference to the parent node, which will be used
		 * to validate current child content against current
		 * configuration for dtd element constraining it.
		 * 
		 * equally, the <element> reference contains a dtd
		 * reference to the already checked DTD element which
		 * configure this parent node. */
		switch (axl_dtd_get_element_type (element)) {
		case ELEMENT_TYPE_PCDATA:
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "  find  PCDATA dtd element=%s: parent=<%s>, ",
				   axl_dtd_get_element_name (element), 
				   axl_node_get_name (parent));

			/* ok, a leaf node was found, know it is
			 * required to check that the node doesn't
			 * have more childs and only have content,
			 * that is, it is not empty  */
			if (!__axl_dtd_validate_element_type_pcdata (element, parent, stack, error)) {
				/* free id_validation */
				axl_hash_free (id_validation);

				/* free the stack */
				axl_stack_free (stack);
				
				/* free the list */
				axl_list_free (idref_validation);
				return false;
			}
			break;
		case ELEMENT_TYPE_CHILDREN:

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "  find  CHILDREN dtd element");
			/* ok, a parent node that have childs */
			if (!__axl_dtd_validate_element_type_children (element, parent, top_level, error)) {
				/* free id_validation */
				axl_hash_free (id_validation);

				/* free the stack */
				axl_stack_free (stack);

				/* free the list */
				axl_list_free (idref_validation);
				
				return false;
			}
			break;
		case ELEMENT_TYPE_EMPTY:
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "  find  EMPTY dtd element");
			/* the element especification is empty, the
			 * node being validated must also be the
			 * same */
			if (!__axl_dtd_validate_element_type_empty (element, parent, stack, error)) {
				/* free id_validation */
				axl_hash_free (id_validation);

				/* free the stack */
				axl_stack_free (stack);

				/* free the list */
				axl_list_free (idref_validation);

				return false;
			}
			break;
		case ELEMENT_TYPE_ANY:
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "  find  ANY dtd element");
			/* the anything is allowed cased from this
			 * parent node. */
			goto continue_with_validation;
		case ELEMENT_TYPE_MIXED:
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "  find  MIXED dtd element");

			/* the mixed case, where nodes and PC data
			 * could be mixed */
			break;
		default:
			/* do not do any thing on this case */
			break;
		}
			
		/* queue more childs, as future parents to be
		 * validated on the provided queue, only in the case
		 * the parent node have childs */
		if (axl_node_have_childs (parent)) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "parent node <%s> have childs, adding its childs (stack size: %d)",
				   axl_node_get_name (parent),
				   axl_stack_size (stack));

			/* queue childs to be processed */
			__axl_dtd_queue_childs (stack, parent);

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "parent node <%s> childs: %d, (stack size: %d)",
				   axl_node_get_name (parent), axl_node_get_child_num (parent),
				   axl_stack_size (stack));
		}
		
		/* set the parent reference to NULL */
		parent = NULL;
			
		/* update the reference to the new parent node, only
		 * if there are new parents on the stack */
	continue_with_validation:
		if (! axl_stack_is_empty (stack)) {


			/* get a new reference */
			parent  = axl_stack_pop (stack);
			
			/* get a reference to the DTD element to used */
			element = axl_dtd_get_element (dtd, axl_node_get_name (parent));
			if (element == NULL) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "found that the node <%s> doesn't have DTD especification", 
					   axl_node_get_name (parent));
				/* prepare the error message */
				err_msg = axl_strdup_printf ("Found a node <%s> that doesn't have a DTD element espefication to validate it, DTD validation failed",
							     axl_node_get_name (parent));
				axl_error_new (-1, err_msg, NULL, error);
				axl_free (err_msg);

				/* free id_validation */
				axl_hash_free (id_validation);

				/* free the list */
				axl_list_free (idref_validation);

				/* free the stack */
				axl_stack_free (stack);
				return false;
			} /* end if */
		} /* end if */

		/* set the top level status */
		top_level = false;
		
		/* until the stack is empty */
	}while (parent != NULL);

	/* check references */
	result = axl_dtd_validate_references (id_validation, idref_validation, error);

	/* deallocate stack used */
	axl_stack_free (stack);

	/* free id_validation */
	axl_hash_free (id_validation);

	/* free the list */
	axl_list_free (idref_validation);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "DTD validation, %s", result ? "ok" : "failed");

	/* the document is valid */
	return result;
}

/** 
 * @brief Allows to get the root node for the provided DTD.
 *
 * Every DTD have a root node defined, which is the root node accepted
 * for the set of XML document considered to be valid under the
 * definition of the DTD provided.
 *
 * The value returned is the name of the root node that must have the
 * XML document being validated.
 * 
 * @param dtd The \ref axlDtd where the root node name will be
 * returned.
 * 
 * @return A reference to the internal representation of the root node
 * Value must not be deallocated.
 */
axlDtdElement  * axl_dtd_get_root        (axlDtd * dtd)
{
	axl_return_val_if_fail (dtd, NULL);
	
	/* return current status for the root node */
	if (dtd->root == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "dtd root element not defined");
		return NULL;
	}
	return dtd->root;
}

/** 
 * @internal function used by \ref axl_dtd_get_element to perform node
 * lookups.
 */
bool __find_dtd_element (axlPointer _element, axlPointer data)
{
	axlDtdElement * element = _element;
	char          * name    = data;

	/* check the name */
	if (axl_cmp (element->name, name))
		return true;

	/* it is not the element */
	return false;
}

/** 
 * @brief Allows to get the DTD element (\ref axlDtdElement), inside
 * the provided DTD (\ref axlDtd), that represent the spefication for
 * the node called by the provided name.
 * 
 * @param dtd The DTD (\ref axlDtd) where the lookup will be
 * performed.
 *
 * @param name The element name to lookup.
 * 
 * @return A reference to the \ref axlDtdElement searched or NULL if
 * fails. The function also returns NULL if values received are NULL.
 */
axlDtdElement      * axl_dtd_get_element      (axlDtd * dtd, const char * name)
{

	axl_return_val_if_fail (dtd, NULL);
	axl_return_val_if_fail (name, NULL);

	/* perform the lookup */
	return axl_list_lookup (dtd->elements, __find_dtd_element, (axlPointer) name);
}

/** 
 * @internal function used by \ref axl_dtd_get_attr to perform node
 * lookups.
 */
bool __find_dtd_attr (axlPointer _element, axlPointer data)
{
	axlDtdAttribute * attr = _element;
	char            * name = data;

	/* check the name */
	if (axl_cmp (attr->name, name))
		return true;

	/* it is not the element */
	return false;
}

/** 
 * @brief Allows to get the set of attribute declerations for a
 * particular node. 
 *
 * The \ref axlDtdAttribute declaration contains all constraints
 * configured for attributes found for the particular xml node
 * (identified by <b>name</b>).
 * 
 * @param dtd A reference to the DTD document.
 *
 * @param nodeName The xml node that is requested to return all attribute
 * declarations.
 * 
 * @return A reference to the \ref axlDtdAttribute or NULL if it
 * fails.
 */
axlDtdAttribute    * axl_dtd_get_attr         (axlDtd * dtd,
					       const char * nodeName)
{
	axl_return_val_if_fail (dtd, NULL);
	axl_return_val_if_fail (nodeName, NULL);

	/* perform the lookup */
	return axl_list_lookup (dtd->attributes, __find_dtd_attr, (axlPointer) nodeName);
}

/** 
 * @brief Allows to get the number of constraints that have been
 * configured for the particular node.
 * 
 * @param dtd The reference to the DTD document.
 *
 * @param nodeName The name of the node that is being asked for its
 * constraints.
 * 
 * @return 0 or the number of contraints. The function return -1 if
 * any of the parameter received is null.
 */
int                  axl_dtd_get_attr_contraints (axlDtd * dtd,
						  const char * nodeName)
{
	axlDtdAttribute * attr;

	axl_return_val_if_fail (dtd, -1);
	axl_return_val_if_fail (nodeName, -1);

	/* get the attribute specification for the node */
	attr = axl_dtd_get_attr (dtd, nodeName);

	/* return the number of items */
	return axl_list_length (attr->list);
}

/** 
 * @brief Returns the name of the provided \ref axlDtdElement.
 * 
 * @param element A reference to a \ref axlDtdElement where the name
 * will be returned.
 * 
 * @return A reference to the internal DTD element name. Returned
 * value mustn't be deallocated.
 */
char           * axl_dtd_get_element_name (axlDtdElement * element)
{
	axl_return_val_if_fail (element,  NULL);

	return element->name;
}

/** 
 * @brief Returns current element type for the provided \ref axlDtdElement.
 * 
 * @param element The axlDtdElement where its type will be returned.
 * 
 * @return Current element type for the provided node.
 */
AxlDtdElementType    axl_dtd_get_element_type (axlDtdElement * element)
{
	axl_return_val_if_fail (element, ELEMENT_TYPE_UNKNOWN);
	
	return element->type;
}

/** 
 * @brief Returns current DTD content specification, represented by the Item list.
 * 
 * @param element The DTD element (\ref axlDtdElement) which is being
 * requested to return its \ref axlDtdElementList.
 * 
 * @return The \ref axlDtdElementList reference. The value returned
 * must not be deallocated. The function returns NULL if the reference received is NULL.
 */
axlDtdElementList  * axl_dtd_get_item_list    (axlDtdElement * element)
{
	axl_return_val_if_fail (element, NULL);

	return element->list; 
}

/** 
 * @brief Allows to check if the provided DTD ELEMENT representation
 * is a top level definition.
 * 
 * @param dtd The DTD document where the operation will be performed.
 * @param element The \ref axlDtdElement to check.
 * 
 * @return \ref true if the dtd element is a top level element or
 * \ref false if not. The function returns \ref false if the
 * provided reference is NULL.
 */
bool                    axl_dtd_element_is_toplevel (axlDtd * dtd, axlDtdElement * element)
{
	/* support several top level definitions */
	int             iterator;
	axlDtdElement * dtd_element_aux;

	axl_return_val_if_fail (dtd,     false);
	axl_return_val_if_fail (element, false);

	/* check which is the top */
	iterator        = 0;
	while (iterator < axl_list_length (dtd->elements)) {
			
		/* get the next reference */
		dtd_element_aux = axl_list_get_nth (dtd->elements, iterator);
			
		/* check which is the top */
		if (__axl_dtd_get_is_parent (dtd_element_aux, element)) {
			/* the element provided have a parent */
			return false;
		}
			
		/* update inner loop iterator */
		iterator ++;
	} /* while end */

	/* return that the provided node doesn't have a parent node */
	return true;
}

/** 
 * @brief Returns the number of item nodes (\ref
 * axlDtdElementListNode) inside the item list received (\ref axlDtdElementList).
 * 
 * @param itemList The \ref axlDtdElementList where the count
 * operation is being requested.
 * 
 * @return The number of item list the provided \ref axlDtdElementList
 * reference has. The function return -1 if the provided reference is
 * NULL.
 */
int                  axl_dtd_item_list_count  (axlDtdElementList * itemList)
{
	axl_return_val_if_fail (itemList, -1);

	if (itemList->itemList == NULL)
		return 0;

	return axl_list_length (itemList->itemList);
}

/** 
 * @brief Allows to get current configuration for the provided item
 * list, which is the content specification for a DTD element.
 * 
 * @param itemList The item list where the operation will be
 * performed.
 * 
 * @return Current configuration (\ref SEQUENCE or a \ref CHOICE).
 */
AxlDtdNestedType     axl_dtd_item_list_type   (axlDtdElementList * itemList)
{
	axl_return_val_if_fail (itemList, -1);

	return itemList->type;
}

/** 
 * @brief Allows to get current configuration for DTD content spec
 * repetition.
 * 
 * @param itemList The content spec where the query will be performed.
 * 
 * @return Current configuration for times to be repeated DTD element
 * content specification.
 */
AxlDtdTimes          axl_dtd_item_list_repeat (axlDtdElementList * itemList)
{
	axl_return_val_if_fail (itemList, DTD_TIMES_UNKNOWN);

	/* returns current times configuration */
	return itemList->times;
}

/** 
 * @brief Allows to get the provided item node reference (\ref
 * axlDtdElementListNode) from the provided item list (\ref
 * axlDtdElementList).
 *
 * Provided position ranges from 0 up to \ref axl_dtd_item_list_count.
 * 
 * @param itemList The itemList where the operation will be performed.
 * @param position The position where the item node will be looked up.
 * 
 * @return A reference to the \ref axlDtdElementListNode, or NULL if
 * there is no item node at the selected index.  The function return
 * NULL if the provided position is a non positive value or it is
 * greater than the current item list count (\ref
 * axl_dtd_item_list_count) or the provided item list reference is
 * NULL.
 */
axlDtdElementListNode * axl_dtd_item_list_get_node (axlDtdElementList * itemList, 
						    int position)
{
	axl_return_val_if_fail (itemList, NULL);
	axl_return_val_if_fail (position >= 0, NULL);
	axl_return_val_if_fail (position < axl_dtd_item_list_count (itemList), NULL);
	
	return axl_list_get_nth (itemList->itemList, position);
}

/** 
 * @brief Allows to get current node type for the provided DTD element
 * type content particule or item node (\ref axlDtdElementListNode).
 *
 * @param node The node where the type is being requested.
 * 
 * @return It returns if the item node contains a final leaf node,
 * making a reference to an explicit node naming that is allowed to be
 * used in the context where is found the provided \ref
 * axlDtdElementListNode or a \ref axlDtdElementList containing more
 * nodes or lists. 
 */
NodeType             axl_dtd_item_node_get_type (axlDtdElementListNode * node)
{
	axl_return_val_if_fail (node, AXL_ELEMENT_NOT_DEFINED);
	return node->type;
}

/** 
 * @brief Returns the item list inside the provided node.
 *
 * The node is supported to contain an item list reference or NULL
 * will be returned. Check \ref axl_dtd_item_node_get_type.
 * 
 * @param node The node where the operation will be performed.
 * 
 * @return The item list inside the node or NULL if fails.
 */
axlDtdElementList   * axl_dtd_item_node_get_list (axlDtdElementListNode * node)
{
	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (node->type == AXL_ELEMENT_LIST, NULL);

	return node->data;
}

/** 
 * @brief Allows to get the dtd item list value, which represents the
 * node name that is being constrained/represented.
 * 
 * @param node The item node where the value is being requested.
 * 
 * @return The value inside the item node, supposing it contains an
 * leaf item node or NULL if fails. The value returned must not be
 * deallocated.
 */
char               * axl_dtd_item_node_get_value (axlDtdElementListNode * node)
{
	axl_return_val_if_fail (node, NULL);
	if (node->type != AXL_ELEMENT_NODE) 
		return "requested-value-on-a-list";

	return node->data;
}

/** 
 * @brief Allows to get current configuration for the provided content
 * particule for the times to be repeated.
 * 
 * @param node The content particule where the query will be
 * performed.
 * 
 * @return Return current repetition configuration. 
 */
AxlDtdTimes          axl_dtd_item_node_get_repeat (axlDtdElementListNode * node)
{
	axlDtdElementList * list;

	axl_return_val_if_fail (node, DTD_TIMES_UNKNOWN);


	if (node->type == AXL_ELEMENT_NODE) {
		/* return value requested */
		return node->times;
	}

	if (node->type == AXL_ELEMENT_LIST) {
		/* return the requested value for an item list */
		list = node->data;
		return list->times;
	}
	
	/* return that we don't know man */
	return DTD_TIMES_UNKNOWN;
}


/** 
 * @internal
 *
 * Internal function which allows to lookup the DTD entity reference
 * provided the name and the type.
 */
axlDtdEntity * __axl_dtd_entity_lookup (axlDtd            * dtd, 
					const char        * name,
					axlDtdEntityType    type)
{
	axlDtdEntity  * entity;
	int             iterator;
	int             length;

	/* check values received */
	axl_return_val_if_fail (dtd,  NULL);
	axl_return_val_if_fail (name, NULL);

	/* lookup for the item */
	iterator = 0;
	length   = axl_list_length (dtd->entities);
	while (iterator < length) {

		/* get the entity at the provided position */
		entity = axl_list_get_nth (dtd->entities, iterator);

		/* check the type and the name */
		if ((entity->type == type) && axl_cmp (entity->name, name))
			return entity;
		
		/* update iterator */
		iterator++;
	} /* end while */

	return NULL;
}

/** 
 * @brief Allows to check if the provided entity name, with the
 * provided type is defined on the given DTD object.
 * 
 * @param dtd The \ref axlDtd instance where the entity lookup will be
 * performed.
 *
 * @param name The entity name to lookup.
 *
 * @param type The entity type to lookup.
 * 
 * @return true if an entity is found named as provided with the type
 * provided. Othewise, false is returned.
 */
bool                 axl_dtd_entity_exists    (axlDtd            * dtd, 
					       const char        * name,
					       axlDtdEntityType    type)
{
	/* return if the entity exists */
	return (__axl_dtd_entity_lookup (dtd, name, type) != NULL);
}

/** 
 * @brief Allows to get the content configured inside the entity that
 * is identified by the provided name and the provided type.
 * 
 * @param dtd The DTD where the lookup will be performed.
 *
 * @param name The entity name to lookup for its content.
 *
 * @param type The entity type to match.
 * 
 * @return An internal reference to the content associated to the
 * entity found or NULL. In case the content is defined (as return
 * value) it must not be deallocated.
 */
char               * axl_dtd_entity_value     (axlDtd            * dtd, 
					       const char        * name,
					       axlDtdEntityType    type)
{
	axlDtdEntity * entity;

	/* get the entity reference */
	entity = __axl_dtd_entity_lookup (dtd, name, type);
	
	/* check the entity reference */
	axl_return_val_if_fail (entity, NULL);

	/* return the content */
	return entity->content;
}

/** 
 * @brief Allows to destroy the provided \ref axlDtd  document.
 * 
 * @param dtd The \ref axlDtd document to destroy.
 */
void       axl_dtd_free  (axlDtd * dtd)
{
	if (dtd == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received a null DTD reference, doing nothing");
		return;
	}
	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "releasing the DTD reference");
	/* free dtd elements */
	if (dtd->elements)
		axl_list_free (dtd->elements);

	/* free entities */
	if (dtd->entities)
		axl_list_free (dtd->entities);

	/* free attributes */
	if (dtd->attributes)
		axl_list_free (dtd->attributes);

	/* free the node itself */
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
	if (element->name != NULL)
		axl_free (element->name);

	/* free element list definitions */
	axl_dtd_item_list_free (element->list);
	
	/* free element itself */
	axl_free (element);

	return;
}

/** 
 * @internal 
 *
 * @brief Deallocates memory used by the \ref axlDtdElementList
 * reference.
 * 
 * @param list The reference to deallocate.
 */
void axl_dtd_item_list_free (axlDtdElementList * list)
{
	if (list == NULL)
		return;
	
	/* check and deallocate the list provided */
	if (list->itemList != NULL)
		axl_list_free (list->itemList);
	
	/* deallocates the node itself */
	axl_free (list);
	return;
}

/* @} */

