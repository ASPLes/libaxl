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
#include <axl_hash.h>
#include <axl_factory.h>
#include <axl_log.h>

#define LOG_DOMAIN "axl-hash"

/**
 * \defgroup axl_hash_module Axl Hash: A hash table used by Axl Library
 */

/** 
 * \addtogroup axl_hash_module
 * @{
 */

typedef struct _axlHashNode axlHashNode;

struct _axlHashNode {
	/* the key stored and the destroy function */
	axlPointer        key;
	axlDestroyFunc    key_destroy;

	/* the data stored and the destroy function */
	axlPointer        data;
	axlDestroyFunc    data_destroy;

	/* a pointer to the next item */
	axlHashNode     * next;
};

struct _axlHash {
	/* the hash function */
	axlHashFunc  hash;
	axlEqualFunc equal;

	/* the hash table, implemented using chaining for collition
	 * resolution. */
	axlHashNode ** table;

	/* factory to allocate nodes */
	axlFactory   * factory;

	/* stored items in the hash */
	int items;

	/* the hash size */
	int hash_size;

	/* steps: how many slots are allocated when the hash is
	 * resized */
	int step;
};

/** 
 * @internal Implementation of the axl hash cursor.
 */
struct _axlHashCursor {
	/* a pointer to the hash */
	axlHash     * hash;

	/* a pointer to the node inside the hash (current value) */
	axlHashNode * node;

	/* the value if the index being accessed by the node
	 * pointed (current value) */
	int           index;
};

/** 
 * @brief Public hash implementation for keys that are strings.
 * 
 * @param _key The string key to hash.
 * 
 * @return A value associated to the key.
 */
unsigned int axl_hash_string (axlPointer _key)
{
	/* never is received a NULL value at this function, so, don't
	 * check it */
	int    g, h = 0;
	char * key  = _key;
  
	/* hashing taken from the Red Dragon book!! */
	while (*key) {
		h = (h << 4) + *key;
		if ((g = h & 0xF0000000) != 0) {
			h ^= g >> 24;
			h ^= g;
		}
		
		++ key;
	} /* end while */

	/* return the value */
	return h;
}

/** 
 * @brief Common equal hash function associated to \ref
 * axl_hash_string that allows to check two keys to be equal,
 * conforming to the results expected by the hash (\ref axlHash).
 * 
 * @param keya The key to compare.
 *
 * @param keyb The other key to compare.
 * 
 * @return 0 if both strings are equal and any other else value when
 * they differs.
 */
int             axl_hash_equal_string (axlPointer keya, 
				       axlPointer keyb)
{
	char * _keya = keya;
	char * _keyb = keyb;
	int    i     = 0;

	while (_keya [i] != 0 && _keyb [i] != 0) {

		/* check values */
		if (_keya [i] != _keyb [i])
			return 1;

		/* update the iterator */
		i++;
	} /* end while */

	/* check that both string ends at the same point */
	if (_keya [i] != 0 || _keyb [i] != 0)
		return 1;
	
	/* both keys are equal */
	return 0;
}

/** 
 * @brief Convenience hashing function to store keys that are
 * integers.
 * 
 * @param key The key that is supported to contain a int value stored
 * using \ref INT_TO_PTR.
 * 
 * @return The index in the hash where is located the associated data.
 */
unsigned int    axl_hash_int          (axlPointer key)
{
	int value = PTR_TO_INT (key);

	return (unsigned int) value;
}

/** 
 * @brief Convenience hash function to compare two keys that holds
 * integers values.
 * 
 * @param keya The first key to compare.
 * @param keyb The second key to compare.
 * 
 * @return 0 if both keys are equal, 1 if not.
 */
int             axl_hash_equal_int    (axlPointer keya, 
				       axlPointer keyb)
{
	/* return if both keys containing int values are equal */
	return (PTR_TO_INT (keya) == PTR_TO_INT(keyb)) ? 0 : 1;
}


/** 
 * @internal Internal lookup function, returns the hole hash node.
 * 
 * @param hash The hash where the lookup will be performed.
 *
 * @param key The key that is being looked up.
 * 
 * @return The axlHashNode reference or NULL if not found.
 */
axlHashNode * __axl_hash_internal_lookup (axlHash * hash, axlPointer key)
{
	axlHashNode * node;

	axl_return_val_if_fail (hash, NULL);
	axl_return_val_if_fail (key, NULL);

	/* get the node at the provided position */
	if (hash->hash_size == 0)
		return NULL;
	node = hash->table [hash->hash (key) % hash->hash_size];

	/* node not found */
	if (node == NULL)
		return NULL;

	/* check for equal keys */
	if (hash->equal (node->key, key) == 0)
		return node;

	while (node->next != NULL) {
		/* seems we have more nodes */
		node = node->next;		

		/* check for equal keys */
		if (hash->equal (node->key, key) == 0)
			return node;
	}  /* end */

	/* node not found */
	return NULL;
}

/** 
 * @brief Creates a new hash table using the function provided as
 * hashing function.
 *
 * The hash function (\ref axlHashFunc) allows the hash table to
 * distribute values across the table while performing inserts
 * operation but it is also used while getting data from the table. 
 *
 * A second function is required (\ref axlEqualFunc) to resolve
 * internal table conflicts while placing data that are indexed using
 * the same value generated by \ref axlHashFunc. This hash
 * implementation store items at the giving position using a linked
 * list (Chaining collition resolution). Knowing this, an external
 * function is required to compare items to ensure they are selected
 * properly.
 *
 * This hash accept any kind of key and values to be stored as long as
 * the provided functions returns different identifiers to store
 * items. However, because the common use of a hash is to store data
 * using strings as keys two functions are provided by default to
 * create a string index hash table: \ref axl_hash_equal_string and
 * \ref axl_hash_string.
 *
 * \code
 * // create a string indexed hash 
 * axlHash * hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);
 * \endcode
 *
 * Additionally, two functions are provided to create hash containing
 * integer values as keys: \ref axl_hash_int and \ref axl_hash_equal_int.
 *
 * Once the hash is created the following functions must be used to
 * store data:
 *
 * - \ref axl_hash_insert
 * - \ref axl_hash_insert_full
 *
 * Then, use the following function to get data associated to the
 * provided key.
 *
 * - \ref axl_hash_get
 *
 * Finally, you can use the following functions to either remove items
 * from the hash and to completely deallocate the memory used by the
 * hash and all of its data:
 *
 * - \ref axl_hash_remove
 * - \ref axl_hash_free
 *
 * 
 * @param hash  The hashing function to be used for this table.
 *
 * @param equal The equal function used by the hash to actually check
 * that two stored items are equal (using the key value)
 * 
 * @return A newly created hash table that is deallocated by using \ref axl_hash_free.
 */
axlHash * axl_hash_new       (axlHashFunc    hash, axlEqualFunc equal)
{
	/* call to full implementation */
	return axl_hash_new_full (hash, equal, 10);
}

/** 
 * @brief The function works the same way like \ref axl_hash_new, but
 * provides a way to configure how many unit are allocated on hash
 * resizing operations.
 *
 * See \ref axl_hash_new for more information. That function uses a
 * default step value equal to 10.
 * 
 * @param hash  The hashing function to be used for this table.
 *
 * @param equal The equal function used by the hash to actually check
 * that two stored items are equal (using the key value)
 *
 * @param step The number of empty new slots to allocate once the hash
 * must be resized.
 * 
 * @return A newly created hash table that is deallocated by using \ref axl_hash_free.
 */
axlHash       * axl_hash_new_full     (axlHashFunc    hash,
				       axlEqualFunc   equal,
				       int            step)
{
	/* create the hash */
	axlHash * result;

	result           = axl_new (axlHash, 1);
	result->factory  = axl_factory_create (sizeof (axlHashNode));
	result->hash     = hash;
	result->equal    = equal;
	result->step     = step;

	/* return the hash table created */
	return result;	
}

/** 
 * @brief Inserts a key index value into the provided hash table.
 *
 * The function will store the data provided on the hash setting no
 * destroy function for the key and the data stored. See \ref
 * axl_hash_insert_full for more details.
 *
 * <b>NOTE:</b> The insert operation will replace a previously
 * inserted item with the same key. If no item is found, and insert
 * will take place, otherwise previous item is replaced calling to the
 * key destroy and data destroy defined.
 * 
 * @param hash The hash table where the insert operation will be
 * produced.
 *
 * @param key The key to store in the hash table. If the key is found,
 * previous data is replaced, storing this new key and the value
 * provided.
 *
 * @param data The data to store associated to the provided key.
 */
void      axl_hash_insert       (axlHash    * hash, 
				 axlPointer   key,
				 axlPointer   data)
{
	/* call to the full implementation */
	axl_hash_insert_full (hash, key, NULL, data, NULL);

	/* nothing more to do */
	return;
}

/** 
 * @internal Function used to create the node that will be stored in
 * the hash.
 */
#define __axl_hash_create_node(factory, node, key, key_destroy, data, data_destroy)\
node                = axl_factory_get (factory);\
node->key           = key;\
node->key_destroy   = key_destroy;\
node->data          = data;\
node->data_destroy  = data_destroy;   

/** 
 * @internal Function that performs the hash insertion for the
 * selected node.
 */
#define __axl_hash_insert_node(_pos,_hash,_key,_node,_increase)\
_pos               = (_hash->hash (_key)) % _hash->hash_size;\
_node->next        = _hash->table [_pos];\
_hash->table [pos] = _node;\
if (_increase)\
    _hash->items++;

/** 
 * @brief Inserts a key value into the provided hash table, allowing
 * to provide deallocation functions for the key and the data to be
 * stored.
 *
 * <b>NOTE:</b> The insert operation will replace a previously
 * inserted item with the same key. If no item is found, and insert
 * will take place, otherwise previous item is replaced calling to the
 * key destroy and data destroy defined.
 * 
 * @param hash The hash table where the data will be added.
 *
 * @param key The key to store in the hash table. If the key is found,
 * previous data is replaced, storing this new key and the value
 * provided.
 *
 * @param key_destroy An optional destroy function that will be called
 * to deallocate the key provided. 
 *
 * @param data The data to store associated to the provided key.
 *
 * @param data_destroy An optional destroy function that will be
 * called to deallocate the data provided.
 */
void       axl_hash_insert_full (axlHash        * hash,
				 axlPointer       key,
				 axlDestroyFunc   key_destroy,
				 axlPointer       data,
				 axlDestroyFunc   data_destroy)
{
	int           pos       = 0;
	axlHashNode * node      = NULL;
	axlHashNode * aux       = NULL;
	int           iterator  = 0;

	/* check incoming data */
	axl_return_if_fail (hash);
	axl_return_if_fail (key);

	/* check the really basic case where the hash has no data
	 * stored yet. */
	if (hash->hash_size == 0) {
		/* allocate memory for the hash */
		hash->table     = axl_new (axlHashNode *, hash->step);
		hash->hash_size = hash->step;
		
		/* create the node to store */
		__axl_hash_create_node (hash->factory, node, key, key_destroy, data, data_destroy);

		/* insert the node into the hash */
		__axl_hash_insert_node (pos, hash, key, node, true);

		return;
	} 

	/* now check for a more complex case */
	node = __axl_hash_internal_lookup (hash, key);

	/* if the node is not found and the hash size can't hold more items, expand it and rehash */
	if ((hash->hash_size == hash->items) && (node == NULL)) {
		/* seems we need to rehash items, reallocating enough
		 * memory */

		/* before allocating more memory, extract all items to
		 * be reallocated */
		iterator = 0;
		node     = NULL;
		while (iterator < hash->hash_size) {
			
			/* check for content */
			if (hash->table[iterator] != NULL) {
				/* check if node has been initialized,
				 * and set it to the first position */
				if (node == NULL) {
					/* configure init node position */
					node = hash->table[iterator];

					/* and last aux position */
					aux = node;
					while (aux->next != NULL) {
						/* update reference */
						aux = aux->next;
					}
					
				} else {
					/* make aux to point to the next list */
					aux->next = hash->table[iterator];
					
					/* and while the last item is not
					 * reached, move aux to point to the
					 * last item of the chain */
					while (aux->next != NULL) {
						/* update reference */
						aux = aux->next;
					}
				}
			} /* end if */

			/* update the iterator */
			iterator++;
		}
	
		/* now we have in node a complete list of items
		 * stored, allocate more memory and rehash */
		hash->hash_size += hash->step;
		hash->table      = realloc (hash->table, sizeof (axlHashNode *) * (hash->hash_size));

		/* clear the table */
		memset (hash->table, 0, sizeof (axlHashNode*) * (hash->hash_size));

		/* for each item inside the list rehash it */
		while (node != NULL) {
			/* store next item to rehash in aux */
			aux = node->next;

			/* insert the node into the hash */
			__axl_hash_insert_node (pos, hash, node->key, node, false);
			
			/* update the reference */
			node = aux;
		} /* end while */

		/* clear node reference */
		node = NULL;
	} /* rehashing if end */

	/* we have enough space to store the item create the
	   node to store */
	if (node == NULL) {
		/* create a new node */
		__axl_hash_create_node (hash->factory, node, key, key_destroy, data, data_destroy);

		/* insert the node into the hash as usual */
		__axl_hash_insert_node (pos, hash, key, node, true);
	} else {
		/* don't create a node, replace previous content and use new content */
		if (node->key_destroy != NULL) {
			node->key_destroy (node->key);
		}
		if (node->data_destroy != NULL) {
			node->data_destroy (node->data);
		}

		/* now use new data */
		node->key          = key;
		node->key_destroy  = key_destroy;
		node->data         = data;
		node->data_destroy = data_destroy;
	}

	/* nothing more man!! */
	return;
}

/** 
 * @brief Allows to remove the selected pair key/value on the provided
 * hash table.
 * 
 * The function will remevo the item but it will not resize the table
 * due to it. The function will call to the key destroy and data
 * destroy function if they were defined at the insertion time (\ref
 * axl_hash_insert_full).
 *
 * 
 * @param hash The hash table where the removal operation will be
 * performed.
 *
 * @param key The key to lookup to be removed. 
 */
void            axl_hash_remove       (axlHash    * hash,
				       axlPointer   key)
{
	axlHashNode * node;
	axlHashNode * aux;
	int           pos;

	axl_return_if_fail (hash);
	axl_return_if_fail (key);
	
	/* do not perform any operation if the hash is empty */
	if (hash->hash_size == 0)
		return;
	
	/* get the node at the provided position */
	pos  = (hash->hash (key)) % hash->hash_size;
	node = hash->table [pos];

	/* node not found */
	if (node == NULL)
		return;

	/* check for equal keys */
	if (hash->equal (node->key, key) == 0) {
		/* set that no items are available at the provided
		 * position */
		hash->table [pos] = node->next;

	remove_element:
		/* key destruction is defined */
		if (node->key_destroy != NULL)
			node->key_destroy (node->key);
		
		/* if data destruction is defined */
		if (node->data_destroy != NULL)
			node->data_destroy (node->data);

		/* decreases elements found */
		hash->items--;
					
		/* delete the node */
		/* axl_free (node); */

		/* element destroyed, nothing more to do around
		 * here */
		return;
	}

	/* seems we have more nodes */
	while (node->next != NULL) {
		/* store a reference to the current node (which will
		 * be the previous on next sentences) */
		aux  = node;
		
		/* update the reference to the next node */
		node = node->next;		

		/* check for equal keys */
		if (hash->equal (node->key, key) == 0) {
			/* make previous node to point to the next
			 * element of the following node */
			aux->next = node->next;
			
			goto remove_element;
		}
	}  /* end */
	
	/* no item was found on the hash */
	return;
}

/** 
 * @brief Allows to get the content associated to the key provided
 * inside the hash provided.
 * 
 * @param hash The hash table where the lookup will be performed.
 *
 * @param key The key to use to retrieve the information.
 * 
 * @return NULL or the associated data to the key provided. The
 * function will also return a NULL value if provided a NULL hash
 * reference or a NULL key reference.
 */
axlPointer axl_hash_get         (axlHash * hash, 
				 axlPointer key)
{
	axlHashNode * node;

	axl_return_val_if_fail (hash, NULL);

	/* lookup using internal function */
	node =  __axl_hash_internal_lookup (hash, key);

	/* return the content or NULL if not defined the node */
	if (node != NULL)
		return node->data;
	return NULL;
}

/** 
 * @internal
 * 
 * Common implementation for both foreach functions: axl_hash_foreach
 * and axl_hash_foreach2.
 */
void __axl_hash_foreach (axlHash             * hash, 
			 axlHashForeachFunc    func, 
			 axlHashForeachFunc2   func2, 
			 axlHashForeachFunc3   func3, 
			 axlHashForeachFunc4   func4,
			 axlPointer            user_data, 
			 axlPointer            user_data2,
			 axlPointer            user_data3,
			 axlPointer            user_data4)
{

	int           iterator = 0;
	axlHashNode * node;

	/* perform some basic checks */
	axl_return_if_fail (hash);

	/* foreach item row inside the hash table */
	while (iterator < hash->hash_size) {
		
		/* check for content */
		if (hash->table[iterator] != NULL) {
			/* get the first item inside the table */
			node = hash->table[iterator];

			do {
				/* check for one user defined pointer
				 * foreach: notify the item found */
				if (func != NULL && func (node->key, node->data, user_data)) {
					/* user have decided to stop */
					return;
				} /* end if */

				/* check for two user defined pointers
				 * notify the item found */
				if (func2 != NULL && func2 (node->key, node->data, user_data, user_data2)) {
					/* user have decided to stop */
					return;
				} /* end if */

				/* check for three user defined pointers
				 * notify the item found */
				if (func3 != NULL && func3 (node->key, node->data, user_data, user_data2, user_data3)) {
					/* user have decided to stop */
					return;
				} /* end if */

				/* check for four user defined
				 * pointers notify the item found */
				if (func4 != NULL && func4 (node->key, node->data, user_data, user_data2, user_data3, user_data4)) {
					/* user have decided to stop */
					return;
				} /* end if */
				
				/* next item inside the same collition
				 * position */
				node = node->next;

				/* while the next item is not null,
				 * keep on iterating */
			} while (node != NULL);
				    
		} /* end if */
		
		/* update the iterator */
		iterator++;
		
	} /* end while */
	
	return;
}

/** 
 * @brief Performs a foreach operation over all items stored in the
 * hash provided.
 * 
 * The function provided (<b>func</b>) will be called passing in the
 * item found, and the data provided (third argument). 
 * 
 * Because the \ref axlHashForeachFunc function is used, \ref true must be
 * returned to stop the foreach process. In the case all items must be
 * visited, \ref false must be returned in any case.
 * 
 * @param hash The hash table where the iteration process will take
 * place.
 *
 * @param func The function to call for each item found.
 *
 * @param user_data User defined data to be passed in to the function callback along with the item found.
 */
void            axl_hash_foreach      (axlHash            * hash, 
				       axlHashForeachFunc   func, 
				       axlPointer           user_data)

{

	/* perform the foreach operation using common support */
	__axl_hash_foreach (hash, 
			    /* foreach function */
			    func, NULL, NULL, NULL, 
			    /* user defined data */
			    user_data, NULL, NULL, NULL);

	return;
}

/** 
 * @brief Allows to perform a foreach operation providing two user
 * defined pointer to be passed to the foreach function for each item
 * found.
 *
 * This function works like \ref axl_hash_foreach function but support
 * two user defined pointers. See \ref axl_hash_foreach for more information.
 * 
 * @param hash The hash where the iteration will be performed.
 * 
 * @param func The foreach function that will be called for all nodes
 * found passed in both pointers defined along with the key value and
 * the value associated.
 *
 * @param user_data User defined data to be passed to the foreach
 * function.
 *
 * @param user_data2 Second User defined data to be passed to the
 * foreach function.
 */
void            axl_hash_foreach2     (axlHash            * hash, 
				       axlHashForeachFunc2  func, 
				       axlPointer           user_data,
				       axlPointer           user_data2)

{
	/* perform the foreach operation using common support */
	__axl_hash_foreach (hash, 
			    /* foreach function */
			    NULL, func, NULL, NULL,
			    /* user defined data */
			    user_data, user_data2, NULL, NULL);

	return;
}

/** 
 * @brief Three user defined pointers foreach function over a hash.
 *
 * See \ref axl_hash_foreach2 and \ref axl_hash_foreach3 for more
 * information.
 * 
 * @param hash The hash where the foreach operation will take place.
 *
 * @param func The function to be called for each item found in the
 * hash.
 *
 * @param user_data The user defined pointer to be configured in the
 * hash.
 *
 * @param user_data2 Second user defined pointer to be configured in
 * the hash.
 *
 * @param user_data3 Third user defined pointer to be configured in
 * the hash.
 */
void            axl_hash_foreach3     (axlHash            * hash, 
				       axlHashForeachFunc3  func, 
				       axlPointer           user_data,
				       axlPointer           user_data2,
				       axlPointer           user_data3)
{
	/* perform the foreach operation using common support */
	__axl_hash_foreach (hash, 
			    /* foreach function */
			    NULL, NULL, func, NULL,
			    /* user defined data */
			    user_data, user_data2, user_data3, NULL);
}

/** 
 * @brief Four user defined pointers foreach function over a hash.
 *
 * See \ref axl_hash_foreach2 and \ref axl_hash_foreach3 for more
 * information.
 * 
 * @param hash The hash where the foreach operation will take place.
 *
 * @param func The function to be called for each item found in the
 * hash.
 *
 * @param user_data The user defined pointer to be configured in the
 * hash.
 *
 * @param user_data2 Second user defined pointer to be configured in
 * the hash.
 *
 * @param user_data3 Third user defined pointer to be configured in
 * the hash.
 *
 * @param user_data4 Forth user defined pointer to be configured in
 * the hash.
 */
void            axl_hash_foreach4     (axlHash              * hash, 
				       axlHashForeachFunc4    func, 
				       axlPointer             user_data,
				       axlPointer             user_data2,
				       axlPointer             user_data3,
				       axlPointer             user_data4)
{
	/* perform the foreach operation using common support */
	__axl_hash_foreach (hash, 
			    /* foreach functions */
			    NULL, NULL, NULL, func, 
			    /* user defined data */
			    user_data, user_data2, user_data3, user_data4);
}

/** 
 * @brief Allows to check if the provided key is found on the given
 * hash table.
 *
 * The function allows to get if the key is found on the table pretty
 * much the behaviour that we could get using the following:
 * \code
 * // just compare if the provided key returns some value 
 * bool value = (axl_hash_get (hash, "key2") != NULL);
 * \endcode
 *
 * However it could happen that the value associated to the key, which
 * already exists, is a NULL pointer, making previous comparation to
 * not work in all cases. This function allows to check for the
 * existance of a key and its associated data no mather what is the
 * value of the associated data.
 * 
 * @param hash The hash table to check for a key value.
 * @param key The key to check for its existance.
 * 
 * @return true if the key is found, otherwise false is returned.
 */
bool            axl_hash_exists       (axlHash    * hash,
				       axlPointer   key)
{
	/* check if the hash is provided without loggin an error */
	return (__axl_hash_internal_lookup (hash, key) != NULL);
}

/** 
 * @internal function for axl_hash_copy.
 */
bool __axl_hash_copy_foreach (axlPointer key,       
			      axlPointer data, 
			      /* user defined pointers */
			      axlPointer user_data,  /* hash       */
			      axlPointer user_data2, /* result     */
			      axlPointer user_data3, /* key_copy   */
			      axlPointer user_data4) /* value_copy */
{
	/* get a reference to the received data */
	axlHash          * hash       = user_data;
	axlHash          * result     = user_data2;
	axlHashItemCopy    key_copy   = user_data3;
	axlHashItemCopy    value_copy = user_data4;
	
	/* additional variables */
	axlHashNode * node;
	
	/* get node to copy */
	node = hash->table [(hash->hash (key)) % hash->hash_size];

	/* check this is the node to copy */
	while (node != NULL) {
		if (hash->equal (node->key, key) == 0)
			break;

		/* it isn't the node looked up */
		node = node->next;
	} /* end while */

	/* copy */
	axl_hash_insert_full (result, 
			      /* insert the key and its destroy function. */
			      key_copy (node->key, node->key_destroy, node->data, node->data_destroy),   node->key_destroy,
			      /* insert data and its destroy function. */
			      value_copy (node->key, node->key_destroy, node->data, node->data_destroy), node->data_destroy);
	
	/* make the foreach process to continue until the last element */
	return false;
}

/** 
 * @brief Allows to copy the provided hash, providing the copy
 * function used to duplicate key and value items stored.
 *
 * The function are optional, so, if they are null, the same value is
 * stored in the hash (for the key and the value). In this case, if
 * the source hash has defined destroy function for either key or
 * values, they will not be configured in the returning hash.
 *
 * If function are provided, \ref axl_hash_copy will use it to get a
 * duplicated version for either the key or the value. In this case,
 * if the source hash has defined the destroy function either for the
 * key or the value, it will be configured in the returning hash.
 * 
 * @param hash The \ref axlHash that will work as data source.
 *
 * @param key_copy The function to be used to duplicate keys.
 *
 * @param value_copy The function used to duplicate values.
 * 
 * @return A newly allocated reference to a \ref axlHash containing
 * all values from the source hash. The function will fail if the hash
 * provided is a null reference or copy functions aren't provided.
 */
axlHash       * axl_hash_copy         (axlHash         * hash,
				       axlHashItemCopy   key_copy,
				       axlHashItemCopy   value_copy)
{
	axlHash         * result;
	
	/* return if the hash reference is null */
	axl_return_val_if_fail (hash, NULL);
	axl_return_val_if_fail (key_copy, NULL);
	axl_return_val_if_fail (value_copy, NULL);

	/* create the hash */
	result = axl_hash_new_full (hash->hash, 
				    hash->equal,
				    /* make initial step to be equal
				     * to the current hash size copied
				     * to avoid resizing operations
				     * during the foreach. */
				    hash->items);
	/* restore step */
	result->step = hash->step;

	/* check empty hash value */
	if (hash->hash_size == 0)
		return result;

	/* copy all items */
	axl_hash_foreach4 (hash, __axl_hash_copy_foreach, hash, result, key_copy, value_copy);


	/* return created hash */
	return result;
}
	
/** 
 * @brief Returns the number of items already stored on the provided
 * hash. 
 * 
 * @param hash The hash that is being requested for the number of
 * indexed data items.
 * 
 * @return The number of items or -1 if it fails.
 */		
int             axl_hash_items        (axlHash * hash)
{
	axl_return_val_if_fail (hash, -1);
	
	/* return current items stored */
	return hash->items;
}

/** 
 * @internal Shows current hash status to the console.
 * 
 * The function is only useful for internal hash module purposes. It
 * shows the numbers of items stored, the table size, the number of
 * collitions, etc.
 * 
 * @param hash The hash where the status is requested.
 */
void            axl_hash_show_status  (axlHash * hash)
{
	/* use full implementation */
	axl_hash_show_status_full (hash, NULL);

	return;
}

/** 
 * @internal Shows current hash content to the console using the
 * provided function to show the hash content.
 * 
 * @param hash The hash that is requested to show its content.
 *
 * @param show_item The function to be used to show the content.
 */
void            axl_hash_show_status_full (axlHash * hash, 
					   axlHashPrintKeyData show_item)
	{
	axlHashNode * node;
	int           iterator;
	int           count;
	axl_return_if_fail (hash);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "axl hash table status:");
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "table size: %d   items: %d   step: %d",
		   hash->hash_size, hash->items, hash->step);
	/* get the number of empty blocks */
	iterator = 0;
	count    = 0;
	while (iterator < hash->hash_size) {
		/* empty item found */
		if (hash->table[iterator] == NULL) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "empty cell at: %d", iterator);
			count++;
		}

		/* update iterator */
		iterator++;
	}
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "number of empty cells: %d", count);

	/* get the number properly used cells (no collitions) */
	iterator = 0;
	count    = 0;
	while (iterator < hash->hash_size) {
		/* empty item found */
		node = hash->table[iterator];
		if (node != NULL && node->next == NULL) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "properly used cell at: %d", iterator);
			count++;

			if (show_item != NULL) {
				show_item (node->key, node->data);
			}
		}

		/* update iterator */
		iterator++;
	}
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "number of properly used cells (no collition): %d", count);

	/* get the number of collitioned cells */
	iterator = 0;
	count    = 0;
	while (iterator < hash->hash_size) {
		/* empty item found */
		node = hash->table[iterator];
		if (node != NULL && node->next != NULL) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "collitioned cell at: %d", iterator);
			count++;
		}

		/* for each node show the content */
		while ((show_item) != NULL && (node != NULL)) {
			if (show_item != NULL) {
				show_item (node->key, node->data);
			}
			
			/* update to the next node */
			node = node->next;
		}

		/* update iterator */
		iterator++;
	}
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "number of collitioned cells: %d", count);


	
	return;
}

/** 
 * @brief Allows to deallocate the hash provided.
 * 
 * @param hash The hash to deallocate.
 */
void       axl_hash_free        (axlHash *  hash)
{
	int iterator = 0;
	axlHashNode * node;
	axlHashNode * aux;

	/* do not perform any operation if a null reference is
	 * received */
	if (hash == NULL)
		return;

	/* release the hash table */
	if (hash->table != NULL) {
		
		/* first release all nodes inside */
		while (iterator < hash->hash_size) {
			node = hash->table[iterator];
			if (node != NULL) {
				do {
					/* key destruction is defined */
					if (node->key_destroy != NULL)
						node->key_destroy (node->key);
					
					/* if data destruction is defined */
					if (node->data_destroy != NULL)
						node->data_destroy (node->data);
					
					/* check data */
					aux  = node;
					node = node->next;
					
					/* while more nodes */
				} while (node != NULL);
			} /* end if */
			
			/* update the iterator */
			iterator++;
		} /* end while */

		/* now release the table */
		axl_free (hash->table);
	} /* end if */

	/* free factory */
	axl_factory_free (hash->factory);

	/* now release the hash itself */
	axl_free (hash);

	/* nothing more to free */
	return;
}

/* @} */

/**
 * \defgroup axl_hash_cursor_module Axl Hash Cursor: Iterator support for the Axl Hash
 */

/* init the cursor */
void __axl_hash_cursor_init (axlHashCursor * cursor, bool first)
{
	/* pointer to a hash node (basic atom holding key and value) */
	axlHashNode   * node = NULL;

	if (first) {
		/* configure first */
		cursor->index = 0;

		/* foreach element inside the has, check the first value */
		while (cursor->index < cursor->hash->hash_size) {
			/* check the table at the current position */
			node = cursor->hash->table[cursor->index];
			if (node != NULL) {
				/* first node found, store it */
				cursor->node = node;
				break;
			} /* end if */
			
			/* node not found, go next position */
			cursor->index++;
		} /* end while */
	} else {
		/* find last value */
		cursor->index = cursor->hash->hash_size - 1;
		cursor->node  = NULL;
		while (cursor->index > 0) {
			/* check the table at the current position */
			node = cursor->hash->table[cursor->index];
			if (node != NULL) {
				/* find last entry filled, now find
				 * last entry in the same index */
				while (node->next != NULL)
					node = node->next;
				
				/* configure it */
				cursor->node = node;
				break;
			} /* end if */
			
			/* node not found, go next position */
			cursor->index--;
		} /* end while */
	} /* end if */
		
	/* if the node wasn't found, reset index */
	if (node == NULL)
		cursor->index = 0;

	/* cursor initialized */
	return;
}

/** 
 * \addtogroup axl_hash_cursor_module
 * @{
 */

/** 
 * @brief Allows to get a cursor to iterate the hash in a linear and
 * efficient way.
 *
 * The \ref axlHashCursor could be used to iterate an \ref axlHash in
 * an efficient way because it stores current state (position), hiding
 * all module details. Then using the following functions you can
 * modify the state (current position to get):
 * 
 *   - \ref axl_hash_cursor_first
 *   - \ref axl_hash_cursor_last
 *   - \ref axl_hash_cursor_next
 *
 * Finally, the following functions are provided to get the data stored at a
 * particular position, pointed by the current status of the cursor:
 * 
 *   - \ref axl_hash_cursor_get_key (returns the key of the current position)
 *   - \ref axl_hash_cursor_get_value (returns the value of the current position)
 *
 * You are allowed to remove elements from the hash (\ref axlHash)
 * having a cursor created (\ref axlHashCursor), using \ref
 * axl_hash_cursor_remove. 
 *
 * Here is an example:
 * \code
 * axlPointer      key;
 * axlPointer      value;
 * axlHashCursor * cursor;
 * 
 * // create the cursor 
 * cursor   = axl_hash_cursor_new (hash);
 *
 * // while there are more elements 
 * while (axl_hash_cursor_has_item (cursor)) {
 *
 *   // get the value and key
 *   value = axl_hash_cursor_get_key   (cursor);
 *   value = axl_hash_cursor_get_value (cursor);
 *
 *   // get the next 
 *   axl_hash_cursor_next (cursor);
 *
 * } 
 *
 * // free the cursor 
 * axl_hash_cursor_free (cursor);
 * \endcode
 * 
 * @param hash The hash that the new cursor (\ref axlHashCursor) will
 * provide access.
 * 
 * @return A newly created \ref axlHashCursor used to iterate the
 * hash. Once finished you must call to \ref axl_hash_cursor_free.
 */
axlHashCursor * axl_hash_cursor_new      (axlHash * hash)
{
	axlHashCursor * cursor;

	axl_return_val_if_fail (hash, NULL);

	/* create the cursor */
	cursor = axl_new (axlHashCursor, 1);

	/* initial configuration */
	cursor->hash    = hash;

	/* init the cursor */
	__axl_hash_cursor_init (cursor, true);

	/* return cursor */
	return cursor;
}

/** 
 * @brief Allows to configure the cursor to point to the first item of
 * the hash (if there are any).
 * 
 * @param cursor The cursor that is required to be configured to point the first item.
 */
void            axl_hash_cursor_first    (axlHashCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* init the cursor at the initial state */
	__axl_hash_cursor_init (cursor, true);

	return;
}

/** 
 * @brief Allows to configure the cursor to point to the last item of
 * the hash (if there are any).
 * 
 * @param cursor The cursor that is required to be configured to point
 * to the last item.
 */
void            axl_hash_cursor_last     (axlHashCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* init the cursor at the initial state, selecting the last
	 * node */
	__axl_hash_cursor_init (cursor, false);

	return;
}

/** 
 * @brief Allows to configure the cursor to point to the next item of
 * the hash (if there are any).
 * 
 * @param cursor The cursor that is required to be configured to point
 * to the next item.
 */
void            axl_hash_cursor_next     (axlHashCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* check if the current node is null and do nothing if nothing
	 * is found */
	if (cursor->node == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "no nodes found....");
		return;
	}

	/* basic case, the item is found in the same level */
	if (cursor->node->next != NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting next node....");
		/* configure new current node */
		cursor->node = cursor->node->next;

		return; 
	} /* end if */

	/* node not found, go next position */
	cursor->index++;

	/* seems next is null, see in other positions  */
	while (cursor->index < cursor->hash->hash_size) {
		/* check the table at the current position */
		cursor->node = cursor->hash->table[cursor->index];
		if (cursor->node != NULL) {
			/* node found, stop! */
			break;
		} /* end if */
			
		/* node not found, go next position */
		cursor->index++;
	} /* end while */
	
	/* nothing to be done */
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
bool            axl_hash_cursor_has_next (axlHashCursor * cursor)
{
	int iterator;
	axl_return_val_if_fail (cursor, false);

	/* check basic case */
	if (cursor->node != NULL && cursor->node->next != NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "next is defined..");
		return true;
	}

	/* seems next is null, see in other positions  */
	iterator = cursor->index + 1;
	while (iterator < cursor->hash->hash_size) {
		/* check the table at the current position */
		if (cursor->hash->table[iterator] != NULL)
			return true;
			
		/* node not found, go next position */
		iterator++;
	} /* end while */

	return false;
}

/** 
 * @brief Allows to know if the current position has items.
 * 
 * @param cursor The cursor pointing to a particular element inside
 * the hash (or not).
 * 
 * @return \ref true if the hash that is iterated can return data at
 * the current position, otherwise \ref false is returned.
 */
bool            axl_hash_cursor_has_item    (axlHashCursor * cursor)
{
	axl_return_val_if_fail (cursor, false);

	/* return true if there are a item selected */
	return (cursor->node != NULL);
}

/** 
 * @brief Allows to remove current element pointed by the cursor,
 * maintainig internal state of the cursor, calling to the destroy
 * function associated in the hash.
 *
 * The function will call to the destroy function asociated to the
 * hash. 
 * 
 * @param cursor The cursor pointing to the item inside the hash that
 * must be removed.
 */
void            axl_hash_cursor_remove       (axlHashCursor * cursor)
{
	axlHashNode * node;

	axl_return_if_fail (cursor);

	/* if current cursor is pointing nowhere, just do nothing */
	if (cursor->node == NULL)
		return;

	/* remember node */
	node = cursor->node->next;

	/* remove node selected */
	axl_hash_remove (cursor->hash, cursor->node->key);

	/* configure next item */
	cursor->node = node;
	if (cursor->node == NULL) {
		while (cursor->index < cursor->hash->hash_size) {
			/* check the table at the current position */
			if (cursor->hash->table[cursor->index] != NULL) {
				/* configure the next node */
				cursor->node = cursor->hash->table[cursor->index];
				return;
			}
			
			/* node not found, go next position */
			cursor->index++;
		} /* end while */
	} /* end if */

	return;
}

/** 
 * @brief Allows to get current value at the current cursor state.
 * 
 * @param cursor The cursor that will be used to return the data
 * located at the hash, using cursor current state.
 */
axlPointer      axl_hash_cursor_get_value    (axlHashCursor * cursor)
{
	axl_return_val_if_fail (cursor, NULL);

	/* nothing to return if current is NULL */
	if (cursor->node == NULL)
		return NULL;

	/* return data */
	return cursor->node->data;
}

/** 
 * @brief Allows to get current key at the current cursor state.
 * 
 * @param cursor The cursor that will be used to return the data
 * located at the hash, using cursor current state.
 */
axlPointer      axl_hash_cursor_get_key    (axlHashCursor * cursor)
{
	axl_return_val_if_fail (cursor, NULL);

	/* nothing to return if current is NULL */
	if (cursor->node == NULL)
		return NULL;

	/* return key pointer */
	return cursor->node->key;
}

/** 
 * @brief Allows to get the reference to the hash that is associated
 * to the cursor received.
 * 
 * @param cursor The cursor that is required to return the hash associated.
 * 
 * @return A reference to the hash being iterated or NULL if fails.
 */
axlHash       * axl_hash_cursor_hash         (axlHashCursor * cursor)
{
	/* check incoming cursor */
	axl_return_val_if_fail (cursor, NULL);

	/* return the hash */
	return cursor->hash;
}

/** 
 * @brief Deallocates memory used by the cursor. 
 *
 * @param cursor The cursor to be deallocated.
 */
void            axl_hash_cursor_free     (axlHashCursor * cursor)
{
	axl_return_if_fail (cursor);

	/* free the cursor */
	axl_free (cursor);

	return;
}


/* @} */
