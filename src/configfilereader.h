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

#ifndef CONFIG_FILE_READER_H
#define CONFIG_FILE_READER_H

#include <err.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "command.h"
#include "installaction.h"
#include "messageaction.h"
#include "module.h"
#include "removeaction.h"
#include "shellaction.h"

namespace dfm {

const char COMMENT_DELIMITER = '#';

class ConfigFileReader {
public:
    ConfigFileReader(const std::string& path);
    ConfigFileReader(const boost::filesystem::path& path);
    /*
     * This one is included to prevent ambiguity when using a string literal.
     */
    ConfigFileReader(const char* path);
    const boost::filesystem::path& getPath() const;
    void setPath(const std::string& path);
    void setPath(const boost::filesystem::path& path);

    bool isOpen();
    void close();
    /*
     * Read the modules in the given file and write them to the given iterator,
     * which must contain elements of type Module.
     *
     * Returns true on success, false on failure.
     */
    template <class OutputIterator> bool readModules(OutputIterator output);

    /*
     * Adds a command with the given action and given names. It takes a list of
     * null terminated list of C strings. The last argument must be NULL or
     * else something will go wrong.
     */
    void addCommand(std::function<std::shared_ptr<ModuleAction>(
                        const std::vector<std::string>&)>
                        createActionFunction,
        const char* firstName, ...);
    void addCommand(std::function<std::shared_ptr<ModuleAction>(
                        const std::vector<std::string>&)>
                        createActionFunction,
        Command::ArgumentCheck argumentCheckingType, int expectedArgumentCount,
        const char* firstName, ...);

    static std::shared_ptr<ModuleAction> createMessageAction(
        const std::vector<std::string>& arguments);

private:
    /* The path to the config file. */
    boost::filesystem::path path;
    /* The reader to open path with. */
    std::ifstream reader;
    /*
     * Whether or not the reader is in a module install which determines what
     * command lines are used for.
     */
    bool inModuleInstall;
    /* Returns whether or not the reader is in a module and whether or not it
     * was changed to an uninstall.
     */
    bool inModuleUninstall;
    /*
     * Pointer to a current module being constructed. Set to nullptr when not
     * currently in a module and is meant to be deleted when the module is
     * copied to the output.
     */
    Module* currentModule;
    /*
     * Whether or not the reader should process lines as shell commands to be
     * added to the current module install or uninstall.
     */
    bool inShell;
    /* The current action to append shell commands to. */
    ShellAction* currentShellAction;
    /*
     * The current line number of the reader, meant to be used with error
     * messages.
     */
    int currentLineNo;
    /*
     * The list of commands, which is checked against when processing a normal
     * command. It looks through these commands in order, so higher priority
     * commands should be first and if two commands share a name the one first
     * will be executed.
     */
    std::vector<Command> commands;

    void addDefaultCommands();

    /* Equivalent to line.length() == 0. */
    bool isEmptyLine(const std::string& line) const;
    /*
     * Tests if the given line is a comment. A line is a comment if the first
     * character in the line is COMMENT_DELIMITER, or if any of the next
     * expectedIndents characters are COMMENT_DELIMITER. The reason for this is
     * to embed the COMMENT_DELIMITER character in commands and in shell
     * commands.
     *
     * Returns whether or not the given line is a comment.
     */
    bool isComment(const std::string& line, int expectedIndents) const;
    /* Returns the number of '\t' characters at the start of the line. */
    int indentCount(const std::string& line) const;
    /*
     * Gets the expected number of indents based on the current state of the
     * reader. Being in a shell means it expects two, being in a module install
     * or uninstall makes it 1, and anything else makes it 0.
     *
     * Returns the number of expected indentations based on the reader state.
     */
    int getExpectedIndents() const;
    /*
     * Removes indents indents from the start of line and returns it.
     *
     * Returns a new string with indents indents stripped from it.
     */
    std::string stripIndents(const std::string& line, int indents);
    /*
     * Tests to see if the line starts a module and sets moduleName to the name
     * if this is so. These don't check if the line is blank or a comment and
     * process as is because it is assumes the checking was already done.
     *
     * If it does match the module syntax, stores the module name in line. Does
     * not affect line if the line is not a module line.
     *
     * Returns whether or not line matches the syntax for a new module line.
     */
    bool isModuleLine(const std::string& line, std::string& moduleName);
    /*
     * Tests to see if the line starts a module and sets moduleName to the name
     * if this is so. These don't check if the line is blank or a comment and
     * process as is because it is assumes the checking was already done.
     *
     * Returns whether or not line matches the syntax for a new module line.
     */
    bool isModuleLine(const std::string& line);
    /*
     * Tests line to see if it represents a line that uninstalls.
     *
     * Returns whether or not line matches the syntax for an uninstall line.
     */
    bool isUninstallLine(const std::string& line);
    /*
     * Tests commandName to see if it's a command that should activate shell
     * action.
     */
    bool isShellCommand(const std::string& commandName);

    /* Processing commands that affect object state. */
    void addShellAction(const std::string& line);
    /* Behavior changes if install or uninstall. */
    void flushShellAction();
    /*
     * Executes command or starts new shell.
     *
     * Split given line after the given number of indents. The first token must
     * be a word with no spaces. Each remaining token can either be a word on
     * its own, or a string surrounded by double-quote characters. Any double
     * quote characters on the inside will be part of a token. If the command
     * is successfully parsed, then the command is stored in command. The
     * arguments vector is cleared regardless, and any arguments are added to
     * it.
     *
     * Returns true on success, false on failure.
     */
    bool processLineAsCommand(const std::string& line);
    /* Executes command string with given arguments. */
    bool processCommand(const std::string& commandName,
        const std::vector<std::string>& arguments);

    /*
     * If the reader is in a module install or uninstall, finishes the module
     * and writes it to output, which must be an output iterator with type
     * module. This must be called or there will be a memory leak.
     */
    template <class OutputIterator> void flushModule(OutputIterator output);
    /*
     * Creates a new module with the given name and sets the current module to
     * it. This assumes that the last module has already be flushed, and don't
     * forget to do so.
     */
    void startNewModule(const std::string& name);
    /*
     * If in a modul install, change the current state so that actions are
     * added to the module's uninstall actions. Gives a warning if not in a
     * module install.
     */
    void changeToUninstall();

    /* The main function for processing a config file. Takes a line, and takes
     * the appropriate action based on the content of the line.
     *
     * This function does not increment currentLineNo. Do that in the method
     * that reads line the lines from the file reader.
     *
     * First checks to see if the line is empty or a comment based on the
     * expected indents. It then executes the appropriate action based on the
     * contents of the line, whether that be to add a shell action, flush the
     * current module, or start a new one. Does a lot of error checking and
     * gives warnings for malformed config files.
     *
     * Returns whether or not parsing the line was successful, which would
     * likely fail because of a malformed line.
     */
    template <class OutputIterator>
    bool processLine(const std::string& line, OutputIterator output);
};

template <class OutputIterator>
bool
ConfigFileReader::readModules(OutputIterator output)
{
    if (!isOpen()) {
        warnx("Attempting to read from non-open file reader");
        return false;
    }

    currentLineNo = 1;
    inModuleInstall = false;
    inModuleUninstall = false;
    currentModule = nullptr;
    inShell = false;
    currentShellAction = nullptr;

    bool noErrors = true;
    std::string line;
    /* Don't read a line if processing the last line wasn't successful. */
    while (noErrors && getline(reader, line)) {
        noErrors = processLine<OutputIterator>(line, output);
        if (noErrors)
            currentLineNo++;
    }

    if (inShell)
        flushShellAction();

    if (inModuleInstall || inModuleUninstall)
        flushModule(output);

    if (!noErrors) {
        warnx("Failed to read config file %s, line %i: %s", getPath().c_str(),
            currentLineNo, line.c_str());
    }

    return noErrors;
}

template <class OutputIterator>
bool
ConfigFileReader::processLine(const std::string& line, OutputIterator output)
{
    if (isEmptyLine(line))
        return true;

    int expectedIndents = getExpectedIndents();
    if (isComment(line, expectedIndents))
        return true;

    int indents = indentCount(line);

    if (inShell) {
        if (indents >= 2) {
            addShellAction(line);
            return true;
        } else
            flushShellAction();
    }

    if (inModuleInstall || inModuleUninstall) {
        if (indents == 1)
            return processLineAsCommand(line);
        else if (indents > 1) {
            warnx("line %i: Unexpected indentation: %s", currentLineNo,
                line.c_str());
            return false;
        }
    }

    std::string moduleName;
    if (isModuleLine(line, moduleName)) {
        if (inModuleInstall || inModuleUninstall)
            flushModule(output);
        startNewModule(moduleName);
        return true;
    }
    if (isUninstallLine(line)) {
        if (!inModuleInstall) {
            warnx("line %i: Uninstall without named module: %s", currentLineNo,
                line.c_str());
            return false;
        }
        changeToUninstall();
        return true;
    }

    warnx("line %i: Unable to process line: %s", currentLineNo, line.c_str());
    return false;
}

template <class OutputIterator>
void
ConfigFileReader::flushModule(OutputIterator output)
{
    *output = *currentModule;
    delete currentModule;
    inModuleInstall = false;
    inModuleUninstall = false;
    output++;
}
} /* namespace dfm */

#endif /* CONFIG_FILE_READER_H */
