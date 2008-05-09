/*
 *  LibAxl:  Another XML library
 *  Copyright (C) 2008 Advanced Software Production Line, S.L.
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
#ifndef __AXL_BABEL_H__
#define __AXL_BABEL_H__

/** 
 * \addtogroup axl_babel
 * @{
 */

/* include base library */
#include <axl.h>

/**
 * @brief Public definition for a translation unit from a native
 * codification into universal unicode representation using UTF-8
 * format. This type is used by modules providing input encoding
 * support to axl babel. If you are not developing an encoding module,
 * this type isn't useful for you.
 */
typedef struct _axlBabelTable {
	/**
	 * @brief Amount of unicode utf-8 atoms stored in buffer.
	 */
	short size;

	/**
	 * @brief Support for 6 octects to represent one unicode unit.
	 */
	unsigned char  buffer[6];
} axlBabelTable;

/* include headers */
#include <axl_babel_iso88591.h>
#include <axl_babel_iso88592.h>
#include <axl_babel_iso88599.h>
#include <axl_babel_iso885915.h>

bool        axl_babel_init (axlError ** error);

void        axl_babel_finish ();

bool        axl_babel_detect_codification (axlStream  * stream, 
					   char      ** encoding,
					   axlPointer   user_data,
					   axlError  ** error);

bool        axl_babel_configure_encoding  (axlStream  * stream, 
					   const char * encoding, 
					   const char * detected, 
					   axlPointer user_data, 
					   axlError ** error);

bool        axl_babel_check_utf8_content  (const char * content,
					   int          content_length,
					   int        * index_error);


#endif /* end __AXL_BABEL_H__ */

/**
 * @}
 */
