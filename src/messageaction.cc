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

#include "config.hh"

#include "messageaction.hh"

#include <iostream>
#include <sstream>

#include "abstractwindow.hh"

namespace dfm {

MessageAction::MessageAction()
{
}

MessageAction::MessageAction(const std::string& message) : message{message}
{
    updateName();
}

bool
MessageAction::performAction()
{
    getWindow()->message(message, AbstractWindow::MESSAGE_INFO);
    return true;
}

void
MessageAction::graphicalEdit()
{
    getWindow()->editMessage(*this);
}

const std::string&
MessageAction::getMessage() const
{
    return message;
}

void
MessageAction::setMessage(const std::string& message)
{
    this->message = message;
}

void
MessageAction::updateName()
{
    setName("Message");
}

std::vector<std::string>
MessageAction::createConfigLines() const
{
    std::ostringstream outputStream;
    for (std::string::size_type i = 0; i < message.length(); i++) {
        if (message[i] == '"')
            outputStream << "\\\"";
        else
            outputStream << message[i];
    }
    std::string line = "message \"" + outputStream.str() + "\"";
    return std::vector<std::string>{line};
}
} /* namespace dfm */
