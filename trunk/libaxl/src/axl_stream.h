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
 *         C/ Dr. Michavila N� 14
 *         Coslada 28820 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://fact.aspl.es
 */
#ifndef __AXL_STREAM_H__
#define __AXL_STREAM_H__

#include <axl_decl.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_stream_module
 * @{
 */

axlStream * axl_stream_new             (const char * stream_source, 
					int stream_size,
					const char * file_path,
					int    fd_handler,
					axlError ** error);

int         axl_stream_inspect         (axlStream * stream, const char * chunk, int inspected_size);

int         axl_stream_peek            (axlStream * stream, const char * chunk, int inspected_size);

int         axl_stream_inspect_several (axlStream * stream, int chunk_num, ...);

void        axl_stream_accept          (axlStream * stream);

void        axl_stream_move            (axlStream * stream, int index);

void        axl_stream_push            (axlStream * stream, const char * content, int size);

bool        axl_stream_fall_outside    (axlStream * stream, int inspected_size);

bool        axl_stream_check           (axlStream * stream, char * chunk, int inspected_size);

char      * axl_stream_get_until       (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int         chunk_num, ...);

char      * axl_stream_get_until_ref   (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int       * result_size,
					int         chunk_num, ...);

char      * axl_stream_get_until_zero  (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int         chunk_num, ...);

char      * axl_stream_get_until_ref_zero  (axlStream * stream, 
					    char      * valid_chars, 
					    int       * chunk_matched,
					    bool        accept_terminator,
					    int       * result_size,
					    int         chunk_num, ...);

void       axl_stream_set_buffer_alloc   (axlStream * stream,
					  axlStreamAlloc handler,
					  axlPointer     data);

/** 
 * @brief Enum value that allows to configure which item to nullify
 * inside the provided \ref axlStream reference.
 * 
 */
typedef enum { 
	/** 
	 * @brief Nullify the last chunk returned due to call from \ref axl_stream_get_until.
	 */
	LAST_CHUNK, 
	/** 
	 * @brief Nullify the last chunk returned by \ref axl_stream_get_near_to function.
	 */
	LAST_NEAR_TO, 
	/** 
	 * @brief Nullify the last chunk returned by \ref axl_stream_get_following function.
	 */
	LAST_GET_FOLLOWING
} NullifyItem;

void        axl_stream_nullify         (axlStream * stream,
					NullifyItem item);
					

char      * axl_stream_get_untilv      (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int       * result_size,
					int         chunk_num,
					va_list args);

int         axl_stream_get_index       (axlStream * stream);

int         axl_stream_get_global_index (axlStream * stream);

int         axl_stream_get_size        (axlStream * stream);

char      * axl_stream_get_near_to     (axlStream * stream, int count);

char      * axl_stream_get_following   (axlStream * stream, int count);

void        axl_stream_link            (axlStream  *   stream,
					axlPointer     element,
					axlDestroyFunc func);

void        axl_stream_unlink          (axlStream * stream);

void        axl_stream_free            (axlStream * stream);

bool        axl_stream_is_white_space  (char * chunk);

void        axl_stream_consume_white_spaces (axlStream * stream);

bool        axl_stream_remains         (axlStream * stream);

/* @} */

/* string handling interface */

void        axl_stream_trim              (char * chunk);

void        axl_stream_trim_with_size    (char * chunk, int * trimmed);

bool        axl_stream_cmp               (const char * chunk1, const char * chunk2, int size);

bool        axl_stream_casecmp           (const char * chunk1, const char * chunk2, int size);

char      * axl_stream_strdup            (const char * chunk);

char      * axl_stream_strdup_n          (const char * chunk, int n);

int         axl_stream_vprintf_len       (const char * format, 
					  va_list args);

int         axl_stream_printf_len        (const char * format, 
					  ...);

char      * axl_stream_strdup_printf     (const char * chunk, ...);

char      * axl_stream_strdup_printfv    (const char * chunk, va_list args);

char      * axl_stream_strdup_printf_len (const char * chunk, 
					  int  * chunk_size, ...);

char     ** axl_stream_split           (const char * chunk, int separator_num, ...);

char      * axl_stream_concat          (const char * chunk1, const char * chunk2);

int         axl_stream_strv_num        (char ** chunks);

void        axl_stream_freev           (char ** chunks);

char      * axl_stream_to_upper        (char  * chunk);

char      * axl_stream_to_lower        (char  * chunk);

char      * axl_stream_to_upper_copy   (const char  * chunk);

char      * axl_stream_to_lower_copy   (const char  * chunk);

bool        axl_cmp (const char * string, const char * string2);

bool        axl_memcmp (const char * string, const char * string2, int size);

char      * axl_strdup (const char * string);

/** 
 * \addtogroup axl_string_module
 * @{
 */

/** 
 * @brief Alias definition for \ref axl_stream_strdup_printf.
 *
 * See \ref axl_stream_strdup_printf information for more details.
 */
#define axl_strdup_printf axl_stream_strdup_printf

/** 
 * @brief Alias definition for \ref axl_stream_strdup_printfv.
 */
#define axl_strdup_printfv axl_stream_strdup_printfv

/* @} */

END_C_DECLS

#endif
