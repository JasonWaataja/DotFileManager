

#include "module.h"

namespace dfm {

Module::Module() : name(defaultModuleName)
{
}

Module::Module(const std::string& name) : name(name)
{
}

const std::string&
Module::getName() const
{
    return name;
}

void
Module::setName(const std::string& name)
{
    this->name = name;
}

bool
Module::install()
{
    bool noErrors = true;
    for (int i = 0; i < modules.size() && noErrors; i++) {
        ModuleComponent* component = modules[i];
        noErrors = component->install();
    }

    return noErrors;
}

bool
Module::uninstall()
{
    bool noErrors = false;
    for (int i = 0; i < modules.size() && noErrors; i++) {
        ModuleComponent* component = modules[i];
        noErrors = component->uninstall();
    }

    return noErrors;
}
