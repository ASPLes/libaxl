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
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <axl.h>
#include <ctype.h>
#include <unistd.h>

#define LOG_DOMAIN "axl-stream"

/** 
 * @internal
 *
 * @brief Basic checking which allows to test if the provided size
 * could be satisfied by the current status of the provided stream.
 * 
 * @param stream The stream where the operation will be performed.
 *
 * @param size The size to check.
 * 
 * @return 1 if the size falls out side the stream limit or 0 it not.
 */
#define fall_out_side_checking(stream, size) ((size + stream->stream_index) > stream->stream_size)

/** 
 * @internal
 * 
 * This is the size of the buffer allocated while using the axlStream
 * as a representation of a streaming media, like a file
 * descriptor. This value should contain a value using the (4k * n + 1).
 * 
 * @param stream 
 */
#define STREAM_BUFFER_SIZE 4096

/** 
 * @internal
 *
 * Internal definition used to represent the maximum value used for
 * calls done to match a set of chunks. 
 */
#define MAX_INSPECTED_CHUNKS 8

typedef  enum {
	STREAM_FD,
	STREAM_MEM
}axlStreamType;

struct _axlStream {

	/* current stream content */
	char * stream;

	/* where the current stream is positioned. */
	int    stream_index;

	/* global index for the device being streamed */
	int    global_index;

	/* current stream size */
	int    stream_size;
	
	/* previous inspected stream size */
	int    previous_inspect;

	/* last chunk get from the stream */
	char   * last_chunk;
	
	/* last near to reference. */
	char   * last_near_to;

	/* last get following reference */
	char   * last_get_following;

	/* support variable for chunk matching */
	char  ** chunks;

	/* a reference to the associated element to this stream */
	axlList * elements_linked;
	
	/*The function to execute when the element must be destroyed. */ 
	axlDestroyFunc element_destroy;

	/* Stream type configuration. Information source is coming
	 * from a file descriptor of a memory address. */
	axlStreamType type;

	/* File descriptor associated with the given stream. In the
	 * case the stream if a STREAM_FD, this is the file descriptor
	 * associated. */
	int           fd;

	/* Temporal buffer used by the stream to handle prebuffering
	 * operations.
	 */
	char  temp[STREAM_BUFFER_SIZE];
};


/**
 * \defgroup axl_stream_module Axl Stream Document: Internal functions used to implement a abstract stream where a xml document is expected.
 */

/** 
 * \addtogroup axl_stream_module
 * @{
 */

/** 
 * @internal
 *
 * @brief Read the next available information on the file descriptor
 * placing that information into the stream.
 * 
 * @param stream The stream where the pre-buffering operation will be
 * performed.
 * 
 * @return AXL_TRUE if the requested padding and buffer size were
 * filled or AXL_FALSE if end of file was reached. In that case the
 * stream size is not updated.
 */
bool axl_stream_prebuffer (axlStream * stream)
{
	int  bytes_read;

	/* check some environment conditions */
	axl_return_val_if_fail (stream, AXL_FALSE);

	/* no prebuffering is the stream type is not a file descriptor
	 * source */
	if (stream->type != STREAM_FD)
		return AXL_FALSE;

	/* no prebuffering operation if the socket is closed */
	if (stream->fd == -1)
		return AXL_FALSE;

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "prebuffering the stream..");

	/* displace memory only if there were data already consumed */
	if (stream->stream_index > 0 && ((stream->stream_size - stream->stream_index) > 0)) {

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   moving previous content at the begining of the buffer, current index: %d, size: %d, current status: %s",
			 stream->stream_index, stream->stream_size - stream->stream_index,
			 stream->stream + stream->stream_index);

		/* clear the memory */
		memset (stream->temp, 0, STREAM_BUFFER_SIZE);
		
		/* displace memory already read to be at the begining
		 * of the stream */
		memcpy (stream->temp, stream->stream + stream->stream_index,
			STREAM_BUFFER_SIZE - stream->stream_index);

		/* clear the memory */
		memset (stream->stream, 0, STREAM_BUFFER_SIZE);

		/* now copy displaced content back to the stream */
		memcpy (stream->stream, stream->temp, 
			STREAM_BUFFER_SIZE - stream->stream_index);

		/* update the index to the positioned at the next byte
		 * available on the buffer */
		stream->stream_size  = STREAM_BUFFER_SIZE - stream->stream_index;
	}else {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   nothing to prebuffer on the tail");
		stream->stream_size  = 0;
	}

	/* reset current index */
	stream->stream_index = 0;

	
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   reading on buffer index: %d, size: %d",
		 stream->stream_index, stream->stream_size);

	/* read current content */
	bytes_read = read (stream->fd, stream->stream + stream->stream_size,
			   STREAM_BUFFER_SIZE - stream->stream_size);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   bytes read from the file: %d", bytes_read);

	/* check for end of file reached */
	if (bytes_read == 0) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "end of file reached");
		close (stream->fd);
		stream->fd = -1;
		return AXL_FALSE;
	}

	/* set the new size, that is the padding, the content already
	 * read, and the bytes already read */
	stream->stream_size += bytes_read;

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   (before read) current buffer size: %d",
		 stream->stream_size);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   prebuffered data: stream size: %d",
		 stream->stream_size);

	return AXL_TRUE;
}

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
 * @param file_path Optional parameter to allow reading the stream
 * from a file using the open API.
 *
 * @param fd_handler Optional parameter to allow reading the stream
 * from a file descriptor, that could be a file, a socket, etc..
 * 
 * @return A newly allocated stream instance that should be
 * deallocated by using \ref axl_stream_free. The function could
 * return a NULL value is received a NULL stream or a non positive
 * stream size.
 */
axlStream * axl_stream_new (char * stream_source, int stream_size,
			    char * file_path, int    fd_handler,
			    axlError ** error)
{
	axlStream * stream;
	int         fd;

	/* perform some environmental checkings */
	if (file_path != NULL || (fd_handler > 0)) {
		if (fd_handler < 0) {
			/* a file handle */
			if ((fd = open (file_path, O_RDONLY)) < 0) {
				axl_error_new (-1, "unable to read file provided", NULL, error);
				return NULL;
			}
		}else
			fd = fd_handler;

		/* create the stream holder */
		stream               = axl_new (axlStream, 1);
		stream->type         = STREAM_FD;
		stream->fd           = fd;

		/* allocate 4k to perform streaming */
		stream->stream       = axl_new (char, STREAM_BUFFER_SIZE + 1);

		/* prebuffer */
		axl_stream_prebuffer (stream);
	}else {
		/* check chunk received */
		if (stream_source == NULL) {
			axl_error_new (-1, "Requested to open a stream without providing an memory chunk, file descriptor or a file path", NULL, error);
			return NULL;
		}

		/* check memory chunk size */
		if (stream_size == -1)
			stream_size = strlen (stream_source);

		/* create the stream holder */
		stream               = axl_new (axlStream, 1);
		stream->type         = STREAM_MEM;

		/* copy source */
		stream->stream       = axl_new (char, stream_size + 1);
		memcpy (stream->stream, stream_source, stream_size);
		
		/* set initial indicators */
		stream->stream_size  = stream_size;
	}

	/* initilize common stream part */
	stream->chunks = axl_new (char *, MAX_INSPECTED_CHUNKS + 1);

	/* return newly created stream */
	return stream;
}

/** 
 * @internal
 *
 * Internal implementation to support axl_stream_inspect and
 * axl_stream_peek.
 * 
 * @param stream The stream where the inspect operation will be
 * performed.
 *
 * @param chunk The chunk to be used while performing the inspect
 * operation.
 *
 * @param alsoAccept AXL_TRUE to make the function to accept stream
 * that is properly inspected, AXL_FALSE if not.
 * 
 * @return See \ref axl_stream_inspect.
 */
int         axl_stream_common_inspect (axlStream * stream, char * chunk, bool alsoAccept)
{
	int inspected_size;

	axl_return_val_if_fail (stream, -2);
	axl_return_val_if_fail (chunk, -1);

	/* get current size to inspect */
	inspected_size = strlen (chunk);

	/* check that chunk to inspect doesn't fall outside the stream
	 * boundaries */
	if (axl_stream_fall_outside (stream, inspected_size)) {
		return -1; /* no more stream is left to satisfy current petition */
	}
	
	/* check that the chunk to be search is found */
	if (axl_stream_check (stream, chunk)) {

		/* chunk found!, remember that the previous inspect
		 * size so we can make the stream to roll on using
		 * this value */
		stream->previous_inspect = inspected_size;

		if (alsoAccept) {
			/* accept the chunk readed */
			axl_stream_accept (stream);
		}
		return 1;
	}
	/* return that the stream chunk wasn't found */
	return 0;

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
	/* call to common implementation */
	return axl_stream_common_inspect (stream, chunk, AXL_TRUE);
}

/** 
 * @internal
 *
 * @brief Allows to perform a stream inspection without automatically
 * accept content properly inspected.
 * 
 * @param stream The stream where the peek operation will be
 * performed.
 *
 * @param chunk The chunk to lookup.
 * 
 * @return See \ref axl_stream_inspect.
 */
int         axl_stream_peek            (axlStream * stream, char * chunk)
{
	/* call to common implementation */
	return axl_stream_common_inspect (stream, chunk, AXL_FALSE);
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

	/* simple memory chunk parsing */
	stream->stream_index     += stream->previous_inspect;
	stream->global_index     += stream->previous_inspect;

	stream->previous_inspect  = 0;
	if (stream->last_chunk != NULL)
		axl_free (stream->last_chunk);
	stream->last_chunk = NULL;

	return;
}

/** 
 * @internal
 *
 * @brief Allows to configure current index to be accepted by the
 * stream. 
 * 
 * @param stream The stream where the operation will be performed.
 *
 * @param count Count to move internal stream index.
 */
void        axl_stream_move            (axlStream * stream, int index)
{
	axl_return_if_fail (stream);

	axl_stream_accept (stream);
	stream->stream_index     = index;
	
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
 * to be returned. Currently this is not implemented, so, you can
 * provide a NULL value.
 *
 * @param chunk_num The number of chunks to be checked as a valid terminators.
 * 
 * @param accept_terminator While calling to this function, the
 * terminator detected to stop the operation could also be accepted by
 * the stream, making it not necessary to accept the terminator once
 * the function have ended. However, this could be a problem while
 * performing peeking code. You can provide a FALSE value to make the
 * function to not accept the terminator found as to be consumed.
 * 
 * @return The chunk recognizied, not including the terminator that
 * have made this operation to stop. 
 */
char      * axl_stream_get_until       (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int         chunk_num, ...)
{
	char * result;
	va_list args;
	
	/* open the standard argument */
	va_start (args, chunk_num);

	/* call to get next chunk separated by the provided values */
	result = axl_stream_get_untilv (stream, valid_chars, chunk_matched, accept_terminator, NULL, chunk_num, args);

	/* close the standard argument */
	va_end (args);

	/* return value read */
	return result;
}

/** 
 * @internal
 * 
 * Works the same way like axl_strteam_get_until but wihtout allocated
 * the memory returned, and filling the size for the chunk returned
 * in result_size reference.
 * 
 * @param stream The stream where the operation will be performed.
 *
 * @param valid_chars The valid chars reference to match (currently
 * not implemented).
 *
 * @param chunk_matched The chunk matched reference 
 *
 * @param accept_terminator Configure if the terminator should be
 * accepted or not.
 *
 * @param result_size The variable where the result size will be
 * returned.
 *
 * @param chunk_num The number of chunks to match.
 * 
 * @return A reference to the internal stream copy. The reference
 * returned must not be deallocated.
 */
char      * axl_stream_get_until_ref   (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int       * result_size,
					int         chunk_num, ...)
{
	char * result;
	va_list args;
	
	/* open the standard argument */
	va_start (args, chunk_num);

	/* call to get next chunk separated by the provided values */
	result = axl_stream_get_untilv (stream, valid_chars, chunk_matched, accept_terminator, result_size, chunk_num, args);

	/* close the standard argument */
	va_end (args);

	/* return value read */
	return result;
}

/** 
 * @internal
 *
 * Allows to nullify the internal reference of the stream, making that
 * reference to be not deallocated once the stream is moving.
 *
 * This is mainly used to reduce the malloc/free round trip while
 * using the stream abstraction, making the stream received from the
 * memory chunk to be allocated only once, avoiding the allocate-free,
 * allocate-free cycle.
 * 
 * @param stream The \ref axlStream where the operation will be
 * performed.
 *
 * @param item The item to nullify. 
 */
void        axl_stream_nullify         (axlStream * stream,
					NullifyItem item)
{
	/* do not operate if a null stream reference is received */
	axl_return_if_fail (stream);

	switch (item) {
	case LAST_CHUNK:
		stream->last_chunk         = NULL;
		break;
	case LAST_NEAR_TO:
		stream->last_near_to       = NULL;
		break;
	case LAST_GET_FOLLOWING:
		stream->last_get_following = NULL;
		break;
	}
	
	/* nothing more to do here */
	return;
}

/** 
 * @internal
 *
 * @brief Allows to perform the same operation like \ref
 * axl_stream_get_untilv but providing an already initialized and
 * opened std arg.
 *
 * This function is in fact, used by \ref axl_stream_get_untilv.
 * 
 * @param stream The stream where the operation will be peformed.
 * 
 * @param valid_chars The valid chars set to be used while reading
 * data.
 *
 * @param chunk_matched An optional value where the matched chunk will
 * be reported.
 *
 * @param accept_terminator Configure if terminator read should be
 * accepted or only the chunk read.
 *
 * @param chunk_num How many terminators are configured.
 *
 * @param args The list of terminators.
 * 
 * @return The chunk read or NULL if fails.
 */
char      * axl_stream_get_untilv      (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int       * result_size,
					int         chunk_num, 
					va_list args)
{
	int          iterator   = 0;
	int          index      = 0;
	int          length     = 0;
	int          max_length = 0;
	bool         matched;
	char       * string     = NULL;
	
	/* perform some environmental checks */
	axl_return_val_if_fail (stream, NULL);
	/* axl_return_val_if_fail (valid_chars, NULL); */
	axl_return_val_if_fail (chunk_num > 0, NULL);

	/* set current matched value */
	if (chunk_matched != NULL)
		*chunk_matched = -1;
	
	/* iterate over the chunk list */
	while (iterator < chunk_num) {
		/* get the chunk */
		stream->chunks[iterator] = va_arg (args, char *);
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "looking for: '%s'", stream->chunks[iterator]);
		
		/* get current length */
		if (strlen (stream->chunks [iterator]) > max_length)
			max_length = strlen (stream->chunks [iterator]);

		/* update index */
		iterator ++;
	}

	/* now we have chunks to lookup, stream until get the stream
	 * limited by the chunks received. */

	do {

		/* check if the index is falling out side the buffer boundaries */
		if (fall_out_side_checking (stream, index)) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "prebuffering from get-until (requested %d, starting from: %d): current status: %s", 
				 index, stream->stream_index, 
				 axl_stream_get_following (stream, 10));

			if (! axl_stream_prebuffer (stream)) {
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "failed while prebuffer");
				return NULL;
			}

			/* work around: because the index is updated
			 * at the end of the loop, it is required to
			 * decrease the index in one unit in the case
			 * a prebuffer operation happens */
			index--;


			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "before prebuffering (requested: %d, starting from: %d): %s", 
				 index, stream->stream_index, 
				 axl_stream_get_following (stream, 10));
		}
		
		/* compare chunks received for each index increased
		 * one step */
	init_get_until:
		iterator = 0;
		while (iterator < chunk_num) {
			
			/* get current length for the chunk to check */
			length = (stream->chunks [iterator] != NULL) ?  strlen (stream->chunks [iterator]) : 0;

			/* if we receive a white space " " as a
			 * delimiter, we will also stop on white
			 * spaces (in the sense defined at the XML
			 * 1.0, that is \r \n \t " " */
			if (axl_cmp (stream->chunks[iterator], " "))
				matched = axl_stream_is_white_space (stream->stream + index + stream->stream_index);
			else 
 				matched = !memcmp (stream->chunks[iterator], stream->stream + index + stream->stream_index, length);

			/* check if we have found the chunk we were looking */
			if (matched) {
				
				/* check for matching a more specific
				 * terminal than a general */
				if ((length < max_length) && 
				    ((index + stream->stream_index + length) == stream->stream_size)) {
					axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "detected possible false positive");

					/* do a prebuffer operation,
					 * and if success, try to
					 * check again all chunks at
					 * the current index */
					if (axl_stream_prebuffer (stream))
						goto init_get_until;
				}


				/* report which is the chunk being
				 * matched by the expresion */
				if (chunk_matched != NULL)
					*chunk_matched = (iterator);

				/* result is found from last stream
				 * index read up to index */
				if (stream->last_chunk != NULL) {
					axl_free (stream->last_chunk);
				}

				/* get a copy to the chunk to be returned */
				if (result_size == NULL) {
					stream->last_chunk = axl_new (char, index + 1);
					memcpy (stream->last_chunk, stream->stream + stream->stream_index, index);
				}else {
					*result_size = index;
					string       = stream->stream + stream->stream_index;
				}

				/* in the case a memory chunk is being read */
				if (accept_terminator)
					stream->stream_index     += length;
				stream->stream_index             += index;
				stream->global_index             += index;
				stream->previous_inspect          = 0;

		 
				if (result_size == NULL) {
					axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "returning: '%s' (chunk num: %d) (index: %d, stream index: %d, stream_size: %d)", 
						 stream->last_chunk, iterator, index, stream->stream_index, stream->stream_size);
					return stream->last_chunk;
				}
				
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "returning: '%s' (chunk num: %d) (index: %d, stream index: %d, stream_size: %d)", 
					 string, iterator, index, stream->stream_index, stream->stream_size);
				return string;
			}
			/* update iterator */
			iterator ++;
		} /* end while */

		/* it seems that the chunk wasn't found */
		index++;

	}while (AXL_TRUE);

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
 * @brief Returns current global index for the device being streamed.
 * 
 * @param stream The stream where the data is being requested.
 * 
 * @return The index or -1 if fails.
 */
int         axl_stream_get_global_index (axlStream * stream)
{
	axl_return_val_if_fail (stream, -1);

	return stream->global_index;
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
 * @brief Allows to get the following <b>count</b> bytes read from the stream.
 * 
 * @param stream The stream where the operation will be performed.
 * @param count How many bytes to get from the stream.
 * 
 * @return A string referece, containing the first <b>count</b> bytes
 * or NULL if fails. Reference returned shouldn't be deallocated.
 */
char      * axl_stream_get_following   (axlStream * stream, int count)
{
	axl_return_val_if_fail (stream, NULL);

	if ((count + stream->stream_index) > stream->stream_size) {
		count = stream->stream_size - stream->stream_index;
	}

	/* free previously allocated memory */
	if (stream->last_get_following != NULL)
		axl_free (stream->last_get_following);

	/* copy stream content */
	stream->last_get_following = axl_new (char, count + 1);
	memcpy (stream->last_get_following, stream->stream + stream->stream_index, count);

	/* return reference created */
	return stream->last_get_following;
}

typedef struct _AxlStreamAssociatedData {
	axlPointer       data;
	axlDestroyFunc   destroy_func;
}AxlStreamAssociatedData;


/** 
 * @internal
 * 
 * Internal deallocation function to reclaim memory used by the \ref
 * AxlStreamAssociatedData.
 * 
 * @param data The data to be deallocated.
 */
void __stream_associated_data_free (AxlStreamAssociatedData * data)
{
	/* do nothing if NULL is received */
	if (data == NULL)
		return;

	/* destroy associated data used provided destroy function */
	if (data->destroy_func != NULL && data->data != NULL)
		data->destroy_func (data->data);

	/* now free the node it self */
	axl_free (data);

	return;
}

/** 
 * @internal
 *
 * @brief Associates the given axlPointer with the given stream to be
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
 * @param element The element to link (may a axlDoc or a axlDtd).
 *
 * @param func The function to call once the stream is released.
 */
void        axl_stream_link            (axlStream  *   stream,
					axlPointer     element,
					axlDestroyFunc func)
{
	AxlStreamAssociatedData * data;
	axl_return_if_fail (stream);
	axl_return_if_fail (element);
	axl_return_if_fail (func);
	
	/* that's all */
	if (stream->elements_linked == NULL)
		stream->elements_linked = axl_list_new (axl_list_always_return_1, 
							(axlDestroyFunc) __stream_associated_data_free);

	/* create the data to be stored */
	data               = axl_new (AxlStreamAssociatedData, 1);
	data->data         = element;
	data->destroy_func = func;

	/* add the item to be destroy once the stream is unrefered */
	axl_list_add (stream->elements_linked, data);

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
	int                       iterator;
	AxlStreamAssociatedData * data;

	axl_return_if_fail (stream);
	
	/* clear document association */
	iterator = 0;
	while (iterator < axl_list_length (stream->elements_linked)) {
		/* get a referece to the node to destroy */
		data = axl_list_get_nth (stream->elements_linked, iterator);
		
		/* clear it */
		data->data         = NULL;
		data->destroy_func = NULL;
		
		iterator++;
	}
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
	if (stream->elements_linked) 
		axl_list_free (stream->elements_linked);

	/* releaset last chunk */
	if (stream->last_chunk != NULL)
		axl_free (stream->last_chunk);

	/* releaset last near to */
	if (stream->last_near_to != NULL)
		axl_free (stream->last_near_to);

	/* releaset last get following */
	if (stream->last_get_following != NULL)
		axl_free (stream->last_get_following);

	if (stream->fd > 0) {
		/* close file descriptor if defined */
		close (stream->fd);
	}

	/* free memory allocated for chunk matching */
	axl_free (stream->chunks);

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
	if (size < 0)
		return AXL_FALSE;
	
	/* report current comparation status */
	if (!memcmp (chunk1, chunk2, size))
		return AXL_TRUE;
	return AXL_FALSE;
}

/** 
 * @internal
 *
 * @brief Allows to check if the provided stream could support
 * checking a chunk of, at least, the provided inspected size.
 * 
 * @param stream The stream where the inspected operation is being
 * requested.
 *
 * @param inspected_size The size to inspected, that is being
 * requested to be checked on the given stream.
 * 
 * @return \ref AXL_TRUE if the provided chunk falls out side the
 * stream boundaries, or AXL_FALSE if requested inspected size could
 * be supported.
 */
bool axl_stream_fall_outside (axlStream * stream, int inspected_size)
{
	axl_return_val_if_fail (stream, AXL_TRUE);

	/* if the content is inside memory, check it */
	if (fall_out_side_checking (stream, inspected_size)) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "prebufferring from fall-outside (index: %d, size: %d, inspected size: %d)", 
			 stream->stream_index, stream->stream_size,
			 inspected_size);

		return (! axl_stream_prebuffer (stream));
	}
	
	/* otherwise, false is returned */
	return AXL_FALSE;
}

/** 
 * @internal
 * @brief Allows to check if the given stream have as a next element
 * the provided chunk.
 * 
 * @param stream The stream where the operation will be performed.
 * @param chunk The chunk to check
 * 
 * @return Returns AXL_TRUE if the given stream contains the value requested
 * or AXL_FALSE if not.
 */
bool         axl_stream_check           (axlStream * stream, char * chunk)
{
	int inspected_size;

	axl_return_val_if_fail (stream, AXL_FALSE);
	axl_return_val_if_fail (chunk, AXL_FALSE);

	/* get current size to inspect */
	inspected_size = strlen (chunk);

	/* check that the chunk will not fall outside the buffer */
	if (axl_stream_fall_outside (stream, inspected_size))
		return AXL_FALSE;

	/* check current chunk against current stream status */
	return (memcmp (chunk, stream->stream + stream->stream_index, inspected_size) == 0) ? AXL_TRUE : AXL_FALSE;
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
		
	/* check if the stream is exhausted */
	if (stream->stream_index >= (stream->stream_size - 1)) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "prebufferring from remains");
		/* in the case the stream is exhausted, try to read
		 * more content from the streaming */
		return axl_stream_prebuffer (stream);
	}
	return AXL_TRUE;
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
 * @internal
 *
 * Internal prototype declaration to avoid getting gcc complaining
 * every time it found a reference to this function. Many environment
 * support this function, in the case this function is not support we
 * will have to provide a fall back. However, it is required to
 * program the library using ansi environment options to ensure the
 * maximum compatibility.
 */
int vsnprintf(char *str, size_t size, const char *format, va_list ap);


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

	/* get current buffer size to copy */
	size     = vsnprintf (NULL, 0, chunk, args);

	/* allocate memory */
	result   = axl_new (char, size + 2);

	/* copy current size */
	new_size = vsnprintf (result, size + 1, chunk, args);

	va_end (args);
	
	return result;
}

/** 
 * @internal
 *
 * @brief Allows to split the provided chunk, into several pieces that
 * are separated by the separator (or separators) provided.
 *
 * The function will try to split the chunk provide using the
 * separator provided, and optionally, all separators provided.
 *
 * @param chunk The chunk to split.
 *
 * @param separator_num The number os separators to be used while
 * spliting the chunk.
 * 
 * @return A newly allocated string, that must be deallocated by using
 * \ref axl_stream_freev. The function will return a NULL if the chunk
 * or the separators provided are NULL.
 */
char     ** axl_stream_split           (char * chunk, int separator_num, ...)
{
	va_list      args;
	char      ** separators;
	char      ** result;
	int          iterator;
	int          index;
	int          previous_index;
	int          count      = 0;
	int          length     = 0;

	/* check received values */
	axl_return_val_if_fail (chunk, NULL);
	axl_return_val_if_fail (separator_num > 0, NULL);

	separators = axl_new (char *, separator_num + 1);
	iterator   = 0;
	va_start (args, separator_num);

	/* get all separators to be used */
	while (iterator < separator_num) {
		separators[iterator] = va_arg (args, char *);
		iterator++;
	}
	
	va_end (args);

	/* now, count the number of strings that we will get by
	 * separating the string into several pieces */
	index    = 0;
	while (*(chunk + index) != 0) {

		/* reset the iterator */
		iterator = 0;
		while (iterator < separator_num) { 

			/* compare the current index with the current
			 * separator */
			length = strlen (separators[iterator]);
			if (! memcmp (chunk + index, separators[iterator], length)) {

				/* update items found */
				count++;

				/* axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found item at the index: %d (count %d)", index, count); */

				/* update index to skip the item found */
				index += length - 1; /* make the last index to be captured the the -1 */

				/* axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "starting to look at index: %d", index); */

				/* break the loop */
				break;
			}
			iterator++;
		}

		/* update the index to the next item */
		index++;
	}
	
	/* create the result that will hold items separated */
	result = axl_new (char *, count + 2);
	/* axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found %d items to split", count); */

	/* now copy items found */
	count          = 0;
	index          = 0;

	/* remember previous_index */
	previous_index = index;
	while (*(chunk + index) != 0) {

		/* reset the iterator */
		iterator = 0;
		while (iterator < separator_num) { 

			/* compare the current index with the current
			 * separator */
			length = strlen (separators[iterator]);
			if (! memcmp (chunk + index, separators[iterator], length)) {

				/* copy the chunk found */
				result[count] = axl_new (char, index - previous_index + 1);
				memcpy (result[count], chunk + previous_index, index - previous_index);

				/* axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "item found (last) '%s' (index=%d != previous=%d", result[count],  index, previous_index); */

				/* update items found */
				count++;

				/* update index to skip the item found */
				if (*(chunk + index + length) == 0) {
					/* in the case no more elements to read will be found */
					/* put an empty space at the end */
					result [count]    = axl_new (char, 1);
					
					axl_free (separators);
					return result;
				}

				/* remember previous_index */
				index += length; 
				previous_index = index;
				index--; /* make the last index to be captured the the -1 */
				break;
			}
			iterator++;
		}

		/* update the index to the next item */
		index++;
	}

	/* check for a last chunk */
	if (index != previous_index) {
		/* copy the chunk found */
		result[count] = axl_new (char, index - previous_index + 1);
		memcpy (result[count], chunk + previous_index, index - previous_index);

		/* axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "item found (last) '%s' (index=%d != previous=%d", result[count], index, previous_index); */
	}

	
	/* release memory */
	axl_free (separators);
	
	return result;
}

/** 
 * @brief Allows to concatenate the two given strings into a single
 * one.
 *
 * The function will concatenate both string into a newly allocated
 * string that is the result of taking chunk1 followed by chunk2. 
 * 
 * If the function receive a NULL value for one of the string
 * received, the result from this function will be the other
 * string. This is done to support a common basic case where the
 * string provided for one of the arguments is the one being used two
 * hold an iterative result. This variable usually is NULL.
 *
 * Once the string returned is no longer needed, \ref axl_free must be
 * used to deallocate the result. 
 *
 * The function will use the strlen function to calculate current
 * chunk sizes to provide the result.
 *
 * 
 * @param chunk1 The first chunk to be placed on the result.
 *
 * @param chunk2 The second chunk to be placed on the result.
 * 
 * @return A newly allocated string, containing both strings, or NULL
 * if fails. The only way for this function to fail is to provide two
 * NULL references as incoming strings.
 */
char      * axl_stream_concat          (char * chunk1, char * chunk2)
{
	axl_return_val_if_fail ((chunk2 != NULL) || (chunk1 != NULL), NULL);

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called..");

	if (chunk1 == NULL) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called.., returning %s", chunk2);
		/* return the result */
		return axl_strdup (chunk2);
	}

	if (chunk2 == NULL) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called.., returning %s", chunk1);
		/* return the result */
		return axl_strdup (chunk1);
	}

	/* return the concatenation */
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called.., returning %s%s", chunk1, chunk2);
	return axl_stream_strdup_printf ("%s%s", chunk1, chunk2);
}


/** 
 * @brief Returns current number of items inside the chunks reference
 * provided.
 * 
 * @param chunks The chunks reference, which contains a list of
 * chunks.
 * 
 * @return The number of chunks that the reference has or -1 if it
 * fails.
 */
int         axl_stream_strv_num        (char ** chunks)
{
	int iterator = 0;

	axl_return_val_if_fail (chunks, -1);
	/* release memory used by all elements inside the chunk */
	while (chunks[iterator] != 0) {
		iterator++;
	}

	/* return current number of chunks */
	return iterator;
	
}

/** 
 * @internal
 *
 * @brief Allows to release memory used by elements returned by \ref
 * axl_stream_split and other function that return a pointer to a char
 * **.
 * 
 * @param chunks The chunk to release.
 */
void        axl_stream_freev           (char ** chunks)
{
	int iterator = 0;

	axl_return_if_fail (chunks);
	 
	/* release memory used by all elements inside the chunk */
	while (chunks[iterator] != 0) {
		axl_free (chunks[iterator]);
		iterator++;
	}
	
	/* now release the chunk inside */
	axl_free (chunks);
	
	/* nothing more to do */
	return;
}

/** 
 * @internal 
 *
 * @brief Internal function to support \ref axl_stream_to_upper and
 * \ref axl_stream_to_lower
 * 
 * @param chunk The chunk to modify
 * @param desp Bits to increase.
 */
void __axl_stream_common_to (char * chunk, bool to_upper)
{
	int iterator = 0;

	axl_return_if_fail (chunk);

	while (chunk[iterator] != 0) {
		/* change first ascii level */
		if (to_upper)
			chunk[iterator] = toupper (chunk[iterator]);
		else
			chunk[iterator] = tolower (chunk[iterator]);
		
		/* update iterators */
		iterator++;
	} 	

	return;
}

/** 
 * @brief Makes the provided string to be converted to upper case
 * letters.
 *
 * The function will modify the address provided. If you want to get a
 * upper case copy for a particular string, copy it first, by using
 * \ref axl_strdup, and then use this function.
 *
 * The function, for convenience, will also return the string
 * reference received, already modified. This could be used while
 * using this function to convert parameters that are expected by
 * other functions.
 * 
 * @param chunk The chunk to upper case.
 */
char *    axl_stream_to_upper        (char  * chunk)
{
	__axl_stream_common_to (chunk, AXL_TRUE);
	return chunk;
}

/** 
 * @brief Allows to convert the provided string into lower cases
 * letter.
 *
 * The function, for convenience, will also return the string
 * reference received, already modified. This could be used while
 * using this function to convert parameters that are expected by
 * other functions.
 * 
 * @param chunk The chunk to lower case.
 */
char *     axl_stream_to_lower        (char  * chunk)
{
	__axl_stream_common_to (chunk, AXL_FALSE);
	return chunk;
}

/* @} */
