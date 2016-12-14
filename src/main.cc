

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "configfilereader.h"
#include "options.h"

int
main(int argc, char* argv[])
{
    dfm::ConfigFileReader reader("testfile.txt");
    std::cout << reader.isOpen() << std::endl;

    std::vector<dfm::Module> modules;
    bool status = reader.readModules(modules.begin());
    std::cout << "status: " << status << std::endl;
    std::cout << "size: " << modules.size() << std::endl;

    return EXIT_SUCCESS;
}
