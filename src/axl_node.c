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
 * @brief Creates a new \ref AxlNode with the provided name.
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

	return node;
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
 * @brief Allows to get current emptyness configuration for the given \ref axlNode.
 *
 * A \ref axlNode, which is a representation if an xml node, is
 * considered to be empty only if it has no content inside it. The
 * following xml code snipet shows which is considered the content:
 *
 * \code
 * <data>
 *    Xml content, data node content
 * </data>
 * \endcode
 * 
 * If a node have content, this function will return \ref
 * AXL_TRUE. The content must not be confused with the node childs. An
 * xml node (\ref axlNode) could be empty but have childs. The
 * following xml code snipet shows a xml &lt;data> node with childs
 * that have content, but the parent node, &lt;data> do not have
 * content, therefore is empty.
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
	return NULL;
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

	/* release memory hold by attributes */
	axl_list_destroy (node->attributes);

	/* release memory hold by childs */
	axl_list_destroy (node->childs);

	/* free attributes */
	axl_free (node);
	
	/* the node to release */
	return;
}


