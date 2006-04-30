/**
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
 *  This program is distributed in the hope that it will be usneful,
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
#include <axl.h>
#include <stdarg.h>
#include <stdio.h>

/** 
 * \mainpage AXL: Another XML Library implementation (XML 1.0 standard)
 *
 * \section intro Introduction
 *
 * AXL is a library which aims to implement the XML 1.0 standard, as
 * defined at this moment, at the XML 1.0 third edition
 * recomendation found at: http://www.w3.org/TR/REC-xml/
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
 * <li>A clean implementation, that only includes, those elements
 * defined, and only those, inside the XML 1.0 standard. As of this
 * writing, that standard is <a href="http://www.w3.org/TR/REC-xml/">the third edition</a>.</li>
 *
 * <li>Ensure that the library is implemented using abstract data types,
 * commonly known as opaque types, to avoid exposing details to the
 * user space code that is consuming the API. 
 * This will allow us to improve the library without breaking existing
 * code and, the most important, to not be limited by details already
 * exposed.
 * In fact, this design have being already used to improve the library
 * performance greatly as is showed on <a href="http://www.aspl.es/axl/memory-usage-report-17-04-2006.html">this
 * report</a>.</li>
 *
 * <li>To be small and efficient, ensuring on every stable release that
 * the library do not leak, not only while using the library in a
 * proper manner but also when errors were found. 
 * This point is really important for us because Af-Arch server
 * applications needs to be working for a long time. </li>

 * <li>The other issue is that the Af-Arch client platform should be
 * easily embeded, so, a small footprint is a requirement.</li>
 *
 * </ul>
 *
 * \section features_and_status What is the status of the library
 *
 * Currently the library is stable and it is known to work under
 * GNUL/Linux and Windows with a really good performance. See <a href="http://www.aspl.es/axl/memory-usage-report-17-04-2006.html">this
 * report</a> to know more about this.
 *
 * The library already covers the 95% of common requires that XML
 * development. Among others, it support XML parsing, from memory and
 * files, and DTD validation. 
 * 
 * At this moment DTD validation is limited to the <b><!ELEMENT</b>
 * directive, allowing to only validate XML structure. 
 *
 * The library is being used by <a href="http://fact.aspl.es">Af-Arch</a> and <a
 * href="http://vortex.aspl.es">Vortex Library</a> which are projects
 * with common XML requirements.
 *
 * \section documentation Library Documentation
 *
 * The library documentation is splited into two pieces. The Axl
 * manual and the API documentation. 
 * 
 * - \ref axl_install
 * - \ref axl_manual
 * - \ref axl_api
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
 * - \ref intro
 * - \ref concepts
 * - \ref parsing
 * - \ref iterating
 * - \ref validation
 * - \ref futher
 *
 *
 * \section intro Introduction 
 * 
 * XML 1.0 definition allows to build documents that could be used to
 * represents textual information, remote procedure invocations or
 * dynamic user interfaces. Its definitions is based on very simple
 * principles, that allows to developers to compose them to create
 * bigger abstractions that are roughly on every place on moderm
 * computer software design.
 *
 * It is a "quite" human readable format, so you will find that is not
 * the best format if you are looking for espace efficiency. What XML
 * 1.0 provides you on the other hand is the hability to quickly
 * prototype and produce working formats that encapsulate your data,
 * and, as your system evolves, XML 1.0 do it with you.
 *
 * Among other things, XML 1.0 provides you ways to validate your
 * documents to ensure your code will ready XML document in the format
 * expected reducing the time and developent cost.
 *
 * Before continuing, we will explain some concepts that are required
 * to understand XML 1.0 and why the Axl API was built this way.
 *
 * \section concepts Some concepts
 *
 * Here is a simple example of a XML 1.0 document:
 * <pre class="xml-doc">
 * <span class="eblue">&lt;?xml version="1.0"></span>
 * <span class="red">&lt;!-- This is a comment --></span>
 * &lt;<span class="node">complex</span>>
 *   &lt;<span class="node">data</span>>
 *     &lt;<span class="node">simple</span>><span class="content">10</span>&lt;<span class="node">/simple</span>>
 *     &lt;<span class="node">empty</span> <span class="attr">attr1="value1"</span> />
 *   &lt;<span class="node">/data</span>>
 * &lt;<span class="node">/complex</span>>
 * </pre>
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
 * Several issues must be considered while interpreseting previous
 * diagram and how Axl library parse and expose those elements through
 * the API to the client application:
 * 
 * - Every XML document have a root node. Without exception. In this
 * case, the root node for our example is <b>complex</b>.
 * 
 * - If a node have content, that content is not represented with
 * another node. That content is associated to the node and could be
 * retrieved using several function (\ref axl_node_get_content, \ref
 * axl_node_get_content_copy and \ref axl_node_get_content_trans). 
 *
 * - Having a node with content doesn't mean to have a node with
 * childs. The child notion is only about having more xml nodes as
 * childs. 
 *
 * - A final node which is empty because it doesn't have content or
 * childs, is usually referred to as <b>EMPTY</b> type node. A final
 * node with content but no childs is usually referred to as
 * <b>PCDATA</b>. A node that have content mixed with references to
 * more child xml nodes is referred to as <b>MIXED</b>.
 *
 * - At the <b>empty</b> node, you'll find that it has an attribute
 * called <b>attr1</b> with a value <b>value1</b>. A node could have
 * any number of attributes but, it should be named
 * differently. Again, if a node is empty, it keeps empty even if it
 * has attributes.
 *
 * So, to sumarize, we have a root node, that could contain more
 * nodes, that could contain PCDATA, or content, and those nodes could
 * contain named attributes with values.
 *
 * \section parsing Parsing XML documents
 * 
 * We have seen how an XML document is. Now we are going to see how to
 * parse those document into data structures that are usable to
 * inspect the content. All parsing functions are available at the
 * \ref axl_doc_module "Axl Doc internface".
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
 *        return AXL_FALSE;
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
 *    return AXL_TRUE;
 * }
 * \endcode
 *
 * \section iterating Travel an XML document
 * 
 * Once the document is loaded you can use several function to
 * traverse the document. First, you have to get a reference to the
 * xml document root. You can use \ref axl_doc_get_root. Then, to get
 * some childs you can use \ref axl_node_get_child_nth.
 *
 * Here is an example to print all nodes inside a xml document: 
 * 
 * \code
 * void print_all_nodes (axlDoc * doc) 
 * {
 *    axlNode * root;
 *    
 *    // get a reference to the root node
 *    root = axl_doc_get_root (doc);
 * 
 *    // print all nodes 
 *    print_all_nodes_aux (root);
 *
 *    return;
 * }
 * 
 * void print_all_nodes (axlNode * node) {
 *
 *    axlNode * child;
 *    int       iterator;
 *
 *    // check and return if the node is NULL  
 *    axl_return_if_fail (node);
 * 
 *    print "Node found <%s>\n", axl_node_get_name (node));
 *
 *    // iterate over all childs inside the root node 
 *    iterator = 0;
 *    while (iterator < axl_node_get_child_num (node)) {
 *          // get the child at the given position 
 *          child = axl_node_get_child_nth (node, iterator);
 * 
 *          printf ("Node found <%s>\n", axl_node_get_name (child));
 * 
 *          // show all nodes inside this child node
 *          print_all_nodes (child);
 *
 *          // update the iterator 
 *          iterator++;
 *    }
 * }
 * \endcode
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
 * which allows to especify the document grammar, how are nested
 * nodes, which attributes could contain, or if the are allocated to
 * be empty nodes or nodes that must have another child nodes.
 *
 * <i><b>NOTE:</b> At this moment Axl only support <b><!ELEMENT</b> directive,
 * which only allows to check XML document structure. Future releases
 * will add <b><!ATTLIST</b> support which allows to configure
 * restrictions applied to node attributes.</i>
 *
 * Let start with the DTD syntax used to configure restrictions about
 * node structure:
 *
 * <pre class="xml-doc">
 * <span class="red">&lt;!-- sequence specification --></span>
 * <span class="eblue">&lt;!ELEMENT testA (test1, test2, test3)></span>
 *
 * <span class="red">&lt;!-- choice specification --></span>
 * <span class="eblue">&lt;!ELEMENT testB (test1 | test2 | test3)></span>
 * </pre>
 *
 * DTD <b><!ELEMENT</b> is modeled on top of two concepts which are
 * later expanded with repetition patterns. We will explain then
 * later. For now, this two top level concepts are: sequence and choice.
 *
 * Sequence especification (elements separated by <b>, (comma)</b>, the
 * one used to apply restriction to the node <b>testA</b>, are used to
 * denote that <b>testA</b> have as childs test1, followed by test2
 * and ended by test3. The order especified must be followed and all
 * instances must appear. This could be tweaked using repetition
 * pattern.
 *
 * In the other hand, choice especification (elements separated by
 * <b>| (pipe)</b>, are used to especify that the content of a node is
 * built using nodes of the choice list. So, in this case,
 * <b>testB</b> node could have either one instance of test1 or test2
 * or test3.
 *
 * Now you know these to basic elements to model how childs are
 * organized for a node, what it is need is to keep on adding more
 * <!ELEMENT directives until all nodes are especified. You will end
 * your DTD document with final nodes that are either empty ones or
 * have PCDATA. At this moment MIXED nodes are not supported.
 *
 * Suppose that all nodes that are inside testA and testB are final
 * ones. Then this could be its DTD especification:
 *
 * <pre class="xml-doc">
 * <span class="red">&lt;!-- test1 is a node that only have content --></span>
 * <span class="blue"><!ELEMENT test1 (\#PCDATA)></span>
 * <span class="red">&lt;!-- test2 is a node that is always empty --></span>
 * <span class="blue"><!ELEMENT test1 EMPTY></span>
 * <span class="red">&lt;!-- test3 is a node that could have either test1 or test2 --></span>
 * <span class="blue"><!ELEMENT test3 (test1 | test2)></span>
 * </pre>
 *
 * Sequences and choices could be composed to create richer DTD
 * expresions that combines sequences of choices and so on.
 * 
 * At this point all required elements to model choices, sequences and
 * final nodes are explained, but, we have to talk about repetition
 * pattern. They are simbols that are appended to elements inside
 * choices (or sequences) including those list especifications.
 *
 * Patterns available are: <b>+</b>, <b>?</b> and <b>*</b>. By
 * default, if no pattern is applied to the element, it means that the
 * match should be produced one and only one time.
 *
 * The <b>+</b> pattern is used to model that element should be
 * matched one, and at least one, or more.
 *
 * The <b>*</b> pattern is used to model elements that should be
 * matched zore or any times.
 *
 * The <b>?</b> pattern is used to model elements that should be
 * matched zero or one times.
 *
 * For the exampled initially explained, let's suppose we want that
 * the content inside <b>testA</b> have sequences repeated at leat one
 * time, being that sequence: test1, test2 and test3. We only need to
 * add a <b>+</b> repetition pattern as follows:
 *
 * <pre class="xml-doc">
 * <span class="red">&lt;!-- sequence specification --></span>
 * <span class="eblue">&lt;!ELEMENT testA (test1, test2, test3)+></span>
 * </pre>
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
 *		return AXL_FALSE;
 *
 *	// parse af-arch DTD 
 *	dtd = axl_dtd_parse_from_file ("channel.dtd", error);
 *	if (dtd == NULL)
 *		return AXL_FALSE;
 *
 *	// perform DTD validation 
 *	if (! axl_dtd_validate (doc, dtd, error)) {
 *		return AXL_FALSE;
 *	}
 *
 *	// free doc reference 
 *	axl_doc_free (doc); 
 *
 *      // free dtd reference
 *      axl_doc_free (dtd);
 * 
 *      return AXL_TRUE;
 * }
 * \endcode
 * 
 *
 * \section futher Futher reading
 * 
 * You can also check \ref axl_api "API documentation" for a complete
 * detailed explanation about the library.
 *
 */

/** 
 * \page axl_api LibAxl API documentation
 *
 * Here is the API for the modules defined inside the library:
 * 
 * <ul>
 *  <li><b>Basic API to interact with XML documents: </b></li>
 * 
 *    - \ref axl_module
 *    - \ref axl_doc_module
 *    - \ref axl_node_module
 *    - \ref axl_dtd_module
 *    - \ref axl_decl_module
 *  </li>
 *  <li><b>Error reporting and debuging functions: </b></li>
 * 
 *    - \ref axl_error_module
 *    - \ref axl_log_module
 *  <li><b>Auxiliar modules, supporting data types, string handling, etc:</b></li>
 *
 * - \ref axl_stream_module
 * - \ref axl_list_module
 * - \ref axl_stack_module
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
 * work on Microsoft Windows and GNU/Linux platforms. Here are a set
 * of instructions to get the library compiled for your platform:
 *
 * \section unix GNU/Linux (or any posix OS) installation instructions
 *
 * First, download the package from the donwload section. Check <a
 * href="http://www.aspl.es/axl/doc.html">this section to know more
 * about this.</a>
 *
 * Then use the standard procedure to get compiled an autotool ready
 * package. Here are the steps:
 *
 * \code
 *  bash: ~$ tar xzvf axl-0.2.1.b1984.g1985.tar.gz
 *  bash: ~$ cd axl-0.2.1.b1984.g1985/
 *  bash: ~/axl-0.2.1.b1984.g1985 $ ./configure
 *  bash: ~/axl-0.2.1.b1984.g1985 $ make
 *  bash: ~/axl-0.2.1.b1984.g1985 $ make install
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
 * Axl library comes with pkg-config support, which makes easy to use
 * it inside your pkg-config enable projects.
 *
 * To check current support for your Axl Library you can execute:
 * 
 * \code
 *  bash: ~$ pkg-config --libs --cflags axl
 * \endcode
 * 
 * To include support into your autotool checks (configure.ac/in
 * files) you can place the following piece:
 * \code
 * PKG_CHECK_MODULES(LIBRARIES, axl) 
 * \endcode
 * 
 *
 * \section Including Axl Library headers
 *
 * For all platforms, Axl Library headers has to be included as follows:
 * \code
 * #include <axl.h>
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
 * @return The function returns AXL_TRUE if it was properly
 * initialized or AXL_FALSE if something fails.  if fails.
 */
aboolean axl_init ()
{
	/* nothing to initialize dude */
	return AXL_TRUE;
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
