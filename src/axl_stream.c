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
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <axl.h>

#define LOG_DOMAIN "axl-stream"

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
	char   * last_chunk;

	/* last near to operation performed */
	char   * last_near_to;

	/* a reference to the associated document to this stream */
	axlDoc * doc;
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
 * @return The function returns the following values according to the result: 
 *
 * - <b>0</b> if the chunk wasn't found inside the stream but no error was
 * found.
 *
 * - <b>1</b> if the chunk is found inside the given stream.
 *
 * - <b>-1</b> means that no more stream is left to satify the operation.
 *
 * - <b>-2</b> means that the parameters received are wrong either
 * because stream is a NULL reference or because chunk is the same.
 */
int         axl_stream_inspect (axlStream * stream, char * chunk)
{
	int inspected_size;

	axl_return_val_if_fail (stream, -2);
	axl_return_val_if_fail (chunk, -1);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "inspecting the xml stream");


	/* get current size to inspect */
	inspected_size = strlen (chunk);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "current stream status: size=%d, index=%d, size-to-inspect=%d, chunk=[%s]",
		 stream->stream_size, stream->stream_index, inspected_size, 
		 axl_stream_is_white_space (chunk) ? "(S)" : chunk);
       
	/* check that chunk to inspect doesn't fall outside the stream
	 * boundaries */
	if ((inspected_size + stream->stream_index) > stream->stream_size) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_WARNING, "requested to inspect a chunk that falls outside the stream size");
		return -1; /* no more stream is left to satisfy current petition */
	}
	
	/* check that the chunk to be search is found */
	if (! memcmp (chunk, stream->stream + stream->stream_index, inspected_size)) {

		/* drop a log */
		if (axl_log_is_enabled ()) {
			if (axl_stream_is_white_space (chunk)) 
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "chunk [(S)] found inside the stream, saving inspected size=%d", inspected_size);
			else
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "chunk [%s] found inside the stream, saving inspected size=%d", chunk, inspected_size);
		}

		/* chunk found!, remember that the previous inspect
		 * size so we can make the stream to roll on using
		 * this value */
		stream->previous_inspect = inspected_size;

		/* accept the chunk readed */
		axl_stream_accept (stream);
		return 1;
	}

	/* drop a log */
	if (axl_log_is_enabled ()) {
		if (axl_stream_is_white_space (chunk)) 
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "chunk [(S)] doesn't found inside the stream");
		else
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "chunk [%s] doesn't found inside the stream", chunk);
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
 * @return The function returns the following values: 
 * 
 * - <b>0</b>: if no chunk is found inside the given stream, according to the
 * provided chunks.
 *
 * - <b>N</b>: is returned to denote that the Nth chunk was found.
 *
 * - <b>-1</b>: is returned if no more stream is left to satisfy the operation.
 * 
 * - <b>-2</b>: means that the parameters received are wrong either because
 * stream is NULL or any other parameter.
 */
int         axl_stream_inspect_several (axlStream * stream, int chunk_num, ...)
{
	va_list   args;
	int       iterator  = 0;
	char    * chunk     = NULL;

	axl_return_val_if_fail (stream,        -1);
	axl_return_val_if_fail (chunk_num > 0, -1);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "entering inside inspect several");

	va_start (args, chunk_num);
	
	/* check each chunk */
	while (iterator < chunk_num) {

		/* get the next chunk */
		chunk = va_arg (args, char *);

		/* check the chunk read */
		switch (axl_stream_inspect (stream, chunk)) {
		case -2:
			/* wrong parameter received */
			va_end (args);
			return -2;
		case -1:
			/* there is no more stream left */
			va_end (args);
			return -1;
		case 0:
			/* the chunk wasn't found, break and
			 * continue. */
			break;
		default:
			/* the chunk was found */
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found chunk (size: %d) [%s]", strlen (chunk), chunk);
			
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
	int          length   = 0;
	
	/* perform some environmental checks */
	axl_return_val_if_fail (stream, NULL);
	/* axl_return_val_if_fail (valid_chars, NULL); */
	axl_return_val_if_fail (chunk_num > 0, NULL);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "inside get-until, chunk-num=%d", chunk_num);

	/* get chunks to lookup */
	chunks = axl_new (char *, chunk_num + 1);
	
	/* begin std args  */
	va_start (args, chunk_num);
	
	/* iterate over the chunk list */
	while (iterator < chunk_num) {
		/* get the chunk */
		chunks[iterator] = va_arg (args, char *);
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "get-until chunk to compare read (%d)=%s", iterator, chunks[iterator]);
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
			
			/* get current length for the chunk to check */
			length = (chunks [iterator] != NULL) ?  strlen (chunks [iterator]) : 0;

			/* check if we have found the chunk we were looking */
			if (!memcmp (chunks[iterator], stream->stream + index + stream->stream_index, length)) {
				
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "chunk %s found at index %d, starting from %d",
					 chunks[iterator], index, stream->stream_index);

				/* chunk found */
				axl_free (chunks); 
				
				/* result is found from last stream
				 * index read up to index */
				if (stream->last_chunk != NULL) {
					axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "releasing previous chunk stored");
					axl_free (stream->last_chunk);
				}

				/* get a copy to the chunk to be returned */
				stream->last_chunk = axl_new (char, index + 1);
				memcpy (stream->last_chunk, stream->stream + stream->stream_index, index);

				/* update internal indexes */
				stream->stream_index     += index + length;
				stream->previous_inspect  = 0;

				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "returning chunk found: [%s]", stream->last_chunk);
				
				return stream->last_chunk;
			}
			/* update iterator */
			iterator ++;
		} /* end while */

		/* it seems that the chunk wasn't found */
		index++;
	}

	/* chunk found */
	axl_free (chunks); 

	/* return a NULL chunk. */
	return NULL;
}

/** 
 * @internal
 * @brief Returns current index status for the given stream.
 *
 * This function could be used to get current index being read for the
 * stream received.
 * 
 * @param stream The stream where the index will be reported.
 * 
 * @return The index or -1 if fails.
 */
int         axl_stream_get_index       (axlStream * stream)
{
	axl_return_val_if_fail (stream, -1);

	return stream->stream_index;
}

/** 
 * @internal
 * @brief Allows to get current stream size.
 * 
 * @param stream The stream where the stream size will be returned.
 * 
 * @return The stream size or -1 if fails. 
 */
int         axl_stream_get_size        (axlStream * stream)
{
	axl_return_val_if_fail (stream, -1);

	return stream->stream_size;
}

/** 
 * @internal
 *
 * @brief Allows to get current status of the given stream, taking the
 * current index, getting an amount of <b>count</b> bytes before
 * and after the given index.
 *
 * This function is mainly used to get a piece of the stream at the
 * given position while reporting errors. This allows to show the
 * piece of xml that is failing.
 *
 * The string return must not be deallocated. Value returned is
 * actually managed by the stream object associated.
 * 
 * @param stream The stream where the near to operation will be performed.
 * @param count The amount of bytes to take.
 * 
 * @return A string that is taken counting bytes with provided
 * <b>count</b> value starting from the index. Stream provided must be
 * not NULL and count value must be greater than 0.
 */
char      * axl_stream_get_near_to     (axlStream * stream, int count)
{
	int first_index;
	int last_index;

	axl_return_val_if_fail (stream, NULL);
	axl_return_val_if_fail (count > 0, NULL);
	
	/* get first index */
	if ((stream->stream_index - count) <= 0)
		first_index = 0;
	else
		first_index = stream->stream_index - count;

	/* get last index */
	if ((stream->stream_index + count) >= (stream->stream_size - 1) )
		last_index  = (stream->stream_size) - first_index;
	else
		last_index  = (stream->stream_index + count) - first_index;

	/* release previous near to chunk */
	if (stream->last_near_to != NULL)
		axl_free (stream->last_near_to);

	stream->last_near_to = axl_new (char, last_index + 1);
	memcpy (stream->last_near_to, stream->stream + first_index, last_index);

	/* return current near to operation */
	return stream->last_near_to;
	
}


/** 
 * @internal
 *
 * @brief Associates the given axlDoc with the given stream to be
 * life-time dependant.
 *
 * While performing the XML parsing, errors will be produced. This
 * function ensures that the axlDoc document will be released if the
 * stream is also released.
 *
 * This not only reduces the possibility to produce a memory leak also
 * allows to write less code.
 *
 * Once the stream is not useful and it is required to be released,
 * but not doing so with the \ref axlDoc instance, a call to \ref
 * axl_stream_unlink is also required.
 * 
 * @param stream The axlStream where the document will be linked to.
 *
 * @param doc The axlDoc where the document will be linked to.
 */
void axl_stream_link (axlStream * stream, axlDoc * doc) 
{
	axl_return_if_fail (stream);
	axl_return_if_fail (doc);
	
	/* that's all */
	stream->doc = doc;

	return;
}

/** 
 * @internal
 *
 * @brief Unlinks the associated \ref axlDoc instance.
 * 
 * @param stream The stream where the operation will be performed.
 */
void axl_stream_unlink (axlStream * stream)
{
	axl_return_if_fail (stream);
	
	/* clear document association */
	stream->doc = NULL;
	
	return;
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

	/* release associated document is defined. */
	if (stream->doc) 
		axl_doc_free (stream->doc);

	/* release last near to */
	if (stream->last_near_to)
		axl_free (stream->last_near_to);

	/* releaset last chunk */
	if (stream->last_chunk != NULL)
		axl_free (stream->last_chunk);

	/* release memory allocated by the stream received. */
	axl_free (stream);

	return;
}


/** 
 * @internal
 *
 * @brief Allows to check if the given chunk is a white space in the
 * same of the XML 1.0 Third edition.
 *
 * The XML standard understand the "white space", also reffered as S,
 * as the following characters: \x20 (the white space itself), \n, \r
 * and \t.
 *
 * This function allows to check if the given chunk contains a white
 * space, in the previous sense.
 * 
 * @param chunk The chunk to check
 * 
 * @return AXL_TRUE if the chunk contains a white space or AXL_FALSE
 * if not.
 */
bool        axl_stream_is_white_space  (char * chunk)
{
	/* do not complain about receive a null refernce chunk */
	if (chunk == NULL)
		return AXL_FALSE;
	
	if (! memcmp (chunk, " ", 1)) 
		return AXL_TRUE;
	if (! memcmp (chunk, "\n", 1)) 
		return AXL_TRUE;
	if (! memcmp (chunk, "\t", 1)) 
		return AXL_TRUE;
	if (! memcmp (chunk, "\r", 1)) 
		return AXL_TRUE;

	/* no white space was found */
	return AXL_FALSE;
}

/** 
 * @internal
 * @brief Allows to compare two strings pointed by 
 * 
 * @param chunk1 The string to be compared.
 *
 * @param chunk2 The second string to be compared.
 *
 * @param size The amount of bytes to be compared for the two incoming
 * values.
 * 
 * @return AXL_TRUE if both string are equal, AXL_FALSE if not. If
 * some value provided is NULL or the size to compare is not greater
 * than 0 the function will return AXL_FALSE directly.
 */
bool        axl_stream_cmp             (char * chunk1, char * chunk2, int size)
{
	/* perform some environmental condition checking */
	if (chunk1 == NULL)
		return AXL_FALSE;
	if (chunk2 == NULL)
		return AXL_FALSE;
	if (size <= 0)
		return AXL_FALSE;
	
	/* report current comparation status */
	if (!memcmp (chunk1, chunk2, size))
		return AXL_TRUE;
	return AXL_FALSE;
}


/** 
 * @internal
 *
 * @brief Allows to get current status of the stream. 
 *
 * If the is exhausted and have no more data to be read.
 * 
 * @param stream 
 * 
 * @return AXL_TRUE if the stream is exhausted or AXL_FALSE if not.
 */
bool        axl_stream_remains         (axlStream * stream)
{
	axl_return_val_if_fail (stream, AXL_FALSE);
	
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking for stream status with stream index=%d and stream size=%d",
		 stream->stream_index, stream->stream_size);
		 
	
	if (stream->stream_index == (stream->stream_size - 1))
		return AXL_TRUE;
	return AXL_FALSE;
}

/** 
 * @internal
 *
 * @brief Allows to copy the given chunk, supposing that is a properly
 * format C string that ends with a \0 value.
 *
 * This function allows to perform a copy for the given string. If a
 * copy limited by a size is required, use \ref axl_stream_strdup_n.
 * 
 * @param chunk The chunk to copy
 * 
 * @return A newly allocated string or NULL if fails.
 */
char      * axl_stream_strdup          (char * chunk)
{
	char * result;
	int    length;

	axl_return_val_if_fail (chunk, NULL);
	
	length = strlen (chunk);
	result = axl_new (char, length + 1);
	
	memcpy (result, chunk, length);

	return result;
}

/** 
 * @brief Allows to perform a copy for the <b>n</b> first bytes from
 * the <b>chunk</b> received.
 * 
 * @param chunk The chunk to copy
 *
 * @param n How many bytes to copy from the given chunk.
 * 
 * @return A newly allocated chunk, copied from the given chunk with a
 * size of <b>n</b> bytes. The function will check that the
 * <b>chunk</b> and the <b>n</b> values are not null and non-zero.
 */
char      * axl_stream_strdup_n (char * chunk, int n)
{
	char * result;

	axl_return_val_if_fail (chunk, NULL);
	axl_return_val_if_fail (n, NULL);
	
	result = axl_new (char, n + 1);
	memcpy (result, chunk, n);
	
	return result;
}

/** 
 * @brief Allows to produce an newly allocated string produced by the
 * chunk received plus arguments, using the printf-like format.
 * 
 * @param chunk The chunk to copy.
 * 
 * @return A newly allocated chunk.
 */
char      * axl_stream_strdup_printf   (char * chunk, ...)
{
	int       size;
	int       new_size;
	char    * result;
	va_list   args;
	
	axl_return_val_if_fail (chunk, NULL);

	va_start (args, chunk);

	size     = vsnprintf (NULL, 0, chunk, args);
	result   = axl_new (char, size + 1);
	new_size = vsnprintf (result, size, chunk, args);

	va_end (args);
	
	return result;
}
