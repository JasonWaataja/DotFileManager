

#ifndef REMOVE_ACTION_H
#define REMOVE_ACTION_H

#include <boost/filesystem.hpp>

#include "moduleaction.h"

namespace dfm {

const char DEFAULT_REMOVE_ACTION_NAME[] = "remove action";

class RemoveAction : public ModuleAction {
public:
    RemoveAction(const boost::filesystem::path& filePath);
    RemoveAction(
        const std::string& filename, const boost::filesystem::path& directory);
    const boost::filesystem::path& getFilePath() const;
    void setFilePath(const boost::filesystem::path& filePath);
    bool performAction() override;

private:
    boost::filesystem::path filePath;
};
}

#endif /* REMOVE_ACTION_H */
