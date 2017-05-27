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

#ifndef ABSTRACT_WINDOW_H
#define ABSTRACT_WINDOW_H

#include "config.h"

#include "dependencyaction.h"
#include "filecheckaction.h"
#include "installaction.h"
#include "messageaction.h"
#include "modulefile.h"
#include "removeaction.h"
#include "shellaction.h"

namespace dfm {

/*
 * AbstractWindow represents a UI object that can provide messages, edit
 * modules, etc.
 *
 * It would have more sense to name this AbstractUI, however the use of
 * "window" implies that there can be multiple windows within the same
 * application, as is the case with GTK.
 */
class AbstractWindow {
public:
    enum MessageType { MESSAGE_INFO, MESSAGE_WARNING, MESSAGE_ERROR };
    virtual void message(const char* message, MessageType type) = 0;
    virtual void editMessage(MessageAction& action);
    virtual void editDependency(DependencyAction& action) = 0;
    virtual void editFileCheck(FileCheckAction& action) = 0;
    virtual void editInstall(InstallAction& action) = 0;
    virtual void editRemove(RemoveAction& action) = 0;
    virtual void editShell(ShellAction& action) = 0;
    virtual void editModuleFile(ModuleFile& moduleFile) = 0;
};
} /* namespace dfm */

#endif /* ABSTRACT_WINDOW_H */
