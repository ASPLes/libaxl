#!/usr/bin/python
# coding: utf-8
#  Copyright (C) 2014 Advanced Software Production Line, S.L.
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License
#  as published by the Free Software Foundation; either version 2.1 of
#  the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of 
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this program; if not, write to the Free
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
#  02111-1307 USA
#  
#  You may find a copy of the license under this software is released
#  at COPYING file. This is LGPL software: you are wellcome to
#  develop propietary applications using this library withtout any
#  royalty or fee but returning back any change, improvement or
#  addition in the form of source code, project image, documentation
#  patches, etc. 
#

def replace_on_file (file_path, maps):

    # get content
    content = open (file_path).read ()

    # write content
    open ("%s.prefix.bak" % file_path, "w").write (content)
    
    for (o, r) in maps:
        content = content.replace (o, r)

    # save back content
    open (file_path, "w").write (content)
    print "INFO: updating %s.." % file_path
    return

print "change-prefix.py : a tool to change library prefix by updating Makefiles before compiling."
print "Copyright (C) 2014 Advanced Software Production Line, S.L."

prefix = raw_input("Please, provide library prefix to configure (i.e.: 64, 32, lts): ")
if prefix:
    prefix = prefix.strip ()
    
if not prefix:
    print "ERROR: no prefix was provided"
    sys.exit (-1)

print "INFO: this is change library compilation from libvortex-1.1.so.0 -> libvortex%s-1.1.so.0" % prefix
answer = raw_input( "Is this correct? (yes/NO): " )
if answer:
    answer = answer.strip ()
if answer == "no":
    print "ERROR: doing nothing.."
    sys.exit (-1)
if answer != "yes":
    print "ERROR: please, answer yes or no.."
    sys.exit (-1)

# create substitution maps
maps = []

# base
maps.append (("libaxl.def", "libaxl%s.def" % prefix))
maps.append (("libaxl.exp", "libaxl%s.exp" % prefix))
maps.append (("libaxl.la", "libaxl%s.la" % prefix))
maps.append (("libaxl.def", "libaxl%s.def" % prefix))
maps.append (("libaxl_", "libaxl%s_" % prefix))

# ns
maps.append (("libaxl-ns.def", "libaxl-ns%s.def" % prefix))
maps.append (("libaxl-ns.exp", "libaxl-ns%s.exp" % prefix))
maps.append (("libaxl-ns.la", "libaxl-ns%s.la" % prefix))
maps.append (("libaxl-ns.def", "libaxl-ns%s.def" % prefix))
maps.append (("libaxl_ns", "libaxl_ns%s_" % prefix))

# babel
maps.append (("libaxl-babel.def", "libaxl-babel%s.def" % prefix))
maps.append (("libaxl-babel.exp", "libaxl-babel%s.exp" % prefix))
maps.append (("libaxl-babel.la", "libaxl-babel%s.la" % prefix))
maps.append (("libaxl-babel.def", "libaxl-babel%s.def" % prefix))
maps.append (("libaxl_babel", "libaxl_babel%s_" % prefix))

replace_on_file ("src/Makefile", maps)
replace_on_file ("ns/Makefile", maps)
replace_on_file ("babel/Makefile", maps)
replace_on_file ("knife/Makefile", maps)
replace_on_file ("py-axl/Makefile", maps)
replace_on_file ("test/Makefile", maps)







