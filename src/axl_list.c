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
#include <axl_list.h>
#include <axl_log.h>
#include <axl_stream.h>

#define LOG_DOMAIN "axl-list"

typedef struct _axlListNode axlListNode;

struct _axlList {
	/* functions used by the list to properly order elements
	 * inside it and how they are destroyed */
	axlEqualFunc     are_equal;
	axlDestroyFunc   destroy_data;

	/* pointers to the list content */
	axlListNode    * first_node;
	axlListNode    * last_node;

	/* list length */
	int              length;

	/* memory management functions */
	axlListNode   ** preallocated;
	int              available;
	int              allocated;
};

struct _axlListNode {
	axlListNode * previous;
	axlListNode * next;
	axlPointer    data;
};

struct _axlListCursor {
	axlList     * list;
	axlListNode * current;
};

/**
 * \defgroup axl_list_module Axl List: A configurable double-linked list used across the library.
 */

/** 
 * \addtogroup axl_list_module
 * @{
 */

/** 
 * @internal
 * 
 * Internal are equal handler implementation for the axl list module
 * that always return 0.
 */
int __axl_list_always_true (axlPointer a, axlPointer b)
{
	return 0;
}

/** 
 * @internal
 *
 * Preallocates nodes to be used to store data on the lists.
 * 
 * @param list The list where the preallocation operation will be
 * performed.
 */
void  __axl_list_allocate_nodes (axlList * list)
{
	int iterator;

	list->available     = 1;
	list->allocated    += list->available;

	/* allocate enough memory to hold all nodes already
	 * allocated */
	if (list->preallocated == NULL)
		list->preallocated  = axl_new (axlListNode *, list->allocated);
	else
		list->preallocated  = realloc (list->preallocated, (sizeof (axlListNode *)) * list->allocated);

	/* allocate a node for each available position */
	for (iterator = 0; iterator < list->available; iterator++) {
		list->preallocated[iterator] = axl_new (axlListNode, 1);
	}
	
	return;
}

/** 
 * @internal
 * 
 * Disposes the node provided, reusing on the next operation
 * requested.
 */
void __axl_list_dispose_node (axlList * list, axlListNode * node)
{
	/* store the node to be usable again */
	list->preallocated [list->available] = node;
	
	/* increase current available nodes */
	list->available++;
	
	return;
}

/** 
 * @internal
 *
 * Returns a reference to an list node to be used.
 */
axlListNode * __axl_list_get_next_node_available (axlList * list)
{
	axlListNode * node;

	/* check that there are nodes available */
	if (list->available == 0) 
		__axl_list_allocate_nodes (list);
	
	/* get the next node available */
	node = list->preallocated[list->available - 1];
	list->available--;

	/* clean node */
	node->next     = NULL;
	node->previous = NULL;
	node->data     = NULL;
		
	return node;
}


/** 
 * @brief Creates a new \ref axlList, using provided handlers.
 *
 * An \ref axlList is a double linked list, with the hability to
 * recognize elements inside its list by providing the \ref
 * axlEqualFunc "are_equal" function and the ability to release
 * memory allocated by the data stored by providing a \ref
 * axlDestroyFunc "destroy_data" handler. 
 *
 * The destroy handler is a optional value. If not provided, any
 * automatic deallocation function will not provided. 
 *
 * The "are equal" handler is not optinal. You must provide it to make
 * the list work. In the case you don't like a ordering feature you
 * can provide an are equal that just return 0 when elements are equal
 * and always -1 for the rest of the cases if element should be
 * appended or 1 if the element should be prepended.
 *
 * The list is not prepared to be managed at the same type by several
 * threads. If the list is created and then used by several threads it
 * will work properly. However, if several threads add and removes
 * elements at the same time rainy days will come and you'll get funny
 * behaviours.
 *
 * To create the list you must provide two function that performs some
 * minimal functions required by the list o properly order the data
 * inside the list and how this data is deallocated (this is
 * optional).
 * 
 * For example, to create a list that will hold strings dinamically
 * allocated you can use:
 * \code
 * axlList * list = axl_list_new (axl_list_equal_string, axl_free);
 * \endcode
 *
 * For a list that will holds integer values you can use: \ref
 * axl_list_equal_int. 
 * 
 * Previous list will cause all strings inside to be deallocated once
 * called to \ref axl_list_free. If you don't like this, do not
 * provide the deallocation function.
 *
 * You can always use the following function to make the list to allways
 * add all data at the end of the list: \ref axl_list_always_return_1,
 * which, as its names indicates, allways return 1, causing the item
 * to be added at the end of the list. See \ref axlEqualFunc
 * documentation to know more about creating ordenation functions.
 *
 * Now you have your list created, you can use the following functions
 * to add items:
 *
 *  - \ref axl_list_add
 *  - \ref axl_list_add_at
 *  - \ref axl_list_prepend
 *  - \ref axl_list_append
 *
 * Once you have inserted some data, you can use the following piece
 * of code to perform an iteration:
 *
 * \code
 * int iterator = 0;
 * while (iterator < axl_list_length (list)) {
 *     // get the data at the given position 
 *     data = axl_list_get_nth (list, iterator);
 *
 *     // update the iterator 
 *     iterator++;
 * }
 * \endcode
 *
 * However, it is preferred to use the \ref axlListCursor, which is
 * far more efficient. See the following function to get more
 * information: \ref axl_list_cursor_new.
 *
 * In general, if you are going to perform a lookup for a single item
 * you can use \ref axl_list_lookup (by providing a handler) or \ref
 * axl_list_get_nth if you know the position.
 *
 * 
 * @param are_equal The equal function to be used by the list to find
 * and order elements inside the list.
 *
 * @param destroy_data An optional handler to destroy nodes in the
 * case the list is unrefered.
 * 
 * @return A newly allocated list, that must be destroy by calling to
 * \ref axl_list_free.
 */
axlList * axl_list_new    (axlEqualFunc are_equal, axlDestroyFunc destroy_data)
{
	axlList * result;

	axl_return_val_if_fail (are_equal, NULL);

	result               = axl_new (axlList, 1);
	result->are_equal    = are_equal;
	result->destroy_data = destroy_data;

	/* preallocate memory for nodes */
	/* __axl_list_allocate_nodes (result); */

	return result;
}

/** 
 * @brief Allows to copy the provided list, returning a newly
 * allocated structure. 
 *
 * The copy process can also perform a deep copy for all data stored
 * inside the \ref axlList. However, for this purpose it is required
 * to provide a duplication function that is able to implement the
 * duplication operation over the data received. 
 * 
 * This handler is optional, and in the case it is not provided, the
 * list returned will be a copy having reference to the content
 * stored.
 * 
 * @param list The list to copy.
 * @param func The duplication function used to perform a deep copy (optional handler).
 * 
 * @return A newly allocated \ref axlList with the same configuration
 * as the one provided.
 */
axlList  * axl_list_copy   (axlList * list, axlDuplicateFunc func)
{
	axlList   * result;
	int         iterator;
	axlPointer  data;

	axl_return_val_if_fail (list, NULL);

	/* create a new axl list */
	result = axl_list_new (list->are_equal, list->destroy_data);

	/* if the duplicate function is NULL, remove the destroy
	 * function */
	if (func == NULL)
		result->destroy_data = NULL;

	/* add all elements */
	iterator = 0;
	while (iterator < axl_list_length (list)) {
		/* get data pointer from the list */
		data = axl_list_get_nth (list, iterator);

		/* try to make a copy */
		if (func != NULL)
			data = func (data);
		
		/* add the data to the new list */
		axl_list_add (result, data);

		/* update index */
		iterator++;
	}

	return result;
}

/** 
 * @brief Equal function that is prepared to receive two strings a
 * return if they are equal.
 *
 * This function is provided as a convenience implementation for the
 * \ref axl_list_new function, allowing to store strings inside the
 * given list.
 * 
 * The function will return 0 if the string are equal and 1 if
 * not. This will cause to make the strings addes to be append at the
 * end of the list.
 * 
 * @param a The first string to compare.
 * @param b The second string to compare.
 */
int      axl_list_equal_string (axlPointer a, axlPointer b)
{
	int length = strlen (a);
	axl_return_val_if_fail (a, 1);
	axl_return_val_if_fail (b, 1);


	if (axl_stream_cmp (a, b, length))
		return 0;
	return 1;
}

/** 
 * @brief Equal function that is preprated to receive to integers and
 * return if they are equal.
 *
 * It is assumed that integers are stored in the list using the
 * following:
 * \code
 * axl_list_add (list, INT_TO_PTR (integer));
 * \endcode
 *
 * You can use this function to create an \ref axlList that stores
 * integer values as follows:
 * \code
 * list = axl_list_new (axl_list_equal_int, NULL);
 * \endcode
 *
 * The list created with this function will order data from litter to
 * greater values.
 * 
 * 
 * @param a A reference to the first integer.
 * @param b A reference to the second integer.
 * 
 * @return 0 if values received are equal, and 1 if b is greater than
 * b. Otherwise -1 is returned.
 */
int        axl_list_equal_int    (axlPointer a, axlPointer b)
{
	/* especial case where a 0 is stored */
	if (PTR_TO_INT (a) == PTR_TO_INT (b))
		return 0;
	else if (PTR_TO_INT (a) < PTR_TO_INT (b))
		return -1;
	else
		return 1;
}

/** 
 * @brief An equal function that could be used to make elements to be
 * stored inside an \ref axlList at the end as the are added, without
 * replacing any previously added item.
 * 
 *
 * If it is needed to create a list that stores elements at the end as
 * they are added, this function could be used as the \ref
 * axlEqualFunc, while calling to axl_list_new.
 * 
 * @param a First item.
 * @param b Second item.
 * 
 * @return The function always return 1.
 *
 * NOTE: If you use this function and your intention is to remove
 * items (without calling to axl_list_free) you must use \ref
 * axl_list_remove_ptr or \ref axl_list_unlink_ptr since \ref
 * axl_list_remove and \ref axl_list_unlink relays on the equal
 * function to find and remove the item. Because this function never
 * return 0, the item is never removed.
 */
int        axl_list_always_return_1 (axlPointer a, axlPointer b)
{
	return 1;
}

/** 
 * @brief Allows to store a new element inside the list, using the
 * provided data.
 *
 * The function will fail to perform any action if a null reference is
 * provided to the function.
 * 
 * @param list The list where the element will be added.
 *
 * @param pointer The pointer to store.
 *
 * NOTE: The function uses the equal function defined at \ref
 * axl_list_new. If the function shows that the item to be added is
 * already added (because the equal function returns 0, then the item
 * won't be added.
 *
 */
void      axl_list_add    (axlList * list, axlPointer pointer)
{
	axlListNode * new_node;
	axlListNode * node;
	axlListNode * node2;


	/* perform some environment checkings */
	axl_return_if_fail (list);
	
	new_node         = __axl_list_get_next_node_available (list); 
	new_node->data   = pointer;
	
	/* check basic case */
	if (list->first_node == NULL) {
		list->first_node = new_node;
		list->last_node  = new_node;
		list->length     = 1;
		return;
	}
	
	/* complex case */
	node  = list->first_node;
	node2 = list->last_node;

	/* lookup */
	while ((node != NULL) || (node2 != NULL)) {
		/* lookup the head */
		if (node != NULL) {
			switch (list->are_equal (node->data, pointer)) {
			case -1:
				/* the node should be added before node  */
				new_node->next     = node;
				new_node->previous = node->previous;
				node->previous     = new_node;

				/* if new previous is null do not update it */
				if (new_node->previous != NULL)
					new_node->previous->next = new_node;
				else
					list->first_node         = new_node;

				/* update list length */
				list->length ++;
				return;
			case 0:

				/* the node found is equal, do not perform any operation */
				return;
			case 1:
			default:
				/* the node should be added after */
				node = node->next;
				break;
			}
		} /* end if */

		/* lookup from the tail */
		if (node2 != NULL) {
			switch (list->are_equal (node2->data, pointer)) {
			case -1:
			default:
				/* the node should be added before node  */
				node2 = node2->previous;
				break;
			case 0:

				/* the node found is equal, do not perform any operation */
				return;
			case 1:
				/* the node should be added after */
				new_node->previous = node2;
				new_node->next     = node2->next;
				node2->next        = new_node;
				
				/* do not update if next is NULL */
				if (new_node->next != NULL)
					new_node->next->previous  = new_node;
				else
					list->last_node           = new_node;

				/* update length size */
				list->length ++;
				return;
			}
		} /* end if */
	} /* end while */

	/* nothing more to do */
	return;
}

/** 
 * @brief Allows to adds the provided item to the given list at the
 * selected position.
 *
 * The function will perform an indexed addition, using the value
 * <b>position</b>, by-passing current list configuration (\ref
 * axl_list_new).
 *
 * If the position is greater than the length of the list, the item is
 * added at the end of the list. If the position is 0, the item is
 * added at the begin (equivalent to call \ref axl_list_prepend). 
 *
 * If an item is found at the provided position, the element is added
 * before the already found.
 * 
 * @param list The list where the addition operation will be performed.
 * 
 * @param pointer The item to add to the list.
 *
 * @param position Position where the addition operation will be
 * performed. Values allowed ranges from 0 up to list length - 1.
 */
void       axl_list_add_at (axlList * list, axlPointer pointer, int position)
{
	int           iterator;
	axlListNode * node;
	axlListNode * new_node;

	/* check incoming values */
	axl_return_if_fail (list);

	/* check if we have a prepend operation */
	if (position <= 0) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "adding item using prepend");
		/* prepend */
		axl_list_prepend (list, pointer);

		return;
	}
	/* check if we have an append operation */
	if (position >= list->length) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "adding item using append (position=%d >= length=%d)",
			   position, list->length);
		/* append */
		axl_list_append (list, pointer);
		
		return;
	}
	
	/* allocate a new node */
	new_node         = __axl_list_get_next_node_available (list); 
	new_node->data   = pointer;
	

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "looking node position: %d", position);

	/* basic case isn't reached here (remove first and last
	 * cases) */
	iterator = 1;
	node     = list->first_node->next;
	while (iterator < position) {

		/* get the next element */
		node = node->next;
		
		/* update the iterator */
		iterator++;
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "adding item at: %d", iterator);

	/* add the element */
	new_node->previous = node->previous;
	if (node->previous != NULL)
		node->previous->next = new_node;
	
	new_node->next     = node;
	node->previous     = new_node;

	/* update number of items inside */
	list->length++;
	return;
}

/** 
 * @brief Allows to add a node to the provided list, at the first
 * position, without taking into consideration current \ref axlList
 * configuration (\ref axlEqualFunc at \ref axl_list_new). 
 * 
 * @param list The list where the data will be added at the first
 * position.
 *
 * @param pointer The pointer to add.
 */
void       axl_list_prepend (axlList * list, axlPointer pointer)
{
	axlListNode * new_node;

	axl_return_if_fail (list);
	
	/* simulate adding the node at the first position */
	new_node         = __axl_list_get_next_node_available (list); 
	new_node->data   = pointer;

	/* make the new node the be the first one */
	if (list->first_node == NULL) {
		list->first_node = new_node;
		list->last_node  = new_node;
	}else {
		list->first_node->previous = new_node;
		new_node->next             = list->first_node;
		list->first_node           = new_node;
	}

	/* update number of items inside */
	list->length++;
	
	return;
}


/** 
 * @brief Allows to add a node to the provided list, at the last
 * position, without taking into consideration current \ref axlList
 * configuration (\ref axlEqualFunc at \ref axl_list_new). 
 * 
 * @param list The list where the data will be added at the last
 * position.
 *
 * @param pointer The pointer to add.
 */
void       axl_list_append  (axlList * list, axlPointer pointer)
{
	axlListNode * new_node;

	axl_return_if_fail (list);
	
	/* simulate adding the node at the first position */
	new_node         = __axl_list_get_next_node_available (list); 
	new_node->data   = pointer;

	/* make the new node the be the first one */
	if (list->last_node == NULL) {
		list->first_node  = new_node;
		list->last_node   = new_node;
	}else {
		list->last_node->next  = new_node;
		new_node->previous     = list->last_node;
		list->last_node        = new_node;
	}

	/* update number of items inside */
	list->length++;
	
	return;
}

/** 
 * @internal Internal list lookup using a linear search, checking all
 * items inside the list taking into considerations hints provided by
 * equal function.
 * 
 * @param list The list where the linear search will be performed.
 * @param pointer The pointer that is being looked up.
 * 
 * @return A reference to the internal axl list node containing the
 * pointer.
 */
axlListNode * axl_list_internal_linear_lookup (axlList    * list, 
					       axlPointer    pointer)
{
	axlListNode * node;

	axl_return_val_if_fail (list, NULL);
	axl_return_val_if_fail (pointer, NULL);

	/* complex case */
	node  = list->first_node;

	/* lookup */
	while (node != NULL) {
		if (list->are_equal (node->data, pointer) == 0)
			return node;

		/* the node should be after this one */
		node = node->next;
		
	} /* end while */

	return NULL;
}

/** 
 * @internal Internal list lookup using a linear search, checking all
 * items inside the list withtout taking into considerations hints
 * provided by equal function (search by pointer).
 * 
 * @param list The list where the linear search will be performed.
 * @param pointer The pointer that is being looked up.
 * 
 * @return A reference to the internal axl list node containing the
 * pointer.
 */
axlListNode * axl_list_internal_linear_lookup_ptr (axlList    * list, 
						   axlPointer    pointer)
{
	axlListNode * node;

	axl_return_val_if_fail (list, NULL);
	axl_return_val_if_fail (pointer, NULL);

	/* complex case */
	node  = list->first_node;

	/* lookup */
	while (node && node->data != pointer) {
		/* the node should be after this one */
		node = node->next;
	} /* end while */

	/* return current result */
	return node;
}

/** 
 * @internal
 * @brief Internal lookup function to locate the axlListNode that contains the pointer.
 *
 * 
 * @param list The list where the lookup will be performed.
 * @param pointer The pointer data to lookup.
 * 
 * @return A reference to the \ref axlListNode or NULL if no pointer
 * is found.
 */
axlListNode * axl_list_internal_lookup (axlList * list, axlPointer pointer)
{
	axlListNode * node;
	axlListNode * node2;

	axl_return_val_if_fail (list, NULL);
	axl_return_val_if_fail (pointer, NULL);

	/* complex case */
	node  = list->first_node;
	node2 = list->last_node;

	/* lookup */
	while ((node != NULL) || (node2 != NULL)) {
		/* lookup the head */
		if (node != NULL) {
			switch (list->are_equal (node->data, pointer)) {
			case -1:
			default:
				/* node should be before the node
				 * found. So we are not going to find
				 * a node that is lower, the element
				 * is not in the list.
				 */
				return NULL;
			case 0:
				return node;
			case 1:
				/* the node should be after this one */
				node = node->next;
				break;
			}
		} /* end if */

		/* lookup from the tail */
		if (node2 != NULL) {
			switch (list->are_equal (node2->data, pointer)) {
			case -1:
				/* the node should be added before node  */
				node2 = node2->next;
				break;
			case 0:
				return node2;
			case 1:
			default:
				/* it seems that the node should be
				 * found after this node but this is
				 * not going to be possible. The node is not in the list.
				 */
				return NULL;
			}
		} /* end if */
	} /* end while */

	return NULL;
}


/** 
 * @internal
 *
 * @brief Allows to lookup the pointer stored, inside the provided
 * list, on the given position.
 * 
 * @param list The list where the operation will be performed.
 * @param position The position to lookup for stored data.
 * 
 * @return A reference or NULL if fails.
 */
axlListNode * axl_list_internal_get_nth (axlList * list, int position)
{
	axlListNode * node;
	int           iterator = 0;

	axl_return_val_if_fail (list, NULL);
	axl_return_val_if_fail (position >= 0 && position < list->length, NULL);
	
	/* iterate until the node is found */
	node = list->first_node;
	while (node != NULL && (iterator != position)) {
		iterator ++;
		node = node->next;
	}

	/* return data found */
	if (iterator == position)
		return node;
	return NULL;
}

/* remove the selected node */
void __axl_list_common_remove_selected_node (axlList * list, axlListNode * node, 
					     bool alsoRemove)
{
	axlPointer pointer;

	/* do not remove anything if a null reference is received */
	if (node == NULL)
		return;
	
	/* get a reference to the pointer */
	pointer = node->data;
	
	if (node->previous == NULL)
		list->first_node = node->next;
	else
		node->previous->next = node->next;
	
	if (node->next == NULL)
		list->last_node      = node->previous;
	else
		node->next->previous = node->previous;
	
	/* release user space allocated memory
	 * if defined destroy function */
	if (alsoRemove && (list->destroy_data != NULL))
		list->destroy_data (pointer);
	
	/* release memory allocated by the node */
	__axl_list_dispose_node (list, node); 

	/* decrease list length */
	list->length--;

	/* nothing more to do */
	return;
}

/** 
 * @internal
 *
 * Internal support for axl_list_remove and axl_list_unlink
 * function. The function perform a node removing, the one that
 * contains the node pointed by the provided pointer, and making a
 * node deallocation according to the configuration of the
 * <b>alsoRemove</b>.
 * 
 * @param list The list where the operation will be performed.
 *
 * @param pointer The pointer to remove
 *
 * @param alsoRemove Also call to destroy function.
 *
 * @param byPtr Makes the linear search to be done by pointer.
 */
void     axl_list_common_remove (axlList * list, axlPointer pointer, bool alsoRemove, bool byPtr)
{
	axlListNode * node;

	axl_return_if_fail (list);
	axl_return_if_fail (pointer);

	/* complex case */
	if (byPtr)
		node = axl_list_internal_linear_lookup_ptr (list, pointer);
	else
		node  = axl_list_internal_linear_lookup (list, pointer);
	if (node == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "unable to find item by pointer (0x%x)",
			   pointer);
		return;
	}

	/* remove the selected node */
	__axl_list_common_remove_selected_node (list, node, alsoRemove);
	
	return;	
}


/** 
 * @brief Allows to remove the given pointer from the list.
 *
 * The element referenced by <b>pointer</b> will be removed from the
 * list, include the memory allocated if a destroy function were
 * provided.
 *
 * If it is required to remove a pointer from the list, without
 * calling to the destroy function, you can use \ref axl_list_unlink.
 *
 * The function will fail to work if references provided are NULL.
 *
 * @param list The list where the removal operation will be performed.
 * @param pointer The pointer where the 
 *
 * NOTE: The function uses the current equal function configured. A
 * not properly configured equal function will make this function to
 * not remove the item selected. If you are trying to remove by
 * pointer, you can use \ref axl_list_remove_ptr.
 */
void      axl_list_remove (axlList * list, axlPointer pointer)
{

	/* perform a complete removing */
	axl_list_common_remove (list, pointer, 
				/* alsoRemove */
				true, 
				/* byPtr */ 
				false);

	return;
}

/** 
 * @brief Allows to remove the provided pointer from the list (calling
 * to the destroy function defined).
 *
 * Unlike \ref axl_list_remove, which removes the element selected by
 * using the equal function configured at \ref axl_list_new, this
 * function allows to perform a remove operation by pointer.
 * 
 * @param list The list on which the operation is performed.
 *
 * @param pointer The pointer to remove from the list.
 */
void axl_list_remove_ptr (axlList * list, axlPointer pointer)
{
	/* perform a complete removing */
	axl_list_common_remove (list, pointer, 
				/* alsoRemove */
				true, 
				/* byPtr */ 
				true);
}

/** 
 * @brief Removes the given pointer from the list, without calling the
 * destroy function, even when it is configured.
 * 
 * @param list The list where the operation will be performed.
 *
 * @param pointer The pointer to remove.
 */
void       axl_list_unlink (axlList * list, axlPointer pointer)
{
	/* perform a complete removing */
	axl_list_common_remove (list, pointer, 
				/* alsoRemove */
				false, 
				/* byPtr */ 
				false);

	return;
}

/** 
 * @brief Allows to remove the provided item from the axl list
 * withtout using the equal function provided (remove by pointer) and
 * without calling to the associated destroy function.
 * 
 * @param list The list where the operation will be implemented.
 *
 * @param pointer the pointer to remove from the list without calling
 * to the destroy function.
 */
void axl_list_unlink_ptr (axlList * list, axlPointer pointer)
{

	/* perform an unlink operation, without using equal
	 * function */
	/* perform a complete removing */
	axl_list_common_remove (list, pointer, 
				/* alsoRemove */
				false, 
				/* byPtr */ 
				true);
	
	return;
}

/** 
 * @brief Allows to remove the first element, calling to the destroy
 * function if defined.
 * 
 * @param list The list where the first element will be removed.
 */
void       axl_list_remove_first (axlList * list)
{

	axl_return_if_fail (list);

	/* do not perform any operation if no node is stored */
	if (list->first_node == NULL)
		return;

	/* remove the selected node */
	__axl_list_common_remove_selected_node (list, list->first_node, true);

	return;
}

/** 
 * @brief Allows to remove the first element from the list without
 * calling to the destroy function, even with it is defined.
 * 
 * @param list The list where the first element will be removed.
 */
void       axl_list_unlink_first (axlList * list)
{

	axl_return_if_fail (list);

	/* do not perform any operation if no node is stored */
	if (list->first_node == NULL)
		return;

	/* remove the selected node */
	__axl_list_common_remove_selected_node (list, list->first_node, false);

	return;
}

/** 
 * @brief Allows to remove the last element, calling to the destroy
 * function if defined.
 * 
 * @param list The list where the last element will be removed.
 */
void       axl_list_remove_last (axlList * list)
{
	axl_return_if_fail (list);

	/* do not perform any operation if no node is stored */
	if (list->last_node == NULL)
		return;

	/* remove the selected node */
	__axl_list_common_remove_selected_node (list, list->last_node, true);

	return;
}

/** 
 * @brief Allows to remove the last element from the list without
 * calling to the destroy function, even with it is defined.
 * 
 * @param list The list where the last element will be removed.
 */
void       axl_list_unlink_last (axlList * list)
{
	axl_return_if_fail (list);

	/* do not perform any operation if no node is stored */
	if (list->last_node == NULL)
		return;

	/* remove the selected node */
	__axl_list_common_remove_selected_node (list, list->last_node, false);

	return;
}

/** 
 * @brief Allows to check if the given pointer is stored on the given
 * list.
 * 
 * @param list The list where the lookup will be performed.
 *
 * @param pointer The pointer to lookup.
 * 
 * @return \ref true if the element is stored on the list,
 * otherwise false is returned. The function will fail to lookup
 * if a NULL reference is received, either the list or the pointer.
 */
bool          axl_list_exists (axlList * list, axlPointer pointer)
{
	axl_return_val_if_fail (list, false);
	axl_return_val_if_fail (pointer, false);

	if (axl_list_internal_lookup (list, pointer) != NULL)
		return true;
	return false;
}

/** 
 * @brief Allows to check if the provided list is empty (no element
 * stored).
 * 
 * @param list The list to check for emptyness.
 * 
 * @return true if the list is empty, false if not. The function
 * return false in the case a null reference is provided.
 */
bool       axl_list_is_empty  (axlList * list)
{
	axl_return_val_if_fail (list, false);

	/* check if the first node is defined */
	return (list->first_node == NULL);
}

/** 
 * @brief Allows to check if the given pointer is stored on the given position.
 *
 * @param list The list where the operation will be run.
 * @param pointer The pointer to check.
 * @param position The position where is expected to find the pointer.
 * 
 * @return true if the given data, referenced by the pointer, is
 * stored on the given position.
 */
bool           axl_list_exists_at (axlList * list, axlPointer pointer, int position)
{
	axlListNode * node;

	node = axl_list_internal_get_nth (list, position);
	if (node != NULL) {
		if (! list->are_equal (node->data, pointer))
			return true;
	}
	return false;
}

/** 
 * @brief Returns the first element stored on the list.
 *
 *
 * @param list The list where the first element stored will be
 * returned.
 * 
 * @return An \ref axlPointer containing the data stored on the list.
 */
axlPointer axl_list_get_first (axlList * list)
{
	axl_return_val_if_fail (list, NULL);

	if (list->first_node == NULL)
		return NULL;
	return list->first_node->data;
}

/** 
 * @brief Returns the last element stored on the list.
 * 
 * @param list The list where the last element will be returned.
 * 
 * @return An \ref axlPointer reference containing the last stored
 * value.
 */
axlPointer axl_list_get_last  (axlList * list)
{
	axl_return_val_if_fail (list, NULL);

	if (list->last_node == NULL)
		return NULL;
	return list->last_node->data;
}

/** 
 * @brief Allows to get current pointer stored at the given position.
 * 
 * @param list The list where the data will be retrieved.
 *
 * @param position A value ranging from 0 up to the the list lenght -
 * 1.
 * 
 * @return The \ref axlPointer stored on the given position or NULL if
 * fail.
 */
axlPointer axl_list_get_nth   (axlList * list, int position)
{
	axlListNode * node;

	node = axl_list_internal_get_nth (list, position);
	if (node != NULL)
		return node->data;
	return NULL;
}

/** 
 * @brief Allows to perform a linear lookup on the list provided,
 * givin a function that is used to now the object to return due to
 * the lookup.
 *
 * The function can also be used as a foreach function. The following
 * example shows how to launch the function and perform a tasks on the
 * lookup function:
 * \code
 * // perform the lookup 
 * return axl_list_lookup (list, __find_item, name);
 *
 * // the lookup function 
 * bool __find_item (axlPointer _element, axlPointer data)
 * {
 *	SomeItem * element = _element;
 *	char     * name    = data;
 *
 *	// check the name 
 *	if (axl_cmp (element->name, name))
 *		return true;
 *
 *	// it is not the element 
 *	return false;
 * }
 * \endcode
 *
 * In the case you create a list to hold string values, you can use
 * \ref axl_list_find_string as lookup function predefined to perform
 * the search.
 * 
 * @param list The list where the lookup will be performed.
 *
 * @param func The function to use to perform the lookup.
 * 
 * @param data User defined data that will be passed to the func provided.
 * 
 * @return A pointer to the object found or NULL if no item was found.
 */
axlPointer axl_list_lookup    (axlList * list, axlLookupFunc func, axlPointer data)
{
	axlListNode * node;
	axl_return_val_if_fail (list, NULL);
	axl_return_val_if_fail (func, NULL);

	/* get the first pointer */
	node = list->first_node;
	do {
		/* if the next node to check is NULL, terminate the
		 * lookup. */
		if (node == NULL)
			return NULL;

		/* check if the node found is the one looked up */
		if (func (node->data, data))
			return node->data;

		/* seems not, update to the next reference */
		node = node->next;
		
	}while (1);

	/* return no node found */
	return NULL;
}

/** 
 * @brief Helper function that could be used at \ref axl_list_lookup if
 * the list created only contains strings.
 *
 * Use this function as a parameter for the lookup function at \ref
 * axl_list_lookup.
 *
 * @param element The element at the list, in this case, an string value.
 *
 * @param data The data provided at \ref axl_list_lookup, in this
 * case, the value we are looking.
 * 
 * @return \ref true if the string was found, \ref false if not.
 */
bool       axl_list_find_string (axlPointer element, axlPointer data)
{
	/* if the string received is null, just return false */
	if (data == NULL)
		return false;

	/* return the comparison status */
	return axl_cmp ((char *) element, (char *) data);
}

/** 
 * @brief Allows to get current list length.
 * 
 * @param list The list to operate.
 * 
 * @return The list length or -1 if fail (the list reference received
 * is null).
 */
int       axl_list_length (axlList * list)
{
	axl_return_val_if_fail (list, -1);
	return list->length;
}

/** 
 * @brief Allows to destroy the given list, and all user space
 * associated memory if a destroy handler was provided.
 * 
 * @param list The list to destroy
 */
void      axl_list_free (axlList * list)
{
	axlListNode * node;
	axlListNode * node2;
	int           iterator;

	/* if a null reference is received do not oper */
	if (list == NULL)
		return;

	node     = list->first_node;
	while (node != NULL) {
		if (list->destroy_data != NULL) {
			list->destroy_data (node->data);
		}
		node2 = node;
		node  = node->next;

		axl_free (node2);
	}

	/* allocate a node for each available position */
	for (iterator = 0; iterator < list->available; iterator++) {
		axl_free (list->preallocated[iterator]);
	}

	/* free the array */
	axl_free (list->preallocated);
	
	/* free the list itself */
	axl_free (list);

	return;
}

/* @} */

/**
 * \defgroup axl_list_cursor_module Axl List Cursor: Iterator support for the Axl List
 */

/** 
 * \addtogroup axl_list_cursor_module
 * @{
 */

/** 
 * @brief Allows to get a cursor to iterate the list in a linear and
 * efficient way.
 *
 * The \ref axlListCursor could be used to iterate an \ref axlList in
 * an efficient way because it stores current state (position). Then
 * using the following functions you can modify the state (current
 * position to get):
 * 
 *   - \ref axl_list_cursor_first
 *   - \ref axl_list_cursor_last
 *   - \ref axl_list_cursor_next
 *   - \ref axl_list_cursor_previous
 *
 * Finally, a function is provided to get the data stored at a
 * particular position, pointed by the current status of the cursor:
 * 
 *   - \ref axl_list_cursor_get
 *
 * You are allowed to remove elements from the list (\ref axlList)
 * having a cursor created (\ref axlListCursor), using \ref
 * axl_list_cursor_unlink. 
 *
 * Here is an example:
 * \code
 * axlPointer      value;
 * axlListCursor * cursor;
 * 
 * // create the cursor 
 * cursor   = axl_list_cursor_new (list);
 *
 * // while there are more elements 
 * while (axl_list_cursor_has_item (cursor)) {
 *
 *   // get the value 
 *   value = axl_list_cursor_get (cursor);
 *
 *
 *   // get the next 
 *   axl_list_cursor_next (cursor);
 *
 *   // update the iterator 
 *   iterator++;
 *		
 * } 
 *
 * // free the cursor 
 * axl_list_cursor_free (cursor);
 * \endcode
 * 
 * @param list The list that the new cursor (\ref axlListCursor) will
 * provide access.
 * 
 * @return A newly created \ref axlListCursor used to iterate the
 * list. Once finished you must call to \ref axl_list_cursor_free.
 */
axlListCursor * axl_list_cursor_new      (axlList * list)
{
	axlListCursor * cursor;

	axl_return_val_if_fail (list, NULL);

	/* create the cursor */
	cursor = axl_new (axlListCursor, 1);

	/* initial configuration */
	cursor->list    = list;
	cursor->current = list->first_node;

	return cursor;
}

/** 
 * @brief Allows to configure the cursor to point to the first item of
 * the list (if there are any).
 * 
 * @param cursor The cursor that is required to be configured to point the first item.
 */
void            axl_list_cursor_first    (axlListCursor * cursor)
{
	axl_return_if_fail (cursor);

	if (cursor->list->length == 0) {
		cursor->current = NULL;
		return;
	} /* end if */

	/* set the first node */
	cursor->current = cursor->list->first_node;

	return;
}

/** 
 * @brief Allows to configure the cursor to point to the last item of
 * the list (if there are any).
 * 
 * @param cursor The cursor that is required to be configured to point
 * to the last item.
 */
void            axl_list_cursor_last     (axlListCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* set the first node */
	cursor->current = cursor->list->last_node;

	return;
}

/** 
 * @brief Allows to configure the cursor to point to the next item of
 * the list (if there are any).
 * 
 * @param cursor The cursor that is required to be configured to point
 * to the next item.
 */
void            axl_list_cursor_next     (axlListCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* set the next node */
	if (cursor->current != NULL)
		cursor->current = cursor->current->next;

	return;
}

/** 
 * @brief Allows to configure the cursor to point to the previous item
 * of the list (if there are any).
 * 
 * @param cursor The cursor that is required to be configured to point
 * to the previous item.
 */
void            axl_list_cursor_previous (axlListCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* set the next node */
	if (cursor->current != NULL)
		cursor->current = cursor->current->previous;

	return;
}

/** 
 * @brief Allows to check if there are more elements next to the
 * current element pointed by the cursor.
 * 
 * @param cursor The cursor that is required to return if there are
 * next items.
 * 
 * @return \ref true if more items are found, otherwise \ref false is
 * returned.
 */
bool            axl_list_cursor_has_next (axlListCursor * cursor)
{
	axl_return_val_if_fail (cursor, false);

	/* check for empty list */
	if (cursor->current == NULL)
		return false;

	/* return if the next element isn't null */
	return (cursor->current->next != NULL);
}

/** 
 * @brief Allows to check if there are more elements next to the
 * current element pointed by the cursor.
 * 
 * @param cursor The cursor that is required to return if there are
 * next items.
 * 
 * @return \ref true if more items are found, otherwise \ref false is
 * returned.
 */
bool            axl_list_cursor_has_previous (axlListCursor * cursor)
{
	axl_return_val_if_fail (cursor, false);

	/* check for empty list */
	if (cursor->current == NULL)
		return false;

	/* return if the next element isn't null */
	return (cursor->current->previous != NULL);
}

/** 
 * @brief Allows to know if the current position has items.
 * 
 * @param cursor The cursor that is requested to return if a call to
 * \ref axl_list_cursor_get will return data.
 * 
 * @return \ref true if the list that is iterated can return data at
 * the current position, otherwise \ref false is returned.
 */
bool            axl_list_cursor_has_item    (axlListCursor * cursor)
{
	axl_return_val_if_fail (cursor, false);

	/* return if there are current */
	return (cursor->current != NULL);
}

/** 
 * @brief Allows to remove current element pointed by the cursor,
 * maintainig internal state of the cursor.
 *
 * The function won't call to the destroy function asociated to the
 * list. If you want the item stored to be also destroyed call \ref
 * axl_list_cursor_remove.
 * 
 * @param cursor The cursor pointing to the item inside the list that
 * must be removed.
 */
void            axl_list_cursor_unlink       (axlListCursor * cursor)
{
	axlListNode * node;

	axl_return_if_fail (cursor);

	/* if current cursor is pointing nowhere, just do nothing */
	if (cursor->current == NULL)
		return;

	/* remember node */
	node = cursor->current;	

	/* configure the cursor to point to the next element (or the previous if the next element is null) */
	cursor->current = (node->next != NULL) ? node->next : node->previous;

	/* call to unlik */
	__axl_list_common_remove_selected_node (cursor->list, node, false);

	return;
}

/** 
 * @brief Allows to remove current element pointed by the cursor,
 * maintainig internal state of the cursor, calling to the destroy
 * function associated in the list.
 *
 * The function will call to the destroy function asociated to the
 * list. If you don't want the item stored to be also destroyed call \ref
 * axl_list_cursor_unlink.
 * 
 * @param cursor The cursor pointing to the item inside the list that
 * must be removed.
 */
void            axl_list_cursor_remove       (axlListCursor * cursor)
{
	axlListNode * node;

	axl_return_if_fail (cursor);

	/* if current cursor is pointing nowhere, just do nothing */
	if (cursor->current == NULL)
		return;

	/* remember node */
	node = cursor->current;

	/* configure the cursor to point to the next element (or the previous if the next element is null) */
	cursor->current = (node->next != NULL) ? node->next : node->previous;

	/* call to remove */
	__axl_list_common_remove_selected_node (cursor->list, node, true);

	return;
}

/** 
 * @brief Allows to get current data at the current cursor state.
 * 
 * @param cursor The cursor that will be used to return the data
 * located at the list, using cursor current state.
 */
axlPointer      axl_list_cursor_get      (axlListCursor * cursor)
{
	axl_return_val_if_fail (cursor, NULL);

	/* nothing to return if current is NULL */
	if (cursor->current == NULL)
		return NULL;

	/* return data */
	return cursor->current->data;
}

/** 
 * @brief Allows to get the reference to the list that is associated
 * to the cursor received.
 * 
 * @param cursor The cursor that is required to return the list associated.
 * 
 * @return A reference to the list being iterated or NULL if fails.
 */
axlList       * axl_list_cursor_list         (axlListCursor * cursor)
{
	/* check incoming cursor */
	axl_return_val_if_fail (cursor, NULL);

	/* return the list */
	return cursor->list;
}

/** 
 * @brief Deallocates memory used by the cursor. 
 *
 * @param cursor The cursor to be deallocated.
 */
void            axl_list_cursor_free     (axlListCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* free the cursor */
	axl_free (cursor);

	return;
}

/* @} */
