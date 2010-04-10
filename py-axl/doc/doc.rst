:mod:`axl.Doc` --- PyAxlDoc class: XML document instance 
========================================================

.. currentmodule:: axl


=====
Intro
=====

API documentation for axl.Doc object representing a XML document
instance. 

There are several ways to create a document. The most easy to load it
from a file. To do so you can use::

     # include axl module
     import axl

     # load document
     (doc, err) = axl.file_parse ("file.xml")
     if err:
     	print ("ERROR: found xml error: " + err.msg)
	return -1

     # dump document into console
     (content, size) = doc.dump ()
     print ("Content loaded: " + content)			

You can also create a document from a few strings::

     (doc, err) = axl.file_parse ("file.xml")

Finally, to create an empty document, you simply instance the class::

     # create a document with default version=None, default
     # encoding=None and default standalone=axl_true configuration
     doc = axl.Doc ()

     # set root node
     doc.root = axl.Node ("test")


==========
Module API
==========

.. class:: Doc

   .. method:: get (path)
   
      Allows to get a node at a particular location using a simplified
      Xpath string. See axl_doc_get for more information::

      	    # get node an prints its content
            node = doc.get ("/document/child/first")

	    if node:
	       print ("Node name: " + node.name + ", with content: " + node.content)

      :param path: A path to the node that is retrieved
      :type  path: String 

      :rtype: Returns an instance of axl.Node or None if no node was found.

   .. method:: dump ([tabular])
   
      Allows to dump the document loaded by the instance into an
      string. The method receives an optional integer value that
      activates the pretty print mode and providing the tabular value
      to use on each level.

      For example, dumping the document without indentantion is done using::

          (content, size) = doc.dump ()

      However, dumping the same content indented in a more readable way use::

          (content, size) = doc.dump (4)

      See also axl_doc_dump and axl_doc_dump_pretty

      :param tabular: Optional integer value that activates the dump indentation support providing the tabular value to use on each level.
      :type  tabular: Integer >= 0 

      :rtype: Returns a tumple (content, size) or an exception (TypeError or ValueError) in the case some error is found.

   .. method:: file_dump (path, [tabular])
   
      Allows to perform a dump operation sending the content to a file
      located at the path provided. If provided optional argument
      tabular will cause indentation to be activated.

      For example, to dump content into a file with a 4 space indentation on each level use::

          doc.file_dump ("/tmp/output.txt", 4):

      :param path: Path where the xml content will be dumped.
      :type  path: String 
	  
      :param tabular: Optional integer value that activates the dump indentation support providing the tabular value to use on each level.
      :type  tabular: Integer >= 0 

      :rtype: Returns True or an exception (TypeError or ValueError) in the case some error is found.

   .. method:: has_pi (pi_name)
   
      Allows to check if the document contains a process instruction with the provided name.

      :param pi_name: Process instruction name to check.
      :type  pi_name: String

      :rtype: Returns True if the process instruction is available, otherwise False is  returned.

   .. method:: pi (pi_name)
   
      Allows to get the Process instruction content with the provide pi_name.

      :param pi_name: Process instruction name to check.
      :type  pi_name: String

      :rtype: Returns the process instruction content or None if nothing is found.

   .. attribute:: encoding

      (Read only attribute) (String) Allows to get document encoding.

   .. attribute:: standalone

      (Read only attribute) (True/False) Allows to get stand alone document configuration.

   .. attribute:: stand_alone

      Alias of standalone

   .. attribute:: root

      (Read/Write attribute) (axl.Node) allows to get/set the current document root node.

