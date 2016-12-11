

#include "shellmodulecomponent.h"

#include <err.h>
#include <iostream>
#include <stdio.h>
#include <sstream>

namespace dfm {

ShellModuleComponent::ShellModuleComponent() : shellCommands()
{
}

const std::vector<std::string>&
ShellModuleComponent::getShellCommands()
{
    return shellCommands;
}

bool
ShellModuleComponent::executeCommands()
{
    FILE *shell = popen(shellProcess, "w");
    if (shell == NULL) {
        perror("Failed to open shell");
        return false;
    }

    /* Set the shell to exit on any failed commands. */
    fprintf(shell, "set -e\n") ;
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
ShellModuleComponent::addCommand(const std::string& command)
{
    shellCommands.push_back(command);
}
}
