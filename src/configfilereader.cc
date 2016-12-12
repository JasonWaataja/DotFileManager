

#include "configfilereader.h"

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
ConfigFileReader::isEmptyLine(const std::string& line)
{
    return line.length() == 0;
}

bool
ConfigFileReader::isComment(const std::string& line, int indents)
{
    if (isEmptyLine(line))
        return false;

    int currentIndex = 0;
    for (; currentIndex < indents && currentIndex < line.length();
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
}
