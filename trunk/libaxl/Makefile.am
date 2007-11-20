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

SUBDIRS = src  $(AXL_NS_DIR) $(AXL_KNIFE_DIR) $(AXL_DOC_DIR) $(AXL_TEST_DIR)
EXTRA_DIST = axl.pc.in VERSION libaxl.sln README.win32
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = axl.pc axl-ns.pc
