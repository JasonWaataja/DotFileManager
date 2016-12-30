/*
 * Copyright (c) 2016 Jason Waataja
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "util.h"

#include <err.h>
#include <pwd.h>
#include <unistd.h>
#include <wordexp.h>

#include <iostream>

namespace dfm {

bool
getYesOrNo()
{
    /* Do the same behavior as with a prompt but with no prompt. */
    return getYesOrNo("");
}

bool
getYesOrNo(const std::string& prompt)
{
    std::cout << prompt << " ";
    std::string input;

    if (!std::getline(std::cin, input))
        errx(EXIT_FAILURE, "Failed to read input.");

    bool yesOrNo = true;
    while (!lineIsYesOrNo(input, yesOrNo)) {
        std::cout << "Please enter y or n." << std::endl;
        if (!std::getline(std::cin, input))
            errx(EXIT_FAILURE, "Failed to read input.");
    }

    return yesOrNo;
}

bool
lineIsYesOrNo(const std::string& input, bool& yesOrNo)
{
    /*
     * This could be done by first checking if input's lenght is zero, then
     * checking the first character with input[0], but this is cooler and could
     * be extended to non-zero length character sequences.
     */
    if (input.find("Y") == 0 || input.find("y") == 0) {
        yesOrNo = true;
        return true;
    }
    if (input.find("N") == 0 || input.find("n") == 0) {
        yesOrNo = false;
        return true;
    }

    return false;
}

std::string
getCurrentDirectory()
{
    size_t currentSize = 128;
    char* currentDirectory = new char[currentSize];
    char* result = getcwd(currentDirectory, currentSize);
    while (result == NULL) {
        if (errno == ERANGE) {
            delete[] currentDirectory;
            currentSize *= 2;
            currentDirectory = new char[currentSize];
        } else
            err(EXIT_FAILURE, "Failed to get current directory.");
        result = getcwd(currentDirectory, currentSize);
    }
    std::string directoryString(currentDirectory);
    delete[] currentDirectory;
    return directoryString;
}

std::string
shellExpandPath(const std::string& path)
{
    wordexp_t expr;
    if (wordexp(path.c_str(), &expr, 0) != 0)
        errx(EXIT_FAILURE, "Failed to expand path.");
    if (expr.we_wordc != 1)
        errx(EXIT_FAILURE, "Path expanded into multiple words.");

    std::string expandedPath = expr.we_wordv[0];
    wordfree(&expr);
    return expandedPath;
}

std::string
getHomeDirectory()
{
    struct passwd* userInfo = getpwuid(getuid());
    if (userInfo == NULL) {
        err(EXIT_FAILURE, "Failed to get user info.");
    }
    return userInfo->pw_dir;
}
} /* namespace dfm */
