

#ifndef MODULE_COMPONENT_H
#define MODULE_COMPONENT_H

#include <string>

namespace dfm {

class ModuleComponent {
public:
    ModuleComponent();
    ModuleComponent(const std::string& name);
    virtual bool install() = 0;
    virtual bool uninstall() = 0;
    const std::string& getName() const;
    void setName(const std::string& name);

private:
    std::string name;
};
}

#endif /* MODULE_COMPONENT_H */
