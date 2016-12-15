

#ifndef CONFIG_FILE_READER_H
#define CONFIG_FILE_READER_H

#include <boost/filesystem.hpp>
#include <err.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
    /* TODO: Write destructor that closes file. */
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

private:
    boost::filesystem::path path;
    std::ifstream reader;

    bool isEmptyLine(const std::string& line) const;
    bool isComment(const std::string& line, int expectedIndents) const;
    int indentCount(const std::string& line) const;
    int getExpectedIndents() const;
    std::string stripIndents(const std::string& line, int indents);
    /*
     * Tests to see if the line starts a module and sets moduleName to the name
     * if this is so. These don't check if the line is blank or a comment and
     * process as is because it is assumes the checking was already done.
     */
    bool isModuleLine(const std::string& line, std::string& moduleName);
    bool isModuleLine(const std::string& line);
    bool isUninstallLine(const std::string& line);
    bool isShellLine(const std::string& line);

    /* Processing commands that affect obect state. */
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
    template <class OutputIterator> void flushModule(OutputIterator output);
    /* These two assume the module has already been flushed. */
    void startNewModule(const std::string& name);
    void changeToUninstall();

    bool parseCommandLine(const std::string& line, int indents,
        std::string& command, std::vector<std::string>& arguments);

    bool inModuleInstall;
    bool inModuleUninstall;
    Module* currentModule;
    bool inShell;
    ShellAction* currentShellAction;
    int currentLineNo;

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
        std::cout << "end of loop" << std::endl;
    }

    std::cout << "About to flush shell action" << std::endl;
    if (inShell)
        flushShellAction();

    std::cout << "about to flush module" << std::endl;
    if (inModuleInstall || inModuleUninstall)
        flushModule(output);

    if (!noErrors) {
        warnx("Failed to read config file %s, line %i: %s", getPath().c_str(),
            currentLineNo, line.c_str());
    }

    std::cout << "About to return from function" << std::endl;

    return noErrors;
}

template <class OutputIterator>
bool
ConfigFileReader::processLine(const std::string& line, OutputIterator output)
{
    std::cout << "Processing line: " << line << std::endl;
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
        std::cout << "Processed line" << std::endl;
        if (inModuleInstall || inModuleUninstall)
            flushModule(output);
        startNewModule(moduleName);
        return true;
    }
    std::cout << "Processed line" << std::endl;
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
    std::cout << "Flushing current module" << std::endl;
    std::cout << "About to set output" << std::endl;
    *output = *currentModule;
    std::cout << "About to delete current module" << std::endl;
    delete currentModule;
    inModuleInstall = false;
    inModuleUninstall = false;
    std::cout << "About to increment module" << std::endl;
    output++;
}
}

#endif /* CONFIG_FILE_READER_H */
