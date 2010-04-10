:mod:`axl.AttrCursor` --- PyAxlAttrCursor class: xml node attributes iterator
==================================================================

.. currentmodule:: axl


=====
Intro
=====

:class:`axl.AttrCursor` class represents an iterator cursor that
maintains state of the current element selected. It is created using a
node (:class:`axl.Node`) and later calls to
:meth:`axl.AttrCursor.next` and :meth:`axl.AttrCursor.has_item`
are used to check all attributes stored on a node::

   # get node 
   node   = doc.get ("/document/child")

   # create the cursor
   cursor = node.cursor_new ()
   while node.has_item ():
   	 # print content
   	 print ("found attribute: " + node.key + "=" + node.value)
	 
	 # next item
	 cursor.next ()
   

==========
Module API
==========

.. class:: AttrCursor

   .. method:: first ()
   
      Set the cursor to point to the first attribute.

   .. method:: nexut ()
   
      Set the cursor to point to the next attribute.

   .. method:: has_item ()
   
      Check if the cursor is pointing to an item (or previous call to
      :meth:`axl.AttrCursor.next` has succeeded.

      :rtype: Returns True in the case the cursor points to an item. Otherwise, False is returned. The method returns False  once the cursor have iterated over all attributes.

   .. method:: has_next ()
   
      Check if the cursor points to an item that has a next attribute.

      :rtype: Returns True in the case there are a next item. Otherwise, False is returned.

   .. attribute:: key

      (Read only attribute) (String) Allows to get the current attribute name pointed by the cursor.

   .. attribute:: value

      (Read only attribute) (String) Allows to get the value associated to the attribute pointed by the cursor.

