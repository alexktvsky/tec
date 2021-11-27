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

#include "token.h"

namespace Tec {


Token::Token(const std::string &name, size_t line, size_t offset)
    : name(name)
    , type(decodeType(name))
    , line(line)
    , offset(offset)
{}

Token::Token(const std::string &name, const Token &obj)
    : name(name)
    , type(decodeType(name))
    , line(obj.line)
    , offset(obj.offset)
{}

TokenType Token::decodeType(const std::string &name)
{
    if (name == "template") {
        return TokenType::TEMPLATE_KEYWORD;
    }
    else if (name == "typename") {
        return TokenType::TYPENAME_KEYWORD;
    }
    else if (name == "include") {
        return TokenType::INCLUDE_KEYWORD;
    }
    else if (name == "struct") {
        return TokenType::STRUCT_KEYWORD;
    }
    else if (name == "typedef") {
        return TokenType::TYPEDEF_KEYWORD;
    }
    else if (name == "<") {
        return TokenType::OPEN_ANGLE_BRACKET;
    }
    else if (name == ">") {
        return TokenType::CLOSE_ANGLE_BRACKET;
    }
    else if (name == "[") {
        return TokenType::OPEN_SQUARE_BRACKET;
    }
    else if (name == "]") {
        return TokenType::CLOSE_SQUARE_BRACKET;
    }
    else if (name == "{") {
        return TokenType::OPEN_CURLE_BRACKET;
    }
    else if (name == "}") {
        return TokenType::CLOSE_CURLE_BRACKET;
    }
    else if (name == "(") {
        return TokenType::OPEN_BRACKET;
    }
    else if (name == ")") {
        return TokenType::CLOSE_BRACKET;
    }
    else if (name == " " || name == "\t") {
        return TokenType::SPACE;
    }
    else if (name == ",") {
        return TokenType::COMMA;
    }
    else if (name == ";") {
        return TokenType::SEMICOLON;
    }
    else if (name == "\n") {
        return TokenType::NEW_LINE;
    }
    else if (name == "#") {
        return TokenType::SHARP;
    }
    else if (name == "*") {
        return TokenType::ASTERISK;
    }
    else if (name == "\"") {
        return TokenType::DOUBLE_QUOTES;
    }
    else if (name == "\'") {
        return TokenType::SINGLE_QUOTES;
    }
    else {
        return TokenType::STRING;
    }
}

std::string Token::getTypeName(const TokenType &type)
{
    switch (type) {
    case TokenType::TEMPLATE_KEYWORD:
        return "TEMPLATE_KEYWORD";
    case TokenType::TYPENAME_KEYWORD:
        return "TYPENAME_KEYWORD";
    case TokenType::INCLUDE_KEYWORD:
        return "INCLUDE_KEYWORD";
    case TokenType::STRUCT_KEYWORD:
        return "STRUCT_KEYWORD";
    case TokenType::TYPEDEF_KEYWORD:
        return "TYPEDEF_KEYWORD";
    case TokenType::OPEN_ANGLE_BRACKET:
        return "OPEN_ANGLE_BRACKET";
    case TokenType::CLOSE_ANGLE_BRACKET:
        return "CLOSE_ANGLE_BRACKET";
    case TokenType::OPEN_SQUARE_BRACKET:
        return "OPEN_SQUARE_BRACKET";
    case TokenType::CLOSE_SQUARE_BRACKET:
        return "CLOSE_SQUARE_BRACKET";
    case TokenType::OPEN_CURLE_BRACKET:
        return "OPEN_CURLE_BRACKET";
    case TokenType::CLOSE_CURLE_BRACKET:
        return "CLOSE_CURLE_BRACKET";
    case TokenType::OPEN_BRACKET:
        return "OPEN_BRACKET";
    case TokenType::CLOSE_BRACKET:
        return "CLOSE_BRACKET";
    case TokenType::SPACE:
        return "SPACE";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::NEW_LINE:
        return "NEW_LINE";
    case TokenType::SHARP:
        return "SHARP";
    case TokenType::ASTERISK:
        return "ASTERISK";
    case TokenType::DOUBLE_QUOTES:
        return "DOUBLE_QUOTES";
    case TokenType::SINGLE_QUOTES:
        return "SINGLE_QUOTES";
    case TokenType::STRING:
        return "STRING";
    default:
        return "UNKNOW_TOKEN_TYPE";
    }
}

std::string Token::getTypeName(void) const
{
    return getTypeName(type);
}

} // namespace Tec
