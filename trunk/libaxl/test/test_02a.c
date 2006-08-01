#include <axl.h>
#include <stdio.h>

int main (int argc, char ** argv)
{
	axlError * error = NULL;
	axlDtd   * dtd   = NULL;

	/* top level definitions */
	axlDoc * doc = NULL;

	/* initialize axl library */
	if (! axl_init ()) {
		printf ("Unable to initialize Axl library\n");
		return -1;
	}

	/* get current doc reference */
	doc = axl_doc_parse_from_file ("gmovil2.xml", &error);
	if (doc == NULL) {
		printf ("Unable to parse file, error was: %s\n", axl_error_get (error));
		axl_error_free (error);
		return AXL_FALSE;
	}

	/* parse dtd file */
	dtd = axl_dtd_parse_from_file ("fact.dtd", &error);
	if (dtd == NULL) {
		printf ("Unable to parse DTD file, error was: %s\n", axl_error_get (error));
		return false;
	}

	/* perform DTD validation */
	if (! axl_dtd_validate (doc, dtd, &error)) {
		printf ("Unable to validate file, error was: %s\n", axl_error_get (error));
		return false;
	}

	/* release the document */
	axl_doc_free (doc);

	axl_dtd_free (dtd);

	/* cleanup axl library */
	axl_end ();

	return AXL_TRUE;
}
