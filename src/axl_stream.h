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
#ifndef __AXL_STREAM_H__
#define __AXL_STREAM_H__

#include <axl_decl.h>
#include <stdarg.h>

BEGIN_C_DECLS

/** 
 * \addtogroup axl_stream_module
 * @{
 */

axlStream * axl_stream_new             (char * stream_source, 
					int stream_size);

int         axl_stream_inspect         (axlStream * stream, char * chunk);

int         axl_stream_peek            (axlStream * stream, char * chunk);

int         axl_stream_inspect_several (axlStream * stream, int chunk_num, ...);

void        axl_stream_accept          (axlStream * stream);

void        axl_stream_move            (axlStream * stream, int count);

char      * axl_stream_get_until       (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int         chunk_num, ...);

char      * axl_stream_get_untilv      (axlStream * stream, 
					char      * valid_chars, 
					int       * chunk_matched,
					bool        accept_terminator,
					int         chunk_num,
					va_list args);

int         axl_stream_get_index       (axlStream * stream);

int         axl_stream_get_size        (axlStream * stream);

char      * axl_stream_get_near_to     (axlStream * stream, int count);

char      * axl_stream_get_following   (axlStream * stream, int count);

void        axl_stream_link            (axlStream * stream,
					axlDoc    * doc);

void        axl_stream_unlink          (axlStream * stream);

void        axl_stream_free            (axlStream * stream);

bool        axl_stream_is_white_space  (char * chunk);

bool        axl_stream_remains         (axlStream * stream);

/* string handling interface */

bool        axl_stream_cmp             (char * chunk1, char * chunk2, int size);

char      * axl_stream_strdup          (char * chunk);

char      * axl_stream_strdup_n        (char * chunk, int n);

char      * axl_stream_strdup_printf   (char * chunk, ...);

char     ** axl_stream_split           (char * chunk, int separator_num, ...);

char      * axl_stream_concat          (char * chunk1, char * chunk2);

int         axl_stream_strv_num        (char ** chunks);

void        axl_stream_freev           (char ** chunks);

char      * axl_stream_to_upper        (char  * chunk);

char      * axl_stream_to_lower        (char  * chunk);

/* @} */

END_C_DECLS

#endif
