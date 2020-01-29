/* KallistiOS ##version##

   arch/dreamcast/include/arch/gdb.h
   (c)2002 Dan Potter

*/

/** \file   arch/gdb.h
    \brief  GNU Debugger support.

    This file contains functions to set up and utilize GDB with KallistiOS.

    \author Dan Potter
*/

#ifndef __ARCH_GDB_H
#define __ARCH_GDB_H

#include <sys/cdefs.h>
__BEGIN_DECLS

/** \brief  Initialize the GDB stub.

    This function initializes GDB support. It should be the first thing you do
    in your program, when you wish to use GDB for debugging.
*/
void gdb_init();

/** \brief  Manually raise a GDB breakpoint.

    This function manually raises a GDB breakpoint at the current location in
    the code, allowing you to inspect things with GDB at the point where the
    function is called.
*/
void gdb_breakpoint();

__END_DECLS

#endif  /* __ARCH_GDB_H */

