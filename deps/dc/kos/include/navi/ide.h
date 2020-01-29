/* KallistiOS ##version##

   navi/ide.h
   Copyright (C) 2002 Dan Potter

*/

/** \file   navi/ide.h
    \brief  External G2 Bus-based IDE support.

    This file is involved with accessing an IDE controller that is attached to
    the G2 Bus expansion port. Exact details of how to build such a device have
    been posted in various places around the Internet. This driver refers to the
    device built by Dan as a part of the Navi project.

    \author Dan Potter
*/

#ifndef __NAVI_IDE_H
#define __NAVI_IDE_H

#include <arch/types.h>

/** \brief  Read sectors from the hard disk via PIO.
    \param  linear          The address to begin reading from.
    \param  numsects        The number of sectors to read.
    \param  bufptr          The buffer to read into.
    \return                 0 on success, <0 on error.
*/
int ide_read(uint32 linear, uint32 numsects, void *bufptr);

/** \brief  Write sectors from the hard disk via PIO.
    \param  linear          The address to begin writing to.
    \param  numsects        The number of sectors to write.
    \param  bufptr          The buffer to write out of.
    \return                 0 on success, <0 on error.
*/
int ide_write(uint32 linear, uint32 numsects, void *bufptr);

/** \brief  Retrieve the number of sectors from the hard disk.
    \returns                The total number of linear sectors.
*/
uint32 ide_num_sectors();

/** \brief  Initialize Navi IDE.
    \return                 0 on success (no error conditions defined).
*/
int ide_init();

/** \brief  Shutdown Navi IDE. */
void ide_shutdown();

#endif  /* __NAVI_IDE_H */
