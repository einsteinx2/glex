/* KallistiOS ##version##

   arch/dreamcast/include/timer.h
   (c)2000-2001 Dan Potter

*/

/** \file   arch/timer.h
    \brief  Low-level timer functionality.

    This file contains functions for interacting with the timer sources on the
    SH4. Many of these functions may interfere with thread operation or other
    such things, and should thus be used with caution. Basically, the only
    functionality that you might use in practice in here in normal programs is
    the gettime functions.

    \author Dan Potter
*/

#ifndef __ARCH_TIMER_H
#define __ARCH_TIMER_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <arch/irq.h>

/* Timer sources -- we get four on the SH4 */

/** \brief  SH4 Timer 0.

    This timer is used for thread operation, and thus is off limits if you want
    that to work properly.
*/
#define TMU0    0

/** \brief  SH4 Timer 1.

    This timer is used for the timer_spin_sleep() function.
*/
#define TMU1    1

/** \brief  SH4 Timer 2.

    This timer is used by the various gettime functions in this header.
*/
#define TMU2    2

/** \brief  SH4 Watchdog Timer.

    KallistiOS does not currently support using this timer.
*/
#define WDT 3

/** \brief  Which timer does the thread system use? */
#define TIMER_ID TMU0

/** \brief  Pre-initialize a timer, but do not start it.

    This function sets up a timer for use, but does not start it.

    \param  which           The timer to set up (i.e, \ref TMU0).
    \param  speed           The number of ticks per second.
    \param  interrupts      Set to 1 to receive interrupts when the timer ticks.
    \retval 0               On success.
*/
int timer_prime(int which, uint32 speed, int interrupts);

/** \brief  Start a timer.

    This function starts a timer that has been initialized with timer_prime(),
    starting raising interrupts if applicable.

    \param  which           The timer to start (i.e, \ref TMU0).
    \retval 0               On success.
*/
int timer_start(int which);

/** \brief  Stop a timer.

    This function stops a timer that was started with timer_start(), and as a
    result stops interrupts coming in from the timer.

    \param  which           The timer to stop (i.e, \ref TMU0).
    \retval 0               On success.
*/
int timer_stop(int which);

/** \brief  Obtain the count of a timer.

    This function simply returns the count of the timer.

    \param  which           The timer to inspect (i.e, \ref TMU0).
    \return                 The timer's count.
*/
uint32 timer_count(int which);

/** \brief  Clear the underflow bit of a timer.

    This function clears the underflow bit of a timer if it was set.

    \param  which           The timer to inspect (i.e, \ref TMU0).
    \retval 0               If the underflow bit was clear (prior to calling).
    \retval 1               If the underflow bit was set (prior to calling).
*/
int timer_clear(int which);

/** \brief  Spin-loop sleep function.

    This function is meant as a very accurate delay function, even if threading
    and interrupts are disabled. It uses \ref TMU1 to sleep.

    \param  ms              The number of milliseconds to sleep.
*/
void timer_spin_sleep(int ms);

/** \brief  Enable high-priority timer interrupts.

    This function enables interrupts on the specified timer.

    \param  which           The timer to enable interrupts on (i.e, \ref TMU0).
*/
void timer_enable_ints(int which);

/** \brief  Disable timer interrupts.

    This function disables interrupts on the specified timer.

    \param  which           The timer to disable interrupts on (i.e, \ref TMU0).
*/
void timer_disable_ints(int which);

/** \brief  Check whether interrupts are enabled on a timer.

    This function checks whether or not interrupts are enabled on the specified
    timer.

    \param  which           The timer to inspect (i.e, \ref TMU0).
    \retval 0               If interrupts are disabled on the timer.
    \retval 1               If interrupts are enabled on the timer.
*/
int timer_ints_enabled(int which);

/** \brief  Enable the millisecond timer.

    This function enables the timer used for the gettime functions. This is on
    by default. These functions use \ref TMU2 to do their work.
*/
void timer_ms_enable();

/** \brief  Disable the millisecond timer.

    This function disables the timer used for the gettime functions. Generally,
    you will not want to do this, unless you have some need to use the timer
    \ref TMU2 for something else.
*/
void timer_ms_disable();

/** \brief  Get the current uptime of the system.

    This function retrieves the number of seconds and milliseconds since KOS was
    started.

    \param  secs            A pointer to store the number of seconds since boot
                            into.
    \param  msecs           A pointer to store the number of milliseconds past
                            a second since boot.
    \note                   To get the total number of milliseconds since boot,
                            calculate (*secs * 1000) + *msecs, or use the
                            timer_ms_gettime64() function.
*/
void timer_ms_gettime(uint32 *secs, uint32 *msecs);

/** \brief  Get the current uptime of the system (in milliseconds).

    This function retrieves the number of milliseconds since KOS was started. It
    is equivalent to calling timer_ms_gettime() and combining the number of
    seconds and milliseconds into one 64-bit value.

    \return                 The number of milliseconds since KOS started.
*/
uint64 timer_ms_gettime64();

/** \brief  Get the current uptime of the system (in microseconds).

    This function retrieves the number of microseconds since KOS was started. It
    should be more precise, in theory, than timer_ms_gettime64(), but the exact
    amount of preciseness is undetermined.

    \return                 The number of microseconds since KOS started.
*/
uint64 timer_us_gettime64();

/** \brief  Primary timer callback type. */
typedef void (*timer_primary_callback_t)(irq_context_t *);

/** \brief  Set the primary timer callback.

    This function sets the primary timer callback to the specified function
    pointer. Generally, you should not do this, as the threading system relies
    on the primary timer to work.

    \param  callback        The new timer callback (set to NULL to disable).
    \return                 The old timer callback.
*/
timer_primary_callback_t timer_primary_set_callback(timer_primary_callback_t callback);

/** \brief  Request a primary timer wakeup.

    This function will wake the caller (by calling the primary timer callback)
    in approximately the number of milliseconds specified. You can only have one
    timer wakeup scheduled at a time. Any subsequently scheduled wakeups will
    replace any existing one.

    \param  millis          The number of milliseconds to schedule for.
*/
void timer_primary_wakeup(uint32 millis);

/* \cond */
/* Init function */
int timer_init();

/* Shutdown */
void timer_shutdown();
/* \endcond */

__END_DECLS

#endif  /* __ARCH_TIMER_H */

