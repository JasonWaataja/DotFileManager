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

#include "filecheck.h"

#include <sys/stat.h>

#include <dirent.h>
#include <err.h>

#include <fstream>
#include <unordered_set>

namespace dfm {

FileCheck::FileCheck()
{
}

FileCheck::FileCheck(
    const std::string& sourcePath, const std::string& destinationPath)
    : sourcePath(sourcePath), destinationPath(destinationPath)
{
}

const std::string&
FileCheck::getSourcePath() const
{
    return sourcePath;
}

void
FileCheck::setSourcePath(const std::string& sourcePath)
{
    this->sourcePath = sourcePath;
}

const std::string&
FileCheck::getDestinationPath() const
{
    return destinationPath;
}

void
FileCheck::setDestinationPath(const std::string& destinationPath)
{
    this->destinationPath = destinationPath;
}

void
FileCheck::setFiles(
    const std::string& sourcePath, const std::string& destinationPath)
{
    setSourcePath(sourcePath);
    setDestinationPath(destinationPath);
}

bool
FileCheck::regularPathsEqual(
    const std::string& path1, const std::string& path2)
{
    if (path1 == path2)
        return true;
    if (path1.length() == 0 || path2.length() == 0)
        return false;

    std::ifstream file1Reader(path1.c_str());
    if (!file1Reader.is_open()) {
        warnx("Failed to open file %s.", path1.c_str());
        return false;
    }
    std::ifstream file2Reader(path2.c_str());
    if (!file2Reader.is_open()) {
        warnx("Failed to open file %s.", path2.c_str());
        return false;
    }
    std::string line1;
    std::string line2;

    bool path1ReadStatus = true;
    bool path2ReadStatus = false;
    /* I wish I knew how to directly assign a getline status to a bool. */
    if (std::getline(file1Reader, line1))
        path1ReadStatus = true;
    else
        path2ReadStatus = false;

    if (std::getline(file2Reader, line2))
        path2ReadStatus = true;
    else
        path2ReadStatus = false;

    if (path1ReadStatus != path2ReadStatus)
        return false;

    while (path1ReadStatus && path2ReadStatus) {
        if (line1 != line2)
            return false;

        if (std::getline(file2Reader, line2))
            path2ReadStatus = true;
        else
            path2ReadStatus = false;

        if (path1ReadStatus != path2ReadStatus)
            return false;
    }

    return true;
}
} /* namespace dfm */
