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
#define axl_strdup(string) (string != NULL) ? (char *) strdup ((const char *) string) : NULL;


#endif
