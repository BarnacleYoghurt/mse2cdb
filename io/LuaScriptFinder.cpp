#include "LuaScriptFinder.hpp"

namespace io{
    namespace luaScriptFinder{
        std::string getConstantsPath(){
            return "./lua/constant.lua";
        }
        std::string getUtilityPath(){
            return "./lua/utility.lua";
        }
        std::string getTemplatePath(const std::string &templateName){
            return "./lua/" + templateName + ".lua";
        }
        std::string getDictPath(const std::string &language){
            return "./lua/lang/" + language + "/dict.lua";
        }
        std::string getArchetypesPath(const std::string &language){
            return "./lua/lang/" + language + "/archetypes.lua";
        }
    }
}