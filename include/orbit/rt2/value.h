//===--------------------------------------------------------------------------------------------===
// value.h - The value API for Orbit Runtime 2.0 (static typed)
// This source is part of Orbit
//
// Created on 2019-06-27 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#ifndef orbit_value_h
#define orbit_value_h
#include <orbit/common.h>

#define ORBIT_VALUE_PACK 1
#if ORBIT_VALUE_PACK
// If ORBIT_VALUE_PACK is defined, we pack all values in 8-byte pointers

/*
The Orbit Runtime 2.0 is statically typed, like the compiler/language itself. However, we want
to be able to enable reflection/type introspection at run time. To keep things simple, we still
have a generic value type used everywhere we hold something.

To keep things fast, we stuff data in pointers if we can. This means we keep values small (8 bytes).
The garbage collector needs to know if a value is a value type (not GC'ed) or a reference type.
Since pointers are (on most platforms) aligned, pointers to OrbitObject will always have their LSB
clear. We can use this to our advantage by setting that bit when a pointer is, in fact, a value.

Orbit value types occupy at most 4 bytes, giving us 3 bytes to use for tagging values with their
primitive type.


*/
typedef union {
    float value;
    int32_t bits;
} FloatBits;

typedef uint64_t OrbitValue;

#define ORBIT_TAG_VALUE 0x00000001
#define ORBIT_TAG_BOOL  (ORBIT_TAG_VALUE | (1 << 1))
#define ORBIT_TAG_INT   (ORBIT_TAG_VALUE | (1 << 2))
#define ORBIT_TAG_FLOAT (ORBIT_TAG_VALUE | (1 << 3))
#define ORBIT_TAG_MAYBE (ORBIT_TAG_VALUE | (1 << 4))

#define ORBIT_IS_REF(value) (((value) & ORBIT_TAG_VALUE) == 0)

#define ORBIT_MASK_REF   (~ORBIT_TAG_VALUE)

#define ORBIT_GET_FLAGS(value) ((value) & 0x0000ffff)

#define ORBIT_FLOAT_BITS(num) (((FloatBits){.value=num}).bits)
#define ORBIT_BITS_FLOAT(val) (((FloatBits){.bits=(val)}).value)

#define ORBIT_VALUE_TRUE ((OrbitValue)(((uintptr_t)1 << 32UL) | ORBIT_TAG_BOOL))
#define ORBIT_VALUE_FALSE ((OrbitValue)(((uintptr_t)0 << 32UL) | ORBIT_TAG_BOOL))
#define ORBIT_VALUE_BOOL(value) ((OrbitValue)(((uintptr_t)((int32_t)value) << 32UL) | ORBIT_TAG_BOOL))
#define ORBIT_VALUE_INT(value) ((OrbitValue)(((uintptr_t)((int32_t)value) << 32UL) | ORBIT_TAG_INT))
#define ORBIT_VALUE_FLOAT(value) ((OrbitValue)(((uintptr_t)ORBIT_FLOAT_BITS(value) << 32UL) | ORBIT_TAG_FLOAT))

#define ORBIT_AS_BOOL(value) ((bool)((value) >> 32))
#define ORBIT_AS_INT(value) ((int32_t)((value) >> 32))
#define ORBIT_AS_FLOAT(value) (ORBIT_BITS_FLOAT(ORBIT_AS_INT(value)))

//#define ORBIT_IS_OBJECT(value) (((value) & ORBIT_VALUE_MASK) == 0)
    //#define ORBIT_IS

#else
typedef struct sOrbitValue OrbitValue;
// typedef int32_t OrbitValue;

struct sOrbitValue {
    enum {
        ORBIT_VK_BOOL,
        ORBIT_VK_INT,
        ORBIT_VK_FLOAT
    } kind;
    union {
        bool boolValue;
        int32_t intValue;
        float floatValue;
    };
};

#define ORBIT_IS_BOOL(value) ((value).kind == ORBIT_VK_BOOL)
#define ORBIT_AS_BOOL(value) ((value).kind == ORBIT_VK_BOOL && (value).boolValue)
#define ORBIT_VALUE_BOOL(value) ((OrbitValue){ .kind=ORBIT_VK_BOOL, .boolValue=(value) })
#define ORBIT_VALUE_TRUE ((OrbitValue){ .kind=ORBIT_VK_BOOL, .boolValue=true })
#define ORBIT_VALUE_FALSE ((OrbitValue){ .kind=ORBIT_VK_BOOL, .boolValue=false })

#define ORBIT_IS_INT(value) ((value).kind == ORBIT_VK_INT)
#define ORBIT_AS_INT(value) ((value).intValue)
#define ORBIT_VALUE_INT(num) ((OrbitValue){ .kind=ORBIT_VK_INT, .intValue=(num) })

#define ORBIT_IS_FLOAT(value) ((value).kind == ORBIT_VK_FLOAT)
#define ORBIT_AS_FLOAT(value) ((value).floatValue)
#define ORBIT_VALUE_FLOAT(num) ((OrbitValue){ .kind=ORBIT_VK_FLOAT, .floatValue=(num) })

#endif


#endif