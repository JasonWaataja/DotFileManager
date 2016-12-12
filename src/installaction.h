

#ifndef INSTALL_ACTION_H
#define INSTALL_ACTION_H

#include <boost/filesystem.hpp>

#include "moduleaction.h"

namespace dfm {

class InstallAction : public ModuleAction {
public:
    InstallAction(const std::string& filename,
        const boost::filesystem::path& sourceDirectory,
        const boost::filesystem::path& destinationDirectory);
    InstallAction(const std::string& filename,
        const boost::filesystem::path& sourceDirectory,
        const std::string& installFilename,
        const boost::filesystem::path& destinationDirectory);
    bool performAction() override;
    const boost::filesystem::path getFilePath() const;
    const boost::filesystem::path getInstallationPath() const;
    const std::string& getFilename() const;
    void setFilename(const std::string& filename);
    const boost::filesystem::path& getSourceDirectory() const;
    void setSourceDirectory(const boost::filesystem::path& sourceDirectory);
    const boost::filesystem::path& getDestinationDirectory() const;
    void setDestinationDirectory(
        const boost::filesystem::path& destinationDirectory);
    const std::string& getInstallFilename() const;
    void setInstallFilename(const std::string& installFilename);

private:
    std::string filename;
    boost::filesystem::path sourceDirectory;
    std::string installFilename;
    boost::filesystem::path destinationDirectory;
};
}

#endif /* INSTALL_ACTION_H */
