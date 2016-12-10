

#include <iostream>
#include <stdlib.h>

#include "options.h"

int
main(int argc, char* argv[])
{
    dfm::DfmOptions options;
    bool status = options.loadFromArguments(argc, argv);
    std::cout << "status: " << status << std::endl;
    std::cout << "install modules: " << options.installModulesFlag
              << std::endl;
    std::cout << "uninstall modules: " << options.uninstallModulesFlag
              << std::endl;
    std::cout << "all flag: " << options.allFlag << std::endl;
    std::cout << "arguments:" << std::endl;
    std::vector<std::string>::iterator i = options.remainingArguments.begin();
    for (; i < options.remainingArguments.end(); i++)
        std::cout << "\t" << *i << std::endl;
    std::cout << "has directory: " << options.hasSourceDirectory << std::endl;
    std::cout << "directory: " << options.sourceDirectory << std::endl;

    return EXIT_SUCCESS;
}
