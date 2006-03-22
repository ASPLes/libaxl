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
 * \defgroup axl_node_module Axl XML Node interface: Functions to use and manipulate xml nodes inside documents.
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
 * @brief Creates a new \ref axlNode with the provided name.
 *
 * @param name The name to be used for the node be created.
 * 
 * @return 
 */
axlNode * axl_node_create (char * name)
{
	axlNode * node = NULL;

	/* perform some environmental checks */
	axl_return_val_if_fail (name, NULL);
	
	node           = axl_new (axlNode, 1);
	node->name     = axl_strdup (name);
	
	/* create the child list */
	node->childs   = axl_list_new (__axl_node_equal, (axlDestroyFunc) axl_node_free);

	/* create attribute list */
	node->attributes = axl_list_new (__axl_node_equal, (axlDestroyFunc) __axl_node_destroy_attr);

	/* create PI list */
	node->piTargets  = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_pi_free);

	return node;
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
	axlAttribute * _attribute;

	/* checks values received */
	axl_return_if_fail (node);
	axl_return_if_fail (attribute);
	axl_return_if_fail (value);

	/* attribute could be added twice */
	axl_return_if_fail (! axl_node_has_attribute (node, attribute));
	

	/* create the attribute */
	_attribute        = axl_new (axlAttribute, 1);
	_attribute->name  = axl_strdup (attribute);
	_attribute->value = axl_strdup (value);

	/* add the attribute */
	axl_list_add (node->attributes, _attribute);

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

	axl_return_val_if_fail (node, AXL_FALSE);
	axl_return_val_if_fail (attribute, AXL_FALSE);

	/* get the first */
	while (iterator < axl_list_length (node->attributes)) {
		attr = axl_list_get_nth (node->attributes, iterator);
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
 * @return A \ref AXL_TRUE if the attribute value is set, otherwise
 * \ref AXL_FALSE is returned.
 */
bool      axl_node_has_attribute      (axlNode * node, char * attribute)
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
 * @brief Allows to configure the given node to be empty.
 *
 * A \ref axlNode is empty when it is known that the node doesn't have
 * any content inside it as a child element.
 *
 * @param node The node to configure as empty.
 *
 * @param empty The value for emptyness to be used. AXL_FALSE will
 * mean that the node is not empty.
 */
void      axl_node_set_is_empty (axlNode * node, bool empty)
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
 * @brief Allows to get current emptyness configuration for the given \ref axlNode.
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
 * AXL_TRUE. The content must not be confused with the node childs. A
 * xml node (\ref axlNode) could be empty but have childs at the same
 * time (\ref axl_node_have_childs). The following xml code snipet shows a xml &lt;data> node with
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
 * 
 * 
 * @param node The node to check for its empty status. 
 * 
 * @return \ref AXL_TRUE if the node is empty or \ref AXL_FALSE if
 * not.
 */
bool      axl_node_is_empty        (axlNode * node)
{
	axl_return_val_if_fail (node, AXL_FALSE);

	return node->is_empty;
}

/** 
 * @brief Allows to get current xml node content (\ref axlNode).
 * 
 * See \ref axl_node_is_empty for more details. This function allows
 * to get current xml node content, which is the free text enclosed by
 * the node.
 * 
 * @param node The \ref axlDoc node where the content will be retrieved.
 *
 * @param content_size Optional pointer to an integer variable where
 * the content size will be reported. If the variable is not set, the
 * function will not report the content size.
 * 
 * @return Current xml node content. You must not deallocate reference
 * returned. If you want a permanet copy you should use \ref axl_node_get_content_copy.
 */
char    * axl_node_get_content     (axlNode * node, int * content_size)
{
	axl_return_val_if_fail (node, NULL);

	if (content_size != NULL)
		*content_size = node->content_size;

	return node->content;
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
	axl_return_if_fail (node);
	axl_return_if_fail (content);

	/* get current content in the case a -1 is provided */
	if (content_size == -1)
		content_size = strlen (content);
	node->content_size = content_size;

	/* set current content */
	node->content = axl_new (char, node->content_size + 1);
	memcpy (node->content, content, node->content_size);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting xml node (name: %s) content (size: %d) %s",
		 node->name, node->content_size, node->content);

	/* job done */
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
	return NULL;
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
	axl_return_if_fail (parent);
	axl_return_if_fail (child);
	
	/* add the child node to the parent node */
	axl_list_add (parent->childs, child);

	return;
}

/** 
 * @brief Allows to configure that the given node have child nodes.
 * 
 * @param node The node to configure.
 *
 * @param childs The child configuration, AXL_TRUE to notify that the
 * node have childs, otherwise, AXL_FALSE is returned.
 */
void      axl_node_set_have_childs (axlNode * node, bool childs)
{
	axl_return_if_fail (node);

	node->have_childs = childs;
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
 * @return An \ref AXL_TRUE if the \ref axlNode have childs or \ref
 * AXL_FALSE if not.
 */
bool      axl_node_have_childs        (axlNode * node)
{
	axl_return_val_if_fail (node, AXL_FALSE);
	
	/* return current configuration */
	return node->have_childs;
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
 * @return AXL_TRUE is the processing instruction is defined,
 * otherwise AXL_FALSE is returned.
 */
bool      axl_node_has_pi_target            (axlNode * node, 
					     char * pi_target)
{
	axlPI * pi;
	int     iterator = 0;
	int     length   = 0;

	
	axl_return_val_if_fail (node,      AXL_FALSE);
	axl_return_val_if_fail (pi_target, AXL_FALSE);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking it target element: %s is defined on the node", pi_target);

	/* get the length for the items inserted */
	length = axl_list_length (node->piTargets);
	while (iterator < length) {
		/* for each item inserted */
		pi = axl_list_get_nth (node->piTargets, iterator);
		/* only check the first ocurrency */
		if (axl_cmp (axl_pi_get_name (pi), pi_target)) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "element %s found, %s", pi_target, axl_pi_get_name (pi));
			return AXL_TRUE;
		}

		iterator++;
	}
	
	return AXL_FALSE;
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
 * @brief Destroy the given node provided by the reference.
 *
 * The function will check for nodes that are null references.
 * 
 * @param node The node to destroy. 
 */
void axl_node_free (axlNode * node) 
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
	axl_list_free (node->attributes);

	/* release memory hold by childs */
	axl_list_free (node->childs);

	/* free attributes */
	axl_free (node);
	
	/* the node to release */
	return;
}


/* @} */
