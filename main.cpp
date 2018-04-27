#include <iostream>
#include <getopt.h>
#include <string>
#include <sqlite3.h>

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
        sqlite3 *cdb;
        if (sqlite3_open(cdbPath.c_str(), &cdb) != SQLITE_OK){
            std::cerr << "Could not open cdb file at " << cdbPath << " (" << sqlite3_errmsg(cdb) << ")." << std::endl;
            return 1;
        }
        sqlite3_stmt *prepared;
        if (sqlite3_prepare_v2(cdb, "SELECT COUNT(*) FROM datas", -1, &prepared, 0) != SQLITE_OK){
            std::cerr << "Could not prepare statement (" << sqlite3_errmsg(cdb) << ")." << std::endl;
            return 1;
        }
        if (sqlite3_step(prepared) != SQLITE_ROW){
            std::cerr << "Could not execute prepared statement (" << sqlite3_errmsg(cdb) << ")." << std::endl;
            return 1;
        }
        int count = sqlite3_column_int(prepared, 0);

        std::cout << "Oh, but I was totally called for language " << argLanguage << " and template " << argTemplate
                  << "to import " << msePath << " into " << cdbPath << ", which currently contains " << count << " cards." << std::endl;
    }
    
    return 0;
}