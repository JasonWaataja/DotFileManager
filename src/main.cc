

#include <iostream>
#include <stdlib.h>

#include "options.h"

int
main(int argc, char* argv[])
{
    dfm::DfmOptions options;
    bool status = options.loadFromArguments(argc, argv);
    std::cout << "status: " << status << std::endl;
    std::cout << "has install: " << options.hasInstallModule() << std::endl;
    std::cout << "install: " << options.getInstallModuleName() << std::endl;
    std::cout << "has uninstall: " << options.hasUninstallModule()
              << std::endl;
    std::cout << "uninstall: " << options.getUninstallModuleName()
              << std::endl;
    std::cout << "source directory: " << options.getSourceDirectory()
              << std::endl;
    std::cout << "config filename: " << options.getConfigFileName()
              << std::endl;

    return EXIT_SUCCESS;
}
