/*
 * Copyright (c) 2016 Jason Waataja
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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
            warnx("File %s doesn't exist.", sourcePath.c_str());
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

        /*
         * Overwrite if file exists. Copy uses the correct defaults otherwise.
         */
        if (boost::filesystem::is_regular_file(sourcePath)) {
            boost::filesystem::copy_file(sourcePath, destinationPath,
                boost::filesystem::copy_option::overwrite_if_exists);
        } else if (boost::filesystem::is_directory(sourcePath)) {
            if (boost::filesystem::exists(destinationPath)) {
                if (boost::filesystem::is_directory(destinationDirectory)) {
                }
            }
        } else {
            boost::filesystem::copy(sourcePath, destinationPath);
        }
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("%s", e.what());
        return false;
    }

    return true;
}

bool
InstallAction::copyRegularFile(const boost::filesystem::path& sourceFilePath,
    const boost::filesystem::path& destinationPath)
{
    try {
        if (!boost::filesystem::exists(sourceFilePath)) {
            warnx("File %s doesn't exist.", sourceFilePath.c_str());
            return false;
        }

        if (!boost::filesystem::is_regular_file(sourceFilePath)) {
            warnx("Attempting to copy non regular file with regular file "
                  "function %s.",
                sourceFilePath.c_str());
            return false;
        }

        if (!boost::filesystem::exists(destinationDirectory.parent_path()))
            boost::filesystem::create_directories(destinationPath);
        else if (!boost::filesystem::is_regular_file(destinationDirectory)) {
            warnx("Copying regular file over directory %s, overwriting.",
                destinationPath.c_str());
            boost::filesystem::remove_all(destinationPath);
        }

        boost::filesystem::copy_file(sourceFilePath, destinationPath);
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("Filesystem error.");
        return false;
    }

    return true;
}

bool
InstallAction::copyDirectory(
    const boost::filesystem::path& sourceDirectoryPath,
    const boost::filesystem::path& destinationPath)
{
    try {
        if (!boost::filesystem::exists(sourceDirectoryPath)) {
            warnx("File %s doesn't exist.");
            return false;
        }

        if (!boost::filesystem::is_directory(sourceDirectoryPath)) {
            warnx("Attempting to copy non-directory with with directory "
                  "function %s.", sourceDirectoryPath.c_str());
            return false;
        }

        if (!boost::filesystem::exists(destinationPath)) {
        }
    }
}
} /* namespace dfm */
