

#ifndef OPTIONS_H
#define OPTIONS_H

#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace dfm {

/* Inital colon gets getopt to return ":" on missing required argument.  */
const char GETOPT_OPTIONS[] = ":iuad:";

class DfmOptions {
public:
    DfmOptions();

    bool loadFromArguments(int argc, char* argv[]);
    bool verifyArguments() const;

    bool installModulesFlag;
    bool uninstallModulesFlag;
    bool allFlag;
    std::vector<std::string> remainingArguments;
    bool hasSourceDirectory;
    boost::filesystem::path sourceDirectory;

private:
    bool verifyFlagsConsistency() const;
    bool verifyFlagsHaveArguments() const;
    bool verifyDirectoryExists() const;
};
}

#endif /* OPTIONS_H */
