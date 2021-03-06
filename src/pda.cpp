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

#include <algorithm>
#include <iostream>

#include "pda.h"
#include "syntax_exception.h"

namespace Tec {


const Pda::StatesRow Pda::states_table[] = {
    {INITIAL_STATE, TEMPLATE_KEYWORD, Stack(), FOUND_TEMPLATE_KEYWORD, NOP},
    {FOUND_TEMPLATE_KEYWORD, OPEN_ANGLE_BRACKET, Stack(), FOUND_OPEN_ANGLE_BRACKET, NOP},
    {FOUND_OPEN_ANGLE_BRACKET, TYPENAME_KEYWORD, Stack(), FOUND_TYPENAME_KEYWORD, NOP},
    {FOUND_TYPENAME_KEYWORD, STRING, Stack(), FOUND_TYPE_ALIAS, NOP},
    {FOUND_TYPE_ALIAS, CLOSE_ANGLE_BRACKET, Stack(), SEARCH_TEMPLATE_NAME, NOP},
    {FOUND_TYPE_ALIAS, COMMA, Stack(), FOUND_OPEN_ANGLE_BRACKET, NOP},

    {SEARCH_TEMPLATE_NAME, TYPEDEF_KEYWORD, Stack(), SEARCH_TEMPLATE_NAME, PUSH},
    {SEARCH_TEMPLATE_NAME, STRUCT_KEYWORD, Stack(), SEARCH_TEMPLATE_NAME, PUSH},
    {SEARCH_TEMPLATE_NAME, STRING, Stack({STRUCT_KEYWORD}), FOUND_STRUCT_NAME, CLEAR},
    {SEARCH_TEMPLATE_NAME, OPEN_BRACKET, Stack({STRING}), FOUND_FUNCTION_NAME, CLEAR},
    {SEARCH_TEMPLATE_NAME, OPEN_CURLE_BRACKET, Stack({STRUCT_KEYWORD}), FOUND_TYPEDEF_DEFINITION, CLEAR},
    {SEARCH_TEMPLATE_NAME, STRING, Stack(), SEARCH_TEMPLATE_NAME, PUSH},

    {FOUND_FUNCTION_NAME, ALL_TOKENS, Stack(), SEARCH_CLOSE_BRACKET, NOP},
    {SEARCH_CLOSE_BRACKET, CLOSE_BRACKET, Stack(), FOUND_CLOSE_BRACKET, NOP},
    {SEARCH_CLOSE_BRACKET, ALL_TOKENS, Stack(), SEARCH_CLOSE_BRACKET, NOP},

    {FOUND_CLOSE_BRACKET, SEMICOLON, Stack(), FOUND_TEMPLATE_END, NOP},
    {FOUND_CLOSE_BRACKET, OPEN_CURLE_BRACKET, Stack(), FOUND_FUNCTION_DEFINITION, NOP},
    {FOUND_CLOSE_BRACKET, CLOSE_BRACKET, Stack(), FOUND_CLOSE_BRACKET, NOP},

    {FOUND_TEMPLATE_END, TEMPLATE_KEYWORD, Stack(), FOUND_TEMPLATE_KEYWORD, NOP},
    {FOUND_TEMPLATE_END, ALL_TOKENS, Stack(), INITIAL_STATE, NOP},
    
    {FOUND_FUNCTION_DEFINITION, OPEN_CURLE_BRACKET, Stack(), FOUND_FUNCTION_DEFINITION, PUSH},
    {FOUND_FUNCTION_DEFINITION, CLOSE_CURLE_BRACKET, Stack({OPEN_CURLE_BRACKET}), FOUND_FUNCTION_DEFINITION, POP},
    {FOUND_FUNCTION_DEFINITION, CLOSE_CURLE_BRACKET, Stack(), FOUND_TEMPLATE_END, NOP},

    {FOUND_FUNCTION_DEFINITION, OPEN_ANGLE_BRACKET, Stack({ALL_TOKENS}), FOUND_NESTED_TEMPLATE_CALL, NOP},
    {FOUND_FUNCTION_DEFINITION, ALL_TOKENS, Stack({ALL_TOKENS}), FOUND_FUNCTION_DEFINITION, NOP}, 

    {INITIAL_STATE, STRING, Stack(), INITIAL_STATE, PUSH},

    {INITIAL_STATE, OPEN_ANGLE_BRACKET, Stack({STRING}), FOUND_TEMPLATE_CALL, CLEAR},
    {INITIAL_STATE, INCLUDE_KEYWORD, Stack(), FOUND_INCLUDE_KEYWORD, NOP},
    {FOUND_INCLUDE_KEYWORD, ALL_TOKENS, Stack(), INITIAL_STATE, NOP},

    {FOUND_TEMPLATE_CALL, STRING, Stack(), FOUND_TEMPLATE_CALL_TYPE, NOP},
    {FOUND_TEMPLATE_CALL, OPEN_ANGLE_BRACKET, Stack(), FOUND_NESTED_TEMPLATE, NOP},

    {FOUND_TEMPLATE_CALL_TYPE, COMMA, Stack(), FOUND_TEMPLATE_CALL_AGAIN, NOP},
    {FOUND_TEMPLATE_CALL_TYPE, CLOSE_ANGLE_BRACKET, Stack(), FOUND_TEMPLATE_CALL_END, NOP},
    {FOUND_TEMPLATE_CALL_TYPE, STRING, Stack(), FOUND_TEMPLATE_CALL_TYPE, NOP},
    {FOUND_TEMPLATE_CALL_AGAIN, STRING, Stack(), FOUND_TEMPLATE_CALL_TYPE_AGAIN, NOP},

    {FOUND_TEMPLATE_CALL_TYPE_AGAIN, CLOSE_ANGLE_BRACKET, Stack(), FOUND_TEMPLATE_CALL_END, NOP},
    {FOUND_TEMPLATE_CALL_TYPE_AGAIN, COMMA, Stack(), FOUND_TEMPLATE_CALL_AGAIN, NOP},
    {FOUND_TEMPLATE_CALL_TYPE_AGAIN, OPEN_ANGLE_BRACKET, Stack(), FOUND_NESTED_TEMPLATE_CALL, NOP},
    {FOUND_TEMPLATE_CALL_TYPE_AGAIN, STRING, Stack(), FOUND_TEMPLATE_CALL_TYPE_AGAIN, NOP},

    {FOUND_TEMPLATE_CALL_END, ALL_TOKENS, Stack(), INITIAL_STATE, NOP},

    {FOUND_STRUCT_NAME, OPEN_CURLE_BRACKET, Stack(), FOUND_STRUCT_DEFINITION, NOP},
    {FOUND_STRUCT_DEFINITION, OPEN_CURLE_BRACKET, Stack(), FOUND_STRUCT_DEFINITION, PUSH},
    {FOUND_STRUCT_DEFINITION, CLOSE_CURLE_BRACKET, Stack({OPEN_CURLE_BRACKET}), FOUND_STRUCT_DEFINITION, POP},
    {FOUND_STRUCT_DEFINITION, CLOSE_CURLE_BRACKET, Stack(), FOUND_STRUCT_DEFINITION, PUSH},
    {FOUND_STRUCT_DEFINITION, SEMICOLON, Stack({CLOSE_CURLE_BRACKET}), FOUND_TEMPLATE_END, CLEAR},
    {FOUND_STRUCT_DEFINITION, STRING, Stack({CLOSE_CURLE_BRACKET}), FOUND_TYPEDEF_STRUCT_NAME, NOP},
    {FOUND_STRUCT_DEFINITION, OPEN_ANGLE_BRACKET, Stack({ALL_TOKENS}), FOUND_NESTED_TEMPLATE, NOP},
    {FOUND_STRUCT_DEFINITION, ALL_TOKENS, Stack({ALL_TOKENS}), FOUND_STRUCT_DEFINITION, NOP},
    {FOUND_TYPEDEF_STRUCT_NAME, SEMICOLON, Stack({CLOSE_CURLE_BRACKET}), FOUND_TEMPLATE_END, CLEAR},

    {FOUND_TYPEDEF_DEFINITION, OPEN_CURLE_BRACKET, Stack(), FOUND_STRUCT_DEFINITION, PUSH},
    {FOUND_TYPEDEF_DEFINITION, CLOSE_CURLE_BRACKET, Stack({OPEN_CURLE_BRACKET}), FOUND_STRUCT_DEFINITION, POP},
    {FOUND_TYPEDEF_DEFINITION, CLOSE_CURLE_BRACKET, Stack(), FOUND_TYPEDEF_DEFINITION, PUSH},
    {FOUND_TYPEDEF_DEFINITION, STRING, Stack({CLOSE_CURLE_BRACKET}), FOUND_STRUCT_NAME, CLEAR},
    {FOUND_TYPEDEF_DEFINITION, OPEN_ANGLE_BRACKET, Stack({ALL_TOKENS}), FOUND_NESTED_TEMPLATE, NOP},
    {FOUND_TYPEDEF_DEFINITION, ALL_TOKENS, Stack({ALL_TOKENS}), FOUND_TYPEDEF_DEFINITION, NOP}, 
    {FOUND_STRUCT_NAME, SEMICOLON, Stack(), FOUND_TEMPLATE_END, NOP},

    {FOUND_STRUCT_NAME, OPEN_ANGLE_BRACKET, Stack({OPEN_ANGLE_BRACKET}), FOUND_TYPEDEF_STRUCT_DECLARATION, POP},
    {FOUND_STRUCT_NAME, CLOSE_ANGLE_BRACKET, Stack({OPEN_ANGLE_BRACKET}), FOUND_STRUCT_NAME, NOP},
    {FOUND_STRUCT_NAME, STRING, Stack({OPEN_ANGLE_BRACKET}), FOUND_STRUCT_NAME, NOP},
    {FOUND_STRUCT_NAME, OPEN_ANGLE_BRACKET, Stack(), FOUND_NESTED_TEMPLATE, PUSH},

    {FOUND_TYPEDEF_STRUCT_DECLARATION, SEMICOLON, Stack(), FOUND_TEMPLATE_END, NOP},
    {FOUND_TYPEDEF_STRUCT_DECLARATION, ALL_TOKENS, Stack(), FOUND_TYPEDEF_STRUCT_DECLARATION, NOP},

    {FOUND_TEMPLATE_CALL_TYPE, OPEN_ANGLE_BRACKET, Stack(), FOUND_NESTED_TEMPLATE_CALL, NOP},
    {FOUND_NESTED_TEMPLATE, ALL_TOKENS, Stack({ALL_TOKENS}), RESTORE_PREVIOUS, NOP},
    {FOUND_NESTED_TEMPLATE_CALL, ALL_TOKENS, Stack({ALL_TOKENS}), RESTORE_PREVIOUS, NOP},

    {INITIAL_STATE, DOUBLE_QUOTES, Stack(), INITIAL_STATE, PUSH},
    {INITIAL_STATE, OPEN_ANGLE_BRACKET, Stack{DOUBLE_QUOTES}, INITIAL_STATE, POP},
    {INITIAL_STATE, ALL_TOKENS, Stack(), INITIAL_STATE, NOP}
};


Pda::State Pda::next(const Token &token)
{
    TokenType type = token.getType();
    bool done = false;

    if (!inInputAlphabet(type)) {
        return NOT_CHANGED;
    }

    for (const auto &x : states_table) {

        if (!(state == x.cur_state)) {
            continue;
        }
        if (!(type == x.input || x.input == ALL_TOKENS)) {
            continue;
        }
        if (!(std::equal(x.stack.cbegin(), x.stack.cend(), stack.cbegin())
            || x.stack.top() == ALL_TOKENS))
        {
            continue;
        }

        if (x.next_state == RESTORE_PREVIOUS) {
            state = prev_state;
        }
        else {
            prev_state = state;
            state = x.next_state;
        }

#if (TEC_DEBUG)
        if (state != INITIAL_STATE && x.next_state != INITIAL_STATE) {
            printf("%s -> %s\n",
                getStateName(prev_state).c_str(),
                getStateName(state).c_str());
        }
#endif

        switch (x.action) {
        case PUSH:
            stack.push(type);
            break;
        case POP:
            if (!stack.empty()) {
                stack.pop();
            }
            break;
        case CLEAR:
            if (!stack.empty()) {
                stack.clear();
            }
            break;
        case REPLACE:
            if (!stack.empty()) {
                stack.pop();
            }
            stack.push(type);
            break;
        case NOP:
            break;
        }
        done = true;
        break;
    }

    if (!done) {
        return FAILED_STATE;
    }

    return state;
}


Pda::Pda(void)
    : state(State::INITIAL_STATE)
    , prev_state(State::INITIAL_STATE)
{}


bool Pda::inInputAlphabet(const TokenType &type)
{
    switch (type) {
    case TokenType::SPACE:
    case TokenType::ASTERISK:
    case TokenType::NEW_LINE:
        return false;
    default:
        return true;
    }
}


std::string Pda::getStateName(const State &state)
{
    switch (state) {
    case INITIAL_STATE:
        return "INITIAL_STATE";
    case FOUND_TEMPLATE_KEYWORD:
        return "FOUND_TEMPLATE_KEYWORD";
    case FOUND_OPEN_ANGLE_BRACKET:
        return "FOUND_OPEN_ANGLE_BRACKET";
    case FOUND_TYPENAME_KEYWORD:
        return "FOUND_TYPENAME_KEYWORD";
    case FOUND_TYPE_ALIAS:
        return "FOUND_TYPE_ALIAS";
    case SEARCH_TEMPLATE_NAME:
        return "SEARCH_TEMPLATE_NAME";
    case SEARCH_CLOSE_BRACKET:
        return "SEARCH_CLOSE_BRACKET";
    case FOUND_CLOSE_BRACKET:
        return "FOUND_CLOSE_BRACKET";
    case FOUND_FUNCTION_DEFINITION:
        return "FOUND_FUNCTION_DEFINITION";
    case SEARCH_TEMPLATE_END:
        return "SEARCH_TEMPLATE_END";
    case FOUND_FUNCTION_NAME:
        return "FOUND_FUNCTION_NAME";
    case FOUND_TEMPLATE_END:
        return "FOUND_TEMPLATE_END";
    case FOUND_TEMPLATE_CALL:
        return "FOUND_TEMPLATE_CALL";
    case FOUND_INCLUDE_KEYWORD:
        return "FOUND_INCLUDE_KEYWORD";
    case FOUND_TEMPLATE_CALL_TYPE:
        return "FOUND_TEMPLATE_CALL_TYPE";
    case FOUND_TEMPLATE_CALL_AGAIN:
        return "FOUND_TEMPLATE_CALL_AGAIN";
    case FOUND_TEMPLATE_CALL_TYPE_AGAIN:
        return "FOUND_TEMPLATE_CALL_TYPE_AGAIN";
    case FOUND_TEMPLATE_CALL_END:
        return "FOUND_TEMPLATE_CALL_END";
    case FOUND_STRUCT_NAME:
        return "FOUND_STRUCT_NAME";
    case FOUND_STRUCT_DEFINITION:
        return "FOUND_STRUCT_DEFINITION";
    case FOUND_TYPEDEF_DEFINITION:
        return "FOUND_TYPEDEF_DEFINITION";
    case FOUND_TYPEDEF_STRUCT_NAME:
        return "FOUND_TYPEDEF_STRUCT_NAME";
    case FOUND_NESTED_TEMPLATE:
        return "FOUND_NESTED_TEMPLATE";
    case FOUND_NESTED_TEMPLATE_CALL:
        return "FOUND_NESTED_TEMPLATE_CALL";
    case RESTORE_PREVIOUS:
        return "RESTORE_PREVIOUS";
    case FOUND_TYPEDEF_STRUCT_DECLARATION:
        return "FOUND_TYPEDEF_STRUCT_DECLARATION";

    // ...
    default:
        return "UNKNOWN_STATE";
    }
}

} // namespace Tec
