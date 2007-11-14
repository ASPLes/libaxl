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
 *         info@aspl.es - http://www.aspl.es/xml
 */
#include <axl.h>

struct _axlError {
	int    code;
	char * error;
	int    defined;
};

/**
 * \defgroup axl_error_module Axl Error report: Functions to help AXL library to report internal error to the application level.
 */

/** 
 * \addtogroup axl_error_module
 * @{
 */

/** 
 * @brief Allows to create a new \ref axlError value that contains an
 * error code and a error string.
 *
 * \ref axlError error reporting abstraction is a convenient way for
 * the developer and the user that makes use of the API to report and
 * get textual diagnostic errors produced. Many times, API provided do
 * not allow to get more information if something wrong happen making
 * it difficult to reach and solve the problem (including the
 * development phase).
 *
 * From a developer's perspective, here is how works \ref axlError,
 * 
 * \code
 * void some_public_exported_function (int param, axlError ** error)
 * {
 *      // do some work, but if it fails call to produce the error
 *      // reporting.
 *      axl_error_new (-2,     // reporting an error code
 *                     "Something wasn't ok while processing..", 
 *                     NULL,   // a reference to the stream (optional)
 *                     error); // variable received.
 *      return;
 * }
 * \endcode
 *
 * Previous construction makes error reporting optional but at the
 * same time, available, because the programmer doesn't requires to
 * check if the user did define the error variable, making it
 * available at user option.
 *
 * Now, if the user defines the \ref axlError reference, by calling to
 * the function, it can get the error reported as follows:
 * 
 * \code
 * // declare the variable and it to null 
 * axlError * error = NULL;
 *
 * // call to function
 * some_pulic_exported_function (10, &error);
 * if (! axl_error_was_ok (error)) {
 *     // drop the message 
 *     printf ("Something has failed: (code: %d) %s\n",
 *             axl_error_get_code (error),
 *             axl_error_get (error));
 *     // dealloc the reference 
 *     axl_error_free (error);
 * }
 * \endcode
 *
 * Alternatively, the user can just bypass the error reporting
 * mechanism, without affecting the written code inside the source of
 * the function supporting the \ref axlError notification (even if the
 * code calls to \ref axl_error_new): 
 * 
 * \code
 * // call to the function without error reporting 
 * some_pulic_exported_function (10, NULL);
 * \endcode
 * 
 * In most cases, \ref axl_error_new is not used by API consumers but
 * by API developers. Once returned the \ref axlError reference the
 * following functions could be checked.
 *
 * - \ref axl_error_get returns textual diagnostic reported.
 *
 * - \ref axl_error_get_code returns error code reported.
 *
 * - \ref axl_error_was_ok allows to check if some error was
 * reported, base on the value initialization.
 *
 * 
 * 
 * @param code The error code to set and the error code string.
 *
 * @param error_code String to report.
 *
 * @param stream If provided, the error will try to get current stream
 * position to add more information to the place where the error was
 * found.
 *
 * @param _error The error string to be used to initialize the received \ref axlError.
 */
void axl_error_new (int code, char * error_code, axlStream * stream, axlError ** _error)
{
	axlError * error;
	char     * following;

	/* get a reference to the error to be created */
	if (_error == NULL)
		return;

	/* create the error to be reported */
	error             = axl_new (axlError, 1); 
	error->code       = code;
	error->defined    = -346715;
	if (stream == NULL) {
		/* allocate enough memory */
		error->error = axl_strdup (error_code);
	} else {
		/* get the following */
		following    = axl_stream_get_following (stream, 10);
		
		/* alloc enough memory */
		error->error = axl_stream_strdup_printf ("Error found (stream size: %d, at byte %d (global index: %d), near to ...%s..., while reading: %s): %s\n", 
							 axl_stream_get_size (stream),
							 axl_stream_get_index (stream),
							 axl_stream_get_global_index (stream),
							 axl_stream_get_near_to (stream, 10),
							 (following != NULL) ? following : "",
							 error_code);
	}
	
	axl_log (NULL, AXL_LEVEL_CRITICAL, "(code: %d) %s", code, error_code);
	
	/* set the error into the recevied reference */
	(* _error )       = error;
	return;
}

/** 
 * @brief Allows to check if the provided reference was used to report
 * an error.
 *
 * Those APIs that return an \ref axlError reference filled with the
 * textual error diagnostic, can take advantage of this function. It
 * Allows to check if the error was used to report an error, instead
 * of checking a returning value containing a particular error code.
 *
 * See \ref axl_error_new for more information.
 * 
 * @param _error The error that is being checked.
 * 
 * @return \ref true if the error reference doesn't contains an
 * "ERROR" (an error wasn't reported), otherwise, \ref false is
 * returned.
 */
bool   axl_error_was_ok   (axlError * _error)
{
	/* check if it was ok */
	if (_error == NULL || _error->error == NULL || (_error->defined != -346715))
		return true;
	
	/* axl error is defined */
	return false;
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


/* @} */
