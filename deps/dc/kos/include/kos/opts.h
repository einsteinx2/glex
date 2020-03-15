/* KallistiOS ##version##

   kos/opts.h
   Copyright (C) 2014 Lawrence Sebald
*/

#ifndef __KOS_OPTS_H
#define __KOS_OPTS_H

#include <sys/cdefs.h>
__BEGIN_DECLS

/** \file   kos/opts.h
    \brief  Compile-time options regarding debugging and other topics.

    This file is meant to be a kind of Grand Central Station for all of the
    various compile-time options that can be set when building KOS. Each of the
    various compile-time macros that control things like additional debugging
    checks and such should be documented in this particular file. In addition,
    groups of related macros are provided here to make it easier to set these
    options at compile time.

    Basically, this is here to make it easier to customize your copy of KOS. In
    the past, you would have had to search through all the various files to find
    potential options, which was both annoying and error-prone. In addition,
    often various debugging flags would end up being set in the main repository
    as people debugged individual pieces of code, leaving them set for everyone
    else, even if they aren't necessarily useful/helpful all the time (KM_DBG,
    I'm looking at you).

    \author Lawrence Sebald
*/

/* Various debug options. Uncomment the #define line to enable the specific
   option described. */
/* Enable debugging in fs_dclnative. This filesystem is deprecated, so there
   probably won't be any good reason to enable these two. The first prints a bit
   of debugging data, whereas the second prints out all packets received and
   transmitted as well. You must uncomment the first in order for the second to
   have any effect. */
/* #define DCLN_DEBUG 1 */
/* #define DCLN_DEBUG_VERBOSE 1 */


/* Enable debugging in fs_vmu. */
/* #define VMUFS_DEBUG 1 */


/* Enable to allow extra debugging checks in the malloc code itself. This
   sometimes catches corrupted blocks. Recommended during debugging phases. */
/* #define MALLOC_DEBUG 1 */

/* Enable this define if you want costly malloc debugging (buffer sentinel
   checking, block leak checking, etc). Recommended during debugging phases, but
   you should probably take it out before you start your final testing. */
/* #define KM_DBG 1 */

/* Enable this define if you want REALLY verbose malloc debugging (print every
   time a block is allocated or freed). Only enable this if you are having some
   serious issues. */
/* #define KM_DBG_VERBOSE 1 */


/* The following three macros are similar to the ones above, but for the PVR
   memory pool malloc. */
/* #define PVR_MALLOC_DEBUG 1 */
/* #define PVR_KM_DBG 1 */
/* #define PVR_KM_DBG_VERBOSE 1 */


/* Aggregate debugging levels. It's probably best to enable these with your
   KOS_CFLAGS when compiling KOS itself, but they're all documented here and
   can be enabled here, if you really want to. */

/* Enable all recommended options for normal debugging use. This enables the
   first level of malloc debugging for the main pool, as well as the internal
   dlmalloc debugging code for the main pool. This is essentially the set that
   was normally enabled in the repository tree. */
/* #define KOS_DEBUG 1 */

/* Enable verbose debugging support. Basically, if your code is crashing and the
   stuff in the first level doesn't help, then use this one instead. This adds
   in verbose malloc debugging in the main pool, as well as basic PVR malloc
   debugging. */
/* #define KOS_DEBUG 2 */

/* Enable verbose debugging support, plus additional debugging options that
   normally wouldn't be needed. Once again, try lighter levels before this one,
   as there's not much reason this one should ever be needed. This level
   includes the internal debugging stuff in fs_vmu, verbose debugging in the PVR
   malloc, as well as everything in level 2. */
/* #define KOS_DEBUG 3 */


#if KOS_DEBUG >= 1
#define MALLOC_DEBUG 1
#define KM_DBG 1
#endif

#if KOS_DEBUG >= 2
#define KM_DBG_VERBOSE 1
#define PVR_MALLOC_DEBUG 1
#define PVR_KM_DBG 1
#endif

#if KOS_DEBUG >= 3
#define PVR_KM_DBG_VERBOSE 1
#define VMUFS_DEBUG 1
#endif

__END_DECLS

#endif /* !__KOS_OPTS_H */
