

#include "modulecomponent.h"

namespace dfm {

ModuleComponent::ModuleComponent() : name()
{
}

ModuleComponent::ModuleComponent(const std::string& name) : name(name)
{
}

const std::string&
ModuleComponent::getName() const
{
    return name;
}

void
ModuleComponent::setName(const std::string& name)
{
    this->name = name;
}
}
