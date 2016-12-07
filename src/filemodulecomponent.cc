

#include "filemodulecomponent.h"

#include <err.h>
#include <iostream>

namespace dfm {

FileModuleComponent::FileModuleComponent()
{
}

FileModuleComponent::FileModuleComponent(const std::string& filename,
    const boost::filesystem::path& sourceDirectory,
    boost::filesystem::path& destinationDirectory)
    : ModuleComponent(filename), filename(filename),
      sourceDirectory(sourceDirectory),
      destinationDirectory(destinationDirectory)
{
}

const boost::filesystem::path
FileModuleComponent::getFilePath() const
{
    return sourceDirectory / filename;
}

const boost::filesystem::path
FileModuleComponent::getInstallationPath() const
{
    return destinationDirectory / filename;
}

const std::string&
FileModuleComponent::getFilename() const
{
    return filename;
}

void
FileModuleComponent::setFilename(const std::string& filename)
{
    this->filename = filename;
}

const boost::filesystem::path&
FileModuleComponent::getSourceDirectory() const
{
    return sourceDirectory;
}

void
FileModuleComponent::setSourceDirectory(
    const boost::filesystem::path& sourceDirectory)
{
    this->sourceDirectory = sourceDirectory;
}

const boost::filesystem::path&
FileModuleComponent::getDestinationDirectory() const
{
    return destinationDirectory;
}

void
FileModuleComponent::setDestinationDirectory(
    const boost::filesystem::path& destinationDirectory)
{
    this->destinationDirectory = destinationDirectory;
}

bool
FileModuleComponent::getRemoveOnUninstall() const
{
    return removeOnUninstall;
}

void
FileModuleComponent::setRemoveOnUninstall(bool removeOnUninstall)
{
    this->removeOnUninstall = removeOnUninstall;
}

bool
FileModuleComponent::install()
{
    boost::filesystem::path sourcePath = getFilePath();
    boost::filesystem::path destinationPath = getInstallationPath();

    try {
        if (!boost::filesystem::exists(sourcePath)) {
            warnx("file %s doesn't exist", sourcePath.c_str());
            return false;
        }

        if (!boost::filesystem::exists(destinationDirectory)) {
            bool status =
                boost::filesystem::create_directories(destinationDirectory);
            if (!status) {
                warnx("failed to create directory %s",
                    destinationDirectory.c_str());
                return false;
            }
        }

        if (!boost::filesystem::is_directory(destinationDirectory)) {
            warnx("%s isn't a directory", destinationDirectory.c_str());
            return false;
        }

        boost::filesystem::copy(sourcePath, destinationPath);
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("%s", e.what());
        return false;
    }

    return true;
}

bool
FileModuleComponent::uninstall()
{
    if (removeOnUninstall) {
        try {
            bool status = boost::filesystem::remove_all(getInstallationPath());

            if (!status) {
                warnx(
                    "failed to remove file %s", getInstallationPath().c_str());
                return false;
            }
        } catch (boost::filesystem::filesystem_error& e) {
            warnx("%s", e.what());
            return false;
        }
    }

    return true;
}
}
