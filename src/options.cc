

#include "options.h"

#include <err.h>
#include <unistd.h>

namespace dfm {

DfmOptions::DfmOptions()
    : valid(false),
      installModule(false),
      installModuleName(),
      uninstallModule(false),
      uninstallModuleName(),
      sourceDirectory(),
      configFileName()
{
}

bool
DfmOptions::loadFromArguments(int argc, char* argv[])
{
    installModule = false;
    uninstallModule = false;
    valid = true;

    opterr = 1;

    int getoptValue = getopt(argc, argv, getoptOptions);

    while (getoptValue != -1) {
        switch (getoptValue) {
        case 'i':
            installModule = true;
            installModuleName = std::string(optarg);
            break;
        case 'u':
            uninstallModule = true;
            uninstallModuleName = std::string(optarg);
            break;
        case ':':
            warnx("missing argument for %c", optopt);
            valid = false;
            return false;
        case '?':
            warnx("unknwon argument %c", optopt);
            valid = false;
            return false;
        }
        getoptValue = getopt(argc, argv, getoptOptions);
    }

    configFile = false;

    if (optind >= argc)
        setToCurrentDirectory();
    else {
        boost::filesystem::path pathArgument(argv[optind]);
        boost::filesystem::path asAbsolute =
            boost::filesystem::absolute(pathArgument);
        if (boost::filesystem::is_directory(pathArgument))
            sourceDirectory = asAbsolute;
        else {
            sourceDirectory = asAbsolute.parent_path();
            configFileName = asAbsolute.filename().string();
            configFile = true;
        }
    }

    return true;
}

void
DfmOptions::setToCurrentDirectory()
{
    sourceDirectory = boost::filesystem::current_path();
}

bool
DfmOptions::isValid() const
{
    return valid;
}

bool
DfmOptions::hasInstallModule() const
{
    return installModule;
}

const std::string&
DfmOptions::getInstallModuleName() const
{
    return installModuleName;
}

bool
DfmOptions::hasUninstallModule() const
{
    return uninstallModule;
}

const std::string&
DfmOptions::getUninstallModuleName() const
{
    return uninstallModuleName;
}

const boost::filesystem::path&
DfmOptions::getSourceDirectory() const
{
    return sourceDirectory;
}

bool
DfmOptions::hasConfigFile() const
{
    return configFile;
}

const std::string&
DfmOptions::getConfigFileName() const
{
    return configFileName;
}

boost::filesystem::path
DfmOptions::getConfigFilePath() const
{
    return sourceDirectory / configFileName;
}
}
