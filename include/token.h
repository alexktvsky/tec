/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Kotovsky <alexktvsky@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <cstddef>
#include <string>
#include <list>

namespace Tec {

// enum class TokenType {
enum TokenType {
    TEMPLATE_KEYWORD = 0,
    TYPENAME_KEYWORD,
    INCLUDE_KEYWORD,
    STRUCT_KEYWORD,
    TYPEDEF_KEYWORD,
    OPEN_ANGLE_BRACKET,
    CLOSE_ANGLE_BRACKET,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    OPEN_CURLE_BRACKET,
    CLOSE_CURLE_BRACKET,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    SPACE,
    COMMA,
    SEMICOLON,
    NEW_LINE,
    SHARP,
    ASTERISK,
    DOUBLE_QUOTES,
    SINGLE_QUOTES,
    STRING,
    ALL_TOKENS
}; // End of enum

class Token {
public:
    Token(const std::string &name, size_t line, size_t offset);
    Token(const std::string &name, const Token &obj);
    std::string getName(void) const { return name; }
    size_t getLine(void) const { return line; }
    size_t getOffset(void) const { return offset; }
    TokenType getType(void) const { return type; }
    std::string getTypeName(void) const;
    static std::string getTypeName(const TokenType &type);
    bool operator==(const Token &other) const {
        return name == other.name &&
            type == other.type &&
            line == other.line &&
            offset == other.offset;
    }
    bool operator!=(const Token &other) const { return !(operator==(other)); }
private:
    TokenType decodeType(const std::string &name);
    std::string name;
    TokenType type;
    size_t line;
    size_t offset;
}; // End of class


using TokenList = std::list<Token>;

} // namespace Tec

#endif // TOKEN_H
