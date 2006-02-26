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

#include <axl_decl.h>
#include <axl_stack.h>
#include <axl_list.h>
#include <axl_log.h>
#define LOG_DOMAIN "axl-stack"

struct _axlStack {
	axlList * list;
};

/**
 * \defgroup axl_stack_module Axl Stack: An stack built on top of axlList, used across AXL library
 */

/** 
 * \addtogroup axl_stack_module
 * @{
 */

/** 
 * @internal
 *
 * Make the list to behave as it were a stack. Always return that the
 * element should be installed a the very begin of the list.
 */
int __axl_stack_compare_func (axlPointer a, axlPointer b)
{
	return -1;
}

/** 
 * @brief Creates a new stack.
 *
 * Creates a new \ref axlStack object, which will accept to store a
 * retrieve objects in a FIFO manner.
 * 
 * @param destroy_data A function to be used to destroy data stored on
 * the stack is the stack is deallocated containing data. This
 * parameter is optional. If not provided, no automatic memory
 * deallocation will peformed.
 * 
 * @return A newly allocated stack that must be deallocated by using
 * axl_stack_destroy.
 */
axlStack * axl_stack_new (axlDestroyFunc destroy_data)
{
	axlStack * stack;

	stack       = axl_new (axlStack, 1);
	stack->list = axl_list_new (__axl_stack_compare_func, destroy_data);
	
	return stack;
}

/** 
 * @brief Push data on top of the stack.
 *
 * The stack doesn't support storing NULL references. If a null
 * reference is provided the function won't perform any operation on
 * the stack.
 * 
 * @param stack The stack where the data will be pushed.
 * @param data The data to push.
 */
void       axl_stack_push (axlStack * stack, axlPointer data)
{
	axl_return_if_fail (stack);
	axl_return_if_fail (data);

	/* simply add the element */
	axl_list_add (stack->list, data);
	
	return;
}

/** 
 * @brief Pop data from the stack.
 * 
 * @param stack The stack where the pop operation will be performed.
 * 
 * @return A \ref axlPointer containing previously pushed data or NULL
 * if fails.
 */
axlPointer axl_stack_pop  (axlStack * stack)
{
	axlPointer pointer;

	axl_return_val_if_fail (stack, NULL);

	/* do not perform any operation if the stack is empty */
	if (axl_stack_is_empty (stack))
		return NULL;

	/* get the first element of the stack */
	pointer = axl_list_get_first (stack->list);
	
	/* remove the reference from the stack */
	axl_list_unlink_first (stack->list);
	
	return pointer;
}

/** 
 * @brief Allows to get current current element at the top of the
 * stack without removing it.
 *
 * This function allows to perform the same operation like pop (\ref
 * axl_stack_pop) function, but without removing the element from the
 * top.
 * 
 * @param stack The stack where the element from the top will be returned.
 * 
 * @return An \ref axlPointer reference or NULL if fails.
 */
axlPointer axl_stack_peek (axlStack * stack)
{
	axl_return_val_if_fail (stack, NULL);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "peeking the stack with current size=%d", axl_list_length (stack->list));

	/* do not perform any operation if the stack is empty */
	if (axl_stack_is_empty (stack)) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "peek operation over a empty stack was detected, returning NULL");
		return NULL;
	}

	/* return the very first element */
	return axl_list_get_first (stack->list);
}


/** 
 * @brief Returns current stack size, that is, elements stored on the
 * stack.
 * 
 * @param stack The stack where the operation will performed.
 * 
 * @return The number of items stored.
 */
int        axl_stack_size (axlStack * stack)
{
	axl_return_val_if_fail (stack, -1);

	return axl_list_length (stack->list);
}


/** 
 * @brief Allows to check if the given stack is empty.
 * 
 * @param stack The stack to check.
 * 
 * @return \ref AXL_TRUE if the stack is empty or AXL_FALSE if not.
 */
bool       axl_stack_is_empty (axlStack * stack)
{
	axl_return_val_if_fail (stack, AXL_FALSE);

	if (axl_list_length (stack->list) == 0)
		return AXL_TRUE;
	return AXL_FALSE;
}

/** 
 * @brief Destroy the given stack.
 * 
 * @param stack The stack to destroy.
 */
void       axl_stack_destroy (axlStack * stack)
{
	axl_return_if_fail (stack);

	/* destroy the list inside */
	axl_list_free (stack->list);
	
	/* destroy the stack */
	axl_free (stack);

	return;
}

/* @} */
