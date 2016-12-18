

#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "module.h"

namespace dfm {

class Command {
public:
    enum ArgumentCheck {
        NO_ARGUMENT_CKECK,
        EXACT_COUNT_ARGUMENT_CHECK,
        MINIMUM_COUNT_ARGUMENT_CHECK
    };

    Command();
    Command(const std::string& name);
    Command(const std::string& name,
        std::function<std::shared_ptr<ModuleAction>(
            const std::vector<std::string>&)>
            createActionFunction);

    void setNoArgumentChecking();
    void setExactAgumentChecking(int argc);
    void setMinimumCountArgumentCheck(int argc);

    ArgumentCheck getArgumentCheckingType() const;
    /*
     * Returns the exact argument count expected if exact checking is enable,
     * the minimum number of arguments if there's a minimum number of
     * arguments, -1 otherwise.
     */
    int getExpectedArgumentCount() const;

    std::shared_ptr<ModuleAction> createAction(
        const std::vector<std::string>& arguments);
    std::shared_ptr<ModuleAction> createAction(int argc, const std::string[]);

    /*
     * Returns whether or not the given name matches any one of the names in
     * callableNames.
     */
    bool matchesName(const std::string& name) const;

    const std::vector<std::string>& getCallableNames() const;
    void addCallableName(const std::string& name);
    void setCallableNames(const std::vector<std::string>& names);

    std::function<std::shared_ptr<ModuleAction>(
        const std::vector<std::string>&)>
    getCreateActionFunction() const;
    void setCreateActionFunction(std::function<std::shared_ptr<ModuleAction>(
            const std::vector<std::string>&)>
            createActionFunction);

    static std::function<std::shared_ptr<ModuleAction>(
        const std::vector<std::string>&)>
    getDefaultAction();

    /*
     * Checks whether or not the given argument list has exactly argc
     * arguments. Issues a warning if that is not the case.
     *
     * Returns whether or not the size of arguments is argc.
     */
    static bool checkArgumentCountEqual(
        const std::vector<std::string>& arguments, int argc);
    /*
     * Checks whether or not the given argument list has argc or greater
     * arguments.
     *
     * Returns whether or not the size of arguments is greater than or equal to
     * argc.
     */
    static bool checkArgumentCountAtLeast(
        const std::vector<std::string>& arguments, int argc);

private:
    std::vector<std::string> callableNames;
    std::function<std::shared_ptr<ModuleAction>(
        const std::vector<std::string>&)>
        createActionFunction;
    ArgumentCheck argumentCheckingType;
    int expectedArgumentCount;
};
}

#endif /* COMMAND_H */
