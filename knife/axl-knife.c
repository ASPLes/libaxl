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

bool axl_knife_htmlize_iterator_node (axlNode * node, int level)
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
		printf ("&lt;<span class=\"node\">%s</span> ",
			axl_node_get_name (node));

		/* get the first cursor */
		cursor   = axl_node_attr_cursor_new (node);

		if (axl_node_num_attributes (node)  < 3) {
			while (axl_node_attr_cursor_has_item (cursor)) {
				
				/* print values */
				printf ("%s=<span class=\"attrvalue\">\"%s\"</span> ",
					axl_node_attr_cursor_get_key (cursor),
					axl_node_attr_cursor_get_value (cursor));
				
				/* get the next cursor */
				axl_node_attr_cursor_next (cursor);
			} /* end while */
		} else {
			while (axl_node_attr_cursor_has_item (cursor)) {

				/* print values */
				printf ("%s=<span class=\"attrvalue\">\"%s\"</span> ",
					axl_node_attr_cursor_get_key (cursor),
					axl_node_attr_cursor_get_value (cursor));

				/* get the next cursor */
				axl_node_attr_cursor_next (cursor);

				/* before getting the next */
				if (axl_node_attr_cursor_has_item (cursor)) {
					printf ("\n");

					/* introduce indentation level */
					axl_knife_introduce_indentation (level );

					iterator = 0;
					while (iterator < (strlen (axl_node_get_name (node)) + 2)) {
						printf (" ");
						iterator++;
					} /* while */
				} /* end if */

			} /* end while */
		}
			
		printf ("/>\n");
		
		/* free cursor */
		axl_node_attr_cursor_free (cursor);
		
	} else {
		printf ("&lt;<span class=\"node\">%s</span>>\n",
			axl_node_get_name (node));
	}

	/* call to produce internal content representation */
	item = axl_item_get_first_child (node);
	while (item != NULL) {
		/* according to the type do */
		switch (axl_item_get_type (item)) {
		case ITEM_NODE:
			/* found node, call to represent this node */
			axl_knife_htmlize_iterator_node (axl_item_get_data (item), level + 1);
			break;
		case ITEM_CONTENT_FROM_FACTORY:
		case ITEM_CONTENT:
			size    = 0;
			content = axl_item_get_content (item, &size);
			if (size == 0 || content [size - 1] == '\n')
				printf ("%s", axl_item_get_content (item, NULL));
			else
				printf ("%s\n", axl_item_get_content (item, NULL));
			break;
		case ITEM_PI:
			break;
		case ITEM_FROM_FACTORY:
			/* never reached */
			break;
		case ITEM_COMMENT:
			/* introduce indentation level */
			axl_knife_introduce_indentation (level + 1);
			printf ("<span class=\"comment\">&lt;!-- %s --></span>\n",
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
		
		printf ("&lt;/<span class=\"node\">%s</span>>\n",
			axl_node_get_name (node));
	} /* end if */

	/* don't stop iteration */
	return true;
}

bool axl_knife_htmlize (axlDoc * doc)
{
	/* currently we only support stderr so reached this place
	 * means it is already checked */

	/* call to iterate */
	printf ("<pre>\n");
	axl_knife_htmlize_iterator_node (axl_doc_get_root (doc), 1);
	printf ("</pre>\n");

	return true;
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

	/* init exarg library */
	exarg_install_arg ("input", "i", EXARG_STRING, 
			   "Allows to configure the input document to process.");

	exarg_install_arg ("stdout", "o", EXARG_NONE,
			   "If the tool must produce an output, its send to the standard console output");

	exarg_install_arg ("htmlize", "e", EXARG_NONE,
			   "Takes an input xml document and produces an transformation preparing the document to be included into an html web page");
	
	/* log options */
	exarg_install_arg ("enable-log", "l", EXARG_NONE,
			   "Allows to activate the console log debug.");

	exarg_install_arg ("enable-log-color", "c", EXARG_NONE,
			   "Activates the console logs and uses some ansi characters to colorify the log output. If this option is activated, it is implicitly activated the --enable-log");

	/* add dependecies */
	exarg_add_dependency ("htmlize", "input");
	exarg_add_dependency ("htmlize", "stdout");

	/* call to parse arguments */
	exarg_parse (argc, argv);

	/* get current process id */
	axl_knife_pid = getpid ();

	/* normal operations */
	if (exarg_is_defined ("version")) {
		return printf ("%s\n", VERSION);
	}

	/* parse log options */
	msg ("Log to console activated: %d", exarg_is_defined ("enable-log"));
	axl_log_enable (exarg_is_defined ("enable-log"));

	/* check color to activate both logs */
	if (exarg_is_defined ("enable-log-color")) {
		axl_log_enable (true);
		axl_log_color_enable (true);
	}

	/* check to load a document */
	if (exarg_is_defined ("input")) {
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
	}


 finish:
	/* dealloc document opened */
	axl_doc_free (doc);

	/* terminating axl */
	axl_end ();

	/* terminate exarg */
	exarg_end ();

	return 0;
}
