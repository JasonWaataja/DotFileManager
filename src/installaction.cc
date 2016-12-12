

#include "installaction.h"

#include <err.h>

namespace dfm {

InstallAction::InstallAction(const std::string& filename,
    const boost::filesystem::path& sourceDirectory,
    const boost::filesystem::path& destinationDirectory)
    : ModuleAction(filename),
      filename(filename),
      sourceDirectory(sourceDirectory),
      installFilename(filename),
      destinationDirectory(destinationDirectory)
{
}

InstallAction::InstallAction(const std::string& filename,
    const boost::filesystem::path& sourceDirectory,
    const std::string& installFilename,
    const boost::filesystem::path& destinationDirectory)
    : ModuleAction(filename),
      filename(filename),
      sourceDirectory(sourceDirectory),
      installFilename(installFilename),
      destinationDirectory(destinationDirectory)
{
}

const boost::filesystem::path
InstallAction::getFilePath() const
{
    return sourceDirectory / filename;
}

const boost::filesystem::path
InstallAction::getInstallationPath() const
{
    return destinationDirectory / installFilename;
}

const std::string&
InstallAction::getFilename() const
{
    return filename;
}

void
InstallAction::setFilename(const std::string& filename)
{
    this->filename = filename;
}

const boost::filesystem::path&
InstallAction::getSourceDirectory() const
{
    return sourceDirectory;
}

void
InstallAction::setSourceDirectory(
    const boost::filesystem::path& sourceDirectory)
{
    this->sourceDirectory = sourceDirectory;
}

const boost::filesystem::path&
InstallAction::getDestinationDirectory() const
{
    return destinationDirectory;
}

void
InstallAction::setDestinationDirectory(
    const boost::filesystem::path& destinationDirectory)
{
    this->destinationDirectory = destinationDirectory;
}

const std::string&
InstallAction::getInstallFilename() const
{
    return installFilename;
}

void
InstallAction::setInstallFilename(const std::string& installFilename)
{
    this->installFilename = installFilename;
}

bool
InstallAction::performAction()
{
    boost::filesystem::path sourcePath = getFilePath();
    boost::filesystem::path destinationPath = getInstallationPath();

    try {
        if (!boost::filesystem::exists(sourcePath)) {
            warnx("#ile %s doesn't exist.", sourcePath.c_str());
            return false;
        }

        if (!boost::filesystem::exists(destinationDirectory)) {
            bool status =
                boost::filesystem::create_directories(destinationDirectory);
            if (!status) {
                warnx("Failed to create directory %s.",
                    destinationDirectory.c_str());
                return false;
            }
        }

        if (!boost::filesystem::is_directory(destinationDirectory)) {
            warnx("File%s isn't a directory.", destinationDirectory.c_str());
            return false;
        }

        boost::filesystem::copy(sourcePath, destinationPath);
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("%s", e.what());
        return false;
    }

    return true;
}
}
