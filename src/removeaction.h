

#ifndef REMOVE_ACTION_H
#define REMOVE_ACTION_H

#include <boost/filesystem.hpp>

#include "moduleaction.h"

namespace dfm {

const char DEFAULT_REMOVE_ACTION_NAME[] = "remove action";

class RemoveAction : public ModuleAction {
public:
    RemoveAction(const std::string& filePath);
    RemoveAction(const std::string& filename, const std::string& directory);
    const std::string& getFilePath() const;
    void setFilePath(const std::string& filePath);
    void setFilePath(
        const std::string& filename, const std::string& directory);
    bool performAction() override;

private:
    std::string filePath;
};
}

#endif /* REMOVE_ACTION_H */
