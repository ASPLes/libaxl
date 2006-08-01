#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

int main (int argc, char ** argv)
{
	
	xmlDocPtr       doc;
	xmlDtdPtr       dtd;
	xmlValidCtxtPtr ctx;

	/* init libXml */
	LIBXML_TEST_VERSION
	
	/* load the document */
	doc = xmlParseFile ("gmovil2.xml");
	if (doc == NULL) {
		printf ("Failed to load the xml document.\n");
		return -1;
	}

	/* parse DTD document */
	dtd = xmlParseDTD ("fact.dtd", "fact.dtd");
	if (dtd == NULL) {
		printf ("Failed to load DTD file.\n");
		return -1;
	}
	
	/* validate document */
	printf ("document loaded..\n");
	ctx = xmlNewValidCtxt ();
	
	if (! xmlValidateDtd (ctx, doc, dtd)) {
		printf ("DTD is not valid!!\n");
		return -1;
	}
	
	printf ("document is valid..\n");

	/* free context */
	xmlFreeValidCtxt (ctx);

	/* release memory */
	xmlFreeDoc (doc);

	/* release the dtd */
	xmlFreeDtd (dtd);

	xmlCleanupParser();

	return 0;
}

