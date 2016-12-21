/*
 * Copyright (c) 2016 Jason Waataja
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "configfilereader.h"

#include <ctype.h>
#include <stdarg.h>

#include <exception>
#include <regex>

#include "dependencyaction.h"

namespace dfm {

ConfigFileReader::ConfigFileReader(const std::string& path)
    : path(path), reader(path)
{
    addDefaultCommands();
}

ConfigFileReader::ConfigFileReader(const boost::filesystem::path& path)
    : ConfigFileReader(path.string())
{
}

ConfigFileReader::ConfigFileReader(
    const boost::filesystem::path& path, std::shared_ptr<DfmOptions> options)
    : path(path), reader(path.string()), options(options)
{
    addDefaultCommands();
}

ConfigFileReader::ConfigFileReader(const char* path)
    : ConfigFileReader(std::string(path))
{
}

std::shared_ptr<DfmOptions>
ConfigFileReader::getOptions()
{
    return options;
}

void
ConfigFileReader::setOptions(std::shared_ptr<DfmOptions> options)
{
    /*
     * I read the page for operator= on std::shared_ptr on cppreference.com.
     * What I'm worried about is that it will not decrement the reference count
     * of the pointer currently stored in this->options when using the =
     * operator on its own. The cppreference.com page for reset says it does,
     * though, so I'm resetting it first just in case.
     */
    this->options.reset();
    this->options = options;
    environment.setOptions(options);
}

const boost::filesystem::path&
ConfigFileReader::getPath() const
{
    return path;
}

void
ConfigFileReader::setPath(const std::string& path)
{
    this->path = path;
}

void
ConfigFileReader::setPath(const boost::filesystem::path& path)
{
    this->path = path;
}

const ReaderEnvironment&
ConfigFileReader::getEnvironment() const
{
    return environment;
}

void
ConfigFileReader::setEnvironment(const ReaderEnvironment& environment)
{
    this->environment = environment;
}

bool
ConfigFileReader::isOpen()
{
    return this->reader.is_open();
}

bool
ConfigFileReader::isEmptyLine(const std::string& line) const
{
    return line.length() == 0;
}

bool
ConfigFileReader::isComment(const std::string& line, int expectedIndents) const
{
    int currentIndex = 0;
    for (; currentIndex < expectedIndents + 1 && currentIndex < line.length();
         currentIndex++) {
        if (line[currentIndex] == COMMENT_DELIMITER)
            return true;
        else if (line[currentIndex] != '\t')
            return false;
    }

    if (currentIndex < line.length()
        && line[currentIndex] == COMMENT_DELIMITER)
        return true;

    return false;
}

int
ConfigFileReader::indentCount(const std::string& line) const
{
    int indents = 0;
    /* I could probably use an empty for loop for this. */
    while (indents < line.length() && line[indents] == '\t')
        indents++;
    return indents;
}

int
ConfigFileReader::getExpectedIndents() const
{
    if (inShell)
        return 2;

    if (inModuleInstall || inModuleUninstall)
        return 1;

    return 0;
}

bool
ConfigFileReader::isModuleLine(
    const std::string& line, std::string& moduleName)
{
    if (isEmptyLine(line) || isComment(line, 0))
        return false;
    /*
     * Match a non-white character at the start, then any more characters and a
     * colon at the end. Capture the line, except the final colon and any
     * whitespace between the colon and the name. The name may include
     * whitespace. There may be trailing whitespace after the colon.
     */
    try {
        std::regex re("^(\\S+(?:\\s+\\S+)*)\\s*:\\s*$");
        std::smatch match;
        if (std::regex_match(line, match, re)) {
            moduleName = match.str(1);
            return true;
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return false;
}

bool
ConfigFileReader::isModuleLine(const std::string& line)
{
    if (isEmptyLine(line) || isComment(line, 0))
        return false;
    std::regex re("^(\\S+(?:\\s+\\S+)*)\\s*:\\s*$");
    if (std::regex_match(line, re))
        return false;
    return true;
}

bool
ConfigFileReader::isUninstallLine(const std::string& line)
{
    if (isEmptyLine(line) || isComment(line, 0))
        return false;
    /*
     * The line must start with uninstall, then there must be a colon, which
     * may be surrounded by whitespace.
     */
    std::regex re("^uninstall\\s*:\\s$");
    if (std::regex_match(line, re)) {
        return true;
    }
    return false;
}

bool
ConfigFileReader::isShellCommand(const std::string& commandName)
{
    return commandName == "sh" || commandName == "shell";
}

std::string
ConfigFileReader::stripIndents(const std::string& line, int indents)
{
    std::string newLine = line;
    int indentsToErase = 0;
    for (; indentsToErase < line.length() && indentsToErase < indents
         && newLine[indentsToErase] == '\t';
         indentsToErase++)
        ;
    newLine.erase(0, indentsToErase);
    return newLine;
}

void
ConfigFileReader::addShellAction(const std::string& line)
{
    if (inShell) {
        currentShellAction->addCommand(stripIndents(line, 2));
    }
}

void
ConfigFileReader::flushShellAction()
{
    if (inModuleInstall) {
        currentModule->addInstallAction(
            std::shared_ptr<ModuleAction>(currentShellAction));
        inShell = false;
        currentShellAction = nullptr;
    } else if (inModuleUninstall) {
        currentModule->addUninstallAction(
            std::shared_ptr<ModuleAction>(currentShellAction));
        inShell = false;
        currentShellAction = nullptr;
    }
}

bool
ConfigFileReader::processLineAsCommand(const std::string& line)
{
    std::string localLine = stripIndents(line, 1);
    /*
     * Match a string that's not whitespace at the beginning, which is the
     * command. Capture the command and the rest of the line.
     */
    std::regex commandRe("^(\\S+).*$");
    std::smatch matchResults;
    if (!std::regex_match(localLine, matchResults, commandRe)) {
        warnx("line %i: No command found for line: %s", currentLineNo,
            line.c_str());
        return false;
    }

    std::string command = matchResults.str(1);
    localLine =
        localLine.substr(matchResults.position(1) + matchResults.length(1),
            localLine.length() - matchResults.length(1));
    /*
     * Match two quotes with as many characters between them that can be the
     * escape sequence \" as possible. I think the quotation one has to come
     * first in the or statement, otherwise it would always include the
     * quotes.
     */
    std::regex argumentsRe("\\s+(\"(?:\\\\\"|[^\"])*\"|\\S+)");
    std::sregex_iterator next(localLine.begin(), localLine.end(), argumentsRe);
    std::sregex_iterator end;
    std::vector<std::string> arguments;
    while (next != end) {
        std::smatch match = *next;
        /* We don't want the whole match, just the capture group. */
        arguments.push_back(match.str(1));
        next++;
    }

    if (isShellCommand(command)) {
        if (!Command::checkArgumentCountEqual(arguments, 0))
            return false;
        inShell = true;
        currentShellAction = new ShellAction;
        return true;
    }

    return processCommand(command, arguments);
}

bool
ConfigFileReader::processCommand(
    const std::string& commandName, const std::vector<std::string>& arguments)
{
    for (Command command : commands) {
        if (command.matchesName(commandName)) {
            std::shared_ptr<ModuleAction> action =
                command.createAction(arguments, environment);
            if (inModuleInstall) {
                currentModule->addInstallAction(action);
                return true;
            } else if (inModuleUninstall) {
                currentModule->addUninstallAction(action);
                return true;
            } else {
                warnx("line %i: Tryng to add action when not in module "
                      "install or uninstall: %s",
                    currentLineNo, commandName.c_str());
                return false;
            }
        }
    }

    warnx("line %i: no matching command for name \"%s\"", currentLineNo,
        commandName.c_str());

    return false;
}

void
ConfigFileReader::startNewModule(const std::string& name)
{
    currentModule = new Module(name);
    inModuleInstall = true;
}

void
ConfigFileReader::changeToUninstall()
{
    if (!inModuleInstall) {
        warnx("line %i: Attempting to uninstall module without module.",
            currentLineNo);
        return;
    }

    inModuleInstall = false;
    inModuleUninstall = true;
}

void
ConfigFileReader::close()
{
    if (inModuleInstall || inModuleUninstall)
        warnx("Attempting to close reader while still reading.");

    reader.close();
}

void
ConfigFileReader::addCommand(
    std::function<std::shared_ptr<ModuleAction>(
        const std::vector<std::string>&, const ReaderEnvironment&)>
        createActionFunction,
    const char* firstName, ...)
{
    Command command(std::string(firstName), createActionFunction);
    va_list argumentList;
    va_start(argumentList, firstName);
    const char* name = va_arg(argumentList, const char*);
    while (name != NULL) {
        command.addCallableName(std::string(name));
        name = va_arg(argumentList, const char*);
    }
    commands.push_back(command);
    va_end(argumentList);
}

void
ConfigFileReader::addCommand(
    std::function<std::shared_ptr<ModuleAction>(
        const std::vector<std::string>&, const ReaderEnvironment&)>
        createActionFunction,
    Command::ArgumentCheck argumentCheckingType, int expectedArgumentCount,
    const char* firstName, ...)
{
    Command command(std::string(firstName), createActionFunction);
    va_list argumentList;
    va_start(argumentList, firstName);
    const char* name = va_arg(argumentList, const char*);
    while (name != NULL) {
        command.addCallableName(std::string(name));
        name = va_arg(argumentList, const char*);
    }
    va_end(argumentList);

    switch (argumentCheckingType) {
    case Command::NO_ARGUMENT_CKECK:
        command.setNoArgumentChecking();
        break;
    case Command::EXACT_COUNT_ARGUMENT_CHECK:
        command.setExactAgumentChecking(expectedArgumentCount);
        break;
    case Command::MINIMUM_COUNT_ARGUMENT_CHECK:
        command.setMinimumCountArgumentCheck(expectedArgumentCount);
        break;
    }

    commands.push_back(command);
}

std::shared_ptr<ModuleAction>
ConfigFileReader::createMessageAction(
    const std::vector<std::string>& arguments,
    const ReaderEnvironment& environment)
{
    return std::shared_ptr<ModuleAction>(new MessageAction(arguments[0]));
}

std::shared_ptr<ModuleAction>
ConfigFileReader::createDependenciesAction(
    const std::vector<std::string>& arguments,
    const ReaderEnvironment& environment)
{
    std::shared_ptr<DependencyAction> action(new DependencyAction(arguments));
    action->setPromptDependencies(
        environment.getOptions()->promptForDependenciesFlag);
    return action;
}

void
ConfigFileReader::addDefaultCommands()
{
    addCommand(&ConfigFileReader::createMessageAction,
        Command::EXACT_COUNT_ARGUMENT_CHECK, 1, "message", "msg", "echo", "m",
        NULL);
    addCommand(&ConfigFileReader::createDependenciesAction,
        Command::NO_ARGUMENT_CKECK, -1, "dependencies", "dep", "depend", NULL);
}

} /* namespace dfm */
