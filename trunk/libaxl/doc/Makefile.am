EXTRA_DIST = axl-doc.doxygen axl.css main-page-logo.png body_back.gif \
             axl-doc-footer.html \
	     axl-doc-header.html image01.png

bin_SCRIPTS = initial_build_doc

help:
	@echo 
	@echo " Documentation not built!:"
	@echo " ~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo " --> Doxygen documentation wasn't built because an stamp was detected: doc.stamp"
	@echo " --> To regenerate or force doc building type make build_doc or remove doc.stamp";
	@echo

initial_build_doc:
	@echo "checking documenation stamp..";
	@test -e doc.stamp & make help
	@test -e doc.stamp || make build_doc
	@test -e doc.stamp || touch doc.stamp

build_doc:
	@echo "Generating module documentation..";
	doxygen axl-doc.doxygen
	cp axl.css html
	cp main-page-logo.png html
	cp body_back.gif html

