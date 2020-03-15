/* KallistiOS ##version##

    dc/video.h
    Copyright (C) 2001 Anders Clerwall (scav)

*/

/** \file   dc/video.h
    \brief  Functions related to video output.

    This file deals with the video output hardware in the Dreamcast. There are
    functions defined herein that deal with setting up the video hardware,
    defining the resolution of the display, dealing with the framebuffer, etc.

    \author Anders Clerwall
    \author Dan Potter
*/

#ifndef __DC_VIDEO_H
#define __DC_VIDEO_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>

/** \defgroup vid_ctype Video Cable types

    The vid_check_cable() function will return one of this set of values to let
    you know what type of cable is connected to the Dreamcast. These are also
    used in the video mode settings to limit modes to certain cable types.

    @{
*/
#define CT_ANY          -1  /**< \brief Any cable type. Used only internally. */
#define CT_VGA          0   /**< \brief VGA Box */
#define CT_NONE         1   /**< \brief Nothing connected */
#define CT_RGB          2   /**< \brief RGB/SCART cable */
#define CT_COMPOSITE    3   /**< \brief Composite cable or RF switch */
/** @} */

/** \defgroup vid_pmode Video pixel modes

    This set of constants control the pixel mode that the framebuffer is set to.

    @{
*/
#define PM_RGB555   0       /**< \brief RGB555 pixel mode (15-bit) */
#define PM_RGB565   1       /**< \brief RGB565 pixel mode (16-bit) */
#define PM_RGB888   3       /**< \brief RGB888 pixel mode (24-bit) */
/** @} */

/** \brief  Generic display modes */
enum {
    DM_GENERIC_FIRST = 0x1000,      /**< \brief First valid generic mode */
    DM_320x240 = 0x1000,            /**< \brief 320x240 resolution */
    DM_640x480,                     /**< \brief 640x480 resolution */
    DM_800x608,                     /**< \brief 800x608 resolution */
    DM_256x256,                     /**< \brief 256x256 resolution */
    DM_768x480,                     /**< \brief 768x480 resolution */
    DM_768x576,                     /**< \brief 768x576 resolution */
    DM_GENERIC_LAST = DM_768x576    /**< \brief Last valid generic mode */
};

/** \brief  Multi-buffered mode setting.

    OR this with the generic mode to get four framebuffers instead of one.
*/
#define DM_MULTIBUFFER  0x2000

//-----------------------------------------------------------------------------
// More specific modes (and actual indeces into the mode table)

/** \brief  Specific display modes */
enum {
    DM_INVALID = 0,                 /**< \brief Invalid display mode */
    // Valid modes below
    DM_320x240_VGA = 1,             /**< \brief 320x240 VGA 60Hz */
    DM_320x240_NTSC,                /**< \brief 320x240 NTSC 60Hz */
    DM_640x480_VGA,                 /**< \brief 640x480 VGA 60Hz */
    DM_640x480_NTSC_IL,             /**< \brief 640x480 NTSC Interlaced 60Hz */
    DM_800x608_VGA,                 /**< \brief 800x608 VGA 60Hz */
    DM_640x480_PAL_IL,              /**< \brief 640x480 PAL Interlaced 50Hz */
    DM_256x256_PAL_IL,              /**< \brief 256x256 PAL Interlaced 50Hz */
    DM_768x480_NTSC_IL,             /**< \brief 768x480 NTSC Interlaced 60Hz */
    DM_768x576_PAL_IL,              /**< \brief 768x576 PAL Interlaced 50Hz */
    DM_768x480_PAL_IL,              /**< \brief 768x480 PAL Interlaced 50Hz */
    DM_320x240_PAL,                 /**< \brief 320x240 PAL 50Hz */
    DM_320x240_VGA_MB,              /**< \brief 320x240 VGA 60Hz, 4FBs */
    DM_320x240_NTSC_MB,             /**< \brief 320x240 NTSC 60Hz, 4FBs */
    DM_640x480_VGA_MB,              /**< \brief 640x480 VGA 60Hz, 4FBs */
    DM_640x480_NTSC_IL_MB,          /**< \brief 640x480 NTSC IL 60Hz, 4FBs */
    DM_800x608_VGA_MB,              /**< \brief 800x608 VGA 60Hz, 4FBs */
    DM_640x480_PAL_IL_MB,           /**< \brief 640x480 PAL IL 50Hz, 4FBs */
    DM_256x256_PAL_IL_MB,           /**< \brief 256x256 PAL IL 50Hz, 4FBs */
    DM_768x480_NTSC_IL_MB,          /**< \brief 768x480 NTSC IL 60Hz, 4FBs */
    DM_768x576_PAL_IL_MB,           /**< \brief 768x576 PAL IL 50Hz, 4FBs */
    DM_768x480_PAL_IL_MB,           /**< \brief 768x480 PAL IL 50Hz, 4FBs */
    DM_320x240_PAL_MB,              /**< \brief 320x240 PAL 50Hz, 4FBs */
    // The below is only for counting..
    DM_SENTINEL,                    /**< \brief Sentinel value, for counting */
    DM_MODE_COUNT                   /**< \brief Number of modes */
};

/** \brief  The maximum number of framebuffers available. */
#define VID_MAX_FB  4   // <-- This should be enough

// These are for the "flags" field of "vid_mode_t"
/** \defgroup vid_flags Flags for the field in vid_mode_t.

    These flags indicate various things related to the modes for a vid_mode_t.

    @{
*/
#define VID_INTERLACE   0x00000001  /**< \brief Interlaced display */
#define VID_LINEDOUBLE  0x00000002  /**< \brief Display each scanline twice */
#define VID_PIXELDOUBLE 0x00000004  /**< \brief Display each pixel twice */
#define VID_PAL         0x00000008  /**< \brief 50Hz refresh rate, if not VGA */
/** @} */

/** \brief  Video mode structure.

    KOS maintains a list of valid video modes internally that correspond to the
    specific display modes enumeration. Each of them is built of one of these.

    \headerfile dc/video.h
*/
typedef struct vid_mode {
    int     generic;    /**< \brief Generic mode type for vid_set_mode() */
    uint16  width;      /**< \brief Width of the display, in pixels */
    uint16  height;     /**< \brief Height of the display, in pixels */
    uint32  flags;      /**< \brief Combination of one or more VID_* flags */

    int16   cable_type; /**< \brief Allowed cable type */
    uint16  pm;         /**< \brief Pixel mode */

    uint16  scanlines;  /**< \brief Number of scanlines */
    uint16  clocks;     /**< \brief Clocks per scanline */
    uint16  bitmapx;    /**< \brief Bitmap window X position */
    uint16  bitmapy;    /**< \brief Bitmap window Y position (automatically
                                    increased for PAL) */
    uint16  scanint1;   /**< \brief First scanline interrupt position */
    uint16  scanint2;   /**< \brief Second scanline interrupt position
                                    (automatically doubled for VGA) */
    uint16  borderx1;   /**< \brief Border X starting position */
    uint16  borderx2;   /**< \brief Border X stop position */
    uint16  bordery1;   /**< \brief Border Y starting position */
    uint16  bordery2;   /**< \brief Border Y stop position */

    uint16  fb_curr;    /**< \brief Current framebuffer */
    uint16  fb_count;   /**< \brief Number of framebuffers */
    uint32  fb_base[VID_MAX_FB];    /**< \brief Offset to framebuffers */
} vid_mode_t;

/** \brief  The list of builtin video modes. Do not modify these! */
extern vid_mode_t vid_builtin[DM_MODE_COUNT];

/** \brief  The current video mode. Do not modify directly! */
extern vid_mode_t *vid_mode;

// These point to the current drawing area. If you're not using a multi-buffered
// mode, that means they do what KOS always used to do (they'll point at the
// start of VRAM). If you're using a multi-buffered mode, they'll point at the
// next framebuffer to be displayed. You must use vid_flip for this to work
// though (if you use vid_set_start, they'll point at the display base, for
// compatibility's sake).

/** \brief  16-bit size pointer to the current drawing area. */
extern uint16 *vram_s;

/** \brief  32-bit size pointer to the current drawing area. */
extern uint32 *vram_l;


/** \brief  Retrieve the connected video cable type.

    This function checks the video cable and reports what it finds.

    \retval CT_VGA          If a VGA Box or cable is connected.
    \retval CT_NONE         If nothing is connected.
    \retval CT_RGB          If a RGB/SCART cable is connected.
    \retval CT_COMPOSITE    If a composite cable or RF switch is connected.
*/
int vid_check_cable();

/** \brief  Set the VRAM base of the framebuffer.

    This function sets the vram_s and vram_l pointsers to specified offset
    within VRAM and sets the start position of the framebuffer to the same
    offset.

    \param  base            The offset within VRAM to set the base to.
*/
void vid_set_start(uint32 base);

/** \brief  Set the current framebuffer in a multibuffered setup.

    This function sets the displayed framebuffer to the specified buffer and
    sets the vram_s and vram_l pointers to point at the next framebuffer, to
    allow for tearing-free framebuffer-direct drawing.

    \param  fb              The framebuffer to display (or -1 for the next one).
*/
void vid_flip(int fb);

/** \brief  Set the border color of the display.

    This sets the color of the border area of the display. On some screens, the
    border area may not be shown at all, whereas on some displays you may see
    the whole thing.

    \param  r               The red value of the color (0-255).
    \param  g               The green value of the color (0-255).
    \param  b               The blue value of the color (0-255).
    \return                 Old border color value (RGB888)
*/
uint32 vid_border_color(int r, int g, int b);

/** \brief  Clear the display.

    This function sets the whole display to the specified color. Internally,
    this uses the store queues to actually clear the display entirely.

    \param  r               The red value of the color (0-255).
    \param  g               The green value of the color (0-255).
    \param  b               The blue value of the color (0-255).
*/
void vid_clear(int r, int g, int b);

/** \brief  Clear VRAM.

    This function is essentially a memset() for the whole of VRAM that will
    clear it all to 0 bytes.
*/
void vid_empty();

/** \brief  Wait for VBlank.

    This function busy loops until the vertical blanking period starts.
*/
void vid_waitvbl();

/** \brief  Set the video mode.

    This function sets the current video mode to the one specified by the
    parameters.

    \param  dm              The display mode to use. One of the DM_* values.
    \param  pm              The pixel mode to use. One of the PM_* values.
*/
void vid_set_mode(int dm, int pm);

/** \brief  Set the video mode.

    This function sets the current video mode to the mode structure passed in.
    You can use this to add support to your program for modes that KOS doesn't
    have support for built-in (of course, you should tell us the settings so we
    can add them into KOS if you do this).

    \param  mode            A filled in vid_mode_t for the mode wanted.
*/
void vid_set_mode_ex(vid_mode_t *mode);

/** \brief  Initialize the video system.

    This function initializes the video display, setting the mode to the
    specified parameters, clearing vram, and setting the first framebuffer as
    active.

    \param  disp_mode       The display mode to use. One of the DM_* values.
    \param  pixel_mode      The pixel mode to use. One of the PM_* values.
*/
void vid_init(int disp_mode, int pixel_mode);

/** \brief  Shut down the video system.

    This function reinitializes the video system to what dcload and friends
    expect it to be.
*/
void vid_shutdown();

/** \brief  Take a screenshot.

    This function takes the current framebuffer (vram_s/vram_l) and dumps it out
    to a PPM file.

    \param  destfn          The filename to save to.
    \return                 0 on success, <0 on failure.
*/
int vid_screen_shot(const char * destfn);

__END_DECLS

#endif  // __DC_VIDEO_H

