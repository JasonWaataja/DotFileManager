

#ifndef OPTIONS_H
#define OPTIONS_H

#include <boost/filesystem.hpp>
#include <string>

namespace dfm {

/* Inital colon gets getopt to return ":" on missing required argument.  */
const char getoptOptions[] = ":i:u:";

class DfmOptions {
public:
    DfmOptions();
    bool loadFromArguments(int argc, char* argv[]);
    void setToCurrentDirectory();
    bool isValid() const;
    bool hasInstallModule() const;
    const std::string& getInstallModuleName() const;
    bool hasUninstallModule() const;
    const std::string& getUninstallModuleName() const;
    const boost::filesystem::path& getSourceDirectory() const;
    bool hasConfigFile() const;
    const std::string& getConfigFileName() const;
    boost::filesystem::path getConfigFilePath() const;

private:
    bool valid;
    bool installModule;
    std::string installModuleName;
    bool uninstallModule;
    std::string uninstallModuleName;
    boost::filesystem::path sourceDirectory;
    bool configFile;
    std::string configFileName;
};
}

#endif /* OPTIONS_H */
