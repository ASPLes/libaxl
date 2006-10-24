#include <axl.h>
#include <stdio.h>

/** 
 * @brief Test xml attribute support.
 * 
 * @param error The optional axlError to be used to report errors.
 * 
 * @return true if the validity test is passed, false if not.
 */
bool test_22 (axlError ** error)
{
	axlDoc  * doc;
	axlNode * node;
	char    * value;

	
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
		axl_error_new (-1, "Not found an attribute requested, which should exists", NULL, error);
		return false;
	}

	/* get attribute */
	value = axl_node_get_attribute_value (node, "attribute2");
	if (! axl_cmp (value, "value2")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists", NULL, error);
		return false;
	}

	/* check for attributes */
	if (! axl_node_has_attribute (node, "attribute3")) {
		axl_error_new (-1, "Not found an attribute requested, which should exists", NULL, error);
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
	axl_node_anotate_data (root, "key", "value");
	
	/* get child1 */
	node = axl_node_get_first_child (root);

	/* get child2 */
	node = axl_node_get_first_child (node);

	/* anotate data */
	axl_node_anotate_data (node, "key1", "value1");

	/* perform searches */
	data = axl_node_anotate_get (node, "key", false);
	if (data != NULL) {
		axl_error_new (-1, "Expected to find nothing while looking for 'key'(1)", NULL, error);
		return false;
	}

	data = axl_node_anotate_get (node, "key", true);
	if (data == NULL || !axl_cmp (data, "value")) {
		axl_error_new (-1, "Expected to find data while looking for 'key' at parents (2)", NULL, error);
		return false;
	}

	/* perform searches inside the node */
	data = axl_node_anotate_get (node, "key1", false);
	if (data == NULL || !axl_cmp (data, "value1")) {
		axl_error_new (-1, "Expected to find nothing while looking for 'key1'(3)", NULL, error);
		return false;
	}

	/* perform more anotation but with native data */
	axl_node_anotate_int (root, "int-value", 14);
	
	if (axl_node_anotate_get_int (root, "int-value", false) != 14) {
		axl_error_new (-1, "Expected to find an integer value (14), but it wasn't found", NULL, error);
		return false;
	}

	axl_node_anotate_double (root, "double-value", 58.20);

	if (axl_node_anotate_get_double (root, "double-value", false) != 58.20) {
		axl_error_new (-1, "Expected to find an double value (58.20), but it wasn't found", NULL, error);
		return false;
	}

	axl_node_anotate_string (root, "string-value", "this is a test string");

	if (! axl_cmp (axl_node_anotate_get_string (root, "string-value", false), "this is a test string")) {
		axl_error_new (-1, "Expected to find a string value (\"this is a test string\"), but it wasn't found", NULL, error);
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
	value = axl_node_get_content (node, &size);
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
	axlDoc * doc  = NULL;
	axlDoc * doc2 = NULL;
	char   * content;
	int      size;
	
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

	/* cleanup axl library */
	axl_end ();
	return 0;
}
