//
//  orbit/sema/type.c
//  Orbit - Sema
//
//  Created by Amy Parent on 2017-10-10.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include <orbit/parser/tokens.h>
#include <orbit/sema/type.h>
#include <orbit/type/type.h>
#include <orbit/type/builders.h>
#include <orbit/ast/traversal.h>

static Type* sema_extractSimpleType(OCToken token) {
    switch(token.type) {
    case TOKEN_NIL:
        return type_make(TYPE_NIL, false);
    case TOKEN_ANY:
        return type_make(TYPE_ANY, false);
    case TOKEN_NUMBER:
        return type_make(TYPE_NUMBER, false);
    case TOKEN_STRING:
        return type_make(TYPE_STRING, false);
    case TOKEN_VOID:
        return type_make(TYPE_VOID, false);
    default:
        // TODO: throw error, invalid token
        break;
    }
    return NULL;
}

static Type* sema_extractFuncType(AST* ast) {
    Type* returnType = sema_extractType(ast->funcType.returnType);
    Type* paramList = NULL;
    Type** next = &paramList;
    AST* param = ast->funcDecl.params;
    
    while(param != NULL) {
        Type* t = sema_extractType(param);
        *next = t;
        next = &(t->next);
        param = param->next;
    }
    
    return type_makeFunction(returnType, paramList);
}

Type* sema_extractType(AST* ast) {
    if(ast == NULL) { return NULL; }
    
    switch(ast->kind) {
    case AST_TYPEEXPR_SIMPLE:
        return sema_extractSimpleType(ast->simpleType.symbol);
    case AST_TYPEEXPR_ARRAY:
        return type_makeArray(sema_extractType(ast->arrayType.elementType));
    case AST_TYPEEXPR_MAP:
        return type_makeMap(sema_extractType(ast->mapType.keyType),
                            sema_extractType(ast->mapType.elementType));
    case AST_TYPEEXPR_FUNC:
        return sema_extractFuncType(ast);
    default:
        // TODO: Throw error here
        break;
    }
    return NULL;
}

void sema_extractVariableTypes(AST* ast, ASTKind filter) {
    if(ast->varDecl.typeAnnotation == NULL) { return; }
    ast->type = sema_extractType(ast->varDecl.typeAnnotation);
}

void sema_extractFunctionTypes(AST* ast, ASTKind filter) {
    Type* returnType = NULL;
    if(ast->funcDecl.returnType == NULL) {
        returnType = type_make(TYPE_VOID, false);
    } else {
        returnType = sema_extractType(ast->funcDecl.returnType);
    }
    
    AST* param = ast->funcDecl.params;
    Type* paramList = NULL;
    Type** next = &paramList;
    
    while(param != NULL) {
        Type* t = type_copy(param->type);
        if(t != NULL) {
            *next = t;
            next = &(t->next);
        }
        param = param->next;
    }
    ast->type = type_makeFunction(returnType, paramList);
}

void sema_extractLiteralTypes(AST* ast, ASTKind filter) {
    switch(ast->kind) {
    case AST_EXPR_CONSTANT_INTEGER:
    case AST_EXPR_CONSTANT_FLOAT:
        ast->type = type_make(TYPE_NUMBER, true);
    case AST_EXPR_CONSTANT_STRING:
        ast->type = type_make(TYPE_STRING, true);
        default: break;
    }
}

void sema_extractCallTypes(AST* ast, ASTKind filter) {
    // TODO: implementation
}

void sema_extractNameTypes(AST* ast, ASTKind filter) {
    // TODO: implementation
}

void sema_extractUnaryTypes(AST* ast, ASTKind filter) {
    // TODO: implementation
}

void sema_extractBinaryTypes(AST* ast, ASTKind filter) {
   // TODO: implementation 
}

void sema_inferVariableTypes(AST* ast, ASTKind filter) {
    // TODO: implementation
}

void sema_runTypeAnalysis(AST* ast) {
    ast_traverse(ast,
                 AST_EXPR_CONSTANT_INTEGER | AST_EXPR_CONSTANT_FLOAT | AST_EXPR_CONSTANT_STRING,
                 &sema_extractLiteralTypes);
    ast_traverse(ast, AST_DECL_VAR, &sema_extractVariableTypes);
    ast_traverse(ast, AST_DECL_FUNC, &sema_extractFunctionTypes);
}

