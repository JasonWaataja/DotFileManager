/*
 j Copyright (c) 2016 Jason Waataja
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

#ifndef DOT_FILE_MANAGER_H
#define DOT_FILE_MANAGER_H

#include "config.h"

#include <memory>

#include "module.h"
#include "options.h"

namespace dfm {

class DotFileManager {
public:
    DotFileManager(int argc, char** argv);
    int run();

private:
    int argc;
    char** argv;

    std::shared_ptr<DfmOptions> options;
    std::vector<Module> modules;

    bool initializeOptions();
    bool readModules();
    bool performOperation();
    bool operateOn(const Module& module);

    /*
     * Determines the correct stream to write to based on the flags in options
     * and writes a config file to it.
     *
     * Returns true on success, false on failure.
     */
    bool createConfigFile() const;
    /*
     * Print modules to standard output. Assumes the config file has already
     * been read
     */
    void printModules() const;

private:
    /*
     * Generates a config file for the given directory and writes it to the
     * given output stream.
     *
     * Returns true on success, false on failure.
     */
    bool writeConfigFile(const std::string& directory, std::ostream&
        outputStream) const;
};
} /* namespace 2016 */

#endif /* DOT_FILE_MANAGER_H */
