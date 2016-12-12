

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
    /* TODO: Write destructor that closes file. */
    const boost::filesystem::path& getPath() const;
    void setPath(const std::string& path);
    void setPath(const boost::filesystem::path& path);

    bool isOpen();
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

    bool isEmptyLine(const std::string& line);
    bool isComment(const std::string& line, int indents);
    int indentCount(const std::string& line);

    bool inModuleInstall;
    bool inModuleUninstall;
    Module* currentModule;
    bool inShell;
    ShellAction currentShellAction;
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

    std::string line;
    while (getline(reader, line)) {
        processLine<OutputIterator>(line, output);
    }

    return true;
}

template <class OutputIterator>
bool
ConfigFileReader::processLine(const std::string& line, OutputIterator output)
{
    currentLineNo++;
    return true;
}
}

#endif /* CONFIG_FILE_READER_H */
