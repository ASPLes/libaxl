if ENABLE_AF_ARCH_DOC
AXL_DOC_DIR = doc
endif

if  ENABLE_AXL_TESTS
AXL_TEST_DIR = test
endif

if  ENABLE_AXL_NS
AXL_NS_DIR = ns
endif

if ENABLE_AXL_KNIFE
AXL_KNIFE_DIR = knife
endif

if ENABLE_AXL_BABEL
AXL_BABEL_DIR = babel
endif

if ENABLE_PY_AXL
PY_AXL_DIR = py-axl
endif

SUBDIRS = src  $(AXL_NS_DIR) $(AXL_KNIFE_DIR) $(AXL_BABEL_DIR) $(PY_AXL_DIR) $(AXL_DOC_DIR) $(AXL_TEST_DIR) 
EXTRA_DIST = axl.pc.in VERSION libaxl.sln README.win32 get-python-config Makefile.win Makefile.win.in
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = axl.pc axl-ns.pc axl-babel.pc

all-local: update-defs

update-defs:
	cd src; make update-def; cd ..
	cd ns; make update-def; cd ..
	cd babel; make update-def; cd ..
