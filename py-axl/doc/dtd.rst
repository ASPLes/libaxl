:mod:`axl` --- PyAxlDtd class: DTD support
===========================================

.. currentmodule:: axl


=====
Intro
=====

API documentation for axl.Dtd object representing a DTD document
instance.

==========
Module API
==========

.. class:: Dtd

   .. method:: validate (doc)
   
      Allows to validate the document with the DTD instance.

      :param doc: the document to validate
      :type  doc: :class:`axl.Doc`

      :rtype: Returns (True/False) according to validation status.

