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
#ifndef __AXL_DECL_H__
#define __AXL_DECL_H__

/* include platform specific configuration */
#include <axl_config.h>

/* include this at this place to load GNU extensions */
#if defined(__GNUC__)
#  ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#  endif
#  define __AXL_PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#  define __AXL_LINE__            __LINE__
#  define __AXL_FILE__            __FILE__
#else
/* non gnu compiler */
#define __AXL_PRETTY_FUNCTION__ ""
#define __AXL_LINE__            ""
#define __AXL_FILE__            ""
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* only include unistd.h if unix platform is found or gnu gcc compiler
 * is found */
#if defined(__GNUC__) || defined(AXL_OS_UNIX)
# include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

/**
 * \defgroup axl_decl_module Axl Declarations: Common Axl declarations, Types, macros, and support functions.
 */

/** 
 * \addtogroup axl_decl_module
 * @{
 */


/**
 * @brief Axl XML document type definition.
 *
 * This type represents a reference to an entiry XML document loaded
 * into memory. Functions to be used to load XML document could be the
 * following:
 * 
 *  - \ref axl_doc_parse
 *  - \ref axl_doc_parse_strings
 *  - \ref axl_doc_parse_from_file
 * 
 * You can also create an empty document by using \ref axl_doc_create
 * and fill the initial root node using the \ref axl_doc_set_root
 * function. Once the document have an initial root node, you can add
 * more nodes as childs to the root element previously added.
 * 
 * Check the \ref axl_doc_module "axlDoc API reference" to get more
 * information.
 */
typedef struct _axlDoc axlDoc;

/** 
 * @brief An abstraction that allows to hold an xml node or any other
 * content that can be found inside an xml node: comments, content,
 * CDATA-content, PI, entity references.
 *
 * This type is mainly used inside the MIXED API, which allows to get
 * access to every particular item found inside a particular node
 * (\ref axlNode). Every item has a type (\ref AxlItemType) and a
 * content that is being encapsulated and usually accessed by \ref
 * axl_item_get_data.
 *
 * Check the \ref axl_item_module "Axl Item interface" for more information.
 */
typedef struct _axlItem axlItem;

/** 
 * @internal Factory allocation type.
 */
typedef struct _axlFactory axlFactory;

/** 
 * @internal String factory allocation type.
 */
typedef struct _axlStrFactory axlStrFactory;

/**
 * @brief Axl XML node type definition.
 *
 * This type reference represents a single XML node. To create a xml node you can use:
 * 
 * - \ref axl_node_create
 * 
 * To get the node name or if the node have childs or if it is empty,
 * you can use the following functions:
 *
 * - \ref axl_node_get_name
 * - \ref axl_node_is_empty
 * - \ref axl_node_have_childs
 *
 * To get the \ref axlNode content you can use the following
 * functions:
 * 
 * - \ref axl_node_get_content
 * - \ref axl_node_get_content_copy
 * - \ref axl_node_get_content_trans
 *
 * For attributes manipulation, you can using the following functions
 * to set them and retrieve them:
 * 
 * - \ref axl_node_get_attribute_value
 * - \ref axl_node_get_attribute_value_copy
 * - \ref axl_node_get_attribute_value_trans
 *
 * To retrive childs number or childs inside the given \ref axlNode
 * you can use the following function:
 * 
 * - \ref axl_node_get_child_num
 * - \ref axl_node_get_child_nth
 *
 * You can also use the following functions to get the parent node for
 * a provided xml node child and the next xml node found at the same
 * level, for a provided xml node reference.
 *
 * - \ref axl_node_get_parent
 * - \ref axl_node_get_next
 * 
 * Check the axlNode \ref axl_node_module "API for more information".
 */
typedef struct _axlNode axlNode;

/** 
 * @brief Public cursor type used to iterate over attributes installed
 * on a particular node (see \ref axl_node_attr_cursor_new to get examples and more information).
 */
typedef struct _axlAttrCursor axlAttrCursor;

/** 
 * @brief Axl DTD entity representation.
 */
typedef struct _axlDtd        axlDtd;

/** 
 * @brief Axl DTD entity element declaration.
 */
typedef struct _axlDtdElement axlDtdElement;

/** 
 * @brief Axl DTD item list element declaration.
 */
typedef struct _axlDtdElementList axlDtdElementList;

/** 
 * @brief Axl DTD item list element declaration.
 */
typedef struct _axlDtdElementListNode axlDtdElementListNode;


/** 
 * @brief Axl DTD attribute declaration (<!ATTLIST..>)
 */
typedef struct _axlDtdAttribute axlDtdAttribute;

/** 
 * @brief Axl DTD attribute list decleration inside \ref axlDtdAttribute.
 */
typedef struct _axlDtdAttributeDecl axlDtdAttributeDecl;

/** 
 * @brief Axl DTD entity declaration (<!ENTITY support>)
 */
typedef struct _axlDtdEntity axlDtdEntity;

/** 
 * @brief Support type definition for \ref axlDtdEntity, which holds
 * information about external resource pointed by the \ref
 * axlDtdEntity instance.
 */
typedef struct _axlDtdEntityExternalData axlDtdEntityExternalData;

/** 
 * @brief Declares the entity type for a provided \ref axlDtdEntity.
 */
typedef enum {
	/** 
	 * @brief The \ref axlDtdEntity definition represents a
	 * general entity definition (that comes without % before the
	 * entity name).
	 */
	GENERAL_ENTITY,
	/** 
	 * @brief The \ref axlDtdEntity definition represents a
	 * parameter entity definition (that comes with a % before the
	 * entity name, making this entity definition to be only
	 * usable from a DTD definition).
	 */
	PARAMETER_ENTITY,
}axlDtdEntityType;

/** 
 * @brief Attribute type declaration (the type of the attribute
 * constrain). This type is used to identifier the attribute contains
 * applied to the node selected.
 */
typedef enum { 
	/** 
	 * @brief The attribute type is defined but its content is
	 * CDATA (any string value is allowed), activated when used 'CDATA'.
	 */
	CDATA_ATTRIBUTE, 
	/** 
	 * @brief Especific token declaration that implicitly contrain
	 * the node attribute value, activated when used 'ID'.
	 */
	TOKENIZED_TYPE_ID, 
	/** 
	 * @brief Especific token declaration that implicitly contrain
	 * the node attribute value, activated when used 'IDREF'.
	 */
	TOKENIZED_TYPE_IDREF,
	/** 
	 * @brief Especific token declaration that implicitly contrain
	 * the node attribute value, activated when used 'IDREFS'.
	 */
	TOKENIZED_TYPE_IDREFS,
	/** 
	 * @brief Especific token declaration that implicitly contrain
	 * the node attribute value, activated when used 'ENTITY'.
	 */
	TOKENIZED_TYPE_ENTITY,
	/** 
	 * @brief Especific token declaration that implicitly contrain
	 * the node attribute value, activated when used 'ENTITIES'.
	 */
	TOKENIZED_TYPE_ENTITIES,
	/** 
	 * @brief Especific token declaration that implicitly contrain
	 * the node attribute value, activated when used 'NMTOKEN'.
	 */
	TOKENIZED_TYPE_NMTOKEN,
	/** 
	 * @brief Especific token declaration that implicitly contrain
	 * the node attribute value, activated when used 'NMTOKENS'.
	 */
	TOKENIZED_TYPE_NMTOKENS,
	/** 
	 * @brief The attribute type declaration is constrained to a
	 * set of values. This values are considered as an
	 * enumeration.
	 */
	ENUMERATION_TYPE, 
	/** 
	 * @brief Attribute type not supported yet (although defined).
	 */
	NOTATION_TYPE
} AxlDtdAttributeType;

/** 
 * @brief Defines the DTD attribute declaration default state.
 */
typedef enum {
	/** 
	 * @brief The attribute is required as especified by the
	 * attribute declaration.
	 */
	ATT_REQUIRED,
	/** 
	 * @brief The attribute is not requried, however, if it
	 * appears it must follow the attribute type declaration.
	 */
	ATT_IMPLIED,
	/** 
	 * @brief The attribute must appear and have the value
	 * provided as the default.
	 */
	ATT_FIXED
}AxlDtdAttributeDefaults;


/** 
 * @brief The type of the DTD sequences stored by the \ref
 * axlDtdElementList.
 */
typedef enum {
	/** 
	 * @internal
	 *
	 * Internal value used by the library to support mixing
	 * content seperator types detection.
	 */
	STILL_UNDEF  = 0,
	/** 
	 * @brief Represents that the item selection is configured to
	 * be a choice (a selection allowed from any item inside the
	 * collection).
	 */
	CHOICE        = 1,
	/** 
	 * @brief Represents that the item selection is configured to
	 * select each item in the order they apper.
	 */
	SEQUENCE      = 2
}AxlDtdNestedType;

/** 
 * @brief An indication of the element type stored on the provided
 * \ref axlDtdElementListNode reference.
 *
 * An \ref axlDtdElementListNode reference could contain a single
 * reference to a content particule name, that is the XML node name to
 * be allowed to be used at the provided position or a reference to an
 * \ref axlDtdElementList which contains a nested list containing more
 * content particules.
 * 
 */
typedef enum {
	/** 
	 * @internal
	 * Represents the not defined value.
	 */
	AXL_ELEMENT_NOT_DEFINED = 1,
	/** 
	 * @brief The reference contains an \ref axlDtdElementList.
	 */
	AXL_ELEMENT_LIST = 2,
	/** 
	 * @brief The reference contains a reference to a leaf node, a
	 * content particule.
	 */
	AXL_ELEMENT_NODE = 3
} NodeType;

/** 
 * @brief DTD element type enumeration.
 *
 * While using DTD declaration, <b>ELEMENT</b> used to define how your
 * xml document is structured and constrained, are clasified using the
 * following values.
 *
 * This type specification must not be confused with \ref NodeType,
 * which is the configuration for an element, inside the content DTD
 * element specification.
 */
typedef enum {
	/** 
	 * @internal
	 *
	 * Internal value to avoid confusing EMPTY declaration with a
	 * non-defined value.
	 */
	ELEMENT_TYPE_UNKNOWN = 0,
	/** 
	 * @brief Used to represent that the element declaration have
	 * no content inside it. This includes not only PCDATA (data
	 * stored between xml tags) but also any child declaration.
	 */
	ELEMENT_TYPE_EMPTY = 1,
	/** 
	 * @brief Used to represent that the element used in your xml
	 * document could contain anthing without any contraint. 
	 */
	ELEMENT_TYPE_ANY = 2,
	/** 
	 * @brief Used to represent that the following xml node have
	 * content not only defined by a set of allowed xml nodes but
	 * also PCDATA.
	 */
	ELEMENT_TYPE_MIXED = 3,
	/** 
	 * @brief Used to represent that the folowing xml node have
	 * only xml nodes as content, in the form of xml childs,
	 * without inlucing PCDATA.
	 */
	ELEMENT_TYPE_CHILDREN = 4,
	/** 
	 * @brief Used to represent that the DTD element specification
	 * contains only PCDATA. No child nodes or childs nodes mixed
	 * with PCDATA.
	 */
	ELEMENT_TYPE_PCDATA = 5
} AxlDtdElementType;

/** 
 * @brief Current configuration for elements definied inside a ELEMENT
 * DTD declaration.
 */
typedef enum {
	/** 
	 * @internal 
	 *
	 * Internal representation to identify wrong repetition
	 * especification values.
	 */
	DTD_TIMES_UNKNOWN = 0,
	/** 
	 * Current configuration for DTD element content specification
	 * signals that it must appear one and only one times.
	 */
	ONE_AND_ONLY_ONE  = 1,
	/** 
	 * Current configuration for DTD element content specification
	 * signals that it could appear zero or one time.
	 */
	ZERO_OR_ONE       = 2,
	/** 
	 * Current configuration for DTD element content specification
	 * signals 
	 */
	ZERO_OR_MANY      = 3,
	/** 
	 * Current configuration for DTD element content specification
	 * signals that it must appear one up to many times.
	 */
	ONE_OR_MANY       = 4
}AxlDtdTimes;

/** 
 * @brief Item types that can hold an xml node (\ref axlNode).
 *
 * \ref AxlItemType is used to notify the type for a particular item
 * (\ref axlItem) that is stored as a child on a particular \ref
 * axlNode.
 *
 * This is mainly used inside the MIXED API, which is the way that Axl
 * exposes the content of a xml node that is expected to contain more
 * nodes mixed with more content.
 *
 * Each type represents a particular basic unit that could be found as
 * a child item inside an xml node.
 */
typedef enum {
	/** 
	 * @brief The \ref axlItem is encapsulating another item
	 * node. Calling to \ref axl_item_get_data will return a
	 * reference to an \ref axlNode.
	 *
	 */
	ITEM_NODE      = 1 << 0,
	/** 
	 * @brief The \ref axlItem is encapsulating an node
	 * content. Calling to the convenience function \ref
	 * axl_item_get_content to get the content and the size that
	 * is stored in the \ref axlItem with this type.
	 */
	ITEM_CONTENT   = 1 << 1,
	/** 
	 * @brief The \ref axlItem is encapsulating an application
	 * process instruction. Calling to \ref axl_item_get_data will
	 * return a reference to a \ref axlPI.
	 *
	 */
	ITEM_PI        = 1 << 2,
	
	/** 
	 * @brief The \ref axlItem is encapsulating an xml
	 * comment. 
	 * 
	 * XML comments inside Axl are handled xml node content. The
	 * comment that is returned from the function has the initial
	 * '<!--' and the ending '-->' elements stripped from its
	 * body.
	 *
	 * You must use the convenience function \ref
	 * axl_item_get_content to get the comment content and the
	 * size that is stored on the \ref axlItem.
	 */
	ITEM_COMMENT   = 1 << 3,
	/** 
	 * @brief The \ref axlItem is encapsulating an xml entity
	 * reference that wasn't resolved yet.
	 *
	 * Not implemented yet.
	 */
	ITEM_REF       = 1 << 4,
	/** 
	 * @brief The \ref axlItem is encapsulating an xml content
	 * that was enclosed using the <![CDATA[]]> construction. This
	 * child item works the same as \ref ITEM_CONTENT but, adding
	 * the CDATA semantic and the fact that the content wasn't
	 * parsed by the Axl XML engine. 
	 *
	 * You must use the convenience function \ref
	 * axl_item_get_content to get the CDATA content and the size
	 * that is stored on the \ref axlItem.
	 *
	 */
	ITEM_CDATA     = 1 << 6,

	/** 
	 * @internal Item type which allows to signal that the item
	 * comes from an item factory and shouldn't be deallocated in
	 * the usual manner.
	 */
	ITEM_FROM_FACTORY = 1 << 7,
	/** 
	 * @internal Item type which allows to signal that the content
	 * item comes from an item factory and shouldn't be
	 * deallocated in the usual manner. 
	 */
	ITEM_CONTENT_FROM_FACTORY = 1 << 8
}AxlItemType;

/** 
 * @brief Simple alias for the AxlDtdElementType.
 */
typedef AxlDtdElementType ElementType;

/** 
 * @brief Axl DTD entity attribute element declaration.
 */
typedef struct _axtDtdAttr    axlDtdAttr;

/** 
 * @brief Axl Processing instruction type definition.
 */
typedef struct _axlPI        axlPI;

/** 
 * @brief Axl error reporting variable.
 *
 * All Axl interface report errors found, with a textual diagnostic,
 * to the application level using this variable. You have to know that
 * it is also optional, so every function that receives an \ref
 * axlError, will properly handle a NULL reference received.
 *
 * Once an error was detected, for that condition you must check the
 * documentation provided for the function that is failing, you can
 * get the error code and the error textual diagnostic by using the
 * following functions:
 *
 *  - \ref axl_error_get_code
 *  - \ref axl_error_get
 *
 * If an error is not detected, there is no especial operation to be
 * done once returned the function that has received the \ref axlError
 * error reference. However, if an error is detected, the reference
 * must be deallocated by using the following function:
 *  
 *  - \ref axl_error_free
 * 
 * Here is an example:
 * \code
 * // declare the axlError reference
 * axlError * error;
 *
 * // parse the document, giving a reference to the axlError
 * // NOTE: you can safely provide a NULL reference.
 * doc = axl_doc_parse_from_file ("test.xml", &error);
 * if (doc == NULL) {
 *     printf ("Parse error: code=%d, message=%s\n", 
 *             axl_error_get_code (error), axl_error_get (error));
 *     axl_error_free (error);
 *     return false;
 * }
 *
 * // beyond this point, it is not required to do
 * // any especial task with the axlError reference
 * \endcode
 *
 * To get more information about the \ref axlError check its \ref axl_error_module "API documentation".
 */
typedef struct _axlError  axlError;

/** 
 * @brief Axl Stream representation (an abstraction API to manage
 * source of data with convenience functions).
 */
typedef struct _axlStream axlStream;

/** 
 * @brief (DEPRECATED use \ref true) Type definition to represent a
 * boolean true value, that is equal to 1.
 */
#define AXL_TRUE  (1)

/** 
 * @brief (DEPRECATED use \ref false) Type definition to represent a
 * boolean false value, that is equal to 0.
 */
#define AXL_FALSE (0)

/** 
 * @brief Alias declaration to bind the <i>int</i> to the <b>boolean</b>
 * concept (TRUE / FALSE states) (DEPRECATED).
 *
 * This is mainly used to emphasize that some integer values that
 * returns some function must be considered to be \ref true or \ref
 * false, that represents the boolean TRUE and FALSE values.
 *
 * This allows to perform boolean comparations using structure
 * controls like if, while, but also making a differenciation about
 * the boolean values and integer values.
 *
 * You are also allowed to use <b>bool</b> as boolean type definition.
 */
typedef int aboolean;

/**
 * @internal
 * 
 * Internal definition to retain compatibility with programs that
 * define the bool value but the compiler doesn't provide it.
 */
#ifndef __cplusplus
typedef int bool;

/** 
 * @brief Common definition to have false value (which is defined to 0
 * integer value).
 */
#define false (0)
/** 
 * @brief Common definition to have true value (which is defined to 1
 * integer value).
 */
#define true  (1)
#endif /* __cplusplus */

/** 
 * @brief Pointer to any structure definition. It should be required
 * to use this definition, however, some platforms doesn't support the
 * <b>void *</b> making it necessary to use the <b>char *</b>
 * definition as a general way to represent references.
 */
typedef void * axlPointer;

/** 
 * @brief \ref axlList definition, a list implementation.
 * See \ref axl_list_new for more information about using this type.
 */
typedef struct _axlList axlList;

/** 
 * @brief \ref axlListCursor definition, a iterator type used to
 * traverse an axlList in a efficient way. See \ref axl_list_cursor_get.
 */
typedef struct _axlListCursor axlListCursor;

/** 
 * @brief \ref axlStack definitinon, a stack implementation on top of \ref
 * axlList.
 * See \ref axl_stack_new for more information about using this type.
 */
typedef struct _axlStack axlStack;

/** 
 * @brief \ref axlHash definition, a hash table to store key indexed
 * values.
 * See \ref axl_hash_new for more information about using this type.
 */
typedef struct _axlHash  axlHash;

/** 
 * @brief \ref axlHashCursor definition, a support type that is used
 * to iterate a hash in a linear mode, without calling to \ref
 * axl_hash_foreach family functions (see \ref axl_hash_cursor_new).
 */
typedef struct _axlHashCursor axlHashCursor;

/** 
 * @brief Handler definition used to compare two elements.
 * 
 * In the case they are equal, 0 is returned. In the case a should be
 * before b the -1 is returned. In the case a should be after b then 1
 * should be returned. A properly configured handler should help
 * collections and other function to order elements.
 *
 * @param a The element to compare
 * @param b The other element to compare
 * 
 * @return A value selected from {-1,0,1} according to previous
 * description.
 */
typedef int (*axlEqualFunc) (axlPointer a, axlPointer b);

/** 
 * @brief Handler definition which represent deallocation functions.
 *
 * @param ptr The pointer to the memory to be released.
 */
typedef void (*axlDestroyFunc) (axlPointer ptr);

/** 
 * @brief Handler used to represent the set of functions that could be
 * used to configure the axl stream allocation method. See \ref
 * axl_stream_set_buffer_alloc.
 * 
 * @param size The amount of memory to be allocated (memory requested by the axl stream).
 * 
 * @param data User defined pointer configured at \ref axl_stream_set_buffer_alloc.
 * 
 * @return The handler must return newly allocated memory to hold <b>size</b>
 * bytes.
 */
typedef char * (*axlStreamAlloc) (int size, axlPointer data);

/** 
 * @brief Axl debug levels.
 * 
 * While reporting log to the console, these levels are used to report
 * the severity for such log.
 */
typedef enum {
	/** 
	 * @brief Debug level. Only used to report common
	 * circumstances that represent the proper functionality.
	 */
	AXL_LEVEL_DEBUG, 
	/** 
	 * @brief Warning level. Only used to report that an internal
	 * issue have happend that could be interesting while
	 * reporting error, but it could also mean common situations.
	 */
	AXL_LEVEL_WARNING, 
	/** 
	 * @brief Critical level. Only used to report critical
	 * situations where some that have happened shouldn't. 
	 *
	 * This level should only be used while reporting critical
	 * situations.
	 */
	AXL_LEVEL_CRITICAL}  
AxlDebugLevel;

/** 
 * @brief Calloc helper for axl library.
 *
 * @param count How many items to allocate.
 * @param size Size of one item.
 * 
 * @return A newly allocated pointer.
 * @see axl_free
 */
axlPointer  axl_calloc(size_t count, size_t size);

/** 
 * @brief Realloc helper for axl library.
 *
 * @param ref the reference to reallocate.
 * @param size Size of the new reference.
 * 
 * @return A newly allocated pointer.
 * @see axl_free
 */
axlPointer  axl_realloc(axlPointer ref, size_t size);

/** 
 * @brief Support macro to allocate memory using axl_calloc function,
 * making a casting and using the sizeof keyword.
 *
 * @param type The type to allocate
 * @param count How many items to allocate.
 * 
 * @return A newly allocated pointer.
 */
#define axl_new(type, count) (type *) axl_calloc (count, sizeof (type))

/** 
 * @brief Free the provided pointer that was allocated using \ref
 * axl_new.
 * 
 * @param ref The reference to deallocate.
 */
void    axl_free(axlPointer ref);

/** 
 * @brief Allows to check a condition and return if it is not meet.
 * 
 * @param expr The expresion to check.
 */
#define axl_return_if_fail(expr) \
if (!(expr)) {__axl_log ("", AXL_LEVEL_CRITICAL, "Expresion '%s' have failed at %s (%s:%d)", #expr, __AXL_PRETTY_FUNCTION__, __AXL_FILE__, __AXL_LINE__); return;}

/** 
 * @brief Allows to check a condition and return the given value if it
 * is not meet.
 * 
 * @param expr The expresion to check.
 *
 * @param val The value to return if the expression is not meet.
 */
#define axl_return_val_if_fail(expr, val) \
if (!(expr)) { __axl_log ("", AXL_LEVEL_CRITICAL, "Expresion '%s' have failed, returning: %s at %s (%s:%d)", #expr, #val, __AXL_PRETTY_FUNCTION__, __AXL_FILE__, __AXL_LINE__); return val;}


/** 
 * @brief Consumes all spaces found and tabulars on the given stream
 * until a different char is found.
 *
 * This internal function also consumes coments inside the xml read.
 * 
 * @param stream The stream where the operation will be performed.
 */
#define AXL_CONSUME_SPACES(stream) \
axl_stream_consume_white_spaces (stream)

/** 
 * @internal
 *
 * @brief Allows to check if the provided string is empty either
 * because it is NULL of because the string contains no data.
 * 
 * @param str The string to check for emptyness.
 * 
 * @return Returns true if the string is empty and false if
 * not.
 */
#define AXL_IS_STR_EMPTY(str) (((str == NULL) || strlen (str) == 0) ? true : false)

/** 
 * @internal
 *
 * C++ support declarations borrowed from the libtool webpage. Thanks
 * you guys for this information. 
 *
 * BEGIN_C_DECLS should be used at the beginning of your declarations,
 * so that C++ compilers don't mangle their names.  Use END_C_DECLS at
 * the end of C declarations.
 */
#undef BEGIN_C_DECLS
#undef END_C_DECLS
#ifdef __cplusplus
# define BEGIN_C_DECLS extern "C" {
# define END_C_DECLS }
#else
# define BEGIN_C_DECLS /* empty */
# define END_C_DECLS /* empty */
#endif

#define _memcmp(i,s1,s2,size)\
i = 0;\
while (s1 [i] != 0 && s2 [i] != 0) {\
     if (s1 [i] != s2 [i])\
	return false;\
     i++;\
     if (i == size)\
	return true;\
}\
return false

/** 
 * @brief Allows to configure how is performed the iteration other the xml document.
 *
 * An xml document could be considered as a tree structure, where the
 * root document node is the root of the tree. This enumerator allows
 * to configure how is visited each node of the tree. 
 */
typedef enum {
	/** 
	 * @brief Makes a deep iteration, visiting first childs of a
	 * visited node instead of brother nodes at the same level. 
	 */
	DEEP_ITERATION, 
	/** 
	 * @brief Makes a wide iteration, visiting first all nodes for
	 * a given level, after visiting nodes for the next level.
	 */
	WIDE_ITERATION
} AxlIterationMode;

/**
 * \defgroup axl_handlers Axl Handlers: Handlers declarations used by Axl Library functions.
 */

/** 
 * \addtogroup axl_handlers
 * @{
 */

/** 
 * @brief Axl iteration function definition.
 *
 * This handler definition is used by \ref axl_doc_iterate as the
 * function definition that will be called for each node found in the
 * document.
 *
 * The function provides a pointer to the node found, the first
 * paramenter, and additionally, provides a pointer to the parent node
 * for the node found, the document where the node is found and an
 * optional user defined pointer provided at the function calling
 * (\ref axl_doc_iterate).
 *
 * The function returns a boolean value to signal the library to stop
 * iterating over the XML structure if \ref false is returned. So, to
 * continue the iteration, you must always return \ref true.
 * 
 * @param node The node found inside the document.
 *
 * @param parent The parent node for the node found (first parameter).
 *
 * @param doc The document that contains the node found.
 *
 * @param was_removed If contains a reference to a boolean value that
 * helps the invoked funtion to notify the iteration system that the
 * node was removed from the tree, using \ref axl_node_remove or \ref
 * axl_node_replace. Iteration support inside axl library is built in
 * a way that allows the programmer to remove a node (including its
 * childs) without breaking the loop, however, you must use this
 * variable to notify that the node was removed, so the iteration
 * system won't iterate over its childs.
 * 
 * @param ptr A user defined pointer that the user provided at \ref
 * axl_doc_iterate.
 * 
 * @return The callback must return false in the case the iteration
 * must be stopped. Otherwise, true must be returned.
 */
typedef bool (*axlIterationFunc) (axlNode * node, axlNode * parent, axlDoc * doc, bool * was_removed, axlPointer ptr);

/** 
 * @brief Axl iteration function definition (with two user defined
 * pointer support).
 *
 * This handler definition is used by \ref axl_doc_iterate_full as the
 * function definition that will be called for each node found in the
 * document.
 *
 * The function provides a pointer to the node found, the first
 * paramenter, and additionally, provides a pointer to the parent node
 * for the node found, the document where the node is found and an
 * optional user defined pointer provided at the function calling
 * (\ref axl_doc_iterate_full).
 *
 * The function returns a boolean value to signal the library to stop
 * iterating over the XML structure if \ref false is returned. So, to
 * continue the iteration, you must always return \ref true.
 * 
 * @param node The node found inside the document.
 *
 * @param parent The parent node for the node found (first parameter).
 *
 * @param doc The document that contains the node found.
 *
 * @param was_removed If contains a reference to a boolean value that
 * helps the invoked funtion to notify the iteration system that the
 * node was removed from the tree, using \ref axl_node_remove or \ref
 * axl_node_replace. Iteration support inside axl library is built in
 * a way that allows the programmer to remove a node (including its
 * childs) without breaking the loop, however, you must use this
 * variable to notify that the node was removed, so the iteration
 * system won't iterate over its childs.
 * 
 * @param ptr A user defined pointer that the user provided at \ref
 * axl_doc_iterate_full.
 *
 * @param ptr2 Second user defined pointer that the user provided at
 * \ref axl_doc_iterate_full.
 * 
 * @return The callback must return false in the case the iteration
 * must be stopped. Otherwise, true must be returned.
 */
typedef bool (*axlIterationFunc2) (axlNode * node, axlNode * parent, axlDoc * doc, bool * was_removed, axlPointer ptr, axlPointer ptr2);

/** 
 * @brief Defines a signature for a set of function that are used to
 * duplicate the content provided at the first parameter, returning a
 * copy.
 *
 * This handler definition is used by: 
 *
 * - \ref axl_list_copy
 * 
 * @param ptr The data to duplicate.
 * 
 * @return A newly allocated data duplicated.
 */
typedef axlPointer (*axlDuplicateFunc) (axlPointer ptr);

/** 
 * @brief Handler used by the \ref axl_list_module "axl list module"
 * to perform linear and efficient lookups.
 * 
 * @param ptr A pointer to the object stored inside the list and to be
 * checked if it is the one looked up. 
 * 
 * @param data A pointer to a user defined data that is received at
 * the lookup function and passed to this handler.
 * 
 * @return The function should return true (found). Otherwise, false
 * must be returned to keep on searching.
 */
typedef bool (*axlLookupFunc) (axlPointer ptr, axlPointer data);

/** 
 * @brief Hashing function used by the axl hash module to implement
 * translation from an user defined pointer into a number that should
 * be as much unique as possible.
 * 
 * @param key User defined data that represents the key for a data to
 * be stored into the hash. The value provided here usually is an
 * string but it could be any other data used as key. 
 * 
 * @return The function must return a positive value that will be used
 * to index the content into the hash table. It doesn't matter if the
 * number is greater than the table size. A modulo operation is
 * applied to the result.
 */
typedef unsigned int (*axlHashFunc) (axlPointer key);

/** 
 * @brief Foreach function signature used to represent the set of
 * functions used at \ref axl_hash_foreach.
 * 
 * The function receives the item found (key and data values) as well
 * as a user defined pointer also defined at \ref
 * axl_hash_foreach. The function must return \ref true (<i>"item
 * found"</i>) to make the search to stop. In the case a full
 * iteration over all items inside the hash is required, the function
 * must always return \ref false.
 * 
 * @param key The key for the item stored.
 * @param data The data associated to the key found
 * @param user_data User defined data that was provided to the axl_hash_foreach function.
 * 
 * @return \ref true to make the foreach process to stop. \ref false
 * to make the process to continue.
 */
typedef bool (* axlHashForeachFunc) (axlPointer key, axlPointer data, axlPointer user_data);

/** 
 * @brief Foreach function signature used to represent the set of
 * functions used at \ref axl_hash_foreach2.
 * 
 * The function receives the item found (key and data values) as well
 * as two user defined pointers also defined at \ref
 * axl_hash_foreach2. The function must return \ref true (<i>"item
 * found"</i>) to make the search to stop. In the case a full
 * iteration over all items inside the hash is required, the function
 * must always return \ref false.
 * 
 * @param key The key for the item stored.
 * @param data The data associated to the key found
 * @param user_data User defined data that was provided to the axl_hash_foreach2 function.
 * @param user_data2 Second User defined data that was provided to the axl_hash_foreach2 function.
 * 
 * @return \ref true to make the foreach process to stop. \ref false
 * to make the process to continue.
 */
typedef bool (* axlHashForeachFunc2) (axlPointer key, axlPointer data, axlPointer user_data, axlPointer user_data2);

/** 
 * @brief Foreach function signature used to represent the set of
 * functions used at \ref axl_hash_foreach3.
 * 
 * The function receives the item found (key and data values) as well
 * as tree user defined pointers also defined at \ref
 * axl_hash_foreach3. The function must return \ref true (<i>"item
 * found"</i>) to make the search to stop. In the case a full
 * iteration over all items inside the hash is required, the function
 * must always return \ref false.
 * 
 * @param key The key for the item stored.
 *
 * @param data The data associated to the key found
 *
 * @param user_data User defined data that was provided to the
 * axl_hash_foreach3 function.
 *
 * @param user_data2 Second User defined data that was provided to the
 * axl_hash_foreach3 function.
 *
 * @param user_data3 Third User defined data that was provided to the
 * axl_hash_foreach3 function.
 * 
 * @return \ref true to make the foreach process to stop. \ref false
 * to make the process to continue.
 */
typedef bool (* axlHashForeachFunc3) (axlPointer key, axlPointer data, axlPointer user_data, axlPointer user_data2, axlPointer user_data3);

/**
 * @brief Foreach function signature used to represent the set of
 * functions used at \ref axl_hash_foreach4.
 * 
 * The function receives the item found (key and data values) as well
 * as tree user defined pointers also defined at \ref
 * axl_hash_foreach4. The function must return \ref true (<i>"item
 * found"</i>) to make the search to stop. In the case a full
 * iteration over all items inside the hash is required, the function
 * must always return \ref false.
 * 
 * @param key The key for the item stored.
 *
 * @param data The data associated to the key found
 *
 * @param user_data User defined data that was provided to the
 * axl_hash_foreach4 function.
 *
 * @param user_data2 Second User defined data that was provided to the
 * axl_hash_foreach4 function.
 *
 * @param user_data3 Third User defined data that was provided to the
 * axl_hash_foreach4 function.
 *
 * @param user_data4 Forth User defined data that was provided to the
 * axl_hash_foreach4 function.
 * 
 * @return \ref true to make the foreach process to stop. \ref false
 * to make the process to continue.
 */
typedef bool (* axlHashForeachFunc4) (axlPointer key, axlPointer data, axlPointer user_data, axlPointer user_data2, axlPointer user_data3, axlPointer user_data4);

/** 
 * @brief Function handler definition for to allowing copying items at
 * the hash by \ref axl_hash_copy function.
 *
 * The function receive both pointers the key and the data value but,
 * only one of them must be copied. This is done to provide more
 * control at the copy process, but only the required value must be
 * returned. There is no indication to about which pointer must be
 * returned, so, don't use the same function to copy both pointers.
 *
 * The function also receive pointers to the current function
 * deallocation associated to the key and value being copied. This
 * could also work as information to know if the data must be
 * replicated or not. Having the destroy function defined for the item
 * is a clue to return an allocated item.
 * 
 * @param key The key to be copied if the function was provided to copy the key.
 *
 * @param key_destroy The key destroy function associated to the value
 * being copied.
 *
 * @param data The data to be copied if the function was provided to copy the data.
 *
 * @param data_destroy The data destroy function associated to the
 * data value being copied.
 * 
 * @return A newly allocated reference representing the copy. 
 */
typedef axlPointer (*axlHashItemCopy) (axlPointer key, axlDestroyFunc key_destroy, axlPointer data, axlDestroyFunc data_destroy);

/** 
 * @brief Foreach function handler used at \ref axl_stack_foreach
 * function to iterate all elements inside the stack, from the head to
 * the tail.
 *
 * The function receives two user defined pointers that are defined at
 * the \ref axl_stack_foreach function. 
 * 
 * @param stack_data A reference to the stack data stored.
 *
 * @param user_data A reference to a user defined pointer passed to
 * \ref axl_stack_foreach.
 *
 * @param user_data2 A second reference to a user defined pointer
 * passed to \ref axl_stack_foreach.
 * 
 * @return \ref true to make the foreach process to stop. \ref false
 * to make the process to continue.
 */
typedef bool (* axlStackForeach2) (axlPointer stack_data, axlPointer user_data, axlPointer user_data2);

/** 
 * @brief Foreach function handler used at \ref axl_stack_foreach3
 * function to iterate all elements inside the stack, from the head to
 * the tail.
 *
 * The function receives three user defined pointers that are defined
 * at the \ref axl_stack_foreach3 function.
 * 
 * @param stack_data A reference to the stack data stored.
 *
 * @param user_data A reference to a user defined pointer passed to
 * \ref axl_stack_foreach3.
 *
 * @param user_data2 A second reference to a user defined pointer
 * passed to \ref axl_stack_foreach3.
 *
 * @param user_data3 Third reference to a user defined pointer passed
 * to \ref axl_stack_foreach3.
 * 
 * @return \ref true to make the foreach process to stop. \ref false
 * to make the process to continue.
 */
typedef bool (* axlStackForeach3) (axlPointer stack_data, axlPointer user_data, axlPointer user_data2, axlPointer user_data3);

/** 
 * @brief Foreach function used by \ref axl_node_attr_foreach function.
 * 
 * @param key The attribute name.
 *
 * @param value The attribute value.
 *
 * @param data User defined pointer provided at \ref
 * axl_node_attr_foreach.
 *
 * @param data2 Second user defined data provided at \ref
 * axl_node_attr_foreach.
 *
 * @return The foreach function can stop the process at a particular
 * attribute by returning \ref true ("item found"). To iterate all
 * attributes return \ref false.
 */
typedef bool (* axlNodeAttrForeachFunc) (const char * key, const char * value, axlPointer data, axlPointer data2);

/** 
 * @brief Entity resolver function used by the library to translate
 * entity references into the replacement text. This is normally used
 * by the library itself, not by the application programmer.
 *
 * This handler is currently used at \ref axl_dtd_check_entity_ref_and_expand
 * 
 * @param entityName The entity name that is being requested to be
 * resolved.
 * 
 * @return The user defined data provided at \ref
 * axl_dtd_check_entity_ref_and_expand, which is passed to the
 * resolver function once it is executed.
 */
typedef const char * (* axlDtdEntityResolver) (const char * entityName, axlPointer data);

/* @} */
#endif

/* @} */
