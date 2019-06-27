//===--------------------------------------------------------------------------------------------===
// orbit/rt2/opcodes.h
// This source is part of Orbit - Runtime
//
// Created on 2016-11-13 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent <amy@amyparent.com>
// Available under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
//  Opcodes used by the Orbit Runtime 2.0. This include is meant to be used as an
//  X-macro (see https://en.wikipedia.org/wiki/X_Macro ). The order of
//  declaration of the opcodes defines how the main interpreter loop's jump
//  table is laid out, and should be changed only if necessary.
//
//  The first parameter is the opcode's mnemonic, the second the number of
//  bytes following the opcode in the bytecode stream that should be read as
//  argument.
//
#ifndef OPCODE
#define OPCODE(_, __, ___)
#endif

OPCODE(OP_RETURN, 0, 0)
OPCODE(OP_PRINT, 0, 0)

OPCODE(OP_CONST, 1, 1)
OPCODE(OP_TRUE, 0, 1)
OPCODE(OP_FALSE, 0, 1)
// OPCODE(OP_ISTORE, 1, -1)
//
    
OPCODE(OP_I2F, 0, 0)
OPCODE(OP_F2I, 0, 0)

OPCODE(OP_IADD, 0, -1)
OPCODE(OP_ISUB, 0, -1)
OPCODE(OP_IMUL, 0, -1)
OPCODE(OP_IDIV, 0, -1)
    
OPCODE(OP_FADD, 0, -1)
OPCODE(OP_FSUB, 0, -1)
OPCODE(OP_FMUL, 0, -1)
OPCODE(OP_FDIV, 0, -1)

OPCODE(OP_IEQ, 0, -1)
OPCODE(OP_ILT, 0, -1)
OPCODE(OP_IGT, 0, -1)
OPCODE(OP_ILTEQ, 0, -1)
OPCODE(OP_IGTEQ, 0, -1)
    
OPCODE(OP_FEQ, 0, -1)
OPCODE(OP_FLT, 0, -1)
OPCODE(OP_FGT, 0, -1)
OPCODE(OP_FLTEQ, 0, -1)
OPCODE(OP_FGTEQ, 0, -1)
//
// OPCODE(OP_RETAIN, 0, 0)
// OPCODE(OP_RELEASE, 0, -1)
