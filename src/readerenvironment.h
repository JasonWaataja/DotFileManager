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


#ifndef READER_ENVIRONMENT_H
#define READER_ENVIRONMENT_H

#include <memory>

#include "options.h"
#include "filecheck.h"

namespace dfm {

class ReaderEnvironment {
public:
    ReaderEnvironment();
    ReaderEnvironment(std::shared_ptr<DfmOptions> options);

    std::shared_ptr<DfmOptions> getOptions() const;
    void setOptions(std::shared_ptr<DfmOptions> options);

    const std::string& getDirectory() const;
    void setDirectory(const std::string& directory);

    void addFileCheck(const FileCheck& check);
    const std::vector<FileCheck>& getFileChecks() const;
    void clearFileChecks();

private:
    std::shared_ptr<DfmOptions> options;
    std::string directory;
    std::vector<FileCheck> checks;
};
} /* namespace 2016 */

#endif /* READER_ENVIRONMENT_H */
