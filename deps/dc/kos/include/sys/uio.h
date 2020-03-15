/* KallistiOS ##version##

   sys/uio.h
   Copyright (C) 2017 Lawrence Sebald

*/

/** \file   sys/uio.h
    \brief  Header for vector I/O.

    This file contains definitions for vector I/O operations, as specified by
    the POSIX 2008 specification. Full vector-based I/O is not supported for
    file operations, but the stuff in here is still useful elsewhere.

    \author Lawrence Sebald
*/

#ifndef __SYS_UIO_H
#define __SYS_UIO_H

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/** \brief  I/O vector structure
    \headerfile sys/iovec.h
*/
struct iovec {
    /** \brief  Base address of memory for I/O. */
    void * iov_base;
    /** \brief  Size of memory pointed to by iov_base. */
    size_t iov_len;
};

/** \brief  Old alias for the maximum length of an iovec. */
#define UIO_MAXIOV IOV_MAX

__END_DECLS

#endif /* __SYS_UIO_H */
