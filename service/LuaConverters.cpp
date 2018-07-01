#include "LuaConverters.hpp"

std::string lua_toStdString(lua_State *L, int i) {
    const char *value = lua_tostring(L,i);
    return value == nullptr?"":value;
}