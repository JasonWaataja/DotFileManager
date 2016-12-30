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

#include <dirent.h>
#include <err.h>

#include <iostream>

#include "util.h"

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

    if (isInteractive()) {
        std::string prompt = "Install " + sourcePath.string() + " to "
            + destinationPath.string() + "?";
        if (!getYesOrNo(prompt))
            return true;
        std::cout << std::endl;
    }
    verboseMessage("Installing %s to %s.\n\n", sourcePath.c_str(),
        destinationPath.c_str());

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

        if (boost::filesystem::is_regular_file(sourcePath))
            return copyRegularFile(sourcePath, destinationPath);
        else if (boost::filesystem::is_directory(sourcePath))
            return copyDirectory(sourcePath, destinationPath);
        else {
            warnx("Skipping non-regular non-directory file %s.",
                sourcePath.c_str());
            /*
             * It could return an error but it shouldn't be an error if there
             * is a symbolic link in one of the directories.
             */
            return true;
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
            warnx(
                "Attempting to copy non regular file with regular file function %s.",
                sourceFilePath.c_str());
            return false;
        }

        /*
         * There is a case that is not accounted for here, which is that the
         * destination path doesn't exist and it tries to create the
         * directories, but one of the parents of destinationPath is not a
         * directory, leading to an exception. It will be caught by the
         * try/catch block in that case.
         */
        if (!boost::filesystem::exists(destinationPath.parent_path()))
            boost::filesystem::create_directories(
                destinationPath.parent_path());
        else if (boost::filesystem::exists(destinationPath)
            && !boost::filesystem::is_regular_file(destinationPath)) {
            warnx("Copying regular file over directory %s, overwriting.",
                destinationPath.c_str());
            boost::filesystem::remove_all(destinationPath);
        }

        boost::filesystem::copy_file(sourceFilePath, destinationPath,
            boost::filesystem::copy_option::overwrite_if_exists);
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("Error copying file %s: %s", sourceFilePath.c_str(), e.what());
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
            warnx("Directory %s doesn't exist.", sourceDirectoryPath.c_str());
            return false;
        }

        if (!boost::filesystem::is_directory(sourceDirectoryPath)) {
            warnx(
                "Attempting to copy non-directory with with directory function %s.",
                sourceDirectoryPath.c_str());
            return false;
        }

        /*
         * There is a case that is not accounted for here, which is that the
         * destination path doesn't exist and it tries to create the
         * directories, but one of the parents of destinationPath is not a
         * directory, leading to an exception. It will be caught by the
         * try/catch block in that case.
         */
        if (!boost::filesystem::exists(destinationPath))
            boost::filesystem::create_directories(destinationPath);
        else if (!boost::filesystem::is_directory(destinationPath)) {
            warnx("Copying directory %s over non-directory %s, removing it.",
                sourceDirectoryPath.c_str(), destinationPath.c_str());
            /* I don't think I need to use remove_all, but just to be safe. */
            boost::filesystem::remove_all(destinationPath);
        }

        copyDirectoryRecursive(sourceDirectoryPath, destinationPath);
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("Error copying directory %s: %s", sourceDirectoryPath.c_str(),
            e.what());
        return false;
    }

    return true;
}

bool
InstallAction::copyDirectoryRecursive(
    const boost::filesystem::path& sourceDirectoryPath,
    const boost::filesystem::path& destinationDirectory)
{
    struct dirent** directoryEntries;
    auto trueFunction = [](const struct dirent*) -> int { return 1; };

    int directoryCount = scandir(sourceDirectoryPath.c_str(),
        &directoryEntries, trueFunction, alphasort);

    if (directoryCount == -1) {
        warn("Failed to list directory %s.", sourceDirectoryPath.c_str());
        return false;
    }

    /*
     * Lol, they say in C that you shouldn't need more than three levels of
     * indentation. I should probably split this into functions or something.
     */
    try {
        for (int i = 0; i < directoryCount; i++) {
            /*
             * Not sure if I should use a continue here, but I think it's
             * pretty readable.
             */
            if (strcmp(directoryEntries[i]->d_name, ".") == 0
                || strcmp(directoryEntries[i]->d_name, "..") == 0)
                continue;
            boost::filesystem::path sourceChildPath =
                sourceDirectoryPath / directoryEntries[i]->d_name;
            boost::filesystem::path destinationChildPath =
                destinationDirectory / directoryEntries[i]->d_name;

            if (boost::filesystem::is_directory(sourceChildPath)) {
                if (!boost::filesystem::exists(destinationChildPath))
                    boost::filesystem::create_directories(
                        destinationChildPath);
                else if (!boost::filesystem::is_directory(
                             destinationChildPath)) {
                    warnx("Copying directory %s over non-directory %s.",
                        sourceChildPath.c_str(), destinationChildPath.c_str());
                    boost::filesystem::remove_all(destinationChildPath);
                }
                copyDirectoryRecursive(sourceChildPath, destinationChildPath);
            } else if (boost::filesystem::is_regular_file(sourceChildPath)) {
                if (boost::filesystem::exists(destinationChildPath)
                    && !boost::filesystem::is_regular_file(
                           destinationChildPath)) {
                    warnx("Copying regular file %s over non-regular file %s.",
                        sourceChildPath.c_str(), destinationChildPath.c_str());
                    boost::filesystem::remove_all(destinationChildPath);
                }
                boost::filesystem::copy_file(sourceChildPath,
                    destinationChildPath,
                    boost::filesystem::copy_option::overwrite_if_exists);
            } else {
                warnx("Skipping non-regular non-directory file %s.",
                    sourceChildPath.c_str());
            }
        }
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("Error processing directory %s: %s", sourceDirectoryPath.c_str(),
            e.what());
        return false;
    }

    /* Nothing threw an exception and there were no errors. */
    return true;
}
} /* namespace dfm */
