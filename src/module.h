

#ifndef MODULE_H
#define MODULE_H

#include <memory>
#include <string>
#include <vector>

#include "modulecomponent.h"

namespace dfm {

const char defaultModuleName[] = "Generic Module";

class Module {
public:
    Module();
    Module(const std::string& name);
    void addComponent(std::shared_ptr<ModuleComponent> component);
    bool install();
    bool uninstall();
    const std::string& getName() const;
    void setName(const std::string& name);

private:
    std::string name;
    std::vector<std::shared_ptr<ModuleComponent>> modules;
};
}

#endif /* MODULE_H */
