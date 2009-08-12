:mod:`axl` --- PyAxl base module
================================

.. currentmodule:: axl

=====
Intro
=====

:mod:`axl` includes static functions that are used to bootstrap some
objects (like :class:`axl.Doc`) with initialized content.

==========
Module API
==========

.. function:: parse (document, [size])
   
   Allows to parse a XML document described by document parameter and
   its corresponding size::

      # parse document
      (doc, err) = axl.parse ("<document><child><next /></child></document>")
      if err:
	  print ("Failed to parse document, error found: " + err.msg)

  :param document: A xml document content to be parsed.
  :type  document: String 

  :rtype: Returns a tuple with (:class:`axl.Doc`, :class:`axl.Error`) having the document defined in the case of proper parsing, or having defined axl.Error in the case an error is found.

.. function:: file_parse (path)
   
   Allows to parse a XML document found at the provided path::

      # parse document
      (doc, err) = axl.file_parse ("/tmp/salida.txt")
      if err:
	  print ("Failed to parse document, error found: " + err.msg)

  :param path: Path to the document to be loaded.
  :type  path: String 

  :rtype: Returns a tuple with (:class:`axl.Doc`, :class:`axl.Error`) having the document defined in the case of proper parsing, or having defined :class:`axl.Error` in the case an error is found.

.. function:: dtd_file_parse (path)
   
   Allows to parse DTD definition found on the provided path::

      # parse dtd content
      (dtd, err) = axl.parse ("<document><child><next /></child></document>")
      if err:
	  print ("Failed to parse dtd, error found: " + err.msg)

  :param path: Path to the DTD file definition.
  :type  path: String 

  :rtype: Returns a tuple with (:class:`axl.Dtd`, :class:`axl.Error`) having the DTD defined in the case of proper parsing, or having defined :class:`axl.Error` in the case an error is found.



