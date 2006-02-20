#include <axl.h>
#include <stdio.h>



/** 
 * @brief Checks a more extended and complex xml documents
 * 
 * @param error The axlError where failures will be reported.
 * 
 * @return \ref AXL_TRUE if test is passed, AXL_FALSE if not.
 */
bool test_03 (axlError ** error)
{

	axlDoc * doc;

	doc = axl_doc_parse ("<?xml version='1.0' ?>\n\
<complex>\n\
  <data>\n\
    <row>10</row><row>20</row><row>30</row>\n\
  </data>\n\
</complex>", -1, error);
	if (doc == NULL)
		return AXL_FALSE;

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
	}

	if (test_02 (&error))
		printf ("Test 02: basic xml error detection [   OK   ]\n");
	else {
		printf ("Test 02: basic xml error detection [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
	}	

	if (test_03 (&error))
		printf ("Test 03: complex xml error detection [   OK   ]\n");
	else {
		printf ("Test 03: complex xml error detection [ FAILED ]\n  (CODE: %d) %s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
	}	

	/* cleanup axl library */
	axl_end ();
	return 0;
}
