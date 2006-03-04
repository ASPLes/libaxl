#include <axl.h>
#include <stdio.h>

/** 
 * @brief DTD operations. Checks that DTD support works, reading
 * simple DTD definitions, and ensuring elements are properly read.
 * 
 * @param error The axlError reference to be filled if the function
 * returns AXL_FALSE.
 * 
 * @return AXL_TRUE if the validity test is passed, AXL_FALSE if not.
 */
bool test_05 (axlError ** error)
{

	axlDoc * doc;
	axlNode * node;
	axlDtd  * dtd;

	/* parse the document found */
	doc = axl_doc_parse_from_file ("test.xml", error);
	if (doc == NULL)
		return AXL_FALSE;

	node = axl_doc_get (doc, "/complex/data/row/td");
	if (node == NULL) {
		axl_error_new (-1, "Expected to receive a node, not found", NULL, error);
		axl_doc_free (doc);
		return AXL_FALSE;
	}

	if (! axl_cmp (axl_node_get_content (node, NULL), "10")) {
		axl_error_new (-1, "Expected to receive a node content, not found", NULL, error);
		axl_doc_free (doc);
		return AXL_FALSE;
	}
	
	dtd = axl_dtd_parse_from_file ("test.dtd", error);
	if (dtd == NULL)
		return AXL_FALSE;

	/* now validate the document */
	if (! axl_dtd_validate (doc, dtd)) {
		axl_error_new (-1, "found that the document provided should be valid, but validation function report an error", NULL, error);
		return AXL_FALSE;
	}

	/* release memory used by the parser */
	axl_doc_free (doc);
	
	/* release memory used by the DTD element */
	axl_dtd_free (dtd);

	return AXL_TRUE;
	
}

/** 
 * @brief Perform some additional checks for a more extended xml
 * documents, that includes attributes inside node definitions.
 *
 * @param error The axlError where failures will be reported 
 *
 * @return The \ref AXL_TRUE if test is passed, AXL_FALSE if not.
 */
bool test_04 (axlError ** error)
{
	axlDoc  * doc;
	axlNode * node;

	/* parse the document */
	doc = axl_doc_parse_strings (error, 
				     "<!-- Coment example -->",
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
				     "              <test2 attr='2.0' />",
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
		return AXL_FALSE;

	/* get the node <td> value */
	node = axl_doc_get (doc, "/complex/data/row/td");
	if (! axl_cmp (axl_node_get_content (node, NULL), "10")) {
		axl_error_new (-1, "Expected to receive a 10 value, but not found", NULL, error);
		return AXL_FALSE;
	}

	/* get a reference to the test2 node */
	node = axl_doc_get (doc, "/complex/data/row/more/test2");
	if (node == NULL) {
		axl_error_new (-1, "Expected to find a test2 node at the given location", NULL, error);
		return AXL_FALSE;
	}

	/* check the attribute */
	if (! axl_node_has_attribute (node, "attr")) {
		axl_error_new (-1, "Expected to find an attribute called 'attr' inside test2 node at the given location", NULL, error);
		return AXL_FALSE;
	}

	/* check the attribute value */
	if (! axl_cmp (axl_node_get_attribute_value (node, "attr"), "2.0")) {
		axl_error_new (-1, "Expected to find an attribute value equal '2.0' inside test2 node at the given location", NULL, error);
		return AXL_FALSE;
	}

	/* add here Pi instruction checking */
	if (! axl_doc_has_pi_target (doc, "test")) {
		axl_error_new (-1, "failed to get expected PI target 'test'", NULL, error);
		return AXL_FALSE;
	}

	if (! axl_cmp (axl_doc_get_pi_target_content (doc, "test"), "\"my content\"")) {
		axl_error_new (-1, "expected to receive a PI content not found", NULL, error);
		return AXL_FALSE;
	}

	node = axl_doc_get (doc, "/complex/data/row");
	if (node == NULL) {
		axl_error_new (-1, "unable to get expected node to check node PI support", NULL, error);
		return AXL_FALSE;
	}

	if (! axl_node_has_pi_target (node, "test")) {
		axl_error_new (-1, "failed to get expected PI target 'test' for the node", NULL, error);
		return AXL_FALSE;
	}

	
	node = axl_doc_get (doc, "/complex/data/non-xml-document");
	if (node == NULL) {
		axl_error_new (-1, "Expected to receive the CDATA node, not found", NULL, error);
		return AXL_FALSE;
	}

	if (! axl_cmp (axl_node_get_content (node, NULL), "<xml><<<<<<>>>>>><<<>>>><<<<<<>>>")) {
		printf ("Content doesn't match: %s != %s\n", 
			axl_node_get_content (node, NULL), "<xml><<<<<<>>>>>><<<>>>><<<<<<>>>");
		axl_error_new (-1, "Expected to recevie CDATA content, not found or not match", NULL, error);
		return AXL_FALSE;
	}

				     
	/* free the memory */
	axl_doc_free (doc);
	
	return AXL_TRUE;
}

/** 
 * @brief Checks a more extended and complex xml documents
 * 
 * @param error The axlError where failures will be reported.
 * 
 * @return \ref AXL_TRUE if test is passed, AXL_FALSE if not.
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
		return AXL_FALSE;

	/* get the root node */
	node = axl_doc_get_root (doc);
	if (! NODE_CMP_NAME (node, "complex")) {
		axl_error_new (-2, "Root node returned from the document is not the one excepted", NULL, error);
		axl_doc_free (doc);
		return AXL_FALSE;
	}

	/* test get node function */
	node = axl_doc_get (doc, "/complex/data2/td");
	if (node == NULL) {
		axl_error_new (-2, "Unable to find a node due to a path selection", NULL, error);
		axl_doc_free (doc);
		return AXL_FALSE;
	}

	/* check the node returned */
	if (! NODE_CMP_NAME (node, "td")) {
		axl_error_new (-2, "The node for the node looked up doesn't match ", NULL, error);
		axl_doc_free (doc);
		return AXL_FALSE;
	}

	/* check for returning bad nodes */
	node = axl_doc_get (doc, "/complex/data3/td");
	if (node != NULL) {
		axl_error_new (-2, "Returned a node that should be NULL", NULL, error);
		axl_doc_free (doc);
		return AXL_FALSE;
	}

	node = axl_doc_get (doc, "/complex/data2/td");
	if (!axl_cmp (axl_node_get_content (node, NULL), " 23  ")) {
		axl_error_new (-2, "Node content have failed, expected a different value", NULL, error);
		return AXL_FALSE;
	}

	node = axl_doc_get (doc, "complex/data3/td");
	if (node != NULL) {
		axl_error_new (-2, "Parsed a path that is invalid", NULL, error);
		return AXL_FALSE;
	}
	
	/* release memory allocated by the document */
	axl_doc_free (doc);

	return AXL_TRUE;
}

/** 
 * @brief Checks xml document error for basic parsing.
 * 
 * @param error The axlError where failures are returned.
 * 
 * @return AXL_TRUE if test are properly run. AXL_FALSE if not.
 */
bool test_02 (axlError ** error) 
{
	axlDoc * doc;
	
	doc = axl_doc_parse ("<? xml >", 8, error);
	if (doc != NULL) {
		axl_error_new (-1, "Failed to detect wrong xml header", NULL, error);
		return AXL_FALSE;
	}
	axl_error_free (* error);

	doc = axl_doc_parse ("<?xml >", 7, error);
	if (doc != NULL) {
		
		axl_error_new (-1, "Failed to detect wrong xml trailing header", NULL, error);
		return AXL_FALSE;
	}
	axl_error_free (* error);


	return AXL_TRUE;
}


/** 
 * @brief Parse an initial document, with an xml header with
 * version=1.0 and no more header.
 * 
 * 
 * @return AXL_FALSE if the function fails to parse the
 * document. AXL_TRUE if the test was properly executed.
 */
bool test_01 (axlError ** error) 
{
	/* axl document representation */
	axlDoc   * doc;
	

	/* parse the given string */
	doc = axl_doc_parse ("<?xml version='1.0' ?><axldoc />", 32, error);
	if (doc == NULL) {
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);	

	doc = axl_doc_parse ("<?xml      version='1.0'            ?>      <another />", 55, error);
	if (doc == NULL) {
		return AXL_FALSE;
	}
	
	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml    \n   \t \n \r version='1.0' ?>    <doc />", 50, error);
	if (doc == NULL) {
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml  version=\"1.0\"        ?>   \r \t \n<another />", 54, error);
	if (doc == NULL) {
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml  version=\"1.0\" \t \n \r encoding='utf-8\"   ?> <data />", 63, error);
	if (doc == NULL) {
		return AXL_FALSE;
	}

	if (strcmp ("utf-8", axl_doc_get_encoding (doc))) {
		printf ("ERROR: encoding read from the document differs from the expected (got %s, expected %s)!\n",
			axl_doc_get_encoding (doc), "utf-8");
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml version='1.0' encoding='utf-8' standalone='yes' ?>  <data/>", 65, error);
	if (doc == NULL) {
		return AXL_FALSE;
	}

	if (!axl_doc_get_standalone (doc)) {
		printf ("ERROR: Expected to receive a true standalone configuration but false was found\n");
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);
	

	return AXL_TRUE;
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
		printf ("Test 05: DTD xml parsing [   OK   ]\n");
	else {
		printf ("Test 05: DTD xml parsing [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}	

	/* cleanup axl library */
	axl_end ();
	return 0;
}
