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
#ifndef __AXL_DECL_H__
#define __AXL_DECL_H__

/* include this at this place to load GNU extensions */
#ifdef __GNUC__
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
#include <unistd.h>

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
 * @brief Axl XML Attribute type definition.
 */
typedef struct _axlAttribute axlAttribute;

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
	NOT_DEFINED = 1,
	/** 
	 * @brief The reference contains an \ref axlDtdElementList.
	 */
	ELEMENT_LIST = 2,
	/** 
	 * @brief The reference contains a reference to a leaf node, a
	 * content particule.
	 */
	NODE = 3
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
 *     return AXL_FALSE;
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
 * @internal
 * @brief Axl XML stream representation.
 */
typedef struct _axlStream axlStream;

/** 
 * @brief Type definition to represent a boolean true value, that is
 * equal to 1. 
 */
#define AXL_TRUE  (1)

/** 
 * @brief Type definition to represent a boolean false value, that is
 * equal to 0.
 */
#define AXL_FALSE (0)

/** 
 * @brief Alias declaration to bind the <i>int</i> to the <b>boolean</b>
 * concept (TRUE / FALSE states).
 *
 * This is mainly used to emphasize that some integer values that
 * returns some function must be considered to be \ref AXL_TRUE or \ref
 * AXL_FALSE, that represents the boolean TRUE and FALSE values.
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
#endif

/** 
 * @internal
 * More boolean compatibility declarations.
 */
#ifndef boolean
#define boolean int
#endif

/** 
 * @brief Pointer to any structure definition. It should be required
 * to use this definition, however, some platforms doesn't support the
 * <b>void *</b> making it necessary to use the <b>char *</b>
 * definition as a general way to represent references.
 */
typedef void * axlPointer;

/** 
 * @brief \ref axlList definition, a list implementation.
 */
typedef struct _axlList axlList;

/** 
 * @brief \ref axlStack definitinon, a stack implementation on top of \ref
 * axlList.
 */
typedef struct _axlStack axlStack;

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
 * @brief Support macro to allocate memory using the calloc function,
 * making a casting and using the sizeof keyword.
 *
 * @param type The type to allocate
 * @param count How many items to allocate.
 * 
 * @return A newly allocated pointer.
 */
#define axl_new(type, count) (type *) calloc (count, sizeof (type))

void    axl_free(axlPointer ref);

/** 
 * @brief Allows to check a condition and return if it is not meet.
 * 
 * @param expr The expresion to check.
 */
#define axl_return_if_fail(expr) \
if (!(expr)) {axl_log ("", AXL_LEVEL_CRITICAL, "Expresion '%s' have failed at %s (%s:lineno=%d)", #expr, __AXL_PRETTY_FUNCTION__, __AXL_FILE__, __AXL_LINE__); return;}

/** 
 * @brief Allows to check a condition and return the given value if it
 * is not meet.
 * 
 * @param expr The expresion to check.
 *
 * @param val The value to return if the expression is not meet.
 */
#define axl_return_val_if_fail(expr, val) \
if (!(expr)) { axl_log ("", AXL_LEVEL_CRITICAL, "Expresion '%s' have failed, returning: %s at %s (%s:lineno=%d)", #expr, #val, __AXL_PRETTY_FUNCTION__, __AXL_FILE__, __AXL_LINE__); return val;}


char * axl_strdup (const char * string);

/** 
 * @brief Alias definition for \ref axl_stream_strdup_printf.
 */
#define axl_strdup_printf axl_stream_strdup_printf

/** 
 * @brief Alias definition for \ref axl_stream_strdup_printfv.
 */
#define axl_strdup_printfv axl_stream_strdup_printfv

/** 
 * @internal
 * 
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
 * @return Returns AXL_TRUE if the string is empty and AXL_FALSE if
 * not.
 */
#define AXL_IS_STR_EMPTY(str) (((str == NULL) || strlen (str) == 0) ? AXL_TRUE : AXL_FALSE)

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

aboolean axl_cmp (char * string, char * string2);

#endif

/* @} */
