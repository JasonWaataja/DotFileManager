

#include "command.h"

#include <assert.h>
#include <err.h>

#include <algorithm>
#include <iostream>
#include <sstream>

namespace dfm {

Command::Command()
    : callableNames(),
      createActionFunction(getDefaultAction()),
      argumentCheckingType(NO_ARGUMENT_CKECK),
      expectedArgumentCount(-1)
{
}

Command::Command(const std::string& name)
    : callableNames(1),
      createActionFunction(getDefaultAction()),
      argumentCheckingType(NO_ARGUMENT_CKECK),
      expectedArgumentCount(-1)
{
    callableNames.push_back(name);
}

Command::Command(const std::string& name,
    std::function<std::shared_ptr<ModuleAction>(
        const std::vector<std::string>&)>
        createActionFunction)
    : callableNames(1),
      createActionFunction(createActionFunction),
      argumentCheckingType(NO_ARGUMENT_CKECK),
      expectedArgumentCount(-1)
{
    callableNames.push_back(name);
}

void
Command::setNoArgumentChecking()
{
    argumentCheckingType = NO_ARGUMENT_CKECK;
    expectedArgumentCount = -1;
}

void
Command::setExactAgumentChecking(int argc)
{
    assert(argc >= 0);
    argumentCheckingType = EXACT_COUNT_ARGUMENT_CHECK;
    expectedArgumentCount = argc;
}

void
Command::setMinimumCountArgumentCheck(int argc)
{
    assert(argc >= 0);
    argumentCheckingType = MINIMUM_COUNT_ARGUMENT_CHECK;
    expectedArgumentCount = argc;
}

Command::ArgumentCheck
Command::getArgumentCheckingType() const
{
    return argumentCheckingType;
}

int
Command::getExpectedArgumentCount() const
{
    if (argumentCheckingType == NO_ARGUMENT_CKECK)
        return -1;
    return expectedArgumentCount;
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

std::function<std::shared_ptr<ModuleAction>(const std::vector<std::string>&)>
Command::getCreateActionFunction() const
{
    return createActionFunction;
}

void
Command::setCreateActionFunction(std::function<std::shared_ptr<ModuleAction>(
        const std::vector<std::string>&)>
        createActionFunction)
{
    this->createActionFunction = createActionFunction;
}

std::shared_ptr<ModuleAction>
Command::createAction(const std::vector<std::string>& arguments)
{
    if (argumentCheckingType == EXACT_COUNT_ARGUMENT_CHECK
        && !checkArgumentCountEqual(arguments, expectedArgumentCount))
        return std::shared_ptr<ModuleAction>();

    if (argumentCheckingType == MINIMUM_COUNT_ARGUMENT_CHECK
        && !checkArgumentCountAtLeast(arguments, expectedArgumentCount))
        return std::shared_ptr<ModuleAction>();

    return createActionFunction(arguments);
}

std::shared_ptr<ModuleAction>
Command::createAction(int argc, const std::string argv[])
{
    assert(argc >= 0);
    std::vector<std::string> arguments(argc);
    for (int i = 0; i < argc; i++)
        arguments[i] = argv[i];
    return createAction(arguments);
}

std::function<std::shared_ptr<ModuleAction>(const std::vector<std::string>&)>
Command::getDefaultAction()
{
    auto createActionFunction = [](
        const std::vector<std::string>&) -> std::shared_ptr<ModuleAction> {
        warnx("Calling command without behavior.");
        return std::shared_ptr<ModuleAction>();
    };
    return createActionFunction;
}

bool
Command::checkArgumentCountEqual(
    const std::vector<std::string>& arguments, int argc)
{
    assert(argc >= 0);

    if (arguments.size() != argc) {
        /*
         * Used a string stream here because it makes things more portable. On
         * my system, arguments.size() is an unsigned long, but I'm not sure if
         * that will be true everywhere so I went with this.
         */
        std::ostringstream messageStream;
        messageStream << "Incorrect number of arguments, expected exactly "
                      << argc << ", got " << arguments.size() << ".";
        warnx("%s", messageStream.str().c_str());
        return false;
    }
    return true;
}

bool
Command::checkArgumentCountAtLeast(
    const std::vector<std::string>& arguments, int argc)
{
    assert(argc >= 0);

    if (arguments.size() < argc) {
        /*
         * Used a string stream here because it makes things more portable. On
         * my system, arguments.size() is an unsigned long, but I'm not sure if
         * that will be true everywhere so I went with this.
         */
        std::ostringstream messageStream;
        messageStream << "Incorrect number of arguments, expected at least "
                      << argc << ", got " << arguments.size() << ".";
        warnx("%s", messageStream.str().c_str());
        return false;
    }
    return true;
}

bool
Command::matchesName(const std::string& name) const
{
    return std::find(callableNames.begin(), callableNames.end(), name)
        != callableNames.end();
}
}
