

#ifndef MODULE_ACTION_H
#define MODULE_ACTION_H

#include <string>

namespace dfm {

const char DEFAULT_ACTION_NAME[] = "generic action";

class ModuleAction {
public:
    ModuleAction();
    ModuleAction(const std::string& name);
    virtual bool performAction() = 0;
    const std::string& getName() const;
    void setName(const std::string& name);

private:
    std::string name;
};
}

#endif /* MODULE_ACTION_H */
