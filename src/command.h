

#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <string>
#include <vector>

namespace dfm {

class Command {
public:
    Command();
    Command(const std::string& name);
    /* Constructor that copies argv into arguments. */
    Command(const std::string& name, int argc, const std::string argv[]);
    Command(
        const std::string& name, const std::vector<std::string>& arguments);
    /*
     * Executes the function with the arguments.
     *
     * Returns whether or not it succeeded.
     */
    bool execute();
    /*
     * Returns whether or not the given name matches any one of the names in
     * callableNames.
     */
    bool matchesName(const std::string& name) const;

    const std::vector<std::string>& getCallableNames() const;
    void addCallableName(const std::string& name);
    void setCallableNames(const std::vector<std::string>& names);

    const std::vector<std::string>& getArguments() const;
    void setArguments(int argc, const std::string argv[]);
    void setArguments(const std::vector<std::string>& arguments);

    std::function<bool(const std::vector<std::string>&)> getAction() const;
    void setAction(std::function<bool(const std::vector<std::string>&)>);


    static std::function<bool(const std::vector<std::string>)>
    getDefaultAction();

private:
    std::vector<std::string> callableNames;
    std::vector<std::string> arguments;
    std::function<bool(const std::vector<std::string>&)> action;
};
}

#endif /* COMMAND_H */
