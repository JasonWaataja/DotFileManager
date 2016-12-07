

#ifndef OPTIONS_H
#define OPTIONS_H

#include <boost/filesystem.hpp>
#include <string>

namespace dfm {

const char getoptOptions[] = "i:u:";

class DfmOptions {
public:
    DfmOptions();
    DfmOptions(int argc, char* argv[]);
    bool loadFromArguments(int argc, char* argv[]);
    bool getInstallFlag() const;
    const std::string& getInstallModuleName() const;
    bool getUninstallFlag() const;
    const std::string& getUninstallModuleName() const;
    /*
     * Returns the path represented by the final argument to the program. This
     * is either a path representing a config file or a directory that contains
     * one.
     */
    const std::string& getInitializationPath() const;
    /*
     * Returns a path for the given initialization path. If none war provided,
     * then the default is the current directory.
     */
    bool getConfigFile(const boost::filesystem::path& pathToSet);

private:
    bool installModuleFlag;
    std::string installModuleName;
    bool uninstallModuleFlag;
    std::string uninstallModuleName;
    std::string initializtionPath;
};
}

#endif /* OPTIONS_H */
