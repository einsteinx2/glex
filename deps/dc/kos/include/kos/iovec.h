/* KallistiOS ##version##

   kos/iovec.h
   Copyright (C) 2001 Dan Potter

*/

/** \file   kos/iovec.h
    \brief  Deprecated header file for I/O scatter/gather arrays.

    See <sys/uio.h> for what used to be here.
*/

#ifndef __KOS_IOVEC_H
#define __KOS_IOVEC_H

#include <sys/uio.h>

/** \brief  Compatibility typedef for old code. */
typedef struct iovec iovec_t;

#endif  /* __KOS_IOVEC_H */
