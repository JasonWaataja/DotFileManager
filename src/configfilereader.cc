

#include "configfilereader.h"

namespace dfm {

ConfigFileReader::ConfigFileReader(const std::string& path)
    : path(path), reader(path)
{
}

ConfigFileReader::ConfigFileReader(const boost::filesystem::path& path)
    : path(path), reader(path.string())
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
    return reader.is_open();
}
}
