/* include support from the base library */
#include <axl.h>

/* include support for ns */
#include <axl_ns.h>

/* declare our namespace */
#define HTML_NS "http://www.w3.org/1999/xhtml"

int main (int argc, char ** argv)
{
	axlDoc   * doc;
	axlNode  * node;
	axlError * error;

	/* init axl library */
	if (! axl_init ()) 
		return -1;

	/* parse xml document with namespace declarations */
	doc = axl_doc_parse_from_file ("test_28.xml", &error);
	if (doc == NULL) {
		printf ("Unable to read document: %s\n", axl_error_get (error));
		axl_error_free (error);
		return -1;
	} /* end if */

	/* call to validate namespace */
	if (! axl_ns_doc_validate (doc, &error)) {
		printf ("Namespace validation error: %s\n", axl_error_get (error));
		axl_error_free (error);
		return -1;
	} /* end if */

	/* get root document */
	node = axl_doc_get_root (doc);

	/* check default namespace */
	if (! axl_ns_node_cmp (node, HTML_NS, "table")) {
		printf ("expected to find a valid ns-node-cmp, but it wasn't found");
		return -1;
	}

	/* free document */
	axl_doc_free (doc);

	/* terminate axl execution */
	axl_end ();

	return 0;
}
