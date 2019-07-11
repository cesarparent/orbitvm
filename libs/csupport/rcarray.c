//===--------------------------------------------------------------------------------------------===
// orbit/utils/rcarray.c
// This source is part of Orbit - Utils
//
// Created on 2017-11-21 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent <amy@amyparent.com>
// Available under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <assert.h>
#include <string.h>
#include <orbit/csupport/rcarray.h>

void orbitRcArrayDeinit(void* ref) {
    ORCArray* array = (ORCArray*)ref;
    for(uint64_t i = 0; i < array->size; ++i) {
        ORCRELEASE(array->data[i]);
    }
    ORBIT_DEALLOC_ARRAY(array->data, sizeof(void*), array->capacity);
}

ORCArray* orbitRcArrayInit(ORCArray* array, uint64_t capacity) {
    assert(array != NULL && "Null instance error");
    
    array->data = ORBIT_ALLOC_ARRAY(void*, capacity);
    ORCINIT(array, &orbitRcArrayDeinit);
    array->size = 0;
    array->capacity = capacity;
    return array;
}

static void _arrayReserve(ORCArray* array, uint64_t newSize) {
    if(newSize < array->capacity) { return; }
    uint64_t oldCapacity = array->capacity;
    while(newSize >= array->capacity)
        array->capacity = ORBIT_GROW_CAPACITY(array->capacity);
    array->data = ORBIT_REALLOC_ARRAY(array->data, void*, oldCapacity, array->capacity);
}

void orbitRcArrayAppend(ORCArray* array, void* item) {
    assert(array != NULL && "Null instance error");
    
    _arrayReserve(array, array->size + 1);
    array->data[array->size] = ORCRETAIN(item);
    array->size += 1;
}

void orbitRcArrayInsert(ORCArray* array, uint64_t index, void* item) {
    assert(array != NULL && "Null instance error");
    assert(index <= array->size && "Index out of range");
    
    _arrayReserve(array, array->size + 1);
    void** insertion = &array->data[index];
    memmove(insertion+1, insertion, sizeof(void*) * (array->size - index));
    array->data[index] = ORCRETAIN(item);
    array->size += 1;
}

void orbitRcArrayRemove(ORCArray* array, uint64_t index) {
    assert(array != NULL && "Null instance error");
    assert(index < array->size && "Index out of range");

    ORCRELEASE(array->data[index]);
    
    void** deletion = &array->data[index];
    memmove(deletion, deletion+1, sizeof(void*) * (array->size - index));
    array->size -= 1;
}

void orbitRcArrayEmpty(ORCArray* array) {
    assert(array != NULL && "Null instance error");
    for(uint64_t i = 0; i < array->size; ++i) {
        ORCRELEASE(array->data[i]);
    }
    array->size = 0;
}

