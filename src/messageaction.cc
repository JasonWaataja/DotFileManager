

#include "messageaction.h"

#include <iostream>

namespace dfm {

MessageAction::MessageAction(const std::string& message) : message(message)
{
}

bool
MessageAction::performAction()
{
    std::cout << message << std::endl;
    return true;
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
}
