

#ifndef SHELL_MODULE_COMPONENT_H
#define SHELL_MODULE_COMPONENT_H

#include <string>
#include <vector>

#include "moduleaction.h"

namespace dfm {

const char SHELL_PROCESS[] = "/usr/bin/env bash";
const char DEFAULT_SHELL_ACTION_NAME[] = "shell command";

class ShellAction : public ModuleAction {
public:
    ShellAction();
    ShellAction(const std::string& name);
    const std::vector<std::string>& getShellCommands() const;

    bool performAction() override;
    void addCommand(const std::string& command);

private:
    std::vector<std::string> shellCommands;
};
}

#endif /* SHELL_MODULE_COMPONENT_H */
