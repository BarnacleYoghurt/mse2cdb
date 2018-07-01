#include "LuaConverters.hpp"

std::string lua_toStdString(lua_State *L, int i) {
    return lua_tostring(L,i);
}