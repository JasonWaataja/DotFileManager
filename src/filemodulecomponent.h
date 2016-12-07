

#ifndef INSTALL_COMPONENT_H
#define INSTALL_COMPONENT_H

#include <boost/filesystem.hpp>

#include "modulecomponent.h"

namespace dfm {

class FileModuleComponent : public ModuleComponent {
public:
    FileModuleComponent();
    FileModuleComponent(const std::string& filename,
        const boost::filesystem::path& sourceDirectory,
        boost::filesystem::path& destinationDirectory);
    bool install();
    bool uninstall();
    const boost::filesystem::path getFilePath() const;
    const boost::filesystem::path getInstallationPath() const;
    const std::string& getFilename() const;
    void setFilename(const std::string& filename);
    const boost::filesystem::path& getSourceDirectory() const;
    void setSourceDirectory(const boost::filesystem::path& sourceDirectory);
    const boost::filesystem::path& getDestinationDirectory() const;
    void setDestinationDirectory(
        const boost::filesystem::path& destinationDirectory);
    bool getRemoveOnUninstall() const;
    void setRemoveOnUninstall(bool removeOnUninstall);

private:
    std::string filename;
    boost::filesystem::path sourceDirectory;
    boost::filesystem::path destinationDirectory;
    bool removeOnUninstall;
};
}

#endif /*INSTALL_COMPONENT_H */
