//===--------------------------------------------------------------------------------------------===
// orbit/utils/memory.c
// This source is part of Orbit - Utils
//
// Created on 2017-10-23 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent <amy@amyparent.com>
// Available under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <stdio.h>
#include <stdlib.h>
#include <orbit/utils/memory.h>
#include <assert.h>

noreturn void orbitDie(const char* message) {
    fprintf(stderr, "fatal error: %s\n", message);
    abort();
}

static size_t total_mem = 0;

void* orbitAllocator(void* ptr, size_t oldSize, size_t newSize) {
    total_mem += (newSize - oldSize);
    
    if(newSize == 0) {
        free(ptr);
        return NULL;
    }
    
    void* mem = realloc(ptr, newSize);
    if(!mem) orbitDie("Error reallocating memory");
    return mem;
}
