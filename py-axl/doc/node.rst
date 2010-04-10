:mod:`axl.Node` --- PyAxlNode class: XML node instance 
======================================================

.. currentmodule:: axl


=====
Intro
=====

:class:`axl.Node` class represents a single node instance that is hold
by a document (:class:`axl.Doc`).

The usual operation is to load a document and the acquire a reference to the node inside as follows::

   # load the document
   (doc, err) = axl.parse ("<root-node><child /></root-node>")
   if err:
      print ("Failed to parse content: " + err.msg)
      return -1

   # get the <child> node
   node = doc.get ("/root-node/child")


However, you can also create new nodes using the type constructor of :class:`axl.Node` as follows::

  # set a new root node
  doc.root = axl.Node ("test")
   

==========
Module API
==========

.. class:: Node

   .. method:: next_called (node_name)
   
      Allows to get the next node on the same level or the instance
      node, but with the provide name.

      :param node_name: node name to match.
      :type  node_name: String 

      :rtype: Returns an instance of :class:`axl.Node` or None if no node was found.

   .. method:: previous_called (node_name)
   
      Allows to get the previous node on the same level or the instance
      node, but with the provide name.

      :param node_name: node name to match.
      :type  node_name: String 

      :rtype: Returns an instance of :class:`axl.Node` or None if no node was found.

   .. method:: child_called (node_name)
   
      Allows to get the first node child, but with the provide name.

      :param node_name: node name to match.
      :type  node_name: String 

      :rtype: Returns an instance of :class:`axl.Node` or None if no node was found.

   .. method:: find_called (node_name)
   
      Gets the xml child node called as provided. The child is looked
      up in all childs found starting the parent node.

      :param node_name: node name to match.
      :type  node_name: String 

      :rtype: Returns an instance of :class:`axl.Node` or None if no node was found.

   .. method:: nth_called (position)
   
      Allows to get a reference to the child node located at the same
      level at the nth position.

      :param position: node position.
      :type  position: Integer 

      :rtype: Returns an instance of :class:`axl.Node` or None if no node was found.

   .. method:: has_attr (attr_name, [attr_value])
   
      Allows to check if the provided node has the given attribute.

      :param attr_name: the attribute name to check.
      :type  attr_name: String

      :param attr_value: Optional the attribute value to check. If provided the method check if the node has an attribute with the provided value.
      :type  attr_value: String

      :rtype: Returns the presence of the provided attribute on the given node, and optionall checking if that attribute has a particular value.

   .. method:: attr (attr_name, [value])
   
      Allows to get the value associated to the attribute (attr_name)
      or to configure the provided value if two arguments are
      provided.

      :param attr_name: the attribute name to get its value.
      :type  attr_name: String

      :param value: If provided, allows to configure the value on the attribute (attr_name).
      :type  value: String

      :rtype: Returns an instance of :class:`axl.Node` or None if no node was found.

   .. method:: set_child (node)
   
      Allows to configure a new child node.

      :param node: the new child node to configure.
      :type  node: :class:`axl.Node`

   .. method:: attr_cursor_new ()
   
      Creates a new :class:`axl.AttrCursor` instance which is used to
      perform efficient node attribute iteration. See axl_node_attr_cursor_new.

      :param node: the new child node to configure.
      :type  node: axl.Node

   .. method:: remove ([dealloc=True])
   
      Allows to remove the node from its current document. By default
      the internal node is deallocated. 

      :param node: the new node to be removed from current holding document
      :type  node: :class:`axl.Node`

   .. method:: set_empty ()
   
      Allows to clear all content set inside a particular node.

   .. attribute:: name

      (Read only attribute) (String) Allows to get the node name.

   .. attribute:: first_child

      (Read only attribute) (:class:`axl.Node`) Allows to get first node child.

   .. attribute:: next

      (Read only attribute) (:class:`axl.Node`) Allows to get next node.

   .. attribute:: previous

      (Read only attribute) (:class:`axl.Node`) Allows to get previous node.

   .. attribute:: parent

      (Read only attribute) (:class:`axl.Node`) Allows to get parent node.

   .. attribute:: content

      (Read/Write attribute) (:class:`axl.Node`) Allows to get/set the
      node content. See axl_node_get_content and axl_node_set_content
      from C API. The attribute detects on write operations if the
      content to be configured is not supported directly without replacing &, <, ', >, ", and ;. In such case, Axl API translate the content (replacing those values by &amp;, &lt;, etc...). 

      The attribute returns a tuple with (content, size).

   .. attribute:: trans

      (Read only attribute) (:class:`axl.Node`) Allows to get content stored on a node translating known entity references: &amp;, &lt;, etc..

      The attribute returns a tuple with (content, size).

   .. attribute:: doc

      (Read only attribute) (:class:`axl.Doc`) Allows to get the document that holds the node.

