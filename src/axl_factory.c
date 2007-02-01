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
 * @internal Allows to create a new factory, for the provided
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

/** 
 * @internal Function that allows to get a block from the factory. 
 * 
 * @param factory The factory that is being used to allocate more
 * memory.
 * 
 * @return A reference to newly allocated memory.
 */
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

		block          = axl_new (axlItemBlock, 1);
		block->items   = calloc (factory->step, factory->type_size);
		factory->count = 1;

		block->next    = factory->block;
		factory->block = block;
		
	} /* end if */
	
	return ((char *)factory->block->items) + ((factory->count - 1) * factory->type_size);
} /* end if */

/** 
 * @internal Allows to deallocate the axlFactory created.
 * 
 * @param factory 
 */
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

typedef struct _axlStrBlock axlStrBlock;

struct _axlStrBlock {
	char        * buffer;
	axlStrBlock * next;
};

struct _axlStrFactory {
	int           index;
	int           step;
	int           size;
	axlStrBlock * block;
};

/** 
 * @internal Allows to create an string factory, an abstraction that
 * allows to allocate string chuncks in a memory efficient way,
 * according to the Axl Library allocation pattern.
 *
 * @return A reference to the newly allocated axlStrFactory.
 */
axlStrFactory * axl_string_factory_create ()
{
	axlStrFactory * factory;

	/* create a factory */
	factory                = axl_new (axlStrFactory, 1);
	factory->step          = 1024;
	factory->size          = 1024;
	factory->block         = axl_new (axlStrBlock, 1);
	factory->block->buffer = axl_new (char, factory->size);
	
	/* return factory created */
	return factory;
}

/** 
 * @internal Allows to allocate a new chunk from the axlStrFactory.
 * 
 * @param factory The factory that will be used to allocate an string.
 *
 * @param size The size to allocate. The amount of memory to allocate
 * to hold the string.
 * 
 * @return A pointer to allocated memory to hold the exact amount of
 * memory requested.
 */
char          * axl_string_factory_alloc  (axlStrFactory * factory, int size)
{
	axlStrBlock * block;
	char        * result;

	/* check that we could satisfy the size request with current
	 * block */
	if ((factory->size - factory->index - 1) < size ) {
		
		/* alloc a new block */
		block = axl_new (axlStrBlock, 1);

		/* ensure the block can hold the string */
		if (size > factory->size) {
			block->buffer = axl_new (char, size + 1);
			factory->size = size + 1;
		} else {
			/* store step allocation */
			factory->size = factory->step;
			block->buffer = axl_new (char, factory->size + 1);
		}

		/* configure the new block */
		block->next    = factory->block;
		factory->block = block;
		factory->index = 0;
	} /* end if */

	/* get current index available */
	result = factory->block->buffer + factory->index;

	/* nullify to normalize string */
	result [ size ] = 0;

	/* reserve size bytes */
	factory->index += size + 1;

	return result;
}

/** 
 * @internal Deallocs the axl stream factory and all chunks allocated.
 * 
 * @param factory The factory to dealloc.
 */
void            axl_string_factory_free   (axlStrFactory * factory)
{
	axlStrBlock * block;
	axlStrBlock * aux;

	/* do nothing if null received */
	if (factory == NULL)
		return;

	/* get the first block */
	block = factory->block;
	
	while (block != NULL) {
		
		/* get a reference to the next */
		aux = block->next;

		/* free items and the block */
		axl_free (block->buffer);
		axl_free (block);

		/* get the next */
		block = aux;

	} /* end while */

	axl_free (factory);

	return;
}


