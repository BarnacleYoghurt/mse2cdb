#include <stdexcept>
#include "LuaCardData.hpp"
#include "LuaMSENodeData.hpp"
#include "LuaConverters.hpp"

namespace service{
    LuaCardData::LuaCardData(const std::string &templatePath) {
        state = luaL_newstate();
        if (state == nullptr){
            throw std::runtime_error("Could not create initial Lua state.");
        }
        luaL_openlibs(state);
        if (luaL_dofile(state, templatePath.c_str()) == 1){
            throw std::runtime_error("Could not load Lua script for selected template (" + templatePath + ")");
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
    }
}