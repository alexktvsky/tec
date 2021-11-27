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

#ifndef PDA_H
#define PDA_H

#include <string>

#include "stack.h"
#include "token.h"

namespace Tec {

class Pda {
public:
    enum State : uint32_t;
    Pda(void);
    State next(const Token &token);
    std::string getStateName(const State &state);
private:
    bool inInputAlphabet(const TokenType &type);
    State state;
    State prev_state;
    Stack stack;
    enum StackAction : uint8_t;
    struct StatesRow {
        State cur_state;
        TokenType input;
        Stack stack;
        State next_state;
        StackAction action;
    }; // End of struct
    static const StatesRow states_table[];
}; // End of class

enum Pda::StackAction : uint8_t {
    PUSH,
    POP,
    CLEAR,
    REPLACE,
    NOP
}; // End of enum

enum Pda::State : uint32_t {
    INITIAL_STATE = 0,
    FOUND_TEMPLATE_KEYWORD,
    FOUND_OPEN_ANGLE_BRACKET,
    FOUND_TYPENAME_KEYWORD,
    FOUND_TYPE_ALIAS,
    SEARCH_TEMPLATE_NAME,
    SEARCH_CLOSE_BRACKET,
    FOUND_CLOSE_BRACKET,
    SEARCH_TEMPLATE_END,
    FOUND_FUNCTION_NAME,
    FOUND_FUNCTION_DEFINITION,
    FOUND_TEMPLATE_END,
    FOUND_TEMPLATE_CALL,
    FOUND_INCLUDE_KEYWORD,
    FOUND_TEMPLATE_CALL_TYPE,
    FOUND_TEMPLATE_CALL_AGAIN,
    FOUND_TEMPLATE_CALL_TYPE_AGAIN,
    FOUND_TEMPLATE_CALL_END,
    FOUND_STRUCT_NAME,
    FOUND_STRUCT_DEFINITION,
    FOUND_TYPEDEF_DEFINITION,
    FOUND_TYPEDEF_STRUCT_NAME,
    FOUND_NESTED_TEMPLATE,
    FOUND_NESTED_TEMPLATE_CALL,
    FOUND_TYPEDEF_STRUCT_DECLARATION,
    NOT_CHANGED,
    RESTORE_PREVIOUS,
    FAILED_STATE
}; // End of enum

} // namespace Tec

#endif // PDA_H
