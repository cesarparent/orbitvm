//===--------------------------------------------------------------------------------------------===
// orbit/ast/diag.h - Orbit's compiler diagnostic API
// This source is part of Orbit - AST
//
// Created on 2018-05-08 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent <amy@amyparent.com>
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#ifndef orbit_ast_diag_h
#define orbit_ast_diag_h
#include <stdint.h>
#include <orbit/ast/ast.h>
#include <orbit/csupport/source.h>
#include <orbit/csupport/string.h>

#define ORBIT_DIAG_MAXCOUNT   64

typedef enum _OrbitDiagLevel        OrbitDiagLevel;
typedef struct _OrbitDiagManager    OrbitDiagManager;
typedef struct _OrbitDiag           OrbitDiag;
typedef struct _OrbitDiagArg        OrbitDiagArg;


typedef struct {
    OrbitDiagManager* manager;
    uint32_t id;
} OrbitDiagID;

typedef void (*OrbitDiagConsumer)(OrbitSource* source, OrbitDiag*);

enum _OrbitDiagLevel {
    ORBIT_DIAGLEVEL_INFO    = 0,
    ORBIT_DIAGLEVEL_WARN    = 1,
    ORBIT_DIAGLEVEL_ERROR   = 2,
};

struct _OrbitDiagArg {
    enum { ORBIT_DPK_INT, ORBIT_DPK_STRING, ORBIT_DPK_CSTRING, ORBIT_DPK_TYPE } kind;
    union {
        int         intValue;
        const char* cstringValue;
        OCStringID  stringValue;
        OrbitAST*   typeValue;
    };
};

struct _OrbitDiagManager {
    OrbitSource*        source;
    OrbitDiagConsumer   consumer;
    uint32_t            errorCount;
    uint32_t            diagnosticCount;
    void*               diagnostics;
};

#define ORBIT_DIAG_INT(val)     ((OrbitDiagArg){.kind=ORBIT_DPK_INT, .intValue=(val)})
#define ORBIT_DIAG_FLOAT(val)   ((OrbitDiagArg){.kind=ORBIT_DPK_FLOAT, .floatValue=(val)})
#define ORBIT_DIAG_STRING(val)  ((OrbitDiagArg){.kind=ORBIT_DPK_STRING, .stringValue=(val)})
#define ORBIT_DIAG_CSTRING(val) ((OrbitDiagArg){.kind=ORBIT_DPK_CSTRING, .cstringValue=(val)})
#define ORBIT_DIAG_TYPE(val)    ((OrbitDiagArg){.kind=ORBIT_DPK_TYPE, .typeValue=(val)})

extern OrbitDiagManager orbit_defaultDiagManager;

void orbit_diagManagerInit(OrbitDiagManager* manager, OrbitSource* source);
void orbit_diagManagerDeinit(OrbitDiagManager* manager);

OrbitDiagID orbit_diagError(OrbitDiagManager* manager, OrbitSLoc loc, const char* fmt, int n, ...);
OrbitDiagID orbit_diagNew(OrbitDiagManager* manager, OrbitDiagLevel level, const char* format);
void orbit_diagAddParam(OrbitDiagID id, OrbitDiagArg param);
void orbit_diagAddSourceLoc(OrbitDiagID id, OrbitSLoc loc);
void orbit_diagAddSourceRange(OrbitDiagID id, OrbitSRange range);

void orbit_diagEmitAll(OrbitDiagManager* manager);
void orbit_diagEmitAbove(OrbitDiagManager* manager, OrbitDiagLevel level);

char* orbit_diagGetFormat(OrbitDiag* diag);
OrbitSLoc orbit_diagGetLoc(OrbitDiag* diag);
OrbitSRange orbit_diagGetRange(OrbitDiag* diag);


#endif /* orbit_ast_diag_h */
