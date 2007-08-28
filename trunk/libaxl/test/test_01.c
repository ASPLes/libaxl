#include <axl.h>
#include <axl_ns.h>

/** 
 * @brief Check DTD validation error found.
 * 
 * @param error The optional axlError to be used to report erros.
 * 
 * @return true if the validity test is passed, otherwise false is
 * returned.
 */
bool test_38 (axlError ** error)
{
	axlDtd          * dtd;
	axlDoc          * doc;

	dtd = axl_dtd_parse_from_file ("test_38.dtd", error);
	if (dtd == NULL)
		return false;

	doc = axl_doc_parse_from_file ("test_38.xml", error);
	if (doc == NULL)
		return false;

	/* validate */
	if (! axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "Expected to find proper a validation for the test (IDREF references)", NULL, error);
		return false;
	}
	
	/* free dtd and doc */
	axl_dtd_free (dtd);
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Check if it is possible to dettach the root node.
 * 
 * @param error The optional axlError to be used to report erros.
 * 
 * @return true if the validity test is passed, otherwise false is
 * returned.
 */
bool test_37 (axlError ** error)
{
	
	axlDoc  * doc;
	axlDoc  * doc2;
	axlNode * root;

	/* parse file */
	doc  = axl_doc_parse ("<child> This is content, more content </child>", -1, error);
	if (doc == NULL)
		return false;

	doc2 = axl_doc_parse ("<child />", -1, error);
	if (doc2 == NULL)
		return false;

	/* clear content */
	root = axl_doc_get_root (doc);
	axl_node_set_is_empty (root, true);

	if (! axl_doc_are_equal (doc, doc2)) {
		axl_error_new (-1, "Expected equal documents, not found", NULL, error);
		return false;
	}

	axl_doc_free (doc);
	axl_doc_free (doc2);

	return true;
}

/** 
 * @brief Check if it is possible to dettach the root node.
 * 
 * @param error The optional axlError to be used to report erros.
 * 
 * @return true if the validity test is passed, otherwise false is
 * returned.
 */
bool test_36 (axlError ** error)
{
	/* parse the document */
	axlDoc  * doc = axl_doc_parse_from_file ("test_35.xml", error);
	axlNode * root;

	/* check returned document */
	if (doc == NULL)
		return false;

	/* now get the root node and detach it */
	root = axl_doc_get_root (doc);
	axl_node_deattach (root);

	/* free the node */
	axl_node_free (root);
	
	/* free the document */
	axl_doc_free (doc);

	root = axl_node_parse_strings (error, 
				       "<child>",
				       "  <widget class=\"GtkLabel\" id=\"label4\">",
				       "    <property name=\"visible\">True</property>",
				       "    <property name=\"label\" translatable=\"yes\">&lt;b&gt;1. Seleccione el sistema:&lt;/b&gt;</property>",
				       "    <property name=\"use_underline\">False</property>",
				       "    <property name=\"use_markup\">True</property>",
				       "    <property name=\"justify\">GTK_JUSTIFY_LEFT</property>",
				       "    <property name=\"wrap\">False</property>",
				       "    <property name=\"selectable\">False</property>",
				       "    <property name=\"xalign\">0</property>",
				       "    <property name=\"yalign\">0.5</property>",
				       "    <property name=\"xpad\">0</property>",
				       "    <property name=\"ypad\">0</property>",
				       "    <property name=\"ellipsize\">PANGO_ELLIPSIZE_NONE</property>",
				       "    <property name=\"width_chars\">-1</property>",
				       "    <property name=\"single_line_mode\">False</property>",
				       "    <property name=\"angle\">0</property>",
				       "  </widget>",
				       "  <packing>",
				       "     <property name=\"padding\">0</property>",
				       "     <property name=\"expand\">False</property>",
				       "     <property name=\"fill\">False</property>",
				       "  </packing>",
				       "</child>",
				       NULL);
	if (root == NULL) {
		printf ("Error: unable to parse content..\n");
		return false;
	}

	axl_node_free (root);

	/* create the node using the parse api */
	root = axl_node_parse (error, "<child><widget class=\"GtkLabel\" id=\"label4\"/></child>");
	if (root == NULL) {
		printf ("Error: unable to parse content..\n");
		return false;
	}

	axl_node_free (root);
	

	return true;

}

/** 
 * @brief Checks a bug while opening a document.
 * 
 * @param error The optional axlError to be used to report erros.
 * 
 * @return true if the validity test is passed, otherwise false is
 * returned.
 */
bool test_35 (axlError ** error)
{
	/* parse the document */
	axlDoc * doc = axl_doc_parse_from_file ("test_35.xml", error);

	if (doc == NULL)
		return false;

	/* free the document */
	axl_doc_free (doc);

	return true;
	
}

/** 
 * @brief Checks a bug while triming strings.
 * 
 * @param error The optional axlError to be used to report erros.
 * 
 * @return true if the validity test is passed, otherwise false is
 * returned.
 */
bool test_34 (axlError ** error)
{
	char  * string = axl_strdup (" ");

	axl_stream_trim (string);
	if (strlen (string) != 0) {
		axl_error_new (-1, "Expected to find empty string after trim operation, but not found", NULL, error);
		return false;
	}

	axl_free (string);

	/* more length */
	string = axl_strdup ("       ");

	axl_stream_trim (string);
	if (strlen (string) != 0) {
		axl_error_new (-1, "Expected to find empty string after trim operation, but not found", NULL, error);
		return false;
	}

	axl_free (string);

	/* more length with one byte */
	string = axl_strdup ("  a     ");

	axl_stream_trim (string);
	if (strlen (string) != 1) {
		axl_error_new (-1, "Expected to find one byte length string after trim operation, but not found", NULL, error);
		return false;
	}

	axl_free (string);

	return true;
}

/** 
 * @brief Checks a recursive root node configuration.
 * 
 * @param error The optional axlError to be used to report erros.
 * 
 * @return true if the validity test is passed, otherwise false is
 * returned.
 */
bool test_33 (axlError ** error)
{
	axlDoc          * doc;
	axlNode         * node;
	axlNode         * temp;
	int               iterator;

	/* create an empty document */
	doc       = axl_doc_create (NULL, NULL, true);
	
	/* set the root node */
	node      = axl_node_create ("test");
	axl_doc_set_root (doc, node);

	/* create a root node */
	iterator = 0;
	while (iterator < 2) {
		/* get current root */
		temp = axl_doc_get_root (doc);

		/* create a new root */
		node = axl_node_create ("test");
		
		/* configure the new root */
		axl_doc_set_root (doc, node);

		/* configure the child */
		axl_node_set_child (node, temp);
		
		/* next iterator */
		iterator++;
	}

	/* free */
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Test DTD attribute declaration support <!ATTLIST > using IDREF
 * declarations.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_32 (axlError ** error) {
	
	axlDtd          * dtd;
	axlDoc          * doc;

	dtd = axl_dtd_parse_from_file ("test_32.dtd", error);
	if (dtd == NULL)
		return false;

	doc = axl_doc_parse_from_file ("test_32.xml", error);
	if (doc == NULL)
		return false;

	/* validate */
	if (! axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "Expected to find a validation error for the test (IDREF references)", NULL, error);
		return false;
	}
	
	/* free dtd and doc */
	axl_dtd_free (dtd);
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Test DTD attribute declaration support <!ATTLIST > using ID
 * declarations.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_31 (axlError ** error) {
	
	axlDtd          * dtd;
	axlDoc          * doc;

	dtd = axl_dtd_parse_from_file ("test_31.dtd", error);
	if (dtd == NULL)
		return false;

	doc = axl_doc_parse_from_file ("test_31.xml", error);
	if (doc == NULL)
		return false;

	/* validate */
	if (! axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "Expected to find a validation error for the test (unique ID)", NULL, error);
		return false;
	}
	
	/* free dtd and doc */
	axl_dtd_free (dtd);
	axl_doc_free (doc);

	dtd = axl_dtd_parse_from_file ("test_31a.dtd", error);
	if (dtd != NULL) {
		axl_error_new (-1, "Expected to find a error due to double declaration for an ID attribute, but DTD was loaded ok", NULL, error);
		axl_dtd_free (dtd);
		return false;
	}
	axl_error_free (*error);
	*error = NULL;

	return true;
}


/** 
 * @brief Test DTD attribute declaration support <!ATTLIST >
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_30 (axlError ** error) {
	
	axlDtd          * dtd;
	axlDtdAttribute * attr;
	axlDoc          * doc;

	dtd = axl_dtd_parse_from_file ("test_30.dtd", error);
	if (dtd == NULL)
		return false;

	/* get the declarations found */
	attr = axl_dtd_get_attr (dtd, "node");
	if (attr == NULL) {
		axl_error_new (-1, "expected to find attribute declaration, but not found", NULL, error);
		return false;
	} /* end if */

	/* get the declarations found */
	attr = axl_dtd_get_attr (dtd, "node1");
	if (attr != NULL) {
		axl_error_new (-1, "expected to NOT find attribute declaration, but not found", NULL, error);
		return false;
	} /* end if */

	/* check the number of contraints */
	if (axl_dtd_get_attr_contraints (dtd, "node") != 3) {
		axl_error_new (-1, "expected to find 2 contraints for the <node>, but not found", NULL, error);
		return false;
	} /* end if */

	doc = axl_doc_parse_from_file ("test_30.xml", error);
	if (doc == NULL) {
		axl_error_new (-1, "unable to parse file to check attribute validation", NULL, error);
		return false;
	}

	/* validate */
	if (! axl_dtd_validate (doc, dtd, error))
		return false;

	axl_doc_free (doc);

	doc = axl_doc_parse_from_file ("test_30a.xml", error);
	if (doc == NULL) {
		axl_error_new (-1, "unable to parse file to check attribute validation", NULL, error);
		return false;
	}

	/* validate */
	if (axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "Expected to find non-proper validation for enum value (2), inside attlist declaration", NULL, error);
		return false;
	}
	axl_error_free (*error);

	axl_dtd_free (dtd);

	dtd = axl_dtd_parse_from_file ("test_30b.dtd", error);
	if (dtd == NULL)
		return false;

	
	/* validate */
	if (axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "Expected to  find non-proper validation for required value, inside attlist declaration", NULL, error);
		return false;
	}
	axl_error_free (*error);
	
	axl_doc_free (doc);

	doc = axl_doc_parse_from_file ("test_30b.xml", error);
	if (doc == NULL) {
		axl_error_new (-1, "unable to parse file to check attribute validation", NULL, error);
		return false;
	}

	/* validate */
	if (! axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "Expected to FIND proper validation for required value, inside attlist declaration", NULL, error);
		return false;
	}

	axl_doc_free (doc);
	axl_dtd_free (dtd);
	return true;
}

#ifdef AXL_NS_SUPPORT

#define HTML_NS "http://www.w3.org/1999/xhtml"
#define BOOK_NS "urn:loc.gov:books"
#define ISBN_NS "urn:ISBN:0-395-36341-6"

/** 
 * @brief Test namespace defaulting support from axl ns library.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_29 (axlError ** error)
{
	axlDoc  * doc;
	axlNode * node;

	/* parse namespace file */
	doc = axl_doc_parse_from_file ("test_29.xml", error);
	if (doc == NULL)
		return false;

	/* call to validate namespace */
	if (! axl_ns_doc_validate (doc, error))
		return false;

	/* get root document */
	node = axl_doc_get_root (doc);

	/* find number inside isbn namespace */
	node = axl_ns_node_find_called (node, HTML_NS, "p");
	if (node == NULL || ! axl_ns_node_cmp (node, HTML_NS, "p")) {
		axl_error_new (-1, "Expected to find xhtml p node, but it wasn't found", NULL, error);
		return false;
	} /* end if */

	/* get root document */
	node = axl_doc_get_root (doc);

	/* find number inside isbn namespace */
	node = axl_ns_node_get_child_called (node, ISBN_NS, "number");
	if (node == NULL || ! axl_ns_node_cmp (node, ISBN_NS, "number")) {
		axl_error_new (-1, "Expected to find isbn number node, but it wasn't found", NULL, error);
		return false;
	} /* end if */

	/* find number inside isbn namespace */
	node = axl_ns_node_get_next_called (node, ISBN_NS, "test");
	if (node == NULL || ! axl_ns_node_cmp (node, ISBN_NS, "test")) {
		axl_error_new (-1, "Expected to find isbn test node, but it wasn't found", NULL, error);
		return false;
	} /* end if */

	
	/* free document */
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Test namespace defaulting support from axl ns library.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_28 (axlError ** error)
{
	axlDoc  * doc;
	axlNode * node;

	/* parse namespace file */
	doc = axl_doc_parse_from_file ("test_28.xml", error);
	if (doc == NULL)
		return false;

	/* call to validate namespace */
	if (! axl_ns_doc_validate (doc, error))
		return false;

	/* get root document */
	node = axl_doc_get_root (doc);

	/* get following node */
	node = axl_node_get_first_child (node);

	/* check default namespace */
	if (! axl_ns_node_cmp (node, HTML_NS, "table")) {
		axl_error_new (-1, "expected to find a valid ns-node-cmp, but it wasn't found", NULL, error);
		return false;
	}

	/* get the following */
	node = axl_node_get_first_child (node);

	/* check default namespace */
	if (! axl_ns_node_cmp (node, HTML_NS, "th")) {
		axl_error_new (-1, "expected to find a valid ns-node-cmp, but it wasn't found", NULL, error);
		return false;
	}

	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Test namespace support from axl ns library.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_27 (axlError ** error)
{
	axlDoc  * doc;
	axlNode * node;

	/* parse namespace file */
	doc = axl_doc_parse_from_file ("test_27.xml", error);
	if (doc == NULL)
		return false;

	/* call to validate namespace */
	if (! axl_ns_doc_validate (doc, error))
		return false;

	/* get root document */
	node = axl_doc_get_root (doc);

	if (! axl_ns_node_cmp (node, BOOK_NS, "book")) {
		axl_error_new (-1, "expected to find a valid ns-node-cmp, but it wasn't found", NULL, error);
		return false;
	}

	/* get first child */
	node = axl_node_get_first_child (node);

	if (! axl_ns_node_cmp (node, BOOK_NS, "title")) {
		axl_error_new (-1, "expected to find a failure validating with ns-node-cmp, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_next (node);
	if (axl_ns_node_cmp (node, BOOK_NS, "number")) {
		axl_error_new (-1, "expected to find a failure validating with ns-node-cmp, but it wasn't found", NULL, error);
		return false;
	}

	/* get next */
	if (! axl_ns_node_cmp (node, ISBN_NS, "number")) {
		axl_error_new (-1, "expected to find a valid ns-node-cmp, but it wasn't found", NULL, error);
		return false;
	}

	axl_doc_free (doc);

	return true;
}


/** 
 * @brief Test namespace support from axl ns library.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_26 (axlError ** error)
{
	axlDoc * doc;

	/* parse namespace file */
	doc = axl_doc_parse_from_file ("test_26.xml", error);
	if (doc == NULL)
		return false;

	/* call to validate namespace */
	if (! axl_ns_doc_validate (doc, error))
		return false;

	axl_doc_free (doc);

	/* parse a namespace file that do not follow rules (node) */
	doc = axl_doc_parse_from_file ("test_26b.xml", error);
	if (doc == NULL)
		return false;

	/* call to validate namespace */
	if (axl_ns_doc_validate (doc, error)) {
		axl_error_new (-1, "Expected to find ns validation error, but not found (test_26b)", NULL, error);
		return false;
	}
	axl_error_free (*error);

	axl_doc_free (doc);

	/* parse a namespace file that do not follow rules (attribute) */
	doc = axl_doc_parse_from_file ("test_26c.xml", error);
	if (doc == NULL)
		return false;

	/* call to validate namespace */
	if (axl_ns_doc_validate (doc, error)) {
		axl_error_new (-1, "Expected to find ns validation error, but not found (test_26c)", NULL, error);
		return false;
	}
	axl_error_free (*error);

	axl_doc_free (doc);

	/* parse a namespace file that do not follow rules (declaring twice default namespace) */
	doc = axl_doc_parse_from_file ("test_26d.xml", error);
	if (doc == NULL)
		return false;

	/* call to validate namespace */
	if (axl_ns_doc_validate (doc, error)) {
		axl_error_new (-1, "Expected to find ns validation error, but not found (test_26d)", NULL, error);
		return false;
	}
	axl_error_free (*error);

	axl_doc_free (doc);

	return true;
}

#endif /* end #ifdef AXL_NS_SUPPORT */

/** 
 * @brief Test Axl Item API while performing lookups.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_25 (axlError ** error) {
	
	axlDoc  * doc;
	axlNode * root;
	axlNode * node;

	/* load the document */
	doc = axl_doc_parse_from_file ("test_23.xml", error);
	if (doc == NULL)
		return false;	

	/* get root node */
	root = axl_doc_get_root (doc);

	/* lookup a node */
	node = axl_node_find_called (root, "child3");

	if (! NODE_CMP_NAME (node, "child3")) {
		axl_error_new (-1, "Expected to find <child3> node but it wasn't found", NULL, error);
		return false;
	} 

	/* lookup a node */
	node = axl_node_find_called (root, "strong");

	if (! NODE_CMP_NAME (node, "strong")) {
		axl_error_new (-1, "Expected to find <strong> node but it wasn't found", NULL, error);
		return false;
	} 

	if (! axl_cmp (axl_node_get_content (node, NULL), "this content goes\n  bold")) {
		axl_error_new (-1, "Expected to find <strong> node content, but it wasn't found", NULL, error);
		return false;
	}

	/* lookup a node */
	node = axl_node_find_called (root, "strong1");
	if (node != NULL) {
		axl_error_new (-1, "Expected to not find node content, but it wasn't found", NULL, error);
		return false;
	}

	/* free the document */
	axl_doc_free (doc);
	
	return true;
}


/** 
 * @brief Test Axl Item API while replacing nodes and adding content
 * on a particular position.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_24 (axlError ** error) {
	if (! axl_node_has_invalid_chars ("MEDIR PIEZAS S/MUESTREO EN MAQUINA DE VISION SIN CONTACTO\n\
\n\
LOTES<10 UDS.                  100%PIEZAS\n\
10 UDS.<LOTES<20 UDS.  10+20% (SOBRE 10 PIEZAS)\n\
LOTES>20  UDS                  12+10% (SOBRE 20 PIEZAS)                    \n\
 \n\
¡¡¡¡ATENCION!!!!\n\
MANIPULAR PIEZAS CON GUANTES DE LATEX EVITANDO CONTAMINAR LAS PIEZAS", -1, NULL)) {
		axl_error_new (-1, "Expected to find invalid characters, but it wasn't found", NULL, error);
		return false;
	}

	if (axl_node_has_invalid_chars ("MEDIR PIEZAS S/MUESTREO EN MAQUINA DE VISION SIN CONTACTO\n\
\n\
LOTES 10 UDS.               100%PIEZAS\n\
10 UDS. LOTES 20 UDS.  10+20% (SOBRE 10 PIEZAS)\n\
LOTES20  UDS                  12+10% (SOBRE 20 PIEZAS)                    \n\
 \n\
¡¡¡¡ATENCION!!!!\n\
MANIPULAR PIEZAS CON GUANTES DE LATEX EVITANDO CONTAMINAR LAS PIEZAS", -1, NULL)) {
		axl_error_new (-1, "Expected to find valid characters, but it wasn't found", NULL, error);
		return false;
	}

	return true;
}

/** 
 * @brief Test Axl Item API while replacing nodes and adding content
 * on a particular position.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_23 (axlError ** error)
{
	axlDoc  * doc;
	axlDoc  * doc2;
	axlNode * node;
	axlItem * item;

	/* load the document */
	doc = axl_doc_parse_from_file ("test_23.xml", error);
	if (doc == NULL)
		return false;

	/* get the child1 node */
	node = axl_doc_get (doc, "/document/childs/child3a");
	if (node == NULL) {
		axl_error_new (-1, "Unable to get child3a node under /document/child3a", NULL, error);
		return false;
	}

	if (! NODE_CMP_NAME (node, "child3a")) {
		axl_error_new (-1, "Found node that wasn't expected", NULL, error);
		return false;
	}

	if (! axl_node_is_empty (node)) {
		axl_error_new (-1, "Expected to find child3a node to be empty, but it wasn't found", NULL, error);
		return false;
	}
	
	/* get the child1 node */
	node = axl_doc_get (doc, "/document/childs/child1");
	if (node == NULL) {
		axl_error_new (-1, "Unable to get child1 node under /document/childs", NULL, error);
		return false;
	}

	if (! NODE_CMP_NAME (node, "child1")) {
		axl_error_new (-1, "Found node that wasn't expected", NULL, error);
		return false;
	}

	/* create content */
	item = axl_item_new (ITEM_CONTENT, "This is a test");

	/* replace the node */
	axl_item_replace (axl_item_node_holder (node), item, true);

	/* now parse the reference xml document */
	doc2 = axl_doc_parse_from_file ("test_23b.xml", error);
	if (doc2 == NULL)
		return false;
	
	/* check that both documents are equal */
	if (! axl_doc_are_equal_trimmed (doc, doc2)) {
		axl_error_new (-1, "Expected to find equal documents, but it wasn't found", NULL, error);
		return false;
	}
	
	/* free the document */
	axl_doc_free (doc);

	/* load the document */
	doc = axl_doc_parse_from_file ("test_23.xml", error);
	if (doc == NULL)
		return false;

	/* check that both documents aren't equal using strict comparation */
	if (axl_doc_are_equal (doc, doc2)) {
		axl_error_new (-1, "Expected to find documents not equal, but it wasn't found", NULL, error);
		return false;
	}

	/* free the document */
	axl_doc_free (doc);
	axl_doc_free (doc2);

	/* test ok */
	return true;
}

/** 
 * @brief Test xml attribute support.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_22 (axlError ** error)
{
	axlDoc        * doc;
	axlNode       * node;
	const   char  * value;
	axlAttrCursor * cursor;

	
	/* create a document */
	doc  = axl_doc_create (NULL, NULL, false);

	node = axl_node_create ("root-node");
	axl_doc_set_root (doc, node);

	/* check for attributes */
	if (axl_node_has_attribute (node, "attribute-not-found")) {
		axl_error_new (-1, "Found an attribute requested, which doesn't exists", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute-not-found");
	if (value != NULL) {
		axl_error_new (-1, "Found an attribute requested, which doesn't exists", NULL, error);
		return false;
	}

	/* store an attriburte */
	axl_node_set_attribute (node, "attribute1", "value1");

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute1")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute1");
	if (! axl_cmp (value, "value1")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists", NULL, error);
		return false;
	}

	/* store more attributes to check function with hashes */
	axl_node_set_attribute (node, "attribute2", "value2");
	axl_node_set_attribute (node, "attribute3", "value3");
	axl_node_set_attribute (node, "attribute4", "value4");
	axl_node_set_attribute (node, "attribute5", "value5");
	axl_node_set_attribute (node, "attribute6", "value6");

	/* check axl attribute iteration API */
	cursor = axl_node_attr_cursor_new (node);
	while (axl_node_attr_cursor_has_item (cursor)) {
		
		if (axl_cmp ("attribute2", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value2", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute2' isn't 'value2'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute3", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value3", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute3' isn't 'value3'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute4", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value4", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute4' isn't 'value4'", NULL, error);
			return false;
		} /* end if */
			     
		if (axl_cmp ("attribute5", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value5", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute5' isn't 'value5'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute6", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value6", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute6' isn't 'value6'", NULL, error);
			return false;
		} /* end if */

		/* go next */
		axl_node_attr_cursor_next (cursor);

	} /* end if */

	axl_node_attr_cursor_free (cursor);


	axl_node_set_attribute (node, "attribute7", "value7");
	axl_node_set_attribute (node, "attribute8", "value8");
	axl_node_set_attribute (node, "attribute9", "value9");
	axl_node_set_attribute (node, "attribute10", "value10");
	axl_node_set_attribute (node, "attribute11", "value11");
	
	/* check that an attribute doesn't exists */
	if (axl_node_has_attribute (node, "attribute-not-found")) {
		axl_error_new (-1, "Found an attribute requested, which doesn't exists", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute-not-found");
	if (value != NULL) {
		axl_error_new (-1, "Found an attribute requested, which doesn't exists", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute2")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists(2)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute2");
	if (! axl_cmp (value, "value2")) {
		printf ("value2 != %s\n", value);
		axl_error_new (-1, "Not found an attribute requested, which should exists(value2)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute3")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists(3)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute3");
	if (! axl_cmp (value, "value3")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value3)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute4")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value4)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute4");
	if (! axl_cmp (value, "value4")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value4)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute5")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value5)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute5");
	if (! axl_cmp (value, "value5")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value5)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute6")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value6)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute6");
	if (! axl_cmp (value, "value6")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value6)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute7")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value7)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute7");
	if (! axl_cmp (value, "value7")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value7)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute8")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value8)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute8");
	if (! axl_cmp (value, "value8")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value8)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute9")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value9)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute9");
	if (! axl_cmp (value, "value9")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value9)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute10")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value10)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute10");
	if (! axl_cmp (value, "value10")) {
		axl_error_new (-1, "Not found an attribute value requested, which should exists (value10)", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute11")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value11)", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute11");
	if (! axl_cmp (value, "value11")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists (value11)", NULL, error);
		return false;
	}

	/* check axl attribute iteration API */
	cursor = axl_node_attr_cursor_new (node);
	while (axl_node_attr_cursor_has_item (cursor)) {
		
		if (axl_cmp ("attribute2", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value2", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute2' isn't 'value2'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute3", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value3", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute3' isn't 'value3'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute4", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value4", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute4' isn't 'value4'", NULL, error);
			return false;
		} /* end if */
			     
		if (axl_cmp ("attribute5", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value5", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute5' isn't 'value5'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute6", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value6", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute6' isn't 'value6'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute7", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value7", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute7' isn't 'value7'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute8", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value8", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute8' isn't 'value8'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute9", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value9", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute9' isn't 'value9'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute10", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value10", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute10' isn't 'value10'", NULL, error);
			return false;
		} /* end if */

		if (axl_cmp ("attribute11", axl_node_attr_cursor_get_key (cursor)) &&
		    ! axl_cmp ("value11", axl_node_attr_cursor_get_value (cursor))) {
			axl_error_new (-1, "Found that the value associated to 'attribute11' isn't 'value11'", NULL, error);
			return false;
		} /* end if */

		/* go next */
		axl_node_attr_cursor_next (cursor);

	} /* end if */

	axl_node_attr_cursor_free (cursor);

	/* remove attributes */
	if (axl_node_num_attributes (node) != 11) {
		axl_error_new (-1, "Expected to find 11 attributes", NULL, error);
		return false;
	}
	axl_node_remove_attribute (node, "attribute1");

	if (axl_node_num_attributes (node) != 10) {
		axl_error_new (-1, "Expected to find 10 attributes", NULL, error);
		return false;
	}
	
	if (axl_node_has_attribute (node, "attribute1")) {
		axl_error_new (-1, "Found that attribute1 should not appear, but it was found", NULL, error);
		return false;
	} /* end if */

	node = axl_node_create ("test");
	axl_node_set_attribute (node, "test", "test");
	
	if (axl_node_num_attributes (node) != 1) {
		axl_error_new (-1, "Expected to find 1 attributes", NULL, error);
		return false;
	}

	axl_node_remove_attribute (node, "test");

	if (axl_node_num_attributes (node) != 0) {
		axl_error_new (-1, "Expected to find 1 attributes", NULL, error);
		return false;
	}

	axl_node_set_attribute (node, "test1", "test");
	axl_node_set_attribute (node, "test2", "test");
	axl_node_set_attribute (node, "test3", "test");

	if (axl_node_num_attributes (node) != 3) {
		axl_error_new (-1, "Expected to find 3 attributes", NULL, error);
		return false;
	}

	axl_node_remove_attribute (node, "test1");
	axl_node_remove_attribute (node, "test2");
	axl_node_remove_attribute (node, "test3");

	if (axl_node_num_attributes (node) != 0) {
		axl_error_new (-1, "Expected to find 0 attributes", NULL, error);
		return false;
	}

	axl_node_free (node);
	
	/* free document */
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Test mixed content documents support (TEST NOT FINISHED).
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_21 (axlError ** error)
{
	axlDoc  * doc;
	axlNode * node;
	axlItem * item;
	char    * content;
	int       content_size;

	/* load the document */
	doc = axl_doc_parse_from_file ("test_21.xml", error);
	if (doc == NULL)
		return false;

	/* get the root node */
	node = axl_doc_get_root (doc);

	/* check document content */
	if (! NODE_CMP_NAME (node, "document")) {
		axl_error_new (-1, "Expected to find root node=<document> but it wasn't found", NULL, error);
		return false;
	}

	/* iterate root childs */
	item = axl_item_get_first_child (node);

	if (axl_item_get_type (item) != ITEM_CONTENT) {
		axl_error_new (-1, "Expected to find content but it wasn't found", NULL, error);
		return false;
	}	

	/* get the content */
	content = axl_item_get_content (item, &content_size);
	if (! axl_cmp (content, "\n  Some content inside the document ")) {
		axl_error_new (-1, "Expected to find content but it wasn't found", NULL, error);
		return false;
	}

	/* get tne the item */
	item = axl_item_get_next (item);
	if (axl_item_get_type (item) != ITEM_NODE) {
		axl_error_new (-1, "Expected to find a node but it wasn't found", NULL, error);
		return false;
	}

	/* get the node */
	node = axl_item_get_data (item);

	/* check document content */
	if (! NODE_CMP_NAME (node, "strong")) {
		axl_error_new (-1, "Expected to find root node=<strong> but it wasn't found", NULL, error);
		return false;
	}

	/* get the first child of <strong> */
	item = axl_item_get_first_child (node);

	if (axl_item_get_type (item) != ITEM_CONTENT) {
		axl_error_new (-1, "Expected to find content inside strong but it wasn't found", NULL, error);
		return false;
	}	

	/* get the content */
	content = axl_item_get_content (item, &content_size);
	if (! axl_cmp (content, "this content goes\n  bold")) {
		axl_error_new (-1, "Expected to find content but it wasn't found", NULL, error);
		return false;
	}

	/* now get the next item following the <strong> node */
	item = axl_item_node_next (node);

	if (item == NULL) {
		axl_error_new (-1, "Expected to find content following <strong> but a null item reference was found", NULL, error);
		return false;
	}

	/* check to be it a content */
	if (axl_item_get_type (item) != ITEM_CONTENT) {
		axl_error_new (-1, "Expected to find content following <strong> node but it wasn't found", NULL, error);
		return false;
	}	

	/* get the content */
	content = axl_item_get_content (item, &content_size);
	if (! axl_cmp (content, " more data stored directly inside the document node.\n\n ")) {
		axl_error_new (-1, "Expected to find content but it wasn't found", NULL, error);
		return false;
	}

	/* get the next item */
	item = axl_item_get_next (item);
	if (axl_item_get_type (item) != ITEM_NODE) {
		axl_error_new (-1, "Expected to find a node but it wasn't found", NULL, error);
		return false;
	}

	/* get the node */
	node = axl_item_get_data (item);

	/* check document content */
	if (! NODE_CMP_NAME (node, "childs")) {
		axl_error_new (-1, "Expected to find root node=<strong> but it wasn't found", NULL, error);
		return false;
	}

	/* get the first item */
	item = axl_item_get_first_child (node);
	if (axl_item_get_type (item) != ITEM_COMMENT) {
		axl_error_new (-1, "Expected to find content inside strong but it wasn't found", NULL, error);
		return false;
	}	

	/* get the content */
	content = axl_item_get_content (item, &content_size);
	if (! axl_cmp (content, " here goes a comment before text block ")) {
		axl_error_new (-1, "Expected to find a comment, child of <childs>, but it wasn't found", NULL, error);
		return false;
	}	

	/* get next item */
	item = axl_item_get_next (item);

	/* check to be it a content */
	if (axl_item_get_type (item) != ITEM_CONTENT) {
		axl_error_new (-1, "Expected to find content following <childs> node comment but it wasn't found", NULL, error);
		return false;
	}	

	/* get the content */
	content = axl_item_get_content (item, &content_size);
	if (! axl_cmp (content, "More text after child declaration.\n   ")) {
		axl_error_new (-1, "Expected to find a content inside <childs> node, but it wasn't found", NULL, error);
		return false;
	}	

	/* get next item */
	item = axl_item_get_next (item);

	/* get the node */
	node = axl_item_get_data (item);

	/* check document content */
	if (! NODE_CMP_NAME (node, "child1")) {
		axl_error_new (-1, "Expected to find root node=<strong> but it wasn't found", NULL, error);
		return false;
	}


	/* free axl document */
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Test entity support (basic entity support).
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_20b (axlError ** error)
{
	axlNode * node;

	axlNode * aux;
	axlNode * aux2;

	axlNode * child1;
	axlNode * root;

	axlDoc  * doc;
	axlDoc  * doc2;
	axlDoc  * doc3;


	int       iterator = 0;
	int       sum = 0;

	/* load the document */
	doc = axl_doc_parse_from_file ("test_20.xml", error);
	if (doc == NULL)
		return false;

	/* load reference */
	doc2 = axl_doc_parse_from_file ("test_20a.xml", error);
	if (doc2 == NULL)
		return false;

	while (iterator < 10) {

		/* get document root */
		root = axl_doc_get_root (doc);
	
		/* copy and release */
		node = axl_node_copy (root, true, true);

		/* get the child1 reference */
		child1 = axl_node_get_first_child (node);
		
		/* get child2 refrence */
		aux    = axl_node_get_first_child (child1);
		while (aux != NULL) {
			
			sum = axl_node_get_child_num (child1);
			
			/* get the next child before deattaching the
			 * node */
			aux2 = axl_node_get_next (aux);

			/* dettach the node from the <child1> parent
			 * and attach it to the document */
			axl_node_deattach (aux);

			if ((sum - 1) != (axl_node_get_child_num (child1))) {
				/* after doing the deattach process,
				 * the copy node doesn't have the
				 * expected child number */
				axl_error_new (-1, "After doing the deattach process, the copy node doesn't have the expected child number", NULL, error);
				return false;
			}

			/* set the node to the document root */
			axl_node_set_child (node, aux);

			if (! NODE_CMP_NAME (axl_node_get_parent (aux), "document")) {
				
				axl_error_new (-1, "Expected to find a parent not found", NULL, error);
				return false;
			}

			/* get the next child */
			aux = aux2;
			
		} /* end while */

		/* remove the child1 node */
		aux = axl_node_get_first_child (node);
		axl_node_remove (aux, true);

		/* create the document holding the result */
		doc3 = axl_doc_create (NULL, NULL, false);
		axl_doc_set_root (doc3, node);

		/* compare the document */
		if (!axl_doc_are_equal (doc2, doc3)) {
			axl_error_new (-1, "Expected to find equal documents but (modified doc2 != doc3), they wasn't found", NULL, error);
			return false;
		}

		/* free the document */
		axl_doc_free (doc3);

		/* update iterator */
		iterator++;

	} /* end while */

	/* free the document 2 */
	axl_doc_free (doc2);

	/* free the document */
	axl_doc_free (doc);

	return true;

}


/** 
 * @brief Test entity support (basic entity support).
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_20 (axlError ** error)
{
	axlNode * node;
	axlNode * root;
	axlDoc  * doc;
	axlDoc  * doc2;
	char    * data;
	

	/* load the document */
	doc = axl_doc_parse_from_file ("test_20.xml", error);
	if (doc == NULL)
		return false;

	/* get document root */
	root = axl_doc_get_root (doc);
	node = axl_node_copy (root, true, true);

	/* check if both nodes are equal */
	if (! axl_node_are_equal (root, node)) {
		axl_error_new (-1, "Expected to find equal nodes but they weren't", NULL, error);
		return false;
	}
	
	/* create a new document */
	doc2 = axl_doc_create (NULL, NULL, false);
	axl_doc_set_root (doc2, node);

	if (! axl_doc_are_equal (doc, doc2)) {
		axl_error_new (-1, "Expected to find equal documents but they weren't", NULL, error);
		return false;
	}

	/* free document */
	axl_doc_free (doc2);

	/* configure some anotation data */
	axl_node_annotate_data (root, "key", "value");
	
	/* get child1 */
	node = axl_node_get_first_child (root);

	/* get child2 */
	node = axl_node_get_first_child (node);

	/* anotate data */
	axl_node_annotate_data (node, "key1", "value1");

	/* perform searches */
	data = axl_node_annotate_get (node, "key", false);
	if (data != NULL) {
		axl_error_new (-1, "Expected to find nothing while looking for 'key'(1)", NULL, error);
		return false;
	}

	data = axl_node_annotate_get (node, "key", true);
	if (data == NULL || !axl_cmp (data, "value")) {
		axl_error_new (-1, "Expected to find data while looking for 'key' at parents (2)", NULL, error);
		return false;
	}

	/* perform searches inside the node */
	data = axl_node_annotate_get (node, "key1", false);
	if (data == NULL || !axl_cmp (data, "value1")) {
		axl_error_new (-1, "Expected to find nothing while looking for 'key1'(3)", NULL, error);
		return false;
	}

	/* perform more anotation but with native data */
	axl_node_annotate_int (root, "int-value", 14);
	
	if (axl_node_annotate_get_int (root, "int-value", false) != 14) {
		axl_error_new (-1, "Expected to find an integer value (14), but it wasn't found", NULL, error);
		return false;
	}

	axl_node_annotate_double (root, "double-value", 58.20);

	if (axl_node_annotate_get_double (root, "double-value", false) != 58.20) {
		axl_error_new (-1, "Expected to find an double value (58.20), but it wasn't found", NULL, error);
		return false;
	}

	axl_node_annotate_string (root, "string-value", "this is a test string");

	if (! axl_cmp (axl_node_annotate_get_string (root, "string-value", false), "this is a test string")) {
		axl_error_new (-1, "Expected to find a string value (\"this is a test string\"), but it wasn't found", NULL, error);
		return false;
	}

	if (axl_node_annotate_get_string (root, "string-not-found", false) != NULL) {
		axl_error_new (-1, "Expected to find empty value for an anotated element which isn't installed", NULL, error);
		return false;
	}

	/* free document created */
	axl_doc_free (doc);

	return true;

}

/** 
 * @brief Test entity support (basic entity support).
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_19 (axlError ** error)
{
	axlDoc  * doc;
	axlDoc  * reference;
	axlNode * node;
	axlNode * replace;

	/* create replace node */
	replace = axl_node_create ("replace");
	axl_node_set_content (replace, "test", -1);

	doc = axl_doc_parse_from_file ("test_19.xml", error);
	if (doc == NULL)
		return false;
	
	/* document */
	node = axl_doc_get_root (doc);

	/* child1 */
	node = axl_node_get_first_child (node);

	/* child2 */
	node = axl_node_get_first_child (node);

	/* replace */
	axl_node_replace (node, replace, false);

	reference = axl_doc_parse_from_file ("test_19a.xml", error);
	if (reference == NULL)
		return false;

	/* check both documents to be equal */
	if (! axl_doc_are_equal (doc, reference)) {
		axl_error_new (-1, "Expected to find equal documents, but not found on first child replace", NULL, error);
		return false;
	}

	/* free reference */
	axl_doc_free (reference);

	/* restore */
	axl_node_replace (replace, node, false);

	/* get child2 */
	node = axl_node_get_next (node);
	
	/* replace */
	axl_node_replace (node, replace, false);


	reference = axl_doc_parse_from_file ("test_19b.xml", error);
	if (reference == NULL)
		return false;

	/* check both documents to be equal */
	if (! axl_doc_are_equal (doc, reference)) {
		axl_error_new (-1, "Expected to find equal documents, but not found on first child replace", NULL, error);
		return false;
	}

	/* free reference */
	axl_doc_free (reference);	

	/* restore */
	axl_node_replace (replace, node, false);

	/* get child2 */
	node = axl_node_get_next (node);
	/* get child2 */
	node = axl_node_get_next (node);
	
	/* replace */
	axl_node_replace (node, replace, true);

	reference = axl_doc_parse_from_file ("test_19c.xml", error);
	if (reference == NULL)
		return false;

	/* check both documents to be equal */
	if (! axl_doc_are_equal (doc, reference)) {
		axl_error_new (-1, "Expected to find equal documents, but not found on first child replace", NULL, error);
		return false;
	}

	/* free reference */
	axl_doc_free (reference);	

	node = axl_node_create ("child5");
	axl_node_set_content (node, "test", -1);

	/* replace */
	axl_node_replace (replace, node, true);

	/* document */
	node = axl_doc_get_root (doc);
	/* child1 */
	node = axl_node_get_first_child (node);
	/* child2 */
	node = axl_node_get_first_child (node);	

	/* remove child2 */
	axl_node_remove (node, true);

	reference = axl_doc_parse_from_file ("test_19d.xml", error);
	if (reference == NULL)
		return false;

	/* check both documents to be equal */
	if (! axl_doc_are_equal (doc, reference)) {
		axl_error_new (-1, "Expected to find equal documents, but not found on first child remove", NULL, error);
		return false;
	}

	/* free reference */
	axl_doc_free (reference);	

	/* document */
	node = axl_doc_get_root (doc);
	/* child1 */
	node = axl_node_get_first_child (node);
	/* child3 */
	node = axl_node_get_first_child (node);	

	/* remove child3 */
	axl_node_remove (node, true);

	reference = axl_doc_parse_from_file ("test_19e.xml", error);
	if (reference == NULL)
		return false;

	/* check both documents to be equal */
	if (! axl_doc_are_equal (doc, reference)) {
		axl_error_new (-1, "Expected to find equal documents, but not found on first child remove", NULL, error);
		return false;
	}

	/* free reference */
	axl_doc_free (reference);	

	/* document */
	node = axl_doc_get_root (doc);
	/* child1 */
	node = axl_node_get_first_child (node);
	/* child4 */
	node = axl_node_get_first_child (node);	

	/* remove child4 */
	axl_node_remove (node, true);

	reference = axl_doc_parse_from_file ("test_19f.xml", error);
	if (reference == NULL)
		return false;

	/* check both documents to be equal */
	if (! axl_doc_are_equal (doc, reference)) {
		axl_error_new (-1, "Expected to find equal documents, but not found on first child remove", NULL, error);
		return false;
	}

	/* free reference */
	axl_doc_free (reference);	

	/* document */
	node = axl_doc_get_root (doc);
	/* child1 */
	node = axl_node_get_first_child (node);
	/* child5 */
	node = axl_node_get_first_child (node);	

	/* remove child5 */
	axl_node_remove (node, true);

	reference = axl_doc_parse_from_file ("test_19g.xml", error);
	if (reference == NULL)
		return false;

	/* check both documents to be equal */
	if (! axl_doc_are_equal (doc, reference)) {
		axl_error_new (-1, "Expected to find equal documents, but not found on first child remove", NULL, error);
		return false;
	}

	/* free reference */
	axl_doc_free (reference);	
	
	/* free document */
	axl_doc_free (doc);

	/* free reference */
	return true;
}

/** 
 * @brief Test entity support (basic entity support).
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_18 (axlError ** error)
{
	axlDtd * dtd = NULL;
	char   * content;

	/* parse af-arch DTD */
	dtd = axl_dtd_parse_from_file ("channel.dtd", error);
	if (dtd == NULL)
		return false;

	/* lookup for entity definitions */
	if (axl_dtd_entity_exists (dtd, "TEST", GENERAL_ENTITY)) {
		axl_error_new (-1, "Expected to not find an entity value which was found (TEST)..", NULL, error);
		return false;
	}
	
	if (axl_dtd_entity_exists (dtd, "URI", GENERAL_ENTITY)) {
		axl_error_new (-1, "Expected to not find an entity value which was found (URI)..", NULL, error);
		return false;
	}
	
	/* lookup for entity definitions that are expected to be found */
	if (! axl_dtd_entity_exists (dtd, "URI", PARAMETER_ENTITY)) {
		axl_error_new (-1, "Expected to find an entity value which wasn't found (% URI)..", NULL, error);
		return false;
	}

	if (! axl_dtd_entity_exists (dtd, "LOCS", PARAMETER_ENTITY)) {
		axl_error_new (-1, "Expected to find an entity value which wasn' found (% LOCS)..", NULL, error);
		return false;
	}

	/* now get the content inside */
	content = axl_dtd_entity_value (dtd, "CHAN", PARAMETER_ENTITY);
	if (content == NULL) {
		axl_error_new (-1, "Expected to find parameter entity content for (% CHAN) but it wasn't found", NULL, error);
		return false;
	}
	
	if (! axl_cmp (content, "CDATA")) {
		axl_error_new (-1, "Expected to find parameter entity content for (% CHAN) it doesn't match", NULL, error);
		return false;	
	}

	/* free the dtd */
	axl_dtd_free (dtd);

	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_17 (axlError ** error) 
{
	axlDoc  * doc  = NULL;
	axlDtd  * dtd  = NULL;

	/* parse common DTD file */
	dtd = axl_dtd_parse_from_file ("xml-rpc.dtd", error);
	if (dtd == NULL)
		return false;

	/* parse a file that must not be valid */
	doc = axl_doc_parse_from_file ("test17.xdl", error);
	if (doc == NULL)
		return false;

	/* the following validation must fail */
	if (axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "A validation was produced when expected a failure", NULL, error);
		return false;
	}

	/* because a failure was expected, release memory allocated by
	 * axl_error_new */
	axl_error_free (*error);

	/* free the document */
	axl_doc_free (doc);

	/* release DTD reference */
	axl_dtd_free (dtd);

	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_16 (axlError ** error) 
{
	axlDoc  * doc  = NULL;
	axlDtd  * dtd  = NULL;

	/* parse common DTD file */
	dtd = axl_dtd_parse_from_file ("xml-rpc.dtd", error);
	if (dtd == NULL)
		return false;

	/* parse a file that must not be valid */
	doc = axl_doc_parse_from_file ("test03.xdl", error);
	if (doc == NULL)
		return false;

	/* the following validation must fail */
	if (axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "A validation was produced when expected a failure", NULL, error);
		return false;
	}

	/* because a failure was expected, release memory allocated by
	 * axl_error_new */
	axl_error_free (*error);

	/* free the document */
	axl_doc_free (doc);

	/* parse the next file that must be valid */
	doc = axl_doc_parse_from_file ("test04.xdl", error);
	if (doc == NULL)
		return false;

	/* the following validation should successed */
	if (! axl_dtd_validate (doc, dtd, error))
		return false;

	/* release the document */
	axl_doc_free (doc);
	
	/* release DTD reference */
	axl_dtd_free (dtd);

	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_15 (axlError ** error) 
{
	axlDoc  * doc  = NULL;
	axlDtd  * dtd  = NULL;

	/* parse common DTD file */
	dtd = axl_dtd_parse_from_file ("xml-rpc.dtd", error);
	if (dtd == NULL)
		return false;

	/* parse a file that must not be valid */
	doc = axl_doc_parse_from_file ("test01.xdl", error);
	if (doc == NULL)
		return false;

	/* the following validation must fail */
	if (axl_dtd_validate (doc, dtd, error)) {
		axl_error_new (-1, "A validation was produced when expected a failure", NULL, error);
		return false;
	}

	/* because a failure was expected, release memory allocated by
	 * axl_error_new */
	axl_error_free (*error);

	/* free the document */
	axl_doc_free (doc);

	/* parse the next file that must be valid */
	doc = axl_doc_parse_from_file ("test02.xdl", error);
	if (doc == NULL)
		return false;

	/* the following validation should successed */
	if (! axl_dtd_validate (doc, dtd, error))
		return false;

	/* release the document */
	axl_doc_free (doc);
	
	/* release DTD reference */
	axl_dtd_free (dtd);

	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_14 (axlError ** error) 
{
	axlDoc  * doc  = NULL;
	axlNode * node = NULL;
	
	char    * xml_document;
	char    * value;
	int       size = 0;
	int       document_size;
	

	/* create an emtpy document */
	doc = axl_doc_create ("1.0", NULL, false);

	/* create the root node */
	node = axl_node_create ("test");
	axl_node_set_content (node, "This is a test (') (\") (>) (<) (&), more data###", -1);
	
	axl_doc_set_root (doc, node);

	/* dump the document */
	axl_doc_dump (doc, &xml_document, &document_size);
	
	if (!axl_cmp ("<?xml version='1.0' ?><test>This is a test (&apos;) (&quot;) (&gt;) (&lt;) (&amp;), more data###</test>",
		      xml_document)) {
		axl_error_new (-1, "Found dump mismatch that shows entities are not handled properly", NULL, error);
		return false;
	}

	/* free memory dump */
	axl_free (xml_document);

	/* get the content translated */
	value = axl_node_get_content_copy (node, &size);

	if (size != 68) {
		axl_error_new (-1, "Found a document size mismatch while dumping entity content", NULL, error);
		return false;
	}
	
	/* free the content received */
	axl_free (value);

	/* get the content translated */
	value = axl_node_get_content_trans (node, &size);

	if (size != 48) {
		axl_error_new (-1, "Found a document size mismatch while dumping entity content (already translated)", NULL, error);
		return false;
	}

	/* check node content returned */
	if (!axl_cmp (value, "This is a test (\') (\") (>) (<) (&), more data###")) {
		axl_error_new (-1, "Found an string mismatch while checking a node content which was translated", NULL, error);
		return false;
	}

	/* free the content translated */
	axl_free (value);

	/* free document */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml version='1.0' ?><test></test>", 37, error);
	if (doc == NULL) {
		printf ("Expected to parse a document but it fails, error was: %s\n", axl_error_get (*error));
		return false;
	}

	/* get the content */
	node = axl_doc_get (doc, "/test");
	if (node == NULL) {
		axl_error_new (-1, "Expected to find a node reference not found (/test)\n", NULL, error);
		return false;		
	}

	/* get the content */
	size  = 11;
	value = (char*) axl_node_get_content (node, &size);
	if (size != 0) {
		axl_error_new (-1, "Expected to find a node content with 0 bytes but it 11 (/test)\n", NULL, error);
		return false;
	}

	/* get the content copy */
	size  = 13;
	value = axl_node_get_content_copy (node, &size);
	if (size != 0) {
		axl_error_new (-1, "Expected to find a node content with 0 bytes but it 13 (/test)\n", NULL, error);
		return false;
	}
	axl_free (value);

	/* get content copy trans */
	size  = 14;
	value = axl_node_get_content_trans (node, &size);
	if (size != 0) {
		axl_error_new (-1, "Expected to find a node content with 0 bytes but it 14 (/test)\n", NULL, error);
		return false;
	}
	axl_free (value);

	/* get content trimmed */
	size  = 15;
	value = axl_node_get_content_trim (node, &size);
	if (size != 0) {
		axl_error_new (-1, "Expected to find a node content with 0 bytes but it 15 (/test)\n", NULL, error);
		return false;
	}

	/* free the document */
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_13 (axlError ** error) 
{
	axlDoc  * doc  = NULL;
	axlDoc  * doc2 = NULL;
	axlDoc  * doc3 = NULL;
	
	axlNode * node = NULL;
	char    * content;
	int       size;
	
	doc = axl_doc_parse_from_file ("test13.xml", error);
	if (doc == NULL)
		return false;

	/* dump xml document */
	axl_doc_dump (doc, &content, &size);

	doc2 = axl_doc_parse (content, size, error);
	if (doc2 == NULL)
		return false;

	/* check if both documents are equals */
	if (! axl_doc_are_equal (doc, doc2)) {
		axl_error_new (-1, "Expected to dump an equivalent xml document, but found an error", NULL, error);
		return false;
	}

	/* free dump */
	axl_free (content);
	   
	/* free axl document */
	axl_doc_free (doc);

	/* free axl document */
	axl_doc_free (doc2);

	doc = axl_doc_parse_from_file ("test_13c.xml", error);
	if (doc == NULL)
		return false;

	if (! axl_doc_dump_pretty (doc, &content, &size, 4)) {
		axl_error_new (-1, "Failed to dump pretty print, while expected a proper execution", NULL, error);
		return false;
	}

	doc2 = axl_doc_parse (content, size, error);
	if (doc2 == NULL)
		return false;

	/* free content */
	axl_free (content);

	if (! axl_doc_are_equal (doc, doc2)) {
		axl_error_new (-1, "Expected to find documents to be equal.", NULL, error);
		return false;
	}

	/* free both document references */
	axl_doc_free (doc);
	axl_doc_free (doc2);

	/* check pretty printing function */
	doc = axl_doc_parse_from_file ("test_13b.xml", error);
	if (doc == NULL) 
		return false;

	if (! axl_doc_dump_pretty (doc, &content, &size, 4)) {
		axl_error_new (-1, "Failed to dump pretty print, while expected a proper execution", NULL, error);
		return false;
	}

	doc2 = axl_doc_parse (content, size, error);
	if (doc2 == NULL)
		return false;

	if (! axl_doc_are_equal (doc, doc2)) {
		axl_error_new (-1, "Expected to find documents to be equal.", NULL, error);
		return false;
	}

	/* free content */
	axl_free (content);

	/* free both document references */
	axl_doc_free (doc);
	axl_doc_free (doc2);

	/* reopen document to dump nodes */
	doc = axl_doc_parse_from_file ("test_13c.xml", error);
	if (doc == NULL)
		return false;

	/* get a reference to the first root child node: <test> */
	node = axl_doc_get_root (doc);
	node = axl_node_get_first_child (node);
	if (! NODE_CMP_NAME (node, "test")) {
		axl_error_new (-1, "Expected to find a child node called: <test>", NULL, error);
		return false;
	} /* end if */

	/* dump the content */
	if (! axl_node_dump (node, &content, &size)) {
		axl_error_new (-1, "Expected to find a proper dump operation", NULL, error);
		return false;
	} /* end if */

	/* parse the content dumped, to check it is really the result
	 * expected */
	doc2 = axl_doc_parse (content, size, NULL);
	if (doc2 == NULL) {
		axl_error_new (-1, "Expected to parse properly dumped content from a node, but a failure was found", NULL, error);
		return false;
	} /* end if */

	doc3 = axl_doc_parse_from_file ("test_13d.xml", NULL);
	if (doc3 == NULL) {
		axl_error_new (-1, "Expected to parse properly a reference file but an error was found", NULL, error);
		return false;
	} /* end if */

	/* check result */
	if (! axl_doc_are_equal (doc2, doc3)) {
		axl_error_new (-1, "Expected to find equal document to reference, at node dump operations, but not found", NULL, error);
		return false;
	} /* end if */

	/* free the content */
	axl_free (content);

	/* free the document */
	axl_doc_free (doc);
	axl_doc_free (doc2);
	axl_doc_free (doc3);
	
	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_12 (axlError ** error) 
{
	axlDoc * doc = NULL;
	axlDtd * dtd = NULL;

	/* parse gmovil file (an af-arch xml chunk) */
	doc = axl_doc_parse_from_file ("channel.xml", error); 
	if (doc == NULL) 
		return false;

	/* parse af-arch DTD */
	dtd = axl_dtd_parse_from_file ("channel.dtd", error);
	if (dtd == NULL)
		return false;

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return false;
	}

	/* free doc reference */
	axl_doc_free (doc); 

	/* parse gmovil file (an af-arch xml chunk) */
	doc = axl_doc_parse_from_file ("channel2.xml", error); 
	if (doc == NULL) 
		return false;

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return false;
	}

	/* free doc reference */
	axl_doc_free (doc); 

	/* parse gmovil file (an af-arch xml chunk) */
	doc = axl_doc_parse_from_file ("channel3.xml", error); 
	if (doc == NULL) 
		return false;

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return false;
	}

	/* free doc reference */
	axl_doc_free (doc); 

	/* parse gmovil file (an af-arch xml chunk) */
	doc = axl_doc_parse_from_file ("channel4.xml", error); 
	if (doc == NULL) 
		return false;

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return false;
	}

	/* free doc reference */
	axl_doc_free (doc); 
	
	/* free dtd reference */
	axl_dtd_free (dtd);

	/* parse a BEEP greetins example */
	doc = axl_doc_parse_from_file ("channel5.xml", error);
	if (doc == NULL)
		return false;

	/* parse the TLS dtd file */
	dtd = axl_dtd_parse_from_file ("channel.dtd", error);
	if (dtd == NULL)
		return false;

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return false;
	}

	/* free doc reference */
	axl_doc_free (doc); 
	
	/* free dtd reference */
	axl_dtd_free (dtd);
	

	/* test end */
	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_11 (axlError ** error) 
{
	axlDoc * doc = NULL;
	axlDtd * dtd = NULL;

	/* parse gmovil file (an af-arch xml chunk) */
	doc = axl_doc_parse_from_file ("gmovil2.xml", error); 
	if (doc == NULL) 
		return false;

	/* parse af-arch DTD */
	dtd = axl_dtd_parse_from_file ("fact.dtd", error);
	if (dtd == NULL)
		return false;

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return false;
	}

	/* free doc reference */
	axl_doc_free (doc); 
	
	/* free dtd reference */
	axl_dtd_free (dtd);

	/* test end */
	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_10 (axlError ** error) 
{
	axlDoc                * doc      = NULL;
	axlDtd                * dtd      = NULL;
	axlDtdElement         * element  = NULL;
	axlDtdElementList     * itemList = NULL;

	/* parse gmovil file (an af-arch xml chunk) */
	doc = axl_doc_parse_from_file ("test5.xml", error); 
	if (doc == NULL) 
		return false;

	/* parse af-arch DTD */
	dtd = axl_dtd_parse_from_file ("test5.dtd", error);
	if (dtd == NULL)
		return false;

	/* get the dtd element representation */
	element = axl_dtd_get_element (dtd, "choices");

	/* get the item list inside */
	itemList = axl_dtd_get_item_list (element);
	if (axl_dtd_item_list_count (itemList) != 4) {
		axl_error_new (-1, "expected to receive an item list with 4 item nodes inside", NULL, error);
		return false;
	}

	if (axl_dtd_item_list_type (itemList) != CHOICE) {
		axl_error_new (-1, "expected to receive a choice item list", NULL, error);
		return false;
	}

	if (axl_dtd_item_list_repeat (itemList) != ZERO_OR_MANY) {
		axl_log ("test-01", AXL_LEVEL_DEBUG, "received a different repeat configuration: %d != %d",
			 ZERO_OR_MANY, axl_dtd_item_list_repeat (itemList));
		axl_error_new (-1, "expected to receive an item list with (*) zero or many spec", NULL, error);
		return false;
	}
	

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) { 
		return false; 
	} 

	/* free dtd reference */
	axl_dtd_free (dtd);


	/* parse af-arch DTD */
	dtd = axl_dtd_parse_from_file ("test5.1.dtd", error);
	if (dtd == NULL)
		return false;
	
	/* perform DTD validation */
	if (axl_dtd_validate (doc, dtd, error)) { 
		axl_error_new (-1, "A validation failure was expected.", NULL, error);
		return false; 
	} 

	/* because a failure was expected, release memory allocated by
	 * axl_error_new */
	axl_error_free (*error);
	
	/* free doc reference */
	axl_doc_free (doc); 
	
	/* free dtd reference */
	axl_dtd_free (dtd);

	/* test end */
	return true;
}

/** 
 * @brief A more complex DTD parsing example
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_09 (axlError ** error) 
{
	axlDoc                * doc      = NULL;
	axlDtd                * dtd      = NULL;
	axlDtdElement         * element  = NULL;
	axlDtdElementList     * itemList = NULL;
	axlDtdElementListNode * itemNode = NULL;
	

	/* parse gmovil file (an af-arch xml chunk) */
	doc = axl_doc_parse_from_file ("test4.xml", error); 
	if (doc == NULL) 
		return false;

	/* parse af-arch DTD */
	dtd = axl_dtd_parse_from_file ("test4.dtd", error);
	if (dtd == NULL)
		return false;

	
	/* get dtd element */
	element = axl_dtd_get_element (dtd, "nodes");
	if (element == NULL) {
		axl_error_new (-1, "unable to find expected DTD element", NULL, error);
		return false;
	}

	/* get the item list */
	itemList = axl_dtd_get_item_list (element);
	if (axl_dtd_item_list_count (itemList) != 8) {
		axl_log ("test-01", AXL_LEVEL_CRITICAL, "found item list size: %d != 8",
			 axl_dtd_item_list_count (itemList));
		axl_error_new (-1, "expected to find an item list definition with 8 elements", NULL, error);
		return false;
	}

	/* check <first> node spec */
	itemNode = axl_dtd_item_list_get_node (itemList, 0);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_NODE) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "first")) {
		axl_error_new (-1, "expected to find an item node name (first) definition, not found", NULL, error);
		return false;
	}

	if (axl_dtd_item_node_get_repeat (itemNode) != ONE_OR_MANY) {
		axl_error_new (-1, "expected to find an item node definition with one or many repeat def (+), not found", NULL, error);
		return false;
	}

	/* check <second> node spec */
	itemNode = axl_dtd_item_list_get_node (itemList, 1);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_NODE) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "second")) {
		axl_error_new (-1, "expected to find an item node name (second) definition, not found", NULL, error);
		return false;
	}

	if (axl_dtd_item_node_get_repeat (itemNode) != ONE_AND_ONLY_ONE) {
		axl_error_new (-1, "expected to find an item node definition with one and only one repeat def (), not found", NULL, error);
		return false;
	}

	/* check <third> node spec */
	itemNode = axl_dtd_item_list_get_node (itemList, 2);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_NODE) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "third")) {
		axl_error_new (-1, "expected to find an item node name (third) definition, not found", NULL, error);
		return false;
	}

	if (axl_dtd_item_node_get_repeat (itemNode) != ZERO_OR_ONE) {
		axl_error_new (-1, "expected to find an item node definition with zero or one repeat def (?), not found", NULL, error);
		return false;
	}

	/* check <fourth> node spec */
	itemNode = axl_dtd_item_list_get_node (itemList, 3);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_NODE) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "fourth")) {
		axl_error_new (-1, "expected to find an item node name (fourth) definition, not found", NULL, error);
		return false;
	}

	if (axl_dtd_item_node_get_repeat (itemNode) != ONE_AND_ONLY_ONE) {
		axl_error_new (-1, "expected to find an item node definition with one and only one repeat def (), not found", NULL, error);
		return false;
	}

	/* check <fifth> node spec */
	itemNode = axl_dtd_item_list_get_node (itemList, 4);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_NODE) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "fifth")) {
		axl_error_new (-1, "expected to find an item node name (fifth) definition, not found", NULL, error);
		return false;
	}

	if (axl_dtd_item_node_get_repeat (itemNode) != ZERO_OR_MANY) {
		axl_error_new (-1, "expected to find an item node definition with zero to many repeat def (*), not found", NULL, error);
		return false;
	}

	/* check <fourth> node spec */
	itemNode = axl_dtd_item_list_get_node (itemList, 5);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_NODE) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "fourth")) {
		axl_error_new (-1, "expected to find an item node name (fourth) definition, not found", NULL, error);
		return false;
	}

	if (axl_dtd_item_node_get_repeat (itemNode) != ZERO_OR_MANY) {
		axl_error_new (-1, "expected to find an item node definition with zero to many repeat def (*), not found", NULL, error);
		return false;
	}

	/* check repetition pattern for node spec 6 */
	itemNode = axl_dtd_item_list_get_node (itemList, 6);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_LIST) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (axl_dtd_item_node_get_repeat (itemNode) != ZERO_OR_ONE) {
		axl_log ("test-01", AXL_LEVEL_DEBUG, "repeat configuration was different: %d != %d",
			 axl_dtd_item_node_get_repeat (itemNode), ZERO_OR_ONE);
		axl_error_new (-1, "expected to find an item node definition with zero or one repeat def (?), not found", NULL, error);
		return false;
	}

	/* check repetition pattern for node spec 7 */
	itemNode = axl_dtd_item_list_get_node (itemList, 7);
	if (axl_dtd_item_node_get_type (itemNode) != AXL_ELEMENT_LIST) {
		axl_error_new (-1, "expected to find an item node definition, not found", NULL, error);
		return false;
	}
	
	if (axl_dtd_item_node_get_repeat (itemNode) != ZERO_OR_MANY) {
		axl_log ("test-01", AXL_LEVEL_DEBUG, "repeat configuration was different: %d != %d",
			 axl_dtd_item_node_get_repeat (itemNode), ZERO_OR_MANY);
		axl_error_new (-1, "expected to find an item node definition with zero or one repeat def (*), not found", NULL, error);
		return false;
	}


	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, error)) { 
		return false; 
	} 

	/* free doc reference */
	axl_doc_free (doc); 
	
	/* free dtd reference */
	axl_dtd_free (dtd);

	/* test end */
	return true;
}

/** 
 * @brief Perform a loading for a large file.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_08 (axlError ** error)
{
	/* top level definitions */
	axlDoc            * doc = NULL;

	/* get current doc reference */
	doc = axl_doc_parse_from_file ("large.xml", error);
	if (doc == NULL)
		return false;

	/* release the document */
	axl_doc_free (doc);

	return true;
}


/** 
 * @brief A bit more complex DTD parsing checkings
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_07 (axlError ** error)
{
	/* top level definitions */
	axlDoc            * doc = NULL;
	axlDtd            * dtd = NULL;

	/* get current doc reference */
	doc = axl_doc_parse_from_file ("test3.xml", error);
	if (doc == NULL)
		return false;

	/* load DTD */
	dtd = axl_dtd_parse_from_file ("test3.dtd", error);
	if (dtd == NULL)
		return false;

	/* validate the xml document */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return  false;
	}
	
	/* free document */
	axl_doc_free (doc);

	/* free dtd document */
	axl_dtd_free (dtd);

	return true;
}

/** 
 * @brief A bit more complex DTD parsing checkings
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_06 (axlError ** error)
{
	/* top level definitions */
	axlDoc            * doc = NULL;
	axlDtd            * dtd = NULL;

	/* dtd iteration variables */
	axlDtdElement         * element;
	axlDtdElementList     * itemList;
	axlDtdElementListNode * itemNode;
	
	/* get current doc reference */
	doc = axl_doc_parse_from_file ("test3.xml", error);
	if (doc == NULL)
		return false;

	/* load DTD */
	dtd = axl_dtd_parse_from_file ("test3.dtd", error);
	if (dtd == NULL)
		return false;

	/* get the DTD element reference and check it */
	element = axl_dtd_get_root (dtd);
	if (element == NULL) {
		axl_error_new (-1, "Expected to receive a root DTD node, not found", NULL, error);
		return false;
	}

	/* check expected DTD root content */
	if (! axl_cmp (axl_dtd_get_element_name (element), "complex")) {
		axl_error_new (-1, "Expected to receive a root DTD node name, not found", NULL, error);
		return false;
	}

	if (axl_dtd_get_element_type (element) != ELEMENT_TYPE_CHILDREN) {
		axl_error_new (-1, "Expected to receive a root DTD node selection type (Children), not found", NULL, error);
		return false;
	}

	/* get content specification */
	itemList = axl_dtd_get_item_list (element);
	if (axl_dtd_item_list_count (itemList) != 1) {
		axl_log ("test-01", AXL_LEVEL_DEBUG, "item count %d != %d item spected",
			 axl_dtd_item_list_count (itemList), 1);
		axl_error_new (-1, "Expected to receive an item list specification with only one node, not found", NULL, error);
		return false;
	}

	/* get times configuration */
	if (axl_dtd_item_list_repeat (itemList) != ONE_AND_ONLY_ONE) {
		axl_error_new (-1, "Expected to receive a repetition configuration (one and only one) but not found", NULL, error);
		return false;
	}

	/* get the child node reference */
	itemNode = axl_dtd_item_list_get_node (itemList, 0);
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "data")) {
		axl_log ("test-01", AXL_LEVEL_CRITICAL, "found item name: '%s' != data",
			 axl_dtd_item_node_get_value (itemNode));
		axl_error_new (-1, "Expected to receive an item node but, not found", NULL, error);
		return false;
	}

	/* get the DTD element which represents the provided data */
	element = axl_dtd_get_element (dtd, "data");
	if ((element == NULL) || (!axl_cmp (axl_dtd_get_element_name (element), "data"))) {
		axl_error_new (-1, "Expected to receive a DTD element definition but NULL was found or a different DTD name, not found", NULL, error);
		return false;
	}
	
	/* get content specification */
	itemList = axl_dtd_get_item_list (element);
	if (axl_dtd_item_list_count (itemList) != 3) {
		axl_log ("test-01", AXL_LEVEL_DEBUG, "item count %d != %d item spected",
			 axl_dtd_item_list_count (itemList), 3);
		axl_error_new (-1, "Expected to receive an item list specification with only one node, not found", NULL, error);
		return false;
	}

	/* get item list especification */
	if (axl_dtd_item_list_type (itemList) != SEQUENCE) {
		axl_log ("test-01", AXL_LEVEL_DEBUG, "item count %d != %d item spected",
			 axl_dtd_item_list_count (itemList), 3);
		axl_error_new (-1, "Expected to receive an item list specification as a sequence type, not found", NULL, error);
		return false;
	}

	/* check item nodes found inside the item list */
	itemNode = axl_dtd_item_list_get_node (itemList, 0);
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "row")) {
		axl_error_new (-1, "Expected to receive an item node (row) but, not found", NULL, error);
		return false;
	}

	/* get the child node reference */
	itemNode = axl_dtd_item_list_get_node (itemList, 1);
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "column")) {
		axl_error_new (-1, "Expected to receive an item node (column) but, not found", NULL, error);
		return false;
	}

	/* get current configuration for repetition value for the
	 * provided content particule */
	if (axl_dtd_item_node_get_repeat (itemNode) != ONE_AND_ONLY_ONE) {
		axl_error_new (-1, "Expected to receive an item node repeat configuration but, not found", NULL, error);
		return false;
	}
		

	/* get the child node reference */
	itemNode = axl_dtd_item_list_get_node (itemList, 2);
	if (! axl_cmp (axl_dtd_item_node_get_value (itemNode), "value")) {
		axl_error_new (-1, "Expected to receive an item node (value) but, not found", NULL, error);
		return false;
	}

	/* now work with the choice element */
	element = axl_dtd_get_element (dtd, "column");
	if ((element == NULL) || (!axl_cmp (axl_dtd_get_element_name (element), "column"))) {
		axl_error_new (-1, "Expected to receive a DTD element definition but NULL was found or a different DTD name (column), not found", NULL, error);
		return false;
	}
	
	/* get content specification */
	itemList = axl_dtd_get_item_list (element);
	if (axl_dtd_item_list_count (itemList) != 4) {
		axl_log ("test-01", AXL_LEVEL_DEBUG, "item count %d != %d item spected",
			 axl_dtd_item_list_count (itemList), 4);
		axl_error_new (-1, "Expected to receive an item list specification with only one node, not found", NULL, error);
		return false;
	}

	/* get item list especification */
	if (axl_dtd_item_list_type (itemList) != CHOICE) {
		axl_error_new (-1, "Expected to receive an item list specification as a CHOICE type, not found", NULL, error);
		return false;
	}

	/* get the DTD element which represents the provided data */
	element = axl_dtd_get_element (dtd, "data");
	if ((element == NULL) || (!axl_cmp (axl_dtd_get_element_name (element), "data"))) {
		axl_error_new (-1, "Expected to receive a DTD element definition but NULL was found or a different DTD name, not found", NULL, error);
		return false;
	}


	/* free document */
	axl_doc_free (doc);

	/* free dtd document */
	axl_dtd_free (dtd);

	return true;
}

/** 
 * @brief DTD operations. Checks that DTD support works, reading
 * simple DTD definitions, and ensuring elements are properly read.
 * 
 * @param error The axlError reference to be filled if the function
 * returns false.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_05 (axlError ** error)
{

	axlDoc * doc;
	axlNode * node;
	axlDtd  * dtd;

	/* parse the document found */
	doc = axl_doc_parse_from_file ("test.xml", error);
	if (doc == NULL)
		return false;

	node = axl_doc_get (doc, "/complex/data/row/td");
	if (node == NULL) {
		axl_error_new (-1, "Expected to receive a node, not found", NULL, error);
		axl_doc_free (doc);
		return false;
	}

	if (! axl_cmp (axl_node_get_content (node, NULL), "10")) {
		axl_error_new (-1, "Expected to receive a node content, not found", NULL, error);
		axl_doc_free (doc);
		return false;
	}

	/* free previous document */
	axl_doc_free (doc);

	/* parse the document found */
	doc = axl_doc_parse_from_file ("test2.xml", error);
	if (doc == NULL)
		return false;
	
	
	dtd = axl_dtd_parse_from_file ("test.dtd", error);
	if (dtd == NULL)
		return false;

	/* now validate the document */
	if (! axl_dtd_validate (doc, dtd, error)) {
		return false;
	}

	/* release memory used by the parser */
	axl_doc_free (doc);
	
	/* release memory used by the DTD element */
	axl_dtd_free (dtd);

	return true;
	
}

/** 
 * @brief Perform some additional checks for a more extended xml
 * documents, that includes attributes inside node definitions.
 *
 * @param error The axlError where failures will be reported 
 *
 * @return The \ref true if test is passed, false if not.
 */
bool test_04 (axlError ** error)
{
	axlDoc  * doc;
	axlNode * node;

	/* parse the document */
	doc = axl_doc_parse_strings (error, 
				     "<?xml version='1.0' ?>",
				     "  <?test \"my content\" ?>",
				     "  <complex>",
				     "    <data>",
				     "       <row>",
				     "         <!-- A comment inside the middle of the document ",
				     " more comments ... ",
				     " more comments \n \r <td>..</td> -->",
				     "          <td>10</td>",
				     "          <test />",
				     " \n \n \r     <?test \"my content\" ?>     \n    \n",
				     "  <?test \"my content\" ?>",
				     "          <more>",
				     "              <test3 attr='2.0' />",
				     "          </more>",
				     "       </row>",
				     "       <test2 />",
				     "       <non-xml-document>",
				     "                             \n \r \n \t",
				     "         <![CDATA[<xml><<<<<<>>>>>><<<>>>><<<<<<>>>]]>"
				     "          \r \n \r \t",
				     "       </non-xml-document>",
				     "    </data>",
				     " <!--   <data>",
				     "       <row>",
				     "         A comment inside the middle of the document ",
				     " more comments ... ",
				     " more comments \n \r <td>..</td> ",
				     "          <td>10</td>",
				     "          <test />",
				     "          <more>",
				     "              <test2 attr='2.0' attr2='3.0' attr4='4.0'/>",
				     "          </more>",
				     "       </row>",
				     "       <test2 />",
				     "    </data> -->",
				     "  </complex>",
				     NULL);
	/* check the result returned */
	if (doc == NULL)
		return false;

	/* get the node <td> value */
	node = axl_doc_get (doc, "/complex/data/row/td");
	if (! axl_cmp (axl_node_get_content (node, NULL), "10")) {
		axl_log ("test-04", AXL_LEVEL_DEBUG, "found a different content than the expected ('10' != '%s')",
			 axl_node_get_content (node, NULL));
		axl_error_new (-1, "Expected to receive a 10 value, but not found", NULL, error);
		return false;
	}

	/* get a reference to the test2 node */
	node = axl_doc_get (doc, "/complex/data/row/more/test3");
	if (node == NULL) {
		axl_error_new (-1, "Expected to find a test3 node at the given location", NULL, error);
		return false;
	}

	/* check the attribute */
	if (! axl_node_has_attribute (node, "attr")) {
		axl_error_new (-1, "Expected to find an attribute called 'attr' inside test3 node at the given location", NULL, error);
		return false;
	}

	/* check the attribute value */
	if (! axl_cmp (axl_node_get_attribute_value (node, "attr"), "2.0")) {
		axl_error_new (-1, "Expected to find an attribute value equal '2.0' inside test2 node at the given location", NULL, error);
		return false;
	}

	/* add here Pi instruction checking */
	if (! axl_doc_has_pi_target (doc, "test")) {
		axl_error_new (-1, "failed to get expected PI target 'test'", NULL, error);
		return false;
	}

	if (! axl_cmp (axl_doc_get_pi_target_content (doc, "test"), "\"my content\"")) {
		axl_error_new (-1, "expected to receive a PI content not found", NULL, error);
		return false;
	}

	node = axl_doc_get (doc, "/complex/data/row");
	if (node == NULL) {
		axl_error_new (-1, "unable to get expected node to check node PI support", NULL, error);
		return false;
	}

	if (! axl_node_has_pi_target (node, "test")) {
		axl_error_new (-1, "failed to get expected PI target 'test' for the node", NULL, error);
		return false;
	}

	
	node = axl_doc_get (doc, "/complex/data/non-xml-document");
	if (node == NULL) {
		axl_error_new (-1, "Expected to receive the CDATA node, not found", NULL, error);
		return false;
	}

	if (! axl_cmp (axl_node_get_content (node, NULL), "<xml><<<<<<>>>>>><<<>>>><<<<<<>>>")) {
		printf ("Content doesn't match: %s != %s\n", 
			axl_node_get_content (node, NULL), "<xml><<<<<<>>>>>><<<>>>><<<<<<>>>");
		axl_error_new (-1, "Expected to recevie CDATA content, not found or not match", NULL, error);
		return false;
	}

				     
	/* free the memory */
	axl_doc_free (doc);
	
	return true;
}

/** 
 * @brief Checks a more extended and complex xml documents
 * 
 * @param error The axlError where failures will be reported.
 * 
 * @return \ref true if test is passed, false if not.
 */
bool test_03 (axlError ** error)
{

	axlDoc  * doc;
	axlNode * node;

	/* parse the xml document provided */
	doc = axl_doc_parse ("<?xml version='1.0' ?>\n\
<complex>\n\
  <data>\n\
    <row>10</row><row>20</row><row>30</row><row>40</row>\n\
  </data>\n\
  <data2>\n\
    <td> 23  </td>\n\
  </data2>\n\
</complex>", -1, error);
	if (doc == NULL)
		return false;

	/* get the root node */
	node = axl_doc_get_root (doc);
	if (! NODE_CMP_NAME (node, "complex")) {
		axl_error_new (-2, "Root node returned from the document is not the one excepted", NULL, error);
		axl_doc_free (doc);
		return false;
	}

	/* test get node function */
	node = axl_doc_get (doc, "/complex/data2/td");
	if (node == NULL) {
		axl_error_new (-2, "Unable to find a node due to a path selection", NULL, error);
		axl_doc_free (doc);
		return false;
	}

	/* check the node returned */
	if (! NODE_CMP_NAME (node, "td")) {
		axl_error_new (-2, "The node for the node looked up doesn't match ", NULL, error);
		axl_doc_free (doc);
		return false;
	}

	/* check for returning bad nodes */
	node = axl_doc_get (doc, "/complex/data3/td");
	if (node != NULL) {
		axl_error_new (-2, "Returned a node that should be NULL", NULL, error);
		axl_doc_free (doc);
		return false;
	}

	node = axl_doc_get (doc, "/complex/data2/td");
	if (!axl_cmp (axl_node_get_content (node, NULL), " 23  ")) {
		axl_log ("test-03", AXL_LEVEL_DEBUG, "expected to receive a node content: ' 23  ' but received '%s'",
			 axl_node_get_content (node, NULL));
		axl_error_new (-2, "Node content have failed, expected a different value", NULL, error);
		return false;
	}

	node = axl_doc_get (doc, "complex/data3/td");
	if (node != NULL) {
		axl_error_new (-2, "Parsed a path that is invalid", NULL, error);
		return false;
	}
	
	/* release memory allocated by the document */
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Checks xml document error for basic parsing.
 * 
 * @param error The axlError where failures are returned.
 * 
 * @return true if test are properly run. false if not.
 */
bool test_02 (axlError ** error) 
{
	axlDoc * doc;
	
	doc = axl_doc_parse ("<? xml >", 8, error);
	if (doc != NULL) {
		axl_error_new (-1, "Failed to detect wrong xml header", NULL, error);
		return false;
	}
	axl_error_free (* error);

	doc = axl_doc_parse ("<?xml >", 7, error);
	if (doc != NULL) {
		
		axl_error_new (-1, "Failed to detect wrong xml trailing header", NULL, error);
		return false;
	}
	axl_error_free (* error);


	return true;
}


/** 
 * @brief Parse an initial document, with an xml header with
 * version=1.0 and no more header.
 * 
 * 
 * @return false if the function fails to parse the
 * document. true if the test was properly executed.
 */
bool test_01 (axlError ** error) 
{
	/* axl document representation */
	axlDoc   * doc;
	

	/* parse the given string */
	doc = axl_doc_parse ("<?xml version='1.0' ?><axldoc />", 32, error);
	if (doc == NULL) {
		return false;
	}
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml      version='1.0'            ?>      <another />", 55, error);
	if (doc == NULL) {
		return false;
	}
	
	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml    \n   \t \n \r version='1.0' ?>    <doc />", 50, error);
	if (doc == NULL) {
		return false;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml  version=\"1.0\"        ?>   \r \t \n<another />", 54, error);
	if (doc == NULL) {
		return false;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml  version=\"1.0\" \t \n \r encoding='utf-8\"   ?> <data />", 63, error);
	if (doc == NULL) {
		return false;
	}

	if (strcmp ("utf-8", axl_doc_get_encoding (doc))) {
		printf ("ERROR: encoding read from the document differs from the expected (got %s, expected %s)!\n",
			axl_doc_get_encoding (doc), "utf-8");
		return false;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml version='1.0' encoding='utf-8' standalone='yes' ?>  <data/>", 65, error);
	if (doc == NULL) {
		return false;
	}

	if (!axl_doc_get_standalone (doc)) {
		printf ("ERROR: Expected to receive a true standalone configuration but false was found\n");
		return false;
	}

	/* release document parsed */
	axl_doc_free (doc);
	

	return true;
}

/** 
 * @brief Axl stream boundary checks.
 * 
 * 
 * @return false if the function fails to parse the
 * document. true if the test was properly executed.
 */
bool test_01a (axlError ** error) 
{
	axlStream * stream;
	char      * value;
	int         chunk_matched = -2;

	/* parse the string */
	stream = axl_stream_new ("customer", -1, NULL, -1, error);
	if (stream == NULL) 
		return false;

	/* get the value */
	value = axl_stream_get_until (stream, NULL, &chunk_matched, true, 2, "[", ".");
	if (value != NULL) {
		/* free the stream */
		axl_stream_free (stream);

		/* fill an error */
		axl_error_new (-1, "expected to find a null value while parsing content", NULL, error);
		
		return false;
	}

	if (chunk_matched != -1) {
		/* free the stream */
		axl_stream_free (stream);

		/* fill an error */
		axl_error_new (-1, "expected to chunk matched value equal to -1", NULL, error);
		
		return false;
	}

	/* parse the stream using zero support */

	/* get the value */
	value = axl_stream_get_until_zero (stream, NULL, &chunk_matched, true, 2, "[", ".");
	if (value == NULL) {
		/* free the stream */
		axl_stream_free (stream);

		/* fill an error */
		axl_error_new (-1, "expected to find a defined value while parsing content", NULL, error);
		
		return false;
	}

	if (chunk_matched != -2) {
		/* free the stream */
		axl_stream_free (stream);

		/* fill an error */
		axl_error_new (-1, "expected to chunk matched value equal to -2", NULL, error);
		
		return false;
	}

	/* zero string found (in the current stream) */
	if (! axl_cmp (value, "customer")) {
		printf ("Found values: '%s' != 'customer'\n", value);
		
		/* fill an error */
		axl_error_new (-1, "expected to find a string not found", NULL, error);

		return false;
	}

	/* free the stream */
	axl_stream_free (stream);

	/* parse the string */
	stream = axl_stream_new ("customer", -1, NULL, -1, error);
	if (stream == NULL) 
		return false;

	axl_stream_push (stream, "provider ", 9);
	if (! (axl_stream_peek (stream, "provider", 8) > 0)) {
		axl_error_new (-1, "failed to check expected input at the stream after push operation", NULL, error);
		return false;
	} /* end if */

	if (axl_stream_get_size (stream) != 17) {
		axl_error_new (-1, "Found unexpected stream buffer size, while expecting 17", NULL, error);
		return false;
	} /* end if */

	axl_stream_inspect (stream, "pro", 3);

	if (! (axl_stream_inspect (stream, "vider ", 6) > 0)) {
		axl_error_new (-1, "Expected to find an string value.. ('vider ') not found", NULL, error);
		return false;
	} /* end if */

	axl_stream_free (stream);

	return true;
}




bool test_01b_show_node_found (axlNode * node,
			       axlNode * parent,
			       axlDoc  * doc,
			       bool    * was_removed,
			       axlPointer ptr, 
			       axlPointer ptr2)
{
	int * iterator = ptr;

	/* show node found  */
	switch (*iterator) {
	case 0:
		/* <document> case */
		if (! NODE_CMP_NAME (node, "document")) {

			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <document>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 1:
		/* <child1> case */
		if (! NODE_CMP_NAME (node, "child1")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child1>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "document")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <document> for child1", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 2:
		/* <child2> case */
		if (! NODE_CMP_NAME (node, "child2")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child2>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "document")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <document> for child2", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 3:
		/* <child3> case */
		if (! NODE_CMP_NAME (node, "child3")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child3>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "document")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <document> for child3", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 4:
		/* <a> case */
		if (! NODE_CMP_NAME (node, "a")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <a>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "child3")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <child3> for a", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 5:
		/* <b> case */
		if (! NODE_CMP_NAME (node, "b")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <b>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "child3")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <child3> for b", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 6:
		/* <c> case */
		if (! NODE_CMP_NAME (node, "c")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <c>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "child3")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <child3> for c", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 7:
		/* <d> case */
		if (! NODE_CMP_NAME (node, "d")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <d>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "c")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <c> for d", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 8:
		/* <e> case */
		if (! NODE_CMP_NAME (node, "e")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <e>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "c")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <c> for e", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 9:
		/* <e> case */
		if (! NODE_CMP_NAME (node, "f")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <f>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "child3")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <child3> for f", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 10:
		/* <g> case */
		if (! NODE_CMP_NAME (node, "g")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <g>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "child3")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <chile3> for g", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	case 11:
		/* <child4> case */
		if (! NODE_CMP_NAME (node, "child4")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child4>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "document")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <document> for child4", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 12:
		/* <child5> case */
		if (! NODE_CMP_NAME (node, "child5")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child5>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		if (! NODE_CMP_NAME (parent, "document")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find parent node <document> for child4", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}

		break;
	}

	/* update count */
	(*iterator)++;

	/* keep iterating */
	return true;
}


bool test_01b_show_node_found2 (axlNode * node, 
				axlNode * parent,
				axlDoc  * doc,  
				bool    * was_removed,
				axlPointer ptr, axlPointer ptr2)
{
	int * iterator = ptr;

	/* show node found  */
	switch (*iterator) {
	case 0:
		/* <document> case */
		if (! NODE_CMP_NAME (node, "document")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <document>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 1:
		/* <child1> case */
		if (! NODE_CMP_NAME (node, "child1")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child1>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 2:
		/* <child2> case */
		if (! NODE_CMP_NAME (node, "child2")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child2>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 3:
		/* <child3> case */
		if (! NODE_CMP_NAME (node, "child3")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child3>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 4:
		/* <child4> case */
		if (! NODE_CMP_NAME (node, "child4")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child4>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 5:
		/* <child5> case */
		if (! NODE_CMP_NAME (node, "child5")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <child5>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 6:
		/* <a> case */
		if (! NODE_CMP_NAME (node, "a")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <a>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 7:
		/* <b> case */
		if (! NODE_CMP_NAME (node, "b")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <b>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 8:
		/* <c> case */
		if (! NODE_CMP_NAME (node, "c")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <c>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 9:
		/* <f> case */
		if (! NODE_CMP_NAME (node, "f")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <f>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 10:
		/* <g> case */
		if (! NODE_CMP_NAME (node, "g")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <g>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 11:
		/* <d> case */
		if (! NODE_CMP_NAME (node, "d")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <d>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	case 12:
		/* <e> case */
		if (! NODE_CMP_NAME (node, "e")) {
			/* fill the error */
			axl_error_new (-1, "Expected to find a document node not found <e>", NULL, (axlError **) ptr2);

			/* stop iterating */
			return false;
		}
		break;
	}

	/* update count */
	(*iterator)++;

	/* keep iterating */
	return true;
}




/** 
 * @brief Axl stream boundary checks.
 * 
 * 
 * @return false if the function fails to parse the
 * document. true if the test was properly executed.
 */
bool test_01b (axlError ** error)  
{
	axlDoc   * doc;
	axlNode  * node;
	int        test_01b_id;
	
	/* parse document */
	doc = axl_doc_parse_from_file ("test_01b.xml", error);
	if (doc == NULL) 
		return false;

	/* get document root */
	node = axl_doc_get_root (doc);
	if (! NODE_CMP_NAME (node, "document")) {
		axl_error_new (-1, "Expected to find root <document>, but it wasn't found", NULL, error);
		return false;
	}

	/* get first child node */
	node = axl_node_get_first_child (node);
	if (! NODE_CMP_NAME (node, "child1")) {
		axl_error_new (-1, "Expected to find child node <child1>, but it wasn't found", NULL, error);
		return false;
	}

	/* get sibling node */
	node = axl_node_get_next (node);
	if (! NODE_CMP_NAME (node, "child2")) {
		axl_error_new (-1, "Expected to find child node <child2>, but it wasn't found", NULL, error);
		return false;
	}

	/* now iterate over all nodes inside the document */
	test_01b_id = 0;
	if (! axl_doc_iterate_full (doc, DEEP_ITERATION, test_01b_show_node_found, &test_01b_id, error))
		return false;

	/* now iterate over all nodes inside (wide mode) the document */
	test_01b_id = 0;
	if (! axl_doc_iterate_full (doc, WIDE_ITERATION, test_01b_show_node_found2, &test_01b_id, error))
		return false; 

	/* test ok */
	axl_doc_free (doc);
	return true;
}

/** 
 * @brief Axl stream boundary checks.
 * 
 * 
 * @return false if the function fails to parse the
 * document. true if the test was properly executed.
 */
bool test_01c (axlError ** error)  
{
	axlDoc   * doc;
	axlNode  * node;
	axlNode  * parent;
	
	/* parse document (uses the same xml document as test_01b) */
	doc = axl_doc_parse_from_file ("test_01b.xml", error);
	if (doc == NULL) 
		return false;

	/* get document root */
	node = axl_doc_get_root (doc);
	if (! NODE_CMP_NAME (node, "document")) {
		axl_error_new (-1, "Expected to find root <document>, but it wasn't found", NULL, error);
		return false;
	}

	/* get first child */
	node = axl_node_get_first_child (node);
	
	if (! NODE_CMP_NAME (node, "child1")) {
		axl_error_new (-1, "Expected to find <child1>, but it wasn't found", NULL, error);
		return false;
	}

	/* check parent */
	parent = axl_node_get_parent (node);
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}
	
	node = axl_node_get_next (node);
	if (! NODE_CMP_NAME (node, "child2")) {
		axl_error_new (-1, "Expected to find <child2>, but it wasn't found", NULL, error);
		return false;
	}

	/* check parent */
	parent = axl_node_get_parent (node);
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_next (node);
	if (! NODE_CMP_NAME (node, "child3")) {
		axl_error_new (-1, "Expected to find <child3>, but it wasn't found", NULL, error);
		return false;
	}
	/* check parent */
	parent = axl_node_get_parent (node);
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_next (node);
	if (! NODE_CMP_NAME (node, "child4")) {
		axl_error_new (-1, "Expected to find <child4>, but it wasn't found", NULL, error);
		return false;
	}
	/* check parent */
	parent = axl_node_get_parent (node);
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_next (node);
	if (! NODE_CMP_NAME (node, "child5")) {
		axl_error_new (-1, "Expected to find <child5>, but it wasn't found", NULL, error);
		return false;
	}
	/* check parent */
	parent = axl_node_get_parent (node);
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	if (axl_node_get_next (node) != NULL) {
		axl_error_new (-1, "Expected to find NULL value while calling to axl_node_get_next, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_previous (node);
	if (! NODE_CMP_NAME (node, "child4")) {
		axl_error_new (-1, "Expected to find <child4>, but it wasn't found", NULL, error);
		return false;
	}
	/* check parent */
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_previous (node);
	if (! NODE_CMP_NAME (node, "child3")) {
		axl_error_new (-1, "Expected to find <child3>, but it wasn't found", NULL, error);
		return false;
	}
	/* check parent */
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_previous (node);
	if (! NODE_CMP_NAME (node, "child2")) {
		axl_error_new (-1, "Expected to find <child2>, but it wasn't found", NULL, error);
		return false;
	}
	/* check parent */
 	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	node = axl_node_get_previous (node);
	if (! NODE_CMP_NAME (node, "child1")) {
		axl_error_new (-1, "Expected to find <child1>, but it wasn't found", NULL, error);
		return false;
	}
	/* check parent */
	parent = axl_node_get_parent (node);
	if (! NODE_CMP_NAME (parent, "document")) {
		axl_error_new (-1, "Expected to find parent <document>, but it wasn't found", NULL, error);
		return false;
	}

	if (axl_node_get_previous (node) != NULL) {
		axl_error_new (-1, "Expected to find NULL value while calling to axl_node_get_next, but it wasn't found", NULL, error);
		return false;
	}

	/* check next called and previous called api */
	node = axl_doc_get_root (doc);
	node = axl_node_get_first_child (node);
	

	/* get <child5> */
	node = axl_node_get_next_called (node, "child5");
	if (! NODE_CMP_NAME (node, "child5")) {
		axl_error_new (-1, "Expected to find <child5> node while calling to axl_node_get_next_called, but it wasn't found", NULL, error);
		return false;
	}
	
	/* check next empty */
	if (axl_node_get_next_called (node, "child5") != NULL) {
		axl_error_new (-1, "Expected to find empty node following to <child5> node while calling to axl_node_get_next_called, but it was found", NULL, error);
		return false;
	}

	/* get <child1> */
	node = axl_node_get_previous_called (node, "child1");
	if (! NODE_CMP_NAME (node, "child1")) {
		axl_error_new (-1, "Expected to find <child1> node while calling to axl_node_get_previous_called, but it wasn't found", NULL, error);
		return false;
	}

	/* check next empty */
	if (axl_node_get_previous_called (node, "child1") != NULL) {
		axl_error_new (-1, "Expected to find empty node following to <child1> node while calling to axl_node_get_previous_called, but it was found", NULL, error);
		return false;
	}

	/* free document */
	axl_doc_free (doc);

	return true;
}

/** 
 * @brief Axl node nth access.
 * 
 * @return false if the function fails to parse the
 * document. true if the test was properly executed.
 */
bool test_01d (axlError ** error)  
{
	axlDoc   * doc;
	axlNode  * node;
	axlNode  * child;
	
	/* parse document */
	doc = axl_doc_parse_from_file ("test_01d.xml", error);
	if (doc == NULL) 
		return false;

	/* get document root */
	node = axl_doc_get_root (doc);
	
	/* get the node at the 0 position */
	child = axl_node_get_child_nth (node, 0);
	if (! NODE_CMP_NAME (child, "child1")) {
		axl_error_new (-1, "Expected to find node <child1> at 0, but it wasn't found", NULL, error);
		return false;
	}

	/* get the node at the 1 position */
	child = axl_node_get_child_nth (node, 1);
	if (! NODE_CMP_NAME (child, "child2")) {
		axl_error_new (-1, "Expected to find node <child2> at 1, but it wasn't found", NULL, error);
		return false;
	}

	/* get the node at the 2 position */
	child = axl_node_get_child_nth (node, 2);
	if (! NODE_CMP_NAME (child, "child3")) {
		axl_error_new (-1, "Expected to find node <child3> at 2, but it wasn't found", NULL, error);
		return false;
	}

	/* get the node at the 3 position */
	child = axl_node_get_child_nth (node, 3);
	if (! NODE_CMP_NAME (child, "child4")) {
		axl_error_new (-1, "Expected to find node <child4> at 3, but it wasn't found", NULL, error);
		return false;
	}

	/* get the node at the 4 position */
	child = axl_node_get_child_nth (node, 4);
	if (! NODE_CMP_NAME (child, "child5")) {
		axl_error_new (-1, "Expected to find node <child5> at 4, but it wasn't found", NULL, error);
		return false;
	}

	/* now with <child3> childs *> */
	node = axl_node_get_child_nth (node, 2);
	
	/* get the node at the 0 position */
	child = axl_node_get_child_nth (node, 0);
	if (! NODE_CMP_NAME (child, "a")) {
		axl_error_new (-1, "Expected to find node <a> at 0, but it wasn't found", NULL, error);
		return false;
	}

	/* get the node at the 1 position */
	child = axl_node_get_child_nth (node, 1);
	if (! NODE_CMP_NAME (child, "b")) {
		axl_error_new (-1, "Expected to find node <b> at 1, but it wasn't found", NULL, error);
		return false;
	}
	   
	/* get the node at the 2 position */
	child = axl_node_get_child_nth (node, 2);
	if (! NODE_CMP_NAME (child, "c")) {
		axl_error_new (-1, "Expected to find node <c> at 2, but it wasn't found", NULL, error);
		return false;
	}

	/* get the node at the 3 position */
	child = axl_node_get_child_nth (node, 3);
	if (! NODE_CMP_NAME (child, "f")) {
		axl_error_new (-1, "Expected to find node <f> at 3, but it wasn't found", NULL, error);
		return false;
	}

	/* get the node at the 4 position */
	child = axl_node_get_child_nth (node, 4);
	if (! NODE_CMP_NAME (child, "g")) {
		axl_error_new (-1, "Expected to find node <g> at 4, but it wasn't found", NULL, error);
		return false;
	}

	/* test ok */
	axl_doc_free (doc);
	return true;
}

/** 
 * @brief Test current libaxl list implementation.
 * 
 * 
 * @return true if it works properly or false if not.
 */
bool test_01_01 () 
{
	axlList * list;

	/* create the list */
	list = axl_list_new (axl_list_equal_string, NULL);
	if (axl_list_length (list) != 0) {
		printf ("Bad length returned by the list (%d != 0)\n",
			axl_list_length (list));
		return false;
	}

	if (! axl_list_is_empty (list)) {
		printf ("Expected to find empty list just after list created\n");
		return false;
	} /* end if */
	
	/* add data */
	axl_list_add (list, "test 1");
	if (axl_list_length (list) != 1) {
		printf ("Bad length returned by the list (%d != 1)\n",
			axl_list_length (list));
		return false;
	}

	if (axl_list_is_empty (list)) {
		printf ("Expected to find a non-empty list just after items added\n");
		return false;
	} /* end if */
	
	axl_list_add (list, "test 2");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list (%d != 2)\n",
			axl_list_length (list));
		return false;
	}

	axl_list_add (list, "test 3");
	if (axl_list_length (list) != 3) {
		printf ("Bad length returned by the list (%d != 3)\n",
			axl_list_length (list));
		return false;
	}

	/* remove data */
	axl_list_remove (list, "test 1");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work (%d != 2)\n",
			axl_list_length (list));
		return false;
	}

	axl_list_remove (list, "test 2");
	if (axl_list_length (list) != 1) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}

	axl_list_remove (list, "test 3");
	if (axl_list_length (list) != 0) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}

	/* add data again */
	axl_list_add (list, "test 4");
	axl_list_add (list, "test 5");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}
	
	axl_list_remove (list, "test 1");
	if (axl_list_length (list) != 2) {
		printf ("Bad length returned by the list, seems that remove doesn't work\n");
		return false;
	}

	if (! axl_list_exists (list, "test 5")) {
		printf ("Exist function have failed\n");
		return false;
	}

	if (! axl_list_exists (list, "test 4")) {
		printf ("Exist function have failed\n");
		return false;
	}
	
	if (axl_list_exists (list, "test 1")) {
		printf ("Exist function have failed\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 4", 0)) {
		printf ("\"Exists at\" functionality seems to not work (0)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 5", 1)) {
		printf ("\"Exists at\" functionality seems to not work (1)\n");
		return false;
	}

	axl_list_free (list);

	/* create a new list */
	list = axl_list_new (axl_list_equal_string, NULL);

	axl_list_add (list, "test 1");
	axl_list_add (list, "test 3");
	axl_list_add (list, "test 5");

	axl_list_add_at (list, "test 0", 0);
	axl_list_add_at (list, "test 2", 2);
	axl_list_add_at (list, "test 4", 4);
	axl_list_add_at (list, "test 6", 6);

	if (! axl_list_exists_at (list, "test 0", 0)) {
		printf ("\"Exists at\" functionality seems to not work (0)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 1", 1)) {
		printf ("\"Exists at\" functionality seems to not work (1)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 2", 2)) {
		printf ("\"Exists at\" functionality seems to not work (2)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 3", 3)) {
		printf ("\"Exists at\" functionality seems to not work (3)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 4", 4)) {
		printf ("\"Exists at\" functionality seems to not work (4)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 5", 5)) {
		printf ("\"Exists at\" functionality seems to not work (5)\n");
		return false;
	}

	if (! axl_list_exists_at (list, "test 6", 6)) {
		printf ("\"Exists at\" functionality seems to not work (6)\n");
		return false;
	}

	/* free the list */
	axl_list_free (list);

	/* create an integer list */
	list = axl_list_new (axl_list_equal_int, NULL);
	axl_list_add (list, INT_TO_PTR (1));
	axl_list_add (list, INT_TO_PTR (3));
	axl_list_add (list, INT_TO_PTR (3900));
	axl_list_add (list, INT_TO_PTR (20230));

	if (axl_list_length (list) != 4) {
		printf ("Expected to find 4 items inside an integer list");
		return false;
	}

	/* remove one item */
	axl_list_remove (list, INT_TO_PTR (1));

	if (axl_list_length (list) != 3) {
		printf ("Expected to find 3 items inside an integer list");
		return false;
	}

	/* remove one item */
	axl_list_remove (list, INT_TO_PTR (1));

	if (axl_list_length (list) != 3) {
		printf ("Expected to find 3 items inside an integer list");
		return false;
	}

	/* remove one item */
	axl_list_remove (list, INT_TO_PTR (3));

	if (axl_list_length (list) != 2) {
		printf ("Expected to find 2 items inside an integer list");
		return false;
	}

	/* remove one item */
	axl_list_remove (list, INT_TO_PTR (3900));

	if (axl_list_length (list) != 1) {
		printf ("Expected to find 1 items inside an integer list");
		return false;
	}

	/* remove one item */
	axl_list_remove (list, INT_TO_PTR (20230));

	if (axl_list_length (list) != 0) {
		printf ("Expected to find 0 items inside an integer list");
		return false;
	}
	axl_list_free (list);

	/* create an integer list */
	list = axl_list_new (axl_list_equal_int, NULL);

	/* add items */
	axl_list_add (list, INT_TO_PTR (8));
	axl_list_add (list, INT_TO_PTR (10));
	axl_list_add (list, INT_TO_PTR (11));
	axl_list_add (list, INT_TO_PTR (12));
	axl_list_add (list, INT_TO_PTR (102));
	axl_list_add (list, INT_TO_PTR (1123));
	axl_list_add (list, INT_TO_PTR (412));
	axl_list_add (list, INT_TO_PTR (122));
	axl_list_add (list, INT_TO_PTR (148));
	axl_list_add (list, INT_TO_PTR (670));
	axl_list_add (list, INT_TO_PTR (2));
	
	if (axl_list_length (list) != 11) {
		printf ("Expected to find 11 element, but found: %d\n", axl_list_length (list));
		return false;
	}

	/* remove the third element */
	axl_list_remove (list, INT_TO_PTR (11));
	
	if (axl_list_length (list) != 10) {
		printf ("Expected to find 10 element, but found: %d\n", axl_list_length (list));
		return false;
	}

	/* remove the third element */
	axl_list_remove (list, INT_TO_PTR (122));
	
	if (axl_list_length (list) != 9) {
		printf ("Expected to find 9 element, but found: %d\n", axl_list_length (list));
		return false;
	}

	/* free the list */
	axl_list_free (list);
	
	list = axl_list_new (axl_list_equal_int, NULL);

	axl_list_append (list, INT_TO_PTR (9));
	axl_list_append (list, INT_TO_PTR (10));
	axl_list_append (list, INT_TO_PTR (8));
	axl_list_append (list, INT_TO_PTR (11));
	
	if (axl_list_length (list) != 4) {
		printf ("Expected to find 4 element, but found: %d\n", axl_list_length (list));
		return false;
	}

	/* remove */
	axl_list_remove (list, INT_TO_PTR(8));
	
	if (axl_list_length (list) != 3) {
		printf ("Expected to find 3 element, but found: %d\n", axl_list_length (list));
		return false;
	}
	
	/* free the list */
	axl_list_free (list);

	list = axl_list_new (axl_list_equal_int, NULL);

	axl_list_append (list, INT_TO_PTR (8));
	axl_list_append (list, INT_TO_PTR (10));
	axl_list_append (list, INT_TO_PTR (11));
	axl_list_append (list, INT_TO_PTR (9));
	
	if (axl_list_length (list) != 4) {
		printf ("Expected to find 4 element, but found: %d\n", axl_list_length (list));
		return false;
	}

	axl_list_remove (list, INT_TO_PTR(10));

	if (axl_list_length (list) != 3) {
		printf ("Expected to find 3 element, but found: %d\n", axl_list_length (list));
		return false;
	}

	/* free the list */
	axl_list_free (list);

	list = axl_list_new (axl_list_equal_int, NULL);

	axl_list_append (list, INT_TO_PTR (9));
	axl_list_append (list, INT_TO_PTR (8));
	axl_list_append (list, INT_TO_PTR (10));
	axl_list_append (list, INT_TO_PTR (11));
	
	if (axl_list_length (list) != 4) {
		printf ("Expected to find 4 element, but found: %d\n", axl_list_length (list));
		return false;
	}

	/* remove */
	axl_list_remove (list, INT_TO_PTR(8));

	/* remove */
	axl_list_remove (list, INT_TO_PTR(10));

	if (axl_list_length (list) != 2) {
		printf ("Expected to find 2 element, but found: %d\n", axl_list_length (list));
		return false;
	}
	
	/* free the list */
	axl_list_free (list);

	return true;
}

bool test_01_02_foreach (axlPointer stack_data, 
		      axlPointer user_data, 
		      axlPointer user_data2)
{
	int * iterator = user_data;

	if ((*iterator == 0) && axl_cmp ((char*) stack_data, "test 3")) {
		(*iterator)++;
		return false;
	} else if ((*iterator == 1) && axl_cmp ((char*) stack_data, "test 2")) {
		(*iterator)++;
		return false;
	} else if ((*iterator == 2) && axl_cmp ((char*) stack_data, "test 1")) {
		(*iterator)++;
		return false;
	}
	
	/* return true to stop operations */
	return true;
}

/** 
 * @brief Checks current configuration for the stack implementation
 * based on the axlList.
 * 
 * 
 * @return true if the stack works properly, otherwise false
 * is returned.
 */
bool test_01_02 () 
{
	axlStack * stack;
	char     * value;
	int        iterator = 0;

	/* create the stack */
	stack = axl_stack_new (NULL);

	axl_stack_push (stack, "test 1");
	axl_stack_push (stack, "test 2");
	axl_stack_push (stack, "test 3");

	/* check foreach function */
	axl_stack_foreach (stack, test_01_02_foreach, &iterator, NULL);

	if (iterator != 3) {
		printf ("Wrong value expected while using the foreach function\n");
		return false;
	}

	if (axl_stack_size (stack) != 3) {
		printf ("Wrong stack size expected ..\n");
		return false;
	}
	
	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 3", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 3");
		return false;
	}

	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 2", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 2");
		return false;
	}

	value = axl_stack_pop (stack);
	if (! axl_stream_cmp (value, "test 1", 6)) {
		printf ("Wrong pop value returned (%s != %s)..\n", value, "test 1");
		return false;
	}

	if (axl_stack_size (stack) != 0) {
		printf ("Wrong stack size before operating..\n");
		return false;
	}

	if (! axl_stack_is_empty (stack)) {
		printf ("Wrong stack emptyness value received..\n");
		return false;
	}

	/* destroy the stack */
	axl_stack_free (stack);

	return true;
}

bool test_01_03_vargs (const char * format, ...)
{
	va_list args;

	va_start (args, format);

	/* get the result */
	if (axl_stream_vprintf_len (format, args) != 8)
		return false;

	if (axl_stream_vprintf_len (format, args) != 8)
		return false;

	if (axl_stream_vprintf_len (format, args) != 8)
		return false;
	
	va_end (args);

	return true;
}

/** 
 *
 * @brief Checks some internal functions that the library provides to
 * manage strings.
 *
 * @return The function must return true if everything is
 * ok. Otherwise false is returned.
 */
bool test_01_03 () 
{
	char  * string;
	char ** result;
	int     res;
	int     trimmed;

	/* check that axl_stream_split works */
	result = axl_stream_split ("value/value1/value3/value4", 1, "/");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions\n");
		return false;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d)\n", 
			axl_stream_strv_num (result),4);
		return false;
	}
	
	if (! axl_cmp (result[0], "value")) {
		printf ("Failed to get the first element: (%s != %s)\n", result[0], "value");
		return false;
	}

	if (! axl_cmp (result[1], "value1")) {
		printf ("Failed to get the second element: (%s != %s)\n", result[1], "value1");
		return false;
	}

	if (! axl_cmp (result[2], "value3")) {
		printf ("Failed to get the third element (%s != %s)\n", result[2], "value3");
		return false;
	}

	if (! axl_cmp (result[3], "value4")) {
		printf ("Failed to get the fourth element (%s != %s)\n", result[3], "value4");
		return false;
	}

	/* release memory used */
	axl_stream_freev (result);

	result = axl_stream_split ("value1, value2/ value3* ", 3, ", ", "/ ", "* ");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions (2)\n");
		return false;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d) (2)\n", 
			axl_stream_strv_num (result), 4);
		return false;
	}

	if (! axl_cmp (result[0], "value1")) {
		printf ("Failed to get the second element: (%s != %s)\n", result[0], "value1");
		return false;
	}

	if (! axl_cmp (result[1], "value2")) {
		printf ("Failed to get the third element (%s != %s)\n", result[1], "value2");
		return false;
	}

	if (! axl_cmp (result[2], "value3")) {
		printf ("Failed to get the fourth element (%s != %s)\n", result[2], "value3");
		return false;
	}

	if (! axl_cmp (result[3], "")) {
		printf ("Failed to get the fourth element ('%s' != '%s')\n", result[3], "");
		return false;
	}

	

	/* release memory used */
	axl_stream_freev (result);

	/* check empty cases */
	result = axl_stream_split ("///", 1, "/");
	if (result == NULL) {
		printf ("Something have failed while using splitting functions (3)\n");
		return false;
	}

	if (axl_stream_strv_num (result) != 4) {
		printf ("Something have failed while getting current number of pieces inside the split result (%d != %d) (3)\n", 
			axl_stream_strv_num (result), 4);
		return false;
	}

	/* release memory used */
	axl_stream_freev (result);

	string = axl_strdup ("AxBcA");
	if (! axl_cmp (axl_stream_to_upper (string), "AXBCA")) {
		/* report error found */
		printf ("failed to upper case letters\n");
		return false;
	}
	axl_free (string);

	string = axl_strdup ("AxBcA");
	if (! axl_cmp (axl_stream_to_lower (string), "axbca")) {
		/* report error found */
		printf ("failed to lower case letters, result found: %s != %s\n", string, "axbca");
		return false;
	}
	axl_free (string);


	string = axl_strdup ("\t \n   \r  value  \n \r \t \t  ");
	axl_stream_trim (string);

	if (! axl_cmp (string, "value")) {
		printf ("failed to trim the string received, expected '%s' == '%s'\n", 
			string, "value");
		return false;
	}

	/* release memory */
	axl_free (string);

	string = axl_strdup ("   value    ");
	axl_stream_trim_with_size (string, &trimmed);
	if (trimmed != 7) {
		printf ("failed, expected to find a trimmed size of 7 but found %d (%s)\n", 
			trimmed, string);
		return false;
	}

	/* check axl_stream_vprintf_len implementation */
	if (axl_stream_vprintf_len (NULL, NULL) != 0) {
		printf ("failed, expected to find an string value of 0, but it wasn't found\n");
		return false;
	} /* end if */

	if (axl_stream_vprintf_len (" this is a test", NULL) != 16) {
		printf ("failed, expected to find an string value of 16, but it wasn't found\n");
		return false;
	}

	if (axl_stream_vprintf_len (" this %% is a %% test", NULL) != 20) {
		printf ("failed, expected to find an string value of 20, but it wasn't found\n");
		return false;
	}

	if (axl_stream_vprintf_len ("", NULL) != 1) {
		printf ("failed, expected to find an string value of 1, but it wasn't found\n");
		return false;
	}

	if (axl_stream_vprintf_len ("%%", NULL) != 2) {
		printf ("failed, expected to find an string value of 2, but it wasn't found\n");
		return false;
	}

	if (axl_stream_vprintf_len ("\"", NULL) != 2) {
		printf ("failed, expected to find an string value of 2, but it wasn't found\n");
		return false;
	}

	if (axl_stream_vprintf_len ("\"", NULL) != 2) {
		printf ("failed, expected to find an string value of 2, but it wasn't found\n");
		return false;
	}

	if (axl_stream_vprintf_len (" this is a test \r \t \n  asdf", NULL) != 28) {
		printf ("failed, expected to find an string value of 28, but it wasn't found\n");
		return false;
	} /* end if */

	res = axl_stream_printf_len ("%s", "This is a test");
	if ( res != 15) {
		printf ("failed, expected to find an string value of 15, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len (" adfasdf %s asdfasdf", "This is a test");
	if ( res != 33) {
		printf ("failed, expected to find an string value of 33, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len (" adfasdf %s asdfasdf %s", "This is a test", "fk2");
	if ( res != 37) {
		printf ("failed, expected to find an string value of 37, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%% adfasdf %s asdfasdf %s", "This is a test", "fk2");
	if ( res != 38) {
		printf ("failed, expected to find an string value of 38, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%d", 1);
	if ( res != 2) {
		printf ("failed, expected to find an string value of 2, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%d", 10);
	if ( res != 3) {
		printf ("failed, expected to find an string value of 3, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%d", -1);
	if ( res != 3) {
		printf ("failed, expected to find an string value of 3, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%d", -10);
	if ( res != 4) {
		printf ("failed, expected to find an string value of 4, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%d", -100);
	if ( res != 5) {
		printf ("failed, expected to find an string value of 5, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%d", -10012);
	if ( res != 7) {
		printf ("failed, expected to find an string value of 7, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("This is a number %d", -10012);
	if ( res != 24) {
		printf ("failed, expected to find an string value of 24, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("This is a number %d with content ", -10012);
	if ( res != 38) {
		printf ("failed, expected to find an string value of 38, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("This is a number %d with content %s", -10012, "This more content");
	if ( res != 55) {
		printf ("failed, expected to find an string value of 55, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%c", 1);
	if ( res != 2) {
		printf ("failed, expected to find an string value of 2, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len (" %c ", 1);
	if ( res != 4) {
		printf ("failed, expected to find an string value of 4, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("\x0D");
	if ( res != 2) {
		printf ("failed, expected to find an string value of 2, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("\x0D\x0A");
	if ( res != 3) {
		printf ("failed, expected to find an string value of 3, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%ld", 182);
	if ( res != 4) {
		printf ("failed, expected to find an string value of 4, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%lu", (unsigned long int) 182);
	if ( res != 4) {
		printf ("failed, expected to find an string value of 4, but it was found (%d)\n", res);
		return false;
	}

	res = axl_stream_printf_len ("%6d", 182);
	if ( res != 7) {
		printf ("failed (%%6d), expected to find an string value of 7, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%f", 182.0);
	if ( res != 11) {
		printf ("failed (%%f,182), expected to find an string value of 11, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%.2f", 18228.0);
	if ( res != 9) {
		printf ("failed (%%.2f), expected to find an string value of 7, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%8.2f", 182);
	if ( res != 9) {
		printf ("failed (%%8.2f), expected to find an string value of 12, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%.5f", 182.10);
	if ( res != 10) {
		printf ("failed (%%.5f), expected to find an string value of 10, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%g", (double) 182.23);
	if ( res != 7) {
		printf ("failed (%%g,182.23), expected to find an string value of 7, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%g", 182.39);
	if ( res != 7) {
		printf ("failed (%%g,182.39), expected to find an string value of 7, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%g", 182.1);
	if ( res != 6) {
		printf ("failed (%%g,182.1), expected to find an string value of 6, but it was found (%d)\n", res);
		return false;
	} 

	res = axl_stream_printf_len ("%g", 182.102);
	if ( res != 8) {
		printf ("failed (%%g,182.1), expected to find an string value of 6, but it was found (%d)\n", res);
		return false;
	} 

	if (! test_01_03_vargs ("%g", 182.102)) {
		printf ("failed to reuse vargs..\n");
		return false;
	}
	

	/* release the memory */
	axl_free (string);
	
	/* case cmp comparisions */
	if (! axl_stream_casecmp ("Content-Type: ", "Content-Type: ", 14)) {
		printf ("Expected to find equal comparision for case insensitive check..\n");
		return false;
	}
	
	if (! axl_stream_casecmp ("CONTENT-Type: ", "Content-Type: ", 14)) {
		printf ("Expected to find equal comparision for case insensitive check..\n");
		return false;
	}

	if (! axl_stream_casecmp ("CONTENT-Type: ", "Content-TYPE: ", 14)) {
		printf ("Expected to find equal comparision for case insensitive check..\n");
		return false;
	}

	if (! axl_stream_casecmp ("CoNtENT-type: ", "Content-TYPE: ", 14)) {
		printf ("Expected to find equal comparision for case insensitive check..\n");
		return false;
	}

	return true;
}

/** 
 * @brief Intensive axl list implementation.
 */
bool test_01_04 () {
	int             iterator = 0;
	int             value;
	axlList       * list;
	axlListCursor * cursor;

	/* create the list */
	list = axl_list_new (axl_list_always_return_1, NULL);

	/* add items */
	while (iterator < 10000) {

		/* add integers */
		axl_list_add (list, INT_TO_PTR(iterator));

		/* update the iterator */
		iterator++;
	}

	/* get items using iterator */
	cursor   = axl_list_cursor_new (list);
	iterator = 0;
	while (axl_list_cursor_has_item (cursor)) {
		/* get the value */
		value = PTR_TO_INT (axl_list_cursor_get (cursor));

		/* check value */
		if (value != iterator) {
			printf ("Values miss match: %d != %d\n", value, iterator);
			return false;
		}

		/* get the next */
		axl_list_cursor_next (cursor);

		/* update the iterator */
		iterator++;
		
	} /* end while */

	/* remove all items */
	axl_list_cursor_first (cursor);
	iterator = 0;
	while (axl_list_cursor_has_item (cursor)) {

		/* get the value */
		value = PTR_TO_INT (axl_list_cursor_get (cursor));

		/* check value */
		if (value != iterator) {
			printf ("Values miss match (2): %d != %d\n", value, iterator);
			return false;
		}

		/* remove */
		axl_list_cursor_remove (cursor);

		if (axl_list_length (list) > 1) {

			/* get the value */
			value = PTR_TO_INT (axl_list_cursor_get (cursor));
			
			/* check value */
			if (value != (iterator + 1)) {
				printf ("Values miss match (3): %d != %d\n", value, iterator + 1);
				return false;
			}
		}

		/* update the iterator */
		iterator++;
		
	} /* end while */

	if (axl_list_length (list) != 0) {
		printf ("List lengths mismatch: %d != 0\n", axl_list_length (list));
		return false;
	}

	/* free cursor */
	axl_list_cursor_free (cursor);

	/* release the list */
	axl_list_free (list);
	
	/* true */
	return true;
}

/** 
 * @brief Checks error reporting functions.
 * 
 * 
 * @return \ref true if ok, \ref false on rainy days.
 */
bool test_01_05 () 
{
	axlError * error = NULL;

	if (! axl_error_was_ok (error)) {
		printf ("Error: expected to find ok on a non-initialized error\n");
		return false;
	}

	axl_error_new (-1, "An error to report", NULL, &error);

	if (axl_error_was_ok (error)) {
		printf ("Error: expected to find error reported on an initialized error\n");
		return false;
	}

	axl_error_free (error);

	return true;
}

int lookups = 0;

void test_02_02_check_key (axlHash * hash, char * key)
{
	char * value = axl_hash_get (hash, key);	
	
	if (! axl_cmp (value, key)) {
		printf ("ERROR: unable to find %s value, found %s in the hash!!!\n", key, value);
		exit (-1);
	}

	lookups++;

	return;
}

/** 
 * @brief Test current libaxl hash implementation.
 *
 * @return true if it works properly or false if not.
 */
bool test_02_02 () 
{
	axlHash * hash;
	int       iterator;

	/* create the hash */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);
	
	/* insert data */
	axl_hash_insert (hash, "sword", "sword"); 
	axl_hash_insert (hash, "mace", "mace");
	axl_hash_insert (hash, "axe", "axe");
	axl_hash_insert (hash, "arrow", "arrow");
	axl_hash_insert (hash, "shield", "shield");
	axl_hash_insert (hash, "bag", "bag");
	axl_hash_insert (hash, "stone", "stone");
	axl_hash_insert (hash, "key", "key");
	axl_hash_insert (hash, "skull", "skull");
	axl_hash_insert (hash, "jar", "jar");
	axl_hash_insert (hash, "bottle", "bottle");
	axl_hash_insert (hash, "fairy", "fairy");
	axl_hash_insert (hash, "potion", "potion");
	axl_hash_insert (hash, "water", "water");
	axl_hash_insert (hash, "spoon", "spoon");
	axl_hash_insert (hash, "book", "book");
	axl_hash_insert (hash, "spear", "spear");
	axl_hash_insert (hash, "dagger", "dagger");
	axl_hash_insert (hash, "katana", "katana");
	axl_hash_insert (hash, "helmet", "helmet");
	axl_hash_insert (hash, "chain", "chain");
	axl_hash_insert (hash, "halberd", "halberd");
	axl_hash_insert (hash, "pipe", "pipe");
	axl_hash_insert (hash, "hat", "hat");
	axl_hash_insert (hash, "eyeofnewt", "eyeofnewt");
	axl_hash_insert (hash, "soup", "soup");
	axl_hash_insert (hash, "wolfbane", "wolfbane");
	axl_hash_insert (hash, "instantcoffee", "instantcoffee");
	axl_hash_insert (hash, "bugspray", "bugspray");
	axl_hash_insert (hash, "flint", "flint");
	axl_hash_insert (hash, "soap", "soap");
	axl_hash_insert (hash, "bones", "bones");
	axl_hash_insert (hash, "orb", "orb");
	axl_hash_insert (hash, "gold", "gold");
	axl_hash_insert (hash, "silver", "silver");
	axl_hash_insert (hash, "wine", "wine");
	axl_hash_insert (hash, "bread", "bread");

	axl_hash_insert (hash, "sword01", "sword01"); 
	axl_hash_insert (hash, "mace01", "mace01");
	axl_hash_insert (hash, "axe01", "axe01");
	axl_hash_insert (hash, "arrow01", "arrow01");
	axl_hash_insert (hash, "shield01", "shield01");
	axl_hash_insert (hash, "bag01", "bag01");
	axl_hash_insert (hash, "stone01", "stone01");
	axl_hash_insert (hash, "key01", "key01");
	axl_hash_insert (hash, "skull01", "skull01");
	axl_hash_insert (hash, "jar01", "jar01");
	axl_hash_insert (hash, "bottle01", "bottle01");
	axl_hash_insert (hash, "fairy01", "fairy01");
	axl_hash_insert (hash, "potion01", "potion01");
	axl_hash_insert (hash, "water01", "water01");
	axl_hash_insert (hash, "spoon01", "spoon01");
	axl_hash_insert (hash, "book01", "book01");
	axl_hash_insert (hash, "spear01", "spear01");
	axl_hash_insert (hash, "dagger01", "dagger01");
	axl_hash_insert (hash, "katana01", "katana01");
	axl_hash_insert (hash, "helmet01", "helmet01");
	axl_hash_insert (hash, "chain01", "chain01");
	axl_hash_insert (hash, "halberd01", "halberd01");
	axl_hash_insert (hash, "pipe01", "pipe01");
	axl_hash_insert (hash, "hat01", "hat01");
	axl_hash_insert (hash, "eyeofnewt01", "eyeofnewt01");
	axl_hash_insert (hash, "soup01", "soup01");
	axl_hash_insert (hash, "wolfbane01", "wolfbane01");
	axl_hash_insert (hash, "instantcoffee01", "instantcoffee01");
	axl_hash_insert (hash, "bugspray01", "bugspray01");
	axl_hash_insert (hash, "flint01", "flint01");
	axl_hash_insert (hash, "soap01", "soap01");
	axl_hash_insert (hash, "bones01", "bones01");
	axl_hash_insert (hash, "orb01", "orb01");
	axl_hash_insert (hash, "gold01", "gold01");
	axl_hash_insert (hash, "silver01", "silver01");
	axl_hash_insert (hash, "wine01", "wine01");
	axl_hash_insert (hash, "bread01", "bread01");

	axl_hash_insert (hash, "sword02", "sword02"); 
	axl_hash_insert (hash, "mace02", "mace02");
	axl_hash_insert (hash, "axe02", "axe02");
	axl_hash_insert (hash, "arrow02", "arrow02");
	axl_hash_insert (hash, "shield02", "shield02");
	axl_hash_insert (hash, "bag02", "bag02");
	axl_hash_insert (hash, "stone02", "stone02");
	axl_hash_insert (hash, "key02", "key02");
	axl_hash_insert (hash, "skull02", "skull02");
	axl_hash_insert (hash, "jar02", "jar02");
	axl_hash_insert (hash, "bottle02", "bottle02");
	axl_hash_insert (hash, "fairy02", "fairy02");
	axl_hash_insert (hash, "potion02", "potion02");
	axl_hash_insert (hash, "water02", "water02");
	axl_hash_insert (hash, "spoon02", "spoon02");
	axl_hash_insert (hash, "book02", "book02");
	axl_hash_insert (hash, "spear02", "spear02");
	axl_hash_insert (hash, "dagger02", "dagger02");
	axl_hash_insert (hash, "katana02", "katana02");
	axl_hash_insert (hash, "helmet02", "helmet02");
	axl_hash_insert (hash, "chain02", "chain02");
	axl_hash_insert (hash, "halberd02", "halberd02");
	axl_hash_insert (hash, "pipe02", "pipe02");
	axl_hash_insert (hash, "hat02", "hat02");
	axl_hash_insert (hash, "eyeofnewt02", "eyeofnewt02");
	axl_hash_insert (hash, "soup02", "soup02");
	axl_hash_insert (hash, "wolfbane02", "wolfbane02");
	axl_hash_insert (hash, "instantcoffee02", "instantcoffee02");
	axl_hash_insert (hash, "bugspray02", "bugspray02");
	axl_hash_insert (hash, "flint02", "flint02");
	axl_hash_insert (hash, "soap02", "soap02");
	axl_hash_insert (hash, "bones02", "bones02");
	axl_hash_insert (hash, "orb02", "orb02");
	axl_hash_insert (hash, "gold02", "gold02");
	axl_hash_insert (hash, "silver02", "silver02");
	axl_hash_insert (hash, "wine02", "wine02");
	axl_hash_insert (hash, "bread02", "bread02");

	axl_hash_insert (hash, "sword03", "sword03"); 
	axl_hash_insert (hash, "mace03", "mace03");
	axl_hash_insert (hash, "axe03", "axe03");
	axl_hash_insert (hash, "arrow03", "arrow03");
	axl_hash_insert (hash, "shield03", "shield03");
	axl_hash_insert (hash, "bag03", "bag03");
	axl_hash_insert (hash, "stone03", "stone03");
	axl_hash_insert (hash, "key03", "key03");
	axl_hash_insert (hash, "skull03", "skull03");
	axl_hash_insert (hash, "jar03", "jar03");
	axl_hash_insert (hash, "bottle03", "bottle03");
	axl_hash_insert (hash, "fairy03", "fairy03");
	axl_hash_insert (hash, "potion03", "potion03");
	axl_hash_insert (hash, "water03", "water03");
	axl_hash_insert (hash, "spoon03", "spoon03");
	axl_hash_insert (hash, "book03", "book03");
	axl_hash_insert (hash, "spear03", "spear03");
	axl_hash_insert (hash, "dagger03", "dagger03");
	axl_hash_insert (hash, "katana03", "katana03");
	axl_hash_insert (hash, "helmet03", "helmet03");
	axl_hash_insert (hash, "chain03", "chain03");
	axl_hash_insert (hash, "halberd03", "halberd03");
	axl_hash_insert (hash, "pipe03", "pipe03");
	axl_hash_insert (hash, "hat03", "hat03");
	axl_hash_insert (hash, "eyeofnewt03", "eyeofnewt03");
	axl_hash_insert (hash, "soup03", "soup03");
	axl_hash_insert (hash, "wolfbane03", "wolfbane03");
	axl_hash_insert (hash, "instantcoffee03", "instantcoffee03");
	axl_hash_insert (hash, "bugspray03", "bugspray03");
	axl_hash_insert (hash, "flint03", "flint03");
	axl_hash_insert (hash, "soap03", "soap03");
	axl_hash_insert (hash, "bones03", "bones03");
	axl_hash_insert (hash, "orb03", "orb03");
	axl_hash_insert (hash, "gold03", "gold03");
	axl_hash_insert (hash, "silver03", "silver03");
	axl_hash_insert (hash, "wine03", "wine03");
	axl_hash_insert (hash, "bread03", "bread03");

	axl_hash_insert (hash, "sword04", "sword04"); 
	axl_hash_insert (hash, "mace04", "mace04");
	axl_hash_insert (hash, "axe04", "axe04");
	axl_hash_insert (hash, "arrow04", "arrow04");
	axl_hash_insert (hash, "shield04", "shield04");
	axl_hash_insert (hash, "bag04", "bag04");
	axl_hash_insert (hash, "stone04", "stone04");
	axl_hash_insert (hash, "key04", "key04");
	axl_hash_insert (hash, "skull04", "skull04");
	axl_hash_insert (hash, "jar04", "jar04");
	axl_hash_insert (hash, "bottle04", "bottle04");
	axl_hash_insert (hash, "fairy04", "fairy04");
	axl_hash_insert (hash, "potion04", "potion04");
	axl_hash_insert (hash, "water04", "water04");
	axl_hash_insert (hash, "spoon04", "spoon04");
	axl_hash_insert (hash, "book04", "book04");
	axl_hash_insert (hash, "spear04", "spear04");
	axl_hash_insert (hash, "dagger04", "dagger04");
	axl_hash_insert (hash, "katana04", "katana04");
	axl_hash_insert (hash, "helmet04", "helmet04");
	axl_hash_insert (hash, "chain04", "chain04");
	axl_hash_insert (hash, "halberd04", "halberd04");
	axl_hash_insert (hash, "pipe04", "pipe04");
	axl_hash_insert (hash, "hat04", "hat04");
	axl_hash_insert (hash, "eyeofnewt04", "eyeofnewt04");
	axl_hash_insert (hash, "soup04", "soup04");
	axl_hash_insert (hash, "wolfbane04", "wolfbane04");
	axl_hash_insert (hash, "instantcoffee04", "instantcoffee04");
	axl_hash_insert (hash, "bugspray04", "bugspray04");
	axl_hash_insert (hash, "flint04", "flint04");
	axl_hash_insert (hash, "soap04", "soap04");
	axl_hash_insert (hash, "bones04", "bones04");
	axl_hash_insert (hash, "orb04", "orb04");
	axl_hash_insert (hash, "gold04", "gold04");
	axl_hash_insert (hash, "silver04", "silver04");
	axl_hash_insert (hash, "wine04", "wine04");
	axl_hash_insert (hash, "bread04", "bread04");

	axl_hash_insert (hash, "sword05", "sword05"); 
	axl_hash_insert (hash, "mace05", "mace05");
	axl_hash_insert (hash, "axe05", "axe05");
	axl_hash_insert (hash, "arrow05", "arrow05");
	axl_hash_insert (hash, "shield05", "shield05");
	axl_hash_insert (hash, "bag05", "bag05");
	axl_hash_insert (hash, "stone05", "stone05");
	axl_hash_insert (hash, "key05", "key05");
	axl_hash_insert (hash, "skull05", "skull05");
	axl_hash_insert (hash, "jar05", "jar05");
	axl_hash_insert (hash, "bottle05", "bottle05");
	axl_hash_insert (hash, "fairy05", "fairy05");
	axl_hash_insert (hash, "potion05", "potion05");
	axl_hash_insert (hash, "water05", "water05");
	axl_hash_insert (hash, "spoon05", "spoon05");
	axl_hash_insert (hash, "book05", "book05");
	axl_hash_insert (hash, "spear05", "spear05");
	axl_hash_insert (hash, "dagger05", "dagger05");
	axl_hash_insert (hash, "katana05", "katana05");
	axl_hash_insert (hash, "helmet05", "helmet05");
	axl_hash_insert (hash, "chain05", "chain05");
	axl_hash_insert (hash, "halberd05", "halberd05");
	axl_hash_insert (hash, "pipe05", "pipe05");
	axl_hash_insert (hash, "hat05", "hat05");
	axl_hash_insert (hash, "eyeofnewt05", "eyeofnewt05");
	axl_hash_insert (hash, "soup05", "soup05");
	axl_hash_insert (hash, "wolfbane05", "wolfbane05");
	axl_hash_insert (hash, "instantcoffee05", "instantcoffee05");
	axl_hash_insert (hash, "bugspray05", "bugspray05");
	axl_hash_insert (hash, "flint05", "flint05");
	axl_hash_insert (hash, "soap05", "soap05");
	axl_hash_insert (hash, "bones05", "bones05");
	axl_hash_insert (hash, "orb05", "orb05");
	axl_hash_insert (hash, "gold05", "gold05");
	axl_hash_insert (hash, "silver05", "silver05");
	axl_hash_insert (hash, "wine05", "wine05");
	axl_hash_insert (hash, "bread05", "bread05");

	axl_hash_insert (hash, "sword06", "sword06"); 
	axl_hash_insert (hash, "mace06", "mace06");
	axl_hash_insert (hash, "axe06", "axe06");
	axl_hash_insert (hash, "arrow06", "arrow06");
	axl_hash_insert (hash, "shield06", "shield06");
	axl_hash_insert (hash, "bag06", "bag06");
	axl_hash_insert (hash, "stone06", "stone06");
	axl_hash_insert (hash, "key06", "key06");
	axl_hash_insert (hash, "skull06", "skull06");
	axl_hash_insert (hash, "jar06", "jar06");
	axl_hash_insert (hash, "bottle06", "bottle06");
	axl_hash_insert (hash, "fairy06", "fairy06");
	axl_hash_insert (hash, "potion06", "potion06");
	axl_hash_insert (hash, "water06", "water06");
	axl_hash_insert (hash, "spoon06", "spoon06");
	axl_hash_insert (hash, "book06", "book06");
	axl_hash_insert (hash, "spear06", "spear06");
	axl_hash_insert (hash, "dagger06", "dagger06");
	axl_hash_insert (hash, "katana06", "katana06");
	axl_hash_insert (hash, "helmet06", "helmet06");
	axl_hash_insert (hash, "chain06", "chain06");
	axl_hash_insert (hash, "halberd06", "halberd06");
	axl_hash_insert (hash, "pipe06", "pipe06");
	axl_hash_insert (hash, "hat06", "hat06");
	axl_hash_insert (hash, "eyeofnewt06", "eyeofnewt06");
	axl_hash_insert (hash, "soup06", "soup06");
	axl_hash_insert (hash, "wolfbane06", "wolfbane06");
	axl_hash_insert (hash, "instantcoffee06", "instantcoffee06");
	axl_hash_insert (hash, "bugspray06", "bugspray06");
	axl_hash_insert (hash, "flint06", "flint06");
	axl_hash_insert (hash, "soap06", "soap06");
	axl_hash_insert (hash, "bones06", "bones06");
	axl_hash_insert (hash, "orb06", "orb06");
	axl_hash_insert (hash, "gold06", "gold06");
	axl_hash_insert (hash, "silver06", "silver06");
	axl_hash_insert (hash, "wine06", "wine06");
	axl_hash_insert (hash, "bread06", "bread06");

	axl_hash_insert (hash, "sword07", "sword07"); 
	axl_hash_insert (hash, "mace07", "mace07");
	axl_hash_insert (hash, "axe07", "axe07");
	axl_hash_insert (hash, "arrow07", "arrow07");
	axl_hash_insert (hash, "shield07", "shield07");
	axl_hash_insert (hash, "bag07", "bag07");
	axl_hash_insert (hash, "stone07", "stone07");
	axl_hash_insert (hash, "key07", "key07");
	axl_hash_insert (hash, "skull07", "skull07");
	axl_hash_insert (hash, "jar07", "jar07");
	axl_hash_insert (hash, "bottle07", "bottle07");
	axl_hash_insert (hash, "fairy07", "fairy07");
	axl_hash_insert (hash, "potion07", "potion07");
	axl_hash_insert (hash, "water07", "water07");
	axl_hash_insert (hash, "spoon07", "spoon07");
	axl_hash_insert (hash, "book07", "book07");
	axl_hash_insert (hash, "spear07", "spear07");
	axl_hash_insert (hash, "dagger07", "dagger07");
	axl_hash_insert (hash, "katana07", "katana07");
	axl_hash_insert (hash, "helmet07", "helmet07");
	axl_hash_insert (hash, "chain07", "chain07");
	axl_hash_insert (hash, "halberd07", "halberd07");
	axl_hash_insert (hash, "pipe07", "pipe07");
	axl_hash_insert (hash, "hat07", "hat07");
	axl_hash_insert (hash, "eyeofnewt07", "eyeofnewt07");
	axl_hash_insert (hash, "soup07", "soup07");
	axl_hash_insert (hash, "wolfbane07", "wolfbane07");
	axl_hash_insert (hash, "instantcoffee07", "instantcoffee07");
	axl_hash_insert (hash, "bugspray07", "bugspray07");
	axl_hash_insert (hash, "flint07", "flint07");
	axl_hash_insert (hash, "soap07", "soap07");
	axl_hash_insert (hash, "bones07", "bones07");
	axl_hash_insert (hash, "orb07", "orb07");
	axl_hash_insert (hash, "gold07", "gold07");
	axl_hash_insert (hash, "silver07", "silver07");
	axl_hash_insert (hash, "wine07", "wine07");
	axl_hash_insert (hash, "bread07", "bread07");

	axl_hash_insert (hash, "sword08", "sword08"); 
	axl_hash_insert (hash, "mace08", "mace08");
	axl_hash_insert (hash, "axe08", "axe08");
	axl_hash_insert (hash, "arrow08", "arrow08");
	axl_hash_insert (hash, "shield08", "shield08");
	axl_hash_insert (hash, "bag08", "bag08");
	axl_hash_insert (hash, "stone08", "stone08");
	axl_hash_insert (hash, "key08", "key08");
	axl_hash_insert (hash, "skull08", "skull08");
	axl_hash_insert (hash, "jar08", "jar08");
	axl_hash_insert (hash, "bottle08", "bottle08");
	axl_hash_insert (hash, "fairy08", "fairy08");
	axl_hash_insert (hash, "potion08", "potion08");
	axl_hash_insert (hash, "water08", "water08");
	axl_hash_insert (hash, "spoon08", "spoon08");
	axl_hash_insert (hash, "book08", "book08");
	axl_hash_insert (hash, "spear08", "spear08");
	axl_hash_insert (hash, "dagger08", "dagger08");
	axl_hash_insert (hash, "katana08", "katana08");
	axl_hash_insert (hash, "helmet08", "helmet08");
	axl_hash_insert (hash, "chain08", "chain08");
	axl_hash_insert (hash, "halberd08", "halberd08");
	axl_hash_insert (hash, "pipe08", "pipe08");
	axl_hash_insert (hash, "hat08", "hat08");
	axl_hash_insert (hash, "eyeofnewt08", "eyeofnewt08");
	axl_hash_insert (hash, "soup08", "soup08");
	axl_hash_insert (hash, "wolfbane08", "wolfbane08");
	axl_hash_insert (hash, "instantcoffee08", "instantcoffee08");
	axl_hash_insert (hash, "bugspray08", "bugspray08");
	axl_hash_insert (hash, "flint08", "flint08");
	axl_hash_insert (hash, "soap08", "soap08");
	axl_hash_insert (hash, "bones08", "bones08");
	axl_hash_insert (hash, "orb08", "orb08");
	axl_hash_insert (hash, "gold08", "gold08");
	axl_hash_insert (hash, "silver08", "silver08");
	axl_hash_insert (hash, "wine08", "wine08");
	axl_hash_insert (hash, "bread08", "bread08");

	axl_hash_insert (hash, "sword09", "sword09"); 
	axl_hash_insert (hash, "mace09", "mace09");
	axl_hash_insert (hash, "axe09", "axe09");
	axl_hash_insert (hash, "arrow09", "arrow09");
	axl_hash_insert (hash, "shield09", "shield09");
	axl_hash_insert (hash, "bag09", "bag09");
	axl_hash_insert (hash, "stone09", "stone09");
	axl_hash_insert (hash, "key09", "key09");
	axl_hash_insert (hash, "skull09", "skull09");
	axl_hash_insert (hash, "jar09", "jar09");
	axl_hash_insert (hash, "bottle09", "bottle09");
	axl_hash_insert (hash, "fairy09", "fairy09");
	axl_hash_insert (hash, "potion09", "potion09");
	axl_hash_insert (hash, "water09", "water09");
	axl_hash_insert (hash, "spoon09", "spoon09");
	axl_hash_insert (hash, "book09", "book09");
	axl_hash_insert (hash, "spear09", "spear09");
	axl_hash_insert (hash, "dagger09", "dagger09");
	axl_hash_insert (hash, "katana09", "katana09");
	axl_hash_insert (hash, "helmet09", "helmet09");
	axl_hash_insert (hash, "chain09", "chain09");
	axl_hash_insert (hash, "halberd09", "halberd09");
	axl_hash_insert (hash, "pipe09", "pipe09");
	axl_hash_insert (hash, "hat09", "hat09");
	axl_hash_insert (hash, "eyeofnewt09", "eyeofnewt09");
	axl_hash_insert (hash, "soup09", "soup09");
	axl_hash_insert (hash, "wolfbane09", "wolfbane09");
	axl_hash_insert (hash, "instantcoffee09", "instantcoffee09");
	axl_hash_insert (hash, "bugspray09", "bugspray09");
	axl_hash_insert (hash, "flint09", "flint09");
	axl_hash_insert (hash, "soap09", "soap09");
	axl_hash_insert (hash, "bones09", "bones09");
	axl_hash_insert (hash, "orb09", "orb09");
	axl_hash_insert (hash, "gold09", "gold09");
	axl_hash_insert (hash, "silver09", "silver09");
	axl_hash_insert (hash, "wine09", "wine09");
	axl_hash_insert (hash, "bread09", "bread09");

	axl_hash_insert (hash, "sword10", "sword10"); 
	axl_hash_insert (hash, "mace10", "mace10");
	axl_hash_insert (hash, "axe10", "axe10");
	axl_hash_insert (hash, "arrow10", "arrow10");
	axl_hash_insert (hash, "shield10", "shield10");
	axl_hash_insert (hash, "bag10", "bag10");
	axl_hash_insert (hash, "stone10", "stone10");
	axl_hash_insert (hash, "key10", "key10");
	axl_hash_insert (hash, "skull10", "skull10");
	axl_hash_insert (hash, "jar10", "jar10");
	axl_hash_insert (hash, "bottle10", "bottle10");
	axl_hash_insert (hash, "fairy10", "fairy10");
	axl_hash_insert (hash, "potion10", "potion10");
	axl_hash_insert (hash, "water10", "water10");
	axl_hash_insert (hash, "spoon10", "spoon10");
	axl_hash_insert (hash, "book10", "book10");
	axl_hash_insert (hash, "spear10", "spear10");
	axl_hash_insert (hash, "dagger10", "dagger10");
	axl_hash_insert (hash, "katana10", "katana10");
	axl_hash_insert (hash, "helmet10", "helmet10");
	axl_hash_insert (hash, "chain10", "chain10");
	axl_hash_insert (hash, "halberd10", "halberd10");
	axl_hash_insert (hash, "pipe10", "pipe10");
	axl_hash_insert (hash, "hat10", "hat10");
	axl_hash_insert (hash, "eyeofnewt10", "eyeofnewt10");
	axl_hash_insert (hash, "soup10", "soup10");
	axl_hash_insert (hash, "wolfbane10", "wolfbane10");
	axl_hash_insert (hash, "instantcoffee10", "instantcoffee10");
	axl_hash_insert (hash, "bugspray10", "bugspray10");
	axl_hash_insert (hash, "flint10", "flint10");
	axl_hash_insert (hash, "soap10", "soap10");
	axl_hash_insert (hash, "bones10", "bones10");
	axl_hash_insert (hash, "orb10", "orb10");
	axl_hash_insert (hash, "gold10", "gold10");
	axl_hash_insert (hash, "silver10", "silver10");
	axl_hash_insert (hash, "wine10", "wine10");
	axl_hash_insert (hash, "bread10", "bread10");

	axl_hash_insert (hash, "sword11", "sword11"); 
	axl_hash_insert (hash, "mace11", "mace11");
	axl_hash_insert (hash, "axe11", "axe11");
	axl_hash_insert (hash, "arrow11", "arrow11");
	axl_hash_insert (hash, "shield11", "shield11");
	axl_hash_insert (hash, "bag11", "bag11");
	axl_hash_insert (hash, "stone11", "stone11");
	axl_hash_insert (hash, "key11", "key11");
	axl_hash_insert (hash, "skull11", "skull11");
	axl_hash_insert (hash, "jar11", "jar11");
	axl_hash_insert (hash, "bottle11", "bottle11");
	axl_hash_insert (hash, "fairy11", "fairy11");
	axl_hash_insert (hash, "potion11", "potion11");
	axl_hash_insert (hash, "water11", "water11");
	axl_hash_insert (hash, "spoon11", "spoon11");
	axl_hash_insert (hash, "book11", "book11");
	axl_hash_insert (hash, "spear11", "spear11");
	axl_hash_insert (hash, "dagger11", "dagger11");
	axl_hash_insert (hash, "katana11", "katana11");
	axl_hash_insert (hash, "helmet11", "helmet11");
	axl_hash_insert (hash, "chain11", "chain11");
	axl_hash_insert (hash, "halberd11", "halberd11");
	axl_hash_insert (hash, "pipe11", "pipe11");
	axl_hash_insert (hash, "hat11", "hat11");
	axl_hash_insert (hash, "eyeofnewt11", "eyeofnewt11");
	axl_hash_insert (hash, "soup11", "soup11");
	axl_hash_insert (hash, "wolfbane11", "wolfbane11");
	axl_hash_insert (hash, "instantcoffee11", "instantcoffee11");
	axl_hash_insert (hash, "bugspray11", "bugspray11");
	axl_hash_insert (hash, "flint11", "flint11");
	axl_hash_insert (hash, "soap11", "soap11");
	axl_hash_insert (hash, "bones11", "bones11");
	axl_hash_insert (hash, "orb11", "orb11");
	axl_hash_insert (hash, "gold11", "gold11");
	axl_hash_insert (hash, "silver11", "silver11");
	axl_hash_insert (hash, "wine11", "wine11");
	axl_hash_insert (hash, "bread11", "bread11");

	/* show hash status */
	axl_hash_show_status  (hash);

	/* perform checks */
	iterator = 0;
	while (iterator < 1000) {

		/* get values and check them */
		test_02_02_check_key (hash, "bag");
		test_02_02_check_key (hash, "sword"); 
		test_02_02_check_key (hash, "mace");
		test_02_02_check_key (hash, "axe");
		test_02_02_check_key (hash, "arrow");
		test_02_02_check_key (hash, "shield");
		test_02_02_check_key (hash, "bag");
		test_02_02_check_key (hash, "stone");
		test_02_02_check_key (hash, "key");
		test_02_02_check_key (hash, "skull");
		test_02_02_check_key (hash, "jar");
		test_02_02_check_key (hash, "bottle");
		test_02_02_check_key (hash, "fairy");
		test_02_02_check_key (hash, "potion");
		test_02_02_check_key (hash, "water");
		test_02_02_check_key (hash, "spoon");
		test_02_02_check_key (hash, "book");
		test_02_02_check_key (hash, "spear");
		test_02_02_check_key (hash, "dagger");
		test_02_02_check_key (hash, "katana");
		test_02_02_check_key (hash, "helmet");
		test_02_02_check_key (hash, "chain");
		test_02_02_check_key (hash, "halberd");
		test_02_02_check_key (hash, "pipe");
		test_02_02_check_key (hash, "hat");
		test_02_02_check_key (hash, "eyeofnewt");
		test_02_02_check_key (hash, "soup");
		test_02_02_check_key (hash, "wolfbane");
		test_02_02_check_key (hash, "instantcoffee");
		test_02_02_check_key (hash, "bugspray");
		test_02_02_check_key (hash, "flint");
		test_02_02_check_key (hash, "soap");
		test_02_02_check_key (hash, "bones");
		test_02_02_check_key (hash, "orb");
		test_02_02_check_key (hash, "gold");
		test_02_02_check_key (hash, "silver");
		test_02_02_check_key (hash, "wine");
		test_02_02_check_key (hash, "bread");
		
		test_02_02_check_key (hash, "bag01");
		test_02_02_check_key (hash, "sword01"); 
		test_02_02_check_key (hash, "mace01");
		test_02_02_check_key (hash, "axe01");
		test_02_02_check_key (hash, "arrow01");
		test_02_02_check_key (hash, "shield01");
		test_02_02_check_key (hash, "bag01");
		test_02_02_check_key (hash, "stone01");
		test_02_02_check_key (hash, "key01");
		test_02_02_check_key (hash, "skull01");
		test_02_02_check_key (hash, "jar01");
		test_02_02_check_key (hash, "bottle01");
		test_02_02_check_key (hash, "fairy01");
		test_02_02_check_key (hash, "potion01");
		test_02_02_check_key (hash, "water01");
		test_02_02_check_key (hash, "spoon01");
		test_02_02_check_key (hash, "book01");
		test_02_02_check_key (hash, "spear01");
		test_02_02_check_key (hash, "dagger01");
		test_02_02_check_key (hash, "katana01");
		test_02_02_check_key (hash, "helmet01");
		test_02_02_check_key (hash, "chain01");
		test_02_02_check_key (hash, "halberd01");
		test_02_02_check_key (hash, "pipe01");
		test_02_02_check_key (hash, "hat01");
		test_02_02_check_key (hash, "eyeofnewt01");
		test_02_02_check_key (hash, "soup01");
		test_02_02_check_key (hash, "wolfbane01");
		test_02_02_check_key (hash, "instantcoffee01");
		test_02_02_check_key (hash, "bugspray01");
		test_02_02_check_key (hash, "flint01");
		test_02_02_check_key (hash, "soap01");
		test_02_02_check_key (hash, "bones01");
		test_02_02_check_key (hash, "orb01");
		test_02_02_check_key (hash, "gold01");
		test_02_02_check_key (hash, "silver01");
		test_02_02_check_key (hash, "wine01");
		test_02_02_check_key (hash, "bread01");
		
		test_02_02_check_key (hash, "bag02");
		test_02_02_check_key (hash, "sword02"); 
		test_02_02_check_key (hash, "mace02");
		test_02_02_check_key (hash, "axe02");
		test_02_02_check_key (hash, "arrow02");
		test_02_02_check_key (hash, "shield02");
		test_02_02_check_key (hash, "bag02");
		test_02_02_check_key (hash, "stone02");
		test_02_02_check_key (hash, "key02");
		test_02_02_check_key (hash, "skull02");
		test_02_02_check_key (hash, "jar02");
		test_02_02_check_key (hash, "bottle02");
		test_02_02_check_key (hash, "fairy02");
		test_02_02_check_key (hash, "potion02");
		test_02_02_check_key (hash, "water02");
		test_02_02_check_key (hash, "spoon02");
		test_02_02_check_key (hash, "book02");
		test_02_02_check_key (hash, "spear02");
		test_02_02_check_key (hash, "dagger02");
		test_02_02_check_key (hash, "katana02");
		test_02_02_check_key (hash, "helmet02");
		test_02_02_check_key (hash, "chain02");
		test_02_02_check_key (hash, "halberd02");
		test_02_02_check_key (hash, "pipe02");
		test_02_02_check_key (hash, "hat02");
		test_02_02_check_key (hash, "eyeofnewt02");
		test_02_02_check_key (hash, "soup02");
		test_02_02_check_key (hash, "wolfbane02");
		test_02_02_check_key (hash, "instantcoffee02");
		test_02_02_check_key (hash, "bugspray02");
		test_02_02_check_key (hash, "flint02");
		test_02_02_check_key (hash, "soap02");
		test_02_02_check_key (hash, "bones02");
		test_02_02_check_key (hash, "orb02");
		test_02_02_check_key (hash, "gold02");
		test_02_02_check_key (hash, "silver02");
		test_02_02_check_key (hash, "wine02");
		test_02_02_check_key (hash, "bread02");

		test_02_02_check_key (hash, "bag03");
		test_02_02_check_key (hash, "sword03"); 
		test_02_02_check_key (hash, "mace03");
		test_02_02_check_key (hash, "axe03");
		test_02_02_check_key (hash, "arrow03");
		test_02_02_check_key (hash, "shield03");
		test_02_02_check_key (hash, "bag03");
		test_02_02_check_key (hash, "stone03");
		test_02_02_check_key (hash, "key03");
		test_02_02_check_key (hash, "skull03");
		test_02_02_check_key (hash, "jar03");
		test_02_02_check_key (hash, "bottle03");
		test_02_02_check_key (hash, "fairy03");
		test_02_02_check_key (hash, "potion03");
		test_02_02_check_key (hash, "water03");
		test_02_02_check_key (hash, "spoon03");
		test_02_02_check_key (hash, "book03");
		test_02_02_check_key (hash, "spear03");
		test_02_02_check_key (hash, "dagger03");
		test_02_02_check_key (hash, "katana03");
		test_02_02_check_key (hash, "helmet03");
		test_02_02_check_key (hash, "chain03");
		test_02_02_check_key (hash, "halberd03");
		test_02_02_check_key (hash, "pipe03");
		test_02_02_check_key (hash, "hat03");
		test_02_02_check_key (hash, "eyeofnewt03");
		test_02_02_check_key (hash, "soup03");
		test_02_02_check_key (hash, "wolfbane03");
		test_02_02_check_key (hash, "instantcoffee03");
		test_02_02_check_key (hash, "bugspray03");
		test_02_02_check_key (hash, "flint03");
		test_02_02_check_key (hash, "soap03");
		test_02_02_check_key (hash, "bones03");
		test_02_02_check_key (hash, "orb03");
		test_02_02_check_key (hash, "gold03");
		test_02_02_check_key (hash, "silver03");
		test_02_02_check_key (hash, "wine03");
		test_02_02_check_key (hash, "bread03");

		test_02_02_check_key (hash, "bag04");
		test_02_02_check_key (hash, "sword04"); 
		test_02_02_check_key (hash, "mace04");
		test_02_02_check_key (hash, "axe04");
		test_02_02_check_key (hash, "arrow04");
		test_02_02_check_key (hash, "shield04");
		test_02_02_check_key (hash, "bag04");
		test_02_02_check_key (hash, "stone04");
		test_02_02_check_key (hash, "key04");
		test_02_02_check_key (hash, "skull04");
		test_02_02_check_key (hash, "jar04");
		test_02_02_check_key (hash, "bottle04");
		test_02_02_check_key (hash, "fairy04");
		test_02_02_check_key (hash, "potion04");
		test_02_02_check_key (hash, "water04");
		test_02_02_check_key (hash, "spoon04");
		test_02_02_check_key (hash, "book04");
		test_02_02_check_key (hash, "spear04");
		test_02_02_check_key (hash, "dagger04");
		test_02_02_check_key (hash, "katana04");
		test_02_02_check_key (hash, "helmet04");
		test_02_02_check_key (hash, "chain04");
		test_02_02_check_key (hash, "halberd04");
		test_02_02_check_key (hash, "pipe04");
		test_02_02_check_key (hash, "hat04");
		test_02_02_check_key (hash, "eyeofnewt04");
		test_02_02_check_key (hash, "soup04");
		test_02_02_check_key (hash, "wolfbane04");
		test_02_02_check_key (hash, "instantcoffee04");
		test_02_02_check_key (hash, "bugspray04");
		test_02_02_check_key (hash, "flint04");
		test_02_02_check_key (hash, "soap04");
		test_02_02_check_key (hash, "bones04");
		test_02_02_check_key (hash, "orb04");
		test_02_02_check_key (hash, "gold04");
		test_02_02_check_key (hash, "silver04");
		test_02_02_check_key (hash, "wine04");
		test_02_02_check_key (hash, "bread04");

		test_02_02_check_key (hash, "bag05");
		test_02_02_check_key (hash, "sword05"); 
		test_02_02_check_key (hash, "mace05");
		test_02_02_check_key (hash, "axe05");
		test_02_02_check_key (hash, "arrow05");
		test_02_02_check_key (hash, "shield05");
		test_02_02_check_key (hash, "bag05");
		test_02_02_check_key (hash, "stone05");
		test_02_02_check_key (hash, "key05");
		test_02_02_check_key (hash, "skull05");
		test_02_02_check_key (hash, "jar05");
		test_02_02_check_key (hash, "bottle05");
		test_02_02_check_key (hash, "fairy05");
		test_02_02_check_key (hash, "potion05");
		test_02_02_check_key (hash, "water05");
		test_02_02_check_key (hash, "spoon05");
		test_02_02_check_key (hash, "book05");
		test_02_02_check_key (hash, "spear05");
		test_02_02_check_key (hash, "dagger05");
		test_02_02_check_key (hash, "katana05");
		test_02_02_check_key (hash, "helmet05");
		test_02_02_check_key (hash, "chain05");
		test_02_02_check_key (hash, "halberd05");
		test_02_02_check_key (hash, "pipe05");
		test_02_02_check_key (hash, "hat05");
		test_02_02_check_key (hash, "eyeofnewt05");
		test_02_02_check_key (hash, "soup05");
		test_02_02_check_key (hash, "wolfbane05");
		test_02_02_check_key (hash, "instantcoffee05");
		test_02_02_check_key (hash, "bugspray05");
		test_02_02_check_key (hash, "flint05");
		test_02_02_check_key (hash, "soap05");
		test_02_02_check_key (hash, "bones05");
		test_02_02_check_key (hash, "orb05");
		test_02_02_check_key (hash, "gold05");
		test_02_02_check_key (hash, "silver05");
		test_02_02_check_key (hash, "wine05");
		test_02_02_check_key (hash, "bread05");

		test_02_02_check_key (hash, "bag06");
		test_02_02_check_key (hash, "sword06"); 
		test_02_02_check_key (hash, "mace06");
		test_02_02_check_key (hash, "axe06");
		test_02_02_check_key (hash, "arrow06");
		test_02_02_check_key (hash, "shield06");
		test_02_02_check_key (hash, "bag06");
		test_02_02_check_key (hash, "stone06");
		test_02_02_check_key (hash, "key06");
		test_02_02_check_key (hash, "skull06");
		test_02_02_check_key (hash, "jar06");
		test_02_02_check_key (hash, "bottle06");
		test_02_02_check_key (hash, "fairy06");
		test_02_02_check_key (hash, "potion06");
		test_02_02_check_key (hash, "water06");
		test_02_02_check_key (hash, "spoon06");
		test_02_02_check_key (hash, "book06");
		test_02_02_check_key (hash, "spear06");
		test_02_02_check_key (hash, "dagger06");
		test_02_02_check_key (hash, "katana06");
		test_02_02_check_key (hash, "helmet06");
		test_02_02_check_key (hash, "chain06");
		test_02_02_check_key (hash, "halberd06");
		test_02_02_check_key (hash, "pipe06");
		test_02_02_check_key (hash, "hat06");
		test_02_02_check_key (hash, "eyeofnewt06");
		test_02_02_check_key (hash, "soup06");
		test_02_02_check_key (hash, "wolfbane06");
		test_02_02_check_key (hash, "instantcoffee06");
		test_02_02_check_key (hash, "bugspray06");
		test_02_02_check_key (hash, "flint06");
		test_02_02_check_key (hash, "soap06");
		test_02_02_check_key (hash, "bones06");
		test_02_02_check_key (hash, "orb06");
		test_02_02_check_key (hash, "gold06");
		test_02_02_check_key (hash, "silver06");
		test_02_02_check_key (hash, "wine06");
		test_02_02_check_key (hash, "bread06");

		test_02_02_check_key (hash, "bag07");
		test_02_02_check_key (hash, "sword07"); 
		test_02_02_check_key (hash, "mace07");
		test_02_02_check_key (hash, "axe07");
		test_02_02_check_key (hash, "arrow07");
		test_02_02_check_key (hash, "shield07");
		test_02_02_check_key (hash, "bag07");
		test_02_02_check_key (hash, "stone07");
		test_02_02_check_key (hash, "key07");
		test_02_02_check_key (hash, "skull07");
		test_02_02_check_key (hash, "jar07");
		test_02_02_check_key (hash, "bottle07");
		test_02_02_check_key (hash, "fairy07");
		test_02_02_check_key (hash, "potion07");
		test_02_02_check_key (hash, "water07");
		test_02_02_check_key (hash, "spoon07");
		test_02_02_check_key (hash, "book07");
		test_02_02_check_key (hash, "spear07");
		test_02_02_check_key (hash, "dagger07");
		test_02_02_check_key (hash, "katana07");
		test_02_02_check_key (hash, "helmet07");
		test_02_02_check_key (hash, "chain07");
		test_02_02_check_key (hash, "halberd07");
		test_02_02_check_key (hash, "pipe07");
		test_02_02_check_key (hash, "hat07");
		test_02_02_check_key (hash, "eyeofnewt07");
		test_02_02_check_key (hash, "soup07");
		test_02_02_check_key (hash, "wolfbane07");
		test_02_02_check_key (hash, "instantcoffee07");
		test_02_02_check_key (hash, "bugspray07");
		test_02_02_check_key (hash, "flint07");
		test_02_02_check_key (hash, "soap07");
		test_02_02_check_key (hash, "bones07");
		test_02_02_check_key (hash, "orb07");
		test_02_02_check_key (hash, "gold07");
		test_02_02_check_key (hash, "silver07");
		test_02_02_check_key (hash, "wine07");
		test_02_02_check_key (hash, "bread07");

		test_02_02_check_key (hash, "bag08");
		test_02_02_check_key (hash, "sword08"); 
		test_02_02_check_key (hash, "mace08");
		test_02_02_check_key (hash, "axe08");
		test_02_02_check_key (hash, "arrow08");
		test_02_02_check_key (hash, "shield08");
		test_02_02_check_key (hash, "bag08");
		test_02_02_check_key (hash, "stone08");
		test_02_02_check_key (hash, "key08");
		test_02_02_check_key (hash, "skull08");
		test_02_02_check_key (hash, "jar08");
		test_02_02_check_key (hash, "bottle08");
		test_02_02_check_key (hash, "fairy08");
		test_02_02_check_key (hash, "potion08");
		test_02_02_check_key (hash, "water08");
		test_02_02_check_key (hash, "spoon08");
		test_02_02_check_key (hash, "book08");
		test_02_02_check_key (hash, "spear08");
		test_02_02_check_key (hash, "dagger08");
		test_02_02_check_key (hash, "katana08");
		test_02_02_check_key (hash, "helmet08");
		test_02_02_check_key (hash, "chain08");
		test_02_02_check_key (hash, "halberd08");
		test_02_02_check_key (hash, "pipe08");
		test_02_02_check_key (hash, "hat08");
		test_02_02_check_key (hash, "eyeofnewt08");
		test_02_02_check_key (hash, "soup08");
		test_02_02_check_key (hash, "wolfbane08");
		test_02_02_check_key (hash, "instantcoffee08");
		test_02_02_check_key (hash, "bugspray08");
		test_02_02_check_key (hash, "flint08");
		test_02_02_check_key (hash, "soap08");
		test_02_02_check_key (hash, "bones08");
		test_02_02_check_key (hash, "orb08");
		test_02_02_check_key (hash, "gold08");
		test_02_02_check_key (hash, "silver08");
		test_02_02_check_key (hash, "wine08");
		test_02_02_check_key (hash, "bread08");

		test_02_02_check_key (hash, "bag09");
		test_02_02_check_key (hash, "sword09"); 
		test_02_02_check_key (hash, "mace09");
		test_02_02_check_key (hash, "axe09");
		test_02_02_check_key (hash, "arrow09");
		test_02_02_check_key (hash, "shield09");
		test_02_02_check_key (hash, "bag09");
		test_02_02_check_key (hash, "stone09");
		test_02_02_check_key (hash, "key09");
		test_02_02_check_key (hash, "skull09");
		test_02_02_check_key (hash, "jar09");
		test_02_02_check_key (hash, "bottle09");
		test_02_02_check_key (hash, "fairy09");
		test_02_02_check_key (hash, "potion09");
		test_02_02_check_key (hash, "water09");
		test_02_02_check_key (hash, "spoon09");
		test_02_02_check_key (hash, "book09");
		test_02_02_check_key (hash, "spear09");
		test_02_02_check_key (hash, "dagger09");
		test_02_02_check_key (hash, "katana09");
		test_02_02_check_key (hash, "helmet09");
		test_02_02_check_key (hash, "chain09");
		test_02_02_check_key (hash, "halberd09");
		test_02_02_check_key (hash, "pipe09");
		test_02_02_check_key (hash, "hat09");
		test_02_02_check_key (hash, "eyeofnewt09");
		test_02_02_check_key (hash, "soup09");
		test_02_02_check_key (hash, "wolfbane09");
		test_02_02_check_key (hash, "instantcoffee09");
		test_02_02_check_key (hash, "bugspray09");
		test_02_02_check_key (hash, "flint09");
		test_02_02_check_key (hash, "soap09");
		test_02_02_check_key (hash, "bones09");
		test_02_02_check_key (hash, "orb09");
		test_02_02_check_key (hash, "gold09");
		test_02_02_check_key (hash, "silver09");
		test_02_02_check_key (hash, "wine09");
		test_02_02_check_key (hash, "bread09");

		test_02_02_check_key (hash, "bag10");
		test_02_02_check_key (hash, "sword10"); 
		test_02_02_check_key (hash, "mace10");
		test_02_02_check_key (hash, "axe10");
		test_02_02_check_key (hash, "arrow10");
		test_02_02_check_key (hash, "shield10");
		test_02_02_check_key (hash, "bag10");
		test_02_02_check_key (hash, "stone10");
		test_02_02_check_key (hash, "key10");
		test_02_02_check_key (hash, "skull10");
		test_02_02_check_key (hash, "jar10");
		test_02_02_check_key (hash, "bottle10");
		test_02_02_check_key (hash, "fairy10");
		test_02_02_check_key (hash, "potion10");
		test_02_02_check_key (hash, "water10");
		test_02_02_check_key (hash, "spoon10");
		test_02_02_check_key (hash, "book10");
		test_02_02_check_key (hash, "spear10");
		test_02_02_check_key (hash, "dagger10");
		test_02_02_check_key (hash, "katana10");
		test_02_02_check_key (hash, "helmet10");
		test_02_02_check_key (hash, "chain10");
		test_02_02_check_key (hash, "halberd10");
		test_02_02_check_key (hash, "pipe10");
		test_02_02_check_key (hash, "hat10");
		test_02_02_check_key (hash, "eyeofnewt10");
		test_02_02_check_key (hash, "soup10");
		test_02_02_check_key (hash, "wolfbane10");
		test_02_02_check_key (hash, "instantcoffee10");
		test_02_02_check_key (hash, "bugspray10");
		test_02_02_check_key (hash, "flint10");
		test_02_02_check_key (hash, "soap10");
		test_02_02_check_key (hash, "bones10");
		test_02_02_check_key (hash, "orb10");
		test_02_02_check_key (hash, "gold10");
		test_02_02_check_key (hash, "silver10");
		test_02_02_check_key (hash, "wine10");
		test_02_02_check_key (hash, "bread10");

		test_02_02_check_key (hash, "bag11");
		test_02_02_check_key (hash, "sword11"); 
		test_02_02_check_key (hash, "mace11");
		test_02_02_check_key (hash, "axe11");
		test_02_02_check_key (hash, "arrow11");
		test_02_02_check_key (hash, "shield11");
		test_02_02_check_key (hash, "bag11");
		test_02_02_check_key (hash, "stone11");
		test_02_02_check_key (hash, "key11");
		test_02_02_check_key (hash, "skull11");
		test_02_02_check_key (hash, "jar11");
		test_02_02_check_key (hash, "bottle11");
		test_02_02_check_key (hash, "fairy11");
		test_02_02_check_key (hash, "potion11");
		test_02_02_check_key (hash, "water11");
		test_02_02_check_key (hash, "spoon11");
		test_02_02_check_key (hash, "book11");
		test_02_02_check_key (hash, "spear11");
		test_02_02_check_key (hash, "dagger11");
		test_02_02_check_key (hash, "katana11");
		test_02_02_check_key (hash, "helmet11");
		test_02_02_check_key (hash, "chain11");
		test_02_02_check_key (hash, "halberd11");
		test_02_02_check_key (hash, "pipe11");
		test_02_02_check_key (hash, "hat11");
		test_02_02_check_key (hash, "eyeofnewt11");
		test_02_02_check_key (hash, "soup11");
		test_02_02_check_key (hash, "wolfbane11");
		test_02_02_check_key (hash, "instantcoffee11");
		test_02_02_check_key (hash, "bugspray11");
		test_02_02_check_key (hash, "flint11");
		test_02_02_check_key (hash, "soap11");
		test_02_02_check_key (hash, "bones11");
		test_02_02_check_key (hash, "orb11");
		test_02_02_check_key (hash, "gold11");
		test_02_02_check_key (hash, "silver11");
		test_02_02_check_key (hash, "wine11");
		test_02_02_check_key (hash, "bread11");

		/* update iterator */
		iterator++;
	}

	/* destroy the hash */
	axl_hash_free (hash);

	/* terminated test */
	return true;
}

axlPointer test_02_01_copy_key (axlPointer key, axlDestroyFunc key_destroy, axlPointer data, axlDestroyFunc data_destroy)
{
	if (key_destroy == NULL)
		return key;

	return axl_strdup (key);
}

axlPointer test_02_01_copy_value (axlPointer key, axlDestroyFunc key_destroy, axlPointer data, axlDestroyFunc data_destroy)
{
	if (data_destroy == NULL)
		return data;

	return axl_strdup (data);
}


/** 
 * @brief Checks normal hash operations.
 * 
 * 
 * @return \ref true if it was ok.
 */
bool test_02_01 () 
{
	axlHash    * hash;
	axlHash    * hash2;
	axlPointer * data;

	/* create a hash */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);

	/* perform operations on empty state */
	if (axl_hash_exists (hash, "value")) {
		printf ("Found that the hash returns that item exist but it is empty\n");
		return false;
	}

	/* perform a remove operation */
	axl_hash_remove (hash, "value");

	/* get data associated */
	data = axl_hash_get (hash, "value");
	if (data != NULL) {
		printf ("Found that the hash returns that item exist but it is empty\n");
		return false;
	}

	/* add items to the hash */
	axl_hash_insert_full (hash, "value", NULL, axl_strdup ("value"), axl_free);
	axl_hash_insert_full (hash, "value2", NULL, axl_strdup ("value2"), axl_free);
	axl_hash_insert_full (hash, "value3", NULL, axl_strdup ("value3"), axl_free);

	/* check items stored */
	test_02_02_check_key (hash, "value");
	test_02_02_check_key (hash, "value2");
	test_02_02_check_key (hash, "value3");

	/* copy the hash */
	hash2 = axl_hash_copy (hash, test_02_01_copy_key, test_02_01_copy_value);

	/* check items stored */
	test_02_02_check_key (hash2, "value");
	test_02_02_check_key (hash2, "value2");
	test_02_02_check_key (hash2, "value3");
	

	/* destroy the hash */
	axl_hash_free (hash);

	/* destroy the hash2 */
	axl_hash_free (hash2);

	return true;
}

/** 
 * @brief Test current libaxl hash implementation.
 *
 * @return true if it works properly or false if not.
 */
bool test_02_03 () 
{
	axlHash * hash;

	/* create the hash */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);
	
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");
	axl_hash_insert (hash, "value", "value");

	axl_hash_show_status (hash);

	if (axl_hash_items (hash) != 1) {
		printf ("ERROR: expected to find a hash size of 1 but found: %d\n",
			axl_hash_items (hash));
		return false;
	}

	/* free the hash */
	axl_hash_free (hash);

	return true;
}

void show_item_test_02_04 (axlPointer key, axlPointer data)
{
	__axl_log ("hash-test", AXL_LEVEL_DEBUG, "  %s -> %s", (char *) key, (char *) data);
}

bool test_02_04 () 
{
	axlHash * hash;
	
	/* create the hash */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);

	/* insert data */
	axl_hash_insert (hash, "sword", "sword"); 
	axl_hash_insert (hash, "mace", "mace");
	axl_hash_insert (hash, "axe", "axe");
	axl_hash_insert (hash, "arrow", "arrow");
	axl_hash_insert (hash, "shield", "shield");
	axl_hash_insert (hash, "bag", "bag");
	axl_hash_insert (hash, "stone", "stone");
	axl_hash_insert (hash, "key", "key");
	axl_hash_insert (hash, "skull", "skull");
	axl_hash_insert (hash, "jar", "jar");
	axl_hash_insert (hash, "bottle", "bottle");
	axl_hash_insert (hash, "fairy", "fairy");
	axl_hash_insert (hash, "potion", "potion");
	axl_hash_insert (hash, "water", "water");
	axl_hash_insert (hash, "spoon", "spoon");
	axl_hash_insert (hash, "book", "book");
	axl_hash_insert (hash, "spear", "spear");
	axl_hash_insert (hash, "dagger", "dagger");
	axl_hash_insert (hash, "katana", "katana");
	axl_hash_insert (hash, "helmet", "helmet");
	axl_hash_insert (hash, "chain", "chain");
	axl_hash_insert (hash, "halberd", "halberd");
	axl_hash_insert (hash, "pipe", "pipe");
	axl_hash_insert (hash, "hat", "hat");
	axl_hash_insert (hash, "eyeofnewt", "eyeofnewt");
	axl_hash_insert (hash, "soup", "soup");
	axl_hash_insert (hash, "wolfbane", "wolfbane");
	axl_hash_insert (hash, "instantcoffee", "instantcoffee");
	axl_hash_insert (hash, "bugspray", "bugspray");
	axl_hash_insert (hash, "flint", "flint");
	axl_hash_insert (hash, "soap", "soap");
	axl_hash_insert (hash, "bones", "bones");
	axl_hash_insert (hash, "orb", "orb");
	axl_hash_insert (hash, "gold", "gold");
	axl_hash_insert (hash, "silver", "silver");
	axl_hash_insert (hash, "wine", "wine");
	axl_hash_insert (hash, "bread", "bread");

	axl_hash_show_status_full (hash, show_item_test_02_04);

	axl_hash_free (hash);

	return true;

}

bool test_02_05 ()
{
	axlHash       * hash;
	axlHashCursor * cursor;
	int             iterator;
	char          * key, * value;
	
	/* create the hash */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);

	/* insert data */
	axl_hash_insert (hash, "sword", "sword"); 
	axl_hash_insert (hash, "mace", "mace");
	axl_hash_insert (hash, "axe", "axe");
	axl_hash_insert (hash, "arrow", "arrow");
	axl_hash_insert (hash, "shield", "shield");
	axl_hash_insert (hash, "bag", "bag");
	axl_hash_insert (hash, "stone", "stone");
	axl_hash_insert (hash, "key", "key");
	axl_hash_insert (hash, "skull", "skull");
	axl_hash_insert (hash, "jar", "jar");
	axl_hash_insert (hash, "bottle", "bottle");
	axl_hash_insert (hash, "fairy", "fairy");
	axl_hash_insert (hash, "potion", "potion");
	axl_hash_insert (hash, "water", "water");
	axl_hash_insert (hash, "spoon", "spoon");
	axl_hash_insert (hash, "book", "book");
	axl_hash_insert (hash, "spear", "spear");
	axl_hash_insert (hash, "dagger", "dagger");
	axl_hash_insert (hash, "katana", "katana");
	axl_hash_insert (hash, "helmet", "helmet");
	axl_hash_insert (hash, "chain", "chain");
	axl_hash_insert (hash, "halberd", "halberd");
	axl_hash_insert (hash, "pipe", "pipe");
	axl_hash_insert (hash, "hat", "hat");
	axl_hash_insert (hash, "eyeofnewt", "eyeofnewt");
	axl_hash_insert (hash, "soup", "soup");
	axl_hash_insert (hash, "wolfbane", "wolfbane");
	axl_hash_insert (hash, "instantcoffee", "instantcoffee");
	axl_hash_insert (hash, "bugspray", "bugspray");
	axl_hash_insert (hash, "flint", "flint");
	axl_hash_insert (hash, "soap", "soap");
	axl_hash_insert (hash, "bones", "bones");
	axl_hash_insert (hash, "orb", "orb");
	axl_hash_insert (hash, "gold", "gold");
	axl_hash_insert (hash, "silver", "silver");
	axl_hash_insert (hash, "wine", "wine");
	axl_hash_insert (hash, "bread", "bread");

	/* create a cursor */
	cursor   = axl_hash_cursor_new (hash);
	iterator = 0;
	while (axl_hash_cursor_has_item (cursor)) {
		/* first item */
		if (! axl_cmp (axl_hash_cursor_get_key (cursor),
			       axl_hash_cursor_get_value (cursor))) {
			printf ("error: supposed to find key and value equal (%s==%s), but not found..\n",
				(char*)axl_hash_cursor_get_key (cursor),
				(char*)axl_hash_cursor_get_value (cursor));
			return false;
		} /* end if */

		/* get next */
		axl_hash_cursor_next (cursor);

		iterator++;

		if (iterator == 38) {
			printf ("error: found more items than actually expected..\n");
			return false;
		}
	} /* end while */

	if (iterator != 37) {
		printf ("error: found more items than actually expected..\n");
		return false;
	}

	iterator = 0;
	axl_hash_cursor_first (cursor);
	while (axl_hash_cursor_has_item (cursor)) {
		/* first item */
		if (! axl_cmp (axl_hash_cursor_get_key (cursor),
			       axl_hash_cursor_get_value (cursor))) {
			printf ("error: supposed to find key and value equal (%s==%s), but not found..\n",
				(char*)axl_hash_cursor_get_key (cursor),
				(char*)axl_hash_cursor_get_value (cursor));
			return false;
		} /* end if */

		iterator++;

		if (iterator < 37) {
			if (! axl_hash_cursor_has_next (cursor)) {
				printf ("error: expected to find next node on iterator (%d)\n", iterator);
				return false;
			} /* end if */
		} /* end if */

		/* get next */
		axl_hash_cursor_next (cursor);

		if (iterator == 38) {
			printf ("error: found more items than actually expected..\n");
			return false;
		}
	} /* end while */

	if (iterator != 37) {
		printf ("error: found different count of items than actually expected (%d != 37)..\n", iterator);
		return false;
	}

	/* check last api */
	axl_hash_cursor_last (cursor);
	if (! axl_hash_cursor_has_item (cursor)) {
		printf ("error: expected to find last element defined..\n");
		return false;
	}

	/* check last */
	if (! axl_cmp ("flint", axl_hash_cursor_get_key (cursor))) {
		printf ("error: expected to find last element \"flint\"=\"%s\"\n", (char*) axl_hash_cursor_get_key (cursor));
		return false;
	}


	if (axl_hash_cursor_has_next (cursor)) {
		printf ("error: expected to not find next element defined..\n");
		return false;
	} /* end if */

	axl_hash_cursor_first (cursor);
	while (axl_hash_cursor_has_item (cursor)) {
		/* first item */
		if (! axl_cmp (axl_hash_cursor_get_key (cursor),
			       axl_hash_cursor_get_value (cursor))) {
			printf ("error: supposed to find key and value equal (%s==%s), but not found..\n",
				(char*)axl_hash_cursor_get_key (cursor),
				(char*)axl_hash_cursor_get_value (cursor));
			return false;
		} /* end if */

		/* get value */
		key   = axl_hash_cursor_get_key (cursor);
		value = axl_hash_cursor_get_value (cursor);

		/* check key to exists */
		if (! axl_hash_exists (axl_hash_cursor_hash (cursor), key)) {
			printf ("error: expected to find key defined=<%s>\n", key);
			return false;
		}

		/* check value to exists */
		if (! axl_cmp (axl_hash_get (axl_hash_cursor_hash (cursor), key), value)) {
			printf ("error: expected to find value not found: <%s>!=<%s>\n", 
				value, (char*) axl_hash_get (axl_hash_cursor_hash (cursor), key));
			return false;
		} /* end if */
		

		/* remove items */
		axl_hash_cursor_remove (cursor);
		
	} /* end while */

	if (axl_hash_items (hash) != 0) {
		printf ("error: expected to find hash with 0 size (but found: %d)\n",
			axl_hash_items (hash));
		return false;
	} /* end if */

	/* free cursor */
	axl_hash_cursor_free (cursor);

	/* free the hash */
	axl_hash_free (hash);

	/* now check the cursor with only one item */
	hash = axl_hash_new (axl_hash_string, axl_hash_equal_string);
	axl_hash_insert (hash, "12:BB:DD:8A:1D:E5:64:0C:7E:F5:EB:B3:21:C7:67:7E", "test");
	cursor = axl_hash_cursor_new (hash);

	iterator = 0;
	while (axl_hash_cursor_has_item (cursor) && iterator < 10) {
		
		axl_hash_cursor_get_value (cursor);
		
		/* get next cursor */
		axl_hash_cursor_next (cursor);
		iterator++;
	}

	if (iterator != 1) {
		printf ("Expected to find only one iteration inside the hash but the limit was found\n");
		return false;
	}

	axl_hash_free (hash);
	axl_hash_cursor_free (cursor);

	/* test ok */
	return true;
}

/** 
 * Test01: Initial xml header checking.
 */
int main (int argc, char ** argv)
{
	axlError * error;
	
	/* initialize axl library */
	if (! axl_init ()) {
		printf ("Unable to initialize Axl library\n");
		return -1;
	}

	/* DATA STRUCTURE TESTS */
	if (test_01_01 ()) {
		printf ("Test 01-01: LibAxl list implementation [   OK   ]\n");
	}else {
		printf ("Test 01-01 ##: LibAxl list implementation [ FAILED ]\n");
		return -1;
	}

	if (test_01_02 ()) {
		printf ("Test 01-02: LibAxl FIFO implementation [   OK   ]\n");
	}else {
		printf ("Test 01-02: LibAxl list implementation [ FAILED ]\n");
		return -1;
	}

	if (test_01_03 ()) {
		printf ("Test 01-03: LibAxl string functions    [   OK   ]\n");
	}else {
		printf ("Test 01-03: LibAxl string functions    [ FAILED ]\n");
		return -1;
	}

	if (test_01_04 ()) {
		printf ("Test 01-04: LibAxl list implementation (II) [   OK   ]\n");
	}else {
		printf ("Test 01-04: LibAxl list implementation (II) [ FAILED ]\n");
		return -1;
	}

	if (test_01_05 ()) {
		printf ("Test 01-05: LibAxl error reporting [   OK   ]\n");
	}else {
		printf ("Test 01-05: LibAxl error reporting [ FAILED ]\n");
		return -1;
	}

	/* HASH IMPLEMENTATION CHECKS */
	if (test_02_01 ()) {
		printf ("Test 02-01: LibAxl hash implementation [   OK   ]\n");
	}else {
		printf ("Test 02-01: LibAxl hash implementation [ FAILEDp ]\n");
		return -1;
	}

	if (test_02_02 ()) {
		printf ("Test 02-02: LibAxl hash implementation (lookup) [   OK   ]\n");
	}else {
		printf ("Test 02-02: LibAxl hash implementation (lookup) [ FAILED ]\n");
		return -1;
	}

	if (test_02_03 ()) {
		printf ("Test 02-03: LibAxl hash implementation (replace) [   OK   ]\n");
	}else {
		printf ("Test 02-03: LibAxl hash implementation (replace) [ FAILED ]\n");
		return -1;
	}

	if (test_02_04 ()) {
		printf ("Test 02-04: LibAxl hash implementation (remove) [   OK   ]\n");
	}else {
		printf ("Test 02-04: LibAxl hash implementation (remove) [ FAILED ]\n");
		return -1;
	}

	if (test_02_05 ()) {
		printf ("Test 02-04: LibAxl hash cursor [   OK   ]\n");
	}else {
		printf ("Test 02-04: LibAxl hash cursor [ FAILED ]\n");
		return -1;
	}
	
	/* LIBRARY TESTS */
	if (test_01 (&error))
		printf ("Test 01: basic xml parsing [   OK   ]\n");
	else {
		printf ("Test 01: basic xml parsing [ FAILED ]\n  (code: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_01a (&error)) {
		printf ("Test 01-a: Axl Stream boundary checks [   OK   ]\n");
	} else {
		printf ("Test 01-a: Axl Stream boundary checks [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_01b (&error)) {
		printf ("Test 01-b: Basic XML parsing, XML document position [   OK   ]\n");
	} else {
		printf ("Test 01-b: Basic XML parsing, XML document position [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_01c (&error)) {
		printf ("Test 01-c: Basic XML parsing, XML document traversing [   OK   ]\n");
	} else {
		printf ("Test 01-c: Basic XML parsing, XML document traversing [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_01d (&error)) {
		printf ("Test 01-d: Basic XML parsing, node nth access [   OK   ]\n");
	} else {
		printf ("Test 01-d: Basic XML parsing, node nth access [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_02 (&error))
		printf ("Test 02: basic xml error detection [   OK   ]\n");
	else {
		printf ("Test 02: basic xml error detection [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_03 (&error))
		printf ("Test 03: complex xml error detection [   OK   ]\n");
	else {
		printf ("Test 03: complex xml error detection [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_04 (&error))
		printf ("Test 04: complex xml parsing [   OK   ]\n");
	else {
		printf ("Test 04: complex xml parsing [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_05 (&error))
		printf ("Test 05: DTD basic parsing [   OK   ]\n");
	else {
		printf ("Test 05: DTD basic parsing [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_06 (&error))
		printf ("Test 06: DTD basic parsing (2) [   OK   ]\n");
	else {
		printf ("Test 06: DTD basic parsing (2) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_07 (&error)) 
		printf ("Test 07: DTD validation (I) [   OK   ]\n");
	else {
		printf ("Test 07: DTD validation (I) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_08 (&error)) 
		printf ("Test 08: Large XML file loading [   OK   ]\n");
	else {
		printf ("Test 08: Large XML file loading [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	



	if (test_09 (&error)) 
		printf ("Test 09: Complex DTD validation [   OK   ]\n");
	else {
		printf ("Test 09: Complex DTD validation [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}


	if (test_10 (&error)) 
		printf ("Test 10: Complex DTD validation (II) [   OK   ]\n");
	else {
		printf ("Test 10: Complex DTD validation (II) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_11 (&error)) 
		printf ("Test 11: Complex DTD validation (III) [   OK   ]\n");
	else {
		printf ("Test 11: Complex DTD validation (III) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_12 (&error)) 
		printf ("Test 12: Complex DTD validation (IV) [   OK   ]\n");
	else {
		printf ("Test 12: Complex DTD validation (IV) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_13 (&error)) 
		printf ("Test 13: XML memory dumping [   OK   ]\n");
	else {
		printf ("Test 13: XML memory dumping [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_14 (&error)) {
		printf ("Test 14: quotation and escape sequences (&,',\",<,>) [   OK   ]\n");
	} else {
		printf ("Test 14: quotation and escape sequences (&,',\",<,>) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_15 (&error)) {
		printf ("Test 15: DTD validation fail checks (25/04/2006) [   OK   ]\n");
	} else {
		printf ("Test 15: DTD validation fail checks (25/04/2006) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	


	if (test_16 (&error)) {
		printf ("Test 16: DTD validation fail checks (03/05/2006) [   OK   ]\n");
	} else {
		printf ("Test 16: DTD validation fail checks (03/05/2006) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_17 (&error)) {
		printf ("Test 17: DTD validation fail checks (02/07/2006) [   OK   ]\n");
	} else {
		printf ("Test 17: DTD validation fail checks (02/07/2006) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_18 (&error)) {
		printf ("Test 18: DTD ENTITY support [   OK   ]\n");
	} else {
		printf ("Test 18: DTD ENTITY support [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_19 (&error)) {
		printf ("Test 19: Axl document node replacing [   OK   ]\n");
	} else {
		printf ("Test 19: Axl document node replacing [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	


	if (test_20 (&error)) {
		printf ("Test 20: Axl node copy and anotation data [   OK   ]\n");
	} else {
		printf ("Test 20: Axl node copy and anotation data [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_20b (&error)) {
		printf ("Test 20b: Axl node copy 2 [   OK   ]\n");
	} else {
		printf ("Test 20b: Axl node copy 2 [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_21 (&error)) {
		printf ("Test 21: Axl node mixed content [   OK   ]\n");
	} else {
		printf ("Test 21: Axl node mixed content [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	if (test_22 (&error)) {
		printf ("Test 22: Axl node attributes [   OK   ]\n");
	} else {
		printf ("Test 22: Axl node attributes [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_23 (&error)) {
		printf ("Test 23: Axl item modification [   OK   ]\n");
	} else {
		printf ("Test 23: Axl item modification [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_24 (&error)) {
		printf ("Test 24: Invalid sequences detection [   OK   ]\n");
	}else {
		printf ("Test 24: Invalid sequences detection [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_25 (&error)) {
		printf ("Test 25: Lookup functions [   OK   ]\n");
	}else {
		printf ("Test 25: Lookup [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

#ifdef AXL_NS_SUPPORT
	if (test_26 (&error)) {
		printf ("Test 26: Namespace support (basic) [   OK   ]\n");
	}else {
		printf ("Test 26: Namespace support (basic) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_27 (&error)) {
		printf ("Test 27: Namespace support [   OK   ]\n");
	}else {
		printf ("Test 27: Namespace support [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_28 (&error)) {
		printf ("Test 28: Namespace defaulting support [   OK   ]\n");
	}else {
		printf ("Test 28: Namespace defaulting support [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_29 (&error)) {
		printf ("Test 29: Namespace lookup support [   OK   ]\n");
	}else {
		printf ("Test 29: Namespace lookup support [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

#endif /* end #ifdef AXL_NS_SUPPORT */	

	if (test_30 (&error)) {
		printf ("Test 30: DTD attribute validation support [   OK   ]\n");
	}else {
		printf ("Test 30: DTD attribute validation support [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_31 (&error)) {
		printf ("Test 31: DTD attribute validation (ID support) [   OK   ]\n");
	}else {
		printf ("Test 31: DTD attribute validation (ID support) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_32 (&error)) {
		printf ("Test 32: DTD attribute validation (IDREF support) [   OK   ]\n");
	}else {
		printf ("Test 32: DTD attribute validation (IDREF support) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}
	
	if (test_33 (&error)) {
		printf ("Test 33: Recursive root node replace [   OK   ]\n");
	}else {
		printf ("Test 33: Recursive root node replace [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_34 (&error)) {
		printf ("Test 34: axl trim bug (19/06/2007) [   OK   ]\n");
	}else {
		printf ("Test 34: axl trim bug (19/06/2007) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_35 (&error)) {
		printf ("Test 35: axl_doc_parse_from_file bug (20/06/2007) [   OK   ]\n");
	}else {
		printf ("Test 35: axl_doc_parse_from_file bug (20/06/2007) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_36 (&error)) {
		printf ("Test 36: deattach root node [   OK   ]\n");
	}else {
		printf ("Test 36: deattach root node [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_37 (&error)) {
		printf ("Test 37: clearing xml node content [   OK   ]\n");
	}else {
		printf ("Test 37: clearing xml node content [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}

	if (test_38 (&error)) {
		printf ("Test 38: IDREF dtd error found (23/06/2007) [   OK   ]\n");
	}else {
		printf ("Test 38: IDREF dtd error found (23/06/2007) [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}



	/* cleanup axl library */
	axl_end ();
	return 0;
}
