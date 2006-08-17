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
 *         info@aspl.es - http://fact.aspl.es
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
 */
void      axl_list_add    (axlList * list, axlPointer pointer)
{
	axlListNode * new_node;
	axlListNode * node;
	axlListNode * node2;


	/* perform some environment checkings */
	axl_return_if_fail (list);
	axl_return_if_fail (pointer);
	
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
	axl_return_if_fail (pointer);

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
	axl_return_if_fail (pointer);
	
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
	axl_return_if_fail (pointer);
	
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
 */
void     axl_list_common_remove (axlList * list, axlPointer pointer, bool alsoRemove)
{
	axlListNode * node;

	axl_return_if_fail (list);
	axl_return_if_fail (pointer);

	/* complex case */
	node  = axl_list_internal_lookup (list, pointer);
	if (node == NULL)
		return;

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
 */
void      axl_list_remove (axlList * list, axlPointer pointer)
{
	/* perform a complete removing */
	axl_list_common_remove (list, pointer, true);

	return;
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
	axl_list_common_remove (list, pointer, false);

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

	axl_return_if_fail (list);

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
