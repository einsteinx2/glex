/* KallistiOS ##version##

   dc/biosfont.h
   (c)2000-2001 Dan Potter
   Japanese Functions (c)2002 Kazuaki Matsumoto
   (c)2017 Donald Haase

*/

/** \file   dc/biosfont.h
    \brief  BIOS font drawing functions.

    This file provides support for utilizing the font built into the Dreamcast's
    BIOS. These functions allow access to both the western character set and
    Japanese characters.

    \author Dan Potter
    \author Kazuaki Matsumoto
    \author Donald Haase
*/

#ifndef __DC_BIOSFONT_H
#define __DC_BIOSFONT_H

#include <kos/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>

/** \defgroup bfont_size  Dimensions of the Bios Font
    @{
*/
#define BFONT_THIN_WIDTH                        12   /**< \brief Width of Thin Font (ISO8859_1, half-JP) */
#define BFONT_WIDE_WIDTH      BFONT_THIN_WIDTH * 2   /**< \brief Width of Wide Font (full-JP) */
#define BFONT_HEIGHT                            24   /**< \brief Height of All Fonts */
/** @} */

#define JISX_0208_ROW_SIZE  94
/** \defgroup bfont_indecies Structure of the Bios Font 
    @{
*/
#define BFONT_NARROW_START          0   /**< \brief Start of Narrow Characters in Font Block */
#define BFONT_OVERBAR               BFONT_NARROW_START 
#define BFONT_ISO_8859_1_33_126     BFONT_NARROW_START+( 1*BFONT_THIN_WIDTH*BFONT_HEIGHT/8) 
#define BFONT_YEN                   BFONT_NARROW_START+(95*BFONT_THIN_WIDTH*BFONT_HEIGHT/8) 
#define BFONT_ISO_8859_1_160_255    BFONT_NARROW_START+(96*BFONT_THIN_WIDTH*BFONT_HEIGHT/8) 

/* JISX-0208 Rows 1-7 and 16-84 are encoded between BFONT_WIDE_START and BFONT_DREAMCAST_SPECIFIC. 
    Only the box-drawing characters (row 8) are missing. */
#define BFONT_WIDE_START            (288*BFONT_THIN_WIDTH*BFONT_HEIGHT/8)   /**< \brief Start of Wide Characters in Font Block */
#define BFONT_JISX_0208_ROW1        BFONT_WIDE_START    /**< \brief Start of JISX-0208 Rows 1-7 in Font Block */
#define BFONT_JISX_0208_ROW16       BFONT_WIDE_START+(658*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)  /**< \brief Start of JISX-0208 Row 16-47 (Start of Level 1) in Font Block */
#define BFONT_JISX_0208_ROW48       BFONT_JISX_0208_ROW16+((32*JISX_0208_ROW_SIZE)*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8) /**< \brief JISX-0208 Row 48-84 (Start of Level 2) in Font Block */


#define BFONT_DREAMCAST_SPECIFIC    BFONT_WIDE_START+(7056*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8) /**< \brief Start of DC Specific Characters in Font Block */
#define BFONT_CIRCLECOPYRIGHT       BFONT_DREAMCAST_SPECIFIC+(0*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_CIRCLER               BFONT_DREAMCAST_SPECIFIC+(1*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_TRADEMARK             BFONT_DREAMCAST_SPECIFIC+(2*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_UPARROW               BFONT_DREAMCAST_SPECIFIC+(3*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_DOWNARROW             BFONT_DREAMCAST_SPECIFIC+(4*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_LEFTARROW             BFONT_DREAMCAST_SPECIFIC+(5*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_RIGHTARROW            BFONT_DREAMCAST_SPECIFIC+(6*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_UPRIGHTARROW          BFONT_DREAMCAST_SPECIFIC+(7*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_DOWNRIGHTARROW        BFONT_DREAMCAST_SPECIFIC+(8*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_DOWNLEFTARROW         BFONT_DREAMCAST_SPECIFIC+(9*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_UPLEFTARROW           BFONT_DREAMCAST_SPECIFIC+(10*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_ABUTTON               BFONT_DREAMCAST_SPECIFIC+(11*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_BBUTTON               BFONT_DREAMCAST_SPECIFIC+(12*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_CBUTTON               BFONT_DREAMCAST_SPECIFIC+(13*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_DBUTTON               BFONT_DREAMCAST_SPECIFIC+(14*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_XBUTTON               BFONT_DREAMCAST_SPECIFIC+(15*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_YBUTTON               BFONT_DREAMCAST_SPECIFIC+(16*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_ZBUTTON               BFONT_DREAMCAST_SPECIFIC+(17*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_LTRIGGER              BFONT_DREAMCAST_SPECIFIC+(18*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_RTRIGGER              BFONT_DREAMCAST_SPECIFIC+(19*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_STARTBUTTON           BFONT_DREAMCAST_SPECIFIC+(20*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)
#define BFONT_VMUICON               BFONT_DREAMCAST_SPECIFIC+(21*BFONT_WIDE_WIDTH*BFONT_HEIGHT/8)



/** @} */

/** \brief  Set the font foreground color.

    This function selects the foreground color to draw when a pixel is opaque in
    the font. The value passed in for the color should be in whatever pixel
    format that you intend to use for the image produced.

    \param  c               The color to use.
    \return                 The old foreground color.
*/
uint32 bfont_set_foreground_color(uint32 c);

/** \brief  Set the font background color.

    This function selects the background color to draw when a pixel is drawn in
    the font. This color is only used for pixels not covered by the font when
    you have selected to have the font be opaque.

    \param  c               The color to use.
    \return                 The old background color.
*/
uint32 bfont_set_background_color(uint32 c);

/** \brief  Set the font to draw 32-bit color.

    This function changes whether the font draws colors as 32-bit or 16-bit. The
    default is to use 16-bit.

    \param  on              Set to 0 to use 16-bit color, 32-bit otherwise.
    \return                 The old state (1 = 32-bit, 0 = 16-bit).
*/
int bfont_set_32bit_mode(int on) __depr("Please use the bpp function of the the bfont_draw_ex functions");

/* Constants for the function below */
#define BFONT_CODE_ISO8859_1    0   /**< \brief ISO-8859-1 (western) charset */
#define BFONT_CODE_EUC          1   /**< \brief EUC-JP charset */
#define BFONT_CODE_SJIS         2   /**< \brief Shift-JIS charset */
#define BFONT_CODE_RAW          3   /**< \brief Raw indexing to the BFONT */

/** \brief  Set the font encoding.

    This function selects the font encoding that is used for the font. This
    allows you to select between the various character sets available.

    \param  enc             The character encoding in use
    \see    BFONT_CODE_ISO8859_1
    \see    BFONT_CODE_EUC
    \see    BFONT_CODE_SJIS
    \see    BFONT_CODE_RAW
*/
void bfont_set_encoding(uint8 enc);

/** \brief  Find an ISO-8859-1 character in the font.

    This function retrieves a pointer to the font data for the specified
    character in the font, if its available. Generally, you will not have to
    use this function, use one of the bfont_draw_* functions instead.

    \param  ch              The character to look up
    \return                 A pointer to the raw character data
*/
uint8 *bfont_find_char(uint32 ch);

/** \brief  Find an full-width Japanese character in the font.

    This function retrieves a pointer to the font data for the specified
    character in the font, if its available. Generally, you will not have to
    use this function, use one of the bfont_draw_* functions instead.

    This function deals with full-width kana and kanji.

    \param  ch              The character to look up
    \return                 A pointer to the raw character data
*/
uint8 *bfont_find_char_jp(uint32 ch);

/** \brief  Find an half-width Japanese character in the font.

    This function retrieves a pointer to the font data for the specified
    character in the font, if its available. Generally, you will not have to
    use this function, use one of the bfont_draw_* functions instead.

    This function deals with half-width kana only.

    \param  ch              The character to look up
    \return                 A pointer to the raw character data
*/
uint8 *bfont_find_char_jp_half(uint32 ch);

/** \brief Draw a single character of any sort to the buffer.

    This function draws a single character in the set encoding to the given 
    buffer. This function sits under draw, draw_thin, and draw_wide, while 
    exposing the colors and bitdepths desired. This will allow the writing 
    of bfont characters to palletted textures.
    
    \param buffer       The buffer to draw to.
    \param bufwidth     The width of the buffer in piexels.
    \param fg           The foreground color to use.
    \param bg           The background color to use.
    \param bpp          The number of bits per pixel in the buffer.
    \param opaque       If non-zero, overwrite background areas with black,
                            otherwise do not change them from what they are.
    \param c            The character to draw.
    \param wide         Draw a wide character.
    \param iskana       Draw a half-width kana character.    
    \return             Amount of width covered in bytes.
*/
unsigned char bfont_draw_ex(uint8 *buffer, uint32 bufwidth, uint32 fg, uint32 bg, uint8 bpp, uint8 opaque, uint32 c, uint8 wide, uint8 iskana);

/** \brief  Draw a single character to a buffer.

    This function draws a single character in the set encoding to the given
    buffer. Calling this is equivalent to calling bfont_draw_thin() with 0 for
    the final parameter.

    \param  buffer          The buffer to draw to (at least 12 x 24 pixels)
    \param  bufwidth        The width of the buffer in pixels
    \param  opaque          If non-zero, overwrite blank areas with black,
                            otherwise do not change them from what they are
    \param  c               The character to draw
    \return                 Amount of width covered in bytes.
*/
unsigned char bfont_draw(void *buffer, uint32 bufwidth, uint8 opaque, uint32 c);

/** \brief  Draw a single thin character to a buffer.

    This function draws a single character in the set encoding to the given
    buffer. This only works with ISO-8859-1 characters and half-width kana.

    \param  buffer          The buffer to draw to (at least 12 x 24 pixels)
    \param  bufwidth        The width of the buffer in pixels
    \param  opaque          If non-zero, overwrite blank areas with black,
                            otherwise do not change them from what they are
    \param  c               The character to draw
    \param  iskana          Set to 1 if the character is a kana, 0 if ISO-8859-1
    \return                 Amount of width covered in bytes.
*/
unsigned char bfont_draw_thin(void *buffer, uint32 bufwidth, uint8 opaque, uint32 c, uint8 iskana);

/** \brief  Draw a single wide character to a buffer.

    This function draws a single character in the set encoding to the given
    buffer. This only works with full-width kana and kanji.

    \param  buffer          The buffer to draw to (at least 24 x 24 pixels)
    \param  bufwidth        The width of the buffer in pixels
    \param  opaque          If non-zero, overwrite blank areas with black,
                            otherwise do not change them from what they are
    \param  c               The character to draw
    \return                 Amount of width covered in bytes.
*/
unsigned char bfont_draw_wide(void *buffer, uint32 bufwidth, uint8 opaque, uint32 c);

/** \brief  Draw a full string to any sort of buffer.

    This function draws a NUL-terminated string in the set encoding to the given
    buffer. This will automatically handle mixed half and full-width characters
    if the encoding is set to one of the Japanese encodings. Colors and bitdepth 
    can be set.

    \param b       The buffer to draw to.
    \param width     The width of the buffer in pixels.
    \param fg           The foreground color to use.
    \param bg           The background color to use.
    \param bpp          The number of bits per pixel in the buffer.
    \param opaque       If non-zero, overwrite background areas with black,
                            otherwise do not change them from what they are.
    \param str          The string to draw.
    
*/
void bfont_draw_str_ex(void *b, uint32 width, uint32 fg, uint32 bg, uint8 bpp, uint8 opaque, char *str);

/** \brief  Draw a full string to a buffer.

    This function draws a NUL-terminated string in the set encoding to the given
    buffer. This will automatically handle mixed half and full-width characters
    if the encoding is set to one of the Japanese encodings. Draws pre-set 
    16-bit colors.

    \param  b          The buffer to draw to
    \param  width           The width of the buffer in pixels
    \param  opaque          If one, overwrite blank areas with bfont_bgcolor,
                            otherwise do not change them from what they are
    \param  str             The string to draw
*/
void bfont_draw_str(void *b, uint32 width, uint8 opaque, char *str);

__END_DECLS

#endif  /* __DC_BIOSFONT_H */
