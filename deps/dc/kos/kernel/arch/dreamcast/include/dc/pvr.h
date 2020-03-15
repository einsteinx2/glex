/* KallistiOS ##version##

   dc/pvr.h
   Copyright (C) 2002 Dan Potter
   Copyright (C) 2014 Lawrence Sebald

   Low-level PVR 3D interface for the DC
   Note: this API does _not_ handle any sort of transformations
   (including perspective!) so for that, you should look to KGL.
*/

/** \file   dc/pvr.h
    \brief  Low-level PVR (3D hardware) interface.

    This file provides support for using the PVR 3D hardware in the Dreamcast.
    Note that this does not handle any sort of perspective transformations or
    anything of the like. This is just a very thin wrapper around the actual
    hardware support.

    This file is used for pretty much everything related to the PVR, from memory
    management to actual primitive rendering.

    \author Dan Potter
    \author Roger Cattermole
    \author Paul Boese
    \author Brian Paul
    \author Lawrence Sebald
    \author Benoit Miller
*/

#ifndef __DC_PVR_H
#define __DC_PVR_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <dc/sq.h>
#include <kos/img.h>

/* Data types ********************************************************/

/** \brief  PVR texture memory pointer.

    Unlike the old "TA" system, PVR pointers in the new system are actually SH-4
    compatible pointers and can be used directly in place of ta_txr_map().

    Not that anyone probably even remembers the old TA system anymore... */
typedef void * pvr_ptr_t;

/** \brief  PVR list specification.

    Each primitive in the PVR is submitted to one of the hardware primitive
    lists. This type is an identifier for a list.

    \see    pvr_lists
*/
typedef uint32 pvr_list_t;

/** \brief  PVR polygon context.

    You should use this more human readable format for specifying your polygon
    contexts, and then compile them into polygon headers when you are ready to
    start using them.

    This has embedded structures in it for two reasons; the first reason is to
    make it easier for me to add new stuff later without breaking existing code.
    The second reason is to make it more readable and usable.

    Unfortunately, it seems that Doxygen chokes up a little bit on this
    structure, and others like it. The documentation should still be mostly
    understandable though...

    \headerfile dc/pvr.h
*/
typedef struct {
    int     list_type;          /**< \brief Primitive list
                                     \see   pvr_lists */
    struct {
        int     alpha;          /**< \brief Enable or disable alpha outside modifier
                                     \see   pvr_alpha_switch */
        int     shading;        /**< \brief Shading type
                                     \see   pvr_shading_types */
        int     fog_type;       /**< \brief Fog type outside modifier
                                     \see   pvr_fog_types */
        int     culling;        /**< \brief Culling mode
                                     \see   pvr_cull_modes */
        int     color_clamp;    /**< \brief Color clamp enable/disable outside modifier
                                     \see   pvr_colclamp_switch */
        int     clip_mode;      /**< \brief Clipping mode
                                     \see   pvr_clip_modes */
        int     modifier_mode;  /**< \brief Modifier mode */
        int     specular;       /**< \brief Offset color enable/disable outside modifier
                                     \see   pvr_offset_switch */
        int     alpha2;         /**< \brief Enable/disable alpha inside modifier
                                     \see   pvr_alpha_switch */
        int     fog_type2;      /**< \brief Fog type inside modifier
                                     \see   pvr_fog_types */
        int     color_clamp2;   /**< \brief Color clamp enable/disable inside modifier
                                     \see   pvr_colclamp_switch */
    } gen;                      /**< \brief General parameters */
    struct {
        int     src;            /**< \brief Source blending mode outside modifier
                                     \see   pvr_blend_modes */
        int     dst;            /**< \brief Dest blending mode outside modifier
                                     \see   pvr_blend_modes */
        int     src_enable;     /**< \brief Source blending enable outside modifier
                                     \see   pvr_blend_switch */
        int     dst_enable;     /**< \brief Dest blending enable outside modifier
                                     \see   pvr_blend_switch */
        int     src2;           /**< \brief Source blending mode inside modifier
                                     \see   pvr_blend_modes */
        int     dst2;           /**< \brief Dest blending mode inside modifier
                                     \see   pvr_blend_modes */
        int     src_enable2;    /**< \brief Source blending mode inside modifier
                                     \see   pvr_blend_switch */
        int     dst_enable2;    /**< \brief Dest blending mode inside modifier
                                     \see   pvr_blend_switch */
    } blend;                    /**< \brief Blending parameters */
    struct {
        int     color;          /**< \brief Color format in vertex
                                     \see   pvr_color_fmts */
        int     uv;             /**< \brief U/V data format in vertex
                                     \see   pvr_uv_fmts */
        int     modifier;       /**< \brief Enable or disable modifier effect
                                     \see   pvr_mod_switch */
    } fmt;                      /**< \brief Format control */
    struct {
        int     comparison;     /**< \brief Depth comparison mode
                                     \see pvr_depth_modes */
        int     write;          /**< \brief Enable or disable depth writes
                                     \see pvr_depth_switch */
    } depth;                    /**< \brief Depth comparison/write modes */
    struct {
        int     enable;         /**< \brief Enable/disable texturing
                                     \see   pvr_txr_switch */
        int     filter;         /**< \brief Filtering mode
                                     \see   pvr_filter_modes */
        int     mipmap;         /**< \brief Enable/disable mipmaps
                                     \see   pvr_mip_switch */
        int     mipmap_bias;    /**< \brief Mipmap bias
                                     \see   pvr_mip_bias */
        int     uv_flip;        /**< \brief Enable/disable U/V flipping
                                     \see   pvr_uv_flip */
        int     uv_clamp;       /**< \brief Enable/disable U/V clamping
                                     \see   pvr_uv_clamp */
        int     alpha;          /**< \brief Enable/disable texture alpha
                                     \see   pvr_txralpha_switch */
        int     env;            /**< \brief Texture color contribution
                                     \see   pvr_txrenv_modes */
        int     width;          /**< \brief Texture width (requires a power of 2) */
        int     height;         /**< \brief Texture height (requires a power of 2) */
        int     format;         /**< \brief Texture format
                                     \see   pvr_txr_fmts */
        pvr_ptr_t base;         /**< \brief Texture pointer */
    } txr;                      /**< \brief Texturing params outside modifier */
    struct {
        int     enable;         /**< \brief Enable/disable texturing
                                     \see   pvr_txr_switch */
        int     filter;         /**< \brief Filtering mode
                                     \see   pvr_filter_modes */
        int     mipmap;         /**< \brief Enable/disable mipmaps
                                     \see   pvr_mip_switch */
        int     mipmap_bias;    /**< \brief Mipmap bias
                                     \see   pvr_mip_bias */
        int     uv_flip;        /**< \brief Enable/disable U/V flipping
                                     \see   pvr_uv_flip */
        int     uv_clamp;       /**< \brief Enable/disable U/V clamping
                                     \see   pvr_uv_clamp */
        int     alpha;          /**< \brief Enable/disable texture alpha
                                     \see   pvr_txralpha_switch */
        int     env;            /**< \brief Texture color contribution
                                     \see   pvr_txrenv_modes */
        int     width;          /**< \brief Texture width (requires a power of 2) */
        int     height;         /**< \brief Texture height (requires a power of 2) */
        int     format;         /**< \brief Texture format
                                     \see   pvr_txr_fmts */
        pvr_ptr_t base;         /**< \brief Texture pointer */
    } txr2;                     /**< \brief Texturing params inside modifier */
} pvr_poly_cxt_t;

/** \brief  PVR sprite context.

    You should use this more human readable format for specifying your sprite
    contexts, and then compile them into sprite headers when you are ready to
    start using them.

    Unfortunately, it seems that Doxygen chokes up a little bit on this
    structure, and others like it. The documentation should still be mostly
    understandable though...

    \headerfile dc/pvr.h
*/
typedef struct {
    int     list_type;          /**< \brief Primitive list
                                     \see   pvr_lists */
    struct {
        int     alpha;          /**< \brief Enable or disable alpha
                                     \see   pvr_alpha_switch */
        int     fog_type;       /**< \brief Fog type
                                     \see   pvr_fog_types */
        int     culling;        /**< \brief Culling mode
                                     \see   pvr_cull_modes */
        int     color_clamp;    /**< \brief Color clamp enable/disable
                                     \see   pvr_colclamp_switch */
        int     clip_mode;      /**< \brief Clipping mode
                                     \see   pvr_clip_modes */
        int     specular;       /**< \brief Offset color enable/disable
                                     \see   pvr_offset_switch */
    } gen;                      /**< \brief General parameters */
    struct {
        int     src;            /**< \brief Source blending mode
                                     \see   pvr_blend_modes */
        int     dst;            /**< \brief Dest blending mode
                                     \see   pvr_blend_modes */
        int     src_enable;     /**< \brief Source blending enable
                                     \see   pvr_blend_switch */
        int     dst_enable;     /**< \brief Dest blending enable
                                     \see   pvr_blend_switch */
    } blend;
    struct {
        int     comparison;     /**< \brief Depth comparison mode
                                     \see pvr_depth_modes */
        int     write;          /**< \brief Enable or disable depth writes
                                     \see pvr_depth_switch */
    } depth;                    /**< \brief Depth comparison/write modes */
    struct {
        int     enable;         /**< \brief Enable/disable texturing
                                     \see   pvr_txr_switch */
        int     filter;         /**< \brief Filtering mode
                                     \see   pvr_filter_modes */
        int     mipmap;         /**< \brief Enable/disable mipmaps
                                     \see   pvr_mip_switch */
        int     mipmap_bias;    /**< \brief Mipmap bias
                                     \see   pvr_mip_bias */
        int     uv_flip;        /**< \brief Enable/disable U/V flipping
                                     \see   pvr_uv_flip */
        int     uv_clamp;       /**< \brief Enable/disable U/V clamping
                                     \see   pvr_uv_clamp */
        int     alpha;          /**< \brief Enable/disable texture alpha
                                     \see   pvr_txralpha_switch */
        int     env;            /**< \brief Texture color contribution
                                     \see   pvr_txrenv_modes */
        int     width;          /**< \brief Texture width (requires a power of 2) */
        int     height;         /**< \brief Texture height (requires a power of 2) */
        int     format;         /**< \brief Texture format
                                     \see   pvr_txr_fmts */
        pvr_ptr_t base;         /**< \brief Texture pointer */
    } txr;                      /**< \brief Texturing params */
} pvr_sprite_cxt_t;

/* Constants for the above structure; thanks to Benoit Miller for these */

/** \defgroup pvr_lists PVR primitive list types

    Each primitive submitted to the PVR must be placed in one of these lists,
    depending on its characteristics.

    @{
*/
#define PVR_LIST_OP_POLY        0   /**< \brief Opaque polygon list */
#define PVR_LIST_OP_MOD         1   /**< \brief Opaque modifier list */
#define PVR_LIST_TR_POLY        2   /**< \brief Translucent polygon list */
#define PVR_LIST_TR_MOD         3   /**< \brief Translucent modifier list*/
#define PVR_LIST_PT_POLY        4   /**< \brief Punch-thru polygon list */
/** @} */

/** \defgroup pvr_shading_types     PVR shading modes

    Each polygon can define how it wants to be shaded, be it with flat or
    Gouraud shading using these constants in the appropriate place in its
    pvr_poly_cxt_t.

    @{
*/
#define PVR_SHADE_FLAT          0   /**< \brief Use flat shading */
#define PVR_SHADE_GOURAUD       1   /**< \brief Use Gouraud shading */
/** @} */

/** \defgroup pvr_depth_modes       PVR depth comparison modes

    These set the depth function used for comparisons.

    @{
*/
#define PVR_DEPTHCMP_NEVER      0   /**< \brief Never pass */
#define PVR_DEPTHCMP_LESS       1   /**< \brief Less than */
#define PVR_DEPTHCMP_EQUAL      2   /**< \brief Equal to */
#define PVR_DEPTHCMP_LEQUAL     3   /**< \brief Less than or equal to */
#define PVR_DEPTHCMP_GREATER    4   /**< \brief Greater than */
#define PVR_DEPTHCMP_NOTEQUAL   5   /**< \brief Not equal to */
#define PVR_DEPTHCMP_GEQUAL     6   /**< \brief Greater than or equal to */
#define PVR_DEPTHCMP_ALWAYS     7   /**< \brief Always pass */
/** @} */

/** \defgroup pvr_cull_modes        PVR culling modes

    These culling modes can be set by polygons to determine when they are
    culled. They work pretty much as you'd expect them to if you've ever used
    any 3D hardware before.

    @{
*/
#define PVR_CULLING_NONE        0   /**< \brief Disable culling */
#define PVR_CULLING_SMALL       1   /**< \brief Cull if small */
#define PVR_CULLING_CCW         2   /**< \brief Cull if counterclockwise */
#define PVR_CULLING_CW          3   /**< \brief Cull if clockwise */
/** @} */

/** \defgroup pvr_depth_switch      Enable or disable PVR depth writes
    @{
*/
#define PVR_DEPTHWRITE_ENABLE   0   /**< \brief Update the Z value */
#define PVR_DEPTHWRITE_DISABLE  1   /**< \brief Do not update the Z value */
/** @} */

/** \defgroup pvr_txr_switch        Enable or disable texturing on polygons
    @{
*/
#define PVR_TEXTURE_DISABLE     0   /**< \brief Disable texturing */
#define PVR_TEXTURE_ENABLE      1   /**< \brief Enable texturing */
/** @} */

/** \defgroup pvr_blend_modes       PVR blending modes

    These are all the blending modes that can be done with regard to alpha
    blending on the PVR.

    @{
*/
#define PVR_BLEND_ZERO          0   /**< \brief None of this color */
#define PVR_BLEND_ONE           1   /**< \brief All of this color */
#define PVR_BLEND_DESTCOLOR     2   /**< \brief Destination color */
#define PVR_BLEND_INVDESTCOLOR  3   /**< \brief Inverse of destination color */
#define PVR_BLEND_SRCALPHA      4   /**< \brief Blend with source alpha */
#define PVR_BLEND_INVSRCALPHA   5   /**< \brief Blend with inverse source alpha */
#define PVR_BLEND_DESTALPHA     6   /**< \brief Blend with destination alpha */
#define PVR_BLEND_INVDESTALPHA  7   /**< \brief Blend with inverse destination alpha */
/** @} */

/** \defgroup pvr_blend_switch      Enable or disable blending
    @{
*/
#define PVR_BLEND_DISABLE       0   /**< \brief Disable blending */
#define PVR_BLEND_ENABLE        1   /**< \brief Enable blending */
/** @} */

/** \defgroup pvr_fog_types         PVR fog modes

    Each polygon can decide what fog type is used with regard to it using these
    constants in its pvr_poly_cxt_t.

    @{
*/
#define PVR_FOG_TABLE           0   /**< \brief Table fog */
#define PVR_FOG_VERTEX          1   /**< \brief Vertex fog */
#define PVR_FOG_DISABLE         2   /**< \brief Disable fog */
#define PVR_FOG_TABLE2          3   /**< \brief Table fog mode 2 */
/** @} */

/** \defgroup pvr_clip_modes        PVR clipping modes

    These control how primitives are clipped against the user clipping area.

    @{
*/
#define PVR_USERCLIP_DISABLE    0   /**< \brief Disable clipping */
#define PVR_USERCLIP_INSIDE     2   /**< \brief Enable clipping inside area */
#define PVR_USERCLIP_OUTSIDE    3   /**< \brief Enable clipping outside area */
/** @} */

/** \defgroup pvr_colclamp_switch   Enable or disable color clamping

    Enabling color clamping will clamp colors between the minimum and maximum
    values before any sort of fog processing.

    @{
*/
#define PVR_CLRCLAMP_DISABLE    0   /**< \brief Disable color clamping */
#define PVR_CLRCLAMP_ENABLE     1   /**< \brief Enable color clamping */
/** @} */

/** \defgroup pvr_offset_switch     Enable or disable offset color

    Enabling offset color calculation allows for "specular" like effects on a
    per-vertex basis, by providing an additive color in the calculation of the
    final pixel colors. In vertex types with a "oargb" parameter, that's what it
    is for.

    Note that this must be enabled for bumpmap polygons in order to allow you to
    specify the parameters in the oargb field of the vertices.

    @{
*/
#define PVR_SPECULAR_DISABLE    0   /**< \brief Disable offset colors */
#define PVR_SPECULAR_ENABLE     1   /**< \brief Enable offset colors */
/** @} */

/** \defgroup pvr_alpha_switch      Enable or disable alpha blending

    This causes the alpha value in the vertex color to be paid attention to. It
    really only makes sense to enable this for translucent or punch-thru polys.

    @{
*/
#define PVR_ALPHA_DISABLE       0   /**< \brief Disable alpha blending */
#define PVR_ALPHA_ENABLE        1   /**< \brief Enable alpha blending */
/** @} */

/** \defgroup pvr_txralpha_switch   Enable or disable texture alpha blending

    This causes the alpha value in the texel color to be paid attention to. It
    really only makes sense to enable this for translucent or punch-thru polys.

    @{
*/
#define PVR_TXRALPHA_ENABLE     0   /**< \brief Enable alpha blending */
#define PVR_TXRALPHA_DISABLE    1   /**< \brief Disable alpha blending */
/** @} */

/** \defgroup pvr_uv_flip           Enable or disable U/V flipping on the PVR

    These flags determine what happens when U/V coordinate values exceed 1.0.
    In any of the flipped cases, the specified coordinate value will flip around
    after 1.0, essentially mirroring the image. So, if you displayed an image
    with a U coordinate of 0.0 on the left hand side and 2.0 on the right hand
    side with U flipping turned on, you'd have an image that was displayed twice
    as if mirrored across the middle. This mirroring behavior happens at every
    unit boundary (so at 2.0 it returns to normal, at 3.0 it flips, etc).

    The default case is to disable mirroring. In addition, clamping of the U/V
    coordinates by PVR_UVCLAMP_U, PVR_UVCLAMP_V, or PVR_UVCLAMP_UV will disable
    the mirroring behavior.
    @{
*/
#define PVR_UVFLIP_NONE         0   /**< \brief No flipped coordinates */
#define PVR_UVFLIP_V            1   /**< \brief Flip V only */
#define PVR_UVFLIP_U            2   /**< \brief Flip U only */
#define PVR_UVFLIP_UV           3   /**< \brief Flip U and V */
/** @} */

/** \defgroup pvr_uv_clamp          Enable or disable clamping of U/V on the PVR

    These flags determine whether clamping will be applied to U/V coordinate
    values that exceed 1.0. If enabled, these modes will explicitly override the
    flip/mirroring modes (PVR_UVFLIP_U, PVR_UVFLIP_V, and PVR_UVFLIP_UV), and
    will instead ensure that the coordinate(s) in question never exceed 1.0.
    @{
*/
#define PVR_UVCLAMP_NONE        0   /**< \brief Disable clamping */
#define PVR_UVCLAMP_V           1   /**< \brief Clamp V only */
#define PVR_UVCLAMP_U           2   /**< \brief Clamp U only */
#define PVR_UVCLAMP_UV          3   /**< \brief Clamp U and V */
/** @} */

/** \defgroup pvr_filter_modes      PVR texture sampling modes
    @{
*/
#define PVR_FILTER_NONE         0   /**< \brief No filtering (point sample) */
#define PVR_FILTER_NEAREST      0   /**< \brief No filtering (point sample) */
#define PVR_FILTER_BILINEAR     2   /**< \brief Bilinear interpolation */
#define PVR_FILTER_TRILINEAR1   4   /**< \brief Trilinear interpolation pass 1 */
#define PVR_FILTER_TRILINEAR2   6   /**< \brief Trilinear interpolation pass 2 */
/** @} */

/** \defgroup pvr_mip_bias          PVR mipmap bias modes
    @{
*/
#define PVR_MIPBIAS_NORMAL      PVR_MIPBIAS_1_00    /* txr_mipmap_bias */
#define PVR_MIPBIAS_0_25        1
#define PVR_MIPBIAS_0_50        2
#define PVR_MIPBIAS_0_75        3
#define PVR_MIPBIAS_1_00        4
#define PVR_MIPBIAS_1_25        5
#define PVR_MIPBIAS_1_50        6
#define PVR_MIPBIAS_1_75        7
#define PVR_MIPBIAS_2_00        8
#define PVR_MIPBIAS_2_25        9
#define PVR_MIPBIAS_2_50        10
#define PVR_MIPBIAS_2_75        11
#define PVR_MIPBIAS_3_00        12
#define PVR_MIPBIAS_3_25        13
#define PVR_MIPBIAS_3_50        14
#define PVR_MIPBIAS_3_75        15
/** @} */

/** \defgroup pvr_txrenv_modes      Texture color calculation modes
    @{
*/
#define PVR_TXRENV_REPLACE          0   /**< \brief C = Ct, A = At */
#define PVR_TXRENV_MODULATE         1   /**< \brief  C = Cs * Ct, A = At */
#define PVR_TXRENV_DECAL            2   /**< \brief C = (Cs * At) + (Cs * (1-At)), A = As */
#define PVR_TXRENV_MODULATEALPHA    3   /**< \brief C = Cs * Ct, A = As * At */
/** @} */

/** \defgroup pvr_mip_switch        Enable or disable PVR mipmap processing
    @{
*/
#define PVR_MIPMAP_DISABLE      0   /**< \brief Disable mipmap processing */
#define PVR_MIPMAP_ENABLE       1   /**< \brief Enable mipmap processing */
/** @} */

/** \defgroup pvr_txr_fmts          PVR texture formats

    These are the texture formats that the PVR supports. Note that some of
    these, you can OR together with other values.

    @{
*/
#define PVR_TXRFMT_NONE         0           /**< \brief No texture */
#define PVR_TXRFMT_VQ_DISABLE   (0 << 30)   /**< \brief Not VQ encoded */
#define PVR_TXRFMT_VQ_ENABLE    (1 << 30)   /**< \brief VQ encoded */
#define PVR_TXRFMT_ARGB1555     (0 << 27)   /**< \brief 16-bit ARGB1555 */
#define PVR_TXRFMT_RGB565       (1 << 27)   /**< \brief 16-bit RGB565 */
#define PVR_TXRFMT_ARGB4444     (2 << 27)   /**< \brief 16-bit ARGB4444 */
#define PVR_TXRFMT_YUV422       (3 << 27)   /**< \brief YUV422 format */
#define PVR_TXRFMT_BUMP         (4 << 27)   /**< \brief Bumpmap format */
#define PVR_TXRFMT_PAL4BPP      (5 << 27)   /**< \brief 4BPP paletted format */
#define PVR_TXRFMT_PAL8BPP      (6 << 27)   /**< \brief 8BPP paletted format */
#define PVR_TXRFMT_TWIDDLED     (0 << 26)   /**< \brief Texture is twiddled */
#define PVR_TXRFMT_NONTWIDDLED  (1 << 26)   /**< \brief Texture is not twiddled */
#define PVR_TXRFMT_NOSTRIDE     (0 << 21)   /**< \brief Texture is not strided */
#define PVR_TXRFMT_STRIDE       (1 << 21)   /**< \brief Texture is strided */

/* OR one of these into your texture format if you need it. Note that
   these coincide with the twiddled/stride bits, so you can't have a
   non-twiddled/strided texture that's paletted! */
/** \brief  8BPP palette selector
    \param  x               The palette index */
#define PVR_TXRFMT_8BPP_PAL(x)  ((x) << 25)

/** \brief 4BPP palette selector
    \param  x               The palette index */
#define PVR_TXRFMT_4BPP_PAL(x)  ((x) << 21)
/** @} */

/** \defgroup pvr_color_fmts        PVR vertex color formats

    These control how colors are represented in polygon data.

    @{
*/
#define PVR_CLRFMT_ARGBPACKED       0   /**< \brief 32-bit integer ARGB */
#define PVR_CLRFMT_4FLOATS          1   /**< \brief 4 floating point values */
#define PVR_CLRFMT_INTENSITY        2   /**< \brief Intensity color */
#define PVR_CLRFMT_INTENSITY_PREV   3   /**< \brief Use last intensity */
/** @} */

/** \defgroup pvr_uv_fmts           PVR U/V data format control
    @{
*/
#define PVR_UVFMT_32BIT         0   /**< \brief 32-bit floating point U/V */
#define PVR_UVFMT_16BIT         1   /**< \brief 16-bit floating point U/V */
/** @} */

/** \defgroup pvr_mod_switch        Enable or disable modifier effects
    @{
*/
#define PVR_MODIFIER_DISABLE    0   /**< \brief Disable modifier effects */
#define PVR_MODIFIER_ENABLE     1   /**< \brief Enable modifier effects */
/** @} */

#define PVR_MODIFIER_CHEAP_SHADOW   0
#define PVR_MODIFIER_NORMAL     1

/** \defgroup pvr_mod_modes         Modifier volume mode parameters

    All triangles in a single modifier volume should be of the other poly type,
    except for the last one. That should be either of the other two types,
    depending on whether you want an inclusion or exclusion volume.

    @{
*/
#define PVR_MODIFIER_OTHER_POLY         0   /**< \brief Not the last polygon in the volume */
#define PVR_MODIFIER_INCLUDE_LAST_POLY  1   /**< \brief Last polygon, inclusion volume */
#define PVR_MODIFIER_EXCLUDE_LAST_POLY  2   /**< \brief Last polygon, exclusion volume */
/** @} */


/** \brief  PVR polygon header.

    This is the hardware equivalent of a rendering context; you'll create one of
    these from your pvr_poly_cxt_t and use it for submission to the hardware.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  cmd;                /**< \brief TA command */
    uint32  mode1;              /**< \brief Parameter word 1 */
    uint32  mode2;              /**< \brief Parameter word 2 */
    uint32  mode3;              /**< \brief Parameter word 3 */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
    uint32  d3;                 /**< \brief Dummy value */
    uint32  d4;                 /**< \brief Dummy value */
} pvr_poly_hdr_t;

/** \brief  PVR polygon header with intensity color.

    This is the equivalent of pvr_poly_hdr_t, but for use with intensity color.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  cmd;                /**< \brief TA command */
    uint32  mode1;              /**< \brief Parameter word 1 */
    uint32  mode2;              /**< \brief Parameter word 2 */
    uint32  mode3;              /**< \brief Parameter word 3 */
    float   a;                  /**< \brief Face color alpha component */
    float   r;                  /**< \brief Face color red component */
    float   g;                  /**< \brief Face color green component */
    float   b;                  /**< \brief Face color blue component */
} pvr_poly_ic_hdr_t;

/** \brief  PVR polygon header to be used with modifier volumes.

    This is the equivalent of a pvr_poly_hdr_t for use when a polygon is to be
    used with modifier volumes.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  cmd;                /**< \brief TA command */
    uint32  mode1;              /**< \brief Parameter word 1 */
    uint32  mode2_0;            /**< \brief Parameter word 2 (outside volume) */
    uint32  mode3_0;            /**< \brief Parameter word 3 (outside volume) */
    uint32  mode2_1;            /**< \brief Parameter word 2 (inside volume) */
    uint32  mode3_1;            /**< \brief Parameter word 3 (inside volume) */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
} pvr_poly_mod_hdr_t;

/** \brief  PVR polygon header specifically for sprites.

    This is the equivalent of a pvr_poly_hdr_t for use when a quad/sprite is to
    be rendered. Note that the color data is here, not in the vertices.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  cmd;                /**< \brief TA command */
    uint32  mode1;              /**< \brief Parameter word 1 */
    uint32  mode2;              /**< \brief Parameter word 2 */
    uint32  mode3;              /**< \brief Parameter word 3 */
    uint32  argb;               /**< \brief Sprite face color */
    uint32  oargb;              /**< \brief Sprite offset color */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
} pvr_sprite_hdr_t;

/** \brief  Modifier volume header.

    This is the header that should be submitted when dealing with setting a
    modifier volume.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  cmd;                /**< \brief TA command */
    uint32  mode1;              /**< \brief Parameter word 1 */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
    uint32  d3;                 /**< \brief Dummy value */
    uint32  d4;                 /**< \brief Dummy value */
    uint32  d5;                 /**< \brief Dummy value */
    uint32  d6;                 /**< \brief Dummy value */
} pvr_mod_hdr_t;

/** \brief  Generic PVR vertex type.

    The PVR chip itself supports many more vertex types, but this is the main
    one that can be used with both textured and non-textured polygons, and is
    fairly fast.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  flags;              /**< \brief TA command (vertex flags) */
    float   x;                  /**< \brief X coordinate */
    float   y;                  /**< \brief Y coordinate */
    float   z;                  /**< \brief Z coordinate */
    float   u;                  /**< \brief Texture U coordinate */
    float   v;                  /**< \brief Texture V coordinate */
    uint32  argb;               /**< \brief Vertex color */
    uint32  oargb;              /**< \brief Vertex offset color */
} pvr_vertex_t;

/** \brief  PVR vertex type: Non-textured, packed color, affected by modifier
            volume.

    This vertex type has two copies of colors. The second color is used when
    enclosed within a modifier volume.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  flags;              /**< \brief TA command (vertex flags) */
    float   x;                  /**< \brief X coordinate */
    float   y;                  /**< \brief Y coordinate */
    float   z;                  /**< \brief Z coordinate */
    uint32  argb0;              /**< \brief Vertex color (outside volume) */
    uint32  argb1;              /**< \brief Vertex color (inside volume) */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
} pvr_vertex_pcm_t;

/** \brief  PVR vertex type: Textured, packed color, affected by modifer volume.

    Note that this vertex type has two copies of colors, offset colors, and
    texture coords. The second set of texture coords, colors, and offset colors
    are used when enclosed within a modifer volume.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  flags;              /**< \brief TA command (vertex flags) */
    float   x;                  /**< \brief X coordinate */
    float   y;                  /**< \brief Y coordinate */
    float   z;                  /**< \brief Z coordinate */
    float   u0;                 /**< \brief Texture U coordinate (outside) */
    float   v0;                 /**< \brief Texture V coordinate (outside) */
    uint32  argb0;              /**< \brief Vertex color (outside) */
    uint32  oargb0;             /**< \brief Vertex offset color (outside) */
    float   u1;                 /**< \brief Texture U coordinate (inside) */
    float   v1;                 /**< \brief Texture V coordinate (inside) */
    uint32  argb1;              /**< \brief Vertex color (inside) */
    uint32  oargb1;             /**< \brief Vertex offset color (inside) */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
    uint32  d3;                 /**< \brief Dummy value */
    uint32  d4;                 /**< \brief Dummy value */
} pvr_vertex_tpcm_t;

/** \brief  PVR vertex type: Textured sprite.

    This vertex type is to be used with the sprite polygon header and the sprite
    related commands to draw textured sprites. Note that there is no fourth Z
    coordinate. I suppose it just gets interpolated?

    The U/V coordinates in here are in the 16-bit per coordinate form. Also,
    like the fourth Z value, there is no fourth U or V, so it must get
    interpolated from the others.

    \headerfile dc/pvr.h
*/
typedef struct {
    uint32  flags;              /**< \brief TA command (vertex flags) */
    float   ax;                 /**< \brief First X coordinate */
    float   ay;                 /**< \brief First Y coordinate */
    float   az;                 /**< \brief First Z coordinate */
    float   bx;                 /**< \brief Second X coordinate */
    float   by;                 /**< \brief Second Y coordinate */
    float   bz;                 /**< \brief Second Z coordinate */
    float   cx;                 /**< \brief Third X coordinate */
    float   cy;                 /**< \brief Third Y coordinate */
    float   cz;                 /**< \brief Third Z coordinate */
    float   dx;                 /**< \brief Fourth X coordinate */
    float   dy;                 /**< \brief Fourth Y coordinate */
    uint32 dummy;               /**< \brief Dummy value */
    uint32 auv;                 /**< \brief First U/V texture coordinates */
    uint32 buv;                 /**< \brief Second U/V texture coordinates */
    uint32 cuv;                 /**< \brief Third U/V texture coordinates */
} pvr_sprite_txr_t;

/** \brief  PVR vertex type: Untextured sprite.

    This vertex type is to be used with the sprite polygon header and the sprite
    related commands to draw untextured sprites (aka, quads).
*/
typedef struct {
    uint32  flags;              /**< \brief TA command (vertex flags) */
    float   ax;                 /**< \brief First X coordinate */
    float   ay;                 /**< \brief First Y coordinate */
    float   az;                 /**< \brief First Z coordinate */
    float   bx;                 /**< \brief Second X coordinate */
    float   by;                 /**< \brief Second Y coordinate */
    float   bz;                 /**< \brief Second Z coordinate */
    float   cx;                 /**< \brief Third X coordinate */
    float   cy;                 /**< \brief Third Y coordinate */
    float   cz;                 /**< \brief Third Z coordinate */
    float   dx;                 /**< \brief Fourth X coordinate */
    float   dy;                 /**< \brief Fourth Y coordinate */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
    uint32  d3;                 /**< \brief Dummy value */
    uint32  d4;                 /**< \brief Dummy value */
} pvr_sprite_col_t;

/** \brief  PVR vertex type: Modifier volume.

    This vertex type is to be used with the modifier volume header to specify
    triangular modifier areas.
*/
typedef struct {
    uint32  flags;              /**< \brief TA command (vertex flags) */
    float   ax;                 /**< \brief First X coordinate */
    float   ay;                 /**< \brief First Y coordinate */
    float   az;                 /**< \brief First Z coordinate */
    float   bx;                 /**< \brief Second X coordinate */
    float   by;                 /**< \brief Second Y coordinate */
    float   bz;                 /**< \brief Second Z coordinate */
    float   cx;                 /**< \brief Third X coordinate */
    float   cy;                 /**< \brief Third Y coordinate */
    float   cz;                 /**< \brief Third Z coordinate */
    uint32  d1;                 /**< \brief Dummy value */
    uint32  d2;                 /**< \brief Dummy value */
    uint32  d3;                 /**< \brief Dummy value */
    uint32  d4;                 /**< \brief Dummy value */
    uint32  d5;                 /**< \brief Dummy value */
    uint32  d6;                 /**< \brief Dummy value */
} pvr_modifier_vol_t;

/** \brief  Pack four floating point color values into a 32-bit integer form.

    All of the color values should be between 0 and 1.

    \param  a               Alpha value
    \param  r               Red value
    \param  g               Green value
    \param  b               Blue value
    \return                 The packed color value
*/
#define PVR_PACK_COLOR(a, r, g, b) ( \
                                     ( ((uint8)( a * 255 ) ) << 24 ) | \
                                     ( ((uint8)( r * 255 ) ) << 16 ) | \
                                     ( ((uint8)( g * 255 ) ) << 8 ) | \
                                     ( ((uint8)( b * 255 ) ) << 0 ) )

/** \brief  Pack two floating point coordinates into one 32-bit value,
            truncating them to 16-bits each.

    \param  u               First coordinate to pack
    \param  v               Second coordinate to pack
    \return                 The packed coordinates
*/
static inline uint32 PVR_PACK_16BIT_UV(float u, float v) {
    union {
        float f;
        uint32 i;
    } u2, v2;

    u2.f = u;
    v2.f = v;

    return (u2.i & 0xFFFF0000) | (v2.i >> 16);
}

/** \defgroup pvr_commands          TA command values

    These are are appropriate values for TA commands. Use whatever goes with the
    primitive type you're using.

    @{
*/
#define PVR_CMD_POLYHDR     0x80840000  /**< \brief PVR polygon header.
Striplength set to 2 */
#define PVR_CMD_VERTEX      0xe0000000  /**< \brief PVR vertex data */
#define PVR_CMD_VERTEX_EOL  0xf0000000  /**< \brief PVR vertex, end of strip */
#define PVR_CMD_USERCLIP    0x20000000  /**< \brief PVR user clipping area */
#define PVR_CMD_MODIFIER    0x80000000  /**< \brief PVR modifier volume */
#define PVR_CMD_SPRITE      0xA0000000  /**< \brief PVR sprite header */
/** @} */

/** \defgroup pvr_bitmasks          Constants and bitmasks for handling polygon
                                    headers.

    Note that thanks to the arrangement of constants, this is mainly a matter of
    bit shifting to compile headers...

    @{
*/
#define PVR_TA_CMD_TYPE_SHIFT       24
#define PVR_TA_CMD_TYPE_MASK        (7 << PVR_TA_CMD_TYPE_SHIFT)

#define PVR_TA_CMD_USERCLIP_SHIFT   16
#define PVR_TA_CMD_USERCLIP_MASK    (3 << PVR_TA_CMD_USERCLIP_SHIFT)

#define PVR_TA_CMD_CLRFMT_SHIFT     4
#define PVR_TA_CMD_CLRFMT_MASK      (7 << PVR_TA_CMD_CLRFMT_SHIFT)

#define PVR_TA_CMD_SPECULAR_SHIFT   2
#define PVR_TA_CMD_SPECULAR_MASK    (1 << PVR_TA_CMD_SPECULAR_SHIFT)

#define PVR_TA_CMD_SHADE_SHIFT      1
#define PVR_TA_CMD_SHADE_MASK       (1 << PVR_TA_CMD_SHADE_SHIFT)

#define PVR_TA_CMD_UVFMT_SHIFT      0
#define PVR_TA_CMD_UVFMT_MASK       (1 << PVR_TA_CMD_UVFMT_SHIFT)

#define PVR_TA_CMD_MODIFIER_SHIFT   7
#define PVR_TA_CMD_MODIFIER_MASK    (1 <<  PVR_TA_CMD_MODIFIER_SHIFT)

#define PVR_TA_CMD_MODIFIERMODE_SHIFT   6
#define PVR_TA_CMD_MODIFIERMODE_MASK    (1 <<  PVR_TA_CMD_MODIFIERMODE_SHIFT)

#define PVR_TA_PM1_DEPTHCMP_SHIFT   29
#define PVR_TA_PM1_DEPTHCMP_MASK    (7 << PVR_TA_PM1_DEPTHCMP_SHIFT)

#define PVR_TA_PM1_CULLING_SHIFT    27
#define PVR_TA_PM1_CULLING_MASK     (3 << PVR_TA_PM1_CULLING_SHIFT)

#define PVR_TA_PM1_DEPTHWRITE_SHIFT 26
#define PVR_TA_PM1_DEPTHWRITE_MASK  (1 << PVR_TA_PM1_DEPTHWRITE_SHIFT)

#define PVR_TA_PM1_TXRENABLE_SHIFT  25
#define PVR_TA_PM1_TXRENABLE_MASK   (1 << PVR_TA_PM1_TXRENABLE_SHIFT)

#define PVR_TA_PM1_MODIFIERINST_SHIFT   29
#define PVR_TA_PM1_MODIFIERINST_MASK    (3 <<  PVR_TA_PM1_MODIFIERINST_SHIFT)

#define PVR_TA_PM2_SRCBLEND_SHIFT   29
#define PVR_TA_PM2_SRCBLEND_MASK    (7 << PVR_TA_PM2_SRCBLEND_SHIFT)

#define PVR_TA_PM2_DSTBLEND_SHIFT   26
#define PVR_TA_PM2_DSTBLEND_MASK    (7 << PVR_TA_PM2_DSTBLEND_SHIFT)

#define PVR_TA_PM2_SRCENABLE_SHIFT  25
#define PVR_TA_PM2_SRCENABLE_MASK   (1 << PVR_TA_PM2_SRCENABLE_SHIFT)

#define PVR_TA_PM2_DSTENABLE_SHIFT  24
#define PVR_TA_PM2_DSTENABLE_MASK   (1 << PVR_TA_PM2_DSTENABLE_SHIFT)

#define PVR_TA_PM2_FOG_SHIFT        22
#define PVR_TA_PM2_FOG_MASK     (3 << PVR_TA_PM2_FOG_SHIFT)

#define PVR_TA_PM2_CLAMP_SHIFT      21
#define PVR_TA_PM2_CLAMP_MASK       (1 << PVR_TA_PM2_CLAMP_SHIFT)

#define PVR_TA_PM2_ALPHA_SHIFT      20
#define PVR_TA_PM2_ALPHA_MASK       (1 << PVR_TA_PM2_ALPHA_SHIFT)

#define PVR_TA_PM2_TXRALPHA_SHIFT   19
#define PVR_TA_PM2_TXRALPHA_MASK    (1 << PVR_TA_PM2_TXRALPHA_SHIFT)

#define PVR_TA_PM2_UVFLIP_SHIFT     17
#define PVR_TA_PM2_UVFLIP_MASK      (3 << PVR_TA_PM2_UVFLIP_SHIFT)

#define PVR_TA_PM2_UVCLAMP_SHIFT    15
#define PVR_TA_PM2_UVCLAMP_MASK     (3 << PVR_TA_PM2_UVCLAMP_SHIFT)

#define PVR_TA_PM2_FILTER_SHIFT     12
#define PVR_TA_PM2_FILTER_MASK      (7 << PVR_TA_PM2_FILTER_SHIFT)

#define PVR_TA_PM2_MIPBIAS_SHIFT    8
#define PVR_TA_PM2_MIPBIAS_MASK     (15 << PVR_TA_PM2_MIPBIAS_SHIFT)

#define PVR_TA_PM2_TXRENV_SHIFT     6
#define PVR_TA_PM2_TXRENV_MASK      (3 << PVR_TA_PM2_TXRENV_SHIFT)

#define PVR_TA_PM2_USIZE_SHIFT      3
#define PVR_TA_PM2_USIZE_MASK       (7 << PVR_TA_PM2_USIZE_SHIFT)

#define PVR_TA_PM2_VSIZE_SHIFT      0
#define PVR_TA_PM2_VSIZE_MASK       (7 << PVR_TA_PM2_VSIZE_SHIFT)

#define PVR_TA_PM3_MIPMAP_SHIFT     31
#define PVR_TA_PM3_MIPMAP_MASK      (1 << PVR_TA_PM3_MIPMAP_SHIFT)

#define PVR_TA_PM3_TXRFMT_SHIFT     0
#define PVR_TA_PM3_TXRFMT_MASK      0xffffffff
/** @} */

/**** Register macros ***************************************************/

/* We use these macros to do all PVR register access, so that it's
   simple later on to hook them for debugging or whatnot. */

/** \brief  Retrieve a PVR register value
    \param  REG             The register to fetch
    \return                 The value of that register (32-bits)
*/
#define PVR_GET(REG) (* ( (vuint32*)( 0xa05f8000 + (REG) ) ) )

/** \brief  Set a PVR register value
    \param  REG             The register to set
    \param  VALUE           The value to set in the register (32-bits)
*/
#define PVR_SET(REG, VALUE) PVR_GET(REG) = (VALUE)

/* The registers themselves; these are from Maiwe's powervr-reg.txt */
/* Note that 2D specific registers have been excluded for now (like
   vsync, hsync, v/h size, etc) */

/** \defgroup pvr_regs              Offsets to registers of the PVR
    @{
*/
#define PVR_ID                  0x0000  /**< \brief Chip ID */
#define PVR_REVISION            0x0004  /**< \brief Chip revision */
#define PVR_RESET               0x0008  /**< \brief Reset pins */
#define PVR_ISP_START           0x0014  /**< \brief Start the ISP/TSP */
#define PVR_UNK_0018            0x0018  /**< \brief ?? */
#define PVR_ISP_VERTBUF_ADDR    0x0020  /**< \brief Vertex buffer address for scene rendering */
#define PVR_ISP_TILEMAT_ADDR    0x002c  /**< \brief Tile matrix address for scene rendering */
#define PVR_SPANSORT_CFG        0x0030  /**< \brief ?? -- write 0x101 for now */
#define PVR_FB_CFG_1            0x0044  /**< \brief Framebuffer config 1 */
#define PVR_FB_CFG_2            0x0048  /**< \brief Framebuffer config 2 */
#define PVR_RENDER_MODULO       0x004c  /**< \brief Render modulo */
#define PVR_RENDER_ADDR         0x0060  /**< \brief Render output address */
#define PVR_RENDER_ADDR_2       0x0064  /**< \brief Output for strip-buffering */
#define PVR_PCLIP_X             0x0068  /**< \brief Horizontal clipping area */
#define PVR_PCLIP_Y             0x006c  /**< \brief Vertical clipping area */
#define PVR_CHEAP_SHADOW        0x0074  /**< \brief Cheap shadow control */
#define PVR_OBJECT_CLIP         0x0078  /**< \brief Distance for polygon culling */
#define PVR_UNK_007C            0x007c  /**< \brief ?? -- write 0x0027df77 for now */
#define PVR_UNK_0080            0x0080  /**< \brief ?? -- write 7 for now */
#define PVR_TEXTURE_CLIP        0x0084  /**< \brief Distance for texture clipping */
#define PVR_BGPLANE_Z           0x0088  /**< \brief Distance for background plane */
#define PVR_BGPLANE_CFG         0x008c  /**< \brief Background plane config */
#define PVR_UNK_0098            0x0098  /**< \brief ?? -- write 0x00800408 for now */
#define PVR_UNK_00A0            0x00a0  /**< \brief ?? -- write 0x20 for now */
#define PVR_UNK_00A8            0x00a8  /**< \brief ?? -- write 0x15d1c951 for now */
#define PVR_FOG_TABLE_COLOR     0x00b0  /**< \brief Table fog color */
#define PVR_FOG_VERTEX_COLOR    0x00b4  /**< \brief Vertex fog color */
#define PVR_FOG_DENSITY         0x00b8  /**< \brief Fog density coefficient */
#define PVR_COLOR_CLAMP_MAX     0x00bc  /**< \brief RGB Color clamp max */
#define PVR_COLOR_CLAMP_MIN     0x00c0  /**< \brief RGB Color clamp min */
#define PVR_GUN_POS             0x00c4  /**< \brief Light gun position */
#define PVR_UNK_00C8            0x00c8  /**< \brief ?? -- write same as border H in 00d4 << 16 */
#define PVR_VPOS_IRQ            0x00cc  /**< \brief Vertical position IRQ */
#define PVR_TEXTURE_MODULO      0x00e4  /**< \brief Output texture width modulo */
#define PVR_VIDEO_CFG           0x00e8  /**< \brief Misc video config */
#define PVR_SCALER_CFG          0x00f4  /**< \brief Smoothing scaler */
#define PVR_PALETTE_CFG         0x0108  /**< \brief Palette format */
#define PVR_SYNC_STATUS         0x010c  /**< \brief V/H blank status */
#define PVR_UNK_0110            0x0110  /**< \brief ?? -- write 0x93f39 for now */
#define PVR_UNK_0114            0x0114  /**< \brief ?? -- write 0x200000 for now */
#define PVR_UNK_0118            0x0118  /**< \brief ?? -- write 0x8040 for now */
#define PVR_TA_OPB_START        0x0124  /**< \brief Object Pointer Buffer start for TA usage */
#define PVR_TA_VERTBUF_START    0x0128  /**< \brief Vertex buffer start for TA usage */
#define PVR_TA_OPB_END          0x012c  /**< \brief OPB end for TA usage */
#define PVR_TA_VERTBUF_END      0x0130  /**< \brief Vertex buffer end for TA usage */
#define PVR_TA_OPB_POS          0x0134  /**< \brief Top used memory location in OPB for TA usage */
#define PVR_TA_VERTBUF_POS      0x0138  /**< \brief Top used memory location in vertbuf for TA usage */
#define PVR_TILEMAT_CFG         0x013c  /**< \brief Tile matrix size config */
#define PVR_OPB_CFG             0x0140  /**< \brief Active lists / list size */
#define PVR_TA_INIT             0x0144  /**< \brief Initialize vertex reg. params */
#define PVR_YUV_ADDR            0x0148  /**< \brief YUV conversion destination */
#define PVR_YUV_CFG_1           0x014c  /**< \brief YUV configuration */
#define PVR_UNK_0160            0x0160  /**< \brief ?? */
#define PVR_TA_OPB_INIT         0x0164  /**< \brief Object pointer buffer position init */
#define PVR_FOG_TABLE_BASE      0x0200  /**< \brief Base of the fog table */
#define PVR_PALETTE_TABLE_BASE  0x1000  /**< \brief Base of the palette table */
/** @} */

/* Useful memory locations */
#define PVR_TA_INPUT        0x10000000  /**< \brief TA command input */
#define PVR_RAM_BASE        0xa5000000  /**< \brief PVR RAM (raw) */
#define PVR_RAM_INT_BASE    0xa4000000  /**< \brief PVR RAM (interleaved) */

#define PVR_RAM_SIZE        (8*1024*1024)   /**< \brief RAM size in bytes */

#define PVR_RAM_TOP         (PVR_RAM_BASE + PVR_RAM_SIZE)       /**< \brief Top of raw PVR RAM */
#define PVR_RAM_INT_TOP     (PVR_RAM_INT_BASE + PVR_RAM_SIZE)   /**< \brief Top of int PVR RAM */

/* Register content defines, as needed; these will be filled in over time
   as the implementation requires them. There's too many to do otherwise. */

/** \defgroup pvr_reset_vals        Values used to reset parts of the PVR

    These values are written to the PVR_RESET register in order to reset the
    system or to take it out of reset.

    @{
*/
#define PVR_RESET_ALL       0xffffffff  /**< \brief Reset the wole PVR */
#define PVR_RESET_NONE      0x00000000  /**< \brief Cancel reset state */
#define PVR_RESET_TA        0x00000001  /**< \brief Reset only the TA */
#define PVR_RESET_ISPTSP    0x00000002  /**< \brief Reset only the ISP/TSP */
/** @} */

#define PVR_ISP_START_GO    0xffffffff  /**< \brief Write to the PVR_ISP_START register to start rendering */

#define PVR_TA_INIT_GO      0x80000000  /**< \brief Write to the PVR_TA_INIT register to confirm settings */


/* Initialization ****************************************************/

/* Initialization and shutdown: stuff you should only ever have to do
   once in your program. */

/** \defgroup pvr_binsizes          Available sizes for primitive bins
    @{
*/
#define PVR_BINSIZE_0   0   /**< \brief 0-length (disables the list) */
#define PVR_BINSIZE_8   8   /**< \brief 8-word (32-byte) length */
#define PVR_BINSIZE_16  16  /**< \brief 16-word (64-byte) length */
#define PVR_BINSIZE_32  32  /**< \brief 32-word (128-byte) length */
/** @} */

/** \brief  PVR initialization structure

    This structure defines how the PVR initializes various parts of the system,
    including the primitive bin sizes, the vertex buffer size, and whether
    vertex DMA will be enabled.

    You essentially fill one of these in, and pass it to pvr_init().

    \headerfile dc/pvr.h
*/
typedef struct {
    /** \brief  Bin sizes.

        The bins go in the following order: opaque polygons, opaque modifiers,
        translucent polygons, translucent modifiers, punch-thrus
    */
    int     opb_sizes[5];

    /** \brief  Vertex buffer size (should be a nice round number) */
    int     vertex_buf_size;

    /** \brief  Enable vertex DMA?

        Set to non-zero if we want to enable vertex DMA mode. Note that if this
        is set, then _all_ enabled lists need to have a vertex buffer assigned,
        even if you never use that list for anything.
    */
    int     dma_enabled;

    /** \brief  Enable horizontal scaling?

        Set to non-zero if horizontal scaling is to be enabled. By enabling this
        setting and stretching your image to double the native screen width, you
        can get horizontal full-screen anti-aliasing. */
    int     fsaa_enabled;

    /** \brief  Disable translucent polygon autosort?

        Set to non-zero to disable translucent polygon autosorting. By enabling
        this setting, the PVR acts more like a traditional Z-buffered system
        when rendering translucent polygons, meaning you must pre-sort them
        yourself if you want them to appear in the right order. */
    int     autosort_disabled;

} pvr_init_params_t;

/** \brief  Initialize the PVR chip to ready status.

    This function enables the specified lists and uses the specified parameters.
    Note that bins and vertex buffers come from the texture memory pool, so only
    allocate what you actually need. Expects that a 2D mode was initialized
    already using the vid_* API.

    \param  params          The set of parameters to initialize with
    \retval 0               On success
    \retval -1              If the PVR has already been initialized or the video
                            mode active is not suitable for 3D
*/
int pvr_init(pvr_init_params_t *params);

/** \brief  Simple PVR initialization.

    This simpler function initializes the PVR using 16/16 for the opaque
    and translucent lists' bin sizes, and 0's for everything else. It sets 512KB
    of vertex buffer. This is equivalent to the old ta_init_defaults() for now.

    \retval 0               On success
    \retval -1              If the PVR has already been initialized or the video
                            mode active is not suitable for 3D
*/
int pvr_init_defaults();

/** \brief  Shut down the PVR chip from ready status.

    This essentially leaves the video system in 2D mode as it was before the
    init.

    \retval 0               On success
    \retval -1              If the PVR has not been initialized
*/
int pvr_shutdown();


/* Misc parameters ***************************************************/

/* These are miscellaneous parameters you can set which affect the
   rendering process. */

/** \brief  Set the background plane color.

    This function sets the color of the area of the screen not covered by any
    other polygons.

    \param  r               Red component of the color to set
    \param  g               Green component of the color to set
    \param  b               Blue component of the color to set
*/
void pvr_set_bg_color(float r, float g, float b);

/** \brief  Set cheap shadow parameters.

    This function sets up the PVR cheap shadow parameters for use. You can only
    specify one scale value per frame, so the effect that you can get from this
    is somewhat limited, but if you want simple shadows, this is the easiest way
    to do it.

    Polygons affected by a shadow modifier volume will effectively multiply
    their final color by the scale value set here when shadows are enabled and
    the polygon is inside the modifier (or outside for exclusion volumes).

    \param  enable          Set to non-zero to enable cheap shadow mode.
    \param  scale_value     Floating point value (between 0 and 1) representing
                            how colors of polygons affected by and inside the
                            volume will be modified by the shadow volume.
*/
void pvr_set_shadow_scale(int enable, float scale_value);

/** \brief  Retrieve the current VBlank count.

    This function retrieves the number of VBlank interrupts that have occurred
    since the PVR was initialized.

    \return                 The number of VBlanks since init
*/
int pvr_get_vbl_count();

/* Statistics structure */
/** \brief  PVR statistics structure.

    This structure is used to hold various statistics about the operation of the
    PVR since initialization.

    \headerfile dc/pvr.h
*/
typedef struct pvr_stats {
    uint32  enabled_list_mask;  /**< \brief Which lists are enabled? */
    uint32  vbl_count;          /**< \brief VBlank count */
    int     frame_last_time;    /**< \brief Ready-to-Ready length for the last frame in milliseconds */
    float   frame_rate;         /**< \brief Current frame rate (per second) */
    int     reg_last_time;      /**< \brief Registration time for the last frame in milliseconds */
    int     rnd_last_time;      /**< \brief Rendering time for the last frame in milliseconds */
    int     vtx_buffer_used;    /**< \brief Number of bytes used in the vertex buffer for the last frame */
    int     vtx_buffer_used_max;/**< \brief Number of bytes used in the vertex buffer for the largest frame */
    int     buf_last_time;      /**< \brief DMA buffer file time for the last frame in milliseconds */
    uint32  frame_count;        /**< \brief Total number of rendered/viewed frames */
    /* ... more later as it's implemented ... */
} pvr_stats_t;

/** \brief  Get the current statistics from the PVR.

    This function fills in the pvr_stats_t structure passed in with the current
    statistics of the system.

    \param  stat            The statistics structure to fill in. Must not be
                            NULL
    \retval 0               On success
    \retval -1              If the PVR is not initialized
*/
int pvr_get_stats(pvr_stats_t *stat);


/* Palette management ************************************************/

/* In addition to its 16-bit truecolor modes, the PVR also supports some
   nice paletted modes. These aren't useful for super high quality images
   most of the time, but they can be useful for doing some interesting
   special effects, like the old cheap "worm hole". */

/** \defgroup pvr_palfmts           PVR palette formats

    Entries in the PVR's palettes can be of any of these formats. Note that you
    can only have one format active at a time.

    @{
*/
#define PVR_PAL_ARGB1555    0   /**< \brief 16-bit ARGB1555 palette format */
#define PVR_PAL_RGB565      1   /**< \brief 16-bit RGB565 palette format */
#define PVR_PAL_ARGB4444    2   /**< \brief 16-bit ARGB4444 palette format */
#define PVR_PAL_ARGB8888    3   /**< \brief 32-bit ARGB8888 palette format */
/** @} */

/** \brief  Set the palette format.

    This function sets the currently active palette format on the PVR. Each
    entry in the palette table is 32-bits in length, regardless of what color
    format is in use.

    Be sure to use care when using the PVR_PAL_ARGB8888 format. Rendering speed
    is greatly affected (cut about in half) if you use any filtering with
    paletted textures with ARGB8888 entries in the palette.

    \param  fmt             The format to use
    \see    pvr_palfmts
*/
void pvr_set_pal_format(int fmt);

/** \brief  Set a palette value.

    Note that while the color format is variable, each entry is still 32-bits in
    length regardless (and you only get a total of 1024 of them). If using one
    of the 16-bit palette formats, only the low-order 16-bits of the entry are
    valid, and the high bits should be filled in with 0.

    \param  idx             The index to set to (0-1023)
    \param  value           The color value to set in that palette entry
*/
static inline void pvr_set_pal_entry(uint32 idx, uint32 value) {
    PVR_SET(PVR_PALETTE_TABLE_BASE + 4 * idx, value);
}


/* Hardware Fog parameters *******************************************/

/* Thanks to Paul Boese for figuring this stuff out */

/** \brief  Set the table fog color.

    This function sets the color of fog for table fog. 0-1 range for all colors.

    \param  a               Alpha value of the fog
    \param  r               Red value of the fog
    \param  g               Green value of the fog
    \param  b               Blue value of the fog
*/
void pvr_fog_table_color(float a, float r, float g, float b);

/** \brief  Set the vertex fog color.

    This function sets the fog color for vertex fog. 0-1 range for all colors.
    This function is currently not implemented, as vertex fog is not supported
    by KOS. Calling this function will cause an assertion failure.

    \param  a               Alpha value of the fog
    \param  r               Red value of the fog
    \param  g               Green value of the fog
    \param  b               Blue value of the fog
*/
void pvr_fog_vertex_color(float a, float r, float g, float b);

/** \brief  Set the fog far depth.

    This function sets the PVR_FOG_DENSITY register appropriately for the
    specified value.

    \param  d               The depth to set
*/
void pvr_fog_far_depth(float d);

/** \brief  Initialize the fog table using an exp2 algorithm (like GL_EXP2).

    This function will automatically set the PVR_FOG_DENSITY register to
    259.999999 as a part of its processing, then set up the fog table.

    \param  density         Fog density value
*/
void pvr_fog_table_exp2(float density);

/** \brief  Initialize the fog table using an exp algorithm (like GL_EXP).

    This function will automatically set the PVR_FOG_DENSITY register to
    259.999999 as a part of its processing, then set up the fog table.

    \param  density         Fog density value
*/
void pvr_fog_table_exp(float density);

/** \brief  Initialize the fog table using a linear algorithm (like GL_LINEAR).

    This function will set the PVR_FOG_DENSITY register to the as appropriate
    for the end value, and initialize the fog table for perspectively correct
    linear fog.

    \param  start           Fog start point
    \param  end             Fog end point
*/
void pvr_fog_table_linear(float start, float end);

/** \brief  Set a custom fog table from float values

    This function allows you to specify whatever values you need to for your fog
    parameters. All values should be clamped between 0 and 1, and its your
    responsibility to set up the PVR_FOG_DENSITY register by calling
    pvr_fog_far_depth() with an appropriate value. The table passed in should
    have 129 entries, where the 0th entry is farthest from the eye and the last
    entry is nearest. Higher values = heavier fog.

    \param  tbl1            The table of fog values to set
*/
void pvr_fog_table_custom(float tbl1[]);


/* Memory management *************************************************/

/* PVR memory management in KOS uses a modified dlmalloc; see the
   source file pvr_mem_core.c for more info. */

/** \brief  Allocate a chunk of memory from texture space.

    This function acts as the memory allocator for the PVR texture RAM pool. It
    acts exactly as one would expect a malloc() function to act, returning a
    normal pointer that can be directly written to if one desires to do so. All
    allocations will be aligned to a 32-byte boundary.

    \param  size            The amount of memory to allocate
    \return                 A pointer to the memory on success, NULL on error
*/
pvr_ptr_t pvr_mem_malloc(size_t size);

/** \brief  Free a block of allocated memory in the PVR RAM pool.

    This function frees memory previously allocated with pvr_mem_malloc().

    \param  chunk           The location of the start of the block to free
*/
void pvr_mem_free(pvr_ptr_t chunk);

/** \brief  Return the number of bytes available still in the PVR RAM pool.
    \return                 The number of bytes available
*/
uint32 pvr_mem_available();

/** \brief  Reset the PVR RAM pool.

    This will essentially free any blocks allocated within the pool. There's
    generally not many good reasons for doing this.
*/
void pvr_mem_reset();

/** \brief  Print the list of allocated blocks in the PVR RAM pool.

    This function only works if you've enabled KM_DBG in pvr_mem.c.
*/
void pvr_mem_print_list();

/** \brief  Print statistics about the PVR RAM pool.

    This prints out statistics like what malloc_stats() provides. Also, if
    KM_DBG is enabled in pvr_mem.c, it prints the list of allocated blocks.
*/
void pvr_mem_stats();

/* Scene rendering ***************************************************/

/* This API is used to submit triangle strips to the PVR via the TA
   interace in the chip.

   An important side note about the PVR is that all primitive types
   must be submitted grouped together. If you have 10 polygons for each
   list type, then the PVR must receive them via the TA by list type,
   with a list delimiter in between.

   So there are two modes you can use here. The first mode allows you to
   submit data directly to the TA. Your data will be forwarded to the
   chip for processing as it is fed to the PVR module. If your data
   is easily sorted into the primitive types, then this is the fastest
   mode for submitting data.

   The second mode allows you to submit data via main-RAM vertex buffers,
   which will be queued until the proper primitive type is active. In this
   case, each piece of data is copied into the vertex buffer while the
   wrong list is activated, and when the proper list becomes activated,
   the data is all sent at once. Ideally this would be via DMA, right
   now it is by store queues. This has the advantage of allowing you to
   send data in any order and have the PVR functions resolve how it should
   get sent to the hardware, but it is slower.

   The nice thing is that any combination of these modes can be used. You
   can assign a vertex buffer for any list, and it will be used to hold the
   incoming vertex data until the proper list has come up. Or if the proper
   list is already up, the data will be submitted directly. So if most of
   your polygons are opaque, and you only have a couple of translucents,
   you can set a small buffer to gather translucent data and then it will
   get sent when you do a pvr_end_scene().

   Thanks to Mikael Kalms for the idea for this API.

   Another somewhat subtle point that bears mentioning is that in the normal
   case (interrupts enabled) an interrupt handler will automatically take
   care of starting a frame rendering (after scene_finish()) and also
   flipping pages when appropriate. */

/** \brief  Is vertex DMA enabled?
    \return                 Non-zero if vertex DMA was enabled at init time
*/
int pvr_vertex_dma_enabled();

/** \brief  Setup a vertex buffer for one of the list types.

    If the specified list type already has a vertex buffer, it will be replaced
    by the new one. Note that each buffer should actually be twice as long as
    what you will need to hold two frames worth of data).

    You should generally not try to do this at any time besides before a frame
    is begun, or Bad Things May Happen.

    \param  list            The primitive list to set the buffer for.
    \param  buffer          The location of the buffer in main RAM. This must be
                            aligned to a 32-byte boundary.
    \param  len             The length of the buffer. This must be a multiple of
                            64, and must be at least 128 (even if you're not
                            using the list).
    \return                 The old buffer location (if any)
*/
void * pvr_set_vertbuf(pvr_list_t list, void * buffer, int len);

/** \brief  Retrieve a pointer to the current output location in the DMA buffer
            for the requested list.

    Vertex DMA must globally be enabled for this to work. Data may be added to
    this buffer by the user program directly; however, make sure to call
    pvr_vertbuf_written() to notify the system of any such changes.

    \param  list            The primitive list to get the buffer for.
    \return                 The tail of that list's buffer.
*/
void * pvr_vertbuf_tail(pvr_list_t list);

/** \brief  Notify the PVR system that data have been written into the output
            buffer for the given list.

    This should always be done after writing data directly to these buffers or
    it will get overwritten by other data.

    \param  list            The primitive list that was modified.
    \param  amt             Number of bytes written. Must be a multiple of 32.
*/
void pvr_vertbuf_written(pvr_list_t list, uint32 amt);

/** \brief  Set the translucent polygon sort mode for the next frame.

    This function sets the translucent polygon sort mode for the next frame of
    output, potentially switching between autosort and presort mode.

    For most programs, you'll probably want to set this at initialization time
    (with the autosort_disabled field in the pvr_init_params_t structure) and
    not mess with it per-frame. It is recommended that if you do use this
    function to change the mode that you should set it each frame to ensure that
    the mode is set properly.

    \param  presort         Set to 1 to set the presort mode for translucent
                            polygons, set to 0 to use autosort mode.
*/
void pvr_set_presort_mode(int presort);

/** \brief  Begin collecting data for a frame of 3D output to the off-screen
            frame buffer.

    You must call this function (or pvr_scene_begin_txr()) for ever frame of
    output.
*/
void pvr_scene_begin();

/** \brief  Begin collecting data for a frame of 3D output to the specified
            texture.

    This function currently only supports outputting at the same size as the
    actual screen. Thus, make sure rx and ry are at least large enough for that.
    For a 640x480 output, rx will generally be 1024 on input and ry 512, as
    these are the smallest values that are powers of two and will hold the full
    screen sized output.

    \param  txr             The texture to render to.
    \param  rx              Width of the texture buffer (in pixels).
    \param  ry              Height of the texture buffer (in pixels).
*/
void pvr_scene_begin_txr(pvr_ptr_t txr, uint32 *rx, uint32 *ry);

/** \brief  Begin collecting data for the given list type.

    Lists do not have to be submitted in any particular order, but all types of
    a list must be submitted at once (unless vertex DMA mode is enabled).

    Note that there is no need to call this function in DMA mode unless you want
    to make use of pvr_prim() for compatibility. This function will
    automatically call pvr_list_finish() if a list is already opened before
    opening the new list.

    \param  list            The list to open.
    \retval 0               On success.
    \retval -1              If the specified list has already been closed.
*/
int pvr_list_begin(pvr_list_t list);

/** \brief  End collecting data for the current list type.

    Lists can never be opened again within a single frame once they have been
    closed. Thus submitting a primitive that belongs in a closed list is
    considered an error. Closing a list that is already closed is also an error.

    Note that if you open a list but do not submit any primitives, a blank one
    will be submitted to satisfy the hardware. If vertex DMA mode is enabled,
    then this simply sets the current list pointer to no list, and none of the
    above restrictions apply.

    \retval 0               On success.
    \retval -1              On error.
*/
int pvr_list_finish();

/** \brief  Submit a primitive of the current list type.

    Note that any values submitted in this fashion will go directly to the
    hardware without any sort of buffering, and submitting a primitive of the
    wrong type will quite likely ruin your scene. Note that this also will not
    work if you haven't begun any list types (i.e., all data is queued). If DMA
    is enabled, the primitive will be appended to the end of the currently
    selected list's buffer.

    \param  data            The primitive to submit.
    \param  size            The length of the primitive, in bytes. Must be a
                            multiple of 32.
    \retval 0               On success.
    \retval -1              On error.
*/
int pvr_prim(void * data, int size);

/** \brief  Direct Rendering state variable type. */
typedef uint32 pvr_dr_state_t;

/** \brief  Initialize a state variable for Direct Rendering.

    \param  vtx_buf_ptr     A variable of type pvr_dr_state_t to init.
*/
#define pvr_dr_init(vtx_buf_ptr) do { \
        (vtx_buf_ptr) = 0; \
        QACR0 = ((((uint32)PVR_TA_INPUT) >> 26) << 2) & 0x1c; \
        QACR1 = ((((uint32)PVR_TA_INPUT) >> 26) << 2) & 0x1c; \
    } while (0)

/** \brief  Obtain the target address for Direct Rendering.

    \param  vtx_buf_ptr     State variable for Direct Rendering. Should be of
                            type pvr_dr_state_t, and must have been initialized
                            previously in the scene with pvr_dr_init().
    \return                 A write-only destination address where a primitive
                            should be written to get ready to submit it to the
                            TA in DR mode.
*/
#define pvr_dr_target(vtx_buf_ptr) \
    ({ (vtx_buf_ptr) ^= 32; \
        (pvr_vertex_t *)(0xe0000000 | (vtx_buf_ptr)); \
    })

/** \brief  Commit a primitive written into the Direct Rendering target address.

    \param  addr            The address returned by pvr_dr_target(), after you
                            have written the primitive to it.
*/
#define pvr_dr_commit(addr) __asm__ __volatile__("pref @%0" : : "r" (addr))

/** \brief  Submit a primitive of the given list type.

    Data will be queued in a vertex buffer, thus one must be available for the
    list specified (will be asserted by the code).

    \param  list            The list to submit to.
    \param  data            The primitive to submit.
    \param  size            The size of the primitive in bytes. This must be a
                            multiple of 32.
    \retval 0               On success.
    \retval -1              On error.
*/
int pvr_list_prim(pvr_list_t list, void * data, int size);

/** \brief  Flush the buffered data of the given list type to the TA.

    This function is currently not implemented, and calling it will result in an
    assertion failure. It is intended to be used later in a "hybrid" mode where
    both direct and DMA TA submission is possible.

    \param  list            The list to flush.
    \retval -1              On error (it is not possible to succeed).
*/
int pvr_list_flush(pvr_list_t list);

/** \brief  Call this after you have finished submitting all data for a frame.

    Once this has been called, you can not submit any more data until one of the
    pvr_scene_begin() or pvr_scene_begin_txr() functions is called again.

    \retval 0               On success.
    \retval -1              On error (no scene started).
*/
int pvr_scene_finish();

/** \brief  Block the caller until the PVR system is ready for another frame to
            be submitted.

    The PVR system allocates enough space for two frames: one in data collection
    mode, and another in rendering mode. If a frame is currently rendering, and
    another frame has already been closed, then the caller cannot do anything
    else until the rendering frame completes. Note also that the new frame
    cannot be activated except during a vertical blanking period, so this
    essentially waits until a rendered frame is complete and a vertical blank
    happens.

    \retval 0               On success. A new scene can be started now.
    \retval -1              On error. Something is probably very wrong...
*/
int pvr_wait_ready();

/** \brief  Check if the PVR system is ready for another frame to be submitted.

    \retval 0               If the PVR is ready for a new scene. You must call
                            pvr_wait_ready() afterwards, before starting a new
                            scene.
    \retval -1              If the PVR is not ready for a new scene yet.
*/
int pvr_check_ready();


/* Primitive handling ************************************************/

/* These functions help you prepare primitives for loading into the
   PVR for scene processing. */

/** \brief  Compile a polygon context into a polygon header.

    This function compiles a pvr_poly_cxt_t into the form needed by the hardware
    for rendering. This is for use with normal polygon headers.

    \param  dst             Where to store the compiled header.
    \param  src             The context to compile.
*/
void pvr_poly_compile(pvr_poly_hdr_t *dst, pvr_poly_cxt_t *src);

/** \brief  Fill in a polygon context for non-textured polygons.

    This function fills in a pvr_poly_cxt_t with default parameters appropriate
    for rendering a non-textured polygon in the given list.

    \param  dst             Where to store the polygon context.
    \param  list            The primitive list to be used.
*/
void pvr_poly_cxt_col(pvr_poly_cxt_t *dst, pvr_list_t list);

/** \brief  Fill in a polygon context for a textured polygon.

    This function fills in a pvr_poly_cxt_t with default parameters appropriate
    for rendering a textured polygon in the given list.

    \param  dst             Where to store the polygon context.
    \param  list            The primitive list to be used.
    \param  textureformat   The format of the texture used.
    \param  tw              The width of the texture, in pixels.
    \param  th              The height of the texture, in pixels.
    \param  textureaddr     A pointer to the texture.
    \param  filtering       The type of filtering to use.

    \see    pvr_txr_fmts
    \see    pvr_filter_modes
*/
void pvr_poly_cxt_txr(pvr_poly_cxt_t *dst, pvr_list_t list,
                      int textureformat, int tw, int th, pvr_ptr_t textureaddr,
                      int filtering);

/** \brief  Compile a sprite context into a sprite header.

    This function compiles a pvr_sprite_cxt_t into the form needed by the
    hardware for rendering. This is for use with sprite headers.

    \param  dst             Where to store the compiled header.
    \param  src             The context to compile.
*/
void pvr_sprite_compile(pvr_sprite_hdr_t *dst,
                        pvr_sprite_cxt_t *src);

/** \brief  Fill in a sprite context for non-textured sprites.

    This function fills in a pvr_sprite_cxt_t with default parameters
    appropriate for rendering a non-textured sprite in the given list.

    \param  dst             Where to store the sprite context.
    \param  list            The primitive list to be used.
*/
void pvr_sprite_cxt_col(pvr_sprite_cxt_t *dst, pvr_list_t list);

/** \brief  Fill in a sprite context for a textured sprite.

    This function fills in a pvr_sprite_cxt_t with default parameters
    appropriate for rendering a textured sprite in the given list.

    \param  dst             Where to store the sprite context.
    \param  list            The primitive list to be used.
    \param  textureformat   The format of the texture used.
    \param  tw              The width of the texture, in pixels.
    \param  th              The height of the texture, in pixels.
    \param  textureaddr     A pointer to the texture.
    \param  filtering       The type of filtering to use.

    \see    pvr_txr_fmts
    \see    pvr_filter_modes
*/
void pvr_sprite_cxt_txr(pvr_sprite_cxt_t *dst, pvr_list_t list,
                        int textureformat, int tw, int th, pvr_ptr_t textureaddr,
                        int filtering);

/** \brief  Create a modifier volume header.

    This function fills in a modifier volume header with the parameters
    specified. Note that unlike for polygons and sprites, there is no context
    step for modifiers.

    \param  dst             Where to store the modifier header.
    \param  list            The primitive list to be used.
    \param  mode            The mode for this modifier.
    \param  cull            The culling mode to use.

    \see    pvr_mod_modes
    \see    pvr_cull_modes
*/
void pvr_mod_compile(pvr_mod_hdr_t *dst, pvr_list_t list, uint32 mode,
                     uint32 cull);

/** \brief  Compile a polygon context into a polygon header that is affected by
            modifier volumes.

    This function works pretty similarly to pvr_poly_compile(), but compiles
    into the header type that is affected by a modifier volume. The context
    should have been created with either pvr_poly_cxt_col_mod() or
    pvr_poly_cxt_txr_mod().

    \param  dst             Where to store the compiled header.
    \param  src             The context to compile.
*/
void pvr_poly_mod_compile(pvr_poly_mod_hdr_t *dst, pvr_poly_cxt_t *src);

/** \brief  Fill in a polygon context for non-textured polygons affected by a
            modifier volume.

    This function fills in a pvr_poly_cxt_t with default parameters appropriate
    for rendering a non-textured polygon in the given list that will be affected
    by modifier volumes.

    \param  dst             Where to store the polygon context.
    \param  list            The primitive list to be used.
*/
void pvr_poly_cxt_col_mod(pvr_poly_cxt_t *dst, pvr_list_t list);

/** \brief  Fill in a polygon context for a textured polygon affected by
            modifier volumes.

    This function fills in a pvr_poly_cxt_t with default parameters appropriate
    for rendering a textured polygon in the given list and being affected by
    modifier volumes.

    \param  dst             Where to store the polygon context.
    \param  list            The primitive list to be used.
    \param  textureformat   The format of the texture used (outside).
    \param  tw              The width of the texture, in pixels (outside).
    \param  th              The height of the texture, in pixels (outside).
    \param  textureaddr     A pointer to the texture (outside).
    \param  filtering       The type of filtering to use (outside).
    \param  textureformat2  The format of the texture used (inside).
    \param  tw2             The width of the texture, in pixels (inside).
    \param  th2             The height of the texture, in pixels (inside).
    \param  textureaddr2    A pointer to the texture (inside).
    \param  filtering2      The type of filtering to use (inside).

    \see    pvr_txr_fmts
    \see    pvr_filter_modes
*/
void pvr_poly_cxt_txr_mod(pvr_poly_cxt_t *dst, pvr_list_t list,
                          int textureformat, int tw, int th,
                          pvr_ptr_t textureaddr, int filtering,
                          int textureformat2, int tw2, int th2,
                          pvr_ptr_t textureaddr2, int filtering2);

/* Texture handling **************************************************/

/* Helper functions for handling texture tasks of various kinds. */

/** \brief  Load raw texture data from an SH-4 buffer into PVR RAM.

    This essentially just acts as a memcpy() from main RAM to PVR RAM, using
    the store queues.

    \param  src             The location in main RAM holding the texture.
    \param  dst             The location in PVR RAM to copy to.
    \param  count           The size of the texture in bytes (must be a multiple
                            of 32).
*/
void pvr_txr_load(void * src, pvr_ptr_t dst, uint32 count);

/** \defgroup pvr_txrload_constants     Texture loading constants

    These are constants for the flags parameter to pvr_txr_load_ex() or
    pvr_txr_load_kimg().

    @{
*/
#define PVR_TXRLOAD_4BPP            0x01    /**< \brief 4BPP format */
#define PVR_TXRLOAD_8BPP            0x02    /**< \brief 8BPP format */
#define PVR_TXRLOAD_16BPP           0x03    /**< \brief 16BPP format */
#define PVR_TXRLOAD_FMT_MASK        0x0f    /**< \brief Bits used for basic formats */

#define PVR_TXRLOAD_VQ_LOAD         0x10    /**< \brief Do VQ encoding (not supported yet, if ever) */
#define PVR_TXRLOAD_INVERT_Y        0x20    /**< \brief Invert the Y axis while loading */
#define PVR_TXRLOAD_FMT_VQ          0x40    /**< \brief Texture is already VQ encoded */
#define PVR_TXRLOAD_FMT_TWIDDLED    0x80    /**< \brief Texture is already twiddled */
#define PVR_TXRLOAD_FMT_NOTWIDDLE   0x80    /**< \brief Don't twiddle the texture while loading */
#define PVR_TXRLOAD_DMA             0x8000  /**< \brief Use DMA to load the texture */
#define PVR_TXRLOAD_NONBLOCK        0x4000  /**< \brief Use non-blocking loads (only for DMA) */
#define PVR_TXRLOAD_SQ              0x2000  /**< \brief Use store queues to load */
/** @} */

/** \brief  Load texture data from an SH-4 buffer into PVR RAM, twiddling it in
            the process.

    This function loads a texture to the PVR's RAM with the specified set of
    flags. It will currently always twiddle the data, whether you ask it to or
    not, and many of the parameters are just plain not supported at all...
    Pretty much the only supported flag, other than the format ones is the
    PVR_TXRLOAD_INVERT_Y one.

    This will be slower than using pvr_txr_load() in pretty much all cases, so
    unless you need to twiddle your texture, just use that instead.

    \param  src             The location to copy from.
    \param  dst             The location to copy to.
    \param  w               The width of the texture, in pixels.
    \param  h               The height of the texture, in pixels.
    \param  flags           Some set of flags, ORed together.

    \see    pvr_txrload_constants
*/
void pvr_txr_load_ex(void * src, pvr_ptr_t dst, uint32 w, uint32 h, uint32 flags);

/** \brief  Load a KOS Platform Independent Image (subject to constraint
            checking).

    This function loads a KOS Platform Independent image to the PVR's RAM with
    the specified set of flags. This function, unlike pvr_txr_load_ex() supports
    everything in the flags available, other than what's explicitly marked as
    not supported.

    \param  img             The image to load.
    \param  dst             The location to copy to.
    \param  flags           Some set of flags, ORed together.

    \see    pvr_txrload_constants
    \note                   Unless you explicitly tell this function to not
                            twiddle the texture (by ORing
                            \ref PVR_TXRLOAD_FMT_NOTWIDDLE or it's equivalent
                            \ref PVR_TXRLOAD_FMT_TWIDDLED with flags), this
                            function will twiddle the texture while loading.
                            Keep that in mind when setting the texture format in
                            polygon headers later.
    \note                   You cannot specify both
                            \ref PVR_TXRLOAD_FMT_NOTWIDDLE (or equivalently
                            \ref PVR_TXRLOAD_FMT_TWIDDLED) and
                            \ref PVR_TXRLOAD_INVERT_Y in the flags.
    \note                   DMA and Store Queue based loading is not available
                            from this function if it twiddles the texture while
                            loading.
*/
void pvr_txr_load_kimg(kos_img_t *img, pvr_ptr_t dst, uint32 flags);


/* PVR DMA ***********************************************************/

/** \brief  PVR DMA interrupt callback type.

    Functions that act as callbacks when DMA completes should be of this type.
    These functions will be called inside an interrupt context, so don't try to
    use anything that might stall.

    \param  data            User data passed in to the pvr_dma_transfer()
                            function.
*/
typedef void (*pvr_dma_callback_t)(ptr_t data);

/** \brief  Perform a DMA transfer to the PVR.

    This function copies a block of data to the PVR or its memory via DMA. There
    are all kinds of constraints that must be fulfilled to actually do this, so
    make sure to read all the fine print with the parameter list.

    If a callback is specified, it will be called in an interrupt context, so
    keep that in mind in writing the callback.

    \param  src             Where to copy from. Must be 32-byte aligned.
    \param  dest            Where to copy to. Must be 32-byte aligned.
    \param  count           The number of bytes to copy. Must be a multiple of
                            32.
    \param  type            The type of DMA transfer to do (see list of modes).
    \param  block           Non-zero if you want the function to block until the
                            DMA completes.
    \param  callback        A function to call upon completion of the DMA.
    \param  cbdata          Data to pass to the callback function.
    \retval 0               On success.
    \retval -1              On failure. Sets errno as appropriate.

    \par    Error Conditions:
    \em     EINPROGRESS - DMA already in progress \n
    \em     EFAULT - dest is not 32-byte aligned \n
    \em     EIO - I/O error

    \see    pvr_dma_modes
*/
int pvr_dma_transfer(void * src, uint32 dest, uint32 count, int type,
                     int block, pvr_dma_callback_t callback, ptr_t cbdata);

/** \defgroup pvr_dma_modes         Transfer modes with PVR DMA
    @{
*/
#define PVR_DMA_VRAM64  0   /**< \brief Transfer to VRAM in interleaved mode */
#define PVR_DMA_VRAM32  1   /**< \brief Transfer to VRAM in linear mode */
#define PVR_DMA_TA      2   /**< \brief Transfer to the tile accelerator */
/** @} */

/** \brief  Load a texture using PVR DMA.

    This is essentially a convenience wrapper for pvr_dma_transfer(), so all
    notes that apply to it also apply here.

    \param  src             Where to copy from. Must be 32-byte aligned.
    \param  dest            Where to copy to. Must be 32-byte aligned.
    \param  count           The number of bytes to copy. Must be a multiple of
                            32.
    \param  block           Non-zero if you want the function to block until the
                            DMA completes.
    \param  callback        A function to call upon completion of the DMA.
    \param  cbdata          Data to pass to the callback function.
    \retval 0               On success.
    \retval -1              On failure. Sets errno as appropriate.

    \par    Error Conditions:
    \em     EINPROGRESS - DMA already in progress \n
    \em     EFAULT - dest is not 32-byte aligned \n
    \em     EIO - I/O error
*/
int pvr_txr_load_dma(void * src, pvr_ptr_t dest, uint32 count, int block,
                     pvr_dma_callback_t callback, ptr_t cbdata);

/** \brief  Load vertex data to the TA using PVR DMA.

    This is essentially a convenience wrapper for pvr_dma_transfer(), so all
    notes that apply to it also apply here.

    \param  src             Where to copy from. Must be 32-byte aligned.
    \param  count           The number of bytes to copy. Must be a multiple of
                            32.
    \param  block           Non-zero if you want the function to block until the
                            DMA completes.
    \param  callback        A function to call upon completion of the DMA.
    \param  cbdata          Data to pass to the callback function.
    \retval 0               On success.
    \retval -1              On failure. Sets errno as appropriate.

    \par    Error Conditions:
    \em     EINPROGRESS - DMA already in progress \n
    \em     EFAULT - dest is not 32-byte aligned \n
    \em     EIO - I/O error
 */
int pvr_dma_load_ta(void * src, uint32 count, int block,
                    pvr_dma_callback_t callback, ptr_t cbdata);

/** \brief  Is PVR DMA is inactive?
    \return                 Non-zero if there is no PVR DMA active, thus a DMA
                            can begin or 0 if there is an active DMA.
*/
int pvr_dma_ready();

/** \brief  Initialize PVR DMA. */
void pvr_dma_init();

/** \brief  Shut down PVR DMA. */
void pvr_dma_shutdown();

/*********************************************************************/


__END_DECLS

#endif
