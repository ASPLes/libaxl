PyAxl Developer Manual
======================

PyAxl is built on top of Axl Library (http://www.aspl.es/xml) and
provides an interface to python language to use Axl Library
components. Currently only main component is available (it is still
missing Axl Namespace and Babel support) but they will be added in
future releases.

The document contains a reference collecting how usual XML operations
are implemented with PyAxl.

================
Installing PyAxl
================

PyAxl is included as an optional component inside Axl Library source
distribution. It is built by default in case python development
headers are found installed on the system.

Here are the set of commands that will be required to get PyAxl
install on your debian system::

  >> tar xzvf axl-VERSION.tar.gz
  >> cd axl-VERSION
  >> ./configure
  >> make
  >> make install

After "./configure" steps finishes, a build status will be showed
indicating if PyAxl will be built:

  Build PyAxl:       yes

===========
Using PyAxl
===========

You must first import axl module::

  # import PyAxl services
  import axl

======================
Opening a XML document
======================

To load an existing xml document into memory, you can use the following::

   (doc, err) = axl.file_parse ("/tmp/file.xml")
  
Previous code will return a tuple
(:class:`axl.Doc`, :class:`axl.Error`) having the document or a defined
error in case something failed. 

In general several operations do error report by using
:class:`axl.Error` objects, which contains a reference to the error
code (:attr:`axl.Error.code`) and a textual error (:attr:`axl.Error.msg`).  

To properly check if an error was found you can use the following:: 

   # load the document
   (doc, err) = axl.file_parse ("/tmp/file.xml")

   # check for errors
   if err: 
       print ("Error found: " + err.msg)
       return False

============================
Getting document's root node
============================

Having a defined (:class:`axl.Doc`) reference you can check top root
node (:class:`axl.Node`) by using .root attribute. This attribute can
be used to also configure new root node value::

   print ("Root node name: " + doc.root.name)

In the case an empty document was created, you can set the root node
as follows::

   # create an xml document with <document /> as content
   doc.root = axl.Node ("document")

======================================================
Managing node content, child nodes and node attributes
======================================================

Having a defined (:class:`axl.Node`), to get or set content associated
to that node you can use :attr:`axl.Node.content` attribute::

  print ("Node content: " + node.content)

To configure a couple of child nodes, use :meth:`axl.Node.set_child` method:: 

  # set <child1 />
  doc.root.set_child (axl.Node ("child1"))

  # set <child2 />
  doc.root.set_child (axl.Node ("child2"))

In the case it is required to check, configure or get node attribute
value, you can use::

  # check if a node has attribute 'test'
  print ("Node has attribute 'test': " + str (node.has_attr ('test')))

  # print node attribute 'test'
  print ("Node attribute 'test' value: " + node.attr ('test'))

  # to set attribute value we use
  node.attr ('test', 'value to configure')












