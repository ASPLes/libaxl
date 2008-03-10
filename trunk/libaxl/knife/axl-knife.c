/*
 *  Axl Knife: Console tool on top of Axl Library
 *  Copyright (C) 2007 Advanced Software Production Line, S.L.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
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
/* main include */
#include <axl-knife.h>
#include <errno.h>

/* internal errno redefinition */
#if defined(AXL_OS_WIN32)
#define errno WSAGetLastError()
#endif

#define HELP_HEADER "Axl Knife: Console Tool on top of Axl Library\n\
Copyright (C) 2007  Advanced Software Production Line, S.L.\n\n"

#define POST_HEADER "\n\
If you have question, bugs to report, patches, you can reach us\n\
at <axl@lists.aspl.es>."

/** 
 * @internal Controls if messages must be send to the console log.
 */
bool        console_enabled        = true;
bool        console_debug          = false;
bool        console_debug2         = false;
bool        console_debug3         = false;
bool        console_color_debug    = false;
int         axl_knife_pid          = -1;

/** 
 * @internal Simple macro to check if the console output is activated
 * or not.
 */
#define CONSOLE if (console_enabled) fprintf

/** 
 * @internal Simple macro to check if the console output is activated
 * or not.
 */
#define CONSOLEV if (console_enabled) vfprintf

/** 
 * @brief Allows to get the file part from the provided path.
 * 
 * @param path The path that is provided to return the file part.
 * 
 * @return A reference newly allocated to the file, or an empty string
 * that is also required to deallocate.
 */
char  * axl_knife_file_name           (const char * path)
{
	int    iterator;
	axl_return_val_if_fail (path, NULL);

	/* start with string length */
	iterator = strlen (path) - 1;

	/* lookup for the back-slash */
	while ((iterator >= 0) && (path [iterator] != '/'))
		iterator--;

	/* check if the file provided doesn't have any file part */
	if (iterator == -1) {
		/* return the an empty file part */
		return axl_strdup (path);
	}

	/* copy the base dir found */
	return axl_strdup (path + iterator + 1);
}

/** 
 * @brief Cleans the string provided removing all values found "-" and
 * ".", replacing them with "_".
 * 
 * @param path The path that must be cleaned.
 * 
 */
void axl_knife_clean_name           (char * path)
{
	int    iterator;
	
	/* lookup for the back-slash */
	iterator = 0;
	while (path [iterator]) {
		/* change values found */
		if (path [iterator] == '.')
			path [iterator] = '_';
		if (path [iterator] == '-')
			path [iterator] = '_';

		/* next position */
		iterator++;
	} /* end while */

	return;
}


/** 
 * @internal function that actually handles the console msg.
 */
void axl_knife_msg (const char * file, int line, const char * format, ...)
{
	va_list args;

	/* check extended console log */
	if (console_debug3) {
#if defined(AXL_OS_UNIX)	
		if (console_color_debug) {
			CONSOLE (stderr, "(proc:%d) [\e[1;32mmsg\e[0m] (%s:%d) ", axl_knife_pid, file, line);
		} else
#endif
			CONSOLE (stderr, "(proc:%d) [msg] (%s:%d) ", axl_knife_pid, file, line);
	} else {
#if defined(AXL_OS_UNIX)	
		if (console_color_debug) {
			CONSOLE (stderr, "\e[1;32mI: \e[0m");
		} else
#endif
			CONSOLE (stderr, "I: ");
	} /* end if */
	
	va_start (args, format);
	
	/* report to console */
	CONSOLEV (stderr, format, args);

	va_end (args);

	CONSOLE (stderr, "\n");
	
	fflush (stderr);
	
	return;
}

/** 
 * @internal function that actually handles the console access
 */
void  axl_knife_access   (const char * file, int line, const char * format, ...)
{
	va_list args;

	/* check extended console log */
	if (console_debug3) {
#if defined(AXL_OS_UNIX)	
		if (console_color_debug) {
			CONSOLE (stderr, "(proc:%d) [\e[1;32mmsg\e[0m] (%s:%d) ", axl_knife_pid, file, line);
		} else
#endif
			CONSOLE (stderr, "(proc:%d) [msg] (%s:%d) ", axl_knife_pid, file, line);
	} else {
#if defined(AXL_OS_UNIX)	
		if (console_color_debug) {
			CONSOLE (stderr, "\e[1;32mI: \e[0m");
		} else
#endif
			CONSOLE (stderr, "I: ");
	} /* end if */
	
	va_start (args, format);
	
	/* report to console */
	CONSOLEV (stderr, format, args);

	va_end (args);

	CONSOLE (stderr, "\n");
	
	fflush (stderr);
	
	return;
}

/** 
 * @internal function that actually handles the console msg (second level debug)
 */
void axl_knife_msg2 (const char * file, int line, const char * format, ...)
{
	va_list args;

	/* check second level debug */
	if (! console_debug2)
		return;

	/* check extended console log */
	if (console_debug3) {
#if defined(AXL_OS_UNIX)	
		if (exarg_is_defined ("color-debug")) {
			CONSOLE (stderr, "(proc:%d) [\e[1;32mmsg\e[0m] (%s:%d) ", axl_knife_pid, file, line);
		} else
#endif
			CONSOLE (stderr, "(proc:%d) [msg] (%s:%d) ", axl_knife_pid, file, line);
	} else {
#if defined(AXL_OS_UNIX)	
		if (console_color_debug) {
			CONSOLE (stderr, "\e[1;32mI: \e[0m");
		} else
#endif
			CONSOLE (stderr, "I: ");
	} /* end if */
	
	va_start (args, format);
	
	/* report to console */
	CONSOLEV (stderr, format, args);

	va_end (args);

	CONSOLE (stderr, "\n");
	
	fflush (stderr);
	
	return;
}

/** 
 * @internal function that actually handles the console wrn.
 */
void axl_knife_wrn (const char * file, int line, const char * format, ...)
{
	va_list args;
	
	/* check extended console log */
	if (console_debug3) {
#if defined(AXL_OS_UNIX)	
		if (exarg_is_defined ("color-debug")) {
			CONSOLE (stderr, "(proc:%d) [\e[1;33m!!!\e[0m] (%s:%d) ", axl_knife_pid, file, line);
		} else
#endif
			CONSOLE (stderr, "(proc:%d) [!!!] (%s:%d) ", axl_knife_pid, file, line);
	} else {
#if defined(AXL_OS_UNIX)	
		if (console_color_debug) {
			CONSOLE (stderr, "\e[1;33m!: \e[0m");
		} else
#endif
			CONSOLE (stderr, "!: ");
	} /* end if */
	
	va_start (args, format);

	CONSOLEV (stderr, format, args);

	va_end (args);

	CONSOLE (stderr, "\n");
	
	fflush (stderr);
	
	return;
}


/** 
 * @internal function that actually handles the console error.
 */
void axl_knife_error (const char * file, int line, const char * format, ...)
{
	va_list args;
	
	
	/* check extended console log */
	if (console_debug3) {
#if defined(AXL_OS_UNIX)	
		if (exarg_is_defined ("color-debug")) {
			CONSOLE (stderr, "(proc:%d) [\e[1;31merr\e[0m] (%s:%d) ", axl_knife_pid, file, line);
		} else
#endif
			CONSOLE (stderr, "(proc:%d) [err] (%s:%d) ", axl_knife_pid, file, line);
	} else {
#if defined(AXL_OS_UNIX)	
		if (console_color_debug) {
			CONSOLE (stderr, "\e[1;31mE: \e[0m");
		} else
#endif
			CONSOLE (stderr, "E: ");
	} /* end if */
	
	va_start (args, format);

	/* report to the console */
	CONSOLEV (stderr, format, args);

	va_end (args);

	CONSOLE (stderr, "\n");
	
	fflush (stderr);
	
	return;
}

void axl_knife_introduce_indentation (int level)
{
	int iterator;

	iterator = 0;
	while (iterator < level) {
		printf ("   ");
		iterator++;
	} /* end while */

	return;
}

bool axl_knife_htmlize_iterator_node (FILE * fstream, axlNode * node, int level)
{
	
	axlItem       * item;
	int             size;
	char          * content;
	axlAttrCursor * cursor;
	int             iterator;
	
	/* introduce indentation level */
	axl_knife_introduce_indentation (level);

	/* print document node */
	if (axl_node_has_attributes (node)) {
		fprintf (fstream, "&lt;<span class=\"node\">%s</span> ",
			 axl_node_get_name (node));

		/* get the first cursor */
		cursor   = axl_node_attr_cursor_new (node);

		if (axl_node_num_attributes (node)  < 3) {
			while (axl_node_attr_cursor_has_item (cursor)) {
				
				/* print values */
				fprintf (fstream, "%s=<span class=\"attrvalue\">\"%s\"</span> ",
					 axl_node_attr_cursor_get_key (cursor),
					 axl_node_attr_cursor_get_value (cursor));
				
				/* get the next cursor */
				axl_node_attr_cursor_next (cursor);
			} /* end while */
		} else {
			while (axl_node_attr_cursor_has_item (cursor)) {

				/* print values */
				fprintf (fstream, "%s=<span class=\"attrvalue\">\"%s\"</span> ",
					 axl_node_attr_cursor_get_key (cursor),
					 axl_node_attr_cursor_get_value (cursor));

				/* get the next cursor */
				axl_node_attr_cursor_next (cursor);

				/* before getting the next */
				if (axl_node_attr_cursor_has_item (cursor)) {
					fprintf (fstream, "\n");

					/* introduce indentation level */
					axl_knife_introduce_indentation (level );

					iterator = 0;
					while (iterator < (strlen (axl_node_get_name (node)) + 2)) {
						fprintf (fstream, " ");
						iterator++;
					} /* while */
				} /* end if */

			} /* end while */
		}
			
		fprintf (fstream, "/>\n");
		
		/* free cursor */
		axl_node_attr_cursor_free (cursor);
		
	} else {
		fprintf (fstream, "&lt;<span class=\"node\">%s</span>>\n",
			 axl_node_get_name (node));
	}

	/* call to produce internal content representation */
	item = axl_item_get_first_child (node);
	while (item != NULL) {
		/* according to the type do */
		switch (axl_item_get_type (item)) {
		case ITEM_NODE:
			/* found node, call to represent this node */
			axl_knife_htmlize_iterator_node (fstream, axl_item_get_data (item), level + 1);
			break;
		case ITEM_CONTENT_FROM_FACTORY:
		case ITEM_CONTENT:
			size    = 0;
			content = axl_item_get_content (item, &size);
			if (size == 0 || content [size - 1] == '\n')
				fprintf (fstream, "%s", axl_item_get_content (item, NULL));
			else
				fprintf (fstream, "%s\n", axl_item_get_content (item, NULL));
			break;
		case ITEM_PI:
			break;
		case ITEM_FROM_FACTORY:
			/* never reached */
			break;
		case ITEM_COMMENT:
			/* introduce indentation level */
			axl_knife_introduce_indentation (level + 1);
			fprintf (fstream, "<span class=\"comment\">&lt;!-- %s --></span>\n",
				 axl_item_get_content (item, NULL));
			break;
		case ITEM_REF:
			break;
		case ITEM_CDATA:
			break;
		} /* end switch */

		/* next item */
		item = axl_item_get_next (item);
	}


	if (axl_node_have_childs (node)) {
		/* introduce indentation level */
		axl_knife_introduce_indentation (level);
		
		fprintf (fstream, "&lt;/<span class=\"node\">%s</span>>\n",
			 axl_node_get_name (node));
	} /* end if */

	/* don't stop iteration */
	return true;
}

bool axl_knife_htmlize (axlDoc * doc)
{
	FILE       * fstream = stdout;

	/* currently we only support stderr so reached this place
	 * means it is already checked */

	/* check output argument */
	if (exarg_is_defined ("output")) {
		/* abrimos el fichero de salida */
		fstream = fopen (exarg_get_string ("output"), "w");
		if (fstream == NULL) {
			error ("unable to open output document: %s, errno=%d:%s", exarg_get_string ("output"),
			       errno, strerror (errno));
			return false;
		} /* end if */
	} /* end if */

	/* call to iterate */
	fprintf (fstream, "<pre>\n");
	axl_knife_htmlize_iterator_node (fstream, axl_doc_get_root (doc), 1);
	fprintf (fstream, "</pre>\n");

	return true;
}

bool axl_knife_dtd_to_c ()
{
	
	/* check the document received is a DTD */
	axlError   * err = NULL;
	axlDtd     * dtd = axl_dtd_parse_from_file (exarg_get_string ("input"), &err);
	axlStream  * stream;
	char       * file;
	int          line_length;
	int          max;
	int          chunk_matched;
	char       * ref;
	char       * format;
	int          iterator;
	FILE       * fstream = stdout;

	if (dtd == NULL) {
		error ("unable to translate document into C, found invalid DTD: %s",
		       axl_error_get (err));
		axl_error_free (err);
		return false;
	} /* end if */
	
	/* free dtd document */
	axl_dtd_free (dtd);

	/* open the stream */
	stream = axl_stream_new (NULL, -1, exarg_get_string ("input"), -1, &err);

	/* check output argument */
	if (exarg_is_defined ("output")) {
		/* abrimos el fichero de salida */
		fstream = fopen (exarg_get_string ("output"), "w");
		if (fstream == NULL) {
			error ("unable to open output document: %s, errno=%d:%s", exarg_get_string ("output"),
			       errno, strerror (errno));
			return false;
		} /* end if */
	} /* end if */
	
	/* try to get the maximum lenght */
	line_length = 0;
	do {
		/* get next */
		ref = axl_stream_get_until_zero (stream, NULL, &chunk_matched, true, 1, "\n", NULL);

		/* get lenght and update */
		max = ref ? strlen (ref) : 0;
		if (max > line_length)
			line_length = max;

	} while (chunk_matched != -2);

	/* close the stream */
	axl_stream_free (stream);
	stream = axl_stream_new (NULL, -1, exarg_get_string ("input"), -1, &err);
	
	/* build format for each line */
	format = axl_strdup_printf ("%%-%ds  \\\n", line_length + 1);

	file   = axl_knife_file_name (exarg_get_string ("input"));

	
	
	fprintf (fstream, "/**\n");
	fprintf (fstream, " * C inline representation for DTD %s, created by axl-knife\n", file);
	fprintf (fstream, " */\n");

	axl_knife_clean_name (file);
	axl_stream_to_upper (file);
	
	fprintf (fstream, "#ifndef __%s_H__\n", file);
	fprintf (fstream, "#define __%s_H__\n", file);
	fprintf (fstream, "#define %s \"\\n\\\n", file);
	do {
		/* get next */
		ref      = axl_stream_get_until_zero (stream, NULL, &chunk_matched, true, 1, "\n", NULL);
		if (ref != NULL) {
			iterator = 0;
			while (iterator < strlen (ref)) {
				/* update blank */
				if (ref[iterator] == '\t')
					ref[iterator] = ' ';
				if (ref[iterator] == '\"')
					ref[iterator] = '\'';

				/* next iterator */
				iterator++;
			} /* end while */
		} /* end if */

		/* get lenght and update */
		fprintf (fstream, format, ref);

	} while (chunk_matched != -2);



	fprintf (fstream, "\\n\"\n");

	fprintf (fstream, "#endif\n");
	
	axl_stream_free (stream);
	axl_free (file);
	axl_free (format);

	/* check output argument */
	if (exarg_is_defined ("output"))
		fclose (fstream);

	return true;
}

/** 
 * @brief Allows to check if the provided file (basefile) is newer
 * than the file (compare).
 * 
 * @param basefile The base file to check.
 * @param compare The compare file to check
 * 
 * @return true if the modification time is newer than compare,
 * otherwise false is returned.
 */
bool axl_knife_check_if_newer (const char * basefile, const char * compare)
{
	struct stat stat1, stat2;
	
	/* get stats from both files */
	if (stat (basefile, &stat1) != 0)
		return false;
	if (stat (compare, &stat2) != 0)
		return false;

	/* return value comparation */
	return stat1.st_mtime > stat2.st_mtime;
}


int main (int argc, char ** argv)
{

	axlDoc   * doc = NULL;
	axlError * err = NULL;

	/* init the axl library */
	if (! axl_init ()) {
		printf ("Failed to initialize axl library. Terminating axl-knife tool..\n");
		return -1;
	} /* end if */

	/* install headers for help */
	exarg_add_usage_header  (HELP_HEADER);
	exarg_add_help_header   (HELP_HEADER);
	exarg_post_help_header  (POST_HEADER);
	exarg_post_usage_header (POST_HEADER);

	/* init exarg library */
	exarg_install_arg ("version", "v", EXARG_NONE, 
			   "Shows current axl-knife version.");

	/* file options */
	exarg_install_arg ("input", "i", EXARG_STRING, 
			   "Allows to configure the input document to process.");
	exarg_install_arg ("output", "o", EXARG_STRING,
			   "Allows to configure the output file to be used. This is optional. If the knife command produces an output, it is by default sent to the stdout.");
	exarg_install_arg ("ifnewer", "n", EXARG_NONE,
			   "In process involving generating output from an input file, this option allows to stop the process and return success code (0) if the output file is found to be newer than input file. ");
			   

	exarg_install_arg ("htmlize", "e", EXARG_NONE,
			   "Takes an input xml document and produces an transformation preparing the document to be included into an html web page");
	
	/* log options */
	exarg_install_arg ("enable-log", "l", EXARG_NONE,
			   "Allows to activate the console log debug.");

	exarg_install_arg ("enable-log-color", "c", EXARG_NONE,
			   "Activates the console logs and uses some ansi characters to colorify the log output. If this option is activated, it is implicitly activated the --enable-log");
	
	/* dtd-to-c options */
	exarg_install_arg ("dtd-to-c", NULL, EXARG_NONE,
			   "Creates a C header definition representing the DTD provided, suitable to be opened by libaxl");

	/* add dependecies */
	exarg_add_dependency ("htmlize", "input");
	exarg_add_dependency ("dtd-to-c", "input");
	
	exarg_add_dependency ("ifnewer", "input");
	exarg_add_dependency ("ifnewer", "output");
	
	/* exclusion */
	exarg_add_exclusion ("htmlize", "dtd-to-c");
	
	/* call to parse arguments */
	exarg_parse (argc, argv);

	/* get current process id */
	axl_knife_pid = getpid ();

	/* normal operations */
	if (exarg_is_defined ("version")) {
		return printf ("%s\n", VERSION);
	}

	/* check ifnewer option */
	if (exarg_is_defined ("ifnewer")) {
		/* check that both files exists */
		if (axl_knife_file_test (exarg_get_string ("input"), FILE_EXISTS) &&
		    axl_knife_file_test (exarg_get_string ("output"), FILE_EXISTS)) {
			if (! axl_knife_check_if_newer (exarg_get_string ("input"), exarg_get_string ("output"))) {
				goto finish;
			} /* end if */
		} /* end if */
	} /* end if */

	/* check parameters defined */
	if (! exarg_is_defined ("htmlize") &&
	    ! exarg_is_defined ("dtd-to-c")) {
		msg ("no action was defined..");
		goto finish;
	}
		

	/* parse log options */
	axl_log_enable (exarg_is_defined ("enable-log"));

	/* check color to activate both logs */
	if (exarg_is_defined ("enable-log-color")) {
		axl_log_enable (true);
		axl_log_color_enable (true);
	}

	/* check to load a document */
	if (exarg_is_defined ("dtd-to-c")) {
		/* do not open any document, as it is opened by an axl
		 * stream directly */
	} else if (exarg_is_defined ("input")) {
		/* parse document from file */
		doc = axl_doc_parse_from_file (exarg_get_string ("input"), &err);
		if (doc == NULL) {
			error ("Failed to open document (%s), error found: %s",
			       exarg_get_string ("input"), axl_error_get (err));
			axl_error_free (err);
			goto finish;
		} /* end if */
		msg ("document loaded properly: %s", exarg_get_string ("input"));
	} /* end if */

	
	/* check process options */
	if (exarg_is_defined ("htmlize")) {
		/* call to htmlize the content received */
		axl_knife_htmlize (doc);
	} else if (exarg_is_defined ("dtd-to-c")) {

		/* call to produce a C representation from the DTD
		 * provided */
		axl_knife_dtd_to_c ();
	} /* end if */


 finish:
	/* dealloc document opened */
	axl_doc_free (doc);

	/* terminating axl */
	axl_end ();

	/* terminate exarg */
	exarg_end ();

	return 0;
}

/** 
 * @brief Allows to perform a set of test for the provided path.
 * 
 * @param path The path that will be checked.
 *
 * @param test The set of test to be performed. Separate each test
 * with "|" to perform several test at the same time.
 * 
 * @return true if all test returns true. Otherwise false is returned.
 */
bool   axl_knife_file_test (const char * path, FileTest test)
{
	bool result = false;
	struct stat file_info;

	/* perform common checks */
	axl_return_val_if_fail (path, false);

	/* call to get status */
	result = (stat (path, &file_info) == 0);
	if (! result) {
		/* check that it is requesting for not file exists */
		if (errno == ENOENT && (test & FILE_EXISTS) == FILE_EXISTS)
			return false;

		error ("filed to check test on %s, stat call has failed (result=%d, error=%s)", path, result, strerror (errno));
		return false;
	} /* end if */

	/* check for file exists */
	if ((test & FILE_EXISTS) == FILE_EXISTS) {
		/* check result */
		if (result == false)
			return false;
		
		/* reached this point the file exists */
		result = true;
	}

	/* check if the file is a link */
	if ((test & FILE_IS_LINK) == FILE_IS_LINK) {
		if (! S_ISLNK (file_info.st_mode))
			return false;

		/* reached this point the file is link */
		result = true;
	}

	/* check if the file is a regular */
	if ((test & FILE_IS_REGULAR) == FILE_IS_REGULAR) {
		if (! S_ISREG (file_info.st_mode))
			return false;

		/* reached this point the file is link */
		result = true;
	}

	/* check if the file is a directory */
	if ((test & FILE_IS_DIR) == FILE_IS_DIR) {
		if (! S_ISDIR (file_info.st_mode)) {
			return false;
		}

		/* reached this point the file is link */
		result = true;
	}

	/* return current result */
	return result;
}

/** 
 * \page axl_knife_manual axl-knife: command line tool built on top of Axl.
 *
 * \section intro Introduction
 *
 * <b>axl-knife</b> is a command line tool that includes xml related
 * features and the intention is to concentrate into the tool,
 * features to make Axl Library available to the command line.
 *
 * This manual includes information about operations that support the
 * tool and how to activate them:
 *
 *  \ref axl_knife_dtd_to_c
 *  \ref axl_knife_htmlize
 * 
 * \section axl_knife_dtd_to_c Building a C inline representation from a DTD file
 *
 * This feature allows to create a C header file representing a
 * particular DTD file, so it can be used and parsed as it were a file
 * by included into the source.
 * 
 * The main advantage from this approach is that the application or
 * library using Axl Library services do not have to bundle DTD files
 * into its installers. Those files are already included into the
 * source. In many cases, those DTD files are than less than 1K.
 * 
 * To create a C DTD representation use:
 * \code
 * >> axl-knife --input DTD-FILE.dtd --dtd-to-c --output DTD-FILE.dtd.h --ifnewer
 * \endcode
 *
 * The argument <b>--ifnewer</b> allows to signal the tool to not
 * produce any output if the input file is not found to be newer. 
 *
 * Now use \ref axl_dtd_parse to open a reference to the DTD.
 *
 * \section axl_knife_htmlize Prepare xml content to be included into xml
 *
 * In the case you require to include xml content into a web page, you
 * can use <b>--htmlize</b> option to prepare the content, adding CSS
 * style classes to configure how your xml document will look.
 *
 * Use the following to produce xml content ready to be included into html:
 *
 * \code
 * >> axl-knife --input xml-file.xml --htmlize
 * \endcode
 *
 */
