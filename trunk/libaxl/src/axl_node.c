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
 *         C/ Dr. Michavila Nº 14
 *         Coslada 28820 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/xml
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

struct _axlAttrCursor {
	axlPointer     data;
	int            count;
	axlNode      * node;
};

/** 
 * @internal Axl content representation used to store comments, xml
 * content, CDATA content and entity references.
 */
typedef struct _axlNodeContent axlNodeContent;

struct _axlNodeContent {
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
	
};

/** 
 * @internal Type representation to hold node attributes while they
 * are fewer.
 */
typedef struct _axlNodeAttr axlNodeAttr;

struct _axlNodeAttr {
	/** 
	 * @internal Node attribute name.
	 */
	char        * attribute;

	/** 
	 * @internal Node attribute value.
	 */
	char        * value;

	/** 
	 * @internal Flags the attribute pair to be considered from a
	 * factory.
	 */
	bool          from_factory;

	/** 
	 * @internal Next attribute.
	 */
	axlNodeAttr * next;
};

/** 
 * @internal Function that allows to copy all attributes found on the
 * list received.
 * 
 * @param list The attribute list to copy.
 * 
 * @return A newly allocated attribute list.
 */
axlNodeAttr * __axl_node_copy_attr_list (axlNodeAttr * list)
{
	axlNodeAttr * result   = NULL;

	/* if the list isn't defined, return NULL */
	if (list == NULL)
		return NULL;

	/* alloc memory to hold attribute name and value, and
	 * copy it from the list */
	result             = axl_new (axlNodeAttr, 1);
	result->attribute  = axl_strdup (list->attribute);
	result->value      = axl_strdup (list->value);

	/* call to copy the rest of the list */
	result->next = __axl_node_copy_attr_list (list->next);

	/* return result created */
	return result;
}

/** 
 * @internal Deallocs the attribute list received.
 * 
 * @param list The attribute list to copy.
 * 
 * @return A newly allocated attribute list.
 */
void __axl_node_free_attr_list (axlNodeAttr * attr)
{
	axlNodeAttr * next;

	/* if the list isn't defined, return NULL */
	if (attr == NULL)
		return;

	/* copy all nodes */
	while (attr != NULL) {
		/* get the next attribute */
		next = attr->next;

		/* free attribute description */
		if (! attr->from_factory) {
			axl_free (attr->attribute);
			axl_free (attr->value);
			axl_free (attr);
		} /* end if */

		/* get the next */
		attr = next;
		
	} /* end while */

	/* return result created */
	return;
}

/** 
 * @internal Function that allows to now if both lists represents the
 * same set of attributes, even if they aren't ordered using the same.
 * 
 * @param attr The first attribute pointing to the rest of the
 * attribute list.
 *
 * @param attr2 The second attribute pointing to the rest of the
 * attribute list.
 * 
 * @return \ref true if both lists are equal, otherwise \ref false is
 * returned.
 */
bool __axl_node_attr_list_is_equal (axlNodeAttr * attr, axlNodeAttr * attr2)
{
	axlNodeAttr * attrAux;
	bool          found;

	/* for each attribute found in the attribute list, check it on
	 * the second list */
	while (attr != NULL) {
		
		attrAux = attr2;
		found   = false;
		while (attrAux != NULL) {
			
			/* check attribute */
			if (axl_cmp (attrAux->attribute, attr->attribute) &&
			    axl_cmp (attrAux->value, attr->value)) {
				/* attribute found, break the loop */
				found = true;
				break;
			}

			/* next attribute */
			attrAux = attrAux->next;
			
		} /* end while */
		
		/* check if the attribute was found */
		if (! found )
			return false;

		/* get the next */
		attr = attr->next;
		
	} /* end while */

	/* all attributes was found, including its values, so, the
	 * list is equal */
	return true;
}

typedef enum {
	/** 
	 * @internal Signal axl library that the node was allocated by
	 * a factory not by the system alloc.
	 */
	NODE_FROM_FACTORY      = 1,

	/** 
	 * @internal Signal axl library that the node node was
	 * allocated by a factory not by the system alloc.
	 */
	NODE_NAME_FROM_FACTORY = 1 << 2
} axlNodeConf;

struct _axlNode {
	/** 
	 * @internal
	 *
	 * Node name that is the value found at the very begining of
	 * the node definition <name.../>
	 */
	char          * name;

	/** 
	 * @internal Number of attributes stored. This value is used
	 * to now if the attributes are stored using a hash or a
	 * linked list. In the case it is equal or greater than 11, a
	 * hash is used to store attribute. Otherwise, a linked list
	 * (using axlAttrNode) is used.
	 */
	int             attr_num;

	/** 
	 * @internal
	 * @brief The attributes this node has.
	 */
	axlPointer    * attributes;

	/**
	 * @internal A reference to the frist child.
	 */
	axlItem      * first;

	/** 
	 * @internal A reference to the last child.
	 */
	axlItem      * last;

	/**
	 * @internal A hash used to store arbitrary data associated to
	 * the node.
	 */
	axlHash       * annotate_data; 

	/** 
	 * @internal Internal reference to the holder axlItem
	 * containing the axl node reference.
	 */
	axlItem       * holder;
	
	/** 
	 * @internal Value that contains flags for configuration.
	 */
	int             conf;
};

struct _axlItem {
	/** 
	 * @internal A reference to the type that is being hold by the
	 * encapsulation reference.
	 */
	AxlItemType     type;
	
	/** 
	 * @internal The reference to the pointer that is actually
	 * stored.
	 */
	axlPointer      data;

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
	axlItem       * next;

	/** 
	 * @internal Stores a reference to the previous node inside
	 * the same level.
	 */
	axlItem       * previous;

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
 * Internal function which allocates the enough memory to copy
 * received content changing all escape sequences.
 */
char * __axl_node_content_copy_and_escape (const char * content, 
					   int          content_size, 
					   int          additional_size)
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
 * @param name The name to be used for the node be created. The
 * function doesn't check if the paramater received is null.
 * 
 * @return A newly allocate \ref axlNode reference, that must be
 * deallocated by \ref axl_node_free.
 */
axlNode * axl_node_create (const char * name)
{

	axlNode * node;

	/* init the node */
	node        = axl_new (axlNode, 1);
	node->name  = axl_strdup (name);
	
	return node;
}

/** 
 * @brief This function allows to create a xml node from the provided
 * xml content.
 *
 * This function is useful if it is required to create a node with a
 * particular complex content, without going into the detail of
 * creating all childs, attributes and content.
 *
 * Here is an example:
 * \code
 * axlError * error = NULL;
 * axlNode  * node  = NULL;
 * 
 * // parse document 
 * root = axl_node_parse_strings (error, 
 *                                "<child>",
 *                                "  <widget class=\"GtkLabel\" id=\"label4\">",
 *                                "    <property name=\"visible\">True</property>",
 *                                "    <property name=\"label\" translatable=\"yes\">&lt;b&gt;1. Seleccione el sistema:&lt;/b&gt;</property>",
 *                                "    <property name=\"use_underline\">False</property>",
 *                                "    <property name=\"use_markup\">True</property>",
 *                                "    <property name=\"justify\">GTK_JUSTIFY_LEFT</property>",
 *                                "    <property name=\"wrap\">False</property>",
 *                                "    <property name=\"selectable\">False</property>",
 *                                "    <property name=\"xalign\">0</property>",
 *                                "    <property name=\"yalign\">0.5</property>",
 *                                "    <property name=\"xpad\">0</property>",
 *                                "    <property name=\"ypad\">0</property>",
 *                                "    <property name=\"ellipsize\">PANGO_ELLIPSIZE_NONE</property>",
 *                                "    <property name=\"width_chars\">-1</property>",
 *                                "    <property name=\"single_line_mode\">False</property>",
 *                                "    <property name=\"angle\">0</property>",
 *                                "  </widget>",
 *                                "  <packing>",
 *                                "     <property name=\"padding\">0</property>",
 *                                "     <property name=\"expand\">False</property>",
 *                                "     <property name=\"fill\">False</property>",
 *                                "  </packing>",
 *                                "</child>",
 *                                NULL);
 * if (root == NULL) {
 *	printf ("Error: unable to parse content, error: %s\n", axl_error_get (error));
 *              axl_error_free (error);
 *	return;
 * }
 *
 * // once finished, free the node 
 * axl_node_free (root);
 * \endcode
 * 
 * @param error Optional error reference to report parsing error problems that can be found.
 * 
 * The function receives a set of strings, separate by comma, ended by
 * NULL.
 * 
 * @return A newly allocated reference to the \ref axlNode or NULL if
 * it fails. In such case, the error variable is filled with the error
 * found.
 */
axlNode * axl_node_parse_strings      (axlError ** error, ...)
{
	axlDoc   * doc;
	axlNode  * root;
	va_list    args;
	char     * string     = NULL;
	char     * stream     = NULL;
	char     * stream_aux = NULL;
	
	/* check incoming data */
	axl_return_val_if_fail (error, NULL);
	
	/* open the stdargs */
	va_start (args, error);
	
	while ((string = va_arg (args, char *)) != NULL) {
		stream_aux = stream;
		stream = axl_stream_concat (stream, string);
		if (stream_aux != NULL) {
			axl_free (stream_aux);
			stream_aux = NULL;
		}
	}

	/* close the stdargs */
	va_end (args);

	/* check that we have received, at least, an string
	 * parseable */
	if (stream == NULL)
		return NULL;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "string to parse: %s", stream);

	/* parse the string */
	doc = axl_doc_parse (stream, -1, error);
	if (doc == NULL) {
		/* free document */
		axl_free (stream);
		return NULL;
	}

	/* free the stream */
	axl_free (stream);

	/* deattach the root node */
	root = axl_doc_get_root (doc);
	axl_node_deattach (root);

	/* do not free the document, rather, store it to be
	 * deallocated by the node just after it is deallocated. */
	axl_node_annotate_data_full (root, "__root_document", NULL, doc, (axlDestroyFunc) axl_doc_free);

	/* return the node created */
	return root;
}

/** 
 * @brief Allows to create a complete node configuring not only the
 * node but its content, using a printf-like format.
 *
 * This handy function, like \ref axl_node_parse_strings, allows to
 * create complex xml structures providing the inline content.
 *
 * Here is an example:
 * \code
 * axlNode * node = axl_node_parse (NULL, "<content attr='value' attr2='value'>This is content</content>");
 * \endcode
 *
 * The previous call will create a node called <b>content</b> with the
 * provided attributes and content, in one step.
 *
 * The node returned can be integrated into a xml document using usual
 * API, for example: \ref axl_node_set_child or \ref axl_doc_set_root.
 * 
 * @param error The optional error reference holding the returned
 * result.
 *
 * @param content The content to be used to create the node.
 * 
 * @return A reference to a newly allocated \ref axlNode or NULL if it
 * fails. The \ref axlError is filled with the error found if provided
 * by the caller.
 */
axlNode * axl_node_parse                    (axlError ** error, const char * content, ...)
{
	char    * _content;
	va_list   args;
	axlDoc  * doc;
	axlNode * root;

	/* open the stdargs */
	va_start (args, content);
	
	/* create the content */
	_content = axl_strdup_printfv (content, args);

	/* close the stdargs */
	va_end (args);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "parsing document:\n %s", _content);

	/* parse the string */
	doc = axl_doc_parse (_content, -1, error);
	if (doc == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "failed to parse document:\n %s", _content);

		/* free document */
		axl_free (_content);

		return NULL;
	}

	/* free the content */
	axl_free (_content);

	/* deattach the root node */
	root = axl_doc_get_root (doc);
	axl_node_deattach (root);

	/* do not free the document, rather, store it to be
	 * deallocated by the node just after it is deallocated. */
	axl_node_annotate_data_full (root, "__root_document", NULL, doc, (axlDestroyFunc) axl_doc_free);

	/* return the node created */
	return root;
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
 * dinamically allocated memory from the user space code. The function
 * doesn't check if the parameter received is null.
 * 
 * @return A newly created \ref axlNode reference that must be
 * deallocated by \ref axl_node_free.
 */
axlNode * axl_node_create_ref         (char * name)
{
	axlNode * node;

	/* init the node */
	node        = axl_new (axlNode, 1);
	node->name  = name;
	
	/* return a reference to a new axlNode */
	return node;
}

/** 
 * @brief Allows to configure the node name, using the value provided. 
 * 
 * The function will dealloc the previous name stored, if found.
 *
 * @param node The node to be configured with a new name.
 * 
 * @param name The new name to configure to the node. The value
 * provided will be allocated, to create a local copy. The function
 * won't check the name parameter received to be a non-null pointer.
 */
void      axl_node_set_name                 (axlNode * node, const char * name)
{
	/* check the name to be already configured and dealloc it */
	if (node->name != NULL && ! (node->conf & NODE_NAME_FROM_FACTORY))
		axl_free (node->name);

	/* alloc the new name */
	node->name = axl_strdup (name);
	
	return;
}

/** 
 * @brief Allows to configure the node name, using the value provided
 * as a reference allocated and to be owned by the node.
 * 
 * The function will dealloc the previous name stored, if found.
 *
 * @param node The node to be configured with a new name.
 * 
 * @param name The new name to configure to the node. The value
 * provided will be owned by the node. The function won't check the
 * name parameter received to be a non-null pointer.
 */
void      axl_node_set_name_ref             (axlNode * node, char * name)
{
	/* check the name to be already configured and dealloc it */
	if (node->name != NULL && ! (node->conf & NODE_NAME_FROM_FACTORY))
		axl_free (node->name);

	/* alloc the new name */
	node->name = name;
	
	return;
}

/** 
 * @internal Allows to configure the xml node, flaging the string
 * received to be owned by a factory. Do not use this API from your
 * application. This is only useful for Axl Library internals.
 * 
 * @param node The xml node that is going to be configured.
 *
 * @param name The xml node name to be configured. Previous
 * configuration won't be deallocated as it is supposed that this
 * function is only used from inside axl library.
 */
void axl_node_set_name_from_factory (axlNode * node, char * name)
{
	/* set node name */
	node->name = name;

	/* update configuration */
	node->conf |= NODE_NAME_FROM_FACTORY;
	
	return;
}

axlPointer __axl_node_copy_key (axlPointer key, axlDestroyFunc key_destroy, 
				axlPointer data, axlDestroyFunc data_destroy)
{
	/* copy the key */
	return axl_strdup (key);
}

axlPointer __axl_node_copy_value (axlPointer key, axlDestroyFunc key_destroy,
				  axlPointer data, axlDestroyFunc data_destroy)
{
	/* copy data */
	return axl_strdup (data);
}

/** 
 * @brief Allows to perform a copy operation for the provided node.
 *
 * The function can perform a simple copy, without attributes and
 * childs, if both attributes are false. If the copy attributes and
 * copy childs are activated, childs copied will have attributes
 * copied.
 * 
 * @param node The node source of information.
 *
 * @param copy_attributes Signals the function to copy node attributes
 * into the newly created node.
 *
 * @param copy_childs Signals the function to copy childs for the
 * source node.
 * 
 * @return A newly created node copy or null if it fails. The function
 * will fail if the node reference provided is null.
 */
axlNode * axl_node_copy                     (axlNode * node,
					     bool      copy_attributes,
					     bool      copy_childs)
{
	axlNode * result;
	axlItem * child;
	axlItem * copy;
	

	axl_return_val_if_fail (node, NULL);

	/* create the copy */
	result = axl_node_create (axl_node_get_name (node));

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node=<%s> (copy operation)",
		   result->name);

	/* check for attributes */
	if (node->attributes != NULL && copy_attributes) {
		
		/* copy attribute configuration and attributes */
		result->attr_num = node->attr_num;
		if (node->attr_num >= 11) {
			/* copy attribute list supposing it is a
			 * hash */
			result->attributes = (axlPointer) axl_hash_copy ((axlHash *) node->attributes, 
									 /* key copy function */
									 __axl_node_copy_key,
									 /* value copy function */
									 __axl_node_copy_value);
		} else {
			/* copy attribute list */
			result->attributes = (axlPointer) __axl_node_copy_attr_list ((axlNodeAttr *) node->attributes);
		}
	}

	/* check if child nodes must be also copied */
	if (copy_childs && (node->first != NULL)) {

		/* copy all content inside */
		child = node->first;
		while (child != NULL) {
			
			/* copy the child found */
			copy       = axl_item_copy (child, result);
			
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "  new item created (copy operation) with parent=<%s> and type=%d",
				   copy->parent->name,
				   axl_item_get_type (copy));

			/* set the content */
			axl_item_set_child_ref (result, copy);

			/* get the next element */
			child = child->next;

		} /* end while */
	} /* end if */
	
	
	/* return the node created */
	return result;
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
	return axl_item_get_doc (node->holder);
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
	axlItem * item;

	axl_return_if_fail (node);
	axl_return_if_fail (doc);

	/* get the item reference */
	item = node->holder;

	if (item == NULL) {

		/* create an empty reference */
		item         = axl_item_factory_get (axl_doc_get_item_factory (doc)); 
		item->type   = ITEM_NODE | ITEM_FROM_FACTORY;
		item->data   = node;
		node->holder = item;
		
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node received doesn't have a holder reference, creating ref=0x%x, node=0x%x, type=%d",
			   item, node, item->type);
	} /* end if */

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting doc to the item node");

	/* call to set item at the document */
	item->doc = doc;

	return;
}


/** 
 * @internal
 * 
 * Support function to install attribute information provided into the
 * \ref axlNode provided.
 */
void      __axl_node_set_attribute      (axlFactory * factory, 
					 axlNode    * node, 
					 char       * attribute, 
					 char       * value, 
					 bool         from_factory)
{
	axlNodeAttr * attr;
	axlNodeAttr * next;

	/* init attribute list */
	/* do not init attribute list twice */
	if (node->attributes == NULL) {
		/* configure default axl attribute list */
		node->attr_num   = 1;
		
		/* create the node */
		if (from_factory)
			attr = axl_factory_get (factory);
		else
			attr = axl_new (axlNodeAttr, 1);
		attr->from_factory = from_factory;
		attr->attribute    = attribute;
		attr->value        = value;
		
		/* store the node */
		node->attributes = (axlPointer) attr;

		return;
	}

	/* store the attribute using the general case */
	if (node->attr_num < 10) {

		/* create the node */
		if (from_factory)
			attr = axl_factory_get (factory);
		else
			attr = axl_new (axlNodeAttr, 1);
		attr->from_factory = from_factory;
		attr->attribute    = attribute;
		attr->value        = value;

		/* set the next to the new item to be the current first */
		attr->next       = (axlNodeAttr *) node->attributes;

		/* set the new first */
		node->attributes = (axlPointer) attr;
		
	} else if (node->attr_num >= 10) {
		
		/* check if we have to translate current attributes */
		if (node->attr_num == 10) {
			/* get a reference to current attribute list */
			attr = (axlNodeAttr *) node->attributes;

			/* create the hash */
			node->attributes = (axlPointer) axl_hash_new_full (axl_hash_string, axl_hash_equal_string, 1);
			
			while (attr != NULL) {
				/* add the attribute */	
				axl_hash_insert_full ((axlHash *) node->attributes, 
						      /* attribute name */
						      attr->attribute, attr->from_factory ?  NULL : axl_free, 
						      /* attribute value */
						      attr->value, attr->from_factory ? NULL : axl_free);
				/* free current node */
				next = attr->next;
				if (! attr->from_factory)
					axl_free (attr);

				/* get the next item to store */
				attr = next;
			} /* end while */

		} /* end if */

		/* add the attribute */	
		axl_hash_insert_full ((axlHash *) node->attributes, 
				      attribute, from_factory ? NULL : axl_free, 
				      value, from_factory ? NULL : axl_free);				
		
	} /* end if */

	/* update attribute count */
	node->attr_num++;

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
void      axl_node_set_attribute      (axlNode    * node, 
				       const char * attribute, 
				       const char * value)
{
	int    additional_size = 0;
	char * _attr;
	char * _value;

	axl_return_if_fail (node);
	axl_return_if_fail (attribute);
	axl_return_if_fail (value);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting attribute: %s='%s'", attribute, value);

	/* check attribute name */
	if (axl_node_has_invalid_chars (attribute, strlen (attribute),
					&additional_size)) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found attribute content with escapable, non-valid content");
		_attr = __axl_node_content_copy_and_escape (attribute, 
							    strlen (attribute),
							    additional_size);
	}else {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "'%s' is a valid string..", attribute);
		_attr = axl_strdup (attribute);
	}

	/* check attribute value */
	additional_size = 0;
	if (axl_node_has_invalid_chars (value, strlen (value),
					&additional_size)) {

		_value = __axl_node_content_copy_and_escape (value, 
							     strlen (value),
							     additional_size);
	}else {
		
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "'%s' is a valid string..", value);
		_value = axl_strdup (value);
	}

	/* insert the attribute */
	__axl_node_set_attribute (NULL, node, _attr, _value, false);
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

	axl_return_if_fail (node);
	axl_return_if_fail (attribute);
	axl_return_if_fail (value);

	/* insert the attribute */
	__axl_node_set_attribute (NULL, node, attribute, value, false);

	return;
}

/** 
 * @internal Function that allows configuring attributes to the
 * selected node, notifying that they come from a factory and
 * shouldn't be deallocated in the usual way. This function shouldn't
 * be used by API consumers. This is only useful for Axl Library
 * internals.
 * 
 * @param node The node to be configured with the attribute values.
 *
 * @param attribute The attribute to configure.
 * @param value The attribute value to configure.
 */
void      axl_node_set_attribute_from_factory  (axlFactory * factory, 
						axlNode    * node, 
						char       * attribute, 
						char       * value)
{
	/* insert the attribute */
	__axl_node_set_attribute (factory, node, attribute, value, true);

	return;
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
bool          axl_node_has_attribute      (axlNode * node, const char * attribute)
{
	axlNodeAttr * attr;

	axl_return_val_if_fail (node, false);
	axl_return_val_if_fail (attribute, false);

	/* check for empty hash */
	if (node->attributes == NULL)
		return false;

	if (node->attr_num <= 10) {
		/* linked configuration */
		attr = (axlNodeAttr *) node->attributes;
		while (attr != NULL) {
			/* check that the attribute is equal */
			if (axl_cmp (attr->attribute, attribute))
				return true;

			/* get the next attribute */
			attr = attr->next;
		} /* end while */

		/* attribute not found */
		return false;
	} /* end if */

	/* hashed configuration */
	return axl_hash_exists ((axlHash *) node->attributes, (axlPointer) attribute);
}

/** 
 * @brief Allows to remove the attribute provided, from the node
 * provided.
 * 
 * @param node The node that will be updated by removing the attribute
 * provided.
 *
 * @param attribute The attribute to locate and remove.
 */
void      axl_node_remove_attribute         (axlNode    * node,
					     const char * attribute)
{
	axlNodeAttr * attr;
	axlNodeAttr * previous;

	axl_return_if_fail (node);
	axl_return_if_fail (attribute);

	/* check for empty hash */
	if (node->attributes == NULL)
		return;

	if (node->attr_num <= 10) {
		/* linked configuration */
		attr     = (axlNodeAttr *) node->attributes;
		previous = NULL;
		while (attr != NULL) {
			/* check that the attribute is equal */
			if (axl_cmp (attr->attribute, attribute)) {
				/* attribute found */
				if (previous == NULL)
					node->attributes = (axlPointer) attr->next;
				else
					previous->next = attr->next;

				/* do not decrease attribute number
				 * since it is used to know the kind
				 * of store used. */

				/* now dealloc the attribute */
				if (! attr->from_factory) {
					axl_free (attr->attribute);
					axl_free (attr->value);
					axl_free (attr);
				} /* end if */
				
				return;
			}

			/* get the next attribute */
			previous = attr;
			attr     = attr->next;
		} /* end while */

		/* attribute not found */
		return;
	} /* end if */

	/* hashed configuration */
	axl_hash_remove ((axlHash *) node->attributes, (axlPointer) attribute);

	/* do not decrease attribute number
	 * since it is used to know the kind
	 * of store used. */
	
	return;
}

/** 
 * @brief Allows to get the number of attributes installed on the
 * provided node.
 * 
 * @param node The node that is requested to return the number of
 * attributes installed.
 * 
 * @return Attributes installed, or -1 if it fails.
 */
int       axl_node_num_attributes           (axlNode    * node)
{
	int           result = 0;
	axlNodeAttr * attr;

	axl_return_val_if_fail (node, -1);

	if (node->attr_num <= 10) {
		/* linked configuration */
		attr     = (axlNodeAttr *) node->attributes;
		while (attr != NULL) {
			/* update sum */
			result++;

			/* get the next attribute */
			attr     = attr->next;
		} /* end while */

		/* attribute not found */
		return result;
	} /* end if */

	/* hashed configuration */
	return axl_hash_items ((axlHash *) node->attributes);
}

/** 
 * @brief Allows to check if the provided has any attribute installed.
 * 
 * @param node The node to be checked for attributes.
 * 
 * @return \ref true if the node has attributes, otherwise \ref false
 * is returned. The function also returns \ref false if a null
 * reference for the node is provided.
 */
bool      axl_node_has_attributes (axlNode * node)
{
	axl_return_val_if_fail (node, false);

	/* return if the attributes reference is configured */
	return (node->attributes != NULL);
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
const char    * axl_node_get_attribute_value (axlNode * node, const char * attribute)
{
	axlNodeAttr * attr;

	/* check values received */
	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (attribute, NULL);

	/* check for empty hash */
	if (node->attributes == NULL)
		return NULL;

	if (node->attr_num <= 10) {
		/* linked configuration */
		attr = (axlNodeAttr *) node->attributes;
		while (attr != NULL) {
			/* check that the attribute is equal */
			if (axl_cmp (attr->attribute, attribute))
				return attr->value;

			/* get the next attribute */
			attr = attr->next;
		} /* end while */
		
		/* attribute not found */
		return NULL;
	} /* end if */

	/* return value stored for the provided key */
	return axl_hash_get ((axlHash *) node->attributes, (axlPointer) attribute);
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
char    * axl_node_get_attribute_value_copy (axlNode    * node, 
					     const char * attribute)
{
	const char * _value;

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
char    * axl_node_get_attribute_value_trans (axlNode    * node, 
					      const char * attribute)
{
	char * _value;
	int    size;

	/* get the attribute */
	_value = (char *) axl_node_get_attribute_value (node, attribute);
	axl_return_val_if_fail (_value, NULL);

	/* perform a local copy */
	_value = axl_strdup (_value);
	
	/* return a copy */
	size   = strlen (_value);
	return __axl_node_content_translate_defaults (_value, &size);
}

/** 
 * @brief Allows to get the value associated to the attributed
 * provided, inside the node selected, removing trailing and ending
 * white spaces (in the W3C sence: \\n, \\t, \\r, ' ').
 *
 * See \ref ATTR_VALUE_TRIMMED for a convenience macro.
 * 
 * @param node The node that is requested to return the associated 
 * value to the attributed.
 *
 * @param attribute The attribute that is being requested.
 * 
 * @return A reference to the attribute value or NULL if it fails. The
 * function doesn't return a copy, it returns a reference to the
 * internal value.
 */
const char    * axl_node_get_attribute_value_trimmed (axlNode    * node,
						      const char * attribute)
{
	char * _value;

	/* get the attribute */
	_value = (char *) axl_node_get_attribute_value (node, attribute);
	axl_return_val_if_fail (_value, NULL);

	/* trim the value */
	axl_stream_trim (_value);

	/* return value */
	return _value;
}

/** 
 * @brief Convenience function that allows to check if a particular
 * attribute with a particular value is found at the provided node.
 *
 * This function will check if the attribute provided in the node is
 * found and in such case, if the value that is contained inside the
 * node is actually the same as the one provide to this function.
 *
 * You can also use hits macro associated: \ref HAS_ATTR_VALUE.
 * 
 * @param node The node that will be checked.
 * @param attribute The attribute to be checked.
 * @param value The value to checked if the attribute is found.
 * 
 * @return \ref true if the node has the attribute with the provided
 * value.
 */
bool axl_node_has_attribute_value       (axlNode    * node, 
					 const char * attribute, 
					 const char * value)
{
	axl_return_val_if_fail (node, false);
	axl_return_val_if_fail (attribute, false);
	axl_return_val_if_fail (value, false);

	/* return if the attribute value found is the same */
	return axl_cmp (axl_node_get_attribute_value (node, attribute), value);
}

/**
 * @}
 */

/**
 * \defgroup axl_node_annotate Axl Node Annotate: An interface that allows associate arbitrary data to a particular node, indexed as a hash.
 */

/** 
 * \addtogroup axl_node_annotate
 * @{
 */

/** 
 * @internal function which checks and initializes the hash used for
 * annotated data.
 */
void __init_node_annotation (axlNode * node)
{
	if (node->annotate_data == NULL)
		node->annotate_data =  axl_hash_new (axl_hash_string, axl_hash_equal_string);
	return;
}

/** 
 * @brief Allows to store user defined data associated to the node
 * that is not visible from an XML perspective.
 *
 * This function allows to store data associated to the node that is
 * accesible in an inherired fashion that allows to store semantic
 * information while parsing xml documents.
 *
 * This function stores the given key using a hashing storage,
 * associating the data provided on the given node. You can also check
 * the \ref axl_node_annotate_data_full which performs the same task
 * but allowing to provide key and data destroy functions.
 *
 * Once data is stored, it could be inherited by child nodes because
 * the access to it is done using \ref axl_node_annotate_get. 
 *
 * Additionally, you can also perform annotation using native types:
 * int, string and double. Check the following functions to do so.
 *
 * - \ref axl_node_annotate_int
 * - \ref axl_node_annotate_double
 * - \ref axl_node_annotate_string
 *
 * @param node The node where the annotated data will be stored.
 *
 * @param key The key under which the annotated data will be stored
 * (and indexed).
 *
 * @param data The data to be stored associated to the key provided.
 */
void      axl_node_annotate_data                 (axlNode     * node, 
						 const char    * key,
						 axlPointer    data)
{
	axl_return_if_fail (node);

	/* check and init node annotation */
	__init_node_annotation (node);
	
	/* insert data */
	axl_hash_insert (node->annotate_data, (axlPointer) key, data);

	/* nothing more to do */
	return;
}

/** 
 * @brief Allows to store user defined data associated to the node
 * that is not visible from an XML perspective.
 *
 * See \ref axl_node_annotate_data for a long explanation. This
 * function performs the same task as \ref axl_node_annotate_data_full
 * but allowing to store a destroy key and a destroy data associated
 * to the annotated data to be stored.
 *
 * @param node The node where the annotated data will be stored.
 *
 * @param key The key under which the annotated data will be stored
 * (and indexed).
 *
 * @param key_destroy The destroy function to be called to deallocate
 * the key stored.
 *
 * @param data The data to be stored associated to the key provided.
 *
 * @param data_destroy The destroy function to be called to deallocate
 * the data provided.
 */
void      axl_node_annotate_data_full            (axlNode       * node,
						 const char    * key,
						 axlDestroyFunc  key_destroy,
						 axlPointer      data,
						 axlDestroyFunc  data_destroy)
{
	axl_return_if_fail (node);

	/* check and init node annotation */
	__init_node_annotation (node);

	/* insert data */
	axl_hash_insert_full (node->annotate_data, (axlPointer) key, key_destroy, data, data_destroy);

	/* nothing more to do */
	return;
}

/** 
 * @brief Allows to perform lookups for annotated data stored on the
 * provided node, configure how data is looked up if it fails to find
 * on the provided node.
 * 
 * @param node The node where the lookup will be performed.
 *
 * @param key The key to lookup in the \ref axlNode reference.
 *
 * @param lookup_in_parent Once the lookup fails, this variable allows
 * to signal the function to also lookup the value in the parent
 * nodes. This mechanism allows to store data on parent nodes that are
 * shared by child nodes.
 *
 * @return The data associated to the key according to the lookup
 * configuration (lookup_in_parent and lookup_in_doc).
 */
axlPointer axl_node_annotate_get                 (axlNode     * node,
						 const char  * key,
						 bool          lookup_in_parent)
{
	axlPointer   result = NULL;
	axlNode    * parent;
	
	/* check node received before nothing */
	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (key, NULL);
	
	/* lookup the data in the current node */
	if (node->annotate_data != NULL) {
		/* lookup the data */
		result = axl_hash_get (node->annotate_data, (axlPointer) key);

		/* check result returned */
		if (result != NULL)
			return result;
	} /* end if */

	/* check if we have to lookup the data in parent nodes */
	if (lookup_in_parent) {
		/* get the first parent reference */
		parent = axl_item_get_parent (node->holder);
		
		/* for each parent, try to lookup the data */
		while (parent != NULL) {
			/* lookup the data */
			if (parent->annotate_data)
				result = axl_hash_get (parent->annotate_data, (axlPointer) key);

			/* check result returned */
			if (result != NULL)
				return result;

			/* get the next parent */
			parent = axl_item_get_parent (parent->holder);
		}
	}

	/* no node was found */
	return result;
}

/** 
 * @internal definition to clasify node annotation.
 */
typedef enum {
	/** 
	 * @internal definition to clasify int elements.
	 */
	ANNOTATE_INT     = 0, 
	/** 
	 * @internal definition to clasify string elements.
	 */
	ANNOTATE_STRING  = 1, 
	/** 
	 * @internal definition to clasify int elements.
	 */
	ANNOTATE_DOUBLE  = 2
} AnnotateType;

typedef struct _AnnotateNodeData {
	/** 
	 * @internal type annotated.
	 */
	AnnotateType type;

	/** 
	 * @internal Value annotated: int, string or double.
	 */
	union {
		int    int_value;
		char * string_value;
		double double_value;
	} value;
} AnnotateNodeData;

void __axl_annotate_data_free (AnnotateNodeData * data)
{
	if (data == NULL)
		return;

	/* free the string */
	if (data->type == ANNOTATE_STRING)
		axl_free (data->value.string_value);

	/* free the data */
	axl_free (data);

	return;
}

/** 
 * @brief Allows to perform an annotation to the node at runtime,
 * storing a integer value.
 *
 * While using xml documents loaded into memory, each node could be
 * processed and annotated with particular information, indexed with a
 * key, that could be retrieved later for faster process. 
 * 
 * This data annotation doesn't perform any modification to the xml
 * document in any form. It is just a programming support that allows
 * developers to avoid created complex and independent structures to
 * the xml document while developing.
 * 
 * While using annotation support, you can use low level functions that
 * provide a simple way to store pointers associated to particular
 * nodes and retrieve them using:
 * 
 * - \ref axl_node_annotate_data_full
 * - \ref axl_node_annotate_data
 * - \ref axl_node_annotate_get
 *
 * However, additional functions are provided to store and retreive
 * easily integers, strings and double data annotated. See the
 * following:
 * 
 *  - \ref axl_node_annotate_int
 *  - \ref axl_node_annotate_string
 *  - \ref axl_node_annotate_double
 *
 * If you use this function to store an integer data you must use \ref
 * axl_node_annotate_get_int to retreive data stored. You can't use \ref axl_node_annotate_get.
 * 
 * @param node The node where the annotation will be aplied.
 *
 * @param key The key to index the data annotated to the node.
 *
 * @param int_value An integer value that will be annotated to the node
 * received under the key provided.
 */
void       axl_node_annotate_int                 (axlNode    * node,
						 const char * key,
						 int          int_value)
{
	AnnotateNodeData * data;
	
	/* check received values */
	axl_return_if_fail (node);
	axl_return_if_fail (key);

	/* allocate the node */
	data                  = axl_new (AnnotateNodeData, 1);
	data->type            = ANNOTATE_INT;
	data->value.int_value = int_value;

	/* annotate the value */
	axl_node_annotate_data_full (node, key, NULL, data, (axlDestroyFunc) __axl_annotate_data_free);
	
	return;
}

/** 
 * @brief Allows to perform an annotation to the node at runtime,
 * storing a string value.
 *
 * While using xml documents loaded into memory, each node could be
 * processed and annotated with particular information, indexed with a
 * key, that could be retrieved later for faster process. 
 * 
 * This data annotation doesn't perform any modification to the xml
 * document in any form. It is just a programming support that allows
 * developers to avoid created complex and independent structures to
 * the xml document while developing.
 * 
 * While using annotation support, you can use low level functions that
 * provide a simple way to store pointers associated to particular
 * nodes and retrieve them using:
 * 
 * - \ref axl_node_annotate_data_full
 * - \ref axl_node_annotate_data
 * - \ref axl_node_annotate_get
 *
 * However, additional functions are provided to store and retreive
 * easily integers, strings and double data annotated. See the
 * following:
 * 
 *  - \ref axl_node_annotate_int
 *  - \ref axl_node_annotate_string
 *  - \ref axl_node_annotate_double
 *
 * If you use this function to store a string data you must use \ref
 * axl_node_annotate_get_string to retreive data stored. You can't use \ref axl_node_annotate_get.
 *
 * @param node The node where the annotation will be aplied.
 *
 * @param key The key to index the data annotated to the node.
 *
 * @param string_value A string value that will be annotated to the
 * node received under the key provided. This value will be copied and
 * released once the node is deallocated.
 */
void       axl_node_annotate_string              (axlNode       * node,
						 const char    * key,
						 const char    * string_value)
{
	AnnotateNodeData * data;
	
	/* check received values */
	axl_return_if_fail (node);
	axl_return_if_fail (key);
	axl_return_if_fail (string_value);

	/* allocate the node */
	data                     = axl_new (AnnotateNodeData, 1);
	data->type               = ANNOTATE_STRING;
	data->value.string_value = axl_strdup (string_value);

	/* annotate the value */
	axl_node_annotate_data_full (node, key, NULL, data, (axlDestroyFunc) __axl_annotate_data_free);
	
	return;
}

/** 
 * @brief Allows to perform an annotation to the node at runtime,
 * storing a double value.
 *
 * While using xml documents loaded into memory, each node could be
 * processed and annotated with particular information, indexed with a
 * key, that could be retrieved later for faster process. 
 * 
 * This data annotation doesn't perform any modification to the xml
 * document in any form. It is just a programming support that allows
 * developers to avoid created complex and independent structures to
 * the xml document while developing.
 * 
 * While using annotation support, you can use low level functions that
 * provide a simple way to store pointers associated to particular
 * nodes and retrieve them using:
 * 
 * - \ref axl_node_annotate_data_full
 * - \ref axl_node_annotate_data
 * - \ref axl_node_annotate_get
 *
 * However, additional functions are provided to store and retreive
 * easily integers, strings and double data annotated. See the
 * following:
 * 
 *  - \ref axl_node_annotate_int
 *  - \ref axl_node_annotate_string
 *  - \ref axl_node_annotate_double
 *
 * If you use this function to store a double data you must use \ref
 * axl_node_annotate_get_double to retreive data stored. You can't use \ref axl_node_annotate_get.
 * 
 * @param node The node where the annotation will be aplied.
 *
 * @param key The key to index the data annotated to the node.
 *
 * @param double_value A string value that will be annotated to the node
 * received under the key provided.
 */
void       axl_node_annotate_double              (axlNode    * node,
						 const char * key,
						 double       double_value)
{
	AnnotateNodeData * data;
	
	/* check received values */
	axl_return_if_fail (node);
	axl_return_if_fail (key);

	/* allocate the node */
	data                     = axl_new (AnnotateNodeData, 1);
	data->type               = ANNOTATE_DOUBLE;
	data->value.double_value = double_value;

	/* annotate the value */
	axl_node_annotate_data_full (node, key, NULL, data, (axlDestroyFunc) __axl_annotate_data_free);
	
	return;
}

/** 
 * @brief Allows to retreive the annotated int value stored on the
 * particular node, under the provided key.
 * 
 * @param node The node that is required to return the annotated data.
 *
 * @param key The key to be used to lookup for the data annotated.
 *
 * @param lookup_in_parent Once the lookup fails, this variable allows
 * to signal the function to also lookup the value in the parent
 * nodes. This mechanism allows to store data on parent nodes that are
 * shared by child nodes.
 *
 * NOTE: To make this function work properly you must store double
 * values using \ref axl_node_annotate_int. Storing values using
 * other functions will lead to unpredictable results.
 * 
 * @return The integer value stored using \ref
 * axl_node_annotate_int. If the key provided doesn't exists, the
 * function returns 0.
 */
int        axl_node_annotate_get_int             (axlNode    * node,
						 const char * key,
						 bool         lookup_in_parent)
{
	AnnotateNodeData * data;
	
	/* check received values */
	axl_return_val_if_fail (node, 0);
	axl_return_val_if_fail (key, 0);

	/* get the annotated data */
	data = axl_node_annotate_get (node, key, lookup_in_parent);

	/* check for null value. */
	if (data == NULL)
		return 0;

	if (data->type != ANNOTATE_INT) {
		/* drop a log */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "accesing to a annotation data that isn't flaged as integer");
		return 0;
	}
	
	/* return the integer value inside */
	return data->value.int_value;
}

/** 
 * @brief Allows to retreive the annotated string value stored on the
 * particular node, under the provided key.
 * 
 * @param node The node that is required to return the annotated data.
 *
 * @param key The key to be used to lookup for the data annotated.
 *
 * @param lookup_in_parent Once the lookup fails, this variable allows
 * to signal the function to also lookup the value in the parent
 * nodes. This mechanism allows to store data on parent nodes that are
 * shared by child nodes.
 *
 * NOTE: To make this function work properly you must store double
 * values using \ref axl_node_annotate_string. Storing values using
 * other functions will lead to unpredictable results.
 * 
 * @return The string value stored using \ref
 * axl_node_annotate_string. If the key provided doesn't exists, the
 * function returns NULL.
 */
char *     axl_node_annotate_get_string          (axlNode    * node,
						 const char * key,
						 bool         lookup_in_parent)
{
	AnnotateNodeData * data;
	
	/* check received values */
	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (key, NULL);

	/* get the annotated data */
	data = axl_node_annotate_get (node, key, lookup_in_parent);

	/* check for null value. */
	if (data == NULL)
		return NULL;

	if (data->type != ANNOTATE_STRING) {
		/* drop a log */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "accesing to a annotation data that isn't flaged as string");
		return NULL;
	}
	
	/* return the string value inside */
	return data->value.string_value;
}

/** 
 * @brief Allows to retreive the annotated double value stored on the
 * particular node, under the provided key.
 * 
 * @param node The node that is required to return the annotated data.
 *
 * @param key The key to be used to lookup for the data annotated.
 *
 * @param lookup_in_parent Once the lookup fails, this variable allows
 * to signal the function to also lookup the value in the parent
 * nodes. This mechanism allows to store data on parent nodes that are
 * shared by child nodes.
 *
 * NOTE: To make this function work properly you must store double
 * values using \ref axl_node_annotate_double. Storing values using
 * other functions will lead to unpredictable results.
 * 
 * @return The double value stored using \ref
 * axl_node_annotate_double. If the key provided doesn't exists, the
 * function returns 0.0.
 */
double     axl_node_annotate_get_double          (axlNode    * node,
						 const char * key,
						 bool         lookup_in_parent)
{
	AnnotateNodeData * data;
	
	/* check received values */
	axl_return_val_if_fail (node, 0.0);
	axl_return_val_if_fail (key, 0.0);

	/* get the annotated data */
	data = axl_node_annotate_get (node, key, lookup_in_parent);

	/* check for null value. */
	if (data == NULL)
		return 0.0;

	if (data->type != ANNOTATE_DOUBLE) {
		/* drop a log */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "accesing to a annotation data that isn't flaged as double");
		return 0.0;
	}
	
	/* return the double value inside */
	return data->value.double_value;
}

/**
 * @}
 */

/** 
 * \addtogroup axl_node_module
 * @{
 */

/** 
 * @brief Allows to configure the given node to be empty.
 *
 * A \ref axlNode is empty when it is known that the node doesn't have
 * any content inside it as a child element. If the node has content,
 * and the value provided to this function is \ref true, the function
 * will deallocate the content inside.
 *
 * You can use this function to clear all the node content (\ref
 * ITEM_CONTENT and \ref ITEM_CDATA) found inside the node, as follows:
 * \code
 * // clear all content inside 
 * axl_node_set_is_empty (node, true);
 * \endcode
 *
 * @param node The node to configure as empty.
 *
 * @param empty The value for emptyness to be used. false will
 * mean that the node is not empty.
 */
void      axl_node_set_is_empty (axlNode * node, bool     empty)
{
	axlItem * child;
	axlItem * aux;
	int       removed = 0;
	int       count   = 0;

	axl_return_if_fail (node);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received set to empty node=<%s> is_empty=\"%s\"",
		   node->name, empty ? "true" : "false");

	/* do no perform any operation if false is received */
	if (! empty)
		return;

	/* get the first child and eliminate all content */
	child = node->first;
	while (child != NULL) {

		/* count each item found */
		count++;

		/* get a reference to the next */
		aux = child->next;

		/* check item node that represents content, and
		 * therefore, an emptyless state */
		if (axl_item_get_type (child) == ITEM_CONTENT || 
		    axl_item_get_type (child) == ITEM_CDATA) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found item content and item CDATA..");

			/* remove the node */
			axl_item_remove (child, true);

			/* count each item removed */
			removed++;
			
		} /* end if */

		/* update to the next */
		child = aux;
		
	} /* end while */

	/* now check if items removed are equal to items counted */
	if (removed == count) {
		/* then clear node references */
		node->first = NULL;
		node->last  = NULL;
	}
	
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
const char    * axl_node_get_name           (axlNode * node)
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

	/* if holder is NULL, no parent is posible */
	if (node->holder == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received a node without holder (an axlItem reference), unable to get the parent reference: %s", 
			   node->name);
		return NULL;
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the node has a holder reference, returning current parent");
	
	/* return the parent */
	return node->holder->parent;
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
 * There are an alternative API that allows to get the next node,
 * following to the node selected, providing the name to match. See
 * \ref axl_node_get_next_called.
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
	axlItem * item;

	axl_return_val_if_fail (node, NULL);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting next node for=<%s>", axl_node_get_name (node));

	if (node->holder == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received a node without item holder (maybe it wasn't inserted into a xml document)");
		return NULL;
	}
	
	/* get the next axlNode situated at the same level of the
	 * provided axlNode reference */
	item = axl_item_get_next (node->holder);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "%s", (item != NULL) ? "next item is defined, check if it is a node" : "next item isn't defined");
	
	/* while the item is not null and different from item node,
	 * get the next */
	while (item != NULL) {

		/* get the item found */
		if (axl_item_get_type (item) == ITEM_NODE)
			return item->data;

		/* get next item */
		item = item->next;
		
	} /* end while */

	/* or null if no reference is defined */
	return NULL;
}

/** 
 * @brief Allows to get the next node, following to the node provided,
 * matching the given name.
 *
 * <i><b>NOTE:</b> This function isn't XML Namespace aware. You must use \ref axl_ns_node_get_next_called instead. See \ref axl_ns_doc_validate. </i>
 * 
 * @param node The node that is requested to return its next sibling node.
 * @param name The name to match for the next node.
 * 
 * @return A reference to the next node or NULL if it fails. The
 * returned reference mustn't be deallocated.
 */
axlNode * axl_node_get_next_called    (axlNode * node, 
				       char    * name)
{
	axlNode * next;
	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (name, NULL);

	/* while there is a next node */
	next = axl_node_get_next (node);
	while (next != NULL) {
		/* check the node */
		if (NODE_CMP_NAME (next, name))
			return next;

		/* update to the next */
		next = axl_node_get_next (next);
	} /* end while */

	/* no node was found */
	return NULL;
}

/** 
 * @brief Allows to get the previous reference from the reference node
 * provided. 
 *
 * See \ref axl_node_get_next. Previous reference is the considered
 * the previous node to the referenced provided that shares the same
 * parent and it is situated in the same level.
 * 
 * @param node The node where the previous reference to the previous node will be returned.
 * 
 * @return The previous node reference or NULL if the node doesn't
 * have previous reference or NULL if the function fails (the function
 * only fails if the node reference provided is null).
 */
axlNode * axl_node_get_previous (axlNode * node)
{
	axlItem * item;

	axl_return_val_if_fail (node, NULL);

	/* get the previous axlNode situated at the same level of the
	 * provided axlNode reference */
	item = axl_item_get_previous (node->holder);

	/* while the item is not null and different from item node,
	 * get the previous */
	while ((item != NULL) && axl_item_get_type (item) !=ITEM_NODE)
		item = axl_item_get_previous (item);

	/* return the previous reference */
	if (item != NULL)
		return item->data;

	/* or null if no reference is defined */
	return NULL;
}

/** 
 * @brief Allows to get the previous node, preceding to the node
 * provided, matching the given name.
 *
 * <i><b>NOTE:</b> This function isn't XML Namespace aware. You must use \ref axl_ns_node_get_previous_called instead. See \ref axl_ns_doc_validate. </i>
 * 
 * @param node The node that is requested to return its previous sibling node.
 * @param name The name to match for the previous node.
 * 
 * @return A reference to the previous node or NULL if it fails. The
 * returned reference mustn't be deallocated.
 */
axlNode * axl_node_get_previous_called    (axlNode * node, 
					   char    * name)
{
	axlNode * previous;

	axl_return_val_if_fail (node, NULL);
	axl_return_val_if_fail (name, NULL);

	/* while there is a previous node */
	previous = axl_node_get_previous (node);
	while (previous != NULL) {
		/* check the node */
		if (NODE_CMP_NAME (previous, name))
			return previous;

		/* update to the next */
		previous = axl_node_get_previous (previous);
	} /* end while */

	/* no node was found */
	return NULL;

} 


/** 
 * @brief Allows to get the first child that holds the node.
 *
 * This function is considered inside the CHILDREN API, which is the
 * set of functions that are used to handle XML documents that have
 * node that contains more nodes or content, but not mixed.
 *
 * This function allows to get the first child found that is an \ref
 * axlNode. In the case your application is handling a document that
 * don't mix nodes and content at the same level inside another xml
 * nodes, you'll get the expected results.
 *
 * But, calling to this function on a node that contains content mixed
 * with another nodes, you will skip all items stored before the first
 * xml node found as child. 
 *
 * Let's see some examples to clarify this. Provided the following xml
 * document:
 *
 * \code
 * <document>
 *   <child>
 *     Content
 *   </child>
 * </document>
 * \endcode
 *
 * If you want to get a reference to the <b>child</b> node you can do
 * the following:
 * 
 * \code
 * // supposing the document is already loaded in "doc"
 * axlNode * node = axl_doc_get_root (doc);
 *
 * // get the first child 
 * node = axl_node_get_first_child (node);
 *
 * // now you have in "node" a reference to the child node.
 * \endcode
 *
 * However, in the case the previous content mix node with content as
 * follows:
 *
 * \code
 * <document>
 *   Some content previous to the first child.
 *   <child>
 *     Content
 *   </child>
 * </document>
 * \endcode
 *
 * Using this function will make you to skip the first content, that
 * is, <i>"Some content previous to the first child"</i>, which is found
 * before the &lt;child> node. In the case you want to have full
 * access to all items stored as child for a particular node, check
 * \ref axl_item_get_first_child.
 * 
 * @param node The node that is requested to return its first child.
 * 
 * @return The first child node or NULL if it has no child node.
 */
axlNode * axl_node_get_first_child    (axlNode * node)
{
	axlItem * item;

	/* check values */
	axl_return_val_if_fail (node, NULL);

	/* get first item child and lookup for the first child that is
	 * a node */
	item = node->first;
	while (item != NULL) {
		/* check the item type */
		if (axl_item_get_type (item) == ITEM_NODE)
			return item->data;

		/* get the next */
		item = item->next;
	}

	/* return NULL: no child axlNode was found */
	return NULL;
}

/** 
 * @brief Allows to get the last child that holds the node.
 *
 * See also \ref axl_node_get_first_child and \ref axl_item_get_last_child.
 * 
 * @param node The node that is requested to return its last child.
 * 
 * @return The last child node or NULL if it has no child node.
 */
axlNode * axl_node_get_last_child     (axlNode * node)
{
	axlItem * item;

	/* check values */
	axl_return_val_if_fail (node, NULL);

	/* get first item child and lookup for the first child that is
	 * a node */
	item = node->last;
	while (item != NULL) {
		/* check the item type */
		if (axl_item_get_type (item) == ITEM_NODE)
			return item->data;

		/* get the next */
		item = item->previous;
	}

	/* return NULL: no child axlNode was found */
	return NULL;
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
	axlItem * child;
	axl_return_val_if_fail (node, false);

	/* get the first child */
	child = node->first;
	while (child != NULL) {
		
		/* check item node that represents content, and
		 * therefore, an emptyless state */
		if (axl_item_get_type (child) == ITEM_CONTENT || 
		    axl_item_get_type (child) == ITEM_CDATA) {

			/* the node has content */
			return false;
			
		} /* end if */

		/* go to the next */
		child = child->next;

	} /* end while */

	return true;
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
 * Here is a summary of functions available to get the content of a
 * node:
 * 
 * - \ref axl_node_get_content_copy (the same like this function but
 * producing an independent copy)
 *
 * - \ref axl_node_get_content_trans (the same like this function but
 * translating all entity references and producing an indenpendent
 * copy).
 *
 * - \ref axl_node_get_content_trim (the same like this function but
 * removing initial and trailing white spaces in the W3C: spaces,
 * tabulars, carry returns and line feed values).
 * 
 * @param node The \ref axlDoc node where the content will be retrieved.
 *
 * @param content_size Optional pointer to an integer variable where
 * the content size will be reported. If the variable is not set, the
 * function will not report the content size. If this value is
 * configured, it will contain the content size starting from 0 up to
 * the content size.
 * 
 * @return Current xml node content. You must not deallocate reference
 * returned. If you want a permanet copy you should use \ref
 * axl_node_get_content_copy. Keep in mind that the function will always
 * return an string reference. In the case the node has no content, an
 * empty string will be returned, not NULL.
 */
const char    * axl_node_get_content     (axlNode * node, int * content_size)
{
	axlNodeContent * content;
	axlItem        * child;

	axl_return_val_if_fail (node, NULL);

	/* get the first child */
	child = node->first;
	while (child != NULL) {
		
		/* check item node that represents content, and
		 * therefore, an emptyless state */
		if (axl_item_get_type (child) == ITEM_CONTENT || 
		    axl_item_get_type (child) == ITEM_CDATA) {
			/* cast a reference */
			content = child->data;

			/* return the content */
			if (content_size != NULL)
				*content_size = content->content_size;

			/* return a local reference */
			return content->content;
		} /* end if */

		/* get the next item */
		child = child->next;

	} /* end while */

	/* set content size to zero */
	if (content_size != NULL)
		*content_size = 0;
	return "";
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
 * <tr><td>'</td><td>&amp;apos;</td></tr>
 * <tr><td><</td><td>&amp;lt;</td></tr>
 * <tr><td>></td><td>&amp;gt;</td></tr>
 * <tr><td>&</td><td>&amp;amp;</td></tr>
 * <tr><td>"</td><td>&amp;quot;</td></tr>
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
void      axl_node_set_content        (axlNode    * node, 
				       const char * content, 
				       int          content_size)
{
	axlNodeContent * itemContent;
	int              additional_size = 0;

	axl_return_if_fail (node);
	axl_return_if_fail (content);

	/* get current content in the case a -1 is provided */
	if (content_size == -1)
		content_size = strlen (content);

	/* allocate the content */
	itemContent  = axl_new (axlNodeContent, 1);

	/* check if the string received have escapable characters */
	if (axl_node_has_invalid_chars (content, content_size, 
					&additional_size)) {
		/* copy content */
		itemContent->content        = __axl_node_content_copy_and_escape (content, 
										  content_size, 
										  additional_size);
		/* set node content size */
		itemContent->content_size   = content_size + additional_size;
	}else {
		/* set current content */
		itemContent->content_size   = content_size;
		itemContent->content        = axl_new (char, content_size + 1);

		/* copy content */
		memcpy (itemContent->content, content, itemContent->content_size);
	}

	/* add it to the current node */
	axl_item_set_child (node, ITEM_CONTENT, itemContent);
	

	/* job done */
	return;
}

/** 
 * @internal Common implementation for axl_node_set_content_ref and
 * axl_node_set_content_from_factory.
 */
void __axl_node_set_content_common_ref (axlFactory * factory, 
					axlNode    * node, 
					char       * content, 
					int          content_size, 
					bool         from_factory,
					bool         cdata)
{
	
	axlNodeContent * itemContent;

	axl_return_if_fail (node);
	axl_return_if_fail (content);

	/* get current content in the case a -1 is provided */
	if (content_size == -1)
		content_size = strlen (content);

	/* create a content checking it it comes from the
	 * factory. Because the string received could come from the
	 * factory, already allocated, do not call to allocate */
	if (from_factory && factory)
		itemContent = axl_factory_get (factory);
	else 
		itemContent = axl_new (axlNodeContent, 1);
		
	/* configure content size */
	itemContent->content_size = content_size;

	/* set current content */
	itemContent->content  = content;

	if (from_factory) {
		if (cdata) {
			/* store it */
			axl_item_set_child (node, ITEM_CDATA | ITEM_CONTENT_FROM_FACTORY, itemContent);
		} else {
			/* store it */
			axl_item_set_child (node, ITEM_CONTENT | ITEM_CONTENT_FROM_FACTORY, itemContent);
		}
	} else {
		/* store it */
		if (cdata) {
			/* store as cdata */
			axl_item_set_child (node, ITEM_CDATA, itemContent);
		} else {
			/* store as parsed xml content */
			axl_item_set_child (node, ITEM_CONTENT, itemContent);
		}
	} /* end if */

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting xml node (name: %s) content (size: %d) %s",
		   node->name, itemContent->content_size, itemContent->content);

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
 * If the node have content already configured, it is deallocated,
 * configuring new content received.
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

	/* call to set content without signaling that the content
	 * wasn't allocated by a factory. */
	__axl_node_set_content_common_ref (NULL, node, content, content_size, false, false);
	
	/* job done */
	return;	
}

/** 
 * @internal Internal API used by the axl doc module to signal that
 * the content was allocated though the string factory and shouldn't
 * be deallocated.
 */
void      axl_node_set_content_from_factory (axlFactory * factory,
					     axlNode    * node,
					     char       * content,
					     int          content_size)
{
	/* call to set content without signaling that the content was
	 * allocated by a factory. */
	__axl_node_set_content_common_ref (factory, node, content, content_size, true, false);
	
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
	axl_return_if_fail (node);
	axl_return_if_fail (content);

	/* reconfigure content_size if found -1 value */
	if (content_size == -1)
		content_size = strlen (content);

	/* call to set node content */	
	content = axl_strdup (content);
	__axl_node_set_content_common_ref (NULL, node, content, content_size, false, true);

	return;
}

/** 
 * @internal Internal API used by the axl doc module to signal that
 * the content was allocated though the string factory and shouldn't
 * be deallocated, and the content was found inside cdata
 * declarations.
 */
void      axl_node_set_cdata_content_from_factory (axlFactory * factory,
						   axlNode    * node,
						   char       * content,
						   int          content_size)
{
	/* call to set content without signaling that the content was
	 * allocated by a factory. */
	__axl_node_set_content_common_ref (factory, node, content, content_size, true, true);
	
	/* job done */
	return;	
}

/** 
 * @brief Allows to configure a new comment (&lt;!-- xml comment -->) that will be
 * stored as a child for the node provided.
 * 
 * The comment will be placed at the end of the current child
 * list. So, if you want to place a xml comment before a xml node,
 * call first to this function and the to \ref axl_node_set_child.
 *
 * @param node The node that will contain the comment.
 *
 * @param comment The comment to be stored. The function will perform
 * a copy from it.
 *
 * @param comment_size The comment size or -1 to make the function to
 * calculate it.
 */
void      axl_node_set_comment        (axlNode * node,
				       char * comment,
				       int    comment_size)
{
	axlNodeContent * content;

	axl_return_if_fail (node);
	axl_return_if_fail (comment);

	/* check current coment size */
	if (comment_size == -1)
		comment_size = strlen (comment);

	/* create the comment */
	content               = axl_new (axlNodeContent, 1);
	content->content      = axl_new (char, comment_size + 1);
	content->content_size = comment_size;

	/* copy the content */
	memcpy (content->content, comment, comment_size);

	/* now store it on the node */
	axl_item_set_child (node, ITEM_COMMENT, content);

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
 * where the content size will be returned. The function will return
 * the content size (if the variable is defined) ranging from 0 up to
 * the content size.
 * 
 * @return A newly allocated string representing the node content. 
 */
char    * axl_node_get_content_copy (axlNode * node, int * content_size)
{
	int          _content_size;
	char       * result;
	const char * content;

	/* get the content and check if it is defined */
	if (content_size)
		content = axl_node_get_content (node, content_size);
	else
		content = axl_node_get_content (node, &_content_size);

	/* check result */
	if (content == NULL || strlen (content) == 0) {
		return axl_strdup ("");
	}

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

	axlNodeContent * content;
	axlItem        * child;

	axl_return_val_if_fail (node, NULL);

	/* get the first child */
	child = node->first;
	while (child != NULL) {
		
		/* check item node that represents content, and
		 * therefore, an emptyless state */
		if (axl_item_get_type (child) == ITEM_CONTENT || 
		    axl_item_get_type (child) == ITEM_CDATA) {
			/* cast a reference */
			content = child->data;

			/* trim the content */
			axl_stream_trim_with_size (content->content, &trimmed);

			/* updates current internal content size */
			content->content_size -= trimmed;

			/* return the content */
			if (content_size != NULL)
				*content_size = content->content_size;

			/* return a local reference */
			return content->content;
		} /* end if */
		
		/* get the next item */
		child = child->next;

	} /* end while */

	/* set content size to zero */
	if (content_size != NULL)
		*content_size = 0;
	return "";
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
        if (result == NULL || strlen (result) == 0) {
		/* do not perform a copy here, it is already done by
		 * get_content_copy, even in error */
                return result;
	}

	/* translate all references that performs the entities to the
	 * replacement text. */
	if (content_size)
		return __axl_node_content_translate_defaults (result, content_size);
	return __axl_node_content_translate_defaults (result, &_content_size);
}


/** 
 * @brief Allows to configure a child node to the given parent.
 *
 * This is a fundamental function while building xml document inside
 * memory. The way the xml nodes are linked to conform the xml
 * document structure relay on this function. 
 * 
 * The idea is that every call to this function makes the <b>child
 * </b> xml node to be placed at the end of the current item child
 * set, that represents current child list for the provided
 * <b>parent</b>.
 *
 * One important question while using this function is that you must
 * not reuse the same xml node reference, adding it several time to
 * the same parent (or different parents). You must create a new xml
 * node reference (axl_node_create) for every call you do to this
 * function.
 * 
 * So, to build the following structure:
 * \code
 * <document>
 *   <child1 />
 *   <child2 />
 * </document>
 * \endcode
 * 
 * You must perform the following operations:
 * \code
 * axlNode * parent;
 * axlNode * child;
 * 
 * // create the parent node 
 * parent = axl_node_create ("document");
 *
 * // create the first child 
 * child = axl_node_create ("child1");
 *
 * // set it to the parent 
 * axl_node_set_child (parent, child);
 *
 * // create the second child 
 * child = axl_node_create ("child2");
 *
 * // set it to the parent 
 * axl_node_set_child (parent, child);
 *
 * \endcode
 *
 * See also \ref axl_node_set_child_after which could help you adding
 * new nodes not using a parent node as a reference but a brother
 * node.
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

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "received parent=0x%x and child=0x%x",  parent, child);

	/* set a xml node child */
	axl_item_set_child (parent, ITEM_NODE, child);

        return;
}

/** 
 * @brief Sets a new child after the node provided as a reference.
 *
 * This function is useful to allow configuring new childs placed
 * after some particular node. The child configured will be placed
 * after the reference and child of the reference's parent node.
 * 
 * @param reference The xml node acting as a reference.
 * @param child The new xml node child to configure.
 */
void      axl_node_set_child_after    (axlNode * reference,
				       axlNode * child)
{
	/* call to the item implementation */
	axl_item_set_after (reference->holder, ITEM_NODE, child);

	return;
}


/** 
 * @brief Allows to replace a selected node, with the new reference
 * inside its context (updating all references: next, previous and
 * parent).
 *
 * If the node replace is inserted in a document, the replace also
 * works. In fact, this function is designed to replace a node already
 * inserted in an xml document (\ref axlDoc). If the node being
 * replaced is the root one, this function will configured the new
 * root node.
 *
 * Previous \ref axlNode will be unreference according to dealloc
 * value. This function will replace the node provided by the second
 * reference (no matter if the node is inside a document or not).
 * 
 * @param node The node to be replaced by the following reference.
 *
 * @param new_node The node that will replace the previous value.
 *
 * @param dealloc Signal if the previous node must be deallocated.
 */
void      axl_node_replace             (axlNode * node, 
					axlNode * new_node,
					bool      dealloc)
{
	axlItem * p_item;

	axl_return_if_fail (node);
	axl_return_if_fail (new_node);

	if (axl_item_get_parent (node->holder) == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "replacing the root node=<%s> with <%s>..", 
			   axl_node_get_name (node), axl_node_get_name (new_node));
		/* seems to be a root document */
		if (axl_item_get_doc (node->holder) != NULL) {
			axl_doc_set_root (axl_item_get_doc (node->holder), new_node);
		}
	} else {

		/* check for holder reference */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "replacing a non-root node=<%s> with <%s>..",
			   axl_node_get_name (node), axl_node_get_name (new_node));
		if (node->holder != NULL) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "axl item holder is defined for node=<%s>",
				   axl_node_get_name (node));
			/* get a reference to the holder item */
			p_item       = node->holder;

			/* configure the new node */
			p_item->data = new_node;

			/* nullify the holder reference, and configure
			 * the holder reference in the node */
			node->holder     = NULL;
			new_node->holder = p_item;
			
		} /* end if */
	}

	/* dealloc node if configured so */
	if (dealloc) {
		/* free the node */
		axl_node_free (node);
	}
	
	return;
}

/** 
 * @brief Allows to the remove the selected reference from the
 * document containing it.
 *
 * The function remove the selected reference from the document. If
 * the node asked to be removed is the root one, the node won't be
 * removed because the \ref axl_doc_set_root doesn't accept to remove
 * the root node.  
 *
 * All childs hold by the node removed from the document whill also be
 * removed if dealloc is selected.
 * 
 * @param node The node to remove.
 *
 * @param dealloc \ref true to not only unlink relations but also
 * remove the node.
 */
void      axl_node_remove             (axlNode * node,
				       bool      dealloc)
{
	axlItem * item;
	axl_return_if_fail (node);

	/* get a reference to the item element */
	item = node->holder;

	/* check if the node is the root node of its document */
	if (item != NULL && item->doc != NULL) {
		if (axl_doc_get_root (item->doc) == node) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "attempting to dettach the root node from the document, nullify");
			axl_doc_set_root (item->doc, NULL);
		} /* end if */
	} /* end if */
	
	if (axl_item_get_parent (item) != NULL) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "doing reference relocation (remove operation) for=<%s>",
			   node->name);
		
		/* make previous node to point to the new node */
		if (item->previous != NULL) {
			item->previous->next = item->next;
		}
		
		/* make next node to point to the new node */
		if (item->next != NULL) {
			item->next->previous = item->previous;
		}

		/* now, update the parent reference */
		if (item->previous == NULL) {
			/* seems the node is the first child of the parent,
			 * update the reference */
			item->parent->first = item->next;
		}
		
		if (item->next == NULL) {
			/* seems the node is the last child of the parent,
			 * update the reference */
			item->parent->last = item->previous;
		}

		if (item != NULL) {
			/* disconnect the item */
			item->previous = NULL;
			item->next     = NULL;
		} /* end if */

	} /* end if */

	/* dealloc node if configured so */
	if (dealloc) {
		/* free the node */
		axl_node_free (node);
	}
	
	return;
}

/** 
 * @brief Supposing the node is attached to a xml document (\ref
 * axlDoc), this function allows to deattach the node from the
 * document that is holding it.
 * 
 * This function is useful while requiring to reallocate nodes from
 * parent to parent, making the parent node that is holding it to lost
 * references to the node, decreasing all internal counts to the node,
 * etc.
 *
 * If the node isn't attached to any document, the function does
 * nothing.
 * 
 * @param node The node to deattach.
 */
void axl_node_deattach (axlNode * node)
{
	axl_return_if_fail (node);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "deattaching node..");

	/* call to remove */
	axl_node_remove (node, false);

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
	/* do nothing */
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
	axlItem * item;

	axl_return_val_if_fail (node, false);

	item = node->first;
	while (item != NULL) {
		/* check item type */
		if (axl_item_get_type (item) == ITEM_NODE)
			return true;

		/* go to the next */
		item = item->next;

	} /* end while */

	/* return false because no item was found with ITEM_NODE
	 * type */
	return false;
}

/** 
 * @brief Allows to get a particular child node for the given node (\ref axlNode).
 *
 * <i><b>NOTE:</b> This function isn't XML Namespace aware. You must use \ref axl_ns_node_get_child_called instead. See \ref axl_ns_doc_validate. </i>
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
	axlItem * item;
	
	axl_return_val_if_fail (parent, NULL);
	axl_return_val_if_fail (name, NULL);

	/* if the child list is not defined, assume there is no node
	 * called the name requested */
	if (parent->first == NULL)
		return NULL;
	
	/* if no childs, no result */
	item = parent->first;
	while (item != NULL) {
		/* check item type */
		if (axl_item_get_type (item) == ITEM_NODE) {
			/* get a reference to the node */
			node = item->data;

			/* compare for find the child */ 
			if (NODE_CMP_NAME (node, name))
				return node;
			
		} /* end if */

		/* next child */
		item = axl_item_get_next (item);
	}

	/* no child was found */
	return NULL;
}

/** 
 * @brief Allows to find the first child called as provided inside the
 * childs (including its descendants) hold by the parent provided.
 *
 * This function is similar to \ref axl_node_get_child_called but it
 * will look for a child node called as provided not only in direct
 * childs hold by the parent but also on its all descendants.
 *
 * If you are looking for a function to search for a particular child
 * node inside direct childs stored for the provided parent, then you
 * must use \ref axl_node_get_child_called.
 *
 * There is also a convenience function that allows to perform a
 * lookup using as a reference a document (using the root node from
 * it): \ref axl_doc_find_called.
 *
 * <i><b>NOTE:</b> This function isn't XML Namespace aware. You must use \ref axl_ns_node_find_called instead. See \ref axl_ns_doc_validate. </i>
 *
 * @param parent The parent where the lookup will be produced.
 *
 * @param name The name of the child to be looked up.
 * 
 * @return A reference to the node found (first instaned matching the
 * name) or NULL if it fails to find a child. 
 */
axlNode * axl_node_find_called    (axlNode * parent, char * name)
{
	axlNode * node;
	axlNode * child;

	/* for the first child found */
	node = axl_node_get_first_child (parent);
	while (node != NULL) {
		/* check and return the node found */
		if (NODE_CMP_NAME (node, name))
			return node;
		
		/* get next */
		node = axl_node_get_next (node);
	} /* end while */

	/* now, for all childs, try to look for the node */
	node = axl_node_get_first_child (parent);
	while (node != NULL) {
		/* make the search */
		child = axl_node_find_called (node, name);
		
		/* child found, return the reference */
		if (child != NULL)
			return child;
		
		/* get next */
		node = axl_node_get_next (node);
	} /* end while */

	/* child note found */
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
	int       iterator;
	axlItem * item;

	/* perform some environment checks */
	axl_return_val_if_fail (parent, NULL);

	/* check for first reference */
	if (parent->first == NULL) {
		return NULL;
	}

	/* get the first item */
	item = parent->first;

	/* get the first node found */
	iterator = 0;
	while (item != NULL) {
		/* check the item type */
		if (axl_item_get_type (item) == ITEM_NODE) {
			if (iterator == position) {
				return item->data;
			} else
				iterator++;
		} /* end if */

		/* get the next */
		item = item->next;

	} /* end while */

	/* no first child found */
	return NULL;
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
	int       count;
	axlItem * item;

	/* perform some environment checks */
	axl_return_val_if_fail (parent, -1);

	/* init values */
	count = 0;
	item  = parent->first;

	/* for each child inside the parent node */
	while (item != NULL) {
		
		/* check item type */
		if (axl_item_get_type (item) == ITEM_NODE)
			count++;
		
		/* get the next */
		item = item->next;

	} /* end while */

	/* return the number of chils */
	return count;
}


/** 
 * @brief Allows to get childs nodes for the given xml node (\ref
 * axlNode).
 *
 * This function creates a newly allocated list. In the case a
 * iterating over all childs for the provided node is its better to
 * use the following:
 * \code
 * axlNode * child;
 *
 * // get the first child 
 * child = axl_node_get_first_child (parent);
 * 
 * // iterate over all nodes 
 * while (child != NULL) {
 *
 *    // do something with the child 
 *    do_some_work (child);
 * 
 *    // update the reference to the next child
 *    child = axl_node_get_next (child);   
 * }
 * \endcode
 *
 * @param node The node where the childs will be returned.
 * 
 * @return An \ref axlList containing \ref axlNode items or NULL if it
 * fails. The list returned MUST be deallocated.
 */
axlList * axl_node_get_childs         (axlNode * node)
{
	axlItem * child;
	axlList * result;

	axl_return_val_if_fail (node, NULL);
	
	/* create the result list without destroy function */
	result  = axl_list_new (__axl_node_equal, NULL);
	
	/* get the first child */
	child = node->first;

	while (child != NULL) {
		/* check the node type */
		if (axl_item_get_type (child) == ITEM_NODE) {
			/* add the child to the list */
			axl_list_add (result, child->data);
			
		} /* end if */

		/* update the reference to the next child */
		child = child->next;
	} /* end while */

	/* return current childs */
	return result;
}

bool __axl_node_are_equal_attr (axlPointer key, 
				axlPointer value, 
				axlPointer user_data, 
				axlPointer user_data2)
{
	char                * value2;
	bool                * result = user_data2;
	
	/* get the attr value and compare it with data */
	value2 = axl_hash_get ((axlHash *) user_data, (char *) key);

	if (! axl_cmp ((char *) value, value2)) {
		/* flag that node attributes aren't equal */
		(* result) = false;

		/* stop foreach */
		return true;
	}
			
	/* make the process to continue */
	return false;
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
	bool result;
	
	axl_return_val_if_fail (node,  false);
	axl_return_val_if_fail (node2, false);

	/* check document root name */
	if (! axl_cmp (axl_node_get_name (node), axl_node_get_name (node2))) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node names aren't equal <%s> != <%s>",
			   node->name, node2->name);
		return false;
	}

	/* check empty ness configuration */
	if (axl_node_is_empty (node) != axl_node_is_empty (node2)) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "emptyness configuration differs <%s> != <%s>",
			   node->name, node2->name);
		return false;
	}
	
	/* check childs configuration */
	if (axl_node_have_childs (node) != axl_node_have_childs (node2)) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "childs configuration differs <%s> != <%s>",
			   node->name, node2->name);
		return false;
	}

	/* check childs number */
	if (axl_node_get_child_num (node) != axl_node_get_child_num (node2)) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "child number differs <%s>(%d) != <%s>(%d)",
			   node->name, axl_node_get_child_num (node), node2->name, axl_node_get_child_num (node2));
		return false;
	}

	/* check attribute values */
	if ((node->attributes != NULL && node2->attributes != NULL)) {

		/* check the number of attributes that has the hash */
		if (node->attr_num != node2->attr_num) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "both nodes have different number of attributes (%d != %d)",
				   node->attr_num, node2->attr_num);
			return false;
		}

		/* now both hashes */
		result = true;
		if (node->attr_num <= 10) {
			/* check both list are equal */
			result = __axl_node_attr_list_is_equal ((axlNodeAttr *) node->attributes, (axlNodeAttr *) node2->attributes);
		} else {
			/* check both hashes are equal */
			axl_hash_foreach2 ((axlHash *) node->attributes, __axl_node_are_equal_attr, (axlHash *) node2->attributes, &result);
		}

		if (! result) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node attributes differs <%s> != <%s>",
				   node->name, node2->name);
			/* attribute missmatch */
			return false;
		}
	} else {
		if (node->attributes == NULL && node2->attributes != NULL) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node <%s> has no attributes but <%s> has",
				   axl_node_get_name (node), axl_node_get_name (node2));
			return false;
		}

		if (node2->attributes == NULL && node->attributes != NULL) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node <%s> has no attributes but <%s> has",
				   axl_node_get_name (node2), axl_node_get_name (node));
			return false;
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
	axlPI   * pi;

	/* perform some environmental checks */
	axl_return_if_fail (node);
	axl_return_if_fail (target);

	/* create the PI element */
	pi = axl_pi_create (target, content);

	/* set the new process instruction found */
	axl_item_set_child (node, ITEM_PI, pi);

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
	axlPI   * pi;
	axlItem * item;
	
	axl_return_val_if_fail (node,      false);
	axl_return_val_if_fail (pi_target, false);

	/* assume the pi target doesn't exist if it is not
	 * initialized */
	item = node->first;
	while (item != NULL) {
		
		/* check the type */
		if (axl_item_get_type (item) == ITEM_PI) {
			/* get a reference */
			pi = item->data;

			/* only check the first ocurrency */
			if (axl_cmp (axl_pi_get_name (pi), pi_target))
				return true;
		} /* end if */

		/* get the next item */
		item = item->next;

	} /* end while */

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
	axlPI   * pi;
	axlItem * item;
	
	axl_return_val_if_fail (node,      false);
	axl_return_val_if_fail (pi_target, false);

	/* assume the pi target doesn't exist if it is not
	 * initialized */
	item = node->first;
	while (item != NULL) {
		
		/* check the type */
		if (axl_item_get_type (item) == ITEM_PI) {
			/* get a reference */
			pi = item->data;

			/* only check the first ocurrency */
			if (axl_cmp (axl_pi_get_name (pi), pi_target))
				return axl_pi_get_content (pi);
		} /* end if */

		/* get the next item */
		item = item->next;

	} /* end while */

	return NULL;
}

/** 
 * @brief Allows to transfer (move from) all childs (including
 * comments, content, PI, nodes, etc) from the old parent to the new
 * parent.
 *
 * This function is particular useful while moving content from nodes.
 * 
 * @param old_parent The old parent node where all childs will be
 * removed and and placed in the new parent.
 *
 * @param new_parent The parent node where the content will be
 * placed. If the parent node already have childs, the content will be
 * appended.
 */
void      axl_node_transfer_childs          (axlNode * old_parent, 
					     axlNode * new_parent)
{
	axlItem * item;
	axlItem * item_aux;
	
	/* get the first child for the old parent */
	item = old_parent->first;
	while (item != NULL) {

		/* get a reference to the next before adding */
		item_aux = item->next;

		/* set the item to parent for the new node */
		axl_item_set_child_ref (new_parent, item);

		

		/* get the next */
		item = item_aux;

	} /* end while */

	/* clear reference from previous parent */
	old_parent->first     = NULL;
	old_parent->last      = NULL;
	
	return;
}

/** 
 * @internal Implementation for the public API provided to dump node content.
 */
bool __axl_node_dump_common (axlNode * node, char ** content, int * size, bool pretty_print, int level, int tabular)
{
	int    _size;
	int    index;
	char * result;

	/* check refererences received */
	axl_return_val_if_fail (node, false);
	axl_return_val_if_fail (content, false);

	/* get dump size */
	_size = axl_node_get_flat_size (node, pretty_print, level, tabular);

	/* dump the content */
	result = axl_new (char, _size + 1);
	index  = axl_node_dump_at (node, result, 0, pretty_print, level, tabular);

	/* check result */
	if (index != _size) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "failed to dump xml node, size dump operation mismatch: %d != %d", 
			   index, _size);
		/* free allocated result */
		axl_free (result);

		/* nullify content */
		if (size)
			*size = -1;
		*content      = NULL;
		
		return false;
	} /* end if */

	/* fill the size */
	if (size)
		*size = _size;
	*content      = result;

	/* return content */
	return true;
}

/** 
 * @brief Allows to dump the xml content taking as starting point the
 * xml node provided. 
 * 
 * @param node The \ref axlNode to dump.
 *
 * @param content The reference where the result will be returned.
 *
 * @param size The reference where the document content size will be
 * returned. 
 *
 * @return The function returns \ref true if the dump operation was
 * performed. Otherwise \ref false is returned.
 */
bool      axl_node_dump                    (axlNode  * node, 
					    char    ** content, 
					    int      * size)
{
	/* use common implementation for all functions */
	return __axl_node_dump_common (node, content, size, false, 0, 0);
}

/** 
 * @brief Allows to perform a pretty print operation using as
 * reference (starting point) the node provided.
 * 
 * @param node The node to be used as reference for the dump operation.
 * 
 * @param content A reference to a user defined pointer where the
 * content will be placed. Not optional parameter.
 *
 * @param size A reference to a user defined pointer where the content
 * size will be placed. Optional parameter.
 * 
 * @param tabular How many spaces to be placed at each level.
 * 
 * @return true if the dump operation was properly done, otherwise
 * false is returned.
 */
bool      axl_node_dump_pretty             (axlNode  * node,
					    char    ** content,
					    int      * size,
					    int        tabular)
{
	/* use common implementation for all functions */
	return __axl_node_dump_common (node, content, size, true, 0, tabular);
}

/** 
 * @brief Allows to dump the xml document using as reference the node
 * provided, at the file path provided.
 * 
 * @param node The document node reference to use to build the
 * content.
 *
 * @param file_path File path where place the result.
 * 
 * @return \ref true if the dump operation was done, otherwise \ref false is
 * returned.
 */
bool      axl_node_dump_to_file            (axlNode  * node,
					    char     * file_path)
{
	char * content;
	int    size;
	FILE * fd;
	int    written;

	/* use common implementation for all functions */
	if (!  __axl_node_dump_common (node, &content, &size, true, 0, 0))
		return false;

	/* open the file and check */
#if defined(AXL_OS_WIN32) && ! defined(__GNUC__)
	if (fopen_s (&fd, file_path, "w") != 0) {
#else
	if ((fd = fopen (file_path, "w")) == NULL) {
#endif
		/* failed to open the file to dump the content */
		axl_free (content);

		return false;
	}

	/* dump the content */
	written = fwrite (content, 1, size, fd);

	/* free the content */
	axl_free (content);

	/* close file */
	fclose (fd);

	/* return if we have failed to dump all the content to the
	 * file or not. */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "returning that the dump was: %s (written:%d == size:%d)", 
		   (written == size) ? "OK" : "FAILED", 
		   written, size);
		   
	return (written == size);
		
}

/** 
 * @brief Allows to pretty print dump the xml document using as
 * reference the node provided, at the file path provided.
 * 
 * @param node The document node reference to use to build the
 * content.
 *
 * @param file_path File path where place the result.
 *
 * @param tabular How many spaces to be placed at each level.
 * 
 * @return \ref true if the dump operation was done, otherwise \ref false is
 * returned.
 */
bool      axl_node_dump_pretty_to_file     (axlNode  * node,
					    char     * file_path,
					    int        tabular)
{
	char * content;
	int    size;
	FILE * fd;
	int    written;

	/* use common implementation for all functions */
	if (!  __axl_node_dump_common (node, &content, &size, true, 0, tabular))
		return false;

	/* open the file and check */
#if defined(AXL_OS_WIN32) && ! defined(__GNUC__)
	if (fopen_s (&fd, file_path, "w") != 0) {
#else
	if ((fd = fopen (file_path, "w")) == NULL) {
#endif
		/* failed to open the file to dump the content */
		axl_free (content);

		return false;
	}

	/* dump the content */
	written = fwrite (content, 1, size, fd);

	/* free the content */
	axl_free (content);

	/* close file */
	fclose (fd);

	/* return if we have failed to dump all the content to the
	 * file or not. */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "returning that the dump was: %s (written:%d == size:%d)", 
		   (written == size) ? "OK" : "FAILED", 
		   written, size);
		   
	return (written == size);
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
 *
 *      // once finished, free the list 
 *      axl_list_free (PIs);
 *      return;
 * }
 * \endcode
 * 
 * @param node The xml node (\ref axlNode) where the process
 * instruction will be returned.
 * 
 * @return A reference to the list of processing instruction that the
 * xml node (\ref axlNode) has. The returned list, if defined, must be
 * deallocated.
 */
axlList * axl_node_get_pi_target_list       (axlNode * node)
{
	axlList * result = NULL;
	axlItem * item;
	
	axl_return_val_if_fail (node,      false);

	/* assume the pi target doesn't exist if it is not
	 * initialized */
	item = node->first;
	while (item != NULL) {
		
		/* check the type */
		if (axl_item_get_type (item) == ITEM_PI) {
			/* create the result list */
			if (result == NULL)
				result = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_pi_free);	 

			/* add the list */
			axl_list_add (result, item->data);
			
		} /* end if */

		/* get the next item */
		item = item->next;

	} /* end while */

	return result;
}

bool __axl_node_get_flat_size_attributes_foreach (axlPointer attr, 
						  axlPointer value, 
						  axlPointer user_data)
{
	int * length = user_data;

	/* " attribute='value' */
	(*length) += 4 + strlen ((char*) attr) + strlen ((char*) value);

	/* make the process to continue */
	return false;
}

/** 
 * @internal
 * @brief Returns the space required to write the attribute part.
 */
int __axl_node_get_flat_size_attributes (axlNode * node)
{

	int            length = 0;
	axlNodeAttr  * attr   = NULL;
	
	/* if attribute list is not defined just return 0 */
	if (node->attributes == NULL)
		return 0;

	/* perform a foreach and accumulate */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "get flat size for node <%s>", 
		   axl_node_get_name (node));
	if (node->attr_num <= 10) {
		/* get the first reference */
		attr = (axlNodeAttr *) node->attributes;
		while (attr != NULL) {
			/* call to get length */
			__axl_node_get_flat_size_attributes_foreach (attr->attribute, attr->value, &length);
			
			/* get the next */
			attr = attr->next;
		}
		
	} else {
		/* perform a foreach */
		axl_hash_foreach ((axlHash *) node->attributes, __axl_node_get_flat_size_attributes_foreach, &length);
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "after foreach flat size for node <%s> is %d", 
		   axl_node_get_name (node), length);

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
 * @param pretty_print If pretty print is activated.
 *
 * @param level Which is the relative level of the node respected to
 * the root node.
 * 
 * @return The stream size or -1 if fails.
 */
int       axl_node_get_flat_size            (axlNode * node, bool pretty_print, int level, int tabular)
{
	int              result    = 0;
	axlItem        * item;
	axlNodeContent * content;
	bool             is_empty;
	bool             have_childs;

	axl_return_val_if_fail (node, -1);

	/* get values */
	is_empty    = axl_node_is_empty (node);
	have_childs = axl_node_have_childs (node);

	if (have_childs || (!have_childs && !is_empty)) {
		/* the node is emtpy because it has no content but it has
		 * childs:
		 *
		 * "<" + strlen (node-name) + ">" + ... + "</" + strlen (node-name) + ">" */
		result = 5 + (2 * strlen (node->name)) + __axl_node_get_flat_size_attributes (node);
		
		/* check pretty_print */
		if (pretty_print) {
			/* two tabulations plus two carry return \r\n
			 * on windows and \n on unix */
			if (have_childs) 
				result += (level * tabular * 2) + 2; 
			else
				result += (level * tabular) + 1; 
#ifdef __AXL_OS_WIN32__
			if (have_childs)
				result += 2;
			else
				result ++;
#endif
		}
	} else {
		if (is_empty) {
			/* "<" + strlen (node-name) + " />" */
			result = strlen (node->name) + 4 + __axl_node_get_flat_size_attributes (node);
			
			/* check pretty print */
			if (pretty_print) {
				/* one tabular plus one carry return
				 * \r\n on windows and \n on unix */
				result += (level * tabular) + 1;
#ifdef __AXL_OS_WIN32__
				result += 1;
#endif
			}
			
			/* return sum */
			return result;
		}
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node=<%s> count=%d", node->name, result);
	
	/* get first child */
	item = node->first;
	while (item != NULL) {
		/* according to the type, check a size */
		switch (axl_item_get_type (item)) {
		case ITEM_NODE:
			/* count how many bytes the node holds */
			result += axl_node_get_flat_size (item->data, pretty_print, level + 1, tabular);
			break;
		case ITEM_CONTENT:
			/* content */
			content = (axlNodeContent *) item->data;
			result += content->content_size;
			break;
		case ITEM_CDATA:
			/* content + '<![CDATA[' + ']]>' */
			content = (axlNodeContent *) item->data;
			result += content->content_size + 12;
			break;
		case ITEM_PI:
			/* get current size */
			result += axl_pi_get_size (item->data);
			break;
		case ITEM_COMMENT:
			/* content + '<!-- ' + ' -->' */
			content = (axlNodeContent *) item->data;
			result += content->content_size + 9;
			if (pretty_print) {
				/* tabular indent + \n */
				result += ((level + 1) * tabular) + 1;
#ifdef __AXL_OS_WIN32__
				/* \r\n */
				result += 1;
#endif
			}

			break;
		case ITEM_REF:
			/* item ref + '&' + ';' */
			content = (axlNodeContent *) item->data;
			result += content->content_size + 2;
			break;
		case ITEM_FROM_FACTORY:
		case ITEM_CONTENT_FROM_FACTORY:
			/* never reached */
			break;
		}
		/* get next item */
		item = item->next;
			
	} /* end while */
		
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "child processing finished for: parent=<%s>, count=%d", node->name, result);

	/* return the result */
	return result;
}

bool __axl_node_dump_attributes_at_foreach (axlPointer key, 
					    axlPointer value, 
					    axlPointer user_data,
					    axlPointer user_data2)
{
	char * content    = user_data;
	int  * _desp      = user_data2;
	bool   terminator = false;
	int    desp       = (*_desp);
	int    length;

	memcpy (content + desp, " ", 1);
	desp += 1;
	
	length = strlen ((char*) key);
	memcpy (content + desp, (char *) key, length);
	desp += length;

	/* check if the content contains a ' so, enclose the attribute
	 * with " */
	if (strstr (value, "'") == NULL) {
		memcpy (content + desp, "='", 2);
		desp += 2;
	}else {
		terminator = true;
		memcpy (content + desp, "=\"", 2);
		desp += 2;
	}
	
	length = strlen ((char*) value);
	memcpy (content + desp, (char*) value, length);
	desp += length;

	/* dump attribute termination */
	if (terminator) {
		memcpy (content + desp, "\"", 1);
	}else {
		memcpy (content + desp, "'", 1);		
	}
	desp += 1;

	/* update desp */
	(*_desp) = desp;

	/* make the process to continue */
	return false;
}

void __axl_node_dump_at_the_end (axlNodeAttr * attr, char * content, int * desp)
{
	/* return if no attribute must be dumped */
	if (attr == NULL)
		return;

	/* call to dump next attributes first */
	__axl_node_dump_at_the_end (attr->next, content, desp);

	/* now dump our attribute */
	__axl_node_dump_attributes_at_foreach (attr->attribute, attr->value, content, desp);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "dumping attribute: %s=%s", attr->attribute, attr->value);

	return;
}

/** 
 * @internal
 * 
 * Internal support function which dumps current attribute
 * configuration into the given memory using the provided desp.
 */
int axl_node_dump_attributes_at (axlNode * node, char * content, int desp)
{
	axlNodeAttr  * attr   = NULL;	

	/* if attribute list is not defined just return 0 */
	if (node->attributes == NULL)
		return desp;

	/* according to the attribute num */
	if (node->attr_num <= 10) {
		/* get the first reference */
		attr = (axlNodeAttr *) node->attributes;
		__axl_node_dump_at_the_end (attr, content, &desp);

	} else {
		/* foreach attribute dump */
		axl_hash_foreach2 ((axlHash *) node->attributes, __axl_node_dump_attributes_at_foreach, content, &desp);
	}

	/* return the length */
	return desp;
}

/** 
 * @internal Writes the indentation according to the tabular size and
 * the indent level.
 * 
 * @param content The reference to the content where the dump
 * operation will be performed.
 *
 * @param tabular The tabular size to be applied for each level.
 *
 * @param level The indent level to be applied.
 * 
 * @return The number of bytes written.
 */
int __axl_node_dump_at_write_indent (char * content, int tabular, int level)
{
	int iterator = 0;

	while (iterator < (tabular * level)) {
		/* write tabular info */
		memcpy (content + iterator, " ", 1);
		
		/* update iterator */
		iterator++;
	} /* end while */

	return iterator;
}
	
/* dump content */
int __axl_node_dump_items (axlItem * item, char * content, int level, bool pretty_print, int desp, int tabular)
{

	axlNodeContent * nodeContent;
	char           * string_aux;

	/* get first child */
	while (item != NULL) {
		/* according to the type, check a size */
		switch (axl_item_get_type (item)) {
		case ITEM_NODE:
			/* write axl node content */
			desp  = axl_node_dump_at (item->data, content, desp, pretty_print, level + 1, tabular);
			break;
		case ITEM_CONTENT:
			/* write content information */
			nodeContent = (axlNodeContent *)item->data;
			memcpy (content + desp, nodeContent->content, nodeContent->content_size);
			desp += nodeContent->content_size;
			break;
		case ITEM_CDATA:
			/* write content information */
			nodeContent = (axlNodeContent *)item->data;
			
			/* write cdata content */
			memcpy (content + desp, "<![CDATA[", 9);
			desp += 9;
			
			/* write content */
			memcpy (content + desp, nodeContent->content, nodeContent->content_size);
			desp += nodeContent->content_size;
			
			/* write cdata end */
			memcpy (content + desp, "]]>", 3);
			desp += 3;
			break;
		case ITEM_PI:
			/* write pi start */
			memcpy (content + desp, "<?", 2);
			desp += 2;
			
			/* write pi name */
			string_aux = axl_pi_get_name (item->data);
			memcpy (content + desp, string_aux, strlen (string_aux));
			desp += strlen (string_aux);
			
			/* write pi start */
			memcpy (content + desp, " ", 1);
			desp += 1;
			
			/* write pi content */
			string_aux = axl_pi_get_content (item->data);
			memcpy (content + desp, string_aux, strlen (string_aux));
			desp += strlen (string_aux);
			
			/* write pi start */
			memcpy (content + desp, "?>", 2);
			desp += 2;
			break;
		case ITEM_COMMENT:

			/* check for pretty print to write indent */
			if (pretty_print) {
				desp += __axl_node_dump_at_write_indent (content + desp, tabular, level + 1);
			}

			/* content + '<!-- ' + ' -->' */
			memcpy (content + desp, "<!-- ", 5);
			desp += 5;
			
			/* get a reference to the content */
			nodeContent = (axlNodeContent *)item->data;
			
			/* write content */
			memcpy (content + desp, nodeContent->content, nodeContent->content_size);
			desp += nodeContent->content_size;
			
			memcpy (content + desp, " -->", 4);
			desp += 4;

			if (pretty_print) {
#ifdef __AXL_OS_WIN32__
				memcpy (content + desp, "\r\n", 2);
				desp += 2;
#else
				memcpy (content + desp, "\n", 1);
				desp += 1;
#endif
			}

			break;
		case ITEM_REF:
			/* content + '&' + ';' */
			memcpy (content + desp, "&", 1);
			desp += 1;
			
			/* get a reference to the content */
			nodeContent = (axlNodeContent *)item->data;
			
			/* write content */
			memcpy (content + desp, nodeContent->content, nodeContent->content_size);
			desp += nodeContent->content_size;
			
			memcpy (content + desp, ";", 1);
			desp += 1;
			break;
		case ITEM_FROM_FACTORY:
		case ITEM_CONTENT_FROM_FACTORY:
			/* never reached */
			break;
		}
		
		/* get next item */
		item = item->next;
		
	} /* end while */

	/* return desp calculated */
	return desp;

} /* end __axl_node_dump_items */

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
					     int       desp,
					     bool      pretty_print,
					     int       level,
					     int       tabular)
{
	axlItem        * item;

	axl_return_val_if_fail (node, -1);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "dumping node=<%s> at %d", 
		   axl_node_get_name (node), desp);

	/* check for pretty print and tabular */
	if (pretty_print) {
		desp += __axl_node_dump_at_write_indent (content + desp, tabular, level);
	} /* end if */

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

			/* write traling node information */
			if (pretty_print) {
#ifdef __AXL_OS_WIN32__
				memcpy (content + desp, "\r\n", 2);
				desp += 2;
#else
				memcpy (content + desp, "\n", 1);
				desp += 1;
#endif
			}

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
		
		/* write traling node information */
		if (pretty_print) {
#ifdef __AXL_OS_WIN32__
			memcpy (content + desp, "\r\n", 2);
			desp += 2;
#else
			memcpy (content + desp, "\n", 1);
			desp += 1;
#endif
		} /* end if */

		/* dump content */
		item = node->first;
		desp = __axl_node_dump_items (item, content, level, pretty_print, desp, tabular);
		
		/* check for pretty print and tabular */
		if (pretty_print) {
			desp += __axl_node_dump_at_write_indent (content + desp, tabular, level);
		} /* end if */

	}else {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the node is not empty and have no childs");

		/* dump content */
		item = node->first;
		desp = __axl_node_dump_items (item, content, level, pretty_print, desp, tabular);
	} /* end if */

	/* dump close tag */
	memcpy (content + desp,	"</", 2);
	desp += 2;
	
	memcpy (content + desp, node->name, strlen (node->name));
	desp += strlen (node->name);
	
	memcpy (content + desp,	">", 1);
	desp += 1;

	/* write traling node information */
	if (pretty_print) {
#ifdef __AXL_OS_WIN32__
		memcpy (content + desp, "\r\n", 2);
		desp += 2;
#else
		memcpy (content + desp, "\n", 1);
		desp += 1;
#endif
	}

	/* return the result */
	return desp;
}

/** 
 * @brief Allows to check if the provided string have escape sequences
 * that must be defined by using the entity reference rather the value
 * itself.
 *
 * This function is useful in the sense it allows to know if a
 * particular content will contain elements not allowed by the XML 1.0
 * definition to be placed directly (like &, <, ;, ' and ").
 *
 * 
 *
 * @param content The content to check.
 *
 * @param content_size The size of the content to be checked. If -1 is
 * provided, the function will calculate the content length.
 *
 * @param added_size An integer reference where the additional size
 * variable will be added. This additional size will be the space
 * required to replace non-valid characters with entity
 * references. This parameter is optional, so passing a NULL value is
 * allowed.
 * 
 * @return true if the string contains non valid sequences that
 * must be escaped using entity references.
 */
bool      axl_node_has_invalid_chars        (const char * content,
					     int          content_size,
					     int        * added_size)
{
	int      iterator = 0;
	bool     result   = false;
	axl_return_val_if_fail (content, false);

	/* reset additional size value */
	if (added_size != NULL)
		*added_size = 0;

	/* calculate the content size */
	if (content_size == -1)
		content_size = strlen (content);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking valid sequence: content size=%d", content_size);

	/* iterate over all content defined */
	while (iterator < content_size) {
		/* check for &apos; */
		if (content [iterator] == '\'') {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found invalid sequence='\\'");
			result = true;
			if (added_size != NULL)
				(*added_size) += 5;
		}

		/* check for &quot; */
		if (content [iterator] == '"') {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found invalid sequence='\"'");
			result = true;
			if (added_size != NULL)
				(*added_size) += 5;
		}

		/* check for &amp; */
		if (content [iterator] == '&') {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found invalid sequence='&'");
			result = true;
			if (added_size != NULL)
				(*added_size) += 4;
		}

		/* check for &gt; */
		if (content [iterator] == '>') {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found invalid sequence='>'");
			result = true;
			if (added_size != NULL)
				(*added_size) += 3;
		}

		/* check for &lt; */
		if (content [iterator] == '<') {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found invalid sequence='<'");
			result = true;
			if (added_size != NULL)
				(*added_size) += 3;
		}

		/* update the iterator */
		iterator++;
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "valid sequence checking result=%d: content size=%d, additional size=%d resul=%s", 
		   result, content_size, (added_size != NULL) ? *added_size : 0, result ? "HAS INVALID SEQUENCES" : "HAS NO INVALID SEQUENCES");

	/* return results */
	return result;
}

/** 
 * @brief Allows to perform a copy for the content provided, doing an
 * xml character escaping for non allowed values (&, <, >, ' and ").
 *
 * This function must be used with \ref axl_node_has_invalid_chars to
 * check if the content has escapable chars an to get the additional
 * content that must be allocated by this function.
 *
 * Here is an example:
 * \code
 * char * content = "Some content with invalid chars & < >";
 * int    additional_size;
 * char * new_content
 *
 * if (axl_node_has_invalid_chars (content, strlen (content), &additional_size)) {
 *      // found that the string has invalid chars, escape them
 *      new_content = axl_node_content_copy_and_escape (content, strlen (content), additional_size);
 * } 
 *
 * \endcode
 * 
 * @param content The content to be escaped. If this parameter is
 * null, the function returns NULL.
 * 
 * @param content_size The content size for the first parameter.
 *
 * @param additional_size The additional size calculated from \ref axl_node_has_invalid_chars.
 *
 * @return A newly allocated string with all characters escaped. Use
 * \ref axl_free to dealloc the result.
 */
char * axl_node_content_copy_and_escape (const char * content, 
					 int          content_size, 
					 int          additional_size)
{
	axl_return_val_if_fail (content, NULL);
	
	/* call to the internal implementation */
	return __axl_node_content_copy_and_escape (content, content_size, additional_size);
} 

void __axl_node_free_internal (axlNode * node, bool also_childs)
{
	axlItem * item;
	axlItem * itemAux;

	axl_return_if_fail (node);

	/* free current node */
	if (node->name != NULL && ! (node->conf & NODE_NAME_FROM_FACTORY))
		axl_free (node->name);

	/* release memory hold by attributes */
	if (node->attributes != NULL) {
		if (node->attr_num <= 10)
			__axl_node_free_attr_list ((axlNodeAttr *) node->attributes);
		else 
			axl_hash_free ((axlHash *) node->attributes);
	}

	/* release memory hold by childs */
	if (node->first != NULL && also_childs) {
		/* get the first item */
		item = node->first;

		/* foreach item stored */
		while (item != NULL) {

			/* get the next item */
			itemAux = item->next;			

			/* free the item */
			axl_item_free (item, true);
			
			/* update reference */
			item = itemAux;

		} /* end while */

	} /* end if */

	/* free the item itself */
	if (node->holder != NULL) {
		if ((node->holder->type & ITEM_FROM_FACTORY) == 0) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "item holder found to be not from a factory, dealloc reference=0x%x, node=0x%x, type=%d",
				   node->holder, node, node->holder->type);
			axl_free (node->holder);
			node->holder = NULL;
		}
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
	axlHash * hash;
	axl_return_if_fail (node);
	
	/* get a reference to the hash */
	hash = node->annotate_data;

	/* free internal content */
	__axl_node_free_internal (node, true);

	/* free attributes */
	if (! (node->conf & NODE_FROM_FACTORY))
		axl_free (node);

	/* annotate data deallocation must be done here because it is
	 * used by the application user and the library to store
	 * reference that could be pointing to internal structures
	 * deallocated by the __axl_node_free_internal */
	axl_hash_free (hash);

	/* the node to release */
	return;
}

/** 
 * @brief Allows to remove the provided node without removing childs
 * inside it.
 * 
 * @param node The node to deallocate.
 *
 * @param also_childs Signal the function to dealloc childs or not.
 */
void      axl_node_free_full       (axlNode * node, bool also_childs)
{
	axlHash * hash;
	axl_return_if_fail (node);
	
	/* get a reference to the hash */
	hash = node->annotate_data;

	/* free node */
	__axl_node_free_internal (node, false);

	/* free the node itself */
	if (!(node->conf & NODE_FROM_FACTORY))
		axl_free (node);

	/* annotate data deallocation must be done here because it is
	 * used by the application user and the library to store
	 * reference that could be pointing to internal structures
	 * deallocated by the __axl_node_free_internal */
	axl_hash_free (hash);

	return;
}

/**
 * @}
 */

/**
 * \defgroup axl_node_attribute_cursor Axl Node Attribute iteration: An interface provided to iterate attribute nodes without knowing them.
 */

/** 
 * \addtogroup axl_node_attribute_cursor
 * @{
 */


/** 
 * @brief Allows to get a cursor to iterate attributes found in the
 * provided node in a linear and efficient way.
 *
 * The \ref axlAttrCursor could be used to iterate attributes
 * inside a particular node in an efficient way because it stores
 * current state (position), hiding all module details, providing
 * access to attributes without knowing them. Then using the following
 * functions you can modify the state (current position to get):
 * 
 *   - \ref axl_node_attr_cursor_first
 *   - \ref axl_node_attr_cursor_next
 *
 * Finally, the following functions are provided to get the the key
 * and the value data associated to the current selected attribute,
 * pointed by the current status of the cursor:
 * 
 *   - \ref axl_node_attr_cursor_get_key (returns the key of the current attribute selected)
 *   - \ref axl_node_attr_cursor_get_value (returns the value of the current attribute selected)
 *
 * Here is an example:
 * \code
 * axlPointer           key;
 * axlPointer           value;
 * axlAttrCursor * cursor;
 * 
 * // create the cursor 
 * cursor   = axl_node_attr_cursor_new (node);
 *
 * // while there are more elements 
 * while (axl_node_attr_cursor_has_item (cursor)) {
 *
 *   // get the value and key
 *   key   = axl_node_attr_cursor_get_key   (cursor);
 *   value = axl_node_attr_cursor_get_value (cursor);
 *
 *   // get the next 
 *   axl_node_attr_cursor_next (cursor);
 *
 * } 
 *
 * // free the cursor 
 * axl_node_attr_cursor_free (cursor);
 * \endcode
 *
 * Once created the \ref axlAttrCursor you must release it and
 * create a new one if you modify your \ref axlNode attribute configuration
 * adding more items.
 * 
 * @param node The node that is requested to create the \ref
 * axlAttrCursor reference to iterate all attributes inside.
 * 
 * @return A newly created \ref axlAttrCursor used to iterate
 * attributes inside the node provided. Once finished you must call to
 * \ref axl_node_attr_cursor_free.
 */
axlAttrCursor * axl_node_attr_cursor_new       (axlNode * node)
{
	axlAttrCursor * cursor;

	axl_return_val_if_fail (node, NULL);

	/* create and configure basics */
	cursor        = axl_new (axlAttrCursor, 1);
	cursor->node  = node;
	cursor->count = node->attr_num;

	/* according to the number of attributes configure the hash or
	 * the linked attribte list */
	if (cursor->count <= 10) {
		/* just point to the first attribute (managed as AxlNodeAttr) */
		cursor->data = node->attributes;		
	} else {
		/* create an axl hash cursor */
		cursor->data = axl_hash_cursor_new ((axlHash *) node->attributes);
	} /* end if */

	/* return created cursor */
	return cursor;
}

/** 
 * @brief Allows to configure the provided cursor to point to the
 * first attribute found inside the node.
 * 
 * @param cursor The cursor that is going to be configured.
 */
void                 axl_node_attr_cursor_first     (axlAttrCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* check attribute configuration */
	if (cursor->count != cursor->node->attr_num) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Your cursor was created with another attribute configuration, renew it.");
		return;
	} /* end if */

	if (cursor->count <= 10) {
		/* just point to the first attribute (managed as AxlAttribute) */
		cursor->data = cursor->node->attributes;		
	} else {
		/* make hash cursor to point to the first */ 
		axl_hash_cursor_first (cursor->data);
	} /* end if */
	
	return;
}

/** 
 * @brief Configures the provided attribute cursor to point to the
 * next attribute.
 * 
 * @param cursor The attribute cursor to be configured.
 */
void                 axl_node_attr_cursor_next      (axlAttrCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* check attribute configuration */
	if (cursor->count != cursor->node->attr_num) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Your cursor was created with another attribute configuration, renew it.");
		return;
	} /* end if */

	if (cursor->count <= 10) {
		/* check null values */
		if (cursor->data == NULL)
			return;

		/* just point to the first attribute (managed as AxlAttribute) */
		cursor->data = ((axlNodeAttr *) cursor->data)->next;

		return;
	} /* end if */

	/* make hash cursor to point to the first */ 
	axl_hash_cursor_next (cursor->data);

	
	return;
}

/** 
 * @brief Allows to check if the is a next attribute, following
 * current attribute selected.
 * 
 * @param cursor The cursor to be configured.
 *
 * @return \ref true if it has next element, otherwise \ref false.
 */
bool                 axl_node_attr_cursor_has_next  (axlAttrCursor * cursor)
{
	axl_return_val_if_fail (cursor, false);

	if (cursor->count != cursor->node->attr_num) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Your cursor was created with another attribute configuration, renew it.");
		return false;
	} /* end if */

	if (cursor->count <= 10) {
		/* just point to the first attribute (managed as AxlAttribute) */
		return (((axlNodeAttr *) cursor->data)->next) != NULL;
	}  /* end if */
	
	/* make hash cursor to point to the first */ 
	return axl_hash_cursor_has_next (cursor->data);
}

/** 
 * @brief Allows to check if the current position selected has an
 * attribute reference.
 * 
 * @param cursor The cursor that is being queried.
 *
 * @return \ref true if it has item element, otherwise \ref false.
 */
bool                 axl_node_attr_cursor_has_item  (axlAttrCursor * cursor)
{
	axl_return_val_if_fail (cursor, false);

	/* check attribute configuration */
	if (cursor->count != cursor->node->attr_num) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Your cursor was created with another attribute configuration, renew it.");
		return false;
	} /* end if */

	if (cursor->count <= 10) {
		/* just point to the first attribute (managed as AxlAttribute) */
		return cursor->data != NULL;
	}  /* end if */
	
	/* make hash cursor to point to the first */ 
	return axl_hash_cursor_has_item (cursor->data);
}

/** 
 * @brief Allows to get the attribute key associated to the attribute
 * selected by the cursor.
 * 
 * @param cursor The cursor that is being queried.
 * 
 * @return A reference to the attribute key or NULL if it fails.
 */
const char *         axl_node_attr_cursor_get_key   (axlAttrCursor * cursor)
{
	axl_return_val_if_fail (cursor, NULL);

	/* check attribute configuration */
	if (cursor->count != cursor->node->attr_num) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Your cursor was created with another attribute configuration, renew it.");
		return NULL;
	} /* end if */

	if (cursor->count <= 10) {
		/* just point to the first attribute (managed as AxlAttribute) */
		return ((axlNodeAttr *) cursor->data)->attribute;
	}  /* end if */
	
	/* make hash cursor to point to the first */ 
	return axl_hash_cursor_get_key (cursor->data);
}

/** 
 * @brief Allows to get the attribute value associated to the
 * attribute selected by the cursor.
 * 
 * @param cursor The cursor that is being queried.
 * 
 * @return A reference to the attribute value or NULL if it fails.
 */
const char *         axl_node_attr_cursor_get_value (axlAttrCursor * cursor)
{
	axl_return_val_if_fail (cursor, NULL);

	/* check attribute configuration */
	if (cursor->count != cursor->node->attr_num) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Your cursor was created with another attribute configuration, renew it.");
		return NULL;
	} /* end if */

	if (cursor->count <= 10) {
		/* just point to the first attribute (managed as AxlAttribute) */
		return ((axlNodeAttr *) cursor->data)->value;
	}  /* end if */
	
	/* make hash cursor to point to the first */ 
	return axl_hash_cursor_get_value (cursor->data);	
}

/** 
 * @brief Allows to release \ref axlAttrCursor.
 * 
 * @param cursor The cursor to release.
 */
void                 axl_node_attr_cursor_free      (axlAttrCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* free hash cursor */
	if (cursor->count > 10)
		axl_hash_cursor_free (cursor->data);

	/* free the cursor */
	axl_free (cursor);

	return;
}


/** 
 * @internal Function that helps axl_node_attr_foreach to iterate all
 * attributes.
 */
bool __axl_node_attr_foreach_aux (axlPointer key, axlPointer data, axlPointer user_data, axlPointer user_data2, axlPointer user_data3)
{
	return ((axlNodeAttrForeachFunc) user_data) (key, data, user_data2, user_data3);
}

/** 
 * @brief Allows to provide a function which is called foreach
 * attribute installed on the provided node.
 *
 * This function will allow you to operate on every attribute
 * installed, doing an foreach operation. This is an alternative API
 * to the \ref axlAttrCursor, which could be used to save allocations.
 * 
 * @param node The node for which the provided function will be called
 * for each attribute found.
 *
 * @param func The foreach function to be called.
 *
 * @param data User defined data to be passed to the foreach function.
 *
 * @param data2 Second user defined data to be passed to the foreach
 * function.
 */
void            axl_node_attr_foreach          (axlNode       * node, 
						axlNodeAttrForeachFunc func, 
						axlPointer      data,
						axlPointer      data2)
{
	axlNodeAttr * attr;
	axl_return_if_fail (node);
	axl_return_if_fail (func);
	
	/* if no attributes no foreach operation */
	if (node->attributes == NULL)
		return;

	/* store the attribute using the general case */
	if (node->attr_num < 11) {
		/* handled as a simple list */
		attr = (axlNodeAttr *) node->attributes;
		while (attr != NULL) {
			/* call to notify each attribute */
			if (func (attr->attribute, attr->value, data, data2))
				return;

			/* get the next */
			attr = attr->next;
		} /* end while */
	} else {
		/* handled as a hash */
		axl_hash_foreach3 ((axlHash *) node->attributes, __axl_node_attr_foreach_aux, func, data, data2);
	} /* end if */

	return;
}

/**
 * @}
 */

/**
 * \defgroup axl_item_module Axl Item: A basic item abstraction that represents a child node that could be another node, content, xml comment, etc.
 */

/** 
 * \addtogroup axl_item_module
 * @{
 */

/** 
 * @brief Allows to create an \ref axlItem, with the provided type and
 * holding the provided data.
 *
 * The function won't configure the parent node holding the
 * item. There is an alternative API that allows to create an \ref
 * axlItem without performing a copy: \ref axl_item_new_ref.
 *
 * @param type The type that will represent the \ref axlItem created.
 * @param data Data associated to the axlItem. In the case the \ref
 * axlItem being created will represent content (\ref ITEM_CONTENT),
 * an entity ref (\ref ITEM_REF), a comment (\ref ITEM_COMMENT) or
 * cdata (\ref ITEM_CDATA), the function will create a local copy. In
 * the case of a \ref ITEM_NODE, the function will copy the entire
 * node, and all its childs.
 * 
 * @return A newly allocated \ref axlItem with no parent and holding
 * the data provided.
 */
axlItem     * axl_item_new             (AxlItemType type,
					axlPointer  data)
{
	axlItem        * item = NULL;
	axlNode        * node;
	axlNodeContent * content;

	/* allocate an item type */
	item         = axl_new (axlItem, 1);
	item->type   = type;

	switch (axl_item_get_type (item)) {
	case ITEM_NODE:
		/* item the node */
		node                 = axl_node_copy (item->data, true, true);
		node->holder         = item;
		item->data           = node;
		break;
	case ITEM_CONTENT:
	case ITEM_CDATA:
	case ITEM_COMMENT:
		/* item content */
		content               = axl_new (axlNodeContent, 1);
		content->content      = axl_strdup ((const char *) data);
		content->content_size = strlen ((const char *) data);

		/* item content */
		item->data = content;
		break;
	case ITEM_PI:
		/* item pi */
		item->data = axl_pi_copy (data);
		break;
	case ITEM_REF:
		/* not implemented yet */
		break;
	case ITEM_FROM_FACTORY:
	case ITEM_CONTENT_FROM_FACTORY:
		/* never reached */
		break;
	} /* end switch */

	/* return item created */
	return item;
}

/** 
 * @brief Allows to create an \ref axlItem, with the provided type and
 * holding the provided data.
 *
 * The function won't configure the parent node holding the item.
 *
 * @param type The type that will represent the \ref axlItem created.
 * @param data Data associated to the axlItem. This function won't
 * perform any copy for the data received. The user calling to this
 * API will ensure that the data is only owned by the \ref axlItem
 * created.
 * 
 * @return A newly allocated \ref axlItem with no parent and holding
 * the data provided.
 */
axlItem     * axl_item_new_ref         (AxlItemType type,
					axlPointer  data)
{
	axlItem        * item = NULL;
	axlNode        * node;
	axlNodeContent * content;

	/* allocate an item type */
	item         = axl_new (axlItem, 1);
	item->type   = type;

	switch (axl_item_get_type (item)) {
	case ITEM_NODE:
		/* item the node */
		node = data;
		node->holder         = item;
		item->data           = node;
		break;
	case ITEM_CONTENT:
	case ITEM_CDATA:
	case ITEM_COMMENT:
		/* item content */
		content               = axl_new (axlNodeContent, 1);
		content->content      = data;
		content->content_size = strlen ((const char *) data);

		/* item content */
		item->data = content;
		break;
	case ITEM_PI:
		/* item pi */
		item->data = data;
		break;
	case ITEM_REF:
		/* not implemented yet */
		break;
	case ITEM_FROM_FACTORY:
	case ITEM_CONTENT_FROM_FACTORY:
		/* never reached */
		break;
	} /* end switch */

	/* return item created */
	return item;
}


/** 
 * @brief Allows to get the reference to the document that is holding
 * the provided item without taking into consideration the item type.
 * 
 * @param item The item that is required to return its document.
 * 
 * @return A reference to the \ref axlDoc that is holding the item.
 */
axlDoc  * axl_item_get_doc         (axlItem * item)
{
	axl_return_val_if_fail (item, NULL);

	/* return the document reference */
	return item->doc;
}

/** 
 * @internal Internal function that allows to configure the document
 * that is holding the item provided.
 * 
 * @param item The axlItem to be configured.
 * @param doc The axlDoc reference to configure.
 */
void      axl_item_set_doc         (axlItem * item, axlDoc * doc)
{
	axl_return_if_fail (item);

	/* configure document */
	item->doc = doc;

	return;
}

/** 
 * @brief Allows to get the parent that is containing the \ref axlItem
 * provider. The parent of a \ref axlItem is always a node.
 * 
 * @param item The \ref axlItem reference that is required to return
 * its parent.
 * 
 * @return A reference to the \ref axlNode.
 */
axlNode * axl_item_get_parent      (axlItem * item)
{
	/* return that we don't have parent */
	if (item == NULL)
		return NULL;

	/* return the parent */
	return item->parent;
}

/** 
 * @brief Allows to get the following element that is next to the item
 * reference provided (\ref axlItem), at the same level.
 * 
 * @param item The item that is required to return its next reference.
 * 
 * @return A reference to the next element or NULL if no it fails or
 * no element is found next to the element provided.
 */
axlItem * axl_item_get_next        (axlItem * item)
{
	axl_return_val_if_fail (item, NULL);

	/* return the next element */
	return item->next;
}

/** 
 * @brief Returns the following \ref axlItem to the \ref axlNode
 * reference, in the same level.
 * 
 * @param node The node that is required to return the following item
 * to it.
 * 
 * @return An reference to the following or NULL.
 */
axlItem * axl_item_node_next (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);

	if (node->holder != NULL) {
		/* return the next */
		return node->holder->next;
	}

	/* no holder, no next */
	return NULL;

}

/** 
 * @brief Allows to get the following element that is previous to the item
 * reference provided (\ref axlItem), at the same level.
 * 
 * @param item The item that is required to return its previous reference.
 * 
 * @return A reference to the previous element or NULL if no it fails or
 * no element is found previous to the element provided.
 */
axlItem * axl_item_get_previous        (axlItem * item)
{
	axl_return_val_if_fail (item, NULL);

	/* return the previous element */
	return item->previous;
}

/** 
 * @brief Returns the previous \ref axlItem to the \ref axlNode
 * reference, in the same level.
 * 
 * @param node The node that is required to return the previous item
 * to it.
 * 
 * @return An reference to the previous or NULL.
 */
axlItem * axl_item_node_previous (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);

	if (node->holder != NULL) {
		/* return the previousx */
		return node->holder->previous;
	}

	/* no holder, no previous */
	return NULL;
}

/** 
 * @brief Allows to get the \ref axlItem reference that is holding the
 * node provided.
 * 
 * @param node The node that is required to return its holding item.
 * 
 * @return The item reference or NULL if it fails or it isn't set.
 */
axlItem     * axl_item_node_holder     (axlNode * node)
{
	axl_return_val_if_fail (node, NULL);

	/* return the holder */
	return node->holder;
}

/** 
 * @brief Allows to get the very first child item stored on the
 * provided \ref axlNode.
 *
 * This function is similar to \ref axl_node_get_first_child, but
 * returning the first \ref axlItem no matter its type.
 *
 * This function is mainly used inside the MIXED API where nodes are
 * expected to enclose content mixed with another xml nodes. See \ref
 * axl_node_get_first_child for more details.
 * 
 * @param node The \ref axlNode reference that is required to return
 * is first child reference (\ref axlItem).
 * 
 * @return The \ref axlItem reference or NULL if the axlNode is empty
 * (\ref axl_node_is_empty) and have no childs (\ref
 * axl_node_have_childs). The function also returns NULL if it fails
 * (when it receives a NULL reference).
 */
axlItem     * axl_item_get_first_child (axlNode * node)
{
	/* check reference received */
	axl_return_val_if_fail (node, NULL);

	/* return the first item reference */
	return node->first;
}

/** 
 * @brief Allows to get the very last child item stored on the
 * provided \ref axlNode.
 *
 * This function is similar to \ref axl_node_get_last_child, but
 * returning the last \ref axlItem no matter its type.
 *
 * This function is mainly used inside the MIXED API where nodes are
 * expected to enclose content mixed with another xml nodes. See \ref
 * axl_node_get_last_child for more details.
 * 
 * @param node The \ref axlNode reference that is required to return
 * is last child reference (\ref axlItem).
 * 
 * @return The \ref axlItem reference or NULL if the axlNode is empty
 * (\ref axl_node_is_empty) and have no childs (\ref
 * axl_node_have_childs). The function also returns NULL if it fails
 * (when it receives a NULL reference).
 */
axlItem     * axl_item_get_last_child  (axlNode * node) 
{
	/* check reference received */
	axl_return_val_if_fail (node, NULL);

	/* return the last item reference */
	return node->last;
}

/** 
 * @brief Allows to get the item type that represents the reference
 * received (\ref axlItem).
 *
 * Every \ref axlItem represents a particular content that could be
 * found inside an xml document parsed by the library (\ref
 * axlDoc). This function allows to return the type associated to the
 * element encapsulated by the \ref axlItem. See \ref AxlItemType for
 * more details.
 * 
 * @param item The reference that is required to return its type.
 * 
 * @return The type that is inside the reference or -1 if it fails.
 */
AxlItemType   axl_item_get_type        (axlItem * item)
{
	/* return stored type */
	return item->type & (~ (ITEM_FROM_FACTORY | ITEM_CONTENT_FROM_FACTORY));
}

/** 
 * @brief Returns the item data that is stored inside the \ref axlItem
 * received.
 *
 * According to the type that is representing the \ref axlItem
 * received, it will return a particular type. Check \ref AxlItemType
 * for more information.
 *
 * @param item The item that is required to return the data
 * encapsulated on it.
 * 
 * @return A pointer to the data stored, or NULL if it fails. The
 * pointer returned, in the case it is defined, mustn't be
 * released. It is a internal reference to the content.
 */
axlPointer axl_item_get_data (axlItem * item)
{
	axl_return_val_if_fail (item, NULL);

	/* return stored type */
	return item->data;
}

/** 
 * @brief Convenience API that allows to get the content stored (and
 * its size) from the received \ref axlItem, supposing it is storing
 * an \ref ITEM_CONTENT, \ref ITEM_CDATA, \ref ITEM_COMMENT or \ref ITEM_REF.
 *
 * @param item The \ref axlItem that is supposed to store an item with type: \ref ITEM_CONTENT, \ref ITEM_CDATA, \ref ITEM_COMMENT or \ref ITEM_REF.
 *
 * @param size Optional variable reference. If defined, if returns the
 * content size.
 * 
 * @return An internal reference to the content stored and optionally
 * the content size notified on the variable received. In the case the
 * function receives an incompatible \ref axlItem (which is not \ref
 * ITEM_CONTENT, \ref ITEM_CDATA, \ref ITEM_COMMENT or \ref ITEM_REF),
 * the function will return NULL, and the optional variable will be
 * filled with -1.
 */
char        * axl_item_get_content     (axlItem * item, 
					int     * size)
{
	axlNodeContent * content;

	/* check content received */
	if (size != NULL)
		*size = -1;
	
	/* check if the item reference is NULL */
	axl_return_val_if_fail (item, 
				NULL);
	axl_return_val_if_fail (axl_item_get_type (item) != ITEM_NODE && axl_item_get_type (item) != ITEM_PI, 
				NULL);

	/* get the content */
	content = item->data;

	/* fill the size */
	if (size != NULL)
		*size = content->content_size;

	/* return a pointer to the content */
	return content->content;
}

/* prepare the item to be added to the xml document */
axlItem *  __axl_item_common_configure (axlNode * parent, AxlItemType type, axlPointer data)
{
	axlNode * node = NULL;
	axlItem * item = NULL;

	/* return if the parent is defined */
	axl_return_val_if_fail (parent, NULL);

	/* check if the node received already have a pointer to a
	 * holder created */
	if (type & ITEM_NODE) {
		/* get a reference to the node */
		node = (axlNode *) data;

		/* get a reference to the holder */
		item = node->holder;

		/* check if the current item was allocated from a
		 * factory to ensure we don't loose that
		 * information */
		if ((item != NULL) && (item->type & ITEM_FROM_FACTORY)) 
			type = type | ITEM_FROM_FACTORY;
	}

	/* create an item to hold the child, configuring the node, the
	 * parent and the document */
	if (item == NULL) {
		if ((parent->holder != NULL) && (parent->holder->doc != NULL)) {
			item = axl_item_factory_get (axl_doc_get_item_factory (parent->holder->doc));
			type = type | ITEM_FROM_FACTORY; 
		} else 
			item   = axl_new (axlItem, 1);
	}
	item->type     = type;
	item->data     = data;
	item->doc      = (parent->holder != NULL) ? parent->holder->doc : NULL;

	if (item->type & ITEM_NODE) {
		/* now configure the item that will hold the new child */
		node->holder  = item;
	} /* end if */	
	
	/* return item created */
	return item;
}

/** 
 * @internal Function that helps adding a new item to the provided
 * parent node.
 *
 * The new item will be added as flaged by the type provided. The
 * function isn't exposed to the public API because there are better
 * alternatives to add items to a \ref axlNode. Don't use this API
 * directly.
 * 
 * @param parent The axl node that will receive the new content.
 *
 * @param type The type to configure to the new item.
 *
 * @param data The data associated to the data being stored.
 *
 * NOTE: the function doesn't check data received as it is supposed to
 * receive calls from the library.
 */
void axl_item_set_child (axlNode * parent, AxlItemType type, axlPointer data)
{
	axlItem * item;

	/* prepare the item to be added to the xml document */
	item = __axl_item_common_configure (parent, type, data);

	/* call to set child with a created item */
	axl_item_set_child_ref (parent, item);

	return;
}

/** 
 * @brief Allows to configure xml content just after the item used as
 * reference. 
 *
 * @param item The item used as reference to place the content after it.
 *
 * @param type AxlItemType to configure the content to be placed.
 *
 * @param data Pointer that is 
 */
void          axl_item_set_after       (axlItem * item,
					AxlItemType type,
					axlPointer data)
{
	axlItem * new_item = NULL;

	/* prepare the item to be added to the xml document */
	new_item = __axl_item_common_configure (item->parent, type, data);

	/* configure the parent node */
	new_item->parent   = item->parent;

	/* configure new item references */
	new_item->previous = item;
	new_item->next     = item->next;
	
	/* configure item references */
	if (item->next != NULL)
		item->next->previous = new_item;
	else
		item->parent->last  = new_item;
	item->next = new_item;

	return;
}

/* call to set child with a created item */
void axl_item_set_child_ref (axlNode * parent, axlItem * item)
{
	axl_return_if_fail (parent);
	axl_return_if_fail (item);

	/* configure the parent node */
	item->parent   = parent;

	/* get the current last child */
	if (parent->first == NULL) {
		/* init first and last reference to the only one
		 * child */
		parent->first = item;
		parent->last  = item;
	}else {
		/* configure the next item to the current last
		 * child */
		parent->last->next = item;

		/* update the last child reference */
		item->previous    = parent->last;
		item->next        = NULL;
		parent->last      = item;
	}

	return;
}

/** 
 * @brief Copies the reference provided creating a newly allocated
 * copy, including he content inside.
 * 
 * @param item The item to copy.
 *
 * @param set_parent Optionally, allows to provide the parent to be
 * configured to the item created. This is really required while
 * copying items that contains nodes.
 * 
 * @return A newly allocated \ref axlItem reference, containing the
 * same data (a deep copy) and optionally configured with the provided
 * parent.
 */
axlItem * axl_item_copy (axlItem * item, axlNode * set_parent)
{
	axlItem        * copy;
	axlNode        * node;
	axlNodeContent * content;

	/* check values received */
	axl_return_val_if_fail (item, NULL);

	/* allocate an copy type */
	copy         = axl_new (axlItem, 1);
	copy->type   = axl_item_get_type (item);
	copy->parent = set_parent;

	switch (axl_item_get_type (item)) {
	case ITEM_NODE:
		/* copy the node */
		node                 = axl_node_copy (item->data, true, true);
		node->holder         = copy;
		copy->data           = node;
		break;
	case ITEM_CONTENT:
	case ITEM_CDATA:
	case ITEM_COMMENT:
		/* copy content */
		content               = axl_new (axlNodeContent, 1);
		content->content      = axl_strdup (((axlNodeContent * ) item->data)->content);
		content->content_size = ((axlNodeContent * ) item->data)->content_size;

		/* copy content */
		copy->data = content;
		break;
	case ITEM_PI:
		/* copy pi */
		copy->data = axl_pi_copy (item->data);
		break;
	case ITEM_REF:
		/* not implemented yet */
		break;
	case ITEM_FROM_FACTORY:
	case ITEM_CONTENT_FROM_FACTORY:
		/* never reached */
		break;
	} /* end switch */

	/* return copy created */
	return copy;
}

/** 
 * @brief Allows to remove the \ref axlItem instance from the document
 * that is currently linked, optionally deallocating the memory used
 * by the structure.
 * 
 * @param item The item to remove from its container (without taking
 * into consideration the item type).
 *
 * @param dealloc Deallocs the memory used by the \ref axlItem
 * reference.
 */
void          axl_item_remove          (axlItem * item,
					bool      dealloc)
{

	/* free the item */
	axl_return_if_fail (item);

	/* realloc references */
	if (item->previous != NULL)
		item->previous->next = item->next;

	if (item->next != NULL)
		item->next->previous = item->previous;

	/* realloc parent references in the case of a node */
	if (axl_item_get_type (item) == ITEM_NODE) {
		if (item->previous == NULL)
			item->parent->first = item->next;

		if (item->next == NULL)
			item->parent->last  = item->previous;
	} /* end if */
	
	/* free the item */
	item->next     = NULL;
	item->previous = NULL;

	if (dealloc) {
		axl_item_free (item, true);
	} /* end if */

	return;

} /* end axl_item_remove */

/** 
 * @brief Allows to replace the content held by the \ref axlItem
 * reference with a new \ref axlItem, updating all references, and
 * optionally, deallocating the memory used by the previous node.
 * 
 * @param item The item to be replaced.
 *
 * @param new_item The new item to be placed where the previous one is
 * located.
 *
 * @param dealloc Signal to function to dealloc the memory hold the
 * the item replaced.
 */
void          axl_item_replace        (axlItem * item, 
				       axlItem * new_item,
				       bool      dealloc)
{
	/* free the item */
	axl_return_if_fail (item);
	axl_return_if_fail (new_item);

	/* realloc references */
	if (item->previous != NULL) {
		item->previous->next = new_item;
		new_item->previous   = item->previous;
	}

	if (item->next != NULL) {
		item->next->previous = new_item;
		new_item->next       = item->next;
	}

	/* realloc parent references in the case of a node */
	if (axl_item_get_type (item) == ITEM_NODE) {
		if (item->previous == NULL)
			item->parent->first = new_item;

		if (item->next == NULL)
			item->parent->last = new_item;
	} /* end if */

	/* free the item */
	item->next     = NULL;
	item->previous = NULL;

	/* configure values */
	new_item->parent = item->parent;
	new_item->doc    = item->doc;

	if (dealloc) {
		axl_item_free (item, true);

	} /* end if */

	return;

} /* end axl_item_replace */

/** 
 * @brief Allows to transfer all childs contained inside the provided
 * \ref axlNode (old_parent) placed after the provided axlItem
 * (item_ref) on the same level.
 *
 * This function allows to manipulate a xml document loaded inside
 * memory, by transfering all childs (including xml nodes, xml
 * comments, content, process instructions and entity references) from
 * the selected parent (the old parent) provided by the
 * <i>old_parent</i> attribute, to be placed at the same level, where
 * the <i>item_ref</i> is situated following it.
 *
 * 
 * @param old_parent Previous parent, where the childs to be
 * transfered will be found.
 *
 * @param item_ref The \ref axlItem that will act as a reference
 * placing all childs following the item.
 */
void          axl_item_transfer_childs_after (axlNode * old_parent,
					      axlItem * item_ref)
{
	axlItem * item;
	axlItem * item_aux;

	/* get the first child for the old parent */
	item = old_parent->first;

	/* check if the parent node contains childs to be
	 * transferred. If no child is found, just return */
	if (item == NULL)
		return;

	/* remember previous next */
	item_aux       = item_ref->next;
	
	/* make the first child to follow the item ref */
	item_ref->next = item;
	item->previous = item_ref;

	/* set the next at the end of all items transferred, and the
	 * new parent node */
	while (item != NULL) {
		/* configure the new parent */
		item->parent = item_ref->parent;

		/* check the item to be the last */
		if (item->next == NULL) {
			/* the last item was found, configure it */
			item->next = item_aux;

			/* configure it to point to the last item
			 * transferred */
			if (item_aux != NULL)
				item_aux->previous = item;

			/* break the loop! */
			break;
		} /* end if */

		/* get the next */
		item = item->next;
		
	} /* end while */

	/* check that the item selected to be reference isn't the last
	 * child inside new parent node. If it is, update the new
	 * last */
	if (item_aux == NULL) {
		/* because item is pointing to the last item in the
		 * level, use it as the new last */
		item->parent->last = item;
	} /* end if */

	/* clear reference from previous parent */
	old_parent->first     = NULL;
	old_parent->last      = NULL;

	return;
}

/** 
 * @brief Allows to check if both items are equal, considering the
 * item type and the content associated to the item type.
 *
 *
 * @param item The first item to check.  @param item2 The second item
 * to check with the first item.  @param trimmed This paramenter
 * allows to configure how equal checking is performed for content
 * element (\ref ITEM_CONTENT, \ref ITEM_CDATA, \ref ITEM_COMMENT and
 * \ref ITEM_REF).
 *
 * @return \ref true if the both items represents the same
 * information, otherwise \ref false is returned. If the function
 * receives a null value it will return false.
 */
bool          axl_item_are_equal      (axlItem * item,
				       axlItem * item2,
				       bool      trimmed)
{
	axlNodeContent * content;
	axlNodeContent * content2;

	/* trim content */
	char           * trim;
	char           * trim2;
	bool             result;
	
	axl_return_val_if_fail (item, false);
	axl_return_val_if_fail (item2, false);

	/* basic type check */
	if (axl_item_get_type (item) != axl_item_get_type (item2))
		return false;

	/* according the type */
	switch (axl_item_get_type (item)) {
	case ITEM_NODE:
		/* check that both nodes are equal */
		return axl_node_are_equal (item->data, item2->data);
	case ITEM_CONTENT:
	case ITEM_CDATA:
	case ITEM_COMMENT:
	case ITEM_REF:
		/* get the contenet */
		content  = item->data;
		content2 = item2->data;

		if (! trimmed) {
			/* check first content length */
			if (content->content_size != content2->content_size)
				return false;
			
			/* now check content value */
			return axl_cmp (content->content, content2->content);
		}else {
			/* duplicate the content */
			trim = axl_strdup (content->content);
			trim2 = axl_strdup (content2->content);

			/* trim content */
			axl_stream_trim (trim);
			axl_stream_trim (trim2);

			/* do the comparision */
			result = axl_cmp (trim, trim2);

			/* free data */
			axl_free (trim);
			axl_free (trim2);

			return result;

		}
	case ITEM_PI:
		/* pi case */
		return axl_pi_are_equal (item->data, item2->data);
	default:
		/* no case identified, not equal */
		break;
	} /* end switch */

	return false;
}

/** 
 * @brief Allows to release the memory hold the item reference
 * provided, and the value stored inside it.
 * 
 * @param item The item to dealloc.
 *
 * @param dealloc true to also dealloc the value inside. 
 */
void          axl_item_free           (axlItem * item,
				       bool      dealloc)
{
	axl_return_if_fail (item);

	
	/* according the type */
	switch (axl_item_get_type (item)) {
	case ITEM_NODE:
		/* free the node */
		axl_node_free (item->data);
		break;
	case ITEM_CONTENT:
	case ITEM_CDATA:
	case ITEM_COMMENT:
	case ITEM_REF:
		/* all of them, managed equally */

		/* check and free content */
		if ((item->type & ITEM_CONTENT_FROM_FACTORY) == 0) {
			axl_free (((axlNodeContent *)item->data)->content);

			/* free node */
			axl_free ((axlNodeContent *)item->data);
		}
		
		if ((item->type & ITEM_FROM_FACTORY) == 0)
			axl_free (item);
		break;
	case ITEM_PI:
		/* an process instruction */
		axl_pi_free (item->data);
		
		/* free the item */
		if ((item->type & ITEM_FROM_FACTORY) == 0)
			axl_free (item);
		break;
	case ITEM_FROM_FACTORY:
	case ITEM_CONTENT_FROM_FACTORY:
		/* never reached */
		break;
		
	} /* end switch */


	return;
}

axlFactory * axl_item_factory_create ()
{
	return axl_factory_create (sizeof (axlItem));
}

axlItem        * axl_item_factory_get (axlFactory * factory)
{
	return axl_factory_get (factory);
}

axlFactory * axl_node_factory_create ()
{
	return axl_factory_create (sizeof (axlNode));
}

axlNode    * axl_node_factory_get (axlFactory * factory)
{
	/* get a node */
	axlNode * node = axl_factory_get (factory);
	
	/* configure it */
	node->conf = NODE_FROM_FACTORY;

	return node;
}

axlFactory     * axl_item_content_factory_create ()
{
	/* create a factory for axlNodeContent elements */
	return axl_factory_create (sizeof (axlNodeContent));
}

axlFactory     * axl_item_attr_factory_create ()
{
	/* create a factory for axlNodeAttr elements */
	return axl_factory_create (sizeof (axlNodeAttr));
}

/* @} */
