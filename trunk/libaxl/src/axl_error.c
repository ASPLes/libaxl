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

struct _axlError {
	int    code;
	char * error;
};

/** 
 * @internal
 *
 * @brief Allows to create a new \ref axlError value that contains an
 * error code and a error string.
 * 
 * @param code The error code to set and the error code string.
 *
 * @param error The error string to be used to initialize the received
 * \ref axlError.
 * 
 * @param _error The \ref axlError to initialize.
 */
void axl_error_new (int code, char * error_code, axlError ** _error)
{
	axlError * error;

	/* get a reference to the error to be created */
	if (_error == NULL)
		return;

	/* create the error to be reported */
	error             = axl_new (axlError, 1); 
	error->code       = code;
	error->error      = axl_strdup (error_code);
	
	axl_log (NULL, AXL_LEVEL_CRITICAL, "(code: %d) %s", code, error_code);
	
	/* set the error into the recevied reference */
	(* _error )       = error;
	return;
}

/** 
 * @brief Allows to get current error code from the given \ref axlError value.
 *
 * If the provided \ref axlError doesn't not contain a valid error
 * information, -1 is returned. Otherwise the specific error code is
 * returned.
 * 
 * @param _error The \ref axlError to use, while getting error code
 * inside.
 * 
 * @return The error code inside or -1 if fails.
 */
int axl_error_get_code (axlError * _error)
{
	/* check received reference */
	if (_error == NULL)
		return -1;
	return _error->code;
}

/** 
 * @brief Allows to get current textual error string inside the given
 * \ref axlError value.
 * 
 * @param _error The \ref axlError where the error string value will
 * be retrieved.
 * 
 * @return The error code or the string "no string error defined" if
 * the given error doesn't contain any string information. You must
 * not deallocate memory returned by this function because it is an
 * internal copy.
 */
char * axl_error_get      (axlError * _error)
{
	/* check received reference */
	if (_error == NULL)
		return "no string error defined";

	return _error->error;
}

/** 
 * @brief Allows to release memory allocated by the given \ref
 * axlError variable.
 * 
 * @param _error The axlError to deallocate.
 */
void   axl_error_free     (axlError * _error)
{

	/* check for null reference received */
	if (_error == NULL)
		return;
	
	/* release man with no mercy */
	axl_free (_error->error);
	axl_free (_error);
	
	return;
}


