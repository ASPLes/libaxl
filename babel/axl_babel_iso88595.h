/**
 * Axl Babel: encoding support for axl: iso-8859-5 coding
 * Copyright (C) 2013 Advanced Software Production Line, S.L.
 */

/* include base header */
#include <axl_babel.h>

#ifndef __AXL_BABEL_ISO88595_H__
#define __AXL_BABEL_ISO88595_H__

/**
 * @brief Creates the translation table for iso-8859-5 representation to move information
 * from iso-8859-5 to utf-8 and viceversa.
 */
axlBabelTable * axl_babel_build_iso88595_table (void);

#endif /* end __AXL_BABEL_ISO88595_H__ */

