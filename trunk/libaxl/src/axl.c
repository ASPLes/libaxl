/*
 *  @internal
 * 
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
 *         Edificio Alius A, Oficina 102,
 *         C/ Antonio Suarez Nº 10,
 *         Alcalá de Henares 28802 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/xml
 */
#include <axl.h>

/** 
 * \mainpage AXL: Another XML Library implementation (XML 1.0 standard)
 *
 * \section intro Introduction
 *
 * AXL is a library which aims to implement the XML 1.0 standard, as
 * defined at the XML 1.0 third edition
 * recommendation found at: http://www.w3.org/TR/REC-xml/. 
 * 
 * It was implemented to support XML requirements inside projects
 * developed by <a href="http://www.aspl.es">Advanced Software
 * Production Line, S.L.</a>, especially <a
 * href="http://fact.aspl.es">Af-Arch</a> and <a
 * href="http://vortex.aspl.es">Vortex Library</a>, which are already
 * using the library successfully in production environments.
 * 
 * Main objectives for this library is to provide:
 *
 * <ul>
 * 
 * <li><b>A clean implementation</b>, that only includes, those elements
 * defined, and only those, inside the XML 1.0 standard, as defined in <a href="http://www.w3.org/TR/REC-xml/">the third edition</a>.</li>
 *
 * <li>To be a <b>fast and memory efficient</b> implementation. If you still
 * think that XML is slow and memory inefficient, you didn't taste Axl
 * Library. You'll find report about its performance and memory usage at: http://xml.aspl.es/doc.html </li>
 *
 * <li>Ensure that the library is implemented using abstract data types,
 * commonly known as opaque types, to avoid exposing details to the
 * user space code that is consuming the API. 
 * This will allow us to improve the library without breaking existing
 * code and, the most important, to not be limited by details already
 * exposed.
 * In fact, this design have being already used to improve the library
 * performance greatly. See reports founds at: http://xml.aspl.es/doc.html</a>.</li>
 *
 * <li>To be <b>small and efficient</b>, ensuring on every stable release that
 * the library do not leak, not only while using the library in a
 * proper manner but also when errors were found. 
 * This point is really important for us because Af-Arch server
 * applications needs to be working for a long time. </li>
 *
 * <li>Have a <b>modular</b> design that allows to use only those
 * elements required by your software. At this moment it is provided a
 * base library (with implements XML 1.0) and an optional library that
 * provides additional features to support XML Namespaces 1.0.</li>
 *
 * <li>The other issue is that the Af-Arch client platform should be
 * easily embedded, so, a <b>small footprint</b> is a requirement. Currently LibAxl (89K) and LibAxl-Ns (9K)</li>
 *
 * </ul>
 *
 * \section features_and_status What is the status of the library
 *
 * Currently the library is stable and it is known to work under
 * GNU/Linux and Windows with a really good performance. See reports found at http://xml.aspl.es/doc.html to know more about this.
 *
 * The library already covers the 95% of common requires that XML
 * development needs. Among others, it support:
 *
 *  - XML tree parsing, from memory and files, allowing a great level
 * of detail while accessing to the data (comments, process
 * instructions, xml nodes, and content).
 * 
 * - Mostly completed DTD validation support, including <!ATTLIST>, <!ENTITY>
 * and <!ELEMENT> elements. Remains to implement NOTATION declarations
 * and full entity replacement.
 *
 *  - Two modes to inspect the xml documents at the same time,
 *  <b>MIXED API</b>: an API to traverse the document allowing access
 *  to all items found (\ref axlItem) inside the document (\ref
 *  axlDoc) root node (\ref axlNode), and <b>CHILDREN API</b>: an API
 *  that allows to traverse the node using as reference only the nodes
 *  (\ref axlNode) inside the document (\ref axlDoc).
 *
 *  - <a href="http://www.w3.org/TR/REC-xml-names/">XML 1.0
 *  Namespaces</a> full support, through the additional component
 *  (libaxl-ns), allowing to produce xml applications that are
 *  XML Namespace aware. 
 * 
 * \section documentation Library Documentation
 *
 * The library documentation is composed into two pieces. The Axl
 * manual and the API documentation. 
 * 
 * - \ref axl_install
 * - \ref axl_manual
 * - \ref axl_api
 * - \ref axl_knife_manual
 * 
 * \section contact_us Contact us
 * 
 * If you find something not properly documented, or some question is
 * not answered on this documentation, check the <a href="http://www.aspl.es/axl/doc.html">mailing list</a>.
 *
 * You can also contact us if you have patches, improvements or
 * suggestions at the mailing list.
 */

/** 
 * \page axl_manual XML development with Axl Library
 *
 * \section Manual Index
 *
 * On this manual you'll find the following section:
 * 
 * <b>Section 1: Basic elements to understand XML and Axl </b><br>
 *
 * - \ref intro
 * - \ref concepts
 * - \ref two_apis
 *
 * <b>Section 2: Manipulating and producing XML documents </b><br>
 *
 * - \ref parsing
 * - \ref iterating
 * - \ref modifying
 * - \ref dumping_functions
 *
 * <b>Section 3: Doing validation on your documents </b><br>
 *
 * - \ref validation
 * - \ref xml_namespace
 *
 * <b>Section 4: Advanced topics </b><br>
 *
 * - \ref reducing_foot_print
 *
 * <b>Apendix</b><br>
 *
 * - \ref futher
 *
 *
 * \section intro Introduction: XML development
 * 
 * XML 1.0 definition allows to build documents that could be used to
 * represents textual information, remote procedure invocations or
 * dynamic user interfaces. Its definition is based on very simple
 * principles, that allows to developers to compose them to create
 * bigger abstractions that are roughly on every place in modern
 * computer software design.
 *
 * It is a "quite" human readable format, so you will find that is not
 * the best format if you are looking for space efficiency. What XML
 * 1.0 provides you on the other hand is the ability to quickly
 * prototype and produce working formats that encapsulate your data,
 * and, as your system evolves, XML 1.0 will do it with you.
 *
 * Among other things, XML 1.0 provides you ways to validate your
 * documents to ensure your code will read XML documents in the format
 * expected, reducing the time and development cost due to additional
 * checkings required.
 *
 * Before continuing, we will explain some concepts that are required
 * to understand XML 1.0 and why the Axl API was built this way.
 *
 * \section concepts Some concepts before starting to use Axl Library
 *
 * Here is a simple example of a XML 1.0 document:
 * <div class="xml-doc">
 * \code
 * <?xml version="1.0">
 * <!-- This is a comment -->
 * <complex>
 *   <data>
 *     <simple>10</simple>
 *     <empty attr1="value1" />
 *   </data>
 * </complex>
 * \endcode
 * </div>
 *
 * Previous XML document represents an structure with a top level
 * node, called <b>complex</b>, that has one single child called
 * <b>data</b> which in turn have two childs. The first one is the
 * child called <b>simple</b> that have content and other one, called
 * <b>empty</b>, which is a node usually called an empty xml node.
 *
 * The XML representation for previous document is the following:
 * \image html image01.png "Document representation"
 *
 * Several issues must be considered while interpreting previous
 * diagram and how Axl library parse and expose those elements through
 * the API to the client application:
 * <ul>
 * 
 * <li>Every XML document have a root node (\ref axl_doc_get_root). Without exception. In this
 * case, the root node for our example is <b>complex</b>. </li>
 * 
 * <li>If a node have content, that content is not represented with
 * another node. That content is associated to the node and could be
 * retrieved using several function (\ref axl_node_get_content, \ref
 * axl_node_get_content_copy and \ref
 * axl_node_get_content_trans). 
 * 
 * Alternatively, while using the MIXED API, you can traverse child
 * items stored for a particular node, detecting those items that are
 * \ref ITEM_CONTENT or \ref ITEM_CDATA (using \ref
 * axl_item_get_type). </li>
 *
 * <li>Having a node (\ref axlNode) with content doesn't mean to have a node with
 * childs. The child notion is only about having more xml nodes (\ref axlNode) as
 * childs. 
 * 
 * This is particularly important if you take into consideration that a
 * node could have content (\ref ITEM_CONTENT), comments (\ref
 * ITEM_COMMENT), application process instructions (\ref ITEM_PI),
 * CDATA content (uninterpreted content \ref ITEM_CDATA), all of them
 * mixed with more xml nodes (\ref ITEM_NODE). </li>
 *
 * <li>A final node which is empty because it doesn't have content or
 * childs, is usually referred to as <b>EMPTY</b> type node. A final
 * node with content but no childs is usually referred to as
 * <b>PCDATA</b>. A node that have content mixed with references to
 * more child xml nodes is referred to as <b>MIXED</b>.</li>
 *
 * <li>At the <b>empty</b> node, you'll find that it has an attribute
 * called <b>attr1</b> with a value <b>value1</b>. A node could have
 * any number of attributes but, it should be named
 * differently. Again, if a node is empty, it keeps empty even if it
 * has attributes.
 * </li>
 *
 * So, to summarize, we have a root node, that could contain more
 * nodes, that could contain PCDATA, or content, and those nodes could
 * contain named attributes with values.
 *
 * \section two_apis MIXED and CHILDREN API: How to use them
 *
 * XML 1.0 is used for a variety of purposes, some of them requires
 * the CHILDREN API and the rest the MIXED API. To <i>require</i>, we
 * mean that it fits better, so you will get better results, your
 * application will react in a proper manner and you'll have to do
 * less work.
 *
 * The reason for this API is simple. XML 1.0 definition allows to mix
 * content with more nodes, comments and many more elements to be
 * placed as childs for a particular node. 
 * 
 * This definition, found at the standard, have moved many XML
 * implementations to support only an API that support all these
 * features, that is, an interface that is complicated and overloaded,
 * that gives you a power that you don't require, making your
 * development more inefficient.
 *
 * As a result, when a developer only requires a usual form of xml,
 * called CHILDREN, that means nodes have only another childs
 * nodes or content but not both at the same time. This kind of xml is
 * really useful, easy to parse, easy to make a DTD definition, more
 * compact and extensible.
 *
 * Lets see an example for both formats to clarify:
 * 
 * <div class="xml-doc">
 * \code
 * <?xml version='1.0' ?>
 * <document>
 *    <!-- Children XML format example: as you can see      -->
 *    <!-- nodes only contains either nodes or node content -->
 *    <!-- but nothing mixed at the same level              -->
 *    <node1> 
 *       This is node1 content 
 *    </node1>
 *    <node2>
 *      <node3>
 *         This is node3 content
 *      </node3>
 *      <node4 />
 *    </node2>
 * </document>
 * \endcode
 * </div>
 *
 * While an MIXED xml document could be:
 * 
 * <div class="xml-doc">
 * \code
 * <?xml version='1.0' ?>
 * <document>
 *    <!-- Children XML format example: as you can see      -->
 *    <!-- nodes only contains either nodes or node content -->
 *    <!-- but nothing mixed at the same level              -->
 *    <node1> 
 *       This is node1 content 
 *    </node1>
 *    Content mixed with xml nodes at the same level. 
 *    <node2>
 *      More content....
 *      <node3>
 *         This is node3 content
 *      </node3>
 *      <node4 />
 *    </node2>
 * </document>
 * \endcode
 * </div>
 *
 * Both approaches, which are valid using the XML 1.0 standard, are
 * appropriate for particular situations:
 * 
 * - CHILDREN API: compact representations, configuration files, rpc
 * invocation description, graphical user interface definition.
 * - MIXED API: textual description, for example: XSL-FO.
 *
 * Having introduced the context of the problem, Axl Library takes no
 * position, providing an API that fits while developing xml content
 * that follows a CHILDREN description and an API for the MIXED
 * description.
 * 
 * In this context, which API you use, will only affect to the way you
 * traverse the document. The CHILDREN API is mainly provided by the
 * \ref axl_node_module "Axl Node interface" and the MIXED API is
 * mainly provided by the \ref axl_item_module "Axl Item interface".
 *
 * You don't need to do any especial operation to activate both APIs,
 * both are provided at the same time. Lets see an example:
 *
 * Supposing the previous mixed example, the following code will get
 * access to the &lt;node2> reference:
 * \code
 * // supposing "doc" reference contains the document loaded
 * axlNode * node;
 * 
 * // get the document root, that is <document>
 * node = axl_doc_get_root (doc);
 *
 * // get the first child for the document root (<node1>)
 * node = axl_node_get_first_child (node);
 *
 * // get the next child (brother of <node1>, that is <node2>)
 * node = axl_node_get_next (node);
 * \endcode
 *
 * However, with the MIXED API you can get every detail, every item
 * found for a particular node. This is how:
 * 
 * \code
 * // supposing "doc" reference contains the document loaded
 * axlNode * node;
 * axlItem * item;
 * 
 * // get the document root, that is <document>
 * node = axl_doc_get_root (doc);
 *
 * // get the first item child for the document root that is the comment:
 * //    "Children XML format example: as you can see".
 * item = axl_item_get_first_child (node);
 *
 * // now skip the following two comments
 * item = axl_item_get_next (item);
 * item = axl_item_get_next (item);
 *
 * // now the next item is holding the <node1>
 * item = axl_item_get_next (item);
 * node = axl_item_get_data (item);
 *
 * // now get the content between the <node1> and <node2>
 * item = axl_item_get_next (item);
 *
 * // and finally, get the next child (brother of <node1>, that is
 * // <node2>)
 * item = axl_item_get_next (item);
 * node = axl_item_get_data (item);
 * \endcode
 *
 * Obviously, the mixed example contains more code and it is more
 * fragile to xml document changes. The problem is that the MIXED API
 * is more general than the CHILDREN, making XML libraries to only
 * provide that API.
 *
 * As a consequence:
 *
 * - You only need to use the MIXED API (\ref axlItem) if you are
 * going to do an xml application that allows having content mixed
 * with nodes, comments, etc, and you want to get access to such
 * content.
 *
 * - If you are planing to develop an XML solution that represents
 * information (user interfaces), module descriptions, configuration
 * files, etc, try to use the CHILDREN API: it will save you lot of
 * work! Remember, CHILDREN xml format: childs are either content or
 * more xml nodes but not both. Never mixed.
 *
 * \section parsing Parsing XML documents
 * 
 * We have seen how an XML document is. Now we are going to see how to
 * parse those document into data structures that are usable to
 * inspect the content. All parsing functions are available at the
 * \ref axl_doc_module "Axl Doc interface".
 *
 * Let's start with a very simple example:
 *
 * \code
 * #include <axl.h>
 * #include <stdio.h>
 *
 * int main (int argc, char ** argv)
 * {
 *    axlError ** error;
 *
 *    // top level definitions 
 *    axlDoc * doc = NULL;
 *
 *    // initialize axl library 
 *    if (! axl_init ()) {
 *        printf ("Unable to initialize Axl library\n");
 *	  return -1;
 *    }
 *
 *    // get current doc reference 
 *    doc = axl_doc_parse_from_file ("large.xml", error);
 *    if (doc == NULL) {
 *        axl_error_free (error);
 *        return false;
 *    }
 *
 *    // DO SOME WORK WITH THE DOCUMENT HERE
 *
 *    // release the document 
 *    axl_doc_free (doc);
 *
 *    // cleanup axl library 
 *    axl_end ();
 *
 *    return true;
 * }
 * \endcode
 *
 * \section iterating Traveling an XML document
 * 
 * Once the document is loaded you can use several function to
 * traverse the document. 
 * 
 * First you must use \ref axl_doc_get_root to get the document root
 * (\ref axlNode) which contains all the information. Then, according
 * to the interface you are using, you must call to either \ref
 * axl_node_get_first_child or \ref axl_item_get_first_child.
 * 
 * Once you have access to the first element, you can use the
 * following set of function to get more references to other nodes or
 * items:
 *
 * <ul>
 *  <li><b>MIXED API</b>: 
 *
 *    - \ref axl_item_get_first_child
 *    - \ref axl_item_get_last_child
 *    - \ref axl_item_get_next
 *    - \ref axl_item_get_previous
 *
 *  </li>
 *  <li><b>CHILDREN API</b>:
 *
 *    - \ref axl_node_get_first_child
 *    - \ref axl_node_get_last_child
 *    - \ref axl_node_get_next
 *    - \ref axl_node_get_previous
 *
 *  </li>
 * </ul>
 *
 * There are alternative APIs that will allow you to iterate the
 * document, providing a callback: \ref axl_doc_iterate. 
 *
 * Another approach is to use \ref axl_doc_get and \ref
 * axl_doc_get_content_at to get fast access to a particular node
 * using a really limited XPath syntax.
 *
 * \section modifying Modifying a loaded XML document
 *
 * One feature that comes with Axl Library is ability to modify the
 * content, replacing it with other content and transferring node node
 * to another place.
 * 
 * Check the following function while operating with \ref axlNode elements:
 * 
 *   - \ref axl_node_replace
 *   - \ref axl_node_remove
 *   - \ref axl_node_transfer_childs
 *
 * Check the following functions while operating with \ref axlItem elements:
 * 
 *   - \ref axl_item_replace
 *   - \ref axl_item_remove
 *   - \ref axl_item_transfer_childs_after
 *
 * \section dumping_functions Producing xml documents from memory
 *
 * Axl Library comes with several functions to perform xml memory dump
 * operations, allowing to translate a xml representation (\ref axlDoc
 * or \ref axlNode) into a string:
 *
 * - \ref axl_doc_dump
 * - \ref axl_doc_dump_pretty
 * - \ref axl_doc_dump_to_file
 * - \ref axl_doc_dump_pretty_to_file
 *
 * In the case you want to produce xml content taking as reference a
 * particular node use:
 *  
 * - \ref axl_node_dump
 * - \ref axl_node_dump_pretty
 * - \ref axl_node_dump_to_file
 * - \ref axl_node_dump_pretty_to_file
 *
 * \section validation Validating XML documents
 *
 * Once you are familiar with the Axl API, or any other XML toolkit,
 * it turns that it is not a good practice to write lot of source code
 * to check node names expected or how they are nested. This makes
 * your program really weak to changes and makes your to write more
 * code that is not actual work but a simple environment check.
 *
 * You may also need to check that some XML document received follows
 * a defined XML structure, but it is too complex to be done.
 *
 * For this purpose, XML 1.0 defines DTD or (Document Type Definition)
 * which allows to specify the document grammar, how are nested
 * nodes, which attributes could contain, or if the are allocated to
 * be empty nodes or nodes that must have another child nodes.
 *
 * Let start with the DTD syntax used to configure restrictions about
 * node structure:
 *
 * <div class="xml-doc">
 * \code
 * <!-- sequence specification -->
 * <!ELEMENT testA (test1, test2, test3)>
 *
 * <!-- choice specification -->
 * <!ELEMENT testB (test1 | test2 | test3)>
 * \endcode
 * </div>
 *
 *
 * DTD <b><!ELEMENT</b> is modeled on top of two concepts which are
 * later expanded with repetition patterns. We will explain then
 * later. For now, this two top level concepts are: sequence and choice.
 *
 * Sequence specification (elements separated by <b>, (comma)</b>, the
 * one used to apply restriction to the node <b>testA</b>, are used to
 * denote that <b>testA</b> have as childs test1, followed by test2
 * and ended by test3. The order specified must be followed and all
 * instances must appear. This could be tweaked using repetition
 * pattern.
 *
 * In the other hand, choice specification (elements separated by
 * <b>| (pipe)</b>, are used to specify that the content of a node is
 * built using nodes of the choice list. So, in this case,
 * <b>testB</b> node could have either one instance of test1 or test2
 * or test3.
 *
 * Now you know these to basic elements to model how childs are
 * organized for a node, what it is need is to keep on adding more
 * <!ELEMENT directives until all nodes are specified. You will end
 * your DTD document with final nodes that are either empty ones or
 * have PCDATA. At this moment MIXED nodes are not supported.
 *
 * Suppose that all nodes that are inside testA and testB are final
 * ones. Then this could be its DTD specification:
 *
 * <div class="xml-doc">
 * \code
 * <!-- test1 is a node that only have content -->
 * <!ELEMENT test1 (#PCDATA)>
 *
 * <!-- test2 is a node that is always empty -->
 * <!ELEMENT test1 EMPTY>
 *
 * <!-- test3 is a node that could have either test1 or test2 -->
 * <!ELEMENT test3 (test1 | test2)>
 * \endcode
 * </div>
 *
 * Sequences and choices could be composed to create richer DTD
 * expressions that combines sequences of choices and so on.
 * 
 * At this point all required elements to model choices, sequences and
 * final nodes are explained, but, we have to talk about repetition
 * pattern. They are symbols that are appended to elements inside
 * choices (or sequences) including those list specifications.
 *
 * Patterns available are: <b>+</b>, <b>?</b> and <b>*</b>. By
 * default, if no pattern is applied to the element, it means that the
 * match should be produced one and only one time.
 *
 * The <b>+</b> pattern is used to model that element should be
 * matched one, and at least one, or more.
 *
 * The <b>*</b> pattern is used to model elements that should be
 * matched zero or any times.
 *
 * The <b>?</b> pattern is used to model elements that should be
 * matched zero or one times.
 *
 * For the exampled initially explained, let's suppose we want that
 * the content inside <b>testA</b> have sequences repeated at leat one
 * time, being that sequence: test1, test2 and test3. We only need to
 * add a <b>+</b> repetition pattern as follows:
 *
 * <div class="xml-doc">
 * \code
 * <!-- sequence specification -->
 * <!ELEMENT testA (test1, test2, test3)+>
 * \endcode
 * </div>
 *
 * So, we are saying to our validation engine that the sequence inside
 * testA could be found one or many times, but the entire sequence
 * match be found every time.
 *
 * Here is an simple example that loads an XML document, then loads an
 * DTD file, and then validates the XML document:
 * \code
 * bool test_12 (axlError ** error) 
 * {
 *	axlDoc * doc = NULL;
 *	axlDtd * dtd = NULL;
 *
 *	// parse gmovil file (an af-arch xml chunk) 
 *	doc = axl_doc_parse_from_file ("channel.xml", error); 
 *	if (doc == NULL) 
 *		return false;
 *
 *	// parse af-arch DTD 
 *	dtd = axl_dtd_parse_from_file ("channel.dtd", error);
 *	if (dtd == NULL)
 *		return false;
 *
 *	// perform DTD validation 
 *	if (! axl_dtd_validate (doc, dtd, error)) {
 *		return false;
 *	}
 *
 *	// free doc reference 
 *	axl_doc_free (doc); 
 *
 *      // free dtd reference
 *      axl_doc_free (dtd);
 * 
 *      return true;
 * }
 * \endcode
 *
 * \section xml_namespace Enabling your software with XML Namespaces
 *
 * XML 1.0 initial design didn't take care about situations where
 * several software vendors could introduce content inside the same
 * XML documents. This has several benefits, but one problem to solve:
 * <i>how to avoid xml node names (tags) to clash from each other.</i>
 *
 * Think about using &lt;table> as a tag for your document. Many XML
 * applications uses &lt;table> as a valid tag for its XML language
 * set. However, each of them has a different meaning and must be
 * handled by the proper XML software.
 *
 * While developing applications with XML, and supposing such XML
 * documents will be used by more applications than yours, you are
 * likely to be interested in use XML Namespaces. In other words, many
 * of the new XML standards that are appearing uses XML Namespaces to
 * allow defining its xml node names, while allowing users/developers
 * to use their own set of xml tags, under their own XML Namespaces,
 * in order they can use them in the same document.
 *
 * XML Namespaces support inside Axl Library is handled through a
 * separated library, which requires the base library to function. \ref axl_install "Here are some instructions to get Axl Library Namespace installed."
 *
 * This library provides functions that replaces some of the functions
 * used by XML applications that don't require XML Namespaces. In
 * particular, some of them are:
 * 
 * - \ref axl_ns_doc_validate (see this for an example)
 * - \ref axl_ns_node_cmp
 *
 * See also API documentation for all functions that are provided to
 * enable your application with XML Namespaces:
 * 
 * - \ref axl_ns_doc_module
 * - \ref axl_ns_node_module
 * 
 * \section reducing_foot_print How to reduce the library footprint 
 *
 * Axl Library is implemented in a modular way to ensure you are only
 * linked against those software elements that you really
 * require. Additionally, the library allows the following to reduce
 * the library footprint to the minimum: 
 *
 * <ul>
 * <li><b>Remove log information:</b> <br> Axl library uses a console log
 * mechanism to report what's happening during processing. See \ref
 * axl_log_module "Axl Log reporting" module for more
 * information. However, under production environments this console
 * log isn't necessary, so you can safely remove it, at compile time,
 * using <b>--axl-log-disable</b> as follow:
 *
 * \code
 * >> ./configure --axl-log-disable
 * \endcode
 *
 * According to our results, the library including the log to console
 * information is about <b>366K</b>. Without log to console information the
 * library takes about <b>288K</b>.
 *
 * </li> 
 * 
 * <li><b>Remove debugging information from the library: </b> <br>You
 * can also remove debugging information from your library on
 * production environments doing the following once finished compilation process:
 *
 * \code
 * >> make install-strip
 * \endcode
 *
 * According to our results, the library without log to console and
 * debugging information takes about <b>100K</b>.
 *
 * </li>
 * </ul>
 *
 * Previous information applies to the Axl base Library
 * (libaxl.so/.dll), however the same happens for the rest of software
 * components bundle with Axl.
 * 
 *
 * \section futher Futher reading where to go for more information
 * 
 * You can also check \ref axl_api "API documentation" for a complete
 * detailed explanation about the library. 
 *
 * Please, if you find that something isn't properly documented or you
 * think that something could be improved, contact us in the mailing
 * list. We are building Axl Library with the aim to produce a high
 * quality, commercial grade, open source XML development kit, so, any
 * help received will be welcome.
 *
 * Remember you can always contact us at the mailing list for any
 * question not properly answered by this documentation. See <a
 * href="http://www.aspl.es/axl/doc.html">Axl Library website
 * documentation</a> to get more
 * information about mailing list.
 *
 */

/** 
 * \page axl_api LibAxl API documentation
 *
 * Here is the API for the modules defined inside the library:
 * 
 * <ul>
 *  <li><b>Basic API to interact with XML documents (base library libaxl): </b></li>
 * 
 *    - \ref axl_module
 *    - \ref axl_doc_module
 *    - \ref axl_node_module
 *    - \ref axl_node_attribute_cursor
 *    - \ref axl_node_annotate
 *    - \ref axl_item_module
 *    - \ref axl_dtd_module
 *    - \ref axl_decl_module
 *    - \ref axl_handlers
 *
 *  </li>
 *  <li><b>XML Namespaces API (required additional library libaxl-ns): </b></li>
 *
 *    - \ref axl_ns_doc_module
 *    - \ref axl_ns_node_module
 *
 *  </li>
 *  <li><b>Error reporting and debugging functions (base library libaxl): </b></li>
 * 
 *    - \ref axl_error_module
 *    - \ref axl_log_module
 *
 *  <li><b>Auxiliary modules, supporting data types, string handling, etc (base library libaxl):</b></li>
 *
 * - \ref axl_stream_module
 * - \ref axl_list_module
 * - \ref axl_list_cursor_module
 * - \ref axl_stack_module
 * - \ref axl_binary_stack_module
 * - \ref axl_hash_module
 * - \ref axl_hash_cursor_module
 * - \ref axl_string_module 
 *
 *  </li>
 * </ul>
 * 
 */

/**
 * @page axl_install Installing Axl library and using it
 *
 * \section intro Introduction
 *
 * Axl library is an XML library written in ANSI C, which is known to
 * work on Microsoft Windows, GNU/Linux, *-BSD, GNU/Linux under
 * Amd64/Itanium and MacOS/X platforms. 
 *
 * The library package is composed by the following items:
 * 
 * - libaxl: base library implementing all XML functions. It has no
 * external dependencies.
 *
 * - libaxl-sn: optional library, built on top of libaxl, which
 * provides Namespaces support. It depends on libaxl.
 *
 * - axl-knife: command line tool, built on top of libaxl and
 * libaxl-ns. It depends on the base library and the namespace library.
 *
 * Here are a set of instructions to get the library compiled for your
 * platform:
 *
 * \section unix GNU/Linux (or any posix OS) installation instructions
 *
 * First, download the package from the download section. Check <a
 * href="http://www.aspl.es/axl/doc.html">this section to know more
 * about this.</a>
 *
 * Then use the standard procedure to get compiled an autotool ready
 * package. Here are the steps:
 *
 * \code
 *  >> tar xzvf axl-0.2.1.b1984.g1985.tar.gz
 *  >> cd axl-0.2.1.b1984.g1985/
 *  >> ./configure
 *  >> make
 *  >> make install
 * \endcode
 * 
 *
 * \section windows Windows installation instructions
 *
 * Axl library comes with packages already built for Microsoft Windows platforms.
 * Check <a href="http://www.aspl.es/axl/doc.html">the download section to get them</a>.
 *
 * \section using Using LibAxl library (installations with pkg-config infrastructure)
 *
 * Axl Library is composed by a base library, which provides all XML
 * 1.0 function. Additionally, a separated library is provided for
 * Namespace functions. 
 * 
 * Axl library comes with pkg-config support, which makes easy to use
 * it inside your pkg-config enable projects.
 *
 * To check current support for your Axl Library you can execute:
 * 
 * \code
 *  >> pkg-config --libs --cflags axl
 * \endcode
 *
 * In the case you want give support for XML Namespaces, you must use:
 * \code
 *  >> pkg-config --libs --cflags axl-ns
 * \endcode
 *
 * To include support into your autotool checks (configure.ac/in
 * files) you can place the following piece:
 * \code
 * PKG_CHECK_MODULES(LIBRARIES, axl) 
 * \endcode
 *
 * Again, add <b>axl-ns</b> to the previous instruction if you want
 * your package to also check for Axl Library Namespace support.
 * 
 * \section Including Axl Library headers
 *
 * For all platforms, Axl Library base headers has to be included as follows:
 * \code
 * #include <axl.h>
 * \endcode
 *
 * In the case Namespace is required, you must use: 
 * \code
 * #include <axl_ns.h>
 * \endcode
 */


/**
 * \defgroup axl_module Axl main: Init functions for the library
 */

/** 
 * \addtogroup axl_module
 * @{
 */

/** 
 * @brief Initializes Axl library.
 *
 * Currently this function is not required to be executed because
 * libaxl implementation don't use it. The implementation will try to
 * make no used for internal variable initialization.
 *
 * However the API is provided for future usage.
 * 
 * @return The function returns true if it was properly
 * initialized or false if something fails.  if fails.
 */
bool axl_init ()
{
	/* nothing to initialize dude */
	return true;
}

/** 
 * @brief Allows to terminate libaxl library function.
 *
 * Currently this function is not required becasue nothing is done to
 * stop libaxl function. The implementation will try to keep this,
 * however, if the future this could be required.
 */
void axl_end ()
{
	/* nothing to do jack */
	return;
}

/* @} */
