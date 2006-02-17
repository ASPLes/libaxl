#include <axl.h>
#include <stdio.h>

/** 
 * @brief Parse an initial document, with an xml header with
 * version=1.0 and no more header.
 * 
 * 
 * @return AXL_FALSE if the function fails to parse the
 * document. AXL_TRUE if the test was properly executed.
 */
bool test_01 () 
{
	/* axl document representation */
	axlDoc   * doc;
	axlError * error;
	

	/* parse the given string */
	doc = axl_doc_parse ("<?xml version='1.0' ?>", 22, &error);
	if (doc == NULL) {
		printf ("An error was found while processing the document: code=%d, message=%s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);	

	doc = axl_doc_parse ("<?xml      version='1.0'            ?>", 38, &error);
	if (doc == NULL) {
		printf ("ERROR: code=%d, message=%s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return AXL_FALSE;
	}
	
	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml    \n   \t \n \r version='1.0' ?>", 39, &error);
	if (doc == NULL) {
		printf ("ERROR: code=%d, message=%s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml  version=\"1.0\"        ?>", 26, &error);
	if (doc == NULL) {
		printf ("ERROR: code=%d, message=%s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml  version=\"1.0\" \t \n \r encoding='utf-8\"   ?>", 53, &error);
	if (doc == NULL) {
		printf ("ERROR: code=%d, message=%s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
	}

	if (strcmp ("utf-8", axl_doc_get_encoding (doc))) {
		printf ("ERROR: encoding read from the document differs from the expected (got %s, expected %s)!\n",
			axl_doc_get_encoding (doc), "utf-8");
		return AXL_FALSE;
	}

	/* release document parsed */
	axl_doc_free (doc);

	doc = axl_doc_parse ("<?xml version='1.0' encoding='utf-8' standalone='yes' ?>", 56, &error);
	if (doc == NULL) {
		printf ("ERROR: code=%d, message=%s\n",
			axl_error_get_code (error), axl_error_get (error));
		return AXL_FALSE;
	}

	if (!axl_doc_get_standalone (doc)) {
		printf ("ERROR: Expected to receive a true standalone configuration but false was found\n");
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
	/* initialize axl library */
	if (! axl_init ()) {
		printf ("Unable to initialize Axl library\n");
		return -1;
	}

	
	if (test_01 ())
		printf ("Test 01: basic xml parsing [   OK   ]\n");
	else 
		printf ("Test 01: basic xml parsing [ FAILED ]\n");
	
	/* cleanup axl library */
	axl_end ();
	return 0;
}
