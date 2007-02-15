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
#ifndef __AXL_NODE_H__
#define __AXL_NODE_H__

#include <axl_decl.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_node_module
 * @{
 */

axlNode * axl_node_create                   (const char * name);

axlNode * axl_node_create_ref               (char * name);

void      axl_node_set_name                 (axlNode * node, 
					     const char * name);

void      axl_node_set_name_ref             (axlNode * node, 
					     char * name);

void      axl_node_set_name_from_factory    (axlNode * node, 
					     char * name);

axlNode * axl_node_copy                     (axlNode * node,
					     bool      copy_attributes,
					     bool      copy_childs);

axlDoc  * axl_node_get_doc                  (axlNode * node);

void      axl_node_set_doc                  (axlNode * node, 
					     axlDoc  * doc);

void      axl_node_set_attribute            (axlNode    * node, 
					     const char * attribute, 
					     const char * value);

void      axl_node_set_attribute_ref        (axlNode * node, 
					     char    * attribute, 
					     char    * value);

void      axl_node_set_attribute_from_factory  (axlFactory * factory, 
						axlNode    * node, 
						char       * attribute, 
						char       * value);

/** 
 * @brief Macro definition which works as an alias for \ref
 * axl_node_get_attribute_value.
 * 
 * @param node The node being checked to have an attribute value.
 * @param attr The attribute that is being check to be present in the node provided.
 * 
 * @return \ref true if the node has the provided attribute, otherwise
 * \ref false is returned.
 */
#define HAS_ATTR(node,attr) axl_node_has_attribute(node,attr)

bool      axl_node_has_attribute            (axlNode    * node, 
					     const char * attribute);

/** 
 * @brief Macro definition, which works as an alias for \ref
 * axl_node_get_attribute_value.
 * 
 * @param node The node that is being required to return the content
 * of a particular node.
 *
 * @param attr The attribute that is requested.
 * 
 * @return The value assocaited to the attribute value or null if it
 * fails. See \ref axl_node_get_attribute_value.
 */
#define ATTR_VALUE(node,attr) axl_node_get_attribute_value(node, attr)

const char    * axl_node_get_attribute_value      (axlNode    * node, 
						   const char * attribute);

char    * axl_node_get_attribute_value_copy (axlNode    * node, 
					     const char * attribute);

/** 
 * @brief Simple alias definition to \ref axl_node_get_attribute_value_trans function.
 * 
 * @param node The node which contains the attribute requested.
 *
 * @param attr The attribute key that is being requested.
 * 
 * @return A newly allocated string containing the attribute value,
 * with known entity references translated.
 */
#define ATTR_VALUE_TRANS(node,attr) axl_node_get_attribute_value_trans(node,attr)

char    * axl_node_get_attribute_value_trans (axlNode * node, 
					      const char * attribute);

/** 
 * @brief Allows to get the value associated to the attributed
 * provided, inside the node selected, removing trailing and ending
 * white spaces (in the W3C sence: \\n, \\t, \\r, ' ').
 *
 * This is a macro alias to \ref axl_node_get_attribute_value_trimmed.
 * 
 * @param node The node that is requested to return the associated 
 * value to the attributed.
 *
 * @param attr The attribute that is being requested.
 * 
 * @return A reference to the attribute value or NULL if it fails.
 */
#define ATTR_VALUE_TRIMMED(node,attr) axl_node_get_attribute_value_trimmed(node,attr)

const char    * axl_node_get_attribute_value_trimmed (axlNode    * node,
						      const char * attribute);

/**
 * @brief Convenience macro wrapping \ref
 * axl_node_has_attribute_value.
 */
#define HAS_ATTR_VALUE(node, attr,value) (axl_node_has_attribute_value (node, attr, value))

bool      axl_node_has_attribute_value       (axlNode    * node, 
					      const char * attribute, 
					      const char * value);

bool      axl_node_has_attributes (axlNode * node);

/**
 * @}
 */

/** 
 * \addtogroup axl_node_attribute_cursor
 * @{
 */

/* XML node attribute iteration API provided to traverse all
 * attributes without knowing them. */
axlAttrCursor * axl_node_attr_cursor_new       (axlNode * node);

void            axl_node_attr_cursor_first     (axlAttrCursor * cursor);

void            axl_node_attr_cursor_next      (axlAttrCursor * cursor);

bool            axl_node_attr_cursor_has_next  (axlAttrCursor * cursor);

bool            axl_node_attr_cursor_has_item  (axlAttrCursor * cursor);

const char *    axl_node_attr_cursor_get_key   (axlAttrCursor * cursor);

const char *    axl_node_attr_cursor_get_value (axlAttrCursor * cursor);

void            axl_node_attr_cursor_free      (axlAttrCursor * cursor);

/**
 * @}
 */

/** 
 * \addtogroup axl_node_annotate
 * @{
 */

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_data.
 */
#define axl_node_annotate_data axl_node_anotate_data

void      axl_node_anotate_data                 (axlNode     * node, 
						 const char  * key, 
						 axlPointer    data);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_data_full.
 */
#define axl_node_annotate_data_full axl_node_anotate_data_full

void      axl_node_anotate_data_full            (axlNode       * node,
						 const char    * key, 
						 axlDestroyFunc  key_destroy,
						 axlPointer      data,
						 axlDestroyFunc  data_destroy);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_get.
 */
#define axl_node_annotate_get axl_node_anotate_get
axlPointer axl_node_anotate_get                 (axlNode       * node,
						 const char    * key,
						 bool            lookup_in_parent);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_int.
 */
#define axl_node_annotate_int axl_node_anotate_int
void       axl_node_anotate_int                 (axlNode * node,
						 const char    * key,
						 int             int_value);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_string.
 */
#define axl_node_annotate_string axl_node_anotate_string
void       axl_node_anotate_string              (axlNode * node,
						 const char    * key,
						 const char    * string_value);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_double.
 */
#define axl_node_annotate_double axl_node_anotate_double
void       axl_node_anotate_double              (axlNode    * node,
						 const char * key,
						 double       double_value);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_get_int.
 */
#define axl_node_annotate_get_int axl_node_anotate_get_int
int        axl_node_anotate_get_int             (axlNode     * node,
						 const char  * key,
						 bool          lookup_in_parent);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_get_string.
 */
#define axl_node_annotate_get_string axl_node_anotate_get_string
char *     axl_node_anotate_get_string          (axlNode       * node,
						 const char    * key,
						 bool            lookup_in_parent);

/** 
 * @brief Simple alias to fix the interface problem for the annotate
 * API that includes only one "n". See \ref axl_node_anotate_get_double.
 */
#define axl_node_annotate_get_double axl_node_anotate_get_double
double     axl_node_anotate_get_double          (axlNode       * node,
						 const char    * key,
						 bool            lookup_in_parent);

/**
 * @}
 */


/** 
 * \addtogroup axl_node_module
 * @{
 */

/** 
 * @brief Allows to easily check that the given xml node (\ref
 * axlNode) have the provided name.
 *
 * This macro is provided as a convenience to check that a particular
 * node have a particular name. Check the documentation that this
 * macro is wrapping to get examples: \ref axl_node_get_name.
 *
 * <i><b>NOTE:</b> This function isn't XML Namespace aware. You must use \ref axl_ns_node_cmp instead. See \ref axl_ns_doc_validate. </i>
 * 
 * @param node The \ref axlNode where the name will be compared with
 * the provided value.
 *
 * @param name The name to compare.
 * 
 * @return \ref true if names are equal or \ref false if not.
 */
#define NODE_CMP_NAME(node,name) (axl_cmp ((node != NULL) ? axl_node_get_name(node) : "><", (name != NULL) ? name : "<>"))

const char * axl_node_get_name           (axlNode * node);

axlNode * axl_node_get_parent         (axlNode * node);

axlNode * axl_node_get_next           (axlNode * node);

axlNode * axl_node_get_next_called    (axlNode * node, 
				       char    * name);

axlNode * axl_node_get_previous         (axlNode * node);

axlNode * axl_node_get_previous_called  (axlNode * node, 
					 char    * name);

axlNode * axl_node_get_first_child    (axlNode * node);

axlNode * axl_node_get_last_child     (axlNode * node);

void      axl_node_set_child          (axlNode * parent, 
				       axlNode * child);

void      axl_node_replace            (axlNode * node, 
				       axlNode * new_node,
				       bool      dealloc);

void      axl_node_remove             (axlNode * node,
				       bool      dealloc);

void      axl_node_deattach           (axlNode * node);
				       

void      axl_node_set_is_empty       (axlNode  * node, 
				       bool       empty);

bool      axl_node_is_empty           (axlNode * node);

const char    * axl_node_get_content  (axlNode * node, 
				       int  * content_size);

char    * axl_node_get_content_copy   (axlNode * node, 
				       int  * content_size);

char    * axl_node_get_content_trans  (axlNode * node, 
				       int * content_size);

char    * axl_node_get_content_trim   (axlNode * node,
				       int * content_size);

void      axl_node_set_content        (axlNode    * node, 
				       const char * content, 
				       int content_size);

void      axl_node_set_content_ref    (axlNode * node, 
				       char * content, 
				       int content_size);

void      axl_node_set_content_from_factory (axlFactory * factory,
					     axlNode    * node,
					     char       * content,
					     int          content_size);

void      axl_node_set_cdata_content  (axlNode * node,
				       char * content,
				       int content_size);

void      axl_node_set_cdata_content_from_factory (axlFactory * factory,
						   axlNode    * node,
						   char       * content,
						   int          content_size);

void      axl_node_set_comment        (axlNode * node,
				       char * comment,
				       int    comment_size);

void      axl_node_set_have_childs    (axlNode * node, bool     childs);

bool      axl_node_have_childs        (axlNode * node);

axlNode * axl_node_get_child_called   (axlNode * parent, char * name);

axlNode * axl_node_find_called        (axlNode * parent, char * name);

axlNode * axl_node_get_child_nth      (axlNode * parent, int position);

int       axl_node_get_child_num      (axlNode * parent);

axlList * axl_node_get_childs         (axlNode * node);

bool      axl_node_are_equal          (axlNode * node, axlNode * node2);

void      axl_node_add_pi_target            (axlNode * node, 
					     char * target, 
					     char * content);

bool      axl_node_has_pi_target            (axlNode * node, 
					     char * pi_target);

char    * axl_node_get_pi_target_content    (axlNode * node, 
					     char * pi_target);

axlList * axl_node_get_pi_target_list       (axlNode * node);

void      axl_node_transfer_childs          (axlNode * old_parent, 
					     axlNode * new_parent);

int       axl_node_get_flat_size            (axlNode * node,
					     bool      pretty_print,
					     int       level,
					     int       tabular);

int       axl_node_dump_at                  (axlNode * node,
					     char    * content,
					     int       desp,
					     bool      pretty_print,
					     int       level,
					     int       tabular);

bool      axl_node_has_invalid_chars        (const char * content,
					     int    content_size,
					     int  * added_size);

char    * axl_node_content_copy_and_escape (const char * content, 
					    int          content_size, 
					    int          additional_size);

void      axl_node_free            (axlNode * node);

void      axl_node_free_full       (axlNode * node, bool also_childs);

/* @} */

/* item interface */

axlItem     * axl_item_new             (AxlItemType type,
					axlPointer  data);

axlItem     * axl_item_new_ref         (AxlItemType type,
					axlPointer  data);

axlDoc      * axl_item_get_doc         (axlItem * item);

void          axl_item_set_doc         (axlItem * item, axlDoc * doc);

axlNode     * axl_item_get_parent      (axlItem * item);

axlItem     * axl_item_get_next        (axlItem * item);

axlItem     * axl_item_node_next       (axlNode * node);

axlItem     * axl_item_get_previous    (axlItem * item);

axlItem     * axl_item_node_previous   (axlNode * node);

axlItem     * axl_item_node_holder     (axlNode * node);

axlItem     * axl_item_get_first_child (axlNode * node);

axlItem     * axl_item_get_last_child  (axlNode * node);

AxlItemType   axl_item_get_type        (axlItem * item);

axlPointer    axl_item_get_data        (axlItem * item);

char        * axl_item_get_content     (axlItem * item, 
					int * size);

void          axl_item_set_child       (axlNode * parent, 
					AxlItemType type, 
					axlPointer data);

void          axl_item_set_child_ref   (axlNode * parent, 
					axlItem * item);

void          axl_item_set_after       (axlItem * item,
					AxlItemType type,
					axlPointer data);

axlItem     * axl_item_copy            (axlItem * item,
					axlNode * set_parent);

void          axl_item_remove          (axlItem * item,
					bool      dealloc);

void          axl_item_replace         (axlItem * item, 
					axlItem * new_item,
					bool      dealloc);

void          axl_item_transfer_childs_after (axlNode * old_parent,
					      axlItem * item_ref);

bool          axl_item_are_equal      (axlItem * item,
				       axlItem * item2,
				       bool      trimmed);

void          axl_item_free           (axlItem * item,
				       bool      dealloc);

axlFactory     * axl_item_factory_create ();

axlItem        * axl_item_factory_get (axlFactory * factory);

axlFactory     * axl_node_factory_create ();

axlNode        * axl_node_factory_get (axlFactory * factory);

axlFactory     * axl_item_content_factory_create ();

axlFactory     * axl_item_attr_factory_create ();

END_C_DECLS

#endif


