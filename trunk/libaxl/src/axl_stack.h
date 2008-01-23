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
#ifndef __AXL_STACK_H__
#define __AXL_STACK_H__

#include <axl_decl.h>



BEGIN_C_DECLS

/** 
 * \addtogroup axl_stack_module
 * @{
 */

axlStack * axl_stack_new (axlDestroyFunc destroy_data);

void       axl_stack_push (axlStack * stack, axlPointer data);

axlPointer axl_stack_pop  (axlStack * stack);

axlPointer axl_stack_peek (axlStack * stack);

bool       axl_stack_foreach (axlStack         * stack, 
			      axlStackForeach2   func,
			      axlPointer         user_data, 
			      axlPointer         user_data2);

bool       axl_stack_foreach3 (axlStack         * stack, 
			       axlStackForeach3   func,
			       axlPointer         user_data,
			       axlPointer         user_data2,
			       axlPointer         user_data3);

int        axl_stack_size (axlStack * stack);

bool       axl_stack_is_empty (axlStack * stack);

void       axl_stack_free (axlStack * stack);

axlBinaryStack * axl_binary_stack_new ();

void             axl_binary_stack_push          (axlBinaryStack * bstack, bool status);

void             axl_binary_stack_push_the_same (axlBinaryStack * bstack);

bool             axl_binary_stack_pop  (axlBinaryStack * bstack);

bool             axl_binary_stack_peek (axlBinaryStack * bstack);

bool             axl_binary_stack_is_empty (axlBinaryStack * bstack);

int              axl_binary_stack_size     (axlBinaryStack * bstack);

void             axl_binary_stack_free (axlBinaryStack * bstack);

/* @} */

END_C_DECLS

#endif
