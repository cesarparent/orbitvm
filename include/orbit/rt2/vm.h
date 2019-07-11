//===--------------------------------------------------------------------------------------------===
// vm.h - The Orbit Runtime 2.0 virtual machine
// This source is part of Orbit
//
// Created on 2019-06-27 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#ifndef orbit_vm_h
#define orbit_vm_h
#include <orbit/common.h>
#include <orbit/rt2/garbage.h>
#include <orbit/rt2/value.h>
#include <orbit/rt2/invocation.h>


typedef enum {
    ORBIT_OK,
    ORBIT_RUNTIME_ERROR,
    ORBIT_COMPILE_ERROR,
} OrbitResult;

#define ORBIT_STACK_MAX 256

struct sOrbitVM {
    OrbitFunction* function;
    // uint8_t* ip;
    // OrbitValue* sp;
    // OrbitValue stack[ORBIT_STACK_MAX];
    
    OrbitTask* task;
    OrbitGC gc;
};

void orbitVMInit(OrbitVM* self);
void orbitVMDeinit(OrbitVM* self);
void orbitDebugTOS(OrbitVM* self);
void orbitDebugStack(OrbitVM* self);

// OrbitResult orbitInterpret(const char* source);
OrbitResult orbitRun(OrbitVM* vm, OrbitFunction* function);


#endif /* orbit_vm_h */
