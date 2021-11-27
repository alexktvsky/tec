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

#include <iterator>
#include <algorithm>

#include "preprocessor.h"
#include "syntax_exception.h"
#include "pda.h"

namespace Tec {


void Preprocessor::generate(std::ostream &out)
{
    for (const auto &x : tokens) {
        out << x.getName();
    }
    out << std::endl;
}


void Preprocessor::tokenize(const std::string &data)
{
    std::string::const_iterator iter, begin, end;
    char c;

    size_t line = 1, offset = 1, saved_offset = 0;

    bool begin_null = true;
    // bool end_null = true;

    bool found = false;
    bool end_file = false;
    bool non_alpha = false;
    bool have_space = false;

    for (iter = data.cbegin(); iter != data.cend(); ++iter) {

        c = *iter;

        switch (c) {
        case '\0':
            end_file = true;
            break;

        case ' ':  // Space
        case '\t': // Tab

            if (!begin_null) {
                end = iter;
                // end_null = false;
                found = true;
            }

            if (have_space) {
                non_alpha = false; // Skip repeating spaces
            }
            else {
                have_space = true;
                non_alpha = true;
            }
            break;

        case '\r': // CR
        case '\n': // LF
        case '<':
        case '>':
        case '#':
        case '*':
        case '{':
        case '}':
        case '(':
        case ')':
        case '[':
        case ']':
        case ',':
        case ';':
        case '"': 
        case '\'':

            if (!begin_null) {
                end = iter;
                // end_null = false;
                found = true;
            }
            non_alpha = true;
            break;

        default:
            if (begin_null) {
                begin = iter;
                begin_null = false;
                saved_offset = offset;
            }
        }

        if (found) {
            tokens.push_back(Token(std::string(begin, end), line, saved_offset));

            begin_null = true;
            // end_null = true;
            found = false;
        }

        if (non_alpha) {
            tokens.push_back(Token(std::string(1, c), line, offset));
            non_alpha = false;
        }

        if (c != ' ' && c != '\t') {
            have_space = false;
        }

        if (end_file) {
            break;
        }

        // TODO: CR, LF, CRLF
        if (c == '\n') {
            line += 1;
            offset = 1;
        }
        else {
            offset += 1;
        }
    }
}


void Preprocessor::parseSyntax(void)
{
    TokenList chain;
    TokenList::iterator iter = tokens.begin();
    TokenList::iterator entry, body, call, call2;
    Pda::State state = Pda::INITIAL_STATE, saved_state;
    std::string temp;
    bool done = false;
    bool need_postprocess = false;

    std::string name, name2;
    std::vector<std::string> argv;

    Pda pda;


    while (iter != tokens.end()) {

        if (state != Pda::NOT_CHANGED) {
            saved_state = state;
        }

        state = pda.next(*iter);

        if (state == Pda::FOUND_TEMPLATE_KEYWORD ||
            (state == Pda::NOT_CHANGED && saved_state == Pda::FOUND_TEMPLATE_KEYWORD))
        {
            entry = std::prev(std::prev(iter));
        }
        else if (state == Pda::FOUND_TYPE_ALIAS) {
            argv.push_back(iter->getName());
        }
        else if (state == Pda::SEARCH_TEMPLATE_NAME) {
            temp = iter->getName();
            if (saved_state != Pda::SEARCH_TEMPLATE_NAME) {
                body = iter;
            }
        }
        else if (state == Pda::FOUND_FUNCTION_NAME) {
            name = temp;
        }
        else if (state == Pda::FOUND_TEMPLATE_END) {
            if (saved_state != Pda::FOUND_TEMPLATE_END) {

                if (saved_state == Pda::FOUND_TYPEDEF_STRUCT_DECLARATION) {
                    chain.assign(std::next(body), std::next(call2));
                    chain.push_back(*iter);
                }
                else {
                    chain.assign(std::next(body), std::next(iter));
                }             

                if (name2.empty()) {
                    table.push_back(TemplateSymbols(name, argv, chain, entry));
                }
                else {
                    table.push_back(TemplateSymbols(name, argv, chain, entry, name2));
                    name2.clear();
                }
                name.clear();
                argv.clear();
                chain.clear();
            }
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL) {
            call = iter;
            name = std::prev(iter)->getName();
        }

        else if (state == Pda::FOUND_TEMPLATE_CALL_TYPE ||
            (state == Pda::NOT_CHANGED && saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE))
        {
            if (saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE) {
                argv.back().append(iter->getName());
            }
            else {
                argv.push_back(iter->getName());
            }
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_TYPE_AGAIN ||
            (state == Pda::NOT_CHANGED && saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE_AGAIN))
        {
            if (saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE_AGAIN) {
                argv.back().append(iter->getName());
            }
            else {
                argv.push_back(iter->getName());
            }
        }
        else if (state == Pda::FOUND_NESTED_TEMPLATE) {
            need_postprocess = true;
        }
        else if (state == Pda::FOUND_STRUCT_NAME) {
            // if (saved_state != Pda::FOUND_STRUCT_NAME) {
            name = iter->getName();
            call2 = iter;
            // }
        }
        else if (state == Pda::FOUND_TYPEDEF_STRUCT_NAME) {
            name2 = iter->getName();
        }
        else if (state == Pda::FOUND_TYPEDEF_STRUCT_DECLARATION &&
            saved_state == Pda::FOUND_TYPEDEF_STRUCT_DECLARATION)
        {
            call = iter;
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_END) {
            if (saved_state != Pda::FOUND_TEMPLATE_CALL_END) {
                for (auto &x : table) {
                    if (name == x.getName()) {
                        x.instantiate(&tokens, argv, std::prev(call));
                        done = true;
                    }
                    else if (name == x.getAdditionalName()) {
                        x.instantiate(&tokens, argv, std::prev(call));
                        done = true;
                    }
                }
                if (done) {
                    TemplateSymbols::modifyCall(&tokens, std::prev(call), argv);
                    done = false;
                }
                else {
                    throw SyntaxException(std::prev(call)->getLine(), std::prev(call)->getOffset(),
                        "Unknown template name \'" + name + "\'");
                }
                argv.clear();
            }
        }
        else if (state == Pda::FAILED_STATE) {
            throw SyntaxException(iter->getLine(), iter->getOffset(),
                "Unexpected token \'" + iter->getName() + "\'");
        }

        if (state == Pda::FOUND_TEMPLATE_END) {
            iter = tokens.erase(std::next(entry), std::next(iter));
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_END
            && saved_state != Pda::FOUND_TEMPLATE_CALL_END)
        {
            iter = tokens.erase(call, std::next(iter));
        }
        else {
            ++iter;
        }
    }

    // Post processing
    if (need_postprocess) {
        processAllNestedCalls(tokens.begin());
    }
}

void Preprocessor::processAllNestedCalls(TokenList::iterator iter)
{
    TokenList::iterator call, saved_iter;
    Pda::State state = Pda::INITIAL_STATE, saved_state;

    std::string name;
    std::vector<std::string> argv;
    bool done = false;
    bool again = false;

    Pda pda;


    while (iter != tokens.end()) {

        saved_state = state;
        state = pda.next(*iter);

        if (state == Pda::FOUND_TEMPLATE_CALL) {
            call = iter;
            name = std::prev(iter)->getName();
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_TYPE) {
            if (saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE) {
                argv.back().append(iter->getName());
            }
            else {
                argv.push_back(iter->getName());
            }
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_TYPE_AGAIN) {
            if (saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE_AGAIN) {
                argv.back().append(iter->getName());
            }
            else {
                argv.push_back(iter->getName());
            }
        }
        else if (state == Pda::FOUND_NESTED_TEMPLATE_CALL) {
            saved_iter = iter;
            iter = std::prev(std::prev(iter));
            processOneNestedCall(std::next(iter));
            argv.pop_back();
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_END) {
            if (saved_state != Pda::FOUND_TEMPLATE_CALL_END) {
                for (auto &x : table) {
                    if (name == x.getName()) {
                        x.instantiate(&tokens, argv, std::prev(call));
                        done = true;
                    }
                    else if (name == x.getAdditionalName()) {
                        x.instantiate(&tokens, argv, std::prev(call));
                        done = true;
                    }
                }
                if (done) {
                    TemplateSymbols::modifyCall(&tokens, std::prev(call), argv);
                    done = false;
                }
                else {
                    throw SyntaxException(std::prev(call)->getLine(), std::prev(call)->getOffset(),
                        "Unknown template name \'" + name + "\'");
                }
                argv.clear();
                again = true;
            }
        }
        else if (state == Pda::FAILED_STATE) {
            throw SyntaxException(iter->getLine(), iter->getOffset(),
                "Unexpected token \'" + iter->getName() + "\'");
        }

        if (state == Pda::FOUND_TEMPLATE_CALL_END
            && saved_state != Pda::FOUND_TEMPLATE_CALL_END)
        {
            iter = tokens.erase(call, std::next(iter));
        }
        else {
            ++iter;
        }
    }

    if (again) {
        processAllNestedCalls(tokens.begin());
    }
}

void Preprocessor::processOneNestedCall(TokenList::iterator iter)
{
    TokenList::iterator call, saved_iter;
    Pda::State state = Pda::INITIAL_STATE, saved_state;

    std::string name;
    std::vector<std::string> argv;
    bool done = false;

    Pda pda;


    while (iter != tokens.end()) {

        saved_state = state;
        state = pda.next(*iter);

        if (state == Pda::FOUND_TEMPLATE_CALL) {
            call = iter;
            name = std::prev(iter)->getName();
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_TYPE) {
            if (saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE) {
                argv.back().append(iter->getName());
            }
            else {
                argv.push_back(iter->getName());
            }
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_TYPE_AGAIN) {
            if (saved_state == Pda::FOUND_TEMPLATE_CALL_TYPE_AGAIN) {
                argv.back().append(iter->getName());
            }
            else {
                argv.push_back(iter->getName());
            }
        }
        else if (state == Pda::FOUND_NESTED_TEMPLATE_CALL) {
            saved_iter = iter;
            iter = std::prev(std::prev(iter));
            processOneNestedCall(std::next(iter));
            argv.pop_back();
        }
        else if (state == Pda::FOUND_TEMPLATE_CALL_END) {

            if (saved_state != Pda::FOUND_TEMPLATE_CALL_END) {
                for (auto &x : table) {
                    if (name == x.getName()) {
                        x.instantiate(&tokens, argv, std::prev(call));
                        done = true;
                    }
                    else if (name == x.getAdditionalName()) {
                        x.instantiate(&tokens, argv, std::prev(call));
                        done = true;
                    }
                }
                if (done) {
                    TemplateSymbols::modifyCall(&tokens, std::prev(call), argv);
                    done = false;
                }
                else {
                    throw SyntaxException(std::prev(call)->getLine(), std::prev(call)->getOffset(),
                        "Unknown template name \'" + name + "\'");
                }
                argv.clear();

                tokens.erase(call, std::next(iter));
                return;
            }
        }
        else if (state == Pda::FAILED_STATE) {
            throw SyntaxException(iter->getLine(), iter->getOffset(),
                "Unexpected token \'" + iter->getName() + "\'");
        }

        ++iter;
    }
}

void Preprocessor::parse(const std::string &data)
{
    tokenize(data);
    parseSyntax();
}

} // namespace Tec
