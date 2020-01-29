/* KallistiOS ##version##

   dc/fs_dclsocket.h
   Copyright (C) 2007, 2008 Lawrence Sebald

*/

/** \file   dc/fs_dclsocket.h
    \brief  Implementation of dcload-ip over KOS sockets.

    This file contains declarations related to using dcload-ip over the KOS
    sockets system. If dcload-ip support is enabled at the same time as network
    support, this is how the communications will happen. There isn't really
    anything that users will need to deal with in here.

    \author Lawrence Sebald
*/

#ifndef __DC_FSDCLSOCKET_H
#define __DC_FSDCLSOCKET_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <dc/fs_dcload.h>

/* \cond */
extern dbgio_handler_t dbgio_dcls;

/* Initialization */
void fs_dclsocket_init_console();
int fs_dclsocket_init();

int fs_dclsocket_shutdown();
/* \endcond */

__END_DECLS

#endif /* __DC_FSDCLSOCKET_H */
