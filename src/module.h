

#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>

#include "modulecomponent.h"

namespace dfm {

const char defaultModuleName[] = "Generic Module";

class Module {
public:
    Module();
    Module(const std::string& name);
    bool install();
    bool uninstall();
    const std::string& getName() const;
    void setName(const std::string& name);

private:
    std::string name;
    std::vector<ModuleComponent*> modules;
};
}

#endif /* MODULE_H */
