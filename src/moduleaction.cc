

#include "moduleaction.h"

namespace dfm {

ModuleAction::ModuleAction() : name(DEFAULT_ACTION_NAME)
{
}

ModuleAction::ModuleAction(const std::string& name) : name(name)
{
}

const std::string&
ModuleAction::getName() const
{
    return name;
}

void
ModuleAction::setName(const std::string& name)
{
    this->name = name;
}
}
