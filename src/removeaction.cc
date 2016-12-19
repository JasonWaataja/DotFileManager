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

#include "removeaction.h"

#include <err.h>

namespace dfm {

RemoveAction::RemoveAction(const boost::filesystem::path& filePath)
    : ModuleAction(DEFAULT_REMOVE_ACTION_NAME), filePath(filePath)
{
}

RemoveAction::RemoveAction(
    const std::string& filename, const boost::filesystem::path& directory)
    : ModuleAction(DEFAULT_REMOVE_ACTION_NAME), filePath(directory / filename)
{
}

const boost::filesystem::path&
RemoveAction::getFilePath() const
{
    return filePath;
}

void
RemoveAction::setFilePath(const boost::filesystem::path& filePath)
{
    this->filePath = filePath;
}

bool
RemoveAction::performAction()
{
    try {
        if (boost::filesystem::exists(filePath)) {
            boost::filesystem::remove_all(filePath);
        }
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("%s", e.what());
        return false;
    }

    return true;
}
} /* namespace dfm */
