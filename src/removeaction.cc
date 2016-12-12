

#include "removeaction.h"

#include <err.h>

namespace dfm {

RemoveAction::RemoveAction(const boost::filesystem::path& filePath)
    : ModuleAction(DEFAULT_REMOVE_ACTION_NAME), filePath(filePath)
{
}

RemoveAction::RemoveAction(
    const std::string& filename, const boost::filesystem::path& directory)
    : ModuleAction(DEFAULT_REMOVE_ACTION_NAME), filePath(directory / filename)
{
}

const boost::filesystem::path&
RemoveAction::getFilePath() const
{
    return filePath;
}

void
RemoveAction::setFilePath(const boost::filesystem::path& filePath)
{
    this->filePath = filePath;
}

bool
RemoveAction::performAction()
{
    try {
        if (boost::filesystem::exists(filePath)) {
            boost::filesystem::remove_all(filePath);
        }
    } catch (boost::filesystem::filesystem_error& e) {
        warnx("%s", e.what());
        return false;
    }

    return true;
}
}
