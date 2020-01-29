/* KallistiOS ##version##

   dc/fmath_base.h
   Copyright (C) 2001 Andrew Kieschnick
   Copyright (C) 2014 Josh Pearson

*/

#ifndef __DC_FMATH_BASE_H
#define __DC_FMATH_BASE_H

#include <sys/cdefs.h>
__BEGIN_DECLS

/**
    \file dc/fmath_base.h
    \brief  Base definitions for the DC's special math instructions
    \author Andrew Kieschnick
    \author Josh Pearson
*/

/** PI constant (if you don't want full math.h) */
#define F_PI 3.1415926f

/** \cond */
#define __fsin(x) \
    ({ float __value, __arg = (x), __scale = 10430.37835; \
        __asm__("fmul   %2,%1\n\t" \
                "ftrc   %1,fpul\n\t" \
                "fsca   fpul,dr0\n\t" \
                "fmov   fr0,%0" \
                : "=f" (__value), "+&f" (__scale) \
                : "f" (__arg) \
                : "fpul", "fr0", "fr1"); \
        __value; })

#define __fcos(x) \
    ({ float __value, __arg = (x), __scale = 10430.37835; \
        __asm__("fmul   %2,%1\n\t" \
                "ftrc   %1,fpul\n\t" \
                "fsca   fpul,dr0\n\t" \
                "fmov   fr1,%0" \
                : "=f" (__value), "+&f" (__scale) \
                : "f" (__arg) \
                : "fpul", "fr0", "fr1"); \
        __value; })

#define __ftan(x) \
    ({ float __value, __arg = (x), __scale = 10430.37835; \
        __asm__("fmul   %2,%1\n\t" \
                "ftrc   %1,fpul\n\t" \
                "fsca   fpul,dr0\n\t" \
                "fdiv   fr1, fr0\n\t" \
                "fmov   fr0,%0" \
                : "=f" (__value), "+&f" (__scale) \
                : "f" (__arg) \
                : "fpul", "fr0", "fr1"); \
        __value; })


#define __fisin(x) \
    ({ float __value, __arg = (x); \
        __asm__("lds    %1,fpul\n\t" \
                "fsca   fpul,dr0\n\t" \
                "fmov   fr0,%0" \
                : "=f" (__value) \
                : "r" (__arg) \
                : "fpul", "fr0", "fr1"); \
        __value; })

#define __ficos(x) \
    ({ float __value, __arg = (x); \
        __asm__("lds    %1,fpul\n\t" \
                "fsca   fpul,dr0\n\t" \
                "fmov   fr1,%0" \
                : "=f" (__value) \
                : "r" (__arg) \
                : "fpul", "fr0", "fr1"); \
        __value; })

#define __fitan(x) \
    ({ float __value, __arg = (x); \
        __asm__("lds    %1,fpul\n\t" \
                "fsca   fpul,dr0\n\t" \
                "fdiv   fr1, fr0\n\t" \
                "fmov   fr0,%0" \
                : "=f" (__value) \
                : "r" (__arg) \
                : "fpul", "fr0", "fr1"); \
        __value; })

#define __fsincos(r, s, c) \
    ({  register float __r __asm__("fr10") = r; \
        register float __a __asm__("fr11") = 182.04444443; \
        __asm__("fmul fr11, fr10\n\t" \
                "ftrc fr10, fpul\n\t" \
                "fsca fpul, dr10\n\t" \
                : "+f" (__r), "+f" (__a) \
                : "0" (__r), "1" (__a) \
                : "fpul"); \
        s = __r; c = __a; })

#define __fsincosr(r, s, c) \
    ({  register float __r __asm__("fr10") = r; \
        register float __a __asm__("fr11") = 10430.37835; \
        __asm__("fmul fr11, fr10\n\t" \
                "ftrc fr10, fpul\n\t" \
                "fsca fpul, dr10\n\t" \
                : "+f" (__r), "+f" (__a) \
                : "0" (__r), "1" (__a) \
                : "fpul"); \
        s = __r; c = __a; })

#define __fsqrt(x) \
    ({ float __arg = (x); \
        __asm__("fsqrt %0\n\t" \
                : "=f" (__arg) : "0" (__arg)); \
        __arg; })

#define __frsqrt(x) \
    ({ float __arg = (x); \
        __asm__("fsrra %0\n\t" \
                : "=f" (__arg) : "0" (__arg)); \
        __arg; })

/* Floating point inner product (dot product) */
#define __fipr(x, y, z, w, a, b, c, d) ({ \
        register float __x __asm__("fr0") = (x); \
        register float __y __asm__("fr1") = (y); \
        register float __z __asm__("fr2") = (z); \
        register float __w __asm__("fr3") = (w); \
        register float __a __asm__("fr4") = (a); \
        register float __b __asm__("fr5") = (b); \
        register float __c __asm__("fr6") = (c); \
        register float __d __asm__("fr7") = (d); \
        __asm__ __volatile__( \
                              "fipr	fv4,fv0" \
                              : "+f" (__w) \
                              : "f" (__x), "f" (__y), "f" (__z), "f" (__w), \
                              "f" (__a), "f" (__b), "f" (__c), "f" (__d) \
                            ); \
        __w; })

/* Floating point inner product w/self (square of vector magnitude) */
#define __fipr_magnitude_sqr(x, y, z, w) ({ \
        register float __x __asm__("fr4") = (x); \
        register float __y __asm__("fr5") = (y); \
        register float __z __asm__("fr6") = (z); \
        register float __w __asm__("fr7") = (w); \
        __asm__ __volatile__( \
                              "fipr	fv4,fv4" \
                              : "+f" (__w) \
                              : "f" (__x), "f" (__y), "f" (__z), "f" (__w) \
                            ); \
        __w; })

/** \endcond */
__END_DECLS

#endif /* !__DC_FMATH_BASE_H */
