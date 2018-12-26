/*
 * Copyright (c) 2017 Jason Waataja
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


#ifndef TERMINAL_WINDOW_H
#define TERMINAL_WINDOW_H

#include "config.hh"

#include "abstractwindow.hh"

namespace dfm {

/*
 * TerminalWindow is an implementation of AbstractWindow that does nothing
 * except message to the terminal output. The command line version is not
 * expected to edit anything.
 */
class TerminalWindow : public AbstractWindow {
public:
    void message(const std::string& message, MessageType type) override;
    virtual void editMessage(MessageAction& action) override;
    virtual void editDependency(DependencyAction& action) override;
    virtual void editFileCheck(FileCheckAction& action) override;
    virtual void editInstall(InstallAction& action) override;
    virtual void editRemove(RemoveAction& action) override;
    virtual void editShell(ShellAction& action) override;
    virtual void editModuleFile(ModuleFile& moduleFile) override;
};
} /* namespace dfm */

#endif /* TERMINAL_WINDOW_H */
