/**
 * Axl Babel: encoding support for axl: iso-8859-1 coding
 * Copyright (C) 2022 Advanced Software Production Line, S.L.
 */

/* include base header */
#include <axl_babel.h>

#ifndef __AXL_BABEL_ISO88591_H__
#define __AXL_BABEL_ISO88591_H__

/**
 * @brief Creates the translation table for iso-8859-1 representation to move information
 * from iso-8859-1 to utf-8 and viceversa.
 */
axlBabelTable * axl_babel_build_iso88591_table (void);

#endif /* end __AXL_BABEL_ISO88591_H__ */

