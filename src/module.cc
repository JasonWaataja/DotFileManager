

#include "module.h"

#include <err.h>

namespace dfm {

Module::Module() : name(defaultModuleName)
{
}

Module::Module(const std::string& name) : name(name)
{
}

void
Module::addComponent(std::shared_ptr<ModuleComponent> component)
{
    modules.push_back(component);
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
    for (std::shared_ptr<ModuleComponent> component : modules) {
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
    for (std::shared_ptr<ModuleComponent> component : modules) {
        bool status = component->uninstall();
        if (!status) {
            warnx("failed to install module %s", component->getName().c_str());
            return false;
        }
    }

    return true;
}
}
