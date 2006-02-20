/**
 *  LibAxl:  Another XML library
 *  Copyright (C) 2006 Advanced Software Production Line, S.L.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to
 *  develop proprietary applications using this library without any
 *  royalty or fee but returning back any change, improvement or
 *  addition in the form of source code, project image, documentation
 *  patches, etc. 
 *
 *  For commercial support on build XML enabled solutions contact us:
 *          
 *      Postal address:
 *         Advanced Software Production Line, S.L.
 *         C/ Dr. Michavila Nº 14
 *         Coslada 28820 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://fact.aspl.es
 */

/**
 * @internal
 * @brief XML 1.0 Third edition grammar
 *
 * [1]  document       ::=   prolog element Misc*
 * [1]  status: partially
 *
 * [2]  Char           ::=   \x9 | \xA | \xD | \x20-\xD7FF | \xE000-\xFFFD | \x10000-\10FFFF
 * [2]  status: not implemented 
 *
 * [3]  S              ::= ( \x20 | \x9 | \xD | \xA)
 * [3]  status: ok
 *
 * [4]  NameChar       ::=   Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
 * [4]  status: not implemented
 *
 * [5]  Name           ::= ( Letter | '_' | ':' |) ( NameChar )*
 * [5]  status: not implemented
 *
 * [6]  Names          ::=   Name ( \x20 Name )*
 * [6]  status: not implemented
 *
 * [7]  Nmtoken        ::= ( NameChar ) +
 * [7]  status: not implemented
 *
 * [8]  Nmtokens       ::=   Nmtoken (\x20 Nmtoken)*
 * [8]  status: not implemented
 *
 * [9]  EntityValue    ::=   '"' ( [^%&"] | PEReference | Reference )* '"' | "'" ( [^%&'] ! PEReference | Reference )* "'"
 * [9]  status: not implemented
 *
 * [10] AttValue       ::=   '"' ( [^<&"] | Reference)*  '"' | "'" ( [^<&'] | Reference )* "'"
 * [10]  status: not implemented
 *
 * [11] SystemLiteral  ::= ( '"' [^"]* '"') | ("'" [^']* "'")
 * [11]  status: not implemented
 *
 * [12] PubidLiteral   ::=   '"' PubidChar* '"' | "'" (PubidChar - "'") * "'"
 * [12]  status: not implemented
 *
 * [13] PubidChar      ::=   \x20 | \xD | \xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
 * [13]  status: not implemented
 *
 * [14] CharData       ::=   [^<&]* - ([^<&]* ']]>' [^<&]*)
 * [14]  status: not implemented
 *
 * [15] Comments       ::=   '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
 * [15]  status: not implemented
 *
 * [16] PI             ::=   '<?' PITarget (S (Char* - (Char* '?<' Char*)))? '?>'
 * [16]  status: not implemented
 *
 * [17] PITarget       ::=   Name - (('X' | 'x') ('M' | 'm') | ('L' | 'l'))
 * [17]  status: not implemented
 *
 * [18] CDsect         ::=   CDStart CData CDend
 * [18]  status: not implemented
 *
 * [19] CDStart        ::=   '<![CDATA['
 * [19]  status: not implemented
 *
 * [20] CData          ::=   (Char* - (Char* ']]>' Char*))
 * [20]  status: not implemented
 *
 * [21] CDEnd          ::=   ']]>'
 * [21]  status: not implemented
 *
 * [22] prolog         ::=   XMLDecl? Misc* (doctypedecl Misc*)?
 * [22]  status: partially
 *
 * [23] XMLDecl        ::=   '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
 * [23]  status: ok
 *
 * [24] VersionInfo    ::=   S 'version' Eq ("'" VersionNum "'" | '"' VersionNum '"')
 * [24]  status: ok
 *
 * [25] Eq             ::=   S? '=' S?
 * [25]  status: ok
 *
 * [26] VersionNum     ::=   '1.0'
 * [26]  status: ok
 *
 * [27] Misc           ::=   Comment | PI | S
 * [27]  status: not implemented
 *
 * [28] doctypedecl    ::=   '<!DOCTYPE' S Name (S ExternalID)? S? ('[' intSubsect ']' S?)? '>'
 * [28]  status: not implemented
 *
 * [28a] DeclSep       ::=   PEReference | S
 * [28a]  status: not implemented
 *
 * [28b] intSubset     ::=   (markupdecl | DeclSep)*
 * [28b]  status: not implemented
 *
 * [29] markupdecl     ::=   elementdecl | AttlistDecl | EntityDecl | NotationDecl | PI | Comment
 * [29]  status: not implemented
 *
 * [30] extSubset      ::=   TextDecl? extSubsetDecl
 * [30]  status: not implemented
 *
 * [31] extSubsetDecl  ::=   ( markupdecl | conditionalSect | DeclSep) *
 * [31]  status: not implemented
 *
 * [32] SDDecl          ::=   S 'standalone' Eq (("'" ('yes' | 'no') "'") | ('"'" ('yes' | 'no') '"'))
 * [32]  status: ok
 *
 * 
 * ** productions 33 through 39 have been removed. It seems that this
 * ** productions were supporting xml:lang stuff that is easily
 * ** supported by using directily the xml standard rather than
 * ** mention it as an special production inside the language.
 *  
 * [39] element        ::=   EmptyElemTag | Stag content ETag
 * [39]  status: not implemented
 *
 * [40] Stag           ::=   '<' Name (S Attribute)* S? '>'
 * [40]  status: not implemented
 *
 * [41] Attribute      ::=   Name Eq AttValue
 * [41]  status: not implemented
 *
 * [42] ETag           ::=   '</' Name S? '>'
 * [42]  status: not implemented
 *
 * [43] content        ::=   CharData? ((element | Reference | CDSect | PI | Comment) CharData?)*
 * [43]  status: not implemented
 *
 * [44] EmptyElemTag   ::=   '<' Name (S Attribute)* S? '/>'
 * [44]  status: not implemented
 *
 * [45] elementdecl    ::=   '<!ELEMENT' S Name S contentspec S? '>' 
 * [45]  status: not implemented
 *
 * [46] contentspec    ::=   'EMPTY' | 'ANY' | Mixed | children
 * [46]  status: not implemented
 *
 * [47] children       ::=   (choice | seq) ('?' | '*' | '+')? 
 * [47]  status: not implemented
 *
 * [48] cp             ::=   (Name | choice | seq) ('?' | '*' | '+')? 
 * [48]  status: not implemented
 *
 * [49] choice         ::=   '(' S? cp ( S? '|' S? cp)+ S? ')'
 * [49]  status: not implemented
 *
 * [50] seq            ::=   '(' S? cp ( S? ',' S? cp )* S? ')'
 * [50]  status: not implemented
 *
 * [51] Mixed          ::=   '(' '#PCDATA' (S? '|' S? Name)* S? ')*' | '(' S? '#PCDATA' S? ')'
 * [51]  status: not implemented
 *
 * [52] AttlistDecl    ::=   '<!ATTLIST' S Name AttDef* S? '>'
 * [52]  status: not implemented
 *
 * [53] AttDef         ::=   S Name S AttType S DefaultDecl
 * [53]  status: not implemented
 *
 * [54] AttType        ::=   Stringtype | TokenizedType | Enumeratedtype
 * [54]  status: not implemented
 *
 * [55] StringType     ::=   'CDATA'
 * [55]  status: not implemented
 *
 * [56] tokenized      ::=   'ID' | 'IDREF' | 'IDREFS' | 'ENTITY' | 'ENTITIES' | 'NMTOKEN' | 'NMTOKENS'
 * [56]  status: not implemented
 *
 * [57] EnumeratedType ::=   NotationType | Enumeration
 * [57]  status: not implemented
 *
 * [58] NotationType   ::=   'NOTATION' S '(' S? Name (S? Name (S? '|' S? Name)* S? ')'
 * [58]  status: not implemented
 *
 * [59] Enumeration    ::=   '(' S? Nmtoken (S? '|' S? Nmtoken)* S? ')'
 * [59]  status: not implemented
 *
 * [60] DefaultDecl    ::=   '#REQUIRED' | '#IMPLIED' | (('#FIXED' S)? AttValue)
 * [60]  status: not implemented
 *
 * [61] conditionalSect  ::= includeSect | ignoreSect
 * [61]  status: not implemented
 *
 * [62] includeSect    ::= '<![' S? 'INCLUDE S? '[' extSubsetDecl ']]>'
 * [62]  status: not implemented
 *
 * [63] ignoreSect     ::=  <![' S? 'IGNORE' S? '[' ignoreSectContents* ']]>'
 * [63]  status: not implemented
 *
 * [64] ignoreSectContents ::=  Ignore ('<![' ignoreSectContents ']]>' Ignore) *
 * [64]  status: not implemented
 *
 * [65] Ignore         ::=  Char * - (Char * ('<!' | ']]>') Char *)
 * [65]  status: not implemented
 *
 * [66] CharRef        ::=  '&#' [0-9]+ ';' | '&#x' [0-9a-FA-F]+ ';'
 * [66]  status: not implemented
 *
 * [67] Reference      ::=  EntityRef | CharRef
 * [67]  status: not implemented
 *
 * [68] EntityRef      ::=  '&' Name ';'
 * [68]  status: not implemented
 *
 * [69] PEReference    ::=  '%' Name ';'
 * [69]  status: not implemented
 *
 * [70] EntityDecl     ::=  GEDecl | PEDecl
 * [70]  status: not implemented
 *
 * [71] GEDecl         ::=  '<!ENTITY' S Name S EntityDef S? '>'
 * [71]  status: not implemented
 *
 * [72] PEDecl         ::=  '<!ENTITY' S '%' S Name S PEDef S? '>'
 * [72]  status: not implemented
 *
 * [73] EntityDef      ::=  EntityValue | (ExternalID NDataDecl?)
 * [73]  status: not implemented
 *
 * [74] PEDef          ::=  EntityValue | ExternalID
 * [74]  status: not implemented
 *
 * [75] ExternalID     ::=  'SYSTEM' S SystemLiteral | 'PUBLIC' S PubidLiteral S SystemLiteral
 * [75]  status: not implemented
 *
 * [76] NDataDecl      ::=  S 'NData' S Name
 * [76]  status: not implemented
 *
 * [77] TextDecl       ::=  '<?xml' VersionInfo? EncodingDecl S? '?>'
 * [77]  status: not implemented
 *
 * [78] extParseEnt    ::=  TextDecl? content
 * [78]  status: not implemented
 *
 * [80] EncodingDecl   ::=  S 'encoding' Eq ( '"' EncName '"' | "'" EncName "'" )
 * [80]  status: ok
 *
 * [81] EncName        ::=  [A-Za-z] ([A-Za-z0-9._] | '-')*
 * [81]  status: ok
 *
 * [82] NotationalDecl ::=  '<!NOTATION' S Name S (ExternalID | PublicID) S? '>' 
 * [82]  status: not implemented
 *
 * [83] PublicID       ::=  'PUBLIC' S PubidLiteral
 * [83]  status: not implemented
 *
 * 
 * 
 * 
 */

#include <axl.h>

#define LOG_DOMAIN "axl-doc"

struct _axlDoc {
	/** 
	 * @internal
	 * @brief A reference to the very first axlNode this axlDoc
	 * has. 
	 */
	axlNode * rootNode;
	/** 
	 * @internal
	 * @brief Current xml encoding document.
	 */
	char    * encoding;
	
	/** 
	 * @internal
	 * @brief Current standalone configuration of the given \ref
	 * axlDoc object.
	 */
	bool      standalone;

	/** 
	 * @internal
	 *
	 * @brief Parent node stack. This stack is used to control how
	 * are nested nodes while creating/parsing xml files. This
	 * nesting allows to not only properly contruct the xml but
	 * also to check if it is well balanced.
	 */
	axlStack  * parentNode;
};

/** 
 * @internal
 * 
 * @brief Support for parsing the xml entity header 
 * 
 * @param stream The axlStream where is expected to receive the xml
 * header
 *
 * @param doc The axlDoc where the header configuration will be
 * placed.
 *
 * @param error An optional error that will be filled in the case an
 * error is found.
 *
 * @return It is supposed that the function return \ref AXL_TRUE, an
 * not deallocation is performed, and all elements were parsed
 * properly. In the case \ref AXL_FALSE is returned, memory associated
 * with the given stream will be released. If the document is
 * associated, it will also be released.
 */
bool __axl_doc_parse_xml_header (axlStream * stream, axlDoc * doc, axlError ** error)
{
	char      * string_aux;

	/* consume spaces */
	AXL_CONSUME_SPACES (stream);
	
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "looking for an xml header declaration");

	/* check for initial XMLDec (production 23) */
	if (axl_stream_inspect (stream, "<?")) {
		
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found xml declaration");

		/* check initial <?xml xml header */
		if (! (axl_stream_inspect (stream, "xml") > 0)) {
			axl_error_new (-2, "expected initial <?xml declaration, not found.", stream, error);
			axl_stream_free (stream);
			return AXL_FALSE;
		}
		
		/* consume spaces */
		AXL_CONSUME_SPACES (stream);

		if (! axl_stream_inspect (stream, "version=")) {
			axl_error_new (-2, "expected to find 'version=' declaration, not found.", stream, error);
			axl_stream_free (stream);
			return AXL_FALSE;
		}

		/* consume spaces */
		AXL_CONSUME_SPACES (stream);

		/* check for " or ' */
		if (! axl_stream_inspect_several (stream, 2, "\"1.0\"", "'1.0'")) {
			axl_error_new (-2, "expected to find either \" or ' while procesing version number, not found.", stream, error);
			axl_stream_free (stream);
			return AXL_FALSE;
		}

		/* check for an space */
		AXL_CONSUME_SPACES(stream);

		/* now check for encoding */
		if (axl_stream_inspect_several (stream, 2, "encoding=\"", "encoding='") > 0) {

			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found encoding declaration");

			/* found encoding instruction */
			string_aux = axl_stream_get_until (stream, NULL, NULL, 2, "'", "\"");
			if (string_aux == NULL) {
				axl_error_new (-2, "expected encoding value, not found.", stream, error);
				axl_stream_free (stream);
				return AXL_FALSE;
			}
			
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "encoding found=%s", string_aux);

			/* set document encoding */
			doc->encoding = axl_strdup (string_aux);
		}

		/* check for an space */
		AXL_CONSUME_SPACES(stream);

		/* get standalone configuration */
		if ((axl_stream_inspect_several (stream, 2, "standalone=\"", "standalone='") > 0)) {
			
			/* found standalone instruction */
			string_aux = axl_stream_get_until (stream, NULL, NULL, 2, "'", "\"");
			if (string_aux == NULL) {
				axl_error_new (-2, "expected to receive standalone value, not found.", stream, error);
				axl_stream_free (stream);
				return AXL_FALSE;
			}

			/* set standalone configuration */
			if (memcmp ("yes", string_aux, 3))
				doc->standalone = AXL_FALSE;
			else
				doc->standalone = AXL_TRUE;
		}
		
		/* check for an space */
		AXL_CONSUME_SPACES(stream);

		/* get the trailing header */
		if (! (axl_stream_inspect (stream, "?>") > 0)) {
			axl_error_new (-2, "expected to receive the xml trailing header ?>, not found.", stream, error);
			axl_stream_free (stream);
			return AXL_FALSE;
		}
	}
	
	/* return TRUE value */
	return AXL_TRUE;
}

/** 
 * @internal
 *
 * @brief Tries to parse the first (and compulsory) node that the xml
 * document must have.
 *
 * The very minimal expresion of an xml document is the one defined by
 * only one node, with no content and no attributes. This minimal xml
 * could be defined as:
 *
 * \code
 *   <hello/>
 * \endcode
 *
 * Or the other form accepted:
 *
 * \code
 *   <hello />
 * \endcode
 *
 * 
 * 
 * 
 * @param stream The \ref axlStream object where is expected to find
 * the xml node content.
 *
 * @param doc The \ref axlDoc object where node read will be placed
 * inside.
 * 
 * @param node The node that has been added due to calling to this
 * function.
 *
 * @param error An optional error reporting variable to used to report
 * upper level the error found.
 * 
 * @return AXL_TRUE if the first node was successfully parsed or
 * AXL_FALSE if not. If the function find something wrong the document
 * is unrefered.
 */
bool __axl_doc_parse_node (axlStream * stream, axlDoc * doc, axlNode ** calling_node, axlError ** error)
{
	char    * string_aux;
	axlNode * node;
	int       matched_chunk;
	
	axl_return_val_if_fail (stream, AXL_FALSE);
	axl_return_val_if_fail (doc, AXL_FALSE);
	
	/* get rid from spaces */
	AXL_CONSUME_SPACES(stream);

	/* check for initial < definition */
	if (! (axl_stream_inspect (stream, "<") > 0) && ! axl_stream_remains (stream)) {
		/* check if we are reading the first node node */
		if (doc->rootNode == NULL)
			axl_error_new (-2, "expected initial < for a root node definition, not found. An xml document must have, at least, one node definition.", 
				       stream, error);
		else
			axl_error_new (-2, "expected initial < for a node definition, not found.", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}

	/* get node name */
	string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, 5, " />", "/>", ">", " >", " ");
	if (AXL_IS_STR_EMPTY (string_aux)) {
		axl_error_new (-2, "expected an non empty content for the node name not found.", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}

	/* create the node and associate it */
	node           = axl_node_create (string_aux);
	if (doc->rootNode == NULL) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting as first node found, the root node: <%s>", string_aux);
		doc->rootNode  = node;
		axl_stack_push (doc->parentNode, node);
	} else {
		/* or set the node as a child of the current parent */
		axl_doc_set_child_current_parent (doc, node);
	}

	/* set the node created to the calling node, so the caller
	 * could get a reference */
	if (calling_node != NULL)
		*calling_node = node;

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node found: %s", string_aux);

	/* know, until the node ends, we have to find the node
	 * attributes or the node defintion end */
	while (axl_stream_remains (stream)) {

		/* get rid from spaces */
		AXL_CONSUME_SPACES (stream);
		
		/* check if we have an attribute for the node, or the node
		 * definition have ended or the node definition is an empty
		 * one 
		 * 
		 * the following code that relies on matched_chunk is
		 * done due to previous call to get_until function. If
		 * the value 0 or 1 was matched, this means that we
		 * are on "/>" case */
		if ((matched_chunk == 0) ||
		    (matched_chunk == 1) ||
		    axl_stream_inspect_several (stream, 2, "/>", " />") > 0) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found end xml node definition '/>'");
			/* empty node configuration found */
			axl_node_set_is_empty (node, AXL_TRUE);
			axl_node_set_have_childs (node, AXL_FALSE);

			/* make this node to be complated and no child could be set. */
			axl_doc_pop_current_parent (doc);
			return AXL_TRUE;
		}
		
		/* check if we have an attribute for the node, or the node
		 * definition have ended or the node definition is an empty
		 * one 
		 * 
		 * the following code that relies on matched_chunk is
		 * done due to previous call to get_until function. If
		 * the value 2 or 3 was matched, this means that we
		 * are on ">" case */
		if ((matched_chunk == 2) ||
		    (matched_chunk == 3) ||
		    axl_stream_inspect_several (stream, 2, ">", " >") > 0) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found end xml node definition '>'");
			axl_node_set_have_childs (node, AXL_TRUE);
			/* this node is ended */
			return AXL_TRUE;
		}
		
		/* found attribute declaration, try to read it */
	}

	/* document properly parsed */
	return AXL_TRUE;
}

/** 
 * @internal
 * @brief Perform the close node operation.
 *
 */
bool __axl_doc_parse_close_node (axlStream * stream, axlDoc * doc, axlNode ** node, axlError ** error)
{
	return AXL_FALSE;
}



/** 
 * @brief Parse an XML entity that is hold inside the memory pointed
 * by <b>entity</b> and limited by <b>entity_size</b>.
 *
 * The function parses the XML document inside the memory hold inside
 * the given reference. The function returns an XML document,
 * represented by \ref axlDoc.
 *
 * The function, optionall, could report error found inside the given
 * \ref axlError variable. In the case the function returns a NULL
 * value, this variable is filled containing the a textual diagnostic
 * error to be showed to the user interface and an error code.
 * 
 * @param entity The XML document to load.
 *
 * @param entity_size The XML document size to load. If a <b>-1</b> is
 * provided, strlen function is used to figure out current document
 * size. This is not recomended while using xml documents that include
 * binary data, that maybe comes inside the CDATA section or because
 * an utf caracter used that includes the \0 inside its value.
 * 
 * @return A newly allocated Axl Document, that must be deallocated
 * using \ref axl_doc_free, when no longer needed. The function could
 * return NULL if the document is not loaded properly.
 *
 * In the case an error is found while procesing the document, error
 * variable will be filled, if defined. -1 will be returned is
 * received parameter are wrong. -2 will be returned if there some
 * error is found while processing the document.
 */
axlDoc * axl_doc_parse (char * entity, int entity_size, axlError ** error)
{
	axlStream * stream    = NULL;
	axlDoc    * doc       = NULL;
	axlNode   * node      = NULL;
	int         iterator  = 0;
		
	
	/* check for environmental parameters */
	if (entity == NULL) {
		axl_error_new (-1, "Received and empty xml stream.", stream, error);
		return NULL;
	}

	if ((entity_size != -1) && (entity_size <= 0)) {
		axl_error_new (-1, "Received an entity size that is less or equal to 0, and it is not -1.", stream, error);
		return NULL;
	}

	/* create the xml stream using provided data */
	if (entity_size == -1)
		stream = axl_stream_new (entity, strlen (entity));
	else
		stream = axl_stream_new (entity, entity_size);
	doc            = axl_new (axlDoc, 1);
	axl_stream_link (stream, doc);

	/* parse initial xml header */
	if (!__axl_doc_parse_xml_header (stream, doc, error))
		return NULL;
	
	/* parse the rest of the document */
	if (!__axl_doc_parse_node (stream, doc, &node, error))
		return NULL;

	/* if the node returned is not empty */
	if (! axl_node_is_empty (node)) {

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the first node ready, have content, reading it");

		/* while the stream have data */
		while (axl_stream_remains (stream)) {
			
			/* consume white spaces */
			AXL_CONSUME_SPACES (stream);

			if (axl_stream_peek (stream, "</") > 0) {
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a node termination signal");
				/* seems that a node is being closed */
				if (! __axl_doc_parse_close_node (stream, doc, &node, error))
					return NULL;
				continue;
			}

			if (axl_stream_peek (stream, "<") > 0) {
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a new node being opened");
				/* seems that another node is being opened */
				if (!__axl_doc_parse_node (stream, doc, &node, error))
					return NULL;
				continue;
			}

			/* found node content */
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a node content");
			if (iterator == 3) {
				axl_error_new (-1, "not implemented xml node content parsing..", stream, error);
				return NULL;
			}
			iterator++;
		}
	}

	/* parse complete */
	axl_stream_unlink (stream);
	axl_stream_free (stream);
	return doc;
}



/** 
 * @brief Gets current axl Document encoding.
 * 
 * @param doc The document where the encoding will be retrieved.
 * 
 * @return A valid \ref axlDoc reference. NULL is returned in the case
 * a NULL \ref axlDoc reference is received. The value returned by
 * this function must not be deallocated.
 */
char   * axl_doc_get_encoding (axlDoc * doc)
{
	/* check parameter received */
	axl_return_val_if_fail (doc, NULL);
	
	return (doc->encoding != NULL) ? doc->encoding : "";
}

/** 
 * @brief Allows to get current standalone configuration for the given
 * axlDoc document.
 * 
 * @param doc The \ref axlDoc document where the standalone value will
 * be retreived.
 * 
 * @return \ref AXL_TRUE if the standalone configuration, found inside
 * the xml header is set to TRUE. Otherwise \ref AXL_FALSE is
 * returned. Keep in mind that the function will return an \ref
 * AXL_FALSE value if a null reference is received.
 */
bool     axl_doc_get_standalone (axlDoc * doc)
{
	axl_return_val_if_fail (doc, AXL_FALSE);

	/* return current configuration */
	return doc->standalone;
}

/** 
 * @brief Allows to set the given axlNode to be child of the current
 * parent.
 *
 * @param doc The \ref axlDoc reference where the \ref axlNode will be
 * configured.
 *
 * @param node The \ref axlNode reference to set as a child for the
 * parent node.
 */
void     axl_doc_set_child_current_parent (axlDoc * doc, axlNode * node)
{
	axlNode * parent;

	/* perform some environment checks */
	axl_return_if_fail (doc);
	axl_return_if_fail (node);
	
	parent = axl_stack_peek (doc->parentNode);
	axl_return_if_fail (parent);

	/* set the child for the current parent */
	axl_node_set_child (parent, node);

	/* set the new parent */
	axl_stack_push (doc->parentNode, node);

	return;
}

/** 
 * @brief Allows to make current axlDocument to pop current parent
 * node, making the new parent node the previously opened.
 * 
 * @param doc The \ref axlDoc where the pop operation will be
 * performed.
 */
void     axl_doc_pop_current_parent       (axlDoc * doc)
{
	axl_return_if_fail (doc);

	/* pop current parent */
	axl_stack_pop (doc->parentNode);

	return;
}


/** 
 * @brief Releases memory allocated by the \ref axlDoc object.
 * 
 * @param doc The \ref axlDoc object to unref.
 */
void     axl_doc_free         (axlDoc * doc)
{
	/* do not complain if an axlDoc reference is received */
	if (doc == NULL)
		return;

	/* free first root node */
	if (doc->rootNode != NULL)
		axl_node_free (doc->rootNode);

	/* free enconding allocated */
	axl_free (doc->encoding);

	/* free document allocated */
	axl_free (doc);

	return;
}

