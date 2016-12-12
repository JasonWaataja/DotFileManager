

#include "shellaction.h"

#include <err.h>
#include <iostream>
#include <stdio.h>

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
}
