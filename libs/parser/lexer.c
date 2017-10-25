//
//  orbit/parser/lexer.c
//  Orbit - Parser
//
//  Created by Amy Parent on 2017-03-01.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <inttypes.h>
#include <orbit/console/console.h>
#include <orbit/parser/lexer.h>
#include <orbit/utils/wcwidth.h>


static void _lexerError(OCLexer* lexer, const char* fmt, ...) {
    OASSERT(lexer != NULL, "Null instance error");
    
    fprintf(stderr, "%s:%"PRIu64":%"PRIu64": error: ",
                     lexer->source.path,
                     lexer->line,
                     lexer->column);
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
    fprintf(stderr, "\n");
    console_printTokenLine(stderr, lexer->currentToken);
    fprintf(stderr, "\n");
}

void lexer_init(OCLexer* lexer, OCSource source) {
    OASSERT(lexer != NULL, "Null instance error");
    
    lexer->source = source;

    lexer->linePtr = source.bytes;
    lexer->currentPtr = source.bytes;
    lexer->currentChar = 0;
    
    lexer->startOfLine = true;
    lexer->column = 0;
    lexer->line = 1;
    
    lexer->string.buffer = NULL;
    lexer->string.length = 0;
    lexer->string.capacity = 0;
    
    lexer->currentToken.kind = 0;
    lexer->currentToken.sourceLoc.start = NULL;
    lexer->currentToken.sourceLoc.length = 0;
    lexer->currentToken.source = &(lexer->source);
}

static codepoint_t _nextChar(OCLexer* lexer) {
    OASSERT(lexer != NULL, "Null instance error");
    if(!lexer->currentPtr) { return lexer->currentChar = '\0'; }
    
    uint64_t remaining = lexer->source.length - (lexer->currentPtr - lexer->source.bytes);
    lexer->currentChar = utf8_getCodepoint(lexer->currentPtr, remaining);
    
    // advance the current character pointer.
    int8_t size = utf8_codepointSize(lexer->currentChar);
    int displayWidth = mk_wcwidth(lexer->currentChar);
    if(size > 0 && lexer->currentChar != 0) {
        lexer->currentPtr += size;
    }
    
    if(lexer->currentChar == '\n') {
        lexer->startOfLine = true;
        lexer->line += 1;
        lexer->column = 0;
        lexer->linePtr = lexer->currentPtr;
    } else {
        lexer->column += displayWidth;
    }
    
    return lexer->currentChar;
}

static inline codepoint_t _next(OCLexer* lexer) {
    uint64_t remaining = lexer->source.length - (lexer->currentPtr - lexer->source.bytes);
    return utf8_getCodepoint(lexer->currentPtr, remaining);
}

static void _makeToken(OCLexer* lexer, int type) {
    OASSERT(lexer != NULL, "Null instance error");
    lexer->currentToken.kind = type;
    lexer->currentToken.sourceLoc.start = lexer->tokenStart;
    lexer->currentToken.sourceLoc.length = lexer->currentPtr - lexer->tokenStart;

    lexer->currentToken.sourceLoc.startOfLine = lexer->startOfLine;
    lexer->currentToken.sourceLoc.displayWidth = lexer->column - lexer->currentToken.sourceLoc.column;
    
    // We reset the start of line marker after a token is produced.
    lexer->startOfLine = false;
}

static bool _match(OCLexer* lexer, codepoint_t c) {
    if(_next(lexer) != c) { return false; }
    _nextChar(lexer);
    return true;
}

static void _twoChars(OCLexer* lexer, codepoint_t c, int match, int nomatch) {
    _makeToken(lexer, _match(lexer, c) ? match : nomatch);
}


// MARK: - LEXER START

// A list of all the reserved orbit keywords:
static const struct _kw {
    const char* name;
    uint64_t    length;
    int         type;
} _keywords[] = {
    {"fun",     3,  TOKEN_FUN},
    {"var",     3,  TOKEN_VAR},
    {"const",   5,  TOKEN_CONST},
    {"maybe",   5,  TOKEN_MAYBE},
    {"type",    4,  TOKEN_TYPE},
    {"return",  6,  TOKEN_RETURN},
    {"for",     3,  TOKEN_FOR},
    {"in",      2,  TOKEN_IN},
    {"while",   5,  TOKEN_WHILE},
    {"break",   5,  TOKEN_BREAK},
    {"continue",7,  TOKEN_CONTINUE},
    {"if",      2,  TOKEN_IF},
    {"else",    4,  TOKEN_ELSE},
    {"init",    4,  TOKEN_INIT},
    {"fail",    4,  TOKEN_FAIL},
    {"range",   5,  TOKEN_RANGE},
    {"Number",  6,  TOKEN_NUMBER},
    {"Bool",    4,  TOKEN_BOOL},
    {"String",  6,  TOKEN_STRING},
    {"Nil",     3,  TOKEN_NIL},
    {"Void",    4,  TOKEN_VOID},
    {"Array",   5,  TOKEN_ARRAY},
    {"Map",     3,  TOKEN_MAP},
    {"Any",     3,  TOKEN_ANY},
    {NULL, 0}
};

static void _lexIdentifier(OCLexer* lexer) {
    while(utf8_isIdentifier(_next(lexer))) {
        _nextChar(lexer);
    }
    
    int type = TOKEN_IDENTIFIER;
    uint32_t length = lexer->currentPtr - lexer->tokenStart;
    for(uint16_t i = 0; _keywords[i].name != NULL; ++i) {
        if(_keywords[i].length == length &&
           memcmp(_keywords[i].name, lexer->tokenStart, length) == 0) {
            type = _keywords[i].type;
            break;
        }
    }
    _makeToken(lexer, type);
}

static void _stringReserve(OCLexer* lexer) {
    if(lexer->string.capacity <= 0) { lexer->string.capacity = 32; }
    lexer->string.buffer = realloc(lexer->string.buffer, lexer->string.capacity);
}

static void _stringAppend(OCLexer* lexer, codepoint_t c) {
    int8_t size = utf8_codepointSize(c);
    if(size < 0) {
        _lexerError(lexer, "Invalid codepoint found in string: U+%X\n", c);
        return;
    }
    
    if(lexer->string.length + size >= lexer->string.capacity) {
        lexer->string.capacity *= 2;
        _stringReserve(lexer);
    }
    utf8_writeCodepoint(c, &lexer->string.buffer[lexer->string.length],
                           lexer->string.capacity - lexer->string.length);
    lexer->string.length += size;
}

static void _lexString(OCLexer* lexer) {
    lexer->tokenStart += 1;
    
    // String literals cannot be tokenised by solely pointing into the source
    // string, since there's the potential for
    lexer->string.buffer = NULL;
    lexer->string.length = 0;
    _stringReserve(lexer);
    
    for(;;) {
        codepoint_t c = _nextChar(lexer);
        
        if(c == '"') {
            break;
        }
        else if(c == '\0') {
            _lexerError(lexer, "unterminated string literal");
            break;
        }
        else if(c == '\\') {
            c = _nextChar(lexer);
            switch(c) {
                case '\\': _stringAppend(lexer, '\\'); break;
                case 'a':  _stringAppend(lexer, '\a'); break;
                case 'b':  _stringAppend(lexer, '\b'); break;
                case 'f':  _stringAppend(lexer, '\f'); break;
                case 'n':  _stringAppend(lexer, '\n'); break;
                case 'r':  _stringAppend(lexer, '\r'); break;
                case 't':  _stringAppend(lexer, '\t'); break;
                case 'v':  _stringAppend(lexer, '\v'); break;
                case '"':  _stringAppend(lexer, '\"'); break;
                default:
                    _lexerError(lexer, "unknown escape sequence in literal");
                    break;
            }
        } else {
            _stringAppend(lexer, c);
        }
    }
    lexer->currentToken.kind = TOKEN_STRING_LITERAL;
    lexer->currentToken.sourceLoc.startOfLine = lexer->startOfLine;
    lexer->currentToken.sourceLoc.start = lexer->string.buffer;
    lexer->currentToken.sourceLoc.length = lexer->string.length;
    lexer->currentToken.sourceLoc.displayWidth = lexer->column - lexer->currentToken.sourceLoc.column;
    
    // We reset the start of line marker after a token is produced.
    lexer->startOfLine = false;
}

static inline bool isDigit(codepoint_t c) {
    return c >= '0' && c <= '9';
}

static void _lexNumber(OCLexer* lexer) {
    
    int type = TOKEN_INTEGER_LITERAL;
    
    while(isDigit(_next(lexer))) {
        _nextChar(lexer);
    }
    if(_match(lexer, '.') && isDigit(_next(lexer))) {
        type = TOKEN_FLOAT_LITERAL;
        while(isDigit(_next(lexer))) {
            _nextChar(lexer);
        }
    }
    _makeToken(lexer, type);
}

static void _eatLineComment(OCLexer* lexer) {
    while(_next(lexer) != '\n' && _next(lexer) != '\0') {
        _nextChar(lexer);
    }
}

static void _updateTokenStart(OCLexer* lexer) {
    lexer->tokenStart = lexer->currentPtr;
    lexer->currentToken.sourceLoc.line = lexer->line;
    lexer->currentToken.sourceLoc.column = lexer->column;
    lexer->currentToken.source = &(lexer->source);
}

void lexer_nextToken(OCLexer* lexer) {
    OASSERT(lexer != NULL, "Null instance error");
    if(lexer->currentToken.kind == TOKEN_EOF) { return; }
    
    while(_next(lexer) != '\0') {
        
        _updateTokenStart(lexer);
        codepoint_t c = _nextChar(lexer);
        
        switch(c) {
            // whitespace, we just loop
            case 0x0020:
            case 0x000d:
            case 0x0009:
            case 0x000b:
            case 0x000c:
            case 0x000a:
                break;
            
            // single character tokens
            case ';': _makeToken(lexer, TOKEN_SEMICOLON);return;
            
            case '{': _makeToken(lexer, TOKEN_LBRACE);   return;
            case '}': _makeToken(lexer, TOKEN_RBRACE);   return;
            case '[': _makeToken(lexer, TOKEN_LBRACKET); return;
            case ']': _makeToken(lexer, TOKEN_RBRACKET); return;
            case '(': _makeToken(lexer, TOKEN_LPAREN);   return;
            case ')': _makeToken(lexer, TOKEN_RPAREN);   return;
            case ':': _makeToken(lexer, TOKEN_COLON);    return;
            case '.': _makeToken(lexer, TOKEN_DOT);      return;
            case ',': _makeToken(lexer, TOKEN_COMMA);    return;

            case '^': _makeToken(lexer, TOKEN_CARET);    return;
            case '~': _makeToken(lexer, TOKEN_TILDE);    return;
            case '%': _makeToken(lexer, TOKEN_PERCENT);  return;
            case '?': _makeToken(lexer, TOKEN_QUESTION); return;
            
            case '+': _twoChars(lexer, '=', TOKEN_PLUSEQ, TOKEN_PLUS);  return;
            case '&': _twoChars(lexer, '&', TOKEN_AMPAMP, TOKEN_AMP);   return;
            case '|': _twoChars(lexer, '|', TOKEN_PIPEPIPE, TOKEN_PIPE);return;
            case '!': _twoChars(lexer, '=', TOKEN_BANGEQ, TOKEN_BANG);  return;
            case '=': _twoChars(lexer, '=', TOKEN_EQEQ, TOKEN_EQUALS);  return;
            
            case '*':
                if(_match(lexer, '*')) {
                    _makeToken(lexer, TOKEN_STARSTAR);
                } else {
                    _twoChars(lexer, '=', TOKEN_STAREQ, TOKEN_STAR);
                }
                return;
            
            case '/':
                if(_match(lexer, '/')) {
                    _eatLineComment(lexer);
                } else {
                    _twoChars(lexer, '=', TOKEN_SLASHEQ, TOKEN_SLASH);
                    return;
                }
                break;
            
            case '-':
                if(_match(lexer, '>')) {
                    _makeToken(lexer, TOKEN_ARROW);
                } else {
                    _twoChars(lexer, '=', TOKEN_MINUSEQ, TOKEN_MINUS);
                }
                return;
            
            case '<':
                if(_match(lexer, '<')) {
                    _makeToken(lexer, TOKEN_LTLT);
                } else {
                    _twoChars(lexer, '=', TOKEN_LTEQ, TOKEN_LT);
                }
                return;
            
            case '>':
                if(_match(lexer, '>')) {
                    _makeToken(lexer, TOKEN_GTGT);
                } else {
                    _twoChars(lexer, '=', TOKEN_GTEQ, TOKEN_GT);
                }
                return;
            
            case '#':
                _eatLineComment(lexer);
                break;
                
            case '"':
                _lexString(lexer);
                return;
            
            // More complex tokens get parsed here
            default:
                if(utf8_isIdentifierHead(c)) {
                    _lexIdentifier(lexer);
                }
                else if(isDigit(c)) {
                    _lexNumber(lexer);
                }
                else {
                    char point[6];
                    int size = utf8_writeCodepoint(c, point, 6);
                    point[size] = '\0';
                    _lexerError(lexer, "invalid character '%.*s'", size, point);
                    _makeToken(lexer, TOKEN_INVALID);
                }
                return;
        }
    }
    lexer->currentToken.kind = TOKEN_EOF;
    lexer->currentToken.sourceLoc.length = 0;
    lexer->currentToken.sourceLoc.start = NULL;
}
