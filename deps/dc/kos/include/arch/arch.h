/* KallistiOS ##version##

   arch/dreamcast/include/arch.h
   Copyright (C) 2001 Dan Potter
   Copyright (C) 2013 Lawrence Sebald

*/

/** \file   arch/arch.h
    \brief  Dreamcast architecture specific options.

    This file has various architecture specific options defined in it. Also, any
    functions that start with arch_ are in here.

    \author Dan Potter
*/

#ifndef __ARCH_ARCH_H
#define __ARCH_ARCH_H

#include <kos/cdefs.h>
__BEGIN_DECLS

#include <dc/video.h>

#define PAGESIZE        4096            /**< \brief Page size (for MMU) */
#define PAGESIZE_BITS   12              /**< \brief Bits for page size */
#define PAGEMASK        (PAGESIZE - 1)  /**< \brief Mask for page offset */

/** \brief  Page count "variable".

    The number of pages is static, so we can optimize this quite a bit. */
#define page_count      ((16*1024*1024 - 0x10000) / PAGESIZE)

/** \brief  Base address of available physical pages. */
#define page_phys_base  0x8c010000

/** \brief  Number of timer ticks per second. */
#define HZ              100

/** \brief  Default thread stack size. */
#define THD_STACK_SIZE  32768

/** \brief  Default video mode. */
#define DEFAULT_VID_MODE    DM_640x480

/** \brief  Default pixel mode for video. */
#define DEFAULT_PIXEL_MODE  PM_RGB565

/** \brief  Default serial bitrate. */
#define DEFAULT_SERIAL_BAUD 57600

/** \brief  Default serial FIFO behavior. */
#define DEFAULT_SERIAL_FIFO 1

/** \brief  Global symbol prefix in ELF files. */
#define ELF_SYM_PREFIX      "_"

/** \brief  Length of global symbol prefix in ELF files. */
#define ELF_SYM_PREFIX_LEN  1

/** \brief  Panic function.

    This function will cause a kernel panic, printing the specified message.

    \param  str             The error message to print.
    \note                   This function will never return!
*/
void arch_panic(const char *str) __noreturn;

/** \brief  Kernel C-level entry point.
    \return                 The program's return value.
*/
int arch_main();

/** \defgroup arch_retpaths         Potential exit paths from the kernel on
                                    arch_exit()

    @{
*/
#define ARCH_EXIT_RETURN    1   /**< \brief Return to loader */
#define ARCH_EXIT_MENU      2   /**< \brief Return to system menu */
#define ARCH_EXIT_REBOOT    3   /**< \brief Reboot the machine */
/** @} */

/** \brief  Set the exit path.

    The default, if you don't call this, is ARCH_EXIT_RETURN.

    \param  path            What arch_exit() should do.
    \see    arch_retpaths
*/
void arch_set_exit_path(int path);

/** \brief  Generic kernel "exit" point.
    \note                   This function will never return!
*/
void arch_exit() __noreturn;

/** \brief  Kernel "return" point.
    \note                   This function will never return!
*/
void arch_return() __noreturn;

/** \brief  Kernel "abort" point.
    \note                   This function will never return!
*/
void arch_abort() __noreturn;

/** \brief  Kernel "reboot" call.
    \note                   This function will never return!
*/
void arch_reboot() __noreturn;

/** \brief Kernel "exit to menu" call.
    \note                   This function will never return!
*/
void arch_menu() __noreturn;

/** \brief  Call to run all ctors. */
void arch_ctors();

/** \brief  Call to run all dtors. */
void arch_dtors();

/** \brief  Hook to ensure linking of crtend.c. */
void __crtend_pullin();

/* These are in mm.c */
/** \brief  Initialize the memory management system.
    \retval 0               On success (no error conditions defined).
*/
int mm_init();

/** \brief  Request more core memory from the system.
    \param  increment       The number of bytes requested.
    \return                 A pointer to the memory.
    \note                   This function will panic if no memory is available.
*/
void * mm_sbrk(unsigned long increment);

/** \brief  Use this macro to determine the level of initialization you'd like
            in your program by default.

    The defaults line will be fine for most things.

    \param  flags           Parts of KOS to init.
*/
#define KOS_INIT_FLAGS(flags)   uint32 __kos_init_flags = (flags)

/** \brief  The init flags. Do not modify this directly! */
extern uint32 __kos_init_flags;

/** \brief  Define a romdisk for your program, if you'd like one.
    \param  rd              Pointer to the romdisk image in your code.
*/
#define KOS_INIT_ROMDISK(rd)    void * __kos_romdisk = (rd)

/** \brief  Built-in romdisk. Do not modify this directly! */
extern void * __kos_romdisk;

/** \brief  State that you don't want a romdisk. */
#define KOS_INIT_ROMDISK_NONE   NULL

/** \defgroup arch_initflags        Available flags for initialization

    These are the flags you can specify with KOS_INIT_FLAGS().
    @{
*/
/** \brief  Default init flags (IRQs on, preemption enabled). */
#define INIT_DEFAULT \
    (INIT_IRQ | INIT_THD_PREEMPT)

#define INIT_NONE           0x0000  /**< \brief Don't init optional things */
#define INIT_IRQ            0x0001  /**< \brief Enable IRQs at startup */
#define INIT_THD_PREEMPT    0x0002  /**< \brief Enable thread preemption */
#define INIT_NET            0x0004  /**< \brief Enable built-in networking */
#define INIT_MALLOCSTATS    0x0008  /**< \brief Enable malloc statistics */
#define INIT_QUIET          0x0010  /**< \brief Disable dbgio */

/* DC-specific stuff */
#define INIT_OCRAM          0x10000 /**< \brief Use half of the dcache as RAM */
#define INIT_NO_DCLOAD      0x20000 /**< \brief Disable dcload */
/** @} */

/* Dreamcast-specific arch init things */
/** \brief  Jump back to the bootloader.

    You generally shouldn't use this function, but rather use arch_exit() or
    exit() instead.

    \note                   This function will never return!
*/
void arch_real_exit() __noreturn;

/** \brief  Initialize bare-bones hardware systems.

    This will be done automatically for you on start by the default arch_main(),
    so you shouldn't have to deal with this yourself.

    \retval 0               On success (no error conditions defined).
*/
int hardware_sys_init();

/** \brief  Initialize some peripheral systems.

    This will be done automatically for you on start by the default arch_main(),
    so you shouldn't have to deal with this yourself.

    \retval 0               On success (no error conditions defined).
*/
int hardware_periph_init();

/** \brief  Shut down hardware that was initted.

    This function will shut down anything initted with hardware_sys_init() and
    hardware_periph_init(). This will be done for you automatically by the
    various exit points, so you shouldn't have to do this yourself.
*/
void hardware_shutdown();

/** \defgroup hw_consoles           Console types

    These are the various console types that can be returned by the
    hardware_sys_mode() function.

    @{
*/
#define HW_TYPE_RETAIL      0x0     /**< \brief A retail Dreamcast. */
#define HW_TYPE_SET5        0x9     /**< \brief A Set5.xx devkit. */
/** @} */

/** \defgroup hw_regions            Region codes

    These are the various region codes that can be returned by the
    hardware_sys_mode() function. Note that a retail Dreamcast will always
    return 0 for the region code. You must read the region of a retail device
    from the flashrom.

    \see    fr_region
    \see    flashrom_get_region()

    @{
*/
#define HW_REGION_UNKNOWN   0x0     /**< \brief Unknown region. */
#define HW_REGION_ASIA      0x1     /**< \brief Japan/Asia (NTSC) */
#define HW_REGION_US        0x4     /**< \brief North America */
#define HW_REGION_EUROPE    0xC     /**< \brief Europe (PAL) */
/** @} */

/** \brief  Retrieve the system mode of the console in use.

    This function retrieves the system mode register of the console that is in
    use. This register details the actual system type in use (and in some system
    types the region of the device).

    \param  region          On return, the region code (one of the
                            \ref hw_regions) of the device if the console type
                            allows reading it through the system mode register
                            -- otherwise, you must retrieve the region from the
                            flashrom.
    \return                 The console type (one of the \ref hw_consoles).
*/
int hardware_sys_mode(int *region);

/* These three aught to be in their own header file at some point, but for now,
   they'll stay here. */

/** \brief  Retrieve the banner printed at program initialization.

    This function retrieves the banner string that is printed at initialization
    time by the kernel. This contains the version of KOS in use and basic
    information about the environment in which it was compiled.

    \retval                 A pointer to the banner string.
*/
const char *kos_get_banner(void);

/** \brief  Retrieve the license information for the compiled copy of KOS.

    This function retrieves a string containing the license terms that the
    version of KOS in use is distributed under. This can be used to easily add
    information to your program to be displayed at runtime.

    \retval                 A pointer to the license terms.
*/
const char *kos_get_license(void);

/** \brief  Retrieve a list of authors and the dates of their contributions.

    This function retrieves the copyright information for the version of KOS in
    use. This function can be used to add such information to the credits of
    programs using KOS to give the appropriate credit to those that have worked
    on KOS.

    Remember, you do need to give credit where credit is due, and this is an
    easy way to do so. ;-)

    \retval                 A pointer to the authors' copyright information.
*/
const char *kos_get_authors(void);

/** \brief  Dreamcast specific sleep mode "function". */
#define arch_sleep() do { \
        __asm__ __volatile__("sleep"); \
    } while(0)

/** \brief  DC specific "function" to get the return address from the current
            function.
    \return                 The return address of the current function.
*/
#define arch_get_ret_addr() ({ \
        uint32 pr; \
        __asm__ __volatile__("sts	pr,%0\n" \
                             : "=&z" (pr) \
                             : /* no inputs */ \
                             : "memory" ); \
        pr; })

/* Please note that all of the following frame pointer macros are ONLY
   valid if you have compiled your code WITHOUT -fomit-frame-pointer. These
   are mainly useful for getting a stack trace from an error. */

/** \brief  DC specific "function" to get the frame pointer from the current
            function.
    \return                 The frame pointer from the current function.
    \note                   This only works if you don't disable frame pointers.
*/
#define arch_get_fptr() ({ \
        uint32 fp; \
        __asm__ __volatile__("mov	r14,%0\n" \
                             : "=&z" (fp) \
                             : /* no inputs */ \
                             : "memory" ); \
        fp; })

/** \brief  Pass in a frame pointer value to get the return address for the
            given frame.

    \param  fptr            The frame pointer to look at.
    \return                 The return address of the pointer.
*/
#define arch_fptr_ret_addr(fptr) (*((uint32*)fptr))

/** \brief  Pass in a frame pointer value to get the previous frame pointer for
            the given frame.

    \param  fptr            The frame pointer to look at.
    \return                 The previous frame pointer.
*/
#define arch_fptr_next(fptr) (*((uint32*)(fptr+4)))

/** \brief  Returns true if the passed address is likely to be valid. Doesn't
            have to be exact, just a sort of general idea.

    \return                 Whether the address is valid or not for normal
                            memory access.
*/
#define arch_valid_address(ptr) ((ptr_t)(ptr) >= 0x8c010000 && (ptr_t)(ptr) < 0x8d000000)

__END_DECLS

#endif  /* __ARCH_ARCH_H */
