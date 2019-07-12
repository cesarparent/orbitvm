//===--------------------------------------------------------------------------------------------===
// errors.c - Semantics checker error wrappers.
// This source is part of Orbit
//
// Created on 2019-07-02 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "errors.h"
#include <orbit/ast/diag.h>

static inline OrbitDiagManager* diag(Sema* self) {
    return &self->context->diagnostics;
}


void errorNotCallable(Sema* self, const OrbitAST* call) {
    OrbitSRange rng = call->callExpr.symbol->sourceRange;
    OrbitDiagID id = orbitDiagError(
        diag(self), rng.start,
        "cannot invoke a value of type '$0'", 1,
        ORBIT_DIAG_TYPE(call->callExpr.symbol->type)
    );
    orbitDiagAddSourceRange(id, rng);
}

void errorInvalidCall(Sema* self, const OrbitAST* call) {
    OrbitSRange rng = call->sourceRange;
    OrbitDiagID id = orbitDiagError(
        diag(self), rng.start,
        "cannot invoke a value of type '$0' with these arguments", 1,
        ORBIT_DIAG_TYPE(call->callExpr.symbol->type)
    );
    orbitDiagAddSourceRange(id, rng);
}

void errorAssign(Sema* self, const OrbitAST* expr) {
    OrbitSRange rng = expr->sourceRange;
    OrbitSLoc loc = expr->binaryExpr.operator.sourceLoc;
    OrbitDiagID id = orbitDiagError(
        diag(self), loc,
        "cannot assign '$0' with a value of type '$1'", 2,
        ORBIT_DIAG_TYPE(expr->binaryExpr.lhs->type),
        ORBIT_DIAG_TYPE(expr->binaryExpr.rhs->type)
    );
    orbitDiagAddSourceRange(id, rng);
}

void errorNameLookup(Sema* self, const OrbitAST* expr) {
    OrbitSRange rng = expr->sourceRange;
    OrbitDiagID id = orbitDiagError(
        diag(self), rng.start,
        "no values named '$0' found", 1,
        ORBIT_DIAG_STRING(expr->nameExpr.name)
    );
    orbitDiagAddSourceRange(id, rng);
}

void errorNoType(Sema* self, const OrbitAST* var) {
    OrbitSRange rng = var->sourceRange;
    OrbitDiagID id = orbitDiagError(
        diag(self), rng.start,
        "The type of '$0' cannot be deduced, add a type annotation or assign the variable", 1,
        ORBIT_DIAG_STRING(var->varDecl.name)
    );
    orbitDiagAddSourceRange(id, rng);
}

static void previousDeclNote(Sema* self, const OrbitAST* prev) {
    OrbitSRange rng = prev->sourceRange;
    OrbitDiagID id = orbitDiagInfo(
        diag(self), rng.start, "the previous declaration of '$0' is here", 1,
        ORBIT_DIAG_STRING(prev->varDecl.name)
    );
    orbitDiagAddSourceRange(id, rng);
}

void errorAlreadyDeclared(Sema* self, const OrbitAST* decl, const OrbitAST* prev) {
    OrbitSRange rng = decl->sourceRange;
    OrbitDiagID id = orbitDiagError(
        diag(self), rng.start,
        "the name '$0' is already declared in this context", 1,
        ORBIT_DIAG_STRING(decl->varDecl.name)
    );
    orbitDiagAddSourceRange(id, rng);
    if(prev) previousDeclNote(self, prev);
}

void errorBinary(Sema* self, const OrbitAST* expr) {
    OrbitSRange rng = expr->sourceRange;
    OrbitSLoc loc = expr->binaryExpr.operator.sourceLoc;
    OrbitDiagID id = orbitDiagError(
        diag(self), loc,
        "invalid binary expression operands: $0 $1 $2 is not declared", 3,
        ORBIT_DIAG_TYPE(expr->binaryExpr.lhs->type),
        ORBIT_DIAG_CSTRING(orbitTokenString(expr->binaryExpr.operator.kind)),
        ORBIT_DIAG_TYPE(expr->binaryExpr.rhs->type)
    );
    orbitDiagAddSourceRange(id, rng);
}


void errorCondition(Sema* self, const char* type, const OrbitAST* expr) {
    OrbitSRange rng = expr->sourceRange;
    OrbitDiagID id = orbitDiagError(
        diag(self), rng.start,
        "$0 condition is of type '$1' instead of Bool", 2,
        ORBIT_DIAG_CSTRING(type),
        ORBIT_DIAG_TYPE(expr->type)
    );
    orbitDiagAddSourceRange(id, rng);
}

void warnUnimplemented(Sema* self, const OrbitAST* node) {
    OrbitSRange rng = node->sourceRange;
    orbitDiagWarn(
        diag(self), rng.start, "language feature not implemented", 0
    );
    //orbitDiagAddSourceRange(id, rng);
}
