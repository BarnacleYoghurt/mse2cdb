#include <stdexcept>
#include "LuaCardData.hpp"
#include "LuaMSENodeData.hpp"

namespace service{
    LuaCardData::LuaCardData(const std::string &templatePath) {
        state = luaL_newstate();
        if (state == nullptr){
            throw std::runtime_error("Could not create initial Lua state.");
        }
        if (luaL_dofile(state, templatePath.c_str()) == 1){
            throw std::runtime_error("Could not load Lua script for selected template (" + templatePath + ")");
        }
    }

    LuaCardData::~LuaCardData() {
        if (state != nullptr) {
            lua_close(state);
        }
    }

    unsigned int LuaCardData::id(const domain::MSEDataNode &data) {
        lua_pushcfunction(state, [](lua_State *state){
            auto *result = static_cast<int*>(lua_touserdata(state, 1));
            auto *dataParam = static_cast<domain::MSEDataNode*>(lua_touserdata(state, 2));

            lua_getglobal(state, "cd");
            lua_getfield(state, -1, "id");
            lua_pushlightuserdata(state, dataParam);

            luaL_newmetatable(state, "NodeData");
            lua_pushvalue(state, -1);
            lua_setfield(state, -2, "__index");
            lua_pushcfunction(state, mseNodeData::getChildValue);
            lua_setfield(state, -2, "GetChildValue");
            lua_setmetatable(state, 5);

            lua_call(state,1,1);
            *result = (int)lua_tointeger(state, -1);
            return 0;
        });
        int result = 0;
        lua_pushlightuserdata(state, &result);
        lua_pushlightuserdata(state, (domain::MSEDataNode*)&data);
        if (lua_pcall(state,2,0,0) == 0) {
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
}