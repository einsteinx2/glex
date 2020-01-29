/* KallistiOS ##version##

   dc/spu.h
   Copyright (C) 2000-2001 Dan Potter

*/

/** \file   dc/spu.h
    \brief  Functions related to sound.

    This file deals with memory transfers and the like for the sound hardware.

    \author Dan Potter
*/

#ifndef __DC_SPU_H
#define __DC_SPU_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <dc/g2bus.h>

/** \brief  Waits for the sound FIFO to empty. */
void spu_write_wait();

/** \brief  Copy a block of data to sound RAM.

    This function acts much like memcpy() but copies to the sound RAM area.

    \param  to              The offset in sound RAM to copy to. Do not include
                            the 0xA0800000 part, it is implied.
    \param  from            A pointer to copy from.
    \param  length          The number of bytes to copy. Automatically rounded
                            up to be a multiple of 4.
*/
void spu_memload(uint32 to, void *from, int length);

/** \brief  Copy a block of data from sound RAM.

    This function acts much like memcpy() but copies from the sound RAM area.

    \param  to              A pointer to copy to.
    \param  from            The offset in sound RAM to copy from. Do not include
                            the 0xA0800000 part, it is implied.
    \param  length          The number of bytes to copy. Automatically rounded
                            up to be a multiple of 4.
*/
void spu_memread(void *to, uint32 from, int length);

/** \brief  Set a block of sound RAM to the specified value.

    This function acts like memset4(), setting the specified block of sound RAM
    to the given 32-bit value.

    \param  to              The offset in sound RAM to set at. Do not include
                            the 0xA0800000 part, it is implied.
    \param  what            The value to set.
    \param  length          The number of bytes to copy. Automatically rounded
                            up to be a multiple of 4.
*/
void spu_memset(uint32 to, uint32 what, int length);

/* DMA copy from SH-4 RAM to SPU RAM; length must be a multiple of 32,
   and the source and destination addresses must be aligned on 32-byte
   boundaries. If block is non-zero, this function won't return until
   the transfer is complete. If callback is non-NULL, it will be called
   upon completion (in an interrupt context!). Returns <0 on error. */

/** \brief  SPU DMA callback type. */
typedef g2_dma_callback_t spu_dma_callback_t;

/** \brief  Copy a block of data from SH4 RAM to sound RAM via DMA.

    This function sets up a DMA transfer from main RAM to the sound RAM with G2
    DMA.

    \param  from            A pointer in main RAM to transfer from. Must be
                            32-byte aligned.
    \param  dest            Offset in sound RAM to transfer to. Do not include
                            the 0xA0800000 part, its implied. Must be 32-byte
                            aligned.
    \param  length          Number of bytes to copy. Must be a multiple of 32.
    \param  block           1 if you want to wait for the transfer to complete,
                            0 otherwise (use the callback for this case).
    \param  callback        Function to call when the DMA completes. Can be NULL
                            if you don't want to have a callback. This will be
                            called in an interrupt context, so keep that in mind
                            when writing the function.
    \param  cbdata          Data to pass to the callback function.
    \retval -1              On failure. Sets errno as appropriate.
    \retval 0               On success.

    \par    Error Conditions:
    \em     EINVAL - Invalid channel \n
    \em     EFAULT - from or dest is not aligned \n
    \em     EIO - I/O error
*/
int spu_dma_transfer(void * from, uint32 dest, uint32 length, int block,
                     spu_dma_callback_t callback, ptr_t cbdata);

/** \brief  Enable the SPU.

    This function resets all sound channels and lets the ARM out of reset.
*/
void spu_enable();

/** \brief  Disable the SPU.

    This function resets all sound channels and puts the ARM in a reset state.
*/
void spu_disable();

/** \brief  Set CDDA volume.

    Valid volume values are 0-15.

    \param  left_volume     Volume of the left channel.
    \param  right_volume    Volume of the right channel.
*/
void spu_cdda_volume(int left_volume, int right_volume);

/** \brief  Set CDDA panning.

    Valid values are from 0-31. 16 is centered.

    \param  left_pan        Pan of the left channel.
    \param  right_pan       Pan of the right channel.
*/
void spu_cdda_pan(int left_pan, int right_pan);

/** \brief  Set master mixer settings.

    This function sets the master mixer volume and mono/stereo setting.

    \param  volume          The volume to set (0-15).
    \param  stereo          1 for stereo output, 0 for mono.
*/
void spu_master_mixer(int volume, int stereo);

/** \brief  Initialize the SPU.

    This function will reset the SPU, clear the sound RAM, reinit the CDDA
    support and run an infinite loop on the ARM.

    \retval 0               On success (no error conditions defined).
*/
int spu_init();

/** \brief  Shutdown the SPU.

    This function disables the SPU and clears sound RAM.

    \retval 0               On success (no error conditions defined).
*/
int spu_shutdown();

/** \brief  Initialize SPU DMA support.

    This function sets up the DMA support for transfers to the sound RAM area.

    \retval 0               On success (no error conditions defined).
*/
int spu_dma_init();

/** \brief  Shutdown SPU DMA support. */
void spu_dma_shutdown();

__END_DECLS

#endif  /* __DC_SPU_H */

