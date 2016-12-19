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

#include "shellaction.h"

#include <err.h>
#include <stdio.h>

#include <iostream>

namespace dfm {

ShellAction::ShellAction()
    : ModuleAction(DEFAULT_SHELL_ACTION_NAME), shellCommands()
{
}

ShellAction::ShellAction(const std::string& name)
    : ModuleAction(name), shellCommands()
{
}

const std::vector<std::string>&
ShellAction::getShellCommands() const
{
    return shellCommands;
}

bool
ShellAction::performAction()
{
    FILE* shell = popen(SHELL_PROCESS, "w");
    if (shell == NULL) {
        perror("Failed to open shell");
        return false;
    }

    /* Set the shell to exit on any failed commands. */
    fprintf(shell, "set -e\n");
    /* Execute commands, I'm not sure if this is the best way to do it. */
    for (std::string command : shellCommands) {
        fprintf(shell, "%s\n", command.c_str());
    }

    int status = pclose(shell);
    if (status != 0) {
        warnx("Non-zero exit status for shell");
        for (std::string command : shellCommands) {
            std::cerr << "\t" << command << std::endl;
        }
        return false;
    }

    return true;
}

void
ShellAction::addCommand(const std::string& command)
{
    shellCommands.push_back(command);
}
} /* namespace dfm */
