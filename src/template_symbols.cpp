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

#include "template_symbols.h"
#include "syntax_exception.h"

namespace Tec {

TemplateSymbols::TemplateSymbols(const std::string &name,
    const std::vector<std::string> &argv,
    const TokenList &chain,
    const TokenList::iterator &entry)
    : name(name)
    , argv(argv)
    , chain(chain)
    , entry(entry)
{}

TemplateSymbols::TemplateSymbols(const std::string &name,
    const std::vector<std::string> &argv, const TokenList &chain,
    const TokenList::iterator &entry,
    const std::string &add_name)
    : name(name)
    , argv(argv)
    , chain(chain)
    , entry(entry)
    , additional_name(add_name)
{}

void TemplateSymbols::instantiate(TokenList *tokens,
    const std::vector<std::string> &in_argv,
    const TokenList::iterator &call)
{
    TokenList chain_with_types;
    bool modify = false;

    if (std::find(saved_argv.begin(), saved_argv.end(), in_argv) !=
        saved_argv.end())
    {
        return;
    }

    saved_argv.push_back(in_argv);

    if (in_argv.size() != argv.size()) {
        throw SyntaxException(call->getLine(), call->getOffset(),
            "Invalid number of types in call of template \'" + name + "\'");
    }

    std::string new_name(funcNameMangling(name, in_argv));
    std::string new_add_name(funcNameMangling(additional_name, in_argv));

    for (const auto &x : chain) {
        for (size_t i = 0; i < argv.size(); ++i) {
            if (x.getName() == argv[i]) {
                chain_with_types.push_back(Token(in_argv[i], x.getLine(), x.getOffset()));
                modify = true;
                break;
            }
            else if (x.getName() == name) {
                chain_with_types.push_back(Token(new_name, x));
                modify = true;
                break;
            }
            else if (x.getName() == additional_name) {
                chain_with_types.push_back(Token(new_add_name, x));
                modify = true;
                break;
            }
        }
        if (!modify) {
            chain_with_types.push_back(x);
        }
        else {
            modify = false;
        }
    }

    tokens->insert(entry, chain_with_types.begin(), chain_with_types.end());
    tokens->insert(entry, Token("\n", 0, 0));
}

void TemplateSymbols::modifyCall(TokenList *tokens,
    const TokenList::iterator &call, const std::vector<std::string> &argv)
{
    tokens->insert(call, Token(funcNameMangling(call->getName(), argv),
        *std::prev(call)));
    tokens->erase(call);
}

std::string TemplateSymbols::typeNameMangling(const std::string &name)
{
    std::string temp("_");
    auto iter = name.begin();

    for ( ; iter != name.end(); ++iter) {
        if (*iter == ' ') {
            continue;
        }
        else if (*iter == '*') {
            temp.append("ptr");
        }
        else {
            temp.append(1, *iter);
        }
    }

    return temp;
}

std::string TemplateSymbols::funcNameMangling(const std::string &name,
    const std::vector<std::string> &argv)
{
    std::string temp(name);
    for (const auto &type_name : argv) {
        temp.append(typeNameMangling(type_name));
    }
    return temp;
}

} // namespace Tec
