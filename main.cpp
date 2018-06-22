#include <iostream>
#include <getopt.h>
#include <fstream>
#include <algorithm>
#include "io/CDBAccess.hpp"
#include "domain/MSEDataNode.hpp"

#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "lua5.1/lualib.h"


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
        std::string mseLine;
        //TODO: Open the contained set file when given a .mse file
        std::ifstream mseStream(msePath);

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
            value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](int c){ return !std::isspace(c); }));
            value.erase(std::find_if(value.rbegin(), value.rend(), [](int c){ return !std::isspace(c); }).base(), value.end());

            std::shared_ptr<domain::MSEDataNode> mseNode = std::make_shared<domain::MSEDataNode>();
            mseNode->setValue(value);
            parents[childIndent]->addChild(key, mseNode);
            parents[childIndent+1] = mseNode;
        }

        //Call lua script
        lua_State *L;
        L = luaL_newstate();
        if (L != nullptr) {
            if (!luaL_dofile(L, ("../lua/" + argTemplate + ".lua").c_str())) {
                    lua_pushcfunction(L, [](lua_State *L){
                        const char **result = static_cast<const char**>(lua_touserdata(L, 1));

                        lua_getglobal(L, "cd");
                        lua_getfield(L, -1, "helloWorld");
                        lua_call(L,0,1);
                        *result = lua_tostring(L, -1);
                        return 0;
                    });
                    const char *result = nullptr;
                    lua_pushlightuserdata(L, &result);
                    if (!lua_pcall(L,1,0,0)) {
                        std::cout << result << std::endl;
                    }
                    else{
                        const char *errMsg = lua_tostring(L, -1);
                        if (errMsg != nullptr) {
                            std::cerr << errMsg << std::endl;
                        }
                        else{
                            std::cerr << "Lua Error :(" << std::endl;
                        }
                    }
            }
        }

        io::CDBAccess cdbAccess(cdbPath);
        int count =  0;
        try {
            count = cdbAccess.getCardCount();
        }
        catch (const std::runtime_error& e){
            std::cout << "Oh wow, you managed to make the test call throw an exception. I hope you're proud of yourself." << std::endl;
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Oh, but I was totally called for language " << argLanguage << " and template " << argTemplate
                  << " to import " << msePath << " into " << cdbPath << ", which currently contains " << count << " cards." << std::endl;


        std::cout << "Behold! The sacred tree which I have grown from your input!" << std::endl << std::endl;
        std::cout << root->toString() << std::endl;

    }
    
    return 0;
}