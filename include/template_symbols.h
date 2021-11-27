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

#ifndef TEMPLATE_SYMBOLS_H
#define TEMPLATE_SYMBOLS_H

#include <string>
#include <vector>

#include "token.h"

namespace Tec {

class TemplateSymbols {
public:
    TemplateSymbols(const std::string &name,
        const std::vector<std::string> &argv,
        const TokenList &chain,
        const TokenList::iterator &entry);
    TemplateSymbols(const std::string &name,
        const std::vector<std::string> &argv,
        const TokenList &chain,
        const TokenList::iterator &entry,
        const std::string &add_name);
    void instantiate(TokenList *tokens,
        const std::vector<std::string> &in_argv,
        const TokenList::iterator &call);
    static void modifyCall(TokenList *tokens,
        const TokenList::iterator &call,
        const std::vector<std::string> &in_argv);
    std::string getName(void) const { return name; }
    std::string getAdditionalName(void) const { return additional_name; }
    void changeName(const std::string &in_name) { name = in_name; }
private:
    static std::string typeNameMangling(const std::string &name);
    static std::string funcNameMangling(const std::string &name,
        const std::vector<std::string> &argv);
    void processNestedCall(TokenList *tokens, TokenList::iterator call);
    std::string name;
    std::vector<std::string> argv;
    TokenList chain;
    TokenList::iterator entry;
    std::string additional_name;
    std::vector<std::vector<std::string>> saved_argv;
}; // End of class

using TemplateSymbolsTable = std::list<TemplateSymbols>;

} // namespace Tec

#endif // TEMPLATE_SYMBOLS_H
