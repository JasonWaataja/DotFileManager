

#include "configfilereader.h"

#include <ctype.h>

#include <exception>
#include <regex>

namespace dfm {

ConfigFileReader::ConfigFileReader(const std::string& path)
    : path(path),
      reader(path),
      inModuleInstall(false),
      inModuleUninstall(false),
      currentModule(nullptr),
      inShell(false),
      currentShellAction(nullptr),
      currentLineNo(1)
{
}

ConfigFileReader::ConfigFileReader(const boost::filesystem::path& path)
    : ConfigFileReader(path.string())
{
}

ConfigFileReader::ConfigFileReader(const char* path)
    : ConfigFileReader(std::string(path))
{
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
    std::cout << "Module reg" << std::endl << std::endl;
    try {
        std::regex re("^(\\S+(?:\\s+\\S+)*)\\s*:\\s*$");
        std::cout << "Compiled" << std::endl;
        std::cout << "line: " << line << std::endl;
        std::smatch match;
        std::cout << "About to match" << std::endl;
        if (std::regex_match(line, match, re)) {
            std::cout << "was matched" << std::endl;
            moduleName = match.str(1);
            std::cout << "Looking for match" << std::endl;
            return true;
        }
        std::cout << "No Match" << std::endl;
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
    std::cout << "Module reg no name" << std::endl;
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
    std::cout << "Uninstall reg" << std::endl;
    std::regex re("^uninstall\\s*:\\s$");
    if (std::regex_match(line, re)) {
        return true;
    }
    return false;
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
    if (!std::regex_match(line, matchResults, commandRe)) {
        warnx("line %i: No command found for line: %s", currentLineNo,
            line.c_str());
        return false;
    }

    std::string command = matchResults.str(1);
    localLine =
        localLine.substr(matchResults.position(1), matchResults.length(1));
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
    }

    return true;
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
        warnx("Attempting to close reader while still readering.");

    reader.close();
}
}
