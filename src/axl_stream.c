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
#include <axl_stream.h>
#include <stdarg.h>
#include <string.h>

struct _axlStream {
	/* current stream content */
	char * stream;

	/* where the current stream is positioned. */
	int    stream_index;
	
	/* current stream size */
	int    stream_size;
	
	/* previous inspected stream size */
	int    previous_inspect;

	/* last chunk get from the stream */
	char * last_chunk;
};


/** 
 * @internal
 * 
 * @brief Creates a new byte stream using as data the string pointer
 * and the size.
 *
 * 
 * @param stream_source A pointer to the memory where the data to be
 * streamed is located.
 *
 * @param stream_size How many bytes are available to perform
 * streaming.
 * 
 * @return A newly allocated stream instance that should be
 * deallocated by using \ref axl_stream_free. The function could
 * return a NULL value is received a NULL stream or a non positive
 * stream size.
 */
axlStream * axl_stream_new (char * stream_source, int stream_size)
{
	axlStream * stream;

	/* perform some environmental checkings */
	axl_return_val_if_fail (stream_source,   NULL);
	axl_return_val_if_fail (stream_size > 0, NULL);

	stream               = axl_new (axlStream, 1);
	/* copy source */
	stream->stream       = axl_new (char, stream_size + 1);
	memcpy (stream->stream, stream_source, stream_size);
	stream->stream_index = 0;
	stream->stream_size  = stream_size;

	/* return newly created stream */
	return stream;
}

/** 
 * @internal
 * @brief Allows to perform an inspection of the given chunk on the given stream.
 *
 * The <i>chunk</i> will be checked to apper inside the stream. This
 * means that, having the stream as its state, the chunk should be
 * found the very begining of the stream.
 * 
 * @param stream 
 * @param chunk 
 * 
 * @return 1 if the chunk is found inside the given stream, othersize
 * 0 is returned.  -1 means that no more stream is left to satify the
 * operation. -2 means that the parameters received are wrong either
 * because stream is a NULL reference or because chunk is the same.
 */
int         axl_stream_inspect (axlStream * stream, char * chunk)
{
	int inspected_size;

	axl_return_val_if_fail (stream, -2);
	axl_return_val_if_fail (chunk, -1);

	/* accept previous inspection if a call to axl_stream_accept
	 * weren't done */
	if (stream->previous_inspect != 0)
		axl_stream_accept (stream);

	/* get current size to inspect */
	inspected_size = strlen (chunk);
       
	/* check that chunk to inspect doesn't fall outside the stream
	 * boundaries */
	if ((inspected_size + stream->stream_index) > stream->stream_size)
		return -1; /* no more stream is left to satisfy current petition */
	
	/* check that the chunk to be search is found */
	if (! memcmp (chunk, stream->stream + stream->stream_index, inspected_size)) {

		/* chunk found!, remember that the previous inspect
		 * size so we can make the stream to roll on using
		 * this value */
		stream->previous_inspect = inspected_size;
		return 1;
	}
	
	/* return that the stream chunk wasn't found */
	return 0;
}

/** 
 * @internal 
 *
 * @brief Allows to perform several, not excluyen inspect operations,
 * over the given stream.
 *
 * @param stream The stream where the operation will be performed.
 *
 * @param chunk_num The chunk number to inspect.
 * 
 * @return The function returns 0 if no chunk is found inside the
 * given stream. Otherwise, N is returned to denote that the the Nth
 * chunk was found. -1 is returned if no more stream is left to
 * satisfy the operation. -2 means that the parameters received are
 * wrong either because stream is NULL or any other parameter.
 */
int         axl_stream_inspect_several (axlStream * stream, int chunk_num, ...)
{
	va_list   args;
	int       iterator  = 0;
	char    * chunk     = NULL;

	axl_return_val_if_fail (stream,        -1);
	axl_return_val_if_fail (chunk_num > 0, -1);

	va_start (args, chunk_num);
	
	/* check each chunk */
	while (iterator < chunk_num) {

		/* get the next chunk */
		chunk = va_arg (args, char *);

		/* check the chunk read */
		if (axl_stream_inspect (stream, chunk)) {
			va_end (args);
			return (iterator + 1);
		}
		
		/* update the iterator */
		iterator ++;
	}

	/* close va arg */
	va_end (args);

	/* return that no chunk was found */
	return 0;
}

/** 
 * @internal
 *
 * @brief Accept previous inspected chunk to be consumed and moves
 * current stream current the size equal to the chunk inspected.
 * 
 * @param stream The stream where the byte inspected size will be
 * accepted.
 */
void axl_stream_accept (axlStream * stream)
{
	axl_return_if_fail (stream);

	stream->stream_index     += stream->previous_inspect;
	stream->previous_inspect  = 0;
	if (stream->last_chunk != NULL)
		axl_free (stream->last_chunk);
	stream->last_chunk = NULL;

	return;
}

/** 
 * @internal
 *
 * @brief Allows to move internal stream index status the amount
 * provided by <b>count</b>.
 *
 * 
 * @param stream The stream where the operation will be performed.
 *
 * @param count Count to move internal stream index.
 */
void        axl_stream_move            (axlStream * stream, int count)
{
	axl_return_if_fail (stream);
	stream->stream_index += count;
	
	return;
}

/** 
 * @internal
 * @brief Returns the next chunk available on the stream.
 *
 * This function allows to get next available chunk, validating it
 * with provided valid_chars variable, until the chunk provided are
 * found.
 *
 * Currently, valid_chars is not used, so, the chunk returned is not
 * validated against the value provided.
 * 
 * As an example if it is required to get the encoding content, you
 * could do the next call:
 * 
 * \code
 * char * result;
 * 
 * // get the next chunk until a " or ' is found
 * result = axl_stream_get_until (stream, NULL, 2, "\"", "'");
 * 
 * \endcode
 *
 * Value returned from this function mustn't be deallocated.
 * 
 * 
 * @param stream The stream were the chunk will be extracted.
 *
 * @param valid_chars The valid set of characters, to validate content
 * to be returned. Currently this is not implemented.
 *
 * @param chunk_num 
 * 
 * @return 
 */
char      * axl_stream_get_until       (axlStream * stream, 
					char      * valid_chars, 
					int         chunk_num, ...)
{
	char      ** chunks;
	va_list      args;
	int          iterator = 0;
	int          index    = 0;
	
	/* perform some environmental checks */
	axl_return_val_if_fail (stream, NULL);
	/* axl_return_val_if_fail (valid_chars, NULL); */
	axl_return_val_if_fail (chunk_num > 0, NULL);

	/* get chunks to lookup */
	chunks = axl_new (char *, chunk_num);
	
	/* begin std args  */
	va_start (args, chunk_num);
	
	/* iterate over the chunk list */
	while (iterator < chunk_num) {
		/* get the chunk */
		chunks[iterator] = va_arg (args, char *);
		iterator ++;
	}

	/* end std arg */
	va_end (args);

	/* now we have chunks to lookup, stream until get the stream
	 * limited by the chunks received. */
	while ((index + stream->stream_index) < stream->stream_size) {

		/* compare chunks received for each index increased
		 * one step */
		iterator = 0;
		while (iterator < chunk_num) {

			/* check if we have found the chunk we were looking */
			if (!memcmp (chunks[iterator], stream->stream + index + stream->stream_index, 
				     strlen (chunks [iterator]))) {

				/* chunk found */
				axl_free (chunks);
				
				/* result is found from last stream
				 * index read up to index */
				if (stream->last_chunk != NULL)
					axl_free (stream->last_chunk);

				/* get a copy to the chunk to be returned */
				stream->last_chunk = axl_new (char, index + 1);
				memcpy (stream->last_chunk, stream->stream + stream->stream_index, index);

				/* update internal indexes */
				stream->stream_index     += index + strlen (chunks[iterator]);
				stream->previous_inspect  = 0;

				return stream->last_chunk;
			}
			/* update iterator */
			iterator ++;
		} /* end while */

		/* it seems that the chunk wasn't found */
		index++;
	}

	/* return a NULL chunk. */
	return NULL;
}


/** 
 * @internal
 *
 * @brief Allows to deallocate memory used by the \ref axlStream
 * received.
 * 
 * @param stream The stream to be deallocated.
 */
void axl_stream_free (axlStream * stream)
{
	axl_return_if_fail (stream);

	/* release memory */
	axl_free (stream->stream);
	axl_free (stream);

	return;
}


