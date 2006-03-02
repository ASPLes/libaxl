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

/**
 * \defgroup axl_doc_module Axl XML Document: XML Documents related functions, loading XML documents and using them.
 */

/** 
 * \addtogroup axl_doc_module
 * @{
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

	/** 
	 * @internal
	 * 
	 * @brief Internal list to hold all PI targets readed.
	 */
	axlList   * piTargets;

	/** 
	 * @internal
	 *
	 * @brief Instruct the \ref axlDoc instance to notify that the
	 * xml header have been defined. This helps to allow define PI
	 * instruction that are only found inside the root document,
	 * or after the xml header definition.
	 */
	bool       headerProcess;
};

struct _axlPI {
	/** 
	 * @internal
	 * 
	 * @brief PI Target name.
	 */
	char * name;
	/** 
	 * @internal
	 * 
	 * @brief PI target content.
	 */
	char * content;
};


/** 
 * @internal
 * @brief Creates a new empty \ref axlDoc reference.
 * 
 * @return A newly allocated \ref axlDoc reference.
 */
axlDoc * __axl_doc_new () 
{
	axlDoc    * result = axl_new (axlDoc, 1);

	result->parentNode = axl_stack_new (NULL);
	result->piTargets  = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_pi_free);

	return result;
}

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

	/* check if the user is defining the header many times */
	if (doc->headerProcess) {
		axl_error_new (-1, "Found a new xml header expecification. Only one header is allowed for each xml document.", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}

	/* consume comments found */
	if (! axl_doc_consume_comments (doc, stream, error))
		return AXL_FALSE;
	
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
			string_aux = axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 2, "'", "\"");
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
			string_aux = axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 2, "'", "\"");
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

		/* consume a possible comment */
		if (! axl_doc_consume_comments (doc, stream, error))
			return AXL_FALSE;
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
	char    * string_aux2;
	axlNode * node;
	int       matched_chunk;
	int       iterator;
	
	axl_return_val_if_fail (stream, AXL_FALSE);
	axl_return_val_if_fail (doc, AXL_FALSE);
	
	/* consume a possible comment */
	if (! axl_doc_consume_comments (doc, stream, error))
		return AXL_FALSE;

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
	string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, AXL_TRUE, 5, " />", "/>", ">", " >", " ");
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
	iterator = 0;
	while (axl_stream_remains (stream)) {

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
		    axl_stream_inspect_several (stream, 2, " />", "/>") > 0) {
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
		    axl_stream_inspect_several (stream, 2, " >", ">") > 0) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found end xml node definition '>'");
			axl_node_set_have_childs (node, AXL_TRUE);
			/* this node is ended */
			return AXL_TRUE;
		}
		
		/* get rid from spaces */
		AXL_CONSUME_SPACES (stream);

		/* found attribute declaration, try to read it */
		string_aux = axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 3, "='", "=\"", "=");
		if (string_aux != NULL) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "attribute found: %s", string_aux);
			/* copy attribute found */
			string_aux = axl_strdup (string_aux);

			/* now get the attribute value */
			string_aux2 = axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 5, "' ", "\" ", "'", "\"", " ");
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "value found: %s", string_aux2);

			/* set a new attribute for the given node */
			axl_node_set_attribute (node, string_aux, string_aux2);

			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "attribute installed..");
			
			/* only deallocate the string_aux, because
			 * string_aux2 is automatically handled by the
			 * library */
			axl_free (string_aux);
			
			continue;
		}
			

		/* do not iterate for ever */
		if (iterator == 3) {
			axl_error_new (-2, "Parse error while reading a node being opened", stream, error);
			axl_stream_free (stream);
			return AXL_FALSE;
		}
		iterator++;
	}

	/* document properly parsed */
	return AXL_TRUE;
}

/** 
 * @internal
 * @brief Perform the close node operation.
 *
 */
bool __axl_doc_parse_close_node (axlStream * stream, axlDoc * doc, axlNode ** _node, axlError ** error)
{
	char    * string;
	axlNode * node;

	/* get the node being closed to check to the current parent */
	string = axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 2, " >", ">");
	if (string == NULL) {
		axl_error_new (-1, "An error was found while closing the xml node", stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}

	/* get current parent node */
	node   = axl_stack_peek (doc->parentNode);
	if (node == NULL) {
		axl_error_new (-1, "Found that the stack doesn't have any node opened, this means either an libaxl error or the xml being read is closing a node not opened",
			       stream, error);
		axl_stream_free (stream);
		return AXL_FALSE;
	}

	if (NODE_CMP_NAME (node, string)) {
		/* ok, axl node to be closed is the one expected */
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "closing xml node, that matched with parent opened");
		return AXL_TRUE;
	}

	/* seems that the node being closed doesn't match */
	axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "xml node names to be closed doesn't matched (%s != %s), current node stack status:",
		 axl_node_get_name (node), string);

	node = axl_stack_pop (doc->parentNode);
	while (node != NULL) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "<%s>", axl_node_get_name (node));
		node = axl_stack_pop (doc->parentNode);
	}

	axl_error_new (-1, "An error was found while closing the opened xml node, parent opened and xml node being closed doesn't match",
		       stream, error);
	axl_stream_free (stream);
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
 * an utf caracter used that includes the \\0 inside its value.
 *
 * @param error Optional \ref axlError reference that will be used to
 * report errors found while processing xml into the \ref axlDoc
 * instance.
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
	axlStream * stream        = NULL;
	axlDoc    * doc           = NULL;
	axlNode   * node          = NULL;
	char      * string        = NULL;
	int         index;
	int         chunk_matched = 0;
		
	
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
	stream = axl_stream_new (entity, entity_size, NULL, -1, error);
	axl_return_val_if_fail (stream, NULL);
	doc            = __axl_doc_new ();
	axl_stream_link (stream, doc, (axlDestroyFunc) axl_doc_free);

	/* parse initial xml header */
	if (!__axl_doc_parse_xml_header (stream, doc, error))
		return NULL;

	/* signal that this document have processed its header */
	doc->headerProcess = AXL_TRUE;
	
	/* parse the rest of the document */
	if (!__axl_doc_parse_node (stream, doc, &node, error))
		return NULL;

	/* set the node read, the root one, to be the parent */
	axl_stack_push (doc->parentNode, node);

	/* if the node returned is not empty */
	if (! axl_node_is_empty (node)) {

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the first node ready, have content, reading it");

		/* while the stream have data */
		while (axl_stream_remains (stream)) {

			/* get current index */
			index = axl_stream_get_index (stream);
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "current index: %d", index);

			/* consume a possible comment */
			if (! axl_doc_consume_comments (doc, stream, error))
				return AXL_FALSE;
			
			if ((axl_stream_peek (stream, "</") > 0)) {
				/* accept previous peek */
				axl_stream_accept (stream);

				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a node termination signal");
				/* seems that a node is being closed */
				if (! __axl_doc_parse_close_node (stream, doc, &node, error))
					return NULL;

				/* because the xml node have been
				 * closed, make the parent to be the
				 * previous one */
				axl_stack_pop (doc->parentNode);
				continue;
			}

			/* check here for CDATA section. This is done
			 * here because the following checking could
			 * be mixed because they starts with the same:
			 * < */
			if ((axl_stream_peek (stream, "<![CDATA[") > 0)) {
				/* accet previous peek */
				axl_stream_accept (stream);

				/* found CDATA section, get current content */
				string = axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 1, "]]>");
				if (string == NULL) {
					axl_error_new (-1, "Unable to get CDATA content. There was an error.", stream, error);
					axl_stream_free (stream);
					return NULL;
				}

				/* set current data */
				axl_node_set_content (node, string, -1);
				continue;
			}


			if ((axl_stream_peek (stream, "<") > 0)) {
				/* accept previous peek */
				axl_stream_accept (stream);

				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a new node being opened");
				/* seems that another node is being opened */
				if (!__axl_doc_parse_node (stream, doc, &node, error))
					return NULL;

				continue;
			}
			
			/* restore index position previous to the axl
			 * space consuming */
			axl_stream_move (stream, index);
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "before restoring index: %d", index);
			
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "seems that xml node content was found, while reading -->'%s'",
				 axl_stream_get_following (stream, 20));
			/* found node content */
			chunk_matched = 0;
			string = axl_stream_get_until (stream, NULL, &chunk_matched, AXL_FALSE, 3, "</", "<", ">");
			/* check for a null content found */
			if (string == NULL) {
				axl_error_new (-1, "an error was found while reading the xml node content", stream, error);
				axl_stream_free (stream);
				return NULL;
			}

			/* check for a not properly formed xml document */
			if (chunk_matched == 2) {
				axl_error_new (-1, "found a closing node definition '>' were expected '<' or '</'", stream, error);
				axl_stream_free (stream);
				return NULL;
			}


			/* store content inside the xml node */
			axl_node_set_content (node, string, -1);

			/* keep on looping */
		}
	}

	/* pop axl parent */
	axl_stack_pop (doc->parentNode);
	if (! axl_stack_is_empty (doc->parentNode)) {
		axl_error_new (-1, "XML document is not balanced, still remains xml nodes", stream, error);
		axl_stream_free (stream);
		return NULL;
	}

	/* parse complete */
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "xml document parse COMPLETED"); 
	axl_stream_unlink (stream);
	axl_stream_free (stream);
	return doc;
}

/** 
 * @brief Allows to parse an xml document that is provided as a set of
 * strings ended by a NULL reference.
 *
 * This function works the same way like \ref axl_doc_parse function,
 * but allowing to provide a set of strings. Here is an example:
 * 
 * \code
 * // a document reference
 * axlDoc   * doc;
 *
 * // note that the error is optional, and, if provided, it is not
 * // required to initialize it.
 * axlError * error;
 * 
 * // parse the following set of strings
 * doc = axl_doc_parse_strings (&error, 
 *                              "<?xml version='1.0' standalone='yes' ?>",
 *                              "<complex>",
 *                              "  <data>",
 *                              "     <row>",
 *                              "       <td>",
 *                              "          <value attr='10'/>
 *                              "       </td>",
 *                              "     </row>",
 *                              "  </data>",
 *                              "</complex>",
 *                              NULL); // last null reference 
 * // check for an error
 * if (doc == NULL) {
 *      printf ("There was an error while parsing the document: (code: %d) %s\n",
 *              axl_error_get_code (error), axl_error_get (error));
 *      axl_error_free (error);
 * }
 * \endcode
 * 
 * @param error An optional \ref axlError reference where a textual
 * diagnostic will be provided.
 * 
 * @return A newly created \ref axlDoc reference that must be
 * deallocated by using \ref axl_doc_free when no longer needed.
 */
axlDoc  * axl_doc_parse_strings            (axlError ** error,
					    ...)
{
	axlDoc   * doc;
	va_list    args;
	char     * string     = NULL;
	char     * stream     = NULL;
	char     * stream_aux = NULL;
	
	/* check incoming data */
	axl_return_val_if_fail (error, NULL);
	
	/* open the stdargs */
	va_start (args, error);
	
	while ((string = va_arg (args, char *)) != NULL) {
		stream_aux = stream;
		stream = axl_stream_concat (stream, string);
		if (stream_aux != NULL) {
			axl_free (stream_aux);
			stream_aux = NULL;
		}
	}

	/* close the stdargs */
	va_end (args);

	/* check that we have received, at least, an string
	 * parseable */
	if (stream == NULL)
		return NULL;

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "string to parse: %s", stream);

	/* parse the string */
	doc = axl_doc_parse (stream, -1, error);

	/* free the stream */
	axl_free (stream);

	return doc;
}


/** 
 * @brief Allows to get current root node for the given xml document,
 * represented by the \ref axlDoc instance.
 *
 * Every XML document has a very first node, which enclose all childs
 * found inside the document, that is called the root node. This xml
 * node, 
 *
 * This function couldn't return NULL because every well opened xml
 * document, always have a root node. However, the function could
 * return a NULL value if the document received is a null reference.
 * 
 * @param doc The xml document (\ref axlDoc) where the root node will
 * be returned. 
 * 
 * @return The root node (\ref axlNode) or NULL if fails.
 */
axlNode * axl_doc_get_root                 (axlDoc * doc)
{
	axl_return_val_if_fail (doc, NULL);
	
	/* return current root node */
	return doc->rootNode;
}

/** 
 * @internal
 *
 * @brief An always return 1 to make the list to store elements append
 * at the end.
 */
int __axl_doc_get_are_equal (axlPointer a, axlPointer b)
{
	return 1;
}

/** 
 * @brief Allows to get a particular node (or list of nodes) that are
 * located at a selected path.
 *
 * Providing a path, the function lookups for nodes stored on the
 * selected location inside the provided document. The path provided
 * doesn't follow the XPath extension. 
 *
 * Taking as a reference for the xml to be explained on the following
 * rules:
 * \code
 * <complex>
 *     <data>
 *       <node>
 *          <row>10</row>
 *       </node>
 *     </data>
 * </complex>
 * \endcode
 *
 *
 * Here is how the path works:
 * 
 * - If provided a "/", the root node is returned. This is same than
 * provided the root node name, like "/complex", when it is expected
 * to find as root node &lt;complex>. However, providing a particular
 * node to search allows to get ensure that the root node is the one
 * looked up.
 *
 * - To select nodes inside the first root node, in a generic way,
 * without providing details about the root node name, you could use
 * "//\*". This will provide all nodes that are found inside the root
 * node, whatever it is called. If it is required to get all nodes,
 * inside the root node, ensuring that the root one is called
 * "complex" you should use: "/complex/\*"
 *
 * - If it is required to get a selected node inside the root node,
 * that is called in a particular way you can use: //data. Again, if
 * it is required to ensure that a particular node exists, from the
 * top level down the leaf node, it will required to write something
 * like: "/complex/data". 
 *
 * - Remember that is totally different to query for "/complex/data"
 * than "/complex/data/\*". The first one, returns the node, or nodes,
 * called <b>data</b> that are inside the root node called
 * <b>complex</b>, while the second one says: return all nodes inside
 * the node <b>data</b> that is inside the root node called
 * <b>complex</b>
 *
 * Finally, keep in mind that this function only returns nodes. To get
 * node content, attributes or anything else, you'll have to get the
 * node first and then operate with it.
 *
 *
 *
 * @param doc The \ref axlDoc reference where the lookup will be
 * performed.
 *
 * @param path_to A path to the node (nodes) that are inside the path
 * especifyied.
 * 
 * @return A list of nodes (\ref axlNode) if the case something is
 * found or NULL if fails to find something at the given path. If the
 * path is right but no node match with it or there is no node, the
 * function will return NULL reference rather a list with no
 * nodes. Returned value must be deallocated by using \ref
 * axl_list_free.
 */
axlList * axl_doc_get_list                  (axlDoc * doc, char * path_to)
{
	axlList  * nodes;
	axlNode  * node     = NULL;
	int        iterator = 0;
	char    ** paths    = 0;
	

	axl_return_val_if_fail (doc, NULL);
	axl_return_val_if_fail (path_to, NULL);
	axl_return_val_if_fail (path_to[0] == '/', NULL);

	/* create the axl list */
	nodes = axl_list_new (__axl_doc_get_are_equal, NULL);
	
	/* split paths */
	paths = axl_stream_split (path_to, 1, "/");
	axl_return_val_if_fail (paths, nodes);

	/* get a reference to the root node */
	node = doc->rootNode;

	/* basic case, check for the root node */
	if (strlen (paths[1]) != 0) {
		/* check the node is the one requested */
		if (! NODE_CMP_NAME (node, paths[1])) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "requested root node = %s wasn't found, current root %s", paths[1],
				 axl_node_get_name (doc->rootNode));
			axl_list_free (nodes);
			axl_stream_freev (paths);
			return NULL;
		}
	}

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found node: %s", paths[1]);

	/* now the general case */
	iterator = 2;
	while ((paths[iterator] != NULL) && (strlen (paths[iterator]) > 0)) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking path item %s", paths[iterator]);
		
		/* check that the last path is used */
		if (axl_cmp (paths[iterator], "*") && 
		    (axl_stream_strv_num (paths) != iterator + 1)) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "using the '*' at that path different from the last one.", paths[iterator]);
			axl_list_free (nodes);
			axl_stream_freev (paths);
			return NULL;
		}

		/* get a reference to the node searched */
		node = axl_node_get_child_called (node, paths[iterator]);
		if (node == NULL) {
			axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the node located at %s wasn't found.", path_to);
			axl_list_free (nodes);
			axl_stream_freev (paths);
			return NULL;
		}
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found node: %s", paths[iterator]);

		/* update iterator value */
		iterator++;
	}

	/* add the node found */
	axl_list_add (nodes, node);

	/* free paths */
	axl_stream_freev (paths);

	/* return the node found */
	return nodes;
}

/** 
 * @brief Allows to return only one node for the selected path.
 *
 * This function works the same way like \ref axl_doc_get_list but
 * extracting the first node reference found inside the list returned
 * by the previous function, and returning it.
 *
 * Many times, a path is selected because it is know that under that
 * location couldn't be more than one element. However, using \ref
 * axl_doc_get_list function makes this task really anoying because
 * you have to get the list, extract the node, from the list, and
 * releasing the list reference to actually get access to the node
 * looked up.
 *
 * This function allows you to get access to the node stored on the
 * selected location, and, if a path that provides several nodes is
 * returned, only the first node found on that list is returned.
 * 
 * @param doc The \ref axlDoc document where the node will be returned.
 * @param path_to A path to the node to get.
 * 
 * @return A reference to a \ref axlNode instace, or NULL if
 * fails. Returned reference must not be deallocated.
 */
axlNode * axl_doc_get                      (axlDoc * doc, char * path_to)
{
	axlList * list = NULL;
	axlNode * node = NULL;
	
	axl_return_val_if_fail (doc, NULL);
	axl_return_val_if_fail (path_to, NULL);

	/* get the list of nodes */
	list = axl_doc_get_list (doc, path_to);
	if (list == NULL)
		return NULL;
	
	/* get the node requested */
	if (axl_list_length (list) > 0)
		node = axl_list_get_nth (list, 0);

	axl_list_free (list);
	return node;
	
	
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
 * @brief Allows to configure a PI target, with its content, on the given \ref axlDoc.
 *
 * A PI is a process instruction that is passed to the
 * application. This process instruction has a target name, which
 * acording to the standard is the application which should receive
 * the target content and an optional target content associated.
 *
 * This function allows to configure (add) a new PI item inside the
 * given xml document (\ref axlDoc). The PI content is optional. If
 * provided NULL, the PI will only contain as information the PI
 * target.
 *
 * Here is how a process instruction is used inside a xml document:
 * \code
 * <?xml version='1.0'>
 * <?launch "some command" ?>
 * <complex>
 *    <?data "some data" ?>
 *    <?data "more data" ?>
 *    <data>
 *       <row attr="20" />
 *    </data>
 * </complex>
 * \endcode
 *
 * Previous example shows how to use PI (process instructions) from
 * outside the xml root node (<b>complex</b>, and also how it is used
 * from inside a xml node definition <b>complex</b>. 
 *
 * As you can see, PI elements could be used as many times as you want
 * and places allowed to do so are just right before begining with the
 * root node and inside xml node definitions.
 * 
 * @param doc The axlDocument where the PI will be added.
 * @param target The PI target name.
 * @param content The PI content (optional value).
 */
void      axl_doc_add_pi_target            (axlDoc * doc, 
					    char * target, 
					    char * content)
{
	axlPI * pi;

	/* perform some environmental checks */
	axl_return_if_fail (doc);
	axl_return_if_fail (target);

	/* create the PI element */
	pi = axl_pi_create (target, content);

	/* add the PI */
	axl_list_add (doc->piTargets, pi);

	return;
}

/** 
 * @brief Allows to check if the provided Processing instruction
 * target is defined on the given xml document (\ref axlDoc).
 *
 * Processing instruction are a way to configure the xml document with
 * processing information to instruct the application level that is
 * going to consume the XML information.
 *
 * @param doc The document where the processing instruction will be read.
 *
 * @param pi_target The process instruction name.
 * 
 * @return AXL_TRUE is the processing instruction is defined,
 * otherwise AXL_FALSE is returned.
 */
bool      axl_doc_has_pi_target            (axlDoc * doc, char * pi_target)
{
	axlPI * pi;
	int     iterator = 0;
	int     length   = 0;

	
	axl_return_val_if_fail (doc,       AXL_FALSE);
	axl_return_val_if_fail (pi_target, AXL_FALSE);

	/* get the length for the items inserted */
	length = axl_list_length (doc->piTargets);
	while (iterator < length) {
		/* for each item inserted */
		pi = axl_list_get_nth (doc->piTargets, iterator);
		/* only check the first ocurrency */
		if (axl_cmp (pi->name, pi_target))
			return AXL_TRUE;

		iterator++;
	}
	
	return AXL_FALSE;
}

/** 
 * @brief Allows to get current processing instruction content.
 * 
 * @param doc The document where the processing instruction is placed.
 *
 * @param pi_target The processing instruction target to get current
 * content.
 * 
 * @return An internal reference to the process instruction target
 * content. Value returned mustn't be deallocated
 */
char    * axl_doc_get_pi_target_content    (axlDoc * doc, char * pi_target)
{
	axlPI * pi;
	int     iterator = 0;
	int     length   = 0;

	axl_return_val_if_fail (doc,       NULL);
	axl_return_val_if_fail (pi_target, NULL);

	/* get the length for the items inserted */
	length = axl_list_length (doc->piTargets);
	while (iterator < length) {
		/* for each item inserted */
		pi = axl_list_get_nth (doc->piTargets, iterator);
		/* only check the first ocurrency */
		if (axl_cmp (pi->name, pi_target))
			return pi->content;

		iterator++;
	}

	return NULL;
}

/** 
 * @brief Allows to get a list which contains \ref axlPI nodes,
 * representing all process instruction that the document has.
 *
 * While using PI, you can use the following functions to get PI
 * information:
 * 
 *  - \ref axl_doc_has_pi_target
 *  - \ref axl_doc_get_pi_target_content
 *
 * However, this function will return first ocurrency for PI found
 * inside the xml document. If you don't use repeated PI elements, you
 * won't find problems, but, if you need to iterate ever all PI found
 * or you are using repeated PI, you can use this function as follows
 * to get current pi elements:
 * \code
 * void show_all_pi (axlDoc * doc) 
 * {
 *      int       iterator;
 *      axlPI   * pi;
 *      axlList * PIs;
 *
 *      // get all PI target that the document has
 *      PIs      = axl_doc_get_pi_target_list (doc);
 *      iterator = 0;
 *
 *      while (iterator < axl_list_length (PIs)) {
 *            // get next pi stored 
 *            pi = axl_list_get_nth (PIs, iterator);
 *
 *            // do some stuff 
 *            printf ("PI found target name=%s, content=%s\n",
 *                    axl_pi_get_name (pi),
 *                    axl_pi_get_content (pi));
 *            
 *            // update the iterator
 *            iterator++;
 *      }
 *      return;
 * }
 * \endcode
 * 
 * @param doc The xml document (\ref axlDoc) where the process
 * instruction will be returned.
 * 
 * @return A reference to the list of processing instruction that the
 * xml document (\ref axlDoc) has.
 */
axlList * axl_doc_get_pi_target_list       (axlDoc * doc)
{
	axl_return_val_if_fail (doc,       NULL);

	return doc->piTargets;
}

/** 
 * @brief Allows to create a new \ref axlPI element. 
 * 
 * @param name The PI target name.
 * @param content The PI content.
 * 
 * @return A newly allocated \ref axlPI element.
 */
axlPI * axl_pi_create (char * name,
		       char * content)
{
	axlPI * pi;

	/* create the PI */
	pi          = axl_new (axlPI, 1);
	pi->name    = axl_strdup (name);
	
	/* copy the content if defined */
	if (content != NULL)
		pi->content = axl_strdup (content);

	return pi;
}

/** 
 * @brief Allows to get current pi name from the given \ref axlPI
 * reference.
 *
 * @param pi The PI reference where the name will returned.
 * 
 * @return A string representing the PI name. Returned value shouldn't
 * be deallocated.
 */
char    * axl_pi_get_name                  (axlPI  * pi)
{
	axl_return_val_if_fail (pi, NULL);

	/* return current PI name */
	return pi->name;
}

/** 
 * @brief Allows to get current optinal PI content.
 * 
 * @param pi The PI where the content will be returned.
 * 
 * @return A string representing the PI content. This value could be
 * NULL because it is optional to be defined. Returned value must not
 * be deallocated.
 */
char    * axl_pi_get_content               (axlPI  * pi)
{
	axl_return_val_if_fail (pi, NULL);
	
	/* return current PI content */
	return pi->content;
}

/** 
 * @brief Deallocates memory used by the \ref axlPI target.
 * 
 * @param target The target to destroy.
 */
void axl_pi_free (axlPI * target)
{
	/* free PI target */
	axl_free (target->name);
	if (target->content != NULL) 
		axl_free (target->content);
	axl_free (target);
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

	/* free node hierarchy */
	if (doc->parentNode != NULL)
		axl_stack_destroy (doc->parentNode);

	/* free pi targets read */
	if (doc->piTargets != NULL)
		axl_list_free (doc->piTargets);

	/* free enconding allocated */
	axl_free (doc->encoding);

	/* free document allocated */
	axl_free (doc);

	return;
}

/** 
 * @internal
 *
 * @brief Allows to consume comments found while reading xml files.
 * 
 * @param stream The axlStream where the comment is spected to be read.
 *
 * @param error An optional axlError where problem will be reported.
 */
bool      axl_doc_consume_comments         (axlDoc * doc, axlStream * stream, axlError ** error)
{

	bool found_item;
	
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking for comemnts");

	/* know, try to read comments a process instructions.  Do this
	 * until both fails. Do this until one of them find
	 * something. */
	do {
		/* flag the loop to end, and only end if both,
		 * comments matching and PI matching fails. */
		found_item = AXL_FALSE;
		
		/* get rid from spaces */
		AXL_CONSUME_SPACES(stream);

		/* check for comments */
		if ((axl_stream_inspect_several (stream, 2, "<!-- ", "<!--")) > 0) { 
			if (! axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 2, " -->", "-->")) { 
				axl_error_new (-1, "detected an opened comment but not found the comment ending",
					       stream, error);
				axl_stream_free (stream);
				return AXL_FALSE;
			} 
			
			/* flag that we have found a comment */
			found_item = AXL_TRUE;
		}
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "now see for process instructions");
	
		/* get rid from spaces */
		AXL_CONSUME_SPACES(stream);

		/* check for PI, only once the xml header have been processed */
		if ((doc != NULL) && doc->headerProcess && (axl_stream_peek (stream, "<?") > 0)) {
			if (! axl_doc_consume_pi (doc, axl_stack_peek (doc->parentNode), stream, error))
				return AXL_FALSE;
			found_item = AXL_TRUE;
		}

		/* get rid from spaces */
		AXL_CONSUME_SPACES(stream);
		
		/* check to break-the-loop */
	}while (found_item);



	/* true value */
	return AXL_TRUE;
}

/** 
 * @internal
 *
 * @brie Consumes Processing intructions that are directed to the
 * application ans configuration or processing instructions.
 * 
 * @param doc The document there the information will be placed.
 * 
 * @param stream The stream where the data is being read.
 *
 * @param error An optional axlError where the information will be
 * reported.
 * 
 * @return AXL_TRUE if not error was found, otherwise AXL_FASLSE is
 * returned.
 */
bool      axl_doc_consume_pi (axlDoc * doc, axlNode * node, 
			      axlStream * stream, axlError ** error)
{
	char  * string_aux;
	char  * string_aux2;
	int     matched_chunk;
	
	
	/* check if a PI target was found */
	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "calling to consume PI..");
	if (axl_stream_peek (stream, "<?") > 0) {
		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a process instruction initialization");
		/* found a pi target initialization */
		axl_stream_accept (stream);
		
		string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, 
						   AXL_TRUE, 3, " ?>", "?>", " ");
		/* check error reported */
		if (string_aux == NULL) {
			axl_error_new (-1, "Found a error while reading the PI target name", stream, error);
			return AXL_FALSE;
		}

		/* check that the reserved xml word is not used for the PI target */
		string_aux2 = axl_strdup (string_aux);
		if (axl_cmp (axl_stream_to_lower (string_aux2), "xml")) {
			axl_free (string_aux2);
			axl_error_new (-1, "Using a reserved PI target name (xml), not allowed", stream, error);
			return AXL_FALSE;
		}
		axl_free (string_aux2);

		axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found PI target name: %s (terminator matched: %d)", 
			 string_aux, matched_chunk);

		/* check which was the matched string */
		if (matched_chunk == 0 || matched_chunk == 1) {
			/* seems that the PI target doesn't have more data associated, craete and return */
			if (node != NULL) {
				axl_node_add_pi_target (node, string_aux, NULL);
				return AXL_TRUE;
			}
			
			if (doc != NULL)
				axl_doc_add_pi_target (doc, string_aux, NULL);
			return AXL_TRUE;
		}

		/* seems that we have additional content to be read */
		if (matched_chunk == 2) {
			/* make a local copy for the PI target name
			 * read previously */
			string_aux  = axl_strdup (string_aux);
			
			/* get the PI content */
			string_aux2 = axl_stream_get_until (stream, NULL, NULL, AXL_TRUE, 2, " ?>", "?>");

			/* check error reported */
			if (string_aux2 == NULL) {
				axl_free (string_aux);
				axl_error_new (-1, "Found a error while reading the PI content", stream, error);
				return AXL_FALSE;
			}

			/* check the destination for the pi */			
			if (node != NULL) {
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "PI processing finished, adding PI (node) and its content");
				axl_node_add_pi_target (node, string_aux, string_aux2);
				axl_free (string_aux);
				return AXL_TRUE;
			}


			if (doc != NULL) {
				axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "PI processing finished, adding PI (doc) and its content");
				axl_doc_add_pi_target (doc, string_aux, string_aux2);
				axl_free (string_aux);
				return AXL_TRUE;
			}

		}

		/* check error reported */
		axl_error_new (-1, "Found a error while reading the PI target name, unable to find PI terminator ?>", stream, error);
		return AXL_FALSE;
	}

	axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "PI processing finished");

	return AXL_TRUE;
}

/* @} */
