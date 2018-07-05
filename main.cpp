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

int main(int argc, char **argv) {
    const char *usage = "Usage: mse2cdb [-l language] [-t template] msePath cdbPath";

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

                std::cout << "Card " << ++progress << "/" << cards.size() << ":" << std::endl;
                try {
                    cdbCard.id = luaCardData.id(mseCard);
                    cdbCard.name = luaCardData.name(mseCard);

                    std::cout << "\t" << cdbCard.id << " (" << cdbCard.name << ")" << std::endl;

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
                    std::cout << "Could not convert card." << std::endl;
                    std::cerr << e.what() << std::endl;
                }
            }
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