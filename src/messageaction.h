

#ifndef MESSAGE_ACTION_H
#define MESSAGE_ACTION_H

#include <string>

#include "moduleaction.h"

namespace dfm {

class MessageAction : public ModuleAction {
public:
    MessageAction(const std::string& message);
    bool performAction() override;
    const std::string& getMessage() const;
    void setMessage(const std::string& message);

private:
    std::string message;
};
}

#endif /* MESSAGE_ACTION_H */
