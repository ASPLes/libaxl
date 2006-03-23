#include <axl.h>
#include <stdio.h>

int main (int argc, char ** argv)
{
	axlError ** error;

	/* top level definitions */
	axlDoc * doc = NULL;

	/* initialize axl library */
	if (! axl_init ()) {
		printf ("Unable to initialize Axl library\n");
		return -1;
	}

	/* get current doc reference */
	doc = axl_doc_parse_from_file ("large.xml", error);
	if (doc == NULL)
		return AXL_FALSE;

	/* cleanup axl library */
	axl_end ();

	/* release the document */
	axl_doc_free (doc);

	return AXL_TRUE;
}
