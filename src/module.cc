

#include "module.h"

#include <err.h>

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
    for (ModuleComponent* component : modules) {
        bool status = component->install();
        if (!status) {
            warnx("failed to install module %s", component->getName().c_str());
            return false;
        }
    }

    return true;
}

bool
Module::uninstall()
{
    for (ModuleComponent* component : modules) {
        bool status = component->uninstall();
        if (!status) {
            warnx("failed to install module %s", component->getName().c_str());
            return false;
        }
    }

    return true;
}
}
