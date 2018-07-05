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


std::shared_ptr<domain::MSEDataNode> buildMseTree(std::string &setData);

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
        //Generate MSE tree
        std::string setData;
        try {
            io::MSEReader mseReader(msePath);
            setData = mseReader.getSetData();
        }
        catch (std::exception &e){
            std::cerr << e.what() << std::endl;
        }

        boost::replace_all(setData, "\r\n", "\n");
        std::shared_ptr<domain::MSEDataNode> root = std::make_shared<domain::MSEDataNode>(setData);

        //Call lua script
        try {
            service::LuaCardData luaCardData(argTemplate, argLanguage);
            domain::MSEDataNode testNode = root->getChildrenWithKey("card").front();
            std::cout << luaCardData.id(testNode) << std::endl;
            std::cout << luaCardData.ot(testNode) << std::endl;
            std::cout << luaCardData.alias(testNode) << std::endl;
            std::cout << luaCardData.setcode(testNode) << std::endl;
            std::cout << luaCardData.type(testNode) << std::endl;
            std::cout << luaCardData.atk(testNode) << std::endl;
            std::cout << luaCardData.def(testNode) << std::endl;
            std::cout << luaCardData.level(testNode) << std::endl;
            std::cout << luaCardData.race(testNode) << std::endl;
            std::cout << luaCardData.attribute(testNode) << std::endl;
            std::cout << luaCardData.category(testNode) << std::endl;
            std::cout << luaCardData.name(testNode) << std::endl;
            std::cout << luaCardData.desc(testNode) << std::endl;
            std::cout << luaCardData.str(testNode).front() << std::endl;
        }
        catch (std::exception &e){
            std::cerr << e.what() << std::endl;
        }

        //Update database
        io::CDBAccess cdbAccess(cdbPath);
        int count =  0;
        try {
            count = cdbAccess.getCardCount();
        }
        catch (const std::runtime_error& e){
            std::cout << "Oh wow, you managed to make the test call throw an exception. I hope you're proud of yourself." << std::endl;
            std::cerr << e.what() << std::endl;
        }

        std::vector<domain::MSEDataNode> cards = root->getChildrenWithKey("card");

        try {
            service::LuaCardData luaCardData(argTemplate, argLanguage);

            for (const auto &mseCard : cards) {
                domain::CDBCard cdbCard;
                cdbCard.id = luaCardData.id(mseCard);
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
                cdbCard.name = luaCardData.name(mseCard);
                cdbCard.desc = luaCardData.desc(mseCard);
                cdbCard.str = luaCardData.str(mseCard);

                std::cout << "Current card: " << cdbCard.id << " (" << cdbCard.name << ")" << std::endl;

                cdbAccess.save(cdbCard);
            }
        }
        catch (std::exception &e){
            std::cerr << e.what() << std::endl;
        }

        try {
            domain::CDBCard card;
            card.id = 8399;
            card.ot = 0;
            card.alias = 0;
            card.setcode = 0;
            card.type = 71;
            card.atk = 1500;
            card.def = 1000;
            card.level = 4;
            card.race = 8;
            card.attribute = 32;
            card.category = 0;
            card.name = "Test2";
            card.desc = "Tests.";
            for (int i = 0; i<16;i++){
                card.str.emplace_back(std::to_string(i+1));
            }
            cdbAccess.save(card);
        }
        catch (const std::runtime_error& e){
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Oh, but I was totally called for language " << argLanguage << " and template " << argTemplate
                  << " to import " << msePath << " into " << cdbPath << ", which currently contains " << count << " cards." << std::endl;

    }
    
    return 0;
}