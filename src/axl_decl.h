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

#include <stdlib.h>
#include <string.h>



/**
 * @brief Axl XML document.
 */
typedef struct _axlDoc axlDoc;

/**
 * @brief Axl XML Node.
 */
typedef struct _axlNode axlNode;

/**
 * @brief Axl XML Attribute.
 */
typedef struct _axlAttribute axlAttribute;

/** 
 * @brief Axl error reporting variable.
 */
typedef struct _axlError  axlError;

/** 
 * @internal
 * @brief Axl XML stream representation.
 */
typedef struct _axlStream axlStream;

#define AXL_TRUE  1
#define AXL_FALSE 0

/** 
 * @brief Alias declaration to bind the <i>int</i> to the <b>bool</b>
 * keyword.
 *
 * This is mainly used to emphasize that some integer values that
 * returns some function must be considred to be \ref AXL_TRUE or \ref
 * AXL_FALSE, that represents the boolean TRUE and FALSE values.
 *
 * This allows to perform boolean comparations using structure
 * controls like if, while, but also making a differenciation about
 * the boolean values and integer values.
 */
typedef int bool;

/** 
 * @brief Pointer to any structure definition. It should be required
 * to use this definition, however, some platforms doesn't support the
 * <b>void *</b> making it necessary to use the <b>char *</b>
 * definition as a general way to represent references.
 */
typedef void * axlPointer;

/** 
 * @brief axlList definition, a list implementation.
 */
typedef struct _axlList axlList;

/** 
 * @brief axlStack definitino, a stack implementation on top of \ref
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
 * @internal
 *
 * @brief Support macro to allocate memory using the calloc function,
 * making a casting and using the sizeof keyword.
 *
 * @param type The type to allocate
 * @param count How many items to allocate.
 * 
 * @return A newly allocated pointer.
 */
#define axl_new(type, count) (type *) calloc (count, sizeof (type))

/** 
 * @internal
 *
 * @brief Allows to deallocate memory referenced by <i>ref</i> but
 * checking before that the reference is different from null.
 * 
 * @param ref The reference to clear.
 */
#define axl_free(ref) if (ref != NULL) free (ref)

/** 
 * @internal
 * @brief Allows to check a condition and return if it is not meet.
 * 
 * @param expr The expresion to check.
 */
#define axl_return_if_fail(expr) \
if (!(expr)) return;


/** 
 * @internal
 *
 * @brief Allows to check a condition and return the given value if it
 * is not meet.
 * 
 * @param expr The expresion to check.
 *
 * @param val The value to return if the expression is not meet.
 */
#define axl_return_val_if_fail(expr, val) \
if (!(expr)) return val;

/** 
 * @internal
 *
 * @brief Call to strdup function check if received is a NULL
 * reference
 * 
 * @param string The string to copy.
 * 
 * @return A newly allocated value or NULL.
 */
#define axl_strdup(string) (string != NULL) ? (char *) axl_stream_strdup ((char *) string) : NULL;


/** 
 * @internal
 * 
 * @brief Consumes all spaces found and tabulars on the given stream
 * until a different char is found.
 * 
 * @param stream The stream where the operation will be performed.
 */
#define AXL_CONSUME_SPACES(stream) \
while ((axl_stream_inspect_several (stream, 4, " ", "\t", "\r", "\n") > 0)) \
	axl_stream_accept (stream); 

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

#endif
