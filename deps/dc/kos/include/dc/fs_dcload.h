/* KallistiOS ##version##

   kernel/arch/dreamcast/include/dc/fs_dcload.h
   (c)2002 Andrew Kieschnick

*/

/** \file   dc/fs_dcload.h
    \brief  Implementation of dcload "filesystem".

    This file contains declarations related to using dcload, both in its -ip and
    -serial forms. This is only used for dcload-ip support if the internal
    network stack is not initialized at start via KOS_INIT_FLAGS().

    \author Andrew Kieschnick
    \see    dc/fs_dclsocket.h
*/

#ifndef __DC_FS_DCLOAD_H
#define __DC_FS_DCLOAD_H

/* Definitions for the "dcload" file system */

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <kos/limits.h>
#include <kos/fs.h>
#include <kos/dbgio.h>

/* \cond */
extern dbgio_handler_t dbgio_dcload;
/* \endcond */

/* dcload magic value */
/** \brief  The dcload magic value! */
#define DCLOADMAGICVALUE 0xdeadbeef

/** \brief  The address of the dcload magic value */
#define DCLOADMAGICADDR (unsigned int *)0x8c004004

/* Are we using dc-load-serial or dc-load-ip? */
#define DCLOAD_TYPE_NONE    -1      /**< \brief No dcload connection */
#define DCLOAD_TYPE_SER     0       /**< \brief dcload-serial connection */
#define DCLOAD_TYPE_IP      1       /**< \brief dcload-ip connection */

/** \brief  What type of dcload connection do we have? */
extern int dcload_type;

/* \cond */
/* Available dcload console commands */

#define DCLOAD_READ 0
#define DCLOAD_WRITE 1
#define DCLOAD_OPEN 2
#define DCLOAD_CLOSE 3
#define DCLOAD_CREAT 4
#define DCLOAD_LINK 5
#define DCLOAD_UNLINK 6
#define DCLOAD_CHDIR 7
#define DCLOAD_CHMOD 8
#define DCLOAD_LSEEK 9
#define DCLOAD_FSTAT 10
#define DCLOAD_TIME 11
#define DCLOAD_STAT 12
#define DCLOAD_UTIME 13
#define DCLOAD_ASSIGNWRKMEM 14
#define DCLOAD_EXIT 15
#define DCLOAD_OPENDIR 16
#define DCLOAD_CLOSEDIR 17
#define DCLOAD_READDIR 18
#define DCLOAD_GETHOSTINFO 19
#define DCLOAD_GDBPACKET 20

/* dcload syscall function */

int dcloadsyscall(unsigned int syscall, ...);

/* dcload dirent */

struct dcload_dirent {
    long            d_ino;  /* inode number */
    off_t           d_off;  /* offset to the next dirent */
    unsigned short  d_reclen;/* length of this record */
    unsigned char   d_type;         /* type of file */
    char            d_name[256];    /* filename */
};

typedef struct dcload_dirent dcload_dirent_t;

/* dcload stat */

struct  dcload_stat {
    unsigned short st_dev;
    unsigned short st_ino;
    int st_mode;
    unsigned short st_nlink;
    unsigned short st_uid;
    unsigned short st_gid;
    unsigned short st_rdev;
    long st_size;
    long st_atime;
    long st_spare1;
    long st_mtime;
    long st_spare2;
    long st_ctime;
    long st_spare3;
    long st_blksize;
    long st_blocks;
    long st_spare4[2];
};

typedef struct dcload_stat dcload_stat_t;

/* Printk replacement */
void dcload_printk(const char *str);

/* GDB tunnel */
size_t dcload_gdbpacket(const char* in_buf, size_t in_size, char* out_buf, size_t out_size);

/* File functions */
void*   dcload_open(vfs_handler_t * vfs, const char *fn, int mode);
int     dcload_close(void * hnd);
ssize_t dcload_read(void * hnd, void *buf, size_t cnt);
off_t   dcload_seek(void * hnd, off_t offset, int whence);
off_t   dcload_tell(void * hnd);
size_t  dcload_total(void * hnd);
dirent_t* dcload_readdir(void * hnd);
int     dcload_rename(vfs_handler_t * vfs, const char *fn1, const char *fn2);
int     dcload_unlink(vfs_handler_t * vfs, const char *fn);

/* Init func */
void fs_dcload_init_console();
int fs_dcload_init();
int fs_dcload_shutdown();

/* Init func for dcload-ip + lwIP */
int fs_dcload_init_lwip(void *p);

/* \endcond */

__END_DECLS

#endif  /* __KALLISTI_FS_DCLOAD_H */


