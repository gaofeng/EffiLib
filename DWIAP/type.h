
#ifndef _TYPE_H
#define _TYPE_H

typedef unsigned long long          u64;
typedef unsigned long               u32;
typedef unsigned short              u16;
typedef unsigned char               u8;

typedef signed long long            s64;
typedef signed long                 s32;
typedef signed short                s16;
typedef signed char                 s8;

#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif
typedef enum { FALSE = 0, TRUE = !FALSE }bool;

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#endif
