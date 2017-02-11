//
//  orbit_utf8.h
//  OrbitVM
//
//  Created by Cesar Parent on 2017-02-06.
//  Copyright © 2017 cesarparent. All rights reserved.
//
#ifndef orbit_utf8_h
#define orbit_utf8_h

#include <stdint.h>
#include <stdbool.h>

// Functions we need to deal with UTF8-encoded strings and files. The goal is
// to have everything in Orbit be unicode-compatible (strings, but also names
// in source files). The characters allowed as start and part of identifiers
// are the same as the ones defined by clang for C/C++ and for Swift.

typedef int32_t codepoint_t;

// Returns how many bytes are required to encode [point], or -1 if the codepoint
// is not a valid one.
int8_t utf8_codepointSize(codepoint_t point);

// Returns whether [point] is in a private Unicode range.
bool utf8_isPrivate(codepoint_t point);

// Returns whether [point] is a Unicode combining character.
bool utf8_isCombining(codepoint_t point);

// Returns whether [point] is in the Unicode Basic Multinlingual Plane.
bool utf8_isBMP(codepoint_t point);

// Returns whether [point] is a valid start of identifier character.
// At the moment, those characters are exactly the same as in Swift.
bool utf8_isIdentifierHead(codepoint_t point);

// Returns whether [point] is a valid identifier character.
bool utf8_isIdentifier(codepoint_t point);

// Returns whether [point] is a valid operator character.
bool utf8_isOperator(codepoint_t point);

// Returns whether [point] is a valid string literal character.
bool utf8_isQuotedItem(codepoint_t point);

#endif /* orbit_utf8_h */
