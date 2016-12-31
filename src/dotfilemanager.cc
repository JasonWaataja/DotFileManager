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

#include "dotfilemanager.h"

#include <stdlib.h>
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include "configfilereader.h"
#include "util.h"

namespace dfm {

DotFileManager::DotFileManager(int argc, char** argv) : argc(argc), argv(argv)
{
}

int
DotFileManager::run()
{
    if (!initializeOptions())
        return EXIT_FAILURE;
    if (!readModules())
        return EXIT_FAILURE;
    if (!performOperation())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

bool
DotFileManager::initializeOptions()
{
    options = std::shared_ptr<DfmOptions>(new DfmOptions());
    /*
     * These could be done in one if statement, but I might want to to
     * different things in each case later.
     */
    if (!options->loadFromArguments(argc, argv))
        return false;
    if (!options->verifyArguments())
        return false;

    /*
     * Change directories to the one specified by the options. This is so that
     * relative paths specified in the config file work.
     */
    if (options->hasSourceDirectory) {
        if (chdir(options->sourceDirectory.c_str()) == -1)
            err(EXIT_FAILURE, "Failed to change directory.");
    }

    return true;
}

bool
DotFileManager::readModules()
{
    std::string programDirectory;
    if (options->hasSourceDirectory)
        programDirectory = options->sourceDirectory;
    else
        programDirectory = getCurrentDirectory();

    std::string configFilePath = programDirectory + "/" + CONFIG_FILE_NAME;

    ConfigFileReader reader(configFilePath);
    reader.setOptions(options);

    bool status = reader.readModules(std::back_inserter(modules));

    if (!status) {
        warnx("Failed to read modules.");
        return false;
    }

    return true;
}

bool
DotFileManager::performOperation()
{
    if (options->allFlag) {
        for (Module module : modules) {
            if (!operateOn(module))
                return false;
        }
        return true;
    }

    for (std::string moduleName : options->remainingArguments) {
        auto nameMatches = [&moduleName](
            const Module& module) { return moduleName == module.getName(); };
        std::vector<Module>::iterator modulePosition =
            std::find_if(modules.begin(), modules.end(), nameMatches);
        if (modulePosition != modules.end()) {
            if (!operateOn(*modulePosition))
                return false;
        } else {
            warnx("Unknown module %s.", moduleName.c_str());
            return false;
        }
    }
    return true;
}

bool
DotFileManager::operateOn(const Module& module)
{
    bool status = true;
    std::string operationWord;
    if (options->interactiveFlag) {
        if (options->installModulesFlag)
            operationWord = "Install";
        else if (options->uninstallModulesFlag)
            operationWord = "Uninstall";
        else if (options->updateModulesFlag)
            operationWord = "Update";

        std::string prompt =
            operationWord + " module " + module.getName() + "?";
        if (!getYesOrNo(prompt))
            return true;
    }

    if (options->installModulesFlag)
        status = module.install();
    else if (options->uninstallModulesFlag)
        status = module.uninstall();
    else if (options->updateModulesFlag)
        status = module.update();
    else {
        warnx("No operation for module %s.", module.getName().c_str());
        return false;
    }
    return status;
}
} /* namespace dfm */
