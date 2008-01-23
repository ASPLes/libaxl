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
 *         Edificio Alius A, Oficina 102,
 *         C/ Antonio Suarez Nº 10,
 *         Alcalá de Henares 28802 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/xml
 */
#include <axl_log.h>

/**
 * \defgroup axl_log_module Axl Log: Console log reporting for AXL library
 */

/** 
 * \addtogroup axl_log_module
 * @{
 */

/* console log */
bool     not_executed  = true;
bool     debug_enabled = false;

/* colored log */
bool     not_executed_color  = true;
bool     debug_color_enabled = false;

/** 
 * @brief Allows to check if the log reporting inside the system is
 * enabled.
 *
 * @return true if the log is enabled or false
 */
bool      axl_log_is_enabled () 
{
#if defined(AXL_OS_WIN32) && ! defined(__GNUC__)
	int requiredSize;
#endif

	if (not_executed) {
#if defined(AXL_OS_WIN32) && ! defined(__GNUC__)
		getenv_s( &requiredSize, NULL, 0, "AXL_DEBUG");
		debug_enabled = (requiredSize > 0);
#else
		debug_enabled = (getenv ("AXL_DEBUG") != NULL);
#endif
		not_executed  = false;
	}

	/* return current value */
	return debug_enabled;
}

/** 
 *
 * @brief Allows to get current log configuration, to use colors.
 * 
 * @return true if the color log is enabled or false
 */
bool    axl_log_color_is_enabled ()
{
#if defined(AXL_OS_WIN32) && ! defined(__GNUC__)
	int requiredSize;
#endif

	if (not_executed_color) {
#if defined(AXL_OS_WIN32) && ! defined(__GNUC__)
		getenv_s( &requiredSize, NULL, 0, "AXL_DEBUG_COLOR");
		debug_color_enabled = (requiredSize > 0);
#else
		debug_color_enabled = (getenv ("AXL_DEBUG_COLOR") != NULL);
#endif
		not_executed_color  = false;
	}

	/* return current value */
	return debug_color_enabled;
}

/** 
 * @brief Allows to control how to activate the log reporting to the
 * console from the axl core library.
 * 
 * @param value true to enable log to console, otherwise false is
 * returned.
 */
void     axl_log_enable (bool value)
{
	/* activate debuging according to the variable */
	not_executed  = false;
	debug_enabled = value;
	return;
}

/** 
 * @brief Allows to control how to activate the colog log reporting to
 * the console from the axl core library.
 * 
 * @param value true to enable log to console, otherwise false is
 * returned.
 */
void     axl_log_color_enable (bool value)
{
	/* activate color debuging according to the variable */
	not_executed_color  = false;
	debug_color_enabled = value;
	return;
}

/** 
 * @brief Allows to drop a log to the console.
 *
 * This function allow to drop a log to the console using the given
 * domain, as an identification of which subsystem have reported the
 * information, and report level. This report level is used to notify
 * the consideration of the log reported.
 * 
 * The function allows to provide a printf like interface to report
 * messages. Here are some examples:
 * 
 * \code
 * // drop a log about current library initialization
 * axl_log ("axl", AXL_LEVEL_DEBUG, "library properly initialized status=%d", status);
 * \endcode
 *
 *
 * @param domain The domain where the log as generated. if NULL is
 * provided a log with no domain will be generated.
 *
 * @param level The level that this message is classificed. 
 * 
 * @param message The message to report. The message to report must be
 * not NULL.
 */
void axl_log (char * domain, AxlDebugLevel level, char * message, ...)
{

#ifdef SHOW_DEBUG_LOG
	va_list    args;

	/* check if the log is enabled */
	if (! axl_log_is_enabled ())
		return;

	/* printout the process pid */
	if (axl_log_color_is_enabled ()) 
		printf ("\e[1;36m(proc %d)\e[0m: ", getpid ());
	else
		printf ("(proc %d): ", getpid ());

	/* drop a log according to the level */
	if (axl_log_color_is_enabled ()) {
		switch (level) {
		case AXL_LEVEL_DEBUG:
			printf ("(\e[1;32mdebug\e[0m) ");
			break;
		case AXL_LEVEL_WARNING:
			printf ("(\e[1;33mwarning\e[0m) ");
			break;
		case AXL_LEVEL_CRITICAL:
			printf ("(\e[1;31mcritical\e[0m) ");
			break;
		}
	}else {
		switch (level) {
		case AXL_LEVEL_DEBUG:
			printf ("(debug)");
			break;
		case AXL_LEVEL_WARNING:
			printf ("(warning)");
			break;
		case AXL_LEVEL_CRITICAL:
			printf ("(critical) ");
			break;
		}
	}

	/* drop a log according to the domain */
	(domain != NULL) ? printf ("%s: ", domain) : printf (": ");

	/* print the message */
	va_start (args, message);
	vprintf (message, args);
	va_end (args);

	printf ("\n");

	/* ensure that the log is droped to the console */
	fflush (stdout);
#endif

	/* return */
	return;
}

/* @} */
