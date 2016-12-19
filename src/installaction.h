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

    bool copyRegularFile(const boost::filesystem::path& sourceFilePath,
        const boost::filesystem::path& destinationPath);
    bool copyDirectory(const boost::filesystem::path& sourceDirectoryPath,
        const boost::filesystem::path& destinationPath);

private:
    std::string filename;
    boost::filesystem::path sourceDirectory;
    std::string installFilename;
    boost::filesystem::path destinationDirectory;
};
} /* namespace dfm */

#endif /* INSTALL_ACTION_H */
