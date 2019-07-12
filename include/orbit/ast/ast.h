//===--------------------------------------------------------------------------------------------===
// orbit/ast/ast.h
// This source is part of Orbit - AST
//
// Created on 2017-09-16 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2016-2019 Amy Parent <amy@amyparent.com>
// Available under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#ifndef orbit_ast_ast_h
#define orbit_ast_ast_h

#include <stdio.h>
#include <stdint.h>
#include <orbit/csupport/tokens.h>
#include <orbit/utils/platforms.h>
#include <orbit/utils/memory.h>

typedef struct _OrbitAST OrbitAST;
typedef struct _OrbitASTType OrbitASTType;
typedef enum _OrbitASTTypeFlags OrbitASTTypeFlags;
typedef enum _ASTKind ASTKind;

#define DECL_AST_KIND(name, num) static const ASTKind ORBIT_AST_##name = 1UL << (__COUNTER__)

// We can't use enum because C restricts them to 32 bit. Could probably not rely on bitsets,
// but that allows much faster pattern matching in AST visitors.

enum _ASTKind {
    // MARK: - AST Statements
    ORBIT_AST_ASSIGN,
    ORBIT_AST_CONDITIONAL,
    ORBIT_AST_FOR_IN,
    ORBIT_AST_WHILE,
    ORBIT_AST_BREAK,
    ORBIT_AST_CONTINUE,
    ORBIT_AST_RETURN,
    ORBIT_AST_PRINT,
    ORBIT_AST_BLOCK,
    
    // MARK: - AST Declarations
    ORBIT_AST_DECL_MODULE,
    ORBIT_AST_DECL_FUNC,
    ORBIT_AST_DECL_VAR,
    ORBIT_AST_DECL_STRUCT,
    
    // MARK: - AST Expressions
    ORBIT_AST_EXPR_UNARY,
    ORBIT_AST_EXPR_BINARY,
    ORBIT_AST_EXPR_CALL,
    ORBIT_AST_EXPR_SUBSCRIPT,
    ORBIT_AST_EXPR_CONSTANT,
    ORBIT_AST_EXPR_CONSTANT_INTEGER,
    ORBIT_AST_EXPR_CONSTANT_FLOAT,
    ORBIT_AST_EXPR_CONSTANT_STRING,
    ORBIT_AST_EXPR_CONSTANT_BOOL,
    ORBIT_AST_EXPR_LAMBDA,
    ORBIT_AST_EXPR_NAME,
    ORBIT_AST_EXPR_INIT,
    ORBIT_AST_EXPR_I2F,
    ORBIT_AST_EXPR_F2I,

    // MARK: - Type Expressions
    ORBIT_AST_TYPEEXPR_VOID,
    ORBIT_AST_TYPEEXPR_BOOL,
    ORBIT_AST_TYPEEXPR_INT,
    ORBIT_AST_TYPEEXPR_FLOAT,
    ORBIT_AST_TYPEEXPR_STRING,
    ORBIT_AST_TYPEEXPR_USER,
    ORBIT_AST_TYPEEXPR_ARRAY,
    ORBIT_AST_TYPEEXPR_MAP,
    ORBIT_AST_TYPEEXPR_FUNC,
    ORBIT_AST_TYPEEXPR_ANY,
};

static inline bool orbitASTisStmt(ASTKind kind) {
    return kind >= ORBIT_AST_ASSIGN && kind <= ORBIT_AST_BLOCK;
}

static inline bool orbitASTisDecl(ASTKind kind) {
    return kind >= ORBIT_AST_DECL_MODULE && kind <= ORBIT_AST_DECL_STRUCT;
}

static inline bool orbitASTisExpr(ASTKind kind) {
    return kind >= ORBIT_AST_EXPR_UNARY && kind <= ORBIT_AST_EXPR_F2I;
}

static inline bool orbitASTisType(ASTKind kind) {
    return kind >= ORBIT_AST_TYPEEXPR_VOID;
}

enum _OrbitASTTypeFlags {
    ORBIT_TYPE_CONST      = 1 << 0,
    ORBIT_TYPE_OPTIONAL   = 1 << 1
};

struct _OrbitASTType {
    //OrbitAST*       canonicalType;
    OrbitASTTypeFlags   flags;
    
    union {
        struct {
            OrbitAST*   elementType;
        } arrayType;
    
        struct {
            OrbitAST*   keyType;
            OrbitAST*   elementType;
        } mapType;
    
        struct {
            OrbitAST*   returnType;
            OrbitAST*   params;
        } funcType;
        
        struct {
            OrbitAST*   genericParams;
            OCStringID  symbol;
        } userType;
    };
};

// The TUD (Tagged Union of Doom). Represents all possible nodes in an orbit
// AST. AST::next is used to represent "same level" collections (for example,
// a list of parameters, or a list of expressions.
struct _OrbitAST {
    ORCObject       super;
    ASTKind         kind;
    OrbitAST*       next;
    const OrbitAST* type;
    
    OrbitSRange sourceRange;
    
    struct {
        uint32_t line;
    } debug;
    
    union {
        // --------------------------------------------------------------------
        // Statements
        // --------------------------------------------------------------------
        struct {
            OrbitToken operator;
            OrbitAST* lhs;
            OrbitAST* rhs;
        } assignStmt;
        
        struct {
            OrbitAST*   condition;
            OrbitAST*   ifBody;
            OrbitAST*   elseBody;
        } conditionalStmt;
        
        struct {
            OrbitToken  variable;
            OrbitAST*   collection;
            OrbitAST*   body;
        } forInLoop;
        
        struct {
            OrbitAST*   condition;
            OrbitAST*   body;
        } whileLoop;
        
        struct {
            OrbitAST*   body;
        } block;
        
        struct {
            OrbitAST*   returnValue;
        } returnStmt;
        
        struct {
            OrbitAST*   expr;
        } printStmt;
        
        // --------------------------------------------------------------------
        // Declarations
        // --------------------------------------------------------------------
        struct {
            OCStringID  symbol;
            OrbitAST*   body;
        } moduleDecl;
        
        struct {
            OrbitToken  symbol;
            OCStringID  name;
            OCStringID  mangledName;
            OrbitAST*   returnType;
            OrbitAST*   params;
            OrbitAST*   body;
        } funcDecl;
        
        struct {
            OrbitToken  symbol;
            OCStringID  name;
            OrbitAST*   typeAnnotation;
        } varDecl;
        
        struct {
            OrbitToken  symbol;
            OCStringID  name;
            OrbitAST*   constructor;
            OrbitAST*   destructor;
            OrbitAST*   fields;
        } structDecl;
        
        // --------------------------------------------------------------------
        // Expressions
        // --------------------------------------------------------------------
        struct  {
            OrbitToken  operator;
            OrbitAST*   rhs;
        } unaryExpr;
        
        struct {
            OrbitToken  operator;
            OrbitAST*   lhs;
            OrbitAST*   rhs;
        } binaryExpr;
        
        struct {
            OrbitAST*   symbol;
            OrbitAST*   params;
        } callExpr;
        
        struct {
            OrbitAST*   symbol;
            OrbitAST*   subscript;
        } subscriptExpr;
        
        struct {
            OrbitToken  symbol;
        } constantExpr;
        
        struct {
            OrbitAST*   params;
            OrbitAST*   body;
        } lambdaExpr;
        
        struct {
            OrbitToken  symbol;
            OCStringID  name;
        } nameExpr;
        
        struct {
            OrbitAST*   type;
            OrbitAST*   params;
        } initExpr;
        
        struct {
            OrbitAST*   expr;
        } conversionExpr;
        
        // Type Expressions (necessary for a non-trivial type system)
        
        OrbitASTType    typeExpr;
        
    };
};

void orbitASTPrint(FILE* out, const OrbitAST* ast);
void orbitASTDestroy(void* ref);
OrbitAST* orbitASTMake(ASTKind kind);

#endif /* orbit_ast_ast_h */
