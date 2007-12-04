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

#include <axl_decl.h>
#include <axl_stack.h>
#include <axl_list.h>
#include <axl_log.h>
#define LOG_DOMAIN "axl-stack"

struct _axlStack {
	axlPointer     * stack;
	int              size;
	int              items;
	axlDestroyFunc   destroy;
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
	stack          = axl_new (axlStack, 1);
	stack->destroy = destroy_data;

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

	/* do not perform any operation if the stack is empty */
	if (axl_stack_is_empty (stack)) {
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

	/* destroy the list inside (if the destroy function is
	 * defined) */
	if (stack->destroy) {
		while (! axl_stack_is_empty (stack)) {
			
			/* destroy */
			stack->destroy (axl_stack_pop (stack));
			
		} /* end while */
	} /* end if */

	/* free stack array */
	axl_free (stack->stack);
	
	/* destroy the stack */
	axl_free (stack);

	return;
}

/* @} */

typedef struct _axlBinaryStackNode {
	int  count;
	bool state;
} axlBinaryStackNode;


struct _axlBinaryStack {
	axlBinaryStackNode * last;
	axlStack           * stack;
	axlFactory         * factory;
	int                  count;
};

/**
 * \defgroup axl_binary_stack_module Axl Binary Stack: A compact binary state stack
 */

/** 
 * \addtogroup axl_binary_stack_module
 * @{
 */

/** 
 * @brief Allows to create a compact binary state stack.
 * 
 * To stack is designed to store boolean values in an efficient
 * way. In idea behind this structure is that storing 10 boolean true
 * values only holds a node, with the true value and the count of
 * items that previously was signaled with the same value. 
 *
 * The intention is to compact a binary succession of data into a list
 * of touples signaling the state and the number of times it was
 * signaled.
 *
 * This structure is meant to be more efficient than a plain \ref
 * axlStack but there is a case where is not more efficient, that is,
 * when all stored boolean values alternates.
 * 
 * @return A newly allocated binary stack. Push boolean values with
 * \ref axl_binary_stack_push. Pop previous stored value with \ref
 * axl_binary_stack_pop. Once finished, use \ref axl_binary_stack_free
 * to free the binary stack.
 */
axlBinaryStack * axl_binary_stack_new ()
{
	axlBinaryStack * result;

	result           = axl_new (axlBinaryStack, 1);
	result->stack    = axl_stack_new (axl_free);

	/* return stack created */
	return result;
} 

/** 
 * @brief Push a new boolean item into the binary stack.
 * 
 * @param bstack The binary stack
 *
 * @param state The state to push into the queue.
 */
void             axl_binary_stack_push (axlBinaryStack * bstack, bool state)
{
	axlBinaryStackNode * node;

	/* check references */
	axl_return_if_fail (bstack);
	
	/* check basic case */
	if (bstack->last == NULL) {

	axl_binary_stack_push_create_and_return:

		/* configure last */
		node         = axl_new (axlBinaryStackNode, 1);
		node->count  = 1;
		node->state  = state;

		/* configure */
		bstack->last = node;

		/* store in the stack */
/*		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Storing first item: 0x%x", node); */
		axl_stack_push (bstack->stack, node);
		
		/* update count */
		bstack->count++;

/*		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Storing item state=%d, count=%d, bcount=%d",
		node->state, node->count, bstack->count); */

		return;
	} /* end if */

	/* default case, check current state */
	if (bstack->last->state == state) {

		/* found same state, increase count and return */
		bstack->last->count++;

		/* update count */
		bstack->count++;

/*		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Found same state, reusing..state=%d, count=%d, bcount=%d",
		bstack->last->state, bstack->last->count, bstack->count); */

		return;
	} /* end if */

/*	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "Found state changed, creating..state=%d (previuos state: %d), count=%d, bcount=%d",
	state, bstack->last->state, bstack->last->count, bstack->count); */

	/* if reached this place, create a new node */
	goto axl_binary_stack_push_create_and_return;
}

/** 
 * @brief Convenience function that allows to push a new value using
 * the last one pushed in the stack. This function allows to implement
 * the same feature provided by peeking (\ref axl_binary_stack_peek)
 * the next visible value on the stack head and then pushing it again.
 * 
 * @param bstack The binary stack where to perform the operation. 
 *
 * NOTE: The stack must not be empty before calling to this
 * function. It has to have at least one value already pushed.
 */
void             axl_binary_stack_push_the_same (axlBinaryStack * bstack)
{
	axl_return_if_fail (bstack);
	axl_return_if_fail (bstack->last);
	
	/* update stack */
	bstack->last->count++;
	bstack->count++;

	return;
}

/** 
 * @brief Allows to pop the next value from the stack.
 * 
 * @param bstack The binary stack where the next value will be
 * restored.
 * 
 * @return The next value stored in the stack. You must check if the
 * stack is empty before calling to this function since you won't be
 * able to differenciate a false value stored from a false value
 * returned by an error.
 */
bool             axl_binary_stack_pop  (axlBinaryStack * bstack)
{
	axlBinaryStackNode * node;
	bool                 state;
	axl_return_val_if_fail (bstack, false);

	/* get last value */
	node = bstack->last; 
	if (node == NULL) {
		/* return false because the stack is empty */
		return false;
	} /* end if */

	/* check if there are more than 1 item, that is, at least
	 * 2. */
	if (node->count > 1) {

		/* update count */
		bstack->count--;

		/* decrease the counter and return */
		node->count--;

/*		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "POP: found state with more than 1 item, poping: state=%d, count=%d, bcount=%d",
		node->state, node->count, bstack->count); */

		return node->state;

	} else if (node->count == 1) {

/*		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "POP: found state with only 1 item, poping: state=%d, count=%d, bcount=%d",
		node->state, node->count, bstack->count); */

		/* get the value */
		state = node->state;
		
		/* update the next last (remove the current last) and
		 * use the next */
		axl_stack_pop (bstack->stack);
		bstack->last = axl_stack_peek (bstack->stack);

		/* release to the factory */
/*		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "POP: new node to use: 0x%x, about to release 0x%x", 
		bstack->last, node); */
		axl_free (node);

		/* update count */
		bstack->count--;

		node = bstack->last;
/*		if (node != NULL) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "POP: after poping: state=%d, count=%d, bcount=%d",
				   node->state, node->count, bstack->count);
		} */

		return state;
	} /* end if */

	/* this shouldn't be reached */
	return false;
}

/** 
 * @brief Allows to get the current stack head value stored without
 * removing it from the stack.
 * 
 * @param bstack The binary stack where to perform the operation.
 * 
 * @return The value stored in the stack head. The stack must contain
 * at least one value, otherwise the returned value is not
 * reliable. See \ref axl_binary_stack_is_empty to avoid calling to this function.
 */
bool             axl_binary_stack_peek (axlBinaryStack * bstack)
{
	axl_return_val_if_fail (bstack, false);
	axl_return_val_if_fail (bstack->last, false);

	/* return current state */
	return bstack->last->state;
}

/** 
 * @brief Check current emptyness status for the provided binary
 * stack.
 * 
 * @param bstack The binary stack to check.
 * 
 * @return true if the binary stack is empty, otherwise false is
 * returned.
 */
bool             axl_binary_stack_is_empty (axlBinaryStack * bstack)
{
	axl_return_val_if_fail (bstack, false);

	/* return current emptyness status */
	return (bstack->last == NULL && axl_stack_is_empty (bstack->stack));
}

/** 
 * @brief Allows to get current number of items stored.
 * 
 * @param bstack The binary stack to check current number of items
 * stored.
 * 
 * @return The number of items stored or -1 if it fails.
 */
int              axl_binary_stack_size     (axlBinaryStack * bstack)
{
	axl_return_val_if_fail (bstack, -1);

	/* return item count */
	return bstack->count;
}

/** 
 * @brief Release the provided binary stack.
 * 
 * @param bstack The binary stack to release.
 */
void             axl_binary_stack_free (axlBinaryStack * bstack)
{
	if (bstack == NULL)
		return;

	/* free resources */
	axl_stack_free (bstack->stack);
	axl_free (bstack);

	return;
}

/**
 * @}
 */
