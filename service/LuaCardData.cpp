#include <stdexcept>
#include <io/LuaScriptFinder.hpp>
#include "LuaCardData.hpp"
#include "LuaMSENodeData.hpp"
#include "LuaConverters.hpp"

namespace service{
    LuaCardData::LuaCardData(const std::string &templateName, const std::string &language) {
        state = luaL_newstate();
        if (state == nullptr){
            throw std::runtime_error("Could not create initial Lua state.");
        }
        luaL_openlibs(state);
        if (luaL_dofile(state, io::luaScriptFinder::getConstantsPath().c_str()) == 1){
            throw std::runtime_error("Could not load Lua constants (" + io::luaScriptFinder::getConstantsPath() + ").");
        }
        if (luaL_dofile(state, io::luaScriptFinder::getUtilityPath().c_str()) == 1){
            throw std::runtime_error("Could not load Lua utility functions (" + io::luaScriptFinder::getUtilityPath() + ").");
        }
        if (luaL_dofile(state, io::luaScriptFinder::getTemplatePath(templateName).c_str()) == 1){
            throw std::runtime_error("Could not load Lua script for selected template (" + io::luaScriptFinder::getTemplatePath(templateName) + ")");
        }
        if (luaL_dofile(state, io::luaScriptFinder::getDictPath(language).c_str()) == 1){
            throw std::runtime_error("Could not load dictionary for selected language (" + io::luaScriptFinder::getDictPath(language) + ")");
        }
        if (luaL_dofile(state, io::luaScriptFinder::getArchetypesPath(language).c_str()) == 1){
            throw std::runtime_error("Could not load archetype list for selected language (" + io::luaScriptFinder::getArchetypesPath(language) + ")");
        }
        createNodeDataMetatable();
    }

    LuaCardData::~LuaCardData() {
        if (state != nullptr) {
            lua_close(state);
        }
    }

    unsigned int LuaCardData::id(const domain::MSEDataNode &data) {
        lua_Integer result = 0;

        if (pcallCardDataFunction<lua_Integer>("id",data,&result,&lua_tointeger) == 0) {
            return result>=0?(unsigned int)result:0;
        }
        else{
            const char *errMsg = lua_tostring(state, -1);
            if (errMsg != nullptr) {
                throw std::runtime_error("An error occurred in Lua call to get id (" + std::string(errMsg) + ")");
            }
            else{
                throw std::runtime_error("An error occurred in Lua call to get id.");
            }
        }
    }

    int LuaCardData::ot(const domain::MSEDataNode &data) {
        lua_Integer result = 0;

        if (pcallCardDataFunction<lua_Integer>("ot",data,&result,&lua_tointeger) == 0) {
            return (int)result;
        }
        else{
            const char *errMsg = lua_tostring(state, -1);
            if (errMsg != nullptr) {
                throw std::runtime_error("An error occurred in Lua call to get ot (" + std::string(errMsg) + ")");
            }
            else{
                throw std::runtime_error("An error occurred in Lua call to get ot.");
            }
        }
    }

    unsigned int LuaCardData::alias(const domain::MSEDataNode &data) {
        lua_Integer result = 0;

        if (pcallCardDataFunction<lua_Integer>("alias",data,&result,&lua_tointeger) == 0) {
            return result>=0?(unsigned int)result:0;
        }
        else{
            const char *errMsg = lua_tostring(state, -1);
            if (errMsg != nullptr) {
                throw std::runtime_error("An error occurred in Lua call to get alias (" + std::string(errMsg) + ")");
            }
            else{
                throw std::runtime_error("An error occurred in Lua call to get alias.");
            }
        }
    }

    unsigned long LuaCardData::setcode(const domain::MSEDataNode &data) {
        lua_Integer result = 0;

        if (pcallCardDataFunction<lua_Integer>("setcode",data,&result,&lua_tointeger) == 0) {
            return result>=0?(unsigned long)result:0;
        }
        else{
            const char *errMsg = lua_tostring(state, -1);
            if (errMsg != nullptr) {
                throw std::runtime_error("An error occurred in Lua call to get setcode (" + std::string(errMsg) + ")");
            }
            else{
                throw std::runtime_error("An error occurred in Lua call to get setcode.");
            }
        }
    }

    std::string LuaCardData::name(const domain::MSEDataNode &data) {
        std::string result;

        if (pcallCardDataFunction<std::string>("name",data,&result,&lua_toStdString) == 0) {
            return result;
        }
        else{
            const char *errMsg = lua_tostring(state, -1);
            if (errMsg != nullptr) {
                throw std::runtime_error("An error occurred in Lua call to get name (" + std::string(errMsg) + ")");
            }
            else{
                throw std::runtime_error("An error occurred in Lua call to get name.");
            }
        }
    }

    void LuaCardData::createNodeDataMetatable() {
        luaL_newmetatable(state, "NodeData");
        lua_pushvalue(state, -1);
        lua_setfield(state, -2, "__index");
        lua_pushcfunction(state, mseNodeData::getChildValue);
        lua_setfield(state, -2, "GetChildValue");
        lua_pushcfunction(state, mseNodeData::getChildFullContent);
        lua_setfield(state, -2, "GetChildFullContent");
    }
}