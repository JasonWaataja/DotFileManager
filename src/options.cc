

#include "options.h"

#include <err.h>
#include <unistd.h>

namespace dfm {

DfmOptions::DfmOptions()
    : installModulesFlag(false),
      uninstallModulesFlag(false),
      allFlag(false),
      remainingArguments(),
      hasSourceDirectory(false),
      sourceDirectory()
{
}

bool
DfmOptions::loadFromArguments(int argc, char* argv[])
{
    installModulesFlag = false;
    uninstallModulesFlag = false;
    allFlag = false;
    remainingArguments.clear();
    hasSourceDirectory = false;
    sourceDirectory = boost::filesystem::path();

    /* Print errors when getopt encounters an error. */
    opterr = 1;

    int getoptResult = getopt(argc, argv, GETOPT_OPTIONS);
    while (getoptResult != -1) {
        switch (getoptResult) {
        case 'i':
            installModulesFlag = true;
            break;
        case 'u':
            uninstallModulesFlag = true;
            break;
        case 'a':
            allFlag = true;
            break;
        case 'd':
            hasSourceDirectory = true;
            sourceDirectory = boost::filesystem::path(optarg);
        case ':':
            if (isprint(optopt))
                warnx("Missing required argument for \"-%c\".", optopt);
            else
                warnx("Missing required argument.");
            return false;
        case '?':
            warnx("Unknown argument \"-%c\".", optopt);
            return false;
        }
        getoptResult = getopt(argc, argv, GETOPT_OPTIONS);
    }

    for (int i = optind; i < argc; i++)
        remainingArguments.push_back(std::string(argv[i]));

    return true;
}

bool
DfmOptions::verifyArguments() const
{
    if (!verifyFlagsConsistency())
        return false;
    if (!verifyFlagsHaveArguments())
        return false;
    if (!verifyDirectoryExists())
        return false;
    return true;
}

bool
DfmOptions::verifyFlagsConsistency() const
{
    if (installModulesFlag && uninstallModulesFlag) {
        warnx("Both install and uninstall flags passed.");
        return false;
    }
    if (allFlag && !installModulesFlag && !uninstallModulesFlag) {
        warnx(
            "The \"-a\" flag cannot be used without the \"-i\" or \"-u\" flags.");
        return false;
    }
    return true;
}

bool
DfmOptions::verifyFlagsHaveArguments() const
{
    if ((installModulesFlag || uninstallModulesFlag) && !allFlag) {
        warnx("Must provide modules to install or uninstall.");
        return false;
    }
    return true;
}

bool
DfmOptions::verifyDirectoryExists() const
{
    if (hasSourceDirectory) {
        if (!boost::filesystem::exists(sourceDirectory)) {
            warnx("Directory doesn't exists: %s.", sourceDirectory.c_str());
            return false;
        }
        if (!boost::filesystem::is_directory(sourceDirectory)) {
            warnx("Given directory isn't a directory: %s",
                sourceDirectory.c_str());
            return false;
        }
    }
    return true;
}
}
