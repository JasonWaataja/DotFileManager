

#ifndef MODULE_H
#define MODULE_H

#include <memory>
#include <string>
#include <vector>

#include "moduleaction.h"

namespace dfm {

const char defaultModuleName[] = "Generic Module";

class Module {
public:
    Module();
    Module(const std::string& name);
    void addInstallAction(std::shared_ptr<ModuleAction> action);
    void addUninstallAction(std::shared_ptr<ModuleAction> action);
    bool install();
    bool uninstall();
    const std::string& getName() const;
    void setName(const std::string& name);

private:
    std::string name;
    std::vector<std::shared_ptr<ModuleAction>> installActions;
    std::vector<std::shared_ptr<ModuleAction>> uninstallActions;
};
}

#endif /* MODULE_H */
