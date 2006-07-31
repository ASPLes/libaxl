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

#include <axl.h>
#define LOG_DOMAIN "axl-node"

/**
 * \defgroup axl_node_module Axl Node: Xml node interface functions to use and manipulate xml nodes inside documents.
 */

/** 
 * \addtogroup axl_node_module
 * @{
 */

struct _axlAttribute {
	/** 
	 * @internal
	 *
	 * The attribute name, that is, the left hand part of the
	 * attribute expresion name=value.
	 */
	char * name;
	/** 
	 * @internal
	 * 
	 * The attribute value, that is, the right hand part of the
	 * attribute expresion name=value.
	 */
	char * value;
};

struct _axlNode {
	/** 
	 * @internal
	 *
	 * Node name that is the value found at the very begining of
	 * the node definition <name.../>
	 */
	char          * name;

	/** 
	 * @internal
	 * @brief The attributes this node has.
	 */
	axlList       * attributes;

	/** 
	 * @internal
	 * 
	 * Current configuration for a node to be considered
	 * empty. This value applies to the content that the node
	 * holds. If the node have only xml childs, the node is still
	 * considered to be empty. This is because we have is_empty
	 * and have_childs.
	 */
	bool            is_empty;

	/** 
	 * @internal
	 *
	 * Current configuration about xml child nodes that must have
	 * the node. This value is set by the axl doc module to advise
	 * that the module must have or not a xml child nodes.
	 */
	bool            have_childs;

	/** 
	 * @internal
	 * @brief A list of childs the current node has.
	 */
	axlList       * childs;

	/** 
	 * @internal
	 *
	 * @brief Pointer which holds current content inside the xml
	 * node.
	 */
	char          * content;
	/** 
	 * @internal
	 *
	 * @brief Current content size stored on the given axlNode.
	 */
	int             content_size;

	/** 
	 * @internal
	 * Pi targets storing.
	 */
	axlList       * piTargets;

	/** 
	 * @internal
	 * A pointer to the parent node.
	 */
	axlNode       * parent;

	/** 
	 * @internal
	 *
	 * A pointer to the brother node, the node that is found on
	 * the next position.
	 */
	axlNode       * next;

	/** 
	 * @internal
	 *
	 * Internal reference to the whole xml document where the node
	 * is contained.
	 */
	axlDoc        * doc;
};

/** 
 * @internal
 *
 * @brief Internal position function which ensures that nodes added
 * will be added at the end of the list.
 */
int __axl_node_equal (axlPointer a, axlPointer b)
{
	return 1;
}

/** 
 * @internal 
 *
 * @brief Implements automatic attribute memory deallocation once the
 * node is finished.
 * 
 * @param attr The axlAttribute to deallocate
 */
void __axl_node_destroy_attr (axlAttribute * attr)
{
	/* release memory hold by the attribute */
	axl_free (attr->name);
	axl_free (attr->value);
	axl_free (attr);

	return;
}

/** 
 * @internal function which initializes the node to the default state.
 * 
 * @param node The node to initialize.
 */
void __axl_node_init_internal (axlNode * node, char * name)
{
	node->is_empty = true;
	node->name     = name;

	return;
}

axlNode * __axl_node_create_internal (char * name)
{
	axlNode * node = NULL;

	/* perform some environmental checks */
	axl_return_val_if_fail (name, NULL);
	
	/* init the node */
	node           = axl_new (axlNode, 1);
	__axl_node_init_internal (node, name);

	return node;
}


/** 
 * @internal
 * 
 * Allows to check if the provided string have escape sequences that
 * must be defined by using the entity reference rather the value
 * itself.
 *
 * @param content The content to check.
 *
 * @param additional_size An integer reference where the additional
 * size variable will be added.
 * 
 * @return true if the string contains non valid sequences that
 * must be escaped using entity references.
 */
bool     __axl_node_content_have_not_valid_sequences (char * content, 
						      int    content_size,
						      int * additional_size)
{
	int      iterator = 0;
	bool     result   = false;
	axl_return_val_if_fail (content, false);

	/* reset additional size value */
	*additional_size = 0;

#ifdef SHOW_DEBUG_LOG
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking valid sequence: content size=%d", content_size);
#endif

	/* iterate over all content defined */
	while (iterator < content_size) {
		/* check for &apos; */
		if (content [iterator] == '\'') {
			result = true;
			(*additional_size) += 5;
		}

		/* check for &quot; */
		if (content [iterator] == '"') {
			result = true;
			(*additional_size) += 5;
		}

		/* check for &amp; */
		if (content [iterator] == '&') {
			result = true;
			(*additional_size) += 4;
		}

		/* check for &gt; */
		if (content [iterator] == '>') {
			result = true;
			(*additional_size) += 3;
		}

		/* check for &lt; */
		if (content [iterator] == '<') {
			result = true;
			(*additional_size) += 3;
		}

		/* update the iterator */
		iterator++;
	}

#ifdef SHOW_DEBUG_LOG
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "valid sequence checking result=%d: content size=%d, additional size=%d", 
		 result, content_size, *additional_size);
#endif

	/* return results */
	return result;
}

/** 
 * @internal
 *
 * Internal function which allocates the enough memory to copy
 * received content changing all escape sequences.
 */
char * __axl_node_content_copy_and_escape (char * content, 
					   int    content_size, 
					   int    additional_size)
{
	int    iterator  = 0;
	int    iterator2 = 0;
	char * result;
	axl_return_val_if_fail (content, false);

	/* allocate the memory to be returned */
	result = axl_new (char, content_size + additional_size + 1);

	/* iterate over all content defined */
	while (iterator2 < content_size) {
		/* check for &apos; */
		if (content [iterator2] == '\'') {
			memcpy (result + iterator, "&apos;", 6);
			iterator += 6;
			iterator2++;
			continue;
		}

		/* check for &quot; */
		if (content [iterator2] == '"') {
			memcpy (result + iterator, "&quot;", 6);
			iterator += 6;
			iterator2++;
			continue;
		}

		/* check for &amp; */
		if (content [iterator2] == '&') {
			memcpy (result + iterator, "&amp;", 5);
			iterator += 5;
			iterator2++;
			continue;
		}

		/* check for &gt; */
		if (content [iterator2] == '>') {
			memcpy (result + iterator, "&gt;", 4);
			iterator += 4;
			iterator2++;
			continue;
		}

		/* check for &lt; */
		if (content [iterator2] == '<') {
			memcpy (result + iterator, "&lt;", 4);
			iterator += 4;
			iterator2++;
			continue;
		}

		/* copy value received because it is not an escape
		 * sequence */
		memcpy (result + iterator, content + iterator2, 1);

		/* update the iterator */
		iterator++;
		iterator2++;
	}

	/* return results */
	return result;
}

/** 
 * @internal
 *
 * 
 * Internal function which replaces all entity references to its
 * corresponding values. Initially, this function only implements
 * translation for default recognized entities (&, <, >, ' and ").
 *
 * Because the pattern substitution applied on this operation makes
 * not necessary to allocate memory, the function return the same
 * string received, but with all values replaced.
 */
char * __axl_node_content_translate_defaults (char * content, 
					      int  * content_size)
{
	int    iterator  = 0;
	int    iterator2 = 0;
	
	axl_return_val_if_fail (content, false);

	/* iterate over all content defined */
	while (iterator < (*content_size)) {
		
		/* check for &apos; */
		if (axl_stream_cmp (content + iterator, "&apos;", 6)) {
			content [iterator2] = '\'';
			iterator2++;
			
			iterator += 6;
			continue;
		}
		
		/* check for &quot; */
		if (axl_stream_cmp (content + iterator, "&quot;", 6)) {
			content [iterator2] = '"';
			iterator2++;
			
			iterator += 6;
			continue;
		}

		/* check for &amp; */
		if (axl_stream_cmp (content + iterator, "&amp;", 5)) {
			content [iterator2] = '&';
			iterator2++;
			
			iterator += 5;
			continue;
		}

		/* check for &gt; */
		if (axl_stream_cmp (content + iterator, "&gt;", 4)) {
			content [iterator2] = '>';
			iterator2++;
			
			iterator += 4;
			continue;
		}

		/* check for &lt; */
		if (axl_stream_cmp (content + iterator, "&lt;", 4)) {
			content [iterator2] = '<';
			iterator2++;
			
			iterator += 4;
			continue;
		}

		/* copy move the content */
		if (iterator2 != iterator)
			content [iterator2] = content [iterator];

		/* update the iterator */
		iterator++;
		iterator2++;
	}

	/* return results, setting the new content size */
	*content_size       = iterator2;
	content [iterator2] = 0;
	return content;
}



/** 
 * @brief Creates a new \ref axlNode with the provided name.
 *
 * The function will perform a new local copy for the reference
 * received. See also \ref axl_node_create_ref for an explanation
 * about saving memory.
 *
 * @param name The name to be used for the node be created.
 * 
 * @return A newly allocate \ref axlNode reference, that must be
 * deallocated by \ref axl_node_free.
 */
axlNode * axl_node_create (char * name)
{
	axl_return_val_if_fail (name, NULL);
	
	/* return a reference to a new axlNode */
	return __axl_node_create_internal (axl_strdup (name));
}

/** 
 * @brief Creates a new \ref axlNode but using the memory passed in by
 * the name reference.
 *
 * This function works the same way like \ref axl_node_create, but
 * previous one makes a local copy for the name provided. This means
 * that, if you have allocated the reference being passed, the
 * previous function will allocate again memory for the name
 * reference.
 *
 * Obviously, for a few xml nodes this have none or little effect but,
 * if your xml document have 100.000 nodes you save 100.000 extra
 * memory allocations and deallocations. This have a great impact in
 * the long term impact of your programm because memory fragmentation
 * is kept as low as possible.
 *
 * Keep in mind that this function should not be used for static names
 * declared on the code. Example:
 * \code
 *    // propery node creation 
 *    axlNode * node = axl_node_create ("test");
 *  
 *    // NOT PROPER node creation
 *    axlNode * node = axl_node_create_ref ("test");
 * \endcode
 *
 * @param name A reference to the node name that is hold by
 * dinamically allocated memory from the user space code.
 * 
 * @return A newly created \ref axlNode reference that must be
 * deallocated by \ref axl_node_free.
 */
axlNode * axl_node_create_ref         (char * name)
{
	axl_return_val_if_fail (name, NULL);
	
	/* return a reference to a new axlNode */
	return __axl_node_create_internal (name);
}

/** 
 * @brief Allows to get the xml document (\ref axlDoc) where the
 * provided xml node is stored.
 * 
 * @param node The node that is being requested to return is xml
 * document reference.
 * 
 * @return The xml document reference or NULL if it is not already
 * set. Once a xml node is created, it must be added to the xml
 * document either because it is the parent node, in that case \ref
 * axl_doc_set_root must be used, or either because the node is being
 * added as a child of a xml node that is the already added root node
 * or a child of it.
 */
axlDoc  * axl_node_get_doc                  (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);
	
	/* return the internal reference */
	return node->doc;
}

/** 
 * @internal
 *
 * This function is not provided to the public API because it is used
 * by the Axl internals to set the root node for a provided xml node,
 * that is usually the root document xml node.
 * 
 * @param node The node that is being configured to have, or being contained in, the provided xml document.
 *
 * @param doc The xml document that holds the node.
 */
void      axl_node_set_doc                  (axlNode * node, axlDoc * doc)
{
	axl_return_if_fail (node);
	axl_return_if_fail (doc);

	node->doc = doc;

	return;
}

/** 
 * @internal
 * 
 * Support function to initialize piTargets if required.
 * 
 * @param node 
 */
void __axl_node_init_pitargets (axlNode * node)
{
	/* do not init pi targets once done */
	if (node->piTargets != NULL)
		return;

	/* create PI list */
	node->piTargets  = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_pi_free);	
}

/** 
 * @internal
 *
 * Support function which initializes the attribute list if required.
 * 
 * @param node 
 */
void __axl_node_init_attributes (axlNode * node)
{
	/* do not init attribute list twice */
	if (node->attributes != NULL)
		return;
	
	/* create attribute list */
	node->attributes = axl_list_new (__axl_node_equal, (axlDestroyFunc) __axl_node_destroy_attr);
}

/** 
 * @internal
 *
 * Support function which initializes current childs node list, if
 * necessary.
 * 
 * @param node The axlNode where the child node initialization will be
 * performed.
 */
void __axl_node_init_childs (axlNode * node)
{
	/* do not init childs if already created */
	if (node->childs != NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "not initializing the child list for <%s>0x%x, pointer is defined",
			   node->name, node);
		return;
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "initializing the child list for <%s>0x%x, pointer isn't defined",
		   node->name, node);
	
	/* create the child list */
	node->childs   = axl_list_new (__axl_node_equal, (axlDestroyFunc) axl_node_free);	
	
	return;
}

/** 
 * @internal
 * 
 * Internal function to perform common checks for setting attribute
 * code.
 */
bool         __axl_node_set_attribute_common_check (axlNode * node, char * attribute, char * value)
{
	/* checks values received */
	axl_return_val_if_fail (node, false);
	axl_return_val_if_fail (attribute, false);
	axl_return_val_if_fail (value, false);

	/* attribute could be added twice */
	axl_return_val_if_fail (! axl_node_has_attribute (node, attribute), false);

	/* check ok */
	return true;
}

/** 
 * @internal
 * 
 * Support function to install attribute information provided into the
 * \ref axlNode provided.
 */
void      __axl_node_set_attribute      (axlNode * node, char * attribute, char * value)
{
	axlAttribute * _attribute;
	
	/* init attribute list */
	/* do not init attribute list twice */
	__axl_node_init_attributes (node);

	/* create the attribute */
	_attribute        = axl_new (axlAttribute, 1);
	_attribute->name  = attribute;
	_attribute->value = value;

	/* add the attribute */
	axl_list_add (node->attributes, _attribute);

	return;
}

/** 
 * @brief Allows to configure an xml attribute to the given node.
 *
 * Attributes are that piece of the xml node definition that is
 * defined the by pair attr=value. Here is an example of an xml node
 * with an attribute:
 * 
 * \code
 * <complex attr1='value'>
 *   ..
 * </complex>
 * \endcode
 *
 * It is not permited to store the same attributes several times
 * inside the same node. If the function detects that a node is being
 * set to have the same attribute name several times, the attribute
 * will not be added.
 *
 * Values for the attribute name (<b>attribute</b>) and its value can
 * be deallocated once the function finish. This function will perform
 * a local copy.
 * 
 * @param node The \ref axlNode where the attribute will be installed
 *
 * @param attribute The attribute name to configure. This value can't
 * be NULL.
 *
 * @param value The value associated to the attribute to be
 * configured. This value can't be NULL.
 */
void      axl_node_set_attribute      (axlNode * node, char * attribute, char * value)
{
	int    additional_size = 0;
	char * _attr;
	char * _value;

	/* perform initial checks */
	if (!__axl_node_set_attribute_common_check (node, attribute, value))
		return;

#ifdef SHOW_DEBUG_LOG
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting attribute: %s='%s'", attribute, value);
#endif

	/* check attribute name */
	if (__axl_node_content_have_not_valid_sequences (attribute, strlen (attribute),
							 &additional_size)) {
#ifdef SHOW_DEBUG_LOG
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found attribute content with escapable, non-valid content");
#endif
		_attr = __axl_node_content_copy_and_escape (attribute, 
							    strlen (attribute),
							    additional_size);
	}else {
#ifdef SHOW_DEBUG_LOG
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "'%s' is a valid string..", attribute);
#endif
		_attr = axl_strdup (attribute);
	}

	/* check attribute value */
	additional_size = 0;
	if (__axl_node_content_have_not_valid_sequences (value, strlen (value),
							 &additional_size)) {

		_value = __axl_node_content_copy_and_escape (value, 
							     strlen (value),
							     additional_size);
	}else {
#ifdef SHOW_DEBUG_LOG
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "'%s' is a valid string..", value);
#endif
		_value = axl_strdup (value);
	}

	/* insert the attribute */
	__axl_node_set_attribute (node, _attr, _value);
	return;
}

/** 
 * @brief Allows to install a new attribute pair, based on the
 * attribute name and the attribute value, without allocating memory
 * for them.
 * 
 * This function works the same way like \ref axl_node_set_attribute
 * but reusing memory allocated by the user space to avoid allocating
 * information twice.
 * 
 * @param node The \ref axlNode where the attributes will be
 * installed.
 *
 * @param attribute The attribute name to be installed.
 *
 * @param value The attribute value to be installed.
 */
void      axl_node_set_attribute_ref  (axlNode * node, char * attribute, char * value)
{
	/* perform initial checks */
	if (!__axl_node_set_attribute_common_check (node, attribute, value))
		return;

	/* insert the attribute */
	__axl_node_set_attribute (node, attribute, value);

	return;
}

/** 
 * @internal
 *
 * @brief Allows to get the attribute inside the node, that has as
 * name the value provided.
 */
axlAttribute * __axl_node_common_attr_get (axlNode * node, char * attribute)
{
	axlAttribute * attr;
	int            iterator = 0;

	axl_return_val_if_fail (node, false);
	axl_return_val_if_fail (attribute, false);

	/* assume the attribute requested doesn't exist because the
	 * attribute list is not initialized  */
	if (node->attributes == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the node doesn't have any attribute installed");
		return NULL;
	}

	/* get the first */
	while (iterator < axl_list_length (node->attributes)) {
		attr = axl_list_get_nth (node->attributes, iterator);

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking attribute name='%s'", attr->name);

		if (axl_cmp (attr->name, attribute))
			return attr;

		iterator++;
	}

	/* attribute not found */
	return NULL;
}

/** 
 * @brief Allows to check if a particular attribute is installed on
 * the given node.
 *
 * @param node The node where the attribute will be checked to be
 * configured.
 *
 * @param attribute The attribute to check.
 * 
 * @return A \ref true if the attribute value is set, otherwise
 * \ref false is returned.
 */
bool          axl_node_has_attribute      (axlNode * node, char * attribute)
{
	/* attribute not found */
	return (__axl_node_common_attr_get (node, attribute) != NULL);
}

/** 
 * @brief Allows to get current content of the provided attribute
 * inside the given node.
 *
 * It is recomended to call first to \ref axl_node_has_attribute to
 * ensure that the attribute to be reported its value already exists.
 *
 * @param node The \ref axlNode the the attribute value associated
 * will be returned.
 *
 * @param attribute The attribute that is being required its value.
 * 
 * @return A string containing the attribute value or NULL if
 * fails. Returned value must not be deallocated, it is a reference to
 * a local copy. Use \ref axl_strdup function to get a persistent
 * copy.
 */
char    * axl_node_get_attribute_value (axlNode * node, char * attribute)
{
	axlAttribute * attr;

	attr = __axl_node_common_attr_get (node, attribute);
	if (attr == NULL)
		return NULL;
	return attr->value;
}

/** 
 * @brief Gets an allocated copy for the value associated to the given
 * attribute.
 *
 * See \ref axl_node_get_attribute_value for more information. This
 * function works the same way but returning an already allocated
 * attribute value.
 * 
 * @param node The node where the attribute value is requested.
 *
 * @param attribute The attribute that is being requested.
 * 
 * @return A newly allocated reference that must be deallocated when
 * no longer needed calling to axl_free.
 */
char    * axl_node_get_attribute_value_copy (axlNode * node, char * attribute)
{
	char * _value;

	/* get the attribute */
	_value = axl_node_get_attribute_value (node, attribute);
	axl_return_val_if_fail (_value, NULL);
	
	/* return a copy */
	return axl_strdup (_value);
}

/** 
 * @brief Gets the attribute content for the provided attribute name,
 * at the provided node, but translating entity references.
 *
 * This function works the same way like \ref
 * axl_node_get_attribute_value_copy, in the sense it returns a
 * dinamically allocated copy for the attribute value requested but,
 * at the same time, it returns the content with all entity references
 * already translated.
 *
 * If the attribute content has <b>&amp;</b>, <b>&lt;</b>, etc, this
 * function will translate them into the corresponding values.
 * 
 * @param node The \ref axlNode instance where the attribute content
 * will be returned.
 *
 * @param attribute The attribute name that is being requested.
 * 
 * @return The attribute content, already translated, for those
 * entities found. The function returns a dinamilly allocated string
 * so \ref axl_free must be used.
 */
char    * axl_node_get_attribute_value_trans (axlNode * node, char * attribute)
{
	char * _value;
	int    size;

	/* get the attribute */
	_value = axl_node_get_attribute_value (node, attribute);
	axl_return_val_if_fail (_value, NULL);

	/* perform a local copy */
	_value = axl_strdup (_value);
	
	/* return a copy */
	size   = strlen (_value);
	return __axl_node_content_translate_defaults (_value, &size);
}


/** 
 * @brief Allows to configure the given node to be empty.
 *
 * A \ref axlNode is empty when it is known that the node doesn't have
 * any content inside it as a child element.
 *
 * @param node The node to configure as empty.
 *
 * @param empty The value for emptyness to be used. false will
 * mean that the node is not empty.
 */
void      axl_node_set_is_empty (axlNode * node, bool     empty)
{
	axl_return_if_fail (node);
	
	/* set value received */
	node->is_empty = empty;

	return;
}

/** 
 * @brief Allows to get current xml node name (represented the xml node by \ref axlNode).
 *
 * The name of a node is the label that identifies it at the start and
 * close tag. On the following example, the xml node name is
 * considered to be <b>"data"</b>.
 *
 * \code
 * <data>...</data>
 * \endcode
 *
 * If it is required to check if the given \ref axlNode have a
 * particular name you can use the macro \ref NODE_CMP_NAME.
 *
 * Here is an example:
 * \code
 * void check_name (axlNode * node) {
 *     if (NODE_CMP_NAME (node, "data")) {
 *         // we got a xml node called "data" 
 *     }
 *   // the node doesn't have that name
 * }
 * \endcode
 *
 * @param node The \ref axlNode where the name will be returned.
 * 
 * @return A string reference containing the name. Returned value must
 * not deallocated. If a copy is required use \ref axl_strdup
 * function.
 */
char    * axl_node_get_name           (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);

	return node->name;
}

/** 
 * @brief Allows to get the parent xml node (\ref axlNode) that is holding
 * as child the provided xml node reference.
 *
 * When a node holds childs, it is also created a parent relation from
 * the child to the parent. This function allows to return that
 * reference.
 * 
 * @param node The xml node that is requested to return its parent
 * node.
 * 
 * @return An internal reference to the parent node, that must not be
 * deallocated, or NULL if fails. The function will also return NULL
 * if the node provided is the document root. The function could only
 * fail if the provided reference is NULL.
 */
axlNode * axl_node_get_parent         (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);

	/* returns a reference to the parent */
	return node->parent;
}

/** 
 * @brief Allows to get the node that is located, at the same level,
 * on the next position of the child list.
 *
 * When a parent node holds more node childs, all of them have the
 * same parent child, and at the same time, all of them have a brother
 * relation. This relation makes that to nodes that are childs for a
 * parent node, are positioned sequentially as childs for the parent.
 *
 * This function allows to get the next childs that is stored at the
 * next position, inside the same level, for the given child node.
 *
 * @param node The node to get the next xml node reference.
 * 
 * @return Returns an internal reference to the next xml node or NULL
 * if fails. The function will also return NULL if no next xml node is
 * found starting from the provided reference. The root node will
 * always returns a NULL reference.
 */
axlNode * axl_node_get_next           (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);

	/* return the next reference */
	return node->next;
}

/** 
 * @brief Allows to get current emptyness configuration for the given
 * \ref axlNode.
 *
 * An \ref axlNode, which is a representation of a xml node, is
 * considered to be empty only if it has no content inside it. The
 * following xml code snipet shows what is considered the content:
 *
 * \code
 * <data>
 *    Xml content, data node content
 * </data>
 * \endcode
 * 
 * If a node have content, this function will return \ref
 * false. The content must not be confused with the node childs. A
 * xml node (\ref axlNode) could be empty but have childs at the same
 * time (\ref axl_node_have_childs).
 *
 * The following xml code snipet shows a xml &lt;data> node with
 * childs that have content, but the parent node, &lt;data> do not
 * have content, therefore is empty.
 *
 * \code
 * <data>
 *    <row>Some data</row>
 *    <row>More content</row>
 * </data>
 * \endcode
 *
 * A node that is empty will return NULL data once called to \ref axl_node_get_content.
 * 
 * @param node The node to check for its empty status. 
 * 
 * @return \ref true if the node is empty or \ref false if
 * not.
 */
bool          axl_node_is_empty        (axlNode * node)
{
	axl_return_val_if_fail (node, false);

	return node->is_empty;
}

/** 
 * @brief Allows to get current xml node content (\ref axlNode).
 * 
 * See \ref axl_node_is_empty for more details. This function allows
 * to get current xml node content, which is the free text enclosed by
 * the node.
 *
 * Returned value is an internal reference to the content stored. So,
 * in the case a local copy is desired, you should check \ref
 * axl_node_get_content_copy. 
 *
 * Keep in mind that the content returned could have references like
 * "&amp;" or "&quot;" which are entities references not translated
 * into the application level values. 
 * 
 * This is done because while using the content, you may be interested
 * on getting the raw content to be passed to another xml parser which
 * is also able to process that entities.
 *
 * If you don't like this behaviour you can check \ref
 * axl_node_get_content_trans which returns a copy for the xml node
 * content with all entities references translated.
 * 
 * @param node The \ref axlDoc node where the content will be retrieved.
 *
 * @param content_size Optional pointer to an integer variable where
 * the content size will be reported. If the variable is not set, the
 * function will not report the content size.
 * 
 * @return Current xml node content. You must not deallocate reference
 * returned. If you want a permanet copy you should use \ref
 * axl_node_get_content_copy. Keep in mind that the function will always
 * return an string reference. In the case the node has no content, an
 * empty string will be returned, not NULL.
 */
char    * axl_node_get_content     (axlNode * node, int * content_size)
{
	axl_return_val_if_fail (node, NULL);

        if (content_size != NULL)
		*content_size = node->content_size;

        /* return the node content or empty string */
	return (node->content != NULL) ? node->content : "";
}

/** 
 * @brief Allows to set content to the given \ref axlNode instance.
 *
 * The xml node content is that part defined inside two xml starts,
 * using the same label, that are balanced. Here is an example:
 * \code
 * <data>
 *   Content inside the xml node.
 * </data>
 * \endcode
 *
 * Because the function will perform a local copy for the provided
 * data, it will also check for especial entities to be placed
 * properly. 
 *
 * The following characters represents the set of characters that
 * should be referenced by using its associated entity reference. But,
 * if provided as is, the function will translate them into the
 * appropiate entity reference.
 *
 * <table>
 * <tr><td><b>Character</b></td><td>Entity name</td></tr>
 * <tr><td>'</td><td>&apos;</td></tr>
 * <tr><td><</td><td>&lt;</td></tr>
 * <tr><td>></td><td>&gt;</td></tr>
 * <tr><td>&</td><td>&amp;</td></tr>
 * <tr><td>"</td><td>&quot;</td></tr>
 * </table>
 *
 * In general it is a good idea to espace previous sequences by
 * providing the right entity value, avoding the additional
 * computation required to translate the value received. 
 * 
 * Rembember that valid XML documents have these values escaped. 
 * 
 * @param node The xml node, represented by an already initialized
 * \ref axlNode, where the node content will be set.
 *
 * @param content The content to set to the \ref axlNode. The function
 * will perform a local copy. Provided value could be unreferenced
 * once the function finish.
 *
 * @param content_size The content size that is being provided. If -1
 * is used, the function will use strlen function to get current
 * content size.
 */
void      axl_node_set_content        (axlNode * node, char * content, int content_size)
{
	int additional_size = 0;

	axl_return_if_fail (node);
	axl_return_if_fail (content);

	/* get current content in the case a -1 is provided */
	if (content_size == -1)
		content_size = strlen (content);


	/* check if the string received have escapable characters */
	if (__axl_node_content_have_not_valid_sequences (content, content_size, 
							 &additional_size)) {
		/* allocate the content */
		node->content = __axl_node_content_copy_and_escape (content, 
								    content_size, 
								    additional_size);
		/* set node content size */
		node->content_size   = content_size + additional_size;
	}else {
		/* set current content */
		node->content_size   = content_size;
		node->content        = axl_new (char, node->content_size + 1);
		memcpy (node->content, content, node->content_size);
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting xml node (name: %s) content (size: %d) %s",
		   node->name, node->content_size, node->content);

	/* set that the node is not empty */
	node->is_empty = false;

	/* job done */
	return;
}

/** 
 * @brief Set the content for the provided node, reusing the reference
 * provided, without making a local copy.
 *
 * This function works like \ref axl_node_set_content_ref but without
 * copy memory provided. This allows reduce memory allocations if the
 * memory was already allocated by the user space.
 *
 * Because this function doesn't perform a copy, if the content
 * received has to be escaped, the function will fail. To use this
 * function the caller must ensure that entity references are used to
 * especify the &, ', ", < or >.
 * 
 * @param node The \ref axlNode where the content will be set.
 *
 * @param content The user space allocated content to be set to the
 * node.
 *
 * @param content_size The content size.
 */
void      axl_node_set_content_ref    (axlNode * node, 
				       char * content, 
				       int content_size)
{
	axl_return_if_fail (node);
	axl_return_if_fail (content);

	/* get current content in the case a -1 is provided */
	if (content_size == -1)
		content_size = strlen (content);
	node->content_size = content_size;

	/* set current content */
	node->content  = content;
	node->is_empty = false;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting xml node (name: %s) content (size: %d) %s",
		   node->name, node->content_size, node->content);

	/* job done */
	return;	
}

/** 
 * @brief Allows to store data as content of the provided noe to be
 * enclosed as CDATA.
 *
 * Some characters are not allowed to be stored "as is" inside a
 * parseable XML document. The basic set of them are: &, ', ", < or >.
 *
 * In order to store content containing previous characters inside an
 * xml node, and to remain valid, functions like \ref
 * axl_node_set_content will translate those value, into the accepted
 * scape sequences. However, in the other hand, the entity parsing the
 * document produced must perform the translation back to the normal
 * state.
 *
 * As an alternative, the XML node content could be stored enclosed as
 * a CDATA section: <![CDATA[..]]> allow to store "un-parsed"
 * characters, including those not allowed.
 * 
 * @param node The node where the CDATA will be stored.
 *
 * @param content The content to store.
 *
 * @param content_size The content size or -1 if required Axl to
 * figure out current sizes.
 */
void      axl_node_set_cdata_content  (axlNode * node,
				       char * content,
				       int content_size)
{
	char * cdata;

	axl_return_if_fail (node);
	axl_return_if_fail (content);

	cdata = axl_stream_strdup_printf ("<![CDATA[%s]]>", content);
	
	/* calculate the new content size */
	if (content_size > 0)
		content_size = content_size + 12;
	
	/* call to set node content */	
	axl_node_set_content_ref (node, cdata, content_size);

	return;
}


/** 
 * @brief Allows to get a copy for the content stored inside the given
 * \ref axlNode reference. 
 *
 * This function works the same way than \ref axl_node_get_content but
 * returning a copy from the internal content, that the caller is
 * responsible of deallocating it.
 * 
 * @param node The \ref axlNode where the content is being required.
 *
 * @param content_size An optinal reference to an integer variable
 * where the content size will be returned.
 * 
 * @return A newly allocated string representing the node content.
 */
char    * axl_node_get_content_copy (axlNode * node, int * content_size)
{
	int    _content_size;
	char * result;
	char * content;

	/* get the content and check if it is defined */
	if (content_size)
		content = axl_node_get_content (node, content_size);
	else
		content = axl_node_get_content (node, &_content_size);

	/* check result */
	if (content == NULL)
		return axl_strdup ("");

	/* allocate enough memory for the result */
	if (content_size) {
		result = axl_new (char, (*content_size) + 1);
		memcpy (result, content, *content_size);
	}else {
		result = axl_new (char, _content_size + 1);
		memcpy (result, content, _content_size);
	}
  	
	/* return a newly allocated reference to the content */
	return result;
}

/** 
 * @brief Allows to get the content inside the provided node, trimming
 * the header and trailing white spaces found.
 *
 * Note that calling to this function will modify the node content,
 * removing "white spaces" found. Once the function is called, the
 * node content will be returned by \ref axl_node_get_content already
 * trimmed.
 *
 * @param node The node where the content will be trimmed and
 * returned.
 *
 * @param content_size The node content size reference where the
 * content size will be reported.
 * 
 * @return The reference returned is an internal copy that must not be
 * deallocated. The function always return content. If the node has no
 * content, the function will return an empty string (but never a NULL
 * value).
 */
char    * axl_node_get_content_trim   (axlNode * node,
				       int * content_size)
{
	int    trimmed;

	/* trim the content */
	axl_stream_trim_with_size (node->content, &trimmed);

	/* updates current internal content size */
	node->content_size -= trimmed;

	/* notify content size to the caller */
	if (content_size != NULL)
		*content_size = node->content_size;

	/* return an internal reference to the node content */
	return (node->content != NULL) ? node->content : "";
}

/** 
 * @brief Allows to the get node content, performing a memory
 * allocation for the returned result, translating default entities
 * values with its replacement text.
 * 
 * @param node The XML node where the content is being requested.
 *
 *
 * @param content_size An optional reference to an integer variable to
 * return the node content size.
 * 
 * @return A newly allocated string, representing the node content,
 * with all entities references already translated into the
 * replacement text.
 */
char    * axl_node_get_content_trans (axlNode * node, int * content_size)
{
	char * result;
	int    _content_size = 0;

	/* get a copy for the node content, getting the node content
	 * size. If the user don't provide a reference, use a local
	 * one. */
	if (content_size)
		result = axl_node_get_content_copy (node, content_size);
	else
		result = axl_node_get_content_copy (node, &_content_size);

	/* check result returned */
        if (result == NULL)
                return "";;

	/* translate all references that performs the entities to the
	 * replacement text. */
	if (content_size)
		return __axl_node_content_translate_defaults (result, content_size);
	return __axl_node_content_translate_defaults (result, &_content_size);
}


/** 
 * @brief Allows to configure a child node to the given parent.
 * 
 * @param parent The parent node.
 *
 * @param child The child node. The child node must be a deep
 * copy. Passing several references, pointing to the same value, will
 * cause to seg fault the program at the time the parent child is
 * deallocated. 
 */
void      axl_node_set_child (axlNode * parent, axlNode * child)
{
	axlNode * last;

	axl_return_if_fail (parent);
	axl_return_if_fail (child);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received parent=0x%x and child=0x%x",  parent, child);

	/* init childs list */
	__axl_node_init_childs (parent);

	/* establish the parent relation */
	child->parent = parent;

	/* configure the document that contains the xml node by using
	 * the one configured at the parent. */
	child->doc    = parent->doc;

	/* get the current last child */
	last = axl_list_get_last (parent->childs);
	if (last != NULL) {
		/* establish the next relation for the last node to
		 * point to the new last node */
		last->next = child;
	}
	
	/* add the child node to the parent node */
	axl_list_add (parent->childs, child);

	return;
}

/** 
 * @brief Allows to configure that the given node have child nodes.
 *
 * <b>DEPRECATED:</b> The function doesn't perform any operation. See
 * \ref axl_node_have_childs.
 * 
 * @param node The node to configure.
 *
 * @param childs The child configuration, true to notify that the
 * node have childs, otherwise, false is returned.
 */
void      axl_node_set_have_childs (axlNode * node, bool     childs)
{
	/* */
	return;
}

/** 
 * @brief Allows to get current childs configuration for the given xml
 * node (\ref axlNode).
 *
 * An xml node (represented by an \ref axlNode) is considered to have
 * childs only if it has more xml child nodes. The content is not
 * considered be a child. See \ref axl_node_is_empty for more
 * information.
 * 
 * @param node The \ref axlNode reference.
 * 
 * @return An \ref true if the \ref axlNode have childs or \ref
 * false if not.
 */
bool          axl_node_have_childs        (axlNode * node)
{
	axl_return_val_if_fail (node, false);

	/* if childs list is NULL, assume the node doesn't have
	 * childs */
	if (node->childs == NULL)
		return false;
	
	/* return current configuration */
	return (axl_list_length (node->childs) > 0);
}

/** 
 * @brief Allows to get a particular child node for the given node (\ref axlNode).
 * 
 * @param parent The parent node where the child will be looked up.
 *
 * @param name The name for the child to search.
 * 
 * @return A refernce to a \ref axlNode or NULL if no child exists
 * called by the name provided, inside the node provided.
 */
axlNode * axl_node_get_child_called   (axlNode * parent, char * name)
{
	axlNode * node;
	int       iterator = 0;
	int       length   = 0;
	
	axl_return_val_if_fail (parent, NULL);
	axl_return_val_if_fail (name, NULL);

	/* if the child list is not defined, assume there is no node
	 * called the name requested */
	if (parent->childs == NULL)
		return NULL;
	
	/* if no childs, no result */
	length = axl_list_length (parent->childs);
	if (length == 0)
		return NULL;

	/* iterate over the childs until get the one called
	 * <b>name</b> */
	while (iterator < length) {
		node = axl_list_get_nth (parent->childs, iterator);
		if (NODE_CMP_NAME (node, name))
			return node;
		iterator++;
	}

	/* no child was found */
	return NULL;
}

/** 
 * @brief Allows to get the child that is located at the given
 * position, for the given parent node.
 *
 * @param parent The parent node where the child will be looked up.
 *
 * @param position The position where the child will be looked up. The
 * values for the position ranges from 0 up to (N - 1).
 * 
 * @return A reference to the child node \ref axlNode or NULL if fails. 
 */
axlNode * axl_node_get_child_nth      (axlNode * parent, int position)
{
	/* perform some environment checks */
	axl_return_val_if_fail (parent, 
				NULL);
	axl_return_val_if_fail (position >= 0 && position < axl_list_length (parent->childs),
				NULL);

	/* if not initialized childs list, assume there is no node at
	 * the provided position */
	if (parent->childs == NULL)
		return NULL;


	return axl_list_get_nth (parent->childs, position);
}

/** 
 * @brief Allows to get the number of childs the provided node has.
 * 
 * @param parent The node where the number of childs is being queried.
 * 
 * @return The number of childs or -1 if fails.
 */
int       axl_node_get_child_num      (axlNode * parent)
{
	/* perform some environment checks */
	axl_return_val_if_fail (parent, 
				-1);

	/* if no child list is created, assume 0 */
	if (parent->childs == NULL)
		return 0;

	/* return current child number */
	return axl_list_length (parent->childs);
}


/** 
 * @brief Allows to get childs nodes for the given xml node (\ref
 * axlNode).
 *
 * @param node The node where the childs will be returned.
 * 
 * @return An \ref axlList containing \ref axlNode items or NULL if it
 * fails. The list returned must not be deallocated.
 */
axlList * axl_node_get_childs         (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);

	/* return current childs */
	return node->childs;
}

/** 
 * @brief Allows to check if the provided references represents two
 * equivalent nodes.
 *
 * The function will check node name, node content and node attributes
 * and its values.
 * 
 * @param node The node to check.
 * @param node2 The second node to check.
 * 
 * @return true if both nodes are equivalent or false if not.
 */
bool          axl_node_are_equal          (axlNode * node, axlNode * node2)
{
	int             iterator = 0;
	int             length;
	
	axlAttribute  * attr;
	axlAttribute  * attr2;
	
	axl_return_val_if_fail (node,  false);
	axl_return_val_if_fail (node2, false);

	/* check document root name */
	if (! axl_cmp (axl_node_get_name (node), axl_node_get_name (node2))) {
		return false;
	}

	/* check empty ness configuration */
	if (axl_node_is_empty (node) != axl_node_is_empty (node2)) {
		return false;
	}
	
	/* check childs configuration */
	if (axl_node_have_childs (node) != axl_node_have_childs (node2)) {
		return false;
	}

	/* check childs number */
	if (axl_node_get_child_num (node) != axl_node_get_child_num (node2)) {
		return false;
	}

	/* check attribute values */
	if (node->attributes != NULL) {
		if (axl_list_length (node->attributes) != axl_list_length (node2->attributes)) {
			return false;
		}
		
		length = axl_list_length (node->attributes);
		while (iterator < length) {
			/* get a reference to the attributes to compare */
			attr  = axl_list_get_nth (node->attributes, iterator);
			attr2 = axl_list_get_nth (node2->attributes, iterator);
			
			/* check attribute name */
			if (! axl_cmp (attr->name, attr2->name))
				return false;
			
			/* check attribute name */
			if (! axl_cmp (attr->value, attr2->value))
				return false;
			
			/* update the iterator index */
			iterator++;
		}
	} /* end if */


	/* both nodes seems to be equal */
	return true;
}

/** 
 * @brief Allows to associate a PI element, including its content to
 * the provided node.
 *
 * See the following function for more information: \ref
 * axl_doc_add_pi_target.
 *
 * @param node The \ref axlNode where the PI element (\ref axlPI) will
 * be added.
 *
 * @param target The PI target name to add.
 *
 * @param content Optional PI content.
 */
void      axl_node_add_pi_target            (axlNode * node, 
					     char * target, 
					     char * content)
{
	axlPI * pi;

	/* perform some environmental checks */
	axl_return_if_fail (node);
	axl_return_if_fail (target);

	/* create the PI element */
	pi = axl_pi_create (target, content);

	/* init pi targets */
	__axl_node_init_pitargets (node);

	/* add the PI */
	axl_list_add (node->piTargets, pi);

	return;
}


/** 
 * @brief Allows to check if the provided Processing instruction
 * target is defined on the given xml node document (\ref axlNode).
 *
 * Processing instruction are a way to configure the xml node document
 * with processing information to instruct the application level that
 * is going to consume the XML information.
 *
 * @param node The \ref axlNode where the processing instruction will
 * be read.
 *
 * @param pi_target The process instruction name.
 * 
 * @return true is the processing instruction is defined,
 * otherwise false is returned.
 */
bool          axl_node_has_pi_target            (axlNode * node, 
						 char * pi_target)
{
	axlPI * pi;
	int     iterator = 0;
	int     length   = 0;

	
	axl_return_val_if_fail (node,      false);
	axl_return_val_if_fail (pi_target, false);

	/* assume the pi target doesn't exist if it is not
	 * initialized */
	if (node->piTargets == NULL)
		return false;

#ifdef SHOW_DEBUG_LOG
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking it target element: %s is defined on the node", pi_target);
#endif

	/* get the length for the items inserted */
	length = axl_list_length (node->piTargets);
	while (iterator < length) {
		/* for each item inserted */
		pi = axl_list_get_nth (node->piTargets, iterator);
		/* only check the first ocurrency */
		if (axl_cmp (axl_pi_get_name (pi), pi_target)) {
#ifdef SHOW_DEBUG_LOG
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "element %s found, %s", pi_target, axl_pi_get_name (pi));
#endif
			return true;
		}

		iterator++;
	}
	
	return false;
}

/** 
 * @brief Allows to get current processing instruction content.
 * 
 * @param node The document where the processing instruction is placed.
 *
 * @param pi_target The processing instruction target to get current
 * content.
 * 
 * @return An internal reference to the process instruction target
 * content. Value returned mustn't be deallocated
 */
char    * axl_node_get_pi_target_content    (axlNode * node, 
					    char * pi_target)
{
	axlPI * pi;
	int     iterator = 0;
	int     length   = 0;

	axl_return_val_if_fail (node,       NULL);
	axl_return_val_if_fail (pi_target, NULL);

	/* assume NULL content if the pitarget list is not
	 * initialized */
	if (node->piTargets == NULL)
		return NULL;

	/* get the length for the items inserted */
	length = axl_list_length (node->piTargets);
	while (iterator < length) {
		/* for each item inserted */
		pi = axl_list_get_nth (node->piTargets, iterator);
		/* only check the first ocurrency */
		if (axl_cmp (axl_pi_get_name (pi), pi_target))
			return axl_pi_get_name (pi);

		iterator++;
	}

	return NULL;
}

/** 
 * @brief Allows to get a list which contains \ref axlPI nodes,
 * representing all process instruction that the \ref axlNode (xml
 * document node) has.
 *
 * While using PI, you can use the following functions to get PI
 * information:
 * 
 *  - \ref axl_node_has_pi_target
 *  - \ref axl_node_get_pi_target_content
 *
 * However, this function will return first ocurrency for PI found
 * inside the xml document. If you don't use repeated PI elements, you
 * won't find problems, but, if you need to iterate ever all PI found
 * or you are using repeated PI, you can use this function as follows
 * to get current pi elements:
 *
 * \code
 * void show_all_pi (axlNode * node) 
 * {
 *      int       iterator;
 *      axlPI   * pi;
 *      axlList * PIs;
 *
 *      // get all PI target that the node has
 *      PIs      = axl_node_get_pi_target_list (node);
 *      iterator = 0;
 *
 *      while (iterator < axl_list_length (PIs)) {
 *            // get next pi stored 
 *            pi = axl_list_get_nth (PIs, iterator);
 *
 *            // do some stuff 
 *            printf ("PI found target name=%s, content=%s\n",
 *                    axl_pi_get_name (pi),
 *                    axl_pi_get_content (pi));
 *            
 *            // update the iterator
 *            iterator++;
 *      }
 *      return;
 * }
 * \endcode
 * 
 * @param node The xml node (\ref axlNode) where the process
 * instruction will be returned.
 * 
 * @return A reference to the list of processing instruction that the
 * xml node (\ref axlNode) has.
 */
axlList * axl_node_get_pi_target_list       (axlNode * node)
{
	axl_return_val_if_fail (node,       NULL);

	return node->piTargets;
}

/** 
 * @internal
 * @brief Returns the space required to write the attribute part.
 */
int __axl_node_get_flat_size_attributes (axlNode * node)
{
	axlAttribute * attr;
	int            length;
	int            iterator;
	
	/* if attribute list is not defined just return 0 */
	if (node->attributes == NULL)
		return 0;

	length   = 0;
	iterator = 0;
	while (iterator < axl_list_length (node->attributes)) {

		/* get a reference to the attributes to compare */
		attr   = axl_list_get_nth (node->attributes, iterator);

		/* " attribute='value' */
		length += 4 + strlen (attr->name) + strlen (attr->value);

		/* update iterator */
		iterator++;
	}

	/* return the length */
	return length;
}

/** 
 * @internal
 *
 * Returns which the size that the node and its childs will hold if
 * the are represented into a flat xml stream.
 * 
 * @param node The node that is requested its stream xml size.
 * 
 * @return The stream size or -1 if fails.
 */
int       axl_node_get_flat_size            (axlNode * node)
{
	int       result   = 0;
	int       iterator = 0;
	axlNode * child;

	axl_return_val_if_fail (node, -1);

	/* check if the node is empty */
	if (axl_node_is_empty (node)) {
		if (! axl_node_have_childs (node)) {
			/* "<" + strlen (node-name) + " />" */
			return strlen (node->name) + 4 + __axl_node_get_flat_size_attributes (node);
		}
	}

	/* the node is emtpy because it has no content but it has
	 * childs:
	 * "<" + strlen (node-name) + ">" + strlen (node-content) + 
	 * "</" + strlen (node-name) + ">" */
	result = 5 + (2 * strlen (node->name)) + node->content_size + __axl_node_get_flat_size_attributes (node);

	/* if the node have childs */
	if (axl_node_have_childs (node)) {

		while (iterator < axl_node_get_child_num (node)) {
			/* get a reference to the child */
			child   = axl_node_get_child_nth (node, iterator);

			/* count how many bytes the node holds */
			result += axl_node_get_flat_size (child);

			/* update the iterator reference */
			iterator++;
		}
	}

	/* return the result */
	return result;
}

/** 
 * @internal
 * 
 * Internal support function which dumps current attribute
 * configuration into the given memory using the provided desp.
 */
int axl_node_dump_attributes_at (axlNode * node, char * content, int desp)
{
	axlAttribute * attr;
	int            length;
	int            iterator;
	
	/* if attribute list is not defined just return 0 */
	if (node->attributes == NULL)
		return desp;

	length   = 0;
	iterator = 0;
	while (iterator < axl_list_length (node->attributes)) {

		/* get a reference to the attributes to compare */
		attr   = axl_list_get_nth (node->attributes, iterator);

		memcpy (content + desp, " ", 1);
		desp += 1;

		memcpy (content + desp, attr->name, strlen (attr->name));
		desp += strlen (attr->name);

		memcpy (content + desp, "='", 2);
		desp += 2;

		memcpy (content + desp, attr->value, strlen (attr->value));
		desp += strlen (attr->value);

		memcpy (content + desp, "'", 1);
		desp += 1;

		/* update iterator */
		iterator++;
	}

	/* return the length */
	return desp;
}

/** 
 * @internal
 *
 * Writes the node information represented by the node provided at the
 * given position into the buffer provided.
 * 
 * @param node The node to dump and its childs.
 *
 * @param content The memory buffer where the content will be dumped.
 *
 * @param desp A memory desp where to dump the node content inside the
 * memory provided.
 * 
 * @return The new desp value to be used on the next dump.
 */
int       axl_node_dump_at                  (axlNode * node,
					     char    * content,
					     int       desp)
{
	int       iterator = 0;
	axlNode * child;

	axl_return_val_if_fail (node, -1);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "dumping node=<%s> at %d", 
		   axl_node_get_name (node), desp);

	/* check if the node is empty */
	if (axl_node_is_empty (node)) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the node <%s> is empty", 
			   axl_node_get_name (node));
		if (! axl_node_have_childs (node)) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "dumping an empty node without childs=<%s>",
				   axl_node_get_name (node));
			/* "<" + strlen (node-name) + " />" */
			memcpy (content + desp, "<", 1);
			desp += 1;

			memcpy (content + desp, node->name, strlen (node->name));
			desp += strlen (node->name);

			/* dump attribute values */
			desp = axl_node_dump_attributes_at (node, content, desp);

			memcpy (content + desp, " />", 3);
			desp += 3;

			return desp;
		}
	}

	/* the node is empty because it doesn't have content but it
	 * has childs 
	 * "<" + strlen (node-name) + ">" + strlen (node-content) + 
	 * "</" + strlen (node-name) + ">" */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting content for: <%s>", axl_node_get_name (node));

	/* dump node start tag */
	memcpy (content + desp, "<", 1);
	desp += 1;

	memcpy (content + desp, node->name, strlen (node->name));
	desp += strlen (node->name);

	/* dump attribute values */
	desp = axl_node_dump_attributes_at (node, content, desp);

	memcpy (content + desp, ">", 1);
	desp += 1;
		
	/* if the node have childs */
	if (axl_node_have_childs (node)) {
		while (iterator < axl_node_get_child_num (node)) {
			/* get a reference to the child */
			child = axl_node_get_child_nth (node, iterator);

			/* count how many bytes the node holds */
			desp  = axl_node_dump_at (child, content, desp);

			/* update the iterator reference */
			iterator++;
		}
	}else {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the node is not empty and have no childs");

		/* dump node content */
		if (node->content != NULL) {
			memcpy (content + desp, node->content, strlen (node->content));
			desp += strlen (node->content);
		}
	}

	/* dump close tag */
	memcpy (content + desp,	"</", 2);
	desp += 2;
	
	memcpy (content + desp, node->name, strlen (node->name));
	desp += strlen (node->name);
	
	memcpy (content + desp,	">", 1);
	desp += 1;

	/* return the result */
	return desp;
}

void __axl_node_free_internal (axlNode * node)
{
	axl_return_if_fail (node);

	/* free current node */
	if (node->name != NULL)
		axl_free (node->name);

	/* free node content */
	if (node->content != NULL)
		axl_free (node->content);

	if (node->piTargets != NULL)
		axl_list_free (node->piTargets);

	/* release memory hold by attributes */
	if (node->attributes)
		axl_list_free (node->attributes);

	/* release memory hold by childs */
	if (node->childs != NULL) {
		axl_list_free (node->childs);
	}

	/* do not free the node itself */
	return;
}

/** 
 * @brief Destroy the given node provided by the reference.
 *
 * The function will check for nodes that are null references.
 * 
 * @param node The node to destroy. 
 */
void axl_node_free (axlNode * node) 
{
	axl_return_if_fail (node);

	/* free internal content */
	__axl_node_free_internal (node);

	/* free attributes */
	axl_free (node);
	
	/* the node to release */
	return;
}

/* @} */
