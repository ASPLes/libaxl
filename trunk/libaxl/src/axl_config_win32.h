/*
 * Axl Library axl_config.h
 * Platform dependant definitions for Win32 platform.
 *
 * This file is maintained manually for those people that do not
 * compile axl using autoconf. It should look really similar to
 * axl_config.h file created on a i386 linux platform but changing
 * AXL_OS_UNIX to AXL_OS_WIN32 (at least for now).
 *
 *  For commercial support on build XML enabled solutions contact us:
 *          
 *      Postal address:
 *         Advanced Software Production Line, S.L.
 *         Edificio Alius A, Oficina 102,
 *         C/ Antonio Suarez Nº 10,
 *         Alcalá de Henares 28802 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/xml
 *
 */

#ifndef __AXL_CONFIG_H__
#define __AXL_CONFIG_H__

/**
 * \addtogroup axl_decl_module
 * @{
 */

/**
 * @brief Allows to convert integer value (including constant values)
 * into a pointer representation.
 *
 * Use the oposite function to restore the value from a pointer to a
 * integer: \ref PTR_TO_INT.
 *
 * @param integer The integer value to cast to pointer.
 *
 * @return A \ref axlPointer reference.
 */
#define INT_TO_PTR(integer) ((axlPointer)  (integer))

/**
 * @brief Allows to convert a pointer reference (\ref axlPointer),
 * which stores an integer that was stored using \ref INT_TO_PTR.
 *
 * Use the oposite function to restore the pointer value stored in the
 * integer value.
 *
 * @param ptr The pointer to cast to a integer value.
 *
 * @return A int value.
 */
#define PTR_TO_INT(ptr) ((int)  (ptr))

/**
 * @brief Allows to get current platform configuration. This is used
 * by Axl library but could be used by applications built on top of
 * Axl to change its configuration based on the platform information.
 */
#define AXL_OS_WIN32 (1)

/* @} */

#endif
