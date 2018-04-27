#include <iostream>
#include <getopt.h>
#include <io/CDBAccess.hpp>

int main(int argc, char **argv) {
    int opt = 0;
    int indexptr = 0;
    std::string argLanguage = "en";
    std::string argTemplate = "series9";
    std::string msePath;
    std::string cdbPath;

    struct option longopts[] = {
            {"language", required_argument, 0, 'l'},
            {"template", required_argument, 0, 't'},
            {0,0,0,0}
    };

    while ((opt = getopt_long(argc,argv,"l:t:", longopts, &indexptr)) != -1){
        switch (opt){
            case 'l':
                argLanguage = optarg;
                break;
            case 't':
                argTemplate = optarg;
                break;
            default:
                return 1;
        }
    }
    if (optind < argc){
        msePath = argv[optind++];
    }
    if (optind < argc) {
        cdbPath = argv[optind++];
    }

    std::cout << "Hello, I'm a lua-configurable MSE->CDB import tool! I haven't been programmed yet, so I'm just outputting this pointless message." << std::endl;
    if (msePath.empty() || cdbPath.empty()){
        std::cout << "I need an MSE file and a CDB file!" << std::endl;
    }
    else {
        io::CDBAccess cdbAccess(cdbPath);
        int count = cdbAccess.getCardCount();

        std::cout << "Oh, but I was totally called for language " << argLanguage << " and template " << argTemplate
                  << " to import " << msePath << " into " << cdbPath << ", which currently contains " << count << " cards." << std::endl;
    }
    
    return 0;
}