//===--------------------------------------------------------------------------------------------===
// builder.h - Builder is the API used to emit Orbit bytecode
// This source is part of Orbit
//
// Created on 2019-07-04 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#ifndef orbit_builder_h
#define orbit_builder_h
#include <orbit/ast/context.h>
#include <orbit/rt2/garbage.h>
#include <orbit/rt2/value.h>
#include <orbit/rt2/value_object.h>
#include <orbit/rt2/opcodes.h>
#include <orbit/rt2/buffer.h>
#include <orbit/rt2/memory.h>
#include <orbit/utils/memory.h>
#include <orbit/csupport/rcmap.h>
#include <orbit/common.h>

typedef struct {
    OrbitTokenKind op;
    ASTKind lhsType;
    ASTKind rhsType;
    OrbitCode instruction;
} OpSelectData;

DECLARE_BUFFER(Selector, OpSelectData);

typedef struct Function {
    ORCObject base;
    // This is going to get tricky *really* fast with upvalues and all.
    struct Function* parent;
    
    OrbitFunction* impl;
    OCStringID locals[256];
    int localCount;
    int maxLocals;
} Function;

typedef struct {
    OrbitGC* gc;
    // OrbitFunction* function;
    OrbitASTContext* context;
    OrbitSelectorBuffer selector;
    
    Function* function;
    
    const OrbitAST* current;
} Builder;


void builderInit(Builder* builder, OrbitGC* gc);
void builderDeinit(Builder* builder);

int findConstant(OrbitValueBuffer* constants, OrbitValue value);
uint8_t emitConstant(Builder* builder, OrbitValue value);

int openScope(Builder* builder);
void dropScope(Builder* builder, int stack);
void openFunction(Builder* builder);
void openFunctionGC(Builder* builder, OrbitFunction* function);
int localVariable(Builder* builder, OCStringID name);
void closeFunction(Builder* builder);

int offset(Builder* builder);
int emitInst(Builder* builder, OrbitCode code);
int emitLocalInst(Builder* builder, OrbitCode code, OCStringID name);
int emitConstInst(Builder* builder, OrbitCode code, OrbitValue value);
int emitJump(Builder* builder, OrbitCode code);
void patchJump(Builder* builder, int patch);
int emitRJump(Builder* builder, OrbitCode code, int target);

OrbitCode instSelect(Builder* builder, OrbitTokenKind op, const OrbitAST* lhs, const OrbitAST* rhs);

#endif