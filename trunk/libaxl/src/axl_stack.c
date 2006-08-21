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
	axlPointer * stack;
	int          size;
	int          items;
};

/**
 * \defgroup axl_stack_module Axl Stack: A stack built used across AXL library
 */

/** 
 * \addtogroup axl_stack_module
 * @{
 */

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

	/* create an empty stack */
	stack       = axl_new (axlStack, 1);

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

	/* check if we have enough space to store the pointer */
	if (stack->size == stack->items) {
		if (stack->size == 0) {
			/* ask for a single pointer */
			stack->stack = axl_new (axlPointer, 1);
		}else {
			/* ask to grow the memory already allocated */
			stack->stack = realloc (stack->stack, sizeof (axlPointer) * (stack->size + 1));
		}
		/* update the size available */
		stack->size++;
	}

	/* store the item */
	stack->stack[stack->items] = data;
	
	/* update items stored */
	stack->items++;
	
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

	/* reduce the items stored */
	stack->items--;

	/* return the pointer */
	pointer = stack->stack[stack->items];

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

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "peeking the stack with current size=%d", stack->items);

	/* do not perform any operation if the stack is empty */
	if (axl_stack_is_empty (stack)) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "peek operation over a empty stack was detected, returning NULL");
		return NULL;
	}

	/* return the very first element without poping */
	return stack->stack[stack->items - 1];
}

/** 
 * @internal Common support function for foreach over an stack for all
 * foreach functions defined.
 */
bool       __axl_stack_foreach_common (axlStack         * stack, 
				       axlStackForeach2   func,
				       axlStackForeach3   func3,
				       axlPointer         user_data, 
				       axlPointer         user_data2,
				       axlPointer         user_data3)
{
	int iterator;

	axl_return_val_if_fail (stack, false);

	/* for each item inside the stack */
	iterator = 0;
	while (iterator < stack->items) {
		/* call fo the function and check returning value  */
		if (func != NULL && func (stack->stack [stack->items - iterator - 1],  user_data,  user_data2))
			return false;

		/* call fo the function and check returning value  */
		if (func3 != NULL && func3 (stack->stack [stack->items - iterator - 1],  user_data,  user_data2, user_data3))
			return false;

		/* update the iterator */
		iterator ++;
	}

	/* iteration performed completely */
	return true;
}

/** 
 * @brief Allows to perform a foreach operation from the head of the
 * stack (the next item to be poped) to the tail of the stack (the
 * very first item pushed).
 *
 * The foreach process is non intrusive: it doesn't perform any change
 * of the stack, but allows to traverse all items in the natural order
 * in which items are stored (push) and removed (pop).
 *
 * The function provided to perform the foreach operation will be
 * called providing the stack data found, and the two user defined
 * pointers provided. 
 * 
 * @param stack The stack where the foreach operation will be
 * performed.
 *
 * @param func The foreach function to be called for each item found
 * in the stack.
 *
 * @param user_data User defined pointer to be passed to the function
 * provided.
 *
 * @param user_data2 User defined pointer to be passed to the function
 * provided.
 * 
 * @return \ref true if the foreach process was performed completely
 * through all items inside the stack or \ref false if not. The
 * function will also return \ref false to indicate a failure the
 * stack and func parameters are null.
 */
bool       axl_stack_foreach (axlStack         * stack, 
			      axlStackForeach2   func,
			      axlPointer         user_data, 
			      axlPointer         user_data2)
{
	/* call to common function */
	return __axl_stack_foreach_common (stack, func, NULL, user_data, user_data2, NULL);
}


/** 
 * @brief Allows to perform a foreach operation from the head of the
 * stack (the next item to be poped) to the tail of the stack (the
 * very first item pushed).
 *
 * The foreach process is non intrusive: it doesn't perform any change
 * of the stack, but allows to traverse all items in the natural order
 * in which items are stored (push) and removed (pop).
 *
 * The function provided to perform the foreach operation will be
 * called providing the stack data found, and the three user defined
 * pointers provided.
 * 
 * @param stack The stack where the foreach operation will be
 * performed.
 *
 * @param func The foreach function to be called for each item found
 * in the stack.
 *
 * @param user_data User defined pointer to be passed to the function
 * provided.
 *
 * @param user_data2 User defined pointer to be passed to the function
 * provided.
 *
 * @param user_data3 User defined pointer to be passed to the function
 * provided.
 * 
 * @return \ref true if the foreach process was performed completely
 * through all items inside the stack or \ref false if not. The
 * function will also return \ref false to indicate a failure the
 * stack and func parameters are null.
 */
bool       axl_stack_foreach3 (axlStack         * stack, 
			       axlStackForeach3   func,
			       axlPointer         user_data,
			       axlPointer         user_data2,
			       axlPointer         user_data3)
{
	/* call to common function */
	return __axl_stack_foreach_common (stack, NULL, func, user_data, user_data2, user_data3);
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

	/* current items stored */
	return stack->items;
}


/** 
 * @brief Allows to check if the given stack is empty.
 * 
 * @param stack The stack to check.
 * 
 * @return \ref true if the stack is empty or false if not.
 */
bool       axl_stack_is_empty (axlStack * stack)
{
	axl_return_val_if_fail (stack, false);
	
	/* return if there are stored items in the stack */
	return (stack->items == 0);
}

/** 
 * @brief Destroy the given stack.
 * 
 * @param stack The stack to destroy.
 */
void       axl_stack_free (axlStack * stack)
{
	axl_return_if_fail (stack);

	/* destroy the list inside */
	if (stack->size > 0)
		axl_free (stack->stack);
	
	/* destroy the stack */
	axl_free (stack);

	return;
}

/* @} */
