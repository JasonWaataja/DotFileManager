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

#include "options.h"

#include <err.h>
#include <unistd.h>

namespace dfm {

DfmOptions::DfmOptions()
    : installModulesFlag(false),
      uninstallModulesFlag(false),
      allFlag(false),
      remainingArguments(),
      hasSourceDirectory(false),
      sourceDirectory()
{
}

bool
DfmOptions::loadFromArguments(int argc, char* argv[])
{
    installModulesFlag = false;
    uninstallModulesFlag = false;
    allFlag = false;
    remainingArguments.clear();
    hasSourceDirectory = false;
    sourceDirectory = boost::filesystem::path();

    /* Print errors when getopt encounters an error. */
    opterr = 1;

    int getoptResult = getopt(argc, argv, GETOPT_OPTIONS);
    while (getoptResult != -1) {
        switch (getoptResult) {
        case 'i':
            installModulesFlag = true;
            break;
        case 'u':
            uninstallModulesFlag = true;
            break;
        case 'a':
            allFlag = true;
            break;
        case 'd':
            hasSourceDirectory = true;
            sourceDirectory = boost::filesystem::path(optarg);
        case ':':
            if (isprint(optopt))
                warnx("Missing required argument for \"-%c\".", optopt);
            else
                warnx("Missing required argument.");
            return false;
        case '?':
            warnx("Unknown argument \"-%c\".", optopt);
            return false;
        }
        getoptResult = getopt(argc, argv, GETOPT_OPTIONS);
    }

    for (int i = optind; i < argc; i++)
        remainingArguments.push_back(std::string(argv[i]));

    return true;
}

bool
DfmOptions::verifyArguments() const
{
    if (!verifyFlagsConsistency())
        return false;
    if (!verifyFlagsHaveArguments())
        return false;
    if (!verifyDirectoryExists())
        return false;
    return true;
}

bool
DfmOptions::verifyFlagsConsistency() const
{
    if (installModulesFlag && uninstallModulesFlag) {
        warnx("Both install and uninstall flags passed.");
        return false;
    }
    if (allFlag && !installModulesFlag && !uninstallModulesFlag) {
        warnx("The \"-a\" flag cannot be used without the \"-i\" or \"-u\" "
              "flags.");
        return false;
    }
    return true;
}

bool
DfmOptions::verifyFlagsHaveArguments() const
{
    if ((installModulesFlag || uninstallModulesFlag) && !allFlag) {
        warnx("Must provide modules to install or uninstall.");
        return false;
    }
    return true;
}

bool
DfmOptions::verifyDirectoryExists() const
{
    if (hasSourceDirectory) {
        if (!boost::filesystem::exists(sourceDirectory)) {
            warnx("Directory doesn't exist: %s.", sourceDirectory.c_str());
            return false;
        }
        if (!boost::filesystem::is_directory(sourceDirectory)) {
            warnx("Given file isn't a directory: %s", sourceDirectory.c_str());
            return false;
        }
    }
    return true;
}
} /* namespace dfm */
