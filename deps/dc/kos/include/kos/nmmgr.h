/* KallistiOS ##version##

   kos/nmmgr.h
   Copyright (C)2003 Dan Potter

*/

/** \file   kos/nmmgr.h
    \brief  Name manager.

    This file contains the definitions of KOS' name manager. A "name" is a
    generic identifier for some kind of module. These modules may include
    services provided by the kernel (such as VFS handlers).

    \author Dan Potter
*/

#ifndef __KOS_NMMGR_H
#define __KOS_NMMGR_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <kos/limits.h>
#include <sys/queue.h>

/* Pre-define list types */
struct nmmgr_handler;

/** \brief  Name handler list type.

    Contrary to what doxygen may think, this is not a function.
*/
typedef LIST_HEAD(nmmgr_list, nmmgr_handler) nmmgr_list_t;

/** \brief  List entry initializer for static structs.

    If you are creating nmmgr handlers, this is what you should initialize the
    list_ent member with.
*/
#define NMMGR_LIST_INIT { NULL }

/** \brief  Name handler interface.

    Every name handler must begin its information structures with this header.
    If the handler conforms to some well-defined interface (such as a VFS), then
    the struct must more specifically be of that type.

    \headerfile kos/nmmgr.h
*/
typedef struct nmmgr_handler {
    char    pathname[MAX_FN_LEN];   /* Path name */
    int pid;            /* Process table ID for handler (0 == static) */
    uint32  version;        /* Version code */
    uint32  flags;          /* Bitmask of flags */
    uint32  type;           /* Type of handler */
    LIST_ENTRY(nmmgr_handler)   list_ent;   /* Linked list entry */
} nmmgr_handler_t;

/* Version codes ('version') have two pieces: a major and minor revision.
   A major revision (top 16 bits) means that the interfaces are totally
   incompatible. A minor revision (lower 16 bits) diffrentiates between
   mostly-compatible but newer/older revisions of the implementing code. */

/* Flag bits */
/** \brief  This structure must be freed when removed. */
#define NMMGR_FLAGS_NEEDSFREE   0x00000001

/** \defgroup   nmmgr_types     Name handler types

    This is the set of all defined types of name manager handlers. All system
    types are defined below NMMGR_SYS_MAX.

    @{
*/
/** \brief  Unknown nmmgr type. */
#define NMMGR_TYPE_UNKNOWN  0x0000      /* ? */
/** \brief  A mounted filesystem. */
#define NMMGR_TYPE_VFS      0x0010      /* Mounted file system */
/** \brief  A block device. */
#define NMMGR_TYPE_BLOCKDEV 0x0020      /* Block device */
/** \brief  A singleton service (e.g., /dev/irq) */
#define NMMGR_TYPE_SINGLETON    0x0030      /* Singleton service (e.g., /dev/irq) */
/** \brief  A symbol table. */
#define NMMGR_TYPE_SYMTAB   0x0040      /* Symbol table */
/** \brief  Everything this and above is a user type. */
#define NMMGR_SYS_MAX       0x10000     /* Here and above are user types */
/** @} */

/** \brief  Retrieve a name handler by name.

    This function will retrieve a name handler by its pathname.

    \param  name            The handler to look up
    \return                 The handler, or NULL on failure.
*/
nmmgr_handler_t * nmmgr_lookup(const char *name);

/** \brief  Get the head element of the name list.

    DO NOT MODIFY THE VALUE RETURNED BY THIS FUNCTION! In fact, don't ever call
    this function.

    \return                 The head of the name handler list
*/
nmmgr_list_t * nmmgr_get_list();

/** \brief  Add a name handler.

    This function adds a new name handler to the list in the kernel.

    \param  hnd             The handler to add
    \retval 0               On success
*/
int nmmgr_handler_add(nmmgr_handler_t *hnd);

/** \brief  Remove a name handler.

    This function removes a name handler from the list in the kernel.

    \param  hnd             The handler to remove
    \retval 0               On success
    \retval -1              If the handler wasn't found
*/
int nmmgr_handler_remove(nmmgr_handler_t *hnd);

/** \cond */
/* Name manager init */
int nmmgr_init();
void    nmmgr_shutdown();
/** \endcond */

__END_DECLS

#endif  /* __KOS_NMMGR_H */

