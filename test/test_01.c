#include <axl.h>

/** 
 * Test01: Initial xml header checking.
 */
int main (int argc, char ** argv) 
{
	/* axl document representation */
	axlDoc   * doc;
	axlError * error;
	

	doc = axl_doc_parse ("<?xml version='1.0' ?>", 22, &error);
	if (doc == NULL) {
		printf ("An error was found while processing the document: code=%d, message=%s\n",
			axl_error_get_code (error), axl_error_get (error));
		axl_error_free (error);
		return -1;
	}
	printf ("Axl have properly parsed the document ..\n");
	
	
	return 0;
}
