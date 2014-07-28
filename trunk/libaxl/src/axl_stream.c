/*
 *  LibAxl:  Another XML library
 *  Copyright (C) 2013 Advanced Software Production Line, S.L.
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

#include <axl.h>
#include <math.h>

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
#define STREAM_BUFFER_SIZE 8192

/** 
 * @internal
 *
 * Internal definition used to represent the maximum value used for
 * calls done to match a set of chunks. 
 */
#define MAX_INSPECTED_CHUNKS 30

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

	/* stream buffer size (maximum amount of bytes to hold, used
	 * also to measure the temporal buffer) */
	int    buffer_size;
	
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

	/* support variable for chunk matching */
	int   * lengths;

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
	char      * temp;
	char      * decode_temp;
	int         decode_temp_size;
	int         decode_temp_index;
	int         decode_temp_last;
	int         decode_temp_remain;
	char      * source_encoding;

	/* here is how these variables are used to decode content:
	 * 
	 * [   <---  decode_temp buffer ---> ] <- decode_temp_size: total buffer size (bytes)
	 *   ^                ^
	 *   |                |     
	 *   |                +-- decode_temp_last: last valid content in the buffer (bytes)
	 *   +-- decode_temp_index: next content to be consumed (bytes)
	 *
	 * [decode_temp_remain]: is the amount of content pending to
	 * be decoded. That is, starting from decode_temp_remain until
	 * last is the valid content still to be decoded.
	 *
	 * [source_encoding]: is defined to hold the value of the
	 * format for the source.
	 */

	/* more variables used to perform work: at get until */
	char      * valid_chars;
	int         chunk_matched;
	axl_bool    accept_terminator;
	int         result_size;
	int         chunk_num;

	/** 
	 * @internal Internal variable used to notify get_until
	 * function that the last 0 character for stream memory
	 * operation done in a STREAM_MEM must be considered as a
	 * terminator character found.
	 */
	axl_bool        zero;

	/** 
	 * @internal Alloc function to be used to require memory for
	 * chunks to be returned. This is used by Axl Stream to allow
	 * external modules to handle how memory is allocated while
	 * calling to axl_stream_get_until* API. 
	 *
	 * Currently, it is used by the axl_doc module to allocate axl
	 * stream using a string factory.
	 */
	axlStreamAlloc alloc;
	
	/** 
	 * @internal User defined data to be passed to the alloc
	 * function.
	 */
	axlPointer     alloc_data;

	/** 
	 * @internal Function used by the stream to decode content
	 * into utf-8. This is not used in all cases.
	 */
	axlStreamDecode decode_f;
	
	/** 
	 * @internal Reference to user defined pointer provided to the
	 * decode_f function.
	 */
	axlPointer      decode_f_data;

	/** 
	 * @internal Value that allows to signal that the buffer needs
	 * to be expanded (no matter what shows current indexes).
	 */
	axl_bool        needs_expansion;

	/** 
	 * @internal Reference to the content check function
	 * installed.
	 */
	axlStreamContentCheck check_f;

	/** 
	 * @internal Reference to user defined pointer to be passed to
	 * the content check function (check_f).
	 */
	axlPointer            check_f_data;
};


/**
 * \defgroup axl_stream_module Axl Stream Document: Abstract stream where a xml document is expected (also provided string functions)
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
 * @param appended_content New content to be included at the begining
 * of the stream while doing the prebuffer operation. This value could
 * be null.
 *
 * @param appended_size The size for the appended content to be added.
 * 
 * @return \ref axl_true if the requested padding and buffer size were
 * filled or \ref axl_false if end of file was reached. In that case the
 * stream size is not updated.
 */
axl_bool axl_stream_prebuffer (axlStream * stream)
{
	int  bytes_read;
	int  op_result;

	/* check some environment conditions */
	axl_return_val_if_fail (stream, axl_false);

	/* no prebuffering is the stream type is not a file descriptor
	 * source and if the socket is closed */
	if (stream->type != STREAM_FD || stream->fd == -1) {
		return axl_false;
	} /* end if */

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "prebuffering the stream..");

	/* displace memory only if there were data already consumed */
	if (stream->stream_index > 0 && ((stream->stream_size - stream->stream_index) > 0)) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   moving previous content at the begining of the buffer, current index: %d, size: %d, current status: %s",
			   stream->stream_index, stream->stream_size - stream->stream_index,
			   stream->stream + stream->stream_index);

		/* displace memory already read to be at the begining
		 * of the stream */
		memcpy (stream->temp, stream->stream + stream->stream_index,
			stream->stream_size - stream->stream_index);

		/* now copy displaced content back to the stream */
		memcpy (stream->stream, stream->temp, 
			stream->stream_size - stream->stream_index);

		/* update the index to the positioned at the next byte
		 * available on the buffer */
		stream->stream_size  = (stream->stream_size - stream->stream_index);
	} else {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   nothing to prebuffer on the head (stream->size=%d, stream-index=%d, buffer-size=%d)",
			   stream->stream_size, stream->stream_index, stream->buffer_size);

		/* check here if the buffer is full of content and a call to
		 * prebuffer was done */
		if (((stream->stream_size == stream->buffer_size) && 
		    (stream->stream_index == 0)) || stream->needs_expansion) {
			/* looks like the caller is prebuffering
			 * having all buffers full of content .. */
			

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "   requested to prebuffer with buffers full of content (stream-index=%d, stream-size=%d, stream-buffer-size=%d, stream-needs-expansion=%d)",
				   stream->stream_index, stream->stream_size, stream->buffer_size, stream->needs_expansion);

			/* unflag expansion requested */
			if (stream->needs_expansion)
				stream->needs_expansion = axl_false;

			/* duplicate the buffer size */
			stream->buffer_size += (stream->buffer_size);
			stream->stream       = realloc (stream->stream, stream->buffer_size + 1);
			stream->temp         = realloc (stream->temp,   stream->buffer_size + 1);

			if (stream->stream == NULL) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "   failed to update buffer sizes (realloc operation failed)",
				   stream->stream_index, stream->stream_size, stream->buffer_size);
				close (stream->fd);
				stream->fd = -1;
				return axl_false;
			} /* end if */
			
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "   buffer updated (stream-index=%d, stream-size=%d, stream-buffer-size=%d)",
				   stream->stream_index, stream->stream_size, stream->buffer_size);
		} else {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG,
				   "   clearing stream-size=%d, having stream-index=%d and stream-buffer-size=%d",
				   stream->stream_size, stream->stream_index, stream->buffer_size);

			/* reset if the stream size is just the content consumed */
			if (stream->stream_size == stream->stream_index)
				stream->stream_size  = 0; 
		}
	}

	/* reset current index */
	stream->stream_index = 0;

	/* check if we have decode functions defined */
	if (stream->decode_f) {
		while (1) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "check temporal decode buffer for pending content: decode_temp_index=%d, decode_temp_last=%d, decode_temp_size=%d",
				   stream->decode_temp_index, 
				   stream->decode_temp_last,
				   stream->decode_temp_size);

			if (stream->decode_temp_last > 0) {
				/* call to decode */
				if (! axl_stream_decode (stream, 
							 /* output */
							 stream->stream + stream->stream_size,
							 /* max output size */
							 stream->buffer_size - stream->stream_size,
							 /* output decoded */
							 &bytes_read, &op_result, NULL)) {
					__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "failed to decode content at the temporal decode buffer");
					return axl_false;
				} /* end if */

				/* check if the decode operation
				 * signaled that not enough espace was
				 * available to decode and no output
				 * was decoed, int his case flag the
				 * stream to do a stream expansion on
				 * the next call */
				if (op_result == 2 && bytes_read == 0)
					stream->needs_expansion = axl_true;

				/* add bytes read to the size */
				stream->stream_size += bytes_read;

			} /* end if */

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, 
				   "after checking to decode, op_result=%d, bytes_read=%d, stream->buffer_size=%d, stream->stream_size=%d",
				   op_result, bytes_read, stream->buffer_size, stream->stream_size);

			/* stop if no enought space if left and no
			 * more bytes were translated */
			if (op_result == 2) 
				break;

			/* check if there are still space to decode at the stream */
			if ((stream->buffer_size - stream->stream_size) > 0) {
				/* read content inside the decde temp buffer */
				bytes_read = read (stream->fd, stream->decode_temp + stream->decode_temp_last,
						   stream->decode_temp_size - stream->decode_temp_last);
				
				/* update the amount of data available tat the decode temp */
				if (bytes_read > 0)
					stream->decode_temp_last += bytes_read;
				else if (bytes_read == 0 && stream->fd > 0) {
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "end of file reached");
					close (stream->fd);
					stream->fd = -1;
				} /* end if */
			} else {
				/* no more space is available at the reading buffer */
				break;
			} /* end if */

			/* check to terminate */
			if (stream->decode_temp_index == 0 && 
			    stream->decode_temp_last == 0 && 
			    stream->fd == -1) 
				return axl_true;
			
		} /* end while */

	} else {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   reading on buffer index: %d, size: %d (starting from: %d, length: %d)",
			   stream->stream_index, stream->stream_size, stream->stream_size, stream->buffer_size - stream->stream_size);

		/* read current content */
		bytes_read = read (stream->fd, stream->stream + stream->stream_size,
				   stream->buffer_size - stream->stream_size);


		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   bytes read from the file, size %d", bytes_read);

		/* call to check */
		if (stream->check_f) {
			/* call to check */
			if (! axl_stream_content_check (stream, stream->stream + stream->stream_size, bytes_read, NULL)) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "failed to prebuffer, content check function have failed");
				return axl_false;
			}
		} /* end if */
		
		/* check for end of file reached */
		if (bytes_read == 0) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "end of file reached");
			close (stream->fd);
			stream->fd = -1;
			return axl_false;
		}
		
		/* set the new size, that is the padding, the content already
		 * read, and the bytes already read */
		stream->stream_size += bytes_read;
	} /* end if */

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   (before read) current buffer size: %d",
		   stream->stream_size);
	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "   prebuffered data: stream size: %d, new content: %s",
		   bytes_read, stream->stream + stream->stream_index);

	return axl_true;
}

/** 
 * @brief Creates a new byte stream using as data the string pointer
 * and the size.
 * 
 * The \ref axlStream object is an abstraction that allows to
 * interface with a memory chunk, a file descriptor (or a socket) or a
 * direct path, with a contenience API that allows inspecting and
 * accepting the streamed content.
 *
 *
 * Here is an example on how to create an stream from a memory chunk:
 * \code
 * axlStream * stream;
 * axlError  * error;
 *
 * // create the stream and check result 
 * stream = axl_stream_new (source_content, size_content, -1, NULL, &error);
 * if (stream == NULL) {
 *     printf ("An error was found: %s\n", axl_error_get (error));
 *     axl_error_free (error);
 *     return;
 * }
 * 
 * // stream created
 * \endcode
 *
 * In the case an stream is required to parse a file descriptor
 * (including a socket):
 * 
 * \code
 * stream = axl_stream_new (NULL, -1, NULL, fd, &error);
 * \endcode
 *
 * You can also provide a file path to let the axl stream module to
 * open the file and buffer it as it requires to consume characters:
 *
 * \code
 * stream = axl_stream_new (NULL, -1, "c:/myFiles/document.xml", -1, &error);
 * \endcode
 * 
 *
 * Once initialized the \ref axlStream object, you can use the
 * following function to check and consume characters:
 * 
 * - \ref axl_stream_inspect
 * - \ref axl_stream_peek
 * - \ref axl_stream_accept
 * 
 * There alternatives APIs that allows to get the content until some
 * patter is found (or a set of patterns):
 * 
 * - \ref axl_stream_get_until
 *
 * 
 * @param stream_source A pointer to the memory where the data to be
 * streamed is located.
 *
 * @param stream_size How many bytes are available to perform
 * streaming. You can pass a -1 value to allow the function to
 * calculate the stream source size (stream_source param).
 *
 * @param file_path Optional parameter to allow reading the stream
 * from a file using the open API.
 *
 * @param fd_handler Optional parameter to allow reading the stream
 * from a file descriptor, that could be a file, a socket, etc..
 *
 * @param error Optional \ref axlError reference where errors will be reported.
 * 
 * @return A newly allocated stream instance that should be
 * deallocated by using \ref axl_stream_free. The function could
 * return a NULL value is received a NULL stream or a non positive
 * stream size.
 */
axlStream * axl_stream_new (const char * stream_source, int stream_size,
			    const char * file_path,     int fd_handler,
			    axlError ** error)
{
	axlStream * stream;
	int         fd;

	/* perform some environmental checkings */
	if (file_path != NULL || (fd_handler > 0)) {
		if (fd_handler < 0) {
			/* a file handle */
			if ((fd = open (file_path, O_RDONLY, 0)) < 0) {
				axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "unable to open file a the location provided: %s, check location and permissions.", file_path);

				axl_error_new (-1, "unable to read file provided", NULL, error);
				return NULL;
			}
		}else
			fd = fd_handler;

		/* create the stream holder */
		stream               = axl_new (axlStream, 1);
		stream->buffer_size  = STREAM_BUFFER_SIZE;
		stream->type         = STREAM_FD;
		stream->fd           = fd;

		/* allocate 4k to perform streaming */
		stream->stream       = axl_new (char, stream->buffer_size + 1);
		stream->temp         = axl_new (char, stream->buffer_size + 1);

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
		stream->buffer_size  = stream_size;
		stream->type         = STREAM_MEM;

		/* copy source */
		stream->stream       = axl_new (char, stream_size + 1);
		memcpy (stream->stream, stream_source, stream_size);
		/* nullify the last entry to avoid problems with
		   printing functions and other APIs relaying on
		   finding \0 at the end of the string */
		stream->stream[stream_size] = 0;
		
		/* set initial indicators */
		stream->stream_size  = stream_size;
	}

	/* initilize common stream part */
	stream->chunks  = axl_new (char *, MAX_INSPECTED_CHUNKS + 1);
	stream->lengths = axl_new (int, MAX_INSPECTED_CHUNKS + 1);

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
 * @param inspected_size The size for the chunk to be checked or -1 if
 * a chunk size calculation is required.
 *
 * @param alsoAccept axl_true to make the function to accept stream
 * that is properly inspected, axl_false if not.
 * 
 * @return See \ref axl_stream_inspect.
 */
#define axl_stream_common_inspect(i,stream,chunk,inspected_size,alsoAccept)\
if (inspected_size == -1)\
	inspected_size = strlen (chunk);\
if (axl_stream_fall_outside (stream, inspected_size))\
	return -1;\
i = 0;\
while (chunk [i] != 0 && (stream->stream + stream->stream_index) [i] != 0) {\
     if (chunk [i] != (stream->stream + stream->stream_index) [i])\
        return 0;\
     i++;\
     if (i == inspected_size) {\
	stream->previous_inspect = inspected_size;\
	if (alsoAccept) {\
		axl_stream_accept (stream);\
	}\
	return 1;\
     }\
}\
	return 0


/** 
 * @brief Allows to perform an inspection of the given chunk on the
 * given stream.
 *
 * The <i>chunk</i> will be checked to apper at the begining of the
 * stream. This means that, having the current state of the stream,
 * the chunk is checked to be found at the very begining of the
 * stream.
 *
 * If the function succeed, an implict call to \ref axl_stream_accept
 * is done. In the case you are only checking but no stream acceptance
 * is required, use instead: \ref axl_stream_peek.
 * 
 * @param stream The \ref axlStream where the operation will be
 * performed.
 * 
 * @param chunk The chunk that is expected to be found at the begin of the stream.
 * 
 * @param inspected_size The size of the chunk provided to be inspected.
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
int         axl_stream_inspect (axlStream * stream, const char * chunk, int inspected_size)
{
	int iterator;

	/* call to common implementation */
	axl_stream_common_inspect (iterator, stream, chunk, inspected_size, axl_true);
}

/** 
 * @brief Allows to check the provide char code at the given \ref
 * axlStream, using optionally an offset to perform the check.
 * 
 * @param stream The stream where the check operation will be
 * implemented.
 *
 * @param value The value to check.
 *
 * @param offset The stream offset to apply to the check. Use 0 to not
 * apply offset.
 * 
 * @return \ref axl_true if the provided value is found at the current
 * stream index (taking into consideration offset).
 */
axl_bool         axl_stream_inspect_code    (axlStream * stream, char value, int offset)
{
	axl_return_val_if_fail (stream, axl_false);
	
	/* check the value */
	return stream->stream [stream->stream_index + offset] == value;
}

/** 
 * @brief Allows to perform a stream inspection without automatically
 * accept content properly inspected.
 * 
 * @param stream The stream where the peek operation will be
 * performed.
 *
 * @param chunk The chunk to lookup.
 *
 * @param inspected_size The size of the chunk provided to be
 * inspected.
 * 
 * @return See \ref axl_stream_inspect.
 */
int         axl_stream_peek            (axlStream * stream, const char * chunk, int inspected_size)
{
	int iterator;

	/* call to common implementation */
	axl_stream_common_inspect (iterator, stream, chunk, inspected_size, axl_false);
}

/** 
 * @brief Allows to perform several, not excluyen inspect operations,
 * over the given stream.
 *
 * Here is an example:
 * \code
 * if (axl_stream_inspect_several (stream,         // the stream 
 *                                 2,              // two chunks to recognize 
 *                                 "or", 2,        // first chunk and its length
 *                                 "||", 2) > 0) { // second chunk and its length
 *      // chunk matched!!
 * }
 * \endcode
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
	int       iterator   = 0;
	char    * chunk      = NULL;
	int       length     = 0;
	int       last_value = 0;

	axl_return_val_if_fail (stream,        -1);
	axl_return_val_if_fail (chunk_num > 0, -1);

	va_start (args, chunk_num);

	/* check each chunk */
	while (iterator < chunk_num) {

		/* get the next chunk */
		chunk  = va_arg (args, char *);
		length = va_arg (args, int);

		if (length == -1)
			length = strlen (chunk);
		
		/* check the chunk read */
		switch (axl_stream_inspect (stream, chunk, length)) {
		case -2:
			/* wrong parameter received */
			last_value = -2;
			break;
		case -1:
			/* there is no more stream left */
			last_value = -1;
			break;
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
	return last_value;
}

/** 
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
 * @brief Push new content at the begin of the stream.
 * 
 * @param stream The stream that will be updated with new content.
 *
 * @param content The content to be added.
 *
 * @param size The size of the content to be added.
 */
void axl_stream_push (axlStream * stream, const char * content, int size)
{
	axl_return_if_fail (stream && content);
	
	/* place the content at the begin of the stream */
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "calling to push the stream..");

	/* check if the current stream buffer could hold the pushed
	 * content plus the content that is already placed */
	if (stream->stream_size < (stream->stream_size - stream->stream_index + size)) {
		/* seems we can't hold the content at this moment, so, update the stream size */
		stream->buffer_size = stream->stream_size - stream->stream_index + size;

		/* alloc a new temporal buffer */
		axl_free (stream->temp);
		stream->temp = axl_new (char, stream->buffer_size + 1);
		memcpy (stream->temp, content, size);
		
		/* displace memory already read to be at the begining
		 * of the stream */
		memcpy (stream->temp + size, stream->stream + stream->stream_index,
			stream->stream_size - stream->stream_index);

		/* now realloc the buffer */
		axl_free (stream->stream);
		stream->stream = axl_new (char, stream->buffer_size + 1);

		/* now copy displaced content back to the stream */
		memcpy (stream->stream, stream->temp, 
			(stream->stream_size - stream->stream_index) + size);
	} else {

		/* check for the temporal buffer to be created */
		if (stream->temp == NULL)
			stream->temp = axl_new (char, stream->buffer_size + 1);

		/* copy the content */
		memcpy (stream->temp, content, size);

		/* displace memory already read to be at the begining
		 * of the stream */
		memcpy (stream->temp + size, stream->stream + stream->stream_index,
			stream->stream_size - stream->stream_index);

		/* now copy displaced content back to the stream */
		memcpy (stream->stream, stream->temp, 
			(stream->stream_size - stream->stream_index) + size);

	} /* end if */

	/* update the index to the positioned at the next byte
	 * available on the buffer */
	stream->stream_size  = (stream->stream_size - stream->stream_index) + size;

	/* reset the index */
	stream->stream_index = 0;

	/* clean previous state */
	axl_stream_accept (stream);

	return;
}

/** 
 * @brief Allows to configure current index to be accepted by the
 * stream. 
 * 
 * @param stream The stream where the operation will be performed.
 *
 * @param index Count to move internal stream index.
 *
 * NOTE: the function reset current internal state (by doing an
 * implicit call to \ref axl_stream_accept).
 */
void        axl_stream_move            (axlStream * stream, int index)
{
	axl_return_if_fail (stream);

	axl_stream_accept (stream);
	stream->stream_index     = index;
	
	return;
}

/** 
 * @brief Makes the current index to be moved the amount of bytes
 * signaled by the parameter bytes.
 * 
 * @param stream The stream to update.
 * @param bytes The number of bytes to move the index. 
 */
void        axl_stream_step            (axlStream * stream, int bytes)
{
	axl_return_if_fail (stream);
	axl_return_if_fail (bytes >= 0);

	axl_stream_accept (stream);
	stream->stream_index += bytes;

	return;
}

/** 
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
 * // reference to the allocated result
 * char * result;
 * 
 * // chunk matched variable
 * int chunk_matched;
 * 
 * // get the next chunk until a " or ' is found
 * result = axl_stream_get_until (stream, NULL, &chunk_matched, AXL_TRUE, 2, "\"", "'");
 * \endcode
 *
 * Value returned from this function mustn't be deallocated. However,
 * because the value returned is dinamically allocated by the
 * function, you can avoid doing a double allocation operation by
 * nullifying the internal reference to the result returned, making
 * the caller the only owner of the reference returned. To do this
 * use: \ref axl_stream_nullify with \ref LAST_CHUNK.
 * 
 * 
 * @param stream The stream were the chunk will be extracted.
 *
 * @param valid_chars The valid set of characters, to validate content
 * to be returned. Currently this is not implemented, so, you can
 * provide a NULL value.
 *
 * @param chunk_matched An optional pointer to an integer to notify
 * the chunk matched by the function. Chunk matching notification
 * starts from 0 up to number of chunks to match - 1. If the end of
 * the stream is reached, -2 is returned.
 * 
 * @param accept_terminator While calling to this function, the
 * terminator detected to stop the operation could also be accepted by
 * the stream, making it not necessary to accept the terminator once
 * the function have ended. However, this could be a problem while
 * performing peeking code. You can provide a AXL_FALSE value to make the
 * function to not accept the terminator found as to be consumed.
 *
 * @param chunk_num The number of chunks to be checked as a valid terminators.
 * 
 * @return The chunk recognizied, not including the terminator that
 * have made this operation to stop. 
 */
char      * axl_stream_get_until       (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					axl_bool    accept_terminator,
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
 * @brief Works the same way like axl_strteam_get_until but wihtout
 * allocating the memory returned, and filling the size for the chunk
 * returned in result_size reference.
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
 * returned. This variable is not optional. It must be configured to
 * hold the size of the content returned. If you provided a NULL
 * reference to this value then the function will fail. 
 *
 * @param chunk_num The number of chunks to match.
 * 
 * @return A reference to the internal stream copy. The reference
 * returned must not be deallocated.
 *
 * NOTE: This function have a particular function that could produce
 * not desired results. Because the stream returns a reference to the
 * current allocated stream, if nullifies the last position (\\0) to
 * avoid memory problems with printf APIs and any other code that
 * relay on the fact that C strings are NULL terminated. If the
 * content immediately following to the string returned is meaningful,
 * then you can't use this function. Example:
 * 
 * \code
 *   stream: CONTENTCONTENT2
 *           ^
 *           |
 *           +--- stream index
 *   
 *   calling to axl_stream_get_until_ref (stream, NULL, NULL, axl_false,
 *                                        &size, 1, "CONTENT");
 * 
 *   while cause stream: CONTENT\0ONTENT2
 *                               ^
 *                               |
 *                               +--- stream index
 * 
 *   and the function returning "CONTENT". See the fact that the
 *   next "C" from the word CONTENT2 is nullified.
 *
 * \endcode
 *
 * An indication that this function is not what you want is that you
 * are not accepting the terminator (accept_terminator=axl_false).
 */
char      * axl_stream_get_until_ref   (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					axl_bool    accept_terminator,
					int       * result_size,
					int         chunk_num, ...)
{
	char * result;
	va_list args;

	axl_return_val_if_fail (result_size, NULL);
	
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
 * @brief Allows to get the next string until the separators provided
 * are found or the end of the stream memory is reached.
 *
 * \ref axlStream type was designed to support parsing xml
 * documents. This documents have elements that allows to now where
 * the input has finished. Howerver, \ref axlStream abstraction has
 * showed to be powerful enough to be usable to parse other kinds of
 * elements that don't have lexical terminators to let the user to
 * provide that chunk to be matched.
 *
 * In those cases, this function allows to perform the same function
 * as \ref axl_stream_get_until but also checking, and using, as
 * terminator the end of the stream.
 * 
 * This allows to parse expressions like:
 * \code
 * int         chunk_matched;
 * axlStream * stream;
 * char      * string;
 * 
 * // create the stream 
 * stream = axl_stream_new ("array.value", -1, NULL, -1, &error);
 *
 * // parse first array identifier 
 * string = axl_stream_get_until_zero (stream, NULL, &chunk_matched, 
 *                                     axl_false, 2, "[", ".", NULL);
 *
 * // received "array"
 *
 * // parse again 
 * string = axl_stream_get_until_zero (stream, NULL, &chunk_matched, 
 *                                     axl_false, 2, "[", ".", NULL);
 *
 * // received "value" and chunk_matched == (-2)
 * \endcode
 *  
 * 
 * @param stream The stream were the chunk will be extracted.
 *
 * @param valid_chars The valid set of characters, to validate content
 * to be returned. Currently this is not implemented, so, you can
 * provide a NULL value.
 *
 * @param chunk_matched An optional pointer to an integer to notify
 * the chunk matched by the function. Chunk matching notification
 * starts from 0 up to number of chunks to match - 1. If the end of
 * the stream is reached while searching for the content to match,
 * chunk_matched is configured to -2.
 * 
 * @param accept_terminator While calling to this function, the
 * terminator detected to stop the operation could also be accepted by
 * the stream, making it not necessary to accept the terminator once
 * the function have ended. However, this could be a problem while
 * performing peeking code. You can provide a AXL_FALSE value to make the
 * function to not accept the terminator found as to be consumed.
 *
 * @param chunk_num The number of chunks to be checked as a valid terminators.
 * 
 * @return The chunk recognizied, not including the terminator that
 * have made this operation to stop. Rembember to check the
 * chunk_matched variable to be equal to -2. This will mean that the
 * string returned doesn't match any terminator provided because end
 * of the stream was reached while looking for them.
 */
char      * axl_stream_get_until_zero  (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					axl_bool    accept_terminator,
					int         chunk_num, ...)
{
	char * result;
	va_list args;
	
	/* open the standard argument */
	va_start (args, chunk_num);

	/* call to get next chunk separated by the provided values */
	stream->zero = axl_true;
	result       = axl_stream_get_untilv (stream, valid_chars, chunk_matched, accept_terminator, NULL, chunk_num, args);
	stream->zero = axl_false;

	/* close the standard argument */
	va_end (args);

	/* return value read */
	return result;
}

char      * axl_stream_get_until_ref_zero  (axlStream * stream, 
					    char      * valid_chars, 
					    int       * chunk_matched,
					    axl_bool    accept_terminator,
					    int       * result_size,
					    int         chunk_num, ...)
{
	char * result;
	va_list args;
	
	/* open the standard argument */
	va_start (args, chunk_num);

	/* call to get next chunk separated by the provided values */
	stream->zero = axl_true;
	result       = axl_stream_get_untilv (stream, valid_chars, chunk_matched, accept_terminator, result_size, chunk_num, args);
	stream->zero = axl_false;

	/* close the standard argument */
	va_end (args);

	/* return value read */
	return result;
}

/** 
 * @brief Allows to configure the handler to be executed to alloc
 * memory for the axl_stream_get_until* API.
 * 
 * @param stream The stream to configure with the alloc function.
 *
 * @param handler The handler to be called when the streams requires
 * to alloc memory.
 *
 * @param data User defined pointer to be called to the allocator
 * function defined by the <b>handler</b> parameter.
 */
void       axl_stream_set_buffer_alloc   (axlStream      * stream,
					  axlStreamAlloc   handler,
					  axlPointer       data)
{
	axl_return_if_fail (stream);

	/* just configure the alloc handler */
	stream->alloc      = handler;
	stream->alloc_data = data;

	return;
}

/** 
 * @brief Allows to nullify the internal reference of the stream,
 * making that reference to be not deallocated once the stream is
 * moving.
 *
 * This is mainly used to reduce the malloc/free round trip while
 * using the stream abstraction, making the stream received from the
 * memory chunk to be allocated only once, avoiding the double
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
 * Wide implementation for axl stream get until (which checks for
 * every index chunks provided instead of checking the first one until
 * a prebuffer operation is required).
 */
char * __axl_stream_get_untilv_wide (axlStream * stream, va_list args)
{

	int          iterator    = 0;
	int          index       = 0;
	int          _index      = 0;
	int          length      = 0;
	int          max_length  = 0;
	axl_bool     matched;
	char       * string      = NULL;
	axl_bool     match_empty = axl_false;
	int          empty_index = 0;

	/* get how many bytes remains to be read */
	int          remains;
	
	/* set current matched value */
	stream->chunk_matched = -1;
	
	/* iterate over the chunk list */
	while (iterator < stream->chunk_num) {

		/* get the chunk */
		stream->chunks [iterator]  = va_arg (args, char *);

		/* check if we have to match the emtpy string, and
		 * don't install the value to be matched */
		if (axl_cmp (stream->chunks[iterator], " ")) {
			match_empty = axl_true;

			/* reset the length size to detect an emtpy
			 * string */
			stream->lengths [iterator] = 0;
			empty_index                = iterator;
		}else {
			/* get the size */
			stream->lengths [iterator] = strlen (stream->chunks [iterator]);
		}

		/* get current length */
		if (stream->lengths [iterator] > max_length)
			max_length = stream->lengths [iterator];

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found matching chunk: '%s' length: %d", 
			   stream->chunks[iterator], stream->lengths [iterator]);

		/* update index */
		iterator ++;
	}

	/* calculate how many bytes ara available */
	remains = stream->stream_size - stream->stream_index;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "remaining data to be inspected: %d bytes (stream->stream-size=%d, stream->stream-index=%d)",
		   remains, stream->stream_size, stream->stream_index);

	/* now we have chunks to lookup, stream until get the stream
	 * limited by the chunks received. */
	do {

		/* decrease remain bytes to be read until perform a
		 * prebuffer operation */
		remains--;

		/* only prebuffer for this type of stream */
		if (stream->type == STREAM_FD) {
			
			/* check if the index is falling out side the buffer boundaries */
			if (remains < 0) {

				if (! axl_stream_prebuffer (stream)) {
					/* check if a call to zero was found */
					if (stream->zero) {
						/* flag that chunk matched
						 * will be -2 */
						stream->chunk_matched = -2;
						goto matched_return_result;
					} /* end if */
					__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "failed while prebuffer (stream->type = %d)", stream->type);

					return NULL;
				}
				
				/* update remains value but this time removing
				 * one unit (the unit to be consumed at the
				 * next sentence) */
				remains = stream->stream_size - index - 1;

				/* work around: because the index is updated
				 * at the end of the loop, it is required to
				 * decrease the index in one unit in the case
				 * a prebuffer operation happens */
				if (index > 0) {
					index--;
				}
				
			} /* end if */
		}

		/* check we don't get out side the memory */
		if (stream->type == STREAM_MEM) {
			if (remains < 0) {

				/* check for zero stream ended
				 * support */
				if (stream->zero) {
					/* flag that chunk matched
					 * will be -2 */
					stream->chunk_matched = -2;
					goto matched_return_result;
				}

				/* seems there is no more room to
				 * read */
				return NULL;
			} /* end if */
		} /* end if */

		
		/* compare chunks received for each index increased
		 * one step */
	init_get_until:
		_index   = stream->stream_index + index;
		matched  = axl_false;
		iterator = 0;

		/* before iterating, check if we have to match the
		 * empty string */
		if (match_empty) {
			/* check for a white space item */
			if ((stream->stream[_index] == ' ')  ||
			    (stream->stream[_index] == '\n') ||
			    (stream->stream[_index] == '\t') ||
			    (stream->stream[_index] == '\r')) {

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "matched by white space value matched_chunk=%d",
					   iterator);

				/* string matched */
				length         = 1;
				matched        = axl_true;
				iterator       = empty_index;
			}
		} /* end if */

		/* the empty string wasn't matched, now check for the
		 * rest of chunks */
		while ((! matched) && (iterator < stream->chunk_num)) {
			
			/* get current length for the chunk to check */
			length  = stream->lengths [iterator];

			/* check the length returned */
			matched = axl_false;
			if (length > 0 && ((_index + length) <= stream->stream_size)) {
				
				/* try to figure out if the next
				 * string match */
				if (stream->chunks [iterator][0] == stream->stream [_index]) {
					
					if ((length == 1) ||
					    (axl_memcmp (stream->chunks [iterator] + 1, stream->stream + _index + 1, length -1))) {
						__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "matched as normal situation.. at index=%d (stream size=%d)",
							   stream->stream_index + _index + 1, stream->stream_size);
						/* flag as matched */
						matched = axl_true;
					} /* end if */
				} /* end if */
			} /* end if */
			
			/* check if we have found the chunk we were looking */
			if (! matched) {
				/* update iterator */
				iterator ++;
			}
		} /* end while */

		/* check that the function have found a chunk */
		if (matched) {
			/* check for matching a more specific
			 * terminal than a general */
			if ((length < max_length) && 
			    ((_index + length) == stream->stream_size)) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "detected possible false positive");
				
				/* do a prebuffer operation,
				 * and if success, try to
				 * check again all chunks at
				 * the current index */
				if (axl_stream_prebuffer (stream))
					goto init_get_until;
			}

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "matched_chunk=%d", iterator);
			
			/* report which is the chunk being
			 * matched by the expresion */
			stream->chunk_matched = iterator;

		matched_return_result:
			
			/* result is found from last stream
			 * index read up to index */
			if (stream->last_chunk != NULL) {
				axl_free (stream->last_chunk);
				stream->last_chunk = NULL;
			}
			
			/* get a copy to the chunk to be returned */
			if (! stream->result_size) {
				if (stream->alloc != NULL)
					stream->last_chunk = stream->alloc (index + 1, stream->alloc_data);
				else 
					stream->last_chunk = axl_new (char, index + 1);
				memcpy (stream->last_chunk, stream->stream + stream->stream_index, index);
			}else {
				/* *result_size = index;*/
				stream->result_size = index;
				string              = stream->stream + stream->stream_index;
				
				/* set a termination mark */
				string [ index  ] = 0;
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG,
					   "Nullify internally index=%d to avoid printf problems", index);
			}
			
			/* in the case a memory chunk is being read */
			if (stream->accept_terminator) {
				stream->stream_index     += length;
				stream->global_index     += length;
			}
			stream->stream_index             += index;
			stream->global_index             += index;
			stream->previous_inspect          = 0;
			
			
			/* return allocated result */
			if (! stream->result_size)
				return stream->last_chunk;

			/* return reference result */
			return string;
		} /* end if */
		
		/* it seems that the chunk wasn't found */
		index++;

	}while (axl_true);

	/* return a NULL chunk. */
	return NULL;	
}

/** 
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
 * @param result_size Allows to notify the caller with the chunk size
 * that is being returned by the function.
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
					axl_bool    accept_terminator,
					int       * result_size,
					int         chunk_num, 
					va_list args)
{
	char * result;


	/* check max inspected chunks */
	if (chunk_num > MAX_INSPECTED_CHUNKS) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "unable to parse stream for the number of chunks to recognize. Max number supported is %d, but received %d",
			   MAX_INSPECTED_CHUNKS, stream->chunk_num);
		return NULL;
	}

	/* configure variables for the operation */
	stream->valid_chars       = valid_chars;
	stream->accept_terminator = accept_terminator;
	stream->result_size       = (result_size != NULL);
	stream->chunk_num         = chunk_num;

	/* call to current implementation */
	result = __axl_stream_get_untilv_wide (stream, args);

	/* check for returning references */
	if (result_size != NULL)
		*result_size   = stream->result_size;
	if (chunk_matched != NULL)
		*chunk_matched = stream->chunk_matched;

	/* return string matched */
	return result;
}


/** 
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
 * @brief Allows to get current status of the given stream, taking the
 * current index, getting an amount of <b>count</b> bytes before and
 * after the given index.
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
const char  * axl_stream_get_near_to     (axlStream * stream, int count)
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
 * @brief Allows to get the following <b>count</b> bytes read from the
 * stream.
 * 
 * @param stream The stream where the operation will be performed.
 * @param count How many bytes to get from the stream.
 * 
 * @return A string referece, containing the first <b>count</b> bytes
 * or NULL if fails. Reference returned shouldn't be deallocated.
 */
const char  * axl_stream_get_following   (axlStream * stream, int count)
{
	axl_return_val_if_fail (stream, NULL);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting next characters from stream: index=%d size=%d",
		   stream->stream_index, stream->stream_size);

	/* check index */
	if (stream->stream_index >= stream->stream_size)
		return NULL;

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
	axl_bool         free_on_finish;
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
 * @brief Associates the given \ref axlPointer with the given stream to be
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
	/* call to base implementation */
	axl_stream_link_full (stream, element, func, axl_false);

	return;
}

/** 
 * @brief Allows to associate data references with a destroy function,
 * like \ref axl_stream_link, but ensuring the object reference will
 * be released once finished the axl stream, no mather if the
 * application code calls to \ref axl_stream_unlink.
 * 
 * @param stream The axlStream where the document will be linked to.
 *
 * @param element The element to link (may a axlDoc or a axlDtd).
 *
 *
 * @param func The function to call once the stream is released.
 *
 * @param free_on_finish axl_true to make the reference to be released on
 * \ref axlStream deallocation. Providing \ref axl_false to this value is
 * equivalent to call to \ref axl_stream_link directly.
 */
void       axl_stream_link_full     (axlStream  *   stream,
				     axlPointer     element,
				     axlDestroyFunc func,
				     axl_bool       free_on_finish)
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
	data                 = axl_new (AxlStreamAssociatedData, 1);
	data->data           = element;
	data->destroy_func   = func;
	data->free_on_finish = free_on_finish;

	/* add the item to be destroy once the stream is unrefered */
	axl_list_add (stream->elements_linked, data);

	return;
}

/** 
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
		if (! data->free_on_finish) {
			data->data         = NULL;
			data->destroy_func = NULL;
		}
		iterator++;
	}
	return;
}

/** 
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

	/* free lengths */
	axl_free (stream->lengths);

	/* free temporal buffer */
	axl_free (stream->temp);
	axl_free (stream->decode_temp);
	axl_free (stream->source_encoding);

	/* release memory allocated by the stream received. */
	axl_free (stream);

	return;
}


/** 
 * @brief Allows to check if the given chunk is a white space in the
 * same of the XML 1.0 Third edition.
 *
 * The XML standard understand the "white space", also reffered as S,
 * as the following characters: \\x20 (the white space itself), \\n, \\r
 * and \\t.
 *
 * This function allows to check if the given chunk contains a white
 * space, in the previous sense.
 * 
 * @param chunk The chunk to check
 * 
 * @return axl_true if the chunk contains a white space or axl_false
 * if not.
 */
axl_bool        axl_stream_is_white_space  (char * chunk)
{
	/* do not complain about receive a null refernce chunk */
	if (chunk == NULL)
		return axl_false;
	
	if (chunk[0] == ' ')
		return axl_true;
	if (chunk[0] == '\n')
		return axl_true;
	if (chunk[0] == '\t')
		return axl_true;
	if (chunk[0] == '\r')
		return axl_true;

	/* no white space was found */
	return axl_false;
}

/** 
 * @brief Support function which consumes white spaces in the W3C
 * sense.
 * 
 * @param stream The stream where the operation will be performed.
 * 
 * @return axl_true if more white spaces could be consumed, axl_false
 * if not.
 */
void axl_stream_consume_white_spaces (axlStream * stream)
{
	/* get how many bytes remains to be read */
	int remains = stream->stream_size - stream->stream_index;

	while (axl_true) {
		/* decrase the number of bytes remaining to be read
		 * and check if it is zero or less than zero to
		 * prebuffer. NOTE: remains could be 1 (remains one
		 * byte) making it possible to consume one byte
		 * more */
		remains--;
		if (remains < 0) {
			/* we fall outside the stream, so a prebuffer
			 * operation is required */
			if (! axl_stream_prebuffer (stream))
				return;
			
			/* update remains value but this time removing
			 * one unit (the unit to be consumed at the
			 * next sentence) */
			remains = stream->stream_size - stream->stream_index - 1;
                }
		
		/* check for a white space */
		if ((stream->stream[stream->stream_index] == ' ')  ||
		    (stream->stream[stream->stream_index] == '\n') ||
		    (stream->stream[stream->stream_index] == '\t') ||
		    (stream->stream[stream->stream_index] == '\r')) {

			/* update internal indexes */
			stream->stream_index++;
			stream->global_index++;
			stream->previous_inspect  = 0;
		}else {
			/* return */
			return;
		}
		
	} /* end while */

	return;
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
 * @return axl_true if both string are equal, axl_false if not. If
 * some value provided is NULL or the size to compare is not greater
 * than 0 the function will return axl_false directly.
 */
axl_bool        axl_stream_cmp             (const char * chunk1, const char * chunk2, int size)
{
	/* perform some environmental condition checking */
	if (chunk1 == NULL)
		return axl_false;
	if (chunk2 == NULL)
		return axl_false;
	if (size < 0)
		return axl_false;
	
	/* report current comparation status */
	if ((chunk1[0] == chunk2[0])) {
		if ((size == 1) ||
		    (axl_memcmp (chunk1 + 1, chunk2 + 1, size -1))) {
			return axl_true;
		} /* end if */
	} /* end if */

	return axl_false;
}

/** 
 * @brief Provides the same function like axl_stream_cmp but
 * ignoring the case of the characters (case insensitive manner).
 * 
 * @param chunk1 The string to be compared.
 *
 * @param chunk2 The second string to be compared.
 *
 * @param size The amount of bytes to be compared for the two incoming
 * values.
 * 
 * @return axl_true if both string are equal, axl_false if not. If
 * some value provided is NULL or the size to compare is not greater
 * than 0 the function will return axl_false directly.
 */
axl_bool        axl_stream_casecmp           (const char * chunk1, const char * chunk2, int size)
{
	/* perform some environmental condition checking */
	if (chunk1 == NULL)
		return axl_false;
	if (chunk2 == NULL)
		return axl_false;
	if (size < 0)
		return axl_false;

	/* returh if both strings are equal. */
#if defined(AXL_OS_WIN32)
	return _strnicmp (chunk1, chunk2, size) == 0;
#else
	return strncasecmp (chunk1, chunk2, size) == 0;
#endif
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
 * @return \ref axl_true if the provided chunk falls out side the
 * stream boundaries, or axl_false if requested inspected size could
 * be supported.
 */
axl_bool axl_stream_fall_outside (axlStream * stream, int inspected_size)
{
	/* if the content is inside memory, check it */
	if (fall_out_side_checking (stream, inspected_size)) {
		return (! axl_stream_prebuffer (stream));
	}
	
	/* otherwise, axl_false is returned */
	return axl_false;
}

/** 
 * @internal
 *
 * @brief Allows to check if the given stream have as a next element
 * the provided chunk.
 * 
 * @param stream The stream where the operation will be performed.
 * @param chunk The chunk to check
 * 
 * @return Returns axl_true if the given stream contains the value requested
 * or axl_false if not.
 */
axl_bool         axl_stream_check           (axlStream * stream, char * chunk, int inspected_size)
{
	int iterator;

	/* call to the internal implementation of axl_memcmp */
	_memcmp(iterator,chunk,(stream->stream + stream->stream_index), inspected_size);

}

/** 
 * @brief Allows to get current status of the stream. 
 *
 * If the is exhausted and have no more data to be read.
 * 
 * @param stream The stream that is being checked.
 * 
 * @return axl_true if the stream is exhausted or axl_false if not.
 */
axl_bool        axl_stream_remains         (axlStream * stream)
{
	axl_return_val_if_fail (stream, axl_false);

	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking for stream status with stream index=%d and stream size=%d",
		   stream->stream_index, stream->stream_size);
		
	/* check if the stream is exhausted */
	if (stream->stream_index >= (stream->stream_size)) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "prebufferring from remains");

		/* in the case the stream is exhausted, try to read
		 * more content from the streaming */
		return axl_stream_prebuffer (stream);
	}
	return axl_true;
}


/* @} */

/**
 * \defgroup axl_string_module Axl String: String functions provided by the Axl Stream module.
 */

/** 
 * \addtogroup axl_string_module
 * @{
 */

/** 
 *
 * @brief Allows to trim the provided chunk, removing all white spaces
 * (returns, white spaces, carry return and tabulars) that comes as
 * preffix and suffix for the string provided, referenced by chunk.
 *
 * The function retuns the reference to the new chunk already
 * translated. The function doesn't perform any memory allocation. It
 * uses the memory already used to hold the chunk provided, returning
 * a pointer for the first item that is not a white space and
 * nullifing the first item that is a white space behind the chunk.
 *
 * This function is particular useful while getting the content 
 * 
 * @param chunk The chunk to trim.
 *
 */
void      axl_stream_trim            (char * chunk)
{
	/* call to trim */
	axl_stream_trim_with_size (chunk, NULL);

	return;
}

/** 
 * @brief The function works like \ref axl_stream_trim, but providing
 * the count of bytes trimmed from the string.
 *
 * @param chunk The chunk to trim.
 *
 * @param trimmed An optional reference that returns the count of bytes
 * trimmed by the operation.
 */
void        axl_stream_trim_with_size  (char * chunk, int * trimmed)
{
	int    iterator;
	int    iterator2;
	int    end;
	int    total;

	/* perform some environment check */
	axl_return_if_fail (chunk);

	/* check empty string received */
	if (strlen (chunk) == 0) {
		if (trimmed)
			*trimmed = 0;
		return;
	}

	/* check the amount of white spaces to remove from the
	 * begin */
	iterator = 0;
	while (chunk[iterator] != 0) {
		
		/* check that the iterator is not pointing to a white
		 * space */
		if (! axl_stream_is_white_space (chunk + iterator))
			break;
		
		/* update the iterator */
		iterator++;
	}

	/* check for the really basic case where an empty string is found */
	if (iterator == strlen (chunk)) {
		/* an empty string, trim it all */
		chunk [0] = 0;
		if (trimmed)
			*trimmed = iterator;
		return;
	} /* end if */

	/* now get the position for the last valid character in the
	 * chunk */
	total   = strlen (chunk) -1;
	end     = total;
	while (chunk[end] != 0) {
		
		/* stop if a white space is found */
		if (! axl_stream_is_white_space (chunk + end)) {
			break;
		}

		/* update the iterator to eat the next white space */
		end--;
	}

	/* the number of items trimmed */
	total -= end;
	total += iterator;
	
	/* copy the exact amount of non white spaces items */
	iterator2 = 0;
	while (iterator2 < (end - iterator + 1)) {
		/* copy the content */
		chunk [iterator2] = chunk [iterator + iterator2];

		/* update the iterator */
		iterator2++;
	}
	chunk [ end - iterator + 1] = 0;

	if (trimmed != NULL)
		*trimmed = total;

	/* return the result reference */
	return;	
}

/** 
 * @brief Allows to remote occurences of value from the provided
 * string (chunk).
 *
 * The function do not allocate new memory for the result. All
 * operations are applied to the string received (chunk).
 *
 * The idea behind the functions is to allow removing values from the
 * string, joining remaining content. For example, removing "-" from
 * the string "iso-8859-15" yields "iso885915".
 * 
 * @param chunk The string that holds values to be removed.  
 *
 * @param value The value to be removed.
 *
 * @param first If only the first ocurrence of value must be removed,
 * otherwise all ocurrences will be removed from the string.
 */
void        axl_stream_remove            (char * chunk, const char * value, axl_bool first)
{
	int iterator;
	int iterator2;
	int length;
	int v_length;

	axl_return_if_fail (chunk);
	axl_return_if_fail (value);

	/* get lengths */
	length   = strlen (chunk);
	v_length = strlen (value);

	/* check for basic cases */
	if (length == v_length) {
		/* check if both strings are equal, then nullify */
		if (axl_cmp (chunk, value))
			chunk [0] = 0;
		return;
	} else if (length < v_length) {
		/* nothing to remove because parent string is too
		 * small to store the content */
		return;
	} /* end if */
		

	/* locate the string */
	iterator = 0;
	while (iterator < length) {
		/* check if the string value was found */
		if (axl_memcmp (chunk + iterator, value, v_length)) {
			/* string found, move content remaining (if is found) */
			if ((length - iterator - v_length) > 0) {
				iterator2 = 0;
				while (iterator2 < (length - iterator - v_length)) {
					chunk [iterator + iterator2] = chunk [iterator + iterator2 + v_length];
					iterator2++;
				} /* end while */
			} /* end if */
			
			/* update length to the new value */
			length -= v_length;

			/* check to terminate for first ocurrence */
			if (first) {
				chunk [length] = 0;
				return;
			}
			continue;
		} /* end if */

		/* next position */
		iterator++;
	} /* end while */

	/* nullify and terminate */
	chunk [length] = 0;
	return;
}

/** 
 * @brief Allows to copy the given chunk, supposing that is a properly
 * format C string that ends with a '\\0' value.
 *
 * This function allows to perform a copy for the given string. If a
 * copy limited by a size is required, use \ref axl_stream_strdup_n.
 * 
 * @param chunk The chunk to copy
 * 
 * @return A newly allocated string or NULL if fails.
 */
char      * axl_stream_strdup          (const char * chunk)
{
	char * result;
	int    length;

	/* return NULL reference if a NULL reference is received */
	if (chunk == NULL)
		return NULL;

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
char      * axl_stream_strdup_n (const char * chunk, int n)
{
	char * result;

	axl_return_val_if_fail (chunk, NULL);
	axl_return_val_if_fail (n, NULL);
	
	result = axl_new (char, n + 1);
	memcpy (result, chunk, n);
	
	return result;
}

/** 
 * @brief Allows get current limit for axl_stream_printf operations.
 *
 * By default, Axl API has no limit for these operation unless there
 * is no suitable secure API to implement these operation. 
 *
 * This function allows to check what's the current limit of the
 * binary being used to to let upper level libraries and/or
 * applications that are the limits for these axl_strdup_printf
 * operations.
 *
 * @return -1 if no limit is found, or a number > 0 letting the limit
 * for these operations.
 */
int axl_stream_printf_limit (void) {

#if defined (AXL_OS_WIN32) && ! defined (__GNUC__)
#   if HAVE_VSCPRINTF
	return -1;
#   else
	return 8192;
#   endif
#else
	return -1;
#endif
}

/** 
 * @internal Allows to calculate the amount of memory required to
 * store the string that will representing the construction provided
 * by the printf-like format received and its arguments.
 * 
 * @param format The printf-like format to be printed.
 *
 * @param args The set of arguments that the printf applies to.
 *
 * <i><b>NOTE:</b> not all printf specification is supported. Generally, the
 * following is supported: %s, %d, %f, %g, %ld, %lg and all
 * combinations that provides precision, number of items inside the
 * integer part, etc: %6.2f, %+2d, etc. An especial case not supported
 * is %lld, %llu and %llg.</i>
 *
 * @return Return the number of bytes that must be allocated to hold
 * the string (including the string terminator \0). If the format is
 * not correct or it is not properly formated according to the value
 * found at the argument set, the function will return -1.
 */
int axl_stream_vprintf_len (const char * format, va_list args)
{
	/** IMPLEMENTATION NOTE: in the case this code is update,
	 * update exarg_vprintf_len **/

#if defined (AXL_OS_WIN32) && ! defined (__GNUC__)
#   if HAVE_VSCPRINTF
	if (format == NULL)
		return 0;
	return _vscprintf (format, args) + 1;
#   else
	char buffer[8192];
	if (format == NULL)
		return 0;
	return _vsnprintf (buffer, 8191, format, args) + 1;
#   endif
#else
	/* gnu gcc case */
	if (format == NULL)
		return 0;
	return vsnprintf (NULL, 0, format, args) + 1;
#endif
}

/** 
 * @brief Allows to perform a printf operation on the provided buffer
 * (which must be allocated by the caller, and its size signaled by
 * buffer_size).
 *
 *
 * @param buffer The already allocated buffer to hold the result.
 *
 * @param buffer_size The size of the buffer provided. 
 *
 * @param real_size Optional reference where the real space required
 * to hold the content will be placed. In cases where the content is
 * enough small to hold in the buffer, this value will contain the
 * same value as returned by the function. In the case the buffer
 * provide can't hold all the content, the function will return at
 * maximum (buffer_size - 1) bytes written, that is, all content that
 * was possible to be included plus a trailing \\0 to terminate the
 * string, and, if defined <i>real_size</i> variable, it will contain
 * the space that will be required.
 *
 * @param format The printf like format to use to create the content
 * to be placed at the buffer provided.
 *
 * @return The amount of bytes written. The function will return at
 * maximum buffer_size - 1 bytes written. Use <i>real_size</i>
 * variable to check if the function was able to write all content (if
 * real_size == value returned). 
 */
int axl_stream_printf_buffer (char * buffer, 
			      int    buffer_size, 
			      int  * real_size,
			      const char * format, ...)
{
	va_list args;
	int     result;
#if defined(AXL_OS_WIN32)
	int     check_size;
#endif 

	/* check foramt and optn */
	if (format == NULL) {
		/* clean real size if it was defined */
		if (real_size)
			(*real_size) = 0;
		return 0;
	}
	/* open stdargs */
	va_start (args, format);

# if defined (AXL_OS_WIN32)
	/* because undre windows all its string family of functions
	 * return -1 in the case not enough espace is available, we
	 * have to check first before calling to _vsnprintf */
	check_size = axl_stream_vprintf_len (format, args);

	/* call to close to avoid problems with amd64 platforms */
	va_end (args);
	va_start (args, format);

	/* windows case */
	result = _vsnprintf (buffer, buffer_size, format, args);
	if (result == -1) {
		/* nullify last character and update "result" to point
		 * to the amount of data written (buffer - 1). */
		result = (buffer_size - 1);
		buffer[result] = 0; 
	} /* end if */
#else
	/* gnu gcc case */
	result = vsnprintf (buffer, buffer_size, format, args);
#endif
	/* close stdarg */
	va_end (args);

	/* report real size required */
	if (real_size) {
#if defined(AXL_OS_WIN32)
		(*real_size) = check_size - 1;
#else
		(*real_size) = result;
#endif
	} /* end if */

	/* limit result */
	if (result > (buffer_size - 1))
		result = (buffer_size - 1);

	return result;
}

/** 
 * @internal Function that allows to get how many bytes will be
 * required to hold the format and the arguments provided.
 * 
 * @param format The printf-like format followed by the rest of
 * arguments.
 * 
 * @return Return the number of bytes that must be allocated to hold
 * the string (including the string terminator \0). If the format is
 * not correct or it is not properly formated according to the value
 * found at the argument set, the function will return -1.
 */
int axl_stream_printf_len (const char * format, ...)
{
	int     result;
	va_list args;

	va_start (args, format);

	/* get the result */
	result = axl_stream_vprintf_len (format, args);
	
	va_end (args);

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
char      * axl_stream_strdup_printf   (const char * chunk, ...)
{
	char    * result   = NULL;
	va_list   args;
	
	axl_return_val_if_fail (chunk, NULL);

	/* open std args */
	va_start (args, chunk);

	/* get the string */
	result = axl_stream_strdup_printfv (chunk, args);
	
	/* close std args */
	va_end (args);
	
	return result;
}

/** 
 * @brief DEPRECATED: Allows to produce an string representing the
 * message hold by chunk with the parameters provided.
 * 
 * @param chunk The message chunk to print.
 * @param args The arguments for the chunk.
 * 
 * @return A newly allocated string.
 *
 * IMPLEMENTATION NOTE: This function may have a fundamental bug due
 * to the design of va_list arguments under amd64 platforms. In short,
 * a function receiving a va_list argument can't use it twice. In you
 * are running amd64, check your axl_config.h did find
 * AXL_HAVE_VASPRINTF.
 */
char  * axl_stream_strdup_printfv    (const char * chunk, va_list args)
{
	/** IMPLEMENTATION NOTE: place update exarg_strdup_printfv
	 * code in the case this code is updated **/

#ifndef AXL_HAVE_VASPRINTF
	int       size;
#endif
	char    * result   = NULL;

	axl_return_val_if_fail (chunk, NULL);

#ifdef AXL_HAVE_VASPRINTF
	/* do the operation using the GNU extension */
	if (vasprintf (&result, chunk, args) == -1)
		return NULL;
#else
	/* get the amount of memory to be allocated */
	size = axl_stream_vprintf_len (chunk, args);

	/* check result */
	if (size == -1) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "unable to calculate the amount of memory for the strdup_printf operation");
		return NULL;
	} /* end if */

	/* allocate memory */
	result   = axl_new (char, size + 2);
	
	/* copy current size */
#    if defined(AXL_OS_WIN32) && ! defined (__GNUC__)
	_vsnprintf_s (result, size + 1, size, chunk, args);
#    else
	vsnprintf (result, size + 1, chunk, args);
#    endif
#endif
	/* return the result */
	return result;
}

/** 
 * @brief Allows to create a newly allocated chunk, providing its
 * values as a printf call function, but also returning the chunk
 * size.
 *
 * This function works like \ref axl_stream_strdup_printf, but
 * providing an integer reference where the result chunk length will
 * be returned. 
 * 
 * @param chunk The printf chunk format to allocate.
 *
 * @param chunk_size A reference to fill the chunk lenght.
 * 
 * @return A newly allocated chunk.
 */
char    * axl_stream_strdup_printf_len (const char * chunk, int * chunk_size, ...)
{
#ifndef AXL_HAVE_VASPRINTF
	int       size;
#endif
	int       new_size;
	char    * result;
	va_list   args;
	
	axl_return_val_if_fail (chunk, NULL);

	/* open std args */
	va_start (args, chunk_size);

#ifdef AXL_HAVE_VASPRINTF
	/* do the operation using the GNU extension */
	new_size = vasprintf (&result, chunk, args);

	/* reopen to avoid amd64 bug */
	va_end (args);
	va_start (args, chunk_size);
#else
	/* get the amount of memory to be allocated */
	size = axl_stream_vprintf_len (chunk, args);

	/* reopen to avoid amd64 bug */
	va_end (args);
	va_start (args, chunk_size);
	
	/* check result */
	if (size == -1) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "unable to calculate the amount of memory for the strdup_printf operation");
		return NULL;
	} /* end if */

	/* allocate memory */
	result   = axl_new (char, size + 2);

	/* copy current size */
#if defined(AXL_OS_WIN32) && ! defined (__GNUC__)
	new_size = _vsnprintf_s (result, size + 1, size, chunk, args);
#else
	new_size = vsnprintf (result, size + 1, chunk, args);
#endif
#endif
	
	/* close std args */
	va_end (args);

	/* fill the chunk size result */
	if (chunk_size != NULL)
		*chunk_size = new_size;

	return result;
}

/** 
 * @brief Allows to split the provided chunk, into several pieces that
 * are separated by the separator (or separators) provided.
 *
 * The function will try to split the chunk provide using the
 * separator provided, and optionally, all separators provided.
 *
 * Here is an example:
 * \code
 * char ** result;
 *
 * // split the provided value using the ':', ';' and ',' as separators.
 * result = axl_stream_split (value, 3, ":", ";", ",");
 * \endcode
 *
 * The value returned must be deallocated using \ref axl_stream_freev.
 *
 * @param chunk The chunk to split.
 *
 * @param separator_num The number os separators to be used while
 * spliting the chunk.
 * 
 * @return A newly allocated string, that must be deallocated by using
 * \ref axl_stream_freev. The function will return a NULL if the chunk
 * or the separators provided are NULL.
 *
 * NOTE: See also \ref axl_split.
 */
char     ** axl_stream_split           (const char * chunk, int separator_num, ...)
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
	} /* end if */
	
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
			if (axl_memcmp (chunk + index, separators[iterator], length)) {

				/* update items found */
				count++;

				/* update index to skip the item found */
				index += length - 1; /* make the last index to be captured the the -1 */

				/* break the loop */
				break;
			}
			iterator++;
		}

		/* update the index to the next item */
		index++;
	} /* end if */
	
	/* create the result that will hold items separated */
	result = axl_new (char *, count + 2);

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
			if (axl_memcmp (chunk + index, separators[iterator], length)) {

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
 * @brief Allows to clean an split created by \ref axl_stream_split by
 * removing all items found to be empty strings.
 * 
 * @param split The split to be updated by removing all empty string
 * items.
 *
 */
void        axl_stream_clean_split     (char ** split)
{
	int iterator;
	int iterator2;
	int iterator3;

	/* check input */
	axl_return_if_fail (split);

	/* remove empty strings */
	iterator = 0;
	while (split[iterator]) {
		if (strlen (split[iterator]) == 0) {

			/* clear position joint */
			axl_free (split[iterator]);
			split[iterator] = NULL;

			/* move strings */
			iterator3 = 0;
			iterator2 = iterator + 1;
			while (split[iterator2 + iterator3]) {
				/* move reference */
				split[iterator + iterator3]  = split[iterator2 + iterator3];
				
				/* nullify */
				split[iterator2 + iterator3] = NULL;
				
				/* next position */
				iterator3++;
			} /* end while */
			continue;
		} /* end if */

		/* next iterator */
		iterator++;
	} /* end while */

	return;
}

/** 
 * @brief Allows to implement the oposite operation of \ref
 * axl_stream_split, by joing all strings provided inside the array
 * (strings), using as separator the value provided.
 * 
 * @param strings The set of strings to be joined.
 *
 * @param separator The separator to be used to join all strings
 * provided.
 *
 * 
 * @return A newly allocated reference, that must be release using
 * \ref axl_free.
 *
 * NOTE: See also \ref axl_join.
 */
char      * axl_stream_join            (char      ** strings, 
					const char * separator)
{
	int         length;
	int         sep_length;
	int         iterator;
	char      * result;
	axl_bool    next_sep;

	axl_return_val_if_fail (strings && strings[0], NULL);
	axl_return_val_if_fail (separator, NULL);

	/* get the amount of data to be allocated */
	length   = 0;
	iterator = 0;

	/* for each value to be joined */
	while (strings [iterator]) {
		/* count the number of bytes for each string */
		length += strlen (strings[iterator]);

		/* next iterator */
		iterator++;
	} /* end while */

	/* check for the basic case */
	if (iterator == 1) {
		/* only one piece is contained in the set of strings
		 * provided, so nothing can be joined */
		return axl_strdup (strings[0]);
	}
	
	/* add to the length the number of separatos to be added
	 * (wihtout 1) and add a traling byte to terminate the
	 * string */
	sep_length = strlen (separator);
	length    += (sep_length * (iterator - 1)) + 1;
	result     = axl_new (char, length);

	iterator   = 0;
	next_sep   = axl_false;
	length     = 0;

	while (strings [iterator]) {

		/* copy the content */
		if (next_sep) {
			memcpy (result + length, separator, sep_length);

			/* update the length */
			length += sep_length;
		} else {
			memcpy (result + length, strings[iterator], strlen (strings[iterator]));

			/* update the length */
			length += strlen (strings[iterator]);
		} /* end if */

		/* check if next is separator */
		next_sep = ! next_sep;

		/* update the iterator only if next value to be
		 * handled is a separator */
		if (next_sep) 
			iterator++;
	} /* end while */

	/* return string created */
	return result;
}

/** 
 * @brief Allows to replace the provided string by the provided
 * replacement on the provided source string, doing the replacement in
 * an effective manner.
 *
 * @param source The source string where to look and replace. The
 * result will be reported as a new pointer on this parameter. The
 * function will dealloc previous string (passed in source).
 *
 * @param source_len The replace function can handle binary
 * strings. This parameter signals the function how many bytes are
 * found on the source pointer.
 *
 * @param string The string that will be looked for replacement. The
 * string can be binary data but its length must be configured.
 *
 * @param string_len String length or -1. The string parameter can be
 * binary data (may include \0) but length must be especified. If -1
 * is provided, the function will use strlen () to get current string
 * size.
 *
 * @param replacement The replace string. The replacement can be
 * binary data but its length must be configured.
 *
 * @param replacement_len Replacement string length or -1. The
 * replacement parameter can be binary data (may include \0) but
 * length must be especified. If -1 is provided, the function will use
 * strlen () to get current string size.
 *
 * @return The function returns the new size of the string. The
 * function returns the same source length when no replacement was
 * done. The function return source_len in the case some argument is NULL.
 */
int  axl_stream_replace         (char        ** source,      int source_len, 
				 const char   * string,      int string_len, 
				 const char   * replacement, int replacement_len)
{
	int    iterator;
	int    iterator2;
	int    count;
	char * result;
	int    old_source_len;

	/* check arguments */
	axl_return_val_if_fail (source && string && replacement, source_len);

	/* get sizes if not configured */
	if (source_len == -1)
		source_len = strlen (*source);
	if (string_len == -1)
		string_len = strlen (string);
	if (replacement_len == -1)
		replacement_len = strlen (replacement);

	/* find how many strings must be replaced */
	iterator = 0;
	count    = 0;
	while ((iterator + string_len - 1) < source_len) {
		/* check if the string is found */
		if (axl_memcmp ((*source) + iterator, string, string_len)) {
			/* string found ! */
			count++;
			
			/* skip these bytes */
			iterator += string_len;
			continue;
		}

		/* next position */
		iterator++;
	} /* end while */

	/* check if we have found some to replace */
	if (count == 0)
		return source_len;

	/* update source length */
	old_source_len = source_len;
	source_len     = source_len - (string_len * count) + (replacement_len * count);

	/* alloc memory for the replacement */
	result     = axl_new (char, source_len + 1);

	/* do replacement */
	iterator  = 0;
	iterator2 = 0;
	while (iterator < old_source_len) {
		/* check if the string is found */
		if (((iterator + string_len - 1) < old_source_len) && axl_memcmp ((*source) + iterator, string, string_len)) {
			/* string found!, replace */
			memcpy (result + iterator2, replacement, replacement_len);
			
			/* skip these bytes */
			iterator  += string_len;
			iterator2 += replacement_len;
			continue;
		}

		/* copy byte by byte */
		result[iterator2] = (*source)[iterator];

		/* next position */
		iterator++;
		iterator2++;
	} /* end while */

	/* release and report new string */
	axl_free (*source);
	*source = result;

	return source_len;
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
 *
 * NOTE: See also \ref axl_concat.
 */
char      * axl_stream_concat          (const char * chunk1, const char * chunk2)
{
	char * result;
	int    len1;
	int    len2;

	axl_return_val_if_fail ((chunk2 != NULL) || (chunk1 != NULL), NULL);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called..");

	if (chunk1 == NULL) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called.., returning %s", chunk2);

		/* return the result */
		return axl_strdup (chunk2);
	}

	if (chunk2 == NULL) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called.., returning %s", chunk1);

		/* return the result */
		return axl_strdup (chunk1);
	}

	/* return the concatenation */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "concat called.., returning %s%s", chunk1, chunk2);

	/* alloc enough memory to hold both strings */
	len1 = strlen (chunk1);
	len2 = strlen (chunk2);
	result = axl_new (char, len1 + len2 + 1);

	/* copy the content */
	memcpy (result, chunk1, len1);
	memcpy (result + len1, chunk2, len2);

	/* return the string created */
	return result;


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
 * @brief Allows to release memory used by elements returned by \ref
 * axl_stream_split and other function that return a pointer to a char **.
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
void __axl_stream_common_to (char * chunk, axl_bool to_upper)
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
	__axl_stream_common_to (chunk, axl_true);
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
	__axl_stream_common_to (chunk, axl_false);
	return chunk;
}

/** 
 * @brief Allows to perform a to upper operation, like \ref
 * axl_stream_to_upper, but returning an new allocated reference.
 * 
 * @param chunk The string reference to upper.
 * 
 * @return A new reference allocated containing the result or NULL if
 * it fails.
 */
char      * axl_stream_to_upper_copy   (const char  * chunk)
{
	char * result;

	/* perform some environmental checks */
	axl_return_val_if_fail (chunk, NULL);

	/* make a copy */
	result = axl_strdup (chunk);

	/* upper it */
	__axl_stream_common_to (result, axl_true);

	/* return the result */
	return result;
}


/** 
 * @brief Allows to perform a to lower operation, like \ref
 * axl_stream_to_upper, but returning an new allocated reference.
 * 
 * @param chunk The string reference to lower.
 * 
 * @return A new reference allocated containing the result or NULL if
 * it fails.
 */
char      * axl_stream_to_lower_copy   (const char  * chunk)
{
	char * result;

	/* perform some environmental checks */
	axl_return_val_if_fail (chunk, NULL);

	/* make a copy */
	result = axl_strdup (chunk);

	/* lower it */
	__axl_stream_common_to (result, axl_false);

	/* return the result */
	return result;
}

/** 
 * @brief Allows to compare two strings provided, s1 and s1 to be
 * equal.
 *
 * In the case both are equal, \ref axl_true is returned. Otherwise \ref
 * axl_false. The function compares that both are equal not only by making
 * the first to be contained inside the second string. The check also
 * ensures that "test" isn't equal to "test1".
 *
 * @param string First string to check.
 *
 * @param string2 Second string to check.
 * 
 * @return \ref axl_true if both string are equal, otherwise \ref axl_false is
 * returned.
 */
axl_bool axl_cmp (const char * string, const char * string2)
{
	int iterator = 0;

	if (string == NULL)
		return axl_false;
	if (string2 == NULL)
		return axl_false;
	
	/* for each item inside the iterator */
	while (string [iterator] != 0 && string2 [iterator] != 0) {
		
		/* check the content */
		if (string [iterator] != string2 [iterator])
			return axl_false;

		/* update the iterator */
		iterator++;
		
	} /* end while */
	
	/* check that both string ends at the same point */
	if (string [iterator] != 0 ||
	    string2 [iterator] != 0)
		return axl_false;
	
	return axl_true;
}

/** 
 * @brief Allows to do a case insensitive string comparison.
 *
 * @param string The first string to compare.
 *
 * @param string2 The second string to compare.
 *
 * @return axl_true in the case the two strings are equal, otherwise
 * axl_false is returned.
 */ 
axl_bool        axl_casecmp (const char * string, const char * string2)
{
	int length;

	if (string == NULL)
		return axl_false;
	if (string2 == NULL)
		return axl_false;

	/* get length associated to first string */
	length = strlen (string);
	if (length != strlen (string2))
		return axl_false;
	
	/* now check both lengths */
	return axl_stream_casecmp (string, string2, length);
	
}


/** 
 * @brief Allows to check if both strings provided are equal on its
 * initial size bytes.
 *
 * This function is more efficient than common memcmp because it
 * doesn't perform the additional work to figure out which are the
 * bytes that differ both strings.
 * 
 * @param string The string to check.
 *
 * @param string2 The second string to check.
 *
 * @param size The size to check for both strings to be equal.
 * 
 * @return \ref axl_true if the both strings are equal for its initial
 * size bytes or \ref axl_false if not.
 */
axl_bool axl_memcmp (const char * string, const char * string2, int size)
{
	int iterator = 0;

	_memcmp(iterator,string,string2,size);
}

/** 
 *
 * @brief Perform a memory copy from the string provided.
 * 
 * @param string The string to copy.
 * 
 * @return A newly allocated value or NULL if it fails. The value
 * returned, must be deallocated using \ref axl_free.
 */
char * axl_strdup (const char * string)
{
	return (string != NULL) ? (char *) axl_stream_strdup ((char *) string) : NULL;
}

/** 
 * @internal Function that handles decoding operations when decode
 * functions are defined.
 * 
 * @param stream The stream where the decode operation will be
 * performed. 
 *
 * @param error Optional \ref axlError reference where errors will be
 * reported.
 * 
 * @return axl_true if the operation was completed.
 */
axl_bool axl_stream_decode (axlStream  * stream, 
			char       * output, 
			int          output_max_size, 
			int        * output_decoded, 
			int        * op_result, 
			axlError  ** error)
{

	int result;
	int size;

	/* clear op_result if defined */
	if (op_result)
		*op_result = 0;

	/* decode content from the stream directly */
	result = stream->decode_f (
		/* source */
		stream->decode_temp,
		/* source size */
		stream->decode_temp_last,
		/* source encoding: encode used by the source
		 * content provided */
		stream->source_encoding,
		/* output of content decoded and its size */
		output,
		/* output max size */
		output_max_size,
		/* output decoded */
		output_decoded,
		/* source remaining */
		&(stream->decode_temp_remain),
		/* user defined data */
		stream->decode_f_data);

	/* set op result if defined */
	if (op_result)
		*op_result = result;
	
	/* check result */
	if (result == 0) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "after decode operation result=%d, output_decoded (new buffer size)=%d (from %d original bytes)",
			   result, output_decoded ? *output_decoded : 0, stream->decode_temp_last);
		axl_error_new (-1, "found internal failure at decode operation, unable to complete entity parsing",
			       stream, error);
		return axl_false;
	} /* end if */
	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "after decode operation result=%d, output_decoded (new buffer size)=%d",
		   result, output_decoded ? *output_decoded : 0);
	
	/* update axl stream internal state */
	if (result == 1) {
		/* then the conversión was complete a no data
		 * is pending the the temporal decode
		 * buffer */
		stream->decode_temp_index = 0;
		stream->decode_temp_last  = 0;
	} else if (result == 2) {
		/* not enough space was found at the destination
		 * buffer, pack content at the decode buffer at the
		 * next operation */
		size = stream->decode_temp_last - stream->decode_temp_remain;
		if (size <= 0) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, 
				   "found decode function return 2 (signaling pending data to be decoded) but last - remain yields to %d",
				   size);
			axl_error_new (-1, "found decode function return 2 (signaling pending data to be decoded) but last - remain yields to 0 or less", NULL, error);
			return axl_false;
		} /* end if */
		
		/* moving data */
		while (stream->decode_temp_index < size) { 
			stream->decode_temp[stream->decode_temp_index] = stream->decode_temp[stream->decode_temp_remain +  stream->decode_temp_index];
			stream->decode_temp_index++;
		} /* end while */
		
		/* now reset */
		stream->decode_temp_index = 0;
		stream->decode_temp_last  = size;
		
		/* reset to 1 since we have moved content to
		 * the begin of the buffer */
		result = 1;
	} /* end if */

	return (result == 1);
}

/** 
 * @internal Function used by the axl stream module to call to check
 * function defined.
 * 
 * @return axl_true if the check was fine, otherwise axl_false is returned.
 */
axl_bool axl_stream_content_check (axlStream * stream, const char * content, int content_length, axlError ** error)
{
	if (stream == NULL || content == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "content check function failed because null reference was received.");
		axl_error_new (-1, "content check function failed because null reference was received.", stream, error);
		return axl_false;
	} /* end if */
	
	/* return appropiate value */
	if (stream->check_f (content, content_length, stream->source_encoding, stream->check_f_data, error) == 1) 
		return axl_true;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "content check function have failed");

	/* check if error reference was defined */
	return axl_false;
}


/** 
 * @internal Allows to configure a decode functions to be used to
 * translate content read into utf-8. 
 * 
 * @param stream The stream to be configured.
 *
 * @param source_encoding Original source encode.
 *
 * @param decode_f The function to be called to translate/check
 * content read into utf-8. If a null value is provided the decode
 * function is removed.
 *
 * @param error The reference where errors will be reported.
 *
 * @return axl_true if the function setup decode handler properly
 * otherwise axl_false is returned.
 */
axl_bool        axl_stream_setup_decode        (axlStream         * stream,
						const char        * source_encoding,
						axlStreamDecode     decode_f,
						axlPointer          user_data,
						axlError         ** error)
{
	axl_return_val_if_fail (stream, axl_false);

	/* do not check if the decode_f function is NULL (it's a valid
	 * case) */
	stream->decode_f      = decode_f;
	stream->decode_f_data = user_data;

	/* store source encoding */
	if (source_encoding != NULL)
		stream->source_encoding = axl_strdup (source_encoding);

	/* call to check and decode if required bufferede content */
	if (stream->decode_f) {
		/* init decode buffer */
		stream->decode_temp_size = (stream->buffer_size * 2) + 1;
		stream->decode_temp      = axl_new (char, stream->decode_temp_size);

		/* move content into decode temporal buffer */
		memcpy (stream->decode_temp, 
			stream->stream + stream->stream_index, 
			stream->stream_size - stream->stream_index);
		stream->decode_temp_index = 0;
		stream->decode_temp_last  = stream->stream_size - stream->stream_index;
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "procesing %d bytes from decode buffer (total size: %d, current index: 0)", 
			   stream->decode_temp_last, stream->decode_temp_size);

		/* call to decode content */
		if (! axl_stream_decode (stream, 
					 /* output */
					 (stream->stream + stream->stream_index),
					 /* output max size */
					 stream->buffer_size - stream->stream_index,
					 /* output decoded */
					 &(stream->stream_size),
					 /* do not define op result */
					 NULL,
					 error))
			return axl_false;
		
		/* add to the stream size the current index */
		stream->stream_size += stream->stream_index;
		
	} /* end if */

	/* return result */
	return axl_true;
}

/** 
 * @brief Function that allows to configure a handler that is executed
 * to check content read into the axl stream buffer. See \ref
 * axlStreamContentCheck for more information.
 * 
 * @param stream The stream that is going to be configured.
 *
 * @param source_encoding The source encoding detected.
 *
 * @param check The function that implements the check.
 *
 * @param user_data User defined data to be passed to the check
 * function.
 *
 * @param error Optional \ref axlError reference where errors will be
 * reported.
 * 
 * @return The function returns axl_true if the cheker was installed and
 * first execution was completed.
 */
axl_bool    axl_stream_setup_check         (axlStream                * stream,
					    const char               * source_encoding,
					    axlStreamContentCheck      check,
					    axlPointer                 user_data,
					    axlError                ** error)
{
	axl_return_val_if_fail (stream, axl_false);

	/* do not check if the decode_f function is NULL (it's a valid
	 * case) */
	stream->check_f      = check;
	stream->check_f_data = user_data;

	/* store source encoding */
	if (source_encoding != NULL) 
		stream->source_encoding = axl_strdup (source_encoding);

	if (stream->check_f) {
		/* call to check */
		if (! axl_stream_content_check (stream, stream->stream + stream->stream_index, stream->stream_size - stream->stream_index, error)) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "content check function have failed, looks like there is a problem with content");
			return axl_false;
		} /* end if */
	} /* end if */

	return axl_true;
}

/* @} */

