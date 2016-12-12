

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
    for (std::shared_ptr<ModuleAction> action : installActions) {
        bool status = action->performAction();
        if (!status) {
            warnx("failed to install module %s", action->getName().c_str());
            return false;
        }
    }

    return true;
}

bool
Module::uninstall()
{
    for (std::shared_ptr<ModuleAction> action : uninstallActions) {
        bool status = action->performAction();
        if (!status) {
            warnx("failed to install module %s", action->getName().c_str());
            return false;
        }
    }

    return true;
}
}
