/* KallistiOS ##version##

   kos/exports.h
   Copyright (C)2003 Dan Potter

*/

/** \file   kos/exports.h
    \brief  Kernel exported symbols support.

    This file contains support related to dynamic linking of the kernel of KOS.
    The kernel (at compile time) produces a list of exported symbols, which can
    be looked through using the funtionality in this file.

    \author Dan Potter
*/

#ifndef __KOS_EXPORTS_H
#define __KOS_EXPORTS_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>

/** \brief  A single export symbol.

    This structure holds a single symbol that has been exported from the kernel.
    These will be patched into loaded ELF binaries at load time.

    \headerfile kos/exports.h
*/
typedef struct export_sym {
    const char  * name;     /**< \brief The name of the symbol. */
    ptr_t       ptr;        /**< \brief A pointer to the symbol. */
} export_sym_t;

/** \cond */
/* These are the platform-independent exports */
extern export_sym_t kernel_symtab[];

/* And these are the arch-specific exports */
extern export_sym_t arch_symtab[];
/** \endcond */

#ifndef __EXPORTS_FILE
#include <kos/nmmgr.h>

/** \brief  A symbol table "handler" for nmmgr.
    \headerfile kos/exports.h
*/
typedef struct symtab_handler {
    struct nmmgr_handler nmmgr;     /**< \brief Name manager handler header */
    export_sym_t         * table;   /**< \brief Location of the first entry */
} symtab_handler_t;
#endif

/** \brief  Setup initial kernel exports.
    \retval 0               On success
    \retval -1              On error
*/
int export_init();

/** \brief  Look up a symbol by name.
    \param  name            The symbol to look up
    \return                 The export structure, or NULL on failure
*/
export_sym_t * export_lookup(const char * name);

__END_DECLS

#endif  /* __KOS_EXPORTS_H */

