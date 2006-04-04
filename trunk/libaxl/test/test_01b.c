
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

int main (int argc, char ** argv)
{
	
	xmlDocPtr     doc;
	xmlNodePtr    cursor;

	/* init libXml */
	LIBXML_TEST_VERSION
	
	/* load the document */
	doc = xmlParseFile ("large.xml");
	if (doc == NULL) {
		printf ("Failed to load the xml document.\n");
		return -1;
	}

	/* release memory */
	xmlFreeDoc (doc);

	xmlCleanupParser();

	return 0;
}

