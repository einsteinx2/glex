/* KallistiOS ##version##

   dc/maple/vmu.h
   Copyright (C)2000-2002 Jordan DeLong and Dan Potter
   Copyright (C)2008 Donald Haase

*/

/** \file   dc/maple/vmu.h
    \brief  Definitions for using the VMU device.

    This file contains the definitions needed to access the Maple VMU device.
    This includes all of the functionality of memory cards, including the
    MAPLE_FUNC_MEMCARD, MAPLE_FUNC_LCD, and MAPLE_FUNC_CLOCK function codes.

    \author Jordan DeLong
    \author Dan Potter
    \author Donald Haase
*/

#ifndef __DC_MAPLE_VMU_H
#define __DC_MAPLE_VMU_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <dc/maple.h>

/** \brief  Make a VMU beep.

    This function sends a raw beep to a VMU, causing the speaker to emit a tone
    noise. See http://dcemulation.org/phpBB/viewtopic.php?f=29&t=97048 for the
    original information about beeping.

    \param  dev             The device to attempt to beep.
    \param  beep            The tone to generate. Actual parameters unknown.
    \retval MAPLE_EOK       On success.
    \retval MAPLE_EAGAIN    If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
*/
int vmu_beep_raw(maple_device_t * dev, uint32 beep);

/** \brief  Display a 1bpp bitmap on a VMU screen.

    This function sends a raw bitmap to a VMU to display on its screen. This
    bitmap is 1bpp, and is 48x32 in size.

    \param  dev             The device to draw to.
    \param  bitmap          The bitmap to show.
    \retval MAPLE_EOK       On success.
    \retval MAPLE_EAGAIN    If the command couldn't be sent. Try again later.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
*/
int vmu_draw_lcd(maple_device_t * dev, void *bitmap);

/** \brief  Read a block from a memory card.

    This function reads a raw block from a memory card. You most likely will not
    ever use this directly, but rather will probably use the fs_vmu stuff.

    \param  dev             The device to read from.
    \param  blocknum        The block number to read.
    \param  buffer          The buffer to read into (512 bytes).
    \retval MAPLE_EOK       On success.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
    \retval MAPLE_EFAIL     On errors other than timeout.
*/
int vmu_block_read(maple_device_t * dev, uint16 blocknum, uint8 *buffer);

/** \brief  Write a block to a memory card.

    This function writes a raw block to a memory card. You most likely will not
    ever use this directly, but rather will probably use the fs_vmu stuff.

    \param  dev             The device to write to.
    \param  blocknum        The block number to write.
    \param  buffer          The buffer to write from (512 bytes).
    \retval MAPLE_EOK       On success.
    \retval MAPLE_ETIMEOUT  If the command timed out while blocking.
    \retval MAPLE_EFAIL     On errors other than timeout.
*/
int vmu_block_write(maple_device_t * dev, uint16 blocknum, uint8 *buffer);

/** \brief  Set a Xwindows XBM on all VMUs.

    This function takes in a Xwindows XBM and sets the image on all VMUs. This
    is a convenience function for vmu_draw_lcd() to broadcast across all VMUs.

    \param  vmu_icon        The icon to set.
*/
void vmu_set_icon(const char *vmu_icon);

/* \cond */
/* Init / Shutdown */
int vmu_init();
void vmu_shutdown();
/* \endcond */

__END_DECLS

#endif  /* __DC_MAPLE_VMU_H */

