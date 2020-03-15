/* KallistiOS ##version##

   dc/maple/lightgun.h
   Copyright (C) 2015 Lawrence Sebald

*/

/** \file   dc/maple/lightgun.h
    \brief  Definitions for using the light gun.

    This file contains the definitions needed to access maple light gun devices.
    There's really no user-serviceable parts in here, as there's very little to
    this driver.

    \author Lawrence Sebald
*/

#ifndef __DC_MAPLE_LIGHTGUN_H
#define __DC_MAPLE_LIGHTGUN_H

#include <sys/cdefs.h>
__BEGIN_DECLS

/* \cond */
/* Init / Shutdown */
int lightgun_init(void);
void lightgun_shutdown(void);
/* \endcond */

__END_DECLS

#endif  /* __DC_MAPLE_LIGHTGUN_H */
