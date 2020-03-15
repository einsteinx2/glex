/* KallistiOS ##version##

   include/kos/genwait.h
   Copyright (C) 2003 Dan Potter
   Copyright (C) 2012 Lawrence Sebald

*/

/** \file   kos/genwait.h
    \brief  Generic wait system.

    The generic wait system in KOS, like many other portions of KOS, is based on
    an idea from the BSD kernel. It allows you to sleep on any random object and
    later wake up any threads that happen to be sleeping on thta object. All of
    KOS' sync primatives (other than spinlocks) are based on this concept, and
    it can be used for some fairly useful things.

    \author Dan Potter
    \author Lawrence Sebald
*/

#ifndef __KOS_GENWAIT_H
#define __KOS_GENWAIT_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <kos/thread.h>

/** \brief  Sleep on an object.

    This function sleeps on the specified object. You are not allowed to call
    this function inside an interrupt.

    \param  obj             The object to sleep on
    \param  mesg            A message to show in the status
    \param  timeout         If not woken before this many milliseconds have
                            passed, wake up anyway
    \param  callback        If non-NULL, call this function with obj as its
                            argument if the wait times out (but before the
                            calling thread has been woken back up)
    \retval 0               On successfully being woken up (not by timeout)
    \retval -1              On error or being woken by timeout

    \par    Error Conditions:
    \em     EAGAIN - on timeout
*/
int genwait_wait(void * obj, const char * mesg, int timeout, void (*callback)(void *));

/* Wake up N threads waiting on the given object. If cnt is <=0, then we
   wake all threads. Returns the number of threads actually woken. */
/** \brief  Wake up a number of threads sleeping on an object.

    This function wakes up the specified number of threads sleeping on the
    object specified.

    \param  obj             The object to wake threads that are sleeping on it
    \param  cnt             The number of threads to wake, if <= 0, wake all
    \param  err             The errno code to set as the errno value on the
                            woken threads. If this is 0 (EOK), then the thread's
                            errno will not be changed, and the thread will get a
                            return value of 0 from the genwait_wait(). If it is
                            non-zero, the thread will get a return value of -1
                            and errno will be set to this value for the woken
                            threads.
    \return                 The number of threads woken
*/
int genwait_wake_cnt(void * obj, int cnt, int err);

/** \brief  Wake up all threads sleeping on an object.

    This function simply calls genwait_wake_cnt(obj, -1, 0).

    \param  obj             The object to wake threads that are sleeping on it
    \see    genwait_wake_cnt()
*/
void genwait_wake_all(void * obj);

/** \brief  Wake up one thread sleeping on an object.

    This function simply calls genwait_wake_cnt(obj, 1, 0).

    \param  obj             The object to wake threads that are sleeping on it
    \see    genwait_wake_cnt()
*/
void genwait_wake_one(void * obj);

/** \brief  Wake up all threads sleeping on an object, with an error.

    This function simply calls genwait_wake_cnt(obj, -1, err).

    \param  obj             The object to wake threads that are sleeping on it
    \param  err             The value to set in the threads' errno values
    \see    genwait_wake_cnt()
*/
void genwait_wake_all_err(void *obj, int err);

/** \brief  Wake up one thread sleeping on an object, with an error.
 
    This function simply calls genwait_wake_cnt(obj, 1, err).
 
    \param  obj             The object to wake threads that are sleeping on it
    \param  err             The value to set in the threads' errno values
    \see    genwait_wake_cnt()
*/
void genwait_wake_one_err(void *obj, int err);

/** \brief  Wake up a specific thread that is sleeping on an object.

    This function wakes up the specfied thread, assuming it is sleeping on the
    specified object.

    \param  obj             The object to wake the thread from
    \param  thd             The specific thread to wake
    \param  err             The errno code to set as the errno value on the
                            woken thread. If this is 0 (EOK), then the thread's
                            errno will not be changed, and the thread will get a
                            return value of 0 from the genwait_wait(). If it is
                            non-zero, the thread will get a return value of -1
                            and errno will be set to this value for the woken
                            threads.
    \return                 The number of threads woken, which should be 1 on
                            success.
*/
int genwait_wake_thd(void *obj, kthread_t *thd, int err);

/** \brief  Look for timed out genwait_wait() calls.

    There should be no reason you need to call this function, it is called
    internally by the scheduler for you.

    \param  now             The current system time, in milliseconds since boot
*/
void genwait_check_timeouts(uint64 now);

/** \brief  Look for the next timeout event time.

    This function looks up when the next genwait_wait() call will timeout. This
    function is for the internal use of the scheduler, and should not be called
    from user code.

    \return                 The next timeout time in milliseconds since boot, or
                            0 if there are no pending genwait_wait() calls
*/
uint64 genwait_next_timeout();

/** \cond */
/* Initialize the genwait system */
int genwait_init();

/* Shut down the genwait system */
void genwait_shutdown();
/** \endcond */


__END_DECLS

#endif  /* __KOS_GENWAIT_H */

