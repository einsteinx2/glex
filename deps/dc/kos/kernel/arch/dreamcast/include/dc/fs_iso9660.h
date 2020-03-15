/* KallistiOS ##version##

   dc/fs/iso9660.h
   (c)2000-2001 Dan Potter

*/

/** \file   dc/fs_iso9660.h
    \brief  ISO9660 (CD-ROM) filesystem driver.

    This driver implements support for reading files from a CD-ROM or CD-R in
    the Dreamcast's disc drive. This filesystem mounts itself on /cd.

    This driver supports Rock Ridge, thanks to Andrew Kieschnick. The driver
    also supports the Joliet extensions thanks to Bero.

    The implementation was originally based on a simple ISO9660 implementation
    by Marcus Comstedt.

    \author Dan Potter
    \author Andrew Kieschnick
    \author Bero
*/

#ifndef __DC_FS_ISO9660_H
#define __DC_FS_ISO9660_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <kos/limits.h>
#include <kos/fs.h>

/** \brief  The maximum number of files that can be open at once. */
#define MAX_ISO_FILES 8

/** \brief  Reset the internal ISO9660 cache.

    This function resets the cache of the ISO9660 driver, breaking connections
    to all files. This generally assumes that a new disc has been or will be
    inserted.

    \retval 0               On success.
*/
int iso_reset();

/* \cond */
int fs_iso9660_init();
int fs_iso9660_shutdown();
/* \endcond */

__END_DECLS

#endif  /* __DC_FS_ISO9660_H */

