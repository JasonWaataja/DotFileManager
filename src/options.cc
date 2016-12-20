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
#include <getopt.h>

namespace dfm {

/*
 * I couyld use default values since this requires C++ 11 and it's easier and
 * easier to read, but it only really offers a benefit it there are multiple
 * constructors.
 */
DfmOptions::DfmOptions()
    : installModulesFlag(false),
      uninstallModulesFlag(false),
      allFlag(false),
      promptForDependenciesFlag(false),
      hasSourceDirectory(false)
{
}

bool
DfmOptions::loadFromArguments(int argc, char* argv[])
{
    int optionIndex = 0;

    /*
     * ClangFormat does a weird thing here, but I don't want to add a
     * suppression and I'll just leave it.
     */
    struct option longOptions[] = { { "install", no_argument, NULL, 'i' },
        { "uninstall", no_argument, NULL, 'u' },
        { "all", no_argument, NULL, 'a' },
        { "prompt-for-dependencies", no_argument, NULL, 'p' },
        { "directory", required_argument, NULL, 'd' }, { 0, 0, 0, 0 } };

    int getoptValue = getopt_long_only(
        argc, argv, GETOPT_SHORT_OPTIONS, longOptions, &optionIndex);

    while (getoptValue != -1) {
        switch (getoptValue) {
        case 0:
            /*
             * A flag was set, so do nothing. In the getopt example on the
             * glibc documentation, they do an additional check to see if it
             * sets a flag and something else if it's not, but in the
             * documentation they give I don't see any other reason it could
             * take 0. I'm kind of confused about it.
             */
            break;
        case 'i':
            installModulesFlag = true;
            break;
        case 'u':
            uninstallModulesFlag = true;
            break;
        case 'a':
            allFlag = true;
            break;
        case 'p':
            promptForDependenciesFlag = true;
            break;
        case 'd':
            hasSourceDirectory = true;
            sourceDirectory = optarg;
            break;
        case '?':
            /*
             * This infuriates me, it doesn't say in the documentation for
             * glibc where it stores the option name if it encounters an
             * unknown option. I did testing and it's not options[optionIndex]
             * and it's not optopt. I guess I just won't print it.
             */
            warnx("Unrecognized argument.");
            /* TODO: Print usage here. */
            return false;
        case ':':
            /* Same problem as above. */
            warnx("Missing required argument.");
            /* TODO: Print usage. */
            return false;
        }
        getoptValue = getopt_long_only(
            argc, argv, GETOPT_SHORT_OPTIONS, longOptions, &optionIndex);
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
