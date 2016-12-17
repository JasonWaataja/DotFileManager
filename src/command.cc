

#include "command.h"

#include <assert.h>
#include <err.h>

namespace dfm {

Command::Command() : callableNames(), arguments(), action(getDefaultAction())
{
}

Command::Command(const std::string& name)
    : callableNames(1), arguments(), action(getDefaultAction())
{
    callableNames.push_back(name);
}

Command::Command(const std::string& name, int argc, const std::string argv[])
    : callableNames(1), arguments(argc), action(getDefaultAction())
{
    callableNames.push_back(name);
    for (int i = 0; i < argc; i++)
        arguments.push_back(argv[i]);
}

Command::Command(
    const std::string& name, const std::vector<std::string>& arguments)
    : callableNames(1), arguments(arguments), action(getDefaultAction())
{
    callableNames.push_back(name);
}

const std::vector<std::string>&
Command::getCallableNames() const
{
    return callableNames;
}

void
Command::addCallableName(const std::string& name)
{
    callableNames.push_back(name);
}

void
Command::setCallableNames(const std::vector<std::string>& names)
{
    /*
     * I hope this is copied quickly. I'm not sure if I should use std::copy
     * here.
     */
    callableNames = names;
}

const std::vector<std::string>&
Command::getArguments() const
{
    return arguments;
}

void
Command::setArguments(int argc, const std::string argv[])
{
    assert(argc >= 0);
    arguments.resize(argc);
    for (int i = 0; i < argc; i++)
        arguments[i] = argv[i];
}

void
Command::setArguments(const std::vector<std::string>& arguments)
{
    /* I'm not sure if it's better to use std::copy here. */
    this->arguments = arguments;
}

std::function<bool(const std::vector<std::string>&)>
Command::getAction() const
{
    return action;
}

void
Command::setAction(std::function<bool(const std::vector<std::string>&)> action)
{
    this->action = action;
}

bool
Command::execute()
{
    return action(arguments);
}


std::function<bool(const std::vector<std::string>)>
Command::getDefaultAction()
{
    auto action = [](const std::vector<std::string>&)->bool {
        warnx("Calling command without behavior.");
        return true;
    };
    return action;
}
}
