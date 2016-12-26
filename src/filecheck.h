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


#ifndef FILE_CHECK_H
#define FILE_CHECK_H

#include <string>
#include <vector>

namespace dfm {

class FileCheck {
public:
    FileCheck();
    FileCheck(
        const std::string& sourcePath, const std::string& destinationPath);

    const std::string& getSourcePath() const;
    void setSourcePath(const std::string& sourcePath);
    const std::string& getDestinationPath() const;
    void setDestinationPath(const std::string& destinationPath);

    void setFiles(
        const std::string& sourcePath, const std::string& destinationPath);


    bool shouldUpdate() const;
    bool updateFile();

    /* std::vector<std::string> getUpdatableFiles(); */

    /*
     * Checks whether the two paths are equal according to some rules. Two
     * files are equal if they have the same bytes. Two directories are equal
     * if they contain the same file names and each file has the same contents,
     * and not directory has extra files. If either of the paths aren't regular
     * files or directories, then the function is false regardless if they are
     * the same, such as if they are the same symlink. If one of the two paths
     * is an empty string, then function is false unless they are both empty
     * strings.
     *
     * Returns whether the contents of the two paths are equal.
     */
    static bool regularPathsEqual(
        const std::string& path1, const std::string& path2);

private:
    bool hasFiles() const;

    std::string sourcePath;
    std::string destinationPath;
};
} /* namespace 2016 */

#endif /* FILE_CHECK_H */
