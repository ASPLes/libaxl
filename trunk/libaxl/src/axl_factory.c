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
#include <axl.h>

typedef struct _axlItemBlock axlItemBlock;

struct _axlItemBlock {
	axlPointer      items;
	axlItemBlock  * next;
};

struct _axlFactory {
	int       count;
	int       step;
	int       type_size;
	axlItemBlock * block;
};

/** 
 * @brief Allows to create a new factory, for the provided
 * type. 
 *
 * This will cause the function to return an axlFactory with blocks
 * initialized.
 * 
 * @param type The type to initialize the factory created.
 * 
 * @return A newly allocated factory.
 */
axlFactory * axl_factory_create (int size_of_type)
{
	axlFactory * result;
	
	result            = axl_new (axlFactory, 1);
	result->type_size = size_of_type;
	result->step      = 256 / size_of_type;

	result->block        = axl_new (axlItemBlock, 1);
	result->block->items = calloc (result->step, size_of_type);

	return result;

} /* end axl_factory_create */


axlPointer   axl_factory_get (axlFactory * factory)
{
	axlItemBlock * block;
	
	/* update factory allocated elements */
	factory->count++;

	if ((factory->count) > factory->step) {
		
		if (factory->step == (256 / factory->type_size))
			factory->step = 512 / factory->type_size;
		else if (factory->step == (512 / factory->type_size))
			factory->step = 1024 / factory->type_size;
		else if (factory->step == (1024 / factory->type_size))
			factory->step = 2048 / factory->type_size;
		else if (factory->step == (2058 / factory->type_size))
			factory->step = 4096 / factory->type_size;
		else if (factory->step == (4096 / factory->type_size))
			factory->step = 8192 / factory->type_size;

		block          = axl_new (axlItemBlock, 1);
		block->items   = calloc (factory->step, factory->type_size);
		factory->count = 1;

		block->next    = factory->block;
		factory->block = block;
	}
	
	return factory->block->items + ((factory->count - 1) * factory->type_size);
}


void         axl_factory_free (axlFactory * factory)
{
	axlItemBlock * block;
	axlItemBlock * aux;

	/* do nothing if null received */
	if (factory == NULL)
		return;

	/* get the first block */
	block = factory->block;
	
	while (block != NULL) {
		
		/* get a reference to the next */
		aux = block->next;

		/* free items and the block */
		axl_free (block->items);
		axl_free (block);

		/* get the next */
		block = aux;

	} /* end while */

	axl_free (factory);

	return;	
}


