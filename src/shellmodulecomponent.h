

#ifndef SHELL_MODULE_COMPONENT_H
#define SHELL_MODULE_COMPONENT_H

#include <string>
#include <vector>

#include "modulecomponent.h"

namespace dfm {

const char shellProcess[] = "/usr/bin/env bash";

class ShellModuleComponent : public ModuleComponent {
public:
    ShellModuleComponent();
    const std::vector<std::string>& getShellCommands();

    bool executeCommands();
    void addCommand(const std::string& command);

private:
    std::vector<std::string> shellCommands;
};
}

#endif /* SHELL_MODULE_COMPONENT_H */
