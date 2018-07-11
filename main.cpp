#include <iostream>
#include <getopt.h>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "io/CDBAccess.hpp"
#include "domain/MSEDataNode.hpp"

#include "lua5.3/lua.h"
#include "lua5.3/lauxlib.h"
#include "lua5.3/lualib.h"

#include <zip.h>
#include <sstream>
#include <io/MSEReader.hpp>
#include <service/LuaCardData.hpp>
//For cross-platform chdir
#include <unistd.h>

int main(int argc, char **argv) {
    const char *usage = "Usage: mse2cdb [-l language] [-t template] msePath cdbPath";
    if (argc > 0){
        std::string exePath(argv[0]);
        exePath = exePath.substr(0, exePath.find_last_of("\\"));
        chdir(exePath.c_str());
    }

    int opt = 0;
    int indexptr = 0;
    std::string argLanguage;
    std::string argTemplate;
    std::string msePath;
    std::string cdbPath;

    struct option longopts[] = {
            {"language", required_argument, nullptr, 'l'},
            {"template", required_argument, nullptr, 't'},
            {nullptr,0,nullptr,0}
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

    if (msePath.empty() || cdbPath.empty()){
        std::cerr << usage << std::endl;
        return EXIT_FAILURE;
    }

    //Generate MSE tree
    std::string setData;

    std::cout << "Reading data from MSE file ..." << std::endl;
    try {
        io::MSEReader mseReader(msePath);
        setData = mseReader.getSetData();
    }
    catch (std::exception &e){
        std::cout << "Failure!" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    boost::replace_all(setData, "\r\n", "\n");
    domain::MSEDataNode root(setData);
    std::cout << "Success!" << std::endl;

    //Try to fill in missing arguments
    if (argLanguage.empty() && !root.getChildrenWithKey("set info").empty()) {
        domain::MSEDataNode setInfoNode = root.getChildrenWithKey("set info").front();
        if (!setInfoNode.getChildrenWithKey("language").empty()) {
            argLanguage = setInfoNode.getChildrenWithKey("language").front().getValue();
            std::transform(argLanguage.begin(), argLanguage.end(), argLanguage.begin(), ::tolower);
            std::cout << "Language guessed based on set file: " << argLanguage << std::endl;
        }
    }
    if (argTemplate.empty() && !root.getChildrenWithKey("stylesheet").empty()){
        argTemplate = root.getChildrenWithKey("stylesheet").front().getValue();
        std::cout << "Template guessed based on set file: " << argTemplate << std::endl;
    }

    //Update database
    std::cout << "Opening connection to CDB file ..." << std::endl;
    try {
        io::CDBAccess cdbAccess(cdbPath);
        std::cout << "Success!" << std::endl;

        std::vector<domain::MSEDataNode> cards = root.getChildrenWithKey("card");
        try {
            std::cout << "Loading lua scripts ..." << std::endl;
            service::LuaCardData luaCardData(argTemplate, argLanguage);
            std::cout << "Success!" << std::endl;

            std::cout << "Converting " << cards.size() << " cards ..." << std::endl;
            int progress = 0;
            for (const auto &mseCard : cards) {
                domain::CDBCard cdbCard;

                std::cout << "\tCard " << ++progress << "/" << cards.size() << ":" << std::endl;
                try {
                    cdbCard.id = luaCardData.id(mseCard);
                    cdbCard.name = luaCardData.name(mseCard);

                    std::cout << "\t\t" << cdbCard.id << " (" << cdbCard.name << ")" << std::endl;

                    cdbCard.ot = luaCardData.ot(mseCard);
                    cdbCard.alias = luaCardData.alias(mseCard);
                    cdbCard.setcode = luaCardData.setcode(mseCard);
                    cdbCard.type = luaCardData.type(mseCard);
                    cdbCard.atk = luaCardData.atk(mseCard);
                    cdbCard.def = luaCardData.def(mseCard);
                    cdbCard.level = luaCardData.level(mseCard);
                    cdbCard.race = luaCardData.race(mseCard);
                    cdbCard.attribute = luaCardData.attribute(mseCard);
                    cdbCard.category = luaCardData.category(mseCard);
                    cdbCard.desc = luaCardData.desc(mseCard);
                    cdbCard.str = luaCardData.str(mseCard);
                    cdbAccess.save(cdbCard);
                }
                catch (const std::exception &e){
                    std::cout << "\t\tCould not convert card." << std::endl;
                    std::cerr << e.what() << std::endl;
                }
            }
            std::cout << "Success!" << std::endl;
        }
        catch (const std::exception &e){
            std::cout << "Failure!" << std::endl;
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception &e){
        std::cout << "Failure!" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}