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

#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "preprocessor.h"
#include "syntax_exception.h"
#include "version.h"


bool read_from_file = true;
bool specify_output_file = false;

std::vector<std::string> input_filenames;
std::string output_filename;


static void showVersionInfo(void)
{
    std::cout << "tec " << TEC_VERSION_STR << " " << TEC_VERSION_DATE << std::endl;
}


static void showHelpInfo(void)
{
    std::cout
        << "Usage: tec [options...] [argments...]" << std::endl
        << "Templates Extension Compiler (TEC) version "
        << TEC_VERSION_STR << " " << TEC_VERSION_DATE << std::endl
        << std::endl
        << "Options:" << std::endl
        << "  -h, --help                Displays this message." << std::endl
        << "  -v, --version             Displays version information." << std::endl
        << "  -o <file>                 Specify output file name." << std::endl;
}


static int runTec(const std::vector<std::string> &args)
{
    for (size_t opt = 0; opt < args.size(); ++opt) {

        if (args[opt] == "-v" || args[opt] == "--version") {
            showVersionInfo();
            return EXIT_SUCCESS;
        }
        else if (args[opt] == "-h" || args[opt] == "--help") {
            showHelpInfo();
            return EXIT_SUCCESS;
        }
        else if (args[opt] == "-") {
            read_from_file = false;
        }
        else if (args[opt] == "-o") {
            if (++opt < args.size()) {
                output_filename = args[opt];
                specify_output_file = true;
            }
            else {
                std::cerr << "Missing argument for option: \"" << args[--opt]
                    << "\"" << std::endl;
                showHelpInfo();
                return EXIT_FAILURE;
            }
            if (input_filenames.size() > 1) {
                std::cerr << "Cannot specify -o when generating multiple output files" << std::endl;
                showHelpInfo();
                return EXIT_FAILURE;
            }
        }
        else {
            input_filenames.push_back(args[opt]);
            if (specify_output_file && input_filenames.size() > 1) {
                std::cerr << "Cannot specify -o when generating multiple output files" << std::endl;
                showHelpInfo();
                return EXIT_FAILURE;
            }
        }
    }

    std::string data;

    Tec::Preprocessor pp;

    if (!read_from_file) {

        data.assign(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());

        try {
            pp.parse(data);
            pp.generate(std::cout);
        }
        catch (const Tec::SyntaxException &e) {
            std::cerr << e.what() << std::endl;
        }
        return EXIT_SUCCESS;
    }


    for (const auto &input_filename : input_filenames) {

        std::ifstream input_file;

        input_file.open(input_filename, std::fstream::in);

        if (!input_file.is_open()) {
            std::cerr << "Failed to open file \"" << input_filename << "\"" << std::endl;
            return EXIT_FAILURE;
        }
        data.assign(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>());

        try {
            pp.parse(data);

            if (!specify_output_file) {
                output_filename = "tec_" + input_filename;
            }

            std::ofstream output_file(output_filename);
            pp.generate(output_file);

        }
        catch (const Tec::SyntaxException &e) {
            std::cerr << input_filename << ":" << e.what() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
    const std::vector<std::string> args(argv + 1, argv + argc);
    return runTec(args);
}
