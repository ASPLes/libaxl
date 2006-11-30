/*
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
 * \defgroup axl_doc_module Axl Doc: XML Documents related functions, loading XML documents and using them.
 */

/** 
 * \addtogroup axl_doc_module
 * @{
 */

#include <axl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


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
	 * The document version.
	 */
	char * version;

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
	bool    standalone;

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
	bool    headerProcess;

	/** 
	 * @internal Factory to create items in a memory efficient
	 * manner.
	 */
	axlFactory * item_factory;

	/** 
	 * @internal Factory to create nodes in a memory efficient
	 * manner.
	 */
	axlFactory    * node_factory;

	/** 
	 * @internal Factory to alloc strings.
	 */
	axlStrFactory * str_factory;
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
 *
 * @brief Creates a new empty \ref axlDoc reference.
 *
 * Creates the parent stack used for parsing functions.
 * 
 * @return A newly allocated \ref axlDoc reference.
 */
axlDoc * __axl_doc_new (bool create_parent_stack) 
{
	axlDoc    * result = axl_new (axlDoc, 1);

	/* default container lists */
	result->parentNode   = axl_stack_new (NULL);
	result->piTargets    = axl_list_new (axl_list_always_return_1, (axlDestroyFunc) axl_pi_free);

	/* create factories */
	result->item_factory = axl_item_factory_create ();
	result->node_factory = axl_node_factory_create ();
	result->str_factory  = axl_string_factory_create ();
	return result;
}

/** 
 * @internal
 *
 * Clears internal axlDoc variables used mainly to parse documents.
 * 
 * @param doc The \ref axlDoc to clear
 */
void __axl_doc_clean (axlDoc * doc)
{
	/* release memory used by the parser */
	if (doc->parentNode != NULL) {
		axl_stack_free (doc->parentNode);
		doc->parentNode = NULL;
	}

	return;
}

/** 
 * @internal Function used by the axl doc module to allocate memory to
 * be used by the axl stream. Currently this is used to alloc xml node
 * names and xml attribute key and its value. The rest of items are
 * allocated by the system memory allocation.
 * 
 * @param size The size that is required by the axl stream to be allocated.
 *
 * @param doc The axlDoc reference, which contains a reference to the
 * string factory used to allocate memory.
 * 
 * @return A reference to the allocated memory. 
 */
char * __axl_doc_alloc (int size, axlDoc * doc)
{
	/* just return a piece of memory */
	return axl_string_factory_alloc (doc->str_factory, size);
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
 * @return It is supposed that the function return \ref true, an
 * not deallocation is performed, and all elements were parsed
 * properly. In the case \ref false is returned, memory associated
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
		return false;
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "looking for an xml header declaration");

	/* check for initial XMLDec (production 23) */
	if (axl_stream_inspect (stream, "<?", 2)) {
		
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found xml declaration");

		/* check initial <?xml xml header */
		if (! (axl_stream_inspect (stream, "xml", 3) > 0)) {
			axl_error_new (-2, "expected initial <?xml declaration, not found.", stream, error);
			axl_stream_free (stream);
			return false;
		}
		
		/* consume spaces */
		AXL_CONSUME_SPACES (stream);

		if (! axl_stream_inspect (stream, "version=", 8)) {
			axl_error_new (-2, "expected to find 'version=' declaration, not found.", stream, error);
			axl_stream_free (stream);
			return false;
		}

		/* consume spaces */
		AXL_CONSUME_SPACES (stream);

		/* check for " or ' */
		if (! axl_stream_inspect_several (stream, 2, "\"1.0\"", 5, "'1.0'", 5)) {
			axl_error_new (-2, "expected to find either \" or ' while procesing version number, not found.", stream, error);
			axl_stream_free (stream);
			return false;
		}

		/* check for an space */
		AXL_CONSUME_SPACES(stream);

		/* now check for encoding */
		if (axl_stream_inspect_several (stream, 2, "encoding=\"", 10, "encoding='", 10) > 0) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found encoding declaration");

			/* found encoding instruction */
			string_aux = axl_stream_get_until (stream, NULL, NULL, true, 2, "'", "\"");
			if (string_aux == NULL) {
				axl_error_new (-2, "expected encoding value, not found.", stream, error);
				axl_stream_free (stream);
				return false;
			}

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "encoding found=%s", string_aux);

			/* set document encoding: do not allocate
			 * twice the string returned, just nullify
			 * stream internal reference and use the same
			 * reference */
			axl_stream_nullify (stream, LAST_CHUNK);
			doc->encoding = string_aux;

		}

		/* check for an space */
		AXL_CONSUME_SPACES(stream);

		/* get standalone configuration */
		if ((axl_stream_inspect_several (stream, 2, "standalone=\"", 12, "standalone='", 12) > 0)) {
			
			/* found standalone instruction */
			string_aux = axl_stream_get_until (stream, NULL, NULL, true, 2, "'", "\"");
			if (string_aux == NULL) {
				axl_error_new (-2, "expected to receive standalone value, not found.", stream, error);
				axl_stream_free (stream);
				return false;
			}

			/* set standalone configuration */
			if (memcmp ("yes", string_aux, 3))
				doc->standalone = false;
			else
				doc->standalone = true;
		}
		
		/* check for an space */
		AXL_CONSUME_SPACES(stream);

		/* get the trailing header */
		if (! (axl_stream_inspect (stream, "?>", 2) > 0)) {
			axl_error_new (-2, "expected to receive the xml trailing header ?>, not found.", stream, error);
			axl_stream_free (stream);
			return false;
		}

		/* consume a possible comment */
		if (! axl_doc_consume_comments (doc, stream, error))
			return false;
	}

	/* return TRUE value */
	return true;
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
 * @return true if the first node was successfully parsed or
 * false if not. If the function find something wrong the document
 * is unrefered.
 */
bool __axl_doc_parse_node (axlStream * stream, axlDoc * doc, axlNode ** calling_node, bool * is_empty, axlError ** error)
{
	char    * string_aux;
	char    * string_aux2;
	axlNode * node;
	int       matched_chunk;
	int       length;
	bool      delim;
	
	/* consume a possible comment */
	if (! axl_doc_consume_comments (doc, stream, error))
		return false;

	/* check for initial < definition */
	if (! (axl_stream_inspect (stream, "<", 1) > 0)  && ! axl_stream_remains (stream)) {
		/* check if we are reading the first node node */
		if (doc->rootNode == NULL)
			axl_error_new (-2, "expected initial < for a root node definition, not found. An xml document must have, at least, one node definition.", 
				       stream, error);
		else
			axl_error_new (-2, "expected initial < for a node definition, not found.", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* get node name, keeping in mind the following:
	 * chunk_matched
	 * >  : 0
	 * /> : 1
	 * " ": 2
	 *
	 * We also reconfigure the alloc method used by the axl stream
	 * to ensure that the module name is allocated through the
	 * string factory.
	 */
	axl_stream_set_buffer_alloc (stream, (axlStreamAlloc)__axl_doc_alloc, doc);
	string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, true, 2, ">", " ");

	/* nullify */
	axl_stream_nullify (stream, LAST_CHUNK);

	if (AXL_IS_STR_EMPTY (string_aux)) {
		/* use alloc though string factory */
		axl_stream_set_buffer_alloc (stream, NULL, NULL);

		axl_error_new (-2, "expected an non empty content for the node name not found.", stream, error);
		axl_stream_free (stream);
		return false;
	}
	
	/* if found a '/', it is matched as 1 */
	if (matched_chunk == 1)
		matched_chunk = 2;
	else {
		/* get the string length */
		length = strlen (string_aux);

		/* if matched / it means that it was readed />, remove
		 * it and all white spaces */
		if (string_aux[length - 1] == '/') {
			/* flag as matched /> */
			matched_chunk          = 1;
			string_aux[length - 1] = 0;
		} /* end if */
	} /* end if */

	/* create the node and associate it the node name found */
	node = axl_node_factory_get (doc->node_factory);
	axl_node_set_name_from_factory (node, string_aux); 

	if (doc->rootNode == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "setting as first node found, the root node: <%s>", string_aux);

		doc->rootNode  = node;
		
		/* set the node read, the root one, to be the parent */
		axl_stack_push (doc->parentNode, node);

		/* configure the node */
		axl_node_set_doc (node, doc);

	} else {
		/* or set the node as a child of the current parent */
		axl_doc_set_child_current_parent (doc, node);
	}

	/* set the node created to the calling node, so the caller
	 * could get a reference */
	if (calling_node != NULL)
		*calling_node = node;

	/* only consume white spaces if matched_chunk is 2 */
	if (matched_chunk == 2) {
		/* get rid from spaces */
		AXL_CONSUME_SPACES (stream);
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node found: [%s]", string_aux);

	/* now, until the node ends, we have to find the node
	 * attributes or the node defintion end */
	while (1) {
		/* check if we have an attribute for the node, or the node
		 * definition have ended or the node definition is an empty
		 * one 
		 * 
		 * the following code that relies on matched_chunk is
		 * done due to previous call to get_until function. If
		 * the value 0 or 1 was matched, this means that we
		 * are on "/>" case */
		if ((matched_chunk == 1) ||
		    axl_stream_inspect (stream, "/>", 2) > 0) {
			/* use alloc though string factory */
			axl_stream_set_buffer_alloc (stream, NULL, NULL);

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found end xml node definition '/>'");

			/* empty node configuration found */
			*is_empty = true;
			/* axl_node_set_is_empty (node, true); */

			/* make this node to be completed and no child
			 * could be set. */
			axl_stack_pop (doc->parentNode);

			/* set the parent node to receive all content
			 * found in the next parsing elements because
			 * the element found is totally empty */
			*calling_node = axl_stack_peek (doc->parentNode);
			return true;
		}
		
		/* check if we have an attribute for the node, or the node
		 * definition have ended or the node definition is an empty
		 * one 
		 * 
		 * the following code that relies on matched_chunk is
		 * done due to previous call to get_until function. If
		 * the value 2 or 3 was matched, this means that we
		 * are on ">" case */
		if ((matched_chunk == 0) ||
		    (axl_stream_inspect (stream, ">", 1) > 0)) {
			/* use alloc though string factory */
			axl_stream_set_buffer_alloc (stream, NULL, NULL);
			
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found [end] xml node definition '>', for node: [%s]",
				   axl_node_get_name (node));

			/* flag that the node is an empty definition */
			*is_empty = false;

			/* this node is ended */
			return true;
		}
		
		/* get rid from spaces */
		AXL_CONSUME_SPACES (stream);

		/* found attribute declaration, try to read it.
		 *
		 * We also reconfigure the alloc method used by the
		 * axl stream to ensure that xml node attributes are
		 * allocated through the string factory.
		 */
		string_aux = axl_stream_get_until (stream, NULL, NULL, true, 1, "=");
		if (string_aux != NULL) {

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "attribute found: [%s]", string_aux);

			/* nullify internal reference to the stream:
			 * now we have inside string_aux the attribute
			 * name */
			axl_stream_nullify (stream, LAST_CHUNK);

			/* remove next " and ' if defined */
			/* flag the we are looking for a " */
			delim = true;
			if (! ((axl_stream_inspect (stream, "\"", 1) > 0))) {
				/* seems it is not found, flag we are
				 * looking for ' */
				delim = false;
				if (! (axl_stream_inspect (stream, "\'", 1) > 0)) {
					/* use alloc though string factory */
					axl_stream_set_buffer_alloc (stream, NULL, NULL);

					axl_error_new (-2, "Expected to find an attribute value initiator (\") or ('), every attribute value must start with them", 
						       stream, error);
					axl_stream_free (stream);
					return false;
				}
			}
			
			
			/* now get the attribute value */
			if (delim)
				string_aux2 = axl_stream_get_until (stream, NULL, NULL, true, 1, "\"");
			else
				string_aux2 = axl_stream_get_until (stream, NULL, NULL, true, 1, "'");

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "value found: [%s]", string_aux2);
			
			/* nullify internal reference so we have the
			 * only one reference to attribute value
			 * inside string_aux2 */
			axl_stream_nullify (stream, LAST_CHUNK);
			
			/* set a new attribute for the given node */
			axl_node_set_attribute_from_factory (node, string_aux, string_aux2);

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "attribute installed..");

			/* get rid from spaces */
			AXL_CONSUME_SPACES (stream);
			continue;
		}
		
		/* if reached this point, error found */
		axl_error_new (-2, "Parse error while reading a node being opened", stream, error);
		axl_stream_free (stream);
		return false;

	} /* end while */
	
	/* node properly parsed  */
	return true;
}

/** 
 * @internal
 * @brief Perform the close node operation.
 *
 */
bool __axl_doc_parse_close_node (axlStream * stream, axlDoc * doc, axlNode ** _node, axlError ** error)
{
	char    * string;
	int       result_size = -1;
	axlNode * node;

	/* get the node being closed to check to the current parent */
	string = axl_stream_get_until_ref (stream, NULL, NULL, true, &result_size, 1, ">");
	if (string == NULL) {
		axl_error_new (-1, "An error was found while closing the xml node", stream, error);
		axl_stream_free (stream);
		return false;
	}

	/* check for optional white space inside the trailing result */
	if (axl_stream_is_white_space (string + result_size - 1)) {
		/* nullify to remove the optional white spaces */
		string [result_size - 1] = 0;
	} /* end if */

	/* get current parent node */
	node   = axl_stack_peek (doc->parentNode);
	if (node == NULL) {
		axl_error_new (-1, "Found that the stack doesn't have any node opened, this means either an libaxl error or the xml being read is closing a node not opened",
			       stream, error);
		axl_stream_free (stream);
		return false;
	}
	
	/* check current axl node name against closed string */
	if (axl_cmp (axl_node_get_name (node), string)) { 

		/* ok, axl node to be closed is the one expected */
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "closing xml node, that matched with parent opened");

		return true;
	}

	/* seems that the node being closed doesn't match */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "xml node names to be closed doesn't matched (%s != %s), current node stack status:",
		 axl_node_get_name (node), string);

	node = axl_stack_pop (doc->parentNode);
	while (node != NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "<%s>", axl_node_get_name (node));
		node = axl_stack_pop (doc->parentNode);
	}

	axl_error_new (-1, "An error was found while closing the opened xml node, parent opened and xml node being closed doesn't match",
		       stream, error);
	axl_stream_free (stream);

	return false;
}

/** 
 * @internal
 * 
 * Internal function which works as a common base for all functions
 * that parse XML documents from different inputs.
 */
axlDoc * __axl_doc_parse_common (const char * entity, int entity_size, 
                                 const char * file_path, int fd_handle, 
			         axlError ** error)
{
	axlStream * stream        = NULL;
	axlDoc    * doc           = NULL;
	axlNode   * node          = NULL;
	char      * string        = NULL;
	int         index;
	bool        is_empty      = false;
	
	/* create the xml stream using provided data */
	stream         = axl_stream_new (entity, entity_size, file_path, fd_handle, error);
	axl_return_val_if_fail (stream, NULL);

	/* create a document reference */
	doc            = __axl_doc_new (true);
	axl_stream_link (stream, doc, (axlDestroyFunc) axl_doc_free);

	/* parse initial xml header */
	if (!__axl_doc_parse_xml_header (stream, doc, error))
		return NULL;

	/* signal that this document have processed its header */
	doc->headerProcess = true;
	
	/* parse the rest of the document, setting as parent NULL
	 * because still no parent is found. */
	if (!__axl_doc_parse_node (stream, doc, &node, &is_empty, error))
		return NULL;
	
	/* if the node returned is not empty */
	if (! is_empty) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the first node ready, have content, reading it");

		/* while the stream have data */
		while (axl_stream_remains (stream)) {

			/* get current index */
			index = axl_stream_get_index (stream);

			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "current index: %d (global: %d)", index,
				   axl_stream_get_global_index (stream));

			/* get rid from spaces */
			AXL_CONSUME_SPACES(stream);

			/* consume a possible comment and process instructions */
			if (axl_stream_peek (stream, "<?", 2) > 0 || axl_stream_peek (stream, "<!--", 4) > 0) {
				if (! axl_doc_consume_comments (doc, stream, error))
					return false;
				
				/* continue on the next index */
				continue;
			} /* end if */
			
			if ((axl_stream_peek (stream, "</", 2) > 0)) {
				/* accept previous peek */
				axl_stream_accept (stream);

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a node termination signal");

				/* seems that a node is being closed */
				if (! __axl_doc_parse_close_node (stream, doc, &node, error))
					return NULL;

				/* because the xml node have been
				 * closed, make the parent to be the
				 * previous one */
				axl_stack_pop (doc->parentNode);

				/* get the new parent */
				node = axl_stack_peek (doc->parentNode);

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "node properly closed, current parent node stack size: %d, parent=<%s>",
					   axl_stack_size (doc->parentNode), (node != NULL) ? axl_node_get_name (node) : "--no parent--");

				if (axl_stack_size (doc->parentNode) > 0)
					continue;
				break;
			} /* end if */

			/* check here for CDATA section. This is done
			 * here because the following checking could
			 * be mixed because they starts with the same:
			 * < */
			if ((axl_stream_peek (stream, "<![CDATA[", 9) > 0)) {
				/* accet previous peek */
				axl_stream_accept (stream);

				/* found CDATA section, get current content */
				string = axl_stream_get_until (stream, NULL, NULL, true, 1, "]]>");
				if (string == NULL) {
					axl_error_new (-1, "Unable to get CDATA content. There was an error.", stream, error);
					axl_stream_free (stream);
					return NULL;
				}

				/* nullify internal reference to the
				 * string_aux so we can use that
				 * memory allocated as our
				 * reference */
				axl_stream_nullify (stream, LAST_CHUNK);

				/* set current data */
				axl_node_set_content_ref (node, string, -1); 
				continue;
			} /* end if */


			if ((axl_stream_peek (stream, "<", 1) > 0)) {
				/* accept previous peek */
				axl_stream_accept (stream);

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a new node being opened");

				/* seems that another node is being opened */
				if (!__axl_doc_parse_node (stream, doc, &node, &is_empty, error))
					return NULL;

				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "finished parsing opened node, current parent=<%s>",
					   axl_node_get_name (node));

				continue;
			}
			
			/* restore index position previous to the axl
			 * space consuming */
			if (axl_stream_get_index (stream) > index) {
				axl_stream_move (stream, index);
			}
			
			/* found node content */
			axl_stream_set_buffer_alloc (stream, (axlStreamAlloc)__axl_doc_alloc, doc);
			string = axl_stream_get_until (stream, NULL, NULL, false, 1, "<");
			axl_stream_set_buffer_alloc (stream, NULL, NULL);

			/* check for a null content found */
			if (string == NULL) {
				axl_error_new (-1, "an error was found while reading the xml node content", stream, error);
				axl_stream_free (stream);
				return NULL;
			}

			/* nullify internal stream reference to have
			 * the unique reference */
			axl_stream_nullify (stream, LAST_CHUNK);

			/* set current data */
			/* axl_node_set_content_ref (node, string, -1); */
			axl_node_set_content_from_factory (node, string, -1); 

			/* keep on looping */
		}
	}

	/* pop axl parent */
	if (! axl_stack_is_empty (doc->parentNode)) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, 
			   "current parent stack size shows that not all opened nodes were closed. This means that the XML document is not properly balanced (stack size: %d)",
			   axl_stack_size (doc->parentNode));

		axl_error_new (-1, "XML document is not balanced, still remains xml nodes", stream, error);
		axl_stream_free (stream);
		return NULL;
	}

	/* parse complete */
	axl_stream_unlink (stream);
	axl_stream_free (stream);

	/* clean document internal variables */
	__axl_doc_clean (doc);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "xml document parse COMPLETED"); 

	return doc;
}

/** 
 * @brief Creates a new empty xml document, especifying options to be
 * used in the header.
 *
 * This function allows to create the xml document representation the
 * must be used to add childs to it.
 * 
 * @param version The xml document version. This value is optional. If
 * NULL is used, the library will use "1.0" as version value.
 *
 * @param encoding The document encoding to be used. This value is
 * optional, if NULL is provided, no encoding specification will be
 * used.
 *
 * @param standalone Standalone configuration flag. By default, use
 * false.
 * 
 * @return Returns a newly allocated \ref axlDoc instance that must be
 * deallocated by using \ref axl_doc_free.
 */
axlDoc  * axl_doc_create                   (const char     * version, 
					    const char     * encoding,
					    bool   standalone)
{
	axlDoc * doc;

	/* create a new reference, without creating  */
	doc = __axl_doc_new (false);
	
	/* save the version */
	if (version != NULL)
		doc->version  = axl_strdup (version);

	/* save encoding value */
	if (encoding != NULL)
		doc->encoding = axl_strdup (encoding);

	/* save standalone configuration */
	doc->standalone       = standalone;
	
	/* return the reference created */
	return doc;
}

/** 
 * @internal Returns how many bytes will hold the document provided.
 * 
 * @param doc The document to measure.
 *
 * @param pretty_print If pretty print is activated.
 * 
 * @return The number of bytes or -1 if it fails.
 */
int __axl_doc_get_flat_size_common (axlDoc * doc, bool pretty_print, int tabular) 
{
	
	int result;
	axl_return_val_if_fail (doc, -1);

	/* count the xml header: 
	 *
	 * "<?xml version='1.0'" = 19 characters 
	 * " standalone='yes'"   = 17 characters
	 * " encoding='enc'"     = 12 characters + strlen (enc)
	 * " ?>"                 = 3  characters
	 *
	 * if pretty print add: "\r\n" +2 on windows
	 * and \n on unix.
	 */
	result = 22;

	if (pretty_print)
#ifdef __AXL_WIN32__
		result += 2;
#else
	        result += 1;
#endif

	if (doc->standalone)
		result += 17;
	
	if (doc->encoding != NULL) {
		result += 12 + strlen (doc->encoding);
	}
	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "xml document header size=%d",
		   result);

	/* now, count every node that the document have */
	result += axl_node_get_flat_size (doc->rootNode, pretty_print, 0, tabular);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "xml document body size=%d",
		   result);
	
	/* return current result */
	return result;
}

/** 
 * @internal
 * Common implementation for the dumping functions.
 */
bool __axl_doc_dump_common (axlDoc * doc, char ** content, int * size, bool pretty_print, int tabular)
{

	char * result;
	int    index;

	/* perform some envrironmental checks */
	axl_return_val_if_fail (doc, false);
	axl_return_val_if_fail (content, false);
	axl_return_val_if_fail (size, false);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "getting document size..");

	/* get the about of memory to allocate so the whole xml
	 * document fit in only one memory block */
	(* size)    = __axl_doc_get_flat_size_common (doc, pretty_print, tabular);
	(* content) = NULL;

	/* check returned size */
	if ((* size) == -1)
		return false;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "document dump size: %d", *size);
	
	/* allocate the memory block required */
	result = axl_new (char, (*size) + 1);

	/* xml document header */
	index = 0;
	memcpy (result, "<?xml version='1.0' ", 20);
	index = 20;

	/* standalone attribute */
	if (doc->standalone) {
		memcpy (result + index, "standalone='yes' ", 17); 
		index += 17;
	}

	/* encoding declaration */
	if (doc->encoding) {
		/* initial encoding declaration */
		memcpy (result + index, "encoding='", 10);
		index += 10;

		/* copy encoding content */
		memcpy (result + index, doc->encoding, strlen (doc->encoding));
		index += strlen (doc->encoding);

		/* encoding trailing */
		memcpy (result + index, "' ", 2);
		index += 2;
	}

	/* header trailing */
	memcpy (result + index, "?>", 2);
	index += 2;

	if (pretty_print) {
#ifdef __AXL_WIN32__
		memcpy (result + index, "\r\n", 2);
		index += 2;
#else
		memcpy (result + index, "\n", 1);
		index += 1;
#endif
	}
	
	/* dump node information */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "starting dump at: %d", index);
	index    = axl_node_dump_at (doc->rootNode, result, index, pretty_print, 0, tabular);

	/* check dump size */
	if (*size != index) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_CRITICAL, "internal dump error, inconsitent size: calculated=%d != returned=%d",
			   *size, index);

		/* free allocated result */
		axl_free (result);

		*size    = -1;
		*content = NULL;

		return false;
	}

	/* set results */
	*content = result;
	*size    = index;
	
	return true;
}
/** 
 * @brief Allows to get the xml representation for the provided \ref
 * axlDoc reference.
 *
 * Given the \ref axlDoc reference, which represents a XML document,
 * this function allows to get its stringify representation.
 * 
 * @param doc The \ref axlDoc to stringify
 *
 * @param content The reference where the result will be returned.
 *
 * @param size The reference where the document content size will be
 * returned.
 *
 * @return The function returns \ref true if the dump operation was
 * performed. Otherwise \ref false is returned.
 */
bool      axl_doc_dump                     (axlDoc  * doc, 
					    char   ** content, 
					    int     * size)
{
	/* use common implementation */
	return __axl_doc_dump_common (doc, content, size, false, 0);
}


/** 
 * @brief Allows to perform a dump operation like \ref axl_doc_dump,
 * but making the output to be pretty printed.
 * 
 * @param doc The \ref axlDoc reference to be dumped.
 *
 * @param content The reference that will hold the dumped information.
 *
 * @param size Result size for the dumped information.
 *
 * @param tabular The tabular size basic unit used for level
 * tabulation. An appropiate value could be 4.
 * 
 * @return \ref true if the document was dumped, \ref false if
 * something has failed.
 */
bool      axl_doc_dump_pretty              (axlDoc  * doc,
					    char   ** content,
					    int     * size,
					    int       tabular)
{
	/* use common implementation */
	return __axl_doc_dump_common (doc, content, size, true, tabular);
}

/** 
 * @brief Allows to dump a xml document directly to the file located
 * at the file path.
 *
 * This function saves you the round trip to declare variables to hold
 * the memory, open a file, dump the content and properly close the
 * output file. The function works the same as \ref axl_doc_dump but
 * doing the extra job to transfer the xml document into a file.
 *
 * See also \ref axl_doc_dump_pretty_to_file to get a version dumps
 * the content doing some pretty printing operations.
 * 
 * @param doc The document to be dumped into a file.
 *
 * @param file_path The file path where the output will be placed. The
 * function will require to have access rights to the file (or to
 * create a new file if it doesnt exists). The default behaviour is to
 * overwrite the file found if exists. So, if you don't want to get
 * content overwrited, you must provide the enough code to avoid such
 * situations prior calling to this function.
 * 
 * @return \ref true if the dump operation was ok, otherwisde \ref
 * false is returned.
 */
bool      axl_doc_dump_to_file             (axlDoc  * doc,
					    char    * file_path)
{
	char * content = NULL;
	int    size    = -1;
	int    written = -1;
	FILE * fd      = NULL;

	/* dump content and check result */
	if (! __axl_doc_dump_common (doc, &content, &size, false, 0)) {
		/* no dump operation done */
		return false;
	}

	/* open the file and check */
	if ((fd = fopen (file_path, "w")) == NULL) {
		/* failed to open the file to dump the content */
		axl_free (content);

		return false;
	}

	/* dump the content */
	written = fwrite (content, 1, size, fd);

	/* free the content */
	axl_free (content);

	/* close file */
	fclose (fd);

	/* return if we have failed to dump all the content to the
	 * file or not. */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "returning that the dump was: %s (written:%d == size:%d)", 
		   (written == size) ? "OK" : "FAILED", 
		   written, size);
		   
	return (written == size);
}

/** 
 * @brief Allows to dump a xml document directly to the file located
 * at the file path, doing pretty printing operations.
 *
 * This function saves you the round trip to declare variables to hold
 * the memory, open a file, dump the content and properly close the
 * output file. The function works the same as \ref axl_doc_dump but
 * doing the extra job to transfer the xml document into a file.
 *
 * See also \ref axl_doc_dump_to_file to get a version dumps the
 * content without doing pretty printing operations.
 * 
 * @param doc The document to be dumped into a file.
 *
 * @param file_path The file path where the output will be placed. The
 * function will require to have access rights to the file (or to
 * create a new file if it doesnt exists). The default behaviour is to
 * overwrite the file found if exists. So, if you don't want to get
 * content overwrited, you must provide the enough code to avoid such
 * situations prior calling to this function.
 * 
 * @param tabular The amount of white spaces to introduce as tabular
 * for each level found inside the xml.
 * 
 * @return \ref true if the dump operation was ok, otherwisde \ref
 * false is returned.
 */
bool      axl_doc_dump_pretty_to_file      (axlDoc  * doc,
					    char    * file_path,
					    int       tabular)
{
	char * content = NULL;
	int    size    = -1;
	int    written = -1;
	FILE * fd      = NULL;

	/* dump content and check result */
	if (! __axl_doc_dump_common (doc, &content, &size, true, tabular)) {
		/* no dump operation done */
		return false;
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "document dumped, now transfer that content to a file");

	/* open the file and check */
	if ((fd = fopen (file_path, "w")) == NULL) {
		/* failed to open the file to dump the content */
		axl_free (content);

		return false;
	}

	/* dump the content */
	written = fwrite (content, 1, size, fd);

	/* free the content */
	axl_free (content);

	/* close file */
	fclose (fd);

	/* return if we have failed to dump all the content to the
	 * file or not. */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "returning that the dump was: %s (written:%d == size:%d)", 
		   (written == size) ? "OK" : "FAILED", 
		   written, size);
	return (written == size);
}

/** 
 * @brief Allows to get how much will take the \ref axlDoc instance
 * represented as an XML document in an storage device (like memory).
 *
 * @param doc The \ref axlDoc reference that is being requested to return its size.
 * 
 * @return The size the \ref axlDoc will represent, in a
 * octect-counting, or -1 if fails. The function will only fail if the
 * provided reference is NULL.
 */
int axl_doc_get_flat_size (axlDoc * doc)
{
	/* use common implementation */
	return __axl_doc_get_flat_size_common (doc, false, 0);
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
 * Here is an example:
 * \code
 * // axl document representation 
 * axlDoc   * doc;
 * axlError * error;
 *	
 *
 * // parse the given string 
 * doc = axl_doc_parse ("<?xml version='1.0' ?><axldoc />", 32, &error);
 * if (doc == NULL) {
 *      printf ("Error found: %s\n", axl_error_get (error));
 *      axl_error_free (error);
 *      return false;
 * }
 *
 * // release document parsed 
 * axl_doc_free (doc);	
 * \endcode
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
axlDoc * axl_doc_parse (const char * entity, int entity_size, axlError ** error)
{
	return __axl_doc_parse_common (entity, entity_size, NULL, -1, error);
}

/** 
 * @internal
 * 
 * Allows to get current file size, in bytes, of the provided file
 * located at the given file path.
 */
int __axl_doc_get_file_size (char * file_path)
{
	struct stat buf;

	axl_return_val_if_fail (file_path, -1);
	
	/* clear the memory hold */
	memset (&buf, 0, sizeof (struct stat));

	/* return current file size */
	if (stat ((const char *) file_path, &buf) < 0)
		return -1;
	
	/* return the file size */
	return buf.st_size;
	
}

/** 
 * @brief Allows to parse an xml document from the given file path
 * location.
 *
 * This function works the same way like \ref axl_doc_parse and \ref
 * axl_doc_parse_strings, but using as an input, the selected file
 * provided by the path. In fact, all this function, use the same xml
 * parse engine. The advantage of this function is that it is more
 * efficient while reading huge xml files. 
 *
 * Here is an example:
 * \code
 * axlDoc   * doc = NULL;
 * axlError * error = NULL;
 *
 * // parse the provide file
 * doc = axl_doc_parse_from_file ("test.xml", &error);
 * if (doc == NULL) {
 *    // check error found
 *    printf ("ERROR: (code: %d) %s\n",
 *            axl_error_get_code (error),
 *            axl_error_get (error));
 *    axl_error_free (error);
 *    return -1;
 * }
 *
 * // do some stuff with the readed document
 * 
 * // release it once no longer needed
 * axl_doc_free (doc);
 * \endcode
 * 
 * @param file_path The file path to report.
 *
 * @param error The \ref axlError where errors found will be reported.
 * 
 * @return 
 */
axlDoc  * axl_doc_parse_from_file          (const char * file_path,
					    axlError  ** error)
{
	return __axl_doc_parse_common (NULL, -1, file_path, -1, error);
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

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "string to parse: %s", stream);

	/* parse the string */
	doc = axl_doc_parse (stream, -1, error);

	/* free the stream */
	axl_free (stream);

	return doc;
}

/** 
 * @internal 
 * 
 * Internal support function which checks the provided child and its
 * childs are equal.
 */
bool __axl_doc_are_equal (axlNode * node, axlNode * node2, bool trimmed)
{
	int       iterator;
	int       length;
	int       length2;

	axlItem * child;
	axlItem * child2;
	
	/* check if parent nodes are equal */
	if (! axl_node_are_equal (node, node2))
		return false;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "<%s>=<%s>", 
		   axl_node_get_name (node), axl_node_get_name (node2));

	/* iterate over all childs inside the node */
	iterator = 0;
	length   = axl_node_get_child_num (node);
	length2  = axl_node_get_child_num (node2);
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking node childs: (%d == %d)?", 
		   length, length2);

	if (length != length2) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "child number differs, documents aren't equal");
		return false;
	}

	/* get the first item inside the node */
	child  = axl_item_get_first_child (node);
	child2 = axl_item_get_first_child (node2);

	/* for each item child found in both nodes */
	while (child != NULL && child2 != NULL) {
		
		if (child == NULL) 
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "child from the first document is null..");

		if (child2 == NULL) 
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "child from the second document is null..");

		/* check if these nodes are also equal */
		if (! axl_item_are_equal (child, child2, trimmed)) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "items  aren't equal, document is not equal");
			return false;
		}

		/* check its childs in the case the axl item is
		 * representing an item node */
		if (axl_item_get_type (child) == ITEM_NODE) {
			/* get a reference */
			node  = axl_item_get_data (child);
			node2 = axl_item_get_data (child2);

			if (! __axl_doc_are_equal (node, node2, trimmed)) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "nodes <%s> and <%s> aren't equal, document is not equal", 
					   axl_node_get_name (node), axl_node_get_name (node2));
				return false;
			} /* end if */
		}

		/* get a referece to the next childs to check */
		child  = axl_item_get_next (child);
		child2 = axl_item_get_next (child2);
		
	} /* end while */

	/* the nodes recieved are equal */
	return (child == NULL && child2 == NULL);
}

/** 
 * @internal Common implementation for equal documents.
 */
bool      axl_doc_are_equal_common (axlDoc * doc,
				    axlDoc * doc2,
				    bool     trimmed)
{
	axlNode * node;
	axlNode * node2;

	axl_return_val_if_fail (doc, false);
	axl_return_val_if_fail (doc, false);

	/* first, check the document root */
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking that both documents are equal");

	node  = axl_doc_get_root (doc);
	if (node == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "document(a) doesn't have document root ..");
	}
	node2 = axl_doc_get_root (doc2);
	if (node2 == NULL) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "document(b) doesn't have document root ..");
	}

	/* call to common implemenation, activating triming */
	return __axl_doc_are_equal (node, node2, trimmed);
}

/** 
 * @brief Allows to perform a document equal check against order,
 * relaxing the checking done to contet found inside nodes.
 *
 * This function works the same as \ref axl_doc_are_equal but
 * considering that two content are equal no matter which is the
 * number of white spaces (in the W3C, ' ', \\t, \\r and \\n) are
 * found starting and ending the content.
 *
 * Under this approach the both document aren't exactly the same, but
 * usually, white spaces found starting and ending content have no
 * meaning for the application processing xml. In the case you want a
 * fully equal document checking you must \ref axl_doc_are_equal
 *
 * @param doc The document to check.
 * @param doc2 The second document to check
 * 
 * @return \ref true if both documents are equal in the sense
 * described, otherwise \ref false is returned.
 */
bool      axl_doc_are_equal_trimmed        (axlDoc * doc,
					    axlDoc * doc2)
{
	/* call to common implemenation, activating triming */
	return axl_doc_are_equal_common (doc, doc2, true);
}

/** 
 * @brief Allows to check if the provided two references represents
 * equivalent xml documents.
 *
 * There is an alternative document checking function (\ref
 * axl_doc_are_equal_trimmed) which considered that content found
 * inside a xml node is equal if they share the same information
 * without considering white spaces found starting and ending both
 * elements being checked.
 *
 * This function considers that two documents are equal only and only
 * if all nodes, attributes and content found is exactly, byte by
 * byte, as found in the other document.
 * 
 * @param doc The first XML document to check.
 * @param doc2 The second XML document to check.
 * 
 * @return true if both documents represents the same document,
 * false if not.
 */
bool      axl_doc_are_equal                (axlDoc * doc, 
					    axlDoc * doc2)
{
	/* call to common implemenation, activating triming */
	return axl_doc_are_equal_common (doc, doc2, true);
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
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "requested root node = %s wasn't found, current root %s", paths[1],
				   axl_node_get_name (doc->rootNode));
			axl_list_free (nodes);
			axl_stream_freev (paths);
			return NULL;
		}
	}

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found node: %s for path=%s", paths[1], path_to);

	/* now the general case */
	iterator = 2;
	while ((paths[iterator] != NULL) && (strlen (paths[iterator]) > 0)) {
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking path item %s", paths[iterator]);
		
		/* check that the last path is used */
		if (axl_cmp (paths[iterator], "*") && 
		    (axl_stream_strv_num (paths) != iterator + 1)) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "using the '*' at that path different from the last one.", paths[iterator]);
			axl_list_free (nodes);
			axl_stream_freev (paths);
			return NULL;
		}

		/* get a reference to the node searched */
		node = axl_node_get_child_called (node, paths[iterator]);
		if (node == NULL) {
			__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "the node located at %s wasn't found.", path_to);

			axl_list_free (nodes);
			axl_stream_freev (paths);
			return NULL;
		}
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found node: %s", paths[iterator]);

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
 * @brief Allows to get the node content for the final node provided
 * by the path.
 * 
 * @param doc The (\ref axlDoc) xml document where the content will be
 * looked up.
 *
 * @param path_to Path to the node where the content will be returned.
 *
 * @param content_size An optional reference to a variable to store
 * the size of the content returned. If the function receives NULL,
 * the content size will not be returned.
 * 
 * @return A reference to the content that the node have or NULL if
 * fails. The function could fail either because the node doesn't have
 * content or because the node identified by the path doesn't
 * exist. The result returned must not be deallocated.
 */
char    * axl_doc_get_content_at           (axlDoc * doc,
					    char   * path_to,
					    int    * content_size)
{

	axlNode * node;

	/* get the node reference */
	node = axl_doc_get (doc, path_to);
	axl_return_val_if_fail (node, NULL);

	/* return the content requested */
	return axl_node_get_content (node, content_size);
	
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
 * @return \ref true if the standalone configuration, found inside
 * the xml header is set to TRUE. Otherwise \ref false is
 * returned. Keep in mind that the function will return an \ref
 * false value if a null reference is received.
 */
bool     axl_doc_get_standalone (axlDoc * doc)
{
	axl_return_val_if_fail (doc, false);

	/* return current configuration */
	return doc->standalone;
}

/** 
 * @brief Allows to configure the document root for the given \ref
 * axlDoc instance.
 *
 * Every xml document has a xml node root. This is the first node,
 * that holds all childs. This function allows to configure that xml
 * document root. See also \ref axl_doc_get_root.
 *
 * Remember that previous document root will not be deallocated so,
 * the user space must take care about previous reference.
 *
 * @param doc The \ref axlDoc where the document root will be
 * configured.
 *
 * @param root The \ref axlNode used to configure the new document
 * root.
 */
void    axl_doc_set_root (axlDoc * doc, axlNode * root)
{
	axl_return_if_fail (doc);
	axl_return_if_fail (root);

	/* set the new root */
	doc->rootNode = root;

	/* set a refeference to the document root */
	axl_node_set_doc (root, doc);

	return;
}

/** 
 * @internal
 *
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

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "pushed a new parent into the stack <%s>, current status after operatoin: %d",
		   axl_node_get_name (node), axl_stack_size (doc->parentNode));
	
	return;
}

/** 
 * @internal Allows to make current \ref axlDoc to pop current parent
 * node, making the new parent node the previously opened.
 *
 * This API is deprecated (internal function used in the past).
 * 
 * @param doc The \ref axlDoc where the pop operation will be
 * performed.
 */
void     axl_doc_pop_current_parent       (axlDoc * doc)
{
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
 * @return true is the processing instruction is defined,
 * otherwise false is returned.
 */
bool      axl_doc_has_pi_target            (axlDoc * doc, char * pi_target)
{
	axlPI * pi;
	int     iterator = 0;
	int     length   = 0;

	
	axl_return_val_if_fail (doc,       false);
	axl_return_val_if_fail (pi_target, false);

	/* get the length for the items inserted */
	length = axl_list_length (doc->piTargets);
	while (iterator < length) {
		/* for each item inserted */
		pi = axl_list_get_nth (doc->piTargets, iterator);
		/* only check the first ocurrency */
		if (axl_cmp (pi->name, pi_target))
			return true;

		iterator++;
	}
	
	return false;
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
 *
 * @brief Allows to create a new \ref axlPI element. 
 * 
 * @param name The PI target name.
 * @param content The PI content.
 * 
 * @return A newly allocated \ref axlPI element.
 */
axlPI * axl_pi_create (char * name, char * content)
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
 * @brief Returns a newly allocated copy representing the same value
 * as the provided \ref axlPI reference.
 * 
 * @param pi The pi reference received.
 * 
 * @return A reference to the \ref axlPI element or null if it fails.
 */
axlPI   * axl_pi_copy                      (axlPI  * pi)
{
	axlPI * _pi;

	axl_return_val_if_fail (pi, NULL);

	/* create the PI */
	_pi          = axl_new (axlPI, 1);
	_pi->name    = axl_strdup (pi->name);
	
	/* copy the content if defined */
	if (pi->content != NULL)
		_pi->content = axl_strdup (pi->content);

	return _pi;
}

/** 
 * @brief Allows to check if both provided process instructions are
 * equal.
 * 
 * @param pi First process instruction to check.
 * @param pi2 Second process instructions to check.
 * 
 * @return \ref true if both process instructions are equal. If some
 * of parameters received are NULL, the function will always return
 * \ref false.
 */
bool      axl_pi_are_equal                 (axlPI  * pi, 
					    axlPI * pi2)
{
	/* basic null reference check */
	axl_return_val_if_fail (pi, false);
	axl_return_val_if_fail (pi2, false);

	/* check internal data */
	if (! axl_cmp (pi->name, pi2->name))
		return false;

	/* final check, both content must be equal */
	return axl_cmp (pi->content, pi2->content);
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
 * @param pi The target to destroy.
 */
void axl_pi_free (axlPI * pi)
{
	/* free PI target */
	axl_free (pi->name);
	if (pi->content != NULL) 
		axl_free (pi->content);
	axl_free (pi);
	return;
}

/** 
 * @internal Allows to get the number of bytes that the process
 * instruction will take.
 * 
 * @param pi The process instruction.
 * 
 * @return A size or -1 if it fails.
 */
int       axl_pi_get_size                  (axlPI  * pi)
{
	axl_return_val_if_fail (pi, -1);

	/* <?name content?> */
	return strlen (pi->name) + strlen (pi->content) + 5;
}

/** 
 * @internal
 *
 * Common implementation for \ref axl_doc_iterate and \ref axl_doc_iterate2.
 */
bool __axl_doc_iterate_common (axlDoc            * doc, 
			       axlNode           * root,
			       AxlIterationMode    mode, 
			       axlIterationFunc    func, 
			       axlIterationFunc2   func2, 
			       axlPointer          ptr, 
			       axlPointer          ptr2)
{
	int        iterator;
	bool       was_removed = false;

	axlNode  * node;
	axlNode  * nodeAux;

	axlList  * pending;

	/* check first node */
	axl_return_val_if_fail (root, false);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "notifying first node inside the iteration");

	/* notify first node found we pass in a null value because it
	   doesn't have a * parent. */
	if (func && ! func (root, NULL, doc, &was_removed, ptr))
		return false;
	if (func2 && ! func2 (root, NULL, doc, &was_removed, ptr, ptr2)) 
		return false;

	/* if the root node was removed, don't continue */
	if (was_removed)
		return false;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "continuing with next nodes");
	
	/* get childs */
	pending  = axl_node_get_childs (root);

	/* for each pending node */
	while (axl_list_length (pending) > 0) {

		/* get the first node inside the pending list */
		node = axl_list_get_first (pending);

		/* remove the node node from the pending list and add
		 * all childs */
		axl_list_remove_first (pending);

		/* notify node found */
		was_removed = false;
		if (func && ! func (node, axl_node_get_parent (node), doc, &was_removed, ptr)) {
			axl_list_free (pending);
			return false;
		}

		/* notify node found */
		if (func2 && ! func2 (node, axl_node_get_parent (node), doc, &was_removed, ptr, ptr2)) {
			axl_list_free (pending);
			return false;
		}

		/* add all its childs */
		if (!was_removed && axl_node_have_childs (node)) {
			
			/* get first child */
			nodeAux = axl_node_get_first_child (node);

			/* get all items of the next level and add
			 * them properly */
			iterator = 0;
			while (nodeAux != NULL) {

				/* add to the pending list */
				switch (mode) {
				case DEEP_ITERATION:
					/* add the element */
					axl_list_add_at (pending, nodeAux, iterator);
					
					/* update the iterator */
					iterator++;
					break;

				case WIDE_ITERATION:
					/* add to the pending list */
					axl_list_add (pending, nodeAux);
					break;
				} /* end switch */
	

				/* update to the next */
				nodeAux = axl_node_get_next (nodeAux);
				
			} /* end while */
		} /* end if */

		
	} /* end while */

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "terminated iteration process, deallocating list: %d",
		   axl_list_length (pending));
	
	axl_list_free (pending);
	
	/* iteration performed completely */
	return true;
}

/** 
 * @brief Allows to perform an iteration over the documented provided,
 * visiting all nodes inside it.
 *
 * The function allows to configure the iteration module using \ref
 * AxlIterationMode (mode variable) and providing a callback function
 * that will be called for each node found (\ref axlIterationFunc).
 *
 * The function, optionall, allows to provide a user pointer that will
 * be passed to the callback function. See documentation for the
 * callback and the iteration module for more details.
 *
 * Here is an example:
 * \code
 * void perform_iteration (axlDoc * doc)
 * {
 *     // call to iterate
 *     axl_doc_iterate (doc, 
 *                      // visit childs before brothers
 *                      DEEP_ITERATION, 
 *                      // the func to execute: see below
 *                      show_node_found, 
 *                      // optional user pointer
 *                      NULL);
 * }
 *
 * bool show_node_found (axlNode * node, axlNode * parent,
 *                       axlDoc  * doc, bool * was_removed, 
 *                       axlPointer ptr)
 * {
 *      // Show node found 
 *      printf ("Node found: %s\n", axl_node_get_name (node));
 *
 *      // If the node is removed inside the iteration
 *      // using axl_node_remove or axl_node_replace, you
 *      // must notify the iteration system using was_removed
 *      // as follow: (* was_removed) = true;
 *      //
 *      // If you don't remove anything, you don't need to do
 *      // anything especial with was_removed.
 *
 *      // don't stop iteration
 *      return true;
 * }
 * \endcode
 *
 * See also alternative APIs:
 * 
 *   - \ref axl_doc_iterate_full
 *   - \ref axl_doc_iterate_full_from
 * 
 * @param doc The xml document that will be iterated.
 *
 * @param mode The iterarion type to be performed.
 *
 * @param func The function to be called for each node found.
 *
 * @param ptr An user defined pointer that will be passed to the
 * callback function.
 *
 * @return The function returns \ref true if the iteration was
 * performed over all nodes or \ref false it it was stoped by the
 * iteration function (by returning \ref false to stop the
 * iteration). The function also false if the parameters provided doc
 * or func are not defined.
 */
bool      axl_doc_iterate                  (axlDoc           * doc,
					    AxlIterationMode   mode,
					    axlIterationFunc   func,
					    axlPointer         ptr)
{
	axlNode * root;

	/* check basic data */
	axl_return_val_if_fail (doc, false);
	axl_return_val_if_fail (func, false);

	/* get the root node where the iteration will start */
	root = axl_doc_get_root (doc);

	/* call to common implementation */
	return __axl_doc_iterate_common (doc, root, mode, func, NULL, ptr, NULL);

}


/** 
 * @brief Allows to perform an iteration over the documented provided,
 * visiting all nodes inside it (with two user defined pointers support).
 *
 * The function allows to configure the iteration module using \ref
 * AxlIterationMode (mode variable) and providing a callback function
 * that will be called for each node found (\ref axlIterationFunc).
 *
 * The function, optionall, allows to provide two user pointer that will
 * be passed to the callback function. See documentation for the
 * callback and the iteration module for more details. See also \ref axl_doc_iterate.
 *
 * 
 * @param doc The xml document that will be iterated.
 *
 * @param mode The iterarion type to be performed.
 *
 * @param func The function to be called for each node found.
 *
 * @param ptr An user defined pointer that will be passed to the
 * callback function.
 *
 * @param ptr2 Second user defined pointer that will be passed to the
 * callback function.
 * 
 *
 * @return The function returns \ref true if the iteration was
 * performed over all nodes or \ref false it it was stoped by the
 * iteration function (by returning \ref false to stop the
 * iteration). The function also false if the parameters provided doc
 * or func are not defined.
 */
bool      axl_doc_iterate_full             (axlDoc            * doc,
					    AxlIterationMode    mode,
					    axlIterationFunc2   func,
					    axlPointer          ptr,
					    axlPointer          ptr2)

{
	axlNode * root;

	/* check basic data */
	axl_return_val_if_fail (doc, false);
	axl_return_val_if_fail (func, false);

	/* get the root node where the iteration will start */
	root = axl_doc_get_root (doc);
	
	/* call to common implementation */
	return __axl_doc_iterate_common (doc, root, mode, NULL, func, ptr, ptr2);
}

/** 
 * @brief Allows to perform a iteration operation but configuring
 * where to start, discarding the rest content.
 *
 * See \ref axl_doc_iterate and \ref axl_doc_iterate_full for more
 * details. This function works the same like previous but, unlike
 * previous, this function doesn't use the default starting point: the
 * root node (\ref axl_doc_get_root). The function allows to configure
 * the node where to start the iteration operation. 
 *
 * This function is equivalent to \ref axl_doc_iterate_full calling if
 * it use the root node document as value for <b>starting_from</b>.
 * 
 * @param doc The xml document that will be iterated.
 *
 * @param starting_from The \ref axlNode where the operation will
 * start, discarding all content from ascending nodes, previous
 * siblings and following sibligins. From a iteration perspective, the
 * iteration opeeration.
 *
 * @param mode The iterarion type to be performed.
 *
 * @param func The function to be called for each node found.
 *
 * @param ptr An user defined pointer that will be passed to the
 * callback function.
 *
 * @param ptr2 Second user defined pointer that will be passed to the
 * callback function.
 * 
 *
 * @return The function returns \ref true if the iteration was
 * performed over all nodes or \ref false it it was stoped by the
 * iteration function (by returning \ref false to stop the
 * iteration). The function also false if the parameters provided doc
 * or func are not defined.
 */
bool      axl_doc_iterate_full_from        (axlDoc           * doc,
					    axlNode          * starting_from,
					    AxlIterationMode   mode,
					    axlIterationFunc2  func,
					    axlPointer         ptr,
					    axlPointer         ptr2)
{
	/* check basic data */
	axl_return_val_if_fail (doc, false);
	axl_return_val_if_fail (func, false);

	/* call to common implementation */
	return __axl_doc_iterate_common (doc, starting_from, mode, NULL, func, ptr, ptr2);
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
		axl_stack_free (doc->parentNode);

	/* free item factory */
	if (doc->item_factory != NULL)
		axl_factory_free (doc->item_factory);

	/* free node factory */
	if (doc->node_factory != NULL)
		axl_factory_free (doc->node_factory);

	if (doc->str_factory != NULL)
		axl_string_factory_free (doc->str_factory);

	/* free pi targets read */
	if (doc->piTargets != NULL)
		axl_list_free (doc->piTargets);

	if (doc->encoding != NULL) {
		/* free enconding allocated */
		axl_free (doc->encoding);
	}
	
	if (doc->version != NULL) {
		/* free allocated version value */
		axl_free (doc->version);
	}

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

	bool      found_item;
	char    * content;
	int       size;

	/* get current parent node */
	axlNode * parent = (doc != NULL) ? axl_stack_peek (doc->parentNode) : NULL;

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "checking for comemnts");

	/* know, try to read comments a process instructions.  Do this
	 * until both fails. Do this until one of them find
	 * something. */
	do {
		/* flag the loop to end, and only end if both,
		 * comments matching and PI matching fails. */
		found_item = false;
		
		/* get rid from spaces */
		AXL_CONSUME_SPACES(stream);

		/* check for comments */
		if (axl_stream_inspect (stream, "<!--", 4) > 0) {

			/* get comment content */
			content = axl_stream_get_until_ref (stream, NULL, NULL, true, &size, 1, "-->");
			if (content == NULL) {
				axl_error_new (-1, "detected an opened comment but not found the comment ending",
					       stream, error);
				axl_stream_free (stream);
				return false;
			} 

			/* store it */
			if (parent != NULL)
				axl_node_set_comment (parent, content, size);
			
			/* flag that we have found a comment */
			found_item = true;
		}
		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "now see for process instructions");
	
		/* get rid from spaces */
		AXL_CONSUME_SPACES(stream);

		/* check for PI, only once the xml header have been processed */
		if ((doc != NULL) && doc->headerProcess && (axl_stream_peek (stream, "<?", 2) > 0)) {
			
			if (! axl_doc_consume_pi (doc, axl_stack_peek (doc->parentNode), stream, error))
				return false;
			found_item = true;
		}
		
		/* do not consume spaces if an item was found because
		 * it is done again at the begin of the loop */
		if (! found_item) {
			/* get rid from spaces */
			AXL_CONSUME_SPACES(stream);
		}
		
		/* check to break-the-loop */
	}while (found_item);

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "comments and pi parsed");

	/* true value */
	return true;
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
 * @return true if not error was found, otherwise AXL_FASLSE is
 * returned.
 */
bool      axl_doc_consume_pi (axlDoc * doc, axlNode * node, 
				  axlStream * stream, axlError ** error)
{
	char  * string_aux;
	char  * string_aux2;
	int     matched_chunk;
	
	
	/* check if a PI target was found */

	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "calling to consume PI..");


	if (axl_stream_peek (stream, "<?", 2) > 0) {

		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found a process instruction initialization");

		/* found a pi target initialization */
		axl_stream_accept (stream);
		
		string_aux = axl_stream_get_until (stream, NULL, &matched_chunk, true, 3, 
						   " ?>", "?>", " ");
		/* check error reported */
		if (string_aux == NULL) {
			axl_error_new (-1, "Found a error while reading the PI target name", stream, error);
			axl_stream_free (stream);
			return false;
		}

		/* check that the reserved xml word is not used for the PI target */
		string_aux2 = axl_strdup (string_aux);
		if (axl_cmp (axl_stream_to_lower (string_aux2), "xml")) {
			axl_free (string_aux2);
			axl_error_new (-1, "Using a reserved PI target name (xml), not allowed", stream, error);
			axl_stream_free (stream);
			return false;
		}
		axl_free (string_aux2);


		__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "found PI target name: %s (terminator matched: %d)", 
			   string_aux, matched_chunk);

		/* check which was the matched string */
		if (matched_chunk == 0 || matched_chunk == 1) {
			/* seems that the PI target doesn't have more data associated, craete and return */
			if (node != NULL) {
				axl_node_add_pi_target (node, string_aux, NULL);
				return true;
			}
			
			if (doc != NULL)
				axl_doc_add_pi_target (doc, string_aux, NULL);
			return true;
		}

		/* seems that we have additional content to be read */
		if (matched_chunk == 2) {
			/* make a local copy for the PI target name
			 * read previously */
			string_aux  = axl_strdup (string_aux);
			
			/* get the PI content */
			string_aux2 = axl_stream_get_until (stream, NULL, NULL, true, 2, " ?>", "?>");

			/* check error reported */
			if (string_aux2 == NULL) {
				axl_free (string_aux);
				axl_error_new (-1, "Found a error while reading the PI content", stream, error);
				axl_stream_free (stream);
				return false;
			}

			/* check the destination for the pi */			
			if (node != NULL) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "PI processing finished, adding PI (node) and its content");

				axl_node_add_pi_target (node, string_aux, string_aux2);
				axl_free (string_aux);
				return true;
			}


			if (doc != NULL) {
				__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "PI processing finished, adding PI (doc) and its content");
				axl_doc_add_pi_target (doc, string_aux, string_aux2);
				axl_free (string_aux);
				return true;
			}

		}

		/* check error reported */
		axl_error_new (-1, "Found a error while reading the PI target name, unable to find PI terminator ?>", stream, error);
		axl_stream_free (stream);
		return false;
	}

	
	__axl_log (LOG_DOMAIN, AXL_LEVEL_DEBUG, "PI processing finished");


	return true;
}

/** 
 * @internal Function that allows to get axlFactory associated to
 * the provided document.
 * 
 * @param doc The axl document that is requested to return its item
 * factory.
 * 
 * @return An internal reference to the item factory. Do not dealloc.
 */
axlFactory * axl_doc_get_item_factory  (axlDoc * doc)
{
	return doc->item_factory;
}

/* @} */
