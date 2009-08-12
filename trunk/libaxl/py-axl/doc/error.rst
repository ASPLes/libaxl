:mod:`axl` --- PyAxlError class: error handling and reporting
===============================================================

.. currentmodule:: axl


=====
Intro
=====

:class:`axl.Error` class is used to report errors found while using
PyAxl API. The class contains two attributes that contains the integer
error code and a textual diagnostic with a human readable text.

==========
Module API
==========

.. class:: Error

   .. attribute:: code

      (Read only attribute) (Integer) Error code.

   .. attribute:: msg

      (Read only attribute) (String) Textual error.

