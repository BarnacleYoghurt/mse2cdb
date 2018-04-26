#include <iostream>
#include <getopt.h>
#include <string>

int main(int argc, char **argv) {

    int opt = 0;
    int indexptr = 0;
    std::string argLanguage;
    std::string argTemplate;

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

    std::cout << "Hello, I'm a lua-configurable MSE->CDB import tool! I haven't been programmed yet, so I'm just outputting this pointless message." << std::endl;
    if (argLanguage.empty() || argTemplate.empty()){
        std::cout << "FEED ME ARGUMENTS (--language and --template/-l and -t)!" << std::endl;
    }
    else {
        std::cout << "Oh, but I was totally called for language " << argLanguage << " and template " << argTemplate
                  << "." << std::endl;
    }
    
    return 0;
}