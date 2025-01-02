#!/bin/bash

# english web
rsync -avz ../doc/logo-aspl-white-100.png *.html *.png *.txt *.svg *.css  aspl-web@www.aspl.es:www/axl/
# spanish web
rsync -avz es/*.html es/*.png es/*.txt es/*.svg es/*.css  aspl-web@www.aspl.es:www/axl/es/

