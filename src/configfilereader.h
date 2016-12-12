

#ifndef CONFIG_FILE_READER_H
#define CONFIG_FILE_READER_H

#include <boost/filesystem.hpp>
#include <err.h>
#include <iostream>
#include <string>
#include <vector>

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

    template <class OutputIterator>
    bool readModulesFromLines(
        const std::vector<std::string>& lines, OutputIterator output);
};

template <class OutputIterator>
bool
ConfigFileReader::readModules(OutputIterator output)
{
    if (!isOpen()) {
        warnx("Attempting to read from non-open file reader");
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (getline(reader, line)) {
        lines.push_back(line);
    }

    return readModulesFromLines(lines, output);
}

/* TODO: Write this implementation. */
template <class OutputIterator>
bool
ConfigFileReader::readModulesFromLines(
    const std::vector<std::string>& lines, OutputIterator output)
{
    return true;
}
}

#endif /* CONFIG_FILE_READER_H */
