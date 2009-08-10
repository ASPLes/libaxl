#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

# import base library 
import axl

def test_01():
    # do two consecutive parse operations
    (doc, err) = axl.parse ("<?xml version='1.0' ?><axldoc />", 32)
    if err:
        error ("Found error: " + str (err.code) + ", message: " + err.msg)
        return False

    (doc, err) = axl.parse ("<?xml  version=\"1.0\" \t \n \r encoding='utf-8\"   ?> <data />", 63)
    if err:
        error ("Found error: " + str (err.code) + ", message: " + err.msg)
        return False

    # check doc encoding 
    if doc.encoding != "utf-8":
        error ("Error, expected to find enconding value utf-8 but found: " + doc.encoding)
        return False

    # do a fail parse operation
    (doc, err) = axl.parse ("aefadsadsf<?xml  version=\"1.0\" \t \n \r encoding='utf-8\"   ?> <data />", 73)
    if not err:
        error ("Expected to find error but found no error report")
        return False
    
    # check error returned
    if not err.code == -2:
        error ("Expected to find error code -2 but found: " + str (err.code))

    # check standalone configuration
    (doc, err) = axl.parse ("<?xml version='1.0' encoding='utf-8' standalone='yes' ?>  <data/>", 65)
    if err:
        error ("Expected to find proper document parse but a failure was found: " + err.msg)
        return False

    if not doc.standalone:
        error ("Expected to find standalone enabled but found it disabled")
        return False
        
    return True

def test_01b():
    # parse document
    (doc, err) = axl.file_parse ("test_01b.xml")
    if err:
        error ("Found error: " + str (err.code) + ", message: " + err.msg)
        return False

    # get root node
    node = doc.root

    # check node name 
    if node.name != "document":
        error ("Expected to find node name 'document' but found: " + node.name)
        return False

    # get first child
    node = node.first_child
    
    # check node name 
    if node.name != "child1":
        error ("Expected to find node name 'child1' but found: " + node.name)
        return False

    # get next
    node = node.next

    # check node name 
    if node.name != "child2":
        error ("Expected to find node name 'child2' but found: " + node.name)
        return False

    return True

def test_01c():
    # parse document
    (doc, err) = axl.file_parse ("test_01b.xml")
    if err:
        error ("Found error: " + str (err.code) + ", message: " + err.msg)
        return False

    # get root node
    node = doc.root

    # check node name 
    if node.name != "document":
        error ("Expected to find node name 'document' but found: " + node.name)
        return False

    # get first child
    node = node.first_child
    
    # check node name 
    if node.name != "child1":
        error ("Expected to find node name 'child1' but found: " + node.name)
        return False

    # get parent
    parent = node.parent

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False

    node = node.next

    # check node name 
    if node.name != "child2":
        error ("Expected to find node name 'child2' but found: " + node.name)
        return False


    # check parent again
    parent = node.parent

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False

    node = node.next

    # check node name 
    if node.name != "child3":
        error ("Expected to find node name 'child3' but found: " + node.name)
        return False

    # check parent again
    parent = node.parent

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False

    node = node.next

    # check node name 
    if node.name != "child4":
        error ("Expected to find node name 'child4' but found: " + node.name)
        return False

    # check parent again
    parent = node.parent

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False

    node = node.next

    # check node name 
    if node.name != "child5":
        error ("Expected to find node name 'child5' but found: " + node.name)
        return False

    # check next is none
    if node.next:
        error ("Expected to find none value for the next child node found next to child5")
        return False

    node = node.previous

    # check node name 
    if node.name != "child4":
        error ("Expected to find node name 'child4' but found: " + node.name)
        return False

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False

    node = node.previous

    # check node name 
    if node.name != "child3":
        error ("Expected to find node name 'child3' but found: " + node.name)
        return False

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False

    node = node.previous

    # check node name 
    if node.name != "child2":
        error ("Expected to find node name 'child2' but found: " + node.name)
        return False

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False

    node = node.previous

    # check node name 
    if node.name != "child1":
        error ("Expected to find node name 'child1' but found: " + node.name)
        return False

    # check node name 
    if parent.name != "document":
        error ("Expected to find node name 'document' but found: " + parent.name)
        return False
    
    # check previous is none
    if node.previous:
        error ("Expected to find none value for the previous child node found previous to child1")
        return False

    # now test called api
    node = doc.root.first_child

    # get <child5>
    node = node.next_called ("child5")
    if node.name != "child5":
        error ("Expected to find child node with name child5 but found: " + node.name)
        return False

    # get <child5>
    node = node.next_called ("child5")
    if node:
        error ("Expected to find none node after calling next_called with child5 but found: " + node.name)
        return False
    
    node = doc.root.first_child.next_called ("child5")
    if not node:
        error ("Expected to find a xml node but found none")
        return False
    if node.name != "child5":
        error ("Expected to find child node child5 but found: " + node.name)
        return False

    # get <child1>
    node = node.previous_called ("child1")
    if not node:
        error ("Expected to find a xml node child1 but found none")
        return False
    if node.name != "child1":
        error ("Expected to find child node child1 but found: " + node.name)
        return False

    node = node.previous_called ("child1")
    if node:
        error ("Expected to not find a xml node but found somethind defined: " + node.name)
        return False
    
    return True

def test_01d():
    (doc, err) = axl.file_parse ("test_01d.xml")
    if err:
        error ("Expected to find proper parse operation but found an error: " + err.msg)
        return False
    
    child = doc.root.nth_child (0)
    if child.name != "child1":
        error ("Expected to find child1 node but found: " + child.name)
        return False

    child = doc.root.nth_child (1)
    if child.name != "child2":
        error ("Expected to find child2 node but found: " + child.name)
        return False

    child = doc.root.nth_child (2)
    if child.name != "child3":
        error ("(2) Expected to find child3 node but found: " + child.name)
        return False

    child = doc.root.nth_child (3)
    if child.name != "child4":
        error ("Expected to find child4 node but found: " + child.name)
        return False

    child = doc.root.nth_child (4)
    if child.name != "child5":
        error ("Expected to find child5 node but found: " + child.name)
        return False
    
    # get the child 0 located at the child 2
    child  = doc.root.nth_child (2).nth_child (0)
    
    if child.name != "a":
        error ("Expected to find node name <a> but found: " + child.name)
        return False

    child  = doc.root.nth_child (2).nth_child (1)
    
    if child.name != "b":
        error ("Expected to find node name <b> but found: " + child.name)
        return False

    child  = doc.root.nth_child (2).nth_child (2)
    
    if child.name != "c":
        error ("Expected to find node name <c> but found: " + child.name)
        return False

    child  = doc.root.nth_child (2).nth_child (3)
    
    if child.name != "f":
        error ("Expected to find node name <f> but found: " + child.name)
        return False


    child  = doc.root.nth_child (2).nth_child (4)
    
    if child.name != "g":
        error ("Expected to find node name <g> but found: " + child.name)
        return False

    return True

def test_01f():

    # parse document
    (doc, err) = axl.file_parse ("test_01f.xml")
    if err:
        error ("Expected proper document parse but found a failure: " + err.msg)
        return False
    
    # get root content
    (content, size) = doc.root.content

    # check content
    if content != "     ":
        error ("Expected to find '     ' but found: " + content)
        return False

    # parse another document 
    (doc, err) = axl.file_parse ("test_01f2.xml")
    if err:
        error ("Expected proper document parse but found a failure: " + err.msg)
        return False

    # get content from node id found as child of the root node
    (content, size) = doc.root.child_called ("id").content

    # check content
    if content != "   ":
        error ("(2) Expected to find '   ' but found: " + content)
        return False

    (content, size) = doc.root.child_called ("id2").content

    # check content
    if content != "":
        error ("Expected to find '' but found: " + content)
        return False

    return True

def test_03():
    (doc, err) = axl.parse ("<?xml version='1.0' ?>\n\
<complex>\n\
  <data>\n\
    <row>10</row><row>20</row><row>30</row><row>40</row>\n\
  </data>\n\
  <data2>\n\
    <td> 23  </td>\n\
  </data2>\n\
</complex>", -1)
    
    if err:
        error ("Expected to find proper document load, but a failure was found: " + err.msg)
        return False

    if doc.root.name != "complex":
        error ("Expected to find complex node name but found: " + doc.root.name)
        return False
    
    node = doc.get ("/complex/data2/td")
    if not node:
        error ("Expected to find node found at /complex/data2/td")
        return False

    # check node name 
    if node.name != "td":
        error ("Expected to find node name td but found: " + node.name)
        return False

    node = doc.get ("/complex/data3/td")
    if node:
        error ("Expected to not find node in wrong path but found: " + node.name)
        return False

    (content, size) = doc.get ("/complex/data2/td").content
    if content != " 23  ":
        error ("Expected to find content ' 23  ' but found: " + content)
        return False

    node = doc.get ("complex/data3/td")
    if node:
        error ("Expected to find no node but found: " + node.name)
        return False

    return True

def test_04():
    (doc, err) = axl.parse ("<?xml version='1.0' ?>" +
                     "  <?test \"my content\" ?>" + 
                     "  <complex>" + 
                     "    <data>" + 
                     "       <row>" + 
                     "         <!-- A comment inside the middle of the document " + 
                     " more comments ... " + 
                     " more comments \n \r <td>..</td> -->" + 
                     "          <td>10</td>" + 
                     "          <test />" + 
                     " \n \n \r     <?test \"my content\" ?>     \n    \n" + 
                     "  <?test \"my content\" ?>" + 
                     "          <more>" + 
                     "              <test3 attr='2.0' />" + 
                     "          </more>" + 
                     "       </row>" + 
                     "       <test2 />" + 
                     "       <non-xml-document>" + 
                     "                             \n \r \n \t" + 
                     "         <![CDATA[<xml><<<<<<>>>>>><<<>>>><<<<<<>>>]]>"
                     "          \r \n \r \t" + 
                     "       </non-xml-document>" + 
                     "    </data>" + 
                     " <!--   <data>" + 
                     "       <row>" + 
                     "         A comment inside the middle of the document " + 
                     " more comments ... " + 
                     " more comments \n \r <td>..</td> " + 
                     "          <td>10</td>" + 
                     "          <test />" + 
                     "          <more>" + 
                     "              <test2 attr='2.0' attr2='3.0' attr4='4.0'/>" + 
                     "          </more>" + 
                     "       </row>" + 
                     "       <test2 />" + 
                     "    </data> -->" + 
                     "  </complex>", -1)
    
    if err:
        error ("Expected to find proper parse result but found a failure: " + err.msg)
        return False
    
    if doc.get ("/complex/data/row/td").content[0] != "10":
        error ("Expected to find content 10 but found: " + doc.get ("/complex/data/row/td").content[0])
        return False

    node = doc.get ("/complex/data/row/more/test3")

    if not node.has_attr ("attr"):
        error ("Expected to find attribute attr but not found")
        return False

    # now check attr content
    if node.attr ("attr") != "2.0":
        error ("Expected to find attribute value 2.0 but found: " + node.attr ("attr"))
        return False
    
    # check pi instructions support
    if not doc.has_pi ("test"):
        error ("Expected to find pi instruction test but not found")
        return False

    if doc.pi ("test") != "\"my content\"":
        error ("Expected to find 'my content' but found: " + doc.pi ("test"))
        return False

    # check node content
    if doc.get ("/complex/data/non-xml-document").content[0] != "<xml><<<<<<>>>>>><<<>>>><<<<<<>>>":
        error ("Expected to find <xml><<<<<<>>>>>><<<>>>><<<<<<>>> but found: " + doc.get ("/complex/data/non-xml-document").content[0])
        return False

    return True

def test_05():
    
    (doc, err) = axl.file_parse ("test.xml")
    
    if doc.get ("/complex/data/row/td").content[0] != "10":
        error ("Expected to find 10 but found: " + doc.get ("/complex/data/row/td").content[0])
        return False

    (doc, err) = axl.file_parse ("test2.xml")
    if err:
        error ("Expected to find proper document parsing, but error was found: " + err.msg)
        return False

    (dtd, err) = axl.dtd_file_parse ("test.dtd")
    if err:
        error ("Expected to find proper dtd parsing, but error was found: " + err.msg)
        return False

    # validate content
    err = dtd.validate (doc)
    if err:
        error ("Expected to find proper DTD validation, but a failure was found: " + err.msg)
        return False

    return True

def test_22():

    # create a document
    doc = axl.Doc ()

    # set root node
    doc.root = axl.Node ("root-node")

    # check if the node has an attribute not available
    if doc.root.has_attr ("attribute-not-found"):
        error ("Expected to not find attribute: attribute-not-found, but found it")
        return False

    # check none value
    if doc.root.attr ("attribute-not-found"):
        error ("Expected to find None value associated to missing attribute, but found something defined")
        return False

    # configure attribute
    doc.root.attr ("attribute1", "value1")

    # check if the node has an attribute available
    if not doc.root.has_attr ("attribute1"):
        error ("Expected to find attribute: attribute1, but it wasn't found")
        return False

    if not doc.root.attr ("attribute1") == "value1":
        error ("Expected to find value1 as value associated to attribute1 but found: " + doc.root.attr ("attribute1"))
        return False

    # set more attributes
    doc.root.attr ("attribute2", "value2")
    doc.root.attr ("attribute3", "value3")
    doc.root.attr ("attribute4", "value4")
    doc.root.attr ("attribute5", "value5")
    doc.root.attr ("attribute6", "value6")

    # check attr iterator
    cursor = doc.root.attr_cursor_new ()
    while cursor.has_item ():
        if cursor.key == "attribute2" and cursor.value != "value2":
            error ("Expected to find value2 for attribute2 but found: " + cursor.value)
            return False
        if cursor.key == "attribute3" and cursor.value != "value3":
            error ("Expected to find value3 for attribute3 but found: " + cursor.value)
            return False
        if cursor.key == "attribute4" and cursor.value != "value4":
            error ("Expected to find value4 for attribute4 but found: " + cursor.value)
            return False
        if cursor.key == "attribute5" and cursor.value != "value5":
            error ("Expected to find value5 for attribute5 but found: " + cursor.value)
            return False
        if cursor.key == "attribute6" and cursor.value != "value6":
            error ("Expected to find value6 for attribute6 but found: " + cursor.value)
            return False

        # next cursor
        cursor.next ()

    return True

def test_33():

    # creates a document with default version, default encoding and standalone = true
    doc  = axl.Doc ()

    # create a node
    node = axl.Node ("test")

    # set as root
    doc.root = node

    iterator = 0
    while iterator < 2:
        # get a temp reference to the current node
        temp = doc.root

        # create a new root
        node = axl.Node ("test")

        # set new root
        doc.root = node

        # set new child
        node.set_child (temp)

        # next position
        iterator += 1

    # now create empty nodes
    node = axl.Node ("test")

    return True

def py_test_01():

    # parse content
    (doc, err) = axl.parse ("<content />")
    if err:
        error ("Expected to find proper parse operation but found an error: " + err.msg)
        return False

    # get the node
    node = doc.root

    # get document containing node
    doc2 = node.doc

    if doc2.root.name != "content":
        error ("Expected to find node name: content but found: " + doc2.root.name)
        return False

    return True

###########################
# intraestructure support #
###########################

def info (msg):
    print "[ INFO  ] : " + msg

def error (msg):
    print "[ ERROR ] : " + msg

def ok (msg):
    print "[  OK   ] : " + msg

def run_all_tests ():
    test_count = 0
    for test in tests:
        
         # print log
        info ("TEST-" + str(test_count) + ": Running " + test[1])
        
        # call test
        if not test[0]():
            error ("detected test failure at: " + test[1])
            return False

        # next test
        test_count += 1
    
    ok ("All tests ok!")
    return True

# declare list of tests available
tests = [
    # note test functions starting with test_ are using the same reg test
    # as defined by test_01.c (axl reg test). Tests that are specific to
    # py-axl must start with py_test_.
    (test_01,    "Check PyVortex context initialization"),
    (test_01b,   "Check Basic XML parsing, XML document position"),
    (test_01c,   "Check Basic XML parsing, XML document traversing"),
    (test_01d,   "Check Basic XML parsing, node nth access"),
    (test_01f,   "Check Basic XML parsing, white space node content"),
    (test_03,    "Check complex xml error detection"),
    (test_04,    "Check complex xml parsing"),
    (test_05,    "Check DTD basic parsing"),
    (test_22,    "Check Axl node attributes"),
    (test_33,    "Check Recursive root node replace"),
    (py_test_01, "Check PyNode type attributes"),
]

info (" LibAxl: Another XML library (regression test).")
info (" Copyright (C) 2008 Advanced Software Production Line, S.L.")
info (" Axl regression tests: version=" + axl.VERSION)
info (" To gather information about time performance you can use:")
info ("")
info ("     >> time ./test_01.py")
info ("")
info (" To gather information about memory consumed (and leaks) use:")
info ("")
info ("     >> valgrind --leak-check=yes --show-reachable=yes --suppressions=suppressions.valgrind --gen-suppressions=yes ./test_01.py ")
info ("")
info (" Report bugs to:")
info ("")
info ("     <axl@lists.aspl.es> Axl mailing list")
info ("")

if __name__ == '__main__':
    iterator = 0
    for arg in sys.argv:
        # according to the argument position, take the value 
        if iterator == 1:
            host = arg
        elif iterator == 2:
            port = arg
            
        # next iterator
        iterator += 1

    # call to run all tests
    run_all_tests ()
