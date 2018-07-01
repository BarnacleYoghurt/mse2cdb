#include <iostream>
#include <getopt.h>
#include <fstream>
#include <algorithm>
#include "io/CDBAccess.hpp"
#include "domain/MSEDataNode.hpp"

#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"

#include <zip.h>
#include <sstream>
#include <io/MSEReader.hpp>
#include <service/LuaCardData.hpp>


std::shared_ptr<domain::MSEDataNode> buildMseTree(const std::string &setData);

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
        std::shared_ptr<domain::MSEDataNode> root = buildMseTree(setData);

        //Call lua script
        try {
            service::LuaCardData luaCardData(argTemplate, argLanguage);
            std::cout << luaCardData.id(root->getChildNode("card")) << std::endl;
            std::cout << luaCardData.ot(root->getChildNode("card")) << std::endl;
            std::cout << luaCardData.alias(root->getChildNode("card")) << std::endl;
            std::cout << luaCardData.setcode(root->getChildNode("card")) << std::endl;
            std::cout << luaCardData.name(root->getChildNode("card")) << std::endl;
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
                card.str[i] = std::to_string(i+1);
            }
            cdbAccess.save(card);
        }
        catch (const std::runtime_error& e){
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Oh, but I was totally called for language " << argLanguage << " and template " << argTemplate
                  << " to import " << msePath << " into " << cdbPath << ", which currently contains " << count << " cards." << std::endl;


        std::cout << "Behold! The sacred tree which I have grown from your input!" << std::endl << std::endl;
        std::cout << root->toString() << std::endl;

    }
    
    return 0;
}

std::shared_ptr<domain::MSEDataNode> buildMseTree(const std::string &setData) {
    std::stringstream mseStream(setData);

    std::string mseLine;
    std::shared_ptr<domain::MSEDataNode> root = std::make_shared<domain::MSEDataNode>();
    std::shared_ptr<domain::MSEDataNode> currentParent;
    std::map<int, std::shared_ptr<domain::MSEDataNode>> parents;
    parents[0] = root;
    int childIndent = 0;

    while (getline(mseStream, mseLine)){
            int currentIndent = 0;
            for (char c : mseLine){
                if (c == '\t'){
                    currentIndent++;
                }
                else{
                    break;
                }
            }
            if (currentIndent > childIndent){
                //A sudden indent by multiple tabs only counts as one hierarchy layer
                childIndent++;
            }
            else if (currentIndent < childIndent){
                childIndent = currentIndent;
            }

            std::string key;
            std::string value;
            size_t sepPos = mseLine.find(':');
            if (sepPos != std::string::npos){
                key = mseLine.substr(0,sepPos);
                value = mseLine.substr(sepPos + 1);
            }
            else{
                value = mseLine;
            }
            value.erase(value.begin(), find_if(value.begin(), value.end(), [](int c){ return !isspace(c); }));
            value.erase(find_if(value.rbegin(), value.rend(), [](int c){ return !isspace(c); }).base(), value.end());
            key.erase(key.begin(), find_if(key.begin(), key.end(), [](int c){ return !isspace(c); }));
            key.erase(find_if(key.rbegin(), key.rend(), [](int c){ return !isspace(c); }).base(), key.end());

            std::shared_ptr<domain::MSEDataNode> mseNode = std::make_shared<domain::MSEDataNode>();
            mseNode->setValue(value);
            parents[childIndent]->addChild(key, mseNode);
            parents[childIndent+1] = mseNode;
        }
    return root;
}